#include "stdafx.h"
#include "Wireless_WiFi_Continuous5GTxTest.h"


// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CWireless_WiFi_Continuous5GTxTest)

bool CWireless_WiFi_Continuous5GTxTest::Run()
{
	bool bRes = MainFunction();

	if (bRes)
	{
		//FactoryLog(true, "WiFi_TxTest", "Pass", szBand, m_strChannel, szLower, szUpper, m_strMeasured, "dBm", "Wireless WiFi Tx power PASS");
	}
	else
	{
		//FactoryLog(false, "WiFi_TxTest", FunErr_WirelessGSM_Test_OutPower_OutRange, szBand, m_strChannel, szLower, szUpper, m_strMeasured, "dBm", m_strMsg);
	}

	return bRes;
}

bool CWireless_WiFi_Continuous5GTxTest::MainFunction()
{
	bool isOk = true;

	if (!(isOk = m_pIPhone->Initial()))
	{
		m_strMsg = "Fail to connect phone with Qisda module";
		TraceLog(MSG_ERROR, m_strMsg);
	}


	// Start WiFi power
	Sleep(m_iCommandDelay);

	if (isOk)
	{
		if (!(isOk = m_pIPhone->WifiModuleOnCertification(true)))
		{
			m_strMsg = "Fail to Switch WIFI module on";
			TraceLog(MSG_ERROR, m_strMsg);
		}

		if (isOk)
		{
			if (!(isOk = m_pIPhone->WifiPowerOn5GTxCertification(m_iDataRate, m_iChannel, m_iPower, m_iWlandMode, m_iPayloadSize, m_iSpacing, m_iBond, m_iChain)))
			{
				m_strMsg = "Fail to execute WifiPowerOnTx";
				TraceLog(MSG_ERROR, m_strMsg);
			}

			int ret = MessageBox(NULL, _T("Do you want to stop Tx?"), _T("Info."), 
				MB_ICONQUESTION | MB_OK);

			if (ret == 1)
			{
				isOk = m_pIPhone->WifiPowerStopTx(m_iChannel);
				if (isOk)
				{
					if (!m_pIPhone->WifiModuleOnCertification(false))
					{
						m_strMsg = "Fail to Switch WIFI module off";
						TraceLog(MSG_ERROR, m_strMsg);
						isOk = false;
					}
				}
			}
		}
	}

	return isOk;
}


bool CWireless_WiFi_Continuous5GTxTest::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find("CommandDelay") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CommandDelay for CWireless_WiFi_Continuous5GTxTest");
		return false;
	}
	m_iCommandDelay = atoi(paramMap["CommandDelay"].c_str());

	if (paramMap.find("Channel") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Channel for CWireless_WiFi_Continuous5GTxTest");
		return false;
	}
	m_iChannel = atoi(paramMap["Channel"].c_str());

	if (paramMap.find("Rate") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Rate for CWireless_WiFi_Continuous5GTxTest");
		return false;
	}
	m_iDataRate = atoi(paramMap["Rate"].c_str());

	if (paramMap.find("WLANMODE") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter WLANMODE for CWireless_WiFi_Continuous5GTxTest");
		return false;
	}
	m_iWlandMode = atoi(paramMap["WLANMODE"].c_str());

	if (paramMap.find("Payload") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Payload for CWireless_WiFi_Continuous5GTxTest");
		return false;
	}
	m_iPayloadSize = atoi(paramMap["Payload"].c_str());

	if (paramMap.find("Spacing") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Spacing for CWireless_WiFi_Continuous5GTxTest");
		return false;
	}
	m_iSpacing = atoi(paramMap["Spacing"].c_str());

	if (paramMap.find("Power") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Power for CWireless_WiFi_Continuous5GTxTest");
		return false;
	}
	m_iPower = atoi(paramMap["Power"].c_str());

	if (paramMap.find("Bond") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Bond for CWireless_WiFi_Continuous5GTxTest");
		return false;
	}
	m_iBond = atoi(paramMap["Bond"].c_str());

	if (paramMap.find("Chain") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Chain for CWireless_WiFi_Continuous5GTxTest");
	//	return false;
	}
	m_iBond = atoi(paramMap["Chain"].c_str());


	return true;
}

bool CWireless_WiFi_Continuous5GTxTest::PostRun()
{
	return true;
}
