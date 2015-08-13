#include "stdafx.h"
#include <cstddef> // size_t

#include "../CmdType.h"
#include "GeneralDLLApi.h" // GeneraCmd
#include "NvdaDLLApi.h"
#include "io.h"
#include "../Utility.h"
#include "../FactoryBlock.h"
#include <psapi.h> // GetModuleFileNameEx
#pragma comment(lib, "psapi")

typedef bool (__cdecl *PicassoCmd)(char*, char*, char*);
typedef bool (__cdecl *ImeiStateUpdateCmd)(char*, char*, char*);


static int DtCmdNvda(int nPort, int nTimeout, char* command,
					 char* InBuffer, size_t InSize, char* OutBuffer,
					 const char* error_code, const char* Func)
{
	int nRetCode = -1*ERROR_FUNCTION_FAILED;
	for (int nTry = 0; nTry < NV_DT_CMD_MAX_RETRY; nTry++) {
		if (InBuffer != NULL && OutBuffer != NULL) {
			TRACE(_T("DtCmdNvda GeneralCmdInOut "));
			nRetCode = GeneralCmdInOut(nPort, nTimeout, command, InBuffer, InSize, OutBuffer);
			if (nRetCode != HS_OK) {
				strncat(OutBuffer, error_code, strlen(error_code));
			}
		}
		else if (InBuffer != NULL) {
			TRACE(_T("DtCmdNvda GeneralCmdIn "));
			size_t nInLength = (InSize == 0) ? strlen(InBuffer) : InSize;
			nRetCode = GeneralCmdIn(nPort, nTimeout, command, InBuffer, nInLength);
		}
		else if (OutBuffer != NULL) {
			TRACE(_T("DtCmdNvda GeneralCmdOut "));
			nRetCode = GeneralCmdOut(nPort, nTimeout, command, OutBuffer);
			if (error_code != NULL && nRetCode != HS_OK) {
				strncat(OutBuffer, error_code, strlen(error_code));
			}
		}
		else {
			TRACE(_T("DtCmdNvda GeneralCmd "));
			nRetCode = GeneralCmd(nPort, nTimeout, command);
			if (error_code != NULL && nRetCode != HS_OK) {
				strncat(OutBuffer, error_code, strlen(error_code));
			}
		}
		TRACE(_T("%s(%d)\n"), Func, nRetCode);
		if (nRetCode == HS_OK) {
			break;
		}
		else {
			Sleep(500);
		}
	}

	return nRetCode;
}

void SetFaFlag(char* InBuffer, const int& nStation, char* pflag)
{
	if (strlen(InBuffer) == 0) {
		memset(pflag, 0, 4);
	}
	else {
		memcpy(pflag, InBuffer, 4);
	}
}

bool ExecAdbOut(CString Command, char* output, char* ErrorCode)
{
	bool isOk = false;
	DWORD nPipeSize = 1024 * 1024; //1M pipeline

	CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));

	CString path_adb = pthToolDir + _T("\\adb.exe");
	if (_taccess(path_adb, 0) == -1) {
		strcpy(ErrorCode, "ERROR: No adb.exe exist!");
		return false;
	}

	HANDLE hRead, hWrite;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, nPipeSize)) {
		strcpy(ErrorCode, "ERROR: CreatePipe fail!");
		return false;
	}

	HANDLE hProcess = NULL;
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
	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo)) {
		Command.ReleaseBuffer();
		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, 10*1000); // timeout in 10 seconds

		//if timeout then exit the process
		if(TimeOutSignal == WAIT_TIMEOUT) {
			isOk = false;
			TerminateProcess(processInfo.hProcess, 0);
			strcpy(ErrorCode, "ERROR: Adb timeout");
		}
		else {
			isOk = true;
			DWORD bytesRead;
			char *message = new char[nPipeSize];
			memset(message, 0, sizeof(message));
			::ReadFile(hRead, message, nPipeSize, &bytesRead, NULL);
			message[bytesRead] = '\0';

			strcpy(output, message);
			strcpy(ErrorCode, "Adb command ok");
			delete [] message;
		}
	}
	else {
		isOk = false;
		strcpy(ErrorCode, "ERROR: Execute adb.exe fail!");
	}

	CloseHandle(hRead);
	CloseHandle(hWrite);
	CloseHandle(hProcess);
	hProcess = NULL;

	return isOk;
}

int EnterFtdMode(int nTimeout, char* ErrorCode)
{
	CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));

	CString adb_path = pthToolDir + _T("\\adb.exe");
	CString param = _T("shell ftd");

	bool isOk;
	int nRetCode = -1*ERROR_FUNCTION_FAILED;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	isOk = Exec(adb_path, param, nTimeout, true);
	
	if(isOk) {
		if(param.Find(_T("system/core/ftd")) != -1) {
			nRetCode = HS_OK;
		}
		else {
			strcpy(ErrorCode, "ERROR: Enter ftd mode fail");
			nRetCode = -1*ERROR_BAD_PROFILE;
		}
	} 
	else {
		nRetCode = -1*ERROR_BAD_COMMAND;
	}

	return nRetCode;
}

int CheckFtdMode(char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	bool isOk;
	int nRetCode = -1*ERROR_FUNCTION_FAILED;
	char *msgBuffer = new char[1024 * 1024 * 2];
	isOk = ExecAdbOut("shell ps", msgBuffer, ErrorCode);

	if(isOk) {
		CString outBuffer = msgBuffer;
		if(outBuffer.Find(_T("ftd")) != -1) {
			nRetCode = HS_OK;
		}
		else {
			strcpy(ErrorCode, "ERROR: Not in ftd mode");
			nRetCode = -1*ERROR_BAD_PROFILE;
		}
	} 
	else {
		nRetCode = -1*ERROR_BAD_COMMAND;
	}
	delete[] msgBuffer;

	return nRetCode;
}

// if has value => true
bool CheckFlagStation(const CString& flag)
{
	bool hasValue = false;
	for (int i = 0; i < 8; i++) {
		if (flag.GetAt(i) != '0') {
			hasValue = true;
			break;
		}
	}
	return hasValue;
}

