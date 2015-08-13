#include "stdafx.h"
#include "ExeADBCmd.h"


RF_IMPLEMENT_DYNCREATE(CExeADBCmd)

bool CExeADBCmd::PreRun()
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

bool CExeADBCmd::Run()
{
	bool bRes = MainFunction();
	return bRes;
}

bool CExeADBCmd::PostRun()
{
	return true;
}

bool CExeADBCmd::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find(std::string("RetryTimes")) != paramMap.end())
	{
		m_iRetry = atoi(paramMap[std::string("RetryTimes")].c_str());
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to find 'RetryTimes' for class 'CExeADBCmd' default Retry=3"));
		m_iRetry = 3;
	}

	if (paramMap.find(std::string("Title")) == paramMap.end())
	{
		TraceLog(MSG_INFO, _T("Failed to find 'Title' for class 'CExeADBCmd'"));
		return false;
	}
	m_strTitle = paramMap[std::string("Title")];

	if (paramMap.find(std::string("Command")) == paramMap.end())
	{
		TraceLog(MSG_INFO, _T("Failed to find 'Command' for class 'CExeADBCmd'"));
		return false;
	}
	m_CStrCommand = paramMap[std::string("Command")].c_str();

	if (paramMap.find(std::string("ErrorCode")) == paramMap.end())
	{
		TraceLog(MSG_INFO, _T("Failed to find 'ErrorCode' for class 'CExeADBCmd'"));
		return false;
	}
	m_strErrorCode = paramMap[std::string("ErrorCode")];
	return true;
}

bool CExeADBCmd::MainFunction()
{
	char output[50000] = {0};
	char ErrorCode[50000] = {0};
	bool bRet;

	m_strMessage = "Start to Adb " + m_strTitle;
	FactoryLog();
	m_strMessage = "Command: adb " + m_CStrCommand;
	FactoryLog();
	bRet = ExecAdbOut(m_CStrCommand, output, ErrorCode);
	m_strMessage = MakeStr("adb output: %s", output);
	FactoryLog();
	m_strMessage = MakeStr("adb Errorcode: %s", ErrorCode);
	FactoryLog();
	if (!bRet)
	{
		m_strMessage = m_strTitle + " fail...";
		FactoryLog();
		return false;
	}

	m_strMessage = m_strTitle + "success...";
	m_strErrorCode = "";
	m_strResult = "PASS";
	TraceLog(MSG_INFO, m_strMessage);
	FactoryLog();

	return true;
}

bool CExeADBCmd::ExecAdbOut(CString Command, char* output, char* ErrorCode)
{
	bool isOk = false;
	DWORD nPipeSize = 1024 * 1024; //1M pipeline

	CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));
	CString path_adb = pthToolDir + _T("\\adb.exe");
	if (_taccess(path_adb, 0) == -1)
	{
		strcpy(ErrorCode, "ERROR: No adb.exe exist!");
		return false;
	}

	HANDLE hRead, hWrite;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, nPipeSize))
	{
		strcpy(ErrorCode, "ERROR: CreatePipe fail!");
		return false;
	}

	//HANDLE hProcess = NULL;
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupInfo.wShowWindow	= SW_HIDE;
	startupInfo.hStdError = hWrite;
	startupInfo.hStdOutput = hWrite;

	Command = _T("\"") + path_adb + _T("\" ") + Command;
	TRACE(_T("Cmd: %s\n"), Command);
	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo))
	{
		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, 10 * 1000); // timeout in 10 seconds

		CloseHandle(hWrite);
		hWrite = NULL;
		//if timeout then exit the process
		if (TimeOutSignal == WAIT_TIMEOUT)
		{
			isOk = false;
			TerminateProcess(processInfo.hProcess, 0);
			strcpy(ErrorCode, "ERROR: Adb timeout");
		}
		else
		{

			isOk = true;
			DWORD bytesRead;
			char* message = new char[nPipeSize];
			memset(message, 0, sizeof(message));
			::ReadFile(hRead, message, nPipeSize, &bytesRead, NULL);
			message[bytesRead] = '\0';

			strcpy(output, message);
			strcpy(ErrorCode, "Adb command ok");
			delete [] message;
		}
	}
	else
	{
		isOk = false;
		strcpy(ErrorCode, "ERROR: Execute adb.exe fail!");
	}
	Command.ReleaseBuffer();
	CloseHandle(hRead);
	if (hWrite)CloseHandle(hWrite);
	//CloseHandle(hProcess);

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	//hProcess = NULL;

	return isOk;
}
