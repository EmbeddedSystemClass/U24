#include "stdafx.h"
#include <crtdbg.h>
#include <tchar.h>
#include "SerialIO.h"
//#include "Logger.h"

#pragma warning(disable: 4996)

#define RUN_SUCCESSFULLY	1
#define RUN_FAILED			(-1)
#define ERROR_MASK			0xF0000000

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSerialIO::CSerialIO()
: m_lLastError(ERROR_SUCCESS),
		m_hFile(NULL),
		m_fStopping(0),
		m_hThread(NULL),
		m_hReadEvent(NULL),
		m_dwBufStart(0),
		m_dwBufEnd(0),
		m_dwCOMNum(0)
{
	LPCTSTR szFunc = _T("CSerialIO::CSerialIO");
	//LogFunctionInformation(LOG_LEVEL_DEBUG, szFunc, _T("Start constructor."));
	////////////////////////////////////////////////////////////////////////////////
	m_dwBufSize = RECV_BUFFER_SIZE;
	m_pbRecvBuf = new BYTE [m_dwBufSize];
	ZeroMemory(m_pbRecvBuf, m_dwBufSize);
	ZeroMemory(m_szDevice, sizeof(TCHAR) * COM_PORT_NAME_LENGTH);
	ZeroMemory(m_szCOM, sizeof(TCHAR) * COM_PORT_NAME_LENGTH);
	m_dwCurWriteCount = 0;
}

CSerialIO::~CSerialIO()
{
	LPCTSTR szFunc = _T("CSerialIO::~CSerialIO");
	//LogFunctionInformation(LOG_LEVEL_DEBUG, szFunc, _T("Start destructor"));
	////////////////////////////////////////////////////////////////////////////////
	//LogFunctionInformation(LOG_LEVEL_DEBUG, szFunc, _T("Start stopping listener."));
	StopListener(INFINITE);

	//LogFunctionInformation(LOG_LEVEL_DEBUG, szFunc, _T("Start internal close."));
	InternalClose();
	_ASSERTE(m_pbRecvBuf != NULL);
	//LogFunctionInformation(LOG_LEVEL_DEBUG, szFunc, _T("Start free read buffer."));
	delete[] m_pbRecvBuf;
	m_pbRecvBuf = NULL;
	//LogFunctionInformation(LOG_LEVEL_DEBUG, szFunc, _T("complete destructor."));
}

LONG CSerialIO::Open(LPCTSTR lpszDevice, bool isEmdl, EBaudrate eBaudrate)
{
	LPCTSTR szFunc = _T("CSerialIO::Open");
	//LogCOMFuncInfo(LOG_LEVEL_DEBUG, lpszDevice, szFunc, _T("Start open operation."));
	////////////////////////////////////////////////////////////////////////////////
	m_isEmdl = isEmdl;
	if (m_hFile) {
		m_lLastError = ERROR_ALREADY_INITIALIZED;
	}
	else {
		m_eBaudrate = eBaudrate;
		m_dwCOMNum = GetCOMNum(lpszDevice);
		_stprintf(m_szCOM, _T("COM%d"), m_dwCOMNum);
		if (m_dwCOMNum > 9)
			_stprintf(m_szDevice, _T("\\\\.\\COM%d"), m_dwCOMNum);
		else
			_stprintf(m_szDevice, _T("COM%d"), m_dwCOMNum);
		StartListener();
	}
	return m_lLastError;
}

LONG CSerialIO::Close(void)
{
	LPCTSTR szFunc = _T("CSerialIO::Close");
	//LogCOMFuncInfo(LOG_LEVEL_WARNING, m_szCOM, szFunc, _T("Try to close com port."));
	////////////////////////////////////////////////////////////////////////////////
	m_lLastError = ERROR_SUCCESS;
	StopListener();
	if (m_hFile == 0) {
		return m_lLastError;
	}
	else {
		if ( InternalClose() != ERROR_SUCCESS ) {
			//LogCOMFuncInfo(LOG_LEVEL_WARNING, m_szCOM, szFunc, _T("internal close failed."));
		}
		else {
			//LogCOMFuncInfo(LOG_LEVEL_WARNING, m_szCOM, szFunc, _T("Close com port successfully."));
			return RUN_SUCCESSFULLY;
		}
	}
	//LogCOMFuncInfo(LOG_LEVEL_WARNING, m_szCOM, szFunc, _T("Close COM port failed."));
	return m_lLastError;
}

