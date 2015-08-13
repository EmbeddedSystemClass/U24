#include "StdAfx.h"
#include "BTWLAN_WiFi_FrequencyErrorTest.h"

// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CBTWLAN_WiFi_FrequencyErrorTest)

CBTWLAN_WiFi_FrequencyErrorTest::CBTWLAN_WiFi_FrequencyErrorTest(void)
	:m_WriteCPKLog(false)
{
}

CBTWLAN_WiFi_FrequencyErrorTest::~CBTWLAN_WiFi_FrequencyErrorTest(void)
{
}

bool CBTWLAN_WiFi_FrequencyErrorTest::InitData(std::map<std::string, std::string>& paramMap)
{
	std::vector<std::string> vTmp;

	if (paramMap.find("PICSName") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PICSName for CBTWLAN_WiFi_FrequencyErrorTest");
		return false;
	}
	m_strPICSName = paramMap["PICSName"];
	m_strPICSName_Value = m_strPICSName + "_Value";

	if (paramMap.find("CommandDelay") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CommandDelay for CBTWLAN_WiFi_FrequencyErrorTest");
		return false;
	}
	m_iCommandDelay = atoi(paramMap["CommandDelay"].c_str());

	if (paramMap.find("Frequency") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Frequency for CBTWLAN_WiFi_FrequencyErrorTest");
		return false;
	}
	m_dFrequency = atof(paramMap["Frequency"].c_str());
	m_strChannel = paramMap["Frequency"].c_str();

	if (paramMap.find("Channel") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Channel for CBTWLAN_WiFi_FrequencyErrorTest");
		return false;
	}
	m_iChannel = atoi(paramMap["Channel"].c_str());

	if (paramMap.find("Gain") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Gain for CBTWLAN_WiFi_FrequencyErrorTest");
		return false;
	}
	m_iGain = atoi(paramMap["Gain"].c_str());

	if (paramMap.find("Tone") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Tone for CBTWLAN_WiFi_FrequencyErrorTest");
		return false;
	}
	m_lTone = atoi(paramMap["Tone"].c_str());

	if (paramMap.find("Ampl") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Ampl for CBTWLAN_WiFi_FrequencyErrorTest");
		return false;
	}
	m_lAmpl = atoi(paramMap["Ampl"].c_str());

	if (paramMap.find("RBW") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RBW for CBTWLAN_WiFi_FrequencyErrorTest");
		return false;
	}
	m_dRBW = atof(paramMap["RBW"].c_str());

	if (paramMap.find("VBW") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter VBW for CBTWLAN_WiFi_FrequencyErrorTest");
		return false;
	}
	m_dVBW = atof(paramMap["VBW"].c_str());

	if (paramMap.find("Span") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Span for CBTWLAN_WiFi_FrequencyErrorTest");
		return false;
	}
	m_dSpan = atof(paramMap["Span"].c_str());

	if (paramMap.find("TraceMode") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter TraceMode for CBTWLAN_WiFi_FrequencyErrorTest");
		return false;
	}
	m_strTraceMode = paramMap["TraceMode"];

	if (paramMap.find("Detector") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Detector for CBTWLAN_WiFi_FrequencyErrorTest");
		return false;
	}
	m_strDetector = paramMap["Detector"];

	if (paramMap.find("DiagramFull") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter DiagramFull for CBTWLAN_WiFi_FrequencyErrorTest");
		return false;
	}
	m_strDiagramFull = paramMap["DiagramFull"];

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

	if (paramMap.find("Trace") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Trace for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_iTrace = atoi(paramMap["Trace"].c_str());

	if (paramMap.find("PeakExcursion") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PeakExcursion for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_dPeakExcursion = atof(paramMap["PeakExcursion"].c_str());

	if (paramMap.find("FreqCounterRes") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter FreqCounterRes for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_iFreqCounterRes = atof(paramMap["FreqCounterRes"].c_str());

	if (paramMap.find("RetryTimes") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RetryTimes for CBTWLAN_WiFi_FrequencyErrorTest");
		return false;
	}
	m_iRetryTimes = atoi(paramMap["RetryTimes"].c_str());

	if (paramMap.find("Range") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CBTWLAN_WiFi_FrequencyErrorTest");
		return false;
	}
	STDStringTokenizer(paramMap["Range"], ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CBTWLAN_WiFi_FrequencyErrorTest");
		return false;
	}
	m_dLower = atof(vTmp[0].c_str());
	m_dUpper = atof(vTmp[1].c_str());

	if (paramMap.find ("WriteCPKLog") != paramMap.end())
		m_WriteCPKLog = paramMap["WriteCPKLog"] != "0";

	ParseCPKItems();

	return true;
}

bool CBTWLAN_WiFi_FrequencyErrorTest::Run(void)
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

	// Prepare CPK log
	std::string cpkfile = string(DEFAULT_CPKLOG_PATH) + "WifiFreqErrCPK";
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
		FactoryLog(true, "WiFi_FrequencyErrorTest_FSP", "Pass", szBand, m_strChannel, szLower, szUpper, m_strMeasured, "dBm", "WiFi_FrequencyErrorTest_FSP PASS");
		SetPICSDate(m_strPICSName, "PASS");
		SetPICSDate(m_strPICSName_Value, m_strMeasured);
	}
	else
	{
		FactoryLog(false, "WiFi_FrequencyErrorTest_FSP", FunErr_WLAN_Freq_Error_Fail, szBand, m_strChannel, szLower, szUpper, m_strMeasured, "dBm", m_strMsg);
		SetPICSDate(m_strPICSName, "FAIL");
		SetPICSDate(m_strPICSName_Value, m_strMeasured);
	}

	// write result to log file
	TraceLog (MSG_INFO, "WifiFreqErrCPK_HEADER: " + gCpkRecord.GetLogHeader());
	TraceLog (MSG_INFO, "WifiFreqErrCPK_RESULT: " + gCpkRecord.GetLogString());

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

bool CBTWLAN_WiFi_FrequencyErrorTest::MainFunction(void)
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
	if (! m_pIPhone->WifiPowerOnCw(m_iChannel, m_iGain, m_lTone, m_lAmpl))
	{
		m_strMsg = "Fail to execute WifiPowerOnCw";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	// Initial tester device
	if (! m_pITesterDeviceFSP->Initial())
	{
		m_strMsg = "Fail to initial tester device";
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

	//Sleep(m_iCommandDelay);	

	if (! m_pITesterDeviceFSP->SetFrequencySpan(m_dSpan))
	{
		m_strMsg = "Fail to set frequency span to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}	

	if (! m_pITesterDeviceFSP->SwitchTraceMode(m_strTraceMode))
	{
		m_strMsg = "Fail to switch trace mode to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDeviceFSP->SetDetector(m_strDetector))
	{
		m_strMsg = "Fail to set detector to tester device";
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

	if (! m_pITesterDeviceFSP->SetFrequencyError(m_dPeakExcursion, m_iTrace, m_iFreqCounterRes))
	{
		m_strMsg = "Fail to adjust reference level to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	Sleep(m_iCommandDelay);

	bool bRes = false;
	double dPower;
	std::string strResponse;
	std::vector<std::string> vPowerList;

	double dMax = -99999.0;

	for (int i = 0; i < m_iRetryTimes; ++i)
	{
		if (! m_pITesterDeviceFSP->FrequencyErrorMeasurement(0, strResponse))
		{
			m_strMsg = "Fail to fetch RF analyzer power";
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}

		if (strstr(strResponse.c_str(), "NAN") != NULL)
		{
			sprintf_s(szTmp, 255, "%0d - WiFi FrequencyError : NAN", i + 1);
			m_strMsg = szTmp;
			TraceLog(MSG_ERROR, m_strMsg);
			continue;
		}

		//STDStringTokenizer(strResponse, ",", vPowerList);
		//dPower = atof(vPowerList[2].c_str());
		dPower = atof(strResponse.c_str());

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%0d - WiFi FrequencyError : %.3f", i + 1, dPower);
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
			bRes = true;
			break;
		}

		Sleep(m_iCommandDelay);
	}

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", dMax);
	m_strMeasured = szTmp;

	gCpkRecord.assign ("Wifi_FrequencyError", m_strMeasured);//write GPS_CN0 to CPK

	// Stop WiFi power Cw
	if (! m_pIPhone->WifiPowerOnCw(0, 0, 0, 0))
	{
		m_strMsg = "Fail to STOP WifiPowerOnCw";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
/*
	// Stop WiFi power
	if (! m_pIPhone->WifiModuleOn(false))
	{
		m_strMsg = "SwitchWifiModule OFF FAIL";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	Sleep(m_iCommandDelay);
*/	
	return bRes;
}

bool CBTWLAN_WiFi_FrequencyErrorTest::ParseCPKItems()
{
	m_CPKHeaderList.clear();

	if (m_TestItemXML.Load(m_cstrTestItemFile) != ERROR_SUCCESS)
		return false;

	XMLNode processObjectSet = m_TestItemXML.SearchNode(_T("//Configuration//ProcessObjectSet//CommonTestObjects//WifiFreqErrCPKcolumn"));
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