/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 * Author     : Lion.Wu
 ********************************************************************************/

#ifndef _C_MONITOR_PROC_H_
#define _C_MONITOR_PROC_H_


//#include "ITestItem.h"
#define FA_SIZE         512
//#include "MonitorRuncard.h"
//#include "MonitorSo.h"
//#include "MonitorPartNo.h"
//#include "MonitorWbcModel.h"
#include "MonitorId.h"
#include "CommonDefine.h"
#include <direct.h>

/* ID Type */

#define ReadScalarID				"ReadScalarID"
#define GetHDCPKEY					"GetHDCPKEY"
#define CheckPCBAID					"CheckPCBAID"
#define CheckFlow					"CheckFlow"
#define CheckAllFlow				"CheckAllFlow"
#define InsertData					"InsertData"
#define WriteHDCP					"WriteHDCP"
#define WriteTag					"WriteTag"

#define ADB_CMD_TIMEOUT			25000
#define ID_SIZE								11
#define i_Station_Count					10
class CMonitor 
{
protected:
private:
	//parameters
	std::string m_str_TestItem;
	std::string m_str_Delay;
	std::string m_str_CMD;
	std::string m_str_OffCMD;
	std::string str_msg;

	std::string str_Pics ;

	int n_Delay;
	int m_i_SleepTime;
	//int m_i_OffSleepTime;
	int m_i_retryTime;
	int m_i_retriedTime;
	int m_i_MexRetryTime;

	std::string st_Test_Flow[i_Station_Count];
	std::string ErrMsg;
	std::string	 g_strScalarID;
	bool b_res_on, b_res_off ;
	bool b_res_msg_try, b_res_msg_retry;
	char sz_local_file_path[MAX_PATH];
	char sz_remote_file_path[MAX_PATH];
	CString cs_local_key_path;

// Member functions
public:
	CMonitor() {
		st_Test_Flow[0] = _T("BBTEST") ;
		st_Test_Flow[1] = _T("BTWLAN") ;
		st_Test_Flow[2] = _T("MMI") ;
		st_Test_Flow[3] = _T("WLS") ;
		st_Test_Flow[4] = _T("RUN_IN") ;
		st_Test_Flow[5] = _T("OS_DL") ;
		st_Test_Flow[6] = _T("WRITE") ;

	//	sprintf_s((char*)sz_ID, ID_SIZE_BUFFER, "", "");
	////	sz_ID[ID_SIZE_BUFFER] = "";
	//	sprintf_s((char*)szStation, ID_SIZE_BUFFER, "", "");
	//	szStation[ID_SIZE_BUFFER] ={0;}
	};
	~CMonitor() {}


public:
	CMonitorId Id;

protected:
private:
	std::string std_key_path;
	std::string std_Key_Id;
	std::string std_Key_Name;
	std::string std_ScalarId ;
	std::string szPcabaId;
	std::string checkStation;
	std::string sz_Hdcp_key_path;

	bool runInsertData(int i_type);
	bool runCheckFlow( int i_type);// i_type 1 = pcbaid,  2 = scalarId
	bool runCheckFlowAllStation( int i_type);// i_type 1 = pcbaid,  2 = scalarId
	bool runReadScalarID( char *szvalue, int iSize );

	bool getWeek(void);
	int GetWeek	(struct tm* date	);
	int GetCurrentWeek();

//	bool ExecAdbOut(CString Command, char* output, char* ErrorCode);
};


#endif // End of #ifndef _C_GSensor_Test_Proc_H_
