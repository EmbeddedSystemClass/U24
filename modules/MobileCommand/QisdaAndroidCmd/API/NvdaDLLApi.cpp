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

static const int NV_DT_CMD_MAX_RETRY = 2;

static int DtCmdNvda(int port, int timeout_ms, byte* ftd_cmd,
					 char* in_buf, size_t in_size, char* out_buf,
					 const char* error_code, const char* func_name)
{
	int ret_code = -1*ERROR_FUNCTION_FAILED;
	for (int nTry = 0; nTry < NV_DT_CMD_MAX_RETRY; nTry++) {
		if (in_buf != NULL && out_buf != NULL) {
			TRACE(_T("DtCmdNvda GeneralCmdInOut "));
			ret_code = GeneralCmdInOut(port, timeout_ms, ftd_cmd, in_buf, in_size, out_buf);
			if (ret_code != HS_OK) {
				strncat(out_buf, error_code, strlen(error_code));
			}
		}
		else if (in_buf != NULL) {
			TRACE(_T("DtCmdNvda GeneralCmdIn "));
			size_t nInLength = (in_size == 0) ? strlen(in_buf) : in_size;
			ret_code = GeneralCmdIn(port, timeout_ms, ftd_cmd, in_buf, nInLength);
		}
		else if (out_buf != NULL) {
			TRACE(_T("DtCmdNvda GeneralCmdOut "));
			ret_code = GeneralCmdOut(port, timeout_ms, ftd_cmd, out_buf);
			if (error_code != NULL && ret_code != HS_OK) {
				strncat(out_buf, error_code, strlen(error_code));
			}
		}
		else {
			TRACE(_T("DtCmdNvda GeneralCmd "));
			ret_code = GeneralCmd(port, timeout_ms, ftd_cmd);
			if (error_code != NULL && ret_code != HS_OK) {
				strncat(out_buf, error_code, strlen(error_code));
			}
		}
		TRACE(_T("%s(%d)\n"), func_name, ret_code);
		if (ret_code == HS_OK) {
			break;
		}
		Sleep(500);
	}
	return ret_code;
}

void SetFaFlag(char* in_buf, const int& nStation, char* pflag)
{
	if (strlen(in_buf) == 0) {
		memset(pflag, 0, 4);
	}
	else {
		memcpy(pflag, in_buf, 4);
	}
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

static bool IsADBLive()
{
	DWORD aProcesses[1024], cbNeeded, cbMNeeded;
	HMODULE hMods[1024];
	char szProcessName[MAX_PATH];
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
		return FALSE;
	}
	for (int i = 0; i < (int)(cbNeeded / sizeof(DWORD)); i++) {
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, aProcesses[i]);
		EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbMNeeded);
		GetModuleFileNameEx(hProcess, hMods[0], szProcessName, sizeof(szProcessName));
		if (strstr(_strupr(szProcessName), "ADB.EXE")) {
			TerminateProcess(hProcess, 0);
		}
		CloseHandle(hProcess);
		hProcess = NULL;
	}
	return true;
}

CString GetFaInfoByFtd(byte* fa_info, int start, const int length)
{
	CString cstrFAinfo;
	const int BUF_SIZE = 1024;
	unsigned char fa_data[BUF_SIZE] = {0};
	memcpy(fa_data, &fa_info[start], length);
	char* hex_str = new char[length * 2 + 1];
	memset(hex_str, 0, sizeof(hex_str));
	if (start == FA_MOBILE_ID_START) {
		for (int i = 0; i < length; i++) {
			sprintf(&hex_str[i * 2], "%02x", fa_data[i]);
		}
		cstrFAinfo = hex_str;
		delete hex_str;
	}
	else {
		cstrFAinfo = fa_data;
	}
	return cstrFAinfo;
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
	const int PORT_TCP = 2000;
	int timeout_ms = 10 * CLOCKS_PER_SEC;
	unsigned char FaInfo[BUF_SIZE] = {0};
	if (NVDA_FACTORY_FARead(PORT_TCP, timeout_ms, (char*)&FaInfo) != HS_OK) {
		return false;
	}
	const int LENGTH_MOBILE = 11;
	const int LENGTH_IMEI = 15;
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
	TRACER_FUNCTION_IN();

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
	int ret_code = HS_OK;
	if (!pFnUpdateMobileIDByPicasso(szModelName, szPicasso, szMobileId, ostrErrCode, ostrErrMsg)) {
		char error_string[512] = {0};
		strcat(error_string, "[");
		strncat(error_string, ostrErrCode, strlen(ostrErrCode));
		strcat(error_string, "] ");
		strncat(error_string, ostrErrMsg, strlen(ostrErrMsg));
		ret_code = -1*ERROR_INVALID_DATA;
		CString err_msg;
		err_msg.Format(_T("Fail to update Mobile ID: %s."), error_string);
		::AfxMessageBox(err_msg);
	}
	if (hmLib) {
		::FreeLibrary(hmLib);
		hmLib = NULL;
	}
	return HS_OK;
}

