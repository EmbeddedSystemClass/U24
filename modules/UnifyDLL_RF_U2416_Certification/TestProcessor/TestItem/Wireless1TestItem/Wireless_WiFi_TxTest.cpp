#include "stdafx.h"
#include "Wireless_WiFi_TxTest.h"


// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CWireless_WiFi_TxTest)

bool CWireless_WiFi_TxTest::Run()
{
	m_strMeasured = "";

	char szLower[256];
	memset(szLower, 0, 256);
	sprintf_s(szLower, 255, "%.3f", m_dLower);

	char szUpper[256];
	memset(szUpper, 0, 256);
	sprintf_s(szUpper, 255, "%.3f", m_dUpper);

	char szBand[256];
	memset(szBand, 0, 256);
	strcpy(szBand, "WiFi");


	bool bRes = MainFunction();

	if (bRes)
	{
		FactoryLog(true, "WiFi_TxTest", "Pass", szBand, m_strChannel, szLower, szUpper, m_strMeasured, "dBm", "Wireless WiFi Tx power PASS");
		SetPICSDate(m_strPICSName, "PASS");
		SetPICSDate(m_strPICSName_Value, m_strMeasured);
	}
	else
	{
		FactoryLog(false, "WiFi_TxTest", FunErr_WirelessGSM_Test_OutPower_OutRange, szBand, m_strChannel, szLower, szUpper, m_strMeasured, "dBm", m_strMsg);
		SetPICSDate(m_strPICSName, "FAIL");
		SetPICSDate(m_strPICSName_Value, m_strMeasured);
	}

	return bRes;
}

bool CWireless_WiFi_TxTest::MainFunction()
{
	char szTmp[256];

	// Initial tester device
	if (! m_pITesterDevice->Initial())
	{
		m_strMsg = "Fail to initial tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDevice->Initial_RF_Nonsignaling())
	{
		m_strMsg = "Fail to initial tester device : RF non-signaling";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	m_pITesterDevice->SetMode(1);

	if (! m_pITesterDevice->SetInputPort(1, m_iInputPort))
	{
		m_strMsg = "Fail to set RF input port";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDevice->RF_InitialAnalyzer())
	{
		m_strMsg = "Fail to initial tester device for RF analyzer";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	double m_dFrequency = WiFi_ChannelToFrequency(m_iChannel);
	if (! m_pITesterDevice->RF_SetAnalyzerFreq(m_dFrequency))
	{
		m_strMsg = "Fail to set RF analyzer frequency to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	memset(szTmp, 0, 256);
	sprintf(szTmp, "%.2f", m_dFrequency);
	m_strChannel = szTmp;

	memset(szTmp, 0, 256);
	sprintf(szTmp, "%d", m_iChannel);
	if (! m_pITesterDevice->SetInputLoss(1, m_iInputPort, GetCableLoss("TX", "WirelessWLAN", szTmp)))
		return false;

	Sleep(m_iCommandDelay);

	// Start WiFi power
	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to connect phone with Qisda module";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	Sleep(m_iCommandDelay);

	if (! m_pIPhone->WifiModuleOn(true))
	{
		m_strMsg = "SwitchWifiModule ON FAIL";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->WifiPowerOnTx(m_iDataRate, m_iChannel, m_iPower))
	{
		m_strMsg = "Fail to execute WifiPowerOnTx";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	Sleep(m_iCommandDelay);

	bool bPass = false;
	double dPower;
	std::string strResponse;
	std::vector<std::string> vPowerList;

	double dMax = -99999.0;

	for (int i = 0; i < m_iRetryTimes; ++i)
	{
		if (! m_pITesterDevice->RF_NPowerMeasurement(10, strResponse))
		{
			m_strMsg = "Fail to fetch RF analyzer power";
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}

		if (strstr(strResponse.c_str(), "NAN") != NULL)
		{
			sprintf_s(szTmp, 255, "%0d - WiFi power : NAN", i + 1);
			m_strMsg = szTmp;
			TraceLog(MSG_ERROR, m_strMsg);
			continue;
		}

		STDStringTokenizer(strResponse, ",", vPowerList);
		dPower = atof(vPowerList[2].c_str());

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%0d - WiFi power : %.3f", i + 1, dPower);
		m_strMsg = szTmp;
		TraceLog(MSG_INFO, m_strMsg);

		if (dPower > dMax)
			dMax = dPower;

		if (! InRange(m_dLower, m_dUpper, dMax))
		{
			if (i < m_iRetryTimes - 1)
			{
				m_strMsg = "WiFi power not in range, try again";
				TraceLog(MSG_ERROR, m_strMsg);
			}
			else
			{
				m_strMsg = "WiFi power not in range";
				TraceLog(MSG_ERROR, m_strMsg);
			}
		}
		else
		{
			bPass = true;
			break;
		}

		Sleep(500);
	}

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", dMax);
	m_strMeasured = szTmp;

	// Stop WiFi power
	if (! m_pIPhone->WifiModuleOn(false))
	{
		m_strMsg = "SwitchWifiModule OFF FAIL";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	Sleep(m_iCommandDelay);

	return bPass;
}


bool CWireless_WiFi_TxTest::InitData(std::map<std::string, std::string>& paramMap)
{
	std::vector<std::string> vTmp;

	if (paramMap.find("PICSName") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PICSName for CWireless_WiFi_TxTest");
		return false;
	}
	m_strPICSName = paramMap["PICSName"];
	m_strPICSName_Value = m_strPICSName + "_Value";

	if (paramMap.find("CommandDelay") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CommandDelay for CWireless_WiFi_TxTest");
		return false;
	}
	m_iCommandDelay = atoi(paramMap["CommandDelay"].c_str());

	if (paramMap.find("Channel") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Channel for CWireless_WiFi_TxTest");
		return false;
	}
	m_iChannel = atoi(paramMap["Channel"].c_str());

	if (paramMap.find("DataRate") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Type for CWireless_WiFi_TxTest");
		return false;
	}
	m_iDataRate = atoi(paramMap["DataRate"].c_str());

	if (paramMap.find("OutPower") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Power for CWireless_WiFi_TxTest");
		return false;
	}
	m_iPower = atoi(paramMap["OutPower"].c_str());

	if (paramMap.find("RetryTimes") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RetryTimes for CWireless_WiFi_TxTest");
		return false;
	}
	m_iRetryTimes = atoi(paramMap["RetryTimes"].c_str());

	if (paramMap.find("InputPort") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter InputPort for CWireless_WiFi_TxTest");
		return false;
	}
	m_iInputPort = atoi(paramMap["InputPort"].c_str());

	if (paramMap.find("Range") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CWireless_WiFi_TxTest");
		return false;
	}
	STDStringTokenizer(paramMap["Range"], ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CWireless_WiFi_TxTest");
		return false;
	}
	m_dLower = atof(vTmp[0].c_str());
	m_dUpper = atof(vTmp[1].c_str());

	return true;
}