LONG CSerialIO::Write(const void* pData, size_t iLen, DWORD* pdwWritten)
{
	LPCTSTR szFunc = _T("CSerialIO::Write");
	//LogCOMFuncInfo(LOG_LEVEL_DEBUG, m_szCOM, szFunc, _T("Start write operation."));
	////////////////////////////////////////////////////////////////////////////////
	::InterlockedIncrement(&m_dwCurWriteCount);
	LONG lRet = InternalWrite(pData, iLen, pdwWritten);
	::InterlockedDecrement(&m_dwCurWriteCount);
	return lRet;
}

LONG CSerialIO::Read(void* pData, size_t iLen, DWORD* pdwRead)
{
	LPCTSTR szFunc = _T("CSerialIO::Read");
	//LogCOMFuncInfo(LOG_LEVEL_DEBUG, m_szCOM, szFunc, _T("Start read operation."));
	////////////////////////////////////////////////////////////////////////////////
	m_lLastError = ERROR_SUCCESS;

	DWORD dwRead;
	if (pdwRead == 0) {
		pdwRead = &dwRead;
	}
	*pdwRead = 0;

	if (m_hFile == 0) {
		m_lLastError = ERROR_INVALID_HANDLE;
		return m_lLastError;
	}
	m_csRead.Lock();
	*pdwRead = ReadBuffer(pData, iLen);
	m_csRead.Unlock();
	return m_lLastError;
}

int	CSerialIO::GetCOMNum(LPCTSTR szDevice)
{
	TCHAR szCOMNum[COM_PORT_NAME_LENGTH] = {0};
	TCHAR szDeviceName[COM_PORT_NAME_LENGTH] = {0};
	TCHAR *szFind = NULL;
	int nCurrentPortNum = 0;

	_tcscpy(szDeviceName, szDevice);
	_tcsupr(szDeviceName);
	szFind = _tcschr(szDeviceName, 'M');
	_ASSERTE(szFind != NULL);
	_tcscpy(szCOMNum, szFind + 1);
	nCurrentPortNum = _ttoi(szCOMNum);
	return nCurrentPortNum;
}

