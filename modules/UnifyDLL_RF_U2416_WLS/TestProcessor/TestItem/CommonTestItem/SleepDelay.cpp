#include "stdafx.h"
#include "SleepDelay.h"


RF_IMPLEMENT_DYNCREATE(CSleepDelay)


bool CSleepDelay::Run()
{
	Sleep(m_iWaitTime);

	FactoryLog(true, "", "", "", "", "", "", "", "", "Sleep delay PASS");
	SetPICSData(m_strPICSName, "PASS");
	return true;
}


bool CSleepDelay::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find("WaitTime") == paramMap.end())
	{
		TraceLog( MSG_ERROR ,"Fail to find parameter WaitTime for CSleepDelay");
		return false;
	}
	m_iWaitTime = atoi(paramMap["WaitTime"].c_str());

	if (paramMap.find("PICSName") != paramMap.end())
	{
		m_strPICSName = paramMap["PICSName"];
	}
	else
	{
		m_strPICSName.clear();
	}

	return true;
}
