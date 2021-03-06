#include "StdAfx.h"
#include "AutoMMIBBCommunicate.h"


CCommunicate::CCommunicate(void)
{
	m_str_port = _T("");
}

CCommunicate::~CCommunicate(void)
{
	/* set m_b_connected */
	m_b_connected = FALSE;
	if (m_h_com != NULL)
		m_h_com = NULL;
}

int CCommunicate::SetCOMPort(CString str_COMPort)
{
	if (str_COMPort == "")
	{
		return 2013031310;
	}

	CString cs_str_port;
	cs_str_port.Format(_T("\\\\.\\%s"), str_COMPort);

	m_str_port        = cs_str_port;
	m_b_connected     = FALSE;
	m_h_com           = NULL;
	m_os_write.hEvent = NULL;
	m_os_read.hEvent  = NULL;

	return 0;
}

bool CCommunicate::ConfigureCOMPort(CString strPort)
{
	bool bRes = false;

	//Create the handle for read the COM port
	m_h_Com =  CreateFile(strPort,
	                      GENERIC_READ | GENERIC_WRITE,
	                      (DWORD)NULL,   // exclusive access
	                      NULL,          // no security
	                      OPEN_EXISTING,
	                      FILE_FLAG_OVERLAPPED,
	                      NULL           // hTemplate
	                     );

	if ((m_h_Com == NULL) || (INVALID_HANDLE_VALUE == m_h_Com))
	{
		return bRes;
	}
	return bRes;
}

