#include "StdAfx.h"
#include "BTWLAN_2_4G_TxTest.h"

// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CBTWLAN_2G4_Test)

CBTWLAN_2G4_Test::CBTWLAN_2G4_Test(void)
	:m_WriteCPKLog(false)
	,m_bKLossMode(false)
{
}

CBTWLAN_2G4_Test::~CBTWLAN_2G4_Test(void)
{
}

bool CBTWLAN_2G4_Test::InitData(std::map<std::string, std::string>& paramMap)
{
	std::vector<std::string> vTmp;

	if (paramMap.find("PICSName") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PICSName for CBTWLAN_2G4_Test");
		return false;
	}
	m_strPICSName = paramMap["PICSName"];
	m_strPICSName_Value = m_strPICSName + "_Value";

	if (paramMap.find("CommandDelay") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CommandDelay for CBTWLAN_2G4_Test");
		return false;
	}
	m_iCommandDelay = atoi(paramMap["CommandDelay"].c_str());

	if (paramMap.find("Frequency") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Frequency for CBTWLAN_2G4_Test");
		return false;
	}
	m_dFrequency = atof(paramMap["Frequency"].c_str());
	m_strChannel = paramMap["Frequency"].c_str();

	if (paramMap.find("Input") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Input for CBTWLAN_2G4_Test");
		return false;
	}
	m_strInput = paramMap["Input"].c_str();

	if (paramMap.find("RBW") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RBW for CBTWLAN_2G4_Test");
		return false;
	}
	m_dRBW = atof(paramMap["RBW"].c_str());

	if (paramMap.find("VBW") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter VBW for CBTWLAN_2G4_Test");
		return false;
	}
	m_dVBW = atof(paramMap["VBW"].c_str());

	if (paramMap.find("Span") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Span for CBTWLAN_2G4_Test");
		return false;
	}
	m_dSpan = atof(paramMap["Span"].c_str());

	if (paramMap.find("TraceMode") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter TraceMode for CBTWLAN_2G4_Test");
		return false;
	}
	m_strTraceMode = paramMap["TraceMode"];

	if (paramMap.find("Detector") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Detector for CBTWLAN_2G4_Test");
		return false;
	}
	m_strDetector = paramMap["Detector"];

	if (paramMap.find("DiagramFull") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter DiagramFull for CBTWLAN_2G4_Test");
		return false;
	}
	m_strDiagramFull = paramMap["DiagramFull"];

	if (paramMap.find("AverageTimes") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter AverageTimes for CBTWLAN_2G4_Test");
		return false;
	}
	m_iAverageTimes = atoi(paramMap["AverageTimes"].c_str());

	if (paramMap.find("SweepTime") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter SweepTime for CBTWLAN_2G4_Test");
		return false;
	}
	m_iSweepTime = atoi(paramMap["SweepTime"].c_str());

	if (paramMap.find("ChannelPower") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter ChannelPower for CBTWLAN_2G4_Test");
		return false;
	}
	m_strChannelPower = paramMap["ChannelPower"];

	if (paramMap.find("ChannelBW") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter ChannelBW for CBTWLAN_2G4_Test");
		return false;
	}
	m_dChannelBWMHz = atof(paramMap["ChannelBW"].c_str());

	if (paramMap.find("AttenuationManual") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter AttenuationManual for CBTWLAN_2G4_Test");
		return false;
	}
	m_iAttenuationManual = atoi(paramMap["AttenuationManual"].c_str());

	if (paramMap.find("RefLevel") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RefLevel for CBTWLAN_2G4_Test");
		return false;
	}
	m_iRefLevel = atoi(paramMap["RefLevel"].c_str());

	if (paramMap.find("Range") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CBTWLAN_2G4_Test");
		return false;
	}
	STDStringTokenizer(paramMap["Range"], ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CBTWLAN_2G4_Test");
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

bool CBTWLAN_2G4_Test::Run(void)
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
	strcpy(szBand, "2G4");

	// Prepare CPK log
	std::string cpkfile = string(DEFAULT_CPKLOG_PATH) + "Nordic2G4_CPK";
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
		FactoryLog(true, "2G4_Power_Test", "Pass", szBand, m_strChannel, szLower, szUpper, m_strMeasured, "dBm", "2G4_Power_Test PASS");
		SetPICSData(m_strPICSName, "PASS");
	}
	else
	{
		FactoryLog(false, "2G4_Power_Test", FunErr_2G4_Power_Test_Fail, szBand, m_strChannel, szLower, szUpper, m_strMeasured, "dBm", m_strMsg);
		SetPICSData(m_strPICSName, "FAIL");
	}
	SetPICSData(m_strPICSName_Value, m_strMeasured);

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

	return bRes;
}

