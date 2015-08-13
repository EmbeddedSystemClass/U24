/*********************************************************************************
* COPYRIGHT Qisda WR24 201
*
* Description: .cpp file of SmartCoverChargeCurrent object.
* FileName   : SmartCoverChargeCurrent.cpp
* Version    : $Revision:   1.0  $
* Author     : Evonne.Hsieh
* Time       : 2013/07/30
* History    :
*  NO      Date        Updated by        Content of change
*  1    2013/07/30    Evonne.Hsieh           Create.
********************************************************************************/

#include "stdafx.h"
#include "SmartCoverChargeCurrent.h"


RF_IMPLEMENT_DYNCREATE(CSmartCoverChargeCurrent)


bool CSmartCoverChargeCurrent::PreRun()
{
	m_strResult = "-";
	m_strItemCode = CStr::IntToStr(ChargeCurrent_BaseItemCode);
	m_strErrorCode = FunErr_SmartCover_Switch5V_Fail;
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = CStr::DblToStr(m_rp_currentSpec.GetMax());
	m_strLower = CStr::DblToStr(m_rp_currentSpec.GetMin());
	m_strMeasured = "";
	m_strUnit = "A";
	m_strMessage = "";
	return true;
}

bool CSmartCoverChargeCurrent::Run()
{
	bool b_res = SmartCoverChargeCurrentTest();
	// Set power supply 3 off
	if (!m_pIPS3->SetPSOnOff(false))
	{
		m_strErrorCode = CommErr_PowerSupply_Control_Fail;
		m_strMessage = "Fail to set power supply 3 off";
		TraceLog(MSG_ERROR, m_strMessage);
		return false;
	}
	return b_res;
}

bool CSmartCoverChargeCurrent::PostRun()
{
	return true;
}

bool CSmartCoverChargeCurrent::InitData(std::map<std::string, std::string>& paramMap)
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
		TraceLog(MSG_INFO, "Fail to find parameter WaitTimeForFetch for CSmartCoverChargeCurrent");
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
		TraceLog(MSG_INFO, "Fail to find parameter Current_Spec for CSmartCoverChargeCurrent");
		return false;
	}

	/* DelayTime */
	std::string str_TotalTime;
	if (paramMap.find(std::string("Total_Time")) != paramMap.end())
	{
		str_TotalTime = paramMap[std::string("Total_Time")];
		m_i_TotalTime = CStr::StrToInt(str_TotalTime);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter Total_Time for CChargeCurrent");
		return false;
	}

	return true;
}

bool CSmartCoverChargeCurrent::SmartCoverChargeCurrentTest()
{
	if (!m_pIPS3)
	{
		m_strResult = "FAIL";
		m_strMessage = "Fail to find powersupply, please check GPIB connection.";
		TraceLog(MSG_INFO, "Fail to find powersupply, please check GPIB connection.");
		return false;
	}

	/* Set power supply 3 on */
	if (!m_pIPS3->SetPSOnOff(true))
	{
		m_strErrorCode = CommErr_PowerSupply_Control_Fail;
		m_strMessage = "Fail to set power supply 3 on";
		TraceLog(MSG_ERROR, m_strMessage);
		return false;
	}

	char sz_outData[FTD_BUF_SIZE] = {0};
	if (!(m_pIPhone->FTD_PWRchargerOn(m_nFtdPort, m_nFtdTimeOut, "1", sz_outData)))
	{
		TraceLog(MSG_INFO, "Fail to set Detroit_PWRchargerOn on.");
		return false;
	}
	Sleep(500);

	// Set Sapporo_SCoverSwitch5V off
	if (!m_pIPhone->Sapporo_SCoverSwitch5V(m_nFtdPort, m_nFtdTimeOut, "0", sz_outData))
	{
		TraceLog(MSG_INFO, "Failed to Set Sapporo_SCoverSwitch5V off.");
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Successed to DC_IN 5V charging off.");
	}
	Sleep(m_i_WaitTimeForFetch);

	// Set Detroit_PWRchargerOn on
	if (!m_pIPhone->Sapporo_SCoverSwitch5V(m_nFtdPort, m_nFtdTimeOut, "1", sz_outData))
	{
		TraceLog(MSG_INFO, "Failed to Set Sapporo_SCoverSwitch5V on.");
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Successed to DC_IN 5V charging on.");
	}
	Sleep(m_i_WaitTimeForFetch);

	int i_retryTestDelay = 100;
	double d_current_on_charging = 0;
	bool b_res = false;
	double d_retry_limit = m_i_TotalTime / i_retryTestDelay;
	for (int i_retry = 0; i_retry < d_retry_limit; i_retry++)
	{
		m_pIPS3->GetCurrent(d_current_on_charging);
		if (m_rp_currentSpec.InRange(d_current_on_charging))
		{
			b_res = true;
			break;
		}
		Sleep(i_retryTestDelay);
		char szMsgBuffer[1024] = {0};
		sprintf(szMsgBuffer, "Charge Current (%d): %f", i_retry, d_current_on_charging);
		TraceLog(MSG_INFO, szMsgBuffer);
	}
	m_strMeasured = CStr::DblToStr(d_current_on_charging);

	// Set Detroit_PWRchargerOn off
	if (!m_pIPhone->Sapporo_SCoverSwitch5V(m_nFtdPort, m_nFtdTimeOut, "0", sz_outData))
	{
		TraceLog(MSG_INFO, "Failed to Set Sapporo_SCoverSwitch5V off.");
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Successed to DC_IN 5V charging off.");
	}

	if (!(m_pIPhone->FTD_PWRchargerOn(m_nFtdPort, m_nFtdTimeOut, _T("0"), sz_outData)))
	{
		TraceLog(MSG_INFO, "Fail to set Detroit_PWRchargerOn off.");
		return false;
	}

	// Test result /
	std::string str_msg;
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "Smart Cover Charge Current Test Pass!";
		str_msg =
		    "Smart Cover Charge Current is in range [" + CStr::DblToStr(m_rp_currentSpec.GetMin()) + ", " +
		    CStr::DblToStr(m_rp_currentSpec.GetMax()) + "], measured value=" + CStr::DblToStr(d_current_on_charging);
		TraceLog(MSG_INFO, str_msg);
		TraceLog(MSG_INFO, "Smart Cover Charge Current Test Pass!");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "Smart Cover Charge Current Test Fail!";
		str_msg =
		    "Smart Cover Charge Current not in range [" + CStr::DblToStr(m_rp_currentSpec.GetMin()) + ", " +
		    CStr::DblToStr(m_rp_currentSpec.GetMax()) + "], measured value=" + CStr::DblToStr(d_current_on_charging);
		TraceLog(MSG_INFO, str_msg);
		TraceLog(MSG_INFO, "Smart Cover Charge Current Test Fail!");
	}
	SetPICSData("SC_CHARGE_CURRENT", m_strMeasured);
	FactoryLog();
	return b_res;
}
