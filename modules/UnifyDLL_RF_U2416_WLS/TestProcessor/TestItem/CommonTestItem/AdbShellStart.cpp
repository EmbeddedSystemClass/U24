#include "stdafx.h"
#include "AdbShellStart.h"


RF_IMPLEMENT_DYNCREATE(CAdbShellStart)


bool CAdbShellStart::Run()
{
	bool bRes = MainFunction();

	if (bRes)
	{
		m_strMsg = "Adb Shall Start PASS";
		FactoryLog(true, "AdbShallStart", "--", "--", "--", "--", "--", "Pass", "--", m_strMsg.c_str());
		SetPICSData("AdbShallStart", "PASS");
	}
	else
	{
		m_strMsg = "Adb Shall Start Fail";
		FactoryLog(false, "--", CommErr_Get_ADB_Fail, "--", "--", "--", "--", "Fail", "--", m_strMsg.c_str());
		SetPICSData("AdbShallStart", "FAIL");
	}
	
	return bRes;
}

bool CAdbShellStart::MainFunction()
{


 	if (m_pIPhone)
 	{
 		if (!m_pIPhone->Initial())
 		{
 			m_strMsg = "Fail to Initial IPhone,Loading Qcomdiag fail!";
 			TraceLog(MSG_ERROR, m_strMsg);
 			return false;
 		}

 		if (!m_pIPhone->IsConnected_FTD())
 		{
 			m_strMsg = "Fail to connect phone with Qcomdiag dll" ;
 			TraceLog(MSG_ERROR, m_strMsg) ;
 			return false;
 		}
 	}
	
	if (! m_pIPhone->Adb_Shell_Start())
	{
		m_strMsg = "Fail to Adb_Shell_Start!";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	Sleep(m_nWiteStart);

	return true;
}




bool CAdbShellStart::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find ("WiteStart") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter WiteStart for CAdbShellStart");
		return false;
	}
	m_nWiteStart = atoi(paramMap["WiteStart"].c_str());
	return true;
}
