/***************************************************************************
* Copyright (C) : QCS/WGC18                                      
* Module name   : DB Access API 
* File name     : DBAccess_API.h             
* Summary       : DB Access API.                                                  
* Version     Author             Date                 Abstract                      
* 1.0         Gilpin.Xi          2011/07/13           First version  
* 1.1		  Luffy.Liu			 2011/12/27			  add api
****************************************************************************/

#pragma once



/* PreDB */
typedef bool (*pQuerySO_Info)(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char* sz_errorCode, char* sz_errorMsg);
typedef bool (*pQuerySO_Info_All)(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char * sz_toolVer, char * sz_facImageVer, char * sz_userImageVer,char * sz_EntranceToolVer,char* sz_errorCode, char* sz_errorMsg);

typedef bool (*pQueryStation_Info)(char * PCname, char* sz_line, char* sz_process, char* sz_segment, char* sz_errorCode, char* sz_errorMsg);
typedef bool (*pInsertFAIL_data)(char* sz_id, char* sz_so, char* sz_employId, char* sz_inputErrorCode, char* sz_errorCode, char* sz_errorMsg);
typedef bool (*pInsertPASS_data)(char* sz_id, char* sz_so, char* sz_employId, char* sz_errorCode, char* sz_errorMsg);
typedef bool (*pInsertdata_Common)(char * sz_XMLname,char * sz_tablename, char* sz_errorCode, char* sz_errorMsg);
typedef bool (*pInsertOOB_data)(char * sz_result,char * sz_InputErrCode, char* sz_errorCode, char* sz_errorMsg);
typedef bool (*pSetValue)(const char * sz_index, const char* sz_value);
typedef bool (*pClear_PICSData)();
typedef bool (*pConnectDB)(void);
typedef void (*pCloseDB)(void);
typedef bool (*pCheckRouting)(char * sz_id, char * sz_model, char * sz_process,char * sz_ErrCode, char * sz_ErrMsg);
typedef bool (*pQuerySOCount)(char * sz_id, char * sz_so, char * sz_process, char * sz_count,char * sz_ErrCode, char * sz_ErrMsg);
typedef bool (*pQueryEmployeeID_Right)(char* sz_employId, char * sz_Level,char* sz_ErrCode, char* sz_ErrMsg);
typedef bool (*pQueryPNinBACH_BySO)(char * sz_PN, char * sz_SO, char * sz_qty,char * sz_ErrCode, char * sz_ErrMsg);
typedef bool (*pQueryKeyparts_Info)(char * sz_PN, char * sz_Model,char * sz_Process,char * sz_Count, char * sz_Info, char * sz_ErrCode, char * sz_ErrMsg);
typedef bool (*pQueryOOBErrCode_Info)(char * sz_model,char * sz_Process, char * sz_ErrCode, char * sz_ErrMsg);
typedef bool (*pGetPicasso)(char * sz_InputID, char * sz_Model,char * sz_Picasso,  char* sz_partno, char * sz_ErrCode, char * sz_ErrMsg);
typedef bool (*pQueryRecordBySO)(char * sz_so, char * sz_process, char * sz_wrkstation, char * sz_totalCount, char * sz_passCount,char * sz_failCount,char * sz_ErrCode,char * sz_ErrMsg);
typedef bool (*pQueryFirstProcess)(char * sz_modelname, char * sz_process, char * sz_ErrCode, char * sz_ErrMsg);
typedef bool (*pQuerySegmentProcess)(char * sz_modelname, char * sz_segment,char * sz_process, char * sz_ErrCode,char * sz_ErrMsg);
typedef bool (*pInsertKeypartsInfo)(char * sz_id, char * sz_process,char * sz_kpid, char * sz_kpname,char * sz_empolyee,char * sz_pn,char * sz_ErrCode,char * sz_ErrMsg);
typedef bool (*pQueryIMEIresult)(char * sz_InputID, char * sz_Model,char * sz_Picasso, char * sz_ErrCode, char * sz_ErrMsg);
typedef bool (*pCheckPicassoStatus) (char * sz_Picasso, char * sz_TimeInfo,char * sz_Status, char * sz_ErrCode, char * sz_ErrMsg);
typedef bool (*pInsertPicassoControl)(char * sz_Picasso, char * sz_TimeInfo, char *sz_status, char * sz_ErrCode, char * sz_ErrMsg);
typedef bool (*pQueryWifiLoginInfo)(char * sz_Imei, char * sz_Mac, char *sz_LoginWifiInfo, char * sz_ErrCode,char * sz_ErrMsg);
typedef bool (*pCheckPicassoDuplicate_SecondStation)(char * sz_Picasso, char * sz_TimeInfo, char * sz_ErrCode, char * sz_ErrMsg);
typedef bool (*pCheckPicassoDuplicate)(char * sz_Picasso, char * sz_FAPicasso, char * sz_TimeInfo, char * sz_FATimeInfo, char * sz_status,char * sz_ErrCode, char * sz_ErrMsg);
typedef bool (*pCheckNAL_duplicate)(char * sz_IMEI/*input*/, char * sz_NAL/*input*/, char * sz_ErrCode/*output*/, char * sz_ErrMsg/*output*/);
typedef bool (*pCheckNAL_duplicate_V2)(char * sz_process/*input*/, char * sz_IMEI/*input*/, char * sz_NAL/*input*/, char * sz_ErrCode/*output*/, char * sz_ErrMsg/*output*/);
typedef bool (*pQuerySOCount_Control)(char * sz_id, char * sz_so, char * sz_process, char * sz_count,char * sz_ErrCode, char * sz_ErrMsg);
typedef bool (*pQuerySO_Info_All_V2)(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char * sz_toolVer, char * sz_facImageVer, char * sz_userImageVer,char * sz_EntranceToolVer,char * sz_Qty,char* sz_errorCode, char* sz_errorMsg);
typedef bool (*pQueryNAL)(char * sz_IMEI, char * sz_NAL, char * sz_ErrCode, char * sz_ErrMsg);
typedef bool (*pQueryIfTestAgain)(char * sz_id, char * sz_model,char * sz_process, char * sz_count,char * sz_ErrCode,char * sz_ErrMsg);
typedef bool (*pInsertTSData_forTS)(char * sz_id, char * sz_model,char * sz_rtnprocess, char * sz_ngprocess, char * sz_ngwrkstation, char * sz_errcode, char * sz_errmsg);
typedef bool (*pQueryholdInfo)(char * sz_id ,char * sz_model,char * sz_process,char * sz_status,char * sz_holdreas,char * sz_ErrCode, char * sz_ErrMsg);
typedef bool (*pQueryIMEI_ByPicasso)(char * sz_picassoid ,char * sz_IMEI,char * sz_ErrCode, char * sz_ErrMsg);
typedef bool (*pQueryIMEIStatus)(char * sz_InputID,char * sz_status,char * sz_cartonID,char * sz_ErrCode,char * sz_ErrMsg);

