#include "stdafx.h"
#include "Wireless_GSM_RxLevel.h"


// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CWireless_GSM_RxLevel)

CWireless_GSM_RxLevel::CWireless_GSM_RxLevel()
{
}

bool CWireless_GSM_RxLevel::Run()
{
	//m_strMeasured = "";
	//char szLower[256];
	//memset(szLower, 0, 256);
	//sprintf_s(szLower, 255, "%.3f", m_dLower);
	//char szUpper[256];
	//memset(szUpper, 0, 256);
	//sprintf_s(szUpper, 255, "%.3f", m_dUpper);
	//bool bRes = MainFunction();
	//if (bRes)
	//{
	//	FactoryLog(true, "GSMRxLevel", "Pass", "", "", szLower, szUpper, m_strMeasured, "", "Wireless GSMRx Level PASS");
	//}
	//else
	//{
	//	FactoryLog(false, "GSMRxLevel", "G01MH09_GSMTest_Rx_Lev_Fail", "", "", szLower, szUpper, m_strMeasured, "", m_strMsg);
	//}
	//SetPICSData(m_strPICSName, m_strMeasured);
	//return bRes;

	m_strMeasured = "";

	char szLower[256];
	memset(szLower, 0, 256);
	sprintf_s(szLower, 255, "%.3f", m_dLower);

	char szUpper[256];
	memset(szUpper, 0, 256);
	sprintf_s(szUpper, 255, "%.3f", m_dUpper);

	char szBand[256];
	memset(szBand, 0, 256);
	
	sprintf_s(szBand, 255, "%d", m_iGSMMode );
	//	strcpy(szBand, "GSM");

	// Prepare CPK log //----------------------------------------------------------------------
	char szBandFunctionCPK[256];
	memset(szBandFunctionCPK, 0, 256);
	sprintf_s(szBandFunctionCPK, 255, "Wireless_GSM_%d_Rx_CPK", m_iGSMMode );
	char szBandFunctionCPKFile[512];
	memset(szBandFunctionCPKFile, 0, 512);
	sprintf_s(szBandFunctionCPKFile, 511, "%s%s", DEFAULT_CPKLOG_PATH , szBandFunctionCPK );
	gCpkRecord.setFileName (szBandFunctionCPKFile);
	gCpkRecord.ResetAll();
	int count = m_CPKHeaderList.size();
	for (int i = 0 ; i < count; ++i)
	{
		gCpkRecord.setHeader ( i , m_CPKHeaderList[i].c_str());
	}
	//---------------------------------------------------------------------------------------------
	char szBandFunction[256];
	memset(szBandFunction, 0, 256);
	sprintf_s(szBandFunction, 255, "GSM_%d_Rx", m_iGSMMode );
	//---------------------------------

	bool bRes = MainFunction();

	if (bRes)
	{
		char szTestSuccess[256];
		memset(szTestSuccess, 0, 256);
		sprintf_s(szTestSuccess, 255, "Wireless GSM_%d Rx PASS", m_iGSMMode );		

		FactoryLog(true, szBandFunction , "Pass", "", "", szLower, szUpper, m_strMeasured, "", "Wireless GSMRx Level PASS");
		//FactoryLog( true , szBandFunction , "Pass" , szBand , m_strFrequency , szLower , szUpper , m_strMeasured , "dBm", szTestSuccess );
		//	FactoryLog(true, "GSMRfTx", "Pass", szBand, m_strFrequency, szLower, szUpper, m_strMeasured, "dBm", "Wireless GSMRfTx power PASS");
	}
	else
	{

		char szErrorcode[256];
		memset(szErrorcode, 0, 256);
		std::string Errorcode;		

		if ( 0 == strcmp(szBand, "850"))
		{
			sprintf_s(szErrorcode, 255, "%s", "G01MH09_GSMTest_Rx_Lev_Fail" );
			Errorcode = szErrorcode;

			//Errorcode = "G01MH09_GSMTest_Rx_Lev_Fail";
		}
		else if ( 0 == strcmp(szBand, "900"))
		{
			sprintf_s(szErrorcode, 255, "%s", "G02MH09_GSMTest_Rx_Lev_Fail" );
			Errorcode = szErrorcode;

			//Errorcode = "G02MH09_GSMTest_Rx_Lev_Fail";
		}
		else if ( 0 == strcmp(szBand, "1800"))
		{
			sprintf_s(szErrorcode, 255, "%s", "G03MH09_GSMTest_Rx_Lev_Fail" );
			Errorcode = szErrorcode;
			//Errorcode = "G03MH09_GSMTest_Rx_Lev_Fail";
		}
		else if ( 0 == strcmp( szBand, "1900"))
		{
			sprintf_s(szErrorcode, 255, "%s", "G04MH09_GSMTest_Rx_Lev_Fail" );
			Errorcode = szErrorcode;
			//Errorcode = "G04MH09_GSMTest_Rx_Lev_Fail";
		}
		else
		{
			sprintf_s(szErrorcode, 255, "%s", "G04MH09_GSMTest_Rx_Lev_Fail" );
			Errorcode = szErrorcode;
			//Errorcode = "G04MH09_GSMTest_Rx_Lev_Fail";
		}	
		FactoryLog(false, szBandFunction, Errorcode , "", "", szLower, szUpper, m_strMeasured, "", m_strMsg);
		//FactoryLog(false, szBandFunction, "G01MH09_GSMTest_Rx_Lev_Fail", "", "", szLower, szUpper, m_strMeasured, "", m_strMsg);
		//FactoryLog(false, szBandFunction , FunErr_WirelessGSM_Test_OutPower_OutRange, szBand, m_strFrequency, szLower, szUpper, m_strMeasured, "dBm", m_strMsg);		
		//FactoryLog(false, "GSMRfTx", FunErr_WirelessGSM_Test_OutPower_OutRange, szBand, m_strFrequency, szLower, szUpper, m_strMeasured, "dBm", m_strMsg);
	}

	SetPICSData( m_strPICSName , m_strMeasured );

	char szCPKRecordValue[256];
	memset(szCPKRecordValue, 0, 256);
	sprintf_s(szCPKRecordValue, 255, "GSM_Rx_VALUE");
	gCpkRecord.assign ( szCPKRecordValue , m_strMeasured ); 
	gCpkRecord.assign ("SN", g_strPicasso);
	gCpkRecord.assign ("Result", ( bRes ? "PASS" : "FAIL"));
	SYSTEMTIME currTime;
	::GetLocalTime(&currTime);
	std::string strTime = MakeSTDstring ("%02d:%02d:%02d", currTime.wHour, currTime.wMinute, currTime.wSecond);
	gCpkRecord.assign ("Time", strTime);

	// write result to log file
	char szCPKRecordHEADERValue[512]; 
	memset(szCPKRecordHEADERValue, 0, 512);
	sprintf_s(szCPKRecordHEADERValue, 511, "Wireless_GSM_%d_Rx_HEADER: %s", m_iGSMMode , gCpkRecord.GetLogHeader().c_str() );

	char szCPKRecordRESULTValue[512]; 
	memset(szCPKRecordRESULTValue, 0, 512);
	sprintf_s(szCPKRecordRESULTValue, 511, "Wireless_GSM_%d_Rx_RESULT: %s", m_iGSMMode , gCpkRecord.GetLogString().c_str() );


	TraceLog ( MSG_INFO , szCPKRecordHEADERValue );
	TraceLog ( MSG_INFO , szCPKRecordRESULTValue );


	// write result to CPK log
	if ( m_WriteCPKLog )
	{
		gCpkRecord.WriteRecordToFile();
	}
	return bRes;

}

