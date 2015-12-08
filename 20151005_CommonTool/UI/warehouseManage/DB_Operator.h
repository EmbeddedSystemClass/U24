#pragma once
class DB_operator//11
{
public:
	DB_operator(void);
	~DB_operator(void);

public:
	bool GetFunadress();
	bool FconnectDB();
	bool FcloseDB();

	typedef bool (*ConnectDB)(void);
	typedef void (*CloseDB)(void);
	typedef bool (*QueryOOBstatus)(char * sz_IMEI,char* sz_status,char* sz_errorCode, char* sz_errorMsg);
	typedef bool (*UpdateOOBStatusToFalse)(char * sz_IMEI,char* sz_errorCode, char* sz_errorMsg);
	typedef bool (*QueryOOBResult)(char * sz_IMEI, char * sz_result,char* sz_errorCode, char* sz_errorMsg);
//(char * sz_IMEI,char* sz_errorCode, char* sz_errorMsg);
	typedef bool (*InsertPTOOBRework)(char * sz_IMEI,char* sz_errorCode, char* sz_errorMsg);
	typedef bool (*CheckRouting)(char * sz_id, char * sz_model, char * sz_process,char * sz_ErrCode, char * sz_ErrMsg);
	typedef bool (*GetIMEI_Info)(char * sz_InputID, char * sz_model,char * sz_Picasso, char * sz_partno,char * sz_SO, char * sz_sw,char * sz_ErrCode, char * sz_ErrMsg);
	typedef bool (*QuerySegmentProcess)(char * sz_modelname, char * sz_segment,char * sz_process, char * sz_ErrCode,char * sz_ErrMsg);
	typedef bool (*GetProcessResult)(char * sz_id, char * sz_model,char * sz_process);
	typedef bool (*QuerySO_Info_All)(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char * sz_toolVer, char * sz_facImageVer, char * sz_userImageVer,char* sz_errorCode, char* sz_errorMsg);
	typedef bool (*QuerySO_Info_All_V2)(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char * sz_toolVer, char * sz_facImageVer, char * sz_userImageVer,char * sz_EntranceToolVer,char * sz_Qty,char* sz_errorCode, char* sz_errorMsg);
	typedef bool (*QueryIMEI_ByCarton)(char * sz_CartonID, char * sz_count,char * sz_IMEIlist, char * sz_ErrCode, char * sz_ErrMsg);
	typedef bool (*QueryIMEIStatus)(char * sz_InputID,char * sz_status,char * sz_cartonID, char * sz_ErrCode,char * sz_ErrMsg);

	typedef bool  (*Insert_WareHouseIn)   (char* sz_picasso,char* sz_imei,char* sz_so, char* sz_partNo,char* sz_sw,char * sz_Qty ,char* sz_CartonID, char* sz_ErrCode, char* sz_ErrMsg);
	typedef bool  (*Update_WareHouseOut)  (char* sz_picasso,char* sz_imei,char* sz_ReworkReason, char* sz_DOARMANo, char* sz_ErrCode, char* sz_ErrMsg);
	typedef bool  (*Query_WareHouseStatus)(char* sz_picasso,char* sz_imei,char* sz_Status, char* sz_ErrCode, char* sz_ErrMsg);
	typedef bool  (*Query_WareHouseStatus_V2)(char* sz_picasso,char* sz_imei,char* sz_Status, char* szInsertDate, char* szInsertTime, char* sz_ErrCode, char* sz_ErrMsg);
	typedef bool  (*InsertWareHouseOutHoldInfo)(char * sz_picasso,char * sz_IMEI, char * sz_model,char * sz_partno,char * sz_ErrCode, char * sz_ErrMsg);

	typedef bool (*Query_IDType)(char* szpartno, char* szIDtype, char* szLength, char* sz_ErrCode, char* sz_ErrMsg);

	CloseDB pCloseDB;
	ConnectDB pConnectDB;
	QueryOOBstatus pQueryOOBstatus;
	UpdateOOBStatusToFalse pUpdateOOBStatusToFalse;
	QueryOOBResult pQueryOOBResult;
	InsertPTOOBRework pInsertPTOOBRework;
	CheckRouting pCheckRouting;
	GetIMEI_Info pGetIMEI_Info;
	QuerySegmentProcess pQuerySegmentProcess;
	GetProcessResult pGetProcessResult;
	QuerySO_Info_All pQuerySO_Info_All;
	QuerySO_Info_All_V2 pQuerySO_Info_All_V2;
	QueryIMEI_ByCarton pQueryIMEI_ByCarton;
	QueryIMEIStatus pQueryIMEIStatus;

	Insert_WareHouseIn pInsert_WareHouseIn;
	Update_WareHouseOut pUpdate_WareHouseOut;
	Query_WareHouseStatus pQuery_WareHouseStatus;
	Query_WareHouseStatus_V2 pQuery_WareHouseStatus_V2;
	InsertWareHouseOutHoldInfo pInsertWareHouseOutHoldInfo;

	Query_IDType pQuery_IDType;

	bool m_b_loadDB_Res;
	HINSTANCE m_hDLL;
};
