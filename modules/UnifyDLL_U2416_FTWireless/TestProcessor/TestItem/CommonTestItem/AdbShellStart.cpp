#include "stdafx.h"
#include "AdbShellStart.h"


RF_IMPLEMENT_DYNCREATE(CAdbShellStart)

bool CAdbShellStart::PreRun()
{
	m_strResult = "-";
	m_strItemCode = "-";
	m_strErrorCode = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";
	m_msWaitTime = 0;
	return true;
}

bool CAdbShellStart::Run()
{
	bool bRes = MainFunction();
	m_strItemCode = CStr::IntToStr(ChangeFTDtoNormal_BaseItemCode);
	if (bRes)
	{
		m_strMessage = "Adb Shall Start PASS";
		m_strResult = "PASS";
		m_strErrorCode = "-";
		FactoryLog();
		TraceLog(MSG_INFO, _T("Check CPU ID PASS!"));
	}
	else
	{
		m_strMessage = "Check and change phone mode Fail";
		m_strResult = "FAIL";
		m_strErrorCode = CommErr_Phone_Restart_Fail;
		FactoryLog();
	}
	return bRes;
}

bool CAdbShellStart::PostRun()
{
	return true;
}

bool CAdbShellStart::InitData(std::map<std::string, std::string>& paramMap)
{
	/* WaitTime */
	if (paramMap.find(std::string("WaitTime")) != paramMap.end())
	{
		m_msWaitTime = atoi(paramMap[std::string("WaitTime")].c_str());
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to Get filed 'WaitTime' for class 'CIDTestProc'"));
		::AfxMessageBox(_T("AdbShellStart get WaitTime fail"));
		return false;
	}

	return true;
}

bool CAdbShellStart::MainFunction()
{
 	if (m_pIPhone) return false;

	if (!m_pIPhone->Initial())
	{
		m_strMessage = "Fail to Initial IPhone,Loading Qcomdiag fail!";
		TraceLog(MSG_ERROR, m_strMessage);
		return false;
	}
	if (!m_pIPhone->IsConnected())
	{
		m_strMessage = "Fail to connect phone with Qcomdiag dll" ;
		TraceLog(MSG_ERROR, m_strMessage) ;
		return false;
	}

	char output[1024] = {0};
	char error[1024] = {0};
	if (!m_pIPhone->ExecAdbCommand("shell start", output, error))
	{
		m_strMessage = "Fail to Adb_Shell_Start!";
		TraceLog(MSG_ERROR, m_strMessage);
		return false;
	}
	Sleep(m_msWaitTime);

	/* Query operator title */
	std::string str_msgEnglish = "OS mode\n";
	std::string str_msgChinese = CW2A(L"手机开机\n");
	std::string str_msg = str_msgEnglish + str_msgChinese;

	/* Init dialog response flag to false */
	m_b_DialogResponse = false;
	m_b_waitFlag = false;

	/* Notify UI to show message dialog */
	ShowDialog(str_msg.c_str());

	return true;
}
