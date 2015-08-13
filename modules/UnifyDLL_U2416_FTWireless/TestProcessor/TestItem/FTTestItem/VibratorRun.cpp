
#include "stdafx.h"
#include "VibratorRun.h"

RF_IMPLEMENT_DYNCREATE(VibratorRun)

bool VibratorRun::InitData(std::map<std::string, std::string>& paramMap)
{
	//-- VibratorDelay --//
	std::string str_VibratorDelay;
	if (paramMap.find("VibratorDelay") != paramMap.end())
	{
		str_VibratorDelay = paramMap[std::string("VibratorDelay")];
		m_i_VibratorDelay = CStr::StrToInt(str_VibratorDelay);
	}
	else
	{
		m_i_VibratorDelay = 1000;
		TraceLog(MSG_INFO, "VibratorDelay set default 1000ms.");
	}

	//-- VIB_TEST Spec --//
	std::string pstrRange;
	if (paramMap.find(std::string("VIB_TESTSpec")) != paramMap.end())
	{
		pstrRange = paramMap[std::string("VIB_TESTSpec")];
		m_obj_VIB_TESTSpec.SetRange(pstrRange);
	}
	else
	{
		TraceLog(MSG_INFO, "Get VIB_TESTSpec Fail.");
		return false;
	}

	return true;
}

bool VibratorRun::PreRun()
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

bool VibratorRun::Run()
{
	if (m_pIPhone == NULL)
	{
		return false;
	}

	return VibrateTestFunc();
}

bool VibratorRun::PostRun()
{
	return true;
}

bool VibratorRun::VibrateTestFunc()
{
	if (m_pIPhone == NULL)
	{
		TraceLog(MSG_INFO, "m_pIPhone is NULL.");
		return false;
	}

	//-- vibrator,D0~2[011],D5[0]  --//
	if (!m_pIPhone->USB4702_ClearDOBit(1, 0))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit0");
		return false;
	}
	if (!m_pIPhone->USB4702_SetDOBit(1, 1))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit1");
		return false;
	}
	if (!m_pIPhone->USB4702_SetDOBit(1, 2))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit2");
		return false;
	}
	if (!m_pIPhone->USB4702_ClearDOBit(1, 5))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit5");
		return false;
	}

	//-- Vibrate Start --//
	bool b_res = true;
	TraceLog(MSG_INFO, "Vibrator ON.");
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_VibRun(m_nFtdPort, m_nFtdTimeOut, "1", sz_outBuffer)))
	{
		//-- Try again --//
		Sleep(500);
		if (!(b_res = m_pIPhone->FTD_VibRun(m_nFtdPort, m_nFtdTimeOut, "1", sz_outBuffer)))
		{
			m_strResult = "FAIL";
			m_strErrorCode = FunErr_Vibrator_Test_Fail;
			m_strMessage = "Vibrator ON Fail.";
			TraceLog(MSG_INFO, "Vibrator ON Fail.");
			FactoryLog();
			return false;
		}
	}
	TraceLog(MSG_INFO, "Vibrator ON Success.");
	Sleep(m_i_VibratorDelay);

	//-- GetADCVol --//
	char sz_VIB_TEST[10];
	if (!m_pIPhone->USB4702_GetADCVol(1, 1, sz_VIB_TEST))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_GetADCVol");
		return false;
	}

	//-- Vibrate Stop --//
	if (!(m_pIPhone->FTD_VibRun(m_nFtdPort, m_nFtdTimeOut, "0", sz_outBuffer)))
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Vibrator_Test_Fail;
		m_strMessage = "Vibrator ON Fail.";
		TraceLog(MSG_INFO, m_strMessage);
		FactoryLog();
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Vibrator OFF Success.");
	}

	//--- VIB_TEST Compare Spec ---//
	m_strMeasured = sz_VIB_TEST;
	double dw_value = 0;
	dw_value = CStr::StrToDouble(m_strMeasured);
	m_strUpper = CStr::FloatToStr((float)m_obj_VIB_TESTSpec.GetMax());
	m_strLower = CStr::FloatToStr((float)m_obj_VIB_TESTSpec.GetMin());
	if (!m_obj_VIB_TESTSpec.InRange(dw_value))
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Vibrator_Test_Fail;
		m_strMessage = "VIB_TEST is not in the spec.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("VIBRATOR", m_strMeasured);
		FactoryLog();
		return false;
	}
	m_strResult = "PASS";
	m_strMessage = "VIB_TEST in the spec.";
	TraceLog(MSG_INFO, m_strMessage);
	SetPICSData("VIBRATOR", m_strMeasured);
	FactoryLog();
	//--- Compare Spec End ---//
	return true;
}
