/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .cpp file of CBostonTouchSelfTest object.
 * FileName   : TouchSelfTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     :Lion.Wu
 * Time       : 2012/06/12
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2012/06/12   Lion.Wu          Create.
 ********************************************************************************/

#include "stdafx.h"
#include "BostonTouchSelfTest.h"


RF_IMPLEMENT_DYNCREATE(CBostonTouchSelfTest)

bool CBostonTouchSelfTest::PreRun()
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

bool CBostonTouchSelfTest::Run()
{
	bool bRes = false;
	bRes = TouchSelfTest();

	return bRes;
}

bool CBostonTouchSelfTest::PostRun()
{
	return true;
}

bool CBostonTouchSelfTest::InitData(std::map<std::string, std::string>& paramMap)
{
	return true;
}

bool CBostonTouchSelfTest::TouchSelfTest()
{
	bool b_res = false;
	std::string str_msg;
	char sz_input[FTD_BUF_SIZE] = {0};
	char sz_outData[FTD_BUF_SIZE] = {0};
	if (!(b_res = m_pIPhone->FTD_TouchSelfTest(m_nFtdPort, m_nFtdTimeOut,sz_input, sz_outData)))
	{
		str_msg = _T("Touch Self Command fail");
		TraceLog(MSG_INFO, str_msg);
	}
	else
	{
		CString check_self_test = sz_outData;
		if (check_self_test.Find(_T("0")) == -1) {//Boston = 0, Detroit = 1;
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

		//  Control Run logcat touch  //
		CString cstrReportBatch;
		cstrReportBatch.Format(_T("C:\\PE_Config\\GBOSB1A\\setting\\ALS_Report.bat"));
		::CFile cf;
		if( cf.Open(cstrReportBatch, ::CFile::modeRead) )
		{
			cf.Close();

			CString str_command = _T("");
			CString str_msg = _T("");
			CString str_LogPath = _T("");
			CString str_Picasso = _T("RD");

			SYSTEMTIME systemTime;
			::GetLocalTime(&systemTime);
			str_LogPath.Format(_T("D:\\LOG\\Android\\%04d%02d%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay);

			if(! g_strPicasso.empty() )
				str_Picasso = g_strPicasso.c_str();

			/* Execute batch file */
			str_command.Format(_T("start /min %s %s %s"), cstrReportBatch, str_Picasso, str_LogPath);
			system(str_command);
			str_msg.Format(_T("Execute batch: %s"), str_command);
			TraceLog(MSG_INFO, str_msg.GetString());
			Sleep(3000);
		}
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
