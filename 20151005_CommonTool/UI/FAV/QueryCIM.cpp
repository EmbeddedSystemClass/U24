#include "StdAfx.h"
#include "QueryCIM.h"

CCriticalSection CQueryCIM::m_obj_DBLock;
CQueryCIM::CQueryCIM(void)
{
	m_i_SoQty = 0;
	m_h_DBModule = NULL;
}

CQueryCIM::~CQueryCIM(void)
{
	UpLoadDBAccessDLL();
}

bool CQueryCIM::UpLoadDBAccessDLL()
{

	if (m_h_DBModule != NULL)
	{
		FreeLibrary (m_h_DBModule);
		m_h_DBModule = NULL;
	}
	return true;
}

bool CQueryCIM::LoadDBAccessDLL()
{
	/* Load library */
	if (m_h_DBModule == NULL)
	{
		m_h_DBModule = ::LoadLibrary(_T("DBAccess.dll"));

		if (m_h_DBModule == NULL)
		{
			return false;
		}
	}

	return true;
}

bool CQueryCIM::ConnectDataBase()
{
	pConnectDB ConnectDB = NULL;
	ConnectDB = (pConnectDB)::GetProcAddress(m_h_DBModule, "ConnectDB");
	if (ConnectDB == NULL)
	{
		//m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nLoad CIM DB dll ConnectDB=NULL");
		return false;
	}

	/* Connect DataBase */
	if (!ConnectDB())
	{
		//m_str_errorCode = CommErr_UI_CIM_Connect_Fail;
		m_str_errorMsg = _T("连接CIM数据库失败!\nConnect CIM Database fail!");
	    return false;
	}

	return true;

}

bool CQueryCIM::CloseDataBase()
{
	pCloseDB CloseDB = NULL;
	CloseDB = (pCloseDB)::GetProcAddress(m_h_DBModule, "CloseDB");
	if (m_h_DBModule == NULL)
	{
		//m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nm_h_DBModule == NULL!");
		return false;
	}
	if (CloseDB == NULL)
	{
		//m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nCloseDB = NULL!");

		return false;
	}

	/* Close DataBase */
	CloseDB();

	return true;
}

void CQueryCIM::GetUIIDParameter( st_UIIDparameter &st_idParameter )
{
	st_idParameter = m_st_idParameter;
}

void CQueryCIM::SetidParameter( st_UIIDparameter st_idParameter )
{
	m_st_idParameter = st_idParameter;
}

CString CQueryCIM::GetErrorMsg( )
{
	CString str_tmp = m_str_errorMsg;
	//reset after use
	m_str_errorMsg = _T("");
	return  str_tmp; 
}

void CQueryCIM::SetErrorCodeToCIM(const CString str_result)
{
	m_str_result = str_result;
}

void CQueryCIM::SetPICSData( std::map<CString, CString> map_PICSData )
{
	m_map_PICSDataFromUI = map_PICSData;
}

bool CQueryCIM::QueryDataFromCIM()
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);
	bool b_res = true;

	if (b_res)
	{
		b_res = ConnectDataBase();
	}

	if (b_res)
	{
		b_res = QuerySOInfo_ALL_V5();
	}

	if (b_res)
	{
		b_res = QueryStationInfo();
	}
	
	if (b_res)
	{
		b_res = QueryEmployeeAuthority();
	}

	if (b_res)
	{
		b_res = QueryOOBErrCodeInfo();
	}

	if (b_res)
	{
		if (m_st_idParameter.str_station == _T("SHIPPINGOS"))
		{
			b_res = QueryApkTestItem_ShippingOS();
		}
	}

	CloseDataBase();

	return b_res;
}

bool CQueryCIM::QueryKeypartsInfoFromCIM(CString &str_keyPartsInfo, int &i_keyPartCount)
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);
	if(ConnectDataBase())
	{
		if (QueryKeypartsInfo(str_keyPartsInfo,i_keyPartCount))
		{
			return CloseDataBase();
		}
	}
	CloseDataBase();
	return false;
}

bool CQueryCIM::InsertTestRecordToCIM( bool b_pass )
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);
	if(ConnectDataBase())
	{
		bool bRet = InsertTestRecordCIM(b_pass);

		if (bRet)
		{
			bRet = CloseDataBase();
		}

		return bRet;
	}

	CloseDataBase();

	return false;
}

bool CQueryCIM::InsertOOBDataToCIM(char * sz_result)
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);
	if(ConnectDataBase())
	{
		if (InsertOOBData(sz_result))
		{
			return CloseDataBase();
		}
	}
	CloseDataBase();
	return false;

}

bool CQueryCIM::UnlinkKeypartsFromCIM(char * sz_id)
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);
	if(ConnectDataBase())
	{
		if (UnlinkKeyparts(sz_id))
		{
			return CloseDataBase();
		}
	}
	CloseDataBase();
	return false;
}

