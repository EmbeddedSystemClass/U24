/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .cpp file of CEcompassSelfTest object.
 * FileName   : EcompassSelfTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     :  Lion.Wu  
 * Time       : 2012/06/12
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2012/06/12     Lion.Wu         Create.
 ********************************************************************************/

#include "stdafx.h"
#include "EcompassSelfTest.h"


RF_IMPLEMENT_DYNCREATE(CEcompassSelfTest)

bool CEcompassSelfTest::PreRun()
{
	m_strItemCode = CStr::IntToStr(ECompass_BaseItemCode);
	m_strErrorCode = FunErr_ECompassr_Test_Fail;
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

bool CEcompassSelfTest::Run()
{
	bool bRes = false;
	bRes = EcompassSelfTest();
	return bRes;
}

bool CEcompassSelfTest::PostRun()
{
	return true;
}

bool CEcompassSelfTest::InitData(std::map<std::string, std::string>& paramMap)
{
	return true;
}

bool CEcompassSelfTest::EcompassSelfTest()
{
	std::string str_msg;
	CString msg;
	bool b_res = false;
	char sz_input[FTD_BUF_SIZE] = {0};
	char sz_outData[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_SNSREcompassSelfTest(m_nFtdPort, m_nFtdTimeOut,sz_input, sz_outData)))
	{
		str_msg = _T("EcompassSelfTest Command fail");
		TraceLog(MSG_INFO, str_msg);
	}
	else
	{
		CString check_self_test = sz_outData;
		if (check_self_test.Find(_T("PASSED")) == -1)
			b_res = false;
	}
	m_strMeasured = sz_outData;
	/* test result */
	if(b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "Ecompass_Self_Test pass!";
		TraceLog(MSG_INFO, "Ecompass_Self_Test pass!");
		SetPICSData("Ecompass_Self_Test", "PASS");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "Ecompass_Self_Test fail";
		TraceLog(MSG_INFO, "Ecompass_Self_Test fail");
		SetPICSData("Ecompass_Self_Test", "FAIL");
	}
	FactoryLog();
	return b_res;
}
