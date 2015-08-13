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

static int c7cmd(int port, int timeout_ms, byte ftd_cmd_main, byte ftd_cmd_sub,
					 char* in_buf, char* out_buf, const char* func_name)
{
	byte ftd_cmd[4] = {ftd_cmd_main, ftd_cmd_sub, 0, 0};
	int ret_code = -1*ERROR_FUNCTION_FAILED;
	for (int nTry = 0; nTry < CMD_MAX_RETRY; nTry++) {
		TRACE(_T("GeneralCmdInOut\n"));
		ret_code = GeneralCmdInOut(port, timeout_ms, ftd_cmd, in_buf, strlen(in_buf), out_buf);
		TRACE(_T("%s(%d)\n"), func_name, ret_code);
		if (ret_code == HS_OK) {
			break;
		}
		Sleep(500);
	}
	return ret_code;
}

NEPDLLAPI bool C7_ExecAdbOut_SPU(char* Command, char* output, char* error_code)
{
	TRACER_FUNCTION_IN();
	return C7_ExecAdbOut(Command, output, error_code);
}

NEPDLLAPI bool __stdcall C7_ExecAdbOut(CString command, char* output, char* error_msg)
{
	return ExecAdbOut(command, output, error_msg);
}

NEPDLLAPI int __stdcall C7_TestFtdMode(int port, int timeout_ms, char* error_msg)
{
	TRACER_FUNCTION_IN();
	return TestFtdMode(port, timeout_ms, error_msg);
}

NEPDLLAPI int C7_TestFtdMode_SPU(char* Port, char* Timeout, char* error_msg)
{
	TRACER_FUNCTION_IN();
	return C7_TestFtdMode(atoi(Port), atoi(Timeout), error_msg);
}

NEPDLLAPI int __stdcall C7_AutoFtdMode(int port, int timeout_ms, char* error_msg)
{
	TRACER_FUNCTION_IN();
	return AutoFtdMode(port, timeout_ms, error_msg);
}

NEPDLLAPI int C7_AutoFtdMode_SPU(char* Port, char* Timeout, char* error_msg)
{
	TRACER_FUNCTION_IN();
	return C7_AutoFtdMode(atoi(Port), atoi(Timeout), error_msg);
}

