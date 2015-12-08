#include "StdAfx.h"
#include "ComComm.h"

CComComm::CComComm(void)
{				
	m_dw_baud         = CBR_115200;			// (default)baud 57600		
	m_i_dataBits      = 8;					// (default)data bit 8			
	m_i_parity        = 0;					// (default)parity 0		
	m_i_stopBits      = 1;			        // (default)stop bit 1	
	m_i_flowCtrl      = 0;					// (default)flow ctrl
	m_h_com           = NULL;
	m_os_write.hEvent = NULL;
	m_os_read.hEvent  = NULL;
	m_b_connected     = FALSE;	
	m_str_port        = _T("");
}

CComComm::~CComComm(void)
{
	/* Close Com port */
	if(m_b_connected) {							
		CloseComm();
	}

	/* Close the handle	*/
	if(m_h_com) {
		CloseHandle(m_h_com);
	}
	if(m_os_write.hEvent) {					
		CloseHandle(m_os_write.hEvent);
	}
	if(m_os_read.hEvent) {							
		CloseHandle(m_os_read.hEvent);
	}

}

/**
 * Function   : SetComm
 * Description: Set COM port and Band
 * Param[in]  :
 *         (1). CString str_comPort 
 *         (2). DWORD dw_baud 
 * Param[out] : 
 *      
 * Return     : 
 *                   
 * Note       : 
 *
 */
void CComComm::SetComm(CString str_comPort, DWORD dw_baud)
{
	m_str_port = str_comPort;
	m_dw_baud  = dw_baud;
}


/**
 * Function   : OpenComm
 * Description: Open COM port.
 * Param[in]  :
 * 
 * Param[out] : 
 *      
 * Return     : 
 *              TRUE: Open succeed.
 *              FALSE:Open failure.
 * Note       : 
 *
 */
BOOL CComComm::OpenComm()
{
	COMMTIMEOUTS st_timeOuts;			// timeout
	BOOL b_setupComm;                   // set up comm
	BOOL b_setCommMask;                 // set comm mask
	BOOL b_setCommTimeouts;             // set comm timeout
	BOOL b_configComm;                  // config comm
	BOOL b_purgeComm;                   // purge comm

	/* Init variable */
	b_setupComm       = FALSE;
	b_setCommMask     = FALSE;
	b_setCommTimeouts = FALSE;
	b_configComm      = FALSE;
	b_purgeComm       = FALSE;

	/* Check connected */
	if(m_b_connected) {									
		return FALSE;
	}

	/* Init overlapped read and write */
	memset(&m_os_write, 0, sizeof(m_os_write));
	memset(&m_os_read, 0, sizeof(m_os_read));

	/* If read and write event handle is existent, close */
	if(m_os_write.hEvent) {					
		CloseHandle(m_os_write.hEvent);
	}
	if(m_os_read.hEvent) {							
		CloseHandle(m_os_read.hEvent);
	}
	
	/* Create read and write event */
	m_os_write.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	if(m_os_write.hEvent == NULL) {
		return FALSE;
	}
	m_os_read.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	if(m_os_read.hEvent == NULL) {
		return FALSE;
	}

	/* Open COM */
	m_h_com = CreateFile(m_str_port,					                // com name
		                 GENERIC_WRITE | GENERIC_READ,	                // read and write mode				
		                 0,								                // do not share				
		                 NULL,							                // file attribute	
		                 OPEN_EXISTING,					                // OPEN_EXISTING
		                 FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,	// overlapped 				
		                 NULL);	                                        // handle
	
	/* Open com failure	*/
	if(m_h_com == INVALID_HANDLE_VALUE){
		CloseComm();
		return FALSE;
	}

	/* Set input buffer and output buffer */
	b_setupComm = SetupComm(m_h_com, MAXBLOCK, MAXBLOCK);
	if(b_setupComm == FALSE) {
		CloseComm();
		return FALSE;
	}
	
	/* Set communicate event */
	b_setCommMask = SetCommMask(m_h_com, EV_RXCHAR);
	if(b_setCommMask == FALSE) {
		CloseComm();
		return FALSE;
	}

	/* Set timeout parameter*/
	st_timeOuts.ReadIntervalTimeout         = MAXTIMEOUTS;
	st_timeOuts.ReadTotalTimeoutMultiplier  = 1000;
	st_timeOuts.ReadTotalTimeoutConstant    = 1000;
	st_timeOuts.WriteTotalTimeoutMultiplier = 1000;
	st_timeOuts.WriteTotalTimeoutConstant   = 5000;
	b_setCommTimeouts = SetCommTimeouts(m_h_com, &st_timeOuts);
	if(b_setCommTimeouts == FALSE) {
		CloseComm();
		return FALSE;
	}

	/* Config com */
	b_configComm = ConfigComm();
	if(b_configComm == FALSE) {							
		CloseComm();
		return FALSE;
	}
	
	/* Flush the port */ 
    b_purgeComm = PurgeComm(m_h_com, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT); 
	if(b_purgeComm == FALSE) {
		CloseComm();
		return FALSE;
	}

	/* Set connected to TRUE */
	m_b_connected = TRUE;

	return TRUE;
}