bool CWireless_GSM_RxLevel::MainFunction()
{
	// Initial tester device
	if (!m_pITesterDevice->Initial())
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
	
	if (!m_pITesterDevice->SetOutputPort(1, m_iOutputPort))
	{
		m_strMsg = "Fail to set RF output port";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (!m_pITesterDevice->SetOutputLoss(1, m_iOutputPort, GetLossByDegree(1)))
    {
        m_strMsg = "Fail to set RX cable loss" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
    }

	if (!m_pITesterDevice->RF_SetGeneratorFreq(m_dFreq))
	{
        m_strMsg = "Fail to RF_SetGeneratorFreq" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
	}

	if (!m_pITesterDevice->RF_SetModulationMode("DSB"))
	{
        m_strMsg = "Fail to RF_SetModulationMode" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
	}

	if (!m_pITesterDevice->RF_SetAMModulationFrequency(m_iAFFrequency))
	{
        m_strMsg = "Fail to RF_SetAMModulationFrequency" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
	}

	if (!m_pITesterDevice->RF_SetAMModulationRation(m_iModIndex))
	{
        m_strMsg = "Fail to RF_SetAMModulationRation" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
	}

	//if (!m_pITesterDevice->RF_SetGeneratorPowerLevel(-60))

	
	if (!m_pITesterDevice->RF_SetGeneratorPowerLevel(m_iRFLevel))
	{
        m_strMsg = "Fail to RF_SetGeneratorPowerLevel" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
	}

	if (!m_pITesterDevice->RF_TurnOnOffGernerator(true))
	{
        m_strMsg = "Fail to RF_TurnOnOffGernerator" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
	}

	Sleep(m_iDelayAfterCMURun);

	if (!m_pIPhone->QMSL_FTM_SET_CALIBRATION_STATE(CAL_STATE_INACTIVE))
	{
		m_strMsg = "Fail to set calibration state";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (!m_pIPhone->QMSL_FTM_SET_MODE(m_sGSMBand))	//PHONE_MODE_GSM_1900=12,			//!<' GSM 1900
	{
		Sleep(2500);
		if(!m_pIPhone->QMSL_FTM_SET_MODE(m_sGSMBand))
		{


			m_strMsg = "Fail to set GSM mode";
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}
	}
	if (! m_pIPhone->QMSL_FTM_SET_MODE_ID(FTM_MODE_ID_GSM)) // FTM_MODE_ID_GSM        = 2,    //!<' RF GSM Mode
	{
		m_strMsg = "Fail to set mode id";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (!m_pIPhone->QMSL_FTM_SET_CHAN(m_iChannel))
	{
		m_strMsg = "Fail to set channel";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	unsigned short iExpected_RX_Power_dbm16 = (unsigned short) m_iExpected_RX_Power_dbm16;
    unsigned short iNumber_of_Averages = (unsigned short) m_iNumber_of_Averages;
    unsigned short iFTM_Error_Code = 0;
	if (!m_pIPhone->QMSL_FTM_GSM_SETUP_RX_BURST_FOR_EXPECTED_PWR(iExpected_RX_Power_dbm16, iNumber_of_Averages, &iFTM_Error_Code))
	{
		m_strMsg = "Fail to set Rx burst for excepted PWR";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	unsigned short iMeasured_Dbm16_Value = 0;
    unsigned long  iRaw_RSSI = 0;
	iFTM_Error_Code = 0;

	if (!m_pIPhone->QMSL_FTM_GSM_GET_MEAS_PWR(&iMeasured_Dbm16_Value, &iRaw_RSSI, &iFTM_Error_Code))
	{
		m_strMsg = "Fail to set FTM GSM Rx Level";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	signed short signed_Measured_Dbm16_Value;
	signed_Measured_Dbm16_Value = (signed)iMeasured_Dbm16_Value;
	signed_Measured_Dbm16_Value /= 16;

	bool bPass = true;
	m_strMeasured = MakeSTDstring("%d", signed_Measured_Dbm16_Value);

	if (!InRange(m_dLower, m_dUpper, atof(m_strMeasured.c_str())))
	{
		bPass = false;
		m_strMsg = "GSM RX level not in range.";
		TraceLog(MSG_ERROR, m_strMsg);
	}

	if(!bPass)
	{
		/* Boston 於工廠 XP 電腦發生若 Disconnect_QMSL 時 5v PS 仍然 ON,
		則 Disconnect_QMSL 後 com port crash，需重啟電腦才能繼續測試。
		Debug 時發現 Tx/Rx off 也會讓 com port crash，故 fail 時直接 return false
		*/
		m_strMsg = "Return false before RX off.";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (!m_pITesterDevice->RF_TurnOnOffGernerator(false))
	{
        m_strMsg = "Fail to RF_TurnOnOffGernerator(false)" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
	}

	return bPass;
}

bool CWireless_GSM_RxLevel::InitData(std::map<std::string, std::string>& paramMap)
{
	std::vector<std::string> vTmp;

	if (paramMap.find("PICSName") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PICSName for CWireless_GSM_RxLevel");
		return false;
	}
	m_strPICSName = paramMap["PICSName"];

	if (paramMap.find("DelayAfterCMURun") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter DelayAfterCMURun for CWireless_GSM_RxLevel");
		return false;
	}
	m_iDelayAfterCMURun = atoi(paramMap["DelayAfterCMURun"].c_str());

	if (paramMap.find("Expected_RX_Power_dbm16") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Expected_RX_Power_dbm16 for CWireless_GSM_RxLevel");
		return false;
	}
	m_iExpected_RX_Power_dbm16 = atoi(paramMap["Expected_RX_Power_dbm16"].c_str());

	if (paramMap.find("Number_of_Averages") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Number_of_Averages for CWireless_GSM_RxLevel");
		return false;
	}
	m_iNumber_of_Averages = atoi(paramMap["Number_of_Averages"].c_str());

	if (paramMap.find("Rx_Level") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Rx_Level for CWireless_GSM_RxLevel");
		return false;
	}
	STDStringTokenizer(paramMap["Rx_Level"], ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Rx_Level for CWireless_GSM_RxLevel");
		return false;
	}
	m_dLower = atof(vTmp[0].c_str());
	m_dUpper = atof(vTmp[1].c_str());

	if (paramMap.find("Frequency") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Frequency for CWireless_GSM_RxLevel");
		return false;
	}
	m_dFreq = atof(paramMap["Frequency"].c_str());

	if (paramMap.find("AFFrequency") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter AFFrequency for CWireless_GSM_RxLevel");
		return false;
	}
	m_iAFFrequency = atoi(paramMap["AFFrequency"].c_str());

	if (paramMap.find("ModIndex") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter ModIndex for CWireless_GSM_RxLevel");
		return false;
	}
	m_iModIndex = atoi(paramMap["ModIndex"].c_str());

	if (paramMap.find("GSMMode") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter GSMMode for CWireless_GSM_RxLevel");
		return false;
	}
	m_iGSMMode = atoi(paramMap["GSMMode"].c_str());

	m_sGSMBand = GetQLibBandNum(m_iGSMMode);

	if ( m_sGSMBand == 0 )
	{
		TraceLog(MSG_ERROR, "Fail to find parameter GSMMode for CWireless_GSM_RxLevel");
		return false;
	}

	if (paramMap.find("Channel") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Channel for CWireless_GSM_RxLevel");
		return false;
	}
	m_iChannel = atoi(paramMap["Channel"].c_str());

	if (paramMap.find("OutputPort") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter OutputPort for CWireless_GSM_RxLevel");
		return false;
	}
	m_iOutputPort = atoi(paramMap["OutputPort"].c_str());
	if (paramMap.find("RFLevel") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter OutputPort for CWireless_GSM_RxLevel");
		return false;
	}
	m_iRFLevel = atoi(paramMap["RFLevel"].c_str());

	if (paramMap.find ("WriteCPKLog") != paramMap.end())
		m_WriteCPKLog = paramMap["WriteCPKLog"] != "0";

	ParseCPKItems();


	return true;
}

double CWireless_GSM_RxLevel::GetLossByDegree(int iDegree)
{
	//double dLoss = 0.0;

	//switch (iDegree)
	//{
	//case 0:
	//	dLoss = GetCableLoss("RX", "WirelessPCS", "low");
	//	break;
	//case 1:
	//	dLoss = GetCableLoss("RX", "WirelessPCS", "mid");
	//	break;
	//default:
	//	dLoss = GetCableLoss("RX", "WirelessPCS", "high");
	//}

	//return dLoss;

	double dLoss = 0.0;

	char szLossStr[256];
	memset(szLossStr, 0, 256);
	sprintf_s(szLossStr, 255, "WirelessGSM_%d", m_iGSMMode );

	switch (iDegree)
	{
	case 0:
		dLoss = GetCableLoss("RX", szLossStr, "0");
		break;
	case 1:
		dLoss = GetCableLoss("RX", szLossStr, "1");
		break;
	default:
		dLoss = GetCableLoss("RX", szLossStr, "2");
	}

	return dLoss;

}
bool CWireless_GSM_RxLevel::ParseCPKItems()
{


	CString str_RXModebuffer = _T("");
	//str_RXModebuffer.Format(_T("GSM_Rx_CPKcolumn"));

	//str_RXModebuffer.Format(_T("Wireless_LTE_Band_%sRxRSSI_CPKcolumn"), m_strRXMode.c_str());

	str_RXModebuffer.Format(_T("GSM_%d_Rx_CPKcolumn"), m_iGSMMode  );



	m_CPKHeaderList.clear();

	if (m_TestItemXML.Load(m_cstrTestItemFile) != ERROR_SUCCESS)
		return false;

	XMLNode processObjectSet = m_TestItemXML.SearchNode( _T("//Configuration//ProcessObjectSet//WirelessTestObjects//") + str_RXModebuffer );
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
short CWireless_GSM_RxLevel::GetQLibBandNum(int iBand)
{	

	switch (iBand)
	{
		case 900:		return PHONE_MODE_GSM_900;
		case 1800:		return PHONE_MODE_GSM_1800;
		case 1900:		return PHONE_MODE_GSM_1900;
	}
	return 0;

}