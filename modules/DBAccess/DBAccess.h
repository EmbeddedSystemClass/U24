/***************************************************************************************
* Copyright (C) : QCS/WGC18                                      
* Module name	: DataBase Access.   
* File name     : DBAccess.h             
* Summary       : DB Dispose                                                             
* Version     Author              Date                 Abstract                      
* 1.0      Sunlight.Qian        2011/03/22           First version   
****************************************************************************************/

#pragma once

#ifdef DBACCESS_EXPORTS
#define DBACCESS_API extern "C" __declspec(dllexport)
#define DBACCESS_CLASS_API __declspec(dllexport)
#else
#define DBACCESS_API extern "C" __declspec(dllimport)
#define DBACCESS_CLASS_API __declspec(dllimport)
#endif

#include <comdef.h>
#import "msado15.dll" no_namespace rename("EOF","adoEOF")  // 这行语句声明在工程中使用ADO，但不使用ADO的名字空间，并且为了避免常数冲突，将常数EOF改名为adoEOF。
#include <map>
using namespace std;

#include "..\..\Lib\MyMSXML\MyMSXML.h"
#include "afxmt.h"


/*============================Macro define=======================================*/
#define  OK             1
#define  NG             0
#define  DBCONFIGFILE   _T(".\\DB\\INI\\DBConfig.ini")
#define  XMLPATHFILE    _T(".\\DB\\XML\\")
/*============================Macro define=======================================*/


/*============================Length define=======================================*/
#define      SO_LENGTH        6
#define      MODEL_LENGTH     7
#define      PN_LENGTH        12 
#define      SOTYPE_LENGTH    1
#define      COLNAME_LENGTH   50
#define      PASS_COUNT       2
/*=================================================================================*/

/*============================ErrorCode Define=======================================*/
#define      ERROR_QUERY_FAIL                  _T("DB1000")
#define      ERROR_INPUTSO_INVALID             _T("DB1001")
#define      ERROR_CONNECTDB_FAIL              _T("DB1002")
#define      ERROR_QUERYSO_FAIL                _T("DB1003")
#define      ERROR_PNLENGTH_FAIL               _T("DB1004")
#define      ERROR_MODELLENGTH_FAIL            _T("DB1005")
#define      ERROR_SOTYPELENGTH_FAIL           _T("DB1006")
#define      ERROR_QUERYPROCESS_FAIL           _T("DB1007")
#define      ERROR_QUERYSEGMENT_FAIL           _T("DB1008")
#define      ERROR_QUERYLINE_FAIL              _T("DB1009")
#define      ERROR_LINE_EMPTY                  _T("DB1010")
#define      ERROR_PROCESS_EMPTY               _T("DB1011")
#define      ERROR_SEGMENT_EMPTY               _T("DB1012")
#define      ERROR_ERRCODE_INVAILD             _T("DB1013")
#define      ERROR_INSERTNG_FAIL               _T("DB1014")
#define      ERROR_INSERTNGTS_FAIL             _T("DB1015")
#define      ERROR_INSERTOK_FAIL               _T("DB1016")
#define      ERROR_CHECKMATCHTABEL_FAIL        _T("DB1017")
#define      ERROR_INPUTEMPLOYEE_INVALID       _T("DB1018")
#define      ERROR_INPUTID_INVALID             _T("DB1019")
#define      ERROR_QUERYPSEQ_FAIL              _T("DB1020")
#define      ERROR_INVALIDROUTING_SEQ          _T("DB1021")
#define      ERROR_RETESTCOUNT_INVALID         _T("DB1022")
#define      ERROR_PCS_SUB_SEQ_INVALID         _T("DB1023")
#define      ERROR_TESTFAIL_UNFIXED            _T("DB1024")
#define      ERROR_CHECK_RETESTCOUNT_FAIL      _T("DB1025")
#define      ERROR_INVALID_EMPLOYEEID          _T("DB1026")
#define      ERROR_QUERYPNINBACH_FAIL          _T("DB1027")
#define      ERROR_QUERYKEYPARTS_FAIL          _T("DB1028")
//add by luffy at 2011.08.15
#define      ERROR_QUERYMAC_FAIL                _T("DB1029")
#define      ERROR_QUERYWIFILOGIN_FAIL          _T("DB1030")
//~end add
#define      ERROR_QUERYPICASSO_FAIL           _T("DB1031")
#define      ERROR_PICASSO_NOTINSMT            _T("DB1032")
#define      ERROR_INSERTPICASSO_FAIL          _T("DB1033")
#define      ERROR_PICASSO_LOCK                _T("DB1034")
#define      ERROR_PICASSO_DUPLICATE           _T("DB1035")
#define      ERROR_INPUTCHARGERID_EMPTY        _T("DB1036")
#define      ERROR_INPUTBATTERYID_EMPTY        _T("DB1037")
#define      ERROR_INPUTCHARGERID_NORECORD     _T("DB1038")
#define      ERROR_INPUTBATTERYID_NORECORD     _T("DB1039")
#define      ERROR_UPDATEOOBSTATUS_FALSE       _T("DB1040")
#define      ERROR_INSERTOOBSTATUS_FAIL        _T("DB1041")
#define      ERROR_NOOOBPASS_RECORD            _T("DB1042")
#define      ERROR_INSERTOOBREWORK_EECORD      _T("DB1043")
#define      ERROR_FILEDVALUE_EMPTY            _T("DB1044")

