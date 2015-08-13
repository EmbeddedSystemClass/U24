#include "stdafx.h"
#include "Wireless_LTE_Band_RxRSSI.h"


// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CWireless_LTE_Band_RxRSSI)

CWireless_LTE_Band_RxRSSI::CWireless_LTE_Band_RxRSSI():m_WriteCPKLog(false)
{
}

bool CWireless_LTE_Band_RxRSSI::Run()
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
	sprintf_s(szBand, 255, "LTE_B%d", m_iBand);


	// Prepare CPK log //----------------------------------------------------------------------
	char szBandFunctionCPK[256];
	memset(szBandFunctionCPK, 0, 256);
	sprintf_s(szBandFunctionCPK, 255, "Wireless_LTE_B%d_%sRxRSSI_CPK", m_iBand , m_strRXMode.c_str() );
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
	sprintf_s(szBandFunction, 255, "LTE_B%d_%sRxRSSI", m_iBand , m_strRXMode.c_str() );
	//----------------------------------------------------------------------------------------
	bool bRes = MainFunction();

	if ( bRes )
	{
		char szTestSuccess[256];
		memset(szTestSuccess, 0, 256);
		sprintf_s(szTestSuccess, 255, "Wireless LTE B%d %sRxRssi PASS", m_iBand , m_strRXMode.c_str() );		

		FactoryLog( true , szBandFunction , "" , szBand , m_strChannel , szLower , szUpper , m_strMeasured , "dBm", szTestSuccess );
	}
	else
	{
		FactoryLog( false , szBandFunction , MakeLTE_ErrorCode( m_iBand , m_strRXMode ) , szBand , m_strChannel , szLower , szUpper , m_strMeasured , "dBm" , m_strMsg );	
	}
	SetPICSData(m_strPICSName, m_strMeasured);
	//----------------------------------------------------------------------------------------------------

	char szCPKRecordValue[256];
	memset(szCPKRecordValue, 0, 256);
	sprintf_s(szCPKRecordValue, 255, "LTE_Band_%sRxRSSI_VALUE", m_strRXMode.c_str() );
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
	sprintf_s(szCPKRecordHEADERValue, 511, "Wireless_LTE_B%d_%sRxRSSI_HEADER: %s", m_iBand , m_strRXMode.c_str() , gCpkRecord.GetLogHeader().c_str() );

	char szCPKRecordRESULTValue[512]; 
	memset(szCPKRecordRESULTValue, 0, 512);
	sprintf_s(szCPKRecordRESULTValue, 511, "Wireless_LTE_B%d_%sRxRSSI_RESULT: %s", m_iBand , m_strRXMode.c_str() , gCpkRecord.GetLogString().c_str() );


	TraceLog ( MSG_INFO , szCPKRecordHEADERValue );
	TraceLog ( MSG_INFO , szCPKRecordRESULTValue );


	// write result to CPK log
	if ( m_WriteCPKLog )
	{
		gCpkRecord.WriteRecordToFile();
	}

	return bRes;
}