bool IsADBLive() {
	DWORD aProcesses[1024], cbNeeded, cbMNeeded;
	HMODULE hMods[1024];
	char szProcessName[MAX_PATH];
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
		return FALSE;
	}

	for (int i = 0; i < (int)(cbNeeded / sizeof(DWORD)); i++) {
		hProcess = OpenProcess(PROCESS_ALL_ACCESS,
			FALSE, aProcesses[i]);
		EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbMNeeded);
		GetModuleFileNameEx(hProcess, hMods[0], szProcessName, sizeof(szProcessName));
		if (strstr(_strupr(szProcessName), "ADB.EXE"))
			TerminateProcess(hProcess, 0);
		CloseHandle(hProcess);
		hProcess = NULL;	
	}

	return true;
}

CString GetFaInfoByFtd(byte* FaInfo, int start, const int length)
{
	CString sOutData;
	const int BUF_SIZE = 1024;
	unsigned char szFaOutData[BUF_SIZE] = {0};

	memcpy(szFaOutData, &FaInfo[start], length);
	char* szFaOutDataToString = new char[length*2+1];
	memset(szFaOutDataToString, 0, sizeof(szFaOutDataToString));
	if (start == FA_MOBILE_ID_START) {
		for (int i = 0; i < length; i++) {
			sprintf(&szFaOutDataToString[i*2], "%02x", szFaOutData[i]);
		}
		sOutData = szFaOutDataToString;
		delete szFaOutDataToString;
	}
	else {
		sOutData = szFaOutData;
	}

	return sOutData;
}

void WritePicsDataTB(const bool& isPass, const int& nSlot, const CString& station, const CString mobile_id,
					 const CString imei, const CString model_name)
{	
	TRACE(_T("%s slot:%d\n"), _T(__FUNCTION__), nSlot);

	SYSTEMTIME tLocal;
	GetLocalTime(&tLocal);
	CString nowtime;
	nowtime.Format(_T("%02d%02d%02d"), tLocal.wHour, tLocal.wMinute, tLocal.wSecond);

	char szComuterName[MAX_COMPUTERNAME_LENGTH] = {0};
	DWORD nNameLength = MAX_COMPUTERNAME_LENGTH;
	GetComputerNameA(szComuterName, &nNameLength);
	CString tLine;    //04. test line
	CString tStation; //05. test station
	CString tSlot;    //06. test slot
	tLine.Format(_T("%02d"), atoi(&szComuterName[1]) % 100);
	tStation.Format(_T("%02d"), atoi(&szComuterName[4]) % 100);
	tSlot.Format("%d", nSlot);

	CString path(_T("c:\\picsdata\\"));
	if (!IsPathExist(path)) {
		CreateDirectory(path, NULL);
	}

	if(station.Find(_T("EID")) != -1) {
		path += mobile_id;
		path += imei;
		path += _T("_");
	}
	else {
		path += mobile_id;
		path += _T("_");
	}
	path += nowtime;
	path += _T(".");
	path += model_name;
	path += _T(station);

	CFile cf;
	CString TB_Str = _T("-1");
	if (cf.Open(path, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite)) {
		//01. so number
		cf.Write(TB_Str, TB_Str.GetLength());
		cf.Write("\r\n", 2);

		//02. mobile id
		cf.Write(TB_Str, TB_Str.GetLength());
		cf.Write("\r\n", 2);
		//03. model name
		cf.Write(TB_Str, TB_Str.GetLength());
		cf.Write("\r\n", 2);

		//04. test line
		cf.Write(TB_Str, TB_Str.GetLength());
		cf.Write("\r\n", 2);
		//05. test station
		cf.Write(TB_Str, TB_Str.GetLength());
		cf.Write("\r\n", 2);
		//06. test slot
		cf.Write(TB_Str, TB_Str.GetLength());
		cf.Write("\r\n", 2);

		//07. OK: PASS, NG: FAIL
		//08. show PASS or show error code
		if (isPass) {
			cf.Write("OK\r\n", (UINT)strlen("OK\r\n"));
			cf.Write("PASS\r\n", (UINT)strlen("PASS\r\n"));
		}
		else {
			cf.Write("NG\r\n", (UINT)strlen("NG\r\n"));
			cf.Write("N/A\r\n", (UINT)strlen("N/A\r\n"));
		}

		//09. hardware versino
		cf.Write(TB_Str, TB_Str.GetLength());
		cf.Write("\r\n", 2);
		//10. software version
		cf.Write(TB_Str, TB_Str.GetLength());
		cf.Write("\r\n", 2);
		//11. program version
		cf.Write(TB_Str, TB_Str.GetLength());
		cf.Write("\r\n", 2);
		//12. CMU Serial number
		cf.Write("NA", static_cast<UINT>(strlen("NA")));
		cf.Write("\r\n", 2);
		//13. Lmt file version, test spec
		cf.Write(TB_Str, TB_Str.GetLength());
		cf.Write("\r\n", 2);
		//14. tcfreep file version, Test configuration
		cf.Write(TB_Str, TB_Str.GetLength());
		cf.Write("\r\n", 2);

		//15. PICASSO number
		cf.Write(TB_Str, TB_Str.GetLength());
		cf.Write("\r\n", 2);

		cf.Close();
	}
}

bool WriteTLineTroubleShootingPics()
{
	const int BUF_SIZE = 4096;
	unsigned char FaInfo[BUF_SIZE];
	int nTimeout = 10 * CLOCKS_PER_SEC;
	const int PORT_TCP = 2000;
	const int LENGTH_MOBILE = 11;
	const int LENGTH_IMEI = 15;
	memset(FaInfo, 0, BUF_SIZE);

	int nRetCode = NVDA_FACTORY_FARead(PORT_TCP, nTimeout, (char*)&FaInfo);
	if (nRetCode != HS_OK) {
		return false;
	}
	CString mobile_id = GetFaInfoByFtd(FaInfo, FA_MOBILE_ID_START, LENGTH_MOBILE);
	CString imei = GetFaInfoByFtd(FaInfo, FA_IMEI_START, LENGTH_IMEI);
	CString model_name = _T("GLUNB1A");

	WritePicsDataTB(false, 1, _T("TRG"), mobile_id, imei, model_name);
	WritePicsDataTB(false, 1, _T("BBT"), mobile_id, imei, model_name);

	return true;
}

