#include "stdafx.h"
#include "CMU200.h"

using namespace StringUtility;
using namespace GSMUtility;

GP_IMPLEMENT_DYNCREATE(CCMU200)

// ------------------- Initial functions -------------------

bool CCMU200::Initial()
{
	if (! GPIB_OPComplete("*CLS; *OPC?"))
		return false;

	if (! GPIB_OPComplete("SYSTem:NONV:DISable; *OPC?"))
		return false;

	if (! GPIB_OPComplete("TRACe:REMote:MODE:DISPlay ON; *OPC?"))
		return false;

	if (! GPIB_OPComplete("CONFigure:SYNC:FREQ:REF:MODE INT; *OPC?"))
		return false;

	if (! GPIB_OPComplete("SYSTem:REMote:RDMode ON; *OPC?"))
		return false;

	return true;
}

bool CCMU200::Initial_GSM()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	// 900 Non-signaling
	sprintf_s(szCommand, 255, MAP_COMMAND, _GSM900MS_NSIG, IDSTR_GSM900MS_NSIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	if (! SetInputPort(_GSM900MS_NSIG))
		return false;
	if (! SetOutputPort(_GSM900MS_NSIG))
		return false;

	// 900 Signaling
	sprintf_s(szCommand, 255, MAP_COMMAND, _GSM900MS_SIG, IDSTR_GSM900MS_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	if (! SetInputPort(_GSM900MS_SIG))
		return false;
	if (! SetOutputPort(_GSM900MS_SIG))
		return false;

	// 1800 Non-signaling
	sprintf_s(szCommand, 255, MAP_COMMAND, _GSM1800MS_NSIG, IDSTR_GSM1800MS_NSIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	if (! SetInputPort(_GSM1800MS_NSIG))
		return false;
	if (! SetOutputPort(_GSM1800MS_NSIG))
		return false;

	// 1800 Signaling
	sprintf_s(szCommand, 255, MAP_COMMAND, _GSM1800MS_SIG, IDSTR_GSM1800MS_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	if (! SetInputPort(_GSM1800MS_SIG))
		return false;
	if (! SetOutputPort(_GSM1800MS_SIG))
		return false;

	// 1900 Non-signaling
	sprintf_s(szCommand, 255, MAP_COMMAND, _GSM1900MS_NSIG, IDSTR_GSM1900MS_NSIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	if (! SetInputPort(_GSM1900MS_NSIG))
		return false;
	if (! SetOutputPort(_GSM1900MS_NSIG))
		return false;

	// 1900 Signaling
	sprintf_s(szCommand, 255, MAP_COMMAND, _GSM1900MS_SIG, IDSTR_GSM1900MS_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	if (! SetInputPort(_GSM1900MS_SIG))
		return false;
	if (! SetOutputPort(_GSM1900MS_SIG))
		return false;

	// 850 Non-signaling
	sprintf_s(szCommand, 255, MAP_COMMAND, _GSM850MS_NSIG, IDSTR_GSM850MS_NSIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	if (! SetInputPort(_GSM850MS_NSIG))
		return false;
	if (! SetOutputPort(_GSM850MS_NSIG))
		return false;

	// 850 Signaling
	sprintf_s(szCommand, 255, MAP_COMMAND, _GSM850MS_SIG, IDSTR_GSM850MS_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	if (! SetInputPort(_GSM850MS_SIG))
		return false;
	if (! SetOutputPort(_GSM850MS_SIG))
		return false;

	return true;
}

// --------------- Get Options of Test Device --------------

bool CCMU200::GetOptions(std::string& strOptions)
{
	return GPIB_Query("*OPT?", strOptions);
}


// -------------------- Reset functions --------------------

bool CCMU200::Reset()
{
	return GPIB_OPComplete("*RST; *OPC?");
}

bool CCMU200::Preset()
{
	return Reset();
}


// ----------------- Set RF port functions -----------------

bool CCMU200::SetInputPort(int iMode, int iPort)
{
	if (-1 == iMode)
		iMode = g_iMode;
	if (-1 == iPort)
		iPort = atoi(g_strRFInputPort.c_str());

	std::string strResponse, strCommand;
	strCommand  = MakeSTDstring ("%d;INPut RF%d; *OPC?", iMode, iPort);

	if (! GPIB_Query(strCommand, strResponse))
		return false;
	if (strstr(strResponse.c_str(), "1") == NULL)
		return false;

	return true;
}

bool CCMU200::SetOutputPort(int iMode, int iPort)
{
	if (-1 == iMode)
		iMode = g_iMode;
	if (-1 == iPort)
		iPort = atoi(g_strRFOutputPort.c_str());

	std::string strResponse, strCommand;
	strCommand  = MakeSTDstring ("%d;OUTPut RF%d; *OPC?", iMode, iPort);

	if (! GPIB_Query(strCommand, strResponse))
		return false;
	if (strstr(strResponse.c_str(), "1") == NULL)
		return false;

	return true;
}

bool CCMU200::Initial_RF_Nonsignaling()
{
	g_iMode = _RF_NSIG;
	std::string strCommand = MakeSTDstring (MAP_COMMAND, _RF_NSIG, IDSTR_RF_NSIG);
	if (! GPIB_Write(strCommand))
		return false;

	if (! SetInputPort(_RF_NSIG))
		return false;
	if (! SetOutputPort(_RF_NSIG))
		return false;

	return true;
}

// ----------------- GPRF functions --> Basic Functions / RF -----------------

bool CCMU200::SetGPRFGeneratorPort(std::string strPort)
{
	int port = atoi (strPort.c_str());
	if (! GPIB_Write(MakeSTDstring (MAP_COMMAND, port, IDSTR_RF_NSIG)))
		return false;

	if (! SetOutputPort(port))
		return false;

	return true;
}

bool CCMU200::SetGPRFMeasurementPort(std::string strPort)
{
	int port = atoi (strPort.c_str());

	if (! GPIB_Write(MakeSTDstring (MAP_COMMAND, port, IDSTR_RF_NSIG)))
		return false;

	if (! SetInputPort(port))
		return false;

	return true;
}

bool CCMU200::SetGPRFGeneratorPort()
{
	return Initial_RF_Nonsignaling();
}

bool CCMU200::SetGPRFMeasurementPort()
{
	return Initial_RF_Nonsignaling();
}

bool CCMU200::SetGPRFGeneratorFrequency(unsigned int uiFrequency)
{
	double dFreq = uiFrequency / 1e6;
	std::string strCmd = MakeSTDstring ("%d;SOURce:RFGenerator:FREQuency %.4f MHZ;*OPC?", g_iMode, dFreq);

	return GPIB_OPComplete (strCmd);
}

bool CCMU200::SetGPRFMeasurementFrequency(unsigned int uiFrequency)
{
	double dFreq = uiFrequency / 1e6;
	std::string strCmd = MakeSTDstring ("%d;RFAN:FREQ %.4f MHZ;*OPC?", g_iMode, dFreq);

	return GPIB_OPComplete (strCmd);
}

bool CCMU200::RF_SetGeneratorFreq(double dFreqMHz)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	// SOURce:RFGenerator[:TX]:FREQuency
	sprintf_s(szCommand, 255, "%d;SOURce:RFGenerator:FREQuency %4.2f MHZ;*OPC?", g_iMode, dFreqMHz);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::SetGPRFFixedMeasureLoss(double dLoss)
{
	if (! GPIB_OPComplete ("0;CONFigure:USER:CORRection:LOSS:TABLe:ENABle OFF;*OPC?"))
		return false;

	int port = atoi(g_strRFInputPort.c_str());
	std::string strCmd = MakeSTDstring ("%d;SOURce:CORRection:LOSS:INPut%d %.3f;*OPC?", g_iMode, port, dLoss);
	if (! GPIB_OPComplete (strCmd))
		return false;

	return true;
}

bool CCMU200::SetGPRFFixedGeneratorLoss(double dLoss)
{
	if (! GPIB_OPComplete ("0;CONFigure:USER:CORRection:LOSS:TABLe:ENABle OFF;*OPC?"))
		return false;

	int port = atoi(g_strRFOutputPort.c_str());
	std::string strCmd = MakeSTDstring ("%d;SOURce:CORRection:LOSS:OUTPut%d %.3f;*OPC?", g_iMode, port, dLoss);
	if (! GPIB_OPComplete (strCmd))
		return false;

	return true;
}

bool CCMU200::SetGPRFGeneratorLevel(double dLevel)
{
	return GPIB_OPComplete (MakeSTDstring ("%d;SOUR:RFG:LEV:UTIM %3.1f; *OPC?", g_iMode, dLevel));
}

bool CCMU200::SetGPRFGeneratorOnOff(bool bOn)
{
	std::string strCmd;
	
	if (bOn)
		strCmd = MakeSTDstring ("%d;INIT:RFG; *OPC?", g_iMode);
	else
		strCmd = MakeSTDstring ("%d;ABOR:RFG; *OPC?", g_iMode);

	return GPIB_OPComplete (strCmd);
}


bool  CCMU200::SetGSMMeasurementBand (int band, bool bSig)
{
	std::string strCommand, strID;
	if (bSig)
	{
		switch (band)
		{
			case 850 : strID = IDSTR_GSM850MS_SIG; break;
			case 900 : strID = IDSTR_GSM900MS_SIG; break;
			case 1800: strID = IDSTR_GSM1800MS_SIG; break;
			case 1900: strID = IDSTR_GSM1900MS_SIG; break;
		}
	}
	else
	{
		switch (band)
		{
			case 850 : strID = IDSTR_GSM850MS_NSIG; break;
			case 900 : strID = IDSTR_GSM900MS_NSIG; break;
			case 1800: strID = IDSTR_GSM1800MS_NSIG; break;
			case 1900: strID = IDSTR_GSM1900MS_NSIG; break;
		}
	}

	g_iMode = GSM_GetCMUMode (band, bSig);

	strCommand = MakeSTDstring ("0;"MAP_COMMAND, g_iMode, strID.c_str());
	if (! GPIB_Write(strCommand))
		return false;
	if (! SetInputPort (g_iMode))
		return false;
	if (! SetOutputPort (g_iMode))
		return false;

	return true;
}

bool CCMU200::RF_InitialAnalyzer()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;ABOR:RFAN; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;TRIG:SOUR IMM; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;TRIG:SLOP POS; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONF:NPOW:CONT:STATistics 10; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;LEV:MODE AUTomatic; *OPC?; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::RF_SetGeneratorPowerLevel(double dLevel)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	// SOURce:RFGenerator[:TX]:LEVel
	sprintf_s(szCommand, 255, "%d;SOURce:RFGenerator:TX:LEVel %4.3f ;*OPC?", g_iMode, dLevel);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::RF_TurnOnOffGernerator(bool bOn)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	if (bOn)
		sprintf_s(szCommand, 255, "%d;INITiate:RFGenerator; *OPC?", g_iMode);
	else
		sprintf_s(szCommand, 255, "%d;ABORt:RFGenerator; *OPC?", g_iMode);

	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::RF_SetModulationMode(std::string strMode)
{
	std::string strCommand;

	if (
		_stricmp("off",const_cast<char*>(strMode.c_str()))!=0 && 
		_stricmp("SSB",const_cast<char*>(strMode.c_str()))!=0 &&
		_stricmp("AM",const_cast<char*>(strMode.c_str()))!=0 &&
		_stricmp("FM",const_cast<char*>(strMode.c_str()))!=0 &&
		_stricmp("FMST",const_cast<char*>(strMode.c_str()))!=0
		)
	{
		return false;
	}

	transform(strMode.begin(),strMode.end(),strMode.begin(),toupper);
	strCommand=MakeSTDstring("%d",g_iMode)+";SOURce:RFGenerator:MODulation "+strMode+"; *OPC?";

	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::RF_SetAnalyzerFreq(double dFreqMHz)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;RFANalyzer:FREQuency %4.4f MHZ; *OPC?", g_iMode, dFreqMHz);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::RF_NPowerMeasurement(int iStatistics, std::string& strMeasured)
{
	bool bFetch = false;
	int iRetry = 50;

	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;CONFigure:NPOWer:CONTrol %d,SINGleshot,NONE,NONE; *OPC?", g_iMode, iStatistics);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;INITiate:NPOWer; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;FETCh:NPOWer:STATus?", g_iMode);
	strCommand = szCommand;
	do
	{
		if (! GPIB_Query(strCommand, strResponse))
			return false;

		if (strstr(strResponse.c_str(), "RDY") != NULL)
		{
			bFetch = true;
			break;
		}

		Sleep(500);
		--iRetry;
	} while (iRetry > 0);

	if (! bFetch)
		return false;

	sprintf_s(szCommand, 255, "%d;FETCh:SCALar:NPOWer?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_Query(strCommand, strMeasured))
		return false;

	sprintf_s(szCommand, 255, "%d;ABORt:NPOWer; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}
