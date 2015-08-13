// SerialIO.cpp: implementation of the CSerialIO class.
// 2004/10/11
// Change std::string to character string
//////////////////////////////////////////////////////////////////////
//#define STRICT

#include "stdafx.h"
#include <crtdbg.h>
#include <tchar.h>
#include <stdio.h>
#include "SerialIO.h"
#include "..\Common\CommonUtil\LogPolicy.h"

#define READ_BUFFER_LEN	100
#define RF_ANALOG_MODE	0
#define CMDIAG_MODE_OFFLINE_A_F  0	/* Go to offline analog */
#define CMDIAG_MODE_OFFLINE_D_F	 1   /* Go to offline digital */
#define DIAG_OFFLINE_DIGITAL 2
#define DIAG_OFFLINE_ANALOG  6
#define MODE_DETECTION_RETY_TIMES 20
#define MODE_DETECTION_DELAY	1000
#define SERIAL_QUEUE_SIZE		4096


CSerialIO::CSerialIO() : m_lLastError(ERROR_SUCCESS),
						 m_hFile(NULL),
						 m_hWriteEvent(NULL),
						 m_hWaitEvent(NULL),
						 m_hModeEvent(NULL),
						 m_hStopEvent(NULL),
						 m_fStopping(FALSE),
						 m_bIsCOMReopen(FALSE),
						 m_bIsReOpen(FALSE),
						 m_hThread(NULL),
						 m_hReadEvent(NULL),
						 m_eState(EStop),
						 m_bDestroy(FALSE),
						 m_dwBufStart(0),
						 m_dwBufEnd(0),
						 m_dwCurWriteCount(0),
						 m_nCurrentSpeedTry(0),
						 m_bIsPreventOuterRead(FALSE),
						 m_bIsPreventOuterWrite(FALSE),
						 m_ePreferBaudRate(EBaud115200),
						 m_dwBufSize(RECV_BUFFER_SIZE),
						 m_bIsSetProbeMode(false),
						 m_bIsRequestFunction(false)
{
	m_pbRecvBuf = new BYTE [m_dwBufSize];
	ZeroMemory(m_pbRecvBuf, m_dwBufSize);
	ZeroMemory(m_szDevice, sizeof(TCHAR) * COM_PORT_NAME_LENGTH);
	m_TestSpeed[0]= EBaud115200;
	m_TestSpeed[1]= EBaud230400;
	m_TestSpeed[2]= EBaud460800;
}

CSerialIO::~CSerialIO()
{
	m_bDestroy = TRUE;
	StopListener(INFINITE);

	InternalClose();
	_ASSERTE(m_pbRecvBuf != NULL);
	delete[] m_pbRecvBuf;
	m_pbRecvBuf = NULL;
}

LONG CSerialIO::Open(LPCTSTR lpszDevice, EBaudrate eBaudrate,bool bRequestFunction)
{
	LONG lRet = ERROR_SUCCESS;
	m_bIsRequestFunction = bRequestFunction;

	_ASSERTE(lpszDevice != NULL);
	if(m_hFile)
		m_lLastError = ERROR_ALREADY_INITIALIZED;
	else
	{
		::_tcscpy(	m_szDevice,lpszDevice);
		m_eBaudrate = eBaudrate;

		lRet = InternalOpen();
		if(lRet == ERROR_SUCCESS)
			StartListener();

		if(lRet != ERROR_SUCCESS)
			m_lLastError = lRet;
	}
	return lRet;
}

LONG CSerialIO::Close(void)
{
	LONG lRet = ERROR_SUCCESS;
	
	StopListener();
	if(m_hFile != 0)
	{
		if ( InternalClose() != ERROR_SUCCESS )
		{
			AddLog(_T("internal close failed."));
			lRet = RUN_FAILED;
		} 
		else 
		{
			lRet = RUN_SUCCESSFULLY;
		}
	}
	m_lLastError = ERROR_SUCCESS;

	return lRet;
}

