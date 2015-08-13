/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CCapkeyLEDTest object.
 * FileName   : CapkeyLEDTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Lion.Wu
 * Time       : 2012/1210
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/1210    Lion.Wu            Create for Detroit project MMI test
 ********************************************************************************/

#include "stdafx.h"
#include "CapkeyLEDTest.h"


RF_IMPLEMENT_DYNCREATE(CCapkeyLEDTest)


bool CCapkeyLEDTest::PreRun()
{
	m_strResult = "-";
	m_strItemCode = CStr::IntToStr(Flash_LED_BaseItemCode);
	m_strErrorCode = FunErr_Touch_Capkey_Fail;
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool CCapkeyLEDTest::Run()
{
	return RedPattern_N_CapkeyLED_Test();
}

bool CCapkeyLEDTest::PostRun()
{
	return true;
}

bool CCapkeyLEDTest::InitData(std::map<std::string, std::string>& paramMap)
{
	/* PatternIndex */
	if (paramMap.find(std::string("OnOff")) != paramMap.end())
	{
		m_str_OnOff = paramMap[std::string("OnOff")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter OnOff for CCapkeyLEDTest");
		return false;
	}
	return true;
}

bool CCapkeyLEDTest::RedPattern_N_CapkeyLED_Test()
{
	bool b_res = true;
	bool showDiag = true;
	std::string str_msg;
	std::string str_Pics;

	/*turn capkey LED*/
	if (b_res)
	{
		int nOnOff = atoi(m_str_OnOff.c_str());
		char sz_patternCapkeyLED[FTD_BUF_SIZE] = {0};
		strcpy(sz_patternCapkeyLED, m_str_OnOff.c_str());
		char sz_outBuffer[FTD_BUF_SIZE] = {0};
		b_res = m_pIPhone->FTD_LEDon(m_nFtdPort, m_nFtdTimeOut, sz_patternCapkeyLED, sz_outBuffer);
		if (!b_res)
		{
			str_msg = "FTD_CapkeyLED command fail";
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			str_msg = "FTD_CapkeyLED command success";
			TraceLog(MSG_INFO, str_msg);
		}
	}
	/*turn red pattern*/
	if (b_res)
	{
		char sz_patternColor[4] = "r";
		char sz_outBuffer[FTD_BUF_SIZE] = {0};
		b_res = m_pIPhone->Display_LCD_RGB(m_nFtdPort, m_nFtdTimeOut, sz_patternColor, sz_outBuffer);
		if (!b_res)
		{
			str_msg = "Fail to set mobile LCD required pattern";
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			str_msg = "Mobile LCD display required pattern success";
			TraceLog(MSG_INFO, str_msg);
		}
	}

	/* Query operator if display pattern is required pattern */
	if (b_res)
	{
		if (m_str_OnOff.find("1,0,1") != string::npos)
		{
			str_msg = CW2A(L"Is this pattern Red and three light on the phone?\n 屏幕显示为红色嗎?.下方三个灯有亮吗?\n");
		}
		else if (m_str_OnOff.find("0,0,0") != string::npos)
		{
			showDiag = false;
		}
		else
		{
			b_res = false;
		}
		if (b_res)
		{
			if (showDiag)
			{
				str_Pics = "REDPATTERN_CAPKEYLED_TEST";

				/* Init dialog response flag to false */
				m_b_DialogResponse = false;
				m_b_waitFlag = false;

				/* Notify UI to show message dialog */
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
			}//shoDiag

			if (b_res)
			{
				/* Get UI dialog operator response */
				if (m_b_DialogResponse == true)
				{
					str_msg = "CapkeyLED Test success.";
					TraceLog(MSG_INFO, str_msg);
				}
				else
				{
					b_res = false;
					str_msg = "CapkeyLED Test fail.";
					TraceLog(MSG_INFO, str_msg);
				}
			}
		}
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		str_msg = "CapkeyLED " + m_str_OnOff + " Test success!";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);
		SetPICSData(str_Pics, "PASS");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		str_msg = "CapkeyLED " + m_str_OnOff + " Test fail!";
		TraceLog(MSG_INFO, str_msg);
		SetPICSData(str_Pics, "FAIL");
	}
	FactoryLog();
	return b_res;
}

bool CCapkeyLEDTest::CapkeyLEDTestTogether()
{
	bool b_res = true;
	bool showDiag = true;
	std::string str_msg;
	std::string str_Pics;

	if (b_res)
	{
		int nOnOff = atoi(m_str_OnOff.c_str());
		char sz_patternCapkeyLED[FTD_BUF_SIZE] = {0};
		strcpy(sz_patternCapkeyLED, m_str_OnOff.c_str());
		char sz_outBuffer[FTD_BUF_SIZE] = {0};
		b_res = m_pIPhone->FTD_LEDon(m_nFtdPort, m_nFtdTimeOut, sz_patternCapkeyLED, sz_outBuffer);
		if (!b_res)
		{
			str_msg = "FTD_CapkeyLED command fail";
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			str_msg = "FTD_CapkeyLED command success";
			TraceLog(MSG_INFO, str_msg);
		}

	}

	/* Query operator if display pattern is required pattern */
	if (b_res)
	{
		if (m_str_OnOff.find("1,0,1") != string::npos)
		{
			str_msg = CW2A(L"Is there three light on the phone?\n 手机正下方的三个灯有亮吗?\n");
		}
		else if (m_str_OnOff.find("0,0,0") != string::npos)
		{
			showDiag = false;
		}
		else
		{
			b_res = false;
		}
		if (b_res)
		{
			if (showDiag)
			{
				str_Pics = "CAPKEYLED_TEST";

				/* Init dialog response flag to false */
				m_b_DialogResponse = false;
				m_b_waitFlag = false;

				/* Notify UI to show message dialog */
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
					else
					{
						i_waitCount++;
						Sleep(200);
						continue;
					}
				}
			}//shoDiag

			if (b_res)
			{
				/* Get UI dialog operator response */
				if (m_b_DialogResponse == true)
				{
					str_msg = "CapkeyLED Test success.";
					TraceLog(MSG_INFO, str_msg);
				}
				else
				{
					b_res = false;
					str_msg = "CapkeyLED Test fail.";
					TraceLog(MSG_INFO, str_msg);
				}
			}
		}
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		str_msg = "CapkeyLED " + m_str_OnOff + " Test success!";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);
		SetPICSData(str_Pics, "PASS");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		str_msg = "CapkeyLED " + m_str_OnOff + " Test fail!";
		TraceLog(MSG_INFO, str_msg);
		SetPICSData(str_Pics, "FAIL");
	}
	FactoryLog();
	return b_res;
}

