#include "stdafx.h"
#include "CMW.h"

using namespace StringUtility;

GP_IMPLEMENT_DYNCREATE(CCMW)


// ------------------- Initial functions -------------------

bool CCMW::Initial()
{
	if (! GPIB_OPComplete(std::string("*CLS; *OPC?")))
        return false;

    if (! GPIB_OPComplete(std::string("*RST; *OPC?")))
        return false ;

	if (! GPIB_OPComplete(std::string("TRACe:REMote:MODE:DISPlay:ENABle LIVE; *OPC?")))
		return false;

	return true;
}


bool CCMW::Initial_GSM()
{
	return false;
}

bool CCMW::Initial_RF_Nonsignaling()
{
	return false;
}
bool CCMW::Initial_LTE()
{
	GPIB_Write("CONFigure:GPRF:MEAS:POWer:REPetition SINGleshot") ;
	GPIB_Write("CONFigure:GPRF:MEAS:POWer:FILTer:TYPE GAUSs");
	return true;
}
// --------------- Get Info of Test Device --------------

bool CCMW::GetOptions(std::string& strOptions)
{
	if (! GPIB_Query(std::string("SYSTem:BASE:OPTion:LIST?"), strOptions))
		return false;

	return true;
}

bool CCMW::GetOptionVersions(std::string& strOptionsVer)
{
    if (! GPIB_Query(std::string("SYSTem:BASE:OPTion:VERSion?"), strOptionsVer))
        return false;

    return true ;
}

bool CCMW::GetDeviceIdentification(std::string& strIdentification)
{
    if (! GPIB_Query(std::string("SYSTem:DEVice:ID?"), strIdentification))
        return false;

    return true ;
}


// -------------------- Reset functions --------------------

bool CCMW::Reset()
{
	if (! GPIB_OPComplete(std::string("SYSTem:RESet; *OPC?")))
		return false;

	return true;
}

bool CCMW::Preset()
{
	if (! GPIB_OPComplete(std::string("SYSTem:PRESet; *OPC?")))
		return false;

	return true;
}


bool CCMW::QueryRelIndicator(int iIndicator, std::string& strMsg)
{
    switch (iIndicator)
    {
    case 0:
        strMsg = "OK" ; break ;
    case 1:
        strMsg = "Measurement Timeout" ; break ;
    case 2:
        strMsg = "Capture Buffer Overflow" ; break ;
    case 3:
        strMsg = "Overdriven" ; break ;
    case 4:
        strMsg = "Underdriven" ; break ;
    case 6:
        strMsg = "Trigger Timeout" ; break ;
    case 7:
        strMsg = "Acquisition Error" ; break ;
    case 8:
        strMsg = "Sync Error" ; break ;
    case 9:
        strMsg = "Uncal" ; break ;
    case 15:
        strMsg = "Reference Frequency Error"; break ;
    case 16:
        strMsg = "RF Not Available"; break ;
    case 17:
        strMsg = "RF Level not Settled" ; break ;
    case 18:
        strMsg = "Frequency not Settled" ; break ;
    case 19:
        strMsg = "Call not Established" ; break ;
    case 20:
        strMsg = "Call Type not Usable" ; break ;
    case 21:
        strMsg = "Call Lost" ; break ;
    case 23:
        strMsg = "Missing Option" ; break ;
    case 27:
        strMsg = "No Sensor Connected" ; break ;
    case 40:
        strMsg = "ARB File CRC Error" ; break ;
    case 42:
        strMsg = "ARB Header Tag Invalid" ; break ;
    case 43:
        strMsg = "Segment Overflow"; break ;
    case 44:
        strMsg = "ARB File not Found"; break ;
    case 50:
        strMsg = "Startup Error" ; break ;
    case 51:
        strMsg = "No Reply" ; break ;
    case 52:
        strMsg = "Connection Error" ; break ;
    case 53:
        strMsg = "Configuration Error" ; break ;
    case 54:
        strMsg = "File system Error" ; break ;
    case 101:
        strMsg = "Firmware Error" ; break ;
    case 102:
        strMsg = "Unidentified Error" ; break ;
    case 103:
        strMsg = "Parameter Error" ; break ;
    default :
        return false ;
    }

    return true ;
}

// ----------------- Set RF port functions -----------------