typedef bool (__cdecl *QueryMobileIDByPicassoType)(char*, char*, char*, char*, char*);
int NVDA_UpdateMobileIDByPicasso(char* szModelName, char* szPicasso, CString sMobileId)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	int nRetCode = HS_OK;
	CString sDll;
	::GetModuleFileName(NULL, sDll.GetBuffer(MAX_PATH), MAX_PATH);
	sDll.ReleaseBuffer();
	sDll = sDll.Left(sDll.ReverseFind('\\'));
	sDll += _T("\\Pics_SSSO2_Factory_StaticM.dll");
	HMODULE hmLib = ::LoadLibrary(sDll);
	if (hmLib == NULL) {
		CString msg;
		msg.Format(_T("Load: %s Fail!"), sDll);
		AfxMessageBox(msg);
		return -1*ERROR_DLL_INIT_FAILED;
	}
	QueryMobileIDByPicassoType pFnUpdateMobileIDByPicasso = (QueryMobileIDByPicassoType)GetProcAddress(hmLib, "UpdateMobileIDByPicasso_SSS02");
	if (pFnUpdateMobileIDByPicasso == NULL) {
		::FreeLibrary(hmLib);
		hmLib = NULL;
		CString message;
		message = _T("Get DLL Function: UpdateMobileIDByPicasso_SSS02 Fail!");
		AfxMessageBox(message);
		return -1*ERROR_FUNCTION_NOT_CALLED;
	}
	char szMobileId[32] = {0};
	strcpy(szMobileId, sMobileId);
	char ostrErrCode[1024];
	char ostrErrMsg[1024];
	if ( !pFnUpdateMobileIDByPicasso(szModelName, szPicasso, szMobileId, ostrErrCode, ostrErrMsg) ) {
		char error_string[512] = {0};
		strcat(error_string, "[");
		strncat(error_string, ostrErrCode, strlen(ostrErrCode));
		strcat(error_string, "] ");
		strncat(error_string, ostrErrMsg, strlen(ostrErrMsg));
		nRetCode = -1*ERROR_INVALID_DATA;
		CString err_msg;
		err_msg.Format(_T("Fail to update Mobile ID: %s."), error_string);
		::AfxMessageBox(err_msg);
	}
	if (hmLib) {
		::FreeLibrary(hmLib);
		hmLib = NULL;
	}

	return nRetCode;
}

NEPDLLAPI void __stdcall NVDA_SetModelName(CString sModelName)
{
	m_ModelName = sModelName;
}

NEPDLLAPI int __stdcall NVDA_QueryMobileIDByPicasso(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	int nRetCode = HS_OK;
	CString sDll;
	::GetModuleFileName(NULL, sDll.GetBuffer(MAX_PATH), MAX_PATH);
	sDll.ReleaseBuffer();
	sDll = sDll.Left(sDll.ReverseFind('\\'));
	sDll += _T("\\Pics_SSSO2_Factory_StaticM.dll");
	HMODULE hmLib = ::LoadLibrary(sDll);
	if (hmLib == NULL) {
		CString msg;
		msg.Format(_T("Load: %s Fail!"), sDll);
		AfxMessageBox(msg);
		return -1*ERROR_DLL_INIT_FAILED;
	}
	QueryMobileIDByPicassoType pFnQueryMobileIDByPicasso = (QueryMobileIDByPicassoType)GetProcAddress(hmLib, "QueryMobileIDByPicasso_SSS02");
	if (pFnQueryMobileIDByPicasso == NULL) {
		::FreeLibrary(hmLib);
		hmLib = NULL;
		CString message;
		message = _T("Get DLL Function: QueryMobileIDByPicasso_SSS02 Fail!");
		AfxMessageBox(message);
		return -1*ERROR_FUNCTION_NOT_CALLED;
	}

	const int PAYLOAD_SIZE = 4096;
	char szModelName[32] = {0};
	char szPicasso[PAYLOAD_SIZE] = {0};
	char ostrMobileId[32];
	char ostrErrCode[1024];
	char ostrErrMsg[1024];
	if (HS_OK != NVDA_FACTORY_ReadPicasso(nPort, nTimeout, szPicasso)) {
		CString message;
		message = _T("Read picasso Fail!");
		AfxMessageBox(message);
		return -1*ERROR_FUNCTION_FAILED;
	}
	CString sModelName = m_ModelName;
	strncpy(szModelName, sModelName, sModelName.GetLength());
	if ( pFnQueryMobileIDByPicasso(szModelName, szPicasso, ostrMobileId, ostrErrCode, ostrErrMsg) ) {
		unsigned char fa_data[4096];
		if (HS_OK != NVDA_FACTORY_FARead(nPort, nTimeout, (char*)&fa_data)) {
			CString message;
			message = _T("Read mobile id Fail!");
			AfxMessageBox(message);
			return -1*ERROR_FUNCTION_FAILED;;
		}
		char* szMobileId[11] = {0};
		memcpy(szMobileId, &fa_data[FA_MOBILE_ID_START], 11);
		CString mobile_id_db = ostrMobileId;
		CString mobile_id_device;
		mobile_id_device.Format(_T("%x%x%x%x%x%x%x%x%x%x%x"), szMobileId[0], 
			szMobileId[1], szMobileId[2], szMobileId[3], szMobileId[4], szMobileId[5], 
			szMobileId[6], szMobileId[7], szMobileId[8], szMobileId[9], szMobileId[10]);
		if (mobile_id_db.Find(_T("0000000000000000000000")) != -1) {
			// picasso is not used, update mobile id to server
			if (HS_OK != NVDA_UpdateMobileIDByPicasso(szModelName, szPicasso, mobile_id_device)) {
				nRetCode = -1*ERROR_BAD_COMMAND;
			}
		}
		else {
			// picasso is used, then check mobile id between device and db
			if (strcmp(mobile_id_device, mobile_id_db) != 0) {
				// picasso reused
				nRetCode = -1*ERROR_NOT_SAME_DEVICE;
				::AfxMessageBox(_T("Check mobile id fail. Picasso is reused!!!"));
			}
		}
	} 
	else {
		char error_string[512] = {0};
		strcat(error_string, "[");
		strncat(error_string, ostrErrCode, strlen(ostrErrCode));
		strcat(error_string, "] ");
		strncat(error_string, ostrErrMsg, strlen(ostrErrMsg));
		nRetCode = -1*ERROR_INVALID_DATA;
		CString err_msg;
		err_msg.Format(_T("Fail to query Mobile ID: %s."), error_string);
		::AfxMessageBox(err_msg);
	}
	if (hmLib) {
		::FreeLibrary(hmLib);
		hmLib = NULL;
	}

	return nRetCode;
}

