
#include "stdafx.h"
#include "PowerConnectorTest.h"

RF_IMPLEMENT_DYNCREATE(CPowerConnectorTest)

bool CPowerConnectorTest::InitData(std::map<std::string, std::string>& paramMap)
{
	std::string pstrRange;
	if (paramMap.find(std::string("PowerVoltageSpec")) != paramMap.end())
	{
		pstrRange = paramMap[std::string("PowerVoltageSpec")];
		m_obj_PowerVoltageSpec.SetRange(pstrRange);
	}
	else
	{
		TraceLog(MSG_INFO, "Get PowerVoltageSpec Fail.");
		return false;
	}

	if (paramMap.find(std::string("PowerBattIDSpec")) != paramMap.end())
	{
		pstrRange = paramMap[std::string("PowerBattIDSpec")];
		m_obj_PowerBattIDSpec.SetRange(pstrRange);
	}
	else
	{
		TraceLog(MSG_INFO, "Get PowerBattIDSpec Fail.");
		return false;
	}

	if (paramMap.find(std::string("PowerGaugeTemperatureSpec")) != paramMap.end())
	{
		pstrRange = paramMap[std::string("PowerGaugeTemperatureSpec")];
		m_obj_PowerGaugeTemperatureSpec.SetRange(pstrRange);
	}
	else
	{
		TraceLog(MSG_INFO, "Get PowerGaugeTemperatureSpec Fail.");
		return false;
	}

	return true;
}

bool CPowerConnectorTest::PreRun()
{
	m_strResult    = "";
	m_strItemCode  = CStr::IntToStr(PowerConnector_Test_BaseItemCode);
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

bool CPowerConnectorTest::Run()
{
	if (m_pIPhone == NULL) return false;

	bool b_res = false;
	b_res = PowerGaugeVoltage();
	if (b_res)
	{
		b_res = PowerBattID();
	}
	if (b_res)
	{
		b_res = PowerGaugeTemperature();
	}
	return b_res;
}

bool CPowerConnectorTest::PostRun()
{
	return true;
}

bool CPowerConnectorTest::PowerGaugeVoltage()
{
	if (m_pIPhone == NULL)
	{
		TraceLog(MSG_INFO, "m_pIPhone is NULL.");
		return false;
	}

	bool b_res = true;
	/* PowerGaugeVoltage Start */
	TraceLog(MSG_INFO, "Read Power Gauge Voltage Start.");
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_PWRgaugeVoltage(m_nFtdPort, m_nFtdTimeOut, "internal", sz_outBuffer)))
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Check_Battery_Voltage_Fail;
		m_strMessage = "Read Power Gauge Voltage Fail.";
		SetPICSData("POWER_GAUGE_VOLTAGE", "FAIL");
		TraceLog(MSG_INFO, "Read Power Gauge Voltage Fail.");
		FactoryLog();
		return false;
	}
	TraceLog(MSG_INFO, "Read Power Gauge Voltage Success.");

	double f_PowerGaugeVoltage;
	f_PowerGaugeVoltage = atof(sz_outBuffer);
	f_PowerGaugeVoltage = f_PowerGaugeVoltage / 1000000;
	m_strMeasured = CStr::DblToStr(f_PowerGaugeVoltage);
	m_strUpper = CStr::FloatToStr((float)m_obj_PowerVoltageSpec.GetMax());
	m_strLower = CStr::FloatToStr((float)m_obj_PowerVoltageSpec.GetMin());
	if(!m_obj_PowerVoltageSpec.InRange(f_PowerGaugeVoltage))
	{
		b_res = false;
	}
	if (b_res)
	{
		m_strResult = "PASS";
		m_strMessage = "Power Gauge Voltage Test Pass.";
		SetPICSData("POWER_GAUGE_VOLTAGE", m_strMeasured);
		TraceLog(MSG_INFO, "Power Gauge Voltage Test Pass.");
		FactoryLog();
	}
	else
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Check_Battery_Voltage_Fail;
		m_strMessage = "Power Gauge Voltage Test Fail.";
		SetPICSData("POWER_GAUGE_VOLTAGE", "FAIL");
		TraceLog(MSG_INFO, "Power Gauge Voltage Test Fail.");
		FactoryLog();
	}
	return b_res;
}

