#include "stdafx.h"
#include "CheckMobileIMEI.h"

#define Dummy_IMEI                  "000000011234560"//"123456789012345","449281960000000"
#define Zero_IMEI                   "000000000000000"

RF_IMPLEMENT_DYNCREATE(CCheckMobileIMEI)

bool CCheckMobileIMEI::PreRun()
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

bool CCheckMobileIMEI::Run()
{
	bool bRes = MainFunction();

	return bRes;
}

bool CCheckMobileIMEI::PostRun()
{
	return true;
}

bool CCheckMobileIMEI::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find(std::string("RetryTimes")) != paramMap.end())
	{
		m_iRetry = atoi(paramMap[std::string("RetryTimes")].c_str());
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to find 'RetryTimes' for class 'CCheckMobileIMEI' default Retry=3"));
		m_iRetry = 3;
	}
/*
	if (paramMap.find(std::string("IMEILength")) != paramMap.end())
	{
		m_iIMEILength = atoi(paramMap[std::string("IMEILength")].c_str());
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to find 'IMEILength' for class 'CCheckMobileIMEI' default IMEILength=15"));
		m_iIMEILength = 15;
	}
*/
	return true;
}

bool CCheckMobileIMEI::MainFunction()
{
	// 1. Read IMEI in phone //
	char sz_input[FTD_BUF_SIZE] = {0};
	char sz_outData[FTD_BUF_SIZE] = {0};

	if (!m_pIPhone->FTD_FAC_OTPRead(m_nFtdPort, m_nFtdTimeOut, "IMEI", sz_outData))
	{
		m_strMessage = "Read_IMEI_Fail fail";
		m_strResult = "FAIL";
		m_strErrorCode = CommErr_Read_IMEI_Fail;
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}

	m_strMessage = MakeStr("IMEI Read from target --> %s", sz_outData);
	FactoryLog();
	m_mapQueryData["IMEI_IN_MOBILE"] = sz_outData;

	// 2. Check Mobile IMEI & Scan IMEI //
	char szIMEI[32] = {0};
	strcpy(szIMEI, g_strIMEI.c_str());

	char szZero[50] = {0};
	for(int i = 0; i <= m_i_IMEILength; i++)
	{
		szZero[i] ='0';
		if(i == m_i_IMEILength)	szZero[i] ='\0';
	}
	

	if(strlen(sz_outData) == 0)
	{
		strcpy(sz_outData, szZero);
		m_strMessage = "The IMEI from phone is empty(first write) ...";
		FactoryLog();
	}
	if(strcmp(sz_outData, Dummy_IMEI) == 0 || strcmp(sz_outData, szZero) == 0)
	{
		m_strMessage = MakeStr("The IMEI from phone is %s...", sz_outData);
		FactoryLog();
	}
	else
	{
		if(strcmp(sz_outData, szIMEI) != 0)
		{
			m_strErrorCode = CommErr_Check_IMEI_Fail_In_Rework;
			m_strMessage = MakeStr("The IMEI from phone(%s) is not equal to the IMEI you scanned(%s)...", sz_outData, szIMEI);
			FactoryLog();
			return false;
		}
		m_strMessage = MakeStr("The IMEI from phone(%s) is equal to the IMEI you scanned(%s)...", sz_outData, szIMEI);
		FactoryLog();
	}

	m_mapIMEIParams["LAST_WRITE_IMEI"] = szIMEI;

	m_strMessage = "Check Mobile IMEI success";
	m_strResult = "PASS";
	TraceLog(MSG_INFO, m_strMessage);
	FactoryLog();
	return true;
}
