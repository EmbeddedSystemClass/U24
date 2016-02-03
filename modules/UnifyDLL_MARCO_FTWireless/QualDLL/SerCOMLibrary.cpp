#include "stdafx.h"
#include "SerCOMLibrary.h"
//#include "Logger.h"
#include "SerialIO.h"
#include "BQPacketLib.h"

CSerCOMLibrary::CSerCOMLibrary()
{
	m_hNotifyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_pbBuffer = new BYTE[READ_BUFFER_SIZE];
	ZeroMemory(m_pbBuffer, READ_BUFFER_SIZE);
	m_nReadCount = 0;
}

CSerCOMLibrary::~CSerCOMLibrary()
{
	CloseHandle(m_hNotifyEvent);
	CSerialIO::Close();
	delete[] m_pbBuffer;
	m_pbBuffer = NULL;
}

int CSerCOMLibrary::Open(LPCTSTR lpszComport, bool isEmdl)
{
	LPCTSTR szFunc = _T("CSerCOMLibrary::OpenComport");
	////////////////////////////////////////////////////////////////////////////////
	if (lpszComport == NULL) {
		//LogFunctionInformation(LOG_LEVEL_DEBUG, szFunc, _T("Function parameters contain null."));
		return ERROR_CSERCOMLIBRARY_INVALID_PARAMETER;
	}
	CString strCOMNum;
	strCOMNum = lpszComport;
	int nIndex = strCOMNum.ReverseFind(_T('M'));
	strCOMNum = strCOMNum.Mid(nIndex + 1, strCOMNum.GetLength() - nIndex - 1);
	m_CRCChecker.Erase();
	if (CSerialIO::Open(lpszComport, isEmdl, EBaud115200) < 0)
		return CSerCOMLibrary::ERROR_CSERCOMLIBRARY_COM_OPEN_FAIL;
	return RUN_SUCCESSFULLY;
}

int CSerCOMLibrary::Close()
{
	LPCTSTR	szFunc = _T("CSerCOMLibrary::CloseCOMPort");
	//LogCOMFuncInfo(LOG_LEVEL_WARNING, GetComport(), szFunc,  _T("Try to close COM %s"), m_szDevice);
	////////////////////////////////////////////////////////////////////////////////
	return CSerialIO::Close();
}

void CSerCOMLibrary::OnEvent()
{
	LPCTSTR szFunc = _T("CSerCOMLibrary::OnEvent");
	////////////////////////////////////////////////////////////////////////////////
	CHAR chStart = PACKET_NULL_CHAR;
	CHAR chEnd   = PACKET_NULL_CHAR;
	int nRet = 0;
	DWORD dwRead = 0;
	ZeroMemory(m_pbBuffer, READ_BUFFER_SIZE);
	EState eState = GetState();

	//LogCOMFuncInfo(LOG_LEVEL_DEBUG, GetComport(), szFunc, _T("Get received data"));
	//while(1) {
	if (Read(m_pbBuffer, DIAG_MAX_RESPONSE_LEN, &dwRead) != ERROR_SUCCESS || dwRead == 0) {
		//LogCOMFuncInfo(LOG_LEVEL_DEBUG, GetComport(), szFunc, _T("Read data from read buffer failed."));
		TRACE(_T("%s data:0x%02x\n"), _T(__FUNCTION__), m_pbBuffer);
		//return;
	}
	//}

	//LogCOMFuncInfo(LOG_LEVEL_DEBUG, GetComport(), szFunc, _T("The original received buffer is"));
	//LogCOMFuncBuffer(LOG_LEVEL_DEBUG, GetComport(), szFunc, m_pbBuffer, dwRead);

	m_CRCChecker.SetState(eState);
	// Set up start character and end character
	if (eState == EDIAG || eState == EDL) {
		for (DWORD j = 0 ; j < dwRead; j++) {
			m_nRxLen = m_CRCChecker.Push(m_pbBuffer[j]);
			if (m_nRxLen > 0) { // buffer unpack successfully.
				//LogCOMFuncInfo(LOG_LEVEL_DEBUG, GetComport(), szFunc, _T("The mem buffer is"));
				//LogCOMFuncBuffer(LOG_LEVEL_DEBUG, GetComport(), szFunc, m_CRCChecker.GetBuffer(), m_nRxLen);
				// Set wait event
				::InterlockedIncrement((LPLONG)& m_nReadCount);
				SetEvent(m_hNotifyEvent);
				WaitForSingleObject(m_hReadEvent, INFINITE);
				m_nRxLen = 0;
				m_CRCChecker.Erase();
				//LogCOMFuncInfo(LOG_LEVEL_DEBUG, GetComport(), szFunc, _T("Complete one return message and forward to upper layer."));
			}
			if (m_nRxLen < 0) {
				//LogCOMFuncInfo(LOG_LEVEL_ERROR, GetComport(), szFunc, _T("Packet check fails with error code %d"), m_nRxLen);
				// Set wait event
				m_nRxLen = 0;
				m_CRCChecker.Erase();
				return;
			}
		}
	}
	else {
		_ASSERTE(FALSE);
	}
	//LogCOMFuncInfo(LOG_LEVEL_DEBUG, GetComport(), szFunc, _T("Complete OnRecv."));
}

