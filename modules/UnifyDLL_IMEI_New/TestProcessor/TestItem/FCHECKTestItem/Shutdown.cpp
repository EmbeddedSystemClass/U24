#include "stdafx.h"
#include "Shutdown.h"


RF_IMPLEMENT_DYNCREATE(CShutdown)

bool CShutdown::PreRun()
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

bool CShutdown::Run()
{
	bool bRes = MainFunction();
	return bRes;
}

bool CShutdown::PostRun()
{
	return true;
}

bool CShutdown::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find(std::string("FTDRetry")) == paramMap.end())
	{
		TraceLog(MSG_INFO, _T("Failed to find 'FTDRetry' for class 'CShutdown' default Retry=150"));
		m_iFTDRetry = 150;
	}
	else
	{
		m_iFTDRetry = atoi(paramMap[std::string("FTDRetry")].c_str());
	}

	if (paramMap.find(std::string("ShutDownRetry")) == paramMap.end())
	{
		TraceLog(MSG_INFO, _T("Failed to find 'ShutDownRetry' for class 'CShutdown' default Retry=5"));
		m_iShutdownRetry = 5;
	}
	else
	{
		m_iShutdownRetry = atoi(paramMap[std::string("ShutDownRetry")].c_str());
	}

	return true;
}

bool CShutdown::MainFunction()
{
	m_strMessage = "Check FTD mode start...";
	FactoryLog();

	for(int i = 1; i <= m_iFTDRetry; i++)
	{
		if (m_pIPhone->IsMobileInFTDMode(m_nFtdPort, m_nFtdTimeOut))	break;
		if( i == m_iFTDRetry)
		{
			m_strMessage = "Check FTD mode fail...";
			m_strResult = "FAIL";
			m_strErrorCode = CommErr_Phone_Not_FTD;
			FactoryLog();
			return false;
		}
		Sleep(1000);
		m_strMessage = "Waiting FTD mode...";
		FactoryLog();
	}
	m_strMessage = "Check FTD mode PASS.";
	FactoryLog();


	m_strMessage = "No Key ShutDown Start";
	FactoryLog();

	char sz_input[FTD_BUF_SIZE] = {0};
	char sz_outData[FTD_BUF_SIZE] = {0};

	strcpy(sz_input, "53687554444F576E");

	for(int i = 1; i <= m_iShutdownRetry; i++)
	{
		if (m_pIPhone->FTD_ShutDown(m_nFtdPort, m_nFtdTimeOut, sz_input, sz_outData))	break;
		if( i == m_iShutdownRetry)
		{
			m_strMessage = "Shut Down command fail...";
			m_strResult = "FAIL";
			m_strErrorCode = "ShutDown Fail";
			FactoryLog();
			return false;
		}
		Sleep(1000);
		m_strMessage = "Waiting FTD_ShutDown...";
		FactoryLog();
	}
	m_strMessage = "ShutDown success...";
	FactoryLog();

	m_strResult = "PASS";
	TraceLog(MSG_INFO, m_strMessage);
	FactoryLog();

	return true;
}
