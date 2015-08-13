
#include "stdafx.h"
#include "LCDBacklight.h"

RF_IMPLEMENT_DYNCREATE(LCDBacklight)

bool LCDBacklight::InitData(std::map<std::string, std::string>& paramMap)
{

	//-- LCDBacklight Spec --//
	std::string pstrRange;
	if (paramMap.find(std::string("Backlight_Spec")) != paramMap.end())
	{
		pstrRange = paramMap[std::string("Backlight_Spec")];
		m_obj_Backlight_Spec.SetRange(pstrRange);
	}
	else
	{
		TraceLog(MSG_INFO, _T("Get Backlight Spec Fail."));
		return false;
	}

	//-- LCDBacklight Level --//
	if (paramMap.find(std::string("Backlight_Level")) != paramMap.end())
	{
		m_str_lightLevel = paramMap[std::string("Backlight_Level")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Get Backlight Level Fail."));
		return false;
	}

	//-- Delay --//
	std::string str_Delay;
	if (paramMap.find("Delay_Time_ms") != paramMap.end())
	{
		str_Delay = paramMap[std::string("Delay_Time_ms")];
		m_i_Delay = CStr::StrToInt(str_Delay);
	}
	else
	{
		m_i_Delay = 0;
		TraceLog(MSG_INFO, _T("Delay set default 0ms."));
	}

	return true;
}

bool LCDBacklight::PreRun()
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

bool LCDBacklight::Run()
{
	if (m_pIPhone == NULL) return false;

	return BacklightTestFunc();
}

bool LCDBacklight::PostRun()
{
	return true;
}

bool LCDBacklight::BacklightTestFunc()
{
	if (m_pIPhone == NULL)
	{
		TraceLog(MSG_INFO, _T("m_pIPhone is NULL."));
		return false;
	}

	//-- LCD backlight,D0~2[000],D5[0],CH1  --//
	if (! m_pIPhone->USB4702_ClearDOBit(1, 0))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit0");
		return false;
	}
	if (! m_pIPhone->USB4702_ClearDOBit(1, 1))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit1");
		return false;
	}
	if (! m_pIPhone->USB4702_ClearDOBit(1, 2))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit2");
		return false;
	}
	if (! m_pIPhone->USB4702_ClearDOBit(1, 5))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit5");
		return false;
	}
	Sleep(m_i_Delay);

	bool b_res = false;
	std::string str_msg;
	/* First turn off Backlight */
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->LED_BacklightOnOff(m_nFtdPort, m_nFtdTimeOut, "0", sz_outBuffer)))
	{
		str_msg = "Fail to turn off mobile backlight.";
		TraceLog(MSG_INFO, str_msg);
	}
	if (b_res)
	{
		Sleep(100);
		char sz_lightLevel[32] = {0};
		strcpy(sz_lightLevel, m_str_lightLevel.c_str());
		if ((b_res = m_pIPhone->LED_BacklightOnOff(m_nFtdPort, m_nFtdTimeOut, sz_lightLevel, sz_outBuffer)))
		{
			str_msg = "Turn on mobile backlight FTD comamnd success";
			TraceLog(MSG_INFO, str_msg);
		}
	}

	//-- GetADCVol --//
	char sz_LCD_BACKLIGHT[10];
	if (! m_pIPhone->USB4702_GetADCVol(1, 1, sz_LCD_BACKLIGHT))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_GetADCVol");
		return false;
	}

	//--- LCD_BACKLIGHT Compare Spec ---//
	m_strMeasured = sz_LCD_BACKLIGHT;
	double dw_value = 0;
	dw_value = CStr::StrToDouble(m_strMeasured);
	m_strUpper = CStr::FloatToStr((float)m_obj_Backlight_Spec.GetMax());
	m_strLower = CStr::FloatToStr((float)m_obj_Backlight_Spec.GetMin());
	if (!m_obj_Backlight_Spec.InRange(dw_value))
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_LCD_Off_Current_Test_Fail;
		m_strMessage = "LCD_BACKLIGHT is not in the spec.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("LCD_BACKLIGHT", m_strMeasured);
		FactoryLog();
		return false;
	}

	if (!(b_res = m_pIPhone->LED_BacklightOnOff(m_nFtdPort, m_nFtdTimeOut, "0", sz_outBuffer)))
	{
		str_msg = "Fail to turn off mobile backlight.";
		TraceLog(MSG_INFO, str_msg);
	}
	m_strResult = "PASS";
	m_strMessage = "LCD_BACKLIGHT in the spec.";
	TraceLog(MSG_INFO, m_strMessage);
	SetPICSData("LCD_BACKLIGHT", m_strMeasured);
	FactoryLog();
	//--- Compare Spec End ---//
	return true;
}