#define      ERROR_SOSAMPLERATE_EMPTY          _T("DB1045")
#define      ERROR_MODELSAMPLERATE_EMPTY       _T("DB1046")
#define      ERROR_SAMPLERATE_EMPTY            _T("DB1047")

#define      ERROR_DELETE_KPWIP                _T("DB1048")
#define      ERROR_UPDATE_PTPPIDLINK           _T("DB1049")
#define      ERROR_KPID_DUPLICATE              _T("DB1050")
#define      ERROR_NAL_DUPLICATE               _T("DB1051")
#define      ERROR_INPUT_NALIMEI               _T("DB1052")
#define      ERROR_NAL_UNLINK                  _T("DB1053")
#define      ERROR_IMEINAL_DUPLICATE           _T("DB1054")

#define      ERROR_SSSCTD_EMPTY                _T("DB1055")
#define      ERROR_RROUTING_TIMEERROR          _T("DB1056")
#define      ERROR_INSERTTSREWORK_RECORD       _T("DB1057")   
#define      ERROR_SEGMENTPROCESS_EMPTY        _T("DB1058") 
/*CR hold*/
#define      ERROR_CRSTATUS                    _T("DB1059")
#define      ERROR_SONOTSAME                   _T("DB1060")
#define      ERROR_FIRSTPROCESS_NOTTEST        _T("DB1061")
#define      ERROR_FIRSTPROCESS_NOTPASS        _T("DB1062")
#define      ERROR_PROCESS_NOTFIRST            _T("DB1063")
#define      ERROR_QUERYLASTERROR              _T("DB1064")
#define      ERROR_NAL_INVALID                 _T("DB1065")
#define      ERROR_FIRSTNAL_NOTMATCH           _T("DB1066")
#define      ERROR_PNNAL_EMPTY                 _T("DB1067")

#define      ERROR_INVALID_EMPLOYEEIDSTATUS    _T("DB1068")
#define      ERROR_UPDATE_EMPLOYEEIDSTATUS     _T("DB1069")
#define      ERROR_CLOSESO                     _T("DB1070")

#define      ERROR_QAERRORCODE_INVLID          _T("DB1071")
#define      ERROR_UNLINK_IMEI_PICASSO         _T("DB1072")

/*query 22 PCBA no for acer*/
#define      ERROR_QUERY_PCBA_NO               _T("DB1073")
#define      ERROR_QUERY_PCBA_SERIAL           _T("DB1074")
#define      ERROR_UPDATE_PCBA_SERIAL          _T("DB1075")
#define      ERROR_QUERY_TESTINFO              _T("DB1076")
#define      INSERT_UNLINKKEYPARTS_RECORD      _T("DB1077")
#define      ERROR_UPDATE_FALINK               _T("DB1078")

