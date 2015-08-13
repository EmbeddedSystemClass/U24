
#include "stdafx.h"
#include "FlashLED.h"

RF_IMPLEMENT_DYNCREATE(FlashLED)

bool FlashLED::InitData(std::map<std::string, std::string>& paramMap)
{
	std::string pstrRange;

	//-- CAM_FLASH_LED Spec --//
	if (paramMap.find(std::string("CAM_FLASH_LEDSpec")) != paramMap.end())
	{
		pstrRange = paramMap[std::string("CAM_FLASH_LEDSpec")];
		m_obj_CAM_FLASH_LEDSpec.SetRange(pstrRange);
	}
	else
	{
		TraceLog(MSG_INFO, "Get CAM_FLASH_LEDSpec Fail.");
		return false;
	}

	return true;
}

bool FlashLED::PreRun()
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

bool FlashLED::Run()
{
	if (m_pIPhone == NULL) return false;

	return FlashLEDTestFunc();
}

bool FlashLED::PostRun()
{
	return true;
}

bool FlashLED::FlashLEDTestFunc()
{
	if (m_pIPhone == NULL)
	{
		TraceLog(MSG_INFO, "m_pIPhone is NULL.");
		return false;
	}

	//-- Open CAMFlashLED --//
	TraceLog(MSG_INFO, "CAMFlashLED ON.");
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (! m_pIPhone->FTD_CAMFlashLED(m_nFtdPort, m_nFtdTimeOut, "1", sz_outBuffer))
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Flash_LED_Test_Fail;
		m_strMessage = "CAMFlashLED ON Fail.";
		TraceLog(MSG_INFO, "CAMFlashLED ON Fail.");
		FactoryLog();
		return false;
	}
	TraceLog(MSG_INFO, "CAMFlashLED ON Success.");

	//-- GetADCVol --//
	char sz_CAM_FLASH_LED[10];
	if (! m_pIPhone->USB4702_GetADCVol(1, 7, sz_CAM_FLASH_LED))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_GetADCVol");
		return false;
	}

	//-- Close CAMFlashLED --//
	TraceLog(MSG_INFO, "CAMFlashLED OFF.");
	if (! m_pIPhone->FTD_CAMFlashLED(m_nFtdPort, m_nFtdTimeOut, "0", sz_outBuffer))
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Flash_LED_Test_Fail;
		m_strMessage = "CAMFlashLED OFF Fail.";
		TraceLog(MSG_INFO, "CAMFlashLED OFF Fail.");
		FactoryLog();
		return false;
	}
	TraceLog(MSG_INFO, "CAMFlashLED OFF Success.");

	//--- CAMFlashLED Compare Spec ---//
	m_strMeasured = sz_CAM_FLASH_LED;
	double dw_value = 0;
	dw_value = CStr::StrToDouble(m_strMeasured);
	m_strUpper = CStr::FloatToStr((float)m_obj_CAM_FLASH_LEDSpec.GetMax());
	m_strLower = CStr::FloatToStr((float)m_obj_CAM_FLASH_LEDSpec.GetMin());
	if (!m_obj_CAM_FLASH_LEDSpec.InRange(dw_value))
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Flash_LED_Test_Fail;
		m_strMessage = "CAMFlashLED is not in the spec.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("CAM_FLASH_LED", m_strMeasured);
		FactoryLog();
		return false;
	}
	m_strResult = "PASS";
	m_strMessage = "CAMFlashLED in the spec.";
	TraceLog(MSG_INFO, m_strMessage);
	SetPICSData("CAM_FLASH_LED", m_strMeasured);
	FactoryLog();
	//--- Compare Spec End ---//
	return true;
}
