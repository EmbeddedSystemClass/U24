#include "stdafx.h"
#include "ExeAdbCommand.h"


RF_IMPLEMENT_DYNCREATE(CExeAdbCommand)

bool CExeAdbCommand::PreRun()
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

	return true;
}

bool CExeAdbCommand::Run()
{
	bool bRes = MainFunction();
	m_strItemCode = CStr::IntToStr(ChangeFTDtoNormal_BaseItemCode);
	
	return bRes;
}

bool CExeAdbCommand::PostRun()
{
	return true;
}

bool CExeAdbCommand::InitData(std::map<std::string, std::string>& paramMap)
{
	std::string str;
	/* WaitTime */
	if (paramMap.find(std::string("WaitTime")) != paramMap.end())
	{
		str = paramMap[std::string("WaitTime")];
		m_msWaitTime = CStr::StrToInt(str);
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to Get filed 'WaitTime' for class 'CExeAdbCommand'"));
		::AfxMessageBox(_T("AdbShellStart get WaitTime fail"));
		return false;
	}

	/* Command */
	if (paramMap.find(std::string("AdbCommand")) != paramMap.end())
	{
		m_Adbcommand = paramMap[std::string("AdbCommand")].c_str();
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to Get filed 'AdbCommand' for class 'CExeAdbCommand'"));
		return false;
	}
	return true;
}

bool CExeAdbCommand::MainFunction()
{
 	if (m_pIPhone)
 	{
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
 	}

	char output[1024] = {0};
	char error[1024] = {0};
	char AdbCommnad[1024] = {0};
	strcpy(AdbCommnad, m_Adbcommand.c_str());

	if (!m_pIPhone->ExecAdbCommand(AdbCommnad, output, error))
	{
		m_strMessage = "Fail to Exeute ADB " + m_Adbcommand;
		m_strResult = "FAIL";
		m_strErrorCode = CommErr_Get_ADB_Fail;
		TraceLog(MSG_ERROR, m_strMessage);
		FactoryLog();
		return false;
	}
	else
	{
		m_strMessage = "Adb Command PASS";
		m_strResult = "PASS";
		m_strErrorCode = "-";
		TraceLog(MSG_INFO, m_strMessage);
		FactoryLog();
	}


	Sleep(m_msWaitTime);

	return true;
}