NEPDLLAPI void __stdcall NVDA_SetModelName(CString sModelName)
{
	m_ModelName = sModelName;
}

NEPDLLAPI int __stdcall NVDA_QueryMobileIDByPicasso(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();

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

	int ret_code = HS_OK;
	const int PAYLOAD_SIZE = 4096;
	char szModelName[32] = {0};
	char szPicasso[PAYLOAD_SIZE] = {0};
	char ostrMobileId[32];
	char ostrErrCode[1024];
	char ostrErrMsg[1024];
	if (HS_OK != NVDA_FACTORY_ReadPicasso(port, timeout_ms, szPicasso)) {
		CString message;
		message = _T("Read picasso Fail!");
		AfxMessageBox(message);
		return -1*ERROR_FUNCTION_FAILED;
	}
	CString sModelName = m_ModelName;
	strncpy(szModelName, sModelName, sModelName.GetLength());
	if ( pFnQueryMobileIDByPicasso(szModelName, szPicasso, ostrMobileId, ostrErrCode, ostrErrMsg) ) {
		unsigned char fa_data[4096];
		if (HS_OK != NVDA_FACTORY_FARead(port, timeout_ms, (char*)&fa_data)) {
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
				ret_code = -1*ERROR_BAD_COMMAND;
			}
		}
		else {
			// picasso is used, then check mobile id between device and db
			if (strcmp(mobile_id_device, mobile_id_db) != 0) {
				// picasso reused
				ret_code = -1*ERROR_NOT_SAME_DEVICE;
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
		ret_code = -1*ERROR_INVALID_DATA;
		CString err_msg;
		err_msg.Format(_T("Fail to query Mobile ID: %s."), error_string);
		::AfxMessageBox(err_msg);
	}
	if (hmLib) {
		::FreeLibrary(hmLib);
		hmLib = NULL;
	}
	return ret_code;
}

NEPDLLAPI int __stdcall NVDA_KillAdb(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();

	if (IsADBLive()) {
		return HS_OK;
	}
	return HS_FAIL;
}

NEPDLLAPI int __stdcall NVDA_ClearTLineFlag(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();

	int ret_code = -1*ERROR_FUNCTION_FAILED;
	const int PORT_TCP = 2000;
	const int BUF_SIZE = 4096;
	char* pBuf = new char[BUF_SIZE];
	if (WriteTLineTroubleShootingPics()) {
		memset(pBuf, 0, sizeof(pBuf));
		ret_code = NVDA_FACTORY_ClearFA(PORT_TCP, timeout_ms, pBuf);
		if (ret_code != HS_OK) {
			strcpy(ErrorCode, "ERROR: Clear FA fail");
			ret_code = -1*ERROR_BAD_COMMAND;
		}
	}
	else {
		strcpy(ErrorCode, "ERROR: Write pics fail");
		ret_code = -1*ERROR_BAD_COMMAND;
	}
	return ret_code;
}

NEPDLLAPI int __stdcall NVDA_UpdateImeiPackState_R(int port, int timeout_ms, char* ErrorCode)
{
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
	NVDA_WWAN_ReadIMEI(port, timeout_ms, szIMEI);

	int ret_code = NO_ERROR;
	char ostrErrCode[1024];
	char ostrErrMsg[1024];
	if ( UpdateImeiState_R(szIMEI, ostrErrCode, ostrErrMsg) ) {
		ret_code = NO_ERROR;
	}
	else {
		char error_string[512] = {0};
		strcat(error_string, "[");
		strncat(error_string, ostrErrCode, strlen(ostrErrCode));
		strcat(error_string, "] ");
		strncat(error_string, ostrErrMsg, strlen(ostrErrMsg));
		ret_code = -1*ERROR_INVALID_DATA;
		CString err_msg;
		err_msg.Format(_T("Fail: %s."), error_string);
		::AfxMessageBox(err_msg);
	}
	if (hmLib) {
		::FreeLibrary(hmLib);
		hmLib = NULL;
	}
	return ret_code;
}

NEPDLLAPI int __stdcall NVDA_TestFtdMode(int port, int timeout_ms, char* error_msg)
{
	TRACER_FUNCTION_IN();
	return TestFtdMode(port, timeout_ms, error_msg);
}

NEPDLLAPI int __stdcall NVDA_AutoFtdMode(int port, int timeout_ms, char* error_msg)
{
	TRACER_FUNCTION_IN();
	IsADBLive();
	return AutoFtdMode(port, timeout_ms, error_msg);
}

NEPDLLAPI int __stdcall NVDA_CheckStationFlag(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();

	int ret_code = -1*ERROR_FUNCTION_FAILED;
	unsigned char FaInfo[4096];
	memset(FaInfo, 0, 4096);
	ret_code = NVDA_FACTORY_FARead(port, timeout_ms, (char*)&FaInfo);
	if (ret_code != HS_OK) {
		strcpy(out_buf, "ERROR: Read FA fail");
	}
	else {
		const int LENGTH_STATION_FLAG  = 12;
		unsigned char FaInfoOut[12] = {0};
		memcpy(FaInfoOut, &FaInfo[FA_P01_DL_START], LENGTH_STATION_FLAG);
		CString mode = in_buf;
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
		//strcpy(out_buf, StationFlags);
		for (int nStation = 0; nStation < nCheckStation; nStation++) {
			flag = StationFlags.Mid(nStation*8, 8);
			switch (nStation) {
				case 0: // pre-DL check flags not 0
					if ( !CheckFlagStation(flag) ) {
						strcpy(out_buf, "ERROR: Check preDL flag fail");
						return HS_FAIL;
					}
					break;
				case 1: // FT checks 0x32323232
					if (flag.Find(_T("32323232")) == -1) {
						strcpy(out_buf, "ERROR: Check FT flag fail");
						return HS_FAIL;
					}
					break;
				case 2: // MMIBB checks 0x1c0a0a04
					if (flag.Find(_T("1c0a0a04")) == -1) {
						strcpy(out_buf, "ERROR: Check MMIBB flag fail");
						return HS_FAIL;
					}
					break;
				case 3: // MMIRF checks 0x31323334
					if (flag.Find(_T("31323334")) == -1) {
						strcpy(out_buf, "ERROR: Check MMIRF flag fail");
						return HS_FAIL;
					}
					break;
				case 4: // IMEI
					if ( !CheckFlagStation(flag) ) {
						strcpy(out_buf, "ERROR: Check IMEI flag fail");
						return HS_FAIL;
					}
					break;
				case 5: // re-DL
					if ( !CheckFlagStation(flag) ) {
						strcpy(out_buf, "ERROR: Check reDL flag fail");
						return HS_FAIL;
					}
					break;
				default:
					break;
			}
		}
	}
	return ret_code;
}

NEPDLLAPI int __stdcall NVDA_PwrOn2UartPktMode(int port, int timeout_ms, char* ErrorCode)
{
	TRACER_FUNCTION_IN();
	char* error_code = " [FT7001]";
	strncat(ErrorCode, error_code, strlen(error_code));
	return PowerOnToUARTPacketMode(port, timeout_ms, false);
}

// 2-CAMERA

// 3-LCD
// L1-	LCD no display
// L4-  LCD no back light
// L4-C Can not switch on/off in backlight test
// L41- LCD back light has some color NG
NEPDLLAPI int __stdcall NVDA_LCD_DisplayRGB(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_LCD,1,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7003]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LCD_Backlight_Test(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_LCD,2,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7003]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LCD_Backlight_On(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_LCD,3,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7003]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LCD_Backlight_Off(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_LCD,4,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7003]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LCD_Touch_Test(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_LCD,5,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT7003]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LCD_Touch_SelfTest(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_LCD,6,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT5619]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LCD_Touch_VddTest(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_LCD,7,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT5619]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LCD_Touch_PinFaultTest(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_LCD,8,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT5619]", __FUNCTION__);
}