bool CPowerConnectorTest::PowerBattID()
{
	if (m_pIPhone == NULL)
	{
		TraceLog(MSG_INFO, "m_pIPhone is NULL.");
		return false;
	}

	bool b_res = true;
	/* PowerBattID Start */
	TraceLog(MSG_INFO, "Read Battery ID Start.");
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->ReadBatteryID(m_nFtdPort, m_nFtdTimeOut, sz_outBuffer)))
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Check_Battery_ID_Fail;
		m_strMessage = "Read Battery ID Fail.";
		SetPICSData("POWER_BATTERY_ID", "FAIL");
		TraceLog(MSG_INFO, "Read Battery ID Fail.");
		FactoryLog();
		return false;
	}
	TraceLog(MSG_INFO, "Read Battery ID Success.");

	double f_PowerBatteryID;
	f_PowerBatteryID = atof(sz_outBuffer);
	f_PowerBatteryID = f_PowerBatteryID / 1000000;
	m_strMeasured = CStr::DblToStr(f_PowerBatteryID);
	m_strUpper = CStr::FloatToStr((float)m_obj_PowerBattIDSpec.GetMax());
	m_strLower = CStr::FloatToStr((float)m_obj_PowerBattIDSpec.GetMin());
	if(!m_obj_PowerBattIDSpec.InRange(f_PowerBatteryID))
	{
		b_res = false;
	}
	if (b_res)
	{
		m_strResult = "PASS";
		m_strMessage = "Battery ID Test Pass.";
		SetPICSData("POWER_BATTERY_ID", m_strMeasured);
		TraceLog(MSG_INFO, "Battery ID Test Pass.");
		FactoryLog();
	}
	else
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Check_Battery_ID_Fail;
		TraceLog(MSG_INFO, "Battery ID Test Fail.");
		SetPICSData("POWER_BATTERY_ID", "FAIL");
		FactoryLog();
	}
	return b_res;
}

bool CPowerConnectorTest::PowerGaugeTemperature()
{
	if (m_pIPhone == NULL)
	{
		TraceLog(MSG_INFO, "m_pIPhone is NULL.");
		return false;
	}

	bool b_res = true;
	/* PowerGaugeTemperature Start */
	TraceLog(MSG_INFO, "Read Power Gauge Temperature Start.");
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_PWRgaugeTemperature(m_nFtdPort, m_nFtdTimeOut, "internal", sz_outBuffer)))
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Check_Battery_Temperature_Fail;
		m_strMessage = "Read Power Gauge Temperature Fail.";
		SetPICSData("POWER_GAUGE_TEMPERATURE", "FAIL");
		TraceLog(MSG_INFO, "Read Power Gauge Temperature Fail.");
		FactoryLog();
		return false;
	}
	TraceLog(MSG_INFO, "Read Power Gauge Temperature Success.");

	double f_PowerGaugeTemperature;
	f_PowerGaugeTemperature = atof(sz_outBuffer);
	f_PowerGaugeTemperature = f_PowerGaugeTemperature / 10;
	m_strMeasured = CStr::DblToStr(f_PowerGaugeTemperature);
	m_strUpper = CStr::FloatToStr((float)m_obj_PowerGaugeTemperatureSpec.GetMax());
	m_strLower = CStr::FloatToStr((float)m_obj_PowerGaugeTemperatureSpec.GetMin());
	if(!(m_obj_PowerGaugeTemperatureSpec.InRange(f_PowerGaugeTemperature)))
	{
		b_res = false;
	}

	if (b_res)
	{
		m_strResult = "PASS";
		m_strMessage = "Power Gauge Temperature Test Pass.";
		SetPICSData("POWER_GAUGE_TEMPERATURE", m_strMeasured);
		TraceLog(MSG_INFO, "Power Gauge Temperature Test Pass.");
		FactoryLog();
	}
	else
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Check_Battery_Temperature_Fail;
		SetPICSData("POWER_GAUGE_TEMPERATURE", "FAIL");
		TraceLog(MSG_INFO, "Power Gauge Temperature Test Fail.");
		FactoryLog();
	}
	return b_res;
}
