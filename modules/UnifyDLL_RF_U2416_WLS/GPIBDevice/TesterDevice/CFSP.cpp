#include "StdAfx.h"
#include "CFSP.h"
#include "../../CommonUtility/CommonFunction.h"

// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CFSP)



//GPIB Command 

// ------------------- Initial functions -------------------

bool CFSP::Initial()
{
	// Reset the Machine

	//if (! GPIB_Write(std::string("SYST:PRES"))) //
	if (! GPIB_OPComplete("SYST:PRES;*OPC?"))
		return false;

	return true;
}

// --------------- Set cable loss functions ----------------

bool CFSP::SetInputLoss(double dLoss)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;
	//sprintf_s(szCommand, 255, "DISP:TRAC1:Y:RLEV:OFFS %8.3f", dLoss);
	sprintf_s(szCommand, 255, "DISP:TRAC1:Y:RLEV:OFFS %8.3f;*OPC?", dLoss);
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

// ----------------- RF functions -----------------

bool CFSP::RF_SetAnalyzerFreq(double dFreqMHz)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	//sprintf_s(szCommand, 255, "SENS1:FREQ:CENT %4.4fMHZ", dFreqMHz);
	sprintf_s(szCommand, 255, "SENS1:FREQ:CENT %4.4fMHZ;*OPC?", dFreqMHz);
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}
bool CFSP::RF_NPowerMeasurementMAX(int iStatistics, std::string& strMeasured)
{
	return true;
}

bool CFSP::RF_NPowerMeasurement(int iStatistics, std::string& strMeasured)
{
	bool bFetch = false;
    int iRetry = 50;

    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand;
/*
    if (! GPIB_Write(std::string("CALC:MARK1:MAX")))
		return false;

	if (! GPIB_Write(std::string("INIT;*WAI")))
		return false;

    sprintf_s(szCommand, 255, "CALC:MARK1:Y?");
    strCommand = szCommand;
    if (! GPIB_Querry(strCommand, strMeasured))
        return false;
*/

 //   sprintf_s(szCommand, 255, "CALC:MARK:FUNC:POW:RES? CPOW");

	sprintf_s(szCommand, 255, "CALC:MARK:FUNC:POW:RES? CPOW;");
    strCommand = szCommand;
    if (! GPIB_Querry(strCommand, strMeasured))
        return false;



	//if (! GPIB_Write(std::string("CALC:MARK:FUNC:POW OFF")))

	if (! GPIB_OPComplete("CALC:MARK:FUNC:POW OFF;*OPC?"))
		return false;

    return true;
}


bool CFSP::RF_NPowerMeasurementNoOFF(int iStatistics, std::string& strMeasured)
{
	bool bFetch = false;
    int iRetry = 50;

    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand;
/*
    if (! GPIB_Write(std::string("CALC:MARK1:MAX")))
		return false;

	if (! GPIB_Write(std::string("INIT;*WAI")))
		return false;

    sprintf_s(szCommand, 255, "CALC:MARK1:Y?");
    strCommand = szCommand;
    if (! GPIB_Querry(strCommand, strMeasured))
        return false;
*/

 //   sprintf_s(szCommand, 255, "CALC:MARK:FUNC:POW:RES? CPOW");

	sprintf_s(szCommand, 255, "CALC:MARK:FUNC:POW:RES? CPOW;");
    strCommand = szCommand;
    if (! GPIB_Querry(strCommand, strMeasured))
        return false;



	//if (! GPIB_Write(std::string("CALC:MARK:FUNC:POW OFF")))
	// FSP Hang 
	//if (! GPIB_OPComplete("CALC:MARK:FUNC:POW OFF;*OPC?"))
	//	return false;

    return true;
}
//

