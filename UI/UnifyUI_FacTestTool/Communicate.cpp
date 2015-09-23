#include "StdAfx.h"
#include "Communicate.h"

CCommunicate::CCommunicate(void)
{
	m_str_port = _T("");
}

CCommunicate::~CCommunicate(void)
{
}

bool CCommunicate::SetCOM(CString str_COMPort)
{
	if (str_COMPort == "") {
		return false;
	}

	m_b_connected     = FALSE;
	m_dw_baud         = CBR_9600;	
	m_i_dataBits      = 8;		
	m_i_parity        = 0;	
	m_i_stopBits      = 1;
	m_i_flowCtrl      = 0;
	m_str_port        = str_COMPort;
	m_h_com           = NULL;
	m_os_write.hEvent = NULL;
	m_os_read.hEvent  = NULL;

	return true;
}

bool CCommunicate::OpenCOM(void)
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

	/* check connected */
	if(m_b_connected) {									
		return false;
	}

	/* init overlapped read and write */
	memset(&m_os_write,0,sizeof(m_os_write));
	memset(&m_os_read,0,sizeof(m_os_read));

	/* if read and write event handle is existent, close */
	if(m_os_write.hEvent) {					
		CloseHandle(m_os_write.hEvent);
	}
	if(m_os_read.hEvent) {							
		CloseHandle(m_os_read.hEvent);
	}

	/* create read and write event */
	m_os_write.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	if(m_os_write.hEvent == NULL) {
		return false;
	}
	m_os_read.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	if(m_os_read.hEvent == NULL) {
		return false;
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
	if(m_h_com == INVALID_HANDLE_VALUE){				
		return false;
	}

	/* set input buffer and output buffer */
	b_setupComm = SetupComm(m_h_com,MAXBLOCK,MAXBLOCK);
	if(b_setupComm == FALSE) {
		CloseCOM();
		return false;
	}

	/* set communicate event */
	b_setCommMask = SetCommMask(m_h_com,EV_RXCHAR);
	if(b_setCommMask == FALSE) {
		CloseCOM();
		return false;
	}

	/* set timeout parameter*/
	st_timeOuts.ReadIntervalTimeout         = MAXTIMEOUTS;
	st_timeOuts.ReadTotalTimeoutMultiplier  = 1000;
	st_timeOuts.ReadTotalTimeoutConstant    = 1000;
	st_timeOuts.WriteTotalTimeoutMultiplier = 50;
	st_timeOuts.WriteTotalTimeoutMultiplier = 2000;

	/* set timeout */
	b_setCommTimeouts = SetCommTimeouts(m_h_com,&st_timeOuts);
	if(b_setCommTimeouts == FALSE) {
		CloseCOM();
		return false;
	}

	/* config com */
	b_configComm = ConfigCOM();
	if(b_configComm == false) {							
		CloseCOM();
		return false;
	}

	/* flush the port */ 
	b_purgeComm = PurgeComm(m_h_com, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT); 
	if(b_purgeComm == FALSE) {
		CloseCOM();
		return false;
	}

	/* set connected to TRUE */
	m_b_connected = TRUE;

	return true;
}

void CCommunicate::CloseCOM(void)
{
	/* set m_b_connected */
	m_b_connected = FALSE;

	/* close the handle	*/
	CloseHandle(m_h_com);

	/* if read and write event handle is existent, close */
	if(m_os_write.hEvent) {					
		CloseHandle(m_os_write.hEvent);
	}
	if(m_os_read.hEvent) {							
		CloseHandle(m_os_read.hEvent);
	}

}

bool CCommunicate::ConfigCOM(void)
{
	DCB st_dcb;	                         // DCB struct
	BOOL b_getCommState;                 // get comm state
	BOOL b_setCommState;                 // set comm state

	b_getCommState = FALSE;
	b_setCommState = FALSE;

	/* get the config of DCB */
	b_getCommState = GetCommState(m_h_com,&st_dcb);
	if(b_getCommState == FALSE) {				
		return false;
	}
	st_dcb.fBinary  = TRUE;					// WIN32 (TRUE)
	st_dcb.BaudRate = m_dw_baud;			// baud
	st_dcb.ByteSize = m_i_dataBits;			// byte size
	st_dcb.fParity  = TRUE;					// parity
	st_dcb.Parity   = m_i_parity;		    // parity
	st_dcb.StopBits = 0;			        // stop bit
	switch(m_i_flowCtrl) {					// flow control
	case 0:
		/* default 0 */
		st_dcb.fOutxCtsFlow = FALSE;		
		st_dcb.fRtsControl = FALSE;			
		st_dcb.fInX = st_dcb.fOutX = FALSE;	
		break;
	case 1:
		st_dcb.fOutxCtsFlow = TRUE;			
		st_dcb.fRtsControl = TRUE;
		st_dcb.fInX = st_dcb.fOutX = FALSE;
		break;
	case 2:
		st_dcb.fOutxCtsFlow = FALSE;
		st_dcb.fRtsControl = FALSE;
		st_dcb.fInX = st_dcb.fOutX = TRUE;
		st_dcb.XonChar = XON;				
		st_dcb.XoffChar = XOFF;				
		st_dcb.XonLim = 50;					
		st_dcb.XoffLim = 50;				
		break;
	default:
		break;
	}

	/* set comm state */
	b_setCommState = SetCommState(m_h_com,&st_dcb);
	if(b_setCommState == FALSE) {
		return false;
	}

	return true;			
}

DWORD CCommunicate::WriteCOM(const BYTE *b_sendBuffer, DWORD dw_writeLen)
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

	//	dw_length = dw_writeLen;

	if(b_sendBuffer == NULL) {
		return 0;
	}

	/* clear com error */
	b_clearCommError = ClearCommError(m_h_com,&dw_errorFlags,&st_comStat);
	if(b_clearCommError == FALSE) {
		return 0;
	}

	/* write data to com */
	b_writeResult = WriteFile(m_h_com,b_sendBuffer,dw_writeLen,&dw_length,&m_os_write);
	if(!b_writeResult) {
		dw_getLastError = GetLastError();
		if(dw_getLastError == ERROR_IO_PENDING) {

			/* overlapped operation */
			b_getOverlappedResult = GetOverlappedResult(m_h_com,&m_os_write,&dw_length,TRUE);
			if(b_getOverlappedResult == FALSE) {
				return 0;
			}
		}
		else {
			dw_length = 0;
		}
	}

	BOOL b_purgeComm;
	b_purgeComm = FALSE;
	/* flush the port */ 
	b_purgeComm = PurgeComm(m_h_com, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT); 
	if(b_purgeComm == FALSE) {
		CloseCOM();
		return 0;
	}

	/* return write length */
	return dw_length ;	
}

bool CCommunicate::SendCommand(const BYTE* b_command, DWORD dw_writeLen)
{
	if(b_command == NULL) 
	{
		return false;
	}

	if (OpenCOM() != true) 
	{
		m_str_errCode = CommErr_UI_Jig_Com_Open_Fail;
		m_str_errMsg = _T("Can't open COM");
		return false;
	}

	if (WriteCOM(b_command, dw_writeLen) != dw_writeLen) 
	{
		m_str_errCode = CommErr_UI_Jig_Data_Send_Fail;
		m_str_errMsg = _T("Error in Send data to COM");
		return false;
	}

	CloseCOM();

	return true;
}

CString CCommunicate::getErrCode()
{
	return m_str_errCode;
}

CString CCommunicate::getErrMsg()
{
	return m_str_errMsg;
}