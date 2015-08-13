#include "stdafx.h"
#include <cstddef> // size_t

#include "../CmdType.h"
#include "GeneralDLLApi.h" // GeneraCmd
#include "DetroitDLLApi.h"
#include "io.h"
#include "../Utility.h"
#include "../FactoryBlock.h"


static const int CMD_MAX_RETRY = 1;

static int MobileCmd(int port, int szTimeout_ms_ms, byte ftd_cmd_main, byte ftd_cmd_sub,
                     char* in_buf, char* out_buf, const char* func_name)
{
	byte ftd_cmd[4] = {ftd_cmd_main, ftd_cmd_sub, 0, 0};
	int ret_code = -1 * ERROR_FUNCTION_FAILED;
	for (int nTry = 0; nTry < CMD_MAX_RETRY; nTry++)
	{
		ret_code = GeneralCmdInOut(port, szTimeout_ms_ms, ftd_cmd, in_buf, strlen(in_buf), out_buf);
		TRACE(_T("%s(%d)\n"), func_name, ret_code);
		if (ret_code == HS_OK)
		{
			break;
		}
		Sleep(500);
	}
	return ret_code;
}

NEPDLLAPI bool __stdcall Detroit_ExecAdbOut(CString Command, char* output, char* error_msg)
{
	TRACER_FUNCTION_IN();
	return ExecAdbOut(Command, output, error_msg);
}

NEPDLLAPI bool __stdcall Detroit_ExecFastbootOut(CString Command, char* output, char* error_msg)
{
	TRACER_FUNCTION_IN();
	return ExecFastbootOut(Command, output, error_msg);
}

NEPDLLAPI int __stdcall Detroit_TestFtdMode(int port, int szTimeout_ms_ms, char* error_msg)
{
	TRACER_FUNCTION_IN();
	return TestFtdMode(port, szTimeout_ms_ms, error_msg);
}

NEPDLLAPI int __stdcall Detroit_AutoFtdMode(int port, int szTimeout_ms_ms, char* error_msg)
{
	TRACER_FUNCTION_IN();
	return AutoFtdMode(port, szTimeout_ms_ms, error_msg);
}

