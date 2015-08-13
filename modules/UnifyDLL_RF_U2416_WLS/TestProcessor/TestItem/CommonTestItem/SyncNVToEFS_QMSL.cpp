#include "stdafx.h"
#include "SyncNVToEFS_QMSL.h"


RF_IMPLEMENT_DYNCREATE(CSyncNVToEFS_QMSL)


bool CSyncNVToEFS_QMSL::Run()
{
	bool bRes = MainFunction();

	if (bRes)
	{
		FactoryLog(true, "", "", "", "", "", "", "", "", "Sync NV to EFS PASS");
		SetPICSData(m_strPICSName, "PASS");
	}
	else
	{
		FactoryLog(false, "", m_strErrCode, "", "", "", "", "", "", m_strMsg);
		SetPICSData(m_strPICSName, "FAIL");
	}

	return bRes;
}

bool CSyncNVToEFS_QMSL::MainFunction()
{
	if (!m_pIPhone->Initial_QMSL())
	{
		m_strMsg = "Fail to initial QMSL";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	if (!m_pIPhone->IsConnected_QMSL())
	{
		m_strMsg = "Fail to connect phone with QMSL";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	bool bSuccess = false;
	unsigned char ucStatus;
	for (int i = 0; i < m_iRetryTimes; ++i)
	{
		if (m_pIPhone->QMSL_EFS2_SyncWithWait(&ucStatus, m_iSyncCmdTimeout))
		{
			bSuccess = true;
			break;
		}
		if (i < (m_iRetryTimes - 1))
		{
			m_strMsg = "Fail to sync NV to EFS, try again";
			TraceLog(MSG_ERROR, m_strMsg);
		}
	}
	if (!bSuccess)
	{
		switch (ucStatus)
		{
		case 1:
			TraceLog(MSG_ERROR, "Command error occurred");
			break;
		case 2:
			TraceLog(MSG_ERROR, "Command can't be dispatched target");
			break;
		case 3:
			TraceLog(MSG_ERROR, "Command timeout occurred");
			break;
		default:
			TraceLog(MSG_ERROR, MakeStr("Unknown error, status = %d", ucStatus));
		}

		m_strMsg = "Fail to sync NV to EFS";
		m_strErrCode = ERR_QLIB_DIAG_NV_WRITE_FlushBatchQueue;
		TraceLog(MSG_ERROR, m_strMsg);
	}

	return bSuccess;
}


bool CSyncNVToEFS_QMSL::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find("RetryTimes") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RetryTimes for CSyncNVToEFS_QMSL");
		return false;
	}
	m_iRetryTimes = atoi(paramMap["RetryTimes"].c_str());
	if (m_iRetryTimes < 1)
	{
		m_iRetryTimes = 1;
	}

	if (paramMap.find("SyncCmdTimeout") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter SyncCmdTimeout for CSyncNVToEFS_QMSL");
		return false;
	}
	m_iSyncCmdTimeout = atoi(paramMap["SyncCmdTimeout"].c_str());
	if (m_iSyncCmdTimeout < 1)
	{
		m_iSyncCmdTimeout = 5000;
	}

	if (paramMap.find("PICSName") != paramMap.end())
	{
		m_strPICSName = paramMap["PICSName"];
	}
	else
	{
		m_strPICSName.clear();
	}

	return true;
}
