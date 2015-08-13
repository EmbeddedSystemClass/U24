#include "stdafx.h"
#include "CheckCreateIMEI.h"


RF_IMPLEMENT_DYNCREATE(CCheckCreateIMEI)

bool CCheckCreateIMEI::PreRun()
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

bool CCheckCreateIMEI::Run()
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

bool CCheckCreateIMEI::PostRun()
{
	return true;
}

bool CCheckCreateIMEI::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find(std::string("RetryTimes")) != paramMap.end())
	{
		m_iRetry = atoi(paramMap[std::string("RetryTimes")].c_str());
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to find 'RetryTimes' for class 'CCheckCreateIMEI' default Retry=3"));
		m_iRetry = 3;
	}

	return true;
}

bool CCheckCreateIMEI::MainFunction()
{
	// 1. Check IMEI MAL&PN IN SSS02 //
	TraceLog(MSG_INFO, "CHECK IMEI MAL&PN IN SSS02 START");
	PFCheck_MAL_PN_SSSO2 fp = (PFCheck_MAL_PN_SSSO2)::GetProcAddress(m_hSSSO2Handle, "Check_MAL_PN_SSSO2");
	
	if(fp == NULL)
	{
		m_strMessage = "Load Check_MAL_PN_SSSO2 fuction fail.";
		m_strErrorCode = CommErr_Check_IMEI_PN_MAL_In_PICS_Fail;
		FactoryLog();
		return false;
	}

	char szIMEI[32] = {0};
	char szPartNumber[32] = {0};
	char szInMal[FTD_BUF_SIZE] = {0};
	char szErrorCode[FTD_BUF_SIZE] = {0};
	char szErrMsg[FTD_BUF_SIZE] = {0};

	strcpy(szIMEI, g_strIMEI.c_str());
	strcpy(szPartNumber, g_strPartNumber.c_str());
	m_strMessage = MakeStr("Part Number = %s", g_strPartNumber.c_str());
	FactoryLog();

	if(! fp(szIMEI, szPartNumber, szInMal, szErrorCode, szErrMsg))
	{
		m_strErrorCode = CommErr_Check_IMEI_PN_MAL_In_PICS_Fail;
		m_strMessage = "Execute Check_MAL_PN_SSSO2 fuction fail.";
		FactoryLog();
		m_strMessage = szErrMsg;
		FactoryLog();
		return false;
	}

	m_strMessage = MakeStr("%s", szErrMsg);
	FactoryLog();	

	TraceLog(MSG_INFO, "CHECK IMEI MAL&PN IN SSS02 END");


	// 2. Create IMEI IN PICS //
	TraceLog(MSG_INFO, "CREATE IMEI IN PICS START");
	PFCreate_IMEI_PICS fp2 = (PFCreate_IMEI_PICS)::GetProcAddress(m_hSSSO2Handle, "Create_IMEI_PICS");
	
	if(fp2 == NULL)
	{
		m_strMessage = "Load Create_IMEI_PICS fuction fail.";
		m_strErrorCode = CommErr_Create_IMEI_Table_In_PICS_Fail;
		FactoryLog();
		return false;
	}

	if(! fp2(szIMEI, szErrMsg))
	{
		if(strcmp(szErrMsg, "-7") == 0)
		{
			m_strMessage = MakeStr("The record for IMEI is already existe in PICS...... %s", szIMEI);
			FactoryLog();
		}
		else
		{
			m_strErrorCode = CommErr_Create_IMEI_Table_In_PICS_Fail;
			m_strMessage = "Create IMEI as a record in PICS fail.";
			FactoryLog();
			m_strMessage = MakeStr("Error code return--> %s", szErrMsg);
			FactoryLog();
			return false;
		}
	}
	else
	{
		m_strMessage = MakeStr("Create IMEI as a record in PICS success---> %s", szIMEI);
		FactoryLog();
	}

	TraceLog(MSG_INFO, "CREATE IMEI IN PICS END");

	m_strMessage = "Create IMEI in PICS success";
	m_strResult = "PASS";
	FactoryLog();
	return true;
}
