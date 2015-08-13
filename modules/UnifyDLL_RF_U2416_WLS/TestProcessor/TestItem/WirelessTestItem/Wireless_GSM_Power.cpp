#include "stdafx.h"
#include "Wireless_GSM_Power.h"


// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CWireless_GSM_Power)

CWireless_GSM_Power::CWireless_GSM_Power()
	:m_WriteCPKLog(false)
{
}

bool CWireless_GSM_Power::Run()
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
	
	sprintf_s(szBand, 255, "%d", m_iGSMMode );
//	strcpy(szBand, "GSM");

	// Prepare CPK log //----------------------------------------------------------------------
	char szBandFunctionCPK[256];
	memset(szBandFunctionCPK, 0, 256);
	sprintf_s(szBandFunctionCPK, 255, "Wireless_GSM_%d_Power_CPK", m_iGSMMode );
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
	sprintf_s(szBandFunction, 255, "GSM_%d_Power", m_iGSMMode );
	//---------------------------------

	bool bRes = MainFunction();

	if (bRes)
	{
		char szTestSuccess[256];
		memset(szTestSuccess, 0, 256);
		sprintf_s(szTestSuccess, 255, "Wireless GSM_%d Power PASS", m_iGSMMode );		
		FactoryLog( true , szBandFunction , "Pass" , szBand , m_strFrequency , szLower , szUpper , m_strMeasured , "dBm", szTestSuccess );
		//	FactoryLog(true, "GSMRfTx", "Pass", szBand, m_strFrequency, szLower, szUpper, m_strMeasured, "dBm", "Wireless GSMRfTx power PASS");
	}
	else
	{
		FactoryLog(false, szBandFunction , FunErr_WirelessGSM_Test_OutPower_OutRange, szBand, m_strFrequency, szLower, szUpper, m_strMeasured, "dBm", m_strMsg);
		//FactoryLog(false, "GSMRfTx", FunErr_WirelessGSM_Test_OutPower_OutRange, szBand, m_strFrequency, szLower, szUpper, m_strMeasured, "dBm", m_strMsg);
	}

	SetPICSData( m_strPICSName , m_strMeasured );

	char szCPKRecordValue[256];
	memset(szCPKRecordValue, 0, 256);
	sprintf_s(szCPKRecordValue, 255, "GSM_%d_Power_VALUE", m_iGSMMode );
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
	sprintf_s(szCPKRecordHEADERValue, 511, "Wireless_GSM_%d_Power_HEADER: %s", m_iGSMMode , gCpkRecord.GetLogHeader().c_str() );

	char szCPKRecordRESULTValue[512]; 
	memset(szCPKRecordRESULTValue, 0, 512);
	sprintf_s(szCPKRecordRESULTValue, 511, "Wireless_GSM_%d_Power_RESULT: %s", m_iGSMMode , gCpkRecord.GetLogString().c_str() );


	TraceLog ( MSG_INFO , szCPKRecordHEADERValue );
	TraceLog ( MSG_INFO , szCPKRecordRESULTValue );


	// write result to CPK log
	if ( m_WriteCPKLog )
	{
		gCpkRecord.WriteRecordToFile();
	}
	return bRes;
}