#define      ERROR_DOASTATUS                   _T("DB1079")
#define      ERROR_MODEL_NAME_RULE             _T("DB1080")//CSD and normal model should follow the name rule:  XXXXCS* ==> XXXXB*A
//CHECK Routing error code
#define      ERROR_CHECK_SEG1_PRO1          _T("DB1111")
#define      ERROR_CHECK_SEG1_PRO2          _T("DB1112")
#define      ERROR_CHECK_SEG1_PRO3          _T("DB1113")
#define      ERROR_CHECK_SEG1_PRO4          _T("DB1114")
#define      ERROR_CHECK_SEG1_PRO5          _T("DB1115")
#define      ERROR_CHECK_SEG1_PRO6          _T("DB1116")
#define      ERROR_CHECK_SEG1_PRO7          _T("DB1117")
#define      ERROR_CHECK_SEG1_PRO8          _T("DB1118")
#define      ERROR_CHECK_SEG1_PRO9          _T("DB1119")

#define      ERROR_CHECK_SEG2_PRO1          _T("DB1121")
#define      ERROR_CHECK_SEG2_PRO2          _T("DB1122")
#define      ERROR_CHECK_SEG2_PRO3          _T("DB1123")
#define      ERROR_CHECK_SEG2_PRO4          _T("DB1124")
#define      ERROR_CHECK_SEG2_PRO5          _T("DB1125")
#define      ERROR_CHECK_SEG2_PRO6          _T("DB1126")
#define      ERROR_CHECK_SEG2_PRO7          _T("DB1127")
#define      ERROR_CHECK_SEG2_PRO8          _T("DB1128")
#define      ERROR_CHECK_SEG2_PRO9          _T("DB1129")

#define      ERROR_CHECK_SEG3_PRO1          _T("DB1131")
#define      ERROR_CHECK_SEG3_PRO2          _T("DB1132")
#define      ERROR_CHECK_SEG3_PRO3          _T("DB1133")
#define      ERROR_CHECK_SEG3_PRO4          _T("DB1134")
#define      ERROR_CHECK_SEG3_PRO5          _T("DB1135")
#define      ERROR_CHECK_SEG3_PRO6          _T("DB1136")
#define      ERROR_CHECK_SEG3_PRO7          _T("DB1137")
#define      ERROR_CHECK_SEG3_PRO8          _T("DB1138")
#define      ERROR_CHECK_SEG3_PRO9          _T("DB1139")


/*===================================================================================*/

#pragma warning(disable:4996)


/* DB Access class */
class CDBAccess
{
public:
	CDBAccess(void);
	virtual ~CDBAccess(void);

public:
    int ReadIniFile(void);



public:
	

private:


private:
	
};

//Luffy add 20120919 for get server date time,2012-09-19 14:45:21
typedef struct _MyDatetime {
	CString date;    //20120919
	CString time;	//144521
} MyDatetime;

extern std::map<CString, CString>  m_mapPICS;             // PICS数据
extern std::map<CString, CString>  m_map_matchingTable;   // Matching Table
extern _ConnectionPtr   m_p_connection;       // _ConnectionPtr接口
extern CString          m_str_serverName;     // 服务器名
extern CString          m_str_dbName;         // 数据库名
extern CString          m_str_userName;       // 登录用户名
extern CString          m_str_userPassword;   // 登录密码
extern CCriticalSection  m_obj_loadADBFileCritSection;

extern int G_ErrNumber;

CString GetInsert_String();
CString GetInsertTS_String();
CString GetCommon_String(CString str_tablename);

//Luffy add for get datetime from server
bool GetServerTime(MyDatetime &datetime);

void DBLOG(CString str_message);
bool ReadXMLFile(void);
bool CheckMatchTable(void);
CString GetMatchTableInsertSql(void);
bool GetPreTableFixtureItems (CString str_ID);

int ExecuteSQL(CString str_sql);
int GetQueryResult(CString str_sql, _RecordsetPtr & p_recordSet);

bool CheckTSUnFixedData(char * sz_id, char * sz_ngprocess, char * sz_ErrCode,char * sz_TSModelName);

/*sunlight modify 20111010, for new rule about retest count, requirement from alex liu*/
bool CheckRetestCount(char * sz_id, char * sz_process, char * sz_ErrCode,char * sz_ErrMsg);
bool CheckRetestCount_new(char * sz_id, char * sz_process, char * sz_ErrCode,char * sz_ErrMsg);
/*Sunlight modify end */

