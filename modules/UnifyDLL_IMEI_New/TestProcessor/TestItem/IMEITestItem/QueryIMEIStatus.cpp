#include "stdafx.h"
#include "QueryIMEIStatus.h"


RF_IMPLEMENT_DYNCREATE(CQueryIMEIStatus)

bool CQueryIMEIStatus::PreRun()
{
	m_strResult = "";
	m_strItemCode = "";
	m_strErrorCode = "";
	m_strBand = "";
	m_strChannel = "";
	m_strUpper = "";
	m_strLower = "";
	m_strMeasured = "";
	m_strUnit = "";
	m_strMessage = "";
	return true;
}

bool CQueryIMEIStatus::Run()
{
	m_hSSSO2Handle = ::LoadLibrary("Pics_SSSO2_Factory.dll");
	
	bool bRes = MainFunction();
	
	if (m_hSSSO2Handle != NULL)
	{
		FreeLibrary(m_hSSSO2Handle);
		m_hSSSO2Handle = NULL;
	}
	return bRes;
}

bool CQueryIMEIStatus::PostRun()
{
	return true;
}

bool CQueryIMEIStatus::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find(std::string("RetryTimes")) != paramMap.end())
	{
		m_iRetry = atoi(paramMap[std::string("RetryTimes")].c_str());
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to find 'RetryTimes' for class 'CQueryIMEIStatus' default Retry=3"));
		m_iRetry = 3;
	}
	
	return true;
}

bool CQueryIMEIStatus::MainFunction()
{
	TraceLog(MSG_INFO, "QUERY IMEI STATUS START");
	
	char szIMEI[32] = {0};
	strcpy(szIMEI, g_strIMEI.c_str());
	m_strMessage = MakeStr("Query IMEI status from server by IMEI/Picasso---> %s", szIMEI);
	FactoryLog();

	// 1. Query BT MAC by IMEI from SSS02 //
	PFQueryIMEI_SSSO2 fpQuery = (PFQueryIMEI_SSSO2)::GetProcAddress(m_hSSSO2Handle, "QueryIMEI_SSSO2");
	if(fpQuery == NULL)
	{
		m_strMessage = "Load QueryIMEI_SSSO2 fuction fail.";
		m_strErrorCode = CommErr_Query_IMEI_From_SSSO2_Fail;
		FactoryLog();
		return false;
	}

	char szStationtState[FTD_BUF_SIZE] = {0};
	char szMSN[FTD_BUF_SIZE] = {0};
	char szBTAddr[FTD_BUF_SIZE] = {0};
	char szMACAddr[FTD_BUF_SIZE] = {0};
	char szSWver[FTD_BUF_SIZE] = {0};
	char szHWver[FTD_BUF_SIZE] = {0};
	char szErrCode[FTD_BUF_SIZE] = {0};
	char szErrMsg[FTD_BUF_SIZE] = {0};
	char szStatusOutput[FTD_BUF_SIZE] = {0};

	if( !fpQuery(szStationtState, szIMEI, szMSN, szBTAddr, szMACAddr, szSWver, szHWver, szErrCode, szErrMsg, szStatusOutput))
	{
		m_strMessage = MakeStr("Error code: %s, Error message: %s, StatusOutput: %s", szErrCode, szErrMsg, szStatusOutput);
		FactoryLog();
		m_strErrorCode = CommErr_IMEI_Status_Error;
		m_strMessage = "Execute Check_MAL_PN_SSSO2 fuction fail.";
		FactoryLog();
		return false;
	}

	m_strMessage = MakeStr("IMEI = %s, IMEI STATE = %s", szIMEI, szStatusOutput);
	FactoryLog();

	if((strcmp(szStatusOutput, "I") != 0) && (strcmp(szStatusOutput, "F") != 0))
	{
		m_strErrorCode = CommErr_IMEI_Status_Error;
		m_strMessage = "IMEI Status Fail. Query IMEI Status != 'I' or 'F'";
		FactoryLog();
		return false;
	}

	m_strMessage = "Query IMEI Status success";
	m_strResult = "PASS";
	TraceLog(MSG_INFO, m_strMessage);
	FactoryLog();
	return true;
}
