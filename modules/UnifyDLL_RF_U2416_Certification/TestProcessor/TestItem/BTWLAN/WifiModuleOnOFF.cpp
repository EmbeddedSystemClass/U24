#include "StdAfx.h"
#include "WifiModuleOnOFF.h"

// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CWifiModuleOnOFF)

CWifiModuleOnOFF::CWifiModuleOnOFF(void)
:m_bResetBin(false)
{
}

CWifiModuleOnOFF::~CWifiModuleOnOFF(void)
{
}

bool CWifiModuleOnOFF::InitData(std::map<std::string, std::string>& paramMap)
{
	std::vector<std::string> vTmp;

	if (paramMap.find("WifiModule") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter WifiModule for CWifiModuleOnOFF");
		return false;
	}
	m_strWifiModule = paramMap["WifiModule"];

	if (paramMap.find("LoadGoldenBin") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter LoadGoldenBin for CWifiModuleOnOFF");
		return false;
	}
	m_iLoadGoldenBin = atoi(paramMap["LoadGoldenBin"].c_str());

	if (0 == strcmp(m_strWifiModule.c_str(), "ON") && m_iLoadGoldenBin > 0)
	{
		if (paramMap.find("CommandDelay") == paramMap.end())
		{
			TraceLog(MSG_ERROR, "Fail to find parameter CommandDelay for CWifiModuleOnOFF");
			return false;
		}
		m_iCommandDelay = atoi(paramMap["CommandDelay"].c_str());

		if (paramMap.find("Frequency") == paramMap.end())
		{
			TraceLog(MSG_ERROR, "Fail to find parameter Frequency for CWifiModuleOnOFF");
			return false;
		}
		m_dFrequency = atof(paramMap["Frequency"].c_str());
		m_strChannel = paramMap["Frequency"].c_str();

		if (paramMap.find("Channel") == paramMap.end())
		{
			TraceLog(MSG_ERROR, "Fail to find parameter Channel for CWifiModuleOnOFF");
			return false;
		}
		m_iChannel = atoi(paramMap["Channel"].c_str());

		if (paramMap.find("Power") == paramMap.end())
		{
			TraceLog(MSG_ERROR, "Fail to find parameter Power for CWifiModuleOnOFF");
			return false;
		}
		m_iPower = atoi(paramMap["Power"].c_str());

		if (paramMap.find("DataRate") == paramMap.end())
		{
			TraceLog(MSG_ERROR, "Fail to find parameter DataRate for CWifiModuleOnOFF");
			return false;
		}
		m_iDataRate = atoi(paramMap["DataRate"].c_str());

		if (paramMap.find("RBW") == paramMap.end())
		{
			TraceLog(MSG_ERROR, "Fail to find parameter RBW for CWifiModuleOnOFF");
			return false;
		}
		m_dRBW = atof(paramMap["RBW"].c_str());

		if (paramMap.find("VBW") == paramMap.end())
		{
			TraceLog(MSG_ERROR, "Fail to find parameter VBW for CWifiModuleOnOFF");
			return false;
		}
		m_dVBW = atof(paramMap["VBW"].c_str());

		if (paramMap.find("Span") == paramMap.end())
		{
			TraceLog(MSG_ERROR, "Fail to find parameter Span for CWifiModuleOnOFF");
			return false;
		}
		m_dSpan = atof(paramMap["Span"].c_str());

		if (paramMap.find("Detector") == paramMap.end())
		{
			TraceLog(MSG_ERROR, "Fail to find parameter Detector for CWifiModuleOnOFF");
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
			TraceLog(MSG_ERROR, "Fail to find parameter DiagramFull for CWifiModuleOnOFF");
			return false;
		}
		m_strDiagramFull = paramMap["DiagramFull"];

		if (paramMap.find("SweepTime") == paramMap.end())
		{
			TraceLog(MSG_ERROR, "Fail to find parameter SweepTime for CWifiModuleOnOFF");
			return false;
		}
		m_iSweepTime = atoi(paramMap["SweepTime"].c_str());

		if (paramMap.find("Trace") == paramMap.end())
		{
			TraceLog(MSG_ERROR, "Fail to find parameter Trace for CWifiModuleOnOFF");
			return false;
		}
		m_iTrace = atoi(paramMap["Trace"].c_str());

		if (paramMap.find("Margin") == paramMap.end())
		{
			TraceLog(MSG_ERROR, "Fail to find parameter Margin for CWifiModuleOnOFF");
			return false;
		}
		m_iMargin = atoi(paramMap["Margin"].c_str());

		if (paramMap.find("Threshold") == paramMap.end())
		{
			TraceLog(MSG_ERROR, "Fail to find parameter Threshold for CWifiModuleOnOFF");
			return false;
		}
		m_iThreshold = atoi(paramMap["Threshold"].c_str());
		
		if (paramMap.find("LineName") == paramMap.end())
		{
			TraceLog(MSG_ERROR, "Fail to find parameter LineName for CWifiModuleOnOFF");
			return false;
		}
		m_strLineName = paramMap["LineName"];

		if (paramMap.find("Domain") == paramMap.end())
		{
			TraceLog(MSG_ERROR, "Fail to find parameter Domain for CWifiModuleOnOFF");
			return false;
		}
		m_strDomain = paramMap["Domain"];

		if (paramMap.find("XscalingMode") == paramMap.end())
		{
			TraceLog(MSG_ERROR, "Fail to find parameter XscalingMode for CWifiModuleOnOFF");
			return false;
		}
		m_strXscalingMode = paramMap["XscalingMode"];

		if (paramMap.find("YscalingMode") == paramMap.end())
		{
			TraceLog(MSG_ERROR, "Fail to find parameter YscalingMode for CWifiModuleOnOFF");
			return false;
		}
		m_strYscalingMode = paramMap["YscalingMode"];

		if (paramMap.find("Unit") == paramMap.end())
		{
			TraceLog(MSG_ERROR, "Fail to find parameter Unit for CWifiModuleOnOFF");
			return false;
		}
		m_strUnit = paramMap["Unit"];

		if (paramMap.find("Spacing") == paramMap.end())
		{
			TraceLog(MSG_ERROR, "Fail to find parameter Spacing for CWifiModuleOnOFF");
			return false;
		}
		m_strSpacing = paramMap["Spacing"];

		if (paramMap.find("ContData") == paramMap.end())
		{
			TraceLog(MSG_ERROR, "Fail to find parameter ContData for CWifiModuleOnOFF");
			return false;
		}
		m_strContData = paramMap["ContData"];

		if (paramMap.find("UppData") == paramMap.end())
		{
			TraceLog(MSG_ERROR, "Fail to find parameter UppData for CWifiModuleOnOFF");
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
			TraceLog(MSG_ERROR, "Fail to find parameter ChannelBW for CWifiModuleOnOFF");
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
			TraceLog(MSG_ERROR, "Fail to find parameter AverageTimes for CWifiModuleOnOFF");
			return false;
		}
		m_iAverageTimes = atoi(paramMap["AverageTimes"].c_str());

		if (paramMap.find("Range") == paramMap.end())
		{
			TraceLog(MSG_ERROR, "Fail to find parameter Range for CWifiModuleOnOFF");
			return false;
		}
		STDStringTokenizer(paramMap["Range"], ",", vTmp);
		if ((signed)vTmp.size() < 2)
		{
			TraceLog(MSG_ERROR, "Fail to find parameter Range for CWifiModuleOnOFF");
			return false;
		}
		m_dLower = atof(vTmp[0].c_str());
		m_dUpper = atof(vTmp[1].c_str());

		if (paramMap.find ("ResetBin") != paramMap.end())
			m_bResetBin = paramMap["ResetBin"] != "0";
	}

	return true;
}

