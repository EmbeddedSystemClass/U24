#include "stdafx.h"
#include "QcomDiagModule.h"
//#include "..\RequestManager\Request\BTRequest.h"
#include "..\RequestManager\Request\PhoneInfoRequest.h"
//#include "..\RequestManager\Request\eMMCRequest.h"
#include "..\QisdaNVModule\NVItem.h"
#include "..\RequestManager\A9_Request\A9_OSRequest.h"

/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/
bool CQcomDiagModule::PhoneInfo_GetPINStatus()
{
	using namespace PLATFORM_TOOL;

	bool bRes,bSuccess = false;

	CRequestPtr ptRequest = new CGetePINStatusRequest (bRes);

	bRes = Execute (ptRequest) && bRes;
	
	if (bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::A9_OS_ReadModemVersion(char* szModemVersion)
{
	using namespace PLATFORM_TOOL;

	bool bSuccess = false;
	bool bReturn = false;

	CRequestPtr ptRequest = new CA9ReadModemVersionRequest(szModemVersion, bSuccess);
	bReturn = Execute(ptRequest);

	if (bSuccess && bReturn)
		return true;

	PopLastLog();
	return false;
}

bool CQcomDiagModule::A9_OS_IsPhoneReady(char* szMaxWaitTime, char* szRetryPeriod, char* szErrMsg,
										char* szIdentify, char* szIdentifyLength)
{
	char szModemVersion[MAX_PATH] = {0} ;
	int iMaxWaitTime = atoi(szMaxWaitTime) ;
	int iRetryPeriod = atoi(szRetryPeriod) ;
	int iRetry ;

	for ( iRetry=0; iRetry<(int)(iMaxWaitTime/iRetryPeriod); ++iRetry )
	{
		if ( A9_OS_ReadModemVersion(szModemVersion) )
		{
			if ( 0!=strncmp(szModemVersion, szIdentify, atoi(szIdentifyLength)) ) {
				sprintf(szErrMsg, "Wrong Modem Version: %s", szModemVersion) ;
				return false ;
			}

			return true ;
		}

		Sleep(iRetryPeriod) ;
	}

	sprintf(szErrMsg, "Fail after %d times retry", iRetry) ;
	return false ;
}


/*****************************************************************************/
//
//  export for SPU 
//
/*****************************************************************************/

QCOM_DIAG_API bool PhoneInfo_GetPINStatus_SPU(const char* szPort)
{
	CQcomDiagModule diag(atoi(szPort));
	return diag.PhoneInfo_GetPINStatus();
}


QCOM_DIAG_API bool PhoneInfo_GetIMEI_SPU(const char* szPort, char* szAddr)
{
	TSTRING strAddr;
	strAddr = CNVItem(atoi(szPort)).GetIMEI();

#ifdef _UNICODE
	wcstombs(szAddr, strAddr.c_str(), 15);
#else
	sprintf(szAddr, "%s", strAddr.c_str());
#endif

	if(strlen(szAddr) == 15)
		return true;
	else
		return false;

	return false;
}

QCOM_DIAG_API bool PhoneInfo_WriteIMEI_SPU(const char* szPort, char* szAddr)
{
	bool bRes = false;
	
	bRes = CNVItem(atoi(szPort)).WriteIMEI(szAddr);

	if(bRes)
		return true;
	else
		return false;

	return false;
}

QCOM_DIAG_API bool PhoneInfo_FFU_List_SPU(char* szSN, char* szKITLName, char* szManufacturer)
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
	{
		//throw _T("CreatePipe stdin");
		return false;
	}
	SetHandleInformation (write_stdin, HANDLE_FLAG_INHERIT, 0);

	// Create stdout pipe
	if (! CreatePipe (&read_stdout, &newstdout, &saAttr, 0))
	{
		//throw _T("CreatePipe stdout");
		return false;
	}
	SetHandleInformation (read_stdout, HANDLE_FLAG_INHERIT, 0);


	STARTUPINFO si = {0};
	si.cb = sizeof (STARTUPINFO);
	si.hStdError = newstdout;
	si.hStdOutput = newstdout;
	si.hStdInput = newstdin;
	si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	TCHAR	szWorkingDir[MAX_PATH] = {0}, szFilePath[MAX_PATH];

	// get child process's full path
	_tcscpy (szFilePath, _T("C:\\Program Files\\Zune\\UpdateWP.exe"));

	//GetDirFullPath (szFilePath, szWorkingDir, MAX_PATH);

	// make it ""C:\\Program Files\\Zune\\UpdateWP.exe" / ffu "ffu file name""
	//wsprintf (szCmdLine, TEXT("\"%s\""), szFilePath); 
	//CString parameters = _T("\"") + pThis->m_ffuPath + _T("\"");
	//_tcscat (szCmdLine, _T(" /ffu "));
	//_tcscat (szCmdLine, parameters);

	//parameters.Empty() ;

	//pThis->m_bIsDownloading = true;


	//if(!pThis->CreateBatFile(_T("test.bat"),pThis->m_ffuPath, true))
	//	return 0;

	if (! CreateProcess(_T("C:\\WINDOWS\\system32\\cmd.exe"),		// do not send parameter
		_T("/c D:\\test.bat"),		// need enclosing quotation
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
	bool	bDownloadFinish	= false;
	bool	bBatFinish		= false;
	DWORD	dwAvail, numread;//process exit code

	while (!bBatFinish)
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

	}

	CloseHandle (processInfo.hProcess);
	CloseHandle (processInfo.hThread);

	return bRes;
}
QCOM_DIAG_API bool PhoneInfo_C7FreezeOTP_SPU(const char* szPort, char *szStatus)
{
	bool bRes = false;
	int nStatus = 199;
	bRes = CNVItem(atoi(szPort)).C7FreezeOTP(1,nStatus);//1.Freeze OTP 2.Erase OTP 3.Erase EFS

	sprintf(szStatus, "%x", nStatus); 
	
	if(bRes )
		return true;
	else
		return false;

	return false;
}


QCOM_DIAG_API bool A9_OS_IsPhoneReady_SPU (const char* szPort, char* szMaxWaitTime, char* szRetryPeriod, char* szErrMsg)
{
	CQcomDiagModule diag(atoi(szPort));
	return diag.A9_OS_IsPhoneReady(szMaxWaitTime, szRetryPeriod, szErrMsg);
}