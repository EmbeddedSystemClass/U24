#include "stdafx.h"
#include "CCMU200.h"
#include "../../CommonUtility/CommonFunction.h"


// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CCMU200)


// ------------------- Initial functions -------------------

bool CCMU200::Initial()
{
	if (! GPIB_OPComplete(std::string("*CLS; *OPC?")))
		return false;

	if (! GPIB_OPComplete(std::string("SYSTem:NONV:DISable; *OPC?")))
		return false;

	if (! GPIB_OPComplete(std::string("TRACe:REMote:MODE:DISPlay ON; *OPC?")))
		return false;

	if (! GPIB_OPComplete(std::string("CONFigure:SYNC:FREQ:REF:MODE INT; *OPC?")))
		return false;

	if (! GPIB_OPComplete(std::string("SYSTem:REMote:RDMode ON; *OPC?")))
		return false;

	return true;
}

bool CCMU200::Initial_RF_Nonsignaling()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, MAP_COMMAND, _RF_NSIG, IDSTR_RF_NSIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	if (! SetInputPort(_RF_NSIG))
		return false;
	if (! SetOutputPort(_RF_NSIG))
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

bool CCMU200::Initial_WCDMA()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	// WCDMA Non-signaling
	sprintf_s(szCommand, 255, MAP_COMMAND, _WCDMA19UEFDD_NSIG, IDSTR_WCDMA19UEFDD_NSIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	if (! SetInputPort(_WCDMA19UEFDD_NSIG))
		return false;
	if (! SetOutputPort(_WCDMA19UEFDD_NSIG))
		return false;

	// WCDMA Signaling
	sprintf_s(szCommand, 255, MAP_COMMAND, _WCDMA19UEFDD_SIG, IDSTR_WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	if (! SetInputPort(_WCDMA19UEFDD_SIG))
		return false;
	if (! SetOutputPort(_WCDMA19UEFDD_SIG))
		return false;

	return true;
}

bool CCMU200::SetMaxPower(double dMaxPower)
{
    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand;

    sprintf_s(szCommand, 255, "%d;LEVel:MODE MANual;:LEVel:MAXimum %4.1f; *OPC?", g_iMode,dMaxPower);
    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true ;
}

// -------------- Get Options of Test Devices --------------

bool CCMU200::GetDeviceOptions(std::string& strOptions)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;*OPT?", g_iMode);
	strCommand = szCommand;

	if (! GPIB_Querry(strCommand, strOptions))
		return false;

	return true;
}


// -------------------- Reset functions --------------------

bool CCMU200::Reset()
{
	if (! GPIB_OPComplete(std::string("*RST; *OPC?")))
		return false;

	return true;
}


// ----------------- Set RF port functions -----------------

bool CCMU200::SetInputPort(int iMode, int iPort)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;INPut RF%d; *OPC?", iMode, iPort);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strResponse))
		return false;
	if (strstr(strResponse.c_str(), "1") == NULL)
		return false;

	return true;
}

bool CCMU200::SetOutputPort(int iMode, int iPort)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;OUTPut RF%d; *OPC?", iMode, iPort);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strResponse))
		return false;
	if (strstr(strResponse.c_str(), "1") == NULL)
		return false;

	return true;
}

bool CCMU200::SetInputPort(int iMode)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;INPut RF%d; *OPC?", iMode, g_iRFInputPort);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strResponse))
		return false;
	if (strstr(strResponse.c_str(), "1") == NULL)
		return false;

	return true;
}

bool CCMU200::SetOutputPort(int iMode)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;OUTPut RF%d; *OPC?", iMode, g_iRFOutputPort);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strResponse))
		return false;
	if (strstr(strResponse.c_str(), "1") == NULL)
		return false;

	return true;
}

bool CCMU200::SetInputPort()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;INPut RF%d; *OPC?", g_iMode, g_iRFInputPort);
	strCommand = szCommand;
	if (GPIB_OPComplete(strCommand))
		return true;

	return false;
}

bool CCMU200::SetOutputPort()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;OUTPut RF%d; *OPC?", g_iMode, g_iRFOutputPort);
	strCommand = szCommand;
	if (GPIB_OPComplete(strCommand))
		return true;

	return false;
}


// --------------- Set cable loss functions ----------------

bool CCMU200::SetInputLoss(int iMode, int iPort, double dLoss)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	if (! GPIB_Write(std::string("0;CONFigure:USER:CORRection:LOSS:TABLe:ENABle OFF")))
		return false;

	sprintf_s(szCommand, 255, "%d;SOURce:CORRection:LOSS:INPUT%d %8.3f; *OPC?", iMode, iPort, dLoss);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strResponse))
		return false;
	if (strstr(strResponse.c_str(), "1") == NULL)
		return false;

	return true;
}

bool CCMU200::SetOutputLoss(int iMode, int iPort, double dLoss)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	if (! GPIB_Write(std::string("0;CONFigure:USER:CORRection:LOSS:TABLe:ENABle OFF")))
		return false;

	sprintf_s(szCommand, 255, "%d;SOURce:CORRection:LOSS:OUTPUT%d %8.3f; *OPC?", iMode, iPort, dLoss);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strResponse))
		return false;
	if (strstr(strResponse.c_str(), "1") == NULL)
		return false;

	return true;
}

bool CCMU200::SetInputLoss(int iMode, double dLoss)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	if (! GPIB_Write(std::string("0;CONFigure:USER:CORRection:LOSS:TABLe:ENABle OFF")))
		return false;

	sprintf_s(szCommand, 255, "%d;SOURce:CORRection:LOSS:INPUT%d %8.3f; *OPC?", iMode, g_iRFInputPort, dLoss);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strResponse))
		return false;
	if (strstr(strResponse.c_str(), "1") == NULL)
		return false;

	return true;
}

bool CCMU200::SetOutputLoss(int iMode, double dLoss)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	if (! GPIB_Write(std::string("0;CONFigure:USER:CORRection:LOSS:TABLe:ENABle OFF")))
		return false;

	sprintf_s(szCommand, 255, "%d;SOURce:CORRection:LOSS:OUTPUT%d %8.3f; *OPC?", iMode, g_iRFOutputPort, dLoss);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strResponse))
		return false;
	if (strstr(strResponse.c_str(), "1") == NULL)
		return false;

	return true;
}

bool CCMU200::SetInputLoss(double dLoss)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	if (! GPIB_Write(std::string("0;CONFigure:USER:CORRection:LOSS:TABLe:ENABle OFF")))
		return false;

	sprintf_s(szCommand, 255, "%d;SOURce:CORRection:LOSS:INPUT%d %8.3f; *OPC?", g_iMode, g_iRFInputPort, dLoss);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::SetOutputLoss(double dLoss)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	if (! GPIB_Write(std::string("0;CONFigure:USER:CORRection:LOSS:TABLe:ENABle OFF")))
		return false;

	sprintf_s(szCommand, 255, "%d;SOURce:CORRection:LOSS:OUTPUT%d %8.3f; *OPC?", g_iMode, g_iRFOutputPort, dLoss);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::TurnOnOffLossTable(bool bOnOfff)
{
	std::string strCmd="0;CONFigure:USER:CORRection:LOSS:TABLe:ENABle ";
	if(bOnOfff)
		strCmd=strCmd+" ON";
	else
		strCmd=strCmd+" OFF";
	
	if (! GPIB_Write(strCmd))
		return false;

	return true;

}

bool CCMU200::SetLossTable(int iNumOfFreqs,double *dFreqs,double *dLoss_RF1,double *dLoss_RF2,double *dLoss_RF3,double *dLoss_RF4)
{
	std::string strCmd="0;CONFigure:USER:CORRection:LOSS:TABLe:";
	std::string	strLine="";
	
	for(int i=0;i<iNumOfFreqs;i++)
		strLine=strLine+"Line"+MakeSTDstring("%d",i+1)+" "+MakeSTDstring("%f",dFreqs[i])+"MHZ,"+MakeSTDstring("%f",dLoss_RF1[i])+","+MakeSTDstring("%f",dLoss_RF2[i])+","+MakeSTDstring("%f",dLoss_RF3[i])+","+MakeSTDstring("%f",dLoss_RF4[i])+";";	
	//strCmd=strCmd+strLine;
	strCmd=strCmd+strLine+" *OPC?";

	//if (! GPIB_Write(strCmd))
	if (! GPIB_OPComplete(strCmd))
		return false;

	return true;
}

// ------------------- Set signal on/off -------------------

bool CCMU200::SetSignalOnOff(int iMode, bool bOn)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	if (bOn)
		sprintf_s(szCommand, 255, "%d;PROC:SIGN:ACT SON; *OPC?", iMode);
	else
		sprintf_s(szCommand, 255, "%d;PROC:SIGN:ACT SOFF; *OPC?", iMode);

	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::SetSignalOnOff(bool bOn)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	if (bOn)
		sprintf_s(szCommand, 255, "%d;PROC:SIGN:ACT SON; *OPC?", g_iMode);
	else
		sprintf_s(szCommand, 255, "%d;PROC:SIGN:ACT SOFF; *OPC?", g_iMode);

	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}


// ----------------- RF functions -----------------

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

bool CCMU200::RF_TurnOnOffAuxTxGenerator(bool bOn)
{
	char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand;

    if (bOn)
		sprintf_s(szCommand, 255, "%d;INITiate:RFGenerator:AUXTx; *OPC?", g_iMode);
    else
        sprintf_s(szCommand, 255, "%d;ABORt:RFGenerator:AUXTx; *OPC?", g_iMode);

    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true;
}

bool CCMU200::RF_TurnOnOffLowSpurModeForGenerator(bool bOn)
{
	char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand;

    if (bOn)
		sprintf_s(szCommand, 255, "%d;SOURce:RFGenerator:LSMode:STATe ON; *OPC?", g_iMode);
    else
        sprintf_s(szCommand, 255, "%d;SOURce:RFGenerator:LSMode:STATe OFF; *OPC?", g_iMode);

    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true;
}

