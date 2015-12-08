/***************************************************************************************
* Copyright (C) : QCS/WR318                                      
* Module name	: VMS DataBase Access.   
* File name     : VMS_DBAccess.h             
* Summary       : DB Dispose                                                             
* Version     Author              Date                 Abstract                      
* 1.0.0.0      Sunlight.Qian        2011/08/22           First version 
* 1.0.0.1      Sunlgiht.Qian        2011/09/09           Add VMS_QueryPartNumber
* 1.0.0.2      Sunlight             2011/09/21           Add VMS_QuerySQNinfo_V2
* 1.0.0.3      Sunlight             2011/12/26           Add VMS_QuerySQNinfo_All, SQN info output change to char *
* 1.0.0.4      Sunlight             2011/12/27           Add VMS_QueryPNControlTable and VMS_QueryPNReserveTable update
* 1.0.0.5      Sunlight             2012/03/08           Support dependencies project
****************************************************************************************/

#ifdef VMS_DBACCESS_EXPORTS
#define VMS_DBACCESS_API extern "C" __declspec(dllexport)
#define VMS_DBACCESS_CLASS_API __declspec(dllexport)
#else
#define VMS_DBACCESS_API extern "C" __declspec(dllexport)
#define VMS_DBACCESS_CLASS_API __declspec(dllimport)
#endif

#include <comdef.h>
#import "msado15.dll" no_namespace rename("EOF","adoEOF")  // 这行语句声明在工程中使用ADO，但不使用ADO的名字空间，并且为了避免常数冲突，将常数EOF改名为adoEOF。
#include <map>
#include "Shlwapi.h"
using namespace std;

#include "afxmt.h"

#pragma warning(disable:4996)

/*============================Macro define=======================================*/
#define  OK             1
#define  NG             0
//#define  DBCONFIGFILE   _T("D:\03.Factory\00.DELL\5.U2416_OK\20151022_readDellID\GBROB1A_OS_DL_V1.000_20151026_OK_test\DB\INI\DB\INI\VMS_DBConfig.ini")
#define  DBCONFIGFILE   _T(".//DB//INI//VMS_DBConfig.ini")
#define  MAX_BUFFER_5E   50
/*============================Macro define=======================================*/

/*============================ErrorCode Define=======================================*/
#define      ERROR_QUERY_FAIL                  _T("VM1001")
#define      ERROR_INPUTSW_EMPTY               _T("VM1002")
#define      ERROR_SWMODEL_NOTMATCH            _T("VM1003")
#define      ERROR_SWSTATUS_INVALID            _T("VM1004")
#define      ERROR_INPUTTOOL_EMPTY             _T("VM1005")
#define      ERROR_TOOLMODEL_NOTMATCH          _T("VM1006")
#define      ERROR_TOOLSTATUS_INVALID          _T("VM1007")
#define      ERROR_SQNSTATUS_INVALID           _T("VM1008")
#define      ERROR_INPUTSQN_EMPTY              _T("VM1009")
#define      ERROR_SQNMODEL_NOTMATCH           _T("VM1010")
#define      ERROR_QUERYPN_INSQN               _T("VM1011")
#define      ERROR_INPUTSQNID_EMPTY            _T("VM1012")
#define      ERROR_INPUTPN_EMPTY               _T("VM1013")  
#define      ERROR_5GSTATUS_INVALID            _T("VM1014")  
#define      ERROR_PNCONTROL_EMPTY             _T("VM1015")  
#define      ERROR_5ESTATUS_INVALID            _T("VM1016")  
#define      ERROR_INPUT5E_EMPTY               _T("VM1017")  
#define      ERROR_QUERYMODELID_EMPTY          _T("VM1018")  
#define      ERROR_RESERVE_EMPTY               _T("VM1019")  
#define      ERROR_INPUT5G_EMPTY               _T("VM1020") 
#define      ERROR_QUERYPROJID_EMPTY           _T("VM1021") 
#define      ERROR_QUERYPROJ_EMPTY             _T("VM1022") 
#define      ERROR_QUERYPN_EMPTY               _T("VM1023")
#define      ERROR_SQN_OUTTIME                 _T("VM1024")
#define      ERROR_FILEDVALUE_EMPTY            _T("VM1025")
#define      ERROR_COMMONTOOL_EMPTY            _T("VM1026")
#define      ERROR_QUERYVRCM_EMPTY             _T("VM1027")

//This class is exported from the VMS_DBAccess.dll
class CVMS_DBAccess {
public:
	CVMS_DBAccess(void);
	virtual ~CVMS_DBAccess(void);
	int ReadIniFile(void);
	

private:

};

/* Local var and function */
extern _ConnectionPtr       m_p_connection;       // _ConnectionPtr接口
extern CCriticalSection     m_obj_loadADBFileCritSection;     //single lock
extern CString              m_str_serverName;     // 服务器名
extern CString              m_str_dbName;         // 数据库名


typedef struct
{
	int SQN_status;
	TCHAR SQN_starttime[20] ;
	TCHAR SQN_endtime[20];
	TCHAR SW_version[10];
	TCHAR variant_no[10];
	TCHAR region	[16];
	TCHAR OP_channel [128];
	TCHAR Qty_limitation [64];
	TCHAR SD_version [8];
	TCHAR user_tool_name [256];
	TCHAR RC_filename [256];
	TCHAR modem_version	[32];
	TCHAR OS_version [32];
	TCHAR touch_version	[32];
	TCHAR gauge_version	[32];
	TCHAR DFI_version [32];
	TCHAR RC_check [256];
	TCHAR lock_filename	[256];
	TCHAR FW_version [32];
	TCHAR band [60];
	TCHAR SIM_lock[60];
	TCHAR SIM_inclueded[60];
	TCHAR IMEI_tag[8];
	TCHAR CERT[256];
}st_9JPNData;


