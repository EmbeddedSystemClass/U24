#include "stdafx.h"
#include "Wireless_WCDMA_Rssi.h"


// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CWireless_WCDMA_Rssi)

CWireless_WCDMA_Rssi::CWireless_WCDMA_Rssi()
	:m_WriteCPKLog(false)
{
}

bool CWireless_WCDMA_Rssi::Run()
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
	strcpy(szBand, "2100_B1");

	char szChannel[256];
	memset(szChannel, 0, 256);
	sprintf_s(szChannel, 255, "%.1f", m_dFreq);

	// Prepare CPK log
	std::string cpkfile = string(DEFAULT_CPKLOG_PATH) + "Wireless_WCDMA_CPK";
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
		FactoryLog(true, "WCDMA_Rssi", "", szBand, szChannel, szLower, szUpper, m_strMeasured, "dBm", "Wireless 2100 Band1 Rssi PASS");
		SetPICSDate(m_strPICSName, m_strMeasured);
		gCpkRecord.assign ("2100_B1_RX_VALUE", m_strMeasured); //write 2100_B1_RX_VALUE to CPK
	}
	else
	{
		FactoryLog(false, "WCDMA_Rssi", FunErr_WirelessWCDMA_Online_RSCP_OutRange, szBand, szChannel, szLower, szUpper, m_strMeasured, "dBm", m_strMsg);
		SetPICSDate(m_strPICSName, m_strMeasured);
		gCpkRecord.assign ("2100_B1_RX_VALUE", m_strMeasured); //write 2100_B1_RX_VALUE to CPK
	}

	// write result to log file
	TraceLog (MSG_INFO, "Wireless_WCDMA_HEADER: " + gCpkRecord.GetLogHeader());
	TraceLog (MSG_INFO, "Wireless_WCDMA_RESULT: " + gCpkRecord.GetLogString());

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

bool CWireless_WCDMA_Rssi::MainFunction()
{
	char szTmp[256];
	bool bPass = false;

	// Initial tester device
	if (! m_pITesterDevice->Initial())
	{
		m_strMsg = "Fail to initial tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	//
	if (! m_pITesterDevice->Initial_RF_Nonsignaling())
	{
		m_strMsg = "Fail to initial tester device : RF non-signaling";
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


	if (! m_pITesterDevice->SetOutputLoss (1, m_iOutputPort, GetLossByDegree(1)))
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

	Sleep (m_dwDelayAfterCMURun);

	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to connect phone with Qisda module";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	
	double dRssi = 0;
	unsigned short iRX_AGC = 0;
    unsigned short iRX_Power_dBm = 0;
    unsigned short iFTM_Error_Code = 0;
	unsigned short iChannel = 9750;

	for (int i = 0; i < m_iRetryTimes; ++i)
	{
		// QLIB get Rssi
		if (! m_pIPhone->WCDMA_Rssi(iChannel, &iRX_AGC, &iRX_Power_dBm, &iFTM_Error_Code) )
		{
			m_strMsg = "Rerty to get WirelessUMTSBand1 Rssi";
			TraceLog(MSG_ERROR, m_strMsg);
			continue;
		}

		dRssi = (double)((iRX_Power_dBm - 65536) / 10.0);
		sprintf_s (szTmp, 255, "%0d - 2100 B1 Rssi = %.2f", (i+1), dRssi);
		m_strMsg = szTmp;
		TraceLog(MSG_INFO, m_strMsg);

		memset(szTmp, 0, 256);
		sprintf_s (szTmp, 255, "%.2f", dRssi);
		m_strMeasured = szTmp;

		if (InRange (m_dLower, m_dUpper, dRssi))
		{
			bPass = true;
			break;
		}
		else
		{
			if (i < m_iRetryTimes - 1)
			{
				m_strMsg = "2100 B1 Rssi not in range, try again";
				TraceLog(MSG_ERROR, m_strMsg);
				Sleep (m_dwCommandDelay);
			}
			else
			{
				m_strMsg = "2100 B1 Rssi not in range";
				TraceLog(MSG_ERROR, m_strMsg);
			}
		}		
	}

	if (! m_pITesterDevice->RF_TurnOnOffGernerator (false))
	{
        m_strMsg = "Fail to RF_TurnOnOffGernerator(false)" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
	}

	return bPass;
}

double CWireless_WCDMA_Rssi::GetLossByDegree(int iDegree)
{
	double dLoss = 0.0;

	switch (iDegree)
	{
	case 0:
		dLoss = GetCableLoss("RX", "WirelessUMTSBand1", "low");
		break;
	case 1:
		dLoss = GetCableLoss("RX", "WirelessUMTSBand1", "mid");
		break;
	default:
		dLoss = GetCableLoss("RX", "WirelessUMTSBand1", "high");
	}

	return dLoss;
}


bool CWireless_WCDMA_Rssi::InitData(std::map<std::string, std::string>& paramMap)
{
	std::vector<std::string> vTmp;

	if (paramMap.find("PICSName") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PICSName for CWireless_WCDMA_Rssi");
		return false;
	}
	m_strPICSName = paramMap["PICSName"];

	if (paramMap.find("RetryTimes") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RetryTimes for CWireless_WCDMA_Rssi");
		return false;
	}
	m_iRetryTimes = atoi(paramMap["RetryTimes"].c_str());

	if (paramMap.find("OutputPort") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter OutputPort for CWireless_WCDMA_Rssi");
		return false;
	}
	m_iOutputPort = atoi(paramMap["OutputPort"].c_str());


	if (paramMap.find("Frequency") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Frequency for CWireless_WCDMA_Rssi");
		return false;
	}
	m_dFreq = atof(paramMap["Frequency"].c_str());


	if (paramMap.find("RFLevel") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RFLevel for CWireless_WCDMA_Rssi");
		return false;
	}
	m_iRFLevel = atoi(paramMap["RFLevel"].c_str());


	if (paramMap.find("DelayAfterCMURun") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter DelayAfterCMURun for CWireless_WCDMA_Rssi");
		return false;
	}
	m_dwDelayAfterCMURun = atoi(paramMap["DelayAfterCMURun"].c_str());


	if (paramMap.find("CommandDelay") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CommandDelay for CWireless_WCDMA_Rssi");
		return false;
	}
	m_dwCommandDelay = atoi(paramMap["CommandDelay"].c_str());


	if (paramMap.find("Range") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CWireless_WCDMA_Rssi");
		return false;
	}
	STDStringTokenizer(paramMap["Range"], ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CWireless_WCDMA_Rssi");
		return false;
	}
	m_dLower = atof(vTmp[0].c_str());
	m_dUpper = atof(vTmp[1].c_str());

	if (paramMap.find ("WriteCPKLog") != paramMap.end())
		m_WriteCPKLog = paramMap["WriteCPKLog"] != "0";

	ParseCPKItems();

	return true;
}

bool CWireless_WCDMA_Rssi::ParseCPKItems()
{
	m_CPKHeaderList.clear();

	if (m_TestItemXML.Load(m_cstrTestItemFile) != ERROR_SUCCESS)
		return false;

	XMLNode processObjectSet = m_TestItemXML.SearchNode(_T("//Configuration//ProcessObjectSet//WirelessTestObjects//WcdmaCPKcolumn"));
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