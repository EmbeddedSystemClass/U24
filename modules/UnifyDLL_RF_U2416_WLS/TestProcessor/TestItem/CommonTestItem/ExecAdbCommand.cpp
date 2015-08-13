#include "stdafx.h"
#include "ExecAdbCommand.h"


RF_IMPLEMENT_DYNCREATE(CExecAdbCommand)


bool CExecAdbCommand::Run()
{
	bool bRes = MainFunction();

	if (bRes)
	{
		m_strMsg = "Execute Adb command PASS";
		FactoryLog(true, "ExecAdbCommand", "--", "--", "--", "--", "--", "Pass", "--", m_strMsg.c_str());
		SetPICSData("ExecAdbCommand", "PASS");
	}
	else
	{
		m_strMsg = "Execute Adb command Fail";
		FactoryLog(false, "--", CommErr_Get_ADB_Fail, "--", "--", "--", "--", "Fail", "--", m_strMsg.c_str());
		SetPICSData("ExecAdbCommand", "FAIL");
	}
	
	return bRes;
}

bool CExecAdbCommand::MainFunction()
{	
	if (!m_pIPhone->MyExecAdbCmd(m_strCommand.c_str(), NULL, false))
	{
		m_strMsg = "Fail to execute adb command";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	Sleep(m_nCommandWait);

	return true;
}

bool CExecAdbCommand::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find ("CommandWait") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CommandWait for CExecAdbCommand");
		return false;
	}
	m_nCommandWait = atoi(paramMap["CommandWait"].c_str());

	if (paramMap.find ("Command") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Command for CExecAdbCommand");
		return false;
	}
	m_strCommand = paramMap["Command"].c_str();
	return true;
}
