/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CIF_LEDTest object.
 * FileName   : IndicatorLEDTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Lion.Wu

 ********************************************************************************/

#include "stdafx.h"
#include "IF_LEDTest.h"


RF_IMPLEMENT_DYNCREATE(CIF_LEDTest)


bool CIF_LEDTest::PreRun()
{
	m_strResult = "-";
	m_strItemCode = CStr::IntToStr(Flash_LED_BaseItemCode);
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMessage = "";

	return true;
}

bool CIF_LEDTest::Run()
{
	return RGBLED_Test();
}

bool CIF_LEDTest::PostRun()
{
	return true;
}

bool CIF_LEDTest::InitData(std::map<std::string, std::string>& paramMap)
{
	/* PatternIndex */
	if (paramMap.find(std::string("OnOff")) != paramMap.end())
	{
		m_str_OnOff = paramMap[std::string("OnOff")];	
	}
	else
	{
		TraceLog(MSG_INFO, _T("Fail to find parameter OnOff for CIF_LEDTest"));
		return false;
	}

	if (paramMap.find(std::string("Func")) != paramMap.end())
	{
		m_str_Func = paramMap[std::string("Func")];	
	}
	else
	{
		TraceLog(MSG_INFO, _T("Fail to find parameter m_str_Func for CIF_LEDTest"));
		return false;
	}
	
	return true;
}


bool CIF_LEDTest::RGBLED_Test()
{
	bool b_res = true;
	bool showDiag = true;
	char sz_msg[MAX_PATH] = {0};
	std::string str_msg, str_msg2;
	std::string str_Pics;


	/* Query operator if display pattern is required pattern */
	if (m_str_Func.find("R") != string::npos){
		m_strErrorCode = FunErr_Check_Indicator_LED_R_Fail;
		str_Pics = "FunErr_Check_Indicator_LED_R_Fail";
		str_msg = CW2A(L"Is there Red light on the phone ?\n 手机上方的红灯有亮吗?\n");
	}
	else if (m_str_Func.find("G") != string::npos){
		m_strErrorCode = FunErr_Check_Indicator_LED_G_Fail;
		str_Pics = "FunErr_Check_Indicator_LED_G_Fail";
		str_msg = CW2A(L"Is there Green light on the phone?\n 手机上方的绿灯有亮吗?\n");
	}
	else if (m_str_Func.find(_T("B")) != string::npos) {
		m_strErrorCode = FunErr_Check_Indicator_LED_B_Fail;
		str_Pics = "FunErr_Check_Indicator_LED_B_Fail";
		str_msg = CW2A(L"Is there Bule Light on phone?\n 手机上方的蓝灯有亮吗?\n");
	}
	else if (m_str_Func.find("Capkey") != string::npos) {
		m_strErrorCode = FunErr_CapKey_LED_Test_Fail;
		str_Pics = "FunErr_CapKey_LED_Test_Fail";
		str_msg = CW2A(L"Is there Light on phone?\n 手机下方的灯有亮吗?\n");
	}
	else if (m_str_OnOff.find("0,0,0,0") != string::npos) {
		showDiag = false;
	}
	else{
		b_res = false;
	}

	char sz_patternIndicatorLED[FTD_BUF_SIZE] = {0};
	strcpy(sz_patternIndicatorLED, m_str_OnOff.c_str());
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	b_res = m_pIPhone->FTD_LEDon(m_nFtdPort, m_nFtdTimeOut, sz_patternIndicatorLED, sz_outBuffer);
	if (!b_res)
	{
		str_msg2 = "CIF_LED command fail";
		TraceLog(MSG_INFO,str_msg2);
	}
	else
	{
		str_msg2 = "CIF_LEDT command success";
		TraceLog(MSG_INFO,str_msg2);
	}

	if(b_res)
	{
		if(showDiag){
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
				str_msg = _T("IndicatorLED Test success.");
				TraceLog(MSG_INFO, str_msg);
			}
			else
			{
				b_res = false;
				str_msg = _T("IndicatorLED Test fail.");
				TraceLog(MSG_INFO, str_msg);
			}
		}
	}

	if(b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		str_msg = _T("IndicatorLED ") + m_str_OnOff + _T(" Test success!");
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);
		SetPICSData(str_Pics, "PASS");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		str_msg = _T("IndicatorLED ") + m_str_OnOff + _T(" Test fail!");
		TraceLog(MSG_INFO, str_msg);
		SetPICSData(str_Pics, "FAIL");
	}

	FactoryLog();
	return b_res;
}
