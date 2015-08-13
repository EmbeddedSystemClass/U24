
#include "stdafx.h"
#include "IndicatorLED.h"

RF_IMPLEMENT_DYNCREATE(CIndicatorLED)

bool CIndicatorLED::InitData(std::map<std::string, std::string>& paramMap)
{
	m_str_IND_LED_R_Input = "";
	m_str_IND_LED_G_Input = "";
	m_str_IND_LED_B_Input = "";
	if (paramMap.find(std::string("IND_LED_R_Input")) != paramMap.end())
	{
		m_str_IND_LED_R_Input = paramMap[std::string("IND_LED_R_Input")];
	}

	if (paramMap.find(std::string("IND_LED_G_Input")) != paramMap.end())
	{
		m_str_IND_LED_G_Input = paramMap[std::string("IND_LED_G_Input")];
	}

	if (paramMap.find(std::string("IND_LED_B_Input")) != paramMap.end())
	{
		m_str_IND_LED_B_Input = paramMap[std::string("IND_LED_B_Input")];
	}

	std::string pstrRange;
	if (paramMap.find(std::string("IND_LED_R_Spec")) != paramMap.end())
	{
		pstrRange = paramMap[std::string("IND_LED_R_Spec")];
		m_obj_IND_LED_R_Spec.SetRange(pstrRange);
	}
	else
	{
		TraceLog(MSG_INFO, "Get IND_LED_R_Spec Fail.");
		return false;
	}

	if (paramMap.find(std::string("IND_LED_G_Spec")) != paramMap.end())
	{
		pstrRange = paramMap[std::string("IND_LED_G_Spec")];
		m_obj_IND_LED_G_Spec.SetRange(pstrRange);
	}
	else
	{
		TraceLog(MSG_INFO, "Get IND_LED_G_Spec Fail.");
		return false;
	}

	if (paramMap.find(std::string("IND_LED_B_Spec")) != paramMap.end())
	{
		pstrRange = paramMap[std::string("IND_LED_B_Spec")];
		m_obj_IND_LED_B_Spec.SetRange(pstrRange);
	}
	else
	{
		TraceLog(MSG_INFO, "Get IND_LED_B_Spec Fail.");
		return false;
	}
	return true;
}

bool CIndicatorLED::PreRun()
{
	m_strResult    = "";
	m_strItemCode  = "";
	m_strErrorCode = "-";
	m_strBand      = "-";
	m_strChannel   = "-";
	m_strUpper     = "";
	m_strLower     = "";
	m_strMeasured  = "-";
	m_strUnit      = "-";
	m_strMessage   = "";
	return true;
}

bool CIndicatorLED::Run()
{
	if (m_pIPhone == NULL)
	{
		return false;
	}

	return FlashLEDTestFunc();
}

bool CIndicatorLED::PostRun()
{
	return true;
}

