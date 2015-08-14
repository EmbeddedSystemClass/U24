#include "stdafx.h"
#include "ExecCmdUtil.h"
#include <sstream>

namespace ExecCmdUtil
{
	#define READ_BUF 512
   //
	unsigned long ExecuteCmdBlockingWithTimeout( char *szExeName, const char *szCmdLineOrFileName, char *szWorkingDir , char **chReturn , int iDuration /*= 5000*/)
	{
		unsigned long ret = 0;
		HANDLE hWrite, hRead;

		PROCESS_INFORMATION processInfo;

		// PROCESS_INFORMATION processInfo;
		memset(&processInfo, 0, sizeof(PROCESS_INFORMATION));

		SECURITY_ATTRIBUTES saAttr;
		saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
		saAttr.bInheritHandle = TRUE;
		saAttr.lpSecurityDescriptor = NULL;

		try
		{
			if (! CreatePipe(&hRead, &hWrite, &saAttr, 0))
				throw "CreatePipe";

			STARTUPINFOA si = {0};
			si.cb = sizeof (STARTUPINFO);
			si.hStdError = hWrite;
			si.hStdOutput = hWrite;
			si.hStdInput = hRead;
			si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_HIDE;

			char szExePath[MAX_PATH] = {0};
			char szCmdLine[1024] = {0};

			// Set WorkPath and ExePath
			strcpy_s(szExePath, MAX_PATH, szWorkingDir);
			PathAppendA(szExePath, szExeName);

			//sprintf_s(szCmdLine, 1024, "\"%s\" %s", szExePath, szCmdLineOrFileName);
			sprintf_s(szCmdLine, 1024, "%s %s", szExePath, szCmdLineOrFileName);

			if (! CreateProcessA(
				NULL,
				szCmdLine,
				NULL,
				NULL,
				TRUE,
				CREATE_DEFAULT_ERROR_MODE,
				NULL,
				szWorkingDir,
				&si,
				&processInfo
				))
			{
				throw "CreateProcess";
			}


			WaitForSingleObject(processInfo.hProcess, iDuration);

			// if needs return value
			if (chReturn != NULL)
			{
				DWORD dwAvail, numread;

				BOOL bRet = PeekNamedPipe(hRead, NULL, 0, NULL, &dwAvail, 0);
				if (bRet && dwAvail > 0)
				{
					*chReturn = new char [dwAvail+1];
					char *pBuffer = *chReturn;
					memset (pBuffer, 0, dwAvail+1);

					::ReadFile (hRead, pBuffer, dwAvail, &numread, 0);
				}
			}
		}
		catch (const TCHAR* szError)
		{
			LPVOID lpMsgBuf;

			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL, GetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf,
				0, NULL);

			char szMessage[1024] = {0};
			sprintf_s(szMessage, 1024, "%s: %s", szError, lpMsgBuf);

			LocalFree(lpMsgBuf);
			ret = GetLastError();
		}

		CloseHandle(hRead);
		CloseHandle(hWrite);
		CloseHandle(processInfo.hProcess);
		processInfo.hProcess = NULL;
		CloseHandle(processInfo.hThread);
		processInfo.hThread = NULL;