bool CCMW::SetInputPort(std::string strFirmware, std::string strScenario, std::string strInputPort)
{
	char szCmd[256];
	memset(szCmd, 0, 256);
	sprintf_s(szCmd, 256, "ROUTe:%s:MEASurement:SCENario:%s %s, RX1; *OPC?",
		strFirmware.c_str(), strScenario.c_str(), strInputPort.c_str());

	if (! GPIB_OPComplete(std::string(szCmd)))
		return false;

	return true;
}

bool CCMW::SetOutputPort(std::string strFirmware, std::string strScenario, std::string strInputPort)
{
	char szCmd[256];
	memset(szCmd, 0, 256);
	sprintf_s(szCmd, 256, "ROUTe:%s:GENerator:SCENario:%s %s, TX1; *OPC?",
		strFirmware.c_str(), strScenario.c_str(), strInputPort.c_str());
 
	if (! GPIB_OPComplete(std::string(szCmd)))
		return false;

	return true;
}

bool CCMW::SetInputPort(std::string strFirmware, std::string strScenario)
{
	return SetInputPort(strFirmware, strScenario, g_strRFInputPort);
}

bool CCMW::SetOutputPort(std::string strFirmware, std::string strScenario)
{
	return SetOutputPort(strFirmware, strScenario, g_strRFOutputPort);
}

bool CCMW::SetInputPort(std::string strFirmware)
{
	return SetInputPort(strFirmware, "SALone", g_strRFInputPort);
}

bool CCMW::SetOutputPort(std::string strFirmware)
{
	return SetOutputPort(strFirmware, "SALone", g_strRFOutputPort);
}

bool CCMW::SetInputPort(int iMode, int iPort1)
{
	return false;
}

bool CCMW::SetOutputPort(int iMode, int iPort)
{
	return false;
}

// ---------------- Set cable loss functions ---------------

bool CCMW::DeactivateLossTable(std::string strPort)
{
	char szCmd[256];
	memset(szCmd, 0, 256);
	sprintf_s(szCmd, 256, "CONFigure:FDCorrection:DEACtivate %s; *OPC?", strPort.c_str());

	if (! GPIB_OPComplete(std::string(szCmd)))
		return false;

	return true;
}

bool CCMW::DeactivateLossTable()
{
	return (DeactivateLossTable(g_strRFInputPort) && DeactivateLossTable(g_strRFOutputPort));
}

bool CCMW::DeleteAllLossTable()
{
	// 1. Check existing tables
	char szCmd[256];
	std::string strResponse;

	memset(szCmd, 0, 256);
	sprintf_s(szCmd, 256, "CONFigure:BASE:FDCorrection:CTABle:CATalog?");
	if (! GPIB_Query(std::string(szCmd), strResponse))
		return false;

	// 2. Delete all tables
	std::vector<std::string> vec_strTmp;
	vec_strTmp.clear();

	if (strstr(strResponse.c_str(), ",") != NULL)
	{
		STDStringTokenizer(strResponse, ",", vec_strTmp);
	}
	else
	{
		if (! strResponse.empty())
		{
			vec_strTmp.push_back(strResponse);
		}
	}

	for (unsigned int i = 0; i < vec_strTmp.size(); ++i)
	{
		memset(szCmd, 0, 256);
		sprintf_s(szCmd, 256, "CONFigure:BASE:FDCorrection:CTABle:DELete %s; *OPC?", vec_strTmp[i].c_str());
		if (! GPIB_OPComplete(std::string(szCmd)))
			return false;
	}

	return true;
}

bool CCMW::SetMeasureLoss(std::string strPort, std::string strFirmware, double dLoss)
{
	char szCmd[256];

	// 1. Deactivate loss table
	if (! DeactivateLossTable(strPort))
	{
		return false;
	}

	// 2. Set loss by firmware
	memset(szCmd, 0, 256);
	sprintf_s(szCmd, 256, "CONFigure:%s:MEASurement:RFSettings:EATTenuation %.02f; *OPC?", strFirmware.c_str(), dLoss);

	if (! GPIB_OPComplete(std::string(szCmd)))
		return false;

	return true;
}

bool CCMW::SetMeasureLoss(std::string strFirmware, double dLoss)
{
	return SetMeasureLoss(g_strRFInputPort, strFirmware, dLoss);
}

