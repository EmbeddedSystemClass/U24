#include "stdafx.h"
#include "Wireless_GPS_CN0Test.h"


// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CWireless_GPS_CN0Test)

CWireless_GPS_CN0Test::CWireless_GPS_CN0Test()
	:m_WriteCPKLog(false)
	,m_bKLossMode(false)
{
}

bool CWireless_GPS_CN0Test::Run()
{
	m_strMeasured = "";

	char szLower[256];
	memset(szLower, 0, 256);
	sprintf_s(szLower, 255, "%.3f", m_dCN0Lower);

	char szUpper[256];
	memset(szUpper, 0, 256);
	sprintf_s(szUpper, 255, "%.3f", m_dCN0Upper);

	char szChannel[256];
	memset(szChannel, 0, 256);
	sprintf_s(szChannel, 255, "%.2f", m_dFreq);

	// Prepare CPK log
	std::string cpkfile = string(DEFAULT_CPKLOG_PATH) + "GpsCPK";
	gCpkRecord.setFileName (cpkfile.c_str());
	gCpkRecord.ResetAll();

	int count = m_CPKHeaderList.size();
	for (int i=0; i < count; ++i)
	{
		gCpkRecord.setHeader (i, m_CPKHeaderList[i].c_str());
	}

	bool bRes = MainFunction();

	SetPICSData(_T("Wireless_GPS_CN0Test_RrequencyOffset"), strFreqOffset);

	if (bRes)
	{
		FactoryLog(true, "GPS_CN0Test", "Pass", "--", szChannel, szLower, szUpper, m_strMeasured, "", "GPS CN0/FreqOffset PASS");
		SetPICSData(m_strPICSName, "PASS");
		if (0 == strcmp(m_strPICSName.c_str(), "CheckCalibrationCable"))
		{
			MessageBox(NULL, "Please switch cable to FSEA.\nPress OK while you are ready.", "Info.", MB_OK|MB_ICONINFORMATION);
		}
	}
	else
	{
		FactoryLog(false, "GPS_CN0Test", FunErr_GPS_Test_CN0_OutRange, "--", szChannel, szLower, szUpper, m_strMeasured, "", m_strMsg);
		SetPICSData(m_strPICSName, "FAIL");
	}
	SetPICSData(m_strPICSName_Value, m_strMeasured);

	// write result to log file
	TraceLog (MSG_INFO, "GpsCPK_HEADER: " + gCpkRecord.GetLogHeader());
	TraceLog (MSG_INFO, "GpsCPK_RESULT: " + gCpkRecord.GetLogString());

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

bool CWireless_GPS_CN0Test::MainFunction()
{
	bool bPass = false;
	int iCN0 = 0;
	double dCN0 = 0.0;
	int iFreq = 0;
	std::string strCN0;


	char szLower[256] = {0};
	sprintf_s(szLower, 255, "%.3f", m_dCN0Lower);

	char szUpper[256] = {0};
	sprintf_s(szUpper, 255, "%.3f", m_dCN0Upper);

	char szChannel[256] = {0};
	sprintf_s(szChannel, 255, "%.2f", m_dFreq);


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

	if (! m_pITesterDevice->SetOutputLoss (1, m_iOutputPort, GetCableLoss("RX", "GPS", "GPSFixFreq")))
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

	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to connect phone with Qisda module";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	// Enter GPS
	if (! m_pIPhone->GPS_EnterGPS(true))
	{
		m_strMsg = "EnterGPS Fail with Qisda module";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	Sleep (m_dwDelayAfterCMURun);

	for (int i = 0; i < m_iRetryTimes; ++i)
	{
		if (! m_pIPhone->GPS_GetCN0value(dCN0, iFreq, 1/*this->CollectMode*/, 32/*this->CapSize*/, 10/*this->Intergrations*/))
		{
			m_strMsg = "Fail to fetch CN0 value with Qisda module";
			TraceLog(MSG_ERROR, m_strMsg);
			continue;
		}

		strCN0 = MakeSTDstring ("%.3f", dCN0);
		strFreqOffset = MakeSTDstring ("%d", iFreq);
		m_strMeasured = strCN0 + _T(",") + strFreqOffset;

		if (InRange(m_dCN0Lower, m_dCN0Upper, dCN0) && InRange(m_iFreqLower, m_iFreqUpper, iFreq))
		{
			bPass = true;
			if (m_bKLossMode)
			{
				char szTmp[256];
				double dLoss = GetRefResult("RX", "GPS", "GPSResult") - dCN0;
				memset(szTmp, 0, 256);
				sprintf_s(szTmp, 255, "%.1f", dLoss);
				UpdateCableLoss("RX", "GPS", "GPSFixFreq", szTmp);
			}
			break;
		}

		Sleep (m_dwCommandDelay);
	}

	gCpkRecord.assign ("GPS_CN0", strCN0);//write GPS_CN0 to CPK
	gCpkRecord.assign ("GPS_FreqOffset", strFreqOffset);//write GPS_FreqOffset to CPK

	if (! m_pIPhone->GPS_EnterGPS(false))
	{
		m_strMsg = "ExitGPS Fail with Qisda module";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDevice->RF_TurnOnOffGernerator (false))
	{
        m_strMsg = "Fail to RF_TurnOnOffGernerator(false)" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
	}

	return bPass;
}


bool CWireless_GPS_CN0Test::InitData(std::map<std::string, std::string>& paramMap)
{
	std::vector<std::string> vTmp;

	if (paramMap.find("PICSName") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PICSName for CWireless_GPS_CN0Test");
		return false;
	}
	m_strPICSName = paramMap["PICSName"];
	m_strPICSName_Value = m_strPICSName + "_Value";

	if (paramMap.find("RetryTimes") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RetryTimes for CWireless_GPS_CN0Test");
		return false;
	}
	m_iRetryTimes = atoi(paramMap["RetryTimes"].c_str());

	if (paramMap.find("CommandDelay") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CommandDelay for CWireless_GPS_CN0Test");
		return false;
	}
	m_dwCommandDelay = atoi(paramMap["CommandDelay"].c_str());

	if (paramMap.find("Frequency") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Frequency for CWireless_GPS_CN0Test");
		return false;
	}
	m_dFreq = atof(paramMap["Frequency"].c_str());

	if (paramMap.find("RFLevel") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RFLevel for CWireless_GPS_CN0Test");
		return false;
	}
	m_iRFLevel = atoi(paramMap["RFLevel"].c_str());

	if (paramMap.find("OutputPort") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter OutputPort for CWireless_GPS_CN0Test");
		return false;
	}
	m_iOutputPort = atoi(paramMap["OutputPort"].c_str());

	if (paramMap.find("DelayAfterCMURun") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter DelayAfterCMURun for CWireless_GPS_CN0Test");
		return false;
	}
	m_dwDelayAfterCMURun = atoi(paramMap["DelayAfterCMURun"].c_str());

	if (paramMap.find("Range") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CWireless_GPS_CN0Test");
		return false;
	}
	STDStringTokenizer(paramMap["Range"], ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CWireless_GPS_CN0Test");
		return false;
	}
	m_dCN0Lower = atof(vTmp[0].c_str());
	m_dCN0Upper = atof(vTmp[1].c_str());

	if (paramMap.find("FreqRange") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CWireless_GPS_CN0Test");
		return false;
	}
	STDStringTokenizer(paramMap["FreqRange"], ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CWireless_GPS_CN0Test");
		return false;
	}
	m_iFreqLower = atoi(vTmp[0].c_str());
	m_iFreqUpper = atoi(vTmp[1].c_str());

	if (paramMap.find ("WriteCPKLog") != paramMap.end())
		m_WriteCPKLog = paramMap["WriteCPKLog"] != "0";

	if (paramMap.find ("KLossMode") != paramMap.end())
		m_bKLossMode = paramMap["KLossMode"] != "0";

	ParseCPKItems();

	return true;
}

bool CWireless_GPS_CN0Test::ParseCPKItems()
{
	m_CPKHeaderList.clear();

	if (m_TestItemXML.Load(m_cstrTestItemFile) != ERROR_SUCCESS)
		return false;

	XMLNode processObjectSet = m_TestItemXML.SearchNode(_T("//Configuration//ProcessObjectSet//CommonTestObjects//GpsCPKcolumn"));
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
double CWireless_GPS_CN0Test::GetLossByDegree(int iDegree)
{
	double dLoss = 0.0;

	switch (iDegree)
	{
		case 0:
			dLoss = GetCableLoss("RX", "GPS_CN0", "low");
			break;
		case 1:
			dLoss = GetCableLoss("RX", "GPS_CN0", "mid");
			break;
		default:
			dLoss = GetCableLoss("RX", "GPS_CN0", "high");
	}
	
	return dLoss;
}
