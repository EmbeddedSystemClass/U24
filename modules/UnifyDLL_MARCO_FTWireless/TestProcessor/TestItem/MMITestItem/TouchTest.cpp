/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CTouchTest object.
 * FileName   : TouchTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2012/04/12
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/04/12    Evonne.Hsieh            Create for Detroit project MMI test
 ********************************************************************************/

#include "stdafx.h"
#include "TouchTest.h"


RF_IMPLEMENT_DYNCREATE(CTouchTest)


bool CTouchTest::PreRun()
{
	m_strResult = "-";
	m_strItemCode = CStr::IntToStr(TOUCH_Test_BaseItemCode);
	m_strErrorCode = FunErr_Touch_General_Fail;
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool CTouchTest::Run()
{
	return TouchTest();
}

bool CTouchTest::PostRun()
{
	bool b_res = true;
	if (!m_pIPhone->ExactCloseDevice(m_nFtdPort, m_nFtdTimeOut))
	{
		m_strMessage = "Device Close Command fail!";
		TraceLog(MSG_ERROR, "Device Close Command fail!");
		b_res = false;
	}
	else
	{
		TraceLog(MSG_INFO, "Device Close Command Succeed.");
	}
	return b_res;
}

bool CTouchTest::InitData(std::map<std::string, std::string>& paramMap)
{
	std::string strTimeOut;
	if (paramMap.find(std::string("TimeOut")) != paramMap.end())
	{
		strTimeOut = paramMap[std::string("TimeOut")];
		TimeOut = CStr::StrToInt(strTimeOut);
	}
	else
	{
		TraceLog(MSG_ERROR, "Failed to Get TimeOut for class CTouchTest");
		return false;
	}

	return true;
}

bool CTouchTest::TouchTest()
{
	/* Init dialog response flag to false */
	m_b_DialogResponse = false;
	m_b_waitFlag = false;

	/* Notify UI to show message dialog */
	std::string str_msg = CW2A(L"Please touch panel\n请按手机屏幕开始测试");
	ShowDialog(str_msg.c_str());

	/* Wait UI response(Max:10s) */
	bool b_res = true;
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

	bool b_StopTouchTest = false;
	if (b_res)
	{
		/* Get UI dialog operator response */
		if (m_b_DialogResponse == true)
		{
			str_msg = "Start touch test.";
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			b_res = false;
			str_msg = "Stop touch test.";
			TraceLog(MSG_INFO, str_msg);
		}
	}
	if (!(b_res = m_pIPhone->ExactOpenDevice(m_nFtdPort, m_nFtdTimeOut)))
	{
		m_strMessage = "Device Open Command fail!";
		TraceLog(MSG_ERROR, "Device Open Command fail!");
		b_res = false;
	}
	else
	{
		TraceLog(MSG_INFO, "Device Open Command Succeed.");
	}

	char szOutput[FTD_BUF_SIZE] = {0};
	if (b_res)
	{
		if (!(b_res = m_pIPhone->TouchPanelTest(m_nFtdPort, m_nFtdTimeOut, 1, szOutput)))
			TraceLog(MSG_INFO, "Touch test command fail");
		else
			TraceLog(MSG_INFO, "Touch test command success");
	}

	clock_t start = clock(); // Count start time
	clock_t finish;
	char command[4] = {4, 2, 0, 0}; //NV_DT_TOUCH
	if (b_res)
	{
		while (1)
		{
			if (m_pIPhone->ReceiveCommandData(m_nFtdPort, m_nFtdTimeOut, command, szOutput))
			{
				TraceLog(MSG_INFO, szOutput);
				b_res = true;
				break;
			}

			finish = clock();
			if (((finish - start) / CLK_TCK) > TimeOut)
			{
				b_StopTouchTest = true;
				break;
			}
		}

		//stop Touch Test
		if (b_StopTouchTest)
		{
			m_strMessage = "Touch Panel Test time out!";
			TraceLog(MSG_INFO, "Touch Panel Test time out!");
			b_res = false;
		}
	}

	if (!m_pIPhone->TouchPanelTest(m_nFtdPort, 1000, false, szOutput))
		TraceLog(MSG_ERROR, "Touch Panel Test Stop Command fail!");
	else
		TraceLog(MSG_INFO, "Touch Panel Test Stop Command Succeed.");

	if (!m_pIPhone->ExactCloseDevice(m_nFtdPort, m_nFtdTimeOut))
	{
		m_strMessage = "Device Open Command fail!";
		TraceLog(MSG_ERROR, "Device Open Command fail!");
		b_res = false;
	}
	else
	{
		TraceLog(MSG_INFO, "Device Open Command Succeed.");
	}

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "Touch test pass!";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("TOUCH_TEST", "PASS");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "Touch test fail!";
		TraceLog(MSG_INFO, "Touch test fail!");
		SetPICSData("TOUCH_TEST", "FAIL");
	}
	FactoryLog();
	return b_res;
}
