#include "stdafx.h"
#include "WritePhoneColorInfo.h"

RF_IMPLEMENT_DYNCREATE(CWritePhoneColorInfo)

bool CWritePhoneColorInfo::PreRun()
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

bool CWritePhoneColorInfo::Run()
{
	bool bRes = MainFunction();
	return bRes;
}

bool CWritePhoneColorInfo::PostRun()
{
	return true;
}

bool CWritePhoneColorInfo::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find(std::string("RetryTimes")) != paramMap.end())
	{
		m_iRetry = atoi(paramMap[std::string("RetryTimes")].c_str());
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to find 'RetryTimes' for class 'CWritePhoneColorInfo' default Retry=3"));
		m_iRetry = 3;
	}

	if (paramMap.find(std::string("ColorMapping")) == paramMap.end())
	{
		TraceLog(MSG_INFO, _T("Failed to find 'ColorMapping' for class 'CWritePhoneColorInfo'"));
		return false;
	}
	std::vector<std::string> vMappingList;
	STDStringTokenizer(paramMap["ColorMapping"], ",", vMappingList);
	char szTmp[32] = {0};
	for(UINT i = 0; i < vMappingList.size(); i++)
	{
		char *strData[2] = {0};
		strcpy(szTmp, vMappingList[i].c_str());
		strData[0] = strtok(szTmp, ":");
		strData[1] = strtok(NULL, ":");
		m_mapCoverColor[strData[0]] = strData[1];
	}

	return true;
}

bool CWritePhoneColorInfo::MainFunction()
{
	std::string strColor;
	if(m_mapCoverColor[g_strPartNumber].compare("") == 0)
	{
		m_strMessage = "Part Number can't not find in mapping table";
		TraceLog(MSG_INFO, m_strMessage);
		strColor = "1";
	}
	else
	{
		strColor = m_mapCoverColor[g_strPartNumber];
	}

	m_strMessage = "Write Phone Color Info Start.";
	FactoryLog();

	CString Command;
	Command.Format("push PhoneColorInfo/config_%s /qcust/PhoneColor/config", strColor.c_str());

	m_strMessage = "Start Command: adb " + Command;
	FactoryLog();

	char output[50000] = {0};
	char ErrorCode[50000] = {0};
	bool bRet;
	bRet = ExecAdbOut(Command, output, ErrorCode);
	m_strMessage = MakeStr("adb output: %s", output);
	FactoryLog();
	m_strMessage = MakeStr("adb Errorcode: %s", ErrorCode);
	FactoryLog();

	if (!bRet)
	{
		m_strResult = "FAIL";
		m_strErrorCode = CommErr_UI_Cmd_Fail;
		m_strMessage = "adb push command fail.";
		FactoryLog();
		return false;
	}

	m_strMessage = "Write phone color info success";
	m_strResult = "PASS";
	TraceLog(MSG_INFO, m_strMessage);
	FactoryLog();
	return true;
}

bool CWritePhoneColorInfo::ExecAdbOut(CString Command, char* output, char* ErrorCode)
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