bool CFSP::SetResolutionBandwidth(double dRBWMHz)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	//sprintf_s(szCommand, 255, "SENS1:BAND:RES %4.4fMHZ", dRBWMHz);
	sprintf_s(szCommand, 255, "SENS1:BAND:RES %4.4fMHZ;*OPC?", dRBWMHz);
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CFSP::SetVideoBandwidth(double dVBWMHz)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	//sprintf_s(szCommand, 255, "SENS1:BAND:VID %4.4fMHZ", dVBWMHz);
	sprintf_s(szCommand, 255, "SENS1:BAND:VID %4.4fMHZ;*OPC?", dVBWMHz);
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CFSP::SetFrequencySpan(double dSpanMHz)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	//sprintf_s(szCommand, 255, "SENS1:FREQ:SPAN %4.4fMHZ", dSpanMHz);
	sprintf_s(szCommand, 255, "SENS1:FREQ:SPAN %4.4fMHZ;*OPC?", dSpanMHz);
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CFSP::SwitchTraceMode(std::string strTraceMode)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	//sprintf_s(szCommand, 255, "DISP:WIND:TRAC:MODE %s", strTraceMode.c_str());
	sprintf_s(szCommand, 255, "DISP:WIND:TRAC:MODE %s;*OPC?", strTraceMode.c_str());
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CFSP::SetDetector(std::string strDetector)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	//sprintf_s(szCommand, 255, "DET %s", strDetector.c_str());
	sprintf_s(szCommand, 255, "DET %s;*OPC?", strDetector.c_str());
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;

}

bool CFSP::SwitcheMeasurementWindow(std::string strDiagramFull)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	//sprintf_s(szCommand, 255, "DISP:WIND1:SIZE %s", strDiagramFull.c_str());
	sprintf_s(szCommand, 255, "DISP:WIND1:SIZE %s;*OPC?", strDiagramFull.c_str());
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CFSP::AdjustReferenceLevel()
{
	//if (! GPIB_Write(std::string("SENS:POW:ACH:PRES:RLEV")))
	if (! GPIB_OPComplete("SENS:POW:ACH:PRES:RLEV;*OPC?"))
		return false;

	return true;
}

bool CFSP::SetAttenuation(bool bEnable)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	if (bEnable)
		//sprintf_s(szCommand, 255, "INP:ATT:AUTO %s", "ON");
		sprintf_s(szCommand, 255, "INP:ATT:AUTO %s;*OPC?", "ON");
	else
		//sprintf_s(szCommand, 255, "INP:ATT:AUTO %s", "OFF");
		sprintf_s(szCommand, 255, "INP:ATT:AUTO %s;*OPC?", "OFF");
	strCommand = szCommand;

	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CFSP::SetSweepTime(int iSweepTime)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;
	
	//sprintf_s(szCommand, 255, "SWE:TIME %dms", iSweepTime);
	sprintf_s(szCommand, 255, "SWE:TIME %dms;*OPC?", iSweepTime);
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CFSP::SetLimitLines(std::string strLineName, std::string strDomain, int iTrace,
						 std::string strXscalingMode, std::string strUnit, 
						 std::string strYscalingMode, std::string strSpacing,
						 int iMargin, int iThreshold, std::string strContData,
						 std::string strUppData)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	//sprintf_s(szCommand, 255, "CALC:LIM1:NAME %s", strLineName.c_str());
	sprintf_s(szCommand, 255, "CALC:LIM1:NAME %s;*OPC?", strLineName.c_str());
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	Sleep(1000); // Neil: For FSEA_20 delay

	memset(szCommand, 0, 256);
	//sprintf_s(szCommand, 255, "CALC:LIM1:CONT:DOM %s", strDomain.c_str());
	sprintf_s(szCommand, 255, "CALC:LIM1:CONT:DOM %s;*OPC?", strDomain.c_str());
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	memset(szCommand, 0, 256);
	//sprintf_s(szCommand, 255, "CALC1:LIM1:TRAC %d", iTrace);
	sprintf_s(szCommand, 255, "CALC1:LIM1:TRAC %d;*OPC?", iTrace);
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	memset(szCommand, 0, 256);
	//sprintf_s(szCommand, 255, "CALC:LIM1:CONT:MODE %s", strXscalingMode.c_str());
	sprintf_s(szCommand, 255, "CALC:LIM1:CONT:MODE %s;*OPC?", strXscalingMode.c_str());
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	memset(szCommand, 0, 256);
	//sprintf_s(szCommand, 255, "CALC:LIM1:UNIT %s", strUnit.c_str());
	sprintf_s(szCommand, 255, "CALC:LIM1:UNIT %s;*OPC?", strUnit.c_str());
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	memset(szCommand, 0, 256);
	//sprintf_s(szCommand, 255, "CALC:LIM1:UPP:MODE %s", strYscalingMode.c_str());
	sprintf_s(szCommand, 255, "CALC:LIM1:UPP:MODE %s;*OPC?", strYscalingMode.c_str());
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	memset(szCommand, 0, 256);
	//sprintf_s(szCommand, 255, "CALC:LIM1:UPP:SPAC %s", strSpacing.c_str());
	sprintf_s(szCommand, 255, "CALC:LIM1:UPP:SPAC %s;*OPC?", strSpacing.c_str());
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	memset(szCommand, 0, 256);
	//sprintf_s(szCommand, 255, "CALC:LIM1:UPP:STAT %s", "ON");
	sprintf_s(szCommand, 255, "CALC:LIM1:UPP:STAT %s;*OPC?", "ON");
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	memset(szCommand, 0, 256);
	//sprintf_s(szCommand, 255, "CALC:LIM1:UPP:MARG %ddB", iMargin);
	sprintf_s(szCommand, 255, "CALC:LIM1:UPP:MARG %ddB;*OPC?", iMargin);
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	memset(szCommand, 0, 256);
	//sprintf_s(szCommand, 255, "CALC:LIM1:UPP:THR %dDBM", iThreshold);
	sprintf_s(szCommand, 255, "CALC:LIM1:UPP:THR %dDBM;*OPC?", iThreshold);
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	memset(szCommand, 0, 256);
	//sprintf_s(szCommand, 255, "CALC:LIM1:CONT:DATA %s", strContData.c_str());
	sprintf_s(szCommand, 255, "CALC:LIM1:CONT:DATA %s;*OPC?", strContData.c_str());
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	memset(szCommand, 0, 256);
	//sprintf_s(szCommand, 255, "CALC:LIM1:UPP:DATA %s", strUppData.c_str());
	sprintf_s(szCommand, 255, "CALC:LIM1:UPP:DATA %s;*OPC?", strUppData.c_str());
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;
/*
	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "CALC:LIM1:STAT %s", "ON");
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
*/
	return true;
}