// 4-SENSOR
// F380 Ambient Light Sensor
// F381 G-sensor
// F382 E-compass
// F383 Proximity
// F384 Cap key
NEPDLLAPI int __stdcall NVDA_SENSOR_GSENSORReadAcc(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_SENSOR,1,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SENSOR_GSENSORReadID(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_SENSOR,2,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SENSOR_ECompassI2CTest(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_SENSOR,3,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7004]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_SENSOR_TouchID(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_SENSOR,4,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7004]", __FUNCTION__);
}

// 5-AUDIO
NEPDLLAPI int __stdcall NVDA_AUDIO_Mic(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_AUDIO,1,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7005]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_Headset(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_AUDIO,2,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7005]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_Speaker(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_AUDIO,3,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7005]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_I2CTest(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_AUDIO,4,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7005]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_JackCheck(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_AUDIO,5,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7005]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_HookCheck(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_AUDIO,6,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7005]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_LineOut(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_AUDIO,7,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7005]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_AUDIO_PlayMusic(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_AUDIO,8,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7005]", __FUNCTION__);
}

// 6-MODEM
// F390 GSM Tx Max Power
// F391 SIM test
// F392 GPS test
NEPDLLAPI int __stdcall NVDA_MODEM_GPSCWTest(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_MODEM,1,0,0};
#ifdef _DEBUG
    strcpy(out_buf, "-130");
    return HS_OK;
