#include "StdAfx.h"
#include "QueryCIM.h"

CCriticalSection CQueryCIM::m_obj_DBLock;

CQueryCIM::CQueryCIM(void)
{
	m_h_DBModule = NULL;
	m_h_VMSDBModule = NULL;
	/* Initial parameter */
	m_st_idParameter.i_parameterNum = 0;
	m_st_idParameter.b_tsFlag = false;
	m_st_idParameter.str_firstProcess = _T("");
	m_st_idParameter.str_station = _T("");
	m_st_idParameter.str_modelName = _T("");
	m_st_idParameter.str_line = _T("");
	m_st_idParameter.str_pcba = _T("");
	m_st_idParameter.str_so = _T("");
	m_st_idParameter.str_partNumber = _T("");
	m_st_idParameter.str_toolVer = _T("");
	m_st_idParameter.str_employee = _T("");
	m_st_idParameter.str_factoryVer = _T("");
	m_st_idParameter.str_userLoadVer = _T("");

	m_map_picasso.clear();
	m_map_picsData.clear();
	m_map_slotPICSData.clear();
	m_map_TestKeyValue.clear();

	m_i_totalNo = 0;
	m_i_passNo = 0;
	m_i_failNo = 0;

	m_i_SoQty = 0;
	m_b_csFlag = false;
	m_b_OOBCheck = false;

	m_str_computerName = _T("");

	m_str_errorCode = _T("");
	m_str_errorMsg = _T("");
	m_str_IMEI =  _T("");
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
	/* Get DB DLL Query employee level function */
	pConnectDB ConnectDB = NULL;
	ConnectDB = (pConnectDB)::GetProcAddress(m_h_DBModule, "ConnectDB");
	if (ConnectDB == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nLoad CIM DB dll ConnectDB=NULL");
		return false;
	}

	/* Connect DataBase */
	if (!ConnectDB())
	{
		m_str_errorCode = CommErr_UI_CIM_Connect_Fail;
		m_str_errorMsg = _T("连接CIM数据库失败!\nConnect CIM Database fail!");
		return false;
	}

	return true;

}

bool CQueryCIM::CloseDataBase()
{
	/* Get DB DLL Query employee level function */
	pCloseDB CloseDB = NULL;
	CloseDB = (pCloseDB)::GetProcAddress(m_h_DBModule, "CloseDB");
	if (m_h_DBModule == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nm_h_DBModule == NULL!");
		return false;
	}
	if (CloseDB == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nCloseDB = NULL!");

		return false;
	}

	/* Close DataBase */
	CloseDB();

	return true;
}


/*****************************************get info****************************************/
CString CQueryCIM::GetErrorCode( )
{
	CString str_tmp = m_str_errorCode;
	//reset after use
	m_str_errorCode = _T("");
	return str_tmp;
}

CString CQueryCIM::GetErrorMsg( )
{
	CString str_tmp = m_str_errorMsg;
	//reset after use
	m_str_errorMsg = _T("");
	return  str_tmp; 
}

CString CQueryCIM::GetComputerName()
{
	return m_str_computerName;
}

void CQueryCIM::SetComputerName( CString str_computername )
{
	m_str_computerName = str_computername;
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

bool CQueryCIM::IsCheckOOB()
{
	return m_b_OOBCheck;
}

bool CQueryCIM::GetUIIDParameter( st_UIIDparameter &st_idParameter )
{
	st_idParameter.i_parameterNum = m_st_idParameter.i_parameterNum;
	st_idParameter.b_tsFlag = m_st_idParameter.b_tsFlag;
	st_idParameter.str_modelName.Format(_T("%s"), m_st_idParameter.str_modelName);
	st_idParameter.str_firstProcess.Format(_T("%s"), m_st_idParameter.str_firstProcess);	
	st_idParameter.str_station.Format(_T("%s"), m_st_idParameter.str_station);
	st_idParameter.str_line.Format(_T("%s"), m_st_idParameter.str_line);
	st_idParameter.str_pcba.Format(_T("%s"), m_st_idParameter.str_pcba);
	st_idParameter.str_so.Format(_T("%s"), m_st_idParameter.str_so);
	st_idParameter.str_partNumber.Format(_T("%s"), m_st_idParameter.str_partNumber);
	st_idParameter.str_toolVer.Format(_T("%s"), m_st_idParameter.str_toolVer);
	st_idParameter.str_employee.Format(_T("%s"), m_st_idParameter.str_employee);
	st_idParameter.str_factoryVer.Format(_T("%s"), m_st_idParameter.str_factoryVer);
	st_idParameter.str_userLoadVer.Format(_T("%s"), m_st_idParameter.str_userLoadVer);
	st_idParameter.str_imagePath.Format(_T("%s"), m_st_idParameter.str_imagePath);
	st_idParameter.str_segment.Format(_T("%s"), m_st_idParameter.str_segment);
	st_idParameter.str_soType.Format(_T("%s"), m_st_idParameter.str_soType);
	st_idParameter.str_CRType.Format(_T("%s"), m_st_idParameter.str_CRType);
	st_idParameter.str_Reserve1.Format(_T("%s"), m_st_idParameter.str_Reserve1);
	st_idParameter.str_Reserve2.Format(_T("%s"), m_st_idParameter.str_Reserve2);

	st_idParameter.str_idtype.Format(_T("%s"), m_st_idParameter.str_idtype);
	st_idParameter.i_idLength = m_st_idParameter.i_idLength;

	return true;
}

bool CQueryCIM::GetStationListMap( std::map<CString, CString> &map_StationVsIndex )
{
	if (m_map_StationIndexMap.empty())
	{
		return false;
	}
	std::map<CString,CString>::iterator Itr;
	for (Itr = m_map_StationIndexMap.begin();Itr!= m_map_StationIndexMap.end();Itr++)
	{
		map_StationVsIndex[Itr->first] = Itr->second;
	}
	return true;
}

bool CQueryCIM::GetTestKeyValue(std::map<CString, CString> &map_TestKeyValue)
{
	if (m_map_TestKeyValue.empty())
	{
		map_TestKeyValue.clear();
		return true;
	}
	std::map<CString,CString>::iterator Itr;
	for (Itr = m_map_TestKeyValue.begin();Itr!= m_map_TestKeyValue.end();Itr++)
	{
		map_TestKeyValue[Itr->first] = Itr->second;
	}
	return true;
}


/***********************************set parameter for query***************************/
void CQueryCIM::SetSTResult( st_pageReturn st_result )
{
	m_st_result = st_result;
}

void CQueryCIM::SetPicassoMap( std::map<int, CString> map_picasso )
{
	m_map_picasso = map_picasso;
}

void CQueryCIM::SetslotPICSData( std::map<int, std::map<CString, CString>> map_slotPICSData )
{
	m_map_slotPICSData = map_slotPICSData;
}

void CQueryCIM::SetidParameter( st_UIIDparameter st_idParameter )
{
	m_st_idParameter = st_idParameter;
}

/*************************************public functions for outer user****************************/
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

		m_b_OOBCheck &= bRet;

		return bRet;
	}
	CloseDataBase();
	return false;
	
}

bool CQueryCIM::CheckPicassoStatusFromCIM( char * sz_Picasso, char * sz_TimeInfo,char * sz_Status )
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);
	if(ConnectDataBase())
	{
		if (CheckPicassoStatusCIM(sz_Picasso, sz_TimeInfo, sz_Status))
		{
			return CloseDataBase();
		}
	}
	CloseDataBase();
	return false;
	
}


bool CQueryCIM::CheckPicassoDuplicateFromCIM( char * sz_Picasso, char * sz_TimeInfo )
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);
	if(ConnectDataBase())
	{
		if (CheckPicassoDuplicate_SndStationCIM(sz_Picasso,sz_TimeInfo))
		{
			return CloseDataBase();
		}
	}
	CloseDataBase();
	return false;	
}


bool CQueryCIM::InsertPicassoControlToCIM( char * sz_Picasso, char * sz_TimeInfo,char * sz_Status )
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);
	if(ConnectDataBase())
	{
		if (InsertPicassoControlCIM(sz_Picasso,sz_TimeInfo,sz_Status))
		{
			return CloseDataBase();
		}
	}
	CloseDataBase();
	return false;
}

bool CQueryCIM::QueryStationRoutingInfoFromCIM(char *p_sz_picasso )
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);

	bool b_ret =  ConnectDataBase();

	//1.check so count,no matter SO type is
	if(b_ret)
	{
		Log(_T("1.Begin to check so count"));
		b_ret = QuerySOCount(p_sz_picasso);
	}

	//2.query picasso if hold by QA
	if (b_ret)
	{
		Log(_T("2.Begin to check if hold by QA"));
		b_ret = QueryholdInfo(p_sz_picasso);
	}

	//3.check test routing
	if(b_ret)
	{
		//Nancy.Yang deleted for new csd rule 20130510
		/*if (m_b_csFlag)
		{
			Log(_T("3.CSD tool, check routing"));
			b_ret = QueryStationRoutingInfoCS(p_sz_picasso);		
		}
		else
		{*/
			Log(_T("3.Facotory tool check routing"));
			b_ret = QueryStationRoutingInfo(p_sz_picasso);
		//} 
		
	}
	
	//4.check CR/BO so whether same with first station at each line
	if (b_ret)
	{
		Log(_T("4.CR SO,begin to check so if same with the first station"));
		b_ret = CheckSO(p_sz_picasso);
	}

	CloseDataBase();
	return b_ret;
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