bool CQueryCIM::QueryPNinBACHBySOFromCIM(char * sz_keyPartPN, char * sz_qty)
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);
	if(ConnectDataBase())
	{
		if (QueryPNinBACHBySO(sz_keyPartPN,sz_qty))
		{
			return CloseDataBase();
		}
	}
	CloseDataBase();
	return false;
}
bool CQueryCIM::QueryholdInfoFromCIM( char * sz_id)
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);
	if(ConnectDataBase())
	{
		if (QueryholdInfo(sz_id))
		{
			return CloseDataBase();
		}
	}
	CloseDataBase();
	return false;
}

bool CQueryCIM::QueryPicassoFromCIM(char * sz_InputID,char * sz_Picasso)
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);
	if(ConnectDataBase())
	{
		if (QueryPicasso(sz_InputID,sz_Picasso))
		{
			return CloseDataBase();
		}
	}
	CloseDataBase();
	return false;
}

bool CQueryCIM::QueryIMEIStatusFromCIM(char * sz_InputID, char * sz_status)
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);
	if(ConnectDataBase())
	{
		if (QueryIMEIStatus(sz_InputID,sz_status))
		{
			return CloseDataBase();
		}
	}
	CloseDataBase();
	return false;
}

bool CQueryCIM::QueryWifiLoginInfoFromCIM(char * sz_InputID, char * sz_WifiMac,char * sz_LoginWifiInfo)
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);
	if(ConnectDataBase())
	{
		if (QueryWifiLoginInfo(sz_InputID,sz_WifiMac,sz_LoginWifiInfo))
		{
			return CloseDataBase();
		}
	}
	CloseDataBase();
	return false;
}

bool CQueryCIM::QueryStationRoutingInfoFromCIM(char *p_sz_picasso,bool b_isOOBSpecialTest )
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);

	bool b_ret =  ConnectDataBase();

	//1.check so count,no matter SO type is
	if(b_ret)
	{
		//Log(_T("1.Begin to check so count"));
		b_ret = QuerySOCount(p_sz_picasso);
	}

	//2.query picasso if hold by QA
	if (b_ret)
	{
		//Log(_T("2.Begin to check if hold by QA"));
		b_ret = QueryholdInfo(p_sz_picasso);
	}

	//3.check test routing
	if(b_ret)
	{
		
		//Log(_T("3.Facotory tool check routing"));
		b_ret = QueryStationRoutingInfo(p_sz_picasso, b_isOOBSpecialTest);
	
	}
	
	//4.check CR/BO so whether same with first station at each line
	if (b_ret)
	{
		//Log(_T("4.CR SO,begin to check so if same with the first station"));
		b_ret = CheckSO(p_sz_picasso);
	}

	CloseDataBase();
	return b_ret;
}

bool CQueryCIM::UpdateEmployeeID_StatusFromCIM()
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);
	if(ConnectDataBase())
	{
		if (UpdateEmployeeID_Status())
		{
			return CloseDataBase();
		}
	}
	CloseDataBase();
	return false;
}

bool CQueryCIM::QueryRecordInfoFromCIM()
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);
	if(ConnectDataBase())
	{
		if (QueryRecordInfo())
		{
			return CloseDataBase();
		}
	}
	CloseDataBase();
	return false;
}

int CQueryCIM::GetTotalNo()
{
	return m_i_totalNo;
}

int CQueryCIM::GetPassNo()
{
	return m_i_passNo;
}

