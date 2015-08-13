/*********************************************************************************
* COPYRIGHT Qisda WGC18 2011
*
* Description: .cpp file of CTouchPanelProc object.
* FileName   : CTouchPanelProc.cpp
* Version    : $Revision:   1.4  $
* Author     : Gilpin.Xi
* Time       : 2011/04/18
* History    :
*  NO      Date        Updated by        Content of change
*  1    2011/04/18      Gilpin.Xi             Create.
********************************************************************************/

#include "stdafx.h"
#include "ADB.h"
#include "../../../UnifyDLL_Detroit_FTWireless.h"
//D:\workingDIR\2012.4.28GIT\Modules\UnifyDLL_Detroit_FTWireless
	//  D:\workingdir\2012.4.28git\modules\unifydll_detroit_ftwireless\testprocessor\Processor

#define MAXBLOCK		4096	// input buffer and out buffer
#define MAXTIMEOUTS		2000	// timeout(ms)
#define EVENTCHAR       0x0d	// event char			  
#define XON			    0x11	// DCB XonChar		
#define XOFF			0x13	// DCB XoffChar

RF_IMPLEMENT_DYNCREATE(ADB)
ADB *pADB;

bool ADB::PreRun()
{
	m_strResult = "-";
	m_strItemCode = "-";
	m_strErrorCode = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}


bool ADB::Run()
{
	char sz_input[4096] = {0};
	char sz_output[4096] = {0};
	std::string str_msg;

	bool b_ret = MyExecAdbCmd("start");
	if(!b_ret){
		str_msg = _T("MyExecAdbCmd Start FAIL");
		//m_strErrorCode = "-";
		m_strMessage = str_msg;
		FactoryLog();
	}

	str_msg = _T("MyExecAdbCmd Stop PASS ");
	m_strErrorCode = "-";
	m_strMessage = str_msg;
	FactoryLog();

	return true;
}
bool ADB::PostRun()
{
	return true;
}


bool ADB::InitData(std::map<std::string, std::string>& paramMap)
{
	/* Name */

	if (paramMap.find(std::string("Name")) != paramMap.end())
	{
		m_str_Name = paramMap[std::string("Name")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to Get filed 'Name' for class 'ADBProc'"));
		return false;
	}


	/* Value */
	if (paramMap.find(std::string("Value")) != paramMap.end())
	{
		m_str_Value = paramMap[std::string("Value")];
		m_rp_Value.SetRange(m_str_Value);
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to Get filed 'CheckMatch' for class 'ADBProc'"));
		return false;
	}

	return true;
}

bool ADB::InitData()
{
	m_iTimeOut = 10;
	return true;
}
ADB::ADB():nWaitResultTime(5000)
{
	m_bStopViewImg = false;	
}


void ADB::Report(bool bRes, string strErrorCode, string strMessage)
{
	if(bRes)
	{
		m_strResult = "PASS";
		m_strErrorCode = strErrorCode;
		m_strMessage = strMessage;
		TraceLog(MSG_INFO, m_strMessage);
	}
	else
	{
		m_strResult = "FAIL";
		m_strErrorCode = strErrorCode;
		m_strMessage = strMessage;
		TraceLog(MSG_ERROR, m_strMessage);
	}

	FactoryLog();
}
UINT ADB::AfxViewImgDlgThread(LPVOID pParam)
{
	return true;
}

ADB::~ADB()
{
}


bool ADB::MyExecAdbCmd(const char* chCmd, char **chReturn, bool bWithShell)
{
	bool ret = true;
	HANDLE hWrite, hRead;

	PROCESS_INFORMATION processInfo;
	memset(&processInfo, 0, sizeof(PROCESS_INFORMATION));

	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	try
	{
		if (! CreatePipe(&hRead, &hWrite, &saAttr, 0))
			throw _T("CreatePipe");

		STARTUPINFO si = {0};
		si.cb = sizeof (STARTUPINFO);
		si.hStdError = hWrite;
		si.hStdOutput = hWrite;
		si.hStdInput = hRead;
		si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;

		TCHAR szWorkingDir[MAX_PATH], szExePath[MAX_PATH];
		TCHAR szCmdLine[1024];

		// Set WorkPath and ExePath
		GetModuleFileName(NULL, szExePath, MAX_PATH);
		PathRemoveFileSpec(szExePath);
		_tcscpy(szWorkingDir, szExePath);

		SetCurrentDirectory(szWorkingDir);

		PathAppend(szExePath, "adb.exe");

		if (bWithShell)
			_stprintf(szCmdLine, _T("\"%s\" shell %s"), szExePath, chCmd);
		else
			_stprintf(szCmdLine, _T("\"%s\" %s"), szExePath, chCmd);

		if (! CreateProcess(NULL,	// do not send parameter
			szCmdLine,				// need enclosing quotation
			NULL,
			NULL,
			TRUE,					// important!
			CREATE_DEFAULT_ERROR_MODE,
			NULL,
			szWorkingDir,			// child process's full path.
			&si,
			&processInfo))
		{
			throw _T("CreateProcess");
		}

		WaitForSingleObject(processInfo.hProcess, INFINITE);

		// if needs return value
		if (chReturn != NULL)
		{
			DWORD dwAvail, numread;

			BOOL bRet = PeekNamedPipe (hRead, NULL, 0, NULL, &dwAvail, 0);
			if (bRet && dwAvail > 0)
			{
				*chReturn = new char [dwAvail+1];
				char *pBuffer = *chReturn;
				memset(pBuffer, 0, dwAvail+1);

				::ReadFile(hRead, pBuffer, dwAvail, &numread, 0);
			}
		}
	}
	catch (const TCHAR* szError)
	{
		LPVOID lpMsgBuf;

		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, GetLastError(),
			//MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), 
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);

		TCHAR szMessage[1024];
		_stprintf (szMessage, "%s: %s", szError, lpMsgBuf);

		LocalFree(lpMsgBuf);
		ret = false;
	}

	CloseHandle(hRead);
	CloseHandle(hWrite);
	CloseHandle(processInfo.hProcess);
	processInfo.hProcess = NULL;
	CloseHandle(processInfo.hThread);
	processInfo.hThread = NULL;

	return ret; 
}

