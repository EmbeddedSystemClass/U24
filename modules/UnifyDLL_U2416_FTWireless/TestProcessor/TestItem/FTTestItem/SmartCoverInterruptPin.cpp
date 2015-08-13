
#include "stdafx.h"
#include "SmartCoverInterruptPin.h"

RF_IMPLEMENT_DYNCREATE(CSmartCoverInterruptPin)

bool CSmartCoverInterruptPin::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find(std::string("SmartCoverInterruptPin")) != paramMap.end())
	{
		m_str_InterruptPin = paramMap[std::string("SmartCoverInterruptPin")];
	}
	else
	{
		TraceLog(MSG_INFO, "Get SmartCoverInterruptPin fail.");
		return false;
	}
	return true;
}

bool CSmartCoverInterruptPin::PreRun()
{
	m_strResult    = "";
	m_strItemCode  = "";
	m_strErrorCode = FunErr_SmartCover_Interrupt_Fail;
	m_strBand      = "-";
	m_strChannel   = "-";
	m_strUpper     = "";
	m_strLower     = "";
	m_strMeasured  = "-";
	m_strUnit      = "-";
	m_strMessage   = "";
	return true;
}

bool CSmartCoverInterruptPin::Run()
{
	if (m_pIPhone == NULL) return false;

	return SmartCoverCheckInterruptPin();
}

bool CSmartCoverInterruptPin::PostRun()
{
	return true;
}

bool CSmartCoverInterruptPin::SmartCoverCheckInterruptPin()
{
	if (m_pIPhone == NULL)
	{
		TraceLog(MSG_INFO, "m_pIPhone is NULL.");
		return false;
	}

	TraceLog(MSG_INFO, "Smart cover check interrupt pin on.");
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	char sz_inBuffer[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->Sapporo_SCoverInterrupt(m_nFtdPort, m_nFtdTimeOut, sz_inBuffer, sz_outBuffer))
	{
		m_strResult = "FAIL";
		m_strMessage = "Sapporo_SCoverInterrupt on fail.";
		TraceLog(MSG_INFO, "Sapporo_SCoverInterrupt on fail.");
		FactoryLog();
		return false;
	}
	TraceLog(MSG_INFO, "Sapporo_SCoverInterrupt on success.");

	//Compare pass criteria
	m_strMeasured = sz_outBuffer;
	bool b_res = false;
	if (b_res = strstr(sz_outBuffer, m_str_InterruptPin.c_str()) != NULL)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "Smart cover interrupt pin verifies pass.";
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "Smart cover interrupt pin verifies fail.";
	}
	TraceLog(MSG_INFO, m_strMessage);
	SetPICSData("SC_INTERRUPT_PIN", m_strMeasured);
	FactoryLog();
	return b_res;
}