int CQueryCIM::GetFailNo()
{
	return m_i_failNo;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CQueryCIM::QuerySOInfo_ALL_V5()
{
	USES_CONVERSION;

	char sz_SO[10] = {0};
	char sz_model[15] = {0};
	char sz_PN[15] = {0};
	char sz_SOType[15] = {0};
	char sz_ToolVersion[15]={0};
	char sz_Factoryversion[30]={0};
	char sz_userversion[30]={0};
	char sz_entrancetoolversion[15]={0};
	char sz_Qty[30]={0};
	char sz_CRType[15]={0};
	char sz_segment[15]={0};
	char sz_Fix_EntTool_ver[15]={0};
	char sz_Fix_Img_ver[15]={0};
	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};

	strncpy_s(sz_SO, sizeof(sz_SO), T2A(m_st_idParameter.str_so), sizeof(sz_SO));

	/* Get DB DLL Query SO function */
	pQuerySO_Info_All_V5 iQuerySO_Info_All_V5 = NULL;
	iQuerySO_Info_All_V5 = (pQuerySO_Info_All_V5)::GetProcAddress(m_h_DBModule, "QuerySO_Info_All_V5");
	if (iQuerySO_Info_All_V5 == NULL)
	{
		//m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQuerySO_Info_All_V5 == NULL!");
		return false;
	}

	/* Query SO info */
	if (!iQuerySO_Info_All_V5(sz_SO, sz_model, sz_PN, sz_SOType, sz_ToolVersion, sz_Factoryversion, sz_userversion, sz_entrancetoolversion, sz_Qty, sz_CRType, sz_segment, sz_Fix_EntTool_ver, sz_Fix_Img_ver, sz_errorCode, sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	m_st_idParameter.str_modelName = ConvertString(sz_model);
	m_st_idParameter.str_partNumber = ConvertString(sz_PN);
	m_st_idParameter.str_soType = ConvertString(sz_SOType);
	m_st_idParameter.str_factoryVer = ConvertString(sz_Factoryversion);
	m_st_idParameter.str_toolVer = ConvertString(sz_ToolVersion);
	m_st_idParameter.str_userLoadVer = ConvertString(sz_userversion);
	m_st_idParameter.str_CRType = ConvertString(sz_CRType);

	m_i_SoQty = atoi(sz_Qty);

	/* Save CIM PICS data(so, model, partno, sotype) */
	m_map_picsData[_T("so")] = m_st_idParameter.str_so;
	m_map_picsData[_T("model")] = m_st_idParameter.str_modelName;
	m_map_picsData[_T("partno")] = m_st_idParameter.str_partNumber;
	m_map_picsData[_T("sotype")] = m_st_idParameter.str_soType;

	if(!QueryIDtype())
		return false;
	return true;
}

bool CQueryCIM::QueryStationRoutingInfo(char *sz_picasso, bool b_isOOBSpecialTest )
{
	USES_CONVERSION;

	char sz_model[15] = {0};
	char sz_process[15] = {0};
	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};

	strncpy_s(sz_model, sizeof(sz_model), T2A(m_st_idParameter.str_modelName), sizeof(sz_model));
	strncpy_s(sz_process, sizeof(sz_process), T2A(m_st_idParameter.str_station), sizeof(sz_process));

	pCheckRouting CheckRouting = NULL;
	CheckRouting = (pCheckRouting)::GetProcAddress(m_h_DBModule, "CheckRouting_V2");
	if (CheckRouting == NULL)
	{
	//	m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nCheckRouting == NULL!");
		return false;
	}

	pGetProcessResult iGetProcessResult = NULL;
	iGetProcessResult = (pGetProcessResult)::GetProcAddress(m_h_DBModule, "GetProcessResult");
	if (iGetProcessResult == NULL)
	{
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nGetProcessResult == NULL!");
		return false;
	}

	bool bret = false;

	if (b_isOOBSpecialTest)// For oob station, if 'Accessory check' is unchecked, skip to check result after FCHECK station
	{
		bret = CheckRouting(sz_picasso, sz_model, "FCHECK", sz_errorCode, sz_errorMsg);
		if (bret)
		{
			bret = iGetProcessResult(sz_picasso,sz_model,"FCHECK");
			if (!bret)
			{
				sprintf(sz_errorMsg,("Check id=%s, process=FCHECK test data fail \n找不到在站别:FCHECK PASS记录，请回到此站重新测试"),sz_picasso);
			}
		}
	}
	else
	{
		bret = CheckRouting(sz_picasso, sz_model, sz_process, sz_errorCode, sz_errorMsg);
	}

	//Save PICASSO
	m_map_picsData[_T("id")] = ConvertString(sz_picasso);


	/* Check station routing */
	if (!bret)
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}


	return true;
}