int CSerCOMLibrary::TXRXDiag(CHAR* lpTxData, DWORD dwTxLen, CHAR* lpRxData, DWORD dwRxLen, UINT uiTimeout)
{
	LPCTSTR szFunc = _T("CSerCOMLibrary::TXRXDiag");
	////////////////////////////////////////////////////////////////////////////////
	if (GetState() != CSerialIO::EDIAG)
		return CSerCOMLibrary::ERROR_CSERCOMLIBRARY_NULL_INTERFACE;
	return TxRx(lpTxData, dwTxLen, lpRxData, dwRxLen, uiTimeout);
}

int CSerCOMLibrary::TXRXDl(CHAR *lpTxData, DWORD dwTxLen, CHAR *lpRxData, DWORD dwRxLen, UINT uiTimeout)
{
	LPCTSTR szFunc = _T("CSerCOMLibrary::TXRXDl");
	////////////////////////////////////////////////////////////////////////////////
	if (GetState() != CSerialIO::EDL)
		return CSerCOMLibrary::ERROR_CSERCOMLIBRARY_NULL_INTERFACE;
	return TxRx(lpTxData, dwTxLen, lpRxData, dwRxLen, uiTimeout);
}

int CSerCOMLibrary::TxRx(CHAR *lpTxData, DWORD dwTxLen, CHAR *lpRxData, DWORD dwRxLen, UINT uiTimeout)
{
	LPCTSTR szFunc = _T("CSerCOMLibrary::TxRx");
	////////////////////////////////////////////////////////////////////////////////
	DWORD dwWritten = 0;
	DWORD volatile dwBufPos = m_dwBufEnd - m_dwBufStart;
	CHAR chStart, chEnd;
	LONG lRet = 0;
	void *pBuffer = NULL;
	int nPackedLen = 0;
	chStart = PACKET_NULL_CHAR;
	chEnd = PACKET_NULL_CHAR;
	BOOL bIsReadCompleted = FALSE;
	int nRet = 0;
	DWORD dwRet = 0;

	_ASSERTE(lpRxData != NULL);
	_ASSERTE(lpRxData != NULL);
	_ASSERTE(dwTxLen > 0);
	_ASSERTE(dwRxLen > 0);

	m_CRCChecker.SetState(m_eState);
	// Boyan Zhou add 2007/4/10
	// for cmd
	BYTE bycmd = lpTxData[0];
	m_CRCChecker.SetCom(bycmd);
	// End
	if (m_eState == EDL) {
		chStart = START_CHAR;
	}
	chEnd = END_CHAR;

	//LogCOMFuncInfo(LOG_LEVEL_DEBUG, GetComport(), szFunc, _T("Unpacked buffer source before TXRX"));
	//LogCOMFuncBuffer(LOG_LEVEL_DEBUG, GetComport(), szFunc, lpTxData, dwTxLen);
	nPackedLen = CBQPacketLib::PackBuffer(lpTxData, dwTxLen, chStart, chEnd, &pBuffer);
	//LogCOMFuncInfo(LOG_LEVEL_DEBUG, GetComport(), szFunc, _T("Packed buffer source before TXRX"));
	//LogCOMFuncBuffer(LOG_LEVEL_DEBUG, GetComport(), szFunc, pBuffer, nPackedLen);

	_ASSERTE(nPackedLen > 0);
	_ASSERTE(pBuffer != NULL);

	lRet = CSerialIO::Write(pBuffer, nPackedLen, &dwWritten);
	delete[] pBuffer;
	pBuffer = NULL;

	// Write to COM failed.
	if (lRet != 0)
		return ERROR_CSERCOMLIBRARY_TX_FAIL;

	dwRet = WaitForSingleObject(m_hNotifyEvent, uiTimeout);
	if (dwRet == WAIT_TIMEOUT) {
		//LogCOMFuncInfo(LOG_LEVEL_ERROR, GetComport(), szFunc, _T("Time out in OnRecv."));
		return ERROR_TIMEOUT;
	}

	if (dwRxLen < static_cast<DWORD>(m_nRxLen)) {
		//LogCOMFuncInfo(LOG_LEVEL_ERROR, GetComport(), szFunc, _T("Buffer overflow."));
		return ERROR_CSERCOMLIBRARY_RX_FAIL;
	}
	ZeroMemory(lpRxData, dwRxLen);
	CopyMemory(lpRxData, m_CRCChecker.GetBuffer(), m_nRxLen);
	SetEvent(m_hReadEvent);
	::InterlockedDecrement((LPLONG)&m_nReadCount);

	return NO_ERROR;
}

