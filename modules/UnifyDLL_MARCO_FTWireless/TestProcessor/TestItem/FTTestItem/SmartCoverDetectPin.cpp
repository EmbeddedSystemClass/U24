
#include "stdafx.h"
#include "SmartCoverDetectPin.h"

RF_IMPLEMENT_DYNCREATE(CSmartCoverDetectPin)

bool CSmartCoverDetectPin::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find(std::string("SmartCoverDetectPin")) != paramMap.end())
	{
		m_str_DetectPin = paramMap[std::string("SmartCoverDetectPin")];
	}
	else
	{
		TraceLog(MSG_INFO, "Get SmartCoverDetectPin fail.");
		return false;
	}

	return true;
}

bool CSmartCoverDetectPin::PreRun()
{
	m_strResult    = "";
	m_strItemCode  = "";
	m_strErrorCode = FunErr_SmartCover_Detect_Fail;
	m_strBand      = "-";
	m_strChannel   = "-";
	m_strUpper     = "";
	m_strLower     = "";
	m_strMeasured  = "-";
	m_strUnit      = "-";
	m_strMessage   = "";
	return true;
}

bool CSmartCoverDetectPin::Run()
{
	if (m_pIPhone == NULL) return false;

	return SmartCoverDetectPin();
}

bool CSmartCoverDetectPin::PostRun()
{
	return true;
}

bool CSmartCoverDetectPin::SmartCoverDetectPin()
{
	if (m_pIPhone == NULL)
	{
		TraceLog(MSG_INFO, "m_pIPhone is NULL.");
		return false;
	}

	TraceLog(MSG_INFO, "Smart cover check detect pin on.");
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	char sz_inBuffer[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->Sapporo_SCoverDetect(m_nFtdPort, m_nFtdTimeOut, sz_inBuffer, sz_outBuffer))
	{
		m_strResult = "FAIL";
		m_strMessage = "Sapporo_SCoverDetect on fail.";
		TraceLog(MSG_INFO, "Sapporo_SCoverDetect on fail.");
		FactoryLog();
		return false;
	}
	TraceLog(MSG_INFO, "Sapporo_SCoverDetect on success.");

	//Compare pass criteria
	m_strMeasured = sz_outBuffer;
	bool b_res = false;
	if (b_res = strstr(sz_outBuffer, m_str_DetectPin.c_str()) != NULL)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "Smart cover detect pin verifies pass.";
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "Smart cover detect pin verifies fail.";
	}
	TraceLog(MSG_INFO, m_strMessage);
	SetPICSData("SC_DETECT_PIN", m_strMeasured);
	FactoryLog();
	return b_res;
}
