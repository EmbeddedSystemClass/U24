/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CDisplay object.
 * FileName   : LCDDisplayColor.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2012/04/12
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/04/12    Evonne.Hsieh            Create for Detroit project MMI test
 ********************************************************************************/

#include "stdafx.h"
#include "LCDDisplayColor.h"


RF_IMPLEMENT_DYNCREATE(CDisplay)


bool CDisplay::PreRun()
{
	m_strResult = "-";
	m_strItemCode = CStr::IntToStr(LCD_Test_BaseItemCode);
	m_strErrorCode = FunErr_LCD_Display_Fail;
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool CDisplay::Run()
{
	return LCDDisplayTest();
}

bool CDisplay::PostRun()
{
	return true;
}

bool CDisplay::InitData(std::map<std::string, std::string>& paramMap)
{
	/* PatternIndex */
	if (paramMap.find(std::string("Color")) != paramMap.end())
	{
		m_str_patternIndex = paramMap[std::string("Color")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter PatternIndex for CDisplay");
		return false;
	}
	return true;
}

bool CDisplay::LCDDisplayTest()
{
	bool b_res = true;
	std::string str_pattern_English;
	std::string str_pattern_Chinese;
	char sz_msg[MAX_PATH] = {0};
	std::string str_msg;
	std::string str_Pics;

	/* Get LCD display pattern */
	if (m_str_patternIndex.compare("r") == 0)
	{
		str_pattern_English = "red";
		str_pattern_Chinese = CW2A(L"红色");
		str_Pics = "RED_PATTERN_TEST";
	}
	else if (m_str_patternIndex.compare("g") == 0)
	{
		str_pattern_English = "Green";
		str_pattern_Chinese = CW2A(L"绿色");
		str_Pics = "GREEN_PATTERN_TEST";
	}
	else if (m_str_patternIndex.compare("b") == 0)
	{
		str_pattern_English = "blue";
		str_pattern_Chinese = CW2A(L"蓝色");
		str_Pics = "BLUE_PATTERN_TEST";
	}
	else if (m_str_patternIndex.compare("w") == 0)
	{
		str_pattern_English = "white";
		str_pattern_Chinese = CW2A(L"白色");
		str_Pics = "WHITE_PATTERN_TEST";
	}
	else if (m_str_patternIndex.compare("d") == 0)
	{
		str_pattern_English = "black";
		str_pattern_Chinese = CW2A(L"黑色");
		str_Pics = "BLACK_PATTERN_TEST";
	}
	else
	{
		str_msg = "required LCD pattern is not defined.";
		TraceLog(MSG_INFO,str_msg);
		b_res = false;
	}

	/* Display required pattern on mobile LCD */
	if (b_res)
	{
		char sz_patternColor[4] = {0};
		strcpy(sz_patternColor, m_str_patternIndex.c_str());
		char sz_outBuffer[FTD_BUF_SIZE] = {0};
		b_res = m_pIPhone->Display_LCD_RGB(m_nFtdPort, m_nFtdTimeOut, sz_patternColor, sz_outBuffer);
		if (!b_res)
		{
			str_msg = "Fail to set mobile LCD required pattern";
			TraceLog(MSG_INFO,str_msg);
		}
		else
		{
			str_msg = "Mobile LCD display required pattern success";
			TraceLog(MSG_INFO,str_msg);
		}
	}

	/* Query operator if display pattern is required pattern */
	if (b_res)
	{
		/* Init dialog response flag to false */
		m_b_DialogResponse = false;
		m_b_waitFlag = false;

		/* Notify UI to show message dialog */
		str_msg = "Is this pattern " + str_pattern_English + "?\n" +
			std::string(CW2A(L"手机上显示的画面是")) + str_pattern_Chinese + std::string(CW2A(L"吗？"));
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
				str_msg = "The displayed pattern is required pattern.";
				TraceLog(MSG_INFO, str_msg);
			}
			else
			{
				b_res = false;
				str_msg = "The dispalyed pattern is not required pattern.";
				TraceLog(MSG_INFO, str_msg);
			}
		}
	}

	/* test result */
	if(b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		str_msg = "LCD display " + str_pattern_English + " pattern pass!";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);
		SetPICSData(str_Pics, "PASS");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		str_msg = "LCD display " + str_pattern_English + " pattern fail!";
		TraceLog(MSG_INFO, str_msg);
		SetPICSData(str_Pics, "FAIL");
	}
	FactoryLog();
	return b_res;
}