bool CQueryCIM::QueryIMEIresultFromCIM( char * sz_InputID, char * sz_Model,char * sz_Picasso )
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);
	if(ConnectDataBase())
	{
		if (QueryIMEIresult(sz_InputID,sz_Model,sz_Picasso))
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

bool CQueryCIM::QueryPicassoFromCIM(char * sz_InputID, char * sz_Model,char * sz_Picasso)
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);
	if(ConnectDataBase())
	{
		if (QueryPicasso(sz_InputID,sz_Model,sz_Picasso))
		{
			return CloseDataBase();
		}
	}
	CloseDataBase();
	return false;
}

bool CQueryCIM::UnlinkKeypartsFromCIM(char * sz_id, char * sz_model)
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);
	if(ConnectDataBase())
	{
		if (UnlinkKeyparts(sz_id,sz_model))
		{
			return CloseDataBase();
		}
	}
	CloseDataBase();
	return false;
}

bool CQueryCIM::CheckNALFromCIM( char * sz_IMEI, char * sz_NAL )
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);
	bool b_ret = ConnectDataBase();
	if(b_ret)
	{
		if (m_st_idParameter.str_station == STATION_NAL_SCAN)
		{
			b_ret = CheckNAL_duplicate_V2(sz_IMEI,sz_NAL);
		}
		else
		{
			b_ret = CheckNAL_duplicate(sz_IMEI,sz_NAL);
		}
		
	}
	if (b_ret)
	{
		b_ret = CheckNAL_Valide(sz_NAL);
	}

	CloseDataBase();
	return b_ret;
}



bool CQueryCIM::QueryNALFromCIM( char * sz_IMEI, char * sz_NAL )
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);
	if(ConnectDataBase())
	{
		if (QueryNAL(sz_IMEI,sz_NAL))
		{
			return CloseDataBase();
		}
	}
	CloseDataBase();
	return false;

}

bool CQueryCIM::QueryIfTestAgainFromCIM( char * sz_id, char* sz_model, char * sz_process, char *sz_cnt )
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);
	if(ConnectDataBase())
	{
		if (QueryIfTestAgain(sz_id,sz_model,sz_process,sz_cnt))
		{
			return CloseDataBase();
		}
	}
	CloseDataBase();
	return false;
}

bool CQueryCIM::InsertTS_DataToCIM( char * sz_id, char * sz_model,char * sz_rtnprocess, char * sz_ngprocess, char * sz_ngwrkstation )
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);
	if(ConnectDataBase())
	{
		if (InsertTS_Data(sz_id, sz_model,sz_rtnprocess,sz_ngprocess,sz_ngwrkstation))
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


bool CQueryCIM::QueryDataFromCIM()
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);
	bool b_res = true;


	if (b_res)
	{
		b_res = ConnectDataBase();
	}

#ifdef FACTORY_MODE
	if (b_res)
	{
		//Nancy.yt.Zhu,20121127
		//b_res = QuerySOInfo_ALL_V3();
		b_res = QuerySOInfo_ALL_V5();
	}

	if (b_res)
	{
		b_res = QueryStationInfo();
	}
	if (b_res)
	{
		if (m_st_idParameter.str_station == STATION_FCHECK)
		{
			b_res = QueryOOBSampleRate();
		}	
	}
#endif

	if (b_res)
	{
		b_res = QueryEmployeeAuthority();
	}
	if (b_res)
	{
		b_res = QueryFirstProcessInfo();
	}

	if (b_res)
	{
		b_res = QueryStationList();
	}

	 CloseDataBase();

	return b_res;
}

bool CQueryCIM::QueryDataFromSQN()
{
	CSingleLock singleLock(&m_obj_DBLock, TRUE);
	bool b_res = true;

	if (b_res)
	{
		if (((m_st_idParameter.str_partNumber.Left(2) == _T("5E"))&&(m_st_idParameter.str_segment.CompareNoCase(_T("TLINE")) != 0))||
			((m_st_idParameter.str_partNumber.Left(2) == _T("6K"))&&(m_st_idParameter.str_segment.CompareNoCase(_T("TLINE")) != 0))||
			((m_st_idParameter.str_partNumber.Left(2) == _T("5G"))&&(m_st_idParameter.str_segment.CompareNoCase(_T("FA")) != 0))||
			((m_st_idParameter.str_partNumber.Left(2) == _T("9J"))&&(m_st_idParameter.str_segment.CompareNoCase(_T("PACKAGE")) != 0))||
			((m_st_idParameter.str_partNumber.Left(2) == _T("9B"))&&(m_st_idParameter.str_segment.CompareNoCase(_T("PACKAGE")) != 0))||
			((m_st_idParameter.str_partNumber.Left(2) == _T("5J"))&&(m_st_idParameter.str_segment.CompareNoCase(_T("PACKAGE")) != 0))||
			((m_st_idParameter.str_partNumber.Left(2) == _T("CS"))&&(m_st_idParameter.str_segment.CompareNoCase(_T("PACKAGE")) != 0)))
		{
			m_str_errorCode = CommErr_UI_PVCS_PN_Segment_Match_Fail;
			m_str_errorMsg.Format(_T("PartNumer(%s) not fit with this line(%s),please contact the administrator!"),m_st_idParameter.str_partNumber,m_st_idParameter.str_segment);
			b_res = false;
		}
	}

	if (b_res)
	{
		b_res = VMS_QueryDataFromSQN();
	}

	return b_res;
}

/*******************************************For inner user****************************************/
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
	map_tempPICSData = m_map_slotPICSData[m_st_result.i_slot];

	/* Copy common PICS data queried from CIM */
	for (Itr = m_map_picsData.begin(); Itr != m_map_picsData.end(); Itr++)
	{
		map_tempPICSData[Itr->first] = Itr->second;
	}

	CStringToChar(m_str_IMEI,sz_imei);
	CStringToChar(m_map_picasso[m_st_result.i_slot],sz_picasso);
	CStringToChar(m_map_picasso[m_st_result.i_slot],sz_ID);
	CStringToChar(m_st_idParameter.str_so,sz_SO);
	CStringToChar(m_st_idParameter.str_employee,sz_employeeID);


	if (!b_pass)
	{
		CStringToChar(m_st_result.str_errorcode,sz_inputerrorCode);
	}


	/* Get DB DLL clear PICS data function */
	pClear_PICSData Clear_PICSData = NULL;
	Clear_PICSData = (pClear_PICSData)::GetProcAddress(m_h_DBModule, "Clear_PICSData");
	if (Clear_PICSData == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nClear_PICSData == NULL!");
		return false;
	}
	/* Get DB DLL set PICS data function */
	pSetValue SetValue = NULL;
	SetValue = (pSetValue)::GetProcAddress(m_h_DBModule, "SetValue");
	if (SetValue == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nSetValue == NULL!");
		return false;
	}

	/* Get DB DLL Insert test record function */
	pInsertFAIL_data InsertFAIL_data = NULL;
	InsertFAIL_data = (pInsertFAIL_data)::GetProcAddress(m_h_DBModule, "InsertFAIL_data");
	if (InsertFAIL_data == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nInsertFAIL_data == NULL!");
		return false;
	}
	pInsertPASS_data InsertPASS_data = NULL;
	InsertPASS_data = (pInsertPASS_data)::GetProcAddress(m_h_DBModule, "InsertPASS_data");
	if (InsertPASS_data == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nInsertPASS_data == NULL!");
		return false;
	}

	pInsertCRHoldInfo iInsertCRHoldInfo = NULL;
	iInsertCRHoldInfo = (pInsertCRHoldInfo)::GetProcAddress(m_h_DBModule, "InsertCRHoldInfo");
	if (iInsertCRHoldInfo == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nInsertCRHoldInfo == NULL!");
		return false;
	}
	
	pInsertOOBStatus iInsertOOBStatus = NULL;
	iInsertOOBStatus = (pInsertOOBStatus)::GetProcAddress(m_h_DBModule, "InsertOOBStatus");
	if (iInsertOOBStatus == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nInsertOOBStatus == NULL!");
		return false;
	}

	
	/* clear DB Access DLL PICS data map */
	if (!Clear_PICSData())
	{
		m_str_errorCode = CommErr_UI_CIM_Pics_Clean_Fail;
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

	if (m_st_idParameter.str_station == STATION_IMEI)
	{
		SetValue("picasso", T2A(map_tempPICSData[_T("L1")]));

		SetValue("sw", T2A(map_tempPICSData[_T("L9")]));

		SetValue("hw", T2A(map_tempPICSData[_T("L8")]));

		SetValue("lineid", T2A(map_tempPICSData[_T("line")].Right(2)));

		SetValue("mal", T2A(map_tempPICSData[_T("L13")]));

		CStringToChar(m_str_IMEI,sz_ID);
	}

	/* Insert test record to CIM */
	Log(_T("The picasso to be insert is ") + ConvertString(sz_ID));
	if (b_pass)
	{
		if(!InsertPASS_data(sz_ID, sz_SO, sz_employeeID, sz_errorCode, sz_errorMsg))
		{
			m_str_errorCode = ConvertString(sz_errorCode);
			m_str_errorMsg = ConvertString(sz_errorMsg);
			return false;
		}

		char sz_CRtype[20]={0};
		char sz_model[30]={0};
		char sz_process[30]={0};
		char sz_segment[30]={0};
		char sz_pn[100]={0};
		CStringToChar(m_st_idParameter.str_CRType,sz_CRtype);
		CStringToChar(m_st_idParameter.str_modelName,sz_model);
		CStringToChar(m_st_idParameter.str_station,sz_process);
		CStringToChar(m_st_idParameter.str_segment,sz_segment);
		CStringToChar(m_st_idParameter.str_partNumber,sz_pn);


		if (!iInsertCRHoldInfo(sz_picasso,sz_imei,sz_SO,sz_CRtype,sz_model,sz_process,sz_segment,sz_employeeID,sz_pn,sz_errorCode,sz_errorMsg))
		{
			m_str_errorCode = ConvertString(sz_errorCode);
			m_str_errorMsg = ConvertString(sz_errorMsg);
			return false;
		}

		if (m_b_OOBCheck && (m_st_idParameter.str_station == STATION_FCHECK))
		{
			
			if (!iInsertOOBStatus(sz_model,sz_SO,sz_pn,sz_imei,sz_errorCode,sz_errorMsg))
			{
				m_str_errorCode = ConvertString(sz_errorCode);
				m_str_errorMsg = ConvertString(sz_errorMsg);
				return false;
			}
		}
	}
	else
	{
		Log(_T("Insert Fail data!"));
		m_b_OOBCheck = false;
		if(!InsertFAIL_data(sz_ID, sz_SO, sz_employeeID, sz_inputerrorCode, sz_errorCode, sz_errorMsg))
		{
			m_str_errorCode = ConvertString(sz_errorCode);
			m_str_errorMsg = ConvertString(sz_errorMsg);
			return false;
		}
	}

	return true;
}