#else
    return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7006]", __FUNCTION__);
#endif
}

NEPDLLAPI int __stdcall NVDA_MODEM_GPSFreqStability(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_MODEM,2,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7006]", __FUNCTION__);
}

// 7-BT
// F393 BT Tx Max Power
NEPDLLAPI int __stdcall NVDA_BT_TxOnAt(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_BT,1,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT7007]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BT_TxOff(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_BT,2,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT7007]", __FUNCTION__);
}

// 11-USB
// 12-CHARGER
// 13-KEYPAD

// 14-VIBRATOR
// F370 Vibrator failed of BB test
NEPDLLAPI int __stdcall NVDA_VIBRATOR_Switch(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_VIBRATOR,1,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT7014]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_VIBRATOR_RunningTime(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_VIBRATOR,2,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7014]", __FUNCTION__);
}

// 15-BATTERY
// F334	Battery failed
// F372 Charging failed of BB test
// F228 USB CHARGE Fail
// E4   Can not charge
// E41  Coin Li-ion battery can't charge
// F396 Backup battery open
// F397 Backup battery short
NEPDLLAPI int __stdcall NVDA_BATTERY_BattChargeOn(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_BATTERY,1,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BATTERY_ChargingRecovery(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_BATTERY,2,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BATTERY_BattTemp(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_BATTERY,3,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BATTERY_PMUBattVoltage(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_BATTERY,4,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BATTERY_PMUBattCapacity(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_BATTERY,5,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BATTERY_GAGBattVoltage(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_BATTERY,6,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7015]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_BATTERY_GAGBattCapacity(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_BATTERY,7,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7015]", __FUNCTION__);
}

// 16-DOWNLOAD
// 17-OS

// 18-WLAN
// F395 WLAN Tx Max
NEPDLLAPI int __stdcall NVDA_WLAN_TxMaxPowerOnCh(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_WLAN,1,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT7018]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_TxMaxPowerOff(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_WLAN,2,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7018]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_LocalFreqOnCh(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_WLAN,3,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT7018]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_LocalFreqOff(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_WLAN,4,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7018]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_TempTxMaxPowerOnCh(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_WLAN,5,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT7018]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_TempTxMaxPowerOff(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_WLAN,6,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7018]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_TempLocalFreqOnCh(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_WLAN,7,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT7018]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WLAN_TempLocalFreqOff(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_WLAN,8,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7018]", __FUNCTION__);
}

// 19-LED
// L3   Keypad LED no good
// L31  Indicator LED no good
// L4-C Can not switch on/off in backlight test
// L3   Keypad LED no good
// L5-  Back light is too dark, LED light not uniform or black mark
// L6-	LED wrong color
// L7-  Flash light no function
NEPDLLAPI int __stdcall NVDA_LED_BacklightOn(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_LED,1,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT7019]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LED_L01On(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_LED,2,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT7019]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LED_L02On(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_LED,3,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT7019]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LED_CapKeyOn(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_LED,4,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT7019]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_LED_FlashOn(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_LED,5,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT7019]", __FUNCTION__);
}

// 20-ID
// F226 HWID version error
NEPDLLAPI int __stdcall NVDA_ID_ReadBoardID(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_ID,1,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7020]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_ID_ReadHWID(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_ID,2,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7020]", __FUNCTION__);
}

// 21-Regulator
NEPDLLAPI int __stdcall NVDA_REGULATOR_LDO0On(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_REGULATOR,1,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT7021]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_REGULATOR_LDO6On(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_REGULATOR,2,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT7021]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_REGULATOR_LDO7On(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_REGULATOR,3,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT7021]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_REGULATOR_LDO8On(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_REGULATOR,4,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT7021]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_REGULATOR_GPIO1On(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_REGULATOR,5,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT7021]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_REGULATOR_GPIO2On(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_REGULATOR,6,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT7021]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_REGULATOR_GPIO3On(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_REGULATOR,7,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT7021]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_REGULATOR_LDORecover(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_REGULATOR,8,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7021]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_REGULATOR_LDOLog(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_REGULATOR,9,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7021]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_REGULATOR_LDOAllOn(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_REGULATOR,10,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7021]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_REGULATOR_LDO1V2(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_REGULATOR,11,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT7021]", __FUNCTION__);
}

