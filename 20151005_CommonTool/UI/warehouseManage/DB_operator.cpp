#include "StdAfx.h"
#include "DB_Operator.h"

DB_operator::DB_operator(void)
{
	HINSTANCE hDLL = ::LoadLibrary( _T("DBAccess.dll") );
	if (!hDLL)
	{
	    m_b_loadDB_Res = false;
	}
	else
	{
		m_hDLL = hDLL;
		m_b_loadDB_Res = true;
	}

}

DB_operator::~DB_operator(void)
{
	::FreeLibrary(m_hDLL);
}


/*****************************************************************************
* function name:  GetFunadress()     
* summary      :  取的函数的地址
* Parameters   :  void           
* Return       :  True 成功取得地址.False取地址失败                                           
* Version      :  Author        Date                Abstract                 
*   1.0           Tom Li     2011/10/09           First version            
*****************************************************************************/
bool DB_operator::GetFunadress()
{
	bool b_res = false;
	if(m_b_loadDB_Res)
	{
		pConnectDB = (ConnectDB)::GetProcAddress((HMODULE)m_hDLL, "ConnectDB") ;
		if(!pConnectDB) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pCloseDB = (CloseDB)::GetProcAddress( (HMODULE)m_hDLL, "CloseDB") ;
		if(!pCloseDB) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}
		pQueryOOBstatus = (QueryOOBstatus)::GetProcAddress( (HMODULE)m_hDLL, "QueryOOBstatus");
		if(!pQueryOOBstatus) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pUpdateOOBStatusToFalse = (UpdateOOBStatusToFalse)::GetProcAddress( (HMODULE)m_hDLL, "UpdateOOBStatusToFalse");
		if(!pUpdateOOBStatusToFalse) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pQueryOOBResult = (QueryOOBResult)::GetProcAddress( (HMODULE)m_hDLL, "QueryOOBResult");
		if(!pQueryOOBResult) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pInsertPTOOBRework = (InsertPTOOBRework)::GetProcAddress( (HMODULE)m_hDLL, "InsertPTOOBRework");
		if(!pInsertPTOOBRework) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pCheckRouting = (CheckRouting)::GetProcAddress( (HMODULE)m_hDLL, "CheckRouting");
		if(!pCheckRouting) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pGetIMEI_Info = (GetIMEI_Info)::GetProcAddress( (HMODULE)m_hDLL, "GetIMEI_Info");
		if(!pGetIMEI_Info) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pQuerySO_Info_All = (QuerySO_Info_All)::GetProcAddress((HMODULE)m_hDLL, "QuerySO_Info_All");
		if(!pQuerySO_Info_All) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pQuerySO_Info_All_V2 = (QuerySO_Info_All_V2)::GetProcAddress((HMODULE)m_hDLL, "QuerySO_Info_All_V2");
		if(!pQuerySO_Info_All_V2) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pQueryIMEI_ByCarton = (QueryIMEI_ByCarton)::GetProcAddress((HMODULE)m_hDLL, "QueryIMEI_ByCarton");
		if(!pQueryIMEI_ByCarton) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pQuerySegmentProcess = (QuerySegmentProcess)::GetProcAddress((HMODULE)m_hDLL, "QuerySegmentProcess");
		if(!pQuerySegmentProcess) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pGetProcessResult = (GetProcessResult)::GetProcAddress((HMODULE)m_hDLL, "GetProcessResult");
		if(!pGetProcessResult) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pQuerySO_Info_All = (QuerySO_Info_All)::GetProcAddress((HMODULE)m_hDLL, "QuerySO_Info_All");
		if(!pQuerySO_Info_All) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pQueryIMEI_ByCarton = (QueryIMEI_ByCarton)::GetProcAddress((HMODULE)m_hDLL, "QueryIMEI_ByCarton");
		if(!pQueryIMEI_ByCarton) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pQueryIMEIStatus = (QueryIMEIStatus)::GetProcAddress((HMODULE)m_hDLL, "QueryIMEIStatus");
		if(!pQueryIMEIStatus) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pInsert_WareHouseIn = (Insert_WareHouseIn)::GetProcAddress((HMODULE)m_hDLL, "Insert_WareHouseIn");
		if(!pInsert_WareHouseIn) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pUpdate_WareHouseOut = (Update_WareHouseOut)::GetProcAddress((HMODULE)m_hDLL, "Update_WareHouseOut");
		if(!pUpdate_WareHouseOut) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pQuery_WareHouseStatus = (Query_WareHouseStatus)::GetProcAddress((HMODULE)m_hDLL, "Query_WareHouseStatus");
		if(!pQuery_WareHouseStatus) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pQuery_WareHouseStatus_V2 = (Query_WareHouseStatus_V2)::GetProcAddress((HMODULE)m_hDLL, "Query_WareHouseStatus_V2");
		if(!pQuery_WareHouseStatus_V2) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}
		pInsertWareHouseOutHoldInfo = (InsertWareHouseOutHoldInfo)::GetProcAddress((HMODULE)m_hDLL, "InsertWareHouseOutHoldInfo");
		if(!pInsertWareHouseOutHoldInfo) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}

		pQuery_IDType = (Query_IDType)::GetProcAddress((HMODULE)m_hDLL, "Query_IDType");
		if(!pQuery_IDType) 
		{
			b_res = false;
			return b_res;
		}
		else
		{
			b_res = true;
		}
		return b_res;
	}
}

/******************************************************************************
* function name:  FconnectDB()                                       
* Parameters   :  void               
* Summary      :  连接数据库
* Return       :  True 数据库连接成功.False数据库连接失败  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Tom Li          2011/10/09           First version                            
*****************************************************************************/
bool DB_operator::FconnectDB()
{
	if(!GetFunadress())
	{
		return false;
	}

	if(pConnectDB())
	{
		return true;
	}
	else
	{
		return false;
	}
}

/******************************************************************************
* function name:  FcloseDB()                                       
* Parameters   :  void               
* Summary      :  关闭数据库
* Return       :  True 数据库关闭成功.False数据库关闭失败  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Tom Li          2011/10/09           First version                            
*****************************************************************************/
bool DB_operator::FcloseDB()
{
   pCloseDB();
   return true;
}