		return ERROR_SUCCESS;
	}


	unsigned long ExecuteCmdBlocking( char *szExeName, char *szCmdLineOrFileName, char *szWorkingDir , string *pstrResult)
	{
		//proc info
		PROCESS_INFORMATION processInfo;
		memset( &processInfo, 0, sizeof(PROCESS_INFORMATION));

		//security attr
		SECURITY_ATTRIBUTES saAttr;
		saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
		saAttr.bInheritHandle = TRUE;
		saAttr.lpSecurityDescriptor = NULL;

		//init pipe for stdout
		HANDLE hSTDOUTRead, hSTDOUTWrite;
		if( !CreatePipe(&hSTDOUTRead, &hSTDOUTWrite, &saAttr,0) )
		{
			return ERROR_BAD_PIPE;
		}

		//init pipe for stdin
		//HANDLE hSTDINRead, hSTDINWrite;
		//if( !CreatePipe(&hSTDINRead, &hSTDINWrite, &saAttr,0) )
		//{
		//	return -1;
		//}

		//start up  info
		STARTUPINFOA si = {0};
		si.cb = sizeof (STARTUPINFO);
		si.hStdError = hSTDOUTWrite;   // redirect stderr to hSTDOUTWrite
		si.hStdOutput = hSTDOUTWrite;  // redirect hStdOutput to hSTDOUTWrite
		//si.hStdInput = hSTDINRead;   // a pipe can into 
		si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;

		// Set WorkPath and ExePath
		char szExePath[MAX_PATH] = {0};
		char szCmdLine[1024] = {0};

		if (szExeName != NULL)
		{
			strcpy_s(szExePath, MAX_PATH, szWorkingDir);
			PathAppendA(szExePath, szExeName);
			sprintf_s(szCmdLine, 1024, "%s   %s", szExePath, szCmdLineOrFileName);
			//sprintf_s(szCmdLine, 1024, "\"%s\" %s", szExePath, szCmdLineOrFileName);
		}
		else
		{
			strcpy_s(szExePath, MAX_PATH, szWorkingDir);
			PathAppendA(szExePath, szCmdLineOrFileName);
			strcpy_s(szCmdLine, 1024, szExePath);
		}


		// Create process
		if (! CreateProcessA (
			NULL,						// AppName,  do not send parameter
			szCmdLine,					// CmdLine, need enclosing quotation
			NULL,
			NULL,
			TRUE,						// Important!
			CREATE_DEFAULT_ERROR_MODE,  // No CREATE_DEFAULT_ERROR_MODE , use ANSI  
			NULL,
			szWorkingDir,				// Child process's full path.
			&si,
			&processInfo))
		{
			throw _T("CreateProcess");
		}

		//!!!  important , 
		//!!!  do this , so ReadFile can return 0 to know child  process stopped
		::CloseHandle( hSTDOUTWrite );

		//process child process output
		DWORD numread;
		char buf[READ_BUF];
		std::ostringstream out;	

		while(1)
		{
			// get one char one time ,  2nd char is null 
			memset(buf,'\0', READ_BUF);
			if ( ReadFile (hSTDOUTRead, &buf[0], READ_BUF-1 , &numread, 0) == 0 )
			{
				if (GetLastError() == ERROR_BROKEN_PIPE)
				{    //normal ending 
					break;
				}
				else
				{
					CloseHandle (hSTDOUTRead);
					CloseHandle (processInfo.hProcess); processInfo.hProcess = NULL;
					CloseHandle (processInfo.hThread); processInfo.hThread = NULL;
					return GetLastError();
				}
			}

			// if got one char ,  append to string
			if ( numread > 0 && numread <= READ_BUF-1 )
			{
				out<<buf;
			}
			else
				break;
		}

		//complete return result
		*pstrResult = out.str();

		CloseHandle (hSTDOUTRead);
		CloseHandle (processInfo.hProcess); processInfo.hProcess = NULL;
		CloseHandle (processInfo.hThread); processInfo.hThread = NULL;

		return ERROR_SUCCESS;

	}






	unsigned long ExecuteCmdCallback( char *szExeName, char *szCmdLineOrFileName, char *szWorkingDir , p_fn_callback pcallback)
	{
		//proc info
		PROCESS_INFORMATION processInfo;
		memset( &processInfo, 0, sizeof(PROCESS_INFORMATION));

		//security attr
		SECURITY_ATTRIBUTES saAttr;
		saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
		saAttr.bInheritHandle = TRUE;
		saAttr.lpSecurityDescriptor = NULL;

		//init pipe for stdout
		HANDLE hSTDOUTRead, hSTDOUTWrite;
		if( !CreatePipe(&hSTDOUTRead, &hSTDOUTWrite, &saAttr,0) )
		{
			return ERROR_BAD_PIPE;
		}

		//init pipe for stdin
		//HANDLE hSTDINRead, hSTDINWrite;
		//if( !CreatePipe(&hSTDINRead, &hSTDINWrite, &saAttr,0) )
		//{
		//	return -1;
		//}

		//start up  info
		STARTUPINFOA si = {0};
		si.cb = sizeof (STARTUPINFO);
		si.hStdError = hSTDOUTWrite;   // redirect stderr to hSTDOUTWrite
		si.hStdOutput = hSTDOUTWrite;  // redirect hStdOutput to hSTDOUTWrite
		//si.hStdInput = hSTDINRead;   // a pipe can into 
		si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;

		// Set WorkPath and ExePath
		char szExePath[MAX_PATH] = {0};
		char szCmdLine[1024] = {0};

		if (szExeName != NULL)
		{
			strcpy_s(szExePath, MAX_PATH, szWorkingDir);
			PathAppendA(szExePath, szExeName);
			sprintf_s(szCmdLine, 1024, "\"%s\" %s", szExePath, szCmdLineOrFileName);
		}
		else
		{
			strcpy_s(szExePath, MAX_PATH, szWorkingDir);
			PathAppendA(szExePath, szCmdLineOrFileName);
			strcpy_s(szCmdLine, 1024, szExePath);
		}


		// Create process
		if (! CreateProcessA (
			NULL,						// AppName,  do not send parameter
			szCmdLine,					// CmdLine, need enclosing quotation
			NULL,
			NULL,
			TRUE,						// Important!
			CREATE_DEFAULT_ERROR_MODE,  // No CREATE_DEFAULT_ERROR_MODE , use ANSI  
			NULL,
			szWorkingDir,				// Child process's full path.
			&si,
			&processInfo))
		{
			throw _T("CreateProcess");
		}

		//!!!  important , 
		//!!!  do this , so ReadFile can return 0 to know child  process stopped
		::CloseHandle( hSTDOUTWrite );

		//process child process output
		DWORD numread;
		char c[2];
		std::ostringstream out;	

		while(1)
		{
			// get one char one time ,  2nd char is null 
			c[1]='\0';
			BOOL bRet = ReadFile (hSTDOUTRead, &c[0], 1 , &numread, 0);
			if (  bRet == 0 )
			{
				if (GetLastError() == ERROR_BROKEN_PIPE)
				{    //normal ending 
					break;
				}
				else
				{
					CloseHandle (hSTDOUTRead);
					CloseHandle (processInfo.hProcess); processInfo.hProcess = NULL;
					CloseHandle (processInfo.hThread); processInfo.hThread = NULL;
					return GetLastError();
				}
			}

			// if got one char ,  append to string
			if ( numread > 0 )
			{
				out<<c;
			}

			//if detect \n , process one line 
			if ( c[0]== '\n')
			{
				string tmp = out.str();
				pcallback( tmp.c_str());

				//clean this line
				out.str("");
				out.clear();
			}
		}


		CloseHandle (hSTDOUTRead);
		CloseHandle (processInfo.hProcess); processInfo.hProcess = NULL;
		CloseHandle (processInfo.hThread); processInfo.hThread = NULL;

		return ERROR_SUCCESS;

	}


}