// 22-WWAN
// F387	3G card
NEPDLLAPI int __stdcall NVDA_WWAN_3GCardDetect(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_WWAN,1,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7022]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WWAN_ReadIMEI(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_WWAN,2,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7022]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WWAN_ReadRSSI(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_WWAN,3,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, strlen(in_buf), out_buf, " [FT7022]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WWAN_Read3GModuleVersion(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_WWAN,4,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7022]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WWAN_CheckSimLock(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_WWAN,5,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7022]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WWAN_CheckNetworkLock(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_WWAN,6,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7022]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WWAN_Read3GModuleType(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_WWAN,7,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7022]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WWAN_IMEIBackUp(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_WWAN,8,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7022]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_WWAN_ReadFAIMEIBackUp(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_WWAN,14,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7022]", __FUNCTION__);
}

// 23-RD

//24-TV_MODULE
NEPDLLAPI int __stdcall NVDA_TV_ModuleDetect(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_TV,1,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7024]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_TV_InBandPower(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_TV,2,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, 0, out_buf, " [FT7024]", __FUNCTION__);
}

// 25-VERSION
// F211 SW version error
// F226 HWID version error
NEPDLLAPI int __stdcall NVDA_VERSION_ReadTestVersion(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_VERSION,1,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7025]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_VERSION_ReadBoardID(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_VERSION,2,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7025]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_VERSION_ReadAccessaryID(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_VERSION,3,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7025]", __FUNCTION__);
}