LONG CSerialIO::InternalOpen()
{
	LPCTSTR szFunc = _T("CSerialIO::InternalOpen");
	//LogCOMFuncInfo(LOG_LEVEL_WARNING, m_szCOM, szFunc, _T("Check COM port existence in the Registry."));
	////////////////////////////////////////////////////////////////////////////////
	int nRet = FALSE;
	if ( m_hFile != NULL ) {
		//LogCOMFuncInfo(LOG_LEVEL_WARNING, m_szCOM, szFunc, _T("COM port has been opened."));
		return TRUE;
	}

	m_csWrite.Lock();
	m_hFile = ::CreateFile(m_szDevice, GENERIC_READ | GENERIC_WRITE | MAXIMUM_ALLOWED,
	                       0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
	if ( m_hFile == INVALID_HANDLE_VALUE ) {
		m_hFile = NULL;
		//LogCOMFuncInfo(LOG_LEVEL_ERROR, m_szCOM, szFunc, _T("Fails in opening COM port."));
	}
	else {
		nRet = InternalResetCOM();
		if ( nRet != TRUE ) {
			::CloseHandle(m_hFile);
			m_hFile = NULL;
			//LogCOMFuncInfo(LOG_LEVEL_ERROR, m_szCOM, szFunc, _T("Fails in InternalResetCOM()."));
		}
		else {
			m_hReadEvent = ::CreateEvent(0, FALSE, FALSE, 0);
			if ( m_hReadEvent == NULL ) {
				nRet = FALSE;
				::CloseHandle(m_hFile);
				m_hFile = NULL;
				//LogCOMFuncInfo(LOG_LEVEL_WARNING, m_szCOM, szFunc, _T("Create read event failed."));
			}
		}
	}
	m_csWrite.Unlock();
	return nRet;
}

LONG CSerialIO::InternalClose()
{
	LPCTSTR szFunc = _T("CSerialIO::InternalClose");
	//LogCOMFuncInfo(LOG_LEVEL_WARNING, m_szCOM, szFunc, _T("InternalClose start."));
	////////////////////////////////////////////////////////////////////////////////
	m_lLastError = ERROR_SUCCESS;
	m_csWrite.Lock();
	if (m_hFile == 0) {
		m_csWrite.Unlock();
		//LogCOMFuncInfo(LOG_LEVEL_WARNING, m_szCOM, szFunc, _T("COM port handle is null."));
		return m_lLastError;
	}

	//LogCOMFuncInfo(LOG_LEVEL_WARNING, m_szCOM, szFunc, _T("Try to close COM port."));
	if ( !::CloseHandle(m_hFile) ) {
		//LogCOMFuncInfo(LOG_LEVEL_WARNING, m_szCOM, szFunc, _T("Close COM port failed."));
		m_lLastError = GetLastError();
		//LogCOMFuncSystemError(m_szCOM, szFunc, m_lLastError);
	}

	m_hFile = 0;
	if (m_hReadEvent) {
		if ( !::CloseHandle(m_hReadEvent) ) {
			//LogCOMFuncInfo(LOG_LEVEL_WARNING, m_szCOM, szFunc, _T("Close write event failed."));
			m_lLastError = GetLastError();
			//LogCOMFuncSystemError(m_szCOM, szFunc, m_lLastError);
			return m_lLastError;
		}
		m_hReadEvent = 0;
	}

	m_csWrite.Unlock();
	//LogCOMFuncInfo(LOG_LEVEL_WARNING, m_szCOM, szFunc, _T("Close COM port successfully."));
	return m_lLastError;
}

long CSerialIO::InternalWrite(const void* pData, size_t iLen, DWORD* pdwWritten /* = 0 */)
{
	LPCTSTR szFunc = _T("CSerialIO::InternalWrite");
	////////////////////////////////////////////////////////////////////////////////
	m_lLastError = ERROR_SUCCESS;
	long lLastError = ERROR_SUCCESS;
	// Use our own variable for read count
	DWORD dwWritten;
	if (pdwWritten == 0) {
		pdwWritten = &dwWritten;
	}
	// Reset the number of bytes written
	*pdwWritten = 0;
	// Check if the device is open
	if (m_hFile == 0) {
		m_lLastError = ERROR_INVALID_HANDLE;
		//LogCOMFuncInfo(LOG_LEVEL_ERROR, m_szCOM, szFunc, _T("COM port handle is null"));
		return m_lLastError;
	}

	OVERLAPPED ovInternal = {0};
	HANDLE hWriteEvent = ::CreateEvent(0, FALSE, FALSE, 0);
	ovInternal.hEvent = hWriteEvent;

	m_csWrite.Lock();
	// Data buffer to write
	//LogCOMFuncInfo(LOG_LEVEL_DEBUG, m_szCOM, szFunc, _T("Write data"));
	//LogCOMFuncBuffer(LOG_LEVEL_DEBUG, m_szCOM, szFunc, (void *)pData, iLen);
	// Write the data
	if (!::WriteFile(m_hFile, pData, iLen, pdwWritten, &ovInternal)) {
		lLastError = ::GetLastError();
		if (lLastError != ERROR_IO_PENDING) {
			m_lLastError = lLastError;
		}
		else {
			switch (::WaitForSingleObject(ovInternal.hEvent, INFINITE)) {
			case WAIT_OBJECT_0:
				// The overlapped operation has completed
				if (!::GetOverlappedResult(m_hFile, &ovInternal, pdwWritten, FALSE)) {
					m_lLastError = ::GetLastError();
					lLastError = m_lLastError;
				}
				break;

			case WAIT_TIMEOUT:
				::CancelIo(m_hFile);
				m_lLastError = ERROR_TIMEOUT;
				lLastError = m_lLastError;
				break;

			default:
				m_lLastError = ::GetLastError();
				lLastError = m_lLastError;
				break;
			}
		}
	}
	m_csWrite.Unlock();
	CloseHandle(ovInternal.hEvent);

	//LogCOMFuncInfo(LOG_LEVEL_DEBUG, m_szCOM, szFunc, _T("InternalWrite() return (%d)"), lLastError);
	return m_lLastError;
}

////////////////////////////////////////////////////////////////////////////
/*************************************************************************
 * Purpose: Rest all serial COM port setting and purge it. The parameters comes from QPST log.
 * Input  : NONE
 * Return : ERROR_SUCCESS -->run successfully.
 *			OTHERS  -->error.
 * Exception: NONE
 * Note	  : 2007/11/13 first version created by Embert
 ***************************************************************************/
////////////////////////////////////////////////////////////////////////////
int CSerialIO::InternalResetCOM()
{
	LPCTSTR szFunc = _T("CSerialIO::InternalResetCOM");
	////////////////////////////////////////////////////////////////////////////////
	if (!::SetCommMask(m_hFile, EV_CTS | EV_DSR | EV_RLSD | EV_BREAK)) {
		m_lLastError = ::GetLastError();
		//LogCOMFuncInfo(LOG_LEVEL_DEBUG, m_szCOM, szFunc, _T("Fails in setting COM wait mask."));
		//LogCOMFuncSystemError(m_szCOM, szFunc, m_lLastError);
		return FALSE;
	}
	if (!::SetupComm(m_hFile, RECV_BUFFER_SIZE, SEND_BUFFER_SIZE)) {
		m_lLastError = ::GetLastError();
		//LogCOMFuncInfo(LOG_LEVEL_DEBUG, m_szCOM, szFunc, _T("Fails in setting COM Input and Output queue size."));
		//LogCOMFuncSystemError(m_szCOM, szFunc, m_lLastError);
		return FALSE;
	}
	if (!::PurgeComm(m_hFile, PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_RXABORT)) {
		m_lLastError = ::GetLastError();
		//LogCOMFuncInfo(LOG_LEVEL_DEBUG, m_szCOM, szFunc, _T("::PurgeComm failed."));
		//LogCOMFuncSystemError(m_szCOM, szFunc, m_lLastError);
		return FALSE;
	}
	DCB dcb;
	if (!::GetCommState(m_hFile, &dcb)) {
		m_lLastError = ::GetLastError();
		//LogCOMFuncInfo(LOG_LEVEL_DEBUG, m_szCOM, szFunc, _T("::GetCommState failed."));
		//LogCOMFuncSystemError( m_szCOM , szFunc, m_lLastError);
		return FALSE;
	}
	//dcb.DCBlength = sizeof(DCB);
	//dcb.BaudRate = CBR_115200;
	//dcb.fBinary = TRUE;
	//dcb.fParity = FALSE;
	//dcb.fRtsControl = RTS_CONTROL_DISABLE;
	//dcb.fDtrControl = DTR_CONTROL_DISABLE;
	//dcb.fOutxCtsFlow = FALSE;
	//dcb.fOutxDsrFlow = FALSE;
	//dcb.fDsrSensitivity = FALSE;
	//dcb.fTXContinueOnXoff = FALSE;
	//dcb.fOutX = FALSE;
	//dcb.fInX = FALSE;
	//dcb.fErrorChar = FALSE;
	//dcb.fNull = FALSE;
	//dcb.fAbortOnError = FALSE; //  check with ClearCommError()
	//dcb.XonLim = 0;  // don't care
	//dcb.XoffLim = 0;  //  don't care
	//dcb.ByteSize = 8;
	//dcb.Parity = NOPARITY;
	//dcb.StopBits = ONESTOPBIT;
	//dcb.XonChar = 0;  //  don't care
	//dcb.XoffChar = 0;  //  don't care
	//dcb.ErrorChar = 0;  //  don't care
	//dcb.EofChar = 0x00;  //  20071113 Embert Tsai. Change for 6280.
	//dcb.EvtChar = 0x7E;  //  should signal EV_RXFLAG


	dcb.BaudRate = m_eBaudrate;
	dcb.fParity = FALSE;
	dcb.fOutxDsrFlow = FALSE;
	dcb.fDtrControl = DTR_CONTROL_ENABLE;
	dcb.fDsrSensitivity = FALSE;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.XoffChar = 0x13;
	dcb.XonChar = 0x11;

	dcb.fTXContinueOnXoff = FALSE;
	dcb.fInX = FALSE;
	dcb.fOutX = FALSE;
	dcb.fOutxCtsFlow = FALSE;
	dcb.fRtsControl = RTS_CONTROL_ENABLE;



	if (!::SetCommState(m_hFile, &dcb)) {
		m_lLastError = ::GetLastError();
		//LogCOMFuncInfo(LOG_LEVEL_DEBUG, m_szCOM, szFunc, _T("::SetCommState failed."));
		//LogCOMFuncSystemError( m_szCOM, szFunc, m_lLastError);
		return FALSE;
	}
	COMMTIMEOUTS cto;
	memset(&cto, 0, sizeof(cto));
	//cto.ReadIntervalTimeout = MAXDWORD;
	//cto.ReadTotalTimeoutMultiplier = 0;
	//cto.ReadTotalTimeoutConstant = 50;

	cto.ReadIntervalTimeout = DEF_READ_INTERVAL_TIMEOUT;
	cto.ReadTotalTimeoutMultiplier = DEF_READ_TOTAL_TIMEOUT_MULTIPLIER;
	cto.ReadTotalTimeoutConstant = DEF_READ_TOTAL_TIMEOUT_CONSTANT;
	cto.WriteTotalTimeoutMultiplier = DEF_WRITE_TOTAL_TIMEOUT_MULTIPLIER;
	cto.WriteTotalTimeoutConstant = DEF_WRITE_TOTAL_CONSTANT;


	if (!::SetCommTimeouts(m_hFile, &cto)) {
		m_lLastError = ::GetLastError();
		//LogCOMFuncInfo(LOG_LEVEL_DEBUG, m_szCOM, szFunc, _T("::SetCommTimeouts failed."));
		//LogCOMFuncSystemError( m_szCOM, szFunc, m_lLastError);
		return FALSE;
	}

	::SetCommMask(m_hFile, EV_RXCHAR);
	::EscapeCommFunction (m_hFile, SETDTR|SETRTS);
	Sleep(100);

	if (!::PurgeComm(m_hFile, PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_RXABORT)) {
		m_lLastError = ::GetLastError();
		//LogCOMFuncInfo(LOG_LEVEL_DEBUG, m_szCOM, szFunc, _T("::PurgeComm failed."));
		//LogCOMFuncSystemError(m_szCOM, szFunc, m_lLastError);
		return FALSE;
	}
	return TRUE;
}

LONG CSerialIO::StartListener()
{
	LPCTSTR szFunc = _T("CSerialIO::StartListener");
	//LogCOMFuncInfo(LOG_LEVEL_WARNING, m_szCOM, szFunc, _T("Start listener."));
	////////////////////////////////////////////////////////////////////////////////
	if (m_hThread == 0) {
		_ASSERTE(!m_fStopping);
		DWORD dwThreadId = 0;
		m_hThread = ::CreateThread(0, 0, ThreadProc, LPVOID(this), 0, &dwThreadId);
		if (m_hThread == 0) {
			m_lLastError = ::GetLastError();
			//LogCOMFuncInfo(LOG_LEVEL_WARNING, m_szCOM, szFunc, _T("Start listener failed."));
			//LogCOMFuncSystemError(m_szCOM, szFunc, m_lLastError);
			return m_lLastError;
		}
		SetThreadPriority(m_hThread, THREAD_PRIORITY_HIGHEST);
		// add code to wait for create create complete
	}
	else {
		//LogCOMFuncInfo(LOG_LEVEL_WARNING, m_szCOM, szFunc, _T("Thread has already existed."));
	}
	//LogCOMFuncInfo(LOG_LEVEL_WARNING, m_szCOM, szFunc, _T("Start listener completed."));
	m_lLastError = ERROR_SUCCESS;
	return m_lLastError;
}

LONG CSerialIO::StopListener(DWORD dwTimeout)
{
	LPCTSTR szFunc = _T("CSerialIO::StopListener");
	////////////////////////////////////////////////////////////////////////////////
	if (m_hThread) {
		m_fStopping = TRUE;
		//LogCOMFuncInfo(LOG_LEVEL_WARNING, m_szCOM, szFunc, _T("Start stop listen thread."));
		SetEvent(m_hReadEvent);
		::WaitForSingleObject(m_hThread, dwTimeout);
		//LogCOMFuncInfo(LOG_LEVEL_WARNING, m_szCOM, szFunc, _T("Complete stop listen thread."));
		m_fStopping = FALSE;
		::CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	else {
		//LogCOMFuncInfo(LOG_LEVEL_ERROR, m_szCOM, szFunc, _T("Listen thread is null, so no stop operation."));
	}
	m_lLastError = ERROR_SUCCESS;
	return m_lLastError;
}

DWORD WINAPI CSerialIO::ThreadProc(LPVOID lpArg)
{
	CSerialIO* pThis = reinterpret_cast<CSerialIO*>(lpArg);
	return pThis->ThreadProc();
}

DWORD CSerialIO::ThreadProc()
{
	LPCTSTR szFunc = _T("CSerialIO::ThreadProc");
	////////////////////////////////////////////////////////////////////////////////
	char szBuffer[RECV_BUFFER_SIZE] = {0};
	DWORD dwRead = 0;
	OVERLAPPED ovInternal = {0};
	HANDLE hReadEvent = ::CreateEvent(0, FALSE, FALSE, 0);
	ovInternal.hEvent = hReadEvent;

	InternalClose();
	::Sleep(100);
	InternalOpen();

	do {
		if (!::ReadFile(m_hFile, szBuffer, RECV_BUFFER_SIZE, &dwRead, &ovInternal)) {
			long lLastError = ::GetLastError();
			if (lLastError != ERROR_IO_PENDING) {
				m_lLastError = lLastError;
				//LogCOMFuncInfo(LOG_LEVEL_WARNING, m_szCOM, szFunc, _T("Read com port data error."));
				//LogCOMFuncSystemError(m_szCOM, szFunc, m_lLastError);
				Sleep(500);
			}
			else {
				switch (::WaitForSingleObject(ovInternal.hEvent, INFINITE)) {
				case WAIT_OBJECT_0:
					if (!::GetOverlappedResult(m_hFile, &ovInternal, &dwRead, FALSE)) {
						m_lLastError = ::GetLastError();
						//LogCOMFuncInfo(LOG_LEVEL_WARNING, m_szCOM, szFunc, _T("GetOverlappedResult failed (%d)"), m_lLastError);
					}
					else if (dwRead != 0) {
						//LogCOMFuncInfo(LOG_LEVEL_WARNING, m_szCOM, szFunc, _T("Read COM port buffer."));
						//LogCOMFuncBuffer(LOG_LEVEL_WARNING, m_szCOM, szFunc, szBuffer, dwRead);
						m_csRead.Lock();
						AppendBuffer(szBuffer, dwRead);
						m_csRead.Unlock();
						OnEvent();
					}
					break;

				case WAIT_TIMEOUT:
					_ASSERTE(FALSE);
					//LogCOMFuncInfo(LOG_LEVEL_ERROR, m_szCOM, szFunc, _T("Read operation time out."));
					::CancelIo(m_hFile);
					m_lLastError = ERROR_TIMEOUT;
					m_fStopping = TRUE;
					break;

				default:
					_ASSERTE(FALSE);
					//LogCOMFuncInfo(LOG_LEVEL_ERROR, m_szCOM, szFunc, _T("Read operation time out."));
					m_lLastError = ::GetLastError();
					//LogCOMFuncSystemError(m_szCOM, szFunc, m_lLastError);
					m_fStopping = TRUE;
					break;
				}
			}
		}
	}
	while (!m_fStopping);

	CloseHandle(hReadEvent);
	InternalClose();
	return 0;
}

int CSerialIO::ReadBuffer(void* pvDest, int nLen)
{
	int nRet;
	DWORD dwBufLen = m_dwBufEnd - m_dwBufStart;
	nRet = nLen <= (int) dwBufLen ? nLen : (int)dwBufLen;
	CopyMemory(pvDest, m_pbRecvBuf + m_dwBufStart, nRet);
	m_dwBufStart += nRet;

	return nRet;
}

int CSerialIO::AppendBuffer(const void* pvSrc, int nLen)
{
	LPCTSTR szFunc = _T("CSerialIO::AppendBuffer");
	//LogCOMFuncInfo(LOG_LEVEL_DEBUG, m_szCOM, szFunc, _T("Add read content to buffer."));
	////////////////////////////////////////////////////////////////////////////////
	if ( ( nLen + ( m_dwBufEnd - m_dwBufStart ) ) > m_dwBufSize  ) {
		BYTE *pbOld = m_pbRecvBuf;
		m_dwBufSize += ((nLen / RECV_BUFFER_SIZE) + 1) * RECV_BUFFER_SIZE;
		m_pbRecvBuf = new BYTE[m_dwBufSize];
		ZeroMemory(m_pbRecvBuf, m_dwBufSize);
		if ( m_dwBufStart != m_dwBufEnd)
			CopyMemory(m_pbRecvBuf, pbOld + m_dwBufStart, m_dwBufEnd - m_dwBufStart);

		m_dwBufEnd -= m_dwBufStart;
		m_dwBufStart = 0;
		delete[] pbOld;
		pbOld = NULL;
	}
	else if ( ( m_dwBufEnd + nLen )  >=  m_dwBufSize) {
		MoveMemory(m_pbRecvBuf, m_pbRecvBuf + m_dwBufStart, m_dwBufEnd - m_dwBufStart);
		m_dwBufEnd -= m_dwBufStart;
		m_dwBufStart = 0;
	}
	CopyMemory(m_pbRecvBuf + m_dwBufEnd, pvSrc, nLen );
	m_dwBufEnd += nLen;
	return nLen;
}