NEPDLLAPI int __stdcall NVDA_KillAdb(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	int nRetCode = -1*ERROR_FUNCTION_FAILED;

	if ( IsADBLive() ) {
		nRetCode = HS_OK;
	}

	return nRetCode;
}

NEPDLLAPI int __stdcall NVDA_ClearTLineFlag(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	int nRetCode = -1*ERROR_FUNCTION_FAILED;
	const int PORT_TCP = 2000;
	const int BUF_SIZE = 4096;
	char* pBuf = new char[BUF_SIZE];
	
	if ( WriteTLineTroubleShootingPics() ) {
		// clear FA
		memset(pBuf, 0, sizeof(pBuf));
		nRetCode = NVDA_FACTORY_ClearFA(PORT_TCP, nTimeout, pBuf);
		if (nRetCode != HS_OK) {
			strcpy(ErrorCode, "ERROR: Clear FA fail");
			nRetCode = -1*ERROR_BAD_COMMAND;
		}
	}
	else {
		strcpy(ErrorCode, "ERROR: Write pics fail");
		nRetCode = -1*ERROR_BAD_COMMAND;
	}

	return nRetCode;
}

NEPDLLAPI int __stdcall NVDA_UpdateImeiPackState_R(int nPort, int nTimeout, char* ErrorCode)
{
	int nRetCode = NO_ERROR;
	CString sDll;
	::GetModuleFileName(NULL, sDll.GetBuffer(MAX_PATH), MAX_PATH);
	sDll.ReleaseBuffer();
	sDll = sDll.Left(sDll.ReverseFind('\\'));
	sDll += _T("\\Pics_SSSO2_Factory_StaticM.dll");
	HMODULE hmLib = ::LoadLibrary(sDll);
	if (hmLib == NULL) {
		CString msg;
		msg.Format(_T("Load: %s Fail!"), sDll);
		AfxMessageBox(msg);
		return -1*ERROR_DLL_INIT_FAILED;
	}
	ImeiStateUpdateCmd UpdateImeiState_R = (ImeiStateUpdateCmd)GetProcAddress(hmLib, "UpdateIMEIStatus_SSSO2_R");
	if (UpdateImeiState_R == NULL) {
		::FreeLibrary(hmLib);
		hmLib = NULL;
		CString message;
		message = _T("Get DLL Function: UpdateIMEIStatus_SSSO2_R Fail!");
		AfxMessageBox(message);
		return -1*ERROR_FUNCTION_NOT_CALLED;
	}

	// read imei
	char szIMEI[MAX_PATH] = {0};
	NVDA_WWAN_ReadIMEI(nPort, nTimeout, szIMEI);

	char ostrErrCode[1024];
	char ostrErrMsg[1024];
	if ( UpdateImeiState_R(szIMEI, ostrErrCode, ostrErrMsg) ) {
		nRetCode = NO_ERROR;
	} 
	else {
		char error_string[512] = {0};
		strcat(error_string, "[");
		strncat(error_string, ostrErrCode, strlen(ostrErrCode));
		strcat(error_string, "] ");
		strncat(error_string, ostrErrMsg, strlen(ostrErrMsg));
		nRetCode = -1*ERROR_INVALID_DATA;
		CString err_msg;
		err_msg.Format(_T("Fail: %s."), error_string);
		::AfxMessageBox(err_msg);
	}
	if (hmLib) {
		::FreeLibrary(hmLib);
		hmLib = NULL;
	}

	return nRetCode;
}

NEPDLLAPI int __stdcall NVDA_TestFtdMode(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	int nRetCode = -1*ERROR_FUNCTION_FAILED;
	CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));
	CString adb_path = pthToolDir + _T("\\adb.exe");
	CString param = _T("kill-server");
	Exec(adb_path, param, nTimeout, false);

	nRetCode = CheckFtdMode(ErrorCode);
	if(nRetCode != HS_OK) {
		strcpy(ErrorCode, "ERROR: Check ftd mode fail");
		nRetCode = -1*ERROR_BAD_COMMAND;
	}

	return nRetCode;
}

NEPDLLAPI int __stdcall NVDA_AutoFtdMode(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	IsADBLive();

	int nRetCode = -1*ERROR_FUNCTION_FAILED;
	CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));
	CString adb_path = pthToolDir + _T("\\adb.exe");
	CString param = _T("kill-server");
	Exec(adb_path, param, nTimeout, false);

	nRetCode = CheckFtdMode(ErrorCode);
	if (nRetCode != HS_OK) {
		nRetCode = EnterFtdMode(0, ErrorCode);
		if(nRetCode != HS_OK) {
			strcpy(ErrorCode, "ERROR: Enter ftd mode fail");
			nRetCode = -1*ERROR_BAD_COMMAND;
		}
		else {
			nRetCode = CheckFtdMode(ErrorCode);
			if(nRetCode != HS_OK) {
				strcpy(ErrorCode, "ERROR: Check ftd mode fail");
				nRetCode = -1*ERROR_BAD_COMMAND;
			}
		}
	}
	if(nRetCode != HS_OK) {
		strcpy(ErrorCode, "ERROR: Check ftd mode fail");
		nRetCode = -1*ERROR_BAD_COMMAND;
	}

	return nRetCode;
}

