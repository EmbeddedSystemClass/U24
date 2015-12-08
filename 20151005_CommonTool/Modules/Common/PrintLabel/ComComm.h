/**
 *  COM Port communication.
 */

#pragma once

#define MAXBLOCK		4096	// input buffer and out buffer
#define MAXTIMEOUTS		2000	// timeout(ms)
#define EVENTCHAR       0x0d	// event char			  
#define XON			    0x11	// DCB XonChar		
#define XOFF			0x13	// DCB XoffChar

class CComComm
{
public:
	CComComm(void);
	virtual ~CComComm(void);

public:
	/* COM */
	void  SetComm(CString str_comPort, DWORD dw_baud = CBR_115200); // Set Com port and Baud
	BOOL  OpenComm();                                               // Open Com port
	void  CloseComm();                                              // Close Com port
	BOOL  IsConnected();                                            // Check Com port connecttion
	DWORD WriteComm(const char* p_str_write, DWORD dw_writeLen);    // Write Com port
	DWORD ReadComm(char* p_str_read, DWORD dw_readLen);             // Read Com port

public:
	/* LPT */
	BOOL  OpenLPT();                                                // Open LPT1 port
	DWORD WriteLPT(const char* p_str_write, DWORD dw_writeLen);     // Write LPT1 port


private:
	BOOL ConfigComm();                                             // Config Com port communication param

private:
	HANDLE      m_h_com;				// Com Port handle	
	DWORD       m_dw_baud;				// Band rate	
	INT         m_i_dataBits;			// Data bits	
	INT         m_i_parity;				// Parity					
	INT         m_i_stopBits;			// Stop bits			
	INT         m_i_flowCtrl;			// Flow control			
	OVERLAPPED  m_os_write;				// overlapped write		
	OVERLAPPED  m_os_read;              // overlapped read
	CString     m_str_port;				// Com Port
	BOOL        m_b_connected;			// Connected flag

};