//nancy update to CheckProcess_V2 on 20130428 for conversion between normal and csd model, requirement for allen.zhao
bool CheckProcess(int i_checkrule,char * sz_id, char * sz_model, char * sz_process, char * sz_partno,char * sz_ErrCode, char * sz_ErrMsg);
bool CheckProcess_V2(int i_checkrule,char * sz_id, char * sz_model, char * sz_process , char * sz_ErrCode, char * sz_ErrMsg);

bool GetPreProcessResult(int i_checkrule,CString str_sql, char * sz_id, char * sz_model,const CString str_Fixdate, const CString str_Fixtime, const int i_ReturnPro, char * sz_ErrCode, char * sz_ErrMsg);

/* sunlight add 20111031,Special for luna*/
bool GetProcessResult_Luna(char * sz_id, char * sz_model,char * sz_process);
bool GetProcessResult_NgReturn_Luna(char * sz_id, char * sz_model,char * sz_process, const CString str_Fixdate, const CString str_Fixtime);
/* Sunlight add end*/

/*Sunlight add 20120504, special for CSD model */
bool GetProcessResult_CSD(char * sz_id, char * sz_model,char * sz_process);
bool GetProcessResult_NgReturn_CSD(char * sz_id, char * sz_model, char * sz_process, const CString str_Fixdate, const CString str_Fixtime);


CString GetErrorCode(int pcsseq, int subseq);
bool GetErrCodeDetail_QA(char * sz_Errcode, char * sz_NGType, char * sz_NGRetail, char * sz_MajorMin, char * sz_desp);

/* =======================================DLL output function========================================================== */

DBACCESS_API bool GetProcessResult(char * sz_id, char * sz_model,char * sz_process);
DBACCESS_API bool GetProcessResult_NgReturn(char * sz_id, char * sz_model,char * sz_process, const CString str_Fixdate, const CString str_Fixtime);

/* sunlight modify 20111010,to open this API for warehouse management tool, new check rule*/
DBACCESS_API bool GetProcessResult_new(char * sz_id, char * sz_model,char * sz_process);
DBACCESS_API bool GetProcessResult_NgReturn_new(char * sz_id, char * sz_model,char * sz_process, const CString str_Fixdate, const CString str_Fixtime);
/* Sunlight modify end*/

DBACCESS_API bool QuerySO_Info(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char* sz_errorCode, char* sz_errorMsg);
DBACCESS_API bool QueryStation_Info(char * PCname, char* sz_line, char* sz_process, char* sz_segment, char* sz_errorCode, char* sz_errorMsg);
DBACCESS_API bool InsertFAIL_data(char* sz_id, char* sz_so, char* sz_employId, char* sz_inputErrorCode, char* sz_errorCode, char* sz_errorMsg);
DBACCESS_API bool InsertPASS_data(char* sz_id, char* sz_so, char* sz_employId, char* sz_errorCode, char* sz_errorMsg);
DBACCESS_API bool Insertdata_Common(char * sz_XMLname,char * sz_tablename, char* sz_errorCode, char* sz_errorMsg);
DBACCESS_API bool InsertOOB_data(char * sz_result,char * sz_InputErrCode, char* sz_errorCode, char* sz_errorMsg);
DBACCESS_API bool SetValue(const char * sz_index, const char* sz_value);
DBACCESS_API bool Clear_PICSData();
DBACCESS_API bool ConnectDB(void);
DBACCESS_API void CloseDB(void);
DBACCESS_API bool CheckRouting(char * sz_id, char * sz_model, char * sz_process,char * sz_ErrCode, char * sz_ErrMsg);
DBACCESS_API bool QuerySOCount(char * sz_id, char * sz_so, char * sz_process, char * sz_count,char * sz_ErrCode, char * sz_ErrMsg);
DBACCESS_API bool QueryEmployeeID_Right(char* sz_employId, char * sz_Level,char* sz_ErrCode, char* sz_ErrMsg);

/* Sunlight add 20110627*/
DBACCESS_API bool QueryPNinBACH_BySO(char * sz_PN, char * sz_SO, char * sz_qty,char * sz_ErrCode, char * sz_ErrMsg);
DBACCESS_API bool QueryKeyparts_Info(char * sz_PN, char * sz_Model,char * sz_Process,char * sz_Count, char * sz_Info, char * sz_ErrCode, char * sz_ErrMsg);
DBACCESS_API bool QueryOOBErrCode_Info(char * sz_model,char * sz_Process, char * sz_ErrCode, char * sz_ErrMsg);
/* Sunlight add 20110627 end */

