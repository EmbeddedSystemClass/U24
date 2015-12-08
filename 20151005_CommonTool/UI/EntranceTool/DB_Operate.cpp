#include "StdAfx.h"
#include "DB_Operate.h"

DB_Operate::DB_Operate(void)
{
	m_b_loadDB_Res = false;
	m_hDLL = NULL;
	m_hDLL_VMS = NULL;

    pVMS_ConnectDB = NULL;
	pVMS_CloseDB = NULL;
	pVMS_QuerySQNinfo_V2 = NULL;
	pVMS_QueryFactoryImageStatus = NULL;
	pVMS_QueryToolStatus = NULL;

	pCloseDB = NULL;
	pConnectDB = NULL;
	pQuerySO_Info = NULL;
	pQuerySO_Info_All = NULL;
	pQueryStation_Info = NULL;
	pInsertFAIL_data = NULL;
	pInsertPASS_data = NULL;
	pSetValue = NULL;
	pInsertOOB_data = NULL;
	pCheckRouting = NULL;
	pQueryEmployeeID_Right = NULL;
	pQuerySegmentProcess = NULL;

	pQueryConsumeType = NULL;
	pQueryConsumeInfo = NULL;
}

DB_Operate::~DB_Operate(void)
{
	if(m_hDLL)
	{
	  FreeLibrary (m_hDLL);
	}
	if(m_hDLL_VMS)
	{
	  FreeLibrary (m_hDLL_VMS);
	}

}

/******************************************************************************
* function name:  GetFunadress()                                                     
* Summary      :  动态加载dll文件"DBAccess.dll" 
* Parameters   :  void  
* Return       :  True 成功加载dll文件.False加载dll文件失败  
* Version     : Author            Date                 Abstract                      
* 1.0         : Tom Li          2011/06/27           First version                            
*****************************************************************************/

bool DB_Operate::LoadDB()
{
	HINSTANCE hDLL = ::LoadLibrary( _T("DBAccess.dll") );
	HINSTANCE hDLL_VMS = ::LoadLibrary( _T("VMS_DBAccess.dll") );
	if (hDLL == NULL || hDLL_VMS == NULL)
	{
	    m_b_loadDB_Res = false;
		if(hDLL != NULL)
			FreeLibrary(hDLL);
		if(hDLL_VMS != NULL)
			FreeLibrary(hDLL_VMS);

	}
	else
	{
		m_hDLL = hDLL;
		m_hDLL_VMS = hDLL_VMS;
		m_b_loadDB_Res = true;
	}
	return m_b_loadDB_Res;
}

/*****************************************************************************
* function name:  GetFunadress()     
* summary      :  取的函数的地址
* Parameters   :  void           
* Return       :  True 成功取得地址.False取地址失败                                           
* Version      :  Author        Date                Abstract                 
*   1.0           Tom Li     2011/06/27           First version            
*****************************************************************************/

