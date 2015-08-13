#include "stdafx.h"
#include <cstddef> // size_t

#include "../CmdType.h"
#include "GeneralDLLApi.h" // GeneraCmd
#include "NvdaDLLApi.h"
#include "io.h"
#include "../Utility.h"
#include "../FactoryBlock.h"

typedef bool (__cdecl *PicassoCmd)(char*, char*, char*);


static int DtCmdNvda(int nPort, int nTimeout, char* command,
					 char* InBuffer, size_t InSize, char* OutBuffer,
					 const char* error_code, const char* Func)
{
	int nRetCode = -1*ERROR_FUNCTION_FAILED;
	for (int nTry = 0; nTry < NV_DT_CMD_MAX_RETRY; nTry++) {
		if (InBuffer != NULL && OutBuffer != NULL) {
			TRACE(_T("GeneralCmdInOut "));
			nRetCode = GeneralCmdInOut(nPort, nTimeout, command, InBuffer, InSize, OutBuffer);
			if (nTry == 0 && nRetCode != HS_OK) {
				strncat(OutBuffer, error_code, strlen(error_code));
			}
		}
		else if (InBuffer != NULL) {
			TRACE(_T("GeneralCmdIn "));
			size_t nInLength = (InSize == 0) ? strlen(InBuffer) : InSize;
			nRetCode = GeneralCmdIn(nPort, nTimeout, command, InBuffer, nInLength);
		}
		else if (OutBuffer != NULL) {
			TRACE(_T("GeneralCmdOut "));
			nRetCode = GeneralCmdOut(nPort, nTimeout, command, OutBuffer);
			if (nTry == 0 && error_code != NULL && nRetCode != HS_OK) {
				strncat(OutBuffer, error_code, strlen(error_code));
			}
		}
		else {
			TRACE(_T("GeneralCmd "));
			nRetCode = GeneralCmd(nPort, nTimeout, command);
			if (nTry == 0 && error_code != NULL && nRetCode != HS_OK) {
				strncat(OutBuffer, error_code, strlen(error_code));
			}
		}
		TRACE(_T("%s(%d)\n"), Func, nRetCode);
		if (nRetCode == HS_OK || nRetCode == HS_EMPTY) {
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

NEPDLLAPI bool __stdcall ExecAdbOut(CString Command, char* output, char* ErrorCode)
{
	bool isOk = false;
	DWORD nPipeSize = 1024 * 1024; //1M pipeline

	/* 110913 Gilpin.Xi Add Start */
	int i_timeout = 0;
	bool b_NeedResponse = true;

	if (Command.Find(_T("shell logcat")) != -1
		||Command.Find(_T("shell cat")) != -1
		|| Command.Find(_T("shell test_logd logd2ext")) != -1)
	{
		i_timeout = 2*1000;
	}
	else
	{
		i_timeout = 10*1000;	//10s
	}

	if (Command.Find(_T("kill-server")) != -1
		||Command.Find(_T("shell logcat")) != -1
		||Command.Find(_T("shell cat")) != -1
		||Command.Find(_T("reboot")) != -1
		||Command.Find(_T("shell test_logd logd2ext")) != -1
		)
	{
		b_NeedResponse = false;
	} 
	/* 110913 Gilpin.Xi Add End */

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
	/* 110913 Gilpin.Xi Add Start */
	if((Command.Find(_T("pull "))==-1) && (Command.Find(_T("push "))==-1) )
	{
		Command = _T("cmd.exe /c ") + Command;
	}

	/* 110913 Gilpin.Xi Add End */
	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo)) {
		Command.ReleaseBuffer();
		/* 110913 Gilpin.Xi modify Start */
		//DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, 10*1000); // timeout in 10 seconds
		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, i_timeout);
		/* 110913 Gilpin.Xi modify End */

		//if timeout then exit the process
		if(TimeOutSignal == WAIT_TIMEOUT) {
			isOk = false;
			TerminateProcess(processInfo.hProcess, 0);
			strcpy(ErrorCode, "ERROR: Adb timeout");
		}
		else {
				isOk = true;
			/* 111003 Gilpin.Xi Add Start */
			if (b_NeedResponse)
			/* 111003 Gilpin.Xi Add End */
			{
				DWORD bytesRead;
				char *message = new char[nPipeSize];
				memset(message, 0, sizeof(message));
				::ReadFile(hRead, message, nPipeSize, &bytesRead, NULL);
				message[bytesRead] = '\0';
				
				if (output != NULL)
				{
					strcpy(output, message);
				}
				
				strcpy(ErrorCode, "Adb command ok");
				delete [] message;
			}
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
	bool isOk;
	int nRetCode = -1*ERROR_FUNCTION_FAILED;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
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

NEPDLLAPI int __stdcall NVDA_TestFtdMode(int nPort, int nTimeout, char* ErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	int nRetCode = -1*ERROR_FUNCTION_FAILED;
	/*CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));
	CString adb_path = pthToolDir + _T("\\adb.exe");
	CString param = _T("kill-server");
	Exec(adb_path, param, nTimeout, false);*/

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

	int nRetCode = -1*ERROR_FUNCTION_FAILED;
	/*CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));
	CString adb_path = pthToolDir + _T("\\adb.exe");
	CString param = _T("kill-server");
	Exec(adb_path, param, nTimeout, false);*/

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
	int nRetCode = -1*ERROR_FUNCTION_FAILED;
	unsigned char FaInfo[4096];
	memset(FaInfo, 0, 4096);
	/* 111123 Gilpin.Xi delete because FA define is changed in Orange project */
	//nRetCode = NVDA_FACTORY_FARead(nPort, nTimeout, (char*)&FaInfo);
	/* 111123 Gilpin.Xi delete because FA define is changed in Orange project */
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
	char* error_code = " [F220]";
	strncat(ErrorCode, error_code, strlen(error_code));
	return PowerOnToUARTPacketMode(nPort, nTimeout, false);
}

// 1-SD
NEPDLLAPI int __stdcall NVDA_SD_DetectStatus(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SD,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SD_OperateTest(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SD,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SD_Mount(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SD,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SD_Unmount(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SD,4,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SD_CheckeMMCSetting(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SD,5,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SD_GetInfo(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SD,6,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SD_DeleteDCIM(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SD,7,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

// 2-CAMERA
NEPDLLAPI int __stdcall NVDA_CAMERA_ReadId(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_CAMERA,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_CAMERA_8MTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_CAMERA,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_CAMERA_5MTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_CAMERA,4,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_CAMERA_StoreInSDCard(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_CAMERA,5,0,0};
	*InBuffer = '1';
	*(InBuffer + 1) = 0;
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_CAMERA_ChooseWBMode(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_CAMERA,6,0,0};
	*InBuffer = '1';
	*(InBuffer + 1) = 0;
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_CAMERA_1D3MTest(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_CAMERA,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

/*sunlight add for cairo 20120706*/
NEPDLLAPI int __stdcall NVDA_CAMERA_1D3MTest_New(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_CAMERA,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}


NEPDLLAPI int __stdcall NVDA_CAMERA_ClearIndex(int nPort, int nTimeout, char* OutBuffer)
{  //clear picture index
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_CAMERA,4,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}
/*sunlight add end , 20120706*/

// 3-Display
NEPDLLAPI int __stdcall NVDA_Display_LCDRGB(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_DISPLAY,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_Display_DetectHDMI(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_DISPLAY,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_Display_HDMIRGB(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_DISPLAY,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_Display_LCDRLE(int nPort, int nTimeout, char* InBuffer, char* p_sz_OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_DISPLAY,4,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), p_sz_OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_Display_CloseLCD(int nPort, int nTimeout, char* p_sz_OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_DISPLAY,5,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, p_sz_OutBuffer, " []", __FUNCTION__);
}

// 4-SENSOR
NEPDLLAPI int __stdcall NVDA_SENSOR_GSENSORReadAcc(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SENSOR,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SENSOR_GSENSORReadID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SENSOR,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SENSOR_ECompassI2CTest(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SENSOR,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SENSOR_TouchID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SENSOR,4,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SENSOR_LSensorReadID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SENSOR,5,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SENSOR_LSensorReadValue(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SENSOR,6,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SENSOR_PSensorReadID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SENSOR,7,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SENSOR_PSensorReadProximity(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SENSOR,8,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);	
}

NEPDLLAPI int __stdcall NVDA_SENSOR_GyroReadID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SENSOR,9,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SENSOR_LSensorCalibration(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SENSOR,10,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SENSOR_PSensorFarCalibration(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SENSOR,11,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SENSOR_PSensorNearCalibration(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SENSOR,12,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SENSOR_GSensorCalibration(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SENSOR,13,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SENSOR_FcheckCalibration(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SENSOR,14,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}
// 5-AUDIO
NEPDLLAPI int __stdcall NVDA_AUDIO_Headset(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_AUDIO,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_Speaker(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_AUDIO,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_JackCheck(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_AUDIO,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_HookCheck(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_AUDIO,4,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_LineOut(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_AUDIO,5,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_PlayMusic(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_AUDIO,6,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_PlayTone(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_AUDIO,7,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_RecordWav(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_AUDIO,8,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_LoopTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_AUDIO,9,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_SwitchRX(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_AUDIO,10,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_SwitchTX(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_AUDIO,11,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_Playwav(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_AUDIO,12,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}
NEPDLLAPI int __stdcall NVDA_AUDIO_ReadRCID(int nPort, int nTimeout, char* OutBuffer)
{// cmd: 13
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_AUDIO,13,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}
// 6-GPS
NEPDLLAPI int __stdcall NVDA_GPS_EnterSARFMode(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_GPS,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);;
}

NEPDLLAPI int __stdcall NVDA_GPS_CWTest(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_GPS,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);;
}

NEPDLLAPI int __stdcall NVDA_GPS_FreqStability(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_GPS,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);;
}

NEPDLLAPI int __stdcall NVDA_GPS_ExitSARFMode(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_GPS,4,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);;
}

// 7-BT
NEPDLLAPI int __stdcall NVDA_BT_TxPowerOn(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BT,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BT_TxPowerOnChannel(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BT,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BT_TxPowerOff(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BT,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BT_TestModeOn(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BT,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BT_TestModeOff(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BT,4,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BT_ReadBTID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BT,5,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

// 8-FMR
NEPDLLAPI int __stdcall NVDA_FMR_ReadRSSI(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FMR,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FMR_PathTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FMR,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

// 9-SIM
NEPDLLAPI int __stdcall NVDA_SIM_CheckStatus(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SIM,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SIM_DetectHotPlug(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_SIM,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

// 10-CLK

// 11-USB

// 12-TOUCH
NEPDLLAPI int __stdcall NVDA_TOUCH_Test(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_TOUCH,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_TOUCH_DownloadFirmware(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_TOUCH,4,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}
NEPDLLAPI int __stdcall NVDA_TOUCH_Self_Test(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_TOUCH,5,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}
NEPDLLAPI int __stdcall NVDA_TOUCH_Cal_Test(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_TOUCH,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}
NEPDLLAPI int __stdcall NVDA_TOUCH_ReadFlag(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_TOUCH,6,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}
NEPDLLAPI int __stdcall NVDA_TOUCH_WriteFlag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_TOUCH,7,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 1, OutBuffer, " []", __FUNCTION__);
}

// 13-KEYPAD
NEPDLLAPI int __stdcall NVDA_KEYPAD_Detect(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_KEYPAD,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_KEYPAD_GetCapkeyChipID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_KEYPAD,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_KEYPAD_GetCapkeyImpendanceValue(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_KEYPAD,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

// 14-VIBRATOR
NEPDLLAPI int __stdcall NVDA_VIBRATOR_Switch(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_VIBRATOR,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_VIBRATOR_RunningTime(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_VIBRATOR,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}


// 15-BATTERY
NEPDLLAPI int __stdcall NVDA_BATTERY_BatteryCharge(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BATTERY,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BATTERY_BatteryCurrent(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BATTERY,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BATTERY_BatteryTemperature(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BATTERY,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BATTERY_BatteryVoltage(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BATTERY,4,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BATTERY_BatteryCapacity(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BATTERY,5,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BATTERY_GaugeDFICalibration(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BATTERY,6,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BATTERY_ReadGaugeID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BATTERY,7,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BATTERY_ReadChargerID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BATTERY,8,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BATTERY_CoinCellCharge(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BATTERY,9,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BATTERY_CoinCellVoltage(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BATTERY,10,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BATTERY_DFIITEnable(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BATTERY,11,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BATTERY_DFIVersion(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BATTERY,13,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BATTERY_DFIITEnableStatus(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_BATTERY,14,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

// 16-DOWNLOAD

// 17-OS

// 18-WLAN
NEPDLLAPI int __stdcall NVDA_WLAN_EnableTestMode(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WLAN,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_EnableNormalMode(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WLAN,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_Disable(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WLAN,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_TxPowerSwitch(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WLAN,4,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_ScanAP(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WLAN,5,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_GetScanResult(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WLAN,6,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_ConnectAP(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WLAN,7,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_GetRSSI(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WLAN,8,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_EnableRx(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WLAN,10,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_GetRxResult(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WLAN,11,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_LoadBin(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WLAN,12,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_SaveBin(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_WLAN,13,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

// 19-LED
NEPDLLAPI int __stdcall NVDA_LED_BacklightLEDTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_LED,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LED_LEDRGBTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_LED,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LED_CapKeyLEDTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_LED,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LED_FlashLEDTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_LED,4,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LED_ReadFlashID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_LED,5,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

// 20-ID
NEPDLLAPI int __stdcall NVDA_ID_ReadHWID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_ID,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

// 21-Regulator
NEPDLLAPI int __stdcall NVDA_REGULATOR_ReadREGValue(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_REGULATOR,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

// 22-RF by jackiesen.wan
NEPDLLAPI int __stdcall NVDA_WIRELESS_GSMTxPower(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_RF,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WIRELESS_WCDMATxPower(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_RF,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WIRELESS_TXPowerOFF(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_RF,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

// 23-WWAN

//24-TV_MODULE
NEPDLLAPI int __stdcall NVDA_TV_ModuleDetect(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_TV,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_TV_InBandPower(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_TV,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}


// 25-VERSION
NEPDLLAPI int __stdcall NVDA_VERSION_ReadSWVersion(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_VERSION,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_VERSION_ReadDDRInfo(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_VERSION,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_VERSION_ReadeMMCInfo(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_VERSION,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_VERSION_ReadSDInfo(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_VERSION,4,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_VERSION_ReadTouchInfo(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_VERSION,5,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_VERSION_ReadCameraInfo(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_VERSION,6,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_VERSION_ReadBTVersion(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_VERSION,7,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_VERSION_ReadWIFIVersion(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_VERSION,8,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_VERSION_ReadLCDInfo(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_VERSION,9,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_VERSION_ReadGyroFWVersion(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_VERSION,10,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_VERSION_ReadRFVersion(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_VERSION,11,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_VERSION_ReadBBVersion(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_VERSION,12,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_VERSION_ReadPMVersion(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_VERSION,13,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

// 26-FACTORY
/* Alex.Chen Modify Start 2011/05/30 */
NEPDLLAPI int __stdcall NVDA_FACTORY_FARead(int nPort, int nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, i_InLength, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_FAWrite(int nPort, int nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, i_InLength, OutBuffer, " []", __FUNCTION__);
}
/* Alex.Chen Modify End 2011/05/30 */

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadPicasso(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
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
	int nRetCode = DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
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
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteWifiMac(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,5,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " [F226]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadBtMac(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,6,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteBtMac(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,7,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " [F226]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadPPID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,8,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WritePPID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,9,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " [F226]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadServiceTag(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,10,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteServiceTag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,11,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " [F226]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteDLFlag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char flag[4] = {0};
	SetFaFlag(InBuffer, 1, flag);

	char command[4] = {NV_DT_FA,14,0,0};
	return DtCmdNvda(nPort, nTimeout, command, flag, sizeof(flag), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteFTFlag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char flag[4] = {0};
	SetFaFlag(InBuffer, 2, flag);
	char command[4] = {NV_DT_FA,15,0,0};
	return DtCmdNvda(nPort, nTimeout, command, flag, strlen(flag), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteMMIBBFlag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char flag[4] = {0};
	SetFaFlag(InBuffer, 3, flag);
	char command[4] = {NV_DT_FA,16,0,0};
	return DtCmdNvda(nPort, nTimeout, command, flag, strlen(flag), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteMMIRFFlag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char flag[4] = {0};
	SetFaFlag(InBuffer, 4, flag);
	char command[4] = {NV_DT_FA,17,0,0};
	return DtCmdNvda(nPort, nTimeout, command, flag, strlen(flag), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_CheckAndMoveFA(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char command[4] = {NV_DT_FA,19,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
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

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadIMEI(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,28,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteIMEI(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,29,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ResetPhone(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,30,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteFCFlag(int nPort, int nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,31,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, i_InLength, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_SetBootMode(int nPort, int nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,34,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, i_InLength, OutBuffer, " []", __FUNCTION__);
}

//2011/09/19 add by jackiesen:Set NV EFS_Sync
NEPDLLAPI int __stdcall NVDA_FACTORY_EFS_Sync(int nPort,int nTimeout,char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char command[4] = {NV_DT_FA,35,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadLCSN(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,36,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteLCSN(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,37,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteOTP(int nPort,int nTimeout,char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,39,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_CheckOTP(int nPort,int nTimeout,char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,40,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadMEID(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,41,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteMEID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,42,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 7, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadPhoneTime(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,43,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WritePhoneTime(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,44,0,0};	
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, 24, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_UnlockPartition(int nPort,int nTimeout,char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,45,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ClearProductionTime(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,46,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ClearItemOnNORMALPartition(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,47,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_TriggerNVrestore(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,48,0,0};
	return DtCmdNvda(nPort, 2000, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadSN(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,49,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteSN(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,50,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ResetPhone_Dubbai(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,51,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_UnlockSPC_ForSPU(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,52,0,0};	
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ProgramSPC_ForSPU(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,53,0,0};	
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_IsSPCLocked_ForSPU (int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,54,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_FileOperation(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,55,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadIMEI_Second(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,56,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteIMEI_Second(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_FA,57,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

// 27-HouseKeeping
NEPDLLAPI int __stdcall NVDA_HOUSEKEEP_PowerGlueLogicCheck(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char command[4] = {NV_DT_HOUSEKEEP,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

// 28-EFS

// 29-NV
NEPDLLAPI int __stdcall NVDA_NV_ReadNV(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_NV,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_NV_WriteNV(int nPort, int nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_NV,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, i_InLength, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_NV_WriteInitNV(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_NV,6,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_NV_BackupNV(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_NV,3,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_NV_CheckBackupNV(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_NV,5,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}
// 30-DIAG
NEPDLLAPI int __stdcall NVDA_DIAG_SetRequest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_DIAG,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}
NEPDLLAPI int __stdcall NVDA_RTC_Read(int nPort, int nTimeout,char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_RTC,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}
NEPDLLAPI int __stdcall NVDA_RTC_Write(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_RTC,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}
// 32-DRM
NEPDLLAPI int __stdcall NVDA_DRM_Write(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_DRM,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}
NEPDLLAPI int __stdcall NVDA_DRM_Read(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_DRM,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}
//33 Efuse
NEPDLLAPI int __stdcall NVDA_EFUSE_Read(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_EFUSE,1,0,0};
	return DtCmdNvda(nPort, nTimeout, command, NULL, 0, OutBuffer, " []", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_EFUSE_Write(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();
	char command[4] = {NV_DT_EFUSE,2,0,0};
	return DtCmdNvda(nPort, nTimeout, command, InBuffer, strlen(InBuffer), OutBuffer, " []", __FUNCTION__);
}

//====================================================================


NEPDLLAPI bool ExecFastbootOut(CString Command,char *OutMsg, char* ErrorCode, int i_timeout)
{
	bool isOk = false;
	DWORD nPipeSize = 1024 * 1024; //1M pipeline

	bool b_NeedResponse = true;


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

	//Command = _T("cmd.exe /c ") + Command;

	/* 110913 Gilpin.Xi Add End */
	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo)) 
	{
		Command.ReleaseBuffer();
		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, i_timeout);

		//if timeout then exit the process
		if(TimeOutSignal == WAIT_TIMEOUT) 
		{
			isOk = false;
			TerminateProcess(processInfo.hProcess, 0);
			strcpy(ErrorCode, "ERROR: fastboot timeout");
		}
		else 
		{
			isOk = true;

			if (b_NeedResponse)
			{
				DWORD bytesRead;
				char *message = new char[nPipeSize];
				memset(message, 0, sizeof(message));
				::ReadFile(hRead, message, nPipeSize, &bytesRead, NULL);
				message[bytesRead] = '\0';
				strcpy(OutMsg, message);
				delete [] message;
			}
		}

	}
	else 
	{
		isOk = false;
		strcpy(ErrorCode, "ERROR: Execute fastboot.exe fail!");
	}

	CloseHandle(hRead);
	CloseHandle(hWrite);
	CloseHandle(hProcess);
	hProcess = NULL;

	return isOk;
}

//========================================================================
