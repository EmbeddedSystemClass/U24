#include "stdafx.h"
#include "Wireless_WiFi_ContinuousRxTest.h"


// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CWireless_WiFi_ContinuousRxTest)

bool CWireless_WiFi_ContinuousRxTest::Run()
{
	bool bRes = MainFunction();

	if (bRes)
	{
		FactoryLog(true, "WiFi_RxTest", "Pass", "-", "-", "-", "-", m_strMeasured, "dBm", "WiFi Rx PASS");
	}
	else
	{
		FactoryLog(false, "WiFi_RxTest", "Fail", "-", "-", "-", "-", m_strMeasured, "dBm", m_strMsg);
	}

	return bRes;
}

bool CWireless_WiFi_ContinuousRxTest::MainFunction()
{
	bool isOk = true;
	unsigned long rxFrameCounter = 0;
	char* szRxFrameCounter = new char[1000];

	if (!(isOk = m_pIPhone->Initial()))
	{
		m_strMsg = "Fail to connect phone with Qisda module  in CWireless_WiFi_ContinuousRxTest";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	// Start WiFi power
	Sleep(m_iCommandDelay);
	char szTmp[256] = {0};
	char szMeasuredTmp[256] = {0};

	if (isOk)
	{
		if (!(isOk = m_pIPhone->WifiModuleOnCertification(true)))
		{
			m_strMsg = "Fail to Switch WIFI module on";
			TraceLog(MSG_ERROR, m_strMsg);
		}

		if (isOk)
		{
			//WifiPowerOnRxGetPacket
			if (!(isOk = m_pIPhone->WifiPowerOnRxCertification(m_iChannel, m_iWlandMode, m_iRateMask, m_iChain)))
			{
				m_strMsg = "Fail to execute WifiPowerOnRxGetPacket";
				TraceLog(MSG_ERROR, m_strMsg);
			}
			Sleep(m_iCommandDelay);

			int ret = MessageBox(NULL, _T("Get Packet Counter?\nYes ->Get Packet Counter\nNo->Stop"), _T("Info."), 
				MB_ICONQUESTION | MB_YESNOCANCEL); 


			while(1)
			{
				if (ret == 6)
				{
					m_pIPhone->WifiPowerOnRxGetPacket(szRxFrameCounter);
	
					sprintf(szTmp, "Total Packets Received: %c\nYes ->Get Packet Counter\nNo->Reset Packet Counter\nCancel->Stop", szRxFrameCounter);
					ret = MessageBox(NULL, szTmp, _T("Info."), 
						MB_ICONQUESTION | MB_YESNOCANCEL);
					sprintf(szMeasuredTmp, "%c", szRxFrameCounter);
				}
				else
				{
					break;
				}
				if (!isOk)
				{
					break;
				}
			}

			if (!m_pIPhone->WifiModuleOnCertification(false))
			{
				m_strMsg = "Fail to Switch WIFI module off";
				TraceLog(MSG_ERROR, m_strMsg);
				isOk = false;
			}
		}
	}

	m_strMeasured = szMeasuredTmp;

	return isOk;
}


bool CWireless_WiFi_ContinuousRxTest::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find("CommandDelay") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CommandDelay for CWireless_WiFi_ContinuousRxTest");
		return false;
	}
	m_iCommandDelay = atoi(paramMap["CommandDelay"].c_str());

	if (paramMap.find("CHANNEL") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CHANNEL for CWireless_WiFi_ContinuousRxTest");
		return false;
	}
	m_iChannel = atoi(paramMap["CHANNEL"].c_str());

	if (paramMap.find("RATEMASK") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RateMask for CWireless_WiFi_ContinuousRxTest");
		return false;
	}
	m_iRateMask = atoi(paramMap["RATEMASK"].c_str());


	if (paramMap.find("WLANMODE") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter WLANMODE for CWireless_WiFi_ContinuousRxTest");
		return false;
	}
	m_iWlandMode = atoi(paramMap["WLANMODE"].c_str());


	if (paramMap.find("CHAIN") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Chain for CWireless_WiFi_ContinuousRxTest");
		return false;
	}
	m_iChain = atoi(paramMap["CHAIN"].c_str());


	return true;
}

bool CWireless_WiFi_ContinuousRxTest::PostRun()
{
	return true;
}