int  ExecuteSQL(CString str_sql);
int  GetQueryResult(CString str_sql, _RecordsetPtr & p_recordSet);
void DBLOG(CString str_message);
bool VMS_QueryModelID(char * sz_model, char * sz_modelID,char * sz_projID,char * sz_ErrCode, char * sz_ErrMsg );
bool VMS_QueryReserveTable(char * sz_projID, char * sz_table, std::map<CString, CString> & map_matching ,char * sz_ErrCode, char * sz_ErrMsg );
bool VMS_QueryControlTable(char * sz_partNo, char * sz_modelID, char * sz_table,std::map<CString, CString> & map_value ,char * sz_ErrCode, char * sz_ErrMsg );
bool VMS_QuerySQNinfo(char * sz_Model, char * sz_Partno, st_9JPNData & st_PNinfo, char * sz_ErrCode, char * sz_ErrMsg );
/*20111226,sunlight add */
bool VMS_QueryPNReserveTable(char * sz_pnID, char * sz_table,char * sz_PN, std::map<CString, CString> & map_matching ,char * sz_ErrCode, char * sz_ErrMsg );
bool VMS_Query9Jinfo(char * sz_model, char * sz_partNo, char * sz_pnID, char * sz_count, char * sz_9Jinfo, char * sz_ErrCode, char * sz_ErrMsg );
/*20111227,sunlight add*/
bool VMS_QueryPNControlTable(char * sz_partID, char * sz_modelID, char * sz_table,std::map<CString, CString> & map_value ,char * sz_ErrCode, char * sz_ErrMsg );

VMS_DBACCESS_API bool VMS_ConnectDB(void);
VMS_DBACCESS_API void VMS_CloseDB(void);
VMS_DBACCESS_API bool VMS_QueryFactoryImageStatus(char * sz_Model, char * sz_ImageVer,char * sz_Status, char * sz_starttime, char * sz_endtime,char * sz_ErrCode, char * sz_ErrMsg );
VMS_DBACCESS_API bool VMS_QueryToolStatus(char * sz_Model, char * sz_ToolVer,char * sz_ImageVer,char * sz_Status, char * sz_Reserve1,char * sz_ErrCode, char * sz_ErrMsg );
VMS_DBACCESS_API bool VMS_Query5Einfo(char * sz_Model, char * sz_Partno, char * sz_count, char * sz_5Einfo, char * sz_ErrCode, char * sz_ErrMsg );
VMS_DBACCESS_API bool VMS_Query5Ginfo(char * sz_Model, char * sz_Partno, char * sz_count, char * sz_5Ginfo, char * sz_ErrCode, char * sz_ErrMsg );
VMS_DBACCESS_API bool VMS_QueryProjectName(char * sz_model, char * sz_proj,char * sz_ErrCode, char * sz_ErrMsg );

/*20110909,Sunlight add for luna OS_dl*/
VMS_DBACCESS_API bool VMS_QueryPartNumber(char * sz_model, char * sz_partnumber,char * sz_Count,char * sz_ErrCode, char * sz_ErrMsg );

/*20110921, Sunlight and */
VMS_DBACCESS_API bool VMS_QuerySQNinfo_V2(char * sz_Model, char * sz_Partno, char * sz_SWver, st_9JPNData & st_PNinfo, char * sz_ErrCode, char * sz_ErrMsg );

/*20111226,sunlight add*/
VMS_DBACCESS_API bool VMS_QuerySQNinfo_All(char * sz_Model, char * sz_Partno, char * sz_SWver, char * sz_count, char * sz_9Jinfo, char * sz_ErrCode, char * sz_ErrMsg );


/*20120316, Sunlight add */
VMS_DBACCESS_API bool VMS_QueryCommonToolVersion(char * sz_Model,  char * sz_CRtype,char * sz_ToolVer,char * sz_Status, char * sz_ErrCode, char * sz_ErrMsg );

/*20120424, Sunlight add for entrance tool get model list*/
VMS_DBACCESS_API bool VMS_QueryModelProjectList(char * sz_modelprojList,char * sz_count,char * sz_ErrCode, char * sz_ErrMsg );

/*20120611,Luffy add for entrance tool get CR image list*/
VMS_DBACCESS_API bool VMS_QueryCRImageList(char * sz_model,char * sz_imageList,bool b_CR, char * sz_ErrCode, char * sz_ErrMsg);
/*20120913,Luffy add for get normal image list*/
VMS_DBACCESS_API bool VMS_QueryNormalImageList(char * sz_model,char * sz_imageList, char * sz_ErrCode, char * sz_ErrMsg);
/*20130415 Fox add for VRCM*/
VMS_DBACCESS_API bool VMS_QueryImagePNToolVersionStatus(char * sz_model,char * sz_PN,char * sz_FactoryImageVer,char * sz_OSImageVer, char * sz_ToolVerison,char * sz_status ,char * sz_Record_status,char * sz_ErrCode, char * sz_ErrMsg);
/*20130415 Fox add for UI to set paremeter to dll(reserve1 and reserve2)*/
VMS_DBACCESS_API bool VMS_QueryImageVersionReserveValue(char * sz_model,char * sz_ImageVer ,char * sz_reserve1_value,char * sz_reserve2_value,char * sz_ErrCode,char * sz_ErrMsg);
/*20130617, Nancy add for UnifyUI*/
VMS_DBACCESS_API bool VMS_QueryVRCMRImageList(char * sz_model,char * sz_PN,char * sz_ToolVerison,char * sz_FactoryImageList,char * sz_ErrCode, char * sz_ErrMsg);