bool CCMW::SetGeneratorLoss(std::string strPort, std::string strFirmware, double dLoss)
{
	char szCmd[256];

	// 1. Deactivate loss table
	if (! DeactivateLossTable(strPort))
	{
		return false;
	}

	// 2. Set loss by firmware
	memset(szCmd, 0, 256);
	sprintf_s(szCmd, 256, "SOURce:%s:GENerator:RFSettings:EATTenuation %.02f; *OPC?", strFirmware.c_str(), dLoss);

	if (! GPIB_OPComplete(std::string(szCmd)))
		return false;

	return true;
}

bool CCMW::SetGeneratorLoss(std::string strFirmware, double dLoss)
{
	return SetGeneratorLoss(g_strRFOutputPort, strFirmware, dLoss);
}

bool CCMW::SetMeasureLossTable(std::string strPort, std::vector<unsigned int> vec_uiFreq, std::vector<double> vec_dLoss)
{
	char szCmd[4096];
	int iSize;
	std::string strTable;

	// 1. Create a new Rx table
	if (vec_uiFreq.size() > vec_dLoss.size())
		iSize = vec_dLoss.size();
	else
		iSize = vec_uiFreq.size();

	if (iSize <= 0)
	{
		CmdLog("Illegal loss table size, table size <= 0");
		return false;
	}

	strTable.clear();
	for (int i = 0; i < iSize; ++i)
	{
		char szTmp[256];
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 256, ", %u, %.02f", vec_uiFreq[i], vec_dLoss[i]);

		strTable = strTable + std::string(szTmp);
	}

	memset(szCmd, 0, 4096);
	sprintf_s(szCmd, 4096, "CONFigure:BASE:FDCorrection:CTABle:CREate 'Rx%s'%s; *OPC?", strPort.c_str(), strTable.c_str());
	if (! GPIB_OPComplete(std::string(szCmd)))
		return false;

	// 2. Activate the Rx table
	memset(szCmd, 0, 4096);
	sprintf_s(szCmd, 4096, "CONFigure:FDCorrection:ACTivate %s, 'Rx%s', RX; *OPC?", strPort.c_str(), strPort.c_str());
	if (! GPIB_OPComplete(std::string(szCmd)))
		return false;

	return true;
}

bool CCMW::SetGeneratorLossTable(std::string strPort, std::vector<unsigned int> vec_uiFreq, std::vector<double> vec_dLoss)
{
	char szCmd[4096];
	int iSize;
	std::string strTable;

	// 1. Create a new Tx table
	if (vec_uiFreq.size() > vec_dLoss.size())
		iSize = vec_dLoss.size();
	else
		iSize = vec_uiFreq.size();

	if (iSize <= 0)
	{
		CmdLog("Illegal loss table size, table size <= 0");
		return false;
	}

	strTable.clear();
	for (int i = 0; i < iSize; ++i)
	{
		char szTmp[256];
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 256, ", %u, %.02f", vec_uiFreq[i], vec_dLoss[i]);

		strTable = strTable + std::string(szTmp);
	}

	memset(szCmd, 0, 4096);
	sprintf_s(szCmd, 4096, "CONFigure:BASE:FDCorrection:CTABle:CREate 'Tx%s'%s; *OPC?", strPort.c_str(), strTable.c_str());
	if (! GPIB_OPComplete(std::string(szCmd)))
		return false;

	// 2. Activate the Tx table
	memset(szCmd, 0, 4096);
	sprintf_s(szCmd, 4096, "CONFigure:FDCorrection:ACTivate %s, 'Tx%s', TX; *OPC?", strPort.c_str(), strPort.c_str());
	if (! GPIB_OPComplete(std::string(szCmd)))
		return false;

	return true;
}

bool CCMW::SetMeasureLossTable(std::vector<unsigned int> vec_uiFreq, std::vector<double> vec_dLoss)
{
	return SetMeasureLossTable(g_strRFInputPort, vec_uiFreq, vec_dLoss);
}

bool CCMW::SetGeneratorLossTable(std::vector<unsigned int> vec_uiFreq, std::vector<double> vec_dLoss)
{
	return SetGeneratorLossTable(g_strRFOutputPort, vec_uiFreq, vec_dLoss);
}


// -------------------- GPRF functions ---------------------