typedef bool (*pQuerySO_Info_All_V3)(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char * sz_toolVer, char * sz_facImageVer, char * sz_userImageVer,char * sz_EntranceToolVer,char * sz_Qty,char * sz_CRtype,char* sz_errorCode, char* sz_errorMsg);
/* Nancy.yt.Zhu 2012.11.27 */
typedef bool (*pQuerySO_Info_All_V5)(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char * sz_toolVer, char * sz_facImageVer, char * sz_userImageVer,char * sz_EntranceToolVer,char * sz_Qty,char * sz_CRtype,char* sz_segment, char* sz_Fix_EntTool_ver, char* sz_Fix_Img_ver,char* sz_errorCode, char* sz_errorMsg);
typedef bool (*pCheckSO)(char * sz_So/*input*/,char * sz_soCRtype/*input*/,char * sz_picasso/*input*/,char * sz_model/*input*/,char * sz_process/*input*/,char * sz_segment/*input*/, char * sz_ErrCode, char * sz_ErrMsg);
typedef bool (*pInsertCRHoldInfo)(char * sz_picasso/*input*/,char * sz_IMEI/*input*/,char * sz_so/*input*/,char * sz_soCRtype/*input*/, char * sz_model/*input*/,char * sz_process/*input*/, char * sz_segment/*input*/,char * sz_employID/*input*/,char * sz_partno/*input*/,char * sz_ErrCode, char * sz_ErrMsg); 
typedef bool (*pCheckNAL_Validity)(char * sz_pn/*input*/ ,char * sz_NAL/*input*/,char * sz_ErrCode, char * sz_ErrMsg);
typedef bool (*pUpdateEmployeeID_Status)(char* sz_employId/*input*/, char * sz_status/*input*/,char * sz_wrkstation/*input*/, char* sz_ErrCode/*output*/, char* sz_ErrMsg/*output*/);

typedef bool (*pQueryOOBSampleRate)(char * sz_model,char* sz_so,char * sz_sampleRate,char* sz_errorCode, char* sz_errorMsg);
typedef bool (*pInsertOOBStatus)(char * sz_model, char * sz_so, char * sz_pn, char * sz_IMEI,char* sz_errorCode, char* sz_errorMsg);

typedef bool (*pUnlinkKeyparts)(char * sz_id, char * sz_model, char * sz_ErrCode, char * sz_ErrMsg);
typedef bool (*pQueryApkTestItem_V2)(char * sz_PN, char * sz_Item,char * sz_SDStatus,char* sz_Switch, char * sz_ErrCode,char * sz_ErrMsg);

typedef bool (*pGetProcessResult)(char * sz_id, char * sz_model,char * sz_process);

/* VMS DB */
typedef bool (*pVMS_ConnectDB)(void);
typedef void (*pVMS_CloseDB)(void);
typedef bool (*pVMS_QuerySQNinfo_All)(char * sz_Model, char * sz_Partno, char * sz_SWver, char * sz_count, char * sz_9Jinfo, char * sz_ErrCode, char * sz_ErrMsg );
typedef bool (*pVMS_Query5Einfo)(char * sz_Model, char * sz_Partno, char * sz_count, char * sz_5Einfo, char * sz_ErrCode, char * sz_ErrMsg );
typedef bool (*pVMS_Query5Ginfo)(char * sz_Model, char * sz_Partno, char * sz_count, char * sz_5Ginfo, char * sz_ErrCode, char * sz_ErrMsg );
typedef bool (*pVMS_QueryCRImageList)(char * sz_model,char * sz_imageList,bool b_CR,char * sz_ErrCode, char * sz_ErrMsg);
/* Nancy.yt.Zhu 2012.09.13 */
typedef bool (*pVMS_QueryNormalImageList)(char * sz_model,char * sz_imageList,char * sz_ErrCode, char * sz_ErrMsg);
typedef bool (*pVMS_QueryImageVersionReserveValue)(char * sz_model,char * sz_ImageVer ,char * sz_reserve1_value,char * sz_reserve2_value,char * sz_ErrCode,char * sz_ErrMsg);
typedef bool (*pVMS_QueryVRCMRImageList)(char * sz_model,char * sz_PN,char * sz_ToolVerison,char * sz_FactoryImageList,char * sz_ErrCode, char * sz_ErrMsg);

//Magan 20140508
typedef bool (*pQueryIDType)(char* szpartno, char* szIDtype, char* szLength, char* sz_ErrCode, char* sz_ErrMsg);