bool DB_Operate::GetFunadress()
{
	bool b_res = false;
	if(LoadDB())
	{
		pQuerySO_Info_All = (QuerySO_Info_All)::GetProcAddress( (HMODULE)m_hDLL, "QuerySO_Info_All");
		if (!pQuerySO_Info_All) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pQuerySO_Info_All_V3 = (QuerySO_Info_All_V3)::GetProcAddress((HMODULE)m_hDLL, "QuerySO_Info_All_V3");
		if (!pQuerySO_Info_All_V3) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}


		pQuerySO_Info_All_V4 = (QuerySO_Info_All_V4)::GetProcAddress((HMODULE)m_hDLL, "QuerySO_Info_All_V4");
		if (!pQuerySO_Info_All_V4) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pQuerySO_Info_All_V5 = (QuerySO_Info_All_V5)::GetProcAddress((HMODULE)m_hDLL, "QuerySO_Info_All_V5");
		if (!pQuerySO_Info_All_V5) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pQuerySO_Info = (QuerySO_Info)::GetProcAddress( (HMODULE)m_hDLL, "QuerySO_Info");
		if (!pQuerySO_Info) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pQueryStation_Info = (QueryStation_Info)::GetProcAddress((HMODULE)m_hDLL, _T("QueryStation_Info"));
		if (!pQueryStation_Info) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pInsertFAIL_data = (InsertFAIL_data)::GetProcAddress((HMODULE)m_hDLL, "InsertFAIL_data");
		if (!pInsertFAIL_data) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}


		pInsertPASS_data = (InsertPASS_data)::GetProcAddress((HMODULE)m_hDLL, _T("InsertPASS_data"));
		if (!pInsertPASS_data) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pSetValue = (SetValue)::GetProcAddress((HMODULE)m_hDLL, _T("SetValue"));
		if (!pSetValue) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pInsertOOB_data = (InsertOOB_data)::GetProcAddress((HMODULE)m_hDLL, _T("InsertOOB_data"));
		if (!pInsertOOB_data) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pConnectDB = (ConnectDB)::GetProcAddress((HMODULE)m_hDLL, _T("ConnectDB"));
		if (!pConnectDB) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pCloseDB = (CloseDB)::GetProcAddress((HMODULE)m_hDLL, _T("CloseDB"));
		if (!pCloseDB) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pCheckRouting = (CheckRouting)::GetProcAddress((HMODULE)m_hDLL, _T("CheckRouting"));
		if (!pCheckRouting) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pQueryEmployeeID_Right = (QueryEmployeeID_Right)::GetProcAddress((HMODULE)m_hDLL, _T("QueryEmployeeID_Right"));
		if (!pQueryEmployeeID_Right) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}


		pQueryEmployeeID_Right_V2= (QueryEmployeeID_Right_V2)::GetProcAddress((HMODULE)m_hDLL, _T("QueryEmployeeID_Right_V2"));
		if (!pQueryEmployeeID_Right_V2) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}


		pUpdateEmployeeID_Status = (UpdateEmployeeID_Status)::GetProcAddress((HMODULE)m_hDLL, "UpdateEmployeeID_Status");
		if (!pUpdateEmployeeID_Status) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pQuerySegmentProcess = (QuerySegmentProcess)::GetProcAddress((HMODULE)m_hDLL, _T("QuerySegmentProcess"));
		if (!pQueryEmployeeID_Right) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pVMS_ConnectDB = (VMS_ConnectDB)::GetProcAddress((HMODULE)m_hDLL_VMS, _T("VMS_ConnectDB"));
		if (!pVMS_ConnectDB) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pVMS_CloseDB = (VMS_CloseDB)::GetProcAddress((HMODULE)m_hDLL_VMS, _T("VMS_CloseDB"));
		if (!pVMS_CloseDB) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pVMS_QuerySQNinfo_V2 = (VMS_QuerySQNinfo_V2)::GetProcAddress((HMODULE)m_hDLL_VMS, _T("VMS_QuerySQNinfo_V2"));
		if (!pVMS_QuerySQNinfo_V2) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}
		//VMS_QuerySQNinfo_All
		pVMS_QuerySQNinfo_All = (VMS_QuerySQNinfo_All)::GetProcAddress((HMODULE)m_hDLL_VMS, _T("VMS_QuerySQNinfo_All"));
		if (!pVMS_QuerySQNinfo_All) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}
		//VMS_QueryModelProjectList
		pVMS_QueryModelProjectList = (VMS_QueryModelProjectList)::GetProcAddress((HMODULE)m_hDLL_VMS, _T("VMS_QueryModelProjectList"));
		if (!pVMS_QueryModelProjectList) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pVMS_QueryFactoryImageStatus = (VMS_QueryFactoryImageStatus)::GetProcAddress((HMODULE)m_hDLL_VMS, _T("VMS_QueryFactoryImageStatus"));
		if (!pVMS_QueryFactoryImageStatus) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pVMS_QueryCommonToolVersion = (VMS_QueryCommonToolVersion)::GetProcAddress((HMODULE)m_hDLL_VMS, _T("VMS_QueryCommonToolVersion"));
		if (!pVMS_QueryCommonToolVersion) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pVMS_QueryToolStatus = (VMS_QueryToolStatus)::GetProcAddress((HMODULE)m_hDLL_VMS, _T("VMS_QueryToolStatus"));
		if (!pVMS_QueryToolStatus) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pVMS_QueryImagePNToolVersionStatus = (VMS_QueryImagePNToolVersionStatus)::GetProcAddress((HMODULE)m_hDLL_VMS, _T("VMS_QueryImagePNToolVersionStatus"));
		if (!pVMS_QueryImagePNToolVersionStatus) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pQueryConsumeType = (QueryConsumeType)::GetProcAddress((HMODULE)m_hDLL, _T("QueryConsumeType"));
		if (!pQueryConsumeType) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}
		pQueryConsumeInfo = (QueryConsumeInfo)::GetProcAddress((HMODULE)m_hDLL, _T("QueryConsumeInfo"));
		if (!pQueryConsumeInfo) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}
	}

	return b_res;
}