NEPDLLAPI int __stdcall NVDA_CheckStationFlag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	int nRetCode = -1*ERROR_FUNCTION_FAILED;
	unsigned char FaInfo[4096];
	memset(FaInfo, 0, 4096);
	nRetCode = NVDA_FACTORY_FARead(nPort, nTimeout, (char*)&FaInfo);
	if (nRetCode != HS_OK) {
		strcpy(OutBuffer, "ERROR: Read FA fail");
	}
	else {
		const int LENGTH_STATION_FLAG  = 12;
		unsigned char FaInfoOut[12] = {0};
		memcpy(FaInfoOut, &FaInfo[FA_P01_DL_START], LENGTH_STATION_FLAG);
		CString mode = InBuffer;
		int nCheckStation;
		if (mode.Find(_T("FT")) != -1) {
			nCheckStation = 1;
		}
		else {
			nCheckStation = 3;
		}

		CString flag;
		CString StationFlags;
		StationFlags.Format(_T("%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"), FaInfoOut[0], 
							FaInfoOut[1], FaInfoOut[2], FaInfoOut[3], FaInfoOut[4], FaInfoOut[5], 
							FaInfoOut[6], FaInfoOut[7], FaInfoOut[8], FaInfoOut[9], FaInfoOut[10],
							FaInfoOut[11]);
		//strcpy(OutBuffer, StationFlags);
		for (int nStation = 0; nStation < nCheckStation; nStation++) {
			flag = StationFlags.Mid(nStation*8, 8);
			switch (nStation) {
				case 0: // pre-DL check flags not 0
					if ( !CheckFlagStation(flag) ) {
						strcpy(OutBuffer, "ERROR: Check preDL flag fail");
						return HS_FAIL;
					}
					break;
				case 1: // FT checks 0x32323232
					if (flag.Find(_T("32323232")) == -1) {
						strcpy(OutBuffer, "ERROR: Check FT flag fail");
						return HS_FAIL;
					}
					break;
				case 2: // MMIBB checks 0x1c0a0a04
					if (flag.Find(_T("1c0a0a04")) == -1) {
						strcpy(OutBuffer, "ERROR: Check MMIBB flag fail");
						return HS_FAIL;
					}
					break;
				case 3: // MMIRF checks 0x31323334
					if (flag.Find(_T("31323334")) == -1) {
						strcpy(OutBuffer, "ERROR: Check MMIRF flag fail");
						return HS_FAIL;
					}
					break;
				case 4: // IMEI
					if ( !CheckFlagStation(flag) ) {
						strcpy(OutBuffer, "ERROR: Check IMEI flag fail");
						return HS_FAIL;
					}
					break;
				case 5: // re-DL
					if ( !CheckFlagStation(flag) ) {
						strcpy(OutBuffer, "ERROR: Check reDL flag fail");
						return HS_FAIL;
					}
					break;
				default:
					break;
			}
		}
	}
	return nRetCode;
}

NEPDLLAPI int __stdcall NVDA_PwrOn2UartPktMode(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " [FT7001]";
	strncat(ErrorCode, error_code, strlen(error_code));
	return PowerOnToUARTPacketMode(nPort, nTimeout, false);
}

// 2-CAMERA
//NEPDLLAPI int __stdcall NVDA_CAMERA_GetModuleId(int nPort, int nTimeout, char* OutBuffer) // cmd: 9
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_CAMERA,9,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7002]", __FUNCTION__);
//}
//
//NEPDLLAPI int __stdcall NVDA_CAMERA_GetSensorId(int nPort, int nTimeout, char* OutBuffer) // cmd: 10
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_CAMERA,10,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7002]", __FUNCTION__);
//}

// 3-LCD
// L1-	LCD no display
// L4-  LCD no back light
// L4-C Can not switch on/off in backlight test
// L41- LCD back light has some color NG
NEPDLLAPI int __stdcall NVDA_LCD_DisplayRGB(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_LCD,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7003]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LCD_Backlight_Test(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_LCD,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7003]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LCD_Backlight_On(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_LCD,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7003]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LCD_Backlight_Off(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_LCD,4,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7003]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LCD_Touch_Test(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_LCD,5,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7003]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LCD_Touch_SelfTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_LCD,6,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT5619]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LCD_Touch_VddTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_LCD,7,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT5619]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LCD_Touch_PinFaultTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_LCD,8,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT5619]", __FUNCTION__);
}


// 4-SENSOR
// F380 Ambient Light Sensor
// F381 G-sensor 
// F382 E-compass
// F383 Proximity
// F384 Cap key
NEPDLLAPI int __stdcall NVDA_SENSOR_GSENSORReadAcc(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SENSOR,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SENSOR_GSENSORReadID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SENSOR,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SENSOR_ECompassI2CTest(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SENSOR,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SENSOR_TouchID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SENSOR,4,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7004]", __FUNCTION__);
}


// 5-AUDIO
NEPDLLAPI int __stdcall NVDA_AUDIO_Mic(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_AUDIO,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7005]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_Headset(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_AUDIO,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7005]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_Speaker(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_AUDIO,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7005]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_I2CTest(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_AUDIO,4,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7005]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_JackCheck(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_AUDIO,5,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7005]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_HookCheck(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_AUDIO,6,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7005]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_LineOut(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_AUDIO,7,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7005]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_PlayMusic(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_AUDIO,8,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7005]", __FUNCTION__);
}

// 6-MODEM
// F390 GSM Tx Max Power
// F391 SIM test
// F392 GPS test
NEPDLLAPI int __stdcall NVDA_MODEM_GPSCWTest(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_MODEM,1,0,0};
#ifdef _DEBUG
    strcpy(OutBuffer, "-130");
    return HS_OK;
#else
    return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7006]", __FUNCTION__);
#endif
}

NEPDLLAPI int __stdcall NVDA_MODEM_GPSFreqStability(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_MODEM,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7006]", __FUNCTION__);
}


// 7-BT
// F393 BT Tx Max Power
NEPDLLAPI int __stdcall NVDA_BT_TxOnAt(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BT,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7007]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BT_TxOff(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BT,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7007]", __FUNCTION__);
}


// 8-FLASH
//NEPDLLAPI int __stdcall NVDA_FLASH_ReadWriteLoopTest(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_FLASH,1,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7008]", __FUNCTION__);//NVDA_FLASH_ReadWriteLoopTest
//}
//
//NEPDLLAPI int __stdcall NVDA_FLASH_ReadPage(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_FLASH,2,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7008]", __FUNCTION__);//NVDA_FLASH_ReadPage
//}
//
//NEPDLLAPI int __stdcall NVDA_FLASH_WritePage(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_FLASH,3,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7008]", __FUNCTION__);//NVDA_FLASH_WritePage
//}
//
//// F226 HWID version error
//NEPDLLAPI int __stdcall NVDA_FLASH_ReadID(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_FLASH,4,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7008]", __FUNCTION__);
//}
//
//NEPDLLAPI int __stdcall NVDA_FLASH_EraceBlock(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_FLASH,5,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7008]", __FUNCTION__);//NVDA_FLASH_EraceBlock
//}
//
//NEPDLLAPI int __stdcall NVDA_FLASH_FindImageStartingPage(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_FLASH,6,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7008]", __FUNCTION__);//NVDA_FLASH_FindImageStartingPage
//}
//
//NEPDLLAPI int __stdcall NVDA_FLASH_MCPTest(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_FLASH,7,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7008]", __FUNCTION__);//NVDA_FLASH_MCPTest
//}