/* Sunlight add 20110630, query PICASSO BY imei or PPID */
DBACCESS_API bool GetPicasso(char * sz_InputID, char * sz_Model,char * sz_Picasso,  char* sz_partno, char * sz_ErrCode, char * sz_ErrMsg);
/* Sunlight add 20110630 end */

/* Sunlight add 20110713, for unify UI */
DBACCESS_API bool QueryRecordBySO(char * sz_so, char * sz_process, char * sz_wrkstation, char * sz_totalCount, char * sz_passCount,char * sz_failCount,char * sz_ErrCode,char * sz_ErrMsg);
DBACCESS_API bool QueryFirstProcess(char * sz_modelname, char * sz_process, char * sz_ErrCode,char * sz_ErrMsg);
/* Sunlight add 20110713, end */ 

/* Sunlight add, for keyparts link process*/
DBACCESS_API bool QuerySegmentProcess(char * sz_modelname, char * sz_segment,char * sz_process, char * sz_ErrCode,char * sz_ErrMsg);

/*Sunlight add 20110725 ,for key parts link process*/
DBACCESS_API bool InsertKeypartsInfo(char * sz_id, char * sz_process,char * sz_kpid, char * sz_kpname,char * sz_empolyee,char * sz_pn,char * sz_ErrCode,char * sz_ErrMsg);

/*sunlight add 20110804, for IMEI station check status*/
DBACCESS_API bool QueryIMEIresult(char * sz_InputID, char * sz_Model,char * sz_Picasso, char * sz_ErrCode, char * sz_ErrMsg);

//Luffy add 20110815 ,query mobile login wifi or not according to Imei
DBACCESS_API bool QueryWifiLoginInfo(char * sz_Imei, char * sz_Mac, char *sz_LoginWifiInfo, char * sz_ErrCode,char * sz_ErrMsg);

/*sunlight add 20110817, for check Picasso duplicate in unify UI */
DBACCESS_API bool CheckPicassoStatus (char * sz_Picasso, char * sz_TimeInfo,char * sz_Status, char * sz_ErrCode, char * sz_ErrMsg);
DBACCESS_API bool InsertPicassoControl(char * sz_Picasso, char * sz_TimeInfo,char * sz_Status,char * sz_ErrCode, char * sz_ErrMsg);
DBACCESS_API bool CheckPicassoDuplicate_SecondStation(char * sz_Picasso, char * sz_TimeInfo, char * sz_ErrCode, char * sz_ErrMsg);
DBACCESS_API bool CheckPicassoDuplicate(char * sz_Picasso, char * sz_FAPicasso, char * sz_TimeInfo, char * sz_FATimeInfo, char * sz_status,char * sz_ErrCode, char * sz_ErrMsg);

/*sunlight add 20110906, for get toolver/factoryimagever/userimagever from SO */
DBACCESS_API bool QuerySO_Info_All(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char * sz_toolVer, char * sz_facImageVer, char * sz_userImageVer,char * sz_EntranceToolVer,char* sz_errorCode, char* sz_errorMsg);

/*sunlight add 20110919, for wolf run_in check*/
DBACCESS_API bool QueryChargerIDinRunin(char * sz_model,char* sz_charger,char* sz_errorCode, char* sz_errorMsg);
DBACCESS_API bool QueryBatteryIDinRunin(char * sz_model,char* sz_battery,char* sz_errorCode, char* sz_errorMsg);

/*sunlight add 20111008, for OOB test requirement*/
DBACCESS_API bool QueryOOBstatus(char * sz_IMEI,char* sz_status,char* sz_errorCode, char* sz_errorMsg);
DBACCESS_API bool UpdateOOBStatusToFalse(char * sz_IMEI,char* sz_errorCode, char* sz_errorMsg);
DBACCESS_API bool InsertOOBStatus(char * sz_model, char * sz_so, char * sz_pn, char * sz_IMEI,char* sz_errorCode, char* sz_errorMsg);
DBACCESS_API bool QueryOOBResult(char * sz_IMEI,char * sz_result,char* sz_errorCode, char* sz_errorMsg);   //20120918, add output sz_result
DBACCESS_API bool InsertPTOOBRework(char * sz_IMEI,char* sz_errorCode, char* sz_errorMsg);
DBACCESS_API bool GetIMEI_Info(char * sz_InputID, char * sz_model,char * sz_Picasso, char * sz_partno,char * sz_SO, char * sz_sw,char * sz_ErrCode, char * sz_ErrMsg);