// 26-FACTORY
// F398 PCBA version error
NEPDLLAPI int __stdcall NVDA_FACTORY_FARead(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_FA,1,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, NULL, __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_FAWrite(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();
	//byte ftd_cmd[4] = {NV_DT_FA,2,0,0};
	//return GeneralCmdIn(port, timeout_ms, ftd_cmd, in_buf, 128);
	return HS_FAIL;
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WritePicasso(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();

	if (strlen(in_buf) != 10) {
		return -1*ERROR_INVALID_INDEX;
	}
	char szPicasso[13] = {0};
	memcpy(szPicasso, in_buf, 10);
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
	byte ftd_cmd[4] = {NV_DT_FA,24,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, szPicasso, 12, out_buf, NULL, __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadPicasso(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_FA,3,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadPicassoCheck(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	CString sDll;
	::GetModuleFileName(NULL, sDll.GetBuffer(MAX_PATH), MAX_PATH);
	sDll.ReleaseBuffer();
	sDll = sDll.Left(sDll.ReverseFind('\\'));
	sDll += _T("\\Pics_SSSO2_Factory_StaticM.dll");
	byte ftd_cmd[4] = {NV_DT_FA,3,0,0};
	int ret_code = DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7026]", __FUNCTION__);
	if (ret_code == HS_OK) {
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
		if ( CheckPicasso(out_buf, ostrErrCode, ostrErrMsg) ) {
			ret_code = HS_OK;
		}
		else { // FT3021
			char *error_code = " [FT3021]";
			strncat(out_buf, error_code, strlen(error_code));
			strcat(out_buf, ":");
			strncat(out_buf, ostrErrCode, strlen(ostrErrCode));
			strcat(out_buf, ":");
			strncat(out_buf, ostrErrMsg, strlen(ostrErrMsg));
			ret_code = HS_FAIL;
		}
		if (hmLib) {
			::FreeLibrary(hmLib);
			hmLib = NULL;
		}
	}
	return ret_code;
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadWifiMac(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_FA,4,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteWifiMac(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_FA,5,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, strlen(in_buf), out_buf, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadBtMac(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_FA,6,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteBtMac(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_FA,7,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, strlen(in_buf), out_buf, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadPPID(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_FA,8,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WritePPID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_FA,9,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, strlen(in_buf), out_buf, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadServiceTag(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_FA,10,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteServiceTag(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_FA,11,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, in_buf, strlen(in_buf), out_buf, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteDLFlag(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	char flag[4] = {0};
	SetFaFlag(in_buf, 1, flag);
	byte ftd_cmd[4] = {NV_DT_FA,14,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, flag, sizeof(flag), out_buf, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteFTFlag(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	char flag[4] = {0};
	SetFaFlag(in_buf, 2, flag);
	byte ftd_cmd[4] = {NV_DT_FA,15,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, flag, strlen(flag), out_buf, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteMMIBBFlag(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	char flag[4] = {0};
	SetFaFlag(in_buf, 3, flag);
	byte ftd_cmd[4] = {NV_DT_FA,16,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, flag, strlen(flag), out_buf, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_WriteMMIRFFlag(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	char flag[4] = {0};
	SetFaFlag(in_buf, 4, flag);
	byte ftd_cmd[4] = {NV_DT_FA,17,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, flag, strlen(flag), out_buf, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_CheckAndMoveFA(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_FA,19,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadRC(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_FA,25,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ClearFA(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_FA,26,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7026]", __FUNCTION__);
}

NEPDLLAPI int __stdcall NVDA_FACTORY_ReadMmiErrorCode(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_FA,29,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7026]", __FUNCTION__);
}

//27-HouseKeeping
//F386	HOUSEKEEP_PowerGlueLogic
NEPDLLAPI int __stdcall NVDA_HOUSEKEEP_PowerGlueLogicCheck(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte ftd_cmd[4] = {NV_DT_HOUSEKEEP,1,0,0};
	return DtCmdNvda(port, timeout_ms, ftd_cmd, NULL, 0, out_buf, " [FT7027]", __FUNCTION__);
}