/**
 * Function   : CloseComm
 * Description: Close COM port.
 * Param[in]  :
 * 
 * Param[out] : 
 *      
 * Return     : 
 * 
 * Note       : 
 *
 */
void CComComm::CloseComm()
{
	/* Set m_b_connected */
	m_b_connected = FALSE;
	
	/* Close the handle	*/
	if(m_h_com) {
		CloseHandle(m_h_com);
		m_h_com = NULL;
	}
	if(m_os_write.hEvent) {					
		CloseHandle(m_os_write.hEvent);
		m_os_write.hEvent = NULL;
	}
	if(m_os_read.hEvent) {							
		CloseHandle(m_os_read.hEvent);
		m_os_read.hEvent = NULL;
	}

}


/**
 * Function   : IsConnected
 * Description: Check COM port if connected.
 * Param[in]  :
 * 
 * Param[out] : 
 *      
 * Return     : 
 *              TRUE: Connected.
 *              FALSE:Not connected.
 * Note       : 
 *
 */
BOOL CComComm::IsConnected()
{
	return m_b_connected;
}


/**
 * Function   : WriteComm
 * Description: Write data to COM.
 * Param[in]  :
 *              p_str_write : Data
 *              dw_writeLen : Data length
 * Param[out] : 
 *      
 * Return     : 
 *              Writed data numbers.
 * Note       : 
 *
 */
DWORD CComComm::WriteComm(const char* p_str_write, DWORD dw_writeLen)
{
	BOOL b_writeResult;	                  // WriteFile result						
	COMSTAT st_comStat;	                  // COMSTAT struct			
	DWORD dw_errorFlags;                  // error flag
	DWORD dw_length;                      // write length
	BOOL b_clearCommError;                // clear comm error result
	DWORD dw_getLastError;                // get error no 
	BOOL b_getOverlappedResult;           // wait result

	/* Init the vary */
	b_writeResult = FALSE;
	dw_errorFlags = 0;
	dw_length = 0;
	b_clearCommError = 0;
	dw_getLastError = 0;
	b_getOverlappedResult = 0;
	
	/* Check input param */
	if(p_str_write == NULL) {
		return 0;
	}

	/* Clear com error */
	b_clearCommError = ClearCommError(m_h_com, &dw_errorFlags, &st_comStat);
	if(b_clearCommError == FALSE) {
		return 0;
	}

	/* Write data to com */
	b_writeResult = WriteFile(m_h_com, p_str_write, dw_writeLen, &dw_length, &m_os_write);
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

	/* Flush the port */ 
	BOOL b_purgeComm;
	b_purgeComm = FALSE;
    b_purgeComm = PurgeComm(m_h_com, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT); 
	if(b_purgeComm == FALSE) {
		CloseComm();
		return 0;
	}

	/* Return write length */
	return dw_length ;			
}


/**
 * Function   : ReadComm
 * Description: Read data from COM.
 * Param[in]  :
 *              p_str_read : Data
 *              dw_readLen : Data length
 * Param[out] : 
 *      
 * Return     : 
 *              Readed data numbers.
 * Note       : 
 *
 */