/*sunlight add 20111010, for OOB test requirement*/
DBACCESS_API bool QueryIMEI_ByCarton(char * sz_CartonID, char * sz_count,char * sz_IMEIlist, char * sz_ErrCode, char * sz_ErrMsg);

/*Sunlight add 20111017, for wolf run in ID check*/
DBACCESS_API bool QueryRuninID(char * sz_model,char* sz_ID,char * sz_column,char* sz_errorCode, char* sz_errorMsg);

/*Sunlight add 20111031, for warehouses management tool*/
DBACCESS_API bool QueryOOBSampleRate(char * sz_model,char* sz_SO,char * sz_sampleRate,char* sz_errorCode, char* sz_errorMsg);

/*Sunlight add 20111102, for new check routing rule*/
DBACCESS_API bool CheckRouting_New(char * sz_id, char * sz_model, char * sz_process,char * sz_ErrCode, char * sz_ErrMsg);

/*Sunlight add 20111201, for keyparts link */
DBACCESS_API bool UnlinkKeyparts(char * sz_id, char * sz_model, char * sz_ErrCode, char * sz_ErrMsg);
DBACCESS_API bool CheckKeyparts_duplicate(char * sz_picasso, char * sz_kpid/*, char * sz_kpname,char * sz_model*/,char * sz_ErrCode, char * sz_ErrMsg);


/*Sunlight add 20111206, for IMEI station NAL check */
DBACCESS_API bool CheckNAL_duplicate(char * sz_IMEI, char * sz_NAL, char * sz_ErrCode, char * sz_ErrMsg);


/*sunlight add 20111213, for UI get so QTY from SO */
DBACCESS_API bool QuerySO_Info_All_V2(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char * sz_toolVer, char * sz_facImageVer, char * sz_userImageVer,char * sz_EntranceToolVer,char * sz_Qty,char* sz_errorCode, char* sz_errorMsg);
DBACCESS_API bool QueryNAL(char * sz_IMEI, char * sz_NAL, char * sz_ErrCode, char * sz_ErrMsg);

/*sunlight add 20111214,for UI calculate the PASS count for SO*/
DBACCESS_API bool QuerySOCount_Control(char * sz_id, char * sz_so, char * sz_process, char * sz_count, char * sz_ErrCode,char * sz_ErrMsg);

/*sunlight add 20111215,for UI get test count*/
DBACCESS_API bool QueryIfTestAgain(char * sz_id, char * sz_model,char * sz_process, char * sz_count,char * sz_ErrCode,char * sz_ErrMsg);

/*Sunlight add 20111223, for OOB TOOL*/
DBACCESS_API bool QueryIMEIStatus(char * sz_InputID,char * sz_status,char * sz_cartonID,char * sz_ErrCode,char * sz_ErrMsg);

/*Sunlight add 20111223, for UNLINK FA KEYPARTS*/
DBACCESS_API bool UnlinkKeyparts_FA(char * sz_id, char * sz_model,char * sz_process,char * sz_ErrCode, char * sz_ErrMsg);

/*Sunlight add 20111226, for FDCS phase, add return so CR or NOT */
DBACCESS_API bool QuerySO_Info_All_V3(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char * sz_toolVer, char * sz_facImageVer, char * sz_userImageVer,char * sz_EntranceToolVer,char * sz_Qty,char * sz_CRtype,char* sz_errorCode, char* sz_errorMsg);

/*Sunlight add 20111227, for keyparts */
DBACCESS_API bool QueryKeypartsByID(char * sz_ID, char * sz_process,char * sz_Count, char * sz_Info, char * sz_ErrCode, char * sz_ErrMsg);
/*Magan.Mei add 2014/02/13, for Berlin FALINK from SN to SN*/
DBACCESS_API bool QueryKeypartsByID_V2(char * sz_ID, char * sz_process,char * sz_Count, char * sz_Info, char * sz_ErrCode, char * sz_ErrMsg);