// 01-Battery
NEPDLLAPI int __stdcall Detroit_BATTcoincellVoltage(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_BATT, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_BATTcoincellChargeOn(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_BATT, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_BATTrtcTime(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_BATT, 3, in_buf, out_buf, __FUNCTION__);
}

// 02-Power
NEPDLLAPI int __stdcall Detroit_PWRgaugeID(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_PWR, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_PWRgaugeVoltage(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_PWR, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_PWRgaugeCurrent(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_PWR, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_PWRgaugeTemperature(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_PWR, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_PWRchargerID(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_PWR, 5, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_PWRchargerOn(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_PWR, 6, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_DFIupgrade(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_PWR, 7, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_ITenable(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_PWR, 8, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_Regualator(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_PWR, 9, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_PWRbattID(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_PWR, 10, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_PWRbattCapacity(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_PWR, 12, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_PWRWriteRsenseData(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_PWR, 13, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_PWRAverageGaugeCurrent(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_PWR, 14, in_buf, out_buf, __FUNCTION__);
}

//03:Memory
NEPDLLAPI int __stdcall Detroit_MCPflashID(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_MCP, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_MCPddrID(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_MCP, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_MCPddrSize(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_MCP, 3, in_buf, out_buf, __FUNCTION__);
}

//04:LCM - LCD & Touch
NEPDLLAPI int __stdcall Detroit_TOUCHpoint(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_LCM, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_TOUCHtest(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_LCM, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_TOUCHid(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_LCM, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_TOUCHversion(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_LCM, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_TOUCHSelfTest(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_LCM, 5, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_LCMbacklight(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_LCM, 6, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_LCMbacklightColor(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_LCM, 7, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_TouchModel(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_LCM, 10, in_buf, out_buf, __FUNCTION__);
}

//05:audio
NEPDLLAPI int __stdcall Detroit_AUpathOut(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_AU, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_AUplaymp3(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_AU, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_AUrecordWAV(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_AU, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_AUplayWAV(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_AU, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_AUdetectHeadset(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_AU, 5, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_AUpathRec(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_AU, 6, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_AUloopbackTest(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_AU, 9, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_AUButtonCheck(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_AU, 10, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_AUinitADC(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_AU, 11, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_AUADCCali(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_AU, 12, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_AUheadsetInfo(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_AU, 13, in_buf, out_buf, __FUNCTION__);
}

//06:camera
NEPDLLAPI int __stdcall Detroit_CAMReadID(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_CAM, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_CAMshot(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_CAM, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_CAMstart(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_CAM, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_CAMend(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_CAM, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_CAMFlashLED(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_CAM, 5, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_CAMFlashLEDID(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_CAM, 6, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_CAM_OTP_AWB(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_CAM, 7, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_CAM_OTP_AFMacro(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_CAM, 8, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_CAM_OTP_AFInfinity(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_CAM, 9, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_CAM_OTP_FrontFuseID(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_CAM, 10, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_CAM_OTP_WriteFrontFuseID(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_CAM, 11, in_buf, out_buf, __FUNCTION__);
}

//7:key
NEPDLLAPI int __stdcall Detroit_KeyTest(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)//cmd:2
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_KEY, 1, in_buf, out_buf, __FUNCTION__);
}

//08:SIM
NEPDLLAPI int __stdcall Detroit_SIMtest(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SIM, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SIMdetect(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SIM, 2, in_buf, out_buf, __FUNCTION__);
}

//10:Vibrator
NEPDLLAPI int __stdcall Detroit_VibRun(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_VIB, 1, in_buf, out_buf, __FUNCTION__);
}

//12:Modem
NEPDLLAPI int __stdcall Detroit_TxRxPower(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_MDM, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_QmiRfTxTest(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_MDM, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_QmiRfTxOff(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_MDM, 4, in_buf, out_buf, __FUNCTION__);
}

//13:BT
NEPDLLAPI int __stdcall Detroit_BTid(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_BT, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_BTTestMode(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_BT, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_BTTXPower(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_BT, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_BTRXrssi(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_BT, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_BTSelfTest(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_BT, 5, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_BTLteTx(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_BT, 6, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_BTLteRx(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_BT, 7, in_buf, out_buf, __FUNCTION__);
}
NEPDLLAPI int __stdcall Detroit_BTReset(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_BT, 8, in_buf, out_buf, __FUNCTION__);
}



// 14-WLAN
NEPDLLAPI int __stdcall Detroit_WLAN_Mode(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_WLAN, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_WLAN_RXrssi(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf) // cmd: 3
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_WLAN, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_WLAN_RXset(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf) // cmd: 4
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_WLAN, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_WLAN_ScanAP(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf) // cmd: 4
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_WLAN, 5, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_WLAN_ConnectAP(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf) // cmd: 4
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_WLAN, 6, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_WLANid(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_WLAN, 7, in_buf, out_buf, __FUNCTION__);
}

// 16-FMR
NEPDLLAPI int __stdcall Detroit_FMRrssi(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_FMR, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_FMRModuleOn(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_FMR, 4, in_buf, out_buf, __FUNCTION__);
}

// 17-NFC
NEPDLLAPI int __stdcall Detroit_NFCid(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_NFC, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_NFCFWVersion(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_NFC, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_NFCAntennaSelftest(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_NFC, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_NFCSwpSelftest(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_NFC, 4, in_buf, out_buf, __FUNCTION__);
}

// 18-Sensor
NEPDLLAPI int __stdcall Detroit_SNSRgsensorID(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SNSR, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRgsensorAcc(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SNSR, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRgsensorCalib(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SNSR, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRecompassID(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SNSR, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRecompassValue(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SNSR, 5, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRlsensorID(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SNSR, 7, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRlsensorValue(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SNSR, 8, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRlsensorCalib(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SNSR, 9, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRpsensorID(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SNSR, 10, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRpsensorValue(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SNSR, 11, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRpsensorCalib(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SNSR, 12, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRgyroscopeID(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SNSR, 13, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRgyroscopeValue(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SNSR, 14, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRgyroscopeCalib(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SNSR, 15, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRGyroscopeSelfTest(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SNSR, 16, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRgsensorSelfTest(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SNSR, 17, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSREcompassSelfTest(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SNSR, 18, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SNSRloadFALPkdata(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SNSR, 99, in_buf, out_buf, __FUNCTION__);
}

// 19-LED
NEPDLLAPI int __stdcall Detroit_LEDid(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_LED, 1, in_buf, out_buf, __FUNCTION__);
}
NEPDLLAPI int __stdcall Detroit_LEDon(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_LED, 2, in_buf, out_buf, __FUNCTION__);
}

// 20-SD
NEPDLLAPI int __stdcall Detroit_SDdetect(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SD, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SDtestStart(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SD, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SDtestStop(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SD, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SDinfo(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SD, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_SDdetectPin(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_SD, 5, in_buf, out_buf, __FUNCTION__);
}

// 21-ID
NEPDLLAPI int __stdcall Detroit_CPUid(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_ID, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_BoardADC(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_ID, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_BoardRev(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_ID, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_RFid(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_ID, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_MHLid(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_ID, 5, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_RFICid(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_ID, 8, in_buf, out_buf, __FUNCTION__);
}

//90:OS
NEPDLLAPI int __stdcall Detroit_OSgetprop(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_OS, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_OSkeyPower(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_OS, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_OSpwr2home(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)//cmd:2
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_OS, 3, in_buf, out_buf, __FUNCTION__);
}

//90-2 shutdown when disconnect
NEPDLLAPI int __stdcall Detroit_ShutDown(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_OS, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_OSkeyFocus(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_OS, 5, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_OSkeyCamera(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_OS, 6, in_buf, out_buf, __FUNCTION__);
}

//91:version
NEPDLLAPI int __stdcall Detroit_VERftd(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_VER, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_VERchange(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	char* prop_name = "ro.build.version.incremental";
	return Detroit_OSgetprop(port, szTimeout_ms_ms, prop_name, out_buf);
}

//92:factory
NEPDLLAPI int __stdcall Detroit_FAC_FARead(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf) //cmd:1
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_FAC, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_FAC_FAWrite(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf) //cmd:2
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_FAC, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_FAC_Picasso(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf) //cmd:3
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_FAC, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_FAC_OTPRead(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf) //cmd:4
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_FAC, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_FAC_OTPWrite(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf) //cmd:5
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_FAC, 5, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_FAC_OTPEnable(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf) //cmd:6
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_FAC, 6, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_FAC_CFGRead(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf) //cmd:7
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_FAC, 7, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_FAC_CFGWrite(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf) //cmd:8
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_FAC, 8, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_FAC_FSGProtect(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf) //cmd:9
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_FAC, 9, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_FAC_BackupFA(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf) //cmd:10
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_FAC, 10, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_FAC_BackupCFG(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf) //cmd:11
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_FAC, 11, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_FAC_RestoreFA(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf) //cmd:12
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_FAC, 12, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Detroit_FAC_RestoreCFG(int port, int szTimeout_ms_ms, char* in_buf, char* out_buf) //cmd:13
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, szTimeout_ms_ms, Detroit_FT_FAC, 13, in_buf, out_buf, __FUNCTION__);
}

//92,***************** SPU **********************************************
NEPDLLAPI int Detroit_FAC_CFGRead_SPU(char* szPort, char* szTimeout_ms, char* in_buf, char* out_buf, char* error_code) //cmd:1
{
	TRACER_FUNCTION_IN();

	int nPort = atoi(szPort);
	int nTimeout_ms = atoi(szTimeout_ms);
	int ret_code = HS_FAIL;
	if (InitDevice(nPort, nTimeout_ms) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = Detroit_FAC_CFGRead(nPort, nTimeout_ms, in_buf, buffer);
		FreeDevice(nPort, nTimeout_ms);
		strcpy(out_buf, buffer);
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI int Detroit_FAC_CFGWrite_SPU(char* szPort, char* szTimeout_ms, char* inType, char* inValue, char* out_buf, char* error_code) //cmd:1
{
	TRACER_FUNCTION_IN();

	int nPort = atoi(szPort);
	int nTimeout_ms = atoi(szTimeout_ms);
	int ret_code = HS_FAIL;
	if (InitDevice(nPort, nTimeout_ms) == HS_OK)
	{
		char buffer[4096] = {0};
		strcat(inType, ",");
		strcat(inType, inValue);
		ret_code = Detroit_FAC_CFGWrite(nPort, nTimeout_ms, inType, buffer);
		FreeDevice(nPort, nTimeout_ms);
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI int Detroit_AutoFtdMode_SPU(char* szPort, char* szTimeout_ms, char* error_code)
{
	TRACER_FUNCTION_IN();
	return Detroit_AutoFtdMode(atoi(szPort), atoi(szTimeout_ms), error_code);
}

NEPDLLAPI int Detroit_PWRbattCapacity_SPU(char* szPort, char* szTimeout_ms, char* in_buf, char* out_buf, char* error_code) //cmd:1
{
	TRACER_FUNCTION_IN();

	int nPort = atoi(szPort);
	int nTimeout_ms = atoi(szTimeout_ms);
	int ret_code = HS_FAIL;
	if (InitDevice(nPort, nTimeout_ms) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = Detroit_PWRbattCapacity(nPort, nTimeout_ms, in_buf, buffer);
		FreeDevice(nPort, nTimeout_ms);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	if (ret_code == HS_OK)
	{
		int batt_capacity = atoi(out_buf);
		if (batt_capacity < 50)
		{
			MessageBoxW(NULL, L"电量不足，请送充电站", NULL, MB_ICONWARNING);
			ret_code = -1;
		}
	}
	else
	{
		MessageBoxW(NULL, L"read Batter Capacity FAIL,读不到电量，请确认系统版本", NULL, MB_ICONWARNING);
		ret_code = -1;
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI int Detroit_FAC_FARead_SPU(char* szPort, char* szTimeout_ms, char* in_buf, char* out_buf, char* error_code) //cmd:1
{
	TRACER_FUNCTION_IN();

	int nPort = atoi(szPort);
	int nTimeout_ms = atoi(szTimeout_ms);
	int ret_code = HS_FAIL;
	if (InitDevice(nPort, nTimeout_ms) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = Detroit_FAC_FARead(nPort, nTimeout_ms, in_buf, buffer);
		FreeDevice(nPort, nTimeout_ms);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI int Detroit_FAC_OTPRead_SPU(char* szPort, char* szTimeout_ms, char* in_buf, char* out_buf, char* error_code) //cmd:1
{
	TRACER_FUNCTION_IN();

	int nPort = atoi(szPort);
	int nTimeout_ms = atoi(szTimeout_ms);
	int ret_code = HS_FAIL;
	if (InitDevice(nPort, nTimeout_ms) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = Detroit_FAC_OTPRead(nPort, nTimeout_ms, in_buf, buffer);
		FreeDevice(nPort, nTimeout_ms);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI int Detroit_FAC_OTPWrite_SPU(char* szPort, char* szTimeout_ms, char* inType, char* inValue, char* out_buf, char* error_code) //cmd:1
{
	TRACER_FUNCTION_IN();

	int nPort = atoi(szPort);
	int nTimeout_ms = atoi(szTimeout_ms);
	/*if exist IMEI and compare PASS , return true, 20120720 LION*/
	if (Detroit_CheckOTPExist(szPort, szTimeout_ms, inType, inValue, out_buf))
		return true;
	int ret_code = HS_FAIL;
	if (InitDevice(nPort, nTimeout_ms) == HS_OK)
	{
		char buffer[4096] = {0};
		strcat(inType, _T(","));
		strcat(inType, inValue);
		ret_code = Detroit_FAC_OTPWrite(nPort, nTimeout_ms, inType, buffer);
		FreeDevice(nPort, nTimeout_ms);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI bool Detroit_CheckOTPExist(char* szPort, char* szTimeout_ms, char* inType, char* inValue, char* out_buf)
{
	TRACER_FUNCTION_IN();

	int nPort = atoi(szPort);
	int nTimeout_ms = atoi(szTimeout_ms);
	int ret_code = HS_FAIL;
	if (InitDevice(nPort, nTimeout_ms) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = Detroit_FAC_OTPRead(nPort, nTimeout_ms, inType, buffer);
		FreeDevice(nPort, nTimeout_ms);
		if (ret_code == HS_OK && strcmp(inValue, buffer) == 0)
			return true;
	}
	return false;
}

NEPDLLAPI int Detroit_FAC_OTPEnable_SPU(char* szPort, char* szTimeout_ms, char* inType, char* inValue, char* out_buf, char* error_code) //cmd:1
{
	TRACER_FUNCTION_IN();

	int nPort = atoi(szPort);
	int nTimeout_ms = atoi(szTimeout_ms);
	int ret_code = HS_FAIL;
	if (InitDevice(nPort, nTimeout_ms) == HS_OK)
	{
		char buffer[4096] = {0};
		strcat(inType, _T(","));
		strcat(inType, inValue);
		ret_code = Detroit_FAC_OTPEnable(nPort, nTimeout_ms, inType, buffer);
		FreeDevice(nPort, nTimeout_ms);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI bool Detroit_ExecAdbOut_SPU(char* Command, char* output, char* error_msg)
{
	TRACER_FUNCTION_IN();
	return Detroit_ExecAdbOut(Command, output, error_msg);
}

NEPDLLAPI bool Detroit_ExecFastbootOut_SPU(char* Command, char* output, char* error_msg)
{
	TRACER_FUNCTION_IN();
	return Detroit_ExecFastbootOut(Command, output, error_msg);
}

NEPDLLAPI int Detroit_TestFtdMode_SPU(char* szPort, char* szTimeout_ms, char* error_msg)
{
	TRACER_FUNCTION_IN();
	return Detroit_TestFtdMode(atoi(szPort), atoi(szTimeout_ms), error_msg);
}

//90-4 shutdown system when disconnect
NEPDLLAPI int Detroit_FAC_ShutDown_SPU(char* szPort, char* szTimeout_ms, char* inValue, char* out_buf, char* error_code) //cmd:1
{
	TRACER_FUNCTION_IN();

	int nPort = atoi(szPort);
	int nTimeout_ms = atoi(szTimeout_ms);
	int ret_code = HS_FAIL;
	if (InitDevice(nPort, nTimeout_ms) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = Detroit_ShutDown(nPort, nTimeout_ms, inValue, buffer);
		FreeDevice(nPort, nTimeout_ms);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

//if I or F, continue, In Fcheck, Lion 20121206
NEPDLLAPI int Detroit_CheckIMEIStatus_SPU(char* szPort, char* szTimeout_ms, char* in_buf, char* out_buf, char* error_code) //cmd:1
{
	TRACER_FUNCTION_IN();
	return (strcmp(in_buf, "I") == 0 || (strcmp(in_buf, "F") == 0));
}

NEPDLLAPI int Detroit_FAC_FSGProtect_SPU(char* szPort, char* szTimeout_ms, char* inType, char* inValue, char* out_buf, char* error_code) //cmd:1
{
	TRACER_FUNCTION_IN();

	int nPort = atoi(szPort);
	int nTimeout_ms = atoi(szTimeout_ms);
	int ret_code = HS_FAIL;
	if (InitDevice(nPort, nTimeout_ms) == HS_OK)
	{
		char buffer[4096] = {0};
		strcat(inType, _T(","));
		strcat(inType, inValue);
		ret_code = Detroit_FAC_FSGProtect(nPort, nTimeout_ms, inType, buffer);
		FreeDevice(nPort, nTimeout_ms);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI int Detroit_FAC_BackupFA_SPU(char* szPort, char* szTimeout_ms, char* inValue, char* out_buf, char* error_code) //cmd:10
{
	TRACER_FUNCTION_IN();

	int nPort = atoi(szPort);
	int nTimeout_ms = atoi(szTimeout_ms);
	int ret_code = HS_FAIL;
	if (InitDevice(nPort, nTimeout_ms) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = Detroit_FAC_BackupFA(nPort, nTimeout_ms, inValue, buffer);
		FreeDevice(nPort, nTimeout_ms);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI int Detroit_FAC_BackupCFG_SPU(char* szPort, char* szTimeout_ms, char* inValue, char* out_buf, char* error_code) //cmd:11
{
	TRACER_FUNCTION_IN();

	int nPort = atoi(szPort);
	int nTimeout_ms = atoi(szTimeout_ms);
	int ret_code = HS_FAIL;
	if (InitDevice(nPort, nTimeout_ms) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = Detroit_FAC_BackupCFG(nPort, nTimeout_ms, inValue, buffer);
		FreeDevice(nPort, nTimeout_ms);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI int Detroit_FAC_RestoreFA_SPU(char* szPort, char* szTimeout_ms, char* inValue, char* out_buf, char* error_code) //cmd:12
{
	TRACER_FUNCTION_IN();

	int nPort = atoi(szPort);
	int nTimeout_ms = atoi(szTimeout_ms);
	int ret_code = HS_FAIL;
	if (InitDevice(nPort, nTimeout_ms) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = Detroit_FAC_RestoreFA(nPort, nTimeout_ms, inValue, buffer);
		FreeDevice(nPort, nTimeout_ms);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI int Detroit_FAC_RestoreCFG_SPU(char* szPort, char* szTimeout_ms, char* inValue, char* out_buf, char* error_code) //cmd:13
{
	TRACER_FUNCTION_IN();

	int nPort = atoi(szPort);
	int nTimeout_ms = atoi(szTimeout_ms);
	int ret_code = HS_FAIL;
	if (InitDevice(nPort, nTimeout_ms) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = Detroit_FAC_RestoreCFG(nPort, nTimeout_ms, inValue, buffer);
		FreeDevice(nPort, nTimeout_ms);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

//****************** SPU *********************************************
NEPDLLAPI bool Detroit_SplitSimData_SPU(char* input, char* output, char* error_code)
{
	TRACER_FUNCTION_IN();
	return Detroit_SplitSimData(input, output, error_code);
}

NEPDLLAPI bool __stdcall Detroit_SplitSimData(char* input, char* output, char* error_code)
{
	CString csInput;
	csInput.Format(_T("%s"), input);
	int nInputLength = csInput.GetLength();
	CString csSimData = csInput.Mid(256, (nInputLength - 256));
	strcpy(output, (LPCTSTR)csSimData);
	return true;
}

NEPDLLAPI bool Detroit_SplitPublicKey_SPU(char* input, char* output, char* error_code)
{
	TRACER_FUNCTION_IN();
	return Detroit_SplitPublicKey(input, output, error_code);
}

NEPDLLAPI bool __stdcall Detroit_SplitPublicKey(char* input, char* output, char* error_code)
{
	CString csInput(input);
	CString csPublicKey = csInput.Left(256);
	strcpy(output, (LPCTSTR)csPublicKey);
	return true;
}
