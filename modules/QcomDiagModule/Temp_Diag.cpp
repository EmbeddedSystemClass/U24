#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\RequestManager\Request\ODMRequest.h"


/*****************************************************************************/
//
//  Temp Function
//
/*****************************************************************************/
void GetDirFullPath (const TCHAR* szFilePath, TCHAR* szDirName, long desSize)
{
	szDirName[0] = 0;

	TCHAR *fullpath = new TCHAR[MAX_PATH];
	memset (fullpath, 0, sizeof(TCHAR) * MAX_PATH);
	TCHAR *filepart;
	DWORD dwReturn;

	dwReturn = GetFullPathName (szFilePath, MAX_PATH, fullpath, &filepart);
	if (dwReturn > MAX_PATH)
	{
		delete [] fullpath;
		fullpath = new TCHAR [dwReturn];
		dwReturn = GetFullPathName (szFilePath, MAX_PATH, fullpath, &filepart);
	}

	size_t len = _tcslen (fullpath);
	if (len > 0)
	{
		while (fullpath[--len] != '\\');
		fullpath[++len] = 0;
		_tcscpy_s (szDirName, desSize, fullpath);
		//_tcscpy (szDirName, fullpath);
	}

	delete [] fullpath;
}

void AppendToCurrentPath (TCHAR* szNewBuffer, DWORD dwBufSize, const TCHAR* szFileName)
{
	TCHAR exepath   [MAX_PATH] = {0};
	GetModuleFileName (NULL, exepath, MAX_PATH);
	GetDirFullPath (exepath, szNewBuffer, dwBufSize);
	_tcscat_s (szNewBuffer, dwBufSize, szFileName);
}

bool CreateBatFile(const char* szFileName, const char* szFFUFile)
{
	TCHAR scriptFile [MAX_PATH] = {0};

	if( strstr(szFileName,":") == NULL )
		AppendToCurrentPath (scriptFile, MAX_PATH, szFileName);
	else
		strcpy(scriptFile, szFileName);

	FILE* pFile = _tfopen (scriptFile, _T("w"));

	//create file fail
	if(!pFile)
	{
		AfxMessageBox(_T("Create BAT File Fail!"));
		return false;
	}

	TCHAR	szWorkingDir[MAX_PATH] = {0}, szFilePath[MAX_PATH],szCmdLine[MAX_PATH];

	// get child process's full path
	_tcscpy (szFilePath, _T("C:\\Program Files\\Zune\\UpdateWP.exe"));

	GetDirFullPath (szFilePath, szWorkingDir, MAX_PATH);

	// make it "C:\\Program Files\\Zune\\UpdateWP.exe" / ffu "ffu-file-name"
	wsprintf (szCmdLine, TEXT("\"%s\""), szFilePath); 
	CString strFFUFile;
	strFFUFile.Format("%s",szFFUFile);
	CString parameters = _T("\"") + strFFUFile + _T("\"");
	_tcscat (szCmdLine, _T(" /ffu "));
	_tcscat (szCmdLine, parameters);

	// make it "C:\\Program Files\\Zune\\UpdateWP.exe" /list
	_tcscat (szCmdLine, _T("\n"));
	_tcscat (szCmdLine, _T("\""));
	_tcscat (szCmdLine, szFilePath);
	_tcscat (szCmdLine, _T("\""));
	_tcscat (szCmdLine, _T(" /list "));
	
	CString cstrTmp = szCmdLine;
	CT2A szTmp(cstrTmp);
	std::string strTmp(szTmp);

	fwrite(strTmp.c_str(), strTmp.size(), 1, pFile);
	::fclose(pFile);


	return true;
}

bool DeleteBatchFile(const char* szFileName)
{
	if( NULL != strstr(szFileName,":"))
		DeleteFile(szFileName);
	else
	{
		TCHAR scriptFile [MAX_PATH] = {0};
		AppendToCurrentPath (scriptFile, MAX_PATH, szFileName);
		DeleteFile(scriptFile);
	}

	return false;
}


/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/
bool CQcomDiagModule::Temp_UpdateDFIFW(char* szErrCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	BYTE iErrCode = 0;

	CRequestPtr ptRequest = new CDiagUpdateDFIFW(bRes, iErrCode);

	bool bSuccess = Execute(ptRequest);
	bRes = bRes && bSuccess;

	if (bRes == false)
		PopLastLog();

	sprintf_s(szErrCode, 15, "%d", iErrCode);

	return bRes;
}


/*****************************************************************************/
//
//  export for SPU 
//
/*****************************************************************************/
QCOM_DIAG_API bool Temp_UpdateDFIFW_SPU (const char* szPort, char* szErrCode)
{
	return CQcomDiagModule(atoi(szPort)).Temp_UpdateDFIFW(szErrCode);
}

