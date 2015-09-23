#pragma once

#include "CommDef.h"
#include "DBAccess_API.h"
#include "WriteLog.h"

class CQueryCIM
{
public:
	CQueryCIM(void);
	~CQueryCIM(void);
public:
	/* Load DB operate DLL */
	bool LoadDBAccessDLL();
	bool UpLoadDBAccessDLL();

	
	CString GetErrorCode();
	CString GetErrorMsg();

	int  GetTotalNo();
	int  GetPassNo();
	int  GetFailNo();
	//20120419,luffy.liu
	bool IsCheckOOB();
	void SetSTResult(st_pageReturn st_result);
	void SetPicassoMap(std::map<int, CString> map_picasso);
	void SetslotPICSData(std::map<int, std::map<CString, CString>> map_slotPICSData);
	void SetidParameter(st_UIIDparameter st_idParameter);
	bool GetUIIDParameter(st_UIIDparameter &st_idParameter);
	bool GetStationListMap(std::map<CString, CString> &map_StationVsIndex);
	//20111227,by luffy
	bool GetTestKeyValue(std::map<CString, CString> &map_TestKeyValue);

	CString GetComputerName();
	void SetComputerName(CString str_computername);
	
	bool QueryStationRoutingInfoFromCIM(char *p_sz_picasso);
	bool QueryDataFromCIM();
	bool InsertTestRecordToCIM(bool b_pass);
	bool CheckPicassoStatusFromCIM(char * sz_Picasso, char * sz_TimeInfo,char * sz_Status);
	bool CheckPicassoDuplicateFromCIM(char * sz_Picasso, char * sz_TimeInfo);
	bool InsertPicassoControlToCIM(char * sz_Picasso, char * sz_TimeInfo,char * sz_Status);
	bool QueryRecordInfoFromCIM();
	bool QueryIMEIresultFromCIM(char * sz_InputID, char * sz_Model,char * sz_Picasso);

	bool QueryIMEIStatusFromCIM(char * sz_InputID, char * sz_status);//nancy.yang 20120629
	bool QueryPicassoFromCIM(char * sz_InputID, char * sz_Model,char * sz_Picasso);//nancy.yang 20120629
	bool UnlinkKeypartsFromCIM(char * sz_id, char * sz_model);//nancy.yang 20120629

	bool CheckPicassoAndMobileIDFromCIM( char *sz_picasso, char *sz_mobileid, char *sz_picassoUI );
	bool CheckNALFromCIM(char * sz_IMEI, char * sz_NAL);

	//luffy,20111213
	bool QueryNALFromCIM(char * sz_IMEI, char * sz_NAL);
	//luffy,20111215
	bool QueryIfTestAgainFromCIM( char * sz_id, char* sz_model, char * sz_process, char *sz_cnt);
	bool InsertTS_DataToCIM(char * sz_id, char * sz_model,char * sz_rtnprocess, char * sz_ngprocess, char * sz_ngwrkstation);
	bool QueryholdInfoFromCIM( char * sz_id);
	bool UpdateEmployeeID_StatusFromCIM();
	
	/*for vms db*/
 	bool LoadVMSDBAccessDLL();
 	bool UnLoadVMSDBAccessDLL();
 	bool VMS_QueryDataFromSQN();

	bool QueryDataFromSQN();
private:

	bool ConnectDataBase();
	bool CloseDataBase();
	/* DB Access operate functions */
	bool QueryEmployeeAuthority();
	bool QuerySOInfo();
	bool QuerySOInfo_ALL();
	bool QueryStationInfo();
	bool QueryFirstProcessInfo();
	bool QueryRecordInfo();
	//luffy,20110819
	bool QueryStationList();
	bool QueryStationRoutingInfo(char *p_sz_picasso);
	bool QueryStationRoutingInfoCS(char *p_sz_picasso);

