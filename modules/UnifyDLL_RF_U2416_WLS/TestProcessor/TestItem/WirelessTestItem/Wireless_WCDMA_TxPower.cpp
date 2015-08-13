#include "stdafx.h"
#include "Wireless_WCDMA_TxPower.h"


// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CWireless_WCDMA_TxPower)

CWireless_WCDMA_TxPower::CWireless_WCDMA_TxPower()
	:m_WriteCPKLog(false)
{
}

bool CWireless_WCDMA_TxPower::Run()
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
	strcpy(szBand, "B1");

	// Prepare CPK log
	std::string cpkfile = string(DEFAULT_CPKLOG_PATH) + "Wireless_WCDMA_B1_TxPower_CPK";
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
		FactoryLog(true, "WCDMA_B1_TxPower", "Pass", szBand, m_strChannel, szLower, szUpper, m_strMeasured, "dBm", "Wireless WCDMA B1 TxPower PASS");
	}
	else
	{
		FactoryLog(false, "WCDMA_B1_TxPower", FunErr_WirelessWCDMA_Test_OutPower_OutRange, szBand, m_strChannel, szLower, szUpper, m_strMeasured, "dBm", m_strMsg);
	}
	SetPICSData(m_strPICSName, m_strMeasured);
	gCpkRecord.assign ("WCDMA_B1_TxPower_VALUE", m_strMeasured); 

	// write result to log file
	TraceLog (MSG_INFO, "Wireless_WCDMA_B1_TxPower_HEADER: " + gCpkRecord.GetLogHeader());
	TraceLog (MSG_INFO, "Wireless_WCDMA_B1_TxPower_RESULT: " + gCpkRecord.GetLogString());

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