bool CCMW::SetGPRFGeneratorPort(std::string strPort)
{
	return SetOutputPort("GPRF", "SALone", strPort);
}

bool CCMW::SetGPRFMeasurementPort(std::string strPort)
{
	return SetInputPort("GPRF", "SALone", strPort);
}

bool CCMW::SetCurrentMeasurementPort(std::string strFirmware)
{
    return SetInputPort(strFirmware, "SALone", g_strRFOutputPort);
}

bool CCMW::SetGPRFGeneratorPort()
{
	return SetGPRFGeneratorPort(g_strRFOutputPort);
}

bool CCMW::SetGPRFMeasurementPort()
{
	return SetGPRFMeasurementPort(g_strRFInputPort);
}

bool CCMW::SetGPRFFixedMeasureLoss(double dLoss)
{
	return SetMeasureLoss("GPRF", dLoss);
}

bool CCMW::SetGPRFFixedGeneratorLoss(double dLoss)
{
	return SetGeneratorLoss("GPRF", dLoss);
}

bool CCMW::SetGPRFGeneratorBBMode(std::string strMode)
{
	char szCmd[256];
	memset(szCmd, 0, 256);
	sprintf_s(szCmd, 256, "SOURce:GPRF:GENerator:BBMode %s; *OPC?", strMode.c_str());

	if (! GPIB_OPComplete(std::string(szCmd)))
		return false;

	return true;
}

bool CCMW::SetGPRFGeneratorPlayARB (std::string strArbFile)
{
	if (! GPIB_OPComplete ("SOUR:GPRF:GEN:BBM ARB; *OPC?"))
		return false ;

	std::string strTemp = "SOUR:GPRF:GEN:ARB:FILE '" + strArbFile + "';*OPC?";
	if (! GPIB_OPComplete (strTemp))
		return false ;

	if (! GPIB_Query ("SOUR:GPRF:GEN:ARB:FILE:DATE?", strTemp))
		return false ;

	//if (strTemp == "\"\"")
	if (strTemp == "NAV")
		return false ;

	return true ;
}

bool CCMW::SetGPRFGeneratorFrequency(unsigned int uiFrequency)
{
	char szCmd[256];
	memset(szCmd, 0, 256);
	sprintf_s(szCmd, 256, "SOURce:GPRF:GENerator:RFSettings:FREQuency %u; *OPC?", uiFrequency);

	if (! GPIB_OPComplete(std::string(szCmd)))
		return false;

	return true;
}

bool CCMW::SetGPRFMeasurementFrequency(unsigned int uiFrequency)
{
	char szCmd[256];
	memset(szCmd, 0, 256);
	sprintf_s(szCmd, 256, "CONFigure:GPRF:MEASurement:RFSettings:FREQuency %u; *OPC?", uiFrequency);

	if (! GPIB_OPComplete(std::string(szCmd)))
		return false;

	return true;
}

bool CCMW::SetGPRFGeneratorLevel(double dLevel)
{
	char szCmd[256];
	memset(szCmd, 0, 256);
	sprintf_s(szCmd, 256, "SOURce:GPRF:GENerator:RFSettings:LEVel %.02f; *OPC?", dLevel);

	if (! GPIB_OPComplete(std::string(szCmd)))
		return false;

	return true;
}

bool CCMW::SetGPRFGeneratorOnOff(bool bOn)
{
	char szCmd[256];
	memset(szCmd, 0, 256);

	if (bOn)
		sprintf_s(szCmd, 256, "SOURce:GPRF:GENerator:STATe ON; *OPC?");
	else
		sprintf_s(szCmd, 256, "SOURce:GPRF:GENerator:STATe OFF; *OPC?");

	if (! GPIB_OPComplete(std::string(szCmd)))
		return false;

	return true;
}