// 20050223 Embert Tsai
// Add this method for Ulysses download
int CSerialIO::ReOpen(){
	TRACE("CSerialIO::ReOpen()");
	int nRet = RUN_FAILED;

	m_bIsReOpen = TRUE;
	StopListener();
	Sleep(500);	
	m_ePreferBaudRate = m_eBaudrate;
	m_nCurrentSpeedTry = GetSpeedIndex(m_ePreferBaudRate);
	int i=1;
	for (i = 1 ; i< 10 ; i+=i){
		if ( InternalOpen() != ERROR_SUCCESS )
			Sleep(i * 1000);	// 2004/09/21 Embert Tsai. Give mobile and driver more time to change to download mode.
		else 
			break;
	}
	m_bIsReOpen = FALSE;
	if ( i <= 10 )
	{	
		nRet = RUN_SUCCESSFULLY;
	}

	return nRet;
}

LONG CSerialIO::StartListener (void)
{
	if (m_hThread == 0)
	{
		_ASSERTE(!m_fStopping);
		DWORD dwThreadId = 0;
		m_hThread = ::CreateThread(0,0,ThreadProc,LPVOID(this),0,&dwThreadId);
		if (m_hThread == 0)
		{
			m_lLastError = ::GetLastError();
		}
		// add code to wait for create create complete
	} 
	else 
	{
		m_lLastError = ERROR_SUCCESS;
	}
	return m_lLastError;
}

LONG CSerialIO::StopListener (DWORD dwTimeout)
{
	if (m_hThread)
	{
		SetEvent(m_hStopEvent);
		Sleep(0);
		SetEvent(m_hReadEvent);
		::WaitForSingleObject(m_hThread,dwTimeout);
		m_fStopping = FALSE;
		::CloseHandle(m_hThread);
		m_hThread = 0;

		ResetMode();
	} 

	m_lLastError = ERROR_SUCCESS;
	return m_lLastError;
}

DWORD WINAPI CSerialIO::ThreadProc (LPVOID lpArg)
{
	CSerialIO* pThis = reinterpret_cast<CSerialIO*>(lpArg);
	return pThis->ThreadProc();
}

DWORD CSerialIO::ThreadProc(void)
{
	char szBuffer[4096] = {0};
	OVERLAPPED ovInternal = {0};
	DWORD dwRead = 0;
	BOOL bContinueRecv = FALSE;

	ResetMode();
	if(m_bDestroy != TRUE)
	{
		OnEvent(EOnStart);
	}
	do
	{
		if(m_eState == EStart)  
		{
			if( (bContinueRecv == FALSE) && m_bIsCOMReopen)
			{ //close and restart continously
				InternalClose();
				::Sleep(100);	// Added by Embert to let driver have more time to start up

				if(InternalOpen() != ERROR_SUCCESS)
				{
					::Sleep(500);
					continue;
				};
			}
			ovInternal.hEvent = m_hReadEvent;
		}

		if (FALSE == ::ReadFile(m_hFile, szBuffer, 4096, &dwRead, &ovInternal))
		{
			long lLastError = ::GetLastError();
			if (lLastError != ERROR_IO_PENDING)
			{
				TRACE("COMM port disconnected\n");
				bContinueRecv = NotErrorIOPending(lLastError, bContinueRecv);// COMM port disconnected.
			}
			else
			{
				bContinueRecv = ErrorIOPending(ovInternal, dwRead, szBuffer, bContinueRecv);
			}
		}
		else
		{
			bContinueRecv = ErrorIOPending(ovInternal, dwRead, szBuffer, bContinueRecv);
		}
	}
	while (WaitForSingleObject(m_hStopEvent, 0) == WAIT_TIMEOUT);
	AddLog(_T("ThreadProc: InternalClose"));
	InternalClose();

	if ( !m_bIsReOpen)
		m_eState = EStop;
	if(m_bDestroy != TRUE && !m_bIsReOpen)
	{
		TRACE("ThreadProc: OnStop Notify\n");
		OnEvent(EOnStop);
	}
	// Bye bye
	return 0;
}
BOOL CSerialIO::ErrorIOPending( OVERLAPPED &ovInternal, DWORD dwRead, char * szBuffer, BOOL bContinueRecv )
{
	Sleep(0);
	HANDLE hEvents[2] = {ovInternal.hEvent, m_hStopEvent};
	//switch (::WaitForSingleObject(ovInternal.hEvent,DEFAULT_CMD_TIME_OUT))
	switch(::WaitForMultipleObjects(2, hEvents, FALSE, INFINITE))
	{
	case WAIT_OBJECT_0 + 0:
		if (!::GetOverlappedResult(m_hFile,&ovInternal,&dwRead,FALSE))
		{
			m_lLastError = ::GetLastError();
		}
		else
		{
			if(dwRead != 0)
			{
				AddLog(_T("Get data"));
				LogBinary(szBuffer,dwRead);

				{
					AutoProtect protect(m_csRead);
					AppendBuffer(szBuffer,dwRead);
				}
				if(m_bDestroy != TRUE)
				{
					if ( !m_bIsPreventOuterRead )
					{
						OnEvent(EOnRecv);
					}
				}
			}
		}
		break;

	case WAIT_OBJECT_0 + 1:
		::CancelIo(m_hFile);
		m_lLastError = ERROR_TIMEOUT;
		SetEvent(m_hStopEvent);
		break;

	default:
		_ASSERTE(FALSE);
		m_lLastError = ::GetLastError();
		SetEvent(m_hStopEvent);
		break;
	};
	return bContinueRecv;
}

