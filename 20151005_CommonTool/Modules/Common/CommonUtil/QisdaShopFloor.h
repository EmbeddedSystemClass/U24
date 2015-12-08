#ifndef __QISDASHOPFLOOR_H__
#define __QISDASHOPFLOOR_H__

#include <vector>
#include <afxmt.h>

//======Same definition in DBAccess.h======//
#define  XMLPATHFILE    _T("\\DB\\XML\\")
#define  SO_LENGTH        6
#define  MODEL_LENGTH     7
#define  PN_LENGTH        12 
#define  SOTYPE_LENGTH    1
#define  COLNAME_LENGTH   50
#define  TOOLVERSION_LENGTH   50
#define  FACTORYIMAGEVERSION_LENGTH   50
#define  USERIMAGEVERSION_LENGTH   50
#define  ENTRANCETOOLVERSION_LENGTH   50
#define  QTY_LENGTH   50
#define  CRTYPE_LENGTH   50
//======Same definition in DBAccess.h======//

//======Self definition error code======//
#define ERROR_XMLMISSING				"DB1900"
#define ERROR_XMLNODELISTMISSING		"DB1901"
#define ERROR_SETTOPREDBFAIL			"DB1902"
#define ERROR_WRITESTRINGFAIL			"DB1903"
#define ERROR_OUTOFRANGEFAIL			"DB1904"

#define DefaultStringLength				512
#define DefaultDeviceInput				8

typedef enum DBInfo{
	DBI_SO,
	DBI_SOTYPE,
	DBI_EMPLOYEEID,
	DBI_PROCESS,
	DBI_MODEL,	
	DBI_PN,
	DBI_LINE,
	DBI_SEGMENT,	
	DBI_PCNAME,
	DBI_EMPLOYEELV,
	DBI_TOOLVERSION,
	DBI_FACTORYIMAGEVERSION,
	DBI_USERIMAGEVERSION,
	DBI_ENTRANCETOOLVERSION,
	DBI_QTY,
	DBI_CRTYPE,
	DBI_LAST,
} dbinfo_e;

//Function pointers to DBAccess.dll
typedef bool (*pQuerySO_Info_All_V3)(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char * sz_toolVer, char * sz_facImageVer, char * sz_userImageVer,char * sz_EntranceToolVer,char * sz_Qty,char * sz_CRtype,char* sz_errorCode, char* sz_errorMsg);
typedef bool (*pCheckSO)(char * sz_So/*input*/,char * sz_soCRtype/*input*/,char * sz_picasso/*input*/,char * sz_model/*input*/,char * sz_process/*input*/,char * sz_segment/*input*/, char * sz_ErrCode, char * sz_ErrMsg);
typedef bool (*pInsertCRHoldInfo)(char * sz_picasso/*input*/,char * sz_IMEI/*input*/,char * sz_so/*input*/,char * sz_soCRtype/*input*/, char * sz_model/*input*/,char * sz_process/*input*/, char * sz_segment/*input*/,char * sz_employID,char * sz_partno/*input*/,char * sz_ErrCode, char * sz_ErrMsg); 
typedef bool (*pQuerySO_Info)(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char* sz_errorCode, char* sz_errorMsg);
typedef bool (*pQueryStation_Info)(char * PCname, char* sz_line, char* sz_process, char* sz_segment, char* sz_errorCode, char* sz_errorMsg);
typedef bool (*pInsertFAIL_data)(char* sz_id, char* sz_so, char* sz_employId, char* sz_inputErrorCode, char* sz_errorCode, char* sz_errorMsg);
typedef bool (*pInsertPASS_data)(char* sz_id, char* sz_so, char* sz_employId, char* sz_errorCode, char* sz_errorMsg);
typedef bool (*pSetValue)(const char * sz_index, const char* sz_value);
typedef bool (*pClear_PICSData)();
typedef bool (*pConnectDB)(void);
typedef void (*pCloseDB)(void);
typedef bool (*pCheckRouting_New)(char * sz_id, char * sz_model, char * sz_process,char * sz_ErrCode, char * sz_ErrMsg);
typedef bool (*pQueryEmployeeID_Right)(char* sz_employId, char * sz_Level,char* sz_ErrCode, char* sz_ErrMsg);
typedef bool (*pQuerySOCount_Control)(char * sz_id, char * sz_so, char * sz_process, char * sz_count, char * sz_ErrCode,char * sz_ErrMsg);
typedef bool (*pUpdateEmployeeID_Status)(char* sz_employId, char * sz_status,char * sz_wrkstation, char* sz_ErrCode, char* sz_ErrMsg);
typedef bool (*pQueryholdInfo_V2)(char * sz_id ,char * sz_model,char * sz_process,char * sz_status,char * sz_holdreas,char * sz_ErrCode, char * sz_ErrMsg);
typedef bool (*pQueryIMEI_ByPicasso)(char * sz_picasso ,char * sz_IMEI,char * szErrCode, char * szErrMsg);

