/*********************************************************************************
 * COPYRIGHT Qisda WGC18 2011
 *
 * Description: .cpp file of CPowerSupply object.
 * FileName   : CPowerSupply.cpp
 * Version    : $Revision:   1.1  $
 * Author     : Gilpin.Xi
 * Time       : 2011/07/28
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/07/28      Gilpin.Xi             Create.
 ********************************************************************************/

#include "stdafx.h"
#include "PowerSupply.h"


RF_IMPLEMENT_DYNCREATE(CPowerSupply)


bool CPowerSupply::PreRun()
{
	m_strResult = "-";
	m_strItemCode = "-";
	m_strErrorCode = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";

	return true;
}

bool CPowerSupply::Run()
{
	if (m_b_PS_OnOff)
	{
		m_strItemCode = CStr::IntToStr(StartPowersupply_BaseItemCode);
		m_strErrorCode = CommErr_PowerSupply_Control_Fail;

		return StartPowerSupply();
	} 
	else
	{
		m_strItemCode = CStr::IntToStr(StopPowersupply_BaseItemCode);
		m_strErrorCode = CommErr_PowerSupply_Control_Fail;

		return StopPowerSupply();
	}
}

bool CPowerSupply::PostRun()
{
	return true;
}

bool CPowerSupply::InitData(std::map<std::string, std::string>& paramMap)
{

	/* Voltage */
	std::string str_voltage;
	if (paramMap.find ("Voltage") != paramMap.end())
	{
		str_voltage = paramMap["Voltage"];
		m_d_voltage = atof(str_voltage.c_str());
	}
	else
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Voltage for CPowerSupply");
		return false;
	}

	/* Current */
	std::string str_current;
	if (paramMap.find ("Current") != paramMap.end())
	{
		str_current = paramMap["Current"];
		m_d_current = atof(str_current.c_str());
	}
	else
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Current for CPowerSupply");
		return false;
	}

	/* PS_OnOff flag */
	std::string str_PS_OnOff;
	if (paramMap.find (std::string("PS_OnOff")) != paramMap.end())
	{
		CString str_temp;
		str_PS_OnOff = paramMap[std::string("PS_OnOff")];

		str_temp = str_PS_OnOff.c_str();
		if (str_temp.CompareNoCase(_T("TRUE")) == 0)
		{
			m_b_PS_OnOff = true;
		} 
		else
		{
			m_b_PS_OnOff = false;
		}
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter PS_OnOff for CPowerSupply");
		return false;
	}

	return true;
}

bool CPowerSupply::StartPowerSupply()
{
	bool b_res = false;
	std::string str_msg;

	if (m_pIPS == NULL)
	{
		str_msg = _T("Fail to control PS, PS not detected");
		TraceLog(MSG_ERROR, str_msg);
	}

	if ( !(b_res = m_pIPS->PowerSupplyIsAvaliable()))
	{
		str_msg = _T("Fail to control PS, PS not available");
		TraceLog(MSG_ERROR, str_msg);
	}

	// Neil
	/* Set PowerSupply OFF */
	if (b_res)
	{
		b_res = m_pIPS->SetPSOnOff(false);
	}

	/* Set PowerSupply voltage */
	if (b_res)
	{
		b_res = m_pIPS->SetVoltage(m_d_voltage);

		if (b_res)
		{
			str_msg = _T("Set PS voltage :") + CStr::DblToStr(m_d_voltage) + _T("v");
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			str_msg = _T("Fail to set voltage to PS");
			TraceLog(MSG_ERROR, str_msg);
		}
	}

	/* Set PowerSupply current */
	if (b_res)
	{
		b_res = m_pIPS->SetCurrentLimit(m_d_current);

		if (b_res)
		{
			str_msg = _T("Set PS current :") + CStr::DblToStr(m_d_current) + _T("A");
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			str_msg = _T("Fail to set current to PS");
			TraceLog(MSG_ERROR, str_msg);
		}
	}

	/* Set PowerSupply current range */
	if (b_res)
	{
		b_res = m_pIPS->SetCurrRangeMax();

		if (b_res)
		{
			str_msg = _T("Set current range MAXIMUM to PS");
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			str_msg = _T("Fail to set current range MAXIMUM to PS");
			TraceLog(MSG_ERROR, str_msg);
		}
	}

	/* Set PowerSupply ON */
	if (b_res)
	{
		b_res = m_pIPS->SetPSOnOff(m_b_PS_OnOff);
	}
	
	/* Test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = _T("Set powerSupply on success");
		TraceLog(MSG_INFO, _T("Set powerSupply on success"));
		FactoryLog();
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_ERROR, _T("Fail to Set powerSupply on"));
		FactoryLog();
	}

	return b_res;
}

bool CPowerSupply::StopPowerSupply()
{
	bool b_res = false;
	std::string str_msg;

	if (m_pIPS == NULL)
	{
		str_msg = _T("Fail to control PS, PS not detected");
		TraceLog(MSG_ERROR, str_msg);
	}

	if ( !(b_res = m_pIPS->PowerSupplyIsAvaliable()))
	{
		str_msg = _T("Fail to control PS, PS not available");
		TraceLog(MSG_ERROR, str_msg);
	}

	/* Set PowerSupply OFF */
	if (b_res)
	{
		b_res = m_pIPS->SetPSOnOff(m_b_PS_OnOff);
	}

	/* Test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = _T("Set powerSupply off success");
		TraceLog(MSG_INFO, _T("Set powerSupply off success"));
		FactoryLog();
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_ERROR, _T("Fail to Set powerSupply off"));
		FactoryLog();
	}

	return b_res;
}