bool CFSP::ResetFSP()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;


	sprintf_s(szCommand, 255, "*RST");
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	
	Sleep(1000);
	return true;
}

bool CFSP::SpectrumMaskMeasurement(int iStatistics, std::string& strMeasured)
{
	bool bFetch = false;
    int iRetry = 50;
	double dValue;

    char szCommand[256], szValue[32];
    memset(szCommand, 0, 256);
    std::string strCommand;

	sprintf_s(szCommand, 255, "CALC:MARK1:Y?");
    strCommand = szCommand;
    if (! GPIB_Querry(strCommand, strMeasured))
        return false;

	dValue = atof(strMeasured.c_str());
	memset(szValue, 0, 32);
	sprintf_s(szValue, 32, "%.3f", dValue);
	dValue = (atof(szValue) + 0.1);

	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "CALC:LIM1:STAT %s;*OPC?", "ON");
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

    sprintf_s(szCommand, 255, "CALC:LIM1:FAIL?");
    strCommand = szCommand;
    if (! GPIB_Querry(strCommand, strMeasured))
        return false;

    return true;
}

bool CFSP::FrequencyErrorMeasurement(int iStatistics, std::string& strMeasured)
{
 //	bool bFetch = false;
  //  int iRetry = 50;

    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand;
/*
	if (! GPIB_Write(std::string("CALC:MARK1:MAX")))
		return false;

	if (! GPIB_Write(std::string("INIT;*WAI")))
		return false;
*/
    //if (! GPIB_Write(std::string("CALC:MARK1:COUN ON")))
	if (! GPIB_OPComplete(std::string("CALC:MARK1:COUN ON;*OPC?")))
		return false;

	Sleep(1000);

	sprintf_s(szCommand, 255, "CALC:MARK1:COUN:FREQ?");

    strCommand = szCommand;
    if (! GPIB_Querry(strCommand, strMeasured))
        return false;

	//if (! GPIB_Write(std::string("CALC:MARK1:COUN OFF")))
	if (! GPIB_OPComplete(std::string("CALC:MARK1:COUN OFF;*OPC?")))
		return false;

    return true;
}