bool CWireless_WCDMA_TxPower::MainFunction()
{
	char szTmp[256];
	unsigned short iFTM_Error_Code = 0;
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

	if (! m_pITesterDevice->RF_SetAnalyzerFreq(m_dFrequency))
	{
		m_strMsg = "Fail to set RF analyzer frequency to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDevice->SetResolutionBandwidth(1))
	{
		m_strMsg = "Fail to set resolution bandwidth";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDevice->RF_SetLevelMode(m_strLevelMode.c_str()))
	{
		m_strMsg = "Fail to set level mode";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDevice->RF_SetLevelMaximum (m_iLevelMax ))
	{
		m_strMsg = "Fail to set level reference";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDevice->SetInputLoss( 1 , m_iInputPort, GetLossByDegree(m_iPowerDegree)))
		return false;

	//------------------------------------------------------------------------------------

	if (! m_pIPhone->QMSL_FTM_SET_CALIBRATION_STATE(m_iCalibrationState))
	{
		m_strMsg = "Fail to set calibration";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_SET_MODE(PHONE_MODE_WCDMA_IMT))	//(PHONE_MODE_WCDMA)
	{
		Sleep(2500);
		if(! m_pIPhone->QMSL_FTM_SET_MODE(PHONE_MODE_WCDMA_IMT))
		{
			m_strMsg = "Fail to set WCDMA mode";
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}
	}
	if (! m_pIPhone->QMSL_FTM_SET_MODE_ID(FTM_MODE_ID_WCDMA)) 
	{
		m_strMsg = "Fail to set mode id";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	if (! m_pIPhone->QMSL_FTM_SET_CHAN(m_iChannel))
	{
		m_strMsg = "Fail to set channel";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_SET_TX_ON())
	{
		m_strMsg = "Fail to set Tx ON";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_CDMA_CW_WAVEFORM(0)) 
	{
		m_strMsg = "Fail to set cw Waveform";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	if (! m_pIPhone->QMSL_FTM_SET_PA_STATE( m_iPA ,  &iFTM_Error_Code)) 
	{
		m_strMsg = "Fail to set PA";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	if (! m_pIPhone->QMSL_FTM_SET_PDM(m_iPDM)) 
	{
		m_strMsg = "Fail to set PDM";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	if (! m_pIPhone->QMSL_FTM_SET_SMPS_PA_BIAS_OVERRIDE(true)) 
	{
		m_strMsg = "Fail to set PA_BIAS_OVERRIDE";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	if (! m_pIPhone->QMSL_FTM_SET_SMPS_PA_BIAS_VAL(3700)) 
	{
		m_strMsg = "Fail to set PA_BIAS_VAL";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}


	//if (! m_pIPhone->QMSL_FTM_SET_MODE_ID(FTM_MODE_ID_WCDMA)) 
	//{
	//	m_strMsg = "Fail to set mode id";
	//	TraceLog(MSG_ERROR, m_strMsg);
	//	return false;
	//}

	//if (! m_pIPhone->QMSL_FTM_SET_TX_POWER_DBM(1, m_iPowerLevel, &iFTM_Error_Code)) //0 ==> Disable, 1 ==> Enable
	//{
	//	m_strMsg = "Fail to enable Tx power DBM";
	//	TraceLog(MSG_ERROR, m_strMsg);
	//	return false;
	//}	

	//if (! m_pIPhone->QMSL_FTM_SET_MODE_ID(FTM_MODE_ID_WCDMA)) 
	//{
	//	m_strMsg = "Fail to set mode id";
	//	TraceLog(MSG_ERROR, m_strMsg);
	//	return false;
	//}

	//if (! m_pIPhone->QMSL_FTM_SET_TX_ON())
	//{
	//	m_strMsg = "Fail to set Tx ON";
	//	TraceLog(MSG_ERROR, m_strMsg);
	//	return false;
	//}

	//---------------------------------------------------------------------
	Sleep( m_iCommandDelay );

	bool bPass = false;
	double dPower;
	std::string strResponse;
	std::vector<std::string> vPowerList;

	double dMax = -99999.0;

	for (int i = 0; i < m_iRetryTimes; ++i)
	{
		if (! m_pITesterDevice->RF_NPowerMeasurement( 10 , strResponse ))
		//if (! m_pITesterDevice->RF_NPowerMeasurementMAX( 10, strResponse ))
		{
			m_strMsg = "Fail to fetch RF analyzer power";
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}

		if (strstr(strResponse.c_str(), "NAN") != NULL)
		{
			sprintf_s(szTmp, 255, "%0d - WCDMA TxPower : NAN", i + 1);
			m_strMsg = szTmp;
			TraceLog(MSG_ERROR, m_strMsg);
			continue;
		}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      

		STDStringTokenizer(strResponse, ",", vPowerList);
		dPower = atof(vPowerList[5].c_str());
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%0d - WCDMA TxPower : %.3f", i + 1, dPower);
		m_strMsg = szTmp;
		TraceLog(MSG_INFO, m_strMsg);

		if (dPower > dMax)
			dMax = dPower;

		if ( ! InRange( m_dLower , m_dUpper , dMax ) )
		{
			if ( i < m_iRetryTimes - 1 )
			{
				m_strMsg = "WCDMA TxPower not in range, try again";
				TraceLog(MSG_ERROR, m_strMsg);
			}
			else
			{
				m_strMsg = "WCDMA TxPower not in range";
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


	if (! m_pIPhone->QMSL_FTM_SET_CALIBRATION_STATE(0))
	{
		m_strMsg = "Fail to set calibration";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	if (! m_pIPhone->QMSL_FTM_SET_TX_OFF())
	{
		m_strMsg = "Fail to set Tx OFF";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	if(!bPass)
	{
		m_strMsg = "Return false before TX off.";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	return bPass;
}

double CWireless_WCDMA_TxPower::GetLossByDegree(int iDegree)
{
	double dLoss = 0.0;

	switch (iDegree)
	{
	case 0:
		dLoss = GetCableLoss("TX", "WirelessWCDMA_B1", "0");
		break;
	case 1:
		dLoss = GetCableLoss("TX", "WirelessWCDMA_B1", "1");
		break;
	default:
		dLoss = GetCableLoss("TX", "WirelessWCDMA_B1", "2");
	}

	return dLoss;
}


bool CWireless_WCDMA_TxPower::InitData(std::map<std::string, std::string>& paramMap)
{
	std::vector<std::string> vTmp;

	if (paramMap.find("PICSName") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PICSName for CWireless_WCDMA_TxPower");
		return false;
	}
	m_strPICSName = paramMap["PICSName"];

	if (paramMap.find("CommandDelay") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CommandDelay for CWireless_WCDMA_TxPower");
		return false;
	}
	m_iCommandDelay = atoi(paramMap["CommandDelay"].c_str());

	if (paramMap.find("StartDelay") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter StartDelay for CWireless_WCDMA_TxPower");
		return false;
	}
	m_iStartDelay = atoi(paramMap["StartDelay"].c_str());

	if (paramMap.find("Frequency") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Frequency for CWireless_WCDMA_TxPower");
		return false;
	}
	m_dFrequency = atof(paramMap["Frequency"].c_str());

	if (paramMap.find("PowerDegree") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PowerDegree for CWireless_WCDMA_TxPower");
		return false;
	}
	m_iPowerDegree = atoi(paramMap["PowerDegree"].c_str());

	if (paramMap.find("PowerLevel") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PowerLevel for CWireless_WCDMA_TxPower");
		return false;
	}
	m_iPowerLevel = atoi(paramMap["PowerLevel"].c_str());



	if (paramMap.find("PAState") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PAState for CWireless_WCDMA_TxPower");
		return false;
	}
	m_iPA = atoi(paramMap["PAState"].c_str());
	if (paramMap.find("PDM") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PDM for CWireless_WCDMA_TxPower");
		return false;
	}
	m_iPDM = atoi(paramMap["PDM"].c_str());


	if (paramMap.find("Channel") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Channel for CWireless_WCDMA_TxPower");
		return false;
	}
	m_iChannel = atoi(paramMap["Channel"].c_str());
	m_strChannel = paramMap["Channel"].c_str();

	if (paramMap.find("TxGainIndex") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Power for CWireless_WCDMA_TxGainIndex");
		return false;
	}
	m_iTxGainIndex = atoi(paramMap["TxGainIndex"].c_str());

	if (paramMap.find("RetryTimes") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RetryTimes for CWireless_WCDMA_TxPower");
		return false;
	}
	m_iRetryTimes = atoi(paramMap["RetryTimes"].c_str());

	if (paramMap.find("InputPort") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter InputPort for CWireless_WCDMA_TxPower");
		return false;
	}
	m_iInputPort = atoi(paramMap["InputPort"].c_str());

	if (paramMap.find("Range") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CWireless_WCDMA_TxPower");
		return false;
	}
	STDStringTokenizer(paramMap["Range"], ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CWireless_WCDMA_TxPower");
		return false;
	}
	m_dLower = atof(vTmp[0].c_str());
	m_dUpper = atof(vTmp[1].c_str());

	if (paramMap.find("TestMethod") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter TestMethod for CWireless_WCDMA_TxPower");
		return false;
	}
	m_iTestMethod = atoi(paramMap["TestMethod"].c_str());

	if (paramMap.find("LevelMode") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter LevelMode for CWireless_WCDMA_TxPower");
		return false;
	}
	m_strLevelMode = paramMap["LevelMode"];

	if (paramMap.find("LevelMax") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter LevelMax for CWireless_WCDMA_TxPower");
		return false;
	}
	m_iLevelMax = atoi(paramMap["LevelMax"].c_str());

	if (paramMap.find("CalibrationState") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CalibrationState for CWireless_WCDMA_TxPower");
		return false;
	}
	m_iCalibrationState = atoi(paramMap["CalibrationState"].c_str());


	if (paramMap.find ("WriteCPKLog") != paramMap.end())
		m_WriteCPKLog = paramMap["WriteCPKLog"] != "0";

	ParseCPKItems();

	return true;
}

bool CWireless_WCDMA_TxPower::ParseCPKItems()
{
	m_CPKHeaderList.clear();

	if (m_TestItemXML.Load(m_cstrTestItemFile) != ERROR_SUCCESS)
		return false;

	XMLNode processObjectSet = m_TestItemXML.SearchNode(_T("//Configuration//ProcessObjectSet//WirelessTestObjects//WCDMA_B1_TxPower_CPKcolumn"));
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