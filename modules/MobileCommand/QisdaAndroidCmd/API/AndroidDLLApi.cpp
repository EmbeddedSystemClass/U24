#include "stdafx.h"
#include <cstddef> // size_t

#include "../CmdType.h"
#include "GeneralDLLApi.h" // GeneraCmd
#include "AndroidDLLApi.h"
#include "io.h"
#include "../Utility.h"
#include "../FactoryBlock.h"

#include <direct.h>
#include <string.h>

static const int CMD_MAX_RETRY = 1;

static int MobileCmd(int port, int timeout_ms, byte ftd_cmd_main, byte ftd_cmd_sub,
                     char* in_buf, char* out_buf, const char* func_name)
{
	byte ftd_cmd[4] = {ftd_cmd_main, ftd_cmd_sub, 0, 0};
	int ret_code = -1 * ERROR_FUNCTION_FAILED;
	for (int nTry = 0; nTry < CMD_MAX_RETRY; nTry++)
	{
		ret_code = GeneralCmdInOut(port, timeout_ms, ftd_cmd, in_buf, strlen(in_buf), out_buf);
		TRACE(_T("%s(%d)\n"), func_name, ret_code);
		if (ret_code == HS_OK)
		{
			break;
		}
		Sleep(500);
	}
	return ret_code;
}

NEPDLLAPI bool __stdcall FTD_ExecAdbOut(CString Command, char* output, char* error_msg)
{
	TRACER_FUNCTION_IN();
	return ExecAdbOut(Command, output, error_msg);
}

NEPDLLAPI bool __stdcall FTD_ExecFastbootOut(CString Command, char* output, char* error_msg)
{
	TRACER_FUNCTION_IN();
	return ExecFastbootOut(Command, output, error_msg);
}

NEPDLLAPI int __stdcall FTD_TestFtdMode(int port, int timeout_ms, char* error_msg)
{
	TRACER_FUNCTION_IN();
	return TestFtdMode(port, timeout_ms, error_msg);
}

NEPDLLAPI int __stdcall FTD_AutoFtdMode(int port, int timeout_ms, char* error_msg)
{
	TRACER_FUNCTION_IN();
	return AutoFtdMode(port, timeout_ms, error_msg);
}

