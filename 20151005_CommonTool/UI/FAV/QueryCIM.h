#pragma once
#pragma warning(disable:4996)
#include "DBAccess_API.h"
#include "CommDef.h"
#include <map>

class CQueryCIM
{
public:
	CQueryCIM(void);
	~CQueryCIM(void);

public:
	st_UIIDparameter m_st_idParameter;

private:
	HMODULE m_h_DBModule;

	CString m_str_errorCode;
	CString m_str_errorMsg;
	CString m_str_result; //UI will set error code to this: SetErrorCodeToCIM

	std::map<CString,CString> m_map_PICSDataFromUI;
	std::map<CString,CString> m_map_picsData;
	int m_i_SoQty;
	CString m_str_computerName;
	int m_i_totalNo;
	int m_i_passNo;
	int m_i_failNo;

	static CCriticalSection m_obj_DBLock;

public:
	/* Load DB operate DLL */
	bool LoadDBAccessDLL();
	bool UpLoadDBAccessDLL();

	bool ConnectDataBase();
	bool CloseDataBase();

	bool QueryDataFromCIM();
	bool QuerySOInfo_ALL_V5();
	bool QueryStationInfo();
	bool QueryEmployeeAuthority();
	bool QueryIDtype(void);
	bool QueryOOBErrCodeInfo();
	bool QueryApkTestItem_ShippingOS();

	bool QueryKeypartsInfoFromCIM(CString &str_keyPartsInfo, int &i_keyPartCount);
	bool QueryKeypartsInfo(CString &str_keyPartsInfo, int &i_keyPartCount);

	bool InsertTestRecordToCIM( bool b_pass );
	bool InsertTestRecordCIM(bool b_pass);

	bool InsertOOBDataToCIM(char * sz_result);
	bool InsertOOBData(char * sz_result);
	
	bool UnlinkKeypartsFromCIM(char * sz_id);
	bool UnlinkKeyparts(char * sz_id);

	bool QueryIMEIStatusFromCIM(char * sz_InputID, char * sz_status);
	bool QueryIMEIStatus(char * sz_InputID, char * sz_status);

	bool QueryholdInfoFromCIM( char * sz_id);
	bool QueryholdInfo( char * sz_id);

	bool QueryPNinBACHBySOFromCIM(char * sz_keyPartPN, char * sz_qty);
	bool QueryPNinBACHBySO(char * sz_keyPartPN, char * sz_qty);

	bool QueryWifiLoginInfoFromCIM( char * sz_InputID, char * sz_WifiMac,char * sz_LoginWifiInfo );
	bool QueryWifiLoginInfo( char * sz_InputID, char * sz_WifiMac,char * sz_LoginWifiInfo );

	bool QueryPicassoFromCIM(char * sz_InputID, char * sz_Picasso);
	bool QueryPicasso( char * sz_InputID, char * sz_Picasso );

	bool QueryRecordInfoFromCIM();
	bool QueryRecordInfo();

	bool QueryStationRoutingInfoFromCIM(char *p_sz_picasso,bool b_isOOBSpecialTest);
	bool QueryStationRoutingInfo(char *p_sz_picasso,bool b_isOOBSpecialTest);
	bool CheckSO(char *sz_picasso);
	bool QuerySOCount(char * sz_id);

	bool UpdateEmployeeID_StatusFromCIM();
	bool UpdateEmployeeID_Status();

	void GetUIIDParameter(st_UIIDparameter &st_idParameter);
	void SetidParameter(st_UIIDparameter st_idParameter);

	CString GetErrorMsg();//Set m_str_errorMsg to UI
	void SetErrorCodeToCIM(const CString str_result);//m_str_result = str_result
	void SetPICSData( std::map<CString, CString> map_PICSData );

	int  GetTotalNo();
	int  GetPassNo();
	int  GetFailNo();

};
