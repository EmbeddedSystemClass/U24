#include "stdafx.h"
#include "Utility.h"
#include <wincrypt.h>	// Cryptographic API Prototypes and Definitions


bool IsPathExist(const CString& path)
{
	return (::GetFileAttributes(path) != INVALID_FILE_ATTRIBUTES);
}

bool IsFindFilePath(CString FullMask, CString& path)
{
	CFileFind finder;
	BOOL isFound = finder.FindFile(FullMask);
	while (isFound) {
		isFound = finder.FindNextFile();
		if ((!finder.IsDirectory())) {
			path = finder.GetFileName();
			return true;
		}
	}
	return false;
}

bool CheckOneInstance(HWND hWnd, HANDLE mutex, TCHAR* szKey)
{
	mutex = CreateMutex(NULL, TRUE, szKey);
	if (mutex) {
		if (GetLastError() == ERROR_ALREADY_EXISTS) {
			CloseHandle(mutex);
			HWND hWndPrevious = ::GetWindow(::GetDesktopWindow(), GW_CHILD);
			while (::IsWindow(hWndPrevious)) {
				if (::GetProp(hWndPrevious, szKey)) {
					if (::IsIconic(hWndPrevious)) {
						::ShowWindow(hWndPrevious, SW_RESTORE);
						::SetForegroundWindow(hWndPrevious);
					}
					else {
						::SetForegroundWindow(::GetLastActivePopup(hWndPrevious));
					}
					return false;
				}
				hWndPrevious = ::GetWindow(hWndPrevious, GW_HWNDNEXT);
			}
			return false;
		}
	}
	SetProp(hWnd, szKey, (HANDLE)1);
	return true;
}

bool Exec(CString& path, CString& param, DWORD msTimeout, bool hasResponse)
{
	if (!IsPathExist(path)) {
		return false;
	}

	HANDLE hRead, hWrite;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	DWORD nPipeSize = 1024;
	if (!CreatePipe(&hRead, &hWrite, &sa, nPipeSize)) {
		::AfxMessageBox(_T("ERROR: CreatePipe fail!\r\n"));
		return false;
	}

	TCHAR szNowPath[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szNowPath);
	TRACE(_T("current path: %s\n"), szNowPath);
	CString WorkDir = path.Left(path.ReverseFind(_T('\\')));
	TRACE(_T("working path: %s\n"), WorkDir);
	SetCurrentDirectory(WorkDir);

	bool isOk = false;
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupInfo.wShowWindow	= SW_HIDE;
	startupInfo.hStdError = hWrite;
	startupInfo.hStdOutput = hWrite;
	CString Command = path;
	if (!param.IsEmpty()) {
		Command = Command + _T(" ") + param;
	}
	TRACE(_T("exec: %s\n"), Command);
	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo)) {
		WaitForSingleObject(processInfo.hProcess, msTimeout);
		isOk = true;

		if (hasResponse) {
			DWORD bytesRead;
			char message[512];
			memset(message, 0, sizeof(message));
			::ReadFile(hRead, message, 511, &bytesRead, NULL);
			if (strstr(message, "FAIL") != NULL) { // handle FAIL
				isOk = false;
			}
			if (strlen(message) > 0) {
				param = CA2T(message);
			}
			else {
				param.Empty();
			}
		}
	}
	else {
		isOk = false;
		CString error_msg(_T("ERROR: Execute fail!\r\ncmd:"));
		error_msg += Command;
		::AfxMessageBox(error_msg);
	}
	Command.ReleaseBuffer();
	CloseHandle(hRead);
	CloseHandle(hWrite);
	SetCurrentDirectory(szNowPath);

	return isOk;
}

bool ShExec(CString& path, CString& param)
{
	TRACE(_T("path: %s\n"), path);
	TRACE(_T("param: %s\n"), param);

	TCHAR szNowPath[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szNowPath);
	TRACE(_T("current path: %s\n"), szNowPath);
	CString WorkDir = path.Left(path.ReverseFind(_T('\\')));
	TRACE(_T("working path: %s\n"), WorkDir);
	SetCurrentDirectory(WorkDir);

	SHELLEXECUTEINFO shExecInfo = {0};
	shExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	shExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	shExecInfo.hwnd = NULL;
	shExecInfo.lpFile = path.GetBuffer();
	shExecInfo.lpParameters = param.GetBuffer();
	shExecInfo.nShow = SW_HIDE;
	bool isOk = ShellExecuteEx(&shExecInfo) == TRUE;
	WaitForSingleObject(shExecInfo.hProcess, INFINITE);

	path.ReleaseBuffer();
	param.ReleaseBuffer();
	SetCurrentDirectory(szNowPath);

	return isOk;
}

static int HashSum(char* szSum, BYTE* pContent, size_t nLength, DWORD nHashLen, ALG_ID nId)
{
	int nRetCode = ERROR_FUNCTION_FAILED;

	char* pszDigest = new char[nHashLen*2 + 1];
	memset(pszDigest, 0, nHashLen*2+1);
	HCRYPTPROV hCryptProv;
	HCRYPTHASH hHash;
	BYTE* pHash = new BYTE[nHashLen];
	if (CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET)) {
		if (CryptCreateHash(hCryptProv, nId, 0, 0, &hHash)) {
			if (CryptHashData(hHash, pContent, static_cast<DWORD>(nLength), 0)) {
				if (CryptGetHashParam(hHash, HP_HASHVAL, pHash, &nHashLen, 0)) {
					// Make a string version of the numeric digest value
					char tmp[3];
					for (DWORD i = 0; i < nHashLen; i++) {
						memset(tmp, 0, 3);
						sprintf(tmp, "%02x", pHash[i]);
						strncat(pszDigest, tmp, 3);
					}
					nRetCode = NO_ERROR;
				}
				else {
					// Error getting hash param
					nRetCode = ERROR_BAD_ARGUMENTS;
				}
			}
			else {
				// Error hashing data
				nRetCode = ERROR_INVALID_DATA;
			}
		}
		else {
			// Error creating hash
			nRetCode = ERROR_INVALID_HANDLE;
		}
	}
	else {
		// Error acquiring context
		nRetCode = ERROR_NO_DATA;
	}

	CryptDestroyHash(hHash);
	CryptReleaseContext(hCryptProv, 0);
	strncpy(szSum, pszDigest, strlen(pszDigest));
	delete pHash;
	delete pszDigest;

	return nRetCode;
}

//d41d8cd98f00b204e9800998ecf8427e
int HashMd5(char* szSum, BYTE* pContent, size_t nLength)
{
	// The MD5 algorithm always returns 16 bytes.
	return HashSum(szSum, pContent, nLength, 16, CALG_MD5);
}

//da39a3ee5e6b4b0d3255bfef95601890afd80709
int HashSha1(char* szSum, BYTE* pContent, size_t nLength)
{
	// The SHA1 algorithm always returns 20 bytes.
	return HashSum(szSum, pContent, nLength, 20, CALG_SHA1);
}