bool CBTWLAN_2G4_Test::MainFunction(void)
{
	if (!m_pIPhone->Nordic2G4_Consumer("1"))
	{
		m_strMsg = "Fail to set 2G4 consumer on";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (!m_pIPhone->Nordic2G4_PowerMode("1"))
	{
		m_strMsg = "Fail to set 2G4 power up";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	char szInput[128] = {0};
	strcpy(szInput, m_strInput.c_str());
	if (!m_pIPhone->Nordic2G4_PowerOn(szInput))
	{
		m_strMsg = "Fail to start 2G4 power";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	// Initial tester device
	if (!m_pITesterDeviceFSP->Initial())
	{
		m_strMsg = "Fail to initial tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	
	if (!m_pITesterDeviceFSP->SetResolutionBandwidth(m_dRBW))
	{
		m_strMsg = "Fail to set resolution bandwidth to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (!m_pITesterDeviceFSP->SetVideoBandwidth(m_dVBW))
	{
		m_strMsg = "Fail to set video bandwidth to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (!m_pITesterDeviceFSP->RF_SetAnalyzerFreq(m_dFrequency))
	{
		m_strMsg = "Fail to set RF analyzer frequency to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (!m_pITesterDeviceFSP->SetSweepTime(m_iSweepTime))
	{
		m_strMsg = "Fail to set sweep time to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (!m_pITesterDeviceFSP->SetFrequencySpan(m_dSpan))
	{
		m_strMsg = "Fail to set frequency span to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	//if (!m_pITesterDeviceFSP->SetInputLoss(GetCableLoss("TX", "Nordic2G4", "Nordic2G4_Power")))
	//{
	//	m_strMsg = "Fail to set input loss to tester device";
	//	TraceLog(MSG_ERROR, m_strMsg);
	//	return false;
	//}

	if (!m_pITesterDeviceFSP->SwitchTraceMode(m_strTraceMode))
	{
		m_strMsg = "Fail to switch trace mode to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (!m_pITesterDeviceFSP->SetDetector(m_strDetector))
	{
		m_strMsg = "Fail to set detector to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (!m_pITesterDeviceFSP->SetAttRefLevel(m_iAttenuationManual, m_iRefLevel))
	{
		m_strMsg = "Fail to set reference level to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	Sleep(2000);

	if (!m_pITesterDeviceFSP->SetMaxLevel())
	{
		m_strMsg = "Fail to set max level to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (!m_pITesterDeviceFSP->SwitchChannelPower(m_strChannelPower))
	{
		m_strMsg = "Fail to switch channel power to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (!m_pITesterDeviceFSP->SetChannelBandwidth(m_dChannelBWMHz))
	{
		m_strMsg = "Fail to set channel bandwidth to tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	Sleep(m_iCommandDelay);

	bool bRes = false;
	double dPower;
	std::string strResponse;
	std::vector<std::string> vPowerList;

	double dTotal = 0.0;

	char szTmp[256];

	for (int i = 0; i < m_iAverageTimes; ++i)
	{
		if (!m_pITesterDeviceFSP->RF_NPowerMeasurement(0, strResponse))
		{
			m_strMsg = "Fail to fetch RF analyzer power";
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}

		if (strstr(strResponse.c_str(), "NAN") != NULL)
		{
			sprintf_s(szTmp, 255, "%0d - 2G4 power : NAN", i + 1);
			m_strMsg = szTmp;
			TraceLog(MSG_ERROR, m_strMsg);
			continue;
		}

		m_dAdjPower = GetCableLoss("TX", "Nordic2G4", "Nordic2G4_Power");
		dPower = (atof(strResponse.c_str()) + m_dAdjPower);

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%0d - 2G4 power : %.3f", i + 1, dPower);
		m_strMsg = szTmp;
		TraceLog(MSG_INFO, m_strMsg);

		dTotal += dPower;
		
	}

	dPower = (dTotal/m_iAverageTimes);
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", dPower);
	m_strMeasured = szTmp;
	TraceLog(MSG_INFO, m_strMeasured);

	if (!InRange(m_dLower, m_dUpper, dPower))
	{
		m_strMsg = "2G4 power not in range";
		TraceLog(MSG_ERROR, m_strMsg);
		bRes = false;
	}
	else
	{
		bRes = true;
		//break;
		if (m_bKLossMode)
		{
			/*double dLoss = GetRefResult("TX", "BTWLAN_BT", "BTResult") - dPower;
			memset(szTmp, 0, 256);
			sprintf_s(szTmp, 255, "%.1f", dLoss);
			UpdateCableLoss("TX", "BTWLAN_BT", "BTFixFreq", szTmp);*/
		}
	}

	gCpkRecord.assign ("2G4Power", m_strMeasured);//write TxPower to CPK

	// Stop 2G4 power
	if (!m_pIPhone->Nordic2G4_PowerMode("0"))
	{
		m_strMsg = "Fail to set 2G4 power down";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (!m_pIPhone->Nordic2G4_Consumer("0"))
	{
		m_strMsg = "Fail to set 2G4 consumer off";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	return bRes;
}

bool CBTWLAN_2G4_Test::ParseCPKItems()
{
	m_CPKHeaderList.clear();

	if (m_TestItemXML.Load(m_cstrTestItemFile) != ERROR_SUCCESS)
		return false;

	XMLNode processObjectSet = m_TestItemXML.SearchNode(_T("//Configuration//ProcessObjectSet//CommonTestObjects//Nordic2G4CPKcolumn"));
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