#include "StdAfx.h"
#include "CFSP.h"

GP_IMPLEMENT_DYNCREATE(CFSP)

// ------------------- Initial functions -------------------

bool CFSP::Initial()
{

	bool ret = false;
	for (int i=0; i <2; i++)
	{
		if (! GPIB_Write(std::string("SYST:PRES")) || !GPIB_Write(std::string("SYSTem:DISPlay:UPDate ON")))
		{
			ret = false;
			Sleep(5000);
			continue;
			//return false;
		}
		else
		{
			ret = true;
			break;
		}
	}
	if(ret)
		return true;
	else
		return false;
	
}

// --------------- Set cable loss functions ----------------

bool CFSP::SetGPRFFixedMeasureLoss(double dLoss)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "DISP:TRAC1:Y:RLEV:OFFS %8.3f", dLoss);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	return true;
}

// ----------------- RF functions -----------------

bool CFSP::RF_SetAnalyzerFreq(double dFreqMHz)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "SENS1:FREQ:CENT %4.4fMHZ", dFreqMHz);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	return true;
}

bool CFSP::RF_NPowerMeasurement(int iStatistics, std::string& strMeasured)
{
	bool bFetch = false;
    int iRetry = 50;

    char szCommand[256];
    memset(szCommand, 0, 256);
    std::string strCommand;
	if (!iStatistics)
	{
		if (! GPIB_Write(std::string("CALC:MARK1:MAX")))
			return false;

		if (! GPIB_Write(std::string("INIT;*WAI")))
			return false;

		sprintf_s(szCommand, 255, "CALC:MARK1:Y?");
		strCommand = szCommand;
		if (! GPIB_Query(strCommand, strMeasured))
			return false;
	}
	else
	{
		sprintf_s(szCommand, 255, "CALC:MARK:FUNC:POW:RES? CPOW");
		strCommand = szCommand;
		if (! GPIB_Query(strCommand, strMeasured))
			return false;
/*
		if (! GPIB_Write(std::string("CALC:MARK:FUNC:POW OFF")))
			return false;
*/
	}

    return true;
}

//

bool CFSP::SetResolutionBandwidth(double dRBWMHz)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "SENS1:BAND:RES %4.4fMHZ", dRBWMHz);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	return true;
}

bool CFSP::SetVideoBandwidth(double dVBWMHz)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "SENS1:BAND:VID %4.4fMHZ", dVBWMHz);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	return true;
}

bool CFSP::SetFrequencySpan(double dSpanMHz)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "SENS1:FREQ:SPAN %4.4fMHZ", dSpanMHz);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	return true;
}

bool CFSP::SetSweepCount(int iSwepCount)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;
	sprintf_s(szCommand, 255, "SWE:COUN %d", iSwepCount);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;	

	Sleep(iSwepCount*1000/3);
/*
	std::string strMeasured  = szCommand ;
	sprintf_s(szCommand, 255, "SWE:COUN:CURR?");
	strCommand = szCommand;

	bool bcmp =true;
	while(bcmp && iSwepCount!=0)
	{
		if (! GPIB_Query(strCommand, strMeasured))
			return false;
		bcmp = atoi(strMeasured.c_str()) != iSwepCount;
		if(!bcmp)
			break;

		Sleep(2000);
	}
*/
	return true;
}

bool CFSP::SwitchTraceMode(std::string strTraceMode)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;
/*
	if(strTraceMode == "VIEW")
	{
		sprintf_s(szCommand, 255, "SWE:COUN %d", 10);
		strCommand = szCommand;
		if (! GPIB_Write(strCommand))
			return false;	

		std::string strMeasured  = szCommand ;
		sprintf_s(szCommand, 255, "SWE:COUN:CURR?");
		strCommand = szCommand;

		while(strMeasured != "10")
		{
			if (! GPIB_Query(strCommand, strMeasured))
				return false;
		}
	}
*/
	sprintf_s(szCommand, 255, "DISP:WIND:TRAC:MODE %s", strTraceMode.c_str());
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	return true;
}

bool CFSP::SetDetector(std::string strDetector)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "DET %s", strDetector.c_str());
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	return true;

}

bool CFSP::SwitcheMeasurementWindow(std::string strDiagramFull)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "DISP:WIND1:SIZE %s", strDiagramFull.c_str());
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	return true;
}

bool CFSP::AdjustReferenceLevel()
{
	if (! GPIB_Write(std::string("SENS:POW:ACH:PRES:RLEV")))
		return false;

	return true;
}

