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

#ifndef _C_OTP_Proc_H_
#define _C_OTP_Proc_H_

/* Name */


#include "../../../CommonUtility/BaseObject/ITestItem.h"

// CSV Mapping Table ITEM Define
#define     _MAPPING_ITEM_INDEX         0
#define     _MAPPING_ITEM_IMEI          1
#define     _MAPPING_ITEM_BT            3
#define     _MAPPING_ITEM_WIFI          4
#define     _MAPPING_ITEM_PVKID         5
#define     _MAPPING_ITEM_PVKFILENAME   6
#define     _MAPPING_ITEM_FSN           7
#define     _MAPPING_ITEM_NWCODE        8
#define     _MAPPING_ITEM_RSA           9
#define     _MAPPING_ITEM_LOC          10



class OTP : public ITestItem
{
RF_DECLARE_DYNCREATE(OTP)

// Data members
public:

protected:

private:
	//parameters
	std::string m_str_Name;
	std::string m_str_Name_IMEI;
	std::string m_str_Name_BT;
	std::string m_str_Name_WIFI;
	std::string m_str_Name_HaierSN;

	CWinThread *ThreadHandle;
	bool m_bStopThread;
	bool m_bStopTouchTest;
	bool m_bStopViewImg;
	//static UINT staticboolStopViewImg;

	std::string m_str_Value;
	CRangePair m_rp_Value;
	bool m_b_CheckMatch;
	int m_i_sleep;

// Member functions
public:
	OTP();
	~OTP();

	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool InitData();

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

    static UINT AfxViewImgDlgThread(LPVOID pParam);
    static UINT AfxTouchLineDlgThread(LPVOID pParam);

private:
	void Report(bool bRes, string strErrorCode = "", string strMessage = "");
	HANDLE m_h_com;
	DWORD  m_dw_baud;
	INT m_i_dataBits;
	INT m_i_parity;
	INT m_i_stopBits;
	INT m_i_flowCtrl;
	CString    m_str_port;
	OVERLAPPED m_os_write;
	OVERLAPPED m_os_read;
	BOOL       m_b_connected;

	/* read csv*/
	CString m_strPort;    //read csv
	CString m_strINICSVFilename ;  //read csv
    CString m_strIMEI ;
    CString m_strBTAdd ;
    CString m_strWIFIAdd ;
	CString m_str_HaierSN;
	CString m_strINILocPath ;
	/**/
	const int nWaitResultTime;

	bool FTD_WirteIMEI(char *szParam);
	bool FTD_WirteBT(char *szParam);
	bool FTD_WirteWIFI(char *szParam);
	bool FTD_WirteHaierSN(char *szParam);
	bool FTD_WirteSERIALNO(char *szParam);
	bool FTD_WirteQLOCK(char *szParam);

	bool FTD_ReadIMEI(char *szParam, char * szData);
	bool FTD_ReadBT(char *szParam, char * szData);
	bool FTD_ReadWIFI(char *szParam, char * szData);
	bool FTD_ReadSERIALNO(char *szParam, char * szData);
	bool FTD_ReadQLOCK(char *szParam, char * szData);

	bool FTD_WirteOTP();
	bool FTD_ReadOTP();
};

#endif // End of #ifndef _C_Touch_Panel_Proc_H_
