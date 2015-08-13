#include "stdafx.h"
#include "ADBSimple.h"


CSimpleADB::CSimpleADB(const char* chSerial, const char* chWorkingDir)
{
	memset (&processInfo, 0, sizeof (PROCESS_INFORMATION));

	memset (m_chSerial, 0, 512);
	if (chSerial && chSerial[0])
		strcpy (m_chSerial, chSerial);

	memset (m_chWorkingDir, 0, 512);
	if (chWorkingDir && chWorkingDir[0])
	{
		strcpy (m_chWorkingDir, chWorkingDir);
		if (m_chWorkingDir[strlen(chWorkingDir)] != '\\')
			strcat (m_chWorkingDir, "\\");
	}
	else
	{
		GetModuleFileName (NULL, m_chWorkingDir, MAX_PATH);
		int len = strlen (m_chWorkingDir);
		while (m_chWorkingDir[--len] != '\\');
		m_chWorkingDir[++len] = 0;
	}

	m_hAdbEvent = CreateEvent (NULL, FALSE, TRUE, _ADB_EVENT_NAME);

	ShellExecute (NULL, "open", "adb", "kill-server", m_chWorkingDir, SW_HIDE);
	Sleep (300);

	ShellExecute (NULL, "open", "adb", "root", m_chWorkingDir, SW_HIDE);
	Sleep (500);
}

CSimpleADB::~CSimpleADB()
{
	if (processInfo.hProcess)
	{
		CloseHandle (processInfo.hProcess);
		processInfo.hProcess = NULL;
	}

	if (processInfo.hThread)
	{
		CloseHandle (processInfo.hThread);
		processInfo.hThread = NULL;
	}

	
	WaitForSingleObject (m_hAdbEvent, INFINITE);

	ShellExecute (NULL, "open", "taskkill", "/F /IM adb.exe", m_chWorkingDir, SW_HIDE);// kill "adb.exe"
	ShellExecute (NULL, "open", "taskkill", "/F /IM fastboot.exe", m_chWorkingDir, SW_HIDE);// kill "fastboot.exe"

	SetEvent (m_hAdbEvent);


	CloseHandle (m_hAdbEvent);
}

bool CSimpleADB::SendAdbCmd(const char *chCmd, char *chReturn, long dwReturnLen, int timeout)
{
	WaitForSingleObject (m_hAdbEvent, 30000);
	bool ret = MyExecAdbCmd ("adb", chCmd, chReturn, dwReturnLen, timeout);
	SetEvent (m_hAdbEvent);

	return ret;
}

bool CSimpleADB::SendAdbShellCmd(const char *chCmd, char *chReturn, long dwReturnLen, int timeout)
{
	int len = strlen (chCmd) + 16;
	char *chShellCmd = new char [len];
	sprintf (chShellCmd, "shell %s", chCmd);

		
	WaitForSingleObject (m_hAdbEvent, 30000);
	
	bool ret = MyExecAdbCmd ("adb", chShellCmd, chReturn, dwReturnLen, timeout);

	SetEvent (m_hAdbEvent);
	
	delete [] chShellCmd;
	return ret;
}

bool CSimpleADB::SendFastbootCmd (const char* chCmd, char *chReturn, long dwReturnLen, int timeout)
{
	return MyExecAdbCmd ("fastboot", chCmd, chReturn, dwReturnLen, timeout);
}




bool CSimpleADB::MyExecAdbCmd (const char* chExeName, const char* chCmd, char *chReturn, long dwReturnLen, int timeout)
{
	bool ret = true;
	HANDLE hWrite, hRead;

	char chOriCurrentDir[512] = {0};
	GetCurrentDirectory (512, chOriCurrentDir);


	memset( &processInfo, 0, sizeof(PROCESS_INFORMATION));

	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	try
	{
		if (! CreatePipe (&hRead, &hWrite, &saAttr, 2097152))
			throw _T("CreatePipe");


		STARTUPINFO si = {0};
		si.cb = sizeof (STARTUPINFO);
		si.hStdError = hWrite; //newstdout;
		si.hStdOutput = hWrite; //newstdout;
		si.hStdInput = hRead; //newstdin;
		si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;

		TCHAR szExePath[MAX_PATH];
		TCHAR szCmdLine[1024];


		SetCurrentDirectory (m_chWorkingDir);
		strcpy (szExePath, m_chWorkingDir);
		strcat (szExePath, chExeName);



		// Formatting: ""C:\\Program Files\\Zune\\UpdateWP.exe" / ffu "ffu file name""
		if (strstr (chCmd, "devices") != NULL)
		{
			// enumerate devices, do not need serial number
			_stprintf (szCmdLine, _T("\"%s\" %s"), szExePath, chCmd);
		}
		else
		{
			if (m_chSerial[0] != 0)
				_stprintf (szCmdLine, _T("\"%s\" -s %s %s"), szExePath, m_chSerial, chCmd);
			else
				_stprintf (szCmdLine, _T("\"%s\" %s"), szExePath, chCmd);
		}

		if (! CreateProcess (NULL, //szExePath,		// do not send parameter
							szCmdLine,		// need enclosing quotation
							NULL,
							NULL,
							TRUE,			// important!
							CREATE_DEFAULT_ERROR_MODE,
							NULL,
							m_chWorkingDir,	// child process's full path.
							&si,
							&processInfo))
		{
			throw _T("CreateProcess");
		}

		WaitForSingleObject (processInfo.hProcess, timeout);

		// if needs return value
		if (chReturn != NULL)
		{
			memset (chReturn, 0, dwReturnLen);

			long lReceived = 0;
			char *pPtr = chReturn;

			while (lReceived < dwReturnLen)
			{
				DWORD dwAvail, numread;
				BOOL bRet = PeekNamedPipe (hRead, NULL, 0, NULL, &dwAvail, 0);
				if (! bRet)
					throw _T("PeekNamedPipe");

				if (0 == dwAvail)
					break;
				
				char *pBuffer = new char [dwAvail+1];
				memset (pBuffer, 0, dwAvail+1);

				if (! ::ReadFile (hRead, pBuffer, dwAvail, &numread, 0))
					throw _T("ReadFile");

				long EffByte = numread;
				if (EffByte > dwReturnLen-lReceived)
					EffByte = dwReturnLen-lReceived;

				memcpy (pPtr, pBuffer, EffByte);
				pPtr += EffByte;
				lReceived += EffByte;
					
				delete [] pBuffer;

			} //while (lReceived < dwReturnLen)

			*pPtr = 0;
		}
		
	}
	catch (const TCHAR* szError)
	{
		LPVOID lpMsgBuf;

		FormatMessage (	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
						NULL, GetLastError(),
						//MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), 
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						(LPTSTR)&lpMsgBuf,
						0, NULL );

		TCHAR szMessage[1024];
		_stprintf (szMessage, "%s: %s", szError, lpMsgBuf);
		
		LocalFree(lpMsgBuf);
		ret = false;

	}

	CloseHandle (hRead);
	CloseHandle (hWrite);
	CloseHandle (processInfo.hThread); processInfo.hThread = NULL;
	CloseHandle (processInfo.hProcess);processInfo.hProcess = NULL;
	


	SetCurrentDirectory (chOriCurrentDir);

	return ret;
}
