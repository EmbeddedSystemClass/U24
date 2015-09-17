#include "stdafx.h"
#include "Wireless_WiFi_ContinuousTxCwWave.h"


// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CWireless_WiFi_ContinuousTxCwWave)

bool CWireless_WiFi_ContinuousTxCwWave::Run()
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

bool CWireless_WiFi_ContinuousTxCwWave::MainFunction()
{
	bool isOk = true;
	
	if (!(isOk = m_pIPhone->Initial()))
	{
		m_strMsg = "Fail to connect phone with Qisda module in CWireless_WiFi_ContinuousTxTest";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
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
			//if (!(isOk = m_pIPhone->WifiPowerOnTxCwCertification(m_iChannel, m_iRfGain)))
			if (!(isOk = m_pIPhone->WifiPowerOnTxCwCertification(m_iChannel, m_iPower, m_iRateBitIndex, m_iWlandMode, m_iChain)))
			{
				m_strMsg = "Fail to execute WifiPowerOnTxCw";
				TraceLog(MSG_ERROR, m_strMsg);
			}

			//int ret = MessageBox(NULL, _T("Do you want to stop Waveform?"), _T("Info."), 
			//	MB_ICONQUESTION | MB_OK);
			
			int ret = ::MessageBox(NULL, _T("Do you want to stop Waveform?"), _T("Info."), 
				MB_ICONQUESTION | MB_OK);
			
			if (ret == 1)
			{
				//isOk = m_pIPhone->WifiPowerStopTxCw();
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


bool CWireless_WiFi_ContinuousTxCwWave::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find("CommandDelay") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CommandDelay for CWireless_WiFi_ContinuousTxCwWave");
		return false;
	}
	m_iCommandDelay = atoi(paramMap["CommandDelay"].c_str());

	if (paramMap.find("CHANNEL") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Channel for CWireless_WiFi_ContinuousTxCwWave");
		return false;
	}
	m_iChannel = atoi(paramMap["CHANNEL"].c_str()); 



	if (paramMap.find("POWER") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Power for CWireless_WiFi_ContinuousTxCwWave");
		return false;
	}
	m_iPower = atoi(paramMap["POWER"].c_str());


	if (paramMap.find("RATEBITINDEX") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RateBitIndex for CWireless_WiFi_ContinuousTxCwWave");
		return false;
	}
	m_iRateBitIndex = atoi(paramMap["RATEBITINDEX"].c_str());


	if (paramMap.find("WLANMODE") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter WLANMODE for CWireless_WiFi_ContinuousTxCwWave");
		return false;
	}
	m_iWlandMode = atoi(paramMap["WLANMODE"].c_str());

	if (paramMap.find("CHAIN") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Chain for CWireless_WiFi_ContinuousTxCwWave");
		return false;
	}
	m_iChain = atoi(paramMap["CHAIN"].c_str());
	
	return true;
}