int CSerCOMLibrary::TxRxEmdl(CHAR *lpTxData, DWORD dwTxLen, CHAR *lpRxData, DWORD dwRxLen, UINT uiTimeout)
{
	DWORD dwWritten = 0;
	DWORD volatile dwBufPos = m_dwBufEnd - m_dwBufStart;
	CHAR chStart, chEnd;
	LONG lRet = 0;
	void *pBuffer = NULL;
	int nPackedLen = 0;
	chStart = PACKET_NULL_CHAR;
	chEnd = PACKET_NULL_CHAR;
	BOOL bIsReadCompleted = FALSE;
	int nRet = 0;
	DWORD dwRet = 0;

	m_CRCChecker.SetState(m_eState);
	// Boyan Zhou add 2007/4/10
	// for cmd
	BYTE bycmd = lpTxData[0];
	m_CRCChecker.SetCom(bycmd);
	// End
	if (m_eState == EDL) {
		//chStart = START_CHAR;
	}
	chEnd = END_CHAR;
	
	//LogCOMFuncInfo(LOG_LEVEL_DEBUG, GetComport(), _T(__FUNCTION__), _T("Unpacked buffer source before TXRX"));
	//LogCOMFuncBuffer(LOG_LEVEL_DEBUG, GetComport(), _T(__FUNCTION__), lpTxData, dwTxLen);
	nPackedLen = CBQPacketLib::PackBuffer(lpTxData, dwTxLen, chStart, chEnd, &pBuffer);
	//LogCOMFuncInfo(LOG_LEVEL_DEBUG, GetComport(), _T(__FUNCTION__), _T("Packed buffer source before TXRX"));
	//LogCOMFuncBuffer(LOG_LEVEL_DEBUG, GetComport(), _T(__FUNCTION__), pBuffer, nPackedLen);

	lRet = CSerialIO::Write(pBuffer, nPackedLen, &dwWritten);
	delete[] pBuffer;
	pBuffer = NULL;
	/////////////////////////////////////////////////////////////

	// Write to COM failed.
	if (lRet != 0)
		return ERROR_CSERCOMLIBRARY_TX_FAIL;

	dwRet = WaitForSingleObject(m_hNotifyEvent, uiTimeout);
	if (dwRet == WAIT_TIMEOUT)
		return ERROR_TIMEOUT;
	if (dwRxLen < static_cast<DWORD>(m_nRxLen))
		return ERROR_CSERCOMLIBRARY_RX_FAIL;

	ZeroMemory(lpRxData, dwRxLen);
	CopyMemory(lpRxData, m_CRCChecker.GetBuffer(), m_nRxLen);
	SetEvent(m_hReadEvent);
	::InterlockedDecrement((LPLONG)&m_nReadCount);

	return NO_ERROR;
}

