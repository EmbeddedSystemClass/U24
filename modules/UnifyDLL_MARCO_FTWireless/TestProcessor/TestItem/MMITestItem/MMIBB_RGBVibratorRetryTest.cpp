/*********************************************************************************
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1   2014    Lion.Wu            Create for Detroit project MMI test
 ********************************************************************************/

#include "stdafx.h"
#include "MMIBB_RGBVibratorRetryTest.h"


RF_IMPLEMENT_DYNCREATE(CMMIBB_RGBVibratorRetryTest)


bool CMMIBB_RGBVibratorRetryTest::PreRun()
{
	m_strResult = "-";
	m_strItemCode = CStr::IntToStr(Loopback_Test_BaseItemCode);
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMessage = "";
	m_strErrorCode = "-";
	return true;
}


bool CMMIBB_RGBVibratorRetryTest::PostRun()
{
	return true;
}

bool CMMIBB_RGBVibratorRetryTest::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find(std::string("XMLTestItem")) != paramMap.end())
	{
		m_str_TestItem = paramMap[std::string("XMLTestItem")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter XMLTestItem for CMMIBB_RGBVibratorRetryTest");
		return false;
	}

	if (paramMap.find(std::string("XMLCMDItem")) != paramMap.end())
	{
		m_str_CMD = paramMap[std::string("XMLCMDItem")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter XMLCMDItem for CMMIBB_RGBVibratorRetryTest");
		return false;
	}

	if (paramMap.find(std::string("XMLOffCMDItem")) != paramMap.end())
	{
		m_str_OffCMD = paramMap[std::string("XMLOffCMDItem")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter XMLOffCMDItem for CMMIBB_RGBVibratorRetryTest");
		return false;
	}

	std::string str_SleepTime;
	if (paramMap.find(std::string("XMLOnSleepTime")) != paramMap.end())
	{
		str_SleepTime = paramMap[std::string("XMLOnSleepTime")];
		m_i_OnSleepTime = CStr::StrToInt(str_SleepTime);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter OnSleepTime for CMMIBB_AudioLoopRetryTest");
		return false;
	}

	if (paramMap.find(std::string("XMLOffSleepTime")) != paramMap.end())
	{
		str_SleepTime = paramMap[std::string("XMLOffSleepTime")];
		m_i_OffSleepTime = CStr::StrToInt(str_SleepTime);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter OffSleepTime for CMMIBB_AudioLoopRetryTest");
		return false;
	}

	std::string str_MexRetryTime;
	if (paramMap.find(std::string("XMLRetryTime")) != paramMap.end())
	{
		str_MexRetryTime = paramMap[std::string("XMLRetryTime")];
		m_i_MexRetryTime = CStr::StrToInt(str_MexRetryTime);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter RetryTime for CMMIBB_RGBVibratorRetryTest");
		return false;
	}


	return true;
}


bool CMMIBB_RGBVibratorRetryTest::Run()
{
	bool passFail = false;
	CString cs_TestItem = m_str_TestItem.c_str();

	if (cs_TestItem.Compare("CameraLedTest") == 0)
	{
		m_strErrorCode = FunErr_Flash_LED_Test_Fail;
		passFail = runLoop_CameraLed();
	}
	else if (cs_TestItem.Compare("RedLEDTest") == 0)
	{
		m_strErrorCode = FunErr_Check_Indicator_LED_R_Fail;
		passFail = runLoop_RGBLED_Test(1);
	}
	else if (cs_TestItem.Compare("GreenLEDTest") == 0)
	{
		m_strErrorCode = FunErr_Check_Indicator_LED_G_Fail;
		passFail = runLoop_RGBLED_Test(2);
	}
	else if (cs_TestItem.Compare("BlueLEDTest") == 0)
	{
		m_strErrorCode = FunErr_Check_Indicator_LED_B_Fail;
		passFail = runLoop_RGBLED_Test(3);
	}
	else if (cs_TestItem.Compare("CapKeyLEDTest") == 0)
	{
		m_strErrorCode = FunErr_CapKey_LED_Test_Fail;
		passFail = runLoop_RGBLED_Test(4);


	}
	else if (cs_TestItem.Compare("RedPatternTest") == 0)
	{
		m_strErrorCode = FunErr_LCD_Display_Fail;
		passFail = runLoop_RGBPattern_Test(1);
	}
	else if (cs_TestItem.Compare("GreenPatternTest") == 0)
	{
		m_strErrorCode = FunErr_LCD_Display_Fail;
		passFail = runLoop_RGBPattern_Test(2);
	}
	else if (cs_TestItem.Compare("BluePatternTest") == 0)
	{
		m_strErrorCode = FunErr_LCD_Display_Fail;
		passFail = runLoop_RGBPattern_Test(3);
	}
	else if (cs_TestItem.Compare("WhitePatternTest") == 0)
	{
		m_strErrorCode = FunErr_LCD_Display_Fail;
		passFail = runLoop_RGBPattern_Test(4);


	}
	else if (cs_TestItem.Compare("BackLightTest") == 0)
	{
		m_strErrorCode = FunErr_LCD_Backlight_Test_Fail;
		passFail = runLoop_BackLight_Test();

	}
	else if (cs_TestItem.Compare("VibratorTest") == 0)
	{
		m_strErrorCode = FunErr_Vibrator_Test_Fail;
		passFail = runLoop_Vibrator_Test();
	}
	else
	{
		AfxMessageBox(_T("Can't find Test Item in CMMIBB_RGBVibratorRetryTest"));
		TraceLog(MSG_INFO, "Can't find Test Item in CMMIBB_RGBVibratorRetryTest");
	}
	return passFail;
}

bool CMMIBB_RGBVibratorRetryTest::runLoop_Vibrator_Test()
{
	m_i_retriedTime = 0;
	m_strItemCode = CStr::IntToStr(Vibrator_Vibrate_BaseItemCode);
	CString cs_TestTime;
	char sz_pattern[FTD_BUF_SIZE] = {0};
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	do
	{
		b_res_on = false;
		b_res_off = false;
		b_res_msg_try = false;
		b_res_msg_retry = false;

		memset(sz_outBuffer, 0, FTD_BUF_SIZE);
		memset(sz_pattern, 0, FTD_BUF_SIZE);
		strcpy(sz_pattern, m_str_CMD.c_str());
		b_res_on = m_pIPhone->FTD_VibRun(m_nFtdPort, m_nFtdTimeOut, sz_pattern, sz_outBuffer);//turn on
		Sleep(m_i_OnSleepTime);

		cs_TestTime.Format(_T("runLoop_Vibrator - TestTime = %d"), m_i_retriedTime);
		str_msg = cs_TestTime.GetString();
		TraceLog(MSG_INFO, str_msg);
		if (b_res_on)
		{
			str_msg = "runLoop_Vibrator CMD Pass";
		}
		else
		{
			str_msg = "runLoop_Vibrator CMD Fail";
			break;
		}
		TraceLog(MSG_INFO, str_msg);

		b_res_msg_try =  showMsg(CW2A(L"Phone is vibrating?\n 手机在振动??\n"));
		if (b_res_msg_try)
		{
			m_strErrorCode = "-";
			str_msg = "runLoop_Vibrator pass!";
			m_i_retriedTime = m_i_MexRetryTime + 1 ;
		}
		else
		{
			if (m_i_retriedTime < m_i_MexRetryTime)
			{
				b_res_msg_retry = showMsg(CW2A(L"please check usb cable\n 请确认USB线有接好?\n\n Retry?是否重测?\n "));
				if (!b_res_msg_retry) m_i_retriedTime = m_i_MexRetryTime + 1 ;
			}
			str_msg = "runLoop_Vibrator fail!";
			m_i_retriedTime += 1;
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);


		strcpy(sz_pattern, m_str_OffCMD.c_str());
		b_res_off = m_pIPhone->FTD_VibRun(m_nFtdPort, m_nFtdTimeOut, sz_pattern, sz_outBuffer);//turn on
		Sleep(m_i_OffSleepTime);

		if (b_res_off)
		{
			str_msg = "runLoop_Vibrator Cmd pass!";
		}
		else
		{
			str_msg = "runLoop_Vibrator Cmd fail!";
			break;
		}
		TraceLog(MSG_INFO, str_msg);
	} while (m_i_retriedTime <= m_i_MexRetryTime);
	FactoryLog();
	if ((!b_res_msg_try) || (!b_res_off)) /*fail*/
	{
		SetPICSData("VIBRATOR_TEST", "FAIL");
		return false;
	}
	SetPICSData("VIBRATOR_TEST", "PASS");
	return true;
}

bool CMMIBB_RGBVibratorRetryTest::runLoop_BackLight_Test()
{
	m_i_retriedTime = 0;
	m_strItemCode = CStr::IntToStr(Backlight_Test_BaseItemCode);
	CString cs_TestTime;
	char sz_pattern[FTD_BUF_SIZE] = {0};
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	do
	{
		b_res_on = false;
		b_res_off = false;
		b_res_msg_try = false;
		b_res_msg_retry = false;

		memset(sz_outBuffer, 0, FTD_BUF_SIZE);
		memset(sz_pattern, 0, FTD_BUF_SIZE);
		strcpy(sz_pattern, m_str_CMD.c_str());
		b_res_on = m_pIPhone->LED_BacklightOnOff(m_nFtdPort, m_nFtdTimeOut, sz_pattern, sz_outBuffer);//turn on
		Sleep(m_i_OnSleepTime);

		cs_TestTime.Format(_T("runLoop_BackLight - TestTime = %d"), m_i_retriedTime);
		str_msg = cs_TestTime.GetString();
		TraceLog(MSG_INFO, str_msg);


		if (b_res_on)
		{
			str_msg = "runLoop_BackLight Pass";
		}
		else
		{
			str_msg = "runLoop_BackLight Fail";
			break;
		}
		TraceLog(MSG_INFO, str_msg);

		b_res_msg_try =  showMsg(CW2A(L"Is mobile backlight on?\n 手机背光变亮了吗?\n"));
		if (b_res_msg_try)
		{
			m_strErrorCode = "-";
			str_msg = "runLoop_BackLight pass!";
			m_i_retriedTime = m_i_MexRetryTime + 1 ;
		}
		else
		{
			if (m_i_retriedTime < m_i_MexRetryTime)
			{
				b_res_msg_retry = showMsg(CW2A(L"please check usb cable\n 请确认USB线有接好?\n\n Retry?是否重测?\n "));
				if (!b_res_msg_retry) m_i_retriedTime = m_i_MexRetryTime + 1 ;
			}
			str_msg = "runLoop_BackLight fail!";
			m_i_retriedTime += 1;
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);

		strcpy(sz_pattern, m_str_OffCMD.c_str());
		b_res_off = m_pIPhone->LED_BacklightOnOff(m_nFtdPort, m_nFtdTimeOut, sz_pattern, sz_outBuffer);//turn on
		Sleep(m_i_OffSleepTime);

		if (b_res_off)
		{
			str_msg = "runLoop_BackLightOff Cmd pass!";
		}
		else
		{
			str_msg = "runLoop_BackLightOff Cmd fail!";

			break;
		}
		TraceLog(MSG_INFO, str_msg);
	} while (m_i_retriedTime <= m_i_MexRetryTime);
	FactoryLog();
	if ((!b_res_msg_try) || (!b_res_off)) /*fail*/
	{
		SetPICSData("BACKLIGHT_ON_OFF_TEST", "FAIL");
		return false;
	}
	SetPICSData("BACKLIGHT_ON_OFF_TEST", "PASS");
	return true;
}

bool CMMIBB_RGBVibratorRetryTest::runLoop_RGBPattern_Test(int n_Case_RGB)
{
	m_i_retriedTime = 0;
	std::string msg;
	CString cs_TestTime;
	std::string str_Pics ;
	std::string str_CMD_Msg_Pass, str_CMD_Msg_Fail, str_Result_Msg_Pass, str_Result_Msg_Fail;
	m_strItemCode = CStr::IntToStr(LCD_Test_BaseItemCode);

	switch (n_Case_RGB)
	{
	case 1:
		msg = CW2A(L"Is this pattern red?\n 手机上显示的画面是红色吗?\n");
		str_Pics = "RED_PATTERN_TEST";
		str_CMD_Msg_Pass = "runLoop_RGBPatern_Test_Command RED Pass";
		str_CMD_Msg_Fail = "runLoop_RGBPatern_Test_Command RED Fail";
		str_Result_Msg_Pass = "runLoop_RGBPatern_Test_Result RED Pass";
		str_Result_Msg_Fail = "runLoop_RGBPatern_Test_Result RED Fail";
		break;
	case 2:
		msg = CW2A(L"Is this pattern Green?\n 手机上显示的画面是绿色吗?\n");
		str_Pics = "GREEN_PATTERN_TEST";
		str_CMD_Msg_Pass = "runLoop_RGBPatern_Test_Command Green Pass";
		str_CMD_Msg_Fail = "runLoop_RGBPatern_Test_Command Green Fail";
		str_Result_Msg_Pass = "runLoop_RGBPatern_Test_Result Green Pass";
		str_Result_Msg_Fail = "runLoop_RGBPatern_Test_Result Green Fail";
		break;
	case 3:
		msg = CW2A(L"Is this pattern Blue?\n 手机上显示的画面是蓝色吗?\n");
		str_Pics = "BLUE_PATTERN_TEST";
		str_CMD_Msg_Pass = "runLoop_RGBPatern_Test_Command Blue Pass";
		str_CMD_Msg_Fail = "runLoop_RGBPatern_Test_Command Blue Fail";
		str_Result_Msg_Pass = "runLoop_RGBPatern_Test_Result Blue Pass";
		str_Result_Msg_Fail = "runLoop_RGBPatern_Test_Result Blue Fail";
		break;
	case 4:
		msg = CW2A(L"Is this pattern White?\n 手机上显示的画面是白色吗?\n");
		str_Pics = "WHITE_PATTERN_TEST";
		str_CMD_Msg_Pass = "runLoop_RGBPatern_Test_Command White Pass";
		str_CMD_Msg_Fail = "runLoop_RGBPatern_Test_Command White Fail";
		str_Result_Msg_Pass = "runLoop_RGBPatern_Test_Result White Pass";
		str_Result_Msg_Fail = "runLoop_RGBPatern_Test_Result White Fail";
		break;
	}

	char sz_pattern[FTD_BUF_SIZE] = {0};
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	do
	{
		b_res_on = false;
		b_res_off = false;
		b_res_msg_try = false;
		b_res_msg_retry = false;

		memset(sz_outBuffer, 0, FTD_BUF_SIZE);
		memset(sz_pattern, 0, FTD_BUF_SIZE);
		strcpy(sz_pattern, m_str_CMD.c_str());
		b_res_on = m_pIPhone->Display_LCD_RGB(m_nFtdPort, m_nFtdTimeOut, sz_pattern, sz_outBuffer);//turn on
		Sleep(m_i_OnSleepTime);
		cs_TestTime.Format(_T("runLoop_RGBLED_Test - TestTime = %d"), m_i_retriedTime);
		str_msg = cs_TestTime.GetString();
		TraceLog(MSG_INFO, str_msg);
		if (b_res_on)
		{
			str_msg = str_CMD_Msg_Pass;
		}
		else
		{
			str_msg = str_CMD_Msg_Fail;
			break;
		}
		TraceLog(MSG_INFO, str_msg);

		b_res_msg_try = showMsg(msg.c_str());
		if (b_res_msg_try)
		{
			m_strErrorCode = "-";
			str_msg = str_Result_Msg_Pass;
			m_i_retriedTime = m_i_MexRetryTime + 1 ;
		}
		else
		{
			if (m_i_retriedTime < m_i_MexRetryTime)
			{
				b_res_msg_retry = showMsg(CW2A(L"please check usb cable\n 请确认USB线有接好?\n\n Retry?是否重测?\n "));
				if (!b_res_msg_retry) m_i_retriedTime = m_i_MexRetryTime + 1 ;
			}
			str_msg = str_Result_Msg_Fail;
			m_i_retriedTime += 1;
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);
		b_res_off = true;//no need to have off command
	} while (m_i_retriedTime <= m_i_MexRetryTime);
	FactoryLog();
	if ((!b_res_msg_try) || (!b_res_off)) /*fail*/
	{
		SetPICSData(str_Pics, "FAIL");
		return false;
	}
	SetPICSData(str_Pics , "PASS");
	return true;
}

bool CMMIBB_RGBVibratorRetryTest::runLoop_RGBLED_Test(int n_Case_RGB)
{
	m_i_retriedTime = 0;
	std::string msg;
	CString cs_TestTime;
	std::string str_Pics ;
	std::string str_CMD_Msg_Pass, str_CMD_Msg_Fail, str_Result_Msg_Pass, str_Result_Msg_Fail;
	m_strItemCode = CStr::IntToStr(Flash_LED_BaseItemCode);
	switch (n_Case_RGB)
	{
	case 1:
		msg = CW2A(L"Is there Red light on the phone ?\n 手机上方的红灯有亮吗?\n");
		str_Pics = "IndicatorLED_TEST_R";
		str_CMD_Msg_Pass = "runLoop_RGBLED_Test_Command RED Pass";
		str_CMD_Msg_Fail = "runLoop_RGBLED_Test_Command RED Fail";
		str_Result_Msg_Pass = "runLoop_RGBLED_Test_Result RED Pass";
		str_Result_Msg_Fail = "runLoop_RGBLED_Test_Result RED Fail";
		break;
	case 2:
		msg = CW2A(L"Is there Green light on the phone?\n 手机上方的绿灯有亮吗?\n");
		str_Pics = "IndicatorLED_TEST_G";
		str_CMD_Msg_Pass = "runLoop_RGBLED_Test_Command Green Pass";
		str_CMD_Msg_Fail = "runLoop_RGBLED_Test_Command Green Fail";
		str_Result_Msg_Pass = "runLoop_RGBLED_Test_Result Green Pass";
		str_Result_Msg_Fail = "runLoop_RGBLED_Test_Result Green Fail";
		break;
	case 3:
		msg = CW2A(L"Is there Bule Light on phone?\n 手机上方的蓝灯有亮吗?\n");
		str_Pics = "IndicatorLED_TEST_B";
		str_CMD_Msg_Pass = "runLoop_RGBLED_Test_Command Blue Pass";
		str_CMD_Msg_Fail = "runLoop_RGBLED_Test_Command Blue Fail";
		str_Result_Msg_Pass = "runLoop_RGBLED_Test_Result Blue Pass";
		str_Result_Msg_Fail = "runLoop_RGBLED_Test_Result Blue Fail";
		break;
	case 4:
		msg =  CW2A(L"Is there Light on phone?\n 手机下方的灯有亮吗?\n");
		str_Pics = "CapKey_LED";
		str_CMD_Msg_Pass = "runLoop_RGBLED_Test_Command CapKey_LED Pass";
		str_CMD_Msg_Fail = "runLoop_RGBLED_Test_Command CapKey_LED Fail";
		str_Result_Msg_Pass = "runLoop_RGBLED_Test_Result CapKey_LED Pass";
		str_Result_Msg_Fail = "runLoop_RGBLED_Test_Result CapKey_LED Fail";
	}

	char sz_pattern[FTD_BUF_SIZE] = {0};
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	do
	{
		b_res_on = false;
		b_res_off = false;
		b_res_msg_try = false;
		b_res_msg_retry = false;

		memset(sz_outBuffer, 0, FTD_BUF_SIZE);
		memset(sz_pattern, 0, FTD_BUF_SIZE);
		strcpy(sz_pattern, m_str_CMD.c_str());
		b_res_on = m_pIPhone->FTD_LEDon(m_nFtdPort, m_nFtdTimeOut, sz_pattern, sz_outBuffer);//turn on
		Sleep(m_i_OnSleepTime);
		cs_TestTime.Format(_T("runLoop_RGBLED_Test - TestTime = %d"), m_i_retriedTime);
		str_msg = cs_TestTime.GetString();
		TraceLog(MSG_INFO, str_msg);
		if (b_res_on)
		{
			str_msg = str_CMD_Msg_Pass;
		}
		else
		{
			str_msg = str_CMD_Msg_Fail;
			break;
		}
		TraceLog(MSG_INFO, str_msg);

		b_res_msg_try =  showMsg(msg.c_str());
		if (b_res_msg_try)
		{
			m_strErrorCode = "-";
			str_msg = str_Result_Msg_Pass;
			m_i_retriedTime = m_i_MexRetryTime + 1 ;
		}
		else
		{
			if (m_i_retriedTime < m_i_MexRetryTime)
			{
				b_res_msg_retry = showMsg(CW2A(L"please check usb cable\n 请确认USB线有接好?\n\n Retry?是否重测?\n "));
				if (!b_res_msg_retry) m_i_retriedTime = m_i_MexRetryTime + 1 ;
			}
			str_msg = str_Result_Msg_Fail;
			m_i_retriedTime += 1;
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);

		strcpy(sz_pattern, m_str_OffCMD.c_str());
		b_res_off = m_pIPhone->FTD_LEDon(m_nFtdPort, m_nFtdTimeOut, sz_pattern, sz_outBuffer);//turn on
		Sleep(m_i_OffSleepTime);
		if (b_res_off)
		{
			str_msg = str_CMD_Msg_Fail;
		}
		else
		{
			str_msg = str_CMD_Msg_Pass;
			break;
		}
		TraceLog(MSG_INFO, str_msg);

	} while (m_i_retriedTime <= m_i_MexRetryTime);
	FactoryLog();
	if ((!b_res_msg_try) || (!b_res_off)) /*fail*/
	{
		SetPICSData(str_Pics, "FAIL");
		return false;
	}
	SetPICSData(str_Pics , "PASS");
	return true;
}

bool CMMIBB_RGBVibratorRetryTest::runLoop_CameraLed()
{
	m_i_retriedTime = 0;
	CString cs_TestTime;
	char sz_pattern[FTD_BUF_SIZE] = {0};
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	do
	{
		b_res_on = false;
		b_res_off = false;
		b_res_msg_try = false;
		b_res_msg_retry = false;

		memset(sz_outBuffer, 0, FTD_BUF_SIZE);
		memset(sz_pattern, 0, FTD_BUF_SIZE);
		strcpy(sz_pattern, m_str_CMD.c_str());
		b_res_on = m_pIPhone->FTD_CAMFlashLED(m_nFtdPort, m_nFtdTimeOut, sz_pattern, sz_outBuffer);//turn on
		Sleep(m_i_OnSleepTime);

		cs_TestTime.Format(_T("runLoop_CameraLed - TestTime = %d"), m_i_retriedTime);
		str_msg = cs_TestTime.GetString();
		TraceLog(MSG_INFO, str_msg);
		if (b_res_on)
		{
			str_msg = "runLoop_CameraLedOn Pass";
		}
		else
		{
			str_msg = "runLoop_CameraLedOn Fail";
			break;
		}
		TraceLog(MSG_INFO, str_msg);

		b_res_msg_try =  showMsg(CW2A(L"Whether there is light on phone?\n手机背面的灯有亮吗??\n"));
		if (b_res_msg_try)
		{
			m_strErrorCode = "-";
			str_msg = "runLoop_CameraLedOn pass!";
			m_i_retriedTime = m_i_MexRetryTime + 1 ;
		}
		else
		{
			if (m_i_retriedTime < m_i_MexRetryTime)
			{
				b_res_msg_retry = showMsg(CW2A(L"please check usb cable\n 请确认USB线有接好?\n\n Retry?是否重测?\n "));
				if (!b_res_msg_retry) m_i_retriedTime = m_i_MexRetryTime + 1 ;
			}
			str_msg = "runLoop_CameraLedOn fail!";
			m_i_retriedTime += 1;
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);

		strcpy(sz_pattern, m_str_OffCMD.c_str());
		b_res_off = m_pIPhone->FTD_CAMFlashLED(m_nFtdPort, m_nFtdTimeOut, sz_pattern, sz_outBuffer);//turn on
		Sleep(m_i_OffSleepTime);

		if (b_res_off)
		{
			str_msg = "runLoop_CameraLedOff Cmd pass!";
		}
		else
		{
			str_msg = "runLoop_CameraLedOff Cmd fail!";
			break;
		}
		TraceLog(MSG_INFO, str_msg);
	} while (m_i_retriedTime <= m_i_MexRetryTime);
	FactoryLog();
	if ((!b_res_msg_try) || (!b_res_off)) /*fail*/
	{
		SetPICSData("Camera_LED", "FAIL");
		return false;
	}
	SetPICSData("Camera_LED", "PASS");
	return true;
}

bool CMMIBB_RGBVibratorRetryTest::showMsg(const char* szMsg)
{
	/* Init dialog response flag to false */
	m_b_DialogResponse = false;
	m_b_waitFlag = false;

	/* Notify UI to show message dialog */
	ShowDialog(szMsg);

	/* Wait UI response(Max:10s) */
	int i_waitCount = 0;
	while (m_b_waitFlag != true)
	{
		if (i_waitCount == 50)
		{
			str_msg = "Wait UI response time out.";
			TraceLog(MSG_INFO, "Wait UI response time out.");
			break;
		}
		i_waitCount++;
		Sleep(200);
	}
	/* Get UI dialog operator response */
	if (m_b_DialogResponse == true)
	{
		str_msg = "Pressed PASS";
		TraceLog(MSG_INFO, str_msg);
	}
	else
	{
		str_msg = "Pressed FAIL";
		TraceLog(MSG_INFO, str_msg);
	}
	return m_b_DialogResponse;
}