BOOL CSerialIO::NotErrorIOPending(long lLastError, BOOL bContinueRecv )
{
	if(m_eState != EStart && m_bDestroy != TRUE)
	{
		ResetMode();
		OnEvent(EOnStart);
	}
	m_lLastError = lLastError;
	bContinueRecv = FALSE;
	Sleep(500);	
	return bContinueRecv;
}

LONG CSerialIO::InternalOpen()
{
	AutoProtect protect(m_csWrite);

	bool bRes = false;
	AddLog(_T("CSerialIO::InternalOpen"));
	m_lLastError = ERROR_SUCCESS;

	m_eBaudrate = m_TestSpeed[m_nCurrentSpeedTry++];

	if ( m_nCurrentSpeedTry == SPEED_ARRAY_SIZE )
		m_nCurrentSpeedTry = 0;

	if (m_hFile)
	{
		m_lLastError = ERROR_ALREADY_INITIALIZED;
		AddLog(_T("COM port has been opened."));
		return m_lLastError;
	}

	int nRetry = 0;
	do
	{
		AddLog(m_szDevice);
		m_hFile = ::CreateFile(m_szDevice,
			GENERIC_READ|GENERIC_WRITE|MAXIMUM_ALLOWED,
			0,
			0,
			OPEN_EXISTING,
			FILE_FLAG_OVERLAPPED,
			0);

		if(m_hFile == INVALID_HANDLE_VALUE)
		{
			m_lLastError = ::GetLastError();
			AddLog(_T("Fails in opening COM port."));

			if(++nRetry > 2)
			{
				AddLog(_T("++nRetry > 2"));
				break;
			}

			Sleep(1000);
		}
		else
		{
			m_lLastError = InternalResetCOM();
			if ( ERROR_SUCCESS != m_lLastError)
			{
				AddLog(_T("InternalResetCOM fail."));
			}
			else
				bRes = true;
			break;
		}
	}
	while(m_hFile == INVALID_HANDLE_VALUE);

	if(bRes)
	{
		// create overlapped event
		m_hWriteEvent = ::CreateEvent(0, FALSE, FALSE, 0);
		if (m_hWriteEvent == 0)
		{
			m_lLastError = ::GetLastError();
			AddLog(_T("::CreateEvent Write event failed."));
		}

		m_hReadEvent = ::CreateEvent(0, FALSE, FALSE, 0);
		if (m_hReadEvent == 0)
		{
			m_lLastError = ::GetLastError();
			AddLog(_T("Create read event failed."));
		}

		if(!m_hStopEvent)
			m_hStopEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);	
		if (m_hStopEvent == 0)
		{
			m_lLastError = ::GetLastError();
			AddLog(_T("Create m_hStopEvent event failed."));
		}
		else
			ResetEvent(m_hStopEvent); //unsignal

		if(!m_hModeEvent)
			m_hModeEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
		if (m_hModeEvent == 0)
		{
			m_lLastError = ::GetLastError();
			AddLog(_T("Create m_hModeEvent event failed."));
		}
		else
			ResetMode();		

		if(m_lLastError != ERROR_SUCCESS)
		{
			m_lLastError = ::GetLastError();
			m_hFile = NULL;
		}
	}
	else{
		AddLog(_T("InternalOpen() fail"));
		m_hFile = NULL;
	}
	return m_lLastError;
}