bool CWifiModuleOnOFF::Run(void)
{
	bool bRes = false;

	char szLower[256];
	memset(szLower, 0, 256);
	sprintf_s(szLower, 255, "%.3f", m_dLower);

	char szUpper[256];
	memset(szUpper, 0, 256);
	sprintf_s(szUpper, 255, "%.3f", m_dUpper);

	if (0 == strcmp(m_strWifiModule.c_str(), "ON"))
	{
		// Start WiFi power
		if (!(bRes = m_pIPhone->Initial()))
		{
			m_strMsg = "Fail to connect phone with Qisda module";
			TraceLog(MSG_ERROR, m_strMsg);
			FactoryLog(false, "WifiModuleOnOFF", FunErr_WLAN_Test_Init_Device_Fail, "", "", "", "", "", "", m_strMsg);
			return false;
		}

		Sleep(500);

		if (m_bResetBin)
		{
			if (!(bRes = m_pIPhone->ResetBinFile()))
			{
				m_strMsg = "Fail to reset bin file";
				TraceLog(MSG_ERROR, m_strMsg);
				FactoryLog(false, "WifiModuleOnOFF", CommErr_Push_File_To_Device_Fail, "", "", "", "", "", "", m_strMsg);
				return false;
			}
		}

		if (0 == m_iLoadGoldenBin)
		{
			if (!(bRes = m_pIPhone->WifiModuleOn(true)))
			{
				m_strMsg = "SwitchWifiModule ON FAIL";
				TraceLog(MSG_ERROR, m_strMsg);
				FactoryLog(false, "WifiModuleOnOFF", FunErr_WLAN_Module_ON_Fail, "", "", "", "", "", "", m_strMsg);
				return false;
			}

			if (!(bRes = m_pITesterDeviceFSP->Initial()))
			{
				m_strMsg = "Fail to initial tester device";
				TraceLog(MSG_ERROR, m_strMsg);
				FactoryLog(false, "WifiModuleOnOFF", FunErr_WLAN_Test_Init_Device_Fail, "", "", "", "", "", "", m_strMsg);
				return false;
			}

			if (bRes)
				FactoryLog(true, "WifiModuleOnOFF", "Pass", "", "", "", "", m_strMeasured, "", "WifiModuleOnOFF PASS");
		}
		else
		{
			bool bRes = false;
			int iBin = 0;
			if (!(bRes = m_pIPhone->WifiModuleOn(true, iBin)))
			{
				m_strMsg = "SwitchWifiModule ON FAIL";
				TraceLog(MSG_ERROR, m_strMsg);
				FactoryLog(false, "WifiModuleOnOFF", FunErr_WLAN_Module_ON_Fail, "", "", "", "", "", "", m_strMsg);
				return false;
			}

			// Initial tester device
			if (!(bRes = m_pITesterDeviceFSP->Initial()))
			{
				m_strMsg = "Fail to initial tester device";
				TraceLog(MSG_ERROR, m_strMsg);
				FactoryLog(false, "WifiModuleOnOFF", FunErr_WLAN_Test_Init_Device_Fail, "", "", "", "", "", "", m_strMsg);
				return false;
			}

			bRes = RunWifiBasicItem(&iBin);

			if (bRes && iBin > 0)
			{
				// Stop WiFi power
				if (!(bRes = m_pIPhone->WifiModuleOn(false)))
				{
					m_strMsg = "SwitchWifiModule OFF FAIL";
					TraceLog(MSG_ERROR, m_strMsg);
					FactoryLog(false, "WifiModuleOnOFF", FunErr_WLAN_Module_OFF_Fail, "", "", "", "", "", "", m_strMsg);
					return false;
				}

				if (!(bRes = m_pIPhone->WifiModuleOn(true, iBin)))
				{
					m_strMsg = "SwitchWifiModule ON FAIL";
					TraceLog(MSG_ERROR, m_strMsg);
					FactoryLog(false, "WifiModuleOnOFF", FunErr_WLAN_Module_ON_Fail, "", "", "", "", "", "", m_strMsg);
					return false;
				}

				if (!(bRes = m_pITesterDeviceFSP->Initial()))
				{
					m_strMsg = "Fail to initial tester device";
					TraceLog(MSG_ERROR, m_strMsg);
					FactoryLog(false, "WifiModuleOnOFF", FunErr_WLAN_Test_Init_Device_Fail, "", "", "", "", "", "", m_strMsg);
					return false;
				}
			}
			
			if (! m_pIPhone->WifiBinNumberWrite( 1220, 4, iBin ))
			{
				m_strMsg = "WifiBinNumberWrite FAIL";
				TraceLog(MSG_ERROR, m_strMsg);
				FactoryLog(false, "WifiModuleOnOFF", FunErr_WLAN_Test_Set_Command_Fail, "", "", "", "", "", "", m_strMsg);
				return false;
			}

			if (bRes)
				FactoryLog(true, "WifiModuleOnOFF", "Pass", "", "", szLower, szUpper, m_strMeasured, "", "WifiModuleOnOFF PASS");
		}
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

bool CWifiModuleOnOFF::RunWifiBasicItem(int *iBin)
{
	char szTmp[256];

	if (! m_pIPhone->WifiPowerOnTx(m_iDataRate, m_iChannel, m_iPower))
	{
		m_strMsg = "Fail to execute WifiPowerOnTx";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

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


	double dPower;
	std::string strResponse;
	std::vector<std::string> vPowerList;

	double dTotal = 0.0;

	if (! m_pITesterDeviceFSP->SpectrumMaskMeasurement(0, strResponse))
	{
		m_strMsg = "Fail to fetch Spectrum Mask";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

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

		m_dAdjPower = GetCableLoss("TX", "BTWLAN_WIFI", "WifiAdjPower_11B");
		dPower = (atof(strResponse.c_str()) + m_dAdjPower);

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%0d - WiFi power : %.3f", i + 1, dPower);
		m_strMsg = szTmp;
		TraceLog(MSG_INFO, m_strMsg);

		dTotal += dPower;
	}

	dPower = (dTotal/m_iAverageTimes);
	
	if (! InRange(m_dLower, m_dUpper, dPower))
	{
		if (dPower > m_dUpper)
			*iBin = 1;
		if (dPower < m_dLower)
			*iBin = 2;
	}
	else
	{
		*iBin = 0;
		//bPowerRes = true;
		//break;
	}

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "Bin File : Bin%d, Power : %.3f", *iBin, dPower);
	m_strMsg = szTmp;
	m_strMeasured = szTmp;
	TraceLog(MSG_INFO, m_strMsg);

	// Stop WiFi power
	if (! m_pIPhone->WifiPowerOnTx(0, 0, 0))
	{
		m_strMsg = "Fail to stop WifiPowerOnTx";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	return true;
}
