#include "stdafx.h"
#include "CheckTouch.h"


RF_IMPLEMENT_DYNCREATE(CCheckTouch)

bool CCheckTouch::PreRun()
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

bool CCheckTouch::Run()
{
	bool bRes = MainFunction();
	return bRes;
}

bool CCheckTouch::PostRun()
{
	return true;
}

bool CCheckTouch::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find(std::string("RetryTimes")) != paramMap.end())
	{
		m_iRetry = atoi(paramMap[std::string("RetryTimes")].c_str());
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to find 'RetryTimes' for class 'CCheckTouch' default Retry=3"));
		m_iRetry = 3;
	}
	
	return true;
}

bool CCheckTouch::MainFunction()
{
	TraceLog(MSG_INFO, "Touch Check START");
	
	char sz_input[FTD_BUF_SIZE] = {0};
	char sz_outData[FTD_BUF_SIZE] = {0};

	if (!m_pIPhone->FTD_TouchCheck(m_nFtdPort, m_nFtdTimeOut, sz_input, sz_outData))
	{
		m_strMessage = "TouchCheck fail.....";
		m_strErrorCode = FunErr_Check_Touch_FW_Fail;
		FactoryLog();
		return false;
	}

	m_strMessage = "TouchCheck Pass.....";
	m_strResult = "PASS";
	TraceLog(MSG_INFO, m_strMessage);
	FactoryLog();
	return true;
}