DWORD CComComm::ReadComm(char* p_str_read, DWORD dw_readLen)
{
	BOOL  b_readFileResult;	              // ReadFile result				
	DWORD dw_length;	                  // read length	
	COMSTAT st_comStat;	                  // COMSTAT struct			
	DWORD dw_errorFlags;                  // error flag
	BOOL b_clearCommError;                // clear comm error result
	DWORD dw_getLastError;                // get error no 
	BOOL b_getOverlappedResult;           // wait result

	/* Init the vary */
	b_readFileResult = FALSE;
	b_clearCommError = FALSE;
	b_getOverlappedResult = FALSE;
	dw_length = 0;
	dw_errorFlags = 0;
	dw_getLastError = 0;
	
	dw_length = dw_readLen;

	/* Clear com error */
	b_clearCommError = ClearCommError(m_h_com,&dw_errorFlags,&st_comStat);
	if(b_clearCommError == FALSE) {
		return 0;
	}

	/* Get the byte length of input buffer */
	dw_length = min(dw_readLen, st_comStat.cbInQue) ;
	if(dw_length > 0) {

		/* Read data from com */
		b_readFileResult = ReadFile(m_h_com,p_str_read,dw_length,&dw_length,&m_os_read);
		if(!b_readFileResult) {
			dw_getLastError = GetLastError();
			if(dw_getLastError == ERROR_IO_PENDING) {
				
				/* overlapped operation */
				b_getOverlappedResult = GetOverlappedResult(m_h_com,&m_os_read,&dw_length,TRUE);
				if(b_getOverlappedResult == FALSE) {
					return 0;
				}
			} else {
				dw_length = 0;
			}
		} 
	} 

	/* Return read length */
	return dw_length ;	
}


/**
 * Function   : ConfigComm
 * Description: Check COM port if connected.
 * Param[in]  :
 * 
 * Param[out] : 
 *      
 * Return     : 
 *              TRUE: Succeed.
 *              FALSE:Failure.
 * Note       : 
 *
 */
BOOL CComComm::ConfigComm()
{
	DCB st_dcb;	                         // DCB struct
	BOOL b_getCommState;                 // get comm state
	BOOL b_setCommState;                 // set comm state

	b_getCommState = FALSE;
	b_setCommState = FALSE;

	/* Get the config of DCB */
	b_getCommState = GetCommState(m_h_com,&st_dcb);
	if(b_getCommState == FALSE) {				
		return FALSE;
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

	/* Set comm state */
	b_setCommState = SetCommState(m_h_com,&st_dcb);
	if(b_setCommState == FALSE) {
		return FALSE;
	}

	return TRUE;
}


BOOL CComComm::OpenLPT()
{
	/* Check connected */
	if(m_b_connected) {									
		return FALSE;
	}

	/* Open LPT */
	m_h_com = CreateFile(m_str_port,					                // com name
		                 GENERIC_WRITE | GENERIC_READ,	                // read and write mode				
		                 0,								                // do not share				
		                 NULL,							                // file attribute	
		                 OPEN_EXISTING,					                // OPEN_EXISTING
		                 FILE_ATTRIBUTE_NORMAL,	                        // normal 				
		                 NULL);	                                        // handle
	
	/* Open com failure	*/
	if(m_h_com == INVALID_HANDLE_VALUE){
		return FALSE;
	}

	/* Set timeout parameter*/
	COMMTIMEOUTS st_timeOuts;
	st_timeOuts.ReadIntervalTimeout         = MAXTIMEOUTS;
	st_timeOuts.ReadTotalTimeoutMultiplier  = 1000;
	st_timeOuts.ReadTotalTimeoutConstant    = 1000;
	st_timeOuts.WriteTotalTimeoutMultiplier = 1000;
	st_timeOuts.WriteTotalTimeoutConstant   = 5000;

	BOOL b_setCommTimeouts = FALSE;
	b_setCommTimeouts = SetCommTimeouts(m_h_com, &st_timeOuts);
	if(b_setCommTimeouts == FALSE) {
		CloseComm();
		return FALSE;
	}

	/* Set connected to TRUE */
	m_b_connected = TRUE;

	return TRUE;
}


DWORD CComComm::WriteLPT(const char* p_str_write, DWORD dw_writeLen)
{						
	COMSTAT st_comStat;	                  // COMSTAT struct			
	DWORD dw_errorFlags;                  // error flag
	DWORD dw_length;                      // write length
	BOOL b_writeResult;

	dw_errorFlags = 0;
	dw_length = 0;
	b_writeResult = FALSE;

	/* Check input param */
	if(p_str_write == NULL) {
		return 0;
	}

	/* Clear com error */
	ClearCommError(m_h_com, &dw_errorFlags, &st_comStat);

	/* Write data to com */
	b_writeResult = ::WriteFile(m_h_com, p_str_write, dw_writeLen, &dw_length, NULL);
	if(!b_writeResult) {
		dw_writeLen = 0;
	}

	return dw_length ;			
}