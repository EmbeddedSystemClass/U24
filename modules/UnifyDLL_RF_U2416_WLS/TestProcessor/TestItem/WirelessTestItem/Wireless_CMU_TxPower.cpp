#include "stdafx.h"
#include "Wireless_CMU_TxPower.h"


// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CWireless_CMU_TxPower)

CWireless_CMU_TxPower::CWireless_CMU_TxPower()
{
}

bool CWireless_CMU_TxPower::Run()
{
	m_strMeasured = "";

	char szLower[256];
	memset(szLower, 0, 256);
	sprintf_s(szLower, 255, "%.3f", m_dLower);

	char szUpper[256];
	memset(szUpper, 0, 256);
	sprintf_s(szUpper, 255, "%.3f", m_dUpper);


	if (0 == strcmp(m_strLineName.c_str(), "\"11B_Chain_1\""))
	{
		m_dAdjPower = GetCableLoss("TX", "BTWLAN_WIFI", "WifiAdjPower_11B_Chain_1");
	}

	// Define //------------------------------------------------------------------------------
	char szBandFunction[256];
	memset(szBandFunction, 0, 256);
	sprintf_s(szBandFunction, 255, "TX_%d_TxPower", m_iChannel );

	//----------------------------------------------------------------------------------------


	// Prepare CPK log
	std::string cpkfile = string(DEFAULT_CPKLOG_PATH) + "CMU_2G4_CPK";
	gCpkRecord.setFileName (cpkfile.c_str());
	gCpkRecord.ResetAll();

	int count = m_CPKHeaderList.size();
	for (int i=0; i < count; ++i)
	{
		gCpkRecord.setHeader (i, m_CPKHeaderList[i].c_str());
	}


	bool bRes = MainFunction();

	// write result to log file
	TraceLog (MSG_INFO, "2G4_CPK_HEADER: " + gCpkRecord.GetLogHeader());
	TraceLog (MSG_INFO, "2G4_CPK_HEADER: " + gCpkRecord.GetLogString());

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


	if ( bRes )
	{
		char szTestSuccess[256];
		memset(szTestSuccess, 0, 256);
		sprintf_s(szTestSuccess, 255, "WifiTxTest 2.4G PASS");		

		FactoryLog( true , szBandFunction , "Pass" , m_strChannel , szLower , szUpper , m_strMeasured , "dBm", szTestSuccess );
	}
	else
	{
		FactoryLog( false , szBandFunction , FunErr_WLAN_Test_OutPower_OutRange , m_strChannel , szLower , szUpper , m_strMeasured , "dBm" , m_strMsg );	
	}
	SetPICSData(m_strPICSName, m_strMeasured);
	//----------------------------------------------------------------------------------------------------


	return bRes;

}



bool CWireless_CMU_TxPower::MainFunction()
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
		m_strMsg = "Fail to initial tester device : RF non-signaling in CWireless_CMU_TxPower";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	m_pITesterDevice->SetMode(1);

	if (! m_pITesterDevice->SetInputPort(1, m_iInputPort))
	{
		m_strMsg = "Fail to set RF input port in CWireless_CMU_TxPower ";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDevice->RF_InitialAnalyzer())
	{
		m_strMsg = "Fail to initial tester device for RF analyzer in CWireless_CMU_TxPower ";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDevice->RF_SetAnalyzerFreq(m_dFrequency))
	{
		m_strMsg = "Fail to set RF analyzer frequency to tester device in CWireless_CMU_TxPower";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDevice->SetResolutionBandwidth(1))
	{
		m_strMsg = "Fail to set resolution bandwidth in CWireless_CMU_TxPower ";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}


	if (! m_pITesterDevice->RF_SetLevelMode(m_strLevelMode.c_str()))
	{
		m_strMsg = "Fail to set level mode in CWireless_CMU_TxPower ";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDevice->RF_SetLevelMaximum(m_iLevelMax))
	{
		m_strMsg = "Fail to set level reference in CWireless_CMU_TxPower ";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}


	if (! m_pIPhone->WifiPowerOnTx(m_iRateBitIndex, m_iChannel, m_iPower, m_iWlanMode, m_iTxChain, m_iLoadBin))	
		
	{
		m_strMsg = "Fail to execute WifiPowerOnTx in CWireless_CMU_TxPower";
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
			m_strMsg = "Fail to fetch RF analyzer power in CWireless_CMU_TxPower";
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}

		if (strstr(strResponse.c_str(), "NAN") != NULL)
		{
			if (! m_pITesterDevice->RF_SetLevelMode(_T("MANual")))
			{
				m_strMsg = "Fail to set level mode";
				TraceLog(MSG_ERROR, m_strMsg);
				return false;
			}
			sprintf_s(szTmp, 255, "%0d - Cmu TxPower : %s", i + 1, strResponse.c_str());
			m_strMsg = szTmp;
			TraceLog(MSG_ERROR, m_strMsg);

			Sleep(1000);
			continue;
		}

		sprintf_s(szTmp, 255, "%0d - Cmu TxPower : %s", i + 1, strResponse.c_str());
		m_strMsg = szTmp;
		TraceLog(MSG_ERROR, m_strMsg);
		

		STDStringTokenizer(strResponse, ",", vPowerList);
		dPower = atof(vPowerList[5].c_str()) + m_dAdjPower;//	double m_dChannelBWMHz, m_dAdjPower;

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%0d - Cmu TxPower : %.3f", i + 1, dPower);
		m_strMsg = szTmp;
		TraceLog(MSG_INFO, m_strMsg);

		if (dPower > dMax)
			dMax = dPower;

		if (! InRange(m_dLower, m_dUpper, dMax))
		{
			if (i < m_iRetryTimes - 1)
			{
				m_strMsg = "Cmu TxPower not in range, try again";
				TraceLog(MSG_ERROR, m_strMsg);

			}
			else
			{
				m_strMsg = "Cmu TxPower not in range";
				TraceLog(MSG_ERROR, m_strMsg);
			}
		}
		else
		{
			bPass = true;
			break;
		}

		Sleep(1000);
	}

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", dMax);
	m_strMeasured = szTmp;

	gCpkRecord.assign ("CMU_2G4Power", m_strMeasured);//write TxPower to CPK

	if(!bPass)
	{
		m_strMsg = "Return false before TX off.in CWireless_CMU_TxPower";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	// Stop WiFi power
	if (!m_pIPhone->WifiPowerOffTx(m_iChannel))
	{
		m_strMsg = "Fail to stop WifiPowerOnTx in CWireless_CMU_TxPower";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}


	return bPass;
}


