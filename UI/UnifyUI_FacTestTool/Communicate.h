
#pragma once

#include "CommDef.h"

/*======================================== Const ==========================*/
#define MAXBLOCK		   4096	        // input buffer and out buffer
#define MAXTIMEOUTS		   2000	        // timeout(ms)
#define EVENTCHAR          0x0d	        // event char			  
#define XON			       0x11	        // DCB XonChar		
#define XOFF			   0x13	        // DCB XoffChar
/*======================================== Const ==========================*/

class CCommunicate
{
public:
	CCommunicate(void);
	~CCommunicate(void);

	bool SendCommand(const BYTE* b_command, DWORD dw_writeLen);
	bool SetCOM(CString str_COMPort);

	CString getErrCode();
	CString getErrMsg();
private:	
	bool OpenCOM(void);
	void CloseCOM(void);
	bool ConfigCOM(void);
	DWORD WriteCOM(const BYTE *b_sendBuffer, DWORD dw_writeLen);

private:
	HANDLE         m_h_com;
	DWORD          m_dw_baud;
	INT            m_i_dataBits;
	INT            m_i_parity;					
	INT            m_i_stopBits;			
	INT            m_i_flowCtrl;		
	OVERLAPPED     m_os_write;	
	OVERLAPPED     m_os_read;
	CString        m_str_port;
	BOOL           m_b_connected;

	CString        m_str_errCode;
	CString        m_str_errMsg;
};
