#include "stdafx.h"
#include "BackupToOTP.h"


RF_IMPLEMENT_DYNCREATE(CBackupToOTP)

bool CBackupToOTP::PreRun()
{
	m_strResult = "";
	m_strItemCode = "";
	m_strErrorCode = "";
	m_strBand = "";
	m_strChannel = "";
	m_strUpper = "";
	m_strLower = "";
	m_strMeasured = "";
	m_strUnit = "";
	m_strMessage = "";
	return true;
}

bool CBackupToOTP::Run()
{
	bool bRes = MainFunction();
	return bRes;
}

bool CBackupToOTP::PostRun()
{
	return true;
}

bool CBackupToOTP::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find(std::string("RetryTimes")) == paramMap.end())
	{
		TraceLog(MSG_INFO, _T("Failed to find 'RetryTimes' for class 'CBackupToOTP' default Retry=3"));
		m_iRetry = 3;
	}
	else
	{
		m_iRetry = atoi(paramMap[std::string("RetryTimes")].c_str());
	}

	if (paramMap.find(std::string("Backup")) == paramMap.end())
	{
		TraceLog(MSG_INFO, _T("Failed to find 'Backup' for class 'CBackupToOTP'"));
		return false;
	}
	std::vector<std::string> vBackupList;
	STDStringTokenizer(paramMap["Backup"], ",", vBackupList);
	if(vBackupList.size() != 3)
	{
		TraceLog(MSG_INFO, _T("Failed to get 'Backup' param for class 'CBackupToOTP', size != 3"));
		return false;
	}
	m_bBackupFA = (atoi(vBackupList[0].c_str()) == 1) ?	true : false;
	m_bBackupCFG = (atoi(vBackupList[1].c_str()) == 1) ? true : false;
	m_bBackupModemCFG = (atoi(vBackupList[2].c_str()) == 1) ? true : false;

	return true;
}

bool CBackupToOTP::MainFunction()
{
	char sz_input[FTD_BUF_SIZE] = {0};
	char sz_outData[FTD_BUF_SIZE] = {0};

	strcpy(sz_input, "0x12345678");

	// 1. Backup FA //
	if(m_bBackupFA)
	{
		m_strMessage = "Backup FA Start.";
		FactoryLog();
		if (!m_pIPhone->FTD_FAC_BackupFA(m_nFtdPort, m_nFtdTimeOut, sz_input, sz_outData))
		{
			m_strMessage = "Backup FA fail...";
			m_strResult = "FAIL";
			m_strErrorCode = "Qfa_Backup_To_Qotp_Fail";
			TraceLog(MSG_ERROR, m_strMessage);
			FactoryLog();
			return false;
		}
		m_strMessage = "Backup FA success...";
		FactoryLog();
	}

	// 2. Backup CFG //
	if(m_bBackupCFG)
	{
		m_strMessage = "Backup CFG Start.";
		FactoryLog();
		if (!m_pIPhone->FTD_FAC_BackupCFG(m_nFtdPort, m_nFtdTimeOut, sz_input, sz_outData))
		{
			m_strMessage = "Backup CFG fail...";
			m_strResult = "FAIL";
			m_strErrorCode = "Qfa_Backup_To_Qotp_Fail";
			TraceLog(MSG_ERROR, m_strMessage);
			FactoryLog();
			return false;
		}
		m_strMessage = "Backup CFG success...";
		FactoryLog();
	}

	// 3. Backup ModemCFG //
	if(m_bBackupModemCFG)
	{
		m_strMessage = "Backup ModemCFG Start.";
		FactoryLog();
		if (!m_pIPhone->FTD_FAC_BackupModemCFG(m_nFtdPort, m_nFtdTimeOut, sz_input, sz_outData))
		{
			m_strMessage = "Backup ModemCFG fail...";
			m_strResult = "FAIL";
			m_strErrorCode = "Qfa_Backup_To_Qotp_Fail";
			TraceLog(MSG_ERROR, m_strMessage);
			FactoryLog();
			return false;
		}
		m_strMessage = "Backup ModemCFG success...";
		FactoryLog();
	}

	m_strMessage = "Backup to OTP success";
	m_strResult = "PASS";
	TraceLog(MSG_INFO, m_strMessage);
	FactoryLog();

	return true;
}
