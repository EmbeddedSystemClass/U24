/******************************************************************************
* Copyright (C) QCS/WGC18                                      
* Module name	: Entrance Tool   
* File name     : DB_Operate.h             
* Summary       : Operate DB                                                              
* Version      Author            Date                 Abstract                      
* 1.0          Tom Li         2011/06/31           First version                        
*****************************************************************************/

#pragma once

#pragma pack (1)
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
}st_9JPNData;
#pragma pack()

class DB_Operate
{
public:
	DB_Operate(void);
	~DB_Operate(void);

	typedef bool (*QuerySO_Info_All)(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char * sz_toolVer, char * sz_facImageVer, char * sz_userImageVer,char * sz_EntranceToolVer,char* sz_errorCode, char* sz_errorMsg);
	typedef bool (*QuerySO_Info)(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char* sz_errorCode, char* sz_errorMsg);
	typedef bool (*QueryStation_Info)(char* sz_pcName, char* sz_line, char* sz_process, char* sz_segment, char* sz_errorCode, char* sz_errorMsg);
	typedef bool (*InsertFAIL_data)(char* sz_id, char* sz_so, char* sz_employId, char* sz_inputErrorCode, char* sz_errorCode, char* sz_errorMsg);
	typedef bool (*InsertPASS_data)(char* sz_id, char* sz_so, char* sz_employId, char* sz_errorCode, char* sz_errorMsg);
	typedef bool (*SetValue)(const char * i_column, const char* sz_value);
    typedef bool (*InsertOOB_data)(char* sz_errorCode, char* sz_errorMsg);
	typedef bool (*SetValue)(const char * i_column, const char* sz_value);
	typedef bool (*ConnectDB)(void);
	typedef void (*CloseDB)(void);
	typedef bool (*CheckRouting)(char * sz_id, char * sz_model, char * sz_process,char * sz_ErrCode, char * sz_ErrMsg);
    typedef bool (*QueryEmployeeID_Right)(char* sz_employId, char * sz_Level,char* sz_ErrCode, char* sz_ErrMsg);
	typedef bool (*QueryEmployeeID_Right_V2)(char* sz_employId, char * sz_Level,char * sz_status,char * sz_wrkstation,char* sz_ErrCode, char* sz_ErrMsg);
	typedef bool (*QuerySegmentProcess)(char * sz_modelname, char * sz_segment,char * sz_process, char * sz_ErrCode,char * sz_ErrMsg);
	//typedef bool (*QuerySO_Info_All)(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char * sz_toolVer, char * sz_facImageVer, char * sz_userImageVer,char* sz_errorCode, char* sz_errorMsg);


