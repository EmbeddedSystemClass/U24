#include "stdafx.h"
#include <cstddef> // size_t

#include "../CmdType.h"
#include "GeneralDLLApi.h" // GeneraCmd
#include "DetroitDLLApi.h"
#include "io.h"
#include "../Utility.h"
#include "../FactoryBlock.h"

#include "QLib.h"
#include "QLib_Defines.h"
#pragma comment(lib, "QMSL_MSVC9R") 

typedef bool (__cdecl *PicassoCmd)(char*, char*, char*);

static const int CMD_MAX_RETRY = 1;

static int MobileCmd(int nPort, int nTimeout, char* command,
					 char* InBuffer, size_t InSize, char* OutBuffer,
					 const char* error_code, const char* Func)
{
	int nRetCode = -1*ERROR_FUNCTION_FAILED;
	for (int nTry = 0; nTry < CMD_MAX_RETRY; nTry++) {
		TRACE(_T("GeneralCmdInOut\n"));
		nRetCode = GeneralCmdInOut(nPort, nTimeout, command, InBuffer, InSize, OutBuffer);
		if (nTry == 0 && nRetCode != HS_OK && strcmp(OutBuffer, "") == 0) {
			strncat(OutBuffer, error_code, strlen(error_code));
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

NEPDLLAPI bool __stdcall Detroit_ExecAdbOut(CString Command, char* output, char* ErrorCode)
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

		CloseHandle(hWrite);
		hWrite=NULL;

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

		CloseHandle(hWrite);
		hWrite=NULL;
	}

	CloseHandle(hRead);
	CloseHandle(processInfo.hProcess);
	processInfo.hProcess = NULL;

	return isOk;
}

int Detroit_EnterFtdMode(int nTimeout, char* ErrorCode)
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

int Detroit_CheckFtdMode(char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	bool isOk;
	int nRetCode = -1*ERROR_FUNCTION_FAILED;
	char *msgBuffer = new char[1024 * 1024 * 2];
	isOk = Detroit_ExecAdbOut("shell ps", msgBuffer, ErrorCode);

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

NEPDLLAPI int __stdcall Detroit_TestFtdMode(int nPort, int nTimeout, char* ErrorCode)
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

	nRetCode = Detroit_CheckFtdMode(ErrorCode);
	if(nRetCode != HS_OK) {
		strcpy(ErrorCode, "ERROR: Check ftd mode fail");
		nRetCode = -1*ERROR_BAD_COMMAND;
	}

	return nRetCode;
}

NEPDLLAPI int __stdcall Detroit_AutoFtdMode(int nPort, int nTimeout, char* ErrorCode)
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

	nRetCode = Detroit_CheckFtdMode(ErrorCode);
	if (nRetCode != HS_OK) {
		nRetCode = Detroit_EnterFtdMode(0, ErrorCode);
		if(nRetCode != HS_OK) {
			strcpy(ErrorCode, "ERROR: Enter ftd mode fail");
			nRetCode = -1*ERROR_BAD_COMMAND;
		}
		else {
			nRetCode = Detroit_CheckFtdMode(ErrorCode);
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

// 01-Battery
NEPDLLAPI int __stdcall Detroit_BATTcoincellVoltage(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_BATT,1,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_BATTcoincellChargeOn(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_BATT,2,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_BATTrtcTime(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_BATT,3,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

// 02-Power
NEPDLLAPI int __stdcall Detroit_PWRgaugeID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_PWR,1,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_PWRgaugeVoltage(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_PWR,2,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_PWRgaugeCurrent(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_PWR,3,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_PWRgaugeTemperature(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_PWR,4,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_PWRchargerID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_PWR,5,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_PWRchargerOn(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_PWR,6,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_DFIupgrade(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_PWR,7,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_ITenable(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_PWR,8,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_Regualator(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_PWR,9,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_PWRbattID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_PWR,10,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_PWRbattCapacity(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_PWR,12,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}


//03:Memory
NEPDLLAPI int __stdcall Detroit_MCPflashID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_MCP,1,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_MCPddrID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_MCP,2,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_MCPddrSize(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_MCP,3,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

//04:LCM - LCD & Touch
NEPDLLAPI int __stdcall Detroit_TOUCHpoint(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_LCM,1,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_TOUCHtest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_LCM,2,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_TOUCHid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_LCM,3,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_TOUCHversion(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_LCM,4,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_TOUCHSelfTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_LCM,5,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_LCMbacklight(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_LCM,6,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_LCMbacklightColor(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_LCM,7,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_TouchModel(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_LCM,10,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

//05:audio
NEPDLLAPI int __stdcall Detroit_AUpathOut(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_AU,1,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_AUplaymp3(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_AU,2,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_AUrecordWAV(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_AU,3,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_AUplayWAV(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_AU,4,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_AUdetectHeadset(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_AU,5,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_AUpathRec(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_AU,6,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_AUloopbackTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_AU,9,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_AUButtonCheck(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_AU,10,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_AUinitADC(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_AU,11,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_AUADCCali(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_AU,12,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_AUheadsetInfo(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_AU,13,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

//06:camera
NEPDLLAPI int __stdcall Detroit_CAMReadID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_CAM,1,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_CAMshot(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_CAM,2,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_CAMstart(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_CAM,3,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_CAMend(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_CAM,4,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_CAMFlashLED(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_CAM,5,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_CAMFlashLEDID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_CAM,6,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_CAM_OTP_AWB(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_CAM,7,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_CAM_OTP_AFMacro(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_CAM,8,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_CAM_OTP_AFInfinity(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_CAM,9,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_CAM_OTP_FrontFuseID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_CAM,10,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_KeyTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)//cmd:2
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_KEY,1,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

//08:SIM
NEPDLLAPI int __stdcall Detroit_SIMtest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " [FT5601]";
	char command[4] = {Detroit_FT_SIM,1,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SIMdetect(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " [FT5601]";
	char command[4] = {Detroit_FT_SIM,2,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

//10:Vibrator
NEPDLLAPI int __stdcall Detroit_VibRun(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_VIB,1,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

//12:Modem
NEPDLLAPI int __stdcall Detroit_TxRxPower(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_MDM,1,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_QmiRfTxTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_MDM,3,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_QmiRfTxOff(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_MDM,4,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}



//13:BT
NEPDLLAPI int __stdcall Detroit_BTid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_BT,1,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_BTTestMode(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_BT,2,0,0};
	//GeneralCmdInOut(nPort, nTimeout, command, InBuffer, OutBuffer);
	//return HS_OK;
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_BTTXPower(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_BT,3,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_BTRXrssi(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_BT,4,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_BTSelfTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_BT,5,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_BTLteTx(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_BT,6,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_BTLteRx(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_BT,7,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

// 14-WLAN
NEPDLLAPI int __stdcall Detroit_WLAN_Mode(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " [FT5301]";
	char command[4] = {Detroit_FT_WLAN,1,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_WLAN_TXPower(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " [FT5302]";
	int onoff = atoi(InBuffer);

	char sLibraryVersion[50];
	QLIB_GetLibraryVersion(sLibraryVersion);
	TRACE(_T("QLIB DLL Version: %s\n"), sLibraryVersion);
	//int nCom = atoi(InBuffer);
	BOOL isOk = FALSE;
	HANDLE m_hQMSL = NULL;
	if (m_hQMSL) {
		if (QLIB_IsPhoneConnected(m_hQMSL)) {
			QLIB_FlushRxBuffer(m_hQMSL);
		}
	}
	else {
		QLIB_SetLibraryMode(false);
		//QLIB_SetLibraryMode(true);
		//unsigned int nComPort = atoi(InBuffer);
		unsigned short nComPort = -1;
		QLIB_GetComPortNumber(m_hQMSL, &nComPort);
		m_hQMSL = QLIB_ConnectServer(nComPort);
		//m_hQMSL = QLIB_ConnectServerWithWait(nComPort, 5000);
		if (m_hQMSL == NULL) {
			//m_hQMSL = QLIB_ConnectServerWithWait(nComPort, 5000);
			//m_hQMSLPhone = QLIB_ConnectServerWithHandShake((unsigned)atoi(m_strCOMport.c_str()));

			QLIB_GetComPortNumber(m_hQMSL, &nComPort);
			if (nComPort == QLIB_COM_AUTO_DETECT) {
				TRACE(_T("COM Port: AUTO, NOT DETECTED\n"));
			}
		}
		if (m_hQMSL != NULL) {
			if (QLIB_IsPhoneConnected(m_hQMSL)) {
				unsigned long iESN;
				QLIB_DIAG_READ_ESN_F(m_hQMSL, &iESN);
				TRACE(_T("ESN:%d\n"), iESN);
				QLIB_FlushRxBuffer(m_hQMSL);
			}
		}
	}
	if (m_hQMSL != NULL) {
		unsigned long MSM_HW_Version = 0;
		unsigned long MobModel = 0;
		char MobSwRev[512];
		char ModelStr[512];
		if (QLIB_DIAG_EXT_BUILD_ID_F(m_hQMSL, &MSM_HW_Version, &MobModel, MobSwRev, ModelStr)) {
			TRACE(_T("      Ext MSM HW Version: 0x%X\n"), MSM_HW_Version);
			TRACE(_T("       Ext Mobile Model#: 0x%X\n"), MobModel);
			TRACE(_T("Ext Mobile Mobile SW Rev: %s\n"), CA2T(MobSwRev));
			TRACE(_T("            Ext Model ID: %s\n"), CA2T(ModelStr));
		}
		else {
			TRACE(_T("QLIB_DIAG_EXT_BUILD_ID_F = FAIL\n"));
		}

		unsigned char FTM_Mode = 0;
		unsigned short status = 0;
		const unsigned short NV_FTM_MODE_I = 453;
		isOk = QLIB_DIAG_NV_READ_F(m_hQMSL, NV_FTM_MODE_I, &FTM_Mode, 1, &status);
		if (isOk) {
			TRACE(_T("FTM Mode: %s\n"), FTM_Mode ? _T("ON") : _T("OFF"));
			if (FTM_Mode == FALSE) {
				isOk = QLIB_DIAG_CONTROL_F( m_hQMSL, MODE_FTM_F );
			}
		}
		else {
			isOk = QLIB_DIAG_CONTROL_F( m_hQMSL, MODE_FTM_F );
		}
		TRACE(_T("Mode FTM: %s\n"), FTM_Mode ? _T("PASS") : _T("FAIL"));

		if (isOk && FTM_Mode) {
			if (onoff) {
				isOk = QLIB_FTM_WLAN_GEN6_START(m_hQMSL, 3660);
				TRACER_STREAM("QLIB_FTM_WLAN_GEN6_START(3660):" << isOk << "\r\n");
				if (isOk) { //Set TX
					isOk = QLIB_FTM_WLAN_GEN6_ENABLE_CHAINS(m_hQMSL, 6);
					TRACER_STREAM("QLIB_FTM_WLAN_GEN6_ENABLE_CHAINS(6):" << isOk << "\r\n");
				}
				if (isOk) { //set channel
					isOk = QLIB_FTM_WLAN_GEN6_SET_CHANNEL(m_hQMSL, /*iChannel*/7);
					TRACER_STREAM("QLIB_FTM_WLAN_GEN6_SET_CHANNEL(7):" << isOk << "\r\n");
				}
				if (isOk) { //Set rate
					isOk = QLIB_FTM_WLAN_GEN6_SET_TX_FRAME(m_hQMSL, 0, 1000, 200, true, /*cmdRate*/1, /*cmdPreamble*/4);
					TRACER_STREAM("QLIB_FTM_WLAN_GEN6_SET_TX_FRAME(0,1000,200,true,1,4):" << isOk << "\r\n");
				}
				if (isOk) {
					isOk = QLIB_FTM_WLAN_GEN6_CLOSE_TPC_LOOP(m_hQMSL, 0);
					TRACER_STREAM("QLIB_FTM_WLAN_GEN6_CLOSE_TPC_LOOP(0):" << isOk << "\r\n");
				}
				if (isOk) { //Set Packet Gain
					isOk = QLIB_FTM_WLAN_GEN6_SET_TX_WAVEFORM_GAIN_V2(m_hQMSL, /* iPower */1376256);
					TRACER_STREAM("QLIB_FTM_WLAN_GEN6_SET_TX_WAVEFORM_GAIN_V2(21):" << isOk << "\r\n");
				}
				if (isOk) { //TX start
					isOk = QLIB_FTM_WLAN_GEN6_TX_PKT_START_STOP(m_hQMSL, 1);
					TRACER_STREAM("QLIB_FTM_WLAN_GEN6_TX_PKT_START_STOP(1):" << isOk << "\r\n");
				}
			}
			else { //stop
				QLIB_FTM_WLAN_GEN6_START(m_hQMSL, 3660);
				TRACER_STREAM("QLIB_FTM_WLAN_GEN6_START(3660):" << isOk << "\r\n");
				QLIB_FTM_WLAN_GEN6_TX_PKT_START_STOP(m_hQMSL, 0); // stop
				TRACER_STREAM("QLIB_FTM_WLAN_GEN6_TX_PKT_START_STOP(0):" << isOk << "\r\n");
				if (isOk) {
					isOk = QLIB_FTM_WLAN_GEN6_STOP(m_hQMSL);
					TRACER_STREAM("QLIB_FTM_WLAN_GEN6_STOP:" << isOk << "\r\n");
					if (!isOk) { // retry
						isOk = QLIB_FTM_WLAN_GEN6_STOP(m_hQMSL);
						TRACER_STREAM("QLIB_FTM_WLAN_GEN6_STOP:" << isOk << "\r\n");
					}
				}
			}
		}
		else {
			isOk = FALSE;
			strcpy(OutBuffer, "FTM FAIL");
		}

		if (m_hQMSL != NULL) {
			QLIB_DisconnectServer(m_hQMSL);
			m_hQMSL = NULL;
		}
	}
	else {
		isOk = FALSE;
		strcpy(OutBuffer, "QLIB connection FAIL");
	}
	//char command[4] = {Detroit_FT_WLAN,2,0,0};
	//return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
	return isOk;
}

NEPDLLAPI int __stdcall Detroit_WLAN_RXrssi(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) // cmd: 3
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " [FT5311]";
	char command[4] = {Detroit_FT_WLAN,3,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_WLAN_RXset(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) // cmd: 4
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " [FT5301]";
	char command[4] = {Detroit_FT_WLAN,4,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_WLAN_ScanAP(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) // cmd: 4
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " [FT5301]";
	char command[4] = {Detroit_FT_WLAN,5,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_WLAN_ConnectAP(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) // cmd: 4
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " [FT5301]";
	char command[4] = {Detroit_FT_WLAN,6,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_WLANid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_WLAN,7,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

// 15-GPS
NEPDLLAPI int __stdcall Detroit_GPSValueCN0(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	
	char sLibraryVersion[50];
	QLIB_GetLibraryVersion(sLibraryVersion);
	TRACE(_T("QLIB DLL Version: %s\n"), sLibraryVersion);
	TRACER_STREAM("QLIB DLL Version:" << sLibraryVersion << "\r\n");
	//int nCom = atoi(InBuffer);
	BOOL isOk = FALSE;
	HANDLE m_hQMSL = NULL;
	if (m_hQMSL) {
		if (QLIB_IsPhoneConnected(m_hQMSL)) {
			QLIB_FlushRxBuffer(m_hQMSL);
		}
	}
	else {
		QLIB_SetLibraryMode(false);
		//QLIB_SetLibraryMode(true);
		//unsigned int nComPort = atoi(InBuffer);
		unsigned short nComPort = -1;
		QLIB_GetComPortNumber(m_hQMSL, &nComPort);
		m_hQMSL = QLIB_ConnectServer(nComPort);
		//m_hQMSL = QLIB_ConnectServerWithWait(nComPort, 5000);
		if (m_hQMSL == NULL) {
			//m_hQMSL = QLIB_ConnectServerWithWait(nComPort, 5000);
			//m_hQMSLPhone = QLIB_ConnectServerWithHandShake((unsigned)atoi(m_strCOMport.c_str()));
			
			QLIB_GetComPortNumber(m_hQMSL, &nComPort);
			if (nComPort == QLIB_COM_AUTO_DETECT) {
				TRACE(_T("COM Port: AUTO, NOT DETECTED\n"));
			}
		}
		if (m_hQMSL != NULL) {
			if (QLIB_IsPhoneConnected(m_hQMSL)) {
				unsigned long iESN;
				QLIB_DIAG_READ_ESN_F(m_hQMSL, &iESN);
				TRACE(_T("ESN:%d\n"), iESN);
				QLIB_FlushRxBuffer(m_hQMSL);
			}
		}
	}
	double CNoDBHz = 0.0;
	int FreqInHz = 0;
	if (m_hQMSL != NULL) {
		unsigned long MSM_HW_Version = 0;
		unsigned long MobModel = 0;
		char MobSwRev[512];
		char ModelStr[512];
		if (QLIB_DIAG_EXT_BUILD_ID_F(m_hQMSL, &MSM_HW_Version, &MobModel, MobSwRev, ModelStr)) {
			TRACE(_T("Ext MSM HW Version: 0x%X\n"), MSM_HW_Version);
			TRACE(_T("Ext Mobile Model#: 0x%X\n"), MobModel);
			TRACE(_T("Ext Mobile Mobile SW Rev: %s\n"), CA2T(MobSwRev));
			TRACE(_T("Ext Model ID: %s\n"), CA2T(ModelStr));
			TRACER_STREAM("Ext MSM HW Version: 0x" << MSM_HW_Version << "\r\n");
			TRACER_STREAM("Ext Mobile Model#: 0x" << MobModel << "\r\n");
			TRACER_STREAM("Ext Mobile Mobile SW Rev: " << MobSwRev << "\r\n");
			TRACER_STREAM("Ext Model ID: " << ModelStr << "\r\n");
		}
		else {
			TRACE(_T("QLIB_DIAG_EXT_BUILD_ID_F = FAIL\n"));
			TRACER_STREAM("QLIB_DIAG_EXT_BUILD_ID_F = FAIL\r\n");
		}

		unsigned char FTM_Mode = 0;
		unsigned short status = 0;
		const unsigned short NV_FTM_MODE_I = 453;
		isOk = QLIB_DIAG_NV_READ_F(m_hQMSL, NV_FTM_MODE_I, &FTM_Mode, 1, &status);
		TRACER_STREAM("QLIB_DIAG_NV_READ_F:" << isOk << "\r\n");
		if (isOk) {
			TRACE(_T("FTM Mode: %s\n"), FTM_Mode ? _T("ON") : _T("OFF"));
			if (FTM_Mode == FALSE) {
				isOk = QLIB_DIAG_CONTROL_F( m_hQMSL, MODE_FTM_F );
			}
		}
		else {
			isOk = QLIB_DIAG_CONTROL_F( m_hQMSL, MODE_FTM_F );
		}
		TRACE(_T("Mode FTM: %s\n"), FTM_Mode ? _T("PASS") : _T("FAIL"));

		if (isOk && FTM_Mode) {
			unsigned char gps_status;
			isOk = QLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH_WithWait(
				m_hQMSL, GPS_SA_RF_VERIF_MODE_SWITCH_ENTER_MODE, &gps_status);
			TRACER_STREAM("QLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH_WithWait(0):" << isOk << "\r\n");
			if (isOk) {
				CGPS_Gen8_Rx_RfMode_Struct_type rfmode;
				rfmode.u_RfMode = 0;
				isOk = QLIB_GPS_GEN8_PRx_Mode_Config(m_hQMSL, &rfmode);
				TRACER_STREAM("QLIB_GPS_GEN8_PRx_Mode_Config:" << isOk << "\r\n");
			}
			Sleep(1000);
			if (isOk) {
				for (int retry = 0; retry < 10; retry++) {
					isOk =
						QLIB_DIAG_GEN8_GET_EMBEDDED_CALCULATED_CTON(m_hQMSL, 1, 32, 10, &CNoDBHz, &FreqInHz);
					TRACER_STREAM(
						"QLIB_DIAG_GEN8_GET_EMBEDDED_CALCULATED_CTON(1,32,10):" << isOk << "->"
						<< CNoDBHz << "," << FreqInHz << "\r\n");
					if (isOk) {
						break;
					}
				}
			}
			QLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH_WithWait(
				m_hQMSL, GPS_SA_RF_VERIF_MODE_SWITCH_EXIT_MODE, &gps_status);
				TRACER_STREAM("QLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH_WithWait(0):" << isOk << "\r\n");
		}

		if (m_hQMSL != NULL) {
			QLIB_DisconnectServer(m_hQMSL);
			m_hQMSL = NULL;
		}
	}

	//char* error_code = " [FT5450]";
	//char command[4] = {Detroit_FT_GPS,1,0,0};
	//return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
	_snprintf(OutBuffer, 64, "%f", CNoDBHz);
	return isOk ? HS_OK : HS_FAIL;
}

// 16-FMR
NEPDLLAPI int __stdcall Detroit_FMRrssi(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " [FT5502]";
	char command[4] = {Detroit_FT_FMR,1,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_FMRModuleOn(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_FMR,4,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

// 17-NFC
NEPDLLAPI int __stdcall Detroit_NFCid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_NFC,1,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_NFCFWVersion(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_NFC,2,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_NFCAntennaSelftest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_NFC,3,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_NFCSwpSelftest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_NFC,4,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

// 18-Sensor
NEPDLLAPI int __stdcall Detroit_SNSRgsensorID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_SNSR,1,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}
NEPDLLAPI int __stdcall Detroit_SNSRgsensorAcc(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_SNSR,2,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}
NEPDLLAPI int __stdcall Detroit_SNSRgsensorCalib(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_SNSR,3,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRecompassID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_SNSR,4,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRecompassValue(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_SNSR,5,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}


NEPDLLAPI int __stdcall Detroit_SNSRlsensorID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_SNSR,7,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRlsensorValue(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_SNSR,8,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRlsensorCalib(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_SNSR,9,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRpsensorID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_SNSR,10,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRpsensorValue(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_SNSR,11,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRpsensorCalib(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_SNSR,12,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRgyroscopeID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_SNSR,13,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRgyroscopeValue(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_SNSR,14,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRgyroscopeCalib(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_SNSR,15,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRGyroscopeSelfTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_SNSR,16,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRgsensorSelfTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_SNSR,17,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSREcompassSelfTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_SNSR,18,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRloadFALPkdata(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_SNSR,99,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}


// 19-LED
NEPDLLAPI int __stdcall Detroit_LEDid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_LED,1,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}
NEPDLLAPI int __stdcall Detroit_LEDon(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_LED,2,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

// 20-SD
NEPDLLAPI int __stdcall Detroit_SDdetect(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_SD,1,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SDtestStart(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_SD,2,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SDtestStop(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_SD,3,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SDinfo(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_SD,4,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SDdetectPin(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_SD,5,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

// 21-ID
NEPDLLAPI int __stdcall Detroit_CPUid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_ID,1,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_BoardADC(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_ID,2,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_BoardRev(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_ID,3,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_RFid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_ID,4,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_RFICid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_ID,8,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_MHLid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_ID,5,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

//90:OS
NEPDLLAPI int __stdcall Detroit_OSgetprop(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_OS,1,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_OSkeyPower(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_OS,2,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_OSpwr2home(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)//cmd:2
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_OS,3,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

//90-2 shutdown when disconnect
NEPDLLAPI int __stdcall Detroit_ShutDown(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_OS,4,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_OSkeyFocus(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_OS,5,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_OSkeyCamera(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_OS,6,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}


//91:version
NEPDLLAPI int __stdcall Detroit_VERftd(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_VER,1,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_VERchange(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* input = "ro.build.version.incremental";
	char* error_code = " [empty]";
	return Detroit_OSgetprop(nPort, nTimeout, input, OutBuffer);
}


//92:factory
NEPDLLAPI int __stdcall Detroit_FAC_FARead(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) //cmd:1
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_FAC,1,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_FAC_FAWrite(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) //cmd:2
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_FAC,2,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_FAC_Picasso(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) //cmd:3
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_FAC,3,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_FAC_OTPRead(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) //cmd:4
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_FAC,4,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_FAC_OTPWrite(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) //cmd:5
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_FAC,5,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_FAC_OTPEnable(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) //cmd:6
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_FAC,6,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_FAC_CFGRead(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) //cmd:7
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_FAC,7,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_FAC_CFGWrite(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) //cmd:8
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {Detroit_FT_FAC,8,0,0};
	return MobileCmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}



//92,***************** SPU **********************************************

NEPDLLAPI int Detroit_FAC_CFGRead_SPU(char* Port, char* Timeout, char* InBuffer, char* OutBuffer, char* ErrorCode) //cmd:1
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	int i_port = 0;
	int i_timeout = 0;
	i_port = atoi(Port);
	i_timeout = atoi(Timeout);

	int nRet = InitDevice(i_port, i_timeout);

	if(nRet == HS_OK)
	{
		char OutBuffer1[4096] ={0};
		nRet = Detroit_FAC_CFGRead(i_port, i_timeout, InBuffer, OutBuffer1);
		
		FreeDevice(i_port, i_timeout);

		//memcpy(OutBuffer, OutBuffer1, 20);
		strcpy(OutBuffer, OutBuffer1);
	}

	sprintf(ErrorCode, "%d", nRet);
	
	return nRet;
}


NEPDLLAPI int Detroit_FAC_CFGWrite_SPU(char* Port, char* Timeout, char* InType, char* InValue, char* OutBuffer, char* ErrorCode) //cmd:1
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	int i_port = 0;
	int i_timeout = 0;
	i_port = atoi(Port);
	i_timeout = atoi(Timeout);

	int nRet = InitDevice(i_port, i_timeout);

	if(nRet == HS_OK)
	{
		
		char OutBuffer1[4096] ={0};
		strcat(InType,_T(","));
		strcat(InType, InValue);
		nRet = Detroit_FAC_CFGWrite(i_port, i_timeout, InType, OutBuffer1);
		
		FreeDevice(i_port, i_timeout);

		//memcpy(OutBuffer, OutBuffer1, 512*sizeof(char));
	}

	sprintf(ErrorCode, "%d", nRet);
	
	return nRet;
}

NEPDLLAPI int Detroit_AutoFtdMode_SPU(char* Port, char* Timeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	return Detroit_AutoFtdMode(atoi(Port), atoi(Timeout), ErrorCode);
}

NEPDLLAPI int Detroit_PWRbattCapacity_SPU(char* Port, char* Timeout, char* InBuffer, char* OutBuffer, char* ErrorCode) //cmd:1
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	int i_port = 0;  

	int i_timeout = 0;
	int nBattCapacity = 0;
	i_port = atoi(Port);
	i_timeout = atoi(Timeout);

	int nRet = InitDevice(i_port, i_timeout);
	//memset(InBuffer, 0, sizeof(InBuffer));

	if(nRet == HS_OK)
	{
		char OutBuffer1[4096] ={0};
		nRet = Detroit_PWRbattCapacity(i_port, i_timeout, InBuffer, OutBuffer1);

		FreeDevice(i_port, i_timeout);

		memcpy(OutBuffer, OutBuffer1, 512*sizeof(char));
	}

	if(nRet == HS_OK)
	{
		nBattCapacity = atoi(OutBuffer);  
		if (nBattCapacity<50){ 
			AfxMessageBox("电量不足，请送充电站");
			nRet = -1;
		}
	}else{
		AfxMessageBox("read Batter Capacity FAIL,读不到电量，请确认系统版本");
		nRet = -1;
	}

	sprintf(ErrorCode, "%d", nRet);
	return nRet;
}

NEPDLLAPI int Detroit_FAC_FARead_SPU(char* Port, char* Timeout, char* InBuffer, char* OutBuffer, char* ErrorCode) //cmd:1
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	int i_port = 0;
	int i_timeout = 0;
	i_port = atoi(Port);
	i_timeout = atoi(Timeout);

	int nRet = InitDevice(i_port, i_timeout);

	if(nRet == HS_OK)
	{
		char OutBuffer1[4096] ={0};
		nRet = Detroit_FAC_FARead(i_port, i_timeout, InBuffer, OutBuffer1);

		FreeDevice(i_port, i_timeout);

		memcpy(OutBuffer, OutBuffer1, 512*sizeof(char));
	}

	sprintf(ErrorCode, _T("%d"), nRet);

	return nRet;
}

NEPDLLAPI int Detroit_FAC_OTPRead_SPU(char* Port, char* Timeout, char* InBuffer, char* OutBuffer, char* ErrorCode) //cmd:1
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	int i_port = 0;
	int i_timeout = 0;
	i_port = atoi(Port);
	i_timeout = atoi(Timeout);

	int nRet = InitDevice(i_port, i_timeout);
	if(nRet == HS_OK)
	{
		char OutBuffer1[4096] ={0};
		nRet = Detroit_FAC_OTPRead(i_port, i_timeout, InBuffer, OutBuffer1);
		//Qisda YTT
		//if IMEI is empty ,detroit ftd pass data ' []'  32 91 93
		if(nRet !=1)
		{	
			//LionTest20120704
			return nRet;
		    //memset(OutBuffer1,0,sizeof(OutBuffer1));
		    //strncpy_s(OutBuffer1,_T("000000000000000"),15); 
		    //nRet = HS_OK;
		}

		FreeDevice(i_port, i_timeout);
		memcpy(OutBuffer, OutBuffer1, 512*sizeof(char));
	}

	sprintf(ErrorCode, _T("%d"), nRet);

	return nRet;
}

NEPDLLAPI int Detroit_FAC_OTPWrite_SPU(char* Port, char* Timeout, char* InType, char* InValue, char* OutBuffer, char* ErrorCode) //cmd:1
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	int i_port = 0;
	int i_timeout = 0;
	i_port = atoi(Port);
	i_timeout = atoi(Timeout);

	/*if exist IMEI and compare PASS , return true, 20120720 LION*/
	if (Detroit_CheckOTPExist(Port, Timeout, InType, InValue, OutBuffer))
	{
		return true;
	}

	int nRet = InitDevice(i_port, i_timeout);

	if(nRet == HS_OK)
	{
		char OutBuffer1[4096] ={0};
		strcat(InType,_T(","));
		strcat(InType, InValue);
		nRet = Detroit_FAC_OTPWrite(i_port, i_timeout, InType, OutBuffer1);

		FreeDevice(i_port, i_timeout);

		memcpy(OutBuffer, OutBuffer1, 512*sizeof(char));
	}

	sprintf(ErrorCode, _T("%d"), nRet);

	return nRet;
}

NEPDLLAPI bool Detroit_CheckOTPExist(char* Port, char* Timeout, char* InType, char* InValue, char* OutBuffer){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	int i_port = 0;
	int i_timeout = 0;
	i_port = atoi(Port);
	i_timeout = atoi(Timeout);

	int nRet = InitDevice(i_port, i_timeout);

	if(nRet == HS_OK)
	{

		char OutBuffer1[4096] ={0};
		nRet = Detroit_FAC_OTPRead(i_port, i_timeout, InType, OutBuffer1);
		
		FreeDevice(i_port, i_timeout);
		if(nRet)
		{
			if ( strcmp(InValue,OutBuffer1) == 0 ){
				return true;
			}
		}

	}

	//sprintf(ErrorCode, _T("%d"), nRet);

	return false;
}

NEPDLLAPI int Detroit_FAC_OTPEnable_SPU(char* Port, char* Timeout, char* InType, char* InValue, char* OutBuffer, char* ErrorCode) //cmd:1
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	int i_port = 0;
	int i_timeout = 0;
	i_port = atoi(Port);
	i_timeout = atoi(Timeout);

	int nRet = InitDevice(i_port, i_timeout);

	if(nRet == HS_OK)
	{
		char OutBuffer1[4096] ={0};
		strcat(InType,_T(","));
		strcat(InType, InValue);
		nRet = Detroit_FAC_OTPEnable(i_port, i_timeout, InType, OutBuffer1);

		FreeDevice(i_port, i_timeout);

		memcpy(OutBuffer, OutBuffer1, 512*sizeof(char));
	}

	sprintf(ErrorCode, _T("%d"), nRet);

	return nRet;
}



NEPDLLAPI bool Detroit_ExecAdbOut_SPU(char* Command, char* output, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	return Detroit_ExecAdbOut(Command, output, ErrorCode);

}


NEPDLLAPI int Detroit_FAC_FactoryReset_SPU(char* Port, char* Timeout, char* InBuffer, char* OutBuffer, char* ErrorCode) //cmd:3
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	int i_port = 0;
	int i_timeout = 0;
	i_port = atoi(Port);
	i_timeout = atoi(Timeout);

	int nRet = InitDevice(i_port, i_timeout);

	if(nRet == HS_OK)
	{
		char OutBuffer1[4096] ={0};
		//nRet = C7_FAC_FactoryReset(i_port, i_timeout, InBuffer, OutBuffer1);

		FreeDevice(2000, i_timeout);

//		memcpy(OutBuffer, OutBuffer1, *sizeof(char));
	}

	sprintf(ErrorCode, _T("%d"), nRet);

	return nRet;
}


NEPDLLAPI int Detroit_TestFtdMode_SPU(char* Port, char* Timeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	return Detroit_TestFtdMode(atoi(Port), atoi(Timeout), ErrorCode);
}

//90-4 shutdown system when disconnect
NEPDLLAPI int Detroit_FAC_ShutDown_SPU(char* Port, char* Timeout, char* InValue, char* OutBuffer, char* ErrorCode) //cmd:1
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	int i_port = 0;
	int i_timeout = 0;
	i_port = atoi(Port);
	i_timeout = atoi(Timeout);

	int nRet = InitDevice(i_port, i_timeout);

	if(nRet == HS_OK)
	{
		char OutBuffer1[4096] ={0};
		//strcat(InType,_T(","));
		//strcat(InType, InValue);
		nRet = Detroit_ShutDown(i_port, i_timeout, InValue, OutBuffer1);

		FreeDevice(i_port, i_timeout);
		
		memcpy(OutBuffer, OutBuffer1, 512*sizeof(char));


	}

	sprintf(ErrorCode, _T("%d"), nRet);

	return nRet;
}


NEPDLLAPI int Detroit_CheckIMEIStatus_SPU(char* Port, char* Timeout, char* InBuffer, char* OutBuffer, char* ErrorCode) //cmd:1
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	int i_port = 0;
	int i_timeout = 0;
	i_port = atoi(Port);
	i_timeout = atoi(Timeout);

	int nRet = InitDevice(i_port, i_timeout);
	if(nRet == HS_OK)
	{
		if ((strcmp(InBuffer, _T("I")) == 0 ) || (strcmp(InBuffer, _T("F")) == 0 )){//if I or F, continue, In Fcheck , Lion 20121206
			return true;
	    }
		return false;
	}

	sprintf(ErrorCode, _T("%d"), nRet);
	return nRet;
}
//****************** SPU *********************************************

NEPDLLAPI bool Detroit_SplitSimData_SPU(char* input, char* output, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	return Detroit_SplitSimData(input, output, ErrorCode);
}

NEPDLLAPI bool __stdcall Detroit_SplitSimData(char* input, char* output, char* ErrorCode)
{
	CString csInput;
	csInput.Format(_T("%s"), input);
	int nInputLength = csInput.GetLength();
	CString csSimData = csInput.Mid(256, (nInputLength-256));
	strcpy( output, (LPCTSTR)csSimData);
	
	return true;
}

NEPDLLAPI bool Detroit_SplitPublicKey_SPU(char* input, char* output, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	return Detroit_SplitPublicKey(input, output, ErrorCode);
}

NEPDLLAPI bool __stdcall Detroit_SplitPublicKey(char* input, char* output, char* ErrorCode)
{
	CString csInput( input );
	int nInputLength = csInput.GetLength();
	CString csPublicKey = csInput.Left(256);
	strcpy( output, (LPCTSTR)csPublicKey);
	
	return true;
}