bool CIndicatorLED::FlashLEDTestFunc()
{
	if (m_pIPhone == NULL)
	{
		TraceLog(MSG_INFO, "m_pIPhone is NULL.");
		return false;
	}

	double dw_value = 0;
	if (m_str_IND_LED_R_Input != "")
	{
		//-- LED_R,D0~2[000],D5[0],CH2 --//
		if (!m_pIPhone->USB4702_ClearDOBit(1, 0))
		{
			TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit0");
			return false;
		}
		if (!m_pIPhone->USB4702_ClearDOBit(1, 1))
		{
			TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit1");
			return false;
		}
		if (!m_pIPhone->USB4702_ClearDOBit(1, 2))
		{
			TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit2");
			return false;
		}
		if (!m_pIPhone->USB4702_ClearDOBit(1, 5))
		{
			TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit5");
			return false;
		}

		//-- Red LED ON --//
		TraceLog(MSG_INFO, "Red LED ON.");
		char sz_inBuffer[FTD_BUF_SIZE] = {0};
		memcpy(sz_inBuffer, m_str_IND_LED_R_Input.c_str(), m_str_IND_LED_R_Input.length());
		char sz_outBuffer[FTD_BUF_SIZE] = {0};
		if (!m_pIPhone->FTD_LEDon(m_nFtdPort, m_nFtdTimeOut, sz_inBuffer, sz_outBuffer))
		{
			m_strResult = "FAIL";
			m_strErrorCode = FunErr_Check_Indicator_LED_R_Fail;
			m_strMessage = "Red LED ON Fail.";
			TraceLog(MSG_INFO, "Red LED ON Fail.");
			FactoryLog();
			return false;
		}
		TraceLog(MSG_INFO, "Red LED ON Success.");
		Sleep(500);

		//-- GetADCVol --//
		char sz_IND_LED_R[10];
		if (!m_pIPhone->USB4702_GetADCVol(1, 2, sz_IND_LED_R))
		{
			TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_GetADCVol");
			return false;
		}

		//--- indicator LED_R Compare Spec ---//
		m_strMeasured = sz_IND_LED_R;
		dw_value = CStr::StrToDouble(m_strMeasured);
		m_strUpper = CStr::FloatToStr((float)m_obj_IND_LED_R_Spec.GetMax());
		m_strLower = CStr::FloatToStr((float)m_obj_IND_LED_R_Spec.GetMin());
		if (!m_obj_IND_LED_R_Spec.InRange(dw_value))
		{
			m_strResult = "FAIL";
			m_strErrorCode = FunErr_Check_Indicator_LED_R_Fail;
			m_strMessage = "Indicator LED_R is not in the spec.";
			TraceLog(MSG_INFO, m_strMessage);
			SetPICSData("IND_LED_R", m_strMeasured);
			FactoryLog();
			return false;
		}
		m_strResult = "PASS";
		m_strMessage = "Indicator LED_R in the spec.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("IND_LED_R", m_strMeasured);
		FactoryLog();
		//--- Compare Spec End ---//

		TraceLog(MSG_INFO, "LED OFF.");
		if (!m_pIPhone->FTD_LEDon(m_nFtdPort, m_nFtdTimeOut, "0,0,0", sz_outBuffer))
		{
			m_strResult = "FAIL";
			m_strErrorCode = FunErr_Check_Indicator_LED_OFF_Fail;
			m_strMessage = "LED OFF Fail.";
			TraceLog(MSG_INFO, "LED OFF Fail.");
			FactoryLog();
			return false;
		}
		TraceLog(MSG_INFO, "LED OFF Success.");
	}

	if (m_str_IND_LED_G_Input != "")
	{
		//-- LED_G,D0~2[010],D5[0],CH2 --//
		if (!m_pIPhone->USB4702_SetDOBit(1, 1))
		{
			TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit0");
			return false;
		}

		//-- Green LED ON --//
		TraceLog(MSG_INFO, "Green LED ON.");
		char sz_outBuffer[FTD_BUF_SIZE] = {0};
		char sz_inBuffer[FTD_BUF_SIZE] = {0};
		memcpy(sz_inBuffer, m_str_IND_LED_G_Input.c_str(), m_str_IND_LED_G_Input.length());
		if (!m_pIPhone->FTD_LEDon(m_nFtdPort, m_nFtdTimeOut, sz_inBuffer, sz_outBuffer))
		{
			m_strResult = "FAIL";
			m_strErrorCode = FunErr_Check_Indicator_LED_G_Fail;
			m_strMessage = "Green LED ON Fail.";
			TraceLog(MSG_INFO, "Green LED ON Fail.");
			FactoryLog();
			return false;
		}
		TraceLog(MSG_INFO, "Green LED ON Success.");
		Sleep(500);

		//-- GetADCVol --//
		char sz_IND_LED_G[10];
		if (!m_pIPhone->USB4702_GetADCVol(1, 2, sz_IND_LED_G))
		{
			TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_GetADCVol");
			return false;
		}

		//--- indicator LED_G Compare Spec ---//
		m_strMeasured = sz_IND_LED_G;
		dw_value = 0;
		dw_value = CStr::StrToDouble(m_strMeasured);
		m_strUpper = CStr::FloatToStr((float)m_obj_IND_LED_G_Spec.GetMax());
		m_strLower = CStr::FloatToStr((float)m_obj_IND_LED_G_Spec.GetMin());
		if (!m_obj_IND_LED_G_Spec.InRange(dw_value))
		{
			m_strResult = "FAIL";
			m_strErrorCode = FunErr_Check_Indicator_LED_G_Fail;
			m_strMessage = "Indicator LED_G is not in the spec.";
			TraceLog(MSG_INFO, m_strMessage);
			SetPICSData("IND_LED_G", m_strMeasured);
			FactoryLog();
			return false;
		}
		m_strResult = "PASS";
		m_strMessage = "Indicator LED_G in the spec.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("IND_LED_G", m_strMeasured);
		FactoryLog();
		//--- Compare Spec End ---//

		TraceLog(MSG_INFO, "LED OFF.");
		if (!m_pIPhone->FTD_LEDon(m_nFtdPort, m_nFtdTimeOut, "0,0,0", sz_outBuffer))
		{
			m_strResult = "FAIL";
			m_strErrorCode = FunErr_Check_Indicator_LED_OFF_Fail;
			m_strMessage = "LED OFF Fail.";
			TraceLog(MSG_INFO, "LED OFF Fail.");
			FactoryLog();
			return false;
		}
		TraceLog(MSG_INFO, "LED OFF Success.");
	}
	if (m_str_IND_LED_B_Input != "")
	{
		//-- LED_B,D0~2[001],D5[0],CH2 --//
		if (!m_pIPhone->USB4702_ClearDOBit(1, 1))
		{
			TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit0");
			return false;
		}

		if (!m_pIPhone->USB4702_SetDOBit(1, 2))
		{
			TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit1");
			return false;
		}

		//-- Blue LED ON --//
		TraceLog(MSG_INFO, "Blue LED ON.");
		char sz_outBuffer[FTD_BUF_SIZE] = {0};
		char sz_inBuffer[FTD_BUF_SIZE] = {0};
		memcpy(sz_inBuffer, m_str_IND_LED_B_Input.c_str(), m_str_IND_LED_B_Input.length());
		if (!m_pIPhone->FTD_LEDon(m_nFtdPort, m_nFtdTimeOut, sz_inBuffer, sz_outBuffer))
		{
			m_strResult = "FAIL";
			m_strErrorCode = FunErr_Check_Indicator_LED_B_Fail;
			m_strMessage = "Blue LED ON Fail.";
			TraceLog(MSG_INFO, "Blue LED ON Fail.");
			FactoryLog();
			return false;
		}
		TraceLog(MSG_INFO, "Blue LED ON Success.");
		Sleep(500);

		//-- GetADCVol --//
		char sz_IND_LED_B[10];
		if (!m_pIPhone->USB4702_GetADCVol(1, 2, sz_IND_LED_B))
		{
			TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_GetADCVol");
			return false;
		}

		//--- indicator LED_B Compare Spec ---//
		m_strMeasured = sz_IND_LED_B;
		dw_value = 0;
		dw_value = CStr::StrToDouble(m_strMeasured);
		m_strUpper = CStr::FloatToStr((float)m_obj_IND_LED_B_Spec.GetMax());
		m_strLower = CStr::FloatToStr((float)m_obj_IND_LED_B_Spec.GetMin());
		if (!m_obj_IND_LED_B_Spec.InRange(dw_value))
		{
			m_strResult = "FAIL";
			m_strErrorCode = FunErr_Check_Indicator_LED_B_Fail;
			m_strMessage = "Indicator LED_B is not in the spec.";
			TraceLog(MSG_INFO, m_strMessage);
			SetPICSData("IND_LED_B", m_strMeasured);
			FactoryLog();
			return false;
		}
		m_strResult = "PASS";
		m_strMessage = "Indicator LED_B in the spec.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("IND_LED_B", m_strMeasured);
		FactoryLog();
		//--- Compare Spec End ---//

		TraceLog(MSG_INFO, "LED OFF.");
		if (!m_pIPhone->FTD_LEDon(m_nFtdPort, m_nFtdTimeOut, "0,0,0", sz_outBuffer))
		{
			m_strResult = "FAIL";
			m_strErrorCode = FunErr_Check_Indicator_LED_OFF_Fail;
			m_strMessage = "LED OFF Fail.";
			TraceLog(MSG_INFO, "LED OFF Fail.");
			FactoryLog();
			return false;
		}
		TraceLog(MSG_INFO, "LED OFF Success.");
	}
	return true;
}
