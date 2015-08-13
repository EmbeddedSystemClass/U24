#include "stdafx.h"
#include "PowerSupply.h"


RF_IMPLEMENT_DYNCREATE(CPowerSupplyItem)


bool CPowerSupplyItem::Run()
{
	bool bRes = MainFunction();

	if (bRes)
	{
		FactoryLog(true, "StartPowerSupply", "--", "--", "--", "--", "--", "Pass", "--", "Power supply control PASS");
		SetPICSDate("StartPowerSupply", "PASS");
	}
	else
	{
		FactoryLog(false, "--", ERR_DEVICE_PS_INIT, "--", "--", "--", "--", "Fail", "--", m_strMsg);
		SetPICSDate("StartPowerSupply", "FAIL");
	}

	return bRes;
}

bool CPowerSupplyItem::MainFunction()
{
	IPowerSupply* pIPS = NULL;

	if (m_iPS_Index == 1)
		pIPS = m_pIPS;
	else if (m_iPS_Index == 2)
		pIPS = m_pIPS2;
	else if (m_iPS_Index == 3)
		pIPS = m_pIPS3;
	else
	{
		m_strMsg = "Fail to parse PS index, no such PS exist";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (pIPS == NULL)
	{
		m_strMsg = "Fail to control PS, PS not detected";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! pIPS->IsAvailable())
	{
		m_strMsg = "Fail to control PS, PS not available";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! pIPS->SetVoltage(m_dVoltage))
	{
		m_strMsg = "Fail to set voltage to PS";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! pIPS->SetCurrentLimit(m_dCurrent))
	{
		m_strMsg = "Fail to set current to PS";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! pIPS->SetCurrRangeMax())
	{
		m_strMsg = "Fail to set current range MAXIMUM to PS";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! pIPS->SetPSOnOff(m_bPS_OnOff))
	{
		m_strMsg = "Fail to control PS on/off";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	return true;
}


bool CPowerSupplyItem::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find("PS_Index") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Voltage for CPowerSupplyControl, default value = 1");
		m_iPS_Index = 1;
	}
	else
		m_iPS_Index = atoi(paramMap["PS_Index"].c_str());

	if (paramMap.find ("Voltage") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Voltage for CPowerSupplyItem");
		return false;
	}
	m_dVoltage = atof(paramMap["Voltage"].c_str());

	if (paramMap.find ("Current") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Current for CPowerSupplyItem");
		return false;
	}
	m_dCurrent = atof(paramMap["Current"].c_str());

	if (paramMap.find ("PS_OnOff") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PS_OnOff for CPowerSupplyItem");
		return false;
	}
	if (paramMap["PS_OnOff"] == "TRUE" || paramMap["PS_OnOff"] == "true")
		m_bPS_OnOff = true;
	else
		m_bPS_OnOff = false;

	return true;
}