	typedef bool (*VMS_ConnectDB)(void);
    typedef void (*VMS_CloseDB)(void);
    //typedef bool (*VMS_QuerySQNinfo)(char * sz_Model, char * sz_Partno, st_9JPNData & st_PNinfo, char * sz_ErrCode, char * sz_ErrMsg );
	typedef bool (*VMS_QuerySQNinfo_V2)(char * sz_Model, char * sz_Partno, char * sz_SWver, st_9JPNData & st_PNinfo, char * sz_ErrCode, char * sz_ErrMsg );
	typedef bool (*VMS_QueryFactoryImageStatus)(char * sz_Model, char * sz_ImageVer,char * sz_Status, char * sz_starttime, char * sz_endtime,char * sz_ErrCode, char * sz_ErrMsg );
    //typedef bool (*VMS_QueryToolStatus)(char * sz_Model, char * sz_ToolVer,char * sz_ImageVer,char * sz_Status, char * sz_ErrCode, char * sz_ErrMsg );
	typedef bool (*VMS_QueryToolStatus)(char * sz_Model, char * sz_ToolVer,char * sz_ImageVer,char * sz_Status, char * sz_Reserve1,char * sz_ErrCode, char * sz_ErrMsg );
	typedef bool (*QuerySO_Info_All_V3)(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char * sz_toolVer, char * sz_facImageVer, char * sz_userImageVer,char * sz_EntranceToolVer,char * sz_Qty,char * sz_CRtype,char* sz_errorCode, char* sz_errorMsg);
	typedef bool (*QuerySO_Info_All_V4)(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char * sz_toolVer, char * sz_facImageVer, char * sz_userImageVer,char * sz_EntranceToolVer,char * sz_Qty,char * sz_CRtype,char * sz_segment/*output*/,char* sz_errorCode, char* sz_errorMsg);
	typedef bool (*QuerySO_Info_All_V5)(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char * sz_toolVer, char * sz_facImageVer, char * sz_userImageVer,char * sz_EntranceToolVer,char * sz_Qty,char * sz_CRtype,char * sz_segment/*output*/,char* sz_Fix_EntTool_ver, char* sz_Fix_Img_ver ,char* sz_errorCode, char* sz_errorMsg);
	//typedef bool (*VMS_QueryCommonToolVersion)(char * sz_Model, char * sz_ToolVer,char * sz_Status, char * sz_ErrCode, char * sz_ErrMsg );
	typedef bool (*VMS_QueryCommonToolVersion)(char * sz_Model/*input*/, char * sz_CRtype/*input*/,char * sz_ToolVer,char * sz_Status, char * sz_ErrCode, char * sz_ErrMsg );
	typedef bool (*UpdateEmployeeID_Status)(char* sz_employId/*input*/, char * sz_status/*input*/,char* sz_wrkstation/*output*/,char* sz_ErrCode/*output*/,char* sz_ErrMsg/*output*/);
	typedef bool (*VMS_QueryModelProjectList)(char * sz_modelprojList/*output*/,char * sz_count/*output*/,char * sz_ErrCode/*output*/, char * sz_ErrMsg/*output*/ );
    typedef bool (*VMS_QuerySQNinfo_All)(char * sz_Model, char * sz_Partno, char * sz_SWver, char * sz_count, char * sz_9Jinfo, char * sz_ErrCode, char * sz_ErrMsg );
	typedef bool (*VMS_QueryImagePNToolVersionStatus)(char * sz_model,char * sz_PN,char * sz_FactoryImageVer, char * sz_OSImageVer,char * sz_ToolVerison,char * sz_status ,char * sz_Record_status,char * sz_ErrCode, char * sz_ErrMsg);
	typedef bool (*QueryConsumeType)(char * sz_model, char * sz_process,char * sz_consume, char * sz_ErrCode,char * sz_ErrMsg);
	typedef bool (*QueryConsumeInfo)(char * sz_sn, char * sz_type,char * sz_times, char * sz_ErrCode,char * sz_ErrMsg);

	VMS_ConnectDB pVMS_ConnectDB;
	 VMS_CloseDB pVMS_CloseDB;
	 VMS_QuerySQNinfo_V2 pVMS_QuerySQNinfo_V2;
	 VMS_QueryFactoryImageStatus pVMS_QueryFactoryImageStatus;
     VMS_QuerySQNinfo_All pVMS_QuerySQNinfo_All;
	 VMS_QueryToolStatus pVMS_QueryToolStatus;


	CloseDB pCloseDB;
	ConnectDB pConnectDB;
	QuerySO_Info pQuerySO_Info;
	QuerySO_Info_All pQuerySO_Info_All;
	QueryStation_Info pQueryStation_Info;
	InsertFAIL_data pInsertFAIL_data;
	InsertPASS_data pInsertPASS_data;
	SetValue pSetValue;
	InsertOOB_data pInsertOOB_data;
	CheckRouting pCheckRouting;
	QueryEmployeeID_Right pQueryEmployeeID_Right;
	QueryEmployeeID_Right_V2 pQueryEmployeeID_Right_V2;
	QuerySegmentProcess pQuerySegmentProcess;
	QuerySO_Info_All_V3 pQuerySO_Info_All_V3;
	QuerySO_Info_All_V4 pQuerySO_Info_All_V4;
	QuerySO_Info_All_V5 pQuerySO_Info_All_V5;
	VMS_QueryCommonToolVersion pVMS_QueryCommonToolVersion;
	UpdateEmployeeID_Status pUpdateEmployeeID_Status;

    VMS_QueryModelProjectList pVMS_QueryModelProjectList;
	VMS_QueryImagePNToolVersionStatus pVMS_QueryImagePNToolVersionStatus;

	QueryConsumeType pQueryConsumeType;
	QueryConsumeInfo pQueryConsumeInfo;

	bool LoadDB();
	bool GetFunadress();


	private:
		bool m_b_loadDB_Res;
		HINSTANCE m_hDLL;
		HINSTANCE m_hDLL_VMS;
};
