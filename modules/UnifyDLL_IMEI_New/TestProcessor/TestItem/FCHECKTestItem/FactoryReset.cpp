#include "stdafx.h"
#include "FactoryReset.h"


RF_IMPLEMENT_DYNCREATE(CFactoryReset)

bool CFactoryReset::PreRun()
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

bool CFactoryReset::Run()
{
	bool bRes = MainFunction();
	return bRes;
}

bool CFactoryReset::PostRun()
{
	return true;
}

bool CFactoryReset::InitData(std::map<std::string, std::string>& paramMap)
{
	return true;
}

bool CFactoryReset::MainFunction()
{
	char output[50000] = {0};
	char ErrorCode[50000] = {0};
	CString Command;
	bool bRet;

	// Command 1 //
	Command = _T("shell /system/bin/newfs_msdos -F 32 /dev/block/platform/msm_sdcc.1/by-name/innersd");
	m_strMessage = "Start Command: adb " + Command;
	FactoryLog();
	bRet = ExecAdbOut(Command, output, ErrorCode);
	m_strMessage = MakeStr("adb output: %s", output);
	FactoryLog();
	m_strMessage = MakeStr("adb Errorcode: %s", ErrorCode);
	FactoryLog();
	if (!bRet)
	{
		m_strMessage = "ExecAdbOut Command fail";
		FactoryLog();
		return false;
	}

	// Command 2 //
	Command = _T("shell mkdir /cache/recovery/");
	m_strMessage = "Start Command: adb " + Command;
	FactoryLog();
	bRet = ExecAdbOut(Command, output, ErrorCode);
	m_strMessage = MakeStr("adb output: %s", output);
	FactoryLog();
	m_strMessage = MakeStr("adb Errorcode: %s", ErrorCode);
	FactoryLog();
	if (!bRet)
	{
		m_strMessage = "ExecAdbOut Command fail";
		FactoryLog();
		return false;
	}

	// Command 3 //
	Command = _T("push command /cache/recovery/");
	m_strMessage = "Start Command: adb " + Command;
	FactoryLog();
	bRet = ExecAdbOut(Command, output, ErrorCode);
	m_strMessage = MakeStr("adb output: %s", output);
	FactoryLog();
	m_strMessage = MakeStr("adb Errorcode: %s", ErrorCode);
	FactoryLog();
	if (!bRet)
	{
		m_strMessage = "ExecAdbOut Command fail";
		FactoryLog();
		return false;
	}

	// Command 4 //
	Command = _T("reboot recovery");
	m_strMessage = "Start Command: adb " + Command;
	FactoryLog();
	bRet = ExecAdbOut(Command, output, ErrorCode);
	m_strMessage = MakeStr("adb output: %s", output);
	FactoryLog();
	m_strMessage = MakeStr("adb Errorcode: %s", ErrorCode);
	FactoryLog();
	if (!bRet)
	{
		m_strMessage = "ExecAdbOut Command fail";
		FactoryLog();
		return false;
	}

	m_strMessage = "Factory Reset success";
	m_strResult = "PASS";
	TraceLog(MSG_INFO, m_strMessage);
	FactoryLog();

	return true;
}

bool CFactoryReset::ExecAdbOut(CString Command, char* output, char* ErrorCode)
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
