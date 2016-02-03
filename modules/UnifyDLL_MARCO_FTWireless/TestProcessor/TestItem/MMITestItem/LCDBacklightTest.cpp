/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CBacklight object.
 * FileName   : LCDBacklight.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2012/04/12
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/04/12    Evonne.Hsieh            Create for Detroit project MMI test
 ********************************************************************************/

#include "stdafx.h"
#include "LCDBacklightTest.h"


RF_IMPLEMENT_DYNCREATE(CBacklight)


bool CBacklight::PreRun()
{
	m_strResult = "-";
	m_strItemCode = CStr::IntToStr(Backlight_Test_BaseItemCode);
	m_strErrorCode = FunErr_LCD_Backlight_Test_Fail;
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool CBacklight::Run()
{
	return BacklightOnOff();
}

bool CBacklight::PostRun()
{
	return true;
}

bool CBacklight::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find(std::string("LightLevel")) != paramMap.end())
	{
		m_str_lightLevel = paramMap[std::string("LightLevel")];
	}
	else
	{
		TraceLog(MSG_ERROR,_T("Failed to Get LightLevel for class CBacklight"));
		return false;
	}
	return true;
}

bool CBacklight::BacklightOnOff()
{
	bool b_res = false;
	std::string str_msg;

	/* First turn off Backlight */
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->LED_BacklightOnOff(m_nFtdPort, m_nFtdTimeOut, "0", sz_outBuffer)))
	{
		str_msg = "Fail to turn off mobile backlight.";
		TraceLog(MSG_INFO, str_msg);
	}

	/* Backlight On test */
	if (b_res)
	{
		Sleep(100);
		char sz_lightLevel[32] = {0};
		strcpy(sz_lightLevel, m_str_lightLevel.c_str());
		if ((b_res = m_pIPhone->LED_BacklightOnOff(m_nFtdPort, m_nFtdTimeOut, sz_lightLevel, sz_outBuffer)))
		{
			str_msg = "Turn on mobile backlight FTD comamnd success";
			TraceLog(MSG_INFO, str_msg);

			/* Init dialog response flag to false */
			m_b_DialogResponse = false;
			m_b_waitFlag = false;

			/* Notify UI to show message dialog */
			str_msg = CW2A(L"Is mobile backlight on?\n手机背光亮了吗？");
			ShowDialog(str_msg.c_str());

			/* Wait UI response(Max:10s) */
			int i_waitCount = 0;
			while (m_b_waitFlag != true)
			{
				if (i_waitCount == 50)
				{
					str_msg = "Wait UI response time out.";
					TraceLog(MSG_INFO, "Wait UI response time out.");
					b_res = false;
					break;
				}
				i_waitCount++;
				Sleep(200);
			}

			if (b_res)
			{
				/* Get UI dialog operator response */
				if (m_b_DialogResponse == true)
				{
					str_msg = "Operator judge Result:Turn on mobile backlight success.";
					TraceLog(MSG_INFO, str_msg);
				}
				else
				{
					b_res = false;
					str_msg = "Operator judge Result:Fail to turn on mobile backlight.";
					TraceLog(MSG_INFO, str_msg);
				}
			}
		}
		else
		{
			str_msg = "Fail to turn on mobile backlight.";
			TraceLog(MSG_INFO, str_msg);
		}
	}

	/* test result */
	if(b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "Backlight on/off test pass!";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("BACKLIGHT_ON_OFF_TEST", "PASS");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Backlight on/off test fail!");
		SetPICSData("BACKLIGHT_ON_OFF_TEST", "FAIL");
	}
	FactoryLog();
	return b_res;
}
