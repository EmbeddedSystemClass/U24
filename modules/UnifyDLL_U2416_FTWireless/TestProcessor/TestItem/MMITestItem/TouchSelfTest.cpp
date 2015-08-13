/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .cpp file of CTouchSelfTest object.
 * FileName   : TouchSelfTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2012/06/12
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2012/06/12    Evonne.Hsieh          Create.
 ********************************************************************************/

#include "stdafx.h"
#include "TouchSelfTest.h"


RF_IMPLEMENT_DYNCREATE(CTouchSelfTest)

bool CTouchSelfTest::PreRun()
{
	m_strItemCode = CStr::IntToStr(TOUCH_Test_BaseItemCode);
	m_strErrorCode = FunErr_Touch_Auto_Check_Fail;
	m_strResult = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool CTouchSelfTest::Run()
{
	bool bRes = false;
	bRes = TouchSelfTest();
	return bRes;
}

bool CTouchSelfTest::PostRun()
{
	return true;
}

bool CTouchSelfTest::InitData(std::map<std::string, std::string>& paramMap)
{
	return true;
}

bool CTouchSelfTest::TouchSelfTest()
{
	bool b_res = false;
	std::string str_msg;

	char sz_input[FTD_BUF_SIZE] = {0};
	char sz_outData[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_TouchSelfTest(m_nFtdPort, m_nFtdTimeOut,sz_input, sz_outData)))
	{
		str_msg = "Touch Self Command fail";
		TraceLog(MSG_INFO, str_msg);
	}
	else
	{
		CString check_self_test = sz_outData;
		if (check_self_test.Find(_T("1")) == -1) {
			b_res = false;
		}
	}
	m_strMeasured = sz_outData;

	/* test result */
	if(b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "Touch Self Test pass!";
		TraceLog(MSG_INFO, "Touch Self Test pass!");
		SetPICSData("TOUCH_SELF_TEST", "PASS");
		ExecAndroidLogcat(true);
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "Touch Self Test fail";
		TraceLog(MSG_INFO, "Touch Self Test fail");
		SetPICSData("TOUCH_SELF_TEST", "FAIL");
	}
	FactoryLog();
	return b_res;
}