bool CWireless_GSM_Power::MainFunction()
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

	if (! m_pITesterDevice->SetInputLoss(1, m_iInputPort, GetLossByDegree(m_iPowerDegree)))
	{
		m_strMsg = "Fail to set input loss";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_SET_MODE_ID(FTM_MODE_ID_GSM)) // FTM_MODE_ID_GSM        = 2,    //!<' RF GSM Mode
	{
		m_strMsg = "Fail to set mode id";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_SET_MODE(m_sGSMBand))	
	{
		Sleep(2500);
		if(! m_pIPhone->QMSL_FTM_SET_MODE(m_sGSMBand))
		{
			m_strMsg = "Fail to set GSM_1800 mode";
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

	if (! m_pIPhone->QMSL_FTM_SET_CHAN(m_iChannel))
	{
		m_strMsg = "Fail to set channel";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_SET_MODE_ID(FTM_MODE_ID_GSM)) // FTM_MODE_ID_GSM        = 2,    //!<' RF GSM Mode
	{
		m_strMsg = "Fail to set mode id";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if(! m_pIPhone->QMSL_FTM_SET_TX_FRAME_MATRIX(0, 1, m_iPowerLevel, 0))
	{
		m_strMsg = "Fail to set TX frame matrix";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if(! m_pIPhone->QMSL_FTM_SET_TX_FRAME_MATRIX(1, 0, 0, 0))
	{
		m_strMsg = "Fail to set TX frame matrix";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if(! m_pIPhone->QMSL_FTM_SET_TX_FRAME_MATRIX(2, 0, 0, 0))
	{
		m_strMsg = "Fail to set TX frame matrix";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if(! m_pIPhone->QMSL_FTM_SET_TX_FRAME_MATRIX(3, 0, 0, 0))
	{
		m_strMsg = "Fail to set TX frame matrix";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if(! m_pIPhone->QMSL_FTM_SET_TX_FRAME_MATRIX(4, 0, 0, 0))
	{
		m_strMsg = "Fail to set TX frame matrix";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if(! m_pIPhone->QMSL_FTM_SET_TX_FRAME_MATRIX(5, 0, 0, 0))
	{
		m_strMsg = "Fail to set TX frame matrix";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if(! m_pIPhone->QMSL_FTM_SET_TX_FRAME_MATRIX(6, 0, 0, 0))
	{
		m_strMsg = "Fail to set TX frame matrix";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if(! m_pIPhone->QMSL_FTM_SET_TX_FRAME_MATRIX(7, 0, 0, 0))
	{
		m_strMsg = "Fail to set TX frame matrix";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_SET_MODE_ID(FTM_MODE_ID_GSM)) // FTM_MODE_ID_GSM        = 2,    //!<' RF GSM Mode
	{
		m_strMsg = "Fail to set mode id";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if(! m_pIPhone->QMSL_FTM_SET_TRANSMIT_BURST(0, 0, 0, 0, false)) // 0 - TX_DATA_SOURCE_PSDRND
	{
		m_strMsg = "Fail to set transmit burst";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_SET_MODE_ID(FTM_MODE_ID_GSM)) // FTM_MODE_ID_GSM        = 2,    //!<' RF GSM Mode
	{
		m_strMsg = "Fail to set mode id";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if(! m_pIPhone->QMSL_FTM_SET_TX_ON())
	{
		m_strMsg = "Fail to set TX ON";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	Sleep( m_iCommandDelay );

	bool bPass = false;
	double dPower;
	std::string strResponse;
	std::vector<std::string> vPowerList;

	double dMax = -99999.0;

	for ( int i = 0; i < m_iRetryTimes; ++i )
	{
		if (! m_pITesterDevice->RF_NPowerMeasurement( 10, strResponse ))
		{
			m_strMsg = "Fail to fetch RF analyzer power";
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}

		if (strstr(strResponse.c_str(), "NAN") != NULL)
		{
			sprintf_s(szTmp, 255, "%0d - GSM power : NAN", i + 1);
			m_strMsg = szTmp;
			TraceLog(MSG_ERROR, m_strMsg);
			continue;
		}

		STDStringTokenizer(strResponse, ",", vPowerList);
		dPower = atof( vPowerList[5].c_str() );

		//if ( dPower < 0 && i == 0 && m_iRetryTimes != 1 ) continue; // first read value error


		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%0d - GSM power : %.3f", i + 1, dPower);
		m_strMsg = szTmp;
		TraceLog(MSG_INFO, m_strMsg);

		if ( dPower > dMax )
			dMax = dPower;

		if ( ! InRange(m_dLower, m_dUpper, dMax ))
		{
			if ( i < m_iRetryTimes - 1)
			{
				m_strMsg = "GSM power not in range, try again";
				TraceLog(MSG_ERROR, m_strMsg);
			}
			else
			{
				m_strMsg = "GSM power not in range";
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

	if (! m_pIPhone->QMSL_FTM_SET_MODE_ID(FTM_MODE_ID_GSM)) // FTM_MODE_ID_GSM        = 2,    //!<' RF GSM Mode
	{
		m_strMsg = "Fail to set mode id";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if(! m_pIPhone->QMSL_FTM_SET_TX_OFF())
	{
		m_strMsg = "Fail to set TX OFF";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	return bPass;
}

double CWireless_GSM_Power::GetLossByDegree(int iDegree)
{
	double dLoss = 0.0;

	char szLossStr[256];
	memset(szLossStr, 0, 256);
	sprintf_s(szLossStr, 255, "WirelessGSM_%d", m_iGSMMode );

	switch (iDegree)
	{
	case 0:
		dLoss = GetCableLoss("TX", szLossStr, "0");
		break;
	case 1:
		dLoss = GetCableLoss("TX", szLossStr, "1");
		break;
	default:
		dLoss = GetCableLoss("TX", szLossStr, "2");
	}

	return dLoss;
}


bool CWireless_GSM_Power::InitData(std::map<std::string, std::string>& paramMap)
{
	std::vector<std::string> vTmp;

	if (paramMap.find("PICSName") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PICSName for CWireless_GSM_Power");
		return false;
	}
	m_strPICSName = paramMap["PICSName"];


	if (paramMap.find("GSMMode") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter GSMMode for CWireless_GSM_Power");
		return false;
	}
	m_iGSMMode = atoi(paramMap["GSMMode"].c_str());

	m_sGSMBand = GetQLibBandNum(m_iGSMMode);

	if ( m_sGSMBand == 0 )
	{
		TraceLog(MSG_ERROR, "Fail to find parameter GSMMode for CWireless_GSM_Power");
		return false;
	}

	if (paramMap.find("CommandDelay") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CommandDelay for CWireless_GSM_Power");
		return false;
	}
	m_iCommandDelay = atoi(paramMap["CommandDelay"].c_str());

	if (paramMap.find("PowerDegree") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PowerDegree for CWireless_GSM_Power");
		return false;
	}
	m_iPowerDegree = atoi(paramMap["PowerDegree"].c_str());

	if (paramMap.find("PowerLevel") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PowerLevel for CWireless_LTE_B20_TxPower");
		return false;
	}
	m_iPowerLevel = atoi(paramMap["PowerLevel"].c_str());

	if (paramMap.find("Frequency") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Frequency for CWireless_GSM_Power");
		return false;
	}
	m_dFrequency = atof(paramMap["Frequency"].c_str());
	m_strFrequency = paramMap["Frequency"].c_str();

	if (paramMap.find("Channel") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Channel for CWireless_LTE_B20_TxPower");
		return false;
	}
	m_iChannel = atoi(paramMap["Channel"].c_str());

	if (paramMap.find("RetryTimes") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RetryTimes for CWireless_GSM_Power");
		return false;
	}
	m_iRetryTimes = atoi(paramMap["RetryTimes"].c_str());

	if (paramMap.find("InputPort") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter InputPort for CWireless_GSM_Power");
		return false;
	}
	m_iInputPort = atoi(paramMap["InputPort"].c_str());

	if (paramMap.find("Range") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CWireless_GSM_Power");
		return false;
	}
	STDStringTokenizer(paramMap["Range"], ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CWireless_GSM_Power");
		return false;
	}
	m_dLower = atof(vTmp[0].c_str());
	m_dUpper = atof(vTmp[1].c_str());

	if (paramMap.find ("WriteCPKLog") != paramMap.end())
		m_WriteCPKLog = paramMap["WriteCPKLog"] != "0";

	ParseCPKItems();

	return true;
}

bool CWireless_GSM_Power::ParseCPKItems()
{


	CString str_RXModebuffer = _T("");
	str_RXModebuffer.Format(_T("GSM_%d_Power_CPKcolumn"), m_iGSMMode);


	m_CPKHeaderList.clear();

	if (m_TestItemXML.Load(m_cstrTestItemFile) != ERROR_SUCCESS)
		return false;

	XMLNode processObjectSet = m_TestItemXML.SearchNode( _T("//Configuration//ProcessObjectSet//WirelessTestObjects//") + str_RXModebuffer );
	if (processObjectSet == NULL)
		return false;




	//m_CPKHeaderList.clear();

	//if (m_TestItemXML.Load(m_cstrTestItemFile) != ERROR_SUCCESS)
	//	return false;

	//XMLNode processObjectSet = m_TestItemXML.SearchNode(_T("//Configuration//ProcessObjectSet//WirelessTestObjects//GSMCPKcolumn"));
	//if (processObjectSet == NULL)
	//	return false;

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

short CWireless_GSM_Power::GetQLibBandNum(int iBand)
{	

	switch (iBand)
	{
		case 900:		return PHONE_MODE_GSM_900;
		case 1800:		return PHONE_MODE_GSM_1800;
		case 1900:		return PHONE_MODE_GSM_1900;
	}
	return 0;

}