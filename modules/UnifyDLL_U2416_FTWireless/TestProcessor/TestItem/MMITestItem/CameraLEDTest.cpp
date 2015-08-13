/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CCameraLEDTest object.
 * FileName   : CameraLEDTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Lion.Wu
 * Time       : 2012/1210
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/1210    Lion.Wu            Create for Detroit project MMI test
 ********************************************************************************/

#include "stdafx.h"
#include "CameraLEDTest.h"


RF_IMPLEMENT_DYNCREATE(CCameraLEDTest)


bool CCameraLEDTest::PreRun()
{
	m_strResult = "-";
	m_strItemCode = CStr::IntToStr(Flash_LED_BaseItemCode);
	m_strErrorCode = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool CCameraLEDTest::Run()
{
	return CameraLEDTest();
}

bool CCameraLEDTest::PostRun()
{
	return true;
}

bool CCameraLEDTest::InitData(std::map<std::string, std::string>& paramMap)
{
	/* PatternIndex */
	if (paramMap.find(std::string("OnOff")) != paramMap.end())
	{
		m_str_OnOff = paramMap[std::string("OnOff")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter OnOff for CCameraLEDTest");
		return false;
	}
	return true;
}

bool CCameraLEDTest::CameraLEDTest()
{
	bool b_res = true;
	std::string str_msg;
	std::string str_Pics;

	if (b_res)
	{
		int nOnOff = atoi(m_str_OnOff.c_str());
		char sz_patternCameraLLED[FTD_BUF_SIZE] = {0};
		strcpy(sz_patternCameraLLED, m_str_OnOff.c_str());
		char sz_outBuffer[FTD_BUF_SIZE] = {0};
		b_res = m_pIPhone->FTD_CAMFlashLED(m_nFtdPort, m_nFtdTimeOut, sz_patternCameraLLED, sz_outBuffer);
		if (!b_res)
		{
			str_msg = "Detroit_CameraLED command fail";
			TraceLog(MSG_INFO,str_msg);
		}
		else
		{
			str_msg = "Detroit_CameraLED command success";
			TraceLog(MSG_INFO,str_msg);
		}
	}

	/* Query operator if display pattern is required pattern */
	if (b_res)
	{
		if (m_str_OnOff.find("1") != string::npos)
		{
			str_msg = CW2A(L"Whether there is light on phone?\n手机背面的灯有亮吗??\n");
				str_Pics = "HEADMIC_TO_HEADSET_ON_TEST";

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
					str_msg = _T("Wait UI response time out.");
					TraceLog(MSG_INFO, _T("Wait UI response time out."));
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
					str_msg = "CameraLED Test success.";
					TraceLog(MSG_INFO, str_msg);
				}
				else
				{
					b_res = false;
					str_msg = "CameraLED Test fail.";
					TraceLog(MSG_INFO, str_msg);
				}
			}
		}
	}

	/* test result */
	if(b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		str_msg = "CameraLED " + m_str_OnOff + " Test success!";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);
		SetPICSData(str_Pics, "PASS");
	}
	else
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Flash_LED_Test_Fail;
		m_strMessage = str_msg;
		str_msg = "CameraLED " + m_str_OnOff + " Test fail!";
		TraceLog(MSG_INFO, str_msg);
		SetPICSData(str_Pics, "FAIL");
	}
	FactoryLog();
	return b_res;
}
