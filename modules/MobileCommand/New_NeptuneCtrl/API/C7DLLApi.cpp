#include "stdafx.h"
#include <cstddef> // size_t

#include "../CmdType.h"
#include "GeneralDLLApi.h" // GeneraCmd
#include "C7DLLApi.h"
#include "io.h"
#include "../Utility.h"
#include "../FactoryBlock.h"

typedef bool (__cdecl *PicassoCmd)(char*, char*, char*);

static const int CMD_MAX_RETRY = 1;

static int c7cmd(int nPort, int nTimeout, char* command,
					 char* InBuffer, size_t InSize, char* OutBuffer,
					 const char* error_code, const char* Func)
{
	int nRetCode = -1*ERROR_FUNCTION_FAILED;
	for (int nTry = 0; nTry < CMD_MAX_RETRY; nTry++) {
		TRACE(_T("GeneralCmdInOut\n"));
		nRetCode = GeneralCmdInOut(nPort, nTimeout, command, InBuffer, InSize, OutBuffer);
		if (nTry == 0 && nRetCode != HS_OK) {
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

NEPDLLAPI bool C7_ExecAdbOut_SPU(char* Command, char* output, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	return C7_ExecAdbOut(Command, output, ErrorCode);

}

NEPDLLAPI bool __stdcall C7_ExecAdbOut(CString Command, char* output, char* ErrorCode)
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

int C7_EnterFtdMode(int nTimeout, char* ErrorCode)
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

int C7_CheckFtdMode(char* ErrorCode)
{
	bool isOk;
	int nRetCode = -1*ERROR_FUNCTION_FAILED;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char *msgBuffer = new char[1024 * 1024 * 2];
	isOk = C7_ExecAdbOut("shell ps", msgBuffer, ErrorCode);

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

NEPDLLAPI int __stdcall C7_TestFtdMode(int nPort, int nTimeout, char* ErrorCode)
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

	nRetCode = C7_CheckFtdMode(ErrorCode);
	if(nRetCode != HS_OK) {
		strcpy(ErrorCode, "ERROR: Check ftd mode fail");
		nRetCode = -1*ERROR_BAD_COMMAND;
	}

	return nRetCode;
}

NEPDLLAPI int C7_TestFtdMode_SPU(char* Port, char* Timeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	return C7_TestFtdMode(atoi(Port), atoi(Timeout), ErrorCode);
}

NEPDLLAPI int __stdcall C7_AutoFtdMode(int nPort, int nTimeout, char* ErrorCode)
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

	nRetCode = C7_CheckFtdMode(ErrorCode);
	if (nRetCode != HS_OK) {
		nRetCode = C7_EnterFtdMode(0, ErrorCode);
		if(nRetCode != HS_OK) {
			strcpy(ErrorCode, "ERROR: Enter ftd mode fail");
			nRetCode = -1*ERROR_BAD_COMMAND;
		}
		else {
			nRetCode = C7_CheckFtdMode(ErrorCode);
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

NEPDLLAPI int C7_AutoFtdMode_SPU(char* Port, char* Timeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	return C7_AutoFtdMode(atoi(Port), atoi(Timeout), ErrorCode);
}


// 01-Battery
NEPDLLAPI int __stdcall C7_BATTcoincellVoltage(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_BATT,1,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_BATTcoincellChargeOn(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_BATT,2,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_BATTrtcTime(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_BATT,3,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

// 02-Power
NEPDLLAPI int __stdcall C7_PWRgaugeID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_PWR,1,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_PWRgaugeVoltage(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_PWR,2,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_PWRgaugeCurrent(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_PWR,3,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_PWRgaugeTemperature(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_PWR,4,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_PWRchargerID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_PWR,5,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_PWRchargerOn(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_PWR,6,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_DFIupgrade(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_PWR,7,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int C7_DFIenable_SPU(char* Port, char* Timeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	int nRet = InitDevice(atoi(Port), atoi(Timeout));

	if(nRet == HS_OK)
	{
		char* error_code = " []";
		char command[4] = {C7_FT_PWR,8,0,0};
		char OutBuffer1[4096] ={0};

		nRet = c7cmd(atoi(Port), atoi(Timeout), command, InBuffer, strlen(InBuffer), OutBuffer1, error_code, __FUNCTION__);

		FreeDevice(2000, atoi(Timeout));

		sprintf(OutBuffer, _T("%d"), nRet);
	}

	return nRet;
}

//03:Memory
NEPDLLAPI int __stdcall C7_MCPflashID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_MCP,1,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_MCPddrID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_MCP,2,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}


//04:LCM - LCD & Touch
NEPDLLAPI int __stdcall C7_TOUCHpoint(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_LCM,1,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_TOUCHtest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_LCM,2,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_TOUCHid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_LCM,3,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_TOUCHversion(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_LCM,4,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_LCDonoff(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_LCM,5,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_TOUCHSelfTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_LCM,6,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

//06:audio
NEPDLLAPI int __stdcall C7_AUoutSwitch(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_AU,1,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_AUplaymp3(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_AU,2,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_AUrecordWAV(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_AU,3,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_AUplayWAV(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_AU,4,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}


//06:camera
NEPDLLAPI int __stdcall C7_CameraID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_CAM,1,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_CAMshot(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_CAM,2,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_CAMstart(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_CAM,3,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_CAMindexReset(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_CAM,4,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

//07:keypad
NEPDLLAPI int __stdcall C7_KPtouchTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_KEY,1,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

//13:BT
NEPDLLAPI int __stdcall C7_BTid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_BT,1,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_BTTestMode(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_BT,2,0,0};
	//GeneralCmdInOut(nPort, nTimeout, command, InBuffer, OutBuffer);
	//return HS_OK;
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_BTTXPower(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_BT,3,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

// 14-WLAN
NEPDLLAPI int __stdcall C7_WLAN_Mode(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_WLAN,1,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_WLAN_TXPower(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_WLAN,2,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_WLAN_RXrssi(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) // cmd: 3
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_WLAN,3,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_WLAN_RXset(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) // cmd: 4
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_WLAN,4,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_WLAN_LoadFABin(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) // cmd: 5
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_WLAN,5,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_WLAN_LoadNextBin(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) // cmd: 6
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_WLAN,6,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_WLAN_SaveBin(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) // cmd: 7
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_WLAN,7,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}


// 15-GPS
NEPDLLAPI int __stdcall C7_GPSValueCN0(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_GPS,1,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

// 16-FMR
NEPDLLAPI int __stdcall C7_FMRrssi(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_FMR,1,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

// 17-NFC
NEPDLLAPI int __stdcall C7_NFCid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_NFC,1,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_NFCselftest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_NFC,2,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

// 18-Sensor
NEPDLLAPI int __stdcall C7_SNSRgsensorID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_SNSR,1,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}
NEPDLLAPI int __stdcall C7_SNSRgsensorAcc(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_SNSR,2,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}
NEPDLLAPI int __stdcall C7_SNSRgsensorCalib(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_SNSR,3,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_SNSRecompassID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_SNSR,4,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_SNSRecompassSelftest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_SNSR,30,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_SNSRlsensorID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_SNSR,7,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_SNSRlsensorValue(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_SNSR,8,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_SNSRlsensorCalib(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_SNSR,9,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_SNSRpsensorID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_SNSR,10,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_SNSRpsensorValue(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_SNSR,11,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_SNSRpsensorCalib(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_SNSR,12,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_SNSRloadFALPkdata(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_SNSR,99,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}


// 20-LED
NEPDLLAPI int __stdcall C7_LEDid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_LED,1,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}


// 21-ID
NEPDLLAPI int __stdcall C7_IDcpu(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_ID,1,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_BoardADC(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_ID,2,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_BoardRev(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_ID,3,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

//90:OS
NEPDLLAPI int __stdcall C7_OSgetprop(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_OS,1,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_OSpwr2home(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)//cmd:2
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_OS,2,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_OSkeyBack(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_OS,3,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_OSkeyFocus(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_OS,4,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_OSkeyCamera(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_OS,5,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_OSmodeSleep(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_OS,6,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

//91:version
NEPDLLAPI int __stdcall C7_VERftd(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_VER,1,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_VERchange(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* input = "ro.build.version.incremental";
	char* error_code = " [empty]";
	return C7_OSgetprop(nPort, nTimeout, input, OutBuffer);
}

//92:factory
NEPDLLAPI int __stdcall C7_FAC_FARead(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) //cmd:1
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_FAC,1,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int C7_FAC_FARead_SPU(char* Port, char* Timeout, char* InBuffer, char* OutBuffer, char* ErrorCode) //cmd:1
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
		nRet = C7_FAC_FARead(i_port, i_timeout, InBuffer, OutBuffer1);

		FreeDevice(i_port, i_timeout);

		memcpy(OutBuffer, OutBuffer1, 512*sizeof(char));
	}

	sprintf(ErrorCode, _T("%d"), nRet);

	return nRet;
}

NEPDLLAPI int __stdcall C7_FAC_FAWrite(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) //cmd:2
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_FAC,2,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_FAC_Picasso(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) //cmd:3
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_FAC,3,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_FAC_WifiDataPath(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) //cmd:3
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_FAC,5,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_FAC_FactoryReset(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) //cmd:3
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_FAC,6,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_FAC_FARead301(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* fa_301_pos = "512,48,2012";
	char* error_code = " [empty]";
	return C7_FAC_FARead(nPort, nTimeout, fa_301_pos, OutBuffer);
}

NEPDLLAPI int __stdcall C7_FAC_FARead303(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* fa_303_pos = "1152,48,2012";
	char* error_code = " [empty]";
	return C7_FAC_FARead(nPort, nTimeout, fa_303_pos, OutBuffer);
}

NEPDLLAPI int __stdcall C7_FAC_FARead304(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* fa_304_pos = "560,12,2012";
	char* error_code = " [empty]";
	return C7_FAC_FARead(nPort, nTimeout, fa_304_pos, OutBuffer);
}

NEPDLLAPI int C7_FAC_FactoryReset_SPU(char* Port, char* Timeout, char* InBuffer, char* OutBuffer, char* ErrorCode) //cmd:3
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
		nRet = C7_FAC_FactoryReset(i_port, i_timeout, InBuffer, OutBuffer1);

		FreeDevice(2000, i_timeout);

//		memcpy(OutBuffer, OutBuffer1, *sizeof(char));
	}

	sprintf(ErrorCode, _T("%d"), nRet);

	return nRet;
}
NEPDLLAPI int __stdcall C7_FAC_ClearNFCVariable(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char* error_code = " []";
	char command[4] = {C7_FT_FAC,7,0,0};
	return c7cmd(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, error_code, __FUNCTION__);
}