bool CWireless_LTE_Band_RxRSSI::MainFunction()
{
	char szTmp[256];
	bool bPass = false;
	CString str_RXModebuffer = _T("");

	if ( m_pITesterDevice == NULL)
	{
		m_strMsg = "Fail to initial tester device";
		TraceLog(MSG_ERROR, m_strMsg);
		return false; 
	}

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

	if (! m_pIPhone->QMSL_FTM_SET_CALIBRATION_STATE(m_iCalibrationState))
	{
		m_strMsg = "Fail to set calibration";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_SET_MODE( GetQLibBandNum(m_iBand)) )
	{
		Sleep(2500);
		if (! m_pIPhone->QMSL_FTM_SET_MODE(GetQLibBandNum(m_iBand)))
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

	if (! m_pIPhone->QMSL_FTM_SET_MODE_ID(FTM_MODE_ID_LTE))
	{
		m_strMsg = "Fail to set FTM_MODE_ID_LTE mode id";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	//---------------------------------------------------------------------------------------------------
	if ( 0 == strcmp(m_strRXMode.c_str(), "P"))
	{
		if (! m_pIPhone->QMSL_FTM_SET_SECONDARY_CHAIN(0)) // 
		{
			m_strMsg = "Fail to set secondary channel";
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}

		if (! m_pIPhone->QMSL_FTM_SET_MODE_ID(FTM_MODE_ID_LTE))
		{
			m_strMsg = "Fail to set FTM_MODE_ID_LTE mode id";
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}
	}
	//else if ( 0 == strcmp(m_strRXMode.c_str(), "D"))
	else
	{
		if (! m_pIPhone->QMSL_FTM_SET_SECONDARY_CHAIN(1)) // 1 - Receive Diversity
		{
			m_strMsg = "Fail to set secondary channel";
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}
		if (! m_pIPhone->QMSL_FTM_SET_MODE_ID(FTM_MODE_LTE_RX1))
		{
			m_strMsg = "Fail to set FTM_MODE_LTE_RX1 mode id";
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}
	}

	if (! m_pIPhone->QMSL_FTM_LTE_SET_TX_MODULATION_TYPE(0))
	{
		m_strMsg = "Fail to set Tx modulation type";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	if ( 0 == strcmp(m_strReadValueMode.c_str(), "AGC"))
	{

		if (! m_pIPhone->QMSL_FTM_SET_LNA_RANGE(m_iAGCModeLNAValue))
		{
			m_strMsg = "Fail to set FTM_SET_LNA_RANGE Value";
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}	
	}
	else
	{

		if (! m_pIPhone->QMSL_FTM_SET_MODE_ID(FTM_MODE_ID_LTE))
		{
			m_strMsg = "Fail to set FTM_MODE_ID_LTE mode id";
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}	
		if (! m_pIPhone->QMSL_FTM_LTE_SET_LNA_STATE_DBM ( m_iRFLevel*10 ) )
		{
			m_strMsg = "Fail to set lna DBM";
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}
	}

	Sleep(100);
	
	double dRssi = 0;
	unsigned short iRX_AGC = 0;
	unsigned short iRX_Power_dBm = 0;
	unsigned short iFTM_Error_Code = 0;

	int iMeasured = 0;
	char szMeasured[20] = {0};

	for (int i = 0; i < m_iRetryTimes; ++i)
	{

		if ( 0 == strcmp(m_strReadValueMode.c_str(), "AGC"))
		{
			if (! m_pIPhone->QMSL_FTM_GET_RX_AGC( &iRX_AGC ) )
			{

				str_RXModebuffer.Format(_T("Rerty to get Wireless LTE Band %sRxRssi"), m_strRXMode.c_str());
				m_strMsg = "Rerty to get Wireless LTE Band RxRssi";
				m_strMsg = string(str_RXModebuffer);
				TraceLog(MSG_ERROR, m_strMsg);
				continue;
			}
			iMeasured = iRX_AGC;
			sprintf_s(szMeasured, 20,  "%hd", iMeasured);
			//m_strMeasured = szMeasured;
			
			dRssi = (float)atof(szMeasured);
			dRssi = dRssi / (float)10;
			sprintf_s (szTmp, 255, "%0d - LTE B%d %sRxRssi = %.2f", (i+1) , m_iBand , m_strRXMode.c_str() , dRssi);
			m_strMsg = szTmp;
			TraceLog(MSG_INFO, m_strMsg);


			sprintf_s(szMeasured, 20, "%.1f", dRssi);
			m_strMeasured = szMeasured;
		}
		else
		{

			// QLIB get Rssi
			if (! m_pIPhone->LTE_Band_RxRssi( &iRX_AGC, &iRX_Power_dBm, &iFTM_Error_Code) )
			{

				str_RXModebuffer.Format(_T("Rerty to get Wireless LTE Band %sRxRssi"), m_strRXMode.c_str());
				m_strMsg = "Rerty to get Wireless LTE Band RxRssi";
				m_strMsg = string(str_RXModebuffer);

				TraceLog(MSG_ERROR, m_strMsg);
				continue;
			}

			if (0 < iRX_Power_dBm && iRX_Power_dBm <= 65536)
			{
				iRX_Power_dBm = ~iRX_Power_dBm;
				dRssi = (double)(-(iRX_Power_dBm + 1) / 10.0);
				//sprintf_s (szTmp, 255, "%0d - LTE Band RxRssi = %.2f", (i+1), dRssi);
				sprintf_s (szTmp, 255, "%0d - LTE B%d %sRxRssi = %.2f", (i+1) , m_iBand , m_strRXMode.c_str() , dRssi);

				m_strMsg = szTmp;
				TraceLog(MSG_INFO, m_strMsg);

				memset(szTmp, 0, 256);
				sprintf_s (szTmp, 255, "%.2f", dRssi);
				m_strMeasured = szTmp;
			}
		}
		

		if (InRange (m_dLower, m_dUpper, dRssi))
		{
			bPass = true;
			break;
		}
		else
		{
			if (i < m_iRetryTimes - 1)
			{
				//str_RXModebuffer.Format(_T("LTE Band %sRxRssi is not in range, try again "), m_strRXMode.c_str());
				str_RXModebuffer.Format(_T("LTE B%d %sRxRssi is not in range, try again value = %.2f"), m_iBand , m_strRXMode.c_str() , dRssi );
				//m_strMsg = "LTE Band RxRssi is not in range, try again";
				m_strMsg = string(str_RXModebuffer);
				TraceLog(MSG_ERROR, m_strMsg);
				Sleep (m_dwCommandDelay);
			}
			else
			{

				str_RXModebuffer.Format(_T("LTE B%d %sRxRssi is not in range"), m_iBand,m_strRXMode.c_str());
				//m_strMsg =  "LTE Band RxRssi is not in range";
				m_strMsg = string(str_RXModebuffer);
				TraceLog(MSG_ERROR, m_strMsg);
				return false;
			}
		}		
	}

	if (! m_pIPhone->QMSL_FTM_SET_CALIBRATION_STATE(0))
	{
		m_strMsg = "Fail to set calibration";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
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

	if (! m_pITesterDevice->RF_TurnOnOffGernerator (false))
	{
        m_strMsg = "Fail to RF_TurnOnOffGernerator(false)" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
	}

	return bPass;
}

double CWireless_LTE_Band_RxRSSI::GetLossByDegree(int iDegree)
{
	double dLoss = 0.0;
	char szLossStr[256];
	memset(szLossStr, 0, 256);
	sprintf_s(szLossStr, 255, "WirelessB%d", m_iBand );


	if ( 0 == strcmp(m_strRXMode.c_str(), "P"))
	{
		switch (iDegree)
		{
		case 0:
			dLoss = GetCableLoss("RX", szLossStr, "P_low");
			break;
		case 1:
			dLoss = GetCableLoss("RX", szLossStr, "P_mid");
			break;
		default:
			dLoss = GetCableLoss("RX", szLossStr, "P_high");
		}
	}
	else if (0 == strcmp(m_strRXMode.c_str(), "D"))
	{
		switch (iDegree)
		{
		case 0:
			dLoss = GetCableLoss("RX", szLossStr, "D_low");
			break;
		case 1:
			dLoss = GetCableLoss("RX", szLossStr, "D_mid");
			break;
		default:
			dLoss = GetCableLoss("RX", szLossStr, "D_high");
		}
	}
	else
	{
		switch (iDegree)
		{
		case 0:
			dLoss = GetCableLoss("RX", "WirelessBand", "low");
			break;
		case 1:
			dLoss = GetCableLoss("RX", "WirelessBand", "mid");
			break;
		default:
			dLoss = GetCableLoss("RX", "WirelessBand", "high");
		}
	}
	return dLoss;
}


bool CWireless_LTE_Band_RxRSSI::InitData(std::map<std::string, std::string>& paramMap)
{
	std::vector<std::string> vTmp;
	// XML Setup //----------------------------------------------------------------------------
	if (paramMap.find("PICSName") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PICSName for CWireless_LTE_Band_RxRSSI");
		return false;
	}
	m_strPICSName = paramMap["PICSName"];
	//------------------------------------------------------------------------------------------
	if (paramMap.find("Band") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Band for CWireless_LTE_Band_RxRSSI");
		return false;
	}
	m_iBand = atoi(paramMap["Band"].c_str());
	//------------------------------------------------------------------------------------------
	if (paramMap.find("RXMode") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RXMode for CWireless_LTE_Band_RxRSSI");
		return false;
	}
	m_strRXMode = paramMap["RXMode"];
	//------------------------------------------------------------------------------------------
	if (paramMap.find("RetryTimes") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RetryTimes for CWireless_LTE_Band_RxRSSI");
		return false;
	}
	m_iRetryTimes = atoi(paramMap["RetryTimes"].c_str());
	//------------------------------------------------------------------------------------------
	if (paramMap.find("CommandDelay") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CommandDelay for CWireless_LTE_Band_TxPower");
		return false;
	}
	m_iCommandDelay = atoi(paramMap["CommandDelay"].c_str());
	//------------------------------------------------------------------------------------------
	if (paramMap.find("OutputPort") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter OutputPort for CWireless_LTE_Band_RxRSSI");
		return false;
	}
	m_iOutputPort = atoi(paramMap["OutputPort"].c_str());
	//------------------------------------------------------------------------------------------
	//if (paramMap.find("Frequency") == paramMap.end())
	//{
	//	TraceLog(MSG_ERROR, "Fail to find parameter Frequency for CWireless_FMR_rssiTest");
	//	return false;
	//}
	//m_dFreq = atof(paramMap["Frequency"].c_str());	
	//------------------------------------------------------------------------------------------
	if (paramMap.find("RFLevel") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RFLevel for CWireless_LTE_Band_RxRSSI");
		return false;
	}
	m_iRFLevel = atoi(paramMap["RFLevel"].c_str());
	//------------------------------------------------------------------------------------------
	if (paramMap.find("TxBandwidth") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Channel for CWireless_LTE_Band_RxRSSI");
		return false;
	}
	m_iTxBandwidth = atoi(paramMap["TxBandwidth"].c_str());
	//------------------------------------------------------------------------------------------
	if (paramMap.find("RxBandwidth") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Channel for CWireless_LTE_Band_RxRSSI");
		return false;
	}
	m_iRxBandwidth = atoi(paramMap["RxBandwidth"].c_str());
	//------------------------------------------------------------------------------------------
	if (paramMap.find("Channel") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Channel for CWireless_LTE_Band_RxRSSI");
		return false;
	}	
	m_iChannel = atoi(paramMap["Channel"].c_str());
	m_strChannel = paramMap["Channel"].c_str();
	unsigned int iFreq = 0;

	if ( !LTE_BSTxChannelToFrequency( m_iBand , m_iChannel - 18000 , iFreq , true ) || iFreq < 0 ) // need key in DownLink value , but XML is UpLink
	{
		TraceLog(MSG_ERROR, "Fail to get parameter Frequency for CWireless_LTE_Band_RxRSSI");
		return false;		
	}
	m_dFreq = iFreq/1000000.0;
	//------------------------------------------------------------------------------------------

	if (paramMap.find("DelayAfterCMURun") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter DelayAfterCMURun for CWireless_LTE_Band_RxRSSI");
		return false;
	}
	m_dwDelayAfterCMURun = atoi(paramMap["DelayAfterCMURun"].c_str());
	//------------------------------------------------------------------------------------------

	if (paramMap.find("CommandDelay") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CommandDelay for CWireless_LTE_Band_RxRSSI");
		return false;
	}
	m_dwCommandDelay = atoi(paramMap["CommandDelay"].c_str());
	//------------------------------------------------------------------------------------------
	if (paramMap.find("Range") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CWireless_LTE_Band_RxRSSI");
		return false;
	}
	STDStringTokenizer(paramMap["Range"], ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CWireless_LTE_Band_RxRSSI");
		return false;
	}
	m_dLower = atof(vTmp[0].c_str());
	m_dUpper = atof(vTmp[1].c_str());
	//------------------------------------------------------------------------------------------
	if (paramMap.find("ReadMode") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter ReadMode for CWireless_LTE_Band_RxRSSI");
		return false;
	}
	m_strReadValueMode = paramMap["ReadMode"];
	//------------------------------------------------------------------------------------------	
	if (paramMap.find("LNAValue") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter LNAValue for CWireless_LTE_Band_RxRSSI");
		return false;
	}
	m_iAGCModeLNAValue = atoi(paramMap["LNAValue"].c_str());
	//-----------------------------------------------------------------------------------------
	if (paramMap.find("CalibrationState") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CalibrationState for CWireless_LTE_Band_RxRSSI");
		return false;
	}
	m_iCalibrationState = atoi(paramMap["CalibrationState"].c_str());
	//------------------------------------------------------------------------------------------
	if (paramMap.find ("WriteCPKLog") != paramMap.end())
		m_WriteCPKLog = paramMap["WriteCPKLog"] != "0";

	ParseCPKItems();
	//------------------------------------------------------------------------------------------
	return true;
}

bool CWireless_LTE_Band_RxRSSI::ParseCPKItems()
{

	CString str_RXModebuffer = _T("");
	str_RXModebuffer.Format(_T("Wireless_LTE_Band_%sRxRSSI_CPKcolumn"), m_strRXMode.c_str());


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

std::string CWireless_LTE_Band_RxRSSI::MakeLTE_ErrorCode( int iBand , std::string sMode )
{
	std::string Errorcode = "" ;
	std::string strBand = "" ;
	
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
		break ;
	}	

	char szErrorcode[256];
	memset(szErrorcode, 0, 256);
	
	if ( 0 == strcmp(sMode.c_str(), "P"))
	{
		sprintf_s(szErrorcode, 255, "L%sM%s_LTETest_RxSens_Fail", strBand.c_str() , "012" );
		Errorcode = szErrorcode;
	}
	else if ( 0 == strcmp(sMode.c_str(), "D"))
	{
		sprintf_s(szErrorcode, 255, "L%sM%s_LTETest_DRX_Sens_Fail", strBand.c_str() , "031" );
		Errorcode = szErrorcode;
	}
	else
	{
		Errorcode = FunErr_WirelessLTE_Diversity_RxRSSI_Fail;
	}
	return Errorcode; 

}

short CWireless_LTE_Band_RxRSSI::GetQLibBandNum(int iBand)
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