bool CCMW::GetARBWaveform(bool bIsAutoSearch, std::string& strArbFile, std::string& strArbFilePath)
{
	bool bRet = false;
	std::string strTemp;
	std::vector<std::string> vec_strTmp;
	vec_strTmp.push_back("D:\\Rohde-Schwarz\\CMW\\Data\\waveform");	

	if (bIsAutoSearch)
	{
		vec_strTmp.push_back("D:\\Rohde-Schwarz\\CMW\\Data\\waveform\\library1");

		for (unsigned int i = 0; i < vec_strTmp.size(); i++)
		{
			strTemp = vec_strTmp[i] + "\\" + strArbFile;
			vec_strTmp.at(i) = strTemp;
		}
	}
	else
	{
		if ( PathIsRelativeA(strArbFile.c_str()) )
			strTemp = vec_strTmp[0] + "\\" + strArbFile;
		else if ( '\\' == strArbFile[0] )
			strTemp = vec_strTmp[0] + strArbFile;
		else
			strTemp = strArbFile;

		vec_strTmp.at(0) = strTemp;
	}

	for (unsigned int i = vec_strTmp.size() - 1; i >= 0; --i)
	{
		strTemp = "SOUR:GPRF:GEN:ARB:FILE '" + vec_strTmp.at(i) + "';*OPC?";
		if (! GPIB_OPComplete(strTemp))
			continue;

		if (! GPIB_Query("SOUR:GPRF:GEN:ARB:FILE:DATE?", strTemp))
			continue;

		if ( "NAV" != strTemp )
		{
			bRet = true;
			strArbFilePath = vec_strTmp.at(i);
			break;
		}
	}

	return bRet;
}

bool CCMW::SetExpectedPower(std::string strFirmware, double dENPower, double dUMARgin)
{
    char szCmd[256] ;
    std::string strResponse ;
    double dLoss ;

    memset(szCmd, 0, 256) ;
    sprintf_s(szCmd, 256, "CONFigure:%s:MEAS:RFSettings:EATTenuation?", strFirmware.c_str()) ;
    if (! GPIB_Query(szCmd, strResponse) )
        return false ;
    dLoss = atof( strResponse.c_str() ) ;
    
    if ( dENPower < -47.0 )
        dENPower = -47.0 + dLoss ;
    else if ( dENPower > 34.0 )  // FW 3.0.10 limit up to +55.
        dENPower = 34.0 + dLoss ;

    // Calculate User Margin Limit
    double dUMARgin_Upper = 90 - dENPower ;

    if ( dUMARgin > dUMARgin_Upper )
        dUMARgin = dUMARgin_Upper ;
    else if ( dUMARgin_Upper < 0 )
        dUMARgin = 0 ;

    memset(szCmd, 0, 256) ;
    sprintf_s(szCmd, 256, "CONFigure:%s:MEAS:RFSettings:UMARgin %.02f; *OPC?", strFirmware.c_str(), dUMARgin) ;
    if (! GPIB_OPComplete(std::string(szCmd)) )
        return false ;

    memset(szCmd, 0, 256) ;
    sprintf_s(szCmd, 256, "CONFigure:%s:MEAS:RFSettings:ENPower %.02f; *OPC?", strFirmware.c_str(), dENPower) ;
    if (! GPIB_OPComplete(std::string(szCmd)) )
        return false ;

    return true ;
}

bool CCMW::SetMeasurementFrequency(std::string strFirmware, unsigned int uiFrequency)
{
    char szCmd[256];
    memset(szCmd, 0, 256);
    sprintf_s(szCmd, 256, "CONFigure:%s:MEASurement:RFSettings:FREQuency %u; *OPC?", strFirmware.c_str(), uiFrequency);

    if (! GPIB_OPComplete(std::string(szCmd)))
        return false;

    return true;
}

bool CCMW::SetGeneratorFrequency(std::string strFirmware, unsigned int uiFrequency)
{
    char szCmd[256];
    memset(szCmd, 0, 256);
    sprintf_s(szCmd, 256, "SOURce:%s:GENerator:RFSettings:FREQuency %u; *OPC?", strFirmware.c_str(), uiFrequency);

    if (! GPIB_OPComplete(std::string(szCmd)))
        return false;

    return true;
}

bool CCMW::SetGSMMeasurementBand (int band, bool bSig)
{
	std::string strCmd;

	switch (band)
	{
		case 850:
			strCmd = "CONF:GSM:MEAS:BAND G085; *OPC?";
			break;
		case 900:
			strCmd = "CONF:GSM:MEAS:BAND G09; *OPC?";
			break;
		case 1800:
			strCmd = "CONF:GSM:MEAS:BAND G18; *OPC?";
			break;
		case 1900:
			strCmd = "CONF:GSM:MEAS:BAND G19; *OPC?";
			break;
		default:
			return false;
	}

	return GPIB_OPComplete (strCmd);
}
