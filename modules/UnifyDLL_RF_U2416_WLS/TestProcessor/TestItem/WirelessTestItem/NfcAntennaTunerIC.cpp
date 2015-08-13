#include "stdafx.h"
#include "NfcAntennaTunerIC.h"


// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CNFC_Antenna_Tuner_IC)

CNFC_Antenna_Tuner_IC::CNFC_Antenna_Tuner_IC()
{
}

bool CNFC_Antenna_Tuner_IC::Run()
{
	m_strMeasured = "";

	bool bRes = MainFunction();

	if (bRes)
	{
		FactoryLog(true, "NFCAntennaTunerIC", "Pass", "", "", "", "", m_strMeasured, "", "NFC Antenna Tuner IC PASS");
	}
	else
	{
		FactoryLog(false, "NFCAntennaTunerIC", FunErr_NFC_Antenna_IC_ID_Fail, "", "", "", "", m_strMeasured, "", m_strMsg);
	}

	SetPICSDate(m_strPICSName, m_strMeasured);

	return bRes;
}

bool CNFC_Antenna_Tuner_IC::MainFunction()
{
	if (! m_pIPhone->QMSL_FTM_SET_MODE(PHONE_MODE_WCDMA_IMT))	//PHONE_MODE_WCDMA_IMT = 9,			//!<' WCDMA IMT, Band I
	{
		Sleep(2500);
		if(! m_pIPhone->QMSL_FTM_SET_MODE(PHONE_MODE_WCDMA_IMT))
		{
			m_strMsg = "Fail to set WCDMA_IMT mode";
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}
	}

	unsigned char szNfcTunerIC = 0;
	if (! m_pIPhone->QMSL_FTM_RFFE_READWRITE_CMD (&szNfcTunerIC))
	{
		m_strMsg = "Fail to read NFC Antenna Tuner IC ID";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	m_strMeasured = MakeSTDstring ("%d", szNfcTunerIC);

	bool bPass = true;
	if (strcmp(m_strMeasured.c_str(), m_strAntennaTunerIC.c_str()) != 0)
	{
		m_strMsg = "Fail to compare NFC tuner IC ID";
		TraceLog(MSG_ERROR, m_strMsg);
		bPass = false;
	}

	return bPass;
}


bool CNFC_Antenna_Tuner_IC::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find("PICSName") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PICSName for CNFC_Antenna_Tuner_IC");
		return false;
	}
	m_strPICSName = paramMap["PICSName"];

	if (paramMap.find("Antenna_Tuner_IC") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Antenna_Tuner_IC for CNFC_Antenna_Tuner_IC");
		return false;
	}
	m_strAntennaTunerIC = paramMap["Antenna_Tuner_IC"];

	return true;
}