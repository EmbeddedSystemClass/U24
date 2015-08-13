#include "stdafx.h"
#include "SyncCombo.h"


RF_IMPLEMENT_DYNCREATE(CSyncCombo)

bool CSyncCombo::Run()
{
	bool bRes = MainFunction();

	if (bRes)
	{
		FactoryLog(true, "", "", "", "", "", "", "", "", "Sync combo PASS");
		SetPICSDate("SyncCombo", "PASS");
	}
	else
	{
		FactoryLog(false, "Sync combo", m_strErrCode, "", "", "", "", "", "", m_strMsg);
		SetPICSDate("SyncCombo", "FAIL");
	}

	return bRes;
}

bool CSyncCombo::MainFunction()
{
	if (! m_pIPhone->Initial())
	{
		TraceLog(MSG_ERROR, "Fail to execute m_pIPhone->Initial()");
		return false;
	}

	bool ret = m_pIPhone->EFS_Sync_Combo(m_iEFsSyncRetryCount);
	if (ret)
	{
		TraceLog(MSG_INFO, "Sync combo success");
	}
	else
	{
		m_strErrCode = ERR_CHANGE_MODE;
		m_strMsg = "Sync combo fail";
		TraceLog(MSG_ERROR, "Fail to Sync combo");
	}

	Sleep(5000);

	return ret;
}

bool CSyncCombo::PostRun()
{
	TraceLog(MSG_INFO, "Sync combo PASS");
	return true;
}

bool CSyncCombo::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find ("EFsSyncRetryCount") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter EFsSyncRetryCount for CSyncCombo");
		return false;
	}
	m_iEFsSyncRetryCount = atoi(paramMap["EFsSyncRetryCount"].c_str());

	return true;
}