bool CFSP::SetAttenuation(bool bEnable)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	if (bEnable)
		sprintf_s(szCommand, 255, "INP:ATT:AUTO %s", "ON");
	else
		sprintf_s(szCommand, 255, "INP:ATT:AUTO %s", "OFF");
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	return true;
}

bool CFSP::SetSweepTime(int iSweepTime)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;
	
	sprintf_s(szCommand, 255, "SWE:TIME %dms", iSweepTime);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
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

	sprintf_s(szCommand, 255, "CALC:LIM1:NAME %s", strLineName.c_str());
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	Sleep(1000); // Neil: For FSEA_20 delay

	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "CALC:LIM1:CONT:DOM %s", strDomain.c_str());
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "CALC1:LIM1:TRAC %d", iTrace);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "CALC:LIM1:CONT:MODE %s", strXscalingMode.c_str());
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "CALC:LIM1:UNIT %s", strUnit.c_str());
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "CALC:LIM1:UPP:MODE %s", strYscalingMode.c_str());
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "CALC:LIM1:UPP:SPAC %s", strSpacing.c_str());
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "CALC:LIM1:UPP:STAT %s", "ON");
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "CALC:LIM1:UPP:MARG %ddB", iMargin);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "CALC:LIM1:UPP:THR %dDBM", iThreshold);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "CALC:LIM1:CONT:DATA %s", strContData.c_str());
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "CALC:LIM1:UPP:DATA %s", strUppData.c_str());
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
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
    if (! GPIB_Query(strCommand, strMeasured))
        return false;

	dValue = atof(strMeasured.c_str());
	memset(szValue, 0, 32);
	sprintf_s(szValue, 32, "%.3f", dValue);
	dValue = (atof(szValue) + 0.1);

	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "DISP:TRAC1:Y:RLEV %.1f", dValue);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	//Sleep(2000);
/*
	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "CALC:LIM1:UPP:STAT %s", "ON");
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
*/		
	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "CALC:LIM1:STAT %s", "ON");
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

    sprintf_s(szCommand, 255, "CALC:LIM1:FAIL?");
    strCommand = szCommand;
    if (! GPIB_Query(strCommand, strMeasured))
        return false;

    return true;
}

bool CFSP::FrequencyErrorMeasurement(int iStatistics, std::string& strMeasured)
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
*/
    sprintf_s(szCommand, 255, "CALC:MARK1:COUN:FREQ?");
    strCommand = szCommand;
    if (! GPIB_Query(strCommand, strMeasured))
        return false;

	if (! GPIB_Write(std::string("CALC:MARK1:COUN OFF")))
		return false;

    return true;
}

bool CFSP::SetAttRefLevel(int iAttenuationManual, int iRefLevel)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "INP1:ATT %d", iAttenuationManual);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	//Sleep(2000);

	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "DISP:TRAC1:Y:RLEV %d", iRefLevel);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	return true;
}

bool CFSP::SetChannelBandwidth(double dChannelBWMHz)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "SENS:POW:ACH:BWID:CHAN %4.4fMHZ", dChannelBWMHz);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	return true;
}

bool CFSP::SetFrequencyError(double dPeakExcursion, int iTrace, double iFreqCounterRes)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "CALC:MARK1:PEXC %4.4fdB", dPeakExcursion);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "CALC:MARK1:STAT %s", "ON");
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "CALC:MARK1:TRAC %d", iTrace);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "CALC:MARK1:MAX");
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "CALC:MARK1:COUN %s", "ON");
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	memset(szCommand, 0, 256);
	sprintf_s(szCommand, 255, "CALC:MARK:COUN:RES %4.4fHz", iFreqCounterRes);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	return true;
}

bool CFSP::SwitchChannelPower(std::string strChannelPower)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "CALC:MARK:FUNC:POW:SEL %s", strChannelPower.c_str());
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	return true;
}

bool CFSP::SetMaxLevel()
{
	if (! GPIB_Write(std::string("CALC:MARK1:MAX")))
		return false;
	
	if (! GPIB_Write(std::string("CALC:MARK:FUNC:REF")))
		return false;

	return true;
}

bool CFSP::SetFreq_QryAtmpt(double d_Freq, std::string& strMeasured)
{
	if (! GPIB_Write(std::string("CALC:MARK1 ON;")))
		return false;

	char szCommand[256] = {0};
	sprintf_s(szCommand, 256, "%s %f GHz", "CALC:MARK1:X", d_Freq);

	if (! GPIB_Write(szCommand))
		return false;

	if (! GPIB_Query("CALC:MARK1:Y?", strMeasured))
		 return false;

	return true;
}