int CSerCOMLibrary::TxRxSpeedUp(CHAR *lpTxData, DWORD dwTxLen, CHAR *lpRxData, DWORD dwRxLen, char *szIMAGEPATH, UINT uiTimeout)
{
	DWORD dwWritten = 0;
	DWORD volatile dwBufPos = m_dwBufEnd - m_dwBufStart;
	CHAR chStart, chEnd;
	void *pBuffer = NULL;
	int nPackedLen = 0;
	chStart = PACKET_NULL_CHAR;
	chEnd = PACKET_NULL_CHAR;
	BOOL bIsReadCompleted = FALSE;
	DWORD dwRet = 0;

	m_CRCChecker.SetState(m_eState);
	// Boyan Zhou add 2007/4/10
	// for cmd
	BYTE bycmd = lpTxData[0];
	m_CRCChecker.SetCom(bycmd);
	// End
	if (m_eState == EDL) {
		chStart = START_CHAR;
	}
	chEnd = END_CHAR;
	nPackedLen = CBQPacketLib::PackBuffer(lpTxData, dwTxLen, chStart, chEnd, &pBuffer);

	int nRetCode = ERROR_FUNCTION_FAILED;
	nRetCode = CSerialIO::Write(pBuffer, nPackedLen, &dwWritten);
	delete[] pBuffer;
	pBuffer = NULL;

	/////////////////////////////////////////////////////////////
	CFile fImage;
	if (fImage.Open(szIMAGEPATH, CFile::modeRead | CFile::shareDenyNone) == 0) {
		return ERROR_FILE_INVALID;
	}
	size_t nFileSize = (size_t)fImage.GetLength();
	pBuffer = new byte[nFileSize];
	memset(pBuffer, 0, nFileSize*sizeof(byte));
	fImage.Read(pBuffer, nFileSize);
	fImage.Close();

	const int PKT_SPEEDUP_PAGE_SIZE = 1024;
	DWORD wAddress = 0;
	DWORD nBytesWrite = 0;
	do {
		TRACE(".");
		if (nFileSize - nBytesWrite >= PKT_SPEEDUP_PAGE_SIZE) { // if last block < PKT_SPEEDUP_PAGE_SIZE
			nRetCode = CSerialIO::Write(&((byte*)pBuffer)[nBytesWrite], PKT_SPEEDUP_PAGE_SIZE, &dwWritten);
		}
		else { // if nSize % PKT_SPEEDUP_PAGE_SIZE != 0, send last block size
			nRetCode = CSerialIO::Write(&((byte*)pBuffer)[nBytesWrite], nFileSize % PKT_SPEEDUP_PAGE_SIZE, &dwWritten);
		}
		nBytesWrite += PKT_SPEEDUP_PAGE_SIZE;
		wAddress += PKT_SPEEDUP_PAGE_SIZE;
		Sleep(0);
	} while(nBytesWrite < nFileSize);
	delete[] pBuffer;
	TRACE("\n");
	/////////////////////////////////////////////////////////////

	// Write to COM failed.
	if (nRetCode != 0)
		return ERROR_CSERCOMLIBRARY_TX_FAIL;

	dwRet = WaitForSingleObject(m_hNotifyEvent, uiTimeout);
	if (dwRet == WAIT_TIMEOUT)
		return ERROR_TIMEOUT;
	if (dwRxLen < static_cast<DWORD>(m_nRxLen))
		return ERROR_CSERCOMLIBRARY_RX_FAIL;

	ZeroMemory(lpRxData, dwRxLen);
	CopyMemory(lpRxData, m_CRCChecker.GetBuffer(), m_nRxLen);
	SetEvent(m_hReadEvent);
	::InterlockedDecrement((LPLONG)&m_nReadCount);

	return NO_ERROR;
}
