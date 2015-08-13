
#include "stdafx.h"
#include "CardTest.h"

RF_IMPLEMENT_DYNCREATE(CardTest)

bool CardTest::InitData(std::map<std::string, std::string>& paramMap)
{
	//-- SD Detect --//
	std::string str_SD_Detect;
	if (paramMap.find("SD_Detect") != paramMap.end())
	{
		str_SD_Detect = paramMap[std::string("SD_Detect")];
		m_i_SD_Detect = CStr::StrToInt(str_SD_Detect);
	}
	else
	{
		TraceLog(MSG_INFO, "SD detect fail.");
		return false;
	}
	return true;
}

bool CardTest::PreRun()
{
	m_strResult    = "";
	m_strItemCode  = "";
	m_strErrorCode = "";
	m_strBand      = "-";
	m_strChannel   = "-";
	m_strUpper     = "";
	m_strLower     = "";
	m_strMeasured  = "-";
	m_strUnit      = "-";
	m_strMessage   = "";
	return true;
}

bool CardTest::Run()
{
	return CardTestFunc();
}

bool CardTest::PostRun()
{
	return true;
}

bool CardTest::CardTestFunc()
{
	if (m_pIPhone == NULL)
	{
		TraceLog(MSG_INFO, "m_pIPhone is NULL.");
		return false;
	}

	//-- SD Card --//
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_SDdetect(m_nFtdPort, m_nFtdTimeOut, sz_outBuffer))
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Detect_SD_Fail;
		m_strMessage = "SD card detect command Fail.";
		TraceLog(MSG_INFO, m_strMessage);
		FactoryLog();
		return false;
	}
	TraceLog(MSG_INFO, "SD card detect success.");
	m_strMeasured = sz_outBuffer;

	//-- Check the SD card status --//
	int i_SDD = CStr::StrToInt(m_strMeasured);
	if (m_i_SD_Detect != i_SDD)
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Detect_SD_Fail;
		m_strMessage = "SD card compare spec Fail.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("SD_DETECT", m_strMeasured);
		FactoryLog();
		return false;
	}
	m_strResult = "PASS";
	m_strMessage = "SD card in the spec.";
	TraceLog(MSG_INFO, m_strMessage);
	SetPICSData("SD_DETECT", m_strMeasured);
	FactoryLog();

	m_strMeasured = "-";
	//-- SD card detect pin--//
	if (!m_pIPhone->FTD_SDdetectPin(m_nFtdPort, m_nFtdTimeOut, sz_outBuffer))
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_SD_Mount_Fail;
		m_strMessage = "SD card detect pin Fail.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("SD_DETECT_PIN", m_strResult);
		FactoryLog();
		return false;
	}
	m_strResult = "PASS";
	m_strMessage = "SD card detect pin Pass";
	TraceLog(MSG_INFO, m_strMessage);
	SetPICSData("SD_DETECT_PIN", m_strResult);
	FactoryLog();

	//-- SIM card test --//
	//if (!m_pIPhone->FTD_SIMtest(m_nFtdPort, m_nFtdTimeOut, sz_outBuffer))
	//{
	//	m_strResult = "FAIL";
	//	m_strErrorCode = FunErr_Detect_SIM_Card_Fail;
	//	m_strMessage = "SIM card test Fail.";
	//	TraceLog(MSG_INFO, m_strMessage);
	//	SetPICSData("SIM_TEST", m_strResult);
	//	FactoryLog();
	//	return false;
	//}
	//m_strResult = "PASS";
	//m_strMessage = "SIM card test Pass.";
	//TraceLog(MSG_INFO, m_strMessage);
	//SetPICSData("SIM_TEST", m_strResult);
	//FactoryLog();

	return true;
}