// 9-DDR
//NEPDLLAPI int __stdcall NVDA_DDR_SetEBI1Clkto99MHZ(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_DDR,1,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7009]", __FUNCTION__);//NVDA_DDR_SetEBI1Clkto99MHZ
//}
//
//NEPDLLAPI int __stdcall NVDA_DDR_SetEBI1Clkto128MHZ(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_DDR,2,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7009]", __FUNCTION__);//NVDA_DDR_SetEBI1Clkto128MHZ
//}
//
//NEPDLLAPI int __stdcall NVDA_DDR_ReadWriteTest(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_DDR,3,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7009]", __FUNCTION__);//NVDA_DDR_ReadWriteTest
//}
//
//NEPDLLAPI int __stdcall NVDA_DDR_ReadWriteLoopTest(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_DDR,4,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7009]", __FUNCTION__);//NVDA_DDR_ReadWriteLoopTest
//}
//
//NEPDLLAPI int __stdcall NVDA_DDR_ReadAddr(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_DDR,5,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7009]", __FUNCTION__);//NVDA_DDR_ReadAddr
//}
//
//NEPDLLAPI int __stdcall NVDA_DDR_RoughTest(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_DDR,7,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7009]", __FUNCTION__);//NVDA_DDR_RoughTest
//}

// 10-RTC
// E5 Alarm setting fail (RTC)
//NEPDLLAPI int __stdcall NVDA_RTC_ReadYear(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_RTC,1,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7010]", __FUNCTION__);
//}
//
//NEPDLLAPI int __stdcall NVDA_RTC_ReadMonth(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_RTC,2,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7010]", __FUNCTION__);
//}
//
//NEPDLLAPI int __stdcall NVDA_RTC_ReadDay(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_RTC,3,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7010]", __FUNCTION__);
//}
//
//NEPDLLAPI int __stdcall NVDA_RTC_ReadHour(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_RTC,4,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7010]", __FUNCTION__);
//}
//
//NEPDLLAPI int __stdcall NVDA_RTC_ReadMinute(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_RTC,5,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7010]", __FUNCTION__);
//}
//
//NEPDLLAPI int __stdcall NVDA_RTC_ReadSecond(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_RTC,6,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7010]", __FUNCTION__);
//}
//
//NEPDLLAPI int __stdcall NVDA_RTC_ReadDayOfWeek(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_RTC,7,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7010]", __FUNCTION__);
//}
//
//NEPDLLAPI int __stdcall NVDA_RTC_WriteYear(int nPort, int nTimeout, char* InBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_RTC,8,0,0};
//	return GeneralCmdIn(nPort, nTimeout, command, InBuffer, strlen(InBuffer));
//}
//
//NEPDLLAPI int __stdcall NVDA_RTC_WriteMonth(int nPort, int nTimeout, char* InBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_RTC,9,0,0};
//	return GeneralCmdIn(nPort, nTimeout, command, InBuffer, strlen(InBuffer));
//}
//
//NEPDLLAPI int __stdcall NVDA_RTC_WriteDay(int nPort, int nTimeout, char* InBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_RTC,10,0,0};
//	return GeneralCmdIn(nPort, nTimeout, command, InBuffer, strlen(InBuffer));
//}
//
//NEPDLLAPI int __stdcall NVDA_RTC_WriteHour(int nPort, int nTimeout, char* InBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_RTC,11,0,0};
//	return GeneralCmdIn(nPort, nTimeout, command, InBuffer, strlen(InBuffer));
//}
//
//NEPDLLAPI int __stdcall NVDA_RTC_WriteMinute(int nPort, int nTimeout, char* InBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_RTC,12,0,0};
//	return GeneralCmdIn(nPort, nTimeout, command, InBuffer, strlen(InBuffer));
//}
//
//NEPDLLAPI int __stdcall NVDA_RTC_WriteSecond(int nPort, int nTimeout, char* InBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_RTC,13,0,0};
//	return GeneralCmdIn(nPort, nTimeout, command, InBuffer, strlen(InBuffer));
//}
//
//NEPDLLAPI int __stdcall NVDA_RTC_WriteDayOfWeek(int nPort, int nTimeout, char* InBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_RTC,14,0,0};
//	return GeneralCmdIn(nPort, nTimeout, command, InBuffer, strlen(InBuffer));
//}
//
//NEPDLLAPI int __stdcall NVDA_RTC_Alert(int nPort, int nTimeout, char* InBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_RTC,15,0,0};
//	return GeneralCmdIn(nPort, nTimeout, command, InBuffer, strlen(InBuffer));
//}
//
//NEPDLLAPI int __stdcall NVDA_RTC_Reset_Second(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_RTC,16,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7010]", __FUNCTION__);
//}


// 11-USB


// 12-CHARGER
// F332 Battery ID failed
// F372 Charging failed of BB test
// F228 USB CHARGE Fail
// E4   Can not charge
// E41  Coin Li-ion battery can't charge
//NEPDLLAPI int __stdcall NVDA_CHARGER_PreCharge(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_CHARGER,1,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7012]", __FUNCTION__);
//}
//
//NEPDLLAPI int __stdcall NVDA_CHARGER_FullCharge(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_CHARGER,2,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7012]", __FUNCTION__);
//}
//
//NEPDLLAPI int __stdcall NVDA_CHARGER_StopCharge(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_CHARGER,3,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7012]", __FUNCTION__);
//}
//
//NEPDLLAPI int __stdcall NVDA_CHARGER_Verify_IC(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_CHARGER,4,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7012]", __FUNCTION__);
//}
//
//NEPDLLAPI int __stdcall NVDA_CHARGER_Verify_TPS65023(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_CHARGER,5,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7012]", __FUNCTION__);
//}
//
//NEPDLLAPI int __stdcall NVDA_CHARGER_EnableCharing(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_CHARGER,6,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7012]", __FUNCTION__);
//}
//
//NEPDLLAPI int __stdcall NVDA_CHARGER_ReadVersion(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_CHARGER,7,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7012]", __FUNCTION__);
//}
//
//NEPDLLAPI int __stdcall NVDA_CHARGER_500mACharge(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_CHARGER,8,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7012]", __FUNCTION__);
//}


