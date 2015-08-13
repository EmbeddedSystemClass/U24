#include "stdafx.h"
#include "AntennaTunerCal.h"


// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CAntenna_Tuner_Cal)

CAntenna_Tuner_Cal::CAntenna_Tuner_Cal()
{
}

bool CAntenna_Tuner_Cal::Run()
{
	m_strMeasured = "";

	bool bRes = MainFunction();

	if (bRes)
	{
		FactoryLog(true, "AntennaTunerICCalibration", "Pass", "", "", "", "", m_strMeasured, "", "Antenna Tuner IC Calibration PASS");
	}
	else
	{
		FactoryLog(false, "AntennaTunerICCalibration", FunErr_Antenna_Tuner_Self_Calibration, "", "", "", "", "", "", m_strMsg);
	}

	SetPICSData(m_strPICSName, (bRes)? "PASS": "FAIL");

	return bRes;
}

bool CAntenna_Tuner_Cal::MainFunction()
{
	if (!m_pIPhone->QMSL_FTM_SET_CALIBRATION_STATE(CAL_STATE_ACTIVE))
	{
		m_strMsg = "Fail to set calibration state";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pIPhone->QMSL_FTM_DO_TUNER_SELF_CAL())
	{
		m_strMsg = "Fail to do Antenna Tuner IC Calibration";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	return true;
}


bool CAntenna_Tuner_Cal::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find("PICSName") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PICSName for CAntenna_Tuner_Cal");
		return false;
	}
	m_strPICSName = paramMap["PICSName"];

	return true;
}