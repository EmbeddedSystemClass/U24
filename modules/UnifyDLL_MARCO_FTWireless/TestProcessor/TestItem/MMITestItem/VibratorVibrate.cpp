
#include "stdafx.h"
#include "VibratorVibrate.h"

RF_IMPLEMENT_DYNCREATE(CVibratorVibrate)

bool CVibratorVibrate::InitData(std::map<std::string, std::string>& paramMap)
{
	return true;
}

bool CVibratorVibrate::PreRun()
{
	m_strResult    = "";
	m_strItemCode  = CStr::IntToStr(Vibrator_Vibrate_BaseItemCode);
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

bool CVibratorVibrate::Run()
{
	if (m_pIPhone == NULL) return false;

	bool b_res = false;
	b_res = VibrateTestFunc();
	return b_res;
}

bool CVibratorVibrate::PostRun()
{
	return true;
}

bool CVibratorVibrate::VibrateTestFunc()
{
	if (m_pIPhone == NULL)
	{
		TraceLog(MSG_INFO, "m_pIPhone is NULL.");
		return false;
	}

	bool b_res = true;
	/* Vibrate Start */
	TraceLog(MSG_INFO, "Vibrator ON.");
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_VibRun(m_nFtdPort, m_nFtdTimeOut, "1", sz_outBuffer)))
	{
		/* Try again */
		Sleep(1000);
		if (!(b_res = m_pIPhone->FTD_VibRun(m_nFtdPort, m_nFtdTimeOut, "1", sz_outBuffer)))
		{
			m_strResult = "FAIL";
			m_strErrorCode = FunErr_Vibrator_Test_Fail;
			m_strMessage = "Vibrator ON Fail.";
			SetPICSData("VIBRATOR_TEST", "FAIL");
			TraceLog(MSG_INFO, "Vibrator ON Fail.");
			FactoryLog();
			return false;
		}
	}
	TraceLog(MSG_INFO, "Vibrator ON Success.");

	/* Query operator title */
	std::string str_msg;
	std::string str_msgEnglish;
	std::string str_msgChinese;
	str_msgEnglish = "Phone is vibrating?\n";
	str_msgChinese = CW2A(L"手机在振动?\n");
	str_msg = str_msgEnglish + str_msgChinese;

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

	if (b_res)
	{
		/* Get UI dialog operator response */
		if (m_b_DialogResponse == true)
		{
			b_res = true;
			TraceLog(MSG_INFO, "Phone vibrate Success.");
		}
		else
		{
			b_res = false;
			m_strMessage = "Phone vibrate Fail.";
			TraceLog(MSG_INFO, "Phone vibrate Fail.");
		}
	}

	/* Vibrate Stop */
	TraceLog(MSG_INFO, "Vibrator OFF.");
	if (!(m_pIPhone->FTD_VibRun(m_nFtdPort, m_nFtdTimeOut, _T("0"), sz_outBuffer)))
	{
		b_res = false;
		TraceLog(MSG_INFO, "Vibrator OFF Fail.");
	}
	else
	{
		TraceLog(MSG_INFO, "Vibrator OFF Success.");
	}

	if (b_res)
	{
		m_strResult = "PASS";
		m_strMessage = "Vibrator Test Pass.";
		TraceLog(MSG_INFO, "Vibrator Test Pass.");
		FactoryLog();
		SetPICSData("VIBRATOR_TEST", "PASS");
	}
	else
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Vibrator_Test_Fail;
		TraceLog(MSG_INFO, "Vibrator Test Fail.");
		FactoryLog();
		SetPICSData("VIBRATOR_TEST", "FAIL");
	}
	return b_res;
}