LONG CSerialIO::InternalClose(void)
{
	AutoProtect protect(m_csWrite);

	m_lLastError = ERROR_SUCCESS;
	if (m_hFile == 0)
	{
		return m_lLastError;
	}

	if ( !::CloseHandle(m_hFile) ){
		m_lLastError = GetLastError();
	}

	m_hFile = 0;

	if (m_hWriteEvent)
	{
		if ( ! ::CloseHandle(m_hWriteEvent) ){
			m_lLastError = GetLastError();
			return m_lLastError;
		}
		m_hWriteEvent = 0;
	}

	if (m_hReadEvent)
	{
		if ( !::CloseHandle(m_hReadEvent) ){
			m_lLastError = GetLastError();
			return m_lLastError;
		}
		m_hReadEvent = 0;
	}

	if (m_hStopEvent)
	{
		if ( !::CloseHandle(m_hStopEvent) ){
			m_lLastError = GetLastError();
			return m_lLastError;
		}
		m_hStopEvent = 0;
	}

	if(m_hModeEvent)
	{
		if ( !::CloseHandle(m_hModeEvent) )
		{
			m_lLastError = GetLastError();
			return m_lLastError;
		}
		m_hModeEvent = 0;
	}

	return m_lLastError;
}


LONG CSerialIO::Write(const void* pData, size_t iLen, DWORD* pdwWritten)
{
	//AddLog(_T("CSerialIO::Write"));

	_ASSERTE( pData != NULL );
	_ASSERTE( iLen > 0 );
	if ( m_bIsPreventOuterWrite)	//Prevent outer class to write packet
	{		
		AddLog(_T("CSerialIO::Write ==> Prevent outer class to write packet"));
		return -1;
	}

	::InterlockedIncrement(&m_dwCurWriteCount);

	long lRet = 0;
	lRet = InternalWrite(pData, iLen, pdwWritten);

	::InterlockedDecrement(&m_dwCurWriteCount);
	return lRet;
}

LONG CSerialIO::Read(void* pData, size_t iLen, DWORD* pdwRead)
{
	_ASSERTE(pData != NULL);
	_ASSERTE(iLen > 0);
	m_lLastError = ERROR_SUCCESS;

	DWORD dwRead;
	if (pdwRead == 0)
	{
		pdwRead = &dwRead;
	}
	*pdwRead = 0;

	if (m_hFile == 0)
	{
		m_lLastError = ERROR_INVALID_HANDLE;
		return m_lLastError;
	}

#ifdef _DEBUG
	//memset(pData,0xDC,iLen);
	FillMemory(pData, iLen, 0xDC);
#endif
	{
		AutoProtect protect(m_csRead);
		*pdwRead = ReadBuffer(pData,iLen);
	}

	return m_lLastError;
}

CSerialIO::EBaudrate CSerialIO::GetBaudrate()
{
	m_lLastError = ERROR_SUCCESS;
	if (m_hFile == 0)
	{
		m_lLastError = ERROR_INVALID_HANDLE;
		return EBaudUnknown;
	}
	DCB dcb;
	dcb.DCBlength = sizeof (DCB);
	if (!::GetCommState(m_hFile,&dcb))
	{
		m_lLastError = ::GetLastError();
		return EBaudUnknown;
	}
	return EBaudrate(dcb.BaudRate);
}

// 20050113 Embert Tsai
// Add this method for Topaz download
int CSerialIO::ChangeBaudRate(EBaudrate eBaudRate)
{
	StopListener();
	Sleep(2000);
	m_ePreferBaudRate = eBaudRate;
	m_nCurrentSpeedTry = GetSpeedIndex(m_ePreferBaudRate);
	StartListener();
	return RUN_SUCCESSFULLY;
}