// 13-KEYPAD
//NEPDLLAPI int __stdcall NVDA_KEYPAD_Detect(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_KEYPAD,1,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7013]", __FUNCTION__);//NVDA_KEYPAD_Detect
//}


// 14-VIBRATOR
// F370 Vibrator failed of BB test
NEPDLLAPI int __stdcall NVDA_VIBRATOR_Switch(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_VIBRATOR,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7014]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_VIBRATOR_RunningTime(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_VIBRATOR,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7014]", __FUNCTION__);
}


// 15-BATTERY
// F334	Battery failed
// F372 Charging failed of BB test
// F228 USB CHARGE Fail
// E4   Can not charge
// E41  Coin Li-ion battery can't charge
// F396 Backup battery open
// F397 Backup battery short
NEPDLLAPI int __stdcall NVDA_BATTERY_BattChargeOn(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BATTERY,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BATTERY_ChargingRecovery(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BATTERY,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BATTERY_BattTemp(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BATTERY,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BATTERY_PMUBattVoltage(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BATTERY,4,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BATTERY_PMUBattCapacity(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BATTERY,5,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BATTERY_GAGBattVoltage(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BATTERY,6,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BATTERY_GAGBattCapacity(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BATTERY,7,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7015]", __FUNCTION__);
}


// 16-DOWNLOAD
//NEPDLLAPI int __stdcall NVDA_DOWNLOAD_FASTBOOT_MODE(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_DOWNLOAD,1,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7016]", __FUNCTION__);//NVDA_DOWNLOAD_FASTBOOT_MODE
//}
//
//NEPDLLAPI int __stdcall NVDA_DOWNLOAD_NewFlashWrite0XFF(int nPort, int nTimeout, char* InBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_DOWNLOAD,2,0,0};
//	return GeneralCmdIn(nPort, nTimeout, command, InBuffer, strlen(InBuffer));
//}
//
//NEPDLLAPI int __stdcall NVDA_DOWNLOAD_NewFlashRead(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_DOWNLOAD,3,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7016]", __FUNCTION__);//NVDA_DOWNLOAD_NewFlashRead
//}
//
//
//// 17-OS
//NEPDLLAPI int __stdcall NVDA_OS_JumpToAPPSBL(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_OS,1,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7017]", __FUNCTION__);//NVDA_OS_JumpToAPPSBL
//}


// 18-WLAN
// F395 WLAN Tx Max
NEPDLLAPI int __stdcall NVDA_WLAN_TxMaxPowerOnCh(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WLAN,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7018]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_TxMaxPowerOff(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WLAN,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7018]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_LocalFreqOnCh(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WLAN,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7018]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_LocalFreqOff(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WLAN,4,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7018]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_TempTxMaxPowerOnCh(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WLAN,5,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7018]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_TempTxMaxPowerOff(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WLAN,6,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7018]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_TempLocalFreqOnCh(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WLAN,7,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7018]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_TempLocalFreqOff(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WLAN,8,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7018]", __FUNCTION__);
}


// 19-LED
// L3   Keypad LED no good
// L31  Indicator LED no good
// L4-C Can not switch on/off in backlight test
// L3   Keypad LED no good
// L5-  Back light is too dark, LED light not uniform or black mark
// L6-	LED wrong color
// L7-  Flash light no function
NEPDLLAPI int __stdcall NVDA_LED_BacklightOn(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_LED,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7019]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LED_L01On(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_LED,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7019]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LED_L02On(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_LED,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7019]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LED_CapKeyOn(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_LED,4,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7019]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LED_FlashOn(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_LED,5,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7019]", __FUNCTION__);
}


// 20-ID
// F226 HWID version error
NEPDLLAPI int __stdcall NVDA_ID_ReadBoardID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_ID,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7020]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_ID_ReadHWID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_ID,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7020]", __FUNCTION__);
}

// 21-Regulator
NEPDLLAPI int __stdcall NVDA_REGULATOR_LDO0On(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_REGULATOR,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7021]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_REGULATOR_LDO6On(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_REGULATOR,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7021]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_REGULATOR_LDO7On(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_REGULATOR,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7021]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_REGULATOR_LDO8On(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_REGULATOR,4,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7021]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_REGULATOR_GPIO1On(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_REGULATOR,5,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7021]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_REGULATOR_GPIO2On(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_REGULATOR,6,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7021]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_REGULATOR_GPIO3On(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_REGULATOR,7,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7021]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_REGULATOR_LDORecover(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_REGULATOR,8,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7021]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_REGULATOR_LDOLog(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_REGULATOR,9,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7021]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_REGULATOR_LDOAllOn(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_REGULATOR,10,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7021]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_REGULATOR_LDO1V2(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_REGULATOR,11,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7021]", __FUNCTION__);
}

// 22-WWAN
// F387	3G card
NEPDLLAPI int __stdcall NVDA_WWAN_3GCardDetect(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WWAN,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7022]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WWAN_ReadIMEI(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WWAN,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7022]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WWAN_ReadRSSI(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WWAN,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " [FT7022]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WWAN_Read3GModuleVersion(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WWAN,4,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7022]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WWAN_CheckSimLock(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WWAN,5,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7022]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WWAN_CheckNetworkLock(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WWAN,6,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7022]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WWAN_Read3GModuleType(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WWAN,7,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7022]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WWAN_IMEIBackUp(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WWAN,8,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7022]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WWAN_ReadFAIMEIBackUp(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WWAN,14,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7022]", __FUNCTION__);
}


// 23-RD
//NEPDLLAPI int __stdcall NVDA_RD_LoadAPPSBL(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_RD,1,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7023]", __FUNCTION__);//NVDA_RD_LoadAPPSBL
//}
//
//NEPDLLAPI int __stdcall NVDA_RD_EnterUARTPktMode(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_RD,2,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7023]", __FUNCTION__);//NVDA_RD_EnterUARTPktMode
//}
//
//NEPDLLAPI int __stdcall NVDA_RD_EVT0AutoTest(int nPort, int nTimeout, char* OutBuffer)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	TRACER_FUNCTION_IN();
//	char command[4] = {NV_DT_RD,3,0,0};
//	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7023]", __FUNCTION__);//NVDA_RD_EVT0AutoTest
//}


