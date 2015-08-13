#include "stdafx.h"
#include "NVbackup.h"


RF_IMPLEMENT_DYNCREATE(CNVbackup)


bool CNVbackup::PreRun()
{
	m_strItemCode = "-";
	m_strErrorCode = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMessage = "";

	return true;
}

bool CNVbackup::Run()
{
	if (! m_pIPhone->Initial())
	{
		TraceLog(MSG_ERROR, "Fail to execute m_pIPhone->Initial()");
		return false;
	}

	bool ret = m_pIPhone->BackupNVpartition();
	if (ret)
	{
		TraceLog(MSG_INFO, "BackupNVPartition success");
	}
	else
	{
		m_strErrorCode = BACKUP_NV_FAIL;
		m_strMessage = "BackupNVPartition fail";
		TraceLog(MSG_ERROR, "Fail to backup NV partition");
	}

	return ret;
}

bool CNVbackup::PostRun()
{
	TraceLog(MSG_INFO, "NV backup PASS");
	return true;
}

bool CNVbackup::InitData(std::map<std::string, std::string>& paramMap)
{
	return true;
}