bool CFSP::SetAttRefLevel(int iAttenuationManual, int iRefLevel)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	memset(szCommand, 0, 256);
	//sprintf_s(szCommand, 255, "INP1:ATT %d", iAttenuationManual);
	sprintf_s(szCommand, 255, "INP1:ATT %d;*OPC?", iAttenuationManual);
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	//Sleep(2000);

	memset(szCommand, 0, 256);
	//sprintf_s(szCommand, 255, "DISP:TRAC1:Y:RLEV %d", iRefLevel);
	sprintf_s(szCommand, 255, "DISP:TRAC1:Y:RLEV %d;*OPC?", iRefLevel);
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CFSP::SetChannelBandwidth(double dChannelBWMHz)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	//sprintf_s(szCommand, 255, "SENS:POW:ACH:BWID:CHAN %4.4fMHZ", dChannelBWMHz);
	sprintf_s(szCommand, 255, "SENS:POW:ACH:BWID:CHAN %4.4fMHZ;*OPC?", dChannelBWMHz);
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CFSP::SetFrequencyError(double dPeakExcursion, int iTrace, double iFreqCounterRes)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	//sprintf_s(szCommand, 255, "CALC:MARK1:PEXC %4.4fdB", dPeakExcursion);
	sprintf_s(szCommand, 255, "CALC:MARK1:PEXC %4.4fdB;*OPC?", dPeakExcursion);
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	memset(szCommand, 0, 256);
	//sprintf_s(szCommand, 255, "CALC:MARK1:STAT %s", "ON");
	sprintf_s(szCommand, 255, "CALC:MARK1:STAT %s;*OPC?", "ON");
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	memset(szCommand, 0, 256);
	//sprintf_s(szCommand, 255, "CALC:MARK1:TRAC %d", iTrace);
	sprintf_s(szCommand, 255, "CALC:MARK1:TRAC %d;*OPC?", iTrace);
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	memset(szCommand, 0, 256);
	//sprintf_s(szCommand, 255, "CALC:MARK1:MAX");
	sprintf_s(szCommand, 255, "CALC:MARK1:MAX;*OPC?");
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	memset(szCommand, 0, 256);
	//sprintf_s(szCommand, 255, "CALC:MARK1:COUN %s", "ON");
	sprintf_s(szCommand, 255, "CALC:MARK1:COUN %s;*OPC?", "ON");
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	memset(szCommand, 0, 256);
	//sprintf_s(szCommand, 255, "CALC:MARK:COUN:RES %4.4fHz", iFreqCounterRes);
	sprintf_s(szCommand, 255, "CALC:MARK:COUN:RES %4.4fHz;*OPC?", iFreqCounterRes);
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CFSP::SwitchChannelPower(std::string strChannelPower)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	//sprintf_s(szCommand, 255, "CALC:MARK:FUNC:POW:SEL %s", strChannelPower.c_str());
	sprintf_s(szCommand, 255, "CALC:MARK:FUNC:POW:SEL %s;*OPC?", strChannelPower.c_str());
	strCommand = szCommand;
	//if (! GPIB_Write(strCommand))
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CFSP::SetMaxLevel()
{
	Sleep(500);
	if (! GPIB_OPComplete("CALC:MARK1:MAX;*OPC?"))
		return false;

    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand, strMeasured;

	sprintf_s(szCommand, 255, "CALC:MARK1:Y?");
    strCommand = szCommand;
    if (! GPIB_Querry(strCommand, strMeasured))
        return false;

	dRefLevel = atof(strMeasured.c_str()) + 20;

	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "DISP:TRAC1:Y:RLEV %4.4f;*OPC?", dRefLevel);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	SwitchTraceMode("WRIT");
	Sleep(500);
	SwitchTraceMode("MAXH");

	return true;
}

bool CFSP::SetMaxLevelForFreErr()
{
	//if (! GPIB_Write(std::string("CALC:MARK1:MAX")))
	if (! GPIB_OPComplete("CALC:MARK1:MAX;*OPC?"))
		return false;
	
	//if (! GPIB_Write(std::string("CALC:MARK:FUNC:REF")))
	if (! GPIB_OPComplete("CALC:MARK:FUNC:REF;*OPC?"))
		return false;

	return true;
}

bool CFSP::SetBlankOFF()
{
	if (! GPIB_OPComplete(std::string("DISP:WIND:TRAC OFF;*OPC?")))
		return false;	
	return true;
}

bool CFSP::MaxYOffset()
{
	if (! GPIB_OPComplete("CALC:MARK1:MAX;*OPC?"))
		return false;

    std::string strMeasured, strCommand;

    if (! GPIB_Querry("CALC:MARK1:Y?", strMeasured))
        return false;

	double dMax = atof(strMeasured.c_str());
	double diff = dRefLevel - dMax;

	char szCommand[256] = {0};
	sprintf(szCommand, "CALC:LIM3:LOW:OFFS %4.4f;*OPC?", -diff+0.5);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}