bool CQueryCIM::QueryStationInfo()
{

	USES_CONVERSION;

	char sz_PCName[100] = {0};
	char sz_line[15] = {0};
	char sz_process[15] = {0};
	char sz_segment[15] = {0};
	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};

	/* Get Computer name */
	DWORD dw_bufferSize=sizeof(sz_PCName);
	::GetComputerNameA(sz_PCName, &dw_bufferSize);

	//if (stricmp(sz_PCName,"o-30b4")==0)
	//{
	//	strcpy(sz_PCName,"P08S01");//for Nancy.Yang test
	//}

	m_str_computerName = ConvertString(sz_PCName);

	/* Get DB DLL Query station function */
	pQueryStation_Info iQueryStation_Info = NULL;
	iQueryStation_Info = (pQueryStation_Info)::GetProcAddress(m_h_DBModule, "QueryStation_Info");
	if (iQueryStation_Info == NULL)
	{
	//	m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQueryStation_Info == NULL!");
		return false;
	}

	/* Query station info */
	if (!iQueryStation_Info(sz_PCName, sz_line, sz_process, sz_segment, sz_errorCode, sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	m_st_idParameter.str_line = ConvertString(sz_line);
	m_st_idParameter.str_station = ConvertString(sz_process);
	m_st_idParameter.str_segment = ConvertString(sz_segment);

	/* Save CIM PICS data(workstation, line, process, segment) */
	m_map_picsData[_T("workstation")] = m_str_computerName;
	m_map_picsData[_T("line")] = m_st_idParameter.str_line;
	m_map_picsData[_T("process")] = m_st_idParameter.str_station;
	m_map_picsData[_T("segment")] = m_st_idParameter.str_segment;

	return true;

}

bool CQueryCIM::QuerySOCount(char * sz_id)
{
	pQuerySOCount_Control iQuerySOCount = NULL;
	iQuerySOCount = (pQuerySOCount_Control)::GetProcAddress(m_h_DBModule,"QuerySOCount_Control");
	if (iQuerySOCount == NULL)
	{
		//m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQuerySOCount == NULL!");
		return false;
	}

	char sz_count[30]={0}; 
	char sz_so[30]={0};
	char sz_process[30]={0};
	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};
	CStringToChar(m_st_idParameter.str_so,sz_so);
	CStringToChar(m_st_idParameter.str_station,sz_process);

	/* Query so count */
	if (!iQuerySOCount(sz_id, sz_so,sz_process, sz_count, sz_errorCode,sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}
	else
	{
		int i_current = atoi(sz_count);
		//2012.09.24 for CR SO,check validity
		if(m_i_SoQty == 0)
			this->QuerySOInfo_ALL_V5();
		if ((m_i_SoQty < i_current) && (m_st_idParameter.str_CRType != _T("")))
		{
		//	m_str_errorCode = CommErr_UI_CIM_SO_CR_Count_Over_Upper;
			m_str_errorMsg.Format(_T("当前工单测试数量(%d)已经超过额定数量(%d),请更换工单!\nTest count exceeds so count!"),i_current,m_i_SoQty);
			return false;
		}
		
	}

	return true;

}

bool CQueryCIM::CheckSO( char *sz_picasso )
{
	pCheckSO iCheckSO = NULL;
	iCheckSO = (pCheckSO)::GetProcAddress(m_h_DBModule, "CheckSO");
	if (iCheckSO == NULL)
	{
	//	m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nCheckSO == NULL!");
		return false;
	}

	char sz_model[15] = {0};
	char sz_So[20]={0};
	char sz_CRtype[10]={0};
	char sz_process[30] = {0};
	char sz_segment[30] = {0};

	CStringToChar(m_st_idParameter.str_modelName,sz_model);
	CStringToChar(m_st_idParameter.str_so,sz_So);
	CStringToChar(m_st_idParameter.str_CRType,sz_CRtype);
	CStringToChar(m_st_idParameter.str_station,sz_process);
	CStringToChar(m_st_idParameter.str_segment,sz_segment);

	char sz_ErrCode[15] = {0};
	char sz_ErrMsg[MAX_BUFFER_SIZE] = {0};

	if (!iCheckSO(sz_So, sz_CRtype,sz_picasso,sz_model,sz_process, sz_segment,sz_ErrCode,sz_ErrMsg))
	{
		m_str_errorCode = ConvertString(sz_ErrCode);
		m_str_errorMsg = ConvertString(sz_ErrMsg);
		return false;
	}

	return true;
}

bool CQueryCIM::QueryEmployeeAuthority()
{

	USES_CONVERSION;

	char sz_employeeID[10] = {0};
	char sz_level[10] = {0};
	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};
	CString str_level;

	strncpy_s(sz_employeeID, sizeof(sz_employeeID), T2A(m_st_idParameter.str_employee), sizeof(sz_employeeID));

	/* Get DB DLL Query employee level function */
	pQueryEmployeeID_Right iQueryEmployeeID_Right = NULL;
	iQueryEmployeeID_Right = (pQueryEmployeeID_Right)::GetProcAddress(m_h_DBModule, "QueryEmployeeID_Right");
	if (iQueryEmployeeID_Right == NULL)
	{
	//	m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQueryEmployeeID_Right == NULL!");
		return false;
	}

	/* Query employee level */
	if (!iQueryEmployeeID_Right(sz_employeeID, sz_level, sz_errorCode, sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	str_level = ConvertString(sz_level);

	if (str_level.CompareNoCase(_T("10")) == 0)//ts mode
	{
		m_str_errorMsg = _T("Common tool, not support TS employee ID!");
		return false;
	}

	/* Save CIM PICS data(employee) */
	m_map_picsData[_T("employee")] = ConvertString(sz_employeeID);

	return true;

}

//Query Error code and save it in '\\DB\\DB_LOG\\ErrorInfo.ini' 
bool CQueryCIM::QueryOOBErrCodeInfo()
{
	USES_CONVERSION;
	
	char sz_process[15] = {0};
	char sz_model[15] = {0};
	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};

	strncpy_s(sz_model, sizeof(sz_model), T2A(m_st_idParameter.str_modelName), sizeof(sz_model));
	strncpy_s(sz_process, sizeof(sz_process), T2A(m_st_idParameter.str_station), sizeof(sz_process));

	/* Get DB DLL Query OOB ErrCode Info */
	pQueryOOBErrCode_Info iQueryOOBErrCode_Info = NULL;
	iQueryOOBErrCode_Info = (pQueryOOBErrCode_Info)::GetProcAddress(m_h_DBModule, "QueryOOBErrCode_Info");
	if (iQueryOOBErrCode_Info == NULL)
	{
	//	m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQueryOOBErrCode_Info == NULL!");
		return false;
	}

	/* Query station info */
	if (!iQueryOOBErrCode_Info(sz_model, sz_process, sz_errorCode, sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	return true;
}

bool CQueryCIM::QueryApkTestItem_ShippingOS()
{
	USES_CONVERSION;

	char sz_PN[15] = {0};
	char sz_SDstatus[50]={0};
	char sz_Item[4096]={0};
	char sz_Switch[50] ={0};
	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};

	strncpy_s(sz_PN, sizeof(sz_PN), T2A(m_st_idParameter.str_partNumber), sizeof(sz_PN));
	
	/* Get DB DLL Query OOB ErrCode Info */
	pQueryApkTestItem_V2 iQueryApkTestItem_V2 = NULL;
	iQueryApkTestItem_V2 = (pQueryApkTestItem_V2)::GetProcAddress(m_h_DBModule, "QueryApkTestItem_V2");
	if (iQueryApkTestItem_V2 == NULL)
	{
		//	m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQueryApkTestItem_V2 == NULL!");
		return false;
	}
	
	/* Query station info */
	if (!iQueryApkTestItem_V2(sz_PN,sz_Item,sz_SDstatus, sz_Switch, sz_errorCode, sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	m_st_idParameter.str_SDstatus = ConvertString(sz_SDstatus);
	m_st_idParameter.str_APKTestItem = ConvertString(sz_Item);

	if (ConvertString(sz_Switch) == _T("Y"))
		m_st_idParameter.b_isReadIMEI = true;
	else
		m_st_idParameter.b_isReadIMEI = false;

	return true;
}

bool CQueryCIM::QueryIDtype(void)
{
	USES_CONVERSION;

	pQueryIDType iQueryIDType = NULL;
	iQueryIDType = (pQueryIDType)::GetProcAddress(m_h_DBModule, "Query_IDType");
	if (iQueryIDType == NULL)
	{
	//	m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQuery_IDType == NULL!");
		return false;
	}

	char sz_PN[30] = {0};
	char sz_idtype[30] = {0};
	char sz_idlength[30] ={0};
	char sz_ErrCode[15] = {0};
	char sz_ErrMsg[2048] = {0};

	strncpy_s(sz_PN, sizeof(sz_PN), T2A(m_st_idParameter.str_partNumber), sizeof(sz_PN));

	if (!iQueryIDType(sz_PN,sz_idtype,sz_idlength,sz_ErrCode,sz_ErrMsg))
	{
		m_str_errorCode = ConvertString(sz_ErrCode);
		m_str_errorMsg = ConvertString(sz_ErrMsg);
		return false;
	}
	CString strTemp = ConvertString(sz_idtype);

	if(strTemp == _T(""))
	{
		//m_str_errorCode = CommErr_UI_ID_Invalid;
		m_str_errorMsg = _T("系统中设置的ID 类型无效, 请于IE联系");
		return false;
	}
	m_st_idParameter.str_idtype = strTemp;
	strTemp = ConvertString(sz_idlength);
	m_st_idParameter.i_idLength = StrToInt(strTemp.GetBuffer());
	strTemp.ReleaseBuffer();

	return true;
}
//str_keyPartsInfo = "kptype,description,kpword,kppn,chkduplic;kptype,description,kpword,kppn,chkduplic;...."
bool CQueryCIM::QueryKeypartsInfo(CString &str_keyPartsInfo, int &i_keyPartCount)
{
	USES_CONVERSION;

	char sz_PN[15] = {0};
	char sz_process[15] = {0};
	char sz_model[15] = {0};
	char sz_count[32]={0};
	char sz_info[8192]={0};
	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};

	CString strtemp;

	strncpy_s(sz_PN, sizeof(sz_PN), T2A(m_st_idParameter.str_partNumber), sizeof(sz_PN));
	strncpy_s(sz_model, sizeof(sz_model), T2A(m_st_idParameter.str_modelName), sizeof(sz_model));
	strncpy_s(sz_process, sizeof(sz_process), T2A(m_st_idParameter.str_station), sizeof(sz_process));

	pQueryKeyparts_Info iQueryKeyparts_Info = NULL;
	iQueryKeyparts_Info = (pQueryKeyparts_Info)::GetProcAddress(m_h_DBModule, "QueryKeyparts_Info");
	if (iQueryKeyparts_Info == NULL)
	{
		//	m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQueryKeyparts_Info == NULL!");
		return false;
	}

	/* Query keyparts model+model+process */
	//eg.SELECT * FROM KPPCS WHERE model='GS03B4A' and process='PPIDLINK' and submodel='GS03B4A' ORDER BY kpseq 
	if(!iQueryKeyparts_Info(sz_model,sz_model,"PPIDLINK",sz_count, sz_info, sz_errorCode, sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	str_keyPartsInfo = ConvertString(sz_info);
	i_keyPartCount = atoi(sz_count);

	ZeroMemory(sz_info,sizeof(sz_info));
	ZeroMemory(sz_count,sizeof(sz_count));

	/* QueryKeyparts_Info by c_PN + c_Model + c_Process , if there is more than one count for one kind*/
	//eg.SELECT * FROM KPPCS WHERE model='GS03B4A' and process='OOB' and submodel='9J.2NPTW.401' ORDER BY kpseq 
	if(!iQueryKeyparts_Info(sz_PN,sz_model,sz_process,sz_count, sz_info, sz_errorCode, sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	strtemp = ConvertString(sz_info);

	str_keyPartsInfo += strtemp;
	i_keyPartCount += atoi(sz_count);

	return true;
}

bool CQueryCIM::QueryPNinBACHBySO(char * sz_keyPartPN, char * sz_qty)
{
	USES_CONVERSION;

	char sz_PN[15] = {0};
	char sz_SO[15] = {0};
	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};

	strncpy_s(sz_SO, sizeof(sz_SO), T2A(m_st_idParameter.str_so), sizeof(sz_SO));

	pQueryPNinBACH_BySO iQueryPNinBACH_BySO = NULL;
	iQueryPNinBACH_BySO = (pQueryPNinBACH_BySO)::GetProcAddress(m_h_DBModule, "QueryPNinBACH_BySO");
	if (iQueryPNinBACH_BySO == NULL)
	{
		//	m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQueryPNinBACH_BySO == NULL!");
		return false;
	}

	if(!iQueryPNinBACH_BySO(sz_keyPartPN, sz_SO, sz_qty, sz_errorCode, sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	return true;
}

/**
*
* ####1. If sz_result is fail then used GetErrCodeDetail_QA to get error detail message (sz_NGType, sz_NGDetail, sz_MajorMin, sz_desp) ####
* ####2. If sz_result is pass, [Pics.xml] use QA.xml and insert the record to oba_accessorycheck table####
* ####3. If sz_result is fail, [Pics.xml] use QANG.xml and insert the record to OBACheckNGInfo table####
*
*/
bool CQueryCIM::InsertOOBData(char * sz_result)
{
	USES_CONVERSION;

	char sz_inputerrorCode[100] = {0};
	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};

	/* Get DB DLL Query OOB ErrCode Info */
	pInsertOOB_data iInsertOOB_data = NULL;
	iInsertOOB_data = (pInsertOOB_data)::GetProcAddress(m_h_DBModule, "InsertOOB_data");
	if (iInsertOOB_data == NULL)
	{
		//	m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nInsertOOB_data == NULL!");
		return false;
	}

	CStringToChar(m_str_result,sz_inputerrorCode);

	/* Query station info */
	if (!iInsertOOB_data(sz_result,sz_inputerrorCode,sz_errorCode, sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	return true;
}

bool CQueryCIM::UnlinkKeyparts(char * sz_id)
{
	USES_CONVERSION;

	pUnlinkKeyparts iUnlinkKeyparts = NULL;
	iUnlinkKeyparts = (pUnlinkKeyparts)::GetProcAddress(m_h_DBModule,"UnlinkKeyparts");
	if (iUnlinkKeyparts == NULL)
	{
		//m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nUnlinkKeyparts == NULL!");
		return false;
	}

	char sz_model[15] = {0};
	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};

	strncpy_s(sz_model, sizeof(sz_model), T2A(m_st_idParameter.str_modelName), sizeof(sz_model));

	if (!iUnlinkKeyparts( sz_id, sz_model,sz_errorCode,sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	return true;
}


bool CQueryCIM::QueryIMEIStatus( char * sz_InputID, char * sz_status)
{
	pQueryIMEIStatus iQueryIMEIStatus = NULL;
	iQueryIMEIStatus = (pQueryIMEIStatus)::GetProcAddress(m_h_DBModule, "QueryIMEIStatus");
	if (iQueryIMEIStatus == NULL)
	{
		//m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQueryIMEIStatus == NULL!");
		return false;
	}

	char sz_carton[50]={0};
	char sz_ErrCode[15]={0};
	char sz_ErrMsg[MAX_BUFFER_SIZE]={0};
	if (!iQueryIMEIStatus(sz_InputID,sz_status,sz_carton,sz_ErrCode,sz_ErrMsg))
	{
		m_str_errorCode = ConvertString(sz_ErrCode);
		m_str_errorMsg = ConvertString(sz_ErrMsg);
		return false;
	}

	return true;
}

bool CQueryCIM::QueryholdInfo( char * sz_id)
{
	pQueryholdInfo iQueryholdInfo = NULL;
	iQueryholdInfo = (pQueryholdInfo)::GetProcAddress(m_h_DBModule,"QueryholdInfo_V2");
	if (iQueryholdInfo == NULL)
	{
		//m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQueryholdInfo == NULL!");
		return false;
	}

	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};
	char sz_status[10]={0};
	char sz_process[40]={0};
	char sz_holdreas[MAX_BUFFER_SIZE]={0};
	char sz_model[30]={0};
	CStringToChar(m_st_idParameter.str_station,sz_process);
	CStringToChar(m_st_idParameter.str_modelName,sz_model);

	if (!iQueryholdInfo( sz_id ,sz_model,sz_process,sz_status,sz_holdreas, sz_errorCode,sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	if (strcmp(sz_status,"H") == 0)
	{
		//m_str_errorCode = CommErr_UI_CIM_Picasso_QA_Hold;
		m_str_errorMsg = ConvertString(sz_holdreas);//_T("此ID已经被Hold，请联系QA解除Hold\nThe status of this id is H");
		return false;
	}
	return true;
}

//input IMEI, output PICASSO
bool CQueryCIM::QueryPicasso( char * sz_InputID, char * sz_Picasso )
{
	pGetPicasso iGetPicasso = NULL;
	iGetPicasso = (pGetPicasso)::GetProcAddress(m_h_DBModule, "GetPicasso");
	if (iGetPicasso == NULL)
	{
		//m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nGetPicasso == NULL!");
		return false;
	}

	char sz_model[30]={0};
	char sz_PN[20]={0};
	char sz_ErrCode[15]={0};
	char sz_ErrMsg[MAX_BUFFER_SIZE]={0};
	
	CStringToChar(m_st_idParameter.str_modelName,sz_model);

	if (!iGetPicasso(sz_InputID, sz_model, sz_Picasso,sz_PN, sz_ErrCode, sz_ErrMsg))
	{
		m_str_errorCode = ConvertString(sz_ErrCode);
		m_str_errorMsg = ConvertString(sz_ErrMsg);

		return false;
	}

	return true;

}

bool CQueryCIM::QueryWifiLoginInfo( char * sz_InputID, char * sz_WifiMac,char * sz_LoginWifiInfo )
{
	pQueryWifiLoginInfo iQueryWifiLoginInfo = NULL;
	iQueryWifiLoginInfo = (pQueryWifiLoginInfo)::GetProcAddress(m_h_DBModule, "QueryWifiLoginInfo");
	if (iQueryWifiLoginInfo == NULL)
	{
		//m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQueryWifiLoginInfo == NULL!");
		return false;
	}

	char sz_ErrCode[15]={0};
	char sz_ErrMsg[MAX_BUFFER_SIZE]={0};

	if (!iQueryWifiLoginInfo(sz_InputID, sz_WifiMac, sz_LoginWifiInfo, sz_ErrCode, sz_ErrMsg))
	{
		m_str_errorCode = ConvertString(sz_ErrCode);
		m_str_errorMsg = ConvertString(sz_ErrMsg);
		return false;
	}

	return true;
}

bool CQueryCIM::QueryRecordInfo()
{
	USES_CONVERSION;

	char sz_SO[10] = {0};
	char sz_process[15] = {0};
	char sz_PCName[100] = {0};
	char sz_totalCount[10] = {0};
	char sz_passCount[10] = {0};
	char sz_failCount[10] = {0};
	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};

	CString str_tempTotal;
	CString str_tempPass;
	CString str_tempFail;

	strncpy_s(sz_SO, sizeof(sz_SO), T2A(m_st_idParameter.str_so), sizeof(sz_SO));
	strncpy_s(sz_process, sizeof(sz_process), T2A(m_st_idParameter.str_station), sizeof(sz_process));
	strncpy_s(sz_PCName, sizeof(sz_PCName), T2A(m_str_computerName), sizeof(sz_PCName));

	/* Get DB DLL Query record function */
	pQueryRecordBySO QueryRecordBySO = NULL;
	QueryRecordBySO = (pQueryRecordBySO)::GetProcAddress(m_h_DBModule, "QueryRecordBySO");
	if (QueryRecordBySO == NULL)
	{
	//	m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQueryRecordBySO == NULL!");
		return false;
	}

	/* Query record info */
	if (!QueryRecordBySO(sz_SO, sz_process, sz_PCName, sz_totalCount, sz_passCount, sz_failCount, sz_errorCode, sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	str_tempTotal = ConvertString(sz_totalCount);
	str_tempPass = ConvertString(sz_passCount);
	str_tempFail = ConvertString(sz_failCount);

	m_i_totalNo = _ttoi(str_tempTotal);
	m_i_passNo = _ttoi(str_tempPass);
	m_i_failNo = _ttoi(str_tempFail);

	return true;

}

bool CQueryCIM::InsertTestRecordCIM( bool b_pass )
{
	USES_CONVERSION;

	std::map<CString, CString> map_tempPICSData;
	std::map<CString, CString>::iterator Itr;

	char sz_SO[50] = {0};
	char sz_ID[50] = {0};
	char sz_employeeID[50] = {0};
	char sz_inputerrorCode[100] = {0};
	char sz_errorCode[50] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};
	char sz_imei[60]={0};
	char sz_picasso[30]={0};

	/* Get PICS data map matched with slot */
	map_tempPICSData = m_map_PICSDataFromUI;

	/* Copy common PICS data queried from CIM */
	for (Itr = m_map_picsData.begin(); Itr != m_map_picsData.end(); Itr++)
	{
		map_tempPICSData[Itr->first] = Itr->second;
	}

	CStringToChar(map_tempPICSData[_T("id")],sz_ID);//picasso
	CStringToChar(m_st_idParameter.str_so,sz_SO);
	CStringToChar(m_st_idParameter.str_employee,sz_employeeID);

	if (!b_pass)
	{
		CStringToChar(m_str_result,sz_inputerrorCode);
	}

	/* Get DB DLL clear PICS data function */
	pClear_PICSData Clear_PICSData = NULL;
	Clear_PICSData = (pClear_PICSData)::GetProcAddress(m_h_DBModule, "Clear_PICSData");
	if (Clear_PICSData == NULL)
	{
		//m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nClear_PICSData == NULL!");
		return false;
	}
	/* Get DB DLL set PICS data function */
	pSetValue SetValue = NULL;
	SetValue = (pSetValue)::GetProcAddress(m_h_DBModule, "SetValue");
	if (SetValue == NULL)
	{
		//m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nSetValue == NULL!");
		return false;
	}

	/* Get DB DLL Insert test record function */
	pInsertFAIL_data InsertFAIL_data = NULL;
	InsertFAIL_data = (pInsertFAIL_data)::GetProcAddress(m_h_DBModule, "InsertFAIL_data");
	if (InsertFAIL_data == NULL)
	{
	//	m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nInsertFAIL_data == NULL!");
		return false;
	}
	pInsertPASS_data InsertPASS_data = NULL;
	InsertPASS_data = (pInsertPASS_data)::GetProcAddress(m_h_DBModule, "InsertPASS_data");
	if (InsertPASS_data == NULL)
	{
		//m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nInsertPASS_data == NULL!");
		return false;
	}

	/* clear DB Access DLL PICS data map */
	if (!Clear_PICSData())
	{
		//m_str_errorCode = CommErr_UI_CIM_Pics_Clean_Fail;
		m_str_errorMsg = _T("清空PICS数据失败!\nClear PICS data fail!");
		return false;
	}

	char sz_key[512] = {0};
	char sz_value[512] = {0};
	/* set PICS data to DB Access DLL */
	for (Itr = map_tempPICSData.begin(); Itr != map_tempPICSData.end(); Itr++)
	{
		memset(sz_key, 0, sizeof(sz_key));
		memset(sz_value, 0, sizeof(sz_value));
		CStringToChar(Itr->first,sz_key);
		CStringToChar(Itr->second,sz_value);
		//strncpy_s(sz_key, sizeof(sz_key), T2A(Itr->first), sizeof(sz_key));
		//strncpy_s(sz_value, sizeof(sz_value), T2A(Itr->second), sizeof(sz_value));

		SetValue(sz_key, sz_value);	
	}

	/* Insert test record to CIM */
	bool bret = false;
	if (b_pass)
	{
		bret = InsertPASS_data(sz_ID, sz_SO, sz_employeeID, sz_errorCode, sz_errorMsg);
	}
	else
	{
		bret = InsertFAIL_data(sz_ID, sz_SO, sz_employeeID, sz_inputerrorCode, sz_errorCode, sz_errorMsg);
	}

	if (!bret)
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	return true;
}

bool CQueryCIM::UpdateEmployeeID_Status()
{
	pUpdateEmployeeID_Status iUpdateEmployeeID_Status = NULL;
	iUpdateEmployeeID_Status = (pUpdateEmployeeID_Status)::GetProcAddress(m_h_DBModule,"UpdateEmployeeID_Status");
	if (iUpdateEmployeeID_Status == NULL)
	{
	//	m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nUpdateEmployeeID_Status == NULL!");
		return false;
	}

	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};
	char sz_employee[50]={0};
	char sz_computerName[50]={0};

	CStringToChar(m_str_computerName,sz_computerName);

	CStringToChar(m_st_idParameter.str_employee,sz_employee);

	if (!iUpdateEmployeeID_Status( sz_employee ,"Y",sz_computerName, sz_errorCode,sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	return true;
}