bool CCapkeyLEDTest::CapkeyLEDTest()
{
	bool b_res = true;
	bool showDiag = true;
	char sz_msg[MAX_PATH] = {0};
	std::string str_msg;
	std::string str_Pics;

	if (b_res)
	{
		int nOnOff = atoi(m_str_OnOff.c_str());
		char sz_patternCapkeyLED[FTD_BUF_SIZE] = {0};
		strcpy(sz_patternCapkeyLED, m_str_OnOff.c_str());
		char sz_outBuffer[FTD_BUF_SIZE] = {0};
		b_res = m_pIPhone->FTD_LEDon(m_nFtdPort, m_nFtdTimeOut, sz_patternCapkeyLED, sz_outBuffer);
		if (!b_res)
		{
			str_msg = "FTD_CapkeyLED command fail";
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			str_msg = "FTD_CapkeyLED command success";
			TraceLog(MSG_INFO, str_msg);
		}
	}

	/* Query operator if display pattern is required pattern */
	if (b_res)
	{
		if (m_str_OnOff.find("1,0,0") != string::npos)
		{
			str_msg = CW2A(L"Is there a mid light on the phone ?\n 手机正下方中间的灯有亮吗?\n");
		}
		else if (m_str_OnOff.find("0,0,1") != string::npos)
		{
			str_msg = CW2A(L"Is there two light on the phone?\n 手机正下方的左右两个灯有亮吗?\n");
		}
		else if (m_str_OnOff.find("0,0,1") != string::npos)
		{
			str_msg = CW2A(L"Whether there is Red Light on phone?\n手机正面的蓝灯有亮吗?\n");
		}
		else if (m_str_OnOff.find("0,0,0") != string::npos)
		{
			showDiag = false;
		}
		else
		{
			b_res = false;
		}

		if (b_res)
		{
			if (showDiag)
			{
				str_Pics = "CAPKEYLED_TEST";

				/* Init dialog response flag to false */
				m_b_DialogResponse = false;
				m_b_waitFlag = false;

				/* Notify UI to show message dialog */
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
			}//shoDiag

			if (b_res)
			{
				/* Get UI dialog operator response */
				if (m_b_DialogResponse == true)
				{
					str_msg = "CapkeyLED Test success.";
					TraceLog(MSG_INFO, str_msg);
				}
				else
				{
					b_res = false;
					str_msg = "CapkeyLED Test fail.";
					TraceLog(MSG_INFO, str_msg);
				}
			}
		}
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		str_msg = "CapkeyLED " + m_str_OnOff + " Test success!";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);
		SetPICSData(str_Pics, "PASS");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		str_msg = "CapkeyLED " + m_str_OnOff + " Test fail!";
		TraceLog(MSG_INFO, str_msg);
		SetPICSData(str_Pics, "FAIL");
	}
	FactoryLog();
	return b_res;
}