bool CCMU200::RF_SetTriggerSourceType(std::string strType)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;
	
	if (stricmp("FRUN",const_cast<char*>(strType.c_str()))!=0 && 
		stricmp("EXTERN",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("EXT",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("IFPower",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("IFP",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("INTern",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("INT",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("SLOT",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("FRAME",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("FRAM",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("TPC",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("HSDPcch",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("HSDP",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("AUTO",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("RFPower",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("RFP",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("IMMediate",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("IMM",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("SFRame",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("SFR",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("SIGN",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("SIGNalling",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("PRE",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("PMP",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("CMODe",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("CMOD",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("CTFC",const_cast<char*>(strType.c_str()))!=0
		)
	{
		return false;
	}
	transform(strType.begin(),strType.end(),strType.begin(),toupper);
	//strCommand=MakeSTDstring("%d",g_iMode)+";TRIGger:SEQuence:SOURce "+strType+"; *OPC?";
	strCommand=MakeSTDstring("%d",g_iMode)+";TRIGger:SEQuence:SOURce "+strType;

	if (! GPIB_Write(strCommand))
		return false;

	return true;
}

bool CCMU200::RF_SetTriggerDelay(int iSlotDelay,int iDelayOffset)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;
	
	sprintf_s(szCommand, 255, "%d;TRIGger:SEQuence:DELay %d,%d", g_iMode,iSlotDelay,iDelayOffset);
	strCommand=szCommand;

	if (! GPIB_Write(strCommand))
		return false;

	return true;
}

bool CCMU200::RF_SetIFPowerConfig(double dThreshold,std::string strSlope)
{
	if (dThreshold<-47)
		dThreshold=-47;
	if (dThreshold>0)
		dThreshold=0;

	if (
		stricmp("Positive",strSlope.c_str())!=0 &&
		stricmp("Pos",strSlope.c_str())!=0  &&
		stricmp("NEGative",strSlope.c_str())!=0 &&
		stricmp("NEG",strSlope.c_str())!=0
		)
	{
		return false;
	}
	
	std::string strCmd=
		MakeSTDstring("%d",g_iMode)+";TRIGger:SEQuence:THReshold:IFPower "+MakeSTDstring("%f",dThreshold)
		+" ; :TRIGger:SEQuence:SLOPe "+strSlope;
	
	if (! GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::RF_SetModulationMode(std::string strMode)
{
	std::string strCommand;

	if (
		stricmp("off",const_cast<char*>(strMode.c_str()))!=0 && 
		stricmp("SSB",const_cast<char*>(strMode.c_str()))!=0 &&
		stricmp("AM",const_cast<char*>(strMode.c_str()))!=0 &&
		stricmp("FM",const_cast<char*>(strMode.c_str()))!=0 &&
		stricmp("FMST",const_cast<char*>(strMode.c_str()))!=0
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
        if (! GPIB_Querry(strCommand, strResponse))
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
    if (! GPIB_Querry(strCommand, strMeasured))
        return false;

    sprintf_s(szCommand, 255, "%d;ABORt:NPOWer; *OPC?", g_iMode);
    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true;
}

bool CCMU200::RF_AnalyzerMeasurement(std::string& strMeasured)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;
	std::string strResponse;

	sprintf_s(szCommand, 255, "%d;INITiate:RFANalyzer; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	bool bReady = false;
	for (int i = 0; i < 20; ++i)
	{
		sprintf_s(szCommand, 255, "%d;FETCh:RFANalyzer:STATus?", g_iMode);
		strCommand = szCommand;
		if (! GPIB_Querry(strCommand, strResponse))
			return false;
		if (strstr(strResponse.c_str(), "RDY") != NULL)
		{
			bReady = true;
			break;
		}

		Sleep(500);
	}
	if (! bReady)
		return false;

	sprintf_s(szCommand, 255, "%d;FETCh:RFANalyzer:POWer?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strMeasured))
		return false;

	sprintf_s(szCommand, 255, "%d;ABORt:RFANalyzer", g_iMode);
	strCommand = szCommand;
	GPIB_Write(strCommand);

	return true;
}

bool CCMU200::RF_SetAMModulationRation(int iPercent)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;SOURce:RFGenerator:MODulation:AM:INDex %d; *OPC?", g_iMode, iPercent);

	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::RF_SetAMModulationFrequency(int iHz)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;SOURce:RFGenerator:MODulation:FREQuency %d; *OPC?", g_iMode, iHz);

	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}



// --------------- GSM signaling functions ---------------

bool CCMU200::Initial_GSM_Param()
{
    char szCommand[256] ;
    ::memset(szCommand, 0, 256) ;
    std::string strCommand ;

    sprintf_s(szCommand, 255, "%d;CONFigure:NETWork:NSUPport GSM;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    sprintf_s(szCommand, 255, "%d;CONFigure:SIGNalling:CSWitched:TCH:SMODe SSL;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    sprintf_s(szCommand, 255, "%d;CONFigure:MSSignal:CSWitched:TCH:SSLot:LOOP C;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    sprintf_s(szCommand, 255, "%d;CONFigure:NETWork:CSWitched:SMODe:SCHannel NONE;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    // Default Value
    //sprintf_s(szCommand, 255, "%d;CONFigure:NETWork:CSWitched:SMODe:BITStream ECHO;*OPC?", g_iMode) ;
    //strCommand = szCommand ;
    //if (! GPIB_OPComplete(strCommand))
    //    return false ;

    // Default Value
    //sprintf_s(szCommand, 255, "%d;CONFigure:NETWork:CSWitched:SMODe:TRAFfic FRV1;*OPC?", g_iMode) ;
    //strCommand = szCommand ;
    //if (! GPIB_OPComplete(strCommand))
    //    return false;

    // Default Value
    //sprintf_s(szCommand, 255, "%d;CONFigure:MSSignal:TCH:TADVance 0;*OPC?", g_iMode) ;
    //strCommand = szCommand ;
    //if (! GPIB_OPComplete(strCommand))
    //    return false;

    // Default Value
    //sprintf_s(szCommand, 255, "%d;CONFigure:BSSignal:CCH:TX:MODE BATC;*OPC?", g_iMode) ;
    //strCommand = szCommand ;
    //if (! GPIB_OPComplete(strCommand))
    //    return false;

    // Default Value
    //sprintf_s(szCommand, 255, "%d;CONFigure:NETWork:IDENtity:LAC 0001;*OPC?", g_iMode) ;
    //strCommand = szCommand ;
    //if (! GPIB_OPComplete(strCommand))
    //    return false ;

    return true ;
}


bool CCMU200::GSM_SetMode(int iBand, int iMode)
{
    // iMode=0: Non-Signaling
    // iMode=1: Signaling
    if (0==iMode)
    {
        if (850 == iBand)
            this->SetMode(_GSM850MS_NSIG) ;
        else if (900 == iBand)
            this->SetMode(_GSM900MS_NSIG) ;
        else if (1800 == iBand)
            this->SetMode(_GSM1800MS_NSIG) ;
        else if (1900 == iBand)
            this->SetMode(_GSM1900MS_NSIG) ;
    }
    else
    {
        if (850 == iBand)
            this->SetMode(_GSM850MS_SIG) ;
        else if (900 == iBand)
            this->SetMode(_GSM900MS_SIG) ;
        else if (1800 == iBand)
            this->SetMode(_GSM1800MS_SIG) ;
        else if (1900 == iBand)
            this->SetMode(_GSM1900MS_SIG) ;
    }

    this->SetBand(iBand) ;

    return true ;
}


bool CCMU200::GSM_SetBandIndicator(int iBand)
{
    char szCommand[256] ;
    ::memset(szCommand, 0, 256) ;
    std::string strCommand ;

    if ( 1800==iBand||900==iBand )
    {
        sprintf_s(szCommand, 255, "%d;CONFigure:NETWork:SYSTem:BINDicator G18; *OPC?", g_iMode) ;
        strCommand = szCommand ;
        if (! GPIB_OPComplete(strCommand))
            return false;
    }
    else if ( 1900==iBand ||850==iBand )
    {
        sprintf_s(szCommand, 255, "%d;CONFigure:NETWork:SYSTem:BINDicator G19; *OPC?", g_iMode) ;
        strCommand = szCommand ;
        if (! GPIB_OPComplete(strCommand))
            return false;
    }
    
    return true ;
}


bool CCMU200::GSM_SetCCH(int iChannel)
{
    char szCommand[256] ;
    ::memset(szCommand, 0, 256) ;
    std::string strCommand ;

    // This command defines the control channel of the signal which the BS transmits for synchronization.
    sprintf_s(szCommand, 255, "%d;CONFigure:BSSignal:CCH:TX:CHANnel %d; *OPC?", g_iMode, iChannel) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true ;
}

bool CCMU200::GSM_SetTCH(int iChannel)
{
    char szCommand[256] ;
    ::memset(szCommand, 0, 256) ;
    std::string strCommand ;
    
    //  This command determines the traffic channel for the BTS traffic channel signal.
    sprintf_s(szCommand, 255, "%d;CONFigure:BSSignal:CSWitched:TCH:CHANnel %d; *OPC?", g_iMode, iChannel) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    this->SetChannel(iChannel) ;

    return true ;
}

bool CCMU200::GSM_SetTCH_Handover(int iChannel)
{
    char szCommand[256] ;
    ::memset(szCommand, 0, 256) ;
    std::string strCommand ;

    // This command changes the traffic channel number (and thus the frequency) for signals of the CMU
    // while a connection is established.
    sprintf_s(szCommand, 255, "%d;PROCedure:SIGNalling:CSWitched:TCH:CHANnel %d; *OPC?", g_iMode, iChannel) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    this->SetChannel(iChannel) ;

    return true ;
}


bool CCMU200::GSM_SetTCHTimeSlot(int iSlot)
{
    char szCommand[256] ;
    ::memset(szCommand, 0, 256) ;
    std::string strCommand ;

    // CONFigure:BSSignal[:CSWitched][:TCH][:SSLot]:TIMeslot <Timeslot>
    // This command determines the timeslot for the BS traffic channel.
    sprintf_s(szCommand, 255, "%d;CONFigure:BSSignal:CSWitched:TCH:SSLot:TIMeslot %d;*OPC?", g_iMode, iSlot) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    // CONFigure:MCONtrol:MSLot:MESLot
    sprintf_s(szCommand, 255, "%d;CONFigure:MCONtrol:MSLot:MESLot %d;*OPC?", g_iMode, iSlot) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true ;
}


bool CCMU200::GSM_SetTCHTimeSlotLevel_StartCall(float fLevel)
{
    char szCommand[256] ;
    ::memset(szCommand, 0, 256) ;
    std::string strCommand ;

    sprintf_s(szCommand, 255, "%d;CONFigure:RXQuality:BER1:CONTrol:CSWitched:TCH:LEVel:UTIMeslot %.1f;*OPC?", g_iMode, fLevel) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    sprintf_s(szCommand, 255, "%d;CONFigure:RXQuality:BAVerage:CONTrol:TCH:LEVel:UTIMeslot %.1f;*OPC?", g_iMode, fLevel) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    // This command determines the absolute level in the used time slot.
    sprintf_s(szCommand, 255, "%d;CONFigure:BSSignal:CSWitched:TCH:LEVel:UTIMeslot %.1f;*OPC?", g_iMode, fLevel) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    // This command determines the level of the control channel in absolute units.
    sprintf_s(szCommand, 255, "%d;CONFigure:BSSignal:CCH:LEVel:ABSolute %.1f;*OPC?", g_iMode, fLevel) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true ;
}


bool CCMU200::GSM_SetTCHTimeSlotLevel_Handover(float fLevel)
{
    char szCommand[256] ;
    ::memset(szCommand, 0, 256) ;
    std::string strCommand ;

    sprintf_s(szCommand, 255, "%d;PROCedure:BSSignal:CSWitched:TCH:LEVel:UTIMeslot %.1f;*OPC?", g_iMode, fLevel) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    sprintf_s(szCommand, 255, "%d;CONFigure:RXQuality:BER1:CONTrol:CSWitched:TCH:LEVel:UTIMeslot %.1f;*OPC?", g_iMode, fLevel) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    sprintf_s(szCommand, 255, "%d;CONFigure:RXQuality:BAVerage:CONTrol:TCH:LEVel:UTIMeslot %.1f;*OPC?", g_iMode, fLevel) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true ;
}


bool CCMU200::GSM_SetTCHTimeSlotPCL(int iPCL)
{
    char szCommand[256] ;
    ::memset(szCommand, 0, 256) ;
    std::string strCommand ;

    // CONFigure:MSSignal[:CSWitched][:TCH][:SSLot]:MS:PCL <Level>
    // Desc.: This command defines the mobile power level upon registration in the network.
    sprintf_s(szCommand, 255, "%d;CONFigure:MSSignal:CSWitched:TCH:SSLot:MS:PCL %d;*OPC?", g_iMode, iPCL) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true ;
}


bool CCMU200::GSM_SetTCHTimeSlotPCL_Handover(int iPCL)
{
    // Desc.: This command commands the mobile phone to change its power control level,
    //        while a connection is established
    char szCommand[256] ;
    ::memset(szCommand, 0, 256) ;
    std::string strCommand ;

    sprintf_s(szCommand, 255, "%d;PROCedure:SIGNalling:CSWitched:TCH:SSLot:MS:PCL %d;*OPC?", g_iMode, iPCL) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true ;
}


bool CCMU200::GSM_MTCall()
{
    char szCommand[256] ;
    memset(szCommand, 0, 256) ;
    std::string strCommand ;

    sprintf_s(szCommand, 255, "%d;PROC:SIGN:ACT MTC; *OPC?", g_iMode);
    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true ;
}


bool CCMU200::GSM_CheckCallStatusCED()
{
    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand, strResponse;

    sprintf_s(szCommand, 255, "%d;SIGN:STAT?", g_iMode) ;
    strCommand = szCommand;
    if (! GPIB_Querry(strCommand, strResponse))
        return false;

    if (strstr(strResponse.c_str(), "CED") != NULL)
        return true;

    return false;
}


bool CCMU200::GSM_CheckCallStatusCEST()
{
    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand, strResponse;

    sprintf_s(szCommand, 255, "%d;SIGN:STAT?", g_iMode) ;
    strCommand = szCommand;
    if (! GPIB_Querry(strCommand, strResponse))
        return false;

    if (strstr(strResponse.c_str(), "CEST") != NULL)
        return true;

    return false;
}

bool CCMU200::GSM_QuerryCampStatus()
{
	int iRetry = 30;
	bool bCamp = false;

	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;SIGN:STAT?", g_iMode);
	strCommand = szCommand;

	do
	{
		if (! GPIB_Querry(strCommand, strResponse))
			return false;

		if ((strstr(strResponse.c_str(), "SYNC") != NULL) || (strstr(strResponse.c_str(), "CEST") != NULL))
		{
			bCamp = true;
			break;
		}

		Sleep(500);
		--iRetry;
	} while (iRetry > 0);

	if (! bCamp)
		return false;

	return true;
}

bool CCMU200::GSM_MT_Call()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;SIGN:STAT?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strResponse))
		return false;

	if (strstr(strResponse.c_str(), "CEST") != NULL)
		return true;

	sprintf_s(szCommand, 255, "%d;PROC:SIGN:ACT MTC; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;SIGN:STAT?", g_iMode);
	strCommand = szCommand;

	bool bUsed2ndChance = false;
	int iNumSON = 0;
	while (true)
	{
		if (! GPIB_Querry(strCommand, strResponse))
			return false;

		if (strstr(strResponse.c_str(), "SON") != NULL || strstr(strResponse.c_str(), "ALER") != NULL)
		{
			iNumSON++;
			if (iNumSON>70)
				break;

			Sleep(500);
			continue;
		}
		else if (strstr(strResponse.c_str(), "CEST") != NULL)
			return true;
		else if (strstr(strResponse.c_str(), "SYNC") != NULL)
		{
			if (bUsed2ndChance)
				break;

			bUsed2ndChance = true;
			sprintf_s(szCommand, 255, "%d;PROC:SIGN:ACT MTC; *OPC?", g_iMode);
			strCommand = szCommand;
			if (! GPIB_OPComplete(strCommand))
				return false;

			sprintf_s(szCommand, 255, "%d;SIGN:STAT?", g_iMode);
			strCommand = szCommand;

			Sleep(500);
			continue;
		}
		else
			break;
	}

	return false;
}


bool CCMU200::GSM_SetMSMaxPower(float fInputLevel)
{
    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand, strResponse;

    // This command defines the maximum expected input level
    sprintf_s(szCommand, 255, "%d;LEVel:MODE AUTomatic;:LEVel:MAXimum %4.1f; *OPC?", g_iMode, fInputLevel) ;
    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true;
}


bool CCMU200::GSM_SetHANDoverTarget(int iTargetBand)
{
    std::string chDualBand ;

    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand, strResponse;

    if ( iTargetBand == 850 )
        chDualBand = "GSM850MSDualBand" ;
    else if ( iTargetBand == 900 )
        chDualBand = "GSM900MSDualBand" ;
    else if ( iTargetBand == 1800 )
        chDualBand = "GSM1800MSDualBand" ;
    else if ( iTargetBand == 1900 )
        chDualBand = "GSM1900MSDualBand" ;

    sprintf_s(szCommand, 255, "%d;CONFigure:HANDover:TARGet \"%s\";*OPC?", g_iMode, chDualBand.c_str()) ;
    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true;
}


bool CCMU200::GSM_Handover()
{
    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand, strResponse;

    sprintf_s(szCommand, 255, "%d;PROCedure:SIGNalling:CSWitched:ACTion HANDover; *OPC?", g_iMode) ;
    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true;
}


int CCMU200::GSM_QueryBand()
{
    return g_iCurrentBand ;
}

int CCMU200::GSM_QueryChannel()
{
    return g_iCurrentChannel ;
}



// --------------- GSM Signaling Test Functions ------------
bool CCMU200::GSM_FetchBERRSSI( std::string& strMeasured )
{
	int iRetry = 20;
	bool bFetch = false;

	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;RREPorts:RXLevel?", g_iMode);
	strCommand = szCommand;

	if (! GPIB_Querry(strCommand, strMeasured))
		return false;

	return true;
} 

bool CCMU200::GSM_FetchRXQualityBER(std::string& strMeasured, int iBursts)
{
    int iRetry = 20;
    bool bFetch = false;

    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand, strResponse;

    sprintf_s(szCommand, 255, "%d;CONFigure:RXQuality:BER1:CONTrol:CSWitched BBB, %d; *OPC?", g_iMode, iBursts);
    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;

    sprintf_s(szCommand, 255, "%d;CONFigure:RXQuality:BER1:CONTrol:REPetition NONE, NONE; *OPC?", g_iMode);
    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;

    sprintf_s(szCommand, 255, "%d;INITiate:RXQuality:BER; *OPC?", g_iMode);
    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;

    sprintf_s(szCommand, 255, "%d;FETCh:RXQuality:BER:STATus?", g_iMode);
    strCommand = szCommand;

    do
    {
        if (! GPIB_Querry(strCommand, strResponse))
            return false;

        if (strstr(strResponse.c_str(), "RDY") != NULL)
        {
            bFetch = true;
            break;
        }

        //Sleep(500);
		Sleep(300);
        --iRetry;
    } while (iRetry > 0);

    if (! bFetch)
        return false;

    sprintf_s(szCommand, 255, "%d;FETCh:RXQuality:BER?", g_iMode);
    strCommand = szCommand;
    if (! GPIB_Querry(strCommand, strMeasured))
        return false;

    return true;
}


bool CCMU200::GSM_FetchRXQualityRFER(std::string& strMeasured, int iBursts)
{
    int iRetry = 20;
    bool bFetch = false;

    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand, strResponse;

    sprintf_s(szCommand, 255, "%d;CONFigure:RXQuality:BER1:CONTrol:CSWitched RFER, %d; *OPC?", g_iMode, iBursts);
    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;

    sprintf_s(szCommand, 255, "%d;CONFigure:RXQuality:BER1:CONTrol:REPetition NONE, NONE; *OPC?", g_iMode);
    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;

    sprintf_s(szCommand, 255, "%d;INITiate:RXQuality:BER; *OPC?", g_iMode);
    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;

    sprintf_s(szCommand, 255, "%d;FETCh:RXQuality:BER:STATus?", g_iMode);
    strCommand = szCommand;

    do
    {
        if (! GPIB_Querry(strCommand, strResponse))
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

    sprintf_s(szCommand, 255, "%d;FETCh:RXQuality:BER?", g_iMode);
    strCommand = szCommand;
    if (! GPIB_Querry(strCommand, strMeasured))
        return false;

    return true;
}


bool CCMU200::GSM_FetchGMSKPower(std::string& strMeasured, int iBursts)
{
    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand, strResponse ;

    sprintf_s(szCommand, 255, "%d;CONFigure:POWer:NORMal:GMSK:CONTrol SCALar,%d;*OPC?", g_iMode, iBursts) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    sprintf_s(szCommand, 255, "%d;INITiate:POWer:NORMal:GMSK;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    bool bFetch = false ;
    //int iRetry = 40 + (int)(iBursts/100) ;
	int iRetry = 10 ;
    sprintf_s(szCommand, 255, "%d;FETCh:POWer:NORMal:GMSK:STATus?", g_iMode);
    strCommand = szCommand;
    do {
        if (! GPIB_Querry(strCommand, strResponse))
            return false;

        if (strstr(strResponse.c_str(), "RDY") != NULL)
        {
            bFetch = true;
            break;
        }
        else if (strstr(strResponse.c_str(), "OFF") != NULL)
        {
            return false ;
        }

        //Sleep(1000);
		Sleep(300);
        --iRetry;
    } while (iRetry > 0);

    if (! bFetch)
        return false;

    // FETCh[:SCALar]:POWer[:NORMal][:GMSK]?
    sprintf_s(szCommand, 255, "%d;FETCh:SCALar:POWer:NORMal:GMSK?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_Querry(strCommand, strMeasured))
        return false ;

    sprintf_s(szCommand, 255, "%d;ABORt:POWer:NORMal:GMSK;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    return true ;
}


bool CCMU200::GSM_FetchGMSKPhaseError(std::string& strMeasured, int iBursts)
{
    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand, strResponse ;

    //CONFigure:MODulation[:PERRor][:GMSK]:CONTrol <Mode>, <Statistics>
    sprintf_s(szCommand, 255, "%d;CONFigure:MODulation:PERRor:GMSK:CONTrol SCALar,%d;*OPC?", g_iMode, iBursts) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    sprintf_s(szCommand, 255, "%d;INITiate:MODulation:PERRor:GMSK;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    bool bFetch = false ;
    //int iRetry = 40 + (int)(iBursts/100) ;
	int iRetry = 10 ;
    sprintf_s(szCommand, 255, "%d;FETCh:MODulation:PERRor:GMSK:STATus?", g_iMode);
    strCommand = szCommand;
    do {
        if (! GPIB_Querry(strCommand, strResponse))
            return false;

        if (strstr(strResponse.c_str(), "RDY") != NULL)
        {
            bFetch = true;
            break;
        }
        else if (strstr(strResponse.c_str(), "OFF") != NULL)
        {
            return false ;
        }

        //Sleep(1000);
		Sleep(300);
        --iRetry;
    } while (iRetry > 0);

    if (! bFetch)
        return false;

    // FETCh[:SCALar]:MODulation[:PERRor][:GMSK]?
    sprintf_s(szCommand, 255, "%d;FETCh:SCALar:MODulation:PERRor:GMSK?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_Querry(strCommand, strMeasured))
        return false ;

    sprintf_s(szCommand, 255, "%d;ABORt:MODulation:PERRor:GMSK;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    return true ;
}


bool CCMU200::GSM_FetchModulationSpectrum(std::string& strMeasured, int iBursts)
{
    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand, strResponse ;

    //CONFigure:SPECtrum:SWITching:MODulation <Mode>,<Statistics>
    sprintf_s(szCommand, 255, "%d;CONFigure:SPECtrum:MODulation:CONTrol ARRay,%d;*OPC?", g_iMode, iBursts) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    sprintf_s(szCommand, 255, "%d;INITiate:SPECtrum:MODulation;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;


    bool bFetch = false ;
    //int iRetry = 40 + (int)(iBursts/100) ;
	int iRetry = 10 ;
    sprintf_s(szCommand, 255, "%d;FETCh:SPECtrum:MODulation:STATus?", g_iMode);
    strCommand = szCommand;
    do {
        if (! GPIB_Querry(strCommand, strResponse))
            return false;

        if (strstr(strResponse.c_str(), "RDY") != NULL)
        {
            bFetch = true;
            break;
        }
        else if (strstr(strResponse.c_str(), "OFF") != NULL)
        {
            return false ;
        }

        //Sleep(1000);
		Sleep(300);
        --iRetry;
    } while (iRetry > 0);

    if (! bFetch)
        return false;


    sprintf_s(szCommand, 255, "%d;FETCh:ARRay:SPECtrum:MODulation:FDOMain?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_Querry(strCommand, strMeasured))
        return false ;

    sprintf_s(szCommand, 255, "%d;ABORt:SPECtrum:MODulation;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    return true ;
}

bool CCMU200::GSM_SampleModulationSpectrum(float *out_pfPowerMOD)
{
	int j = 0;
	char seps[] = ",";
	char *token = NULL;
	char *next_token = NULL;

    char szCommand[256];
    memset(szCommand, 0, 256);
	std::string strCommand, strResponse;
	std::string strMeasured;

    sprintf_s(szCommand, 255, "%d;INITiate:SPECtrum:MODulation;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;


    bool bFetch = false ;
    //int iRetry = 40 + (int)(iBursts/100) ;
	int iRetry = 50 ;
    sprintf_s(szCommand, 255, "%d;FETCh:SPECtrum:MODulation:STATus?", g_iMode);
    strCommand = szCommand;
    do {
        if (! GPIB_Querry(strCommand, strResponse))
            return false;

        if (strstr(strResponse.c_str(), "RDY") != NULL)
        {
            bFetch = true;
            break;
        }
        else if (strstr(strResponse.c_str(), "OFF") != NULL)
        {
            return false ;
        }

        //Sleep(1000);
		Sleep(100);
        --iRetry;
    } while (iRetry > 0);

    if (! bFetch)
        return false;

    sprintf_s(szCommand, 255, "%d;SAMPle:SPECtrum:MODulation?", g_iMode) ;
    strCommand = szCommand ;
	/*
	if (! GPIB_OPComplete(strCommand))
        return false ;
	*/
	if (! GPIB_Querry(strCommand, strMeasured))
		return false ;

    sprintf_s(szCommand, 255, "%d;SAMPle:ARRay:SPECtrum:MODulation?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_Querry(strCommand, strMeasured))
        return false ;

	/*
	if (strstr((char*)strMeasured.c_str(),"NAN") != NULL){
		return false;
	}
	*/

	token = strtok_s( (char*)strMeasured.c_str(), seps, &next_token);
	while( token != NULL ){
		out_pfPowerMOD[j]= (float)atof(token);
		token = strtok_s( NULL, seps, &next_token);
		j++;
	}

    sprintf_s(szCommand, 255, "%d;ABORt:SPECtrum:MODulation;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    return true ;
}


bool CCMU200::GSM_FetchPowerVsTimeMask(std::string& strMeasured, int iBursts)
{
    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand ;

    // CONFigure:POWer[:NORMal][:GMSK]:CONTrol <Mode>, <Statistics> 
    sprintf_s(szCommand, 255, "%d;CONFigure:POWer:NORMal:GMSK:CONTrol ARRay,%d;*OPC?", g_iMode, iBursts) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    sprintf_s(szCommand, 255, "%d;INITiate:POWer:NORMal:GMSK;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    sprintf_s(szCommand, 255, "%d;FETCh:ARRay:POWer:NORMal:GMSK:AVERage?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_Querry(strCommand, strMeasured))
        return false ;

    sprintf_s(szCommand, 255, "%d;ABORt:POWer:NORMal:GMSK;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;


    return true ;
}


bool CCMU200::GSM_PowerVsTimeMaskPaser(std::vector<std::string> vData, int iArea, double fPowerUpper, std::string& strMeasured)
{
    // If pass, return an avg. value.
    // if not,  return the first out of range value.
    const float fCMU200PowerLower = -10.0f ;
    const float fCMU200PowerUpper = 156.75f ;

    // Query Line Limit
    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand, strQuery ;

    sprintf_s(szCommand, 255, "%d;CONFigure:POWer:NORMal:GMSK:LIMit:LINE:UPPer%d:STATic?", g_iMode, iArea) ;
    strCommand = szCommand ;
    if (! GPIB_Querry(strCommand, strQuery))
        return false ;

    std::vector<std::string> vLineLimit ;
    if (! STDStringTokenizer(strQuery, ",", vLineLimit))
        return false;

    float fTmp ;
    fTmp = (float)atof(vLineLimit[0].c_str()) ;
    int iIndexStart = (int)( fTmp-(fCMU200PowerLower) ) * 4 ;
    fTmp = (float)atof(vLineLimit[1].c_str()) ;
    int iIndexStop  = (int)( fTmp-(fCMU200PowerLower) ) * 4 ;

    double dTotal = 0 ;
    char szTmp[MAX_PATH] ;
    ::ZeroMemory(szTmp, MAX_PATH) ;
    
    for (int i=iIndexStart; i<iIndexStop; ++i) {
        double dData = atof( vData[i].c_str() ) ;

        if ( dData>=fPowerUpper ){
            sprintf(szTmp, "%4.3f", dData) ;
            strMeasured = szTmp ;
            return false ;
        }

        dTotal = dTotal + dData ;
    }

    double dReusl = dTotal/(double)(iIndexStop-iIndexStart) ;

    sprintf(szTmp, "%4.3f", dReusl) ;
    strMeasured = szTmp ;
    
    return true ;
}


bool CCMU200::GSM_FetchSwitchingSpectrum(std::string& strMeasured, int iBursts)
{
    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand, strResponse ;

    //CONFigure:SPECtrum:SWITching:CONTrol <Mode>,<Statistics>
    sprintf_s(szCommand, 255, "%d;CONFigure:SPECtrum:SWITching:CONTrol ARRay,%d;*OPC?", g_iMode, iBursts) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    sprintf_s(szCommand, 255, "%d;INITiate:SPECtrum:SWITching;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    bool bFetch = false ;
    //int iRetry = 40 + (int)(iBursts/100) ;
	int iRetry = 10 ;
    sprintf_s(szCommand, 255, "%d;FETCh:SPECtrum:SWITching:STATus?", g_iMode);
    strCommand = szCommand;
    do {
        if (! GPIB_Querry(strCommand, strResponse))
            return false;

        if (strstr(strResponse.c_str(), "RDY") != NULL)
        {
            bFetch = true;
            break;
        }
        else if (strstr(strResponse.c_str(), "OFF") != NULL)
        {
            return false ;
        }

        //Sleep(1000);
		Sleep(300);
        --iRetry;
    } while (iRetry > 0);

    if (! bFetch)
        return false;

    sprintf_s(szCommand, 255, "%d;FETCh:ARRay:SPECtrum:SWITching:FDOMain?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_Querry(strCommand, strMeasured))
        return false ;

    sprintf_s(szCommand, 255, "%d;ABORt:SPECtrum:SWITching;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    return true ;
}



//////////////////////////////////////////////////////////////////////////
// --------------- GPRS signaling functions ---------------

bool CCMU200::GPRS_NeworkSelect()
{
    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand;

    sprintf_s(szCommand, 255, "%d;CONFigure:NETWork:NSUPport GGPR;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    // CONFigure:NETWork:MSERvice 
    sprintf_s(szCommand, 255, "%d;CONFigure:NETWork:MSERvice PDAT; *OPC?", g_iMode);
    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true;
}

bool CCMU200::GPRS_SetCS(int iCS)
{
    char szCommand[256] ;
    ::memset(szCommand, 0, 256) ;
    std::string strCommand ;

    sprintf_s(szCommand, 255, "%d;CONFigure:NETWork:PDATa:CSCHeme CS%d;*OPC?", g_iMode, iCS) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    return true ;
}

bool CCMU200::GPRS_SetCS_Handover(int iCS)
{
    char szCommand[256] ;
    ::memset(szCommand, 0, 256) ;
    std::string strCommand ;

    sprintf_s(szCommand, 255, "%d;PROCedure:NETWork:PDATa:CSCHeme CS%d;*OPC?", g_iMode, iCS) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    return true ;
}


// --------------- EDGE signaling functions ---------------

bool CCMU200::EDGE_NeworkSelect()
{
    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand;

    sprintf_s(szCommand, 255, "%d;CONFigure:NETWork:NSUPport GEGP;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    // CONFigure:NETWork:MSERvice 
    sprintf_s(szCommand, 255, "%d;CONFigure:NETWork:MSERvice PDAT; *OPC?", g_iMode);
    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;
    
    return true;
}

bool CCMU200::EDGE_SetSignalOnOff(bool bOn)
{
    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand;

    // PROCedure:SIGNalling:PDATa:ACTion
    if (bOn)
        sprintf_s(szCommand, 255, "%d;PROCedure:SIGNalling:PDATa:ACTion SON; *OPC?", g_iMode);
    else
        sprintf_s(szCommand, 255, "%d;PROCedure:SIGNalling:PDATa:ACTion SOFF; *OPC?", g_iMode);

    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true;
}

bool CCMU200::Initial_EDGE_Param()
{
    char szCommand[256] ;
    ::memset(szCommand, 0, 256) ;
    std::string strCommand ;

    sprintf_s(szCommand, 255, "%d;CONFigure:NETWork:PDATa:BITStream PR9;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    sprintf_s(szCommand, 255, "%d;CONFigure:NETWork:PDATa:EGPRs:PSCHeme P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    sprintf_s(szCommand, 255, "%d;CONFigure:NETWork:IDENtity:LAC 0003;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    return true ;
}


bool CCMU200::EDGE_SetTCH(int iChannel)
{
    char szCommand[256] ;
    ::memset(szCommand, 0, 256) ;
    std::string strCommand ;

    // CONFigure:BSSignal:PDATa[:TCH]:MSLot:CHANnel
    //  This command determines the traffic channel for the BTS traffic channel signal.
    sprintf_s(szCommand, 255, "%d;CONFigure:BSSignal:PDATa:TCH:MSLot:CHANnel %d; *OPC?", g_iMode, iChannel) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    this->SetChannel(iChannel) ;

    return true ;
}


bool CCMU200::EDGE_SetTCH_Handover(int iChannel)
{
    char szCommand[256] ;
    ::memset(szCommand, 0, 256) ;
    std::string strCommand ;

    // This command changes the traffic channel number (and thus the frequency) for signals of the CMU
    // while a connection is established.
    sprintf_s(szCommand, 255, "%d;PROCedure:SIGNalling:PDATa:TCH:MSLot:CHANnel %d; *OPC?", g_iMode, iChannel) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    this->SetChannel(iChannel) ;

    return true ;
}


bool CCMU200::EDGE_SetTCHMainTimeslot(int iSlot)
{
    char szCommand[256] ;
    ::memset(szCommand, 0, 256) ;
    std::string strCommand ;

    // CONFigure:BSSignal:PDATa[:TCH]:MSLot:MTIMeslot <Number> 
    // This command determines the timeslot for the BS traffic channel.
    sprintf_s(szCommand, 255, "%d;CONFigure:BSSignal:PDATa:TCH:MSLot:MTIMeslot %d;*OPC?", g_iMode, iSlot) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    // CONFigure:MCONtrol:MSLot:MESLot
    //sprintf_s(szCommand, 255, "%d;CONFigure:MCONtrol:MSLot:MESLot %d;*OPC?", g_iMode, iSlot) ;
    //strCommand = szCommand ;
    //if (! GPIB_OPComplete(strCommand))
    //    return false;

    return true ;
}


bool CCMU200::EDGE_SetBCCHAbsLevel(float fLevel)
{
    char szCommand[256] ;
    ::memset(szCommand, 0, 256) ;
    std::string strCommand ;

    // This command determines the level of the control channel in absolute units.
    sprintf_s(szCommand, 255, "%d;CONFigure:BSSignal:CCH:LEVel:ABSolute %f;*OPC?", g_iMode, fLevel) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true ;
}


bool CCMU200::EDGE_SetTCHLevel(float fLevel)
{
    char szCommand[256] ;
    ::memset(szCommand, 0, 256) ;
    std::string strCommand ;

    sprintf_s(szCommand, 255, "%d;CONFigure:RXQuality:BER1:CONTrol:PDATa:TCH:MSLot:RLEVel %f;*OPC?", g_iMode, fLevel) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    sprintf_s(szCommand, 255, "%d;CONFigure:RXQuality:BLER:CONTrol:PDATa:TCH:MSLot:RLEVel %f;*OPC?", g_iMode, fLevel) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    sprintf_s(szCommand, 255, "%d;CONFigure:RXQuality:BAVerage:CONTrol:PDATa:TCH:MSLot:RLEVel %f;*OPC?", g_iMode, fLevel) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true ;
}


bool CCMU200::EDGE_SetTimeslotConfig_Downlink(std::string strEnable, std::string strLevel)
{
    char szCommand[256] ;
    ::memset(szCommand, 0, 256) ;
    std::string strCommand ;

    // CONFigure:BSSignal:PDATa[:TCH]:MSLot:SCONfig
    sprintf_s(szCommand, 255, "%d;CONFigure:BSSignal:PDATa:TCH:MSLot:SCONfig %s,%s;*OPC?", g_iMode, strEnable.c_str(), strLevel.c_str()) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true ;

}

bool CCMU200::EDGE_SetTimeslotConfig_Uplink(std::string strEnable, std::string strLevel)
{
    char szCommand[256] ;
    ::memset(szCommand, 0, 256) ;
    std::string strCommand ;

    // CONFigure:MSSignal:PDATa[:TCH]:MSLot:SCONfig
    sprintf_s(szCommand, 255, "%d;CONFigure:MSSignal:PDATa:TCH:MSLot:SCONfig %s,%s;*OPC?", g_iMode, strEnable.c_str(), strLevel.c_str()) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true ;
}



bool CCMU200::EDGE_SetTimeslotConfig_ULDL_Handover(int iMainSlot,
                                                   std::string strDLEnable, std::string strDLLevel,
                                                   std::string strULEnable, std::string strULGamma)
{
    char szCommand[512] ;
    ::memset(szCommand, 0, 512) ;
    std::string strCommand ;

    // PROCedure:SIGNalling:PDATa[:TCH]:MSLot:SCONfig
    sprintf_s(szCommand, 511, "%d;PROCedure:SIGNalling:PDATa:TCH:MSLot:SCONfig %i,%s,%s,%s,%s;*OPC?",
              g_iMode, iMainSlot, strDLEnable.c_str(), strDLLevel.c_str(), strULEnable.c_str(), strULGamma.c_str()) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    // CONFigure:MCONtrol:MSLot:MESLot
    sprintf_s(szCommand, 255, "%d;CONFigure:MCONtrol:MSLot:MESLot %d;*OPC?", g_iMode, iMainSlot) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true ;
}


bool CCMU200::EDGE_SetTCHPzero(int iPzero)
{
    char szCommand[256] ;
    ::memset(szCommand, 0, 256) ;
    std::string strCommand ;

    // This command defines the downlink power control parameter P0
    sprintf_s(szCommand, 255, "%d;CONFigure:BSSignal:PDATa:TCH:MSLot:PZERo %d;*OPC?", g_iMode, iPzero) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true ;
}

bool CCMU200::EDGE_ConnectRES()
{
    char szCommand[256] ;
    memset(szCommand, 0, 256) ;
    std::string strCommand, strResponse ;

    sprintf_s(szCommand, 255, "%d;PROCedure:SIGNalling:PDATa:ACTion CRES; *OPC?", g_iMode);
    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true ;
}

bool CCMU200::EDGE_ConnectTestModeA()
{
    char szCommand[256] ;
    memset(szCommand, 0, 256) ;
    std::string strCommand, strResponse ;

    sprintf_s(szCommand, 255, "%d;PROCedure:SIGNalling:PDATa:ACTion CTMA; *OPC?", g_iMode);
    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true ;
}


bool CCMU200::EDGE_ConnectBLER()
{
    char szCommand[256] ;
    memset(szCommand, 0, 256) ;
    std::string strCommand, strResponse ;

    sprintf_s(szCommand, 255, "%d;PROCedure:SIGNalling:PDATa:ACTion CBLer; *OPC?", g_iMode);
    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true ;
}


bool CCMU200::EDGE_Handover()
{
    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand, strResponse;

    sprintf_s(szCommand, 255, "%d;PROCedure:SIGNalling:PDATa:ACTion HANDover; *OPC?", g_iMode) ;
    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true;
}


bool CCMU200::EDGE_DisconnectMobile()
{
    char szCommand[256] ;
    memset(szCommand, 0, 256) ;
    std::string strCommand, strResponse ;

    sprintf_s(szCommand, 255, "%d;PROCedure:SIGNalling:PDATa:ACTion DISConnect; *OPC?", g_iMode);
    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;

    return true ;
}


bool CCMU200::EDGE_QueryAttachStatus()
{
    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand, strResponse;

    sprintf_s(szCommand, 255, "%d;SIGNalling:PDATA:STATe?", g_iMode) ;
    strCommand = szCommand;
    if (! GPIB_Querry(strCommand, strResponse))
        return false;

    if (strstr(strResponse.c_str(), "SOF") != NULL) {
        if ( !EDGE_SetSignalOnOff(true) ) {
            return false;
        }
        Sleep(5000) ;
    }

    if (strstr(strResponse.c_str(), "AIPR") != NULL) {
        Sleep(5000) ;
    }
    
    if (strstr(strResponse.c_str(), "ATT") != NULL) {
        return true;
    }

    if (strstr(strResponse.c_str(), "TPEN") != NULL) {
        return true;
    }

    return false;
}

bool CCMU200::EDGE_QueryService(std::string& strService)
{
    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand, strResponse;

    sprintf_s(szCommand, 255, "%d;SIGNalling:PDATa:SERVice?", g_iMode) ;
    strCommand = szCommand;
    if (! GPIB_Querry(strCommand, strResponse))
        return false;

    strService = strResponse ;

    return true ;
}


bool CCMU200::EDGE_CheckCallStatus()
{
    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand, strResponse;

    sprintf_s(szCommand, 255, "%d;SIGNalling:PDATA:STATe?", g_iMode) ;
    strCommand = szCommand;
    if (! GPIB_Querry(strCommand, strResponse))
        return false;

    if ((strstr(strResponse.c_str(), "TED") != NULL) || (strstr(strResponse.c_str(), "TEST") != NULL))
        return true;


    if (strstr(strResponse.c_str(), "SOF") != NULL) {
        if ( !EDGE_SetSignalOnOff(true) ) {
            return false;
        }
        Sleep(5000) ;
    }

    return false;
}


bool CCMU200::EDGE_SetMCS(int iMCS)
{
    char szCommand[256] ;
    ::memset(szCommand, 0, 256) ;
    std::string strCommand ;

    sprintf_s(szCommand, 255, "%d;CONFigure:NETWork:PDATa:CSCHeme MCS%d;*OPC?", g_iMode, iMCS) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    return true ;
}

bool CCMU200::EDGE_SetMCS_Handover(int iMCS)
{
    char szCommand[256] ;
    ::memset(szCommand, 0, 256) ;
    std::string strCommand ;

    sprintf_s(szCommand, 255, "%d;PROCedure:NETWork:PDATa:CSCHeme MCS%d;*OPC?", g_iMode, iMCS) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    return true ;
}


// --------------- EDGE Signaling Test Functions -----------

bool CCMU200::EDGE_FetchRXQualityBLER(std::string& strMeasured, int iBursts)
{
    int iRetry = 40 + (int)(iBursts/100) ;
    bool bFetch = false;

    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand, strResponse;

    sprintf_s(szCommand, 255, "%d;CONFigure:RXQuality:BLER:CONTrol:RLBCount %d; *OPC?", g_iMode, iBursts);
    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;

    sprintf_s(szCommand, 255, "%d;CONFigure:RXQuality:BLER:CONTrol:REPetition SINGleshot, NONE, NONE; *OPC?", g_iMode);
    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;

    sprintf_s(szCommand, 255, "%d;INITiate:RXQuality:BLER; *OPC?", g_iMode);
    strCommand = szCommand;
    if (! GPIB_OPComplete(strCommand))
        return false;

    sprintf_s(szCommand, 255, "%d;FETCh:RXQuality:BLER:STATus?", g_iMode);
    strCommand = szCommand;

    do
    {
        if (! GPIB_Querry(strCommand, strResponse))
            return false;

        if (strstr(strResponse.c_str(), "RDY") != NULL)
        {
            bFetch = true;
            break;
        }
        else if (strstr(strResponse.c_str(), "OFF") != NULL)
        {
            return false ;
        }

        Sleep(1500);
        --iRetry;
    } while (iRetry > 0);

    if (! bFetch)
        return false;

    sprintf_s(szCommand, 255, "%d;FETCh:RXQuality:BLER?", g_iMode);
    strCommand = szCommand;
    if (! GPIB_Querry(strCommand, strMeasured))
        return false;

    return true;
}


bool CCMU200::EDGE_FetchEPSKPower(std::string& strMeasured, int iBursts)
{
    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand, strResponse ;

    sprintf_s(szCommand, 255, "%d;CONFigure:POWer:NORMal:EPSK:CONTrol SCALar,%d;*OPC?", g_iMode, iBursts) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    sprintf_s(szCommand, 255, "%d;INITiate:POWer:NORMal:EPSK;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    bool bFetch = false ;
    //int iRetry = 40 + (int)(iBursts/100) ;
	int iRetry = 10 ;
    sprintf_s(szCommand, 255, "%d;FETCh:POWer:NORMal:EPSK:STATus?", g_iMode);
    strCommand = szCommand;
    do {
        if (! GPIB_Querry(strCommand, strResponse))
            return false;

        if (strstr(strResponse.c_str(), "RDY") != NULL)
        {
            bFetch = true;
            break;
        }
        else if (strstr(strResponse.c_str(), "OFF") != NULL)
        {
            return false ;
        }

        //Sleep(1000);
		Sleep(300);
        --iRetry;
    } while (iRetry > 0);

    if (! bFetch)
        return false;

    // FETCh[:SCALar]:POWer[:NORMal]:EPSK?
    sprintf_s(szCommand, 255, "%d;FETCh:SCALar:POWer:NORMal:EPSK?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_Querry(strCommand, strMeasured))
        return false ;

    sprintf_s(szCommand, 255, "%d;ABORt:POWer:NORMal:EPSK;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    return true ;
}


bool CCMU200::EDGE_FetchEVMEPSKPhaseError(std::string& strMeasured, int iBursts)
{
    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand, strResponse ;

    //CONFigure:MODulation:EVMagnitude:EPSK:CONTrol <Mode>, <Statistics>
    sprintf_s(szCommand, 255, "%d;CONFigure:MODulation:EVMagnitude:EPSK:CONTrol SCALar,%d;*OPC?", g_iMode, iBursts) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    sprintf_s(szCommand, 255, "%d;INITiate:MODulation:EVMagnitude:EPSK;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    bool bFetch = false ;
    //int iRetry = 40 + (int)(iBursts/100) ;
	int iRetry = 10 ;
    sprintf_s(szCommand, 255, "%d;FETCh:MODulation:EVMagnitude:EPSK:STATus?", g_iMode);
    strCommand = szCommand;
    do {
        if (! GPIB_Querry(strCommand, strResponse))
            return false;

        if (strstr(strResponse.c_str(), "RDY") != NULL)
        {
            bFetch = true;
            break;
        }
        else if (strstr(strResponse.c_str(), "OFF") != NULL)
        {
            return false ;
        }

        //Sleep(1000);
		Sleep(300);
        --iRetry;
    } while (iRetry > 0);

    if (! bFetch)
        return false;


    // FETCh[:SCALar]:MODulation:EVMagnitude:EPSK?
    sprintf_s(szCommand, 255, "%d;FETCh:SCALar:MODulation:EVMagnitude:EPSK?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_Querry(strCommand, strMeasured))
        return false ;

    sprintf_s(szCommand, 255, "%d;ABORt:MODulation:EVMagnitude:EPSK;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    return true ;
}


bool CCMU200::EDGE_FetchPowerVsTimeMask(std::string& strMeasured, int iBursts)
{
    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand, strResponse ;

    // CONFigure:POWer[:NORMal]:EPSK:CONTrol <Mode>, <Statistics> 
    sprintf_s(szCommand, 255, "%d;CONFigure:POWer:NORMal:EPSK:CONTrol ARRay,%d;*OPC?", g_iMode, iBursts) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    sprintf_s(szCommand, 255, "%d;INITiate:POWer:NORMal:EPSK;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    bool bFetch = false ;
    int iRetry = 40 + (int)(iBursts/100) ;
    sprintf_s(szCommand, 255, "%d;FETCh:POWer:NORMal:EPSK:STATus?", g_iMode);
    strCommand = szCommand;
    do {
        if (! GPIB_Querry(strCommand, strResponse))
            return false;

        if (strstr(strResponse.c_str(), "RDY") != NULL)
        {
            bFetch = true;
            break;
        }
        else if (strstr(strResponse.c_str(), "OFF") != NULL)
        {
            return false ;
        }

        Sleep(1000);
        --iRetry;
    } while (iRetry > 0);

    if (! bFetch)
        return false;

    sprintf_s(szCommand, 255, "%d;FETCh:ARRay:POWer:NORMal:EPSK:AVERage?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_Querry(strCommand, strMeasured))
        return false ;

    sprintf_s(szCommand, 255, "%d;ABORt:POWer:NORMal:EPSK;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;


    return true ;
}

bool CCMU200::EDGE_PowerVsTimeMaskPaser(std::vector<std::string> vData, int iArea, double fPowerUpper, std::string& strMeasured)
{
    // If pass, return an avg. value.
    // if not,  return the first out of range value.
    const float fCMU200PowerLower = -10.0f ;
    const float fCMU200PowerUpper = 156.75f ;

    // Query Line Limit
    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand, strQuery ;

    sprintf_s(szCommand, 255, "%d;CONFigure:POWer:NORMal:EPSK:LIMit:LINE:UPPer%d:STATic?", g_iMode, iArea) ;
    strCommand = szCommand ;
    if (! GPIB_Querry(strCommand, strQuery))
        return false ;

    std::vector<std::string> vLineLimit ;
    if (! STDStringTokenizer(strQuery, ",", vLineLimit))
        return false;

    float fTmp ;
    fTmp = (float)atof(vLineLimit[0].c_str()) ;
    int iIndexStart = (int)( fTmp-(fCMU200PowerLower) ) * 4 ;
    fTmp = (float)atof(vLineLimit[1].c_str()) ;
    int iIndexStop  = (int)( fTmp-(fCMU200PowerLower) ) * 4 ;

    double dTotal = 0 ;
    char szTmp[MAX_PATH] ;
    ::ZeroMemory(szTmp, MAX_PATH) ;

    for (int i=iIndexStart; i<iIndexStop; ++i) {
        double dData = atof( vData[i].c_str() ) ;

        if ( dData>=fPowerUpper ){
            sprintf(szTmp, "%4.3f", dData) ;
            strMeasured = szTmp ;
            return false ;
        }

        dTotal = dTotal + dData ;
    }

    double dReusl = dTotal/(double)(iIndexStop-iIndexStart) ;

    sprintf(szTmp, "%4.3f", dReusl) ;
    strMeasured = szTmp ;

    return true ;
}


bool CCMU200::EDGE_FetchPowerVsTimeMask_MultiSlot(std::string& strMeasured, int iBursts, int iSlotAmount)
{
    int iRetry = 20 ;
    bool bFetch = false ;
    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand, strResponse ;

    sprintf_s(szCommand, 255, "%d;CONFigure:POWer:MSLot:CONTrol SCALar,%d;*OPC?", g_iMode, iBursts) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    sprintf_s(szCommand, 255, "%d;CONFigure:POWer:MSLot:SCOunt %d;*OPC?", g_iMode, iSlotAmount) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    sprintf_s(szCommand, 255, "%d;INITiate:POWer:MSLot;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;

    sprintf_s(szCommand, 255, "%d;FETCh:POWer:MSLot:STATus?", g_iMode);
    strCommand = szCommand;
    do
    {
        if (! GPIB_Querry(strCommand, strResponse))
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

    sprintf_s(szCommand, 255, "%d;FETCh:SCALar:POWer:MSLot?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_Querry(strCommand, strMeasured))
        return false ;

    sprintf_s(szCommand, 255, "%d;ABORt:POWer:MSLot;*OPC?", g_iMode) ;
    strCommand = szCommand ;
    if (! GPIB_OPComplete(strCommand))
        return false ;


    return true ;
}


// --------------- WCDMA signaling functions ---------------

bool CCMU200::Initial_WCDMA_Online()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;CONF:NETW:PSW:DOM OFF; *OPC?", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONF:BSS:DCH:TYPE RMC; *OPC?", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	/*
	sprintf_s(szCommand, 255, "%d;CONF:BSS:DCH:RMC:TYPE RMC12", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONF:BSS:DCH:SRAB:SRB K2_5", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONF:BSS:DCH:RMC:SDTC PR9; *OPC?", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;
	*/

	sprintf_s(szCommand, 255, "%d;CONF:BSS:DCH:RMC:TMOD MODE1; *OPC?", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	/*
	sprintf_s(szCommand, 255, "%d;CONF:BSS:DCH:RMC:UCRC OFF", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	*/

	sprintf_s(szCommand, 255, "%d;CONF:BSS:TPC:MODE ALG1; *OPC?", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	/*
	sprintf_s(szCommand, 255, "%d;CONF:BSS:TPC:SSIZ 1", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	*/

	sprintf_s(szCommand, 255, "%d;CONF:NETW:IDEN:MNC:DIG TWO; *OPC?", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	/*
	sprintf_s(szCommand, 255, "%d;CONF:NETW:IDEN:MCC 1; *OPC?", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONF:NETW:IDEN:MNC 1", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONF:NETW:IDEN:LAC 1", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONF:NETW:REQ:TTIM 1", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	*/

	sprintf_s(szCommand, 255, "%d;CONF:NETW:REQ:T31T 15; *OPC?", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONF:NETW:REQ:N31S 200; *OPC?", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	/*
	sprintf_s(szCommand, 255, "%d;CONF:NETW:REQ:S2G OFF", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	*/

	sprintf_s(szCommand, 255, "%d;CONF:NETW:REQ:OST 15; *OPC?", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	/*
	sprintf_s(szCommand, 255, "%d;CONF:NETW:REQ:ADET ON", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	*/

	sprintf_s(szCommand, 255, "%d;CONF:NETW:REQ:IMEI ON; *OPC?", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	/*
	sprintf_s(szCommand, 255, "%d;CONF:NETW:REQ:RRCC OFF", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	*/

	sprintf_s(szCommand, 255, "%d;CONF:NETW:RAC:DRXC 6; *OPC?", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONF:NETW:REQ:SRBR OFF", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	/*
	sprintf_s(szCommand, 255, "%d;CONF:BSS:AWGN OFF", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	*/

	sprintf_s(szCommand, 255, "%d;CONF:BSS:LREF OPOW; *OPC?", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	/*
	sprintf_s(szCommand, 255, "%d;CONF:BSS:PHYS:LEV -3.30,-8.30,-8.30,-5.30,-5.30,-8.30,-8.30,-10.30,0.00", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONF:BSS:SCOD:PRIM #H9", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONF:BSS:SCOD:SEC #H0", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONF:UES:PCON:UEP 33.0; *OPC?", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;
	*/

	sprintf_s(szCommand, 255, "%d;LEV:MODE AUT; *OPC?", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	/*
	sprintf_s(szCommand, 255, "%d;CONF:BSS:TPC:PTYP CLOP", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONF:UES:PCON:OLPC:INT -80.00", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	*/

	sprintf_s(szCommand, 255, "%d;CONF:UES:PCON:OLPC:CVAL -10.00; *OPC?", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	/*
	sprintf_s(szCommand, 255, "%d;CONF:UES:PCON:OLPC:PCP:RPOW 31.00", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	*/

	sprintf_s(szCommand, 255, "%d;CONF:UES:PCON:OLPC:DPCC:POFF -59.00; *OPC?", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetBand(int iBand)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	if (iBand == 1 || iBand == 2100)
		sprintf_s(szCommand, 255, "%d;CONF:NETW:OBAN OB1; *OPC?", _WCDMA19UEFDD_SIG);
	else if (iBand == 2 || iBand == 1900)
		sprintf_s(szCommand, 255, "%d;CONF:NETW:OBAN OB2; *OPC?", _WCDMA19UEFDD_SIG);
	else if (iBand == 3 || iBand == 1800)
		sprintf_s(szCommand, 255, "%d;CONF:NETW:OBAN OB3; *OPC?", _WCDMA19UEFDD_SIG);
	else if (iBand == 4 || iBand == 1700)
		sprintf_s(szCommand, 255, "%d;CONF:NETW:OBAN OB4; *OPC?", _WCDMA19UEFDD_SIG);
	else if (iBand == 5 || iBand == 850)
		sprintf_s(szCommand, 255, "%d;CONF:NETW:OBAN OB5; *OPC?", _WCDMA19UEFDD_SIG);
	else if (iBand == 6 || iBand == 800)
		sprintf_s(szCommand, 255, "%d;CONF:NETW:OBAN OB6; *OPC?", _WCDMA19UEFDD_SIG);
	else if (iBand == 7 || iBand == 2600)
		sprintf_s(szCommand, 255, "%d;CONF:NETW:OBAN OB7; *OPC?", _WCDMA19UEFDD_SIG);
	else if (iBand == 8 || iBand == 900)
		sprintf_s(szCommand, 255, "%d;CONF:NETW:OBAN OB8; *OPC?", _WCDMA19UEFDD_SIG);
	else
		return false;

	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetDownlinkChannel(int iChannel)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;CONF:BSS:CHAN %d CH; *OPC?", _WCDMA19UEFDD_SIG, iChannel);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetDCHType(std::string strType)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;CONF:BSS:DCH:TYPE %s; *OPC?", _WCDMA19UEFDD_SIG, strType.c_str());
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetBSOutputPower(double dPower)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;CONF:BSS:OPOW %4.3f; *OPC?", g_iMode, dPower);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetMaxMSPower(double dPower)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;CONFigure:UESignal:PCONtrol:UEPower %4.3f; *OPC?", g_iMode, dPower);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetMSTargetPower(double dPower)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;CONFigure:UESignal:PCONtrol:TPOWer:VALue %4.3f; *OPC?", g_iMode, dPower);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetTPCPattern(std::string strPattern)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;CONF:BSS:TPC:PSET %s; *OPC?", g_iMode, strPattern.c_str());
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetTPCPatternType(std::string strType)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;CONF:BSS:TPC:PTYP %s; *OPC?", _WCDMA19UEFDD_SIG, strType.c_str());
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetAuthentication(bool bOnOff)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	if (bOnOff)
		sprintf_s(szCommand, 255, "%d;CONFigure:NETWork:REQuest:AUTHenticat ON; *OPC?", _WCDMA19UEFDD_SIG);
	else
		sprintf_s(szCommand, 255, "%d;CONFigure:NETWork:REQuest:AUTHenticat OFF; *OPC?", _WCDMA19UEFDD_SIG);

	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetSecurityKey(std::string strKey1, std::string strKey2)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;CONF:NETW:REQ:SEC ON; *OPC?", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONF:NETW:REQ:AUTH ON; *OPC?", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONF:NETW:REQ:SKEY '%s','%s'; *OPC?", _WCDMA19UEFDD_SIG, strKey1.c_str(), strKey2.c_str());
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::WCDMA_QuerryCampStatus()
{
	int iRetry = 50;
	bool bCamp = false;

	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;SIGN:STAT?", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;

	do
	{
		if (! GPIB_Querry(strCommand, strResponse))
			return false;

		if ((strstr(strResponse.c_str(), "REG") != NULL) || (strstr(strResponse.c_str(), "CEST") != NULL))
		{
			bCamp = true;
			break;
		}

		Sleep(500);
		--iRetry;
	} while (iRetry > 0);

	if (! bCamp)
		return false;

	return true;
}

bool CCMU200::WCDMA_MTCall()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;SIGN:STAT?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strResponse))
	{
		return false;
	}

	if (strstr(strResponse.c_str(), "CEST") != NULL)
	{
		return true;
	}

	sprintf_s(szCommand, 255, "%d;PROC:SIGN:ACT CTM; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
	{
		return false;
	}

	sprintf_s(szCommand, 255, "%d;SIGN:STAT?", g_iMode);
	strCommand = szCommand;

	bool bUsed2ndChance = false;
	while (true)
	{
		if (! GPIB_Querry(strCommand, strResponse))
		{
			return false;
		}

		if (strstr(strResponse.c_str(), "PAG") != NULL)
		{
			Sleep(500);
			continue;
		}
		else if (strstr(strResponse.c_str(), "CEST") != NULL)
		{
			return true;
		}
		else if (strstr(strResponse.c_str(), "REG") != NULL)
		{
			if (bUsed2ndChance)
			{
				break;
			}

			bUsed2ndChance = true;
			sprintf_s(szCommand, 255, "%d;PROC:SIGN:ACT CTM; *OPC?", g_iMode);
			strCommand = szCommand;
			if (! GPIB_OPComplete(strCommand))
			{
				return false;
			}

			sprintf_s(szCommand, 255, "%d;SIGN:STAT?", g_iMode);
			strCommand = szCommand;

			Sleep(500);
			continue;
		}
		else
		{
			break;
		}
	}

	return false;
}

bool CCMU200::WCDMA_SetBandAndDownlinkChannel(int iBand, int iChannel)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	if (iBand == 1 || iBand == 2100)
		iBand = 1;
	else if (iBand == 2 || iBand == 1900)
		iBand = 2;
	else if (iBand == 3 || iBand == 1800)
		iBand = 3;
	else if (iBand == 4 || iBand == 1700)
		iBand = 4;
	else if (iBand == 5 || iBand == 850)
		iBand = 5;
	else if (iBand == 6 || iBand == 800)
		iBand = 6;
	else if (iBand == 7 || iBand == 2600)
		iBand = 7;
	else if (iBand == 8 || iBand == 900)
		iBand = 8;
	else
		return false;

	sprintf_s(szCommand, 255, "%d;CONF:BSS:COMB:OCH OB%d,%d CH; *OPC?", _WCDMA19UEFDD_SIG, iBand, iChannel);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::WCDMA_QuerryCallStatus()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;SIGN:STAT?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strResponse))
	{
		return false;
	}

	if (strstr(strResponse.c_str(), "CEST") != NULL)
	{
		return true;
	}
	else if (strstr(strResponse.c_str(), "PAG") != NULL)
	{
		for (int i = 0; i < 3; ++i)
		{
			Sleep(3000);

			if (! GPIB_Querry(strCommand, strResponse))
				return false;

			if (strstr(strResponse.c_str(), "REG") != NULL)
			{
				return WCDMA_MTCall();
			}
			else if (strstr(strResponse.c_str(), "CEST") != NULL)
			{
				return true;
			}
		}

		return false;
	}

	if (strstr(strResponse.c_str(), "REG") != NULL)
	{
		return WCDMA_MTCall();
	}

	return false;
}

bool CCMU200::WCDMA_InitMaxPowerMeasurement()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;DEFault:POWer:MAXimum:CONTrol ON; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONFigure:POWer:MAXimum:CONTrol:REPetition SINGleshot,NONE,NONE; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::WCDMA_FetchMaxPower(std::string& strMeasured)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;READ:POWer:MAXimum?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strMeasured))
		return false;

	return true;
}

bool CCMU200::WCDMA_InitMinPowerMeasurement()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;DEFault:POWer:MINimum:CONTrol ON; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONFigure:POWer:MINimum:CONTrol:REPetition SINGleshot,NONE,NONE; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::WCDMA_FetchMinPower(std::string& strMeasured)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;READ:POWer:MINimum?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strMeasured))
		return false;

	return true;
}

bool CCMU200::WCDMA_InitModulationMeasurement()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;DEFault:MODulation:OVERview:WCDMa:DPCH:CONTrol ON; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONFigure:MODulation:OVERview:WCDMa:DPCH:CONTrol:REPetition SINGleshot,NONE,NONE; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::WCDMA_FetchModulation(std::string& strMeasured)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;READ:MODulation:OVERview:WCDMa:DPCH?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strMeasured))
		return false;

	return true;
}

bool CCMU200::WCDMA_InitACLR_OBW_Measurement()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;DEFault:SPECtrum:MFFT:CONTrol ON; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONFigure:SPECtrum:MFFT:CONTrol:REPetition SINGleshot,NONE,NONE; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::WCDMA_FetchACLR_OBW(std::string& strMeasured)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;READ:SPECtrum:MFFT:RELative?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strMeasured))
		return false;

	return true;
}

bool CCMU200::WCDMA_InitSEMMeasurement()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;DEFault:SPECtrum:EMASk:CONTrol ON; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONFigure:SPECtrum:EMASk:CONTrol:REPetition SINGleshot,NONE,NONE; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::WCDMA_FetchSEM(std::string& strMeasured)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;READ:ARRay:SPECtrum:EMASk:AVERage?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strMeasured))
		return false;

	return true;
}

bool CCMU200::WCDMA_InitILPC()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;DEFault:POWer:XSLot:CONTrol ON; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONFigure:POWer:XSLot:CONTrol:REPetition SINGleshot,NONE,NONE; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;INITiate:POWer:XSLot; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::WCDMA_FetchILPC(std::string strPattern, std::string& strMeasured)
{
	int iRetry = 20;
	bool bReady = false;
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;ABORt:POWer:XSLot; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONFigure:BSS:TPC:PSET T%s; *OPC?", g_iMode, strPattern.c_str());
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;INITiate:POWer:XSLot; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONFigure:BSS:TPC:RPAT ON; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;FETCh:POWer:XSLot:STAT?", g_iMode);
	strCommand = szCommand;
	do
	{
		if (! GPIB_Querry(strCommand, strResponse))
			return false;

		if (strstr(strResponse.c_str(), "RDY") != NULL)
		{
			bReady = true;
			break;
		}

		Sleep(500);
		--iRetry;
	} while (iRetry > 0);

	if (! bReady)
		return false;

	sprintf_s(szCommand, 255, "%d;FETCh:POWer:XSLot:ABS?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strMeasured))
		return false;

	sprintf_s(szCommand, 255, "%d;ABORt:POWer:XSLot; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::WCDMA_InitOpenLoop_OffPower(double dBSPower, double dULInterference, double dConstantValue, double dPCPICHPower)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;SIGN:STAT?", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strResponse))
		return false;

	if (strstr(strResponse.c_str(), "CEST") != NULL)
		WCDMA_EndCall();
	else if (strstr(strResponse.c_str(), "PAG") != NULL)
	{
		int iRetry = 20;
		bool bCamp = false;
		while (iRetry > 0)
		{
			if (! GPIB_Querry(strCommand, strResponse))
				return false;

			if (strstr(strResponse.c_str(), "PAG") == NULL)
			{
				bCamp = true;
				break;
			}

			--iRetry;
			Sleep(2000);
		}

		if (! bCamp)
			return false;
	}

	if (! WCDMA_SetBSOutputPower(dBSPower))
		return false;

	sprintf_s(szCommand, 255, "%d;CONFigure:UESignal:PCONtrol:OLPC:INTerference %.3f; *OPC?", g_iMode, dULInterference);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONFigure:UESignal:PCONtrol:OLPC:CVALue %.3f; *OPC?", g_iMode, dConstantValue);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONFigure:UESignal:PCONtrol:OLPC:PCPich:RPOWer %.3f; *OPC?", g_iMode, dPCPICHPower);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONFigure:POWer:ONOFf:CONTrol:MPReamble MP1; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONFigure:POWer:ONOFf:CONTrol:MITem MIOO; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::WCDMA_FetchOpenLoop_OffPower(std::string& strMeasured)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;SIGN:STAT?", g_iMode);
	strCommand = szCommand;

	int iRetry = 20;
	do
	{
		if (! GPIB_Querry(strCommand, strResponse))
			return false;

		if (strstr(strResponse.c_str(), "REG") != NULL)
			break;
		else if (strstr(strResponse.c_str(), "CEST") != NULL)
			WCDMA_EndCall();

		Sleep(500);
		--iRetry;
	} while (iRetry > 0);

	sprintf_s(szCommand, 255, "%d;INITiate:POWer:ONOFf; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	if (! WCDMA_MTCall())
	{
		sprintf_s(szCommand, 255, "%d;SIGN:STAT?", g_iMode);
		strCommand = szCommand;

		iRetry = 20;
		while (iRetry > 0)
		{
			if (! GPIB_Querry(strCommand, strResponse))
				return false;

			if (strstr(strResponse.c_str(), "PAG") == NULL)
				break;

			--iRetry;
			Sleep(2000);
		}

		sprintf_s(szCommand, 255, "%d;ABORt:POWer:ONOFf", g_iMode);
		strCommand = szCommand;
		GPIB_Write(strCommand);
		return false;
	}

	sprintf_s(szCommand, 255, "%d;FETCh:POWer:ONOFf:STATus?", g_iMode);
	strCommand = szCommand;

	bool bFetch = false;
	iRetry = 20;
	do
	{
		if (! GPIB_Querry(strCommand, strResponse))
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

	sprintf_s(szCommand, 255, "%d;FETCh:SCALar:POWer:ONOFf:CURRent?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strMeasured))
		return false;

	return true;
}

bool CCMU200::WCDMA_EndCall()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;SIGN:STAT?", _WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strResponse))
		return false;

	if (strstr(strResponse.c_str(), "CEST") == NULL)
		return true;
	else
	{
		sprintf_s(szCommand, 255, "%d;PROC:SIGN:ACT CREL; *OPC?", _WCDMA19UEFDD_SIG);
		strCommand = szCommand;
		if (! GPIB_OPComplete(strCommand))
			return false;
		else
		{
			sprintf_s(szCommand, 255, "%d;SIGN:STAT?", _WCDMA19UEFDD_SIG);
			strCommand = szCommand;
			if (! GPIB_Querry(strCommand, strResponse))
				return false;

			if (strstr(strResponse.c_str(), "CEST") != NULL)
				return false;
		}
	}

	return true;
}

bool CCMU200::WCDMA_InitBERMMeasurement()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;CONFigure:RXQuality:BER:CONTrol:REPetition SINGleshot,NONE,NONE; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;DEFault:RXQuality:BER:LIMit ON; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::WCDMA_FetchBERMMeasurement(std::string& strMeasured)
{
	int iRetry = 20;
	bool bFetch = false;

	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;INITiate:RXQuality:BER; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;FETCh:RXQuality:BER:STATus?", g_iMode);
	strCommand = szCommand;

	do
	{
		if (! GPIB_Querry(strCommand, strResponse))
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

	sprintf_s(szCommand, 255, "%d;FETCh:RXQuality:BER?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strMeasured))
		return false;

	return true;
}

bool CCMU200::WCDMA_FetchRSCPMeasurement( std::string& strMeasured )
{
	int iRetry = 20;
	bool bFetch = false;

	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;CONFigure:RXQuality:UEReport1:ENABle ON;*OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand,strMeasured))
		return false;

	sprintf_s(szCommand, 255, "%d;DEFault:RXQuality:UEReport1:CONTrol ON;*OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand,strMeasured))
		return false;


	sprintf_s(szCommand, 255, "%d;INITiate:RXQuality:UEReport1;*OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand,strMeasured))
		return false;

	sprintf_s(szCommand, 255, "%d;FETCh:RXQuality:UEReport1:STATus?", g_iMode);
	strCommand = szCommand;

	Sleep(1500);

	sprintf_s(szCommand, 255, "%d;RXQuality:UEReport1?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strMeasured))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetHANDoverTarget()
{
	std::string strTargetBand = "WCDMA1900UEFDD";

	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;CONFigure:HANDover:TARGet \"%s\";*OPC?", g_iMode, strTargetBand.c_str()) ;
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

// --------------- HSDPA signaling functions ---------------

bool CCMU200::HSDPA_QuerryCampStatus()
{
	int iRetry = 50;
	bool bCamp = false;

	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;SIGN:PSW:STAT?", g_iMode);
	strCommand = szCommand;

	do
	{
		if (! GPIB_Querry(strCommand, strResponse))
			return false;

		if ((strstr(strResponse.c_str(), "ATT") != NULL) || (strstr(strResponse.c_str(), "PEST") != NULL))
		{
			bCamp = true;
			break;
		}

		Sleep(500);
		--iRetry;
	} while (iRetry > 0);

	if (! bCamp)
		return false;

	return true;
}


bool CCMU200::HSDPA_QuerryCallStatus()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;SIGN:PSW:STAT?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strResponse))
		return false;

	if (strstr(strResponse.c_str(), "PEST") != NULL)
		return true;

	if (strstr(strResponse.c_str(), "CIPR") != NULL)
	{
		for (int i = 0; i < 3; ++i)
		{
			Sleep(3000);

			if (! GPIB_Querry(strCommand, strResponse))
				return false;

			if (strstr(strResponse.c_str(), "PEST") != NULL)
				return true;
			if (strstr(strResponse.c_str(), "ATT") != NULL)
				return HSDPA_MTCall();
		}

		return false;
	}

	if (strstr(strResponse.c_str(), "ATT") != NULL)
		return HSDPA_MTCall();

	return false;
}

bool CCMU200::HSDPA_MTCall()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;SIGN:PSW:STAT?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strResponse))
		return false;

	if (strstr(strResponse.c_str(), "PEST") != NULL)
		return true;

	sprintf_s(szCommand, 255, "%d;PROC:SIGN:PSW:ACT PCON; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;SIGN:PSW:STAT?", g_iMode);
	strCommand = szCommand;

	bool bUsed2ndChance = false;
	while (true)
	{
		if (! GPIB_Querry(strCommand, strResponse))
			return false;

		if (strstr(strResponse.c_str(), "CIPR") != NULL)
		{
			Sleep(500);
			continue;
		}
		else if (strstr(strResponse.c_str(), "PEST") != NULL)
			return true;
		else if (strstr(strResponse.c_str(), "ATT") != NULL)
		{
			if (bUsed2ndChance)
				break;

			bUsed2ndChance = true;
			sprintf_s(szCommand, 255, "%d;PROC:SIGN:PSW:ACT PCON; *OPC?", g_iMode);
			strCommand = szCommand;
			if (! GPIB_OPComplete(strCommand))
				return false;

			sprintf_s(szCommand, 255, "%d;SIGN:PSW:STAT?", g_iMode);
			strCommand = szCommand;

			Sleep(500);
			continue;
		}
		else
			break;
	}

	return false;
}

bool CCMU200::HSDPA_EndCall()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;SIGN:PSW:STAT?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strResponse))
		return false;

	if (strstr(strResponse.c_str(), "PEST") == NULL)
		return true;
	else
	{
		sprintf_s(szCommand, 255, "%d;PROC:SIGN:PSW:ACT PREL; *OPC?", g_iMode);
		strCommand = szCommand;
		if (! GPIB_OPComplete(strCommand))
			return false;
		else
		{
			sprintf_s(szCommand, 255, "%d;SIGN:PSW:STAT?", g_iMode);
			strCommand = szCommand;
			if (! GPIB_Querry(strCommand, strResponse))
				return false;

			if (strstr(strResponse.c_str(), "PEST") != NULL)
				return false;
		}
	}

	return true;
}

bool CCMU200::HSDPA_SetGainFactor(int iBetaC, int iBetaD, int iDeltaACK, int iDeltaNACK, int iDeltaCQI)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;CONF:UES:GFAC:HSTM %d,%d,%d,%d,%d; *OPC?", g_iMode, iBetaC, iBetaD, iDeltaACK, iDeltaNACK, iDeltaCQI);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}



// -------------------- WCDMA Calibration functions --------------------
bool CCMU200::WCDMA_SetMode(bool bSignalling)
{
	if (bSignalling)
	{
		this->SetMode(_WCDMA19UEFDD_SIG);
	}
	else 
	{
		this->SetMode(_WCDMA19UEFDD_NSIG);
	}
	return true;
}

bool CCMU200::WCDMA_SetGeneratorPower(double dOutputPower)
{
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";SOUR:RFG:OPOW "+MakeSTDstring("%4.3f",dOutputPower)+";*OPC?";
	if (! GPIB_OPComplete(strCmd))
		return false;
	return true;
}

bool CCMU200::WCDMA_SetPowerVsSlotMeasuredMode(std::string strMode)
{
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:POWer:XSLot:CONTrol:MMODe ";
	if (
		stricmp("TSTeps",const_cast<char*>(strMode.c_str()))!=0 && 
		stricmp("TST",const_cast<char*>(strMode.c_str()))!=0 &&
		stricmp("SOFF",const_cast<char*>(strMode.c_str()))!=0 &&
		stricmp("SAUT",const_cast<char*>(strMode.c_str()))!=0 &&
		stricmp("SAUTo",const_cast<char*>(strMode.c_str()))!=0 &&
		stricmp("CMTS",const_cast<char*>(strMode.c_str()))!=0
		)
	{
		return false;
	}

	//strCmd=strCmd+strMode+"; *OPC?";
	//if (! GPIB_OPComplete(strCmd))
	strCmd=strCmd+strMode;
	if (! GPIB_Write(strCmd))
		return false;

	return true;
}
bool CCMU200::WCDMA_SetPowerVsSlotMeasuredConfigure(double dPowerMeasuredWidth,int iStepCount,int iStepWidth,int iStepDelay)
{
	std::string strCmd="";
	if(dPowerMeasuredWidth!=0.25 && dPowerMeasuredWidth!=0.5 && dPowerMeasuredWidth!=1)
		return false;
	if(iStepCount<1 || 100<iStepCount)
		return false;
	if(iStepDelay<1 || 100<iStepDelay)
		return false;
	

	strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:POWer:XSLot:CONTrol:PMWidth "+MakeSTDstring("%f",dPowerMeasuredWidth);
	strCmd=strCmd+"; :CONFigure:POWer:XSLot:CONTrol:SCOunt "+MakeSTDstring("%d",iStepCount);
	strCmd=strCmd+"; :CONFigure:POWer:XSLot:CONTrol:SWIDth "+MakeSTDstring("%d",iStepWidth);
	//strCmd=strCmd+"; :CONFigure:POWer:XSLot:CONTrol:SDELay "+MakeSTDstring("%d",iStepDelay)+"; *OPC?";
	strCmd=strCmd+"; :CONFigure:POWer:XSLot:CONTrol:SDELay "+MakeSTDstring("%d",iStepDelay);
	//if (! GPIB_OPComplete(strCmd))
	if (! GPIB_Write(strCmd))
		return false;

	return true;
}
bool CCMU200::WCDMA_SetPowerVsSlotMeasuredRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode)
{
	if (
		(
			stricmp("CONT",const_cast<char*>(strRepetition.c_str()))!=0 && 
			stricmp("CONTINUOUS",const_cast<char*>(strRepetition.c_str()))!=0 &&
			stricmp("SINGLSHOT",const_cast<char*>(strRepetition.c_str()))!=0 &&
			stricmp("SING",const_cast<char*>(strRepetition.c_str()))!=0 &&
			!(IsDigit(strRepetition) && 1<=atoi(strRepetition.c_str())  && atoi(strRepetition.c_str())<=1000)
		) ||
		(
			stricmp("SONerror",const_cast<char*>(strStopCondition.c_str()))!=0 &&
			stricmp("SON",const_cast<char*>(strStopCondition.c_str()))!=0  &&
			stricmp("NONE",const_cast<char*>(strStopCondition.c_str()))!=0  	
		) ||
		(
			stricmp("NONE",const_cast<char*>(strStepMode.c_str()))!=0  &&
			stricmp("STEP",const_cast<char*>(strStepMode.c_str()))!=0 
		)
	  )
	{
		return false;
	}
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:POWer:XSLot:CONTrol:REPetition ";
	//strCmd=strCmd+strRepetition+","+strStopCondition+","+strStepMode+"; *OPC?";
	//if (! GPIB_OPComplete(strCmd))
	strCmd=strCmd+strRepetition+","+strStopCondition+","+strStepMode;
	if (! GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_InitiatePowerVsSlotMeasurment()
{
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";ABORt:POWer:XSLot; *OPC?";
	
	if (! GPIB_OPComplete(strCmd))
		return false;

	strCmd=MakeSTDstring("%d",g_iMode)+";INITiate:POWer:XSLot; *OPC?";
	if (! GPIB_OPComplete(strCmd))
		return false;
	
	return true;
}

bool CCMU200::WCDMA_FetchPowerVsSlotMeasurment(double* dMearsurment)
{	
	//std::string strCmd=MakeSTDstring("%d",g_iMode)+";ABORt:POWer:XSLot; *OPC?";
	/*
	if (! GPIB_OPComplete(strCmd))
		return false;

	strCmd=MakeSTDstring("%d",g_iMode)+";INITiate:POWer:XSLot; *OPC?";
	if (! GPIB_OPComplete(strCmd))
		return false;
	*/

	char *token = NULL;
	std::string strMeasured="";
	char seps[] = ",";
	
	int iRetryTimes=70;
	std::string strCmd= MakeSTDstring("%d",g_iMode)+";FETCh:POW:XSL:STAT?";
	while(iRetryTimes-->0)
	{
		if (! GPIB_Querry(strCmd, strMeasured))
			return false;

		if (strstr(strMeasured.c_str(), "RUN") == NULL)
			break;
		else
			Sleep(500);
	} 

	std::string strCmd1= MakeSTDstring("%d",g_iMode)+";FETCh:SCALar:POWer:XSLot:CURRent?";
	if (! GPIB_Querry(strCmd1, strMeasured))
		return false;

	if (strstr((char*)strMeasured.c_str(),"NAN") != NULL){
		return false;
	}
	token = strtok( (char*)strMeasured.c_str(), seps);
	int i=0;
	while(token != NULL){
		dMearsurment[i++]= (float)atof(token);
		token = strtok( NULL, seps );
	}

	strCmd= MakeSTDstring("%d",g_iMode)+";ABORt:POWer:XSLot; *OPC?";
	if (! GPIB_OPComplete(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetFastPowerMearsuredConfig(std::string strRepetition,std::string strStopCondition,std::string strStepMode)
{
	if (
		(
		stricmp("CONT",const_cast<char*>(strRepetition.c_str()))!=0 && 
		stricmp("CONTINUOUS",const_cast<char*>(strRepetition.c_str()))!=0 &&
		stricmp("SINGLSHOT",const_cast<char*>(strRepetition.c_str()))!=0 &&
		stricmp("SING",const_cast<char*>(strRepetition.c_str()))!=0 &&
		!(IsDigit(strRepetition) && 1<=atoi(strRepetition.c_str())  && atoi(strRepetition.c_str())<=1000)
		) ||
		(
		stricmp("SONerror",const_cast<char*>(strStopCondition.c_str()))!=0 &&
		stricmp("SON",const_cast<char*>(strStopCondition.c_str()))!=0  &&
		stricmp("NONE",const_cast<char*>(strStopCondition.c_str()))!=0  	
		) ||
		(
		stricmp("NONE",const_cast<char*>(strStepMode.c_str()))!=0  &&
		stricmp("STEP",const_cast<char*>(strStepMode.c_str()))!=0 
		)
		)
	{
		return false;
	}
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:NPOWer:CONTrol:REPetition ";
	strCmd=strCmd+strRepetition+","+strStopCondition+","+strStepMode+"; *OPC?";
	if (! GPIB_OPComplete(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetFastPowerMearsuredConfig(std::string strStatics,std::string strRepetition,std::string strStopCondition,std::string strStepMode)
{
	if (
		(
		stricmp("NONE",const_cast<char*>(strStatics.c_str()))!=0 &&
		!(IsDigit(strStatics) && 1<=atoi(strStatics.c_str())  && atoi(strStatics.c_str())<=1000)
		)||
		(
		stricmp("CONT",const_cast<char*>(strRepetition.c_str()))!=0 && 
		stricmp("CONTINUOUS",const_cast<char*>(strRepetition.c_str()))!=0 &&
		stricmp("SINGLSHOT",const_cast<char*>(strRepetition.c_str()))!=0 &&
		stricmp("SING",const_cast<char*>(strRepetition.c_str()))!=0 &&
		!(IsDigit(strRepetition) && 1<=atoi(strRepetition.c_str())  && atoi(strRepetition.c_str())<=1000)
		) ||
		(
		stricmp("SONerror",const_cast<char*>(strStopCondition.c_str()))!=0 &&
		stricmp("SON",const_cast<char*>(strStopCondition.c_str()))!=0  &&
		stricmp("NONE",const_cast<char*>(strStopCondition.c_str()))!=0  	
		) ||
		(
		stricmp("NONE",const_cast<char*>(strStepMode.c_str()))!=0  &&
		stricmp("STEP",const_cast<char*>(strStepMode.c_str()))!=0 
		)
		)
	{
		return false;
	}
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:NPOWer:CONTrol ";
	//strCmd=strCmd+strStatics+","+strRepetition+","+strStopCondition+","+strStepMode+"; *OPC?";
	strCmd=strCmd+strStatics+","+strRepetition+","+strStopCondition+","+strStepMode;
	if (! GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_FetchFastPowerMeasurement(double& dAvgPwr_Current,double& dMinPwr_Current,double& dMaxPwr_Current,double& dAvgPwr_LastCycle,double& dMinPwr_Entire,double& dMaxPwr_Entire)
{
	double dMearsurment[6]={0};
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";ABORt:NPOWer; *OPC?";
	if (! GPIB_OPComplete(strCmd))
		return false;

	strCmd=MakeSTDstring("%d",g_iMode)+";INITiate:NPOWer; *OPC?";
	if (! GPIB_OPComplete(strCmd))
		return false;

	char *token = NULL;
	std::string strMeasured="";
	char seps[] = ",";

	int iRetryTimes=70;
	strCmd= MakeSTDstring("%d",g_iMode)+";FETCh:NPOWer:STATus?";
	while(iRetryTimes-->0)
	{
		if (! GPIB_Querry(strCmd, strMeasured))
			return false;

		if (strstr(strMeasured.c_str(), "RUN") == NULL)
			break;
		else
			Sleep(500);
	} 

	strCmd= MakeSTDstring("%d",g_iMode)+";FETCh:SCALar:NPOWer?";
	if (! GPIB_Querry(strCmd, strMeasured))
		return false;

	if (strstr((char*)strMeasured.c_str(),"NAN") != NULL){
		return false;
	}
	token = strtok( (char*)strMeasured.c_str(), seps);
	int i=0;
	while(token != NULL){
		dMearsurment[i++]= (double)atof(token);
		token = strtok( NULL, seps );
	}

	strCmd= MakeSTDstring("%d",g_iMode)+";ABORt:NPOWer; *OPC?";
	if (! GPIB_OPComplete(strCmd))
		return false;

	dAvgPwr_Current=dMearsurment[0];
	dMinPwr_Current=dMearsurment[1];
	dMaxPwr_Current=dMearsurment[2];
	dAvgPwr_LastCycle=dMearsurment[3];
	dMinPwr_Entire=dMearsurment[4];
	dMaxPwr_Entire=dMearsurment[5];

	return true;
}

bool CCMU200::WCDMA_SetSmartAlignmentRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode)
{
	if (
		(
		stricmp("CONT",const_cast<char*>(strRepetition.c_str()))!=0 && 
		stricmp("CONTINUOUS",const_cast<char*>(strRepetition.c_str()))!=0 &&
		stricmp("SINGLSHOT",const_cast<char*>(strRepetition.c_str()))!=0 &&
		stricmp("SING",const_cast<char*>(strRepetition.c_str()))!=0 &&
		!(IsDigit(strRepetition) && 1<=atoi(strRepetition.c_str())  && atoi(strRepetition.c_str())<=10000)
		) ||
		stricmp("NONE",const_cast<char*>(strStopCondition.c_str()))!=0
		  ||
		(
		stricmp("NONE",const_cast<char*>(strStepMode.c_str()))!=0  &&
		stricmp("STEP",const_cast<char*>(strStepMode.c_str()))!=0 
		)
		)
	{
		return false;
	}
	
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:POWer:SALignment:CONTrol:REPetition ";
	//strCmd=strCmd+strRepetition+","+strStopCondition+","+strStepMode+"; *OPC?";
	strCmd=strCmd+strRepetition+","+strStopCondition+","+strStepMode;
	//if (! GPIB_OPComplete(strCmd))
	if (! GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetSmartAlignmentConfig(int iFilterType,int iUnitType,double dStepWidth)
{
	//std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:POWer:SALignment:CONTrol:SWIDth "+MakeSTDstring("%f",dStepWidth)+"; *OPC?";
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:POWer:SALignment:CONTrol:SWIDth "+MakeSTDstring("%f",dStepWidth);
	//if (! GPIB_OPComplete(strCmd))
	if (! GPIB_Write(strCmd))
		return false;

	if (iFilterType==0)
		//strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:POWer:SALignment:CONTrol:FILTer WIDE; *OPC?";
		strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:POWer:SALignment:CONTrol:FILTer WIDE";
	else
		//strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:POWer:SALignment:CONTrol:FILTer F30K; *OPC?";
		strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:POWer:SALignment:CONTrol:FILTer F30K";
	//if (! GPIB_OPComplete(strCmd))
	if (! GPIB_Write(strCmd))
		return false;

	if (iFilterType==0)
		//strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:POWer:SALignment:CONTrol:SQUence:UNIT MHZ; *OPC?";
		strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:POWer:SALignment:CONTrol:SQUence:UNIT MHZ";
	else
		//strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:POWer:SALignment:CONTrol:SQUence:UNIT CH; *OPC?";
		strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:POWer:SALignment:CONTrol:SQUence:UNIT CH";
	//if (! GPIB_OPComplete(strCmd))
	if (! GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetSmartAlignmentConfigOfPowers(int iNumOfPower,double *dOutputPower,double *dMaxLevel)
{
	bool bRet = false;
	std::string strCmd="";

	strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:POWer:SALignment:CONTrol:SCOunt "+MakeSTDstring("%d",iNumOfPower+1);
	if(!GPIB_Write(strCmd))
		return false;

	std::string strPwr="CONFigure:POWer:SALignment:CONTrol:";
	for (int i=0;i<iNumOfPower;i++)
		strPwr=strPwr+"Step"+MakeSTDstring("%d",i+2)+" "+MakeSTDstring("%f",dOutputPower[i])+","+MakeSTDstring("%f",dMaxLevel[i])+";";

	if (!GPIB_Write(strPwr))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetSmartAlignmentConfigOfFreqs(int iNumOfFreq,double *dGenFreq,double *dAnalyzerFreq)
{
	bool bRet = false;
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:POWer:SALignment:CONTrol:SQCount "+MakeSTDstring("%d",iNumOfFreq+1);
	if (!GPIB_Write(strCmd))
		return false;

	std::string strFreqs="CONFigure:POWer:SALignment:CONTrol:";
	for (int i=0;i<iNumOfFreq;i++)
		strFreqs=strFreqs+"SQUence"+MakeSTDstring("%d",i+2)+" "+MakeSTDstring("%f",dGenFreq[i])+"MHZ,"+MakeSTDstring("%f",dAnalyzerFreq[i])+"MHZ;";

	if (!GPIB_Write(strFreqs))
		return false;

	return true;
}

bool CCMU200::WCDMA_InitiateSmartAlignmentMeasurement()
{
	//std::string strCmd=MakeSTDstring("%d",g_iMode)+";ABORt:POWer:SALignment; *OPC?";
	//if (! GPIB_OPComplete(strCmd))
	//	return false;

	std::string strCmd=MakeSTDstring("%d",g_iMode)+";INITiate:POWer:SALignment;*OPC?";
	if (!GPIB_OPComplete(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_FetchSmartAlignmentMeasurement(double* dMeasurements)
{


	char *token = NULL;
	std::string strMeasured="";
	char seps[] = ",";

	int iRetryTimes=70;
	std::string strCmd= MakeSTDstring("%d",g_iMode)+";FETCh:POWer:SALignment:STAT?";
	while(iRetryTimes-->0)
	{
		if (! GPIB_Querry(strCmd, strMeasured))
			return false;

		if (strstr(strMeasured.c_str(), "RUN") == NULL)
			break;
		else
			Sleep(500);
	} 

	strCmd= MakeSTDstring("%d",g_iMode)+";FETCh:POWer:SALignment?";
	if (! GPIB_Querry(strCmd, strMeasured))
		return false;

//	if (strstr((char*)strMeasured.c_str(),"NAN") != NULL){
//		return false;
//	}
	token = strtok( (char*)strMeasured.c_str(), seps);
	int i=0;
	while(token != NULL){
		dMeasurements[i++]= (double)atof(token);
		token = strtok( NULL, seps );
	}

	strCmd= MakeSTDstring("%d",g_iMode)+";ABORt:POWer:SALignment; *OPC?";
	if (! GPIB_OPComplete(strCmd))
		return false;

	return true;
}

// -------------------- GSM Calibration functions ----------------------
bool CCMU200::GSM_FetchTxSweepRez(float *fTxPowerArray,float *fTxPhasArray,int iSizeofTotalTable)
{
	int i = 0;
	int j = 0;
	char seps[] = ",";
	char *token = NULL;
	char *next_token = NULL;

	// -----------------------------------
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strMeasured;

	sprintf_s(szCommand, 255, "%d;FETC:IQSL:STATus?", g_iMode);
	strCommand = szCommand ;

	while (true)
	{
		if (! GPIB_Querry(strCommand, strMeasured))
			return false;

		if (strstr(strMeasured.c_str(), "RUN") == NULL)
			break;
		else
			Sleep(500);
	}

	// -----------------------------------
    
	sprintf_s(szCommand, 255, "%d;FETC:ARR:IQSL:LEV?", g_iMode);
	strCommand = szCommand ;
	if (! GPIB_Querry(strCommand, strMeasured))
		return false;

	if (strstr((char*)strMeasured.c_str(),"NAN") != NULL){
		return false;
	}
	token = strtok_s( (char*)strMeasured.c_str(), seps, &next_token);
	while((i<iSizeofTotalTable) && (token != NULL)){
		fTxPowerArray[i]= (float)atof(token);
		//token = strtok( NULL, seps );
		token = strtok_s( NULL, seps, &next_token);
		i++;
	}
 
	sprintf_s(szCommand, 255, "%d;FETC:ARR:IQSL:PHAS?", g_iMode);
	strCommand = szCommand ;
	
	if (! GPIB_Querry(strCommand, strMeasured))
		return false;

	if (strstr((char*)strMeasured.c_str(),"NAN") != NULL){
		return false;
	}
	token = strtok_s( (char*)strMeasured.c_str(), seps, &next_token);
	while((j<iSizeofTotalTable) && ( token != NULL )){
		fTxPhasArray[j]= (float)atof(token);
		token = strtok_s( NULL, seps, &next_token);
		j++;
	}
 
	return true;
}

bool CCMU200::GSM_FetchIQRecorderPL(float *fTxPowerArray, float *fTxPhaseArray, int iSizeofTotalTable)
{
	char szCommand[256];
	std::string strRead, strCommand;

	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "%d;FETCh:IQRecorder:STATus?", g_iMode);

	while (true)
	{
		if (! GPIB_Querry(std::string(szCommand), strRead))
			return false;

		if (strstr(strRead.c_str(), "RUN") != NULL)
		{
			Sleep(500);
			continue;
		}
		else
			break;
	}

	if (strstr(strRead.c_str(), _T("RDY")) != NULL)
	{
		memset(szCommand, 0, 256);
		sprintf_s(szCommand, 255, "%d;FETCh:BINary:ARRay:IQRecorder:PL? ALTernate", g_iMode);

		char ReadBuf[160000];
		memset(ReadBuf, 0, 160000);

		if (! GPIB_Query_char(std::string(szCommand)))
			return false;

		memcpy(ReadBuf, GetReadBuffer(), 160000);

		if (memcmp(ReadBuf, "OK", 2) == 0)
		{
			int L = ReadBuf[4] - '0';

			char a[4];
			int i = 0;
			while (i < iSizeofTotalTable)
			{
				/*
				example:
				a[0] = 0x0;
				a[1] = 0x0;
				a[2] = 0x20;
				a[3] = 0x3E;
				b = 0.15625
				*/

				a[0] = ReadBuf[i * 8 + L + 5];
				a[1] = ReadBuf[i * 8 + L + 6];
				a[2] = ReadBuf[i * 8 + L + 7];
				a[3] = ReadBuf[i * 8 + L + 8];
				float b = *((float *)a);
				fTxPhaseArray[i] = b;

				a[0] = ReadBuf[i * 8 + L + 9];
				a[1] = ReadBuf[i * 8 + L + 10];
				a[2] = ReadBuf[i * 8 + L + 11];
				a[3] = ReadBuf[i * 8 + L + 12];
				b = *((float *)a);
				fTxPowerArray[i] = b;

				++i;
			}

			return true;
		}
	}

	return false;
}
// -------------------- WCDMA Test functions ----------------------
bool CCMU200::WCDMA_ReferenceChannelType(std::string strType)
{
	if (
		stricmp("12.2",const_cast<char*>(strType.c_str()))!=0 && 
		stricmp("64",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("384",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("144",const_cast<char*>(strType.c_str()))!=0
	   )
	{
		return false;
	}

	std::string strCmd=MakeSTDstring("%d",g_iMode)+";SOURce:RFGenerator:RCHannel:TYPE "+strType+" kbps";
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetScramblingCode(int iPrimaryScramblingCode,int iSecondaryScramblingCode)
{
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";SOURce:RFGenerator:SCODe:PRIMary "+MakeSTDstring("%d",iPrimaryScramblingCode);
	if (!GPIB_Write(strCmd))
		return false;

	strCmd=MakeSTDstring("%d",g_iMode)+";SOURce:RFGenerator:SCODe:SECondary "+MakeSTDstring("%d",iSecondaryScramblingCode);
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetBSSynchronizationChannelLevel(double dPCPICH_Level,double dPSCH_Level,double dSSCH_Level,double SCPICH_Level)
{
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";SOURce:RFGenerator:SCHannel:LEVel:PCPich "+MakeSTDstring("%f",dPCPICH_Level);
	if (!GPIB_Write(strCmd))
		return false;

	strCmd=MakeSTDstring("%d",g_iMode)+";SOURce:RFGenerator:SCHannel:LEVel:PSCH "+MakeSTDstring("%f",dPSCH_Level);
	if (!GPIB_Write(strCmd))
		return false;

	strCmd=MakeSTDstring("%d",g_iMode)+";SOURce:RFGenerator:SCHannel:LEVel:SSCH "+MakeSTDstring("%f",dSSCH_Level);
	if (!GPIB_Write(strCmd))
		return false;

	strCmd=MakeSTDstring("%d",g_iMode)+";SOURce:RFGenerator:SCHannel:LEVel:SCPICH "+MakeSTDstring("%f",SCPICH_Level);
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetBSPhysicalChannelLevel(double dP_CPICH,double dP_SCH,double dS_SCH,double dP_CCPCH,double dPICH,double dDPDCH,double dPowerOffset)
{
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";SOURce:RFGenerator:PHYSical:LEVel "
								+MakeSTDstring("%f",dP_CPICH)+" dB,"+MakeSTDstring("%f",dP_SCH)+" dB,"+MakeSTDstring("%f",dS_SCH)+" dB,"+MakeSTDstring("%f",dP_CCPCH)
								+" dB,"+MakeSTDstring("%f",dPICH)+" dB,"+MakeSTDstring("%f",dDPDCH)+" dB,"+MakeSTDstring("%f",dPowerOffset);
	if (!GPIB_Write(strCmd))
		return false;
	return true;
}

bool CCMU200::WCDMA_SetBSDedicatedBroadcastChannelCode(int iDPDCHCode,int iPICHCode)
{
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";SOURce:RFGenerator:DBCHannel:PICH:CODE "+MakeSTDstring("%d",iPICHCode);
	if (!GPIB_Write(strCmd))
		return false;

	strCmd=MakeSTDstring("%d",g_iMode)+";SOURce:RFGenerator:DBCHannel:DPDCh:CODE "+MakeSTDstring("%d",iDPDCHCode);
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetUEScramblingCode(int iSramblingCode)
{
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:UECode:DPCH:SCODe "+MakeSTDstring("%d",iSramblingCode);
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetBS_TPCControlType(int iPatternSetNumber,std::string strType)
{
	if (
		stricmp("ALT",const_cast<char*>(strType.c_str()))!=0 && 
		stricmp("ALL0",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("ALL1",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("CPAT",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("SPA0",const_cast<char*>(strType.c_str()))!=0 && 
		stricmp("SPA1",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("SPAL",const_cast<char*>(strType.c_str()))!=0 &&
		stricmp("CLOP",const_cast<char*>(strType.c_str()))!=0
		)
	{
		return false;
	}

	std::string strCmd=MakeSTDstring("%d",g_iMode)+";SOURce:RFGenerator:TPC:PTYPe"+MakeSTDstring("%d",iPatternSetNumber)+" "+strType;
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetBS_TPC_PatternSet(int iPatternSetNumber)
{
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";SOURce:RFGenerator:TPC:PSET SET"+MakeSTDstring("%d",iPatternSetNumber);
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetBS_TPC_AlgorithmMode(int iAlg)
{
	if(iAlg!=1&&iAlg!=2)
		return false;
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";SOURce:RFGenerator:TPC:MODE ";
	if (iAlg==1)
		strCmd=strCmd+"ALG1";
	else
		strCmd=strCmd+"ALG2";
	
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetBS_TPC_PatternSet(std::string strPatternSet)
{
	if (
		stricmp("SET1",const_cast<char*>(strPatternSet.c_str()))!=0 && 
		stricmp("SET2",const_cast<char*>(strPatternSet.c_str()))!=0 &&
		stricmp("SET3",const_cast<char*>(strPatternSet.c_str()))!=0 &&
		stricmp("SET4",const_cast<char*>(strPatternSet.c_str()))!=0 &&
		stricmp("SET5",const_cast<char*>(strPatternSet.c_str()))!=0 && 
		stricmp("TA",const_cast<char*>(strPatternSet.c_str()))!=0 &&
		stricmp("TB",const_cast<char*>(strPatternSet.c_str()))!=0 &&
		stricmp("TC",const_cast<char*>(strPatternSet.c_str()))!=0 &&
		stricmp("TD",const_cast<char*>(strPatternSet.c_str()))!=0 &&
		stricmp("TE",const_cast<char*>(strPatternSet.c_str()))!=0 &&
		stricmp("TF",const_cast<char*>(strPatternSet.c_str()))!=0 &&
		stricmp("TG",const_cast<char*>(strPatternSet.c_str()))!=0 &&
		stricmp("TH",const_cast<char*>(strPatternSet.c_str()))!=0 &&
		stricmp("PDOW",const_cast<char*>(strPatternSet.c_str()))!=0 &&
		stricmp("PUP",const_cast<char*>(strPatternSet.c_str()))!=0
		)
	{
		return false;
	}
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";SOURce:RFGenerator:TPC:PSET "+strPatternSet;
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}

bool  CCMU200::WCDMA_TurnOnILPC_Measurement(bool bOn)
{
	
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";SOURce:RFGenerator:TPC:PCONditions ";
	if (bOn)
		strCmd=strCmd+"AUTO";
	else
		strCmd=strCmd+"MAN";

	if (!GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_TurnBS_TPC_PatternOn()
{
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";SOURce:RFGenerator:TPC:RPATtern  ON; *OPC?";

	if (!GPIB_OPComplete(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_FetchILPCMearsurement(std::string strILPCMode,double dMaxInputPower,double* dPowerVsSlotArray)
{
	char *token = NULL;
	std::string strMeasured="";
	char seps[] = ",";
	int iRetryTimes=70;
	

	if (
		stricmp("A",const_cast<char*>(strILPCMode.c_str()))!=0 &&
		stricmp("B",const_cast<char*>(strILPCMode.c_str()))!=0 &&
		stricmp("C",const_cast<char*>(strILPCMode.c_str()))!=0 &&
		stricmp("D",const_cast<char*>(strILPCMode.c_str()))!=0 &&
		stricmp("E",const_cast<char*>(strILPCMode.c_str()))!=0 &&
		stricmp("F",const_cast<char*>(strILPCMode.c_str()))!=0 &&
		stricmp("G",const_cast<char*>(strILPCMode.c_str()))!=0 &&
		stricmp("H",const_cast<char*>(strILPCMode.c_str()))!=0 
		)
	{
		return false;
	}
	/*
	 if(!WCDMA_SetBS_TPCControlType(2,"ALL1"))
		 return false;

	 if(!WCDMA_SetBS_TPC_PatternSet("T"+strILPCMode))
		 return false;
	 
	 if (!RF_SetTriggerSourceType("TPC"))
		return false;

	 if(!WCDMA_SetPowerVsSlotMeasuredRepetition("SING","NONE","NONE"))
		 return false;

	 if (!WCDMA_SetPowerVsSlotMeasuredMode("TSTeps"))
		 return false;

	 if (strILPCMode=="E")
	 {
		 if (!SetMaxPower(dMaxInputPower))
			 return false;
	 }
	 else if(strILPCMode=="F")
		if(!GSM_SetMSMaxPower(dMaxInputPower))
			return false;
	 

	 if(!WCDMA_TurnOnILPC_Measurement(true))
		 return false;

	 if(!WCDMA_InitiatePowerVsSlotMeasurment())
		 return false;

	 if (!WCDMA_TurnBS_TPC_PatternOn())
		return false;
	 */
	 std::string strCmd= MakeSTDstring("%d",g_iMode)+";FETCh:POWer:XSLot:STATus?";
	 while(iRetryTimes-->0)
	 {
		 if (! GPIB_Querry(strCmd, strMeasured))
			 return false;

		 if (strstr(strMeasured.c_str(), "RUN") == NULL)
			 break;
		 else
			 Sleep(500);
	 } 

	 strCmd= MakeSTDstring("%d",g_iMode)+";SAMPle:POWer:XSLot:ABSolute?";
	 if (! GPIB_Querry(strCmd, strMeasured))
		 return false;

	 token = strtok( (char*)strMeasured.c_str(), seps);
	 int i=0;
	 while(token != NULL){
		 if(stricmp("NAN",token)==0)
			 dPowerVsSlotArray[i++]=0;
		 else
			 dPowerVsSlotArray[i++]= (double)atof(token);
		 token = strtok( NULL, seps );
	 }

	 strCmd= MakeSTDstring("%d",g_iMode)+";ABORt:POWer:XSLot; *OPC?";
	 if (! GPIB_OPComplete(strCmd))
		 return false;
	 /*
	 if (!RF_SetTriggerSourceType("FRUN"))
		 return false;

	 if(!WCDMA_TurnOnILPC_Measurement(false))
		 return false;
	 */

	 return true;
}


bool CCMU200::WCDMA_SetACLRMearsuredRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode)
{
	if (
		(
		stricmp("CONT",const_cast<char*>(strRepetition.c_str()))!=0 && 
		stricmp("CONTINUOUS",const_cast<char*>(strRepetition.c_str()))!=0 &&
		stricmp("SINGLSHOT",const_cast<char*>(strRepetition.c_str()))!=0 &&
		stricmp("SING",const_cast<char*>(strRepetition.c_str()))!=0 &&
		!(IsDigit(strRepetition) && 1<=atoi(strRepetition.c_str())  && atoi(strRepetition.c_str())<=10000)
		) ||
		(
		stricmp("NONE",const_cast<char*>(strStopCondition.c_str()))!=0 &&
		stricmp("SONerror",const_cast<char*>(strStopCondition.c_str()))!=0 &&
		stricmp("SON",const_cast<char*>(strStopCondition.c_str()))!=0 
		)
		||
		(
		stricmp("NONE",const_cast<char*>(strStepMode.c_str()))!=0  &&
		stricmp("STEP",const_cast<char*>(strStepMode.c_str()))!=0 
		)
		)
	{
		return false;
	}
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:SPECtrum:MFILter:CONTrol:REPetition "+strRepetition+","+strStopCondition+","+strStepMode;
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}
bool CCMU200::WCDMA_SetACLRMearsuredLength(int iChipNumbers)
{
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:SPECtrum:MFILter:CONTrol:MLENgth "+MakeSTDstring("%d",iChipNumbers);
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}
bool CCMU200::WCDMA_SetACLRNumberOfBurstsToMearsured(int iNumberOfBursts)
{
	if (iNumberOfBursts<0 || 1000<iNumberOfBursts)
		return false;

	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:SPECtrum:MFILter:CONTrol:STATistics "+MakeSTDstring("%d",iNumberOfBursts);
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}
bool CCMU200::WCDMA_TurnOnOffACLRMearsurement(bool bOn)
{

	std::string strCmd="";
	if (bOn)
		strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:SPECtrum:MFILter:CONTrol:UEPMeasure ON";
	else
		strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:SPECtrum:MFILter:CONTrol:UEPMeasure OFF";
	
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_FetchACLRMearsurement(
										  double& dCarrierPower_PeakCurrent,	double& dCarrierPower_RMSCurrent,
										  double& dACLR_PeakCurrent_Minus10MHz,	double& dACLR_PeakCurrent_Minus5MHz,double& dACLR_PeakCurrent_5MHz,	double& dACLR_PeakCurrent_10MHz,
										  double& dACLR_RMSCurrent_Minus10MHz,	double& dACLR_RMSCurrent_Minus5MHz,	double& dACLR_RMSCurrent_5MHz,	double& dACLR_RMSCurrent_10MHz,
										  double& dACLR_RMSAvg_Minus10MHz,		double& dACLR_RMSAvg_Minus5MHz,		double& dACLR_RMSAvg_5MHz,		double& dACLR_RMSAvg_10MHz,
										  double& dACLR_PeakMax_Minus10MHz,		double& dACLR_PeakMax_Minus5MHz,	double& dACLR_PeakMax_5MHz,		double& dACLR_PeakMax_10MHz,
										  double& dACLR_RMSMax_Minus10MHz,		double& dACLR_RMSMax_Minus5MHz,		double& dACLR_RMSMax_5MHz,		double& dACLR_RMSMax_10MHz,
										  double& dUEPower_RMSCurrent,			double& dOutOfTolerance
										  )
{
	double dMearsurment[24]={0};
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";ABORt:SPECtrum:MFILter; *OPC?";
	if (! GPIB_OPComplete(strCmd))
		return false;

	strCmd=MakeSTDstring("%d",g_iMode)+";INITiate:SPECtrum:MFILter; *OPC?";
	if (! GPIB_OPComplete(strCmd))
		return false;

	char *token = NULL;
	std::string strMeasured="";
	char seps[] = ",";

	int iRetryTimes=70;
	strCmd= MakeSTDstring("%d",g_iMode)+";FETCh:SPECtrum:MFILter:STATus?";
	while(iRetryTimes-->0)
	{
		if (! GPIB_Querry(strCmd, strMeasured))
			return false;

		if (strstr(strMeasured.c_str(), "RUN") == NULL)
			break;
		else
			Sleep(500);
	} 

	strCmd= MakeSTDstring("%d",g_iMode)+";SAMPle:SCALar:SPECtrum:MFILter:RELative?";
	if (! GPIB_Querry(strCmd, strMeasured))
		return false;

	token = strtok( (char*)strMeasured.c_str(), seps);
	int i=0;
	while(token != NULL){
		if(stricmp("NAN",token)==0)
			dMearsurment[i++]=0;
		else
			dMearsurment[i++]= (double)atof(token);
		token = strtok( NULL, seps );
	}

	strCmd= MakeSTDstring("%d",g_iMode)+";ABORt:SPECtrum:MFILter; *OPC?";
	if (! GPIB_OPComplete(strCmd))
		return false;

	dCarrierPower_PeakCurrent	=	dMearsurment[0];
	dCarrierPower_RMSCurrent	=	dMearsurment[1];
	dACLR_PeakCurrent_Minus10MHz=	dMearsurment[2];
	dACLR_PeakCurrent_Minus5MHz	=	dMearsurment[3];
	dACLR_PeakCurrent_5MHz		=	dMearsurment[4];
	dACLR_PeakCurrent_10MHz		=	dMearsurment[5];
	dACLR_RMSCurrent_Minus10MHz	=	dMearsurment[6];
	dACLR_RMSCurrent_Minus5MHz	=	dMearsurment[7];
	dACLR_RMSCurrent_5MHz		=	dMearsurment[8];
	dACLR_RMSCurrent_10MHz		=	dMearsurment[9];
	dACLR_RMSAvg_Minus10MHz		=	dMearsurment[10];
	dACLR_RMSAvg_Minus5MHz		=	dMearsurment[11];
	dACLR_RMSAvg_5MHz			=	dMearsurment[12];
	dACLR_RMSAvg_10MHz			=	dMearsurment[13];
	dACLR_PeakMax_Minus10MHz	=	dMearsurment[14];
	dACLR_PeakMax_Minus5MHz		=	dMearsurment[15];
	dACLR_PeakMax_5MHz			=	dMearsurment[16];
	dACLR_PeakMax_10MHz			=	dMearsurment[17];
	dACLR_RMSMax_Minus10MHz		=	dMearsurment[18];
	dACLR_RMSMax_Minus5MHz		=	dMearsurment[19];
	dACLR_RMSMax_5MHz			=	dMearsurment[20];
	dACLR_RMSMax_10MHz			=	dMearsurment[21];
	dUEPower_RMSCurrent			=	dMearsurment[22];
	dOutOfTolerance				=	dMearsurment[23];

	return true;
}

bool CCMU200::WCDMA_SetOBWNumberOfBurstsToMearsured(int iNumberOfBursts)
{
	if (iNumberOfBursts<0 || 1000<iNumberOfBursts)
		return false;

	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:SPECtrum:MFFT:CONTrol:STATistics "+MakeSTDstring("%d",iNumberOfBursts);
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetOBWMeasuredRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode)
{
	if (
		(
		stricmp("CONT",const_cast<char*>(strRepetition.c_str()))!=0 && 
		stricmp("CONTINUOUS",const_cast<char*>(strRepetition.c_str()))!=0 &&
		stricmp("SINGLSHOT",const_cast<char*>(strRepetition.c_str()))!=0 &&
		stricmp("SING",const_cast<char*>(strRepetition.c_str()))!=0 &&
		!(IsDigit(strRepetition) && 1<=atoi(strRepetition.c_str())  && atoi(strRepetition.c_str())<=10000)
		) ||
		(
		stricmp("NONE",const_cast<char*>(strStopCondition.c_str()))!=0 &&
		stricmp("SONerror",const_cast<char*>(strStopCondition.c_str()))!=0 &&
		stricmp("SON",const_cast<char*>(strStopCondition.c_str()))!=0 
		)
		||
		(
		stricmp("NONE",const_cast<char*>(strStepMode.c_str()))!=0  &&
		stricmp("STEP",const_cast<char*>(strStepMode.c_str()))!=0 
		)
		)
	{
		return false;
	}

	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:SPECtrum:MFFT:CONTrol:REPetition "+strRepetition+","+strStopCondition+","+strStepMode;
	if (!GPIB_Write(strCmd))
		return false;

	return true;

}
bool CCMU200::WCDMA_SetOBWMeasuredType(std::string strType)
{
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:SPECtrum:MFFT:CONTrol:RMODe "+strType;
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetOBWMearsuredLength(int iChipNumbers)
{
	if(iChipNumbers!=320 && iChipNumbers != 640 && iChipNumbers != 1280 && iChipNumbers != 2560)
		return false;

	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:SPECtrum:MFFT:CONTrol:MLENgth "+MakeSTDstring("%d",iChipNumbers);
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetDefaultOBWMeasurement(bool bOn)
{
	std::string strCmd="";
	if (bOn)
		strCmd=MakeSTDstring("%d",g_iMode)+";DEFault:SPECtrum:MFFT:CONTrol ON";
	else
		strCmd=MakeSTDstring("%d",g_iMode)+";DEFault:SPECtrum:MFFT:CONTrol OFF";

	if (!GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_TurnOnOffMaxDynamicRangeForACLR_OBW(bool bOn)
{
	std::string strCmd="";
	if (bOn)
		strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:SPECtrum:CONTrol:HDYNamic ON";
	else
		strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:SPECtrum:CONTrol:HDYNamic OFF";

	if (!GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_FetchOBWMearsurement(
										double& dCarrierPower_PeakCurrent,		double& dCarrierPower_RMSCurrent,
										double& dACLR_RMSCurrent_Minus10MHz,	double& dACLR_RMSCurrent_Minus5MHz,	double& dACLR_RMSCurrent_5MHz,	double& dACLR_RMSCurrent_10MHz,
										double& dACLR_RMSAvg_Minus10MHz,		double& dACLR_RMSAvg_Minus5MHz,		double& dACLR_RMSAvg_5MHz,		double& dACLR_RMSAvg_10MHz,
										double& dACLR_RMSMax_Minus10MHz,		double& dACLR_RMSMax_Minus5MHz,		double& dACLR_RMSMax_5MHz,		double& dACLR_RMSMax_10MHz,
										double& dOBW_Current,					double& dOBW_Avg,					double& dOBW_Max,				double& dOBW_Left,
										double& dOBW_Right,						double& dUEPower_RMSCurrent,		double& dOutOfTolerance
										)
{
	double dMearsurment[21]={0};
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";ABORt:SPECtrum:MFFT; *OPC?";
	if (! GPIB_OPComplete(strCmd))
		return false;

	strCmd=MakeSTDstring("%d",g_iMode)+";INITiate:SPECtrum:MFFT; *OPC?";
	if (! GPIB_OPComplete(strCmd))
		return false;

	char *token = NULL;
	std::string strMeasured="";
	char seps[] = ",";

	int iRetryTimes=70;
	strCmd= MakeSTDstring("%d",g_iMode)+";FETCh:SPECtrum:MFFT:STATus?";
	while(iRetryTimes-->0)
	{
		if (! GPIB_Querry(strCmd, strMeasured))
			return false;

		if (strstr(strMeasured.c_str(), "RUN") == NULL)
			break;
		else
			Sleep(500);
	} 

	strCmd= MakeSTDstring("%d",g_iMode)+";SAMPle:SCALar:SPECtrum:MFFT:RELative?";
	if (! GPIB_Querry(strCmd, strMeasured))
		return false;

	token = strtok( (char*)strMeasured.c_str(), seps);
	int i=0;
	while(token != NULL){
		if(stricmp("NAN",token)==0)
			dMearsurment[i++]=NONE_VALUE;
		else
			dMearsurment[i++]= (double)atof(token);
		token = strtok( NULL, seps );
	}

	strCmd= MakeSTDstring("%d",g_iMode)+";ABORt:SPECtrum:MFFT; *OPC?";
	if (! GPIB_OPComplete(strCmd))
		return false;

	dCarrierPower_PeakCurrent	=	dMearsurment[0];
	dCarrierPower_RMSCurrent	=	dMearsurment[1];
	dACLR_RMSCurrent_Minus10MHz	=	dMearsurment[2];
	dACLR_RMSCurrent_Minus5MHz	=	dMearsurment[3];
	dACLR_RMSCurrent_5MHz		=	dMearsurment[4];
	dACLR_RMSCurrent_10MHz		=	dMearsurment[5];
	dACLR_RMSAvg_Minus10MHz		=	dMearsurment[6];
	dACLR_RMSAvg_Minus5MHz		=	dMearsurment[7];
	dACLR_RMSAvg_5MHz			=	dMearsurment[8];
	dACLR_RMSAvg_10MHz			=	dMearsurment[9];
	dACLR_RMSMax_Minus10MHz		=	dMearsurment[10];
	dACLR_RMSMax_Minus5MHz		=	dMearsurment[11];
	dACLR_RMSMax_5MHz			=	dMearsurment[12];
	dACLR_RMSMax_10MHz			=	dMearsurment[13];
	dOBW_Current				=	dMearsurment[14]/pow((float)10, 6);
	dOBW_Avg					=	dMearsurment[15]/pow((float)10, 6);
	dOBW_Max					=	dMearsurment[16]/pow((float)10, 6);
	dOBW_Left					=	dMearsurment[17]/pow((float)10, 6);
	dOBW_Right					=	dMearsurment[18]/pow((float)10, 6);
	dUEPower_RMSCurrent			=	dMearsurment[19];
	dOutOfTolerance				=	dMearsurment[20];

	return true;
}

bool CCMU200::WCDMA_SetSEMNumberOfBurstsToMearsured(int iNumberOfBursts)
{
	if (iNumberOfBursts<0 || 1000<iNumberOfBursts)
		return false;

	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:SPECtrum:EMASk:CONTrol:STATistics "+MakeSTDstring("%d",iNumberOfBursts);
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}
bool CCMU200::WCDMA_SetSEMMearsuredRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode)
{
	if (
		(
		stricmp("CONT",const_cast<char*>(strRepetition.c_str()))!=0 && 
		stricmp("CONTINUOUS",const_cast<char*>(strRepetition.c_str()))!=0 &&
		stricmp("SINGLSHOT",const_cast<char*>(strRepetition.c_str()))!=0 &&
		stricmp("SING",const_cast<char*>(strRepetition.c_str()))!=0 &&
		!(IsDigit(strRepetition) && 1<=atoi(strRepetition.c_str())  && atoi(strRepetition.c_str())<=10000)
		) ||
		(
		stricmp("NONE",const_cast<char*>(strStopCondition.c_str()))!=0 &&
		stricmp("SONerror",const_cast<char*>(strStopCondition.c_str()))!=0 &&
		stricmp("SON",const_cast<char*>(strStopCondition.c_str()))!=0 
		)
		||
		(
		stricmp("NONE",const_cast<char*>(strStepMode.c_str()))!=0  &&
		stricmp("STEP",const_cast<char*>(strStepMode.c_str()))!=0 
		)
		)
	{
		return false;
	}

	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:SPECtrum:EMASk:CONTrol:REPetition "+strRepetition+","+strStopCondition+","+strStepMode;
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}
bool CCMU200::WCDMA_SetSEMMeasuredType(std::string strType)
{
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:SPECtrum:EMASk:CONTrol:RMODe "+strType;
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}
bool CCMU200::WCDMA_SetSEMMearsuredLength(int iChipNumbers)
{
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:SPECtrum:EMASk:CONTrol:MLENgth "+MakeSTDstring("%d",iChipNumbers);
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}
bool CCMU200::WCDMA_SetDefaultSEMMeasurement(bool bOn)
{
	std::string strCmd="";
	if (bOn)
		strCmd=MakeSTDstring("%d",g_iMode)+";DEFault:SPECtrum:EMASk:LIMit:LINE ON";
	else
		strCmd=MakeSTDstring("%d",g_iMode)+";DEFault:SPECtrum:EMASk:LIMit:LINE OFF";

	if (!GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetSEMLineMaskPower(double dMaskPwr_12MHz,double dMaskPwr_8_5MHz,double dMaskPwr_7_5MHz,double dMaskPwr_4MHz,double dMaskPwr_3_845MHz,double dMaskPwr_2_515MHz)
{
	std::string strCmd="";
	if(dMaskPwr_12MHz>0 || dMaskPwr_12MHz < -90)
		return false;
	
	if(dMaskPwr_8_5MHz>0 || dMaskPwr_8_5MHz < -90)
		return false;
	
	if(dMaskPwr_4MHz>0 || dMaskPwr_4MHz < -90)
		return false;
	
	if(dMaskPwr_3_845MHz>0 || dMaskPwr_3_845MHz < -90)
		return false;

	if(dMaskPwr_2_515MHz>0 || dMaskPwr_2_515MHz < -90)
		return false;

	strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:SPECtrum:EMASk:RELative:AVERage:LIMit:LINE:SYMmetric:VALue "
			+MakeSTDstring("%f",dMaskPwr_12MHz)+","
			+MakeSTDstring("%f",dMaskPwr_8_5MHz)+","
			+MakeSTDstring("%f",dMaskPwr_7_5MHz)+","
			+MakeSTDstring("%f",dMaskPwr_4MHz)+","
			+MakeSTDstring("%f",dMaskPwr_3_845MHz)+","
			+MakeSTDstring("%f",dMaskPwr_2_515MHz);

	if (!GPIB_Write(strCmd))
		return false;

	strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:SPECtrum:EMASk:RELative:CMAX:LIMit:LINE:SYMmetric:VALue "
		+MakeSTDstring("%f",dMaskPwr_12MHz)+","
		+MakeSTDstring("%f",dMaskPwr_8_5MHz)+","
		+MakeSTDstring("%f",dMaskPwr_7_5MHz)+","
		+MakeSTDstring("%f",dMaskPwr_4MHz)+","
		+MakeSTDstring("%f",dMaskPwr_3_845MHz)+","
		+MakeSTDstring("%f",dMaskPwr_2_515MHz);

	if (!GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetSEMMeasurementLowBoundaryCheck(bool bOn_Max,bool bOn_Average)
{
	std::string strCmd="";
	if (bOn_Max)
		strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:SPECtrum:EMASk:ABSolute:CMAX:LIMit:LINE:SYMmetric:ENABle ON";
	else
		strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:SPECtrum:EMASk:ABSolute:CMAX:LIMit:LINE:SYMmetric:ENABle OFF";

	if (!GPIB_Write(strCmd))
		return false;

	if (bOn_Average)
		strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:SPECtrum:EMASk:ABSolute:AVERage:LIMit:LINE:SYMmetric:ENABle ON";
	else
		strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:SPECtrum:EMASk:ABSolute:AVERage:LIMit:LINE:SYMmetric:ENABle OFF";

	if (!GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_FetchSEMMearsurement(
										std::string strMeasuredType,
										double& dReferencePower_Current,double& dReferencePower_Avg,double& dReferencePower_Max,
										double& dSEM_Minus_12_8p5MHz,	double& dSEM_Minus_8p5_7p5MHz,	double& dSEM_Minus_7p5_4MHz,double& dSEM_Minus_3p845_2p515MHz,
										double& dSEM_12_8p5MHz,			double& dSEM_8p5_7p5MHz,		double& dSEM_7p5_4MHz,		double& dSEM_3p845_2p515MHz,
										double& dUEPower_RMSCurrent,	double& dOutOfTolerance
										)
{
	if (
		
		stricmp("Max",const_cast<char*>(strMeasuredType.c_str()))!=0 && 
		stricmp("current",const_cast<char*>(strMeasuredType.c_str()))!=0 &&
		stricmp("Average",const_cast<char*>(strMeasuredType.c_str()))!=0
	   )
	{
		return false;
	}

	double dMearsurment[21]={0};
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";ABORt:SPECtrum:EMASk; *OPC?";
	if (! GPIB_OPComplete(strCmd))
		return false;

	strCmd=MakeSTDstring("%d",g_iMode)+";INITiate:SPECtrum:EMASk; *OPC?";
	if (! GPIB_OPComplete(strCmd))
		return false;

	char *token = NULL;
	std::string strMeasured="";
	char seps[] = ",";

	int iRetryTimes=70;
	strCmd= MakeSTDstring("%d",g_iMode)+";FETCh:SPECtrum:EMASk:STATus?";
	while(iRetryTimes-->0)
	{
		if (! GPIB_Querry(strCmd, strMeasured))
			return false;

		if (strstr(strMeasured.c_str(), "RUN") == NULL)
			break;
		else
			Sleep(500);
	} 
	{
		strCmd= MakeSTDstring("%d",g_iMode)+";SAMPle:SPECtrum:EMASk?";
		if (! GPIB_Querry(strCmd, strMeasured))
			return false;

		token = strtok( (char*)strMeasured.c_str(), seps);
		int i=0;
		while(token != NULL){
			if(stricmp("NAN",token)==0)
				dMearsurment[i++]=NONE_VALUE;
			else
				dMearsurment[i++]= (double)atof(token);
			token = strtok( NULL, seps );
		}

		dReferencePower_Current	=	dMearsurment[0];
		dReferencePower_Avg	=	dMearsurment[1];
		dReferencePower_Max	=	dMearsurment[2];
		dUEPower_RMSCurrent	=	dMearsurment[3];
		dOutOfTolerance		=	dMearsurment[4];
	}
	{
		if (stricmp("Average",const_cast<char*>(strMeasuredType.c_str()))==0)
		{
			strCmd= MakeSTDstring("%d",g_iMode)+";SAMPle:SPECtrum:EMASk:AVERage:MARGin?";
		}
		else if(stricmp("current",const_cast<char*>(strMeasuredType.c_str()))==0)
			strCmd= MakeSTDstring("%d",g_iMode)+";SAMPle:SPECtrum:EMASk:CURRent:MARGin?";
		else
			strCmd= MakeSTDstring("%d",g_iMode)+";SAMPle:SPECtrum:EMASk:MAXimum:MARGin?";

		if (! GPIB_Querry(strCmd, strMeasured))
			return false;

		token = strtok( (char*)strMeasured.c_str(), seps);
		int i=0;
		while(token != NULL){
			if(stricmp("NAN",token)==0)
				dMearsurment[i++]=NONE_VALUE;
			else
				dMearsurment[i++]= (double)atof(token);
			token = strtok( NULL, seps );
		}

		dSEM_Minus_12_8p5MHz	=	dMearsurment[0];
		dSEM_Minus_8p5_7p5MHz	=	dMearsurment[1];
		dSEM_Minus_7p5_4MHz		=	dMearsurment[2];
		dSEM_Minus_3p845_2p515MHz=	dMearsurment[3];
		dSEM_3p845_2p515MHz		=	dMearsurment[4];
		dSEM_7p5_4MHz			=	dMearsurment[5];
		dSEM_8p5_7p5MHz			=	dMearsurment[6];
		dSEM_12_8p5MHz			=	dMearsurment[7];
	}

	strCmd= MakeSTDstring("%d",g_iMode)+";ABORt:SPECtrum:EMASk; *OPC?";
	if (! GPIB_OPComplete(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetBERMearsuredRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode)
{
	if (
		(
		stricmp("CONT",const_cast<char*>(strRepetition.c_str()))!=0 && 
		stricmp("CONTINUOUS",const_cast<char*>(strRepetition.c_str()))!=0 &&
		stricmp("SINGLSHOT",const_cast<char*>(strRepetition.c_str()))!=0 &&
		stricmp("SING",const_cast<char*>(strRepetition.c_str()))!=0 &&
		!(IsDigit(strRepetition) && 1<=atoi(strRepetition.c_str())  && atoi(strRepetition.c_str())<=10000)
		) ||
		(
		stricmp("NONE",const_cast<char*>(strStopCondition.c_str()))!=0 &&
		stricmp("SONerror",const_cast<char*>(strStopCondition.c_str()))!=0 &&
		stricmp("SON",const_cast<char*>(strStopCondition.c_str()))!=0 &&
		stricmp("CLEVel",const_cast<char*>(strStopCondition.c_str()))!=0 &&
		stricmp("CLEV",const_cast<char*>(strStopCondition.c_str()))!=0
		)
		||
		(
		stricmp("NONE",const_cast<char*>(strStepMode.c_str()))!=0  &&
		stricmp("STEP",const_cast<char*>(strStepMode.c_str()))!=0 
		)
		)
	{
		return false;
	}

	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:RXQuality:BER:CONTrol:REPetition "+strRepetition+","+strStopCondition+","+strStepMode;
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}
bool CCMU200::WCDMA_SetBERMearsuredDelayTime(int iChipNumbers)
{
	if (iChipNumbers>1172 || iChipNumbers<876)
		return false;

	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:RXQuality:CONTrol:TIMing "+MakeSTDstring("%d",iChipNumbers);
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetSameBlockSize_DownLink_UpLink(bool bSame)
{
	std::string strCmd="";
	if (bSame)
		strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:RXQuality:BER:CONTrol:TBSize SYMM";
	else
		strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:RXQuality:BER:CONTrol:TBSize ASYM";

	if (!GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetBERNumberOfMearsuredBlock(std::string strMeasuredType,int iNumOfBlocks)
{
	if (stricmp("SING",strMeasuredType.c_str())==0 || stricmp("SINGLSHOT",strMeasuredType.c_str())==0)
	{
		if (iNumOfBlocks>50000 || iNumOfBlocks<1)
			return false;
	}
	else if(stricmp("CONT",strMeasuredType.c_str())==0 || stricmp("CONTINUOUS",strMeasuredType.c_str())==0)
	{
		if (iNumOfBlocks>500 || iNumOfBlocks<1)
			return false;
	}
	else
		return false;

	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:RXQuality:BER:CONTrol:TBLocks "+MakeSTDstring("%d",iNumOfBlocks);
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_FetchBERMearsurement(
	double& dBER,
	double& dBLER,
	double &dDBLER,
	int& iLostTransportBlocks,
	int& iAlignmentDLUL,
	double& dFDR,
	double& dTFCI_ULFaults,
	double&	dPNDiscontinuity,
	int& iTransportBlockDelay)
{
	double dMearsurment[10]={0};
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";ABORt:RXQuality:BER; *OPC?";
	if (! GPIB_OPComplete(strCmd))
		return false;

	strCmd=MakeSTDstring("%d",g_iMode)+";INITiate:RXQuality:BER; *OPC?";
	if (! GPIB_OPComplete(strCmd))
		return false;

	char *token = NULL;
	std::string strMeasured="";
	char seps[] = ",";

	int iRetryTimes=70;
	strCmd= MakeSTDstring("%d",g_iMode)+";FETCh:RXQuality:BER:STATus?";
	while(iRetryTimes-->0)
	{
		if (! GPIB_Querry(strCmd, strMeasured))
			return false;

		if (strstr(strMeasured.c_str(), "RUN") == NULL)
			break;
		else
			Sleep(500);
	} 

	strCmd= MakeSTDstring("%d",g_iMode)+";SAMPle:RXQuality:BER?";
	if (! GPIB_Querry(strCmd, strMeasured))
		return false;

	token = strtok( (char*)strMeasured.c_str(), seps);
	int i=0;
	while(token != NULL){
		if(stricmp("NAN",token)==0)
			dMearsurment[i++]=NONE_VALUE;
		else
			dMearsurment[i++]= (double)atof(token);
		token = strtok( NULL, seps );
	}

	strCmd= MakeSTDstring("%d",g_iMode)+";ABORt:MODulation:OVERview:WCDMa:DPCH; *OPC?";
	if (! GPIB_OPComplete(strCmd))
		return false;

	dBER					=dMearsurment[0];
	dBLER					=dMearsurment[1];
	dDBLER					=dMearsurment[2];
	iLostTransportBlocks	=(int)dMearsurment[3];
	iAlignmentDLUL			=(int)dMearsurment[4];
	dFDR					=dMearsurment[5];
	dTFCI_ULFaults			=dMearsurment[6];
	dPNDiscontinuity		=dMearsurment[7];
	iTransportBlockDelay	=(int)dMearsurment[8];

	return true;
}


bool CCMU200::WCDMA_SetModulationNumberOfBurstsToMearsured(int iNumberOfBursts)
{
	if (iNumberOfBursts<0 || 1000<iNumberOfBursts)
		return false;

	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:MODulation:OVERview:WCDMa:DPCH:CONTrol:STATistics "+MakeSTDstring("%d",iNumberOfBursts);
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_SetModulationMearsuredRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode)
{
	if (
		(
		stricmp("CONT",const_cast<char*>(strRepetition.c_str()))!=0 && 
		stricmp("CONTINUOUS",const_cast<char*>(strRepetition.c_str()))!=0 &&
		stricmp("SINGLSHOT",const_cast<char*>(strRepetition.c_str()))!=0 &&
		stricmp("SING",const_cast<char*>(strRepetition.c_str()))!=0 &&
		!(IsDigit(strRepetition) && 1<=atoi(strRepetition.c_str())  && atoi(strRepetition.c_str())<=10000)
		) ||
		(
		stricmp("NONE",const_cast<char*>(strStopCondition.c_str()))!=0 &&
		stricmp("SONerror",const_cast<char*>(strStopCondition.c_str()))!=0 &&
		stricmp("SON",const_cast<char*>(strStopCondition.c_str()))!=0
		)
		||
		(
		stricmp("NONE",const_cast<char*>(strStepMode.c_str()))!=0  &&
		stricmp("STEP",const_cast<char*>(strStepMode.c_str()))!=0 
		)
		)
	{
		return false;
	}

	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:MODulation:OVERview:WCDMa:DPCH:CONTrol:REPetition "+strRepetition+","+strStopCondition+","+strStepMode;
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}
bool CCMU200::WCDMA_SetModulationMeasuredLength(int iChipNumbers)
{
	if(iChipNumbers!=1280 && iChipNumbers!=2560)
		return false;

	std::string strCmd=MakeSTDstring("%d",g_iMode)+";CONFigure:MODulation:OVERview:WCDMa:DPCH:CONTrol:MLENgth "+MakeSTDstring("%d",iChipNumbers);
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}

bool CCMU200::WCDMA_FetchModulationMearsurement(
										double& dEVM_PeakCurrent,			double& dEVM_PeakAvg,			double& dEVM_PeakMax,
										double& dEVM_RMSCurrent,			double& dEVM_RMSAvg,			double& dEVM_RMSMax,
										double& dMagnErr_PeakCurrent,		double& dMagnErr_PeakAvg,		double& dMagnErr_PeakMax,
										double& dMagnErr_RMSCurrent,		double& dMagnErr_RMSAvg,		double& dMagnErr_RMSMax,
										double& dPhaseErr_PeakCurrent,		double& dPhaseErr_PeakAvg,		double& dPhaseErr_PeakMax,
										double& dPhaseErr_RMSCurrent,		double& dPhaseErr_RMSAvg,		double& dPhaseErr_RMSMax,
										double& dIQ_OriginOffset_Current,	double& dIQ_OriginOffset_Avg,	double& dIQ_OriginOffset_Max,
										double& dIQ_Imbalance_Current,		double& dIQ_Imbalance_Avg,		double& dIQ_Imbalance_Max,
										double& dCarrierFreqErr_Current,	double& dCarrierFreqErr_Avg,	double& dCarrierFreqErr_Max,
										double& dWaveQuality_Current,		double& dWaveQuality_Avg,		double& dWaveQuality_Max,
										double& dPeakCodeDomErr_Current,	double& dPeakCodeDomErr_Avg,	double& dPeakCodeDomErr_Max,
										double& dTransmitTimeErr_Current,	double& dTransmitTimeErr_Avg,	double& dTransmitTimeErr_Max,		
										double& dUEPower_RMSCurrent,		double& dOutOfTolerance,		int&	iSlotNumber
												)
{
	double dMearsurment[40]={0};
	std::string strCmd=MakeSTDstring("%d",g_iMode)+";ABORt:MODulation:OVERview:WCDMa:DPCH; *OPC?";
	if (! GPIB_OPComplete(strCmd))
		return false;

	strCmd=MakeSTDstring("%d",g_iMode)+";INITiate:MODulation:OVERview:WCDMa:DPCH; *OPC?";
	if (! GPIB_OPComplete(strCmd))
		return false;

	char *token = NULL;
	std::string strMeasured="";
	char seps[] = ",";

	int iRetryTimes=70;
	strCmd= MakeSTDstring("%d",g_iMode)+";FETCh:MODulation:OVERview:WCDMa:DPCH:STATus?";
	while(iRetryTimes-->0)
	{
		if (! GPIB_Querry(strCmd, strMeasured))
			return false;

		if (strstr(strMeasured.c_str(), "RUN") == NULL)
			break;
		else
			Sleep(500);
	} 

	strCmd= MakeSTDstring("%d",g_iMode)+";SAMPle:MODulation:OVERview:WCDMa:DPCH?";
	if (! GPIB_Querry(strCmd, strMeasured))
		return false;

	token = strtok( (char*)strMeasured.c_str(), seps);
	int i=0;
	while(token != NULL){
		if(stricmp("NAN",token)==0)
			dMearsurment[i++]=NONE_VALUE;
		else
			dMearsurment[i++]= (double)atof(token);
		token = strtok( NULL, seps );
	}

	strCmd= MakeSTDstring("%d",g_iMode)+";ABORt:MODulation:OVERview:WCDMa:DPCH; *OPC?";
	if (! GPIB_OPComplete(strCmd))
		return false;

	dEVM_PeakCurrent	=dMearsurment[0];
	dEVM_PeakAvg		=dMearsurment[1];
	dEVM_PeakMax		=dMearsurment[2];
	dEVM_RMSCurrent		=dMearsurment[3];
	dEVM_RMSAvg			=dMearsurment[4];
	dEVM_RMSMax			=dMearsurment[5];
	dMagnErr_PeakCurrent=dMearsurment[6];
	dMagnErr_PeakAvg	=dMearsurment[7];
	dMagnErr_PeakMax	=dMearsurment[8];
	dMagnErr_RMSCurrent	=dMearsurment[9];
	dMagnErr_RMSAvg		=dMearsurment[10];
	dMagnErr_RMSMax		=dMearsurment[11];
	dPhaseErr_PeakCurrent=dMearsurment[12];
	dPhaseErr_PeakAvg	=dMearsurment[13];
	dPhaseErr_PeakMax	=dMearsurment[14];
	dPhaseErr_RMSCurrent=dMearsurment[15];
	dPhaseErr_RMSAvg	=dMearsurment[16];
	dPhaseErr_RMSMax	=dMearsurment[17];
	dIQ_OriginOffset_Current=dMearsurment[18];
	dIQ_OriginOffset_Avg=dMearsurment[19];
	dIQ_OriginOffset_Max=dMearsurment[20];
	dIQ_Imbalance_Current=dMearsurment[21];
	dIQ_Imbalance_Avg	=dMearsurment[22];
	dIQ_Imbalance_Max	=dMearsurment[23];
	dCarrierFreqErr_Current=dMearsurment[24];
	dCarrierFreqErr_Avg	=dMearsurment[25];
	dCarrierFreqErr_Max	=dMearsurment[26];
	dWaveQuality_Current=dMearsurment[27];
	dWaveQuality_Avg	=dMearsurment[28];
	dWaveQuality_Max	=dMearsurment[29];
	dPeakCodeDomErr_Current=dMearsurment[30];
	dPeakCodeDomErr_Avg	=dMearsurment[31];
	dPeakCodeDomErr_Max	=dMearsurment[32];
	dTransmitTimeErr_Current=dMearsurment[33];
	dTransmitTimeErr_Avg=dMearsurment[34];
	dTransmitTimeErr_Max=dMearsurment[35];		
	dUEPower_RMSCurrent	=dMearsurment[36];
	dOutOfTolerance		=dMearsurment[37];
	iSlotNumber			=(int)dMearsurment[38];

	return true;
}

bool CCMU200::WCDMA_SetBSOutputPower_NonSignaling(double dOutputPower)
{
	if(dOutputPower<-147 || dOutputPower>-15)
		return false;

	std::string strCmd=MakeSTDstring("%d",g_iMode)+";SOURce:RFGenerator:OPOWer "+MakeSTDstring("%f",dOutputPower);
	if (!GPIB_Write(strCmd))
		return false;

	return true;
}