//24-TV_MODULE
NEPDLLAPI int __stdcall NVDA_TV_ModuleDetect(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_TV,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7024]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_TV_InBandPower(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_TV,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 0, OutBuffer, " [FT7024]", __FUNCTION__);
}


// 25-VERSION
// F211 SW version error
// F226 HWID version error
NEPDLLAPI int __stdcall NVDA_VERSION_ReadTestVersion(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_VERSION,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7025]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_VERSION_ReadBoardID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_VERSION,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7025]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_VERSION_ReadAccessaryID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_VERSION,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7025]", __FUNCTION__);
}

// 26-FACTORY
// F398 PCBA version error
NEPDLLAPI int __stdcall NVDA_FACTORY_FARead(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, NULL, __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_FAWrite(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	//char command[4] = {NV_DT_FA,2,0,0};
	//return GeneralCmdIn(nPort, nTimeout, command, InBuffer, 128);
	return HS_FAIL;
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WritePicasso(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (strlen(InBuffer) != 10) {
		return -1*ERROR_INVALID_INDEX;
	}
	char szPicasso[13] = {0};
	memcpy(szPicasso, InBuffer, 10);
	if (strlen(szPicasso) == 10) {
		if (strlen(szPicasso) == 10) {
			for (int i = 0; i < 10; i++) {
				szPicasso[i] = toupper(szPicasso[i]);
			}
			strcat(szPicasso, "OK");
		}
	}
	if (isupper(szPicasso[0]) && isupper(szPicasso[1])) {
		for (int i = 2; i < 10; i++) {
			if (!isalnum(szPicasso[i])) {
				return -1*ERROR_INVALID_DATA;
			}
		}
		if (szPicasso[10] != 'O' || szPicasso[11] != 'K') {
			return -1*ERROR_INVALID_ID_AUTHORITY;
		}
	}
	else {
		return -1*ERROR_INVALID_DATA;
	}
	char command[4] = {NV_DT_FA,24,0,0};
	return DtCmdNvda(nPort, nTimeout, command, szPicasso, 12, OutBuffer, NULL, __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadPicasso(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadPicassoCheck(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	CString sDll;
	::GetModuleFileName(NULL, sDll.GetBuffer(MAX_PATH), MAX_PATH);
	sDll.ReleaseBuffer();
	sDll = sDll.Left(sDll.ReverseFind('\\'));
	sDll += _T("\\Pics_SSSO2_Factory_StaticM.dll");
	char command[4] = {NV_DT_FA,3,0,0};
	int nRetCode = DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7026]", __FUNCTION__);
	if (nRetCode == HS_OK) {
		char ostrErrCode[1024];
		char ostrErrMsg[1024];
		HMODULE hmLib = ::LoadLibrary(sDll);
		if (hmLib == NULL) {
			CString msg;
			msg.Format(_T("Load: %s Fail!"), sDll);
			AfxMessageBox(msg);
			return -1*ERROR_DLL_INIT_FAILED;
		}
		PicassoCmd CheckPicasso = (PicassoCmd)GetProcAddress(hmLib, "CheckPiacssoLuna_SSS02");
		if (CheckPicasso == NULL) {
			::FreeLibrary(hmLib);
			hmLib = NULL;
			CString message;
			message = _T("Get DLL Function: CheckPiacssoLuna_SSS02 Fail!");
			AfxMessageBox(message);
			return -1*ERROR_FUNCTION_NOT_CALLED;
		}
		if ( CheckPicasso(OutBuffer, ostrErrCode, ostrErrMsg) ) {
			nRetCode = HS_OK;
		} 
		else { // FT3021
			char *error_code = " [FT3021]";
			strncat(OutBuffer, error_code, strlen(error_code));
			strcat(OutBuffer, ":");
			strncat(OutBuffer, ostrErrCode, strlen(ostrErrCode));
			strcat(OutBuffer, ":");
			strncat(OutBuffer, ostrErrMsg, strlen(ostrErrMsg));
			nRetCode = HS_FAIL;
		}
		if (hmLib) {
			::FreeLibrary(hmLib);
			hmLib = NULL;
		}
	}
	return nRetCode;
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadWifiMac(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,4,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteWifiMac(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,5,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadBtMac(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,6,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteBtMac(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,7,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadPPID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,8,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WritePPID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,9,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadServiceTag(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,10,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteServiceTag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,11,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteDLFlag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char flag[4] = {0};
	SetFaFlag(InBuffer, 1, flag);

	char command[4] = {NV_DT_FA,14,0,0};
	return DtCmdNvda(nPort, nTimeout, command, flag, sizeof(flag), OutBuffer, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteFTFlag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char flag[4] = {0};
	SetFaFlag(InBuffer, 2, flag);
	char command[4] = {NV_DT_FA,15,0,0};
	return DtCmdNvda(nPort, nTimeout, command, flag, strlen(flag), OutBuffer, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteMMIBBFlag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char flag[4] = {0};
	SetFaFlag(InBuffer, 3, flag);
	char command[4] = {NV_DT_FA,16,0,0};
	return DtCmdNvda(nPort, nTimeout, command, flag, strlen(flag), OutBuffer, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteMMIRFFlag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char flag[4] = {0};
	SetFaFlag(InBuffer, 4, flag);
	char command[4] = {NV_DT_FA,17,0,0};
	return DtCmdNvda(nPort, nTimeout, command, flag, strlen(flag), OutBuffer, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_CheckAndMoveFA(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char command[4] = {NV_DT_FA,19,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadRC(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char command[4] = {NV_DT_FA,25,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ClearFA(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char command[4] = {NV_DT_FA,26,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadMmiErrorCode(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char command[4] = {NV_DT_FA,29,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7026]", __FUNCTION__);
}

//27-HouseKeeping
//F386	HOUSEKEEP_PowerGlueLogic
NEPDLLAPI int __stdcall NVDA_HOUSEKEEP_PowerGlueLogicCheck(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char command[4] = {NV_DT_HOUSEKEEP,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " [FT7027]", __FUNCTION__);
}