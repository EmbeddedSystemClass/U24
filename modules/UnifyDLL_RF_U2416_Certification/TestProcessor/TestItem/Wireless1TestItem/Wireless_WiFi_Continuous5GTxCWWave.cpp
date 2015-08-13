#include "stdafx.h"
#include "Wireless_WiFi_Continuous5GTxCwWave.h"


// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CWireless_WiFi_Continuous5GTxCwWave)

bool CWireless_WiFi_Continuous5GTxCwWave::Run()
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

bool CWireless_WiFi_Continuous5GTxCwWave::MainFunction()
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
			if (!(isOk = m_pIPhone->WifiPowerOn5GTxCwCertification(m_iChannel, m_iRfGain, m_iBond)))
			{
				m_strMsg = "Fail to execute WifiPowerOnTxCw";
				TraceLog(MSG_ERROR, m_strMsg);
				isOk = false;
			}

			if(isOk)
			{
				int ret = MessageBox(NULL, _T("Do you want to stop Waveform?"), _T("Info."), 
					MB_ICONQUESTION | MB_OK);

				if (ret == 1)
				{
					isOk = m_pIPhone->WifiPowerStopTxCw();
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
			else
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

	return isOk;
}


bool CWireless_WiFi_Continuous5GTxCwWave::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find("CommandDelay") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CommandDelay for CWireless_WiFi_Continuous5GTxCwWave");
		return false;
	}
	m_iCommandDelay = atoi(paramMap["CommandDelay"].c_str());

	if (paramMap.find("Channel") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Channel for CWireless_WiFi_Continuous5GTxCwWave");
		return false;
	}
	m_iChannel = atoi(paramMap["Channel"].c_str());

	if (paramMap.find("RFGain") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RFGain for CWireless_WiFi_Continuous5GTxCwWave");
		return false;
	}
	m_iRfGain = atoi(paramMap["RFGain"].c_str());

	if (paramMap.find("Bond") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Bond for CWireless_WiFi_Continuous5GTxTest");
		return false;
	}
	m_iBond = atoi(paramMap["Bond"].c_str());

	if (paramMap.find("Chain") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Chain for CWireless_WiFi_ContinuousTxTest");
		//return false;
	}
	m_iChain = atoi(paramMap["Chain"].c_str());
	return true;
}