void CSerialIO::LogBinary(const char* szTemp,DWORD nBufferSize)
{
#ifndef DISABLE_LOG

	const int nLenMax = 32;
	char szCharTemp[nLenMax+1] = {0};
	int nLineCount = 0;

	CStringA strLog;

	for(DWORD i=0;i<nBufferSize;i++)
	{		
		if(i > 0 && i % nLenMax == 0)
		{
			strLog += "   ";
			for(int j=0;j<nLenMax;j++)
			{
				CStringA strTemp;
				strTemp.Format("%c",szCharTemp[j]);
				strLog += strTemp;
			}
			strLog += "\n";
			nLineCount = 0;
		}
		BYTE nTest = szTemp[i];
		CStringA strTemp;
		strTemp.Format("%02X ",nTest);
		strLog += strTemp;
		szCharTemp[i%nLenMax] = (nTest <= 0x1F || nTest >= 0xFF) ? '.' : szTemp[i];
		nLineCount++;
	}

	for(int nSpaceCount = 0;nSpaceCount < (nLenMax - nLineCount);nSpaceCount++)	
		strLog += "   ";

	strLog += "   ";
	for(int j=0;j<nLineCount;j++)
	{		
		CStringA strTemp;
		strTemp.Format("%c",szCharTemp[j]);
		strLog += strTemp;
	}
	strLog += "\n\n";

	AddLog(strLog);

#endif//#ifndef DISABLE_LOG
}