/*Sunlight add 20120104, for keyparts and rework tool  */
DBACCESS_API bool InsertTSData_forTS(char * sz_id, char * sz_model,char * sz_rtnprocess, char * sz_ngprocess, char * sz_ngwrkstation, char * sz_ErrCode, char * sz_ErrMsg);

/*Sunlight add 20120111, for keyparts */
DBACCESS_API bool QueryIMEI_ByPicasso(char * sz_picassoid ,char * sz_IMEI,char * sz_ErrCode, char * sz_ErrMsg);

/*Sunlight add 20120112, Query hold infomation */
DBACCESS_API bool QueryholdInfo(char * sz_id ,char * sz_model,char * sz_status,char * sz_process,char * sz_holdreas,char * sz_ErrCode, char * sz_ErrMsg);

/*Sunlight add 20120119, query Picasso in SMT */
DBACCESS_API bool CheckPicassoSMT (char * sz_Picasso, char * sz_ErrCode, char * sz_ErrMsg);

/*Sunlight add 20120201, check process test time sequence */
DBACCESS_API bool CheckProcessTestDate(char * sz_Picasso,char * sz_model, char * sz_segment,char * sz_process,char * sz_ErrCode, char * sz_ErrMsg);


/*Sunlight add 20120220, For CR control */
DBACCESS_API bool CheckSO(char * sz_so,char * sz_soCRtype,char * sz_picasso,char * sz_model,char * sz_process,char * sz_segment, char * sz_ErrCode, char * sz_ErrMsg);
DBACCESS_API bool InsertCRHoldInfo(char * sz_picasso,char * sz_IMEI,char * sz_so,char * sz_soCRtype, char * sz_model,char * sz_process, char * sz_segment,char * sz_employID,char * sz_partno,char * sz_ErrCode, char * sz_ErrMsg);
DBACCESS_API bool InsertWareHouseOutHoldInfo(char * sz_picasso,char * sz_IMEI, char * sz_model,char * sz_partno, char * sz_ErrCode, char * sz_ErrMsg);
DBACCESS_API bool QueryCRHoldInfo(char * sz_picasso,char * sz_status, char * sz_ErrCode, char * sz_ErrMsg);
DBACCESS_API bool QueryFirstProcessOfSegment(char * sz_model,char * sz_segment, char * sz_process,char * sz_ErrCode, char * sz_ErrMsg);
DBACCESS_API bool QueryLastErrorcode(char * sz_id,char * sz_process,char * sz_NGcode,char * sz_ErrCode, char * sz_ErrMsg);

/*Sunlight add 20120319,  For SSS hold information*/
DBACCESS_API bool QueryholdInfo_V2(char * sz_id ,char * sz_model,char * sz_process,char * sz_status,char * sz_holdreas,char * sz_ErrCode, char * sz_ErrMsg);

/*Sunlight add 20120322,  Add check NAL */
DBACCESS_API bool CheckNAL_Validity(char * sz_pn ,char * sz_NAL,char * sz_ErrCode, char * sz_ErrMsg);


/*Sunlight add 20120409, add for PVCS phase in*/
DBACCESS_API bool QueryEmployeeID_Right_V2(char* sz_employId, char * sz_Level,char * sz_status,char * sz_wrkstation, char* sz_ErrCode, char* sz_ErrMsg);
DBACCESS_API bool UpdateEmployeeID_Status(char* sz_employId, char * sz_status,char * sz_wrkstation, char* sz_ErrCode, char* sz_ErrMsg);
DBACCESS_API bool QuerySO_Info_All_V4(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char * sz_toolVer, char * sz_facImageVer, char * sz_userImageVer,char * sz_EntranceToolVer,char * sz_Qty,char * sz_CRtype,char * sz_segment,char* sz_errorCode, char* sz_errorMsg);

/*Sunlight add 20120522, add for IMEI TS tool*/
DBACCESS_API bool UnlinkIMEIPicasso(char * sz_imei, char * sz_picasso,char * sz_ErrCode, char * sz_ErrMsg);