QCOM_DIAG_API bool GetPVKFileName_SPU(char* szListFileName, char* szPVKFileName, char* szError)
{
	srand((unsigned int)time(NULL));
	
	char*		szBufferPVK	= new char [100];
	char*		szListPath	= new char [100];
	CString		strBuffer;
	CStdioFile	fpListFile;
	
	int	iCount	= 0;
	int iLine	= (rand() % 1000) +1;

	::GetModuleFileNameA(NULL, szListPath, MAX_PATH);
	::PathRemoveFileSpecA(szListPath);
	strcat(szListPath,"\\");

	strcpy(szPVKFileName, szListPath);
	strcat(szPVKFileName, szListFileName);
	::PathRemoveFileSpecA(szPVKFileName);
	strcat(szPVKFileName, "\\");

	strcat(szListPath, szListFileName);
	
	if(!fpListFile.Open(szListPath, CFile::modeRead))
	{
		strcpy(szError, "List File NOT exists!");
		return false;
	}

	while(fpListFile.ReadString(strBuffer))
	{
		iCount++;

		if(iCount == iLine)
		{
			memset(szBufferPVK, 0, 100);
			sprintf_s(szBufferPVK, 99, "%s", strBuffer.Left(strBuffer.GetLength() - 5));

			break;
		}
	}

	strcat(szPVKFileName, szBufferPVK);

	delete [] szBufferPVK;
	delete [] szListPath;

	return true;
}

QCOM_DIAG_API bool Temp_IsConnected_Zune_SPU(const char* szCycleTimes)
{
	bool bRes = false;

	HANDLE	newstdin = NULL, write_stdin = NULL, 
		newstdout = NULL, read_stdout = NULL;

	PROCESS_INFORMATION processInfo;
	memset( &processInfo, 0, sizeof(PROCESS_INFORMATION));

	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	if (! CreatePipe (&newstdin, &write_stdin, &saAttr, 0))
		return false;//throw _T("CreatePipe stdin");

	SetHandleInformation (write_stdin, HANDLE_FLAG_INHERIT, 0);

	// Create stdout pipe
	if (! CreatePipe (&read_stdout, &newstdout, &saAttr, 0))
		return false;//throw _T("CreatePipe stdout");

	SetHandleInformation (read_stdout, HANDLE_FLAG_INHERIT, 0);


	STARTUPINFO si = {0};
	si.cb = sizeof (STARTUPINFO);
	si.hStdError = newstdout;
	si.hStdOutput = newstdout;
	si.hStdInput = newstdin;
	si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	TCHAR	szWorkingDir[MAX_PATH] = {0};


	if (! CreateProcess(_T("C:\\WINDOWS\\system32\\cmd.exe"),		// do not send parameter
		_T("/c C:\\WolfFFUList.bat"),		// need enclosing quotation
		NULL,
		NULL,
		TRUE,			// important!
		CREATE_DEFAULT_ERROR_MODE,
		NULL,
		NULL,	// child process's full path.
		&si,
		&processInfo))
	{
		//throw _T("CreateProcess");
		return false;
	}

	int		curPos			= 0;
	bool	bBatFinish		= false;
	DWORD	dwAvail, numread;//process exit code

	//while (!bBatFinish)
	//{
		
		Sleep(1000);	
		BOOL bRet = PeekNamedPipe (read_stdout, NULL, 0, NULL, &dwAvail, 0);
		

		if (bRet && dwAvail > 0)
		{
			char *csNew = new char [dwAvail+1];
			memset (csNew, 0, dwAvail+1);

			::ReadFile (read_stdout, csNew, dwAvail, &numread, 0);

			CString strBuffer (csNew) ;
			int pos = 0;

			//No Device
			pos = strBuffer.Find(_T("No devices were found"),(pos+1));
			if( -1 != pos)
			{
				//TerminateProcess (processInfo.hProcess, 0);
				bBatFinish = true;
			}

			//Error
			pos = strBuffer.Find(_T("Error"),(pos+1));
			if( -1 != pos)
			{
				//TerminateProcess (processInfo.hProcess, 0);
				bBatFinish = true;
			}

			//Device Found
			pos = strBuffer.Find(_T("Id:"),(pos+1));
			if( -1 != pos)
			{
				bRes = true;
				bBatFinish = true;
			}

			TerminateProcess (processInfo.hProcess, 0);

			// Frees Memory.
			delete [] csNew;
			strBuffer.Empty() ;
		}

		

	//}

//	DeleteBatchFile(szBatFile);
	CloseHandle (processInfo.hProcess);
	CloseHandle (processInfo.hThread);

	return bRes;
}