long CSerialIO::InternalWrite(const void* pData, size_t iLen, DWORD* pdwWritten /* = 0 */)
{
	AutoProtect protect(m_csWrite);

	//AddLog(_T("CSerialIO::InternalWrite"));

	m_lLastError = ERROR_SUCCESS;
	// Use our own variable for read count
	DWORD dwWritten;
	if (pdwWritten == 0)
	{
		pdwWritten = &dwWritten;
	}
	// Reset the number of bytes written
	*pdwWritten = 0;
	// Check if the device is open
	if (m_hFile == 0 || m_hFile == INVALID_HANDLE_VALUE)
	{
		m_lLastError = ERROR_INVALID_HANDLE;
		AddLog(_T("COM port handle is null"));
		return m_lLastError;
	}

	OVERLAPPED ovInternal = {0};
	ovInternal.hEvent = m_hWriteEvent;

	if (!::WriteFile(m_hFile,pData,iLen,pdwWritten,&ovInternal))
	{
		long lLastError = ::GetLastError();
		if (lLastError != ERROR_IO_PENDING)
		{
			m_lLastError = lLastError;
		}
		else
		{
			switch (::WaitForSingleObject(ovInternal.hEvent,INFINITE))
			{
			case WAIT_OBJECT_0:
				// The overlapped operation has completed
				if (!::GetOverlappedResult(m_hFile,&ovInternal,pdwWritten,FALSE))
				{
					m_lLastError = ::GetLastError();
					AddLog(_T("InteralWrite: Write fail\n"));
				}
				else
				{					
					AddLog(_T("Write data success"));
					LogBinary((const char*)pData,*pdwWritten);
				}
				break;

			case WAIT_TIMEOUT:
				::CancelIo(m_hFile);
				m_lLastError = ERROR_TIMEOUT;
				AddLog(_T("InteralWrite: Write time out\n"));
				break;
			default:
				m_lLastError = ::GetLastError();
				break;
			}
		}
	}
	else
		// It should means the unit is disconnected.
		m_lLastError = ERROR_NOT_READY;

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
int CSerialIO::InternalResetCOM(){
	// Set COM port event mask

	if ( !::SetCommMask(m_hFile, EV_CTS | EV_DSR | EV_RLSD | EV_BREAK) ){
		m_lLastError = ::GetLastError();
		return m_lLastError;	
	}

	// Set Input/Ouput Queue Size
	if (!::SetupComm(m_hFile, SERIAL_QUEUE_SIZE, SERIAL_QUEUE_SIZE)){
		m_lLastError = ::GetLastError();
		return m_lLastError;	
	}

	// Purge COM port buffer
	if(!::PurgeComm(m_hFile, PURGE_TXCLEAR |
		PURGE_RXCLEAR |
		PURGE_TXABORT |
		PURGE_RXABORT) )
	{
		m_lLastError = ::GetLastError();
		return m_lLastError;	
	}

	// setup the DCB structure for the device
	DCB dcb;
	if(!::GetCommState(m_hFile, &dcb))
	{
		m_lLastError = ::GetLastError();
		return m_lLastError;	
	}
	//dcb.DCBlength = sizeof (DCB);
	//dcb.BaudRate = m_eBaudrate;
	//dcb.fBinary = TRUE;
	//dcb.fParity = FALSE;
	//// Enable RTS & DTR control signal to support power for data cable (PhoneJack)
	//dcb.fRtsControl = RTS_CONTROL_ENABLE;
	//dcb.fDtrControl = DTR_CONTROL_ENABLE;
	//dcb.fOutxCtsFlow = FALSE;
	//dcb.fOutxDsrFlow = FALSE;
	//dcb.fDsrSensitivity = FALSE;
	//dcb.fTXContinueOnXoff = FALSE;
	//dcb.fOutX = FALSE;
	//dcb.fInX = FALSE;
	//dcb.fErrorChar = FALSE;
	//dcb.fNull = FALSE;
	//dcb.fAbortOnError = TRUE;  //  check with ClearCommError ()
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


	if(!::SetCommState(m_hFile, &dcb))
	{
		m_lLastError = ::GetLastError();
		return m_lLastError;	
	}

	// The following default value are changed according to QPST log.
	COMMTIMEOUTS cto;
	cto.ReadIntervalTimeout = DEF_READ_INTERVAL_TIMEOUT;
	cto.ReadTotalTimeoutMultiplier = DEF_READ_TOTAL_TIMEOUT_MULTIPLIER;
	cto.ReadTotalTimeoutConstant = DEF_READ_TOTAL_TIMEOUT_CONSTANT;
	cto.WriteTotalTimeoutMultiplier = DEF_WRITE_TOTAL_TIMEOUT_MULTIPLIER;
	cto.WriteTotalTimeoutConstant = DEF_WRITE_TOTAL_CONSTANT;

	if (!::SetCommTimeouts(m_hFile,&cto))
	{
		m_lLastError = ::GetLastError();
		return m_lLastError;	
	}

	::SetCommMask(m_hFile, EV_RXCHAR);

	::EscapeCommFunction (m_hFile, SETDTR|SETRTS);

	Sleep(100);
	if(!::PurgeComm(m_hFile, PURGE_TXCLEAR |
		PURGE_RXCLEAR |
		PURGE_TXABORT |
		PURGE_RXABORT) )
	{
		m_lLastError = ::GetLastError();
		return m_lLastError;	
	}

	return ERROR_SUCCESS;
}

// 20041110 Embert Tsai
// Change read buffer operation to reduce memory operation.
int CSerialIO::ReadBuffer(void* pvDest, int nLen)
{
	int nRet;
	DWORD dwBufLen = m_dwBufEnd - m_dwBufStart;
	nRet = nLen <= (int) dwBufLen ? nLen : (int)dwBufLen;
	CopyMemory(pvDest, m_pbRecvBuf+m_dwBufStart, nRet);
	m_dwBufStart += nRet;

	return nRet;
}

// 20041110 Embert Tsai
// Change read buffer operation to reduce memory operation.
int CSerialIO::AppendBuffer(const void* pvSrc, int nLen){

	if ( ( nLen + ( m_dwBufEnd - m_dwBufStart ) ) > m_dwBufSize  ){ // buffer size is tool small to contain the buffer
		BYTE *pbOld = m_pbRecvBuf;
		m_dwBufSize += ((nLen/RECV_BUFFER_SIZE)+1) * RECV_BUFFER_SIZE;
		m_pbRecvBuf = new BYTE[m_dwBufSize];
		ZeroMemory(m_pbRecvBuf, m_dwBufSize);
		if ( m_dwBufStart != m_dwBufEnd)	// Check buffer is empty.
			CopyMemory(m_pbRecvBuf, pbOld + m_dwBufStart, m_dwBufEnd-m_dwBufStart);

		m_dwBufEnd -= m_dwBufStart;
		m_dwBufStart = 0;
		delete[] pbOld;
		pbOld = NULL;
	} else if ( ( m_dwBufEnd + nLen )  >=  m_dwBufSize) {
		MoveMemory(m_pbRecvBuf, m_pbRecvBuf + m_dwBufStart, m_dwBufEnd - m_dwBufStart);
		m_dwBufEnd -= m_dwBufStart;
		m_dwBufStart = 0;
	}
	CopyMemory(m_pbRecvBuf+m_dwBufEnd,pvSrc, nLen );
	m_dwBufEnd += nLen;
	if ( m_hWaitEvent != NULL ){
		::SetEvent(m_hWaitEvent);
	}
	return nLen;

}

int CSerialIO::GetSpeedIndex(EBaudrate eBaudrate)
{
	for ( int i = 0 ; i < SPEED_ARRAY_SIZE; i ++){
		if ( eBaudrate == m_TestSpeed[i])
			return i;
	}
	return -1;
}