// 01-Battery
NEPDLLAPI int __stdcall C7_BATTcoincellVoltage(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_BATT, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_BATTcoincellChargeOn(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_BATT, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_BATTrtcTime(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_BATT, 3, in_buf, out_buf, __FUNCTION__);
}

// 02-Power
NEPDLLAPI int __stdcall C7_PWRgaugeID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();

	return c7cmd(port, timeout_ms, C7_FT_PWR, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_PWRgaugeVoltage(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_PWR, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_PWRgaugeCurrent(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_PWR, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_PWRgaugeTemperature(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_PWR, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_PWRchargerID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_PWR, 5, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_PWRchargerOn(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_PWR, 6, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_DFIupgrade(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_PWR, 7, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int C7_DFIenable_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();

	int ret_code = InitDevice(atoi(Port), atoi(Timeout));
	if (ret_code == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = c7cmd(atoi(Port), atoi(Timeout), C7_FT_PWR, 8, in_buf, buffer, __FUNCTION__);
		FreeDevice(2000, atoi(Timeout));
		sprintf(out_buf, "%d", ret_code);
	}
	return ret_code;
}

//03:Memory
NEPDLLAPI int __stdcall C7_MCPflashID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_MCP, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_MCPddrID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_MCP, 2, in_buf, out_buf, __FUNCTION__);
}

//04:LCM - LCD & Touch
NEPDLLAPI int __stdcall C7_TOUCHpoint(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_LCM, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_TOUCHtest(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_LCM, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_TOUCHid(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_LCM, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_TOUCHversion(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_LCM, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_LCDonoff(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_LCM, 5, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_TOUCHSelfTest(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_LCM,6, in_buf, out_buf, __FUNCTION__);
}

//06:audio
NEPDLLAPI int __stdcall C7_AUoutSwitch(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_AU, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_AUplaymp3(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_AU, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_AUrecordWAV(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_AU, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_AUplayWAV(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_AU, 4, in_buf, out_buf, __FUNCTION__);
}

//06:camera
NEPDLLAPI int __stdcall C7_CameraID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_CAM, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_CAMshot(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_CAM, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_CAMstart(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_CAM, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_CAMindexReset(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_CAM, 4, in_buf, out_buf, __FUNCTION__);
}

//07:keypad
NEPDLLAPI int __stdcall C7_KPtouchTest(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_KEY, 1, in_buf, out_buf, __FUNCTION__);
}

//13:BT
NEPDLLAPI int __stdcall C7_BTid(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();

	char command[4] = {C7_FT_BT,1,0,0};
	return c7cmd(port, timeout_ms, C7_FT_BT, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_BTTestMode(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_BT, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_BTTXPower(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_BT, 3, in_buf, out_buf, __FUNCTION__);
}

// 14-WLAN
NEPDLLAPI int __stdcall C7_WLAN_Mode(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_WLAN, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_WLAN_TXPower(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_WLAN, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_WLAN_RXrssi(int port, int timeout_ms, char* in_buf, char* out_buf) // cmd: 3
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_WLAN, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_WLAN_RXset(int port, int timeout_ms, char* in_buf, char* out_buf) // cmd: 4
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_WLAN, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_WLAN_LoadFABin(int port, int timeout_ms, char* in_buf, char* out_buf) // cmd: 5
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_WLAN, 5, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_WLAN_LoadNextBin(int port, int timeout_ms, char* in_buf, char* out_buf) // cmd: 6
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_WLAN, 6, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_WLAN_SaveBin(int port, int timeout_ms, char* in_buf, char* out_buf) // cmd: 7
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_WLAN, 7, in_buf, out_buf, __FUNCTION__);
}

// 15-GPS
NEPDLLAPI int __stdcall C7_GPSValueCN0(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_GPS,1, in_buf, out_buf, __FUNCTION__);
}

// 16-FMR
NEPDLLAPI int __stdcall C7_FMRrssi(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_FMR, 1, in_buf, out_buf, __FUNCTION__);
}

// 17-NFC
NEPDLLAPI int __stdcall C7_NFCid(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_NFC, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_NFCselftest(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_NFC, 2, in_buf, out_buf, __FUNCTION__);
}

// 18-Sensor
NEPDLLAPI int __stdcall C7_SNSRgsensorID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_SNSR, 1, in_buf, out_buf, __FUNCTION__);
}
NEPDLLAPI int __stdcall C7_SNSRgsensorAcc(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_SNSR, 2, in_buf, out_buf, __FUNCTION__);
}
NEPDLLAPI int __stdcall C7_SNSRgsensorCalib(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_SNSR, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_SNSRecompassID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_SNSR, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_SNSRecompassSelftest(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_SNSR, 30, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_SNSRlsensorID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_SNSR, 7, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_SNSRlsensorValue(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_SNSR, 8, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_SNSRlsensorCalib(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_SNSR, 9, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_SNSRpsensorID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_SNSR, 10, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_SNSRpsensorValue(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_SNSR, 11, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_SNSRpsensorCalib(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_SNSR, 12, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_SNSRloadFALPkdata(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_SNSR, 99, in_buf, out_buf, __FUNCTION__);
}

// 20-LED
NEPDLLAPI int __stdcall C7_LEDid(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_LED, 1, in_buf, out_buf, __FUNCTION__);
}

// 21-ID
NEPDLLAPI int __stdcall C7_IDcpu(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_ID, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_BoardADC(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_ID, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_BoardRev(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_ID, 3, in_buf, out_buf, __FUNCTION__);
}

//90:OS
NEPDLLAPI int __stdcall C7_OSgetprop(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_OS, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_OSpwr2home(int port, int timeout_ms, char* in_buf, char* out_buf)//cmd:2
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_OS, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_OSkeyBack(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_OS, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_OSkeyFocus(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_OS, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_OSkeyCamera(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_OS, 5, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_OSmodeSleep(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_OS, 6, in_buf, out_buf, __FUNCTION__);
}

//91:version
NEPDLLAPI int __stdcall C7_VERftd(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_VER, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_VERchange(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	char* prop_name = "ro.build.version.incremental";
	return C7_OSgetprop(port, timeout_ms, prop_name, out_buf);
}

//92:factory
NEPDLLAPI int __stdcall C7_FAC_FARead(int port, int timeout_ms, char* in_buf, char* out_buf) //cmd:1
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_FAC, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int C7_FAC_FARead_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code) //cmd:1
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = C7_FAC_FARead(i_port, i_timeout, in_buf, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI int __stdcall C7_FAC_FAWrite(int port, int timeout_ms, char* in_buf, char* out_buf) //cmd:2
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_FAC, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_FAC_Picasso(int port, int timeout_ms, char* in_buf, char* out_buf) //cmd:3
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_FAC, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_FAC_WifiDataPath(int port, int timeout_ms, char* in_buf, char* out_buf) //cmd:3
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_FAC, 5, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_FAC_FactoryReset(int port, int timeout_ms, char* in_buf, char* out_buf) //cmd:3
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_FAC, 6, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall C7_FAC_FARead301(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	char* fa_301_pos = "512,48,2012";
	return C7_FAC_FARead(port, timeout_ms, fa_301_pos, out_buf);
}

NEPDLLAPI int __stdcall C7_FAC_FARead303(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	char* fa_303_pos = "1152,48,2012";
	return C7_FAC_FARead(port, timeout_ms, fa_303_pos, out_buf);
}

NEPDLLAPI int __stdcall C7_FAC_FARead304(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	char* fa_304_pos = "560,12,2012";
	return C7_FAC_FARead(port, timeout_ms, fa_304_pos, out_buf);
}

NEPDLLAPI int C7_FAC_FactoryReset_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code) //cmd:3
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = C7_FAC_FactoryReset(i_port, i_timeout, in_buf, buffer);
		FreeDevice(i_timeout, i_timeout);
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI int __stdcall C7_FAC_ClearNFCVariable(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return c7cmd(port, timeout_ms, C7_FT_FAC, 7, in_buf, out_buf, __FUNCTION__);
}