bool CQueryCIM::CheckPicassoStatusCIM( char * sz_Picasso, char * sz_TimeInfo,char * sz_Status )
{
	pCheckPicassoStatus CheckPicassoStatus = NULL;
	CheckPicassoStatus = (pCheckPicassoStatus)::GetProcAddress(m_h_DBModule, "CheckPicassoStatus");
	if (CheckPicassoStatus == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nCheckPicassoStatus == NULL!");
		return false;
	}

	char sz_ErrCode[15]={0};
	char sz_ErrMsg[MAX_BUFFER_SIZE]={0};
	if (!CheckPicassoStatus(sz_Picasso, sz_TimeInfo, sz_Status, sz_ErrCode, sz_ErrMsg))
	{
		m_str_errorCode = ConvertString(sz_ErrCode);
		m_str_errorMsg = ConvertString(sz_ErrMsg);
		return false;
	}

	return true;
}

bool CQueryCIM::CheckPicassoDuplicate_SndStationCIM( char * sz_Picasso, char * sz_TimeInfo )
{
	pCheckPicassoDuplicate_SecondStation CheckPicassoDuplicate_SecondStation = NULL;
	CheckPicassoDuplicate_SecondStation = (pCheckPicassoDuplicate_SecondStation)::GetProcAddress(m_h_DBModule, "CheckPicassoDuplicate_SecondStation");
	if (CheckPicassoDuplicate_SecondStation == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nCheckPicassoDuplicate_SecondStation == NULL!");
		return false;
	}

	char sz_ErrCode[15]={0};
	char sz_ErrMsg[MAX_BUFFER_SIZE]={0};
	if (!CheckPicassoDuplicate_SecondStation(sz_Picasso, sz_TimeInfo, sz_ErrCode, sz_ErrMsg))
	{
		m_str_errorCode = ConvertString(sz_ErrCode);
		m_str_errorMsg = ConvertString(sz_ErrMsg);
		return false;
	}


	return true;

}

bool CQueryCIM::InsertPicassoControlCIM( char * sz_Picasso, char * sz_TimeInfo,char * sz_Status )
{

	pInsertPicassoControl InsertPicassoControl = NULL;
	InsertPicassoControl = (pInsertPicassoControl)::GetProcAddress(m_h_DBModule, "InsertPicassoControl");
	if (InsertPicassoControl == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nInsertPicassoControl == NULL!");
		return false;
	}

	char sz_ErrCode[15]={0};
	char sz_ErrMsg[MAX_BUFFER_SIZE]={0};
	if (!InsertPicassoControl(sz_Picasso, sz_TimeInfo, sz_Status,sz_ErrCode, sz_ErrMsg))
	{
		m_str_errorCode = ConvertString(sz_ErrCode);
		m_str_errorMsg = ConvertString(sz_ErrMsg);
		return false;
	}

	return true;
}


bool CQueryCIM::QueryPicasso( char * sz_InputID, char * sz_Model,char * sz_Picasso )
{
	pGetPicasso iGetPicasso = NULL;
	iGetPicasso = (pGetPicasso)::GetProcAddress(m_h_DBModule, "GetPicasso");
	if (iGetPicasso == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nGetPicasso == NULL!");
		return false;
	}

	char sz_ErrCode[15]={0};
	char sz_PN[20]={0};
	char sz_ErrMsg[MAX_BUFFER_SIZE]={0};

	CString str_errcode,str_errmsg;
	if (!iGetPicasso(sz_InputID, sz_Model, sz_Picasso,sz_PN, sz_ErrCode, sz_ErrMsg))
	{
		m_str_errorCode = ConvertString(sz_ErrCode);
		m_str_errorMsg = ConvertString(sz_ErrMsg);

		return false;
	}
	
	return true;

}

bool CQueryCIM::QueryIMEIresult( char * sz_InputID, char * sz_Model,char * sz_Picasso )
{
	pQueryIMEIresult iQueryIMEIresult = NULL;
	iQueryIMEIresult = (pQueryIMEIresult)::GetProcAddress(m_h_DBModule, "QueryIMEIresult");
	if (iQueryIMEIresult == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQueryIMEIresult == NULL!");
		return false;
	}


	pQueryIMEI_ByPicasso iQueryIMEI_ByPicasso = NULL;
	iQueryIMEI_ByPicasso = (pQueryIMEI_ByPicasso)::GetProcAddress(m_h_DBModule, "QueryIMEI_ByPicasso");
	if (iQueryIMEI_ByPicasso == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQueryIMEI_ByPicasso == NULL!");
		return false;
	}

	pQueryIMEIStatus iQueryIMEIStatus = NULL;
	iQueryIMEIStatus = (pQueryIMEIStatus)::GetProcAddress(m_h_DBModule, "QueryIMEIStatus");
	if (iQueryIMEIStatus == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQueryIMEIStatus == NULL!");
		return false;
	}


	char sz_ErrCode[15]={0};
	char sz_ErrMsg[MAX_BUFFER_SIZE]={0};
	char sz_CIMPicasso[20] = {0};

	CString str_errcode,str_errmsg;
	if (!iQueryIMEIresult(sz_InputID,sz_Model,sz_CIMPicasso, sz_ErrCode, sz_ErrMsg))
	{
		m_str_errorCode = ConvertString(sz_ErrCode);
		m_str_errorMsg = ConvertString(sz_ErrMsg);

		return false;
	}
	//if one exist,make sure picasso in mobile and cim same	 
	if ((strlen(sz_CIMPicasso) == PICASSO_LENGTH)&&(strcmp(sz_Picasso,sz_CIMPicasso) != 0))
	{
		m_str_errorCode = CommErr_UI_CIM_Picasso_Match_IMEI_Fail;
		m_str_errorMsg.Format(_T("Picasso in target(%s) and CIM(%s) mismatched for IMEI(%s)!\n手机中的picasso与CIM中不匹配"),ConvertString(sz_Picasso),ConvertString(sz_CIMPicasso),ConvertString(sz_InputID));
		return false;
	}

	char sz_CIMimei[20]={0};
	if (!iQueryIMEI_ByPicasso(sz_Picasso,sz_CIMimei,sz_ErrCode,sz_ErrMsg))
	{
		m_str_errorCode = ConvertString(sz_ErrCode);
		m_str_errorMsg = ConvertString(sz_ErrMsg);

		return false;
	}
	else if ((strlen(sz_CIMimei) == IMEI_LENGTH)&&(strcmp(sz_CIMimei,sz_InputID) != 0))
	{
		m_str_errorCode = CommErr_UI_CIM_Picasso_Match_IMEI_Fail;
		m_str_errorMsg.Format(_T("IMEI in scaned(%s) and CIM(%s) mismatched for picasso(%s)!\n扫入的IMEI与CIM中不匹配"),ConvertString(sz_InputID),ConvertString(sz_CIMimei),ConvertString(sz_Picasso));
		return false;
	}

	char sz_status[20]={0};
	char sz_carton[50]={0};
	if (!iQueryIMEIStatus(sz_InputID,sz_status,sz_carton,sz_ErrCode,sz_ErrMsg))
	{
		m_str_errorCode = ConvertString(sz_ErrCode);
		m_str_errorMsg = ConvertString(sz_ErrMsg);
		return false;
	}
	else if ((strcmp(sz_status,"P") == 0) || (strcmp(sz_status,"S") == 0))
	{
		m_str_errorCode = CommErr_UI_CIM_IMEI_Status_Invalide;
		m_str_errorMsg.Format(_T("IMEI Status is %s"),ConvertString(sz_status));
		return false;
	}

	m_str_IMEI = ConvertString(sz_InputID);
	return true;

}

