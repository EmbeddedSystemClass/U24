#include "stdafx.h"
#include "Wireless_LTE_Band_TxPower.h"


// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CWireless_LTE_Band_TxPower)

CWireless_LTE_Band_TxPower::CWireless_LTE_Band_TxPower()
	:m_WriteCPKLog(false)
{
}

bool CWireless_LTE_Band_TxPower::Run()
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
	strcpy(szBand, "LTE_Band");



	// Prepare CPK log //----------------------------------------------------------------------
	char szBandFunctionCPK[256];
	memset(szBandFunctionCPK, 0, 256);
	sprintf_s(szBandFunctionCPK, 255, "Wireless_LTE_B%d_TxPower_CPK", m_iBand );
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
	//----------------------------------------------------------------------------------------
	// Define //------------------------------------------------------------------------------
	char szBandFunction[256];
	memset(szBandFunction, 0, 256);
	sprintf_s(szBandFunction, 255, "LTE_B%d_TxPower", m_iBand );
	//----------------------------------------------------------------------------------------
	bool bRes = MainFunction();

	if ( bRes )
	{
		char szTestSuccess[256];
		memset(szTestSuccess, 0, 256);
		sprintf_s(szTestSuccess, 255, "Wireless LTE B%d TxPower PASS", m_iBand );		

		FactoryLog( true , szBandFunction , "Pass" , szBand , m_strChannel , szLower , szUpper , m_strMeasured , "dBm", szTestSuccess );
	}
	else
	{
		FactoryLog( false , szBandFunction , MakeLTE_ErrorCode( m_iBand ) , szBand , m_strChannel , szLower , szUpper , m_strMeasured , "dBm" , m_strMsg );	
	}
	SetPICSData(m_strPICSName, m_strMeasured);
	//----------------------------------------------------------------------------------------------------

	char szCPKRecordValue[256];
	memset(szCPKRecordValue, 0, 256);
	sprintf_s(szCPKRecordValue, 255, "LTE_Band_TxPower_VALUE"  );
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
	sprintf_s(szCPKRecordHEADERValue, 511, "Wireless_LTE_B%d_TxPower_HEADER: %s", m_iBand , gCpkRecord.GetLogHeader().c_str() );

	char szCPKRecordRESULTValue[512]; 
	memset(szCPKRecordRESULTValue, 0, 512);
	sprintf_s(szCPKRecordRESULTValue, 511, "Wireless_LTE_B%d_TxPower_RESULT: %s", m_iBand , gCpkRecord.GetLogString().c_str() );


	TraceLog ( MSG_INFO , szCPKRecordHEADERValue );
	TraceLog ( MSG_INFO , szCPKRecordRESULTValue );

	// write result to CPK log
	if ( m_WriteCPKLog )
	{
		gCpkRecord.WriteRecordToFile();
	}
	return bRes;

}