class CQisdaShopFloor
{
public:
	CQisdaShopFloor();
	~CQisdaShopFloor();

private:
	CArray <std::vector <CStringA>, char *> m_cstrPICSdata;
	CArray <CStringA, char *> m_cstrDBInfo;
	CArray <CStringA, char *> m_cstrPICSItem;

	CStringA m_ModulePath;

	char m_sz_toolVer[50];
	char m_sz_facImageVer[50];
	char m_sz_userImageVer[50];
	char m_sz_ETtoolVer[50];
	char m_sz_starttime[50];
	char m_sz_endtime[50];
	char m_sz_Status_factory[50];
	char m_sz_sz_Qty[50];
	char m_sz_CRType[50];
	char m_sz_picasso[50];
	char m_sz_PCname[15];
	char m_sz_model[50];
	char m_sz_partNo[50];
	char m_sz_soType[50];
	char m_sz_line[50];
	char m_sz_process[50];
	char m_sz_segment[50];
	
	CCriticalSection m_CritSection_WritePICS;
	CCriticalSection m_CritSection_SetPICS;
	CCriticalSection m_CritSection_CheckRoute;

	HMODULE m_hDBAccess;

	pQuerySO_Info			m_QuerySO_Info;
	pQuerySO_Info_All_V3    m_QuerySO_Info_V3;
	pCheckSO                m_CheckSO;
	pQueryStation_Info		m_QueryStation_Info;
	pInsertFAIL_data		m_InsertFAIL_data;
	pInsertPASS_data		m_InsertPASS_data;
	pSetValue				m_SetValue;
	pClear_PICSData			m_Clear_PICSData;
	pConnectDB				m_ConnectDB;
	pCloseDB				m_CloseDB;
	pCheckRouting_New		m_CheckRouting_New;
	pQueryEmployeeID_Right	m_QueryEmployeeID_Right;
	pInsertCRHoldInfo       m_pInsertCRHoldInfo;
	pQuerySOCount_Control   m_pQuerySOCount_Control;
	pUpdateEmployeeID_Status m_pUpdateEmployeeID_Status;
	pQueryholdInfo_V2 m_pQueryHoldInfo_V2;
	pQueryIMEI_ByPicasso m_pQueryIMEI_ByPicasso;

public:
	const bool LoadDBAccess(char *szErrorMsg);

	const bool GetDBInfo(dbinfo_e nItem, char *szItemBuffer, char *szErrCode, char *szErrMsg);

	void SetPICS(const char *szValue, int nSlot = 0);

	const bool Connect() const;
	void Disconnect() const;
	
	const bool QueryDB(const char* szSO, const char* szEmployee, char* szErrCode, char* szErrMsg);
	const bool CheckRoute(char *szID, char *szErrCode, char *szErrMsg);
	const bool CheckSO(char *szPicaso, char *szErrCode, char *szErrMsg);
	
	const bool Write(bool res, char *szInErrcode, char *szErrCode, char *szErrMsg, int nSlot = 0);

	const int GetPicsItemCount();
	const bool GetPicsItem(int nItem, char *szItemBuffer) const;
	bool SetPics(char *szPicaso);

	//sunlight add 20120412
	bool UpdateEmployeeIDStatus(char *szErrCode, char *szErrMsg);
    bool GetHoldInformation(char *sz_Id,char *sz_model,char *sz_process,char *sz_status,char *sz_holdreas,char *szErrCode,char *szErrMsg);
private:
	const bool ParsingXML(char *szErrCode, char *szErrMsg);
	const bool WriteToPicsFile(bool res, char *szInErrcode, int nSlot);
	const bool WriteToQisdaShopFloor(bool res, char *szInErrcode, char *szErrCode, char *szErrMsg, int nSlot);
};

#endif