/*Sunlight add 20120619, add for faprint process*/
DBACCESS_API bool Query_PCBA_SerialNO(char * sz_picasso,char * sz_AcerMB, char * sz_YWW, char * sz_Employee,char * sz_Serial, char * sz_ErrCode, char * sz_ErrMsg);
DBACCESS_API bool Query_PCBA_NO(char * sz_picasso, char * sz_process,char * sz_PCBAno,char * sz_ErrCode, char * sz_ErrMsg);

/*Sunlight add 20120627, for all process check NAL*/
DBACCESS_API bool CheckNAL_duplicate_V2(char * sz_process,char * sz_IMEI, char * sz_NAL,char * sz_ErrCode, char * sz_ErrMsg);

/*Fox add 20120716*/
DBACCESS_API bool ReturnErrNumber(char* sz_model,char* sz_process,char * sz_ErrCode, char * sz_ErrMsg);

/*Sunlight add 20120723*/
DBACCESS_API bool GetTestInfo(char* sz_id, char * sz_model,char* sz_process,char * sz_count,char * sz_info,char * sz_ErrCode, char * sz_ErrMsg);

/*Luffy add 20120730*/
DBACCESS_API bool GetLastRecord(char* sz_id,char* sz_model,char* sz_process,std::map<CString, CString> & map_Record,char * sz_ErrCode, char * sz_ErrMsg);
/*Alex.Chen add 20121126*/
DBACCESS_API bool QuerySO_Info_All_V5(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char* sz_toolVer, char* sz_facImageVer, char* sz_userImageVer, char* sz_EntranceToolVer, char* sz_Qty, char* sz_CRtype,char* sz_segment, char* sz_Fix_EntTool_ver, char* sz_Fix_Img_ver, char* sz_errorCode, char* sz_errorMsg);

/*Lennon add 20130326 for WareHouse Tool*/
DBACCESS_API bool Insert_WareHouseIn   (char* sz_picasso,char* sz_imei,char* sz_so, char* sz_partNo,char* sz_sw,char * sz_Qty ,char* sz_CartonID, char* sz_ErrCode, char* sz_ErrMsg);
DBACCESS_API bool Update_WareHouseOut  (char* sz_picasso,char* sz_imei,char* sz_ReworkReason, char* sz_DOARMANo, char* sz_ErrCode, char* sz_ErrMsg);
DBACCESS_API bool Query_WareHouseStatus(char* sz_picasso,char* sz_imei,char* sz_Status, char* sz_ErrCode, char* sz_ErrMsg);
DBACCESS_API bool Query_WareHouseStatus_V2(char* sz_picasso,char* sz_imei,char* sz_Status,char* szInsertData, char* szInsertTime, char* sz_ErrCode, char* sz_ErrMsg);

DBACCESS_API bool QueryApkTestItem(char * sz_PN, char * sz_Item,char * sz_SDStatus, char * sz_ErrCode,char * sz_ErrMsg);
DBACCESS_API bool QueryApkTestItem_V2(char * sz_PN, char * sz_Item,char * sz_SDStatus,char* sz_Switch, char * sz_ErrCode,char * sz_ErrMsg);

/*Nancy add 20130422 for DOA/RMA*/
DBACCESS_API bool QueryDOAInfo(char * sz_picasso,char * sz_status, char * sz_ErrCode, char * sz_ErrMsg);

//redefine check routing for allen's new rule(Conversion between CSD and normal model) by Nancy.Yang on 20130429
DBACCESS_API bool CheckRouting_V2(char * sz_id, char * sz_model, char * sz_process,char * sz_ErrCode, char * sz_ErrMsg);

//query hold info for warehorse tool
DBACCESS_API bool QueryCRHoldInfo_byMNID(char * sz_mnid,char * sz_status, char * sz_ErrCode, char * sz_ErrMsg);

//query consumtype and maxtimes by model and process
DBACCESS_API bool QueryConsumeType(char * sz_model, char * sz_process,char * sz_consume, char * sz_ErrCode,char * sz_ErrMsg);
//query consume info by consume id and type
DBACCESS_API bool QueryConsumeInfo(char * sz_sn, char * sz_type,char * sz_times, char * sz_ErrCode,char * sz_ErrMsg);

//query id type for IMEI/FCHECK/OOB/WareHous
DBACCESS_API bool Query_IDType(char* szpartno, char* szIDtype,  char* szLength, char* sz_ErrCode, char* sz_ErrMsg);

