/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CSapporoFtdTest object.
 * FileName   : SapporoFtdTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Lion.Wu
 * Time       : 2012.03.15
 ********************************************************************************/

#include "stdafx.h"
#include "SapporoFtdTest.h"


RF_IMPLEMENT_DYNCREATE(CSapporoFtdTest)


bool CSapporoFtdTest::PreRun()
{
	m_strResult = "-";
	m_strItemCode = "-";
	m_strErrorCode = "-";//FunErr_Ftd_Start_Fail;
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool CSapporoFtdTest::Run()
{
	CString cs_FtdFunction = std_FtdFunction.c_str();
	bool b_res = false;
	if (cs_FtdFunction.Compare("runUsbDetectStart") == 0)
		b_res = runUsbDetectStart();
	else if (cs_FtdFunction.Compare("runUsbDetectEnd") == 0)
		b_res = runUsbDetectEnd();
	return b_res;
}

bool CSapporoFtdTest::PostRun()
{
	return true;
}

bool CSapporoFtdTest::InitData(std::map<std::string, std::string>& paramMap)
{
	///* PatternIndex */
	if (paramMap.find(std::string("Name")) != paramMap.end())
	{
		std_FtdFunction = paramMap[std::string("Name")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter std_FtdFunction for AudioFunction");
		return false;
	}
	return true;
}

bool CSapporoFtdTest::runUsbDetectStart()
{
	bool b_res = true;
	CString cs_Result;
	std::string str_msg, msg;


	/* read PressButton */
	char sz_Input[FTD_BUF_SIZE] = {0};
	strncpy(sz_Input, "1", 1);
	char sz_Output[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->Sapporo_ftdStartEnd(m_nFtdPort, m_nFtdTimeOut, sz_Input, sz_Output))
	{
		str_msg = "runUsbDetectStart from mobile fail";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	/* test result */
	if(b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check runUsbDetectStart PASS!");
		SetPICSData("runUsbDetectStart", "PASS");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		m_strErrorCode = FunErr_USB_Detect_Start_Fail;//FunErr_Ftd_End_Fail;
		TraceLog(MSG_INFO, "runUsbDetectStart FAIL!");
		SetPICSData("runUsbDetectStart", "FAIL");
	}
	FactoryLog();
	return b_res;
}

bool CSapporoFtdTest::runUsbDetectEnd()
{
	bool b_res = true;
	std::string str_msg;

	/* read PressButton */
	char sz_Input[FTD_BUF_SIZE] = {0};
	strncpy(sz_Input, "0", 1);
	char sz_Output[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->Sapporo_ftdStartEnd(m_nFtdPort, m_nFtdTimeOut, sz_Input, sz_Output))
	{
		str_msg = "Sapporo_ftdStartEnd from mobile fail";
		TraceLog(MSG_INFO, str_msg);
		b_res = false;
	}

	/* test result */
	if(b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check Sapporo_ftdStartEnd PASS!");
		SetPICSData("Sapporo_ftdStartEnd", "PASS");
	}
	else
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_USB_Detect_End_Fail;//FunErr_Ftd_Start_Fail;
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check Sapporo_ftdStartEnd FAIL!");
		SetPICSData("Sapporo_ftdStartEnd", "FAIL");
	}
	FactoryLog();
	return b_res;
}
