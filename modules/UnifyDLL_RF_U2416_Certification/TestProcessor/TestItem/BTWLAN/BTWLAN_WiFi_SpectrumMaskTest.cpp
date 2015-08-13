#include "StdAfx.h"
#include "BTWLAN_WiFi_SpectrumMaskTest.h"

// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CBTWLAN_WiFi_SpectrumMaskTest)

CBTWLAN_WiFi_SpectrumMaskTest::CBTWLAN_WiFi_SpectrumMaskTest(void)
	:m_WriteCPKLog(false)
	,m_bKLossMode(false)
{
}

CBTWLAN_WiFi_SpectrumMaskTest::~CBTWLAN_WiFi_SpectrumMaskTest(void)
{
}

bool CBTWLAN_WiFi_SpectrumMaskTest::InitData(std::map<std::string, std::string>& paramMap)
{
	std::vector<std::string> vTmp;

	if (paramMap.find("PICSName") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PICSName for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_strPICSName = paramMap["PICSName"];
	m_strPICSName_Value = m_strPICSName + "_Value";

	if (paramMap.find("CommandDelay") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CommandDelay for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_iCommandDelay = atoi(paramMap["CommandDelay"].c_str());

	if (paramMap.find("Frequency") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Frequency for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_dFrequency = atof(paramMap["Frequency"].c_str());
	m_strChannel = paramMap["Frequency"].c_str();

	if (paramMap.find("Channel") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Channel for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_iChannel = atoi(paramMap["Channel"].c_str());

	if (paramMap.find("Power") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Power for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_iPower = atoi(paramMap["Power"].c_str());

	if (paramMap.find("DataRate") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter DataRate for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_iDataRate = atoi(paramMap["DataRate"].c_str());

	if (paramMap.find("RBW") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RBW for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_dRBW = atof(paramMap["RBW"].c_str());

	if (paramMap.find("VBW") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter VBW for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_dVBW = atof(paramMap["VBW"].c_str());

	if (paramMap.find("Span") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Span for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_dSpan = atof(paramMap["Span"].c_str());

	if (paramMap.find("Detector") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Detector for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_strDetector = paramMap["Detector"];

	if (paramMap.find("TraceMode") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter TraceMode for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_strTraceMode = paramMap["TraceMode"];

	if (paramMap.find("DiagramFull") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter DiagramFull for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_strDiagramFull = paramMap["DiagramFull"];

	if (paramMap.find("SweepTime") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter SweepTime for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_iSweepTime = atoi(paramMap["SweepTime"].c_str());

	if (paramMap.find("Trace") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Trace for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_iTrace = atoi(paramMap["Trace"].c_str());

	if (paramMap.find("Margin") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Margin for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_iMargin = atoi(paramMap["Margin"].c_str());

	if (paramMap.find("Threshold") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Threshold for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_iThreshold = atoi(paramMap["Threshold"].c_str());
	
	if (paramMap.find("LineName") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter LineName for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_strLineName = paramMap["LineName"];

	if (paramMap.find("Domain") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Domain for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_strDomain = paramMap["Domain"];

	if (paramMap.find("XscalingMode") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter XscalingMode for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_strXscalingMode = paramMap["XscalingMode"];

	if (paramMap.find("YscalingMode") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter YscalingMode for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_strYscalingMode = paramMap["YscalingMode"];

	if (paramMap.find("Unit") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Unit for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_strUnit = paramMap["Unit"];

	if (paramMap.find("Spacing") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Spacing for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_strSpacing = paramMap["Spacing"];

	if (paramMap.find("ContData") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter ContData for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_strContData = paramMap["ContData"];

	if (paramMap.find("UppData") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter UppData for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_strUppData = paramMap["UppData"];

	if (paramMap.find("AttenuationManual") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter AttenuationManual for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_iAttenuationManual = atoi(paramMap["AttenuationManual"].c_str());

	if (paramMap.find("RefLevel") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RefLevel for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_iRefLevel = atoi(paramMap["RefLevel"].c_str());

	if (paramMap.find("ChannelPower") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter ChannelPower for CBTWLAN_BT_TxTest");
		return false;
	}
	m_strChannelPower = paramMap["ChannelPower"];

	if (paramMap.find("ChannelBW") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter ChannelBW for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_dChannelBWMHz = atof(paramMap["ChannelBW"].c_str());
/*
	if (paramMap.find("AdjPower") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter AdjPower for CBTWLAN_BT_TxTest");
		return false;
	}
	m_dAdjPower = atof(paramMap["AdjPower"].c_str());
*/
	if (paramMap.find("AverageTimes") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter AverageTimes for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_iAverageTimes = atoi(paramMap["AverageTimes"].c_str());


	if (paramMap.find ("Regulatory_Fixed") == paramMap.end()){
		TraceLog(MSG_ERROR, "Fail to find parameter Regulatory_Fixed for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_Regulatory_Fixed = atoi(paramMap["Regulatory_Fixed"].c_str());



	if (paramMap.find("Range") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	STDStringTokenizer(paramMap["Range"], ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CBTWLAN_WiFi_SpectrumMaskTest");
		return false;
	}
	m_dLower = atof(vTmp[0].c_str());
	m_dUpper = atof(vTmp[1].c_str());

	if (paramMap.find ("WriteCPKLog") != paramMap.end())
		m_WriteCPKLog = paramMap["WriteCPKLog"] != "0";

	if (paramMap.find ("KLossMode") != paramMap.end())
		m_bKLossMode = paramMap["KLossMode"] != "0";

	ParseCPKItems();

	return true;
}

bool CBTWLAN_WiFi_SpectrumMaskTest::Run(void)
{
	m_strMeasured = "";
/*
	char szLower[256];
	memset(szLower, 0, 256);
	sprintf_s(szLower, 255, "%.3f", m_dLower);

	char szUpper[256];
	memset(szUpper, 0, 256);
	sprintf_s(szUpper, 255, "%.3f", m_dUpper);

	char szBand[256];
	memset(szBand, 0, 256);
	strcpy(szBand, "WiFi");
*/
	// Prepare CPK log
	std::string cpkfile;
	if (0 == strcmp(m_strLineName.c_str(), "\"11B\""))
	{
		cpkfile = string(DEFAULT_CPKLOG_PATH) + "WifiSpecMaskCPK_11B";
		m_dAdjPower = GetCableLoss("TX", "BTWLAN_WIFI", "WifiAdjPower_11B");
	}
	else if (0 == strcmp(m_strLineName.c_str(), "\"11G\""))
	{
		cpkfile = string(DEFAULT_CPKLOG_PATH) + "WifiSpecMaskCPK_11G";
		m_dAdjPower = GetCableLoss("TX", "BTWLAN_WIFI", "WifiAdjPower_11G");
	}
	else if (0 == strcmp(m_strLineName.c_str(), "\"11N\""))
	{
		cpkfile = string(DEFAULT_CPKLOG_PATH) + "WifiSpecMaskCPK_11N";
		m_dAdjPower = GetCableLoss("TX", "BTWLAN_WIFI", "WifiAdjPower_11N");
	}

	gCpkRecord.setFileName (cpkfile.c_str());
	gCpkRecord.ResetAll();

	int count = m_CPKHeaderList.size();
	for (int i=0; i < count; ++i)
	{
		gCpkRecord.setHeader (i, m_CPKHeaderList[i].c_str());
	}

	bool bRes = MainFunction();

	if (bRes)
	{
		//FactoryLog(true, "WiFi_SpectrumMaskTest_FSP", "Pass", szBand, m_strChannel, szLower, szUpper, m_strMeasured, "dBm", "WiFi_SpectrumMaskTest_FSP PASS");
		SetPICSDate(m_strPICSName, "PASS");
		SetPICSDate(m_strPICSName_Value, m_strMeasured);
	}
	else
	{
		//FactoryLog(false, "WiFi_SpectrumMaskTest_FSP", FunErr_WLAN_Spectral_Mask_Fail, szBand, m_strChannel, szLower, szUpper, m_strMeasured, "dBm", m_strMsg);
		SetPICSDate(m_strPICSName, "FAIL");
		SetPICSDate(m_strPICSName_Value, m_strMeasured);
	}

	// write result to log file
	TraceLog (MSG_INFO, "WifiSpecMskCPK_HEADER: " + gCpkRecord.GetLogHeader());
	TraceLog (MSG_INFO, "WifiSpecMskCPK_RESULT: " + gCpkRecord.GetLogString());

	// write result to CPK log
	if (m_WriteCPKLog)
	{
		gCpkRecord.assign ("SN", g_strPicasso);
		gCpkRecord.assign ("Result", ( bRes ? "PASS" : "FAIL"));

		SYSTEMTIME currTime;
		::GetLocalTime(&currTime);
		std::string strTime = MakeSTDstring ("%02d:%02d:%02d", currTime.wHour, currTime.wMinute, currTime.wSecond);
		gCpkRecord.assign ("Time", strTime);

		gCpkRecord.WriteRecordToFile();
	}

	return bRes;
}

bool CBTWLAN_WiFi_SpectrumMaskTest::MainFunction(void)
{
	char szTmp[256];
/*
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
*/
	//if (! m_pIPhone->WifiPowerOnTx(m_iDataRate, m_iChannel, m_iPower))
	if (! m_pIPhone->WifiPowerOnTx(m_iDataRate, m_iChannel, m_iPower, m_Regulatory_Fixed))
		
	{
		m_strMsg = "Fail to execute WifiPowerOnTx";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
/*
	// Initial tester device
	if (! m_pITesterDeviceFSP->Initial())
	{
		m_strMsg = "Fail to initial tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
*/
	if (! m_pITesterDeviceFSP->SetAttenuation(true))
	{
		m_strMsg = "Fail to set attenuation ON";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	//Sleep(m_iCommandDelay);

	if (! m_pITesterDeviceFSP->SetResolutionBandwidth(m_dRBW))
	{
		m_strMsg = "Fail to set resolution bandwidth to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDeviceFSP->SetVideoBandwidth(m_dVBW))
	{
		m_strMsg = "Fail to set video bandwidth to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDeviceFSP->RF_SetAnalyzerFreq(m_dFrequency))
	{
		m_strMsg = "Fail to set RF analyzer frequency to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDeviceFSP->SetSweepTime(m_iSweepTime))
	{
		m_strMsg = "Fail to set sweep time to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDeviceFSP->SetFrequencySpan(m_dSpan))
	{
		m_strMsg = "Fail to set frequency span to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDeviceFSP->SetDetector(m_strDetector))
	{
		m_strMsg = "Fail to set detector to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDeviceFSP->SwitchTraceMode(m_strTraceMode))
	{
		m_strMsg = "Fail to switch trace mode to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

/*
	if (! m_pITesterDeviceFSP->AdjustReferenceLevel())
	{
		m_strMsg = "Fail to adjust reference level to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
*/
	if (! m_pITesterDeviceFSP->SetAttRefLevel(m_iAttenuationManual, m_iRefLevel))
	{
		m_strMsg = "Fail to set reference level to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	Sleep(2000);

	if (! m_pITesterDeviceFSP->SwitchTraceMode("VIEW"))
	{
		m_strMsg = "Fail to switch trace mode to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDeviceFSP->SetMaxLevel())
	{
		m_strMsg = "Fail to set max level to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	//Sleep(m_iCommandDelay);

	if (! m_pITesterDeviceFSP->SetLimitLines(m_strLineName, m_strDomain, m_iTrace, m_strXscalingMode,
		m_strUnit, m_strYscalingMode, m_strSpacing, m_iMargin, m_iThreshold, m_strContData, m_strUppData))
	{
		m_strMsg = "Fail to set limit lines to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDeviceFSP->SetChannelBandwidth(m_dChannelBWMHz))
	{
		m_strMsg = "Fail to set channel bandwidth to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
/*
	if (! m_pITesterDeviceFSP->SwitcheMeasurementWindow(m_strDiagramFull))
	{
		m_strMsg = "Fail to switch measurement window to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDeviceFSP->SetAttRefLevel(m_iAttenuationManual, m_iRefLevel))
	{
		m_strMsg = "Fail to set reference level to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	Sleep(2000);

	if (! m_pITesterDeviceFSP->SetMaxLevel())
	{
		m_strMsg = "Fail to set max level to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	Sleep(m_iCommandDelay);

	if (! m_pITesterDeviceFSP->AdjustReferenceLevel())
	{
		m_strMsg = "Fail to adjust reference level to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

*/
	bool bMaskRes = false;
	bool bPowerRes = false;
	int iMask;
	double dPower;
	std::string strResponse;
	std::vector<std::string> vPowerList;

	//double dMax = -99999.0;
	double dTotal = 0.0;

	char szLower[256];
	memset(szLower, 0, 256);
	sprintf_s(szLower, 255, "%.3f", m_dLower);

	char szUpper[256];
	memset(szUpper, 0, 256);
	sprintf_s(szUpper, 255, "%.3f", m_dUpper);

	char szBand[256];
	memset(szBand, 0, 256);
	strcpy(szBand, "WiFi");

	if (! m_pITesterDeviceFSP->SpectrumMaskMeasurement(0, strResponse))
	{
		m_strMsg = "Fail to fetch Spectrum Mask";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (strstr(strResponse.c_str(), "NAN") != NULL)
	{
		sprintf_s(szTmp, 255, "WiFi Spectrum Mask : NAN");
		m_strMsg = szTmp;
		TraceLog(MSG_ERROR, m_strMsg);
		//continue;
	}

	iMask = atoi(strResponse.c_str());

	if (! InRange(0, 0, iMask))
	{/*
		if (i < m_iRetryTimes - 1)
		{
			m_strMsg = "WiFi Spectrum Mask not in range, try again";
			TraceLog(MSG_ERROR, m_strMsg);
		}
		else*/
		{
			m_strMsg = "WiFi Spectrum Mask not in range";
			TraceLog(MSG_ERROR, m_strMsg);
			bMaskRes = false;
		}
	}
	else
	{
		bMaskRes = true;
		//break;
	}

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "WiFi Spectrum Mask : %d", iMask);
	m_strMeasured = szTmp;
	TraceLog(MSG_INFO, m_strMeasured);

	if (!bMaskRes)
		FactoryLog(false, "WiFi_SpectrumMaskTest_FSP", FunErr_WLAN_Spectral_Mask_Fail, szBand, m_strChannel, "0", "0", m_strMeasured, "", m_strMsg);

	if (! m_pITesterDeviceFSP->SwitchChannelPower(m_strChannelPower))
	{
		m_strMsg = "Fail to switch channel power to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	for (int i = 0; i < m_iAverageTimes; ++i)
	{
		if (! m_pITesterDeviceFSP->RF_NPowerMeasurement(0, strResponse))
		{
			m_strMsg = "Fail to fetch Wifi Power Tx";
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

		//STDStringTokenizer(strResponse, ",", vPowerList);
		//dPower = atof(vPowerList[2].c_str());
		dPower = (atof(strResponse.c_str()) + m_dAdjPower);

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%0d - WiFi power : %.3f", i + 1, dPower);
		m_strMsg = szTmp;
		TraceLog(MSG_INFO, m_strMsg);
/*
		if (dPower > dMax)
			dMax = dPower;
*/
		dTotal += dPower;
	}

	dPower = (dTotal/m_iAverageTimes);
	if (! InRange(m_dLower, m_dUpper, dPower))
	{/*
		if (i < m_iRetryTimes - 1)
		{
			m_strMsg = "WiFi power not in range, try again";
			TraceLog(MSG_ERROR, m_strMsg);
		}
		else*/
		{
			m_strMsg = "WiFi power not in range";
			TraceLog(MSG_ERROR, m_strMsg);
			bPowerRes = false;
		}
	}
	else
	{
		bPowerRes = true;
		//break;
		if (m_bKLossMode)
		{
			double dLoss = 0.0;
			if (0 == strcmp(m_strLineName.c_str(), "\"11B\""))
				dLoss = GetRefResult("TX", "BTWLAN_WIFI", "WifiResult_11B") - dPower;
			else if (0 == strcmp(m_strLineName.c_str(), "\"11G\""))
				dLoss = GetRefResult("TX", "BTWLAN_WIFI", "WifiResult_11G") - dPower;
			else if (0 == strcmp(m_strLineName.c_str(), "\"11N\""))
				dLoss = GetRefResult("TX", "BTWLAN_WIFI", "WifiResult_11N") - dPower;

			memset(szTmp, 0, 256);
			sprintf_s(szTmp, 255, "%.1f", dLoss);

			if (0 == strcmp(m_strLineName.c_str(), "\"11B\""))
				UpdateCableLoss("TX", "BTWLAN_WIFI", "WifiAdjPower_11B", szTmp);
			else if (0 == strcmp(m_strLineName.c_str(), "\"11G\""))
				UpdateCableLoss("TX", "BTWLAN_WIFI", "WifiAdjPower_11G", szTmp);
			else if (0 == strcmp(m_strLineName.c_str(), "\"11N\""))
				UpdateCableLoss("TX", "BTWLAN_WIFI", "WifiAdjPower_11N", szTmp);
		}
	}

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "SpectrumMask : %d, Power : %.3f", iMask, dPower);
	m_strMeasured = szTmp;
	TraceLog(MSG_INFO, m_strMeasured);

	if (bMaskRes & bPowerRes)
		FactoryLog(true, "WiFi_SpectrumMask & PowerTxTest_FSP", "Pass", szBand, m_strChannel, szLower, szUpper, m_strMeasured, "dBm", "WiFi_SpectrumMask & PowerTxTest_FSP PASS");
	
	if (!bPowerRes)
		FactoryLog(false, "WiFi_PowerTxTest_FSP", FunErr_WLAN_Test_OutPower_OutRange, szBand, m_strChannel, szLower, szUpper, m_strMeasured, "dBm", m_strMsg);

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", dPower);
	m_strMeasured = szTmp;
	gCpkRecord.assign ("Wifi_SpectrumMask", (iMask == 0 ? "PASS" : "FAIL"));//write Spectrum Mask to CPK
	gCpkRecord.assign ("Wifi_TxPower", m_strMeasured);//write Tx Power to CPK

	// Stop WiFi power
	if (! m_pIPhone->WifiPowerOnTx(0, 0, 0))
	{
		m_strMsg = "Fail to stop WifiPowerOnTx";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
/*
	if (! m_pIPhone->WifiModuleOn(false))
	{
		m_strMsg = "SwitchWifiModule OFF FAIL";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	Sleep(m_iCommandDelay);
*/
	return (bMaskRes & bPowerRes);
}

bool CBTWLAN_WiFi_SpectrumMaskTest::ParseCPKItems()
{
	m_CPKHeaderList.clear();

	if (m_TestItemXML.Load(m_cstrTestItemFile) != ERROR_SUCCESS)
		return false;

	XMLNode processObjectSet = m_TestItemXML.SearchNode(_T("//Configuration//ProcessObjectSet//CommonTestObjects//WifiTxCPKcolumn"));
	if (processObjectSet == NULL)
		return false;

	XMLNodeList processObjectsCategoryList = processObjectSet->GetchildNodes();
	if (processObjectsCategoryList == NULL)
		return false;

	int listsize = m_TestItemXML.GetNodeListSize(processObjectsCategoryList);
	for (int j = 0; j < listsize; ++j)
	{
		XMLNode processObject = m_TestItemXML.GetNodeListItem(processObjectsCategoryList, j);
		if (processObject == NULL)
			continue;
			
		// Parse the process object
		CString cstrName = m_TestItemXML.GetNodeAttrValue(processObject, _T("Name"));
		if (cstrName.IsEmpty())
			continue;
		CT2A szName(cstrName);
		std::string strName(szName);

		m_CPKHeaderList.push_back(strName);
	}

	return true;
}