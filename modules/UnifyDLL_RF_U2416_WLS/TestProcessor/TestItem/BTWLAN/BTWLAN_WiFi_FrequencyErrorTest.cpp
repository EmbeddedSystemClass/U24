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

	if (paramMap.find("RetryTimes") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RetryTimes for CBTWLAN_WiFi_FrequencyErrorTest");
		return false;
	}
	m_iRetryTimes = atoi(paramMap["RetryTimes"].c_str());

	if (paramMap.find("CommandDelay") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CommandDelay for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_iCommandDelay = atoi(paramMap["CommandDelay"].c_str());

	if (paramMap.find("Frequency") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Frequency for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_dFrequency = atof(paramMap["Frequency"].c_str());
	m_strChannel = paramMap["Frequency"].c_str();


	if (paramMap.find("WlanMode") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter WlanMode for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_iWlanMode = atoi(paramMap["WlanMode"].c_str());

	if (paramMap.find("TxChain") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter TxChain for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_iTxChain = atoi(paramMap["TxChain"].c_str());

	if (paramMap.find("LoadBin") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter LoadBin for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_iLoadBin = atoi(paramMap["LoadBin"].c_str());

	if (paramMap.find("Channel") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Channel for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_iChannel = atoi(paramMap["Channel"].c_str());

	if (paramMap.find("Power") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Power for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_iPower = atoi(paramMap["Power"].c_str());

	if (paramMap.find("RateBitIndex") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RateBitIndex for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_iRateBitIndex = atoi(paramMap["RateBitIndex"].c_str());

	if (paramMap.find("RBW") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RBW for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_dRBW = atof(paramMap["RBW"].c_str());

	if (paramMap.find("VBW") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter VBW for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_dVBW = atof(paramMap["VBW"].c_str());

	if (paramMap.find("Span") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Span for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_dSpan = atof(paramMap["Span"].c_str());

	if (paramMap.find("Detector") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Detector for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_strDetector = paramMap["Detector"];

	if (paramMap.find("TraceMode") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter TraceMode for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_strTraceMode = paramMap["TraceMode"];

	if (paramMap.find("SweepTime") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter SweepTime for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_iSweepTime = atoi(paramMap["SweepTime"].c_str());

	if (paramMap.find("Trace") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Trace for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_iTrace = atoi(paramMap["Trace"].c_str());


	if (paramMap.find("LineName") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter LineName for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_strLineName = paramMap["LineName"];

	if (paramMap.find("Domain") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Domain for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_strDomain = paramMap["Domain"];

	if (paramMap.find("XscalingMode") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter XscalingMode for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_strXscalingMode = paramMap["XscalingMode"];

	if (paramMap.find("YscalingMode") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter YscalingMode for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_strYscalingMode = paramMap["YscalingMode"];

	if (paramMap.find("Unit") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Unit for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_strUnit = paramMap["Unit"];

	if (paramMap.find("Spacing") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Spacing for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_strSpacing = paramMap["Spacing"];

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
		TraceLog(MSG_ERROR, "Fail to find parameter ChannelPower for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_strChannelPower = paramMap["ChannelPower"];

	if (paramMap.find("ChannelBW") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter ChannelBW for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_dChannelBWMHz = atof(paramMap["ChannelBW"].c_str());

	if (paramMap.find("AverageTimes") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter AverageTimes for CBTWLAN_WiFi_TxTest");
		return false;
	}
	m_iAverageTimes = atoi(paramMap["AverageTimes"].c_str());

	if (paramMap.find("Range") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CBTWLAN_WiFi_TxTest");
		return false;
	}
	STDStringTokenizer(paramMap["Range"], ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CBTWLAN_WiFi_TxTest");
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
		FactoryLog(true, "WiFi_FrequencyErrorTest_FSP", "Pass", szBand, m_strChannel, szLower, szUpper, m_strMeasured, "Hz", "WiFi_FrequencyErrorTest_FSP PASS");
		SetPICSData(m_strPICSName, "PASS");
	}
	else
	{
		FactoryLog(false, "WiFi_FrequencyErrorTest_FSP", FunErr_WLAN_Freq_Error_Fail, szBand, m_strChannel, szLower, szUpper, m_strMeasured, "Hz", m_strMsg);
		SetPICSData(m_strPICSName, "FAIL");
	}
	SetPICSData(m_strPICSName_Value, m_strMeasured);

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

	if (! m_pIPhone->WifiPowerOnCW(m_iRateBitIndex, m_iChannel, m_iPower, m_iWlanMode, m_iTxChain, m_iLoadBin))
	{
		m_strMsg = "Fail to execute WifiPowerOnCw in CBTWLAN_WiFi_FrequencyErrorTest";
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
	if (! m_pITesterDeviceFSP->SetResolutionBandwidth(m_dRBW))
	{
		m_strMsg = "Fail to set resolution bandwidth to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	Sleep(1000);

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

	if (! m_pITesterDeviceFSP->SetAttRefLevel(m_iAttenuationManual, m_iRefLevel))
	{
		m_strMsg = "Fail to set reference level to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	Sleep(1000);

	if (! m_pITesterDeviceFSP->SwitchTraceMode("VIEW"))
	{
		m_strMsg = "Fail to switch trace mode to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDeviceFSP->SetMaxLevelForFreErr())
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
		if ( i > 0 )
		{
				if (! m_pITesterDeviceFSP->SetBlankOFF())//
				{
					m_strMsg = "Fail to set Blank OFF to tester device";
					TraceLog(MSG_ERROR, m_strMsg);
					return false;
				}
			    if (! m_pITesterDeviceFSP->SwitchTraceMode("WRIT"))//
				{
					m_strMsg = "Fail to switch trace mode to tester device";
					TraceLog(MSG_ERROR, m_strMsg);
					return false;
				}	
				if (! m_pITesterDeviceFSP->SwitchTraceMode(m_strTraceMode))//
				{
					m_strMsg = "Fail to switch trace mode to tester device";
					TraceLog(MSG_ERROR, m_strMsg);
					return false;
				}

				if (! m_pITesterDeviceFSP->SetDetector(m_strDetector))//
				{
					m_strMsg = "Fail to set detector to tester device";
					TraceLog(MSG_ERROR, m_strMsg);
					return false;
				}

				if (! m_pITesterDeviceFSP->SetAttRefLevel(m_iAttenuationManual, m_iRefLevel))//
				{
					m_strMsg = "Fail to set reference level to tester device";
					TraceLog(MSG_ERROR, m_strMsg);
					return false;
				}
				Sleep(1000);
				if (! m_pITesterDeviceFSP->SwitchTraceMode("VIEW"))//
				{
					m_strMsg = "Fail to switch trace mode to tester device";
					TraceLog(MSG_ERROR, m_strMsg);
					return false;
				}

				if (! m_pITesterDeviceFSP->SetMaxLevelForFreErr())//
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
		}

		if (! m_pITesterDeviceFSP->FrequencyErrorMeasurement(0, strResponse))
		{
			m_strMsg = "Fail to fetch RF analyzer power";
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}

		//AfxMessageBox("stop error test");
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
	//if (! m_pIPhone->WifiPowerOnCw(0, 0, 0, 0))
	//{
	//	m_strMsg = "Fail to STOP WifiPowerOnCw";
	//	TraceLog(MSG_ERROR, m_strMsg);
	//	return false;
	//}

	if (!m_pIPhone->WifiPowerOffTx(m_iChannel))
	{
		m_strMsg = "Fail to stop WifiPowerOnTx";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

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