bool CWireless_LTE_Band_TxPower::GainIndexTest()
{
	if (! m_pIPhone->QMSL_FTM_SET_TX_ON())
	{
		m_strMsg = "Fail to set Tx ON";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_LTE_SET_TX_WAVEFORM(1, 50, 0, 0))
	{
		m_strMsg = "Fail to set Tx waveform";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_SET_PA_RANGE(0))
	{
		m_strMsg = "Fail to set PA range";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_SET_TX_GAIN_INDEX(m_iTxGainIndex))
	{
		m_strMsg = "Fail to set Tx Gain index";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	return true;
}

bool CWireless_LTE_Band_TxPower::PowerDbmTest()
{
	unsigned short iFTM_Error_Code = 0;

	if (! m_pIPhone->QMSL_FTM_LTE_SET_TX_MODULATION_TYPE(0))
	{
		m_strMsg = "Fail to set Tx modulation type";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_LTE_SET_TX_WAVEFORM(1, 25, 0, 0))
	{
		m_strMsg = "Fail to set Tx waveform";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_SET_MODE_ID(FTM_MODE_ID_LTE)) // FTM_MODE_ID_LTE = 29,       //!<' LTE FTM Calibration
	{
		m_strMsg = "Fail to set mode id";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_LTE_SET_NET_SIG_VALUE(0, &iFTM_Error_Code))
	{
		m_strMsg = "Fail to set NET SIG value";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_SET_TX_ON())
	{
		m_strMsg = "Fail to set Tx ON";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_SET_MODE_ID(FTM_MODE_ID_LTE)) // FTM_MODE_ID_LTE = 29,       //!<' LTE FTM Calibration
	{
		m_strMsg = "Fail to set mode id";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_SET_TX_POWER_DBM(1, m_iPowerLevel, &iFTM_Error_Code)) //0 ==> Disable, 1 ==> Enable
	{
		m_strMsg = "Fail to enable Tx power DBM";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_SET_MODE_ID(FTM_MODE_ID_LTE)) // FTM_MODE_ID_LTE = 29,       //!<' LTE FTM Calibration
	{
		m_strMsg = "Fail to set mode id";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	return true;
}

bool CWireless_LTE_Band_TxPower::MainFunction()
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

	if (! m_pITesterDevice->SetResolutionBandwidth(1))
	{
		m_strMsg = "Fail to set resolution bandwidth";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDevice->SetInputLoss(1, m_iInputPort, GetLossByDegree(m_iPowerDegree)))
		return false;

	if (! m_pITesterDevice->RF_SetLevelMode(m_strLevelMode.c_str()))
	{
		m_strMsg = "Fail to set level mode";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDevice->RF_SetLevelMaximum(m_iLevelMax))
	{
		m_strMsg = "Fail to set level reference";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	//-----------------------------------------------------------------------------------------	

	if (! m_pIPhone->QMSL_FTM_SET_MODE(GetQLibBandNum(m_iBand)))	
	{
		Sleep(2500);
		if(! m_pIPhone->QMSL_FTM_SET_MODE(GetQLibBandNum(m_iBand)))
		{
			m_strMsg = "Fail to set LTE_Band mode";
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}
	}

	if (! m_pIPhone->QMSL_FTM_LTE_SET_TX_BANDWIDTH(m_iTxBandwidth))
	{
		m_strMsg = "Fail to set Tx bandwidth";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_LTE_SET_RX_BANDWIDTH(m_iRxBandwidth))
	{
		m_strMsg = "Fail to set Rx bandwidth";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_SET_CHAN(m_iChannel))
	{
		m_strMsg = "Fail to set channel";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_SET_MODE_ID(FTM_MODE_ID_LTE)) // FTM_MODE_ID_LTE = 29,       //!<' LTE FTM Calibration
	{
		m_strMsg = "Fail to set mode id";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_SET_SECONDARY_CHAIN(0)) // 2 - Disabled
	{
		m_strMsg = "Fail to set secondary channel";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}


	if ( 0 == m_iTestMethod)
	{
		GainIndexTest();
	}
	else if ( 1 == m_iTestMethod)
	{
		PowerDbmTest();
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
			m_strMsg = "Fail to fetch RF analyzer power";
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
			sprintf_s(szTmp, 255, "%0d - LTE Band TxPower : %s", i + 1, strResponse.c_str());
			m_strMsg = szTmp;
			TraceLog(MSG_ERROR, m_strMsg);

			Sleep(1000);
			continue;
		}

		sprintf_s(szTmp, 255, "%0d - LTE Band TxPower : %s", i + 1, strResponse.c_str());
		m_strMsg = szTmp;
		TraceLog(MSG_ERROR, m_strMsg);
		

		STDStringTokenizer(strResponse, ",", vPowerList);
		dPower = atof(vPowerList[5].c_str());

		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%0d - LTE Band TxPower : %.3f", i + 1, dPower);
		m_strMsg = szTmp;
		TraceLog(MSG_INFO, m_strMsg);

		if (dPower > dMax)
			dMax = dPower;

		if (! InRange(m_dLower, m_dUpper, dMax))
		{
			if (i < m_iRetryTimes - 1)
			{
				m_strMsg = "LTE Band TxPower not in range, try again";
				TraceLog(MSG_ERROR, m_strMsg);

			}
			else
			{
				m_strMsg = "LTE Band TxPower not in range";
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

	if(!bPass)
	{
		m_strMsg = "Return false before TX off.";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if ( 0 == m_iTestMethod)
	{
		if (! m_pIPhone->QMSL_FTM_LTE_STOP_TX_WAVEFORM())
		{
			m_strMsg = "Fail to stop Tx waveform";
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}
	}
	else if ( 1 == m_iTestMethod)
	{
		unsigned short iFTM_Error_Code = 0;
		if (! m_pIPhone->QMSL_FTM_SET_TX_POWER_DBM(0, m_iPowerLevel, &iFTM_Error_Code)) //0 ==> Disable, 1 ==> Enable
		{
			m_strMsg = "Fail to disable Tx power DBM";
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}
	}

	// Stop LTE Band TxPower
	if (! m_pIPhone->QMSL_FTM_SET_TX_OFF())
	{
		m_strMsg = "Fail to set Tx OFF";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	//Sleep(m_iCommandDelay);

	return bPass;
}

double CWireless_LTE_Band_TxPower::GetLossByDegree(int iDegree)
{

	double dLoss = 0.0;
	char szLossStr[256];
	memset(szLossStr, 0, 256);
	sprintf_s(szLossStr, 255, "WirelessB%d", m_iBand );

	switch (iDegree)
	{
	case 0:
		dLoss = GetCableLoss("TX", szLossStr, "low");
		break;
	case 1:
		dLoss = GetCableLoss("TX", szLossStr, "mid");
		break;
	default:
		dLoss = GetCableLoss("TX", szLossStr, "high");
	}
	
	return dLoss;

}


bool CWireless_LTE_Band_TxPower::InitData(std::map<std::string, std::string>& paramMap)
{
	std::vector<std::string> vTmp;

	if (paramMap.find("PICSName") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PICSName for CWireless_LTE_Band_TxPower");
		return false;
	}
	m_strPICSName = paramMap["PICSName"];

	//------------------------------------------------------------------------------------------
	if (paramMap.find("Band") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Band for CWireless_LTE_Band_TxPower");
		return false;
	}
	m_iBand = atoi(paramMap["Band"].c_str());


	if (paramMap.find("CommandDelay") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CommandDelay for CWireless_LTE_Band_TxPower");
		return false;
	}
	m_iCommandDelay = atoi(paramMap["CommandDelay"].c_str());

	if (paramMap.find("StartDelay") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter StartDelay for CWireless_LTE_Band_TxPower");
		return false;
	}
	m_iStartDelay = atoi(paramMap["StartDelay"].c_str());

	//if (paramMap.find("Frequency") == paramMap.end())
	//{
	//	TraceLog(MSG_ERROR, "Fail to find parameter Frequency for CWireless_LTE_Band_TxPower");
	//	return false;
	//}
	//m_dFrequency = atof(paramMap["Frequency"].c_str());

	if (paramMap.find("PowerDegree") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PowerDegree for CWireless_LTE_Band_TxPower");
		return false;
	}
	m_iPowerDegree = atoi(paramMap["PowerDegree"].c_str());

	if (paramMap.find("PowerLevel") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PowerLevel for CWireless_LTE_Band_TxPower");
		return false;
	}
	m_iPowerLevel = atoi(paramMap["PowerLevel"].c_str());

	if (paramMap.find("TxBandwidth") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Channel for CWireless_LTE_Band_TxBandwidth");
		return false;
	}
	m_iTxBandwidth = atoi(paramMap["TxBandwidth"].c_str());

	if (paramMap.find("RxBandwidth") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Channel for CWireless_LTE_Band_RxBandwidth");
		return false;
	}
	m_iRxBandwidth = atoi(paramMap["RxBandwidth"].c_str());

	if (paramMap.find("Channel") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Channel for CWireless_LTE_Band_TxPower");
		return false;
	}
	m_iChannel = atoi(paramMap["Channel"].c_str());
	m_strChannel = paramMap["Channel"].c_str();

	unsigned int iFreq = 0;

	if ( !LTE_BSTxChannelToFrequency( m_iBand , m_iChannel - 18000 , iFreq , false ) || iFreq < 0 ) // need key in DownLink value , but XML is UpLink
	{
		TraceLog(MSG_ERROR, "Fail to get parameter Frequency for CWireless_LTE_Band_TxPower");
		return false;		
	}
	m_dFrequency = iFreq/1000000.0;

	if (paramMap.find("TxGainIndex") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Power for CWireless_LTE_Band_TxGainIndex");
		return false;
	}
	m_iTxGainIndex = atoi(paramMap["TxGainIndex"].c_str());

	if (paramMap.find("RetryTimes") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RetryTimes for CWireless_LTE_Band_TxPower");
		return false;
	}
	m_iRetryTimes = atoi(paramMap["RetryTimes"].c_str());

	if (paramMap.find("InputPort") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter InputPort for CWireless_LTE_Band_TxPower");
		return false;
	}
	m_iInputPort = atoi(paramMap["InputPort"].c_str());

	if (paramMap.find("Range") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CWireless_LTE_Band_TxPower");
		return false;
	}
	STDStringTokenizer(paramMap["Range"], ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CWireless_LTE_Band_TxPower");
		return false;
	}
	m_dLower = atof(vTmp[0].c_str());
	m_dUpper = atof(vTmp[1].c_str());

	if (paramMap.find("TestMethod") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter TestMethod for CWireless_LTE_Band_TxPower");
		return false;
	}
	m_iTestMethod = atoi(paramMap["TestMethod"].c_str());

	if (paramMap.find("LevelMode") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter LevelMode for CWireless_LTE_Band_TxPower");
		return false;
	}
	m_strLevelMode = paramMap["LevelMode"];

	if (paramMap.find("LevelMax") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter LevelMax for CWireless_LTE_Band_TxPower");
		return false;
	}
	m_iLevelMax = atoi(paramMap["LevelMax"].c_str());

	if (paramMap.find ("WriteCPKLog") != paramMap.end())
		m_WriteCPKLog = paramMap["WriteCPKLog"] != "0";

	ParseCPKItems();

	return true;
}

bool CWireless_LTE_Band_TxPower::ParseCPKItems()
{


	CString str_RXModebuffer = _T("");
	str_RXModebuffer.Format(_T("Wireless_LTE_Band_TxPower_CPKcolumn"));


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

std::string CWireless_LTE_Band_TxPower::MakeLTE_ErrorCode( int iBand )
{
	std::string Errorcode = "" ;
	std::string strBand = "" ;
	bool hasErrorcode = true;

	switch (iBand)
	{
	case 1:
	case 2100:
		strBand = "01" ;
		break;
	case 2:
	case 1900:
		strBand = "02" ;
		break;
	case 3:
	case 1800:
		strBand = "03" ;
		break;
	case 4:
	case 1700:
		strBand = "04" ;
		break;
	case 5:
	case 850:
		strBand = "05" ;
		break;
	case 6:
	case 800:
		strBand = "06" ;
		break;
	case 7:
	case 2600:
		strBand = "07" ;
		break;
	case 8:
	case 900:
		strBand = "08" ;
		break;
	case 20:
		strBand = "20" ;
		break;
	case 28:
	case 281:
	case 282:
		strBand = "28" ;
		break;
	default :
		hasErrorcode = false;
		break ;
	}	

//L01M002_LTETest_Max_Power_Fail ¥ý¥Î002
//L01L003_LTETest_Min_Power_Fail 

	char szErrorcode[256];
	memset(szErrorcode, 0, 256);

	if ( hasErrorcode )
	{
		sprintf_s(szErrorcode, 255, "L%sM002_LTETest_Max_Power_Fail", strBand.c_str() );
		Errorcode = szErrorcode;
	}
	else
	{
		Errorcode = FunErr_WirelessLTE_Diversity_RxRSSI_Fail;
	}

	return Errorcode; 

}

short CWireless_LTE_Band_TxPower::GetQLibBandNum(int iBand)
{	

	switch (iBand)
	{
		case 1:		return PHONE_MODE_LTE_B1;
		case 2:		return PHONE_MODE_LTE_B2;
		case 3:		return PHONE_MODE_LTE_B3;
		case 4:		return PHONE_MODE_LTE_B4;
		case 5:		return PHONE_MODE_LTE_B5;
		case 6:		return PHONE_MODE_LTE_B6;
		case 7:		return PHONE_MODE_LTE_B7;
		case 8:		return PHONE_MODE_LTE_B8;
		case 9:		return PHONE_MODE_LTE_B9;
		case 10:	return PHONE_MODE_LTE_B10;
		case 11:	return PHONE_MODE_LTE_B11;
		case 12:	return PHONE_MODE_LTE_B12;
		case 13:	return PHONE_MODE_LTE_B13;
		case 14:	return PHONE_MODE_LTE_B14;
		case 15:	return PHONE_MODE_LTE_B15;
		case 16:	return PHONE_MODE_LTE_B16;
		case 17:	return PHONE_MODE_LTE_B17;
		case 18:	return PHONE_MODE_LTE_B18;
		case 19:	return PHONE_MODE_LTE_B19;
		case 20:	return PHONE_MODE_LTE_B20;
		case 21:	return PHONE_MODE_LTE_B21;
		case 22:	return PHONE_MODE_LTE_B22;
		case 23:	return PHONE_MODE_LTE_B23;
		case 24:	return PHONE_MODE_LTE_B24;
		case 25:	return PHONE_MODE_LTE_B25;
		case 26:	return PHONE_MODE_LTE_B26;
		case 27:	return PHONE_MODE_LTE_B27;
		case 28:	return PHONE_MODE_LTE_B28;
		case 29:	return PHONE_MODE_LTE_B29;
		case 30:	return PHONE_MODE_LTE_B30;
		case 31:	return PHONE_MODE_LTE_B31;
		case 32:	return PHONE_MODE_LTE_B32;
		case 33:	return PHONE_MODE_LTE_B33;
		case 34:	return PHONE_MODE_LTE_B34;
		case 35:	return PHONE_MODE_LTE_B35;
		case 36:	return PHONE_MODE_LTE_B36;
		case 37:	return PHONE_MODE_LTE_B37;
		case 38:	return PHONE_MODE_LTE_B38;
		case 39:	return PHONE_MODE_LTE_B39;
		case 40:	return PHONE_MODE_LTE_B40;
		case 41:	return PHONE_MODE_LTE_B41;
		case 42:	return PHONE_MODE_LTE_B42;
	}
	return 0;

}