	bool InsertTestRecordCIM(bool b_pass);
	bool CheckPicassoStatusCIM(char * sz_Picasso, char * sz_TimeInfo,char * sz_Status);
	bool CheckPicassoDuplicate_SndStationCIM(char * sz_Picasso, char * sz_TimeInfo);
	bool InsertPicassoControlCIM(char * sz_Picasso, char * sz_TimeInfo,char * sz_Status);
	bool QueryIMEIresult(char * sz_InputID, char * sz_Model,char * sz_Picasso);
	bool QueryIMEIStatus(char * sz_InputID, char * sz_status);//nancy.yang 20120629
	bool QueryPicasso(char * sz_InputID, char * sz_Model,char * sz_Picasso);//nancy.yang 20120629
	bool CheckNAL_duplicate(char * sz_IMEI, char * sz_NAL);
	bool CheckNAL_duplicate_V2(char * sz_IMEI, char * sz_NAL);//nancy.yang 20120629
	bool CheckNAL_Valide(char * sz_NAL );
	//luffy,20111213
	bool QuerySOCount(char * sz_id);
	bool QueryNAL(char * sz_IMEI, char * sz_NAL);
	//luffy,20111214
	bool QueryIfTestAgain( char * sz_id, char* sz_model, char * sz_process, char *sz_cnt);
	//luffy,20120104
	bool InsertTS_Data(char * sz_id, char * sz_model,char * sz_rtnprocess, char * sz_ngprocess, char * sz_ngwrkstation);
	bool QueryholdInfo(char * sz_id);
	//luffy,20120222
	bool CheckSO(char *sz_picasso);
	bool QuerySOInfo_ALL_V3();
	//luffy.liu,20120410
	bool UpdateEmployeeID_Status();
	//luffy.liu,20120419
	bool QueryOOBSampleRate();
	bool InsertOOBStatus(char * sz_IMEI);
	//Nancy.yt.Zhu,20121127
	bool QuerySOInfo_ALL_V5();

	bool UnlinkKeyparts(char * sz_id, char * sz_model);

	/*for vms db,luffy,20111209*/
 	bool ConnectVMSDataBase();
 	bool CloseVMSDataBase();
 	bool VMS_QuerySQNinfo_All(char * sz_Model, char * sz_Partno, char * sz_SWver,char *sz_count, char *sz_pnInfo );
	bool VMS_Query5Einfo(char * sz_Model, char * sz_Partno,char *sz_count, char *sz_pnInfo );
	bool VMS_Query5Ginfo(char * sz_Model, char * sz_Partno,char *sz_count, char *sz_pnInfo );
	bool VMS_QueryCRImageList(char * sz_model,char * sz_imageList,bool b_CR);
	/* Nancy.yt.Zhu 2012.09.13 */
	bool VMS_QueryNormalImageList( char * sz_model,char * sz_imageList);
    /* Fox 2012.04.18 */
	bool VMS_QueryImageVersionReserveValue(char * sz_model,char * sz_imageVer);

	bool VMS_QueryVRCMRImageList( char * sz_model,char * sz_PN,char * sz_ToolVerison,char * sz_FactoryImageList);
	bool SplitStringToMap(char *sz_count, char *sz_info);
	CString ConvertModelName(CString str_modelname);
	void Log(CString str_content);
	CString StringIntersection(CString strOne,CString strTwo);
private:
	HMODULE m_h_DBModule;

	HMODULE m_h_VMSDBModule;

	CString m_str_errorCode;
	CString m_str_errorMsg;

	CString m_str_computerName;
	CString m_str_IMEI;
	//Nancy.yt.Zhu,20121127
	CString m_str_FixImgVer;

	CString m_str_BA1/*,m_str_CS1*/;
	bool    m_b_csFlag;

	st_UIIDparameter m_st_idParameter;
	st_pageReturn m_st_result;
	std::map<CString, CString> m_map_picsData;
	std::map<int, CString> m_map_picasso;
	std::map<int, std::map<CString, CString>> m_map_slotPICSData;
	std::map<CString, CString> m_map_StationIndexMap;
	std::map<CString, CString> m_map_TestKeyValue;

	int m_i_totalNo;
	int m_i_passNo;
	int m_i_failNo;

	int m_i_SoQty;
	int m_i_SampleRate;
	bool m_b_OOBCheck;

	static CCriticalSection m_obj_DBLock;

	CWriteLog m_obj_log;
public:
	// Query ID type from sssctd to sure id length in IMEI/FCHECK 
	bool QueryIDtype(void);
};
