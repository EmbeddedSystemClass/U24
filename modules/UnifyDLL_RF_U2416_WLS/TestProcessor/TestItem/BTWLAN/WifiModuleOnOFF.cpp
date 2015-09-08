#include "StdAfx.h"
#include "WifiModuleOnOFF.h"

// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CWifiModuleOnOFF)

CWifiModuleOnOFF::CWifiModuleOnOFF(void)
{
}

CWifiModuleOnOFF::~CWifiModuleOnOFF(void)
{
}

bool CWifiModuleOnOFF::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find("WifiModule") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter WifiModule for CWifiModuleOnOFF");
		return false;
	}
	m_strWifiModule = paramMap["WifiModule"];
	
	if (paramMap.find("RetryTimes") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RetryTimes for CWifiModuleOnOFF");
		return false;
	}
	m_iRetryCount = atoi(paramMap["RetryTimes"].c_str());


	return true;
}

bool CWifiModuleOnOFF::Run(void)
{
	bool bRes = false;

	if (0 == strcmp(m_strWifiModule.c_str(), "ON"))
	{
		for(int i = 0; i < m_iRetryCount; i++)
		{
			// Start WiFi power
			if ((bRes = m_pIPhone->WifiModuleOn(true)))
			{
				break;
			}
			Sleep(1000);
		}

		if(!bRes)
		{
			m_strMsg = "SwitchWifiModule ON FAIL";
			TraceLog(MSG_ERROR, m_strMsg);
			FactoryLog(false, "WifiModuleOnOFF", FunErr_WLAN_Module_ON_Fail, "", "", "", "", "", "", m_strMsg);
			return false;
		}

		////liontest
		//if (!(bRes = m_pITesterDeviceFSP->Initial()))
		//{
		//	m_strMsg = "Fail to initial tester device";
		//	TraceLog(MSG_ERROR, m_strMsg);
		//	FactoryLog(false, "WifiModuleOnOFF", FunErr_WLAN_Test_Init_Device_Fail, "", "", "", "", "", "", m_strMsg);
		//	return false;
		//}

		if (bRes)
			FactoryLog(true, "WifiModuleOnOFF", "Pass", "", "", "", "", "", "", "WifiModuleOnOFF PASS");

	}
	else if (0 == strcmp(m_strWifiModule.c_str(), "OFF"))
	{
		// Stop WiFi power
		if (!(bRes = m_pIPhone->WifiModuleOn(false)))
		{
			m_strMsg = "SwitchWifiModule OFF FAIL";
			TraceLog(MSG_ERROR, m_strMsg);
			FactoryLog(false, "WifiModuleOnOFF", FunErr_WLAN_Module_OFF_Fail, "", "", "", "", "", "", m_strMsg);
			return false;
		}

		if (bRes)
			FactoryLog(true, "WifiModuleOnOFF", "Pass", "", "", "", "", "", "", "WifiModuleOnOFF PASS");
	}

	return bRes;
}