QCOM_DIAG_API bool Temp_FFU_List_SPU(const char* szBatFile, const char* szFFUFile, char* szSN, char* szKITLName, char* szManufacturer)
{
	bool bRes = false;

	//HWND hwnd = pThis->m_hWnd;
	//pThis->m_bForceExit = false;

	HANDLE	newstdin = NULL, write_stdin = NULL, 
		newstdout = NULL, read_stdout = NULL;  //pipe handles

	PROCESS_INFORMATION processInfo;
	memset( &processInfo, 0, sizeof(PROCESS_INFORMATION));

	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	//	try
	//	{
	// Create stdin pipe

	if (! CreatePipe (&newstdin, &write_stdin, &saAttr, 0))
		return false;

	SetHandleInformation (write_stdin, HANDLE_FLAG_INHERIT, 0);

	// Create stdout pipe
	if (! CreatePipe (&read_stdout, &newstdout, &saAttr, 0))
		return false;
	SetHandleInformation (read_stdout, HANDLE_FLAG_INHERIT, 0);


	STARTUPINFO si = {0};
	si.cb = sizeof (STARTUPINFO);
	si.hStdError = newstdout;
	si.hStdOutput = newstdout;
	si.hStdInput = newstdin;
	si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	TCHAR	szWorkingDir[MAX_PATH] = {0};

	//if(!CreateBatFile(szBatFile,szFFUFile))
	//	return 0;

	//bool a = DeleteBatchFile(szBatFile);

	//bool a = DeleteFile("D:\\Git\\PlatformTool\\Code\\Solution\\Debug\\test123.bat");


	if (! CreateProcess(_T("C:\\WINDOWS\\system32\\cmd.exe"),		// do not send parameter
		_T("/c C:\\WolfFFUDownload.bat"),		// need enclosing quotation
		NULL,
		NULL,
		TRUE,			// important!
		CREATE_DEFAULT_ERROR_MODE,
		NULL,
		NULL,	// child process's full path.
		&si,
		&processInfo))
	{
		return false;
	}

	int		curPos			= 0;
	bool	bDownloadFinish	= false;
	bool	bBatFinish		= false;
	DWORD	dwAvail, numread;//process exit code

	while ((!bBatFinish) || (curPos == 200))
	{
		BOOL bRet = PeekNamedPipe (read_stdout, NULL, 0, NULL, &dwAvail, 0);

		if (bRet && dwAvail > 0)
		{
			char *csNew = new char [dwAvail+1];
			memset (csNew, 0, dwAvail+1);

			::ReadFile (read_stdout, csNew, dwAvail, &numread, 0);


			// parse the buffer, we want to find "Installing updates. Please wait: Completed"
			CString strBuffer (csNew) ;

			CString resToken;
			int pos = 0;

			//No Device
			pos = strBuffer.Find(_T("No devices were found"),(pos+1));
			if( -1 != pos)
			{
				Sleep (2000); // ensure that the device reboot.
				TerminateProcess (processInfo.hProcess, 0);
				bBatFinish = true;
			}

			//No Internet
			pos = strBuffer.Find(_T("Make sure your computer is connected to the Internet and try again"),(pos+1));
			if( -1 != pos)
			{
				Sleep (2000); // ensure that the device reboot.
				TerminateProcess (processInfo.hProcess, 0);
				bBatFinish = true;
			}

			//other error
			pos = strBuffer.Find(_T("Error"),(pos+1));
			if( -1 != pos)
			{
				Sleep (2000); // ensure that the device reboot.
				TerminateProcess (processInfo.hProcess, 0);
				bBatFinish = true;
			}

			//FFU Download Finish
			pos = strBuffer.Find(_T("The device has been updated to version"),(pos+1));
			if( -1 != pos)
			{
				bDownloadFinish = true;
			}

			if(bDownloadFinish)
			{
				//SN
				pos = strBuffer.Find(_T("SN: "),(pos+1));
				if( -1 != pos)
				{
					int	pos_enter = -1;
					pos_enter = strBuffer.Find(_T("\n"),(pos+1));
					resToken = strBuffer.Mid(pos + _tcslen(_T("SN: ")), (pos_enter-pos));
					strcpy(szSN, (LPCTSTR)resToken);
				}

				//KITLName
				pos = strBuffer.Find(_T("KITLName: "),(pos+1));
				if( -1 != pos)
				{
					int	pos_enter = -1;
					pos_enter = strBuffer.Find(_T("\n"),(pos+1));
					resToken = strBuffer.Mid(pos + _tcslen(_T("KITLName: ")), 15);
					strcpy(szKITLName, (LPCTSTR)resToken);
				}

				//Manufacturer
				pos = strBuffer.Find(_T("Manufacturer: "),(pos+1));
				if( -1 != pos)
				{

					int	pos_enter = -1;
					pos_enter = strBuffer.Find(_T("\n"),(pos+1));
					resToken = strBuffer.Mid(pos + _tcslen(_T("Manufacturer: ")), (pos_enter-pos));

					pos = resToken.Find(_T("factory"),(pos+1));
					if( -1 == pos)
					{
						strcpy(szManufacturer, _T(" Non Factory"));
						bRes		= true;
						bBatFinish	= true;
					}
				}
			}

			// Frees Memory.
			delete [] csNew;
			strBuffer.Empty() ;
		}

		Sleep(50);
		curPos++;

	}

//	DeleteBatchFile(szBatFile);

	return bRes;
}