int CCommunicate::OpenCOM(void)
{
	COMMTIMEOUTS st_timeOuts;			// timeout
	BOOL b_setupComm;                   // set up comm
	BOOL b_setCommMask;                 // set comm mask
	BOOL b_setCommTimeouts;             // set comm timeout
	BOOL b_configComm;                  // config comm
	BOOL b_purgeComm;                   // purge comm

	b_setupComm = FALSE;
	b_setCommMask = FALSE;
	b_setCommTimeouts = FALSE;
	b_configComm = FALSE;
	b_purgeComm = FALSE;

	/* init overlapped read and write */
	memset(&m_os_write, 0, sizeof(m_os_write));
	memset(&m_os_read, 0, sizeof(m_os_read));

	/* if read and write event handle is existent, close */
	if (m_os_write.hEvent)
	{
		CloseHandle(m_os_write.hEvent);
	}
	if (m_os_read.hEvent)
	{
		CloseHandle(m_os_read.hEvent);
	}

	/* create read and write event */
	m_os_write.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (m_os_write.hEvent == NULL)
	{
		return 2013032501;
	}
	m_os_read.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (m_os_read.hEvent == NULL)
	{
		return 2013032502;
	}

	/* open COM */
	m_h_com = CreateFile(m_str_port,					// com name
	                     GENERIC_WRITE | GENERIC_READ,					// read and write mode
	                     0,												// do not share
	                     NULL,											// file attribute
	                     OPEN_EXISTING,									// OPEN_EXISTING
	                     FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,	// overlapped
	                     NULL);	                                        // handle

	/* open com failure	*/
	if (m_h_com == INVALID_HANDLE_VALUE)
	{
		return 2013032503;
	}

	/* set input buffer and output buffer */
	b_setupComm = SetupComm(m_h_com, MAXBLOCK, MAXBLOCK);
	if (b_setupComm == FALSE)
	{
		CloseCOM();
		return 2013032504;
	}

	/* set communicate event */
	b_setCommMask = SetCommMask(m_h_com, EV_RXCHAR);
	if (b_setCommMask == FALSE)
	{
		CloseCOM();
		return 2013032505;
	}

	/* set timeout parameter*/
	st_timeOuts.ReadIntervalTimeout         = MAXTIMEOUTS;
	st_timeOuts.ReadTotalTimeoutMultiplier  = 1000;
	st_timeOuts.ReadTotalTimeoutConstant    = 1000;
	st_timeOuts.WriteTotalTimeoutMultiplier = 50;
	st_timeOuts.WriteTotalTimeoutMultiplier = 2000;

	/* set timeout */
	b_setCommTimeouts = SetCommTimeouts(m_h_com, &st_timeOuts);
	if (b_setCommTimeouts == FALSE)
	{
		CloseCOM();
		return 2013032506;
	}

	///* config com */
	//b_configComm = ConfigCOM();
	//if(b_configComm == false) {
	//	CloseCOM();
	//	return 2013032507;
	//}

	/* flush the port */
	b_purgeComm = PurgeComm(m_h_com, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	if (b_purgeComm == FALSE)
	{
		CloseCOM();
		return 2013032508;
	}

	/* set connected to TRUE */
	m_b_connected = TRUE;

	return 0;
}

void CCommunicate::CloseCOM(void)
{
	/* set m_b_connected */
	m_b_connected = FALSE;

	/* close the handle	*/
	CloseHandle(m_h_com);

	/* if read and write event handle is existent, close */
	if (m_os_write.hEvent)
	{
		CloseHandle(m_os_write.hEvent);
	}
	if (m_os_read.hEvent)
	{
		CloseHandle(m_os_read.hEvent);
	}

}

DWORD CCommunicate::WriteCOM(const BYTE* b_sendBuffer, DWORD dw_writeLen)
{
	BOOL b_writeResult;	                  // WriteFile result
	COMSTAT st_comStat;	                  // COMSTAT struct
	DWORD dw_errorFlags;                  // error flag
	DWORD dw_length;                      // write length
	BOOL b_clearCommError;                // clear comm error result
	DWORD dw_getLastError;                // get error no
	BOOL b_getOverlappedResult;           // wait result

	/* init the vary */
	b_writeResult = FALSE;
	dw_errorFlags = 0;
	dw_length = 0;
	b_clearCommError = 0;
	dw_getLastError = 0;
	b_getOverlappedResult = 0;
	if (b_sendBuffer == NULL)
		return 0;

	/* clear com error */
	b_clearCommError = ClearCommError(m_h_com, &dw_errorFlags, &st_comStat);
	if (b_clearCommError == FALSE)
		return 0;

	/* write data to com */
	b_writeResult = WriteFile(m_h_com, b_sendBuffer, dw_writeLen, &dw_length, &m_os_write);
	if (!b_writeResult)
	{
		dw_getLastError = GetLastError();
		if (dw_getLastError == ERROR_IO_PENDING)
		{
			/* overlapped operation */
			b_getOverlappedResult = GetOverlappedResult(m_h_com, &m_os_write, &dw_length, TRUE);
			if (b_getOverlappedResult == FALSE)
				return 0;
		}
		else
		{
			dw_length = 0;
		}
	}

	BOOL b_purgeComm;
	b_purgeComm = FALSE;
	/* flush the port */
	b_purgeComm = PurgeComm(m_h_com, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	if (b_purgeComm == FALSE)
	{
		CloseCOM();
		return 0;
	}

	/* return write length */
	return dw_length ;
}

DWORD CCommunicate::ReadCOM(char* b_ReadBuffer, DWORD dw_ReadLen)
{
	BOOL b_readResult;	                  // WriteFile result
	COMSTAT st_comStat;	                  // COMSTAT struct
	DWORD dw_errorFlags;                  // error flag
	DWORD dw_length;                      // write length
	BOOL b_clearCommError;                // clear comm error result
	DWORD dw_getLastError;                // get error no
	BOOL b_getOverlappedResult;           // wait result
	DWORD dwRes;
	BOOL b_purgeComm;

	/* init the vary */
	b_readResult = FALSE;
	dw_errorFlags = 0;
	dw_length = 0;
	b_clearCommError = 0;
	dw_getLastError = 0;
	b_getOverlappedResult = 0;
	dwRes = 0;
	dw_length = dw_ReadLen;

	/* clear com error */
	b_clearCommError = ClearCommError(m_h_com, &dw_errorFlags, &st_comStat);
	if (b_clearCommError == FALSE)
	{
		return 2013031901;
	}

	/* read data from com */
	b_readResult = ReadFile(m_h_com, b_ReadBuffer, dw_ReadLen, &dw_ReadLen, &m_os_read);
	if (!b_readResult)
	{
		dw_getLastError = GetLastError();
		if (dw_getLastError == ERROR_IO_PENDING)
		{
			dwRes = WaitForSingleObject(m_os_read.hEvent, 5000);/*1000:  delay time*/
			switch (dwRes)
			{
			// Read completed.
			case WAIT_OBJECT_0:
				// Error in communications; report it.
				/* overlapped operation */
				b_getOverlappedResult = GetOverlappedResult(m_h_com, &m_os_read, &dw_ReadLen, FALSE);
				if (b_getOverlappedResult == FALSE)
				{
					return 2013031902;
				}
				else
				{
					/*read OK*/
					char cs_readData[10] = {0};
					int nBufferLength = 0 ;
					nBufferLength = strlen(b_ReadBuffer);
					if (nBufferLength != 0) //make sure data not empty
					{
						strncpy(cs_readData, b_ReadBuffer, dw_ReadLen);
						CString CS_buffer = cs_readData;
						return 0;
					}
				}
				break;
			}/*end switch*/
		}
		else
		{
			dw_length = 0;
		}
	}/*end of b_readResult*/

	b_purgeComm = false;
	/* flush the port */
	b_purgeComm = PurgeComm(m_h_com, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	if (b_purgeComm == FALSE)
	{
		CloseCOM();
		return 2013031904;
	}

	/* return read length */
	return 0 ;
}

int CCommunicate::SendCommand(const BYTE* b_command, DWORD dw_writeLen, DWORD dr_readLen)
{
	int n_bRes = -1;/* 0 = PASS */
	if (b_command == NULL)
	{
		return 2003031330;
	}

	if (OpenCOM() != 0)
	{
		m_str_errMsg = _T("Can't open COM");
		return 2003031331;
	}

	if (WriteCOM(b_command, dw_writeLen) != dw_writeLen) /* 0 = write fail, */
	{
		m_str_errMsg = _T("Error in Send data to COM");
		return 2003031332;
	}

	DWORD d_readResult;
	char c_ReadBuffer[10];
	for (int i = 0 ; i < 2 ; i ++)
	{
		d_readResult = ReadCOM(c_ReadBuffer, dr_readLen);
		if (d_readResult == 0)
		{
			int abc = 0;
			break;
		}
		else
		{
			/* 0 = read fail, */
			m_str_errMsg = _T("Error in Send data to COM");
		}
	}
	if (d_readResult == 0)
		return d_readResult;  /*failed*/
	CloseCOM();
	return 0;
}

CString CCommunicate::getErrCode()
{
	return m_str_errCode;
}

CString CCommunicate::getErrMsg()
{
	return m_str_errMsg;
}
