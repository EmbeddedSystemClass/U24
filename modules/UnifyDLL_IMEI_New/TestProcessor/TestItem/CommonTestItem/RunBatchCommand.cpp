#include "stdafx.h"
#include "RunBatchCommand.h"


RF_IMPLEMENT_DYNCREATE(CRunBatchCommand)

bool CRunBatchCommand::PreRun()
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

bool CRunBatchCommand::Run()
{
	bool bRes = MainFunction();
	m_strItemCode = CStr::IntToStr(ChangeFTDtoNormal_BaseItemCode);
	return bRes;
}

bool CRunBatchCommand::PostRun()
{
	return true;
}

bool CRunBatchCommand::InitData(std::map<std::string, std::string>& paramMap)
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
		TraceLog(MSG_INFO, "Failed to Get filed 'WaitTime' for class 'CRunBatchCommand'");
		return false;
	}

	/* File */
	if (paramMap.find(std::string("File")) != paramMap.end())
	{
		m_str_file = paramMap[std::string("File")].c_str();
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'File' for class 'CRunBatchCommand'");
		return false;
	}
	return true;
}

bool CRunBatchCommand::MainFunction()
{
	CString str_command;
	CString str_msg;
	int ret;
	bool isOk = true;
	CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));
	CString file_path = pthToolDir + _T("\\") + m_str_file.c_str();

	if(access(file_path, 0) == 0)
	{
		/* Execute batch file */
		str_command.Format(_T("start /min %s"), file_path);
		ret = system(str_command);
		isOk = (ret == 0);
		str_msg.Format(_T("Execute batch: %s"), str_command);
		TraceLog(MSG_INFO, std::string(CT2A(str_msg)));
		str_msg.ReleaseBuffer();
	}
	else
	{
		isOk = false;
		TraceLog(MSG_INFO, "File is not found.");
	}

	if (isOk)
	{
		m_strMessage = "Run batch PASS";
		m_strResult = "PASS";
		m_strErrorCode = "-";
	}
	else
	{
		m_strMessage = "Run batch FAIL";
		m_strResult = "FAIL";
		m_strErrorCode = "-";
		isOk = false;
	}

	TraceLog(MSG_INFO, m_strMessage);
	FactoryLog();
	Sleep(m_msWaitTime);
	return isOk;
}