bool CQueryCIM::QueryIMEIStatus( char * sz_InputID, char * sz_status)
{
	pQueryIMEIStatus iQueryIMEIStatus = NULL;
	iQueryIMEIStatus = (pQueryIMEIStatus)::GetProcAddress(m_h_DBModule, "QueryIMEIStatus");
	if (iQueryIMEIStatus == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
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
	pQueryEmployeeID_Right QueryEmployeeID_Right = NULL;
	QueryEmployeeID_Right = (pQueryEmployeeID_Right)::GetProcAddress(m_h_DBModule, "QueryEmployeeID_Right");
	if (QueryEmployeeID_Right == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQueryEmployeeID_Right == NULL!");
		return false;
	}

	/* Query employee level */
	if (!QueryEmployeeID_Right(sz_employeeID, sz_level, sz_errorCode, sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	str_level = ConvertString(sz_level);

	if (str_level.CompareNoCase(_T("10")) == 0)
	{
		m_st_idParameter.b_tsFlag = true;
	}
	else
	{
		m_st_idParameter.b_tsFlag = false;

#ifdef TS_MODE
		m_str_errorCode = CommErr_UI_CIM_EmployeeID_Not_TS;
		m_str_errorMsg =ConvertString(sz_employeeID) + _T("不是合法的TS员工号");
		return false;
#endif

	}

	/* Save CIM PICS data(employee) */
	m_map_picsData[_T("employee")] = ConvertString(sz_employeeID);

	return true;

}

bool CQueryCIM::QuerySOInfo()
{

	if (m_st_idParameter.b_tsFlag)
	{
		m_map_picsData[_T("so")] = m_st_idParameter.str_so;
		m_map_picsData[_T("model")] = m_st_idParameter.str_modelName;
		m_map_picsData[_T("partno")] = m_st_idParameter.str_partNumber;
		m_map_picsData[_T("sotype")] = m_st_idParameter.str_soType;

	}
	else
	{
		USES_CONVERSION;

		char sz_SO[10] = {0};
		char sz_model[15] = {0};
		char sz_PN[15] = {0};
		char sz_SOType[15] = {0};
		char sz_errorCode[15] = {0};
		char sz_errorMsg[MAX_BUFFER_SIZE] = {0};

		strncpy_s(sz_SO, sizeof(sz_SO), T2A(m_st_idParameter.str_so), sizeof(sz_SO));

		/* Get DB DLL Query SO function */
		pQuerySO_Info QuerySO_Info = NULL;
		QuerySO_Info = (pQuerySO_Info)::GetProcAddress(m_h_DBModule, "QuerySO_Info");
		if (QuerySO_Info == NULL)
		{
			m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
			m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQuerySO_Info == NULL!");
			return false;
		}

		/* Query SO info */
		if (!QuerySO_Info(sz_SO, sz_model, sz_PN, sz_SOType, sz_errorCode, sz_errorMsg))
		{
			m_str_errorCode = ConvertString(sz_errorCode);
			m_str_errorMsg = ConvertString(sz_errorMsg);
			return false;
		}

		m_st_idParameter.str_modelName = ConvertString(sz_model);
		m_st_idParameter.str_partNumber = ConvertString(sz_PN);
		m_st_idParameter.str_soType = ConvertString(sz_SOType);

		/* Save CIM PICS data(so, model, partno, sotype) */
		m_map_picsData[_T("so")] = m_st_idParameter.str_so;
		m_map_picsData[_T("model")] = m_st_idParameter.str_modelName;
		m_map_picsData[_T("partno")] = m_st_idParameter.str_partNumber;
		m_map_picsData[_T("sotype")] = m_st_idParameter.str_soType;

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
	//	strcpy(sz_PCName,"P03I02");//for Nancy.Yang test
	//}
	
	m_str_computerName = ConvertString(sz_PCName);

	/* Get DB DLL Query station function */
	pQueryStation_Info QueryStation_Info = NULL;
	QueryStation_Info = (pQueryStation_Info)::GetProcAddress(m_h_DBModule, "QueryStation_Info");
	if (QueryStation_Info == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQueryStation_Info == NULL!");
		return false;
	}
	
	/* Query station info */
	if (!QueryStation_Info(sz_PCName, sz_line, sz_process, sz_segment, sz_errorCode, sz_errorMsg))
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

bool CQueryCIM::QueryFirstProcessInfo()
{
	USES_CONVERSION;

	char sz_model[15] = {0};
	char sz_process[15] = {0};
	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};

	strncpy_s(sz_model, sizeof(sz_model), T2A(m_st_idParameter.str_modelName), sizeof(sz_model));

	/* Get DB DLL Query First Process function */
	pQueryFirstProcess QueryFirstProcess = NULL;
	QueryFirstProcess = (pQueryFirstProcess)::GetProcAddress(m_h_DBModule, "QueryFirstProcess");
	if (QueryFirstProcess == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQueryFirstProcess == NULL!");
		return false;
	}

	/* Query First Process */
	if (!QueryFirstProcess(sz_model, sz_process, sz_errorCode, sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}
 
	m_st_idParameter.str_firstProcess = ConvertString(sz_process);

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
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
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

bool CQueryCIM::QueryStationList()
{
	/* Get DB DLL Query record function */
	pQuerySegmentProcess QuerySegmentProcess = NULL;
	QuerySegmentProcess = (pQuerySegmentProcess)::GetProcAddress(m_h_DBModule, "QuerySegmentProcess");
	if (QuerySegmentProcess == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQuerySegmentProcess == NULL!");
		return false;
	}
	USES_CONVERSION;

	char str_segment[1] = {0};
	char sz_errorCode[15] = {0};
	char sz_errorMsg[256] = {0};
	char sz_processs[512] = {0};
	char sz_model[15] = {0};
	strncpy_s(sz_model, sizeof(sz_model), T2A(m_st_idParameter.str_modelName), sizeof(sz_model));

	/* Query record info */
	if (!QuerySegmentProcess(sz_model, str_segment, sz_processs, sz_errorCode, sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	CString str_process = _T("");
	str_process = ConvertString(sz_processs);

	if (str_process.GetLength() < 2)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("站列表不合法!\nInvalide Station List!");
		return false;
	}

	CString strTemp,strStationIndex,str_stationlist;
	int iStationIndex = 0;	

	while(str_process.GetLength()>1)
	{	
		strTemp = str_process.Left(str_process.Find(_T(",")));
		str_process = str_process.Mid(str_process.Find(_T(",")) + 1,str_process.GetLength());
		iStationIndex++;
		strStationIndex.Format(_T("%.2d"),iStationIndex);
		m_map_StationIndexMap[strTemp] = strStationIndex;		
	}

	//if so is CS
	if (m_b_csFlag)
	{
		//CS model ,get first segment of current segment
// 		memset(sz_processs,0,sizeof(sz_processs));
// 		char sz_segment[30]={0};
// 		CStringToChar(m_st_idParameter.str_segment,sz_segment);
// 		if (!QuerySegmentProcess(sz_model, sz_segment, sz_processs, sz_errorCode, sz_errorMsg))
// 		{
// 			m_str_errorCode = ConvertString(sz_errorCode);
// 			m_str_errorMsg = ConvertString(sz_errorMsg);
// 			return false;
// 		}
// 		else
// 		{
// 			strTemp = ConvertString(sz_processs);
// 			m_str_CS1 = strTemp.Left(strTemp.Find(_T(","))) ;
// 		}

		//change to normal model, get first station of FA segment
		//Nancy.Yang deleted for new csd rule 20130510
		/*memset(sz_processs,0,sizeof(sz_processs));	
		CString str_model = ConvertModelName(m_st_idParameter.str_modelName);
		CStringToChar(str_model,sz_model);

		if (!QuerySegmentProcess(sz_model, "FA", sz_processs, sz_errorCode, sz_errorMsg))
		{
			m_str_errorCode = ConvertString(sz_errorCode);
			m_str_errorMsg = ConvertString(sz_errorMsg);
			return false;
		}
		else
		{
			strTemp = ConvertString(sz_processs);
			m_str_BA1 = strTemp.Left(strTemp.Find(_T(","))) ;
		}*/
			
	}
	
	return true;
}


bool CQueryCIM::QueryStationRoutingInfo(char *sz_picasso )
{
	USES_CONVERSION;

	char sz_model[15] = {0};
	char sz_process[15] = {0};
	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};

	strncpy_s(sz_model, sizeof(sz_model), T2A(m_st_idParameter.str_modelName), sizeof(sz_model));
	strncpy_s(sz_process, sizeof(sz_process), T2A(m_st_idParameter.str_station), sizeof(sz_process));

	/* Get DB DLL Query station function */
	pCheckRouting CheckRouting = NULL;
	//CheckRouting = (pCheckRouting)::GetProcAddress(m_h_DBModule, "CheckRouting_New");
	//Nancy.Yang changed from CheckRouting_New to CheckRouting_V2 for new csd rule 20130510 
	CheckRouting = (pCheckRouting)::GetProcAddress(m_h_DBModule, "CheckRouting_V2");
	if (CheckRouting == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nCheckRouting == NULL!");
		return false;
	}

	/* Check station routing */
	if (!CheckRouting(sz_picasso, sz_model, sz_process, sz_errorCode, sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	return true;
}

bool CQueryCIM::QueryStationRoutingInfoCS(char *sz_picasso )
{
	USES_CONVERSION;
	char sz_model[15] = {0};
	char sz_process[15] = {0};
	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};


	/* Get DB DLL Query station function */
	pCheckRouting CheckRouting = NULL;
	CheckRouting = (pCheckRouting)::GetProcAddress(m_h_DBModule, "CheckRouting_New");
	if (CheckRouting == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nCheckRouting == NULL!");
		return false;
	}

	//if this is the first csd station, check first station of normal model of this segment
	if (m_st_idParameter.str_station == m_st_idParameter.str_firstProcess)
	{
		CString str_model = ConvertModelName(m_st_idParameter.str_modelName);
		strncpy_s(sz_model, sizeof(sz_model), T2A(str_model), sizeof(sz_model));
		strncpy_s(sz_process, sizeof(sz_process), T2A(m_str_BA1), sizeof(sz_process));
	}
	else
	{
		strncpy_s(sz_model, sizeof(sz_model), T2A(m_st_idParameter.str_modelName), sizeof(sz_model));
		strncpy_s(sz_process, sizeof(sz_process), T2A(m_st_idParameter.str_station), sizeof(sz_process));
	}

	/* Check station routing */
	if (!CheckRouting(sz_picasso, sz_model, sz_process, sz_errorCode, sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	return true;
}

bool CQueryCIM::CheckSO( char *sz_picasso )
{
	/* Get DB DLL Query station function */
	pCheckSO iCheckSO = NULL;
	iCheckSO = (pCheckSO)::GetProcAddress(m_h_DBModule, "CheckSO");
	if (iCheckSO == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
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




bool CQueryCIM::QuerySOInfo_ALL()
{

#ifndef FACTORY_MODE
	m_map_picsData[_T("so")] = m_st_idParameter.str_so;
	m_map_picsData[_T("model")] = m_st_idParameter.str_modelName;
	m_map_picsData[_T("partno")] = m_st_idParameter.str_partNumber;
	m_map_picsData[_T("sotype")] = m_st_idParameter.str_soType;
#else

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

	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};

	strncpy_s(sz_SO, sizeof(sz_SO), T2A(m_st_idParameter.str_so), sizeof(sz_SO));

	/* Get DB DLL Query SO function */
	pQuerySO_Info_All_V2 iQuerySO_Info_All_V2 = NULL;
	iQuerySO_Info_All_V2 = (pQuerySO_Info_All_V2)::GetProcAddress(m_h_DBModule, "QuerySO_Info_All_V2");
	if (iQuerySO_Info_All_V2 == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQuerySO_Info == NULL!");
		return false;
	}

	/* Query SO info */
	if (!iQuerySO_Info_All_V2(sz_SO, sz_model, sz_PN, sz_SOType,sz_ToolVersion,sz_Factoryversion,sz_userversion,sz_entrancetoolversion,sz_Qty, sz_errorCode, sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	m_st_idParameter.str_modelName = ConvertString(sz_model);
	m_st_idParameter.str_partNumber = ConvertString(sz_PN);
	m_st_idParameter.str_soType = ConvertString(sz_SOType);
	m_st_idParameter.str_factoryVer = ConvertString(sz_Factoryversion);
	//m_st_idParameter.str_toolVer = ConvertString(sz_ToolVersion);
	m_st_idParameter.str_userLoadVer = ConvertString(sz_userversion);

	m_i_SoQty = atoi(sz_Qty);

	/* Save CIM PICS data(so, model, partno, sotype) */
	m_map_picsData[_T("so")] = m_st_idParameter.str_so;
	m_map_picsData[_T("model")] = m_st_idParameter.str_modelName;
	m_map_picsData[_T("partno")] = m_st_idParameter.str_partNumber;
	m_map_picsData[_T("sotype")] = m_st_idParameter.str_soType;

	

	//Nancy.Yang deleted for new csd rule 20130510
	/*if (ConvertModelName(m_st_idParameter.str_modelName) != m_st_idParameter.str_modelName)
	{
		m_b_csFlag = true;
	}
	else
	{
		m_b_csFlag = false;
	}*/

	if(!QueryIDtype())
		return false;
#endif

	return true;
}

bool CQueryCIM::QuerySOInfo_ALL_V3()
{

#ifndef FACTORY_MODE
	m_map_picsData[_T("so")] = m_st_idParameter.str_so;
	m_map_picsData[_T("model")] = m_st_idParameter.str_modelName;
	m_map_picsData[_T("partno")] = m_st_idParameter.str_partNumber;
	m_map_picsData[_T("sotype")] = m_st_idParameter.str_soType;
#else

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
	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};

	strncpy_s(sz_SO, sizeof(sz_SO), T2A(m_st_idParameter.str_so), sizeof(sz_SO));

	/* Get DB DLL Query SO function */
	pQuerySO_Info_All_V3 iQuerySO_Info_All_V3 = NULL;
	iQuerySO_Info_All_V3 = (pQuerySO_Info_All_V3)::GetProcAddress(m_h_DBModule, "QuerySO_Info_All_V3");
	if (iQuerySO_Info_All_V3 == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQuerySO_Info_All_V3 == NULL!");
		return false;
	}

	/* Query SO info */
	if (!iQuerySO_Info_All_V3(sz_SO, sz_model, sz_PN, sz_SOType,sz_ToolVersion,sz_Factoryversion,sz_userversion,sz_entrancetoolversion,sz_Qty, sz_CRType, sz_errorCode, sz_errorMsg))
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



	//Nancy.Yang deleted for new csd rule 20130510
	/*if (ConvertModelName(m_st_idParameter.str_modelName) != m_st_idParameter.str_modelName)
	{
		m_b_csFlag = true;
	}
	else
	{
		m_b_csFlag = false;
	}*/

	if(!QueryIDtype())
		return false;

#endif

	return true;
}

//Nancy.yt.Zhu,20121127
bool CQueryCIM::QuerySOInfo_ALL_V5()
{

#ifndef FACTORY_MODE
	m_map_picsData[_T("so")] = m_st_idParameter.str_so;
	m_map_picsData[_T("model")] = m_st_idParameter.str_modelName;
	m_map_picsData[_T("partno")] = m_st_idParameter.str_partNumber;
	m_map_picsData[_T("sotype")] = m_st_idParameter.str_soType;
#else

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
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
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

	m_str_FixImgVer = ConvertString(sz_Fix_Img_ver);
	m_i_SoQty = atoi(sz_Qty);

	/* Save CIM PICS data(so, model, partno, sotype) */
	m_map_picsData[_T("so")] = m_st_idParameter.str_so;
	m_map_picsData[_T("model")] = m_st_idParameter.str_modelName;
	m_map_picsData[_T("partno")] = m_st_idParameter.str_partNumber;
	m_map_picsData[_T("sotype")] = m_st_idParameter.str_soType;


	//Nancy.Yang deleted for new csd rule 20130510
	/*if (ConvertModelName(m_st_idParameter.str_modelName) != m_st_idParameter.str_modelName)
	{
		m_b_csFlag = true;
	}
	else
	{
		m_b_csFlag = false;
	}*/

	if(!QueryIDtype())
		return false;

#endif

	return true;
}

bool CQueryCIM::CheckNAL_duplicate( char * sz_IMEI, char * sz_NAL )
{
	pCheckNAL_duplicate iCheckNAL_duplicate = NULL;
	iCheckNAL_duplicate = (pCheckNAL_duplicate)::GetProcAddress(m_h_DBModule,"CheckNAL_duplicate");
	if (iCheckNAL_duplicate == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nCheckNAL_duplicate == NULL!");
		return false;
	}

	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};
	/* Query NAL and imei matchable info */
	if (!iCheckNAL_duplicate(sz_IMEI, sz_NAL,sz_errorCode, sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	return true;
}

bool CQueryCIM::CheckNAL_duplicate_V2( char * sz_IMEI, char * sz_NAL )
{
	USES_CONVERSION;

	pCheckNAL_duplicate_V2 iCheckNAL_duplicate_V2 = NULL;
	iCheckNAL_duplicate_V2 = (pCheckNAL_duplicate_V2)::GetProcAddress(m_h_DBModule,"CheckNAL_duplicate_V2");
	if (iCheckNAL_duplicate_V2 == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nCheckNAL_duplicate_V2 == NULL!");
		return false;
	}

	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};
	char sz_process[15] = {0};

	strncpy_s(sz_process, sizeof(sz_process), T2A(m_st_idParameter.str_station), sizeof(sz_process));

	/* Query NAL and imei matchable info */
	if (!iCheckNAL_duplicate_V2(sz_process,sz_IMEI, sz_NAL,sz_errorCode, sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	return true;
}

bool CQueryCIM::CheckNAL_Valide(char * sz_NAL )
{
	pCheckNAL_Validity iCheckNAL_Validity = NULL;
	iCheckNAL_Validity = (pCheckNAL_Validity)::GetProcAddress(m_h_DBModule,"CheckNAL_Validity");
	if (iCheckNAL_Validity == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nCheckNAL_Validity == NULL!");
		return false;
	}

	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};
	char sz_pn[60]={0};
	CStringToChar(m_st_idParameter.str_partNumber,sz_pn);
	/* Query NAL valide*/
	if (!iCheckNAL_Validity(sz_pn, sz_NAL,sz_errorCode, sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	return true;
}

bool CQueryCIM::QuerySOCount(char * sz_id)
{
	pQuerySOCount_Control iQuerySOCount = NULL;
	iQuerySOCount = (pQuerySOCount_Control)::GetProcAddress(m_h_DBModule,"QuerySOCount_Control");
	if (iQuerySOCount == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
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
			m_str_errorCode = CommErr_UI_CIM_SO_CR_Count_Over_Upper;
			m_str_errorMsg.Format(_T("当前工单测试数量(%d)已经超过额定数量(%d),请更换工单!\nTest count exceeds so count!"),i_current,m_i_SoQty);
			return false;
		}
		//2012.09.24 if i_current = 0, means it is rework one in same SO, not match the condition to check OOB
		if (i_current && (m_st_idParameter.str_station == STATION_FCHECK))
		{
			m_b_OOBCheck = (i_current%m_i_SampleRate == 0);
		}
		else
		{
			m_b_OOBCheck = false;
		}
			
	}

	return true;

}

bool CQueryCIM::QueryNAL( char * sz_IMEI, char * sz_NAL )
{
	pQueryNAL iQueryNAL = NULL;
	iQueryNAL = (pQueryNAL)::GetProcAddress(m_h_DBModule,"QueryNAL");
	if (iQueryNAL == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQueryNAL == NULL!");
		return false;
	}

	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};

	/* Query NAL */
	if (!iQueryNAL(sz_IMEI, sz_NAL, sz_errorCode,sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	return true;
}

bool CQueryCIM::QueryIfTestAgain( char * sz_id, char* sz_model, char * sz_process, char *sz_cnt)
{
	pQueryIfTestAgain iQueryIfTestAgain = NULL;
	iQueryIfTestAgain = (pQueryIfTestAgain)::GetProcAddress(m_h_DBModule,"QueryIfTestAgain");
	if (iQueryIfTestAgain == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQueryIfTestAgain == NULL!");
		return false;
	}

	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};

	/* Query NAL */
	if (!iQueryIfTestAgain(sz_id, sz_model,sz_process, sz_cnt, sz_errorCode,sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	return true;
}

bool CQueryCIM::InsertTS_Data(char * sz_id, char * sz_model,char * sz_rtnprocess, char * sz_ngprocess, char * sz_ngwrkstation)
{
	pInsertTSData_forTS iInsertTSData_forTS = NULL;
	iInsertTSData_forTS = (pInsertTSData_forTS)::GetProcAddress(m_h_DBModule,"InsertTSData_forTS");
	if (iInsertTSData_forTS == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nInsertTSData_forTS == NULL!");
		return false;
	}

	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};

	if (!iInsertTSData_forTS(sz_id, sz_model,sz_rtnprocess,sz_ngprocess,sz_ngwrkstation, sz_errorCode,sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
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
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
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
		m_str_errorCode = CommErr_UI_CIM_Picasso_QA_Hold;
		m_str_errorMsg = ConvertString(sz_holdreas);//_T("此ID已经被Hold，请联系QA解除Hold\nThe status of this id is H");
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
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
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
bool CQueryCIM::QueryOOBSampleRate()
{
	pQueryOOBSampleRate iQueryOOBSampleRate = NULL;
	iQueryOOBSampleRate = (pQueryOOBSampleRate)::GetProcAddress(m_h_DBModule,"QueryOOBSampleRate");
	if (iQueryOOBSampleRate == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQueryOOBSampleRate == NULL!");
		return false;
	}

	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};
	char sz_modelname[50]={0};
	char sz_so[50]={0};
	char sz_samplerate[50]={0};

	CStringToChar(m_st_idParameter.str_modelName,sz_modelname);

	CStringToChar(m_st_idParameter.str_so,sz_so);

	if (!iQueryOOBSampleRate( sz_modelname,sz_so,sz_samplerate, sz_errorCode,sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	m_i_SampleRate = atoi(sz_samplerate);

	return true;
}

bool CQueryCIM::UnlinkKeyparts(char * sz_id, char * sz_model)
{
	pUnlinkKeyparts iUnlinkKeyparts = NULL;
	iUnlinkKeyparts = (pUnlinkKeyparts)::GetProcAddress(m_h_DBModule,"UnlinkKeyparts");
	if (iUnlinkKeyparts == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nUnlinkKeyparts == NULL!");
		return false;
	}

	char sz_errorCode[15] = {0};
	char sz_errorMsg[MAX_BUFFER_SIZE] = {0};

	if (!iUnlinkKeyparts( sz_id ,sz_model, sz_errorCode,sz_errorMsg))
	{
		m_str_errorCode = ConvertString(sz_errorCode);
		m_str_errorMsg = ConvertString(sz_errorMsg);
		return false;
	}

	return true;
}

/*************************************************for VMS SQN db***********************************/
bool CQueryCIM::UnLoadVMSDBAccessDLL()
{
	if (m_h_VMSDBModule != NULL)
	{
		FreeLibrary (m_h_VMSDBModule);
		m_h_VMSDBModule = NULL;
	}
	return true;
}

bool CQueryCIM::LoadVMSDBAccessDLL()
{
	/* Load library */
	if (m_h_VMSDBModule == NULL)
	{
		m_h_VMSDBModule = ::LoadLibrary(_T("VMS_DBAccess.dll"));

		if (m_h_VMSDBModule == NULL)
		{
			return false;
		}
	}

	return true;
}

bool CQueryCIM::ConnectVMSDataBase()
{
	/* Get DB DLL Query employee level function */
	pVMS_ConnectDB iVMS_ConnectDB = NULL;
	iVMS_ConnectDB = (pVMS_ConnectDB)::GetProcAddress(m_h_VMSDBModule, "VMS_ConnectDB");
	if (iVMS_ConnectDB == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载SQN数据库DLL函数失败!\nLoad SQN DB dll VMS_ConnectDB=NULL");
		return false;
	}

	/* Connect DataBase */
	if (!iVMS_ConnectDB())
	{
		m_str_errorCode = CommErr_UI_CIM_Connect_Fail;
		m_str_errorMsg = _T("连接SQN数据库失败!\nConnect SQN Database fail!");
		return false;
	}

	return true;

}

bool CQueryCIM::CloseVMSDataBase()
{
	/* Get DB DLL Query employee level function */
	pVMS_CloseDB iVMS_CloseDB = NULL;
	iVMS_CloseDB = (pVMS_CloseDB)::GetProcAddress(m_h_VMSDBModule, "VMS_CloseDB");
	if (m_h_VMSDBModule == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载SQN数据库DLL函数失败!\nm_h_VMSDBModule == NULL!");
		return false;
	}
	if (iVMS_CloseDB == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载SQN数据库DLL函数失败!\nVMS_CloseDB = NULL!");

		return false;
	}

	/* Close DataBase */
	iVMS_CloseDB();

	return true;
}

bool CQueryCIM::VMS_QueryDataFromSQN()
{
	bool b_ret = true;
	
	b_ret = LoadVMSDBAccessDLL();
	if (b_ret)
	{
		b_ret = ConnectVMSDataBase();
	}
	else
	{
		UnLoadVMSDBAccessDLL();
		return false;
	}

	if (b_ret)
	{
		char sz_model[30]={0};
		char sz_partno[50]={0};
		char sz_count[20]={0};
		char sz_toolversion[50]={0};
		char sz_pnInfo[4096*2] = {0};

		CStringToChar(m_st_idParameter.str_modelName,sz_model);
		CStringToChar(m_st_idParameter.str_partNumber,sz_partno);
		CStringToChar(m_st_idParameter.str_toolVer,sz_toolversion);
		CString str_pn2 = m_st_idParameter.str_partNumber.Left(2);

		if (str_pn2 == _T("5E") || str_pn2 == _T("6K"))
		{
			b_ret = VMS_Query5Einfo(sz_model,sz_partno,sz_count,sz_pnInfo);
		}
		else if (str_pn2 == _T("5G"))
		{
			b_ret = VMS_Query5Ginfo(sz_model,sz_partno,sz_count,sz_pnInfo);
		}
		else if (str_pn2 == _T("9J") || str_pn2 == _T("CS") || str_pn2 == _T("5J") || str_pn2 == _T("9B"))
		{
			char sz_userload[30]={0};

			CStringToChar(m_st_idParameter.str_userLoadVer,sz_userload);

			b_ret = VMS_QuerySQNinfo_All(sz_model,sz_partno,sz_userload, sz_count,sz_pnInfo);
		}

		if (b_ret)
		{
			Log(CString(sz_pnInfo));
			b_ret = SplitStringToMap(sz_count,sz_pnInfo);
			m_map_TestKeyValue.erase(_T("SW_version"));//erase by Nancy.Yang 
		}

		Log(m_st_idParameter.str_segment);
		if ((m_st_idParameter.str_segment.CompareNoCase(_T("FA"))==0)
			||(m_st_idParameter.str_segment.CompareNoCase(_T("PACKAGE"))==0))
		{
			char sz_imageList[MAX_BUFFER_SIZE]={0};

			/* Nancy.yt.Zhu 2012.09.13 */
			CString str_imgtableVersionList;
			CString str_vrcmVersionList;
			CString str_temp;
			if (b_ret)
			{
				/* Nancy.yt.Zhu 2012.11.27 Image version unfixed*/
				Log(_T("isFixImgVer: ") + m_str_FixImgVer);
				if (m_str_FixImgVer != "Y")
				{				
					//20120929 CR SO need not to query PVCS, use the ImageVer in CIM
					Log(_T("CRType: ") + m_st_idParameter.str_CRType);
					if(m_st_idParameter.str_CRType != _T("C"))
					{
						b_ret = VMS_QueryNormalImageList(sz_model,sz_imageList);

						if (b_ret)
						{
							str_imgtableVersionList = ConvertString(sz_imageList);

							Log(_T("Image version list read from image_version_control is: ") + str_imgtableVersionList);

							ZeroMemory(sz_imageList,sizeof(sz_imageList));

							b_ret = VMS_QueryVRCMRImageList(sz_model,sz_partno,sz_toolversion,sz_imageList);

							if (b_ret)
							{
								str_vrcmVersionList = ConvertString(sz_imageList);

								Log(_T("Image version list read from vrcm is: ") + str_imgtableVersionList);

								str_temp = StringIntersection(str_imgtableVersionList,str_vrcmVersionList);

								Log(_T("The intersection of two image lists is: ") + str_temp);

								if (str_temp == "")
								{
									m_str_errorCode = CommErr_UI_PVCS_ImageList_Empty_Fail;
									m_str_errorMsg = _T("非CR工单，从VRCM和Image表里获取交集为空!\nGet image list empty!");
									b_ret = false;
								}
								else
								{
                                    m_st_idParameter.str_factoryVer = str_temp;
								}
							}
							else
							{
								if (m_str_errorCode == _T("NoRecord"))
								{
									m_str_errorCode = _T("");
									Log(_T("No record of this model in VRCM!"));
									CloseVMSDataBase();
									UnLoadVMSDBAccessDLL();
									return true;
								}
							}
						}
					}
				}
			}

			//delete by Nancy.Yang on 20130722
			/*if (b_ret)
			{
				if (m_map_TestKeyValue.find(_T(HW_VERSION)) != m_map_TestKeyValue.end())
				{
					if (!m_map_TestKeyValue[_T(HW_VERSION)].IsEmpty())
					{
						m_st_idParameter.str_pcba = m_map_TestKeyValue[_T(HW_VERSION)];
					}
				}
			}*/
		}	
	}
	
	if (b_ret)
	{
		char sz_model[30]={0};
		char sz_imageVer[50]={0};
		CStringToChar(m_st_idParameter.str_modelName,sz_model);
		CStringToChar(m_st_idParameter.str_factoryVer,sz_imageVer);

		Log(_T("str_factoryVer: ") + m_st_idParameter.str_factoryVer);

		if (m_st_idParameter.str_factoryVer.Find(_T(",")) == -1)
		{
			b_ret = VMS_QueryImageVersionReserveValue(sz_model,sz_imageVer);
		}
	}

	CloseVMSDataBase();

	UnLoadVMSDBAccessDLL();

	return b_ret;

}

bool CQueryCIM::VMS_Query5Einfo( char * sz_Model, char * sz_Partno,char *sz_count, char *sz_pnInfo )
{
	pVMS_Query5Einfo iVMS_Query5Einfo = NULL;
	iVMS_Query5Einfo = (pVMS_Query5Einfo)::GetProcAddress(m_h_VMSDBModule,"VMS_Query5Einfo");
	if (iVMS_Query5Einfo == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nVMS_Query5Einfo == NULL!");
		return false;
	}

	char sz_errCode[20]={0};
	char sz_errMsg[1024]={0};

	if (!iVMS_Query5Einfo(sz_Model,sz_Partno,sz_count, sz_pnInfo,sz_errCode,sz_errMsg))
	{
		m_str_errorMsg = ConvertString(sz_errMsg);
		m_str_errorCode = ConvertString(sz_errCode);
		return false;
	}

	return true;
}

bool CQueryCIM::VMS_Query5Ginfo( char *sz_Model, char *sz_Partno,char *sz_count, char *sz_pnInfo )
{
	pVMS_Query5Ginfo iVMS_Query5Ginfo = NULL;
	iVMS_Query5Ginfo = (pVMS_Query5Ginfo)::GetProcAddress(m_h_VMSDBModule,"VMS_Query5Ginfo");
	if (iVMS_Query5Ginfo == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nVMS_Query5Ginfo == NULL!");
		return false;
	}

	char sz_errCode[20]={0};
	char sz_errMsg[1024]={0};
	if (!iVMS_Query5Ginfo(sz_Model,sz_Partno,sz_count, sz_pnInfo,sz_errCode,sz_errMsg))
	{
		m_str_errorMsg = ConvertString(sz_errMsg);
		m_str_errorCode = ConvertString(sz_errCode);
		return false;
	}

	return true;
}

bool CQueryCIM::VMS_QuerySQNinfo_All(char * sz_Model, char * sz_Partno, char * sz_SWver,char *sz_count, char *sz_pnInfo )
{
	pVMS_QuerySQNinfo_All iVMS_QuerySQNinfo_All = NULL;
	iVMS_QuerySQNinfo_All = (pVMS_QuerySQNinfo_All)::GetProcAddress(m_h_VMSDBModule,"VMS_QuerySQNinfo_All");
	if (iVMS_QuerySQNinfo_All == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nVMS_QuerySQNinfo_V2 == NULL!");
		return false;
	}

	char sz_errCode[20]={0};
	char sz_errMsg[1024]={0};
	if (!iVMS_QuerySQNinfo_All(sz_Model,sz_Partno,sz_SWver,sz_count, sz_pnInfo,sz_errCode,sz_errMsg))
	{
		m_str_errorMsg = ConvertString(sz_errMsg);
		m_str_errorCode = ConvertString(sz_errCode);
		return false;
	}

	return true;
}

bool CQueryCIM::VMS_QueryCRImageList( char * sz_model,char * sz_imageList ,bool b_CR)
{
	pVMS_QueryCRImageList iVMS_QueryCRImageList = NULL;
	iVMS_QueryCRImageList = (pVMS_QueryCRImageList)::GetProcAddress(m_h_VMSDBModule,"VMS_QueryCRImageList");
	if (iVMS_QueryCRImageList == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nVMS_QueryCRImageList == NULL!");
		return false;
	}

	char sz_errCode[20]={0};
	char sz_errMsg[1024]={0};
	if (!iVMS_QueryCRImageList(sz_model,sz_imageList,b_CR,sz_errCode,sz_errMsg))
	{
		m_str_errorMsg = ConvertString(sz_errMsg);
		m_str_errorCode = ConvertString(sz_errCode);
		return false;
	}

	return true;
}

/* Nancy.yt.Zhu 2012.09.13 */
bool CQueryCIM::VMS_QueryNormalImageList( char * sz_model,char * sz_imageList)
{
	pVMS_QueryNormalImageList iVMS_QueryNormalImageList = NULL;
	iVMS_QueryNormalImageList = (pVMS_QueryNormalImageList)::GetProcAddress(m_h_VMSDBModule,"VMS_QueryNormalImageList");
	if (iVMS_QueryNormalImageList == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nVMS_QueryNormalImageList == NULL!");
		return false;
	}

	char sz_errCode[20]={0};
	char sz_errMsg[1024]={0};
	if (!iVMS_QueryNormalImageList(sz_model,sz_imageList,sz_errCode,sz_errMsg))
	{
		m_str_errorMsg = ConvertString(sz_errMsg);
		m_str_errorCode = ConvertString(sz_errCode);
		return false;
	}

	return true;
}

bool CQueryCIM::VMS_QueryImageVersionReserveValue(char * sz_model,char * sz_imageVer)
{
	pVMS_QueryImageVersionReserveValue iVMS_QueryImageVersionReserveValue = NULL;
	iVMS_QueryImageVersionReserveValue = (pVMS_QueryImageVersionReserveValue)::GetProcAddress(m_h_VMSDBModule,"VMS_QueryImageVersionReserveValue");
	if (iVMS_QueryImageVersionReserveValue == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nVMS_QueryImageVersionReserveValue == NULL!");
		return false;
	}

	char sz_errCode[20]={0};
	char sz_errMsg[1024]={0};
	char sz_reserve1_value[50]={0};
	char sz_reserve2_value[50]={0};
	if (!iVMS_QueryImageVersionReserveValue(sz_model,sz_imageVer,sz_reserve1_value,sz_reserve2_value,sz_errCode,sz_errMsg))
	{
		m_str_errorMsg = ConvertString(sz_errMsg);
		m_str_errorCode = ConvertString(sz_errCode);
		return false;
	}

	m_st_idParameter.str_Reserve1 = ConvertString(sz_reserve1_value);
	m_st_idParameter.str_Reserve2 = ConvertString(sz_reserve2_value);

	Log(_T("str_Reserve1: ") + m_st_idParameter.str_Reserve1);
	Log(_T("str_Reserve2: ") + m_st_idParameter.str_Reserve2);

	return true;

}

/* Nancy.Yang 2013.06.17 */
bool CQueryCIM::VMS_QueryVRCMRImageList( char * sz_model,char * sz_PN,char * sz_ToolVerison,char * sz_FactoryImageList)
{
	pVMS_QueryVRCMRImageList iVMS_QueryVRCMRImageList = NULL;
	iVMS_QueryVRCMRImageList = (pVMS_QueryVRCMRImageList)::GetProcAddress(m_h_VMSDBModule,"VMS_QueryVRCMRImageList");
	if (iVMS_QueryVRCMRImageList == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nVMS_QueryVRCMRImageList == NULL!");
		return false;
	}

	char sz_errCode[20]={0};
	char sz_errMsg[1024]={0};
	if (!iVMS_QueryVRCMRImageList(sz_model,sz_PN,sz_ToolVerison,sz_FactoryImageList,sz_errCode,sz_errMsg))
	{
		m_str_errorMsg = ConvertString(sz_errMsg);
		m_str_errorCode = ConvertString(sz_errCode);
		return false;
	}

	return true;
}

bool CQueryCIM::SplitStringToMap(char *sz_count, char *sz_info)
{
	int i_cnt = atoi(sz_count);
	CString str_info = ConvertString(sz_info);
	CString str_head,str_key, str_value;
	int i_pos, i_dotpos;
	bool b_ret = true;

	for (int i=0; i<i_cnt; i++)
	{
		if (str_info == _T(""))
		{
			CString strTmp;
			strTmp.Format(_T("Get Count:%d, but actual Count:%d"),i_cnt,i);
			Log(strTmp);

			break;
		}

		i_pos = str_info.Find(_T(";"));
		if (i_pos != -1)
		{
			str_head = str_info.Left(i_pos);
			str_info = str_info.Mid(i_pos + 1);
		}
		else
		{
			str_head = str_info;
			str_info = _T("");
		}

		i_dotpos = str_head.Find(_T(","));

		if(i_dotpos != -1)
		{
			str_key = str_head.Left(i_dotpos);
			str_value = str_head.Mid(i_dotpos + 1);

			if (str_value != _T(""))
			{
				m_map_TestKeyValue[str_key] = str_value;
			}

		}
		else
		{
			b_ret = false;
			break;
		}	
	}

	if (!b_ret)
	{
		m_str_errorCode = CommErr_UI_PVCS_ParamList_Invalide;
		m_str_errorMsg.Format(_T("Reserve vs control info invalide! Item count(%d),Item info(%s)"),i_cnt, ConvertString(sz_info));
	}

	return b_ret;
}

CString CQueryCIM::ConvertModelName( CString str_modelname )
{
	CString str_cs = str_modelname.Right(3);
	str_cs = str_cs.Left(2);

	if (str_cs.CompareNoCase(_T("CS")) == 0)
	{
		str_cs.Format(_T("%sB%sA"), str_modelname.Left(str_modelname.GetLength() - 3) ,str_modelname.Right(1));
		return str_cs;
	}
	else
	{
		str_cs = str_modelname.Left(1);
		if(str_cs.CompareNoCase(_T("G")) != 0)
			str_cs = _T("G") + str_modelname.Right(6);
		else
			str_cs = str_modelname;
		return str_cs;
	}
}

void CQueryCIM::Log( CString str_content )
{
	m_obj_log.SaveLog(str_content);
}

CString CQueryCIM::StringIntersection(CString strOne,CString strTwo)
{
	CString strtemp = _T("");

	if(strOne == _T("") || strTwo ==_T(""))
		return strtemp;

	int i_pos = 0;

	do 
	{
		i_pos = strOne.Find(_T(","));

		if (i_pos == -1 )
		{
			if(strTwo.Find(strOne) != -1)
			{
				strtemp += strOne;
				strtemp += _T(",");
			}
		}
		else
		{
			if(strTwo.Find(strOne.Left(i_pos)) != -1)
			{
				strtemp += strOne.Left(i_pos);
				strtemp += _T(",");
			}

			strOne = strOne.Mid(i_pos+1);
		}		

	}while (i_pos != -1);


	if (strtemp.Right(1) == _T(","))
	{
		strtemp = strtemp.Left(strtemp.GetLength()-1);
	}

	return strtemp;
}
// Query ID type from sssctd to sure id length in IMEI/FCHECK 
bool CQueryCIM::QueryIDtype(void)
{
	pQueryIDType iQueryIDType = NULL;
	iQueryIDType = (pQueryIDType)::GetProcAddress(m_h_DBModule, "Query_IDType");
	if (iQueryIDType == NULL)
	{
		m_str_errorCode = CommErr_UI_CIM_Interface_Get_Fail;
		m_str_errorMsg = _T("加载CIM数据库DLL函数失败!\nQuery_IDType == NULL!");
		return false;
	}

	char sz_partno[30] = {0};
	char sz_idtype[30] = {0};
	char sz_idlength[30] ={0};

	::WideCharToMultiByte(CP_ACP, 0, m_st_idParameter.str_partNumber, -1, sz_partno, m_st_idParameter.str_partNumber.GetLength() * 2, NULL, NULL);
//	CStringToChar(m_st_idParameter.str_partNumber,sz_partno);

	char sz_ErrCode[15] = {0};
	char sz_ErrMsg[2048] = {0};

	if (!iQueryIDType(sz_partno,sz_idtype,sz_idlength,sz_ErrCode,sz_ErrMsg))
	{
		m_str_errorCode = ConvertString(sz_ErrCode);
		m_str_errorMsg = ConvertString(sz_ErrMsg);
		return false;
	}
	CString strTemp = ConvertString(sz_idtype);

	if(strTemp == _T(""))
	{
		m_str_errorCode = CommErr_UI_ID_Invalid;
		m_str_errorMsg = _T("系统中设置的ID 类型无效, 请于IE联系");
		return false;
	}
	m_st_idParameter.str_idtype = strTemp;
	strTemp = ConvertString(sz_idlength);
	m_st_idParameter.i_idLength = StrToInt(strTemp.GetBuffer());
	strTemp.ReleaseBuffer();


	Log(_T("str_idtype: ") + m_st_idParameter.str_idtype);
	Log(_T("i_idLength: ") + ConvertString(sz_idlength));

	return true;
}
