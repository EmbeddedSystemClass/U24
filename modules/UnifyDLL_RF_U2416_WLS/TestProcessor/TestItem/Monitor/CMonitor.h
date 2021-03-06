/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 * Author     : Lion.Wu
 ********************************************************************************/

#ifndef _C_MONITOR_PROC_H_
#define _C_MONITOR_PROC_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"
#define FA_SIZE         512
//#include "MonitorRuncard.h"
#include "MonitorSo.h"
#include "MonitorPartNo.h"
#include "MonitorWbcModel.h"
#include "MonitorId.h"
#include "../../../CommonUtility/CommonDefine.h"
#include "io.h"

/* ID Type */
#define BATTERY						"Battery"
#define ReadScalarID					"ReadScalarID"
#define GetHDCPKEY					"GetHDCPKEY"
#define CheckPCBAID					"CheckPCBAID"
#define CheckFlow					"CheckFlow"
#define CheckAllFlow					"CheckAllFlow"
#define CheckModel					"CheckModel"
#define CheckPartNo					"CheckPartNo"
#define InsertData					"InsertData"


#define ADB_CMD_TIMEOUT			25000
#define ID_SIZE								11
#define i_Station_Count					10
class CMonitor : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CMonitor)


// Data members
public:
protected:
private:
	//parameters
	std::string m_str_TestItem;
	std::string m_str_Delay;
	std::string m_str_CMD;
	std::string m_str_OffCMD;
	std::string str_msg;

	std::string m_szId;
	std::string m_szErrMsg;
	std::string m_strItemCode;
	std::string m_strErrorCode;
	std::string m_strResult;
	std::string m_strMessage;
	
	std::string str_Pics ;

	int n_Delay;
	int m_i_SleepTime;
	//int m_i_OffSleepTime;
	int m_i_retryTime;
	int m_i_retriedTime;
	int m_i_MexRetryTime;

	std::string st_Test_Flow[i_Station_Count];
	std::string ErrMsg;

	bool b_res_on, b_res_off ;
	bool b_res_msg_try, b_res_msg_retry;

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

	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool Run();
	virtual bool PostRun();

public:
	CMonitorId Id;
	CMonitorSo So;
	CMonitorPartNo PartNo;
	CMonitorWbcModel WbcFileName;

protected:
private:
	std::string s_newKey;
	std::string szScalarId ;
	std::string szPcabaId;
	std::string checkStation;
	std::string m_szPartNo;
	std::string m_ModelName;

	//unsigned char sz_ID[ID_SIZE_BUFFER] ;
	//unsigned char szStation[ID_SIZE_BUFFER];
	
	//bool runReadScalarID( char *szvalue, int iSize );


	bool runGetHDCPKEY();
	bool bGetNewHDCPKEY(char *scalarID);
	bool brunGetExistHDCPKEY(char *scalarID);
	bool runInsertData(int i_type);
	bool runCheckFlow( int i_type);// i_type 1 = pcbaid,  2 = scalarId
	bool runCheckFlowAllStation( int i_type);// i_type 1 = pcbaid,  2 = scalarId
	bool runCheckPartNo(int n_type);// i_type 1 = pcbaid,  2 = scalarId

	bool runReadScalarID( char *szvalue, int iSize );
	bool runCheckModel( );
	bool GetPartNo(int n_type);
	bool GetModelByPartNo();
	bool runCheckPCBAID( char *szvalue );
	bool IfRepeated( char *szvalue );    // 1 = ID ,2 = SN
	//bool IfRepeated(unsigned short IdType);    // 1 = ID ,2 = SN

	bool getWeek(void);
	int GetWeek	(struct tm* date	);
	int GetCurrentWeek();

	bool showMsg(const char* szMsg);
	bool ExecAdbOut(CString Command, char* output, char* ErrorCode);
};


#endif // End of #ifndef _C_GSensor_Test_Proc_H_
