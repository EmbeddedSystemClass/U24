#include "stdafx.h"
#include "CheckDeviceNotInOSMode.h"


RF_IMPLEMENT_DYNCREATE(CCheckDeviceNotInOSMode)

bool CCheckDeviceNotInOSMode::PreRun()
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

bool CCheckDeviceNotInOSMode::Run()
{
	bool bRes = MainFunction();
	return bRes;
}

bool CCheckDeviceNotInOSMode::PostRun()
{
	return true;
}

bool CCheckDeviceNotInOSMode::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find(std::string("RetryTimes")) != paramMap.end())
	{
		m_iRetry = atoi(paramMap[std::string("RetryTimes")].c_str());
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to find 'RetryTimes' for class 'CCheckDeviceNotInOSMode' default Retry=3"));
		m_iRetry = 3;
	}

	return true;
}

bool CCheckDeviceNotInOSMode::MainFunction()
{
	char sz_input[FTD_BUF_SIZE] = {0};
	char sz_outData[FTD_BUF_SIZE] = {0};
	strcpy(sz_input, "init.svc.zygote");
	m_strMessage = MakeStr("Check cmd: %s", sz_input);
	FactoryLog();

	for(int i = 1; i <= m_iRetry; i++)
	{
		int RetValue;
		RetValue = m_pIPhone->FTD_OSgetprop(m_nFtdPort, m_nFtdTimeOut, sz_input, sz_outData);
		if(RetValue > 0 && strcmp(sz_outData, "0") == 0) break;
		else if(RetValue == -10900)
		{
			m_strMessage = "Return -10900. Not in OS Mode...";
			FactoryLog();
			break;
		}
		else
		{
			if(i == m_iRetry)
			{
				m_strMessage = MakeStr("Check OS Mode fail(Return = %d, output = %s)", RetValue, sz_outData);				
				m_strResult = "FAIL";
				m_strErrorCode = CommErr_Device_In_OS_Mode;
				TraceLog(MSG_ERROR, m_strMessage);
				FactoryLog();
				return false;
			}
			else
			{
				m_strMessage =  MakeStr("Check OS Mode fail(Return = %d, output = %s), Retry again", RetValue, sz_outData);
				TraceLog(MSG_ERROR, m_strMessage);
			}
		}
	}

	m_strResult = "PASS";
	m_strMessage = "Check Not OS Mode success...";
	FactoryLog();
	return true;
}