// 01-Battery
NEPDLLAPI int __stdcall FTD_BATTcoincellVoltage(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_BATT, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_BATTcoincellChargeOn(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_BATT, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_BATTrtcTime(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_BATT, 3, in_buf, out_buf, __FUNCTION__);
}

// 02-Power
NEPDLLAPI int __stdcall FTD_PWRgaugeID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_PWR, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_PWRgaugeVoltage(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_PWR, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_PWRgaugeCurrent(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_PWR, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_PWRgaugeTemperature(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_PWR, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_PWRchargerID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_PWR, 5, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_PWRchargerOn(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_PWR, 6, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_DFIupgrade(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_PWR, 7, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_ITenable(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_PWR, 8, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_Regualator(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_PWR, 9, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_PWRbattID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_PWR, 10, in_buf, out_buf, __FUNCTION__);
}
NEPDLLAPI int __stdcall FTD_PWRRFBandIDTemperature(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_PWR, 11, in_buf, out_buf, __FUNCTION__);
}
NEPDLLAPI int __stdcall FTD_PWRbattCapacity(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_PWR, 12, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_PWRWriteRsenseData(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_PWR, 13, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_PWRAverageGaugeCurrent(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_PWR, 14, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_WriteMaxChargeCurrent(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_PWR, 15, in_buf, out_buf, __FUNCTION__);
}
NEPDLLAPI int __stdcall FTD_ReadPowerMaxCurrent(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_PWR, 16, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_ReadBuckID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_PWR, 18, in_buf, out_buf, __FUNCTION__);
}
NEPDLLAPI int __stdcall FTD_ReadBuckfeature(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_PWR, 19, in_buf, out_buf, __FUNCTION__);
}

//03:Memory
NEPDLLAPI int __stdcall FTD_MCPflashID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_MCP, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_MCPddrID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_MCP, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_MCPddrSize(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_MCP, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_MCPCsdInfo(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_MCP, 7, in_buf, out_buf, __FUNCTION__);
}
NEPDLLAPI int __stdcall FTD_MCPemmcsize(int port, int timeout_ms, char* in_buf, char* out_buf)		//Eason
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_MCP, 8, in_buf, out_buf, __FUNCTION__);
}
NEPDLLAPI int __stdcall FTD_MCPemmcFWversion(int port, int timeout_ms, char* in_buf, char* out_buf) //Eason
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_MCP, 9, in_buf, out_buf, __FUNCTION__);
}
NEPDLLAPI int __stdcall FTD_MCPddrmr8(int port, int timeout_ms, char* in_buf, char* out_buf)		//Eason
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_MCP, 10, in_buf, out_buf, __FUNCTION__);
}
NEPDLLAPI int __stdcall FTD_MCPddrmr6(int port, int timeout_ms, char* in_buf, char* out_buf)		//Eason
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_MCP, 11, in_buf, out_buf, __FUNCTION__);
}
//04:LCM - LCD & Touch
NEPDLLAPI int __stdcall FTD_TOUCHpoint(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_LCM, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_TOUCHtest(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_LCM, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_TOUCHid(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_LCM, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_TOUCHversion(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_LCM, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_TOUCHSelfTest(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_LCM, 5, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_LCMbacklight(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_LCM, 6, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_LCMbacklightColor(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_LCM, 7, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_TouchModel(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_LCM, 10, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_TouchCheck(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_LCM, 13, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_OLEDTest(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_LCM, 14, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_BacklightDriver(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_LCM, 14, in_buf, out_buf, __FUNCTION__);
}



//05:audio
NEPDLLAPI int __stdcall FTD_AUpathOut(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_AU, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_AUplaymp3(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_AU, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_AUrecordWAV(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_AU, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_AUplayWAV(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_AU, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_AUdetectHeadset(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_AU, 5, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_AUpathRec(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_AU, 6, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_AUloopbackTest(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_AU, 9, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_AUButtonCheck(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_AU, 10, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_AUinitADC(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_AU, 11, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_AUADCCali(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_AU, 12, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_AUheadsetInfo(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_AU, 13, in_buf, out_buf, __FUNCTION__);
}

//06:camera
NEPDLLAPI int __stdcall FTD_CAMReadID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_CAM, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_CAMshot(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_CAM, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_CAMstart(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_CAM, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_CAMend(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_CAM, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_CAMFlashLED(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_CAM, 5, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_CAMFlashLEDID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_CAM, 6, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_CAM_OTP_AWB(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_CAM, 7, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_CAM_OTP_AFMacro(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_CAM, 8, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_CAM_OTP_AFInfinity(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_CAM, 9, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_CAM_OTP_FrontFuseID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_CAM, 10, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_CAM_OTP_WriteFrontFuseID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_CAM, 11, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_CAM_Read_Bridge_ID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_CAM, 12, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_CAM_EEPROM_Power(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_CAM, 13, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_CAM_EEPROM_Write(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_CAM, 14, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_CAM_EEPROM_Read(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_CAM, 15, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_CAM_MIPI_LVDS_Status(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_CAM, 16, in_buf, out_buf, __FUNCTION__);
}


//7:key
NEPDLLAPI int __stdcall FTD_KeyTest(int port, int timeout_ms, char* in_buf, char* out_buf)//cmd:2
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_KEY, 1, in_buf, out_buf, __FUNCTION__);
}

//08:SIM
NEPDLLAPI int __stdcall FTD_SIMtest(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SIM, 1, in_buf, out_buf, __FUNCTION__);
}


//09:USB
NEPDLLAPI int __stdcall FTD_USBtest(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_USB, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_SIMdetect(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SIM, 2, in_buf, out_buf, __FUNCTION__);
}

//10:Vibrator
NEPDLLAPI int __stdcall FTD_VibRun(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_VIB, 1, in_buf, out_buf, __FUNCTION__);
}

//12:Modem
NEPDLLAPI int __stdcall FTD_TxRxPower(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_MDM, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_QmiRfTxTest(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_MDM, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_QmiRfTxOff(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_MDM, 4, in_buf, out_buf, __FUNCTION__);
}

//13:BT
NEPDLLAPI int __stdcall FTD_BTid(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_BT, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_BTTestMode(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_BT, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_BTTXPower(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_BT, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_BTRXrssi(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_BT, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_BTSelfTest(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_BT, 5, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_BTLteTx(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_BT, 6, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_BTLteRx(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_BT, 7, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_BT_INQUIRY(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_BT, 10, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_2_4GPowerMode(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_BT, 14, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_2_4GConsumer(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_BT, 15, in_buf, out_buf, __FUNCTION__);
}

// 14-WLAN
NEPDLLAPI int __stdcall FTD_WLAN_Mode(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_WLAN, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_WLAN_RXrssi(int port, int timeout_ms, char* in_buf, char* out_buf) // cmd: 3
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_WLAN, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_WLAN_RXset(int port, int timeout_ms, char* in_buf, char* out_buf) // cmd: 4
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_WLAN, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_WLAN_ScanAP(int port, int timeout_ms, char* in_buf, char* out_buf) // cmd: 4
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_WLAN, 5, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_WLAN_ConnectAP(int port, int timeout_ms, char* in_buf, char* out_buf) // cmd: 4
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_WLAN, 6, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_WLANid(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_WLAN, 7, in_buf, out_buf, __FUNCTION__);
}
NEPDLLAPI int __stdcall FTD_WLAN_selfTest(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_WLAN, 8, in_buf, out_buf, __FUNCTION__);
}


NEPDLLAPI int __stdcall FTD_WLAN_MODULE_Test(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_WLAN, 10, in_buf, out_buf, __FUNCTION__);
}

// 16-FMR
NEPDLLAPI int __stdcall FTD_FMRrssi(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_FMR, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_FMRModuleOn(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_FMR, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_FMR_SelfTest(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_FMR, 3, in_buf, out_buf, __FUNCTION__);
}


// 17-NFC
NEPDLLAPI int __stdcall FTD_NFCid(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_NFC, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_NFCFWVersion(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_NFC, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_NFCAntennaSelftest(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_NFC, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_NFCSwpSelftest(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_NFC, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_NFCHCITypeAReader(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_NFC, 5, in_buf, out_buf, __FUNCTION__);
}

// 18-Sensor
NEPDLLAPI int __stdcall FTD_SNSRgsensorID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SNSR, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_SNSRgsensorAcc(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SNSR, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_SNSRgsensorCalib(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SNSR, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_SNSRecompassID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SNSR, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_SNSRecompassValue(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SNSR, 5, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_SNSRlsensorID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SNSR, 7, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_SNSRlsensorValue(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SNSR, 8, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_SNSRlsensorCalib(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SNSR, 9, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_SNSRpsensorID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SNSR, 10, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_SNSRpsensorValue(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SNSR, 11, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_SNSRpsensorCalib(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SNSR, 12, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_SNSRgyroscopeID(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SNSR, 13, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_SNSRgyroscopeValue(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SNSR, 14, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_SNSRgyroscopeCalib(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SNSR, 15, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_SNSRGyroscopeSelfTest(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SNSR, 16, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_SNSRgsensorSelfTest(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SNSR, 17, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_SNSREcompassSelfTest(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SNSR, 18, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_SNSRloadFALPkdata(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SNSR, 99, in_buf, out_buf, __FUNCTION__);
}

// 19-LED
NEPDLLAPI int __stdcall FTD_LEDid(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_LED, 1, in_buf, out_buf, __FUNCTION__);
}
NEPDLLAPI int __stdcall FTD_LEDon(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_LED, 2, in_buf, out_buf, __FUNCTION__);
}

// 20-SD
NEPDLLAPI int __stdcall FTD_SDdetect(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SD, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_SDtestStart(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SD, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_SDtestStop(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SD, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_SDinfo(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SD, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_SDdetectPin(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_SD, 5, in_buf, out_buf, __FUNCTION__);
}

// 21-ID
NEPDLLAPI int __stdcall FTD_CPUid(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_ID, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_BoardADC(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_ID, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_BoardRev(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_ID, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_RFid(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_ID, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_MHLid(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_ID, 5, in_buf, out_buf, __FUNCTION__);
}
NEPDLLAPI int __stdcall FTD_PMid(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_ID, 6, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_SOCversion(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_ID, 7, in_buf, out_buf, __FUNCTION__);
}


NEPDLLAPI int __stdcall FTD_RFICid(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_ID, 8, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_PCBVerMPP3(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_ID, 9, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_PCBVerMPP2(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_ID, 10, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_PMIid(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_ID, 11, in_buf, out_buf, __FUNCTION__);
}
NEPDLLAPI int __stdcall FTD_RFASMid(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_ID, 12, in_buf, out_buf, __FUNCTION__);
}
NEPDLLAPI int __stdcall FTD_AntennaIC(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_ID, 13, in_buf, out_buf, __FUNCTION__);
}
NEPDLLAPI int __stdcall FTD_Boardid(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_ID, 14, in_buf, out_buf, __FUNCTION__);
}
NEPDLLAPI int __stdcall FTD_SKUid(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_ID, 15, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_SOCfeature(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_ID, 17, in_buf, out_buf, __FUNCTION__);
}


//90:OS
NEPDLLAPI int __stdcall FTD_OSgetprop(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_OS, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_OSkeyPower(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_OS, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_OSpwr2home(int port, int timeout_ms, char* in_buf, char* out_buf)//cmd:2
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_OS, 3, in_buf, out_buf, __FUNCTION__);
}

//90-2 shutdown when disconnect
NEPDLLAPI int __stdcall FTD_ShutDown(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_OS, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_OSkeyFocus(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_OS, 5, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_OSkeyCamera(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_OS, 6, in_buf, out_buf, __FUNCTION__);
}

//91:version
NEPDLLAPI int __stdcall FTD_VERftd(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_VER, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_VERchange(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	char* prop_name = "ro.build.version.incremental";
	return  FTD_OSgetprop(port, timeout_ms, prop_name, out_buf);
}

//92:factory
NEPDLLAPI int __stdcall FTD_FAC_FARead(int port, int timeout_ms, char* in_buf, char* out_buf) //cmd:1
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_FAC, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_FAC_FAWrite(int port, int timeout_ms, char* in_buf, char* out_buf) //cmd:2
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_FAC, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_FAC_Picasso(int port, int timeout_ms, char* in_buf, char* out_buf) //cmd:3
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_FAC, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_FAC_OTPRead(int port, int timeout_ms, char* in_buf, char* out_buf) //cmd:4
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_FAC, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_FAC_OTPWrite(int port, int timeout_ms, char* in_buf, char* out_buf) //cmd:5
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_FAC, 5, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_FAC_OTPEnable(int port, int timeout_ms, char* in_buf, char* out_buf) //cmd:6
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_FAC, 6, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_FAC_CFGRead(int port, int timeout_ms, char* in_buf, char* out_buf) //cmd:7
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_FAC, 7, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_FAC_CFGWrite(int port, int timeout_ms, char* in_buf, char* out_buf) //cmd:8
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_FAC, 8, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_FAC_FSGProtect(int port, int timeout_ms, char* in_buf, char* out_buf) //cmd:9
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_FAC, 9, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_FAC_BackupFA(int port, int timeout_ms, char* in_buf, char* out_buf) //cmd:10
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_FAC, 10, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_FAC_BackupCFG(int port, int timeout_ms, char* in_buf, char* out_buf) //cmd:11
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_FAC, 11, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_FAC_RestoreFA(int port, int timeout_ms, char* in_buf, char* out_buf) //cmd:12
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_FAC, 12, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_FAC_RestoreCFG(int port, int timeout_ms, char* in_buf, char* out_buf) //cmd:13
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_FAC, 13, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_FAC_EraseL1Log(int port, int timeout_ms, char* in_buf, char* out_buf) //cmd:14
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_FAC, 14, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_FAC_BackupModemCFG(int port, int timeout_ms, char* in_buf, char* out_buf) //cmd:15
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_FAC, 15, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_FAC_RestoreModemCFG(int port, int timeout_ms, char* in_buf, char* out_buf) //cmd:16
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_FAC, 16, in_buf, out_buf, __FUNCTION__);
}

//93:misc
NEPDLLAPI int __stdcall FTD_MISC_CallDrop(int port, int timeout_ms, char* in_buf, char* out_buf) //cmd:2
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_MISC, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_Get_GPIO_Value(int port, int timeout_ms, char* in_buf, char* out_buf) //cmd:3
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_MISC, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall FTD_Check_APQ_UART(int port, int timeout_ms, char* in_buf, char* out_buf) //cmd:3
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Andriod_FT_MISC, 5, in_buf, out_buf, __FUNCTION__);
}



//***************** Normal Functions SPU **************************//
NEPDLLAPI int FTD_TestFtdMode_SPU(char* Port, char* Timeout, char* error_msg)
{
	TRACER_FUNCTION_IN();
	return TestFtdMode(atoi(Port), atoi(Timeout), error_msg);
}

NEPDLLAPI int FTD_AutoFtdMode_SPU(char* Port, char* Timeout, char* error_code)
{
	TRACER_FUNCTION_IN();
	return AutoFtdMode(atoi(Port), atoi(Timeout), error_code);
}


NEPDLLAPI bool FTD_ExecAdbOut_SPU(char* Command, char* output, char* error_msg)
{
	TRACER_FUNCTION_IN();
	return ExecAdbOut(Command, output, error_msg);
}

NEPDLLAPI bool FTD_ExecAdbPull_SPU(char* From, char* To, char* output, char* error_msg)
{
	TRACER_FUNCTION_IN();

	char Command[4096] = {0};
	strcat(Command, "pull ");
	strcat(Command, From);
	strcat(Command, " ");
	strcat(Command, To);

	return ExecAdbOut(Command, output, error_msg);
}

NEPDLLAPI bool FTD_ExecAdbPush_SPU(char* From, char* To, char* output, char* error_msg)
{
	TRACER_FUNCTION_IN();

	char Command[4096] = {0};
	strcat(Command, "push ");
	strcat(Command, From);
	strcat(Command, " ");
	strcat(Command, To);

	return ExecAdbOut(Command, output, error_msg);
}

NEPDLLAPI bool FTD_ExecFastbootOut_SPU(char* Command, char* output, char* error_msg)
{
	TRACER_FUNCTION_IN();
	return ExecFastbootOut(Command, output, error_msg);
}

NEPDLLAPI int FTD_PWRgaugeVoltage_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code) //cmd:2-2
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	bool b_res = false;
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_PWRgaugeVoltage(i_port, i_timeout, in_buf, buffer);
		FreeDevice(i_port, i_timeout);
		double d_output = atof(buffer);
		d_output /= 1000000;
		sprintf(buffer, "%.2f", d_output);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	if ( ret_code == HS_OK ) b_res = true;

	return b_res;
}

NEPDLLAPI int FTD_PWRbattCapacity_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code) //cmd:2-12
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	int i_check_value = atoi(in_buf);
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_PWRbattCapacity(i_port, i_timeout, in_buf, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	if (ret_code == HS_OK)
	{
		int batt_capacity = atoi(out_buf);
		if (batt_capacity < i_check_value)
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

NEPDLLAPI bool FTD_TouchCheck_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code) //cmd:4-13
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	bool b_res = false;
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_TouchCheck(i_port, i_timeout, in_buf, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	if ( ret_code == HS_OK ) b_res = true;

	return b_res;
}

NEPDLLAPI bool FTD_TOUCHSelfTest_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code) //cmd:4-13
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	bool b_res = false;
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_TOUCHSelfTest(i_port, i_timeout, in_buf, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	if ( ret_code == HS_OK ) b_res = true;

	return b_res;
}

NEPDLLAPI bool FTD_OLEDTest_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code) //cmd:4-14
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	bool b_res = false;
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_OLEDTest(i_port, i_timeout, in_buf, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	if ( ret_code == HS_OK ) b_res = true;

	return b_res;
}

NEPDLLAPI bool FTD_AUplayWAV_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code) //cmd:5-4
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	bool b_res = false;
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_AUplayWAV(i_port, i_timeout, in_buf, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	if ( ret_code == HS_OK ) b_res = true;

	return b_res;
}

NEPDLLAPI bool FTD_AUloopbackTest_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code) //cmd:5-9
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	bool b_res = false;
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_AUloopbackTest(i_port, i_timeout, in_buf, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	if ( ret_code == HS_OK ) b_res = true;

	return b_res;
}

NEPDLLAPI bool FTD_CAMshot_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code) //cmd:6-2
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	bool b_res = false;
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_CAMshot(i_port, i_timeout, in_buf, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	if ( ret_code == HS_OK ) b_res = true;

	return b_res;
}

NEPDLLAPI bool FTD_CAMstart_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code) //cmd:6-3
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	bool b_res = false;
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_CAMstart(i_port, i_timeout, in_buf, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	if ( ret_code == HS_OK ) b_res = true;

	return b_res;
}

NEPDLLAPI bool FTD_CAMend_SPU(char* Port, char* Timeout, char* out_buf, char* error_code) //cmd:6-4
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	bool b_res = false;
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_CAMend(i_port, i_timeout, "", buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	if ( ret_code == HS_OK ) b_res = true;
	
	return b_res;
}

NEPDLLAPI bool FTD_CAM_OTP_WriteFrontFuseID_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code) //cmd:6-11
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	bool b_res = false;
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_CAM_OTP_WriteFrontFuseID(i_port, i_timeout, in_buf, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	if ( ret_code == HS_OK )
		if ( strncmp(in_buf, out_buf, 12) == 0 ) b_res = true; 

	return b_res;
}

NEPDLLAPI bool FTD_CAM_Read_Bridge_ID_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code) //cmd:6-12
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	bool b_res = false;
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_CAM_Read_Bridge_ID(i_port, i_timeout, in_buf, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	if ( ret_code == HS_OK )
		if ( strncmp(in_buf, out_buf, 10) == 0 )b_res = true;
		
	return b_res;
}

NEPDLLAPI bool FTD_CAM_EEPROM_Power_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code) //cmd:6-13
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	bool b_res = false;
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_CAM_EEPROM_Power(i_port, i_timeout, in_buf, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	if ( ret_code == HS_OK ) b_res = true;
	
	return b_res;
}

NEPDLLAPI bool FTD_CAM_EEPROM_Write_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code) //cmd:6-14
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	bool b_res = false;
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_CAM_EEPROM_Write(i_port, i_timeout, in_buf, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	if ( ret_code == HS_OK ) b_res = true;

	return b_res;
}

NEPDLLAPI bool FTD_CAM_EEPROM_Read_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code) //cmd:6-15
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	bool b_res = false;
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_CAM_EEPROM_Read(i_port, i_timeout, in_buf, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	if ( ret_code == HS_OK ) b_res = true;

	return b_res;
}

NEPDLLAPI bool FTD_CAM_MIPI_LVDS_Status_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code) //cmd:6-15
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	bool b_res = false;
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_CAM_MIPI_LVDS_Status(i_port, i_timeout, in_buf, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	
	if ( ret_code == HS_OK ) {		
		int i_out_buf = atoi(out_buf);
		if ( i_out_buf==3 ) {
			b_res = true;
			sprintf(error_code, "PASS (output value = %d)", i_out_buf);
		}
		else if ( i_out_buf==2 ) sprintf(error_code, "sLVDS fail (output value = %d)", i_out_buf);
		else if ( i_out_buf==1 ) sprintf(error_code, "MIPI fail (output value = %d)", i_out_buf);
		else if ( i_out_buf==0 ) sprintf(error_code, "sLVDS&MIPI fail (output value = %d)", i_out_buf);
	}else sprintf(error_code, "%d", ret_code);

	return b_res;
}

NEPDLLAPI bool FTD_LEDon_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code) //cmd:19-2
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	bool b_res = false;
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_LEDon(i_port, i_timeout, in_buf, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	if ( ret_code == HS_OK ) b_res = true;

	return b_res;
}

NEPDLLAPI bool FTD_Boardid_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code) //cmd:21-14
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	bool b_res = false;
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_Boardid(i_port, i_timeout, in_buf, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	if ( ret_code == HS_OK ) b_res = true;

	return b_res;
}

NEPDLLAPI int FTD_OSgetprop_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code) //cmd:90-1
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_OSgetprop(i_port, i_timeout, in_buf, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

//90-4 shutdown system when disconnect
NEPDLLAPI int FTD_FAC_ShutDown_SPU(char* Port, char* Timeout, char* InValue, char* out_buf, char* error_code) //cmd:90-4
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_ShutDown(i_port, i_timeout, InValue, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI int FTD_FAC_FARead_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code) //cmd:92-1
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_FAC_FARead(i_port, i_timeout, in_buf, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI int FTD_FAC_OTPRead_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code) //cmd:92-4
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_FAC_OTPRead(i_port, i_timeout, in_buf, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI int FTD_FAC_OTPWrite_SPU(char* Port, char* Timeout, char* InType, char* InValue, char* out_buf, char* error_code) //cmd:92-5
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	/*if exist IMEI and compare PASS , return true, 20120720 LION*/
	if (FTD_CheckOTPExist(Port, Timeout, InType, InValue, out_buf))
		return true;

	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		strcat(InType, _T(","));
		strcat(InType, InValue);
		ret_code = FTD_FAC_OTPWrite(i_port, i_timeout, InType, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI bool FTD_CheckOTPExist(char* Port, char* Timeout, char* InType, char* InValue, char* out_buf)
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_FAC_OTPRead(i_port, i_timeout, InType, buffer);
		FreeDevice(i_port, i_timeout);
		if (ret_code == HS_OK && strcmp(InValue, buffer) == 0)
			return true;
	}
	return false;
}

NEPDLLAPI int FTD_FAC_OTPEnable_SPU(char* Port, char* Timeout, char* InType, char* InValue, char* out_buf, char* error_code) //cmd:92-6
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		strcat(InType, _T(","));
		strcat(InType, InValue);
		ret_code = FTD_FAC_OTPEnable(i_port, i_timeout, InType, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}


NEPDLLAPI int FTD_FAC_CFGRead_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code) //cmd:92-7
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_FAC_CFGRead(i_port, i_timeout, in_buf, buffer);
		FreeDevice(i_port, i_timeout);
		strcpy(out_buf, buffer);
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI int FTD_FAC_CFGWrite_SPU(char* Port, char* Timeout, char* InType, char* InValue, char* out_buf, char* error_code) //cmd:92-8
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		strcat(InType, ",");
		strcat(InType, InValue);
		ret_code = FTD_FAC_CFGWrite(i_port, i_timeout, InType, buffer);
		FreeDevice(i_port, i_timeout);
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI int FTD_FAC_FSGProtect_SPU(char* Port, char* Timeout, char* InType, char* InValue, char* out_buf, char* error_code) //cmd:92-9
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		strcat(InType, _T(","));
		strcat(InType, InValue);
		ret_code = FTD_FAC_FSGProtect(i_port, i_timeout, InType, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI int FTD_FAC_BackupFA_SPU(char* Port, char* Timeout, char* InValue, char* out_buf, char* error_code) //cmd:92-10
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_FAC_BackupFA(i_port, i_timeout, InValue, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI int FTD_FAC_BackupCFG_SPU(char* Port, char* Timeout, char* InValue, char* out_buf, char* error_code) //cmd:92-11
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_FAC_BackupCFG(i_port, i_timeout, InValue, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI int FTD_FAC_RestoreFA_SPU(char* Port, char* Timeout, char* InValue, char* out_buf, char* error_code) //cmd:92-12
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_FAC_RestoreFA(i_port, i_timeout, InValue, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI int FTD_FAC_RestoreCFG_SPU(char* Port, char* Timeout, char* InValue, char* out_buf, char* error_code) //cmd:92-13
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_FAC_RestoreCFG(i_port, i_timeout, InValue, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI int FTD_FAC_EraseL1Log_SPU(char* Port, char* Timeout, char* InValue, char* out_buf, char* error_code)//92-14
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_FAC_EraseL1Log(i_port, i_timeout, InValue, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI int FTD_FAC_BackupModemCFG_SPU(char* Port, char* Timeout, char* InValue, char* out_buf, char* error_code)//92-15
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_FAC_BackupModemCFG(i_port, i_timeout, InValue, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

NEPDLLAPI int FTD_FAC_RestoreModemCFG_SPU(char* Port, char* Timeout, char* InValue, char* out_buf, char* error_code)//92-16
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_FAC_RestoreModemCFG(i_port, i_timeout, InValue, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	return ret_code;
}

//***************** Special Control Functions SPU **************************//
NEPDLLAPI bool FTD_SplitSimData_SPU(char* input, char* output, char* error_code)
{
	TRACER_FUNCTION_IN();
	return FTD_SplitSimData(input, output, error_code);
}

NEPDLLAPI bool __stdcall FTD_SplitSimData(char* input, char* output, char* error_code)
{
	CString csInput;
	csInput.Format(_T("%s"), input);
	int nInputLength = csInput.GetLength();
	CString csSimData = csInput.Mid(256, (nInputLength - 256));
	strcpy(output, (LPCTSTR)csSimData);
	return true;
}

NEPDLLAPI bool FTD_SplitPublicKey_SPU(char* input, char* output, char* error_code)
{
	TRACER_FUNCTION_IN();
	return FTD_SplitPublicKey(input, output, error_code);
}

NEPDLLAPI bool __stdcall FTD_SplitPublicKey(char* input, char* output, char* error_code)
{
	CString csInput(input);
	CString csPublicKey = csInput.Left(256);
	strcpy(output, (LPCTSTR)csPublicKey);
	return true;
}

NEPDLLAPI int FTD_CheckIMEIStatus_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code)
{
	TRACER_FUNCTION_IN();
	//if I or F, continue, In Fcheck, Lion 20121206
	return (strcmp(in_buf, "I") == 0 || strcmp(in_buf, "F") == 0);
}

NEPDLLAPI bool FTD_WDS_Contorl_SPU(char* in_buf, char* out_buf, char* error_msg)
{
	TRACER_FUNCTION_IN();
	char Command[2048];
	sprintf(Command, "shell am start -a android.settings.WIFI_DISPLAY_SETTINGS  --es switchWFD \"%s\"", in_buf);
	return ExecAdbOut(Command, out_buf, error_msg);
}


NEPDLLAPI bool FTD_CheckDeviceNotInOSMode_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code)
{
	TRACER_FUNCTION_IN();

	int i_port = atoi(Port);
	int i_timeout = atoi(Timeout);
	int ret_code = HS_FAIL;
	if (InitDevice(i_port, i_timeout) == HS_OK)
	{
		char buffer[4096] = {0};
		ret_code = FTD_OSgetprop(i_port, i_timeout, in_buf, buffer);
		FreeDevice(i_port, i_timeout);
		memcpy(out_buf, buffer, 512 * sizeof(char));
	}
	sprintf(error_code, "%d", ret_code);
	if (ret_code != (HS_FAIL+ERR_GENERALCMDINOUT+ERR_TYPE_USB)) return false;

	return true;
}

NEPDLLAPI bool FTD_SaveBIResult_SPU(char* BIResult, char* LogFolder, char* TargetID, char* out_buf, char* error_code)
{
	TRACER_FUNCTION_IN();

	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);

	char sz_file_name[4096] = {0};
	sprintf(sz_file_name, "BI_%s_%04d%02d%02d_%02d%02d_%02d.txt", TargetID,
		systemTime.wYear, systemTime.wMonth, systemTime.wDay,
		systemTime.wHour, systemTime.wMinute, systemTime.wSecond);

	char in_string[4096] = {0};
	sprintf(in_string, "pull %s %s\\%s", BIResult, LogFolder, sz_file_name);
	
	_mkdir( LogFolder );
	
	bool bRet = false;
	char out_string[4096] = {0};
	char err_string[4096] = {0};
	bRet = FTD_ExecAdbOut(in_string, out_string, err_string);
	memcpy(out_buf, out_string, 512 * sizeof(char));
	memcpy(error_code, err_string, 512 * sizeof(char));

	return bRet;
}
