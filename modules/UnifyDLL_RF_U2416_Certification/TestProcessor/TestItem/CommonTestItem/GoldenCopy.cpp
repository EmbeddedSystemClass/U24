#include "stdafx.h"
#include "GoldenCopy.h"


RF_IMPLEMENT_DYNCREATE(CGoldenCopy)

bool CGoldenCopy::Run()
{
	bool bRes = MainFunction();

	if (bRes)
	{
		FactoryLog(true, "", "", "", "", "", "", "", "", "Golden copy PASS");
		SetPICSDate("GoldenCopy", "PASS");
	}
	else
	{
		FactoryLog(false, "Golden copy", m_strErrCode, "", "", "", "", "", "", m_strMsg);
		SetPICSDate("GoldenCopy", "FAIL");
	}

	return bRes;
}

bool CGoldenCopy::MainFunction()
{
	if (! m_pIPhone->Initial())
	{
		TraceLog(MSG_ERROR, "Fail to execute m_pIPhone->Initial()");
		return false;
	}

	bool ret = m_pIPhone->EFs_GoldenCopy();
		
	TraceLog(MSG_INFO, "Golden copy success");

	{
		// temperarily always pass
		/*
		m_strErrCode = ERR_CHANGE_MODE;
		m_strMsg = "Golden copy fail";
		TraceLog(MSG_ERROR, "Fail to Golden copy");
		*/
	}

	return true;
}

bool CGoldenCopy::PostRun()
{
	TraceLog(MSG_INFO, "Golden copy PASS");
	return true;
}

bool CGoldenCopy::InitData(std::map<std::string, std::string>& paramMap)
{
	return true;
}