bool CWireless_CMU_TxPower::InitData(std::map<std::string, std::string>& paramMap)
{
	std::vector<std::string> vTmp;

	if (paramMap.find("PICSName") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PICSName for CWireless_CMU_TxPower");
		return false;
	}
	m_strPICSName = paramMap["PICSName"];

	if (paramMap.find("LineName") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter LineName for CWireless_CMU_TxPower");
		return false;
	}
	m_strLineName = paramMap["LineName"];


	if (paramMap.find("LoadBin") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter LoadBin for CWireless_CMU_TxPower");
		return false;
	}
	m_iLoadBin = atoi(paramMap["LoadBin"].c_str());

	if (paramMap.find("CommandDelay") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CommandDelay for CWireless_CMU_TxPower");
		return false;
	}
	m_iCommandDelay = atoi(paramMap["CommandDelay"].c_str());

	if (paramMap.find("StartDelay") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter StartDelay for CWireless_CMU_TxPower");
		return false;
	}
	m_iStartDelay = atoi(paramMap["StartDelay"].c_str());

	if (paramMap.find("Frequency") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Frequency for CWireless_CMU_TxPower");
		return false;
	}
	m_dFrequency = atof(paramMap["Frequency"].c_str());


	if (paramMap.find("Power") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Power for CWireless_CMU_TxPower");
		return false;
	}
	m_iPower = atoi(paramMap["Power"].c_str());

	if (paramMap.find("TxBandwidth") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Channel for CWireless_LTE_Band_TxBandwidth");
		return false;
	}
	m_iTxBandwidth = atoi(paramMap["TxBandwidth"].c_str());


	if (paramMap.find("Channel") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Channel for CWireless_CMU_TxPower");
		return false;
	}
	m_iChannel = atoi(paramMap["Channel"].c_str());
	m_strChannel = paramMap["Channel"].c_str();


	if (paramMap.find("RetryTimes") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RetryTimes for CWireless_CMU_TxPower");
		return false;
	}
	m_iRetryTimes = atoi(paramMap["RetryTimes"].c_str());

	if (paramMap.find("InputPort") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter InputPort for CWireless_CMU_TxPower");
		return false;
	}
	m_iInputPort = atoi(paramMap["InputPort"].c_str());

	if (paramMap.find("Range") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CWireless_CMU_TxPower");
		return false;
	}
	STDStringTokenizer(paramMap["Range"], ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CWireless_CMU_TxPower");
		return false;
	}
	m_dLower = atof(vTmp[0].c_str());
	m_dUpper = atof(vTmp[1].c_str());


	if (paramMap.find("LevelMode") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter LevelMode for CWireless_CMU_TxPower");
		return false;
	}
	m_strLevelMode = paramMap["LevelMode"];

	if (paramMap.find("LevelMax") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter LevelMax for CWireless_CMU_TxPower");
		return false;
	}
	m_iLevelMax = atoi(paramMap["LevelMax"].c_str());


	if (paramMap.find("RateBitIndex") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RateBitIndex for CWireless_CMU_TxPower");
		return false;
	}
	m_iRateBitIndex = atoi(paramMap["RateBitIndex"].c_str());

	if (paramMap.find("WlanMode") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter WlanMode for CWireless_CMU_TxPower");
		return false;
	}
	m_iWlanMode = atoi(paramMap["WlanMode"].c_str());

	if (paramMap.find("TxChain") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter TxChain for CWireless_CMU_TxPower");
		return false;
	}
	m_iTxChain = atoi(paramMap["TxChain"].c_str());


	if (paramMap.find ("WriteCPKLog") != paramMap.end())
		m_WriteCPKLog = paramMap["WriteCPKLog"] != "0";

	ParseCPKItems();

	return true;
}




bool CWireless_CMU_TxPower::ParseCPKItems()
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