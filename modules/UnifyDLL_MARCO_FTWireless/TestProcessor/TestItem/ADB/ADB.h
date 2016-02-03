/*********************************************************************************
 * COPYRIGHT Qisda WGC18 2011
 *
 * Description: .h file of CTouchPanelProc object.
 * FileName   : CTouchPanelProc.h
 * Version    : $Revision:   1.2  $
 * Author     : Gilpin.Xi
 * Time       : 2011/04/18
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/04/18      Gilpin.Xi             Create.
 ********************************************************************************/

#ifndef _C_ADB_Proc_H_
#define _C_ADB_Proc_H_

/* Name */


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class ADB : public ITestItem
{
RF_DECLARE_DYNCREATE(ADB)

// Data members
public:

protected:

private:
	//parameters
	std::string m_str_Name;

	int m_iTimeOut;

	CWinThread *ThreadHandle;
	bool m_bStopThread;
	bool m_bStopTouchTest;
	bool m_bStopViewImg;


	std::string m_str_Value;
	CRangePair m_rp_Value;
	bool m_b_CheckMatch;
	int m_i_sleep;
	char *sz_sectorData;// = new char[4096];

// Member functions
public:
	ADB();
	~ADB();

	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool InitData();

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

    static UINT AfxViewImgDlgThread(LPVOID pParam);
    static UINT AfxTouchLineDlgThread(LPVOID pParam);

private:
	void Report(bool bRes, string strErrorCode = "", string strMessage = "");
	bool MyExecAdbCmd(const char* chCmd, char **chReturn = NULL, bool bWithShell = true);
	HANDLE         m_h_com;	
	DWORD          m_dw_baud;	
	INT            m_i_dataBits;	
	INT            m_i_parity;					
	INT            m_i_stopBits;			
	INT            m_i_flowCtrl;		
	CString        m_str_port;
	OVERLAPPED     m_os_write;	
	OVERLAPPED     m_os_read;
	BOOL           m_b_connected;

	/**/
	const int nWaitResultTime;

	//bool Detroit_WirteADB();
	// Detroit_ReadOTP();

};

#endif // End of #ifndef _C_Touch_Panel_Proc_H_
