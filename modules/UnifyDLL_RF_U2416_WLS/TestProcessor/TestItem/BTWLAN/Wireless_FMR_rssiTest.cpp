#include "StdAfx.h"
#include "Wireless_FMR_rssiTest.h"

// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CWireless_FMR_rssiTest)

CWireless_FMR_rssiTest::CWireless_FMR_rssiTest(void)
	:m_WriteCPKLog(false)
{
}

CWireless_FMR_rssiTest::~CWireless_FMR_rssiTest(void)
{
}

bool CWireless_FMR_rssiTest::InitData(std::map<std::string, std::string>& paramMap)
{
	std::vector<std::string> vTmp;

	if (paramMap.find("PICSName") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PICSName for CWireless_FMR_rssiTest");
		return false;
	}
	m_strPICSName = paramMap["PICSName"];
	m_strPICSName_Value = m_strPICSName + "_Value";

	if (paramMap.find("RetryTimes") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RetryTimes for CWireless_FMR_rssiTest");
		return false;
	}
	m_iRetryTimes = atoi(paramMap["RetryTimes"].c_str());

	if (paramMap.find("CommandDelay") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CommandDelay for CWireless_FMR_rssiTest");
		return false;
	}
	m_dwCommandDelay = atoi(paramMap["CommandDelay"].c_str());

	if (paramMap.find("Frequency") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Frequency for CWireless_FMR_rssiTest");
		return false;
	}
	m_dFreq = atof(paramMap["Frequency"].c_str());

	if (paramMap.find("RFLevel") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RFLevel for CWireless_FMR_rssiTest");
		return false;
	}
	m_iRFLevel = atoi(paramMap["RFLevel"].c_str());

	if (paramMap.find("OutputPort") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter OutputPort for CWireless_FMR_rssiTest");
		return false;
	}
	m_iOutputPort = atoi(paramMap["OutputPort"].c_str());

	if (paramMap.find("DelayAfterCMURun") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter DelayAfterCMURun for CWireless_FMR_rssiTest");
		return false;
	}
	m_dwDelayAfterCMURun = atoi(paramMap["DelayAfterCMURun"].c_str());

	if (paramMap.find("Range") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CWireless_FMR_rssiTest");
		return false;
	}
	STDStringTokenizer(paramMap["Range"], ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CWireless_FMR_rssiTest");
		return false;
	}
	m_iLower = atoi(vTmp[0].c_str());
	m_iUpper = atoi(vTmp[1].c_str());

	if (paramMap.find ("WriteCPKLog") != paramMap.end())
		m_WriteCPKLog = paramMap["WriteCPKLog"] != "0";

	ParseCPKItems();

	return true;
}

bool CWireless_FMR_rssiTest::Run(void)
{
	m_strMeasured = "";

	char szLower[256];
	memset(szLower, 0, 256);
	sprintf_s(szLower, 255, "%d", m_iLower);

	char szUpper[256];
	memset(szUpper, 0, 256);
	sprintf_s(szUpper, 255, "%d", m_iUpper);

	char szChannel[256];
	memset(szChannel, 0, 256);
	sprintf_s(szChannel, 255, "%.2f", m_dFreq);

	// Prepare CPK log
	std::string cpkfile = string(DEFAULT_CPKLOG_PATH) + "FmrCPK";
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
		FactoryLog(true, "FMR_rssiTest", "Pass", "--", szChannel, szLower, szUpper, m_strMeasured, "", "FMR rssiTest PASS");
		SetPICSData(m_strPICSName, "PASS");
	}
	else
	{
		FactoryLog(false, "FMR_rssiTest", FunErr_FM_Test_Fail, "--", szChannel, szLower, szUpper, m_strMeasured, "", m_strMsg);
		SetPICSData(m_strPICSName, "FAIL");
	}
	SetPICSData(m_strPICSName_Value, m_strMeasured);

	// write result to log file
	TraceLog (MSG_INFO, "FmrCPK_HEADER: " + gCpkRecord.GetLogHeader());
	TraceLog (MSG_INFO, "FmrCPK_RESULT: " + gCpkRecord.GetLogString());

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

bool CWireless_FMR_rssiTest::MainFunction(void)
{
	bool bPass = false;

	// Initial tester device
	if (! m_pITesterDevice->Initial())
	{
		m_strMsg = "Fail to initial tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDevice->Initial_RF_Nonsignaling())
	{
		m_strMsg = "Fail to initial tester device : Initial_GPS";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	m_pITesterDevice->SetMode(1);
	
	if (! m_pITesterDevice->SetOutputPort(1, m_iOutputPort))
	{
		m_strMsg = "Fail to set RF output port";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDevice->SetOutputLoss (1, m_iOutputPort, GetCableLoss("RX", "FMR", "FRMFixFreq")))
    {
        m_strMsg = "Fail to set RX cable loss" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
    }

	if (! m_pITesterDevice->RF_SetGeneratorFreq (m_dFreq))
	{
        m_strMsg = "Fail to RF_SetGeneratorFreq" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
	}

	if (! m_pITesterDevice->RF_SetModulationMode ("off"))
	{
        m_strMsg = "Fail to RF_SetModulationMode" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
	}

	if (! m_pITesterDevice->RF_SetGeneratorPowerLevel (m_iRFLevel))
	{
        m_strMsg = "Fail to RF_SetGeneratorPowerLevel" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
	}


	if (! m_pITesterDevice->RF_TurnOnOffGernerator (true))
	{
        m_strMsg = "Fail to RF_TurnOnOffGernerator" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
	}

	if (! m_pIPhone->Initial_FTD())
	{
		m_strMsg = "Fail to initial phone connection with FTD DLL";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	Sleep (m_dwDelayAfterCMURun);
	//Sleep (m_dwCommandDelay);

	char sz_Input[FTD_BUF_SIZE] = {0};
	char sz_Output[FTD_BUF_SIZE] = {0};
	int iInput = (int)(1000 * m_dFreq);
	sprintf_s(sz_Input, "%d", iInput);

	for (int i = 0; i < m_iRetryTimes; ++i)
	{
		/* read FMRrssi from mobile */
		if (! (bPass = m_pIPhone->FMR_Rssi(sz_Input, sz_Output)))
		{
			m_strMsg = "Read FMRrssi from mobile error";
			TraceLog(MSG_ERROR, m_strMsg);
			continue;
		}

		/* Compare FMRrssi with config file */
		m_strMeasured = sz_Output;

		int iValue = atoi(sz_Output);
		if( InRange(m_iLower, m_iUpper, iValue) )
		{
			m_strMsg = "Check FMRrssi success!";
			TraceLog(MSG_ERROR, m_strMsg);
			bPass = true;
			break;
		}
		else
		{
			m_strMsg = "FMRrssi is not matched.";
			TraceLog(MSG_ERROR, m_strMsg);
			bPass = false;
		}
	}

	gCpkRecord.assign ("FMR_RSSI", m_strMeasured);//write FMRrssi to CPK

	if (! m_pITesterDevice->RF_TurnOnOffGernerator (false))
	{
        m_strMsg = "Fail to RF_TurnOnOffGernerator(false)" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
	}

	return bPass;
}

bool CWireless_FMR_rssiTest::ParseCPKItems()
{
	m_CPKHeaderList.clear();

	if (m_TestItemXML.Load(m_cstrTestItemFile) != ERROR_SUCCESS)
		return false;

	XMLNode processObjectSet = m_TestItemXML.SearchNode(_T("//Configuration//ProcessObjectSet//CommonTestObjects//FmrCPKcolumn"));
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