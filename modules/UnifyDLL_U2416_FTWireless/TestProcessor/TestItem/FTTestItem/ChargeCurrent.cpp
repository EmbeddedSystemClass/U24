/*********************************************************************************
* COPYRIGHT Qisda WR24 2012
*
* Description: .cpp file of ChargeCurrent object.
* FileName   : ChargeCurrent.cpp
* Version    : $Revision:   1.3  $
* Author     : CY.Lee
* Time       : 2012/05/11
* History    :
*  NO      Date        Updated by        Content of change
*  1    2012/05/11      CY.Lee                Create.
********************************************************************************/

#include "stdafx.h"
#include "ChargeCurrent.h"


RF_IMPLEMENT_DYNCREATE(ChargeCurrent)


bool ChargeCurrent::PreRun()
{
	m_strResult = "-";
	m_strItemCode = CStr::IntToStr(ChargeCurrent_BaseItemCode);
	m_strErrorCode = FunErr_USB_Charge_Fail;
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = CStr::DblToStr(m_rp_currentSpec.GetMax());
	m_strLower = CStr::DblToStr(m_rp_currentSpec.GetMin());
	m_strMeasured = "";
	m_strUnit = "A";
	m_strMessage = "";
	return true;
}

bool ChargeCurrent::Run()
{
	bool b_res = ChargeCurrentTest();
	SetPICSData(_T("CHARGE_CURRENT"), m_strMeasured);
	FactoryLog();
	return b_res;
}

bool ChargeCurrent::PostRun()
{
	return true;
}

bool ChargeCurrent::InitData(std::map<std::string, std::string>& paramMap)
{
	// WaitTimeForFetch /
	std::string str_fetchTime;
	if (paramMap.find(std::string("WaitTimeForFetch")) != paramMap.end())
	{
		str_fetchTime = paramMap[std::string("WaitTimeForFetch")];
		m_i_WaitTimeForFetch = CStr::StrToInt(str_fetchTime);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter 'WaitTimeForFetch' for 'CLeakageCurrent'");
		return false;
	}

	// Current_Spec /
	std::string str_limit;
	if (paramMap.find(std::string("Current_Spec")) != paramMap.end())
	{
		str_limit = paramMap[std::string("Current_Spec")];
		m_rp_currentSpec.SetRange(str_limit);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter Current_Spec for ChargeCurrent");
		return false;
	}

	/* DelayTime */
	std::string str_delayTime;
	if (paramMap.find(std::string("Delay_Time_ms")) != paramMap.end())
	{
		str_delayTime = paramMap[std::string("Delay_Time_ms")];
		m_i_delayTimeMS = CStr::StrToInt(str_delayTime);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter Delay_Time_ms for CChargeCurrent");
		return false;
	}

	return true;
}

bool ChargeCurrent::ChargeCurrentTest()
{
	if (!m_pIPS)
	{
		m_strResult = "FAIL";
		m_strMessage = "Fail to find powersupply, please check GPIB connection.";
		TraceLog(MSG_INFO, "Fail to find powersupply, please check GPIB connection.");
		return false;
	}

	std::string str_msg;
	// Set Detroit_PWRchargerOn off
	bool b_res = false;
	char sz_outData[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_PWRchargerOn(m_nFtdPort, m_nFtdTimeOut, "0", sz_outData)))
	{
		str_msg = "Failed to Set Detroit_PWRchargerOn off.";
		TraceLog(MSG_INFO, str_msg);
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Successed to Power charging off.");
	}
	Sleep(m_i_WaitTimeForFetch);

	// Get powersupply current for offset
	double d_current_offset = 0;
	if (!m_pIPS->GetCurrent(d_current_offset))
	{
		m_strResult = "FAIL";
		m_strMessage = "Fail to get powersupply current offset.";
		TraceLog(MSG_INFO, "Fail to get powersupply current offset.");
		return false;
	}

	// Set Detroit_PWRchargerOn on
	if (!(b_res = m_pIPhone->FTD_PWRchargerOn(m_nFtdPort, m_nFtdTimeOut, "1", sz_outData)))
	{
		str_msg = "Failed to Set Detroit_PWRchargerOn on.";
		TraceLog(MSG_INFO, str_msg);
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Successed to Power charging on.");
	}
	Sleep(m_i_WaitTimeForFetch);

	int i_retry = 0;
	int i_retryTestDelay = 100;
	int i_testTotalTime = m_i_delayTimeMS;
	double d_current = 0;
	double d_current_on_charging = 0;
	b_res = false;
	do {
		i_retry++;
		Sleep(i_retryTestDelay);
		m_pIPS->GetCurrent(d_current_on_charging);
		d_current = d_current_on_charging - d_current_offset;
		if (m_rp_currentSpec.InRange(d_current))
		{
			b_res = true;
		}
		CString str_trace;
		str_trace.Format(_T("Charge Current (%d): %f"), i_retry, d_current);
		TraceLog(MSG_INFO, str_trace.GetBuffer());
		str_trace.ReleaseBuffer();
	} while (i_retry < (i_testTotalTime / i_retryTestDelay) && !b_res);
	m_strMeasured = CStr::DblToStr(d_current);

	// Set Detroit_PWRchargerOn off
	if (!m_pIPhone->FTD_PWRchargerOn(m_nFtdPort, m_nFtdTimeOut, "0", sz_outData))
	{
		str_msg = "Failed to Set Detroit_PWRchargerOn off.";
		TraceLog(MSG_INFO, str_msg);
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Successed to Power charging off.");
	}

	// Test result /
	if(b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "Charge Current Test Pass!";
		str_msg =
			"Charge Current is in range [" + CStr::DblToStr(m_rp_currentSpec.GetMin()) + ", " +
			CStr::DblToStr(m_rp_currentSpec.GetMax()) + "], measured value=" + CStr::DblToStr(d_current);
		TraceLog(MSG_INFO, str_msg);
		TraceLog(MSG_INFO, "Charge Current Test Pass!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "Charge Current Test Fail!";
		str_msg =
			"Charge Current not in range [" + CStr::DblToStr(m_rp_currentSpec.GetMin()) + ", " +
			CStr::DblToStr(m_rp_currentSpec.GetMax()) + "], measured value=" + CStr::DblToStr(d_current);
		TraceLog(MSG_INFO, str_msg);
		TraceLog(MSG_INFO, "Charge Current Test Fail!");
	}
	return b_res;
}
