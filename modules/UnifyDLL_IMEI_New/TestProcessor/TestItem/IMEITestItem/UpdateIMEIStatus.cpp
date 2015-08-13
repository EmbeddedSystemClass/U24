#include "stdafx.h"
#include "UpdateIMEIStatus.h"


RF_IMPLEMENT_DYNCREATE(CUpdateIMEIStatus)

bool CUpdateIMEIStatus::PreRun()
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

bool CUpdateIMEIStatus::Run()
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

bool CUpdateIMEIStatus::PostRun()
{
	return true;
}

bool CUpdateIMEIStatus::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find(std::string("RetryTimes")) == paramMap.end())
	{
		TraceLog(MSG_INFO, _T("Failed to find 'RetryTimes' for class 'CUpdateIMEIStatus' default Retry=3"));
		m_iRetry = 3;
	}
	else
	{
		m_iRetry = atoi(paramMap[std::string("RetryTimes")].c_str());
	}

	if (paramMap.find(std::string("Status")) == paramMap.end())
	{
		TraceLog(MSG_INFO, _T("Failed to find 'Status' for class 'CUpdateIMEIStatus'"));
		return false;
	}
	m_strStatus = paramMap[std::string("Status")];

	return true;
}

bool CUpdateIMEIStatus::MainFunction()
{

	TraceLog(MSG_INFO, "UPDATE STATUS START");

	if(m_strStatus.compare("I") == 0)
	{
		if(!Update_I()) return false;
	}
	else if(m_strStatus.compare("F") == 0)
	{
		if(!Update_F()) return false;
	}
	else
	{
		m_strMessage = MakeStr("Status '%s' unacceptable.", m_strStatus.c_str());
		m_strErrorCode = CommErr_Update_IMEI_Status_To_PICS_Fail;
		FactoryLog();
		return false;
	}

	TraceLog(MSG_INFO, "UPDATE STATUS END");
	


	return true;
}

bool CUpdateIMEIStatus::Update_I()
{
	char szIMEI[32] = {0};
	strcpy(szIMEI, g_strIMEI.c_str());
	char szSWVer[FTD_BUF_SIZE] = {0};
	GetSWVersion(szSWVer);
	char szHWVer[32] = {0};
	strcpy(szHWVer, "101A0");
	char szMal[32] = {0};
	char szErrorCode[FTD_BUF_SIZE] = {0};
	char szErrMsg[FTD_BUF_SIZE] = {0};

	TraceLog(MSG_INFO, MakeStr("Update IMEI Status to 'I'...%s", szIMEI));
	PFUpdateIMEIStatus_I fp_I = (PFUpdateIMEIStatus_I)::GetProcAddress(m_hSSSO2Handle, "UpdateIMEIStatus_I");
	if(fp_I == NULL)
	{
		m_strMessage = "Load UpdateIMEIStatus_I fuction fail.";
		m_strErrorCode = CommErr_Update_IMEI_Status_To_PICS_Fail;
		FactoryLog();
		return false;
	}

	if(! fp_I(szIMEI, szSWVer, szErrorCode, szErrMsg))
	{
		m_strErrorCode = CommErr_Update_IMEI_Status_To_PICS_Fail;
		m_strMessage = MakeStr("Error code------->%s", szErrorCode);
		FactoryLog();
		m_strMessage = MakeStr("Error message------->%s", szErrMsg);
		FactoryLog();
		m_strMessage = "Update IMEI status 'I' fail......";
		FactoryLog();
		return false;
	}
	m_strMessage = "Update IMEI status 'I' Success...";
	m_strResult = "PASS";
	FactoryLog();
	return true;
}

bool CUpdateIMEIStatus::Update_F()
{
	char szIMEI[32] = {0};
	strcpy(szIMEI, g_strIMEI.c_str());
	char szSWVer[FTD_BUF_SIZE] = {0};
	GetSWVersion(szSWVer);
	char szHWVer[32] = {0};
	strcpy(szHWVer, "101A0");
	char szMal[32] = {0};
	char szErrorCode[FTD_BUF_SIZE] = {0};
	char szErrMsg[FTD_BUF_SIZE] = {0};

	TraceLog(MSG_INFO, MakeStr("Update IMEI Status to 'F'...%s", szIMEI));
	PFUpdateIMEIStatus_F fp_F = (PFUpdateIMEIStatus_F)::GetProcAddress(m_hSSSO2Handle, "UpdateIMEIStatus_F");
	if(fp_F == NULL)
	{
		m_strMessage = "Load UpdateIMEIStatus_F fuction fail.";
		m_strErrorCode = CommErr_Update_IMEI_Status_To_PICS_Fail;
		FactoryLog();
		return false;
	}
	if(! fp_F(szIMEI, szSWVer, szHWVer, szMal, szErrorCode, szErrMsg))
	{
		m_strErrorCode = CommErr_Update_IMEI_Status_To_PICS_Fail;
		m_strMessage = MakeStr("Error code------->%s", szErrorCode);
		FactoryLog();
		m_strMessage = MakeStr("Error message------->%s", szErrMsg);
		FactoryLog();
		m_strMessage = "Update IMEI status 'F' fail......";
		FactoryLog();
		return false;
	}
	m_strMessage = "Update IMEI status 'F' Success...";
	m_strResult = "PASS";
	FactoryLog();
	return true;
}