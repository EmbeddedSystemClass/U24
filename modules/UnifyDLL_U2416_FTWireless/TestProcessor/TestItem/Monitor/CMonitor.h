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
#include <direct.h>

/* ID Type */

#define ReadScalarID				"ReadScalarID"
#define GetHDCPKEY				"GetHDCPKEY"
#define CheckPCBAID				"CheckPCBAID"
#define CheckFlow					"CheckFlow"
#define CheckAllFlow				"CheckAllFlow"
#define InsertData					"InsertData"
#define WriteHDCP				"WriteHDCP"
#define WriteHDCP_CSD				"WriteHDCP_CSD"
#define WriteTag					"WriteTag"
#define WriteCSDTag					"WriteCSDTag"
#define ReadCSDTag					"ReadCSDTag"
#define CheckTag					"CheckTag"
#define CheckModel				"CheckModel"
#define CheckSWVersion		"CheckSWVersion"
#define Postcmd					"Postcmd"
#define Precmd					"Precmd"
#define PlayVideo					"PlayVideo"
#define CheckSWversionByDB "CheckSWversionByDB"
#define CheckSWversionByDB_CSD "CheckSWversionByDB_CSD"

#define DNULL                        "NULL"
#define DREAD	                       "READ"
#define VMS_DBACCESS				_T("VMS_DBACCESS.dll")

#define USB_BUF_SIZE 512
#define BUFFER_SIZE 4096

#define ADB_CMD_TIMEOUT			25000
#define ID_SIZE								11
#define i_Station_Count					10
class CMonitor : public ITestItem
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

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

public:
	CMonitorId Id;
	CMonitorSo So;
	CMonitorPartNo PartNo;
	CMonitorWbcModel WbcFileName;

protected:
private:
	std::string std_key_path;
	std::string std_Key_Id;
	std::string std_Key_Name;
	std::string std_ScalarId ;
	std::string szPcabaId;
	std::string checkStation;
	std::string sz_Hdcp_key_path;
	std::string m_szPartNo;
	std::string m_ModelName;
	std::string m_szSWver;
	CString     cs_modelName_cmonitor;
	CString cs_DBModelNamel;

	//unsigned char sz_ID[ID_SIZE_BUFFER] ;
	//unsigned char szStation[ID_SIZE_BUFFER];
	
	//bool runReadScalarID( char *szvalue, int iSize );

	bool GetPartNo();
	bool GetModelByPartNo();
	bool changeModel();
	bool runGetHDCPKEY(int nIdtype);
	bool runWriteHDCPKEY();
	bool runWriteHDCPKEY_CSD();
	bool runCheckTag();
	bool runWriteTag();
	bool runWriteCSDTag();
	bool runReadCSDTag();
	bool bGetNewHDCPKEY(char *scalarID);
	bool brunGetExistHDCPKEY(char *scalarID);
	bool runInsertData(int i_type);
	bool runCheckFlow( int i_type);// i_type 1 = pcbaid,  2 = scalarId
	bool runCheckFlowAllStation( int i_type);// i_type 1 = pcbaid,  2 = scalarId
	bool bUpdateKEYWrite(int nIdtype); // 1 = picasso, 2 = scalar
	bool runReadScalarID( char *szvalue, int iSize );
	bool runCheckPCBAID( char *szvalue );
	bool runCheckModel();
	bool runCheckSWversion();
	bool runCheckSWversionByDB();
	bool runCheckSWversionByDB_CSD();
	bool GetSWVersionFromDB();
	bool runPostCmd( );
	bool runPreCmd( );
	bool runPlayVideo( );
	bool IfRepeated( char *szvalue );    // 1 = ID ,2 = SN
	//bool IfRepeated(unsigned short IdType);    // 1 = ID ,2 = SN
	bool checkFileExist(CString csFilePath);
	bool getWeek(void);
	int GetWeek	(struct tm* date	);
	int GetCurrentWeek();

	bool showMsg(const char* szMsg);
	bool ExecAdbOut(CString Command, char* output, char* ErrorCode);
	bool ExecFastbootOut(CString Command, char* output, char* ErrorCode);
	bool bCallAdbFastbootCMD(CString csAdbFastboot, CString Command, char* output, char* ErrorCode, CString cs_FindData);
};


#endif // End of #ifndef _C_GSensor_Test_Proc_H_
