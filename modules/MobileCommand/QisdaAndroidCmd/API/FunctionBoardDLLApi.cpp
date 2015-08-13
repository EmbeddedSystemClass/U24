#include "stdafx.h"
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include "io.h"
#include "../QisdaAndroidCmdCtrl.h"
#include "GeneralDLLApi.h" // GeneralCmd
#include "FunctionBoardDLLApi.h"
#include "FunctionBoardType.h"

using namespace std;

extern CQisdaAndroidCmdCtrlApp theApp;

NEPDLLAPI int __stdcall FB_InitDevice(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[port] == NULL) {
		theApp.m_pCmdCtrl[port] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[port]->InitDevice(port);
	if (nRetCode != HS_OK) {
		nRetCode += (port <= QTH_PORT_COM_MAX) ? ERR_TYPE_UART : ERR_TYPE_USB;
	}

	return nRetCode;
}

NEPDLLAPI int __stdcall FB_FreeDevice(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[port] == NULL) {
		theApp.m_pCmdCtrl[port] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[port]->FreeDevice(port);
	if (nRetCode != HS_OK) {
		nRetCode += (port <= QTH_PORT_COM_MAX) ? ERR_TYPE_UART : ERR_TYPE_USB;
	}

	return nRetCode;
}

NEPDLLAPI int __stdcall FB_VBatMeasurement(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_VBAT};
	return GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);
}

NEPDLLAPI int __stdcall FB_VRDBBMeasurement(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_VRDBB};
	int nRetCode = GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);

	unsigned int nHighByte = 0;
	nHighByte += ((unsigned int)out_buf[0]);
	nHighByte = nHighByte<<8;
	unsigned int nLowByte = 0xff;
	nLowByte &= (unsigned int)out_buf[1];

	double nValue = (double)(nHighByte + nLowByte);
	nValue /= 1000.0;
	memset(out_buf, 0, sizeof(out_buf));
	sprintf(out_buf, "%lf", nValue);

	return nRetCode;
}

NEPDLLAPI int __stdcall FB_BackupBatMeasurement(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_BACKUP_BATTERY};
	int nRetCode = GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);

	unsigned int nHighByte = 0;
	nHighByte += ((unsigned int)out_buf[0]);
	nHighByte = nHighByte<<8;
	unsigned int nLowByte = 0xff;
	nLowByte &= (unsigned int)out_buf[1];

	double nValue = (double)(nHighByte + nLowByte);
	nValue /= 1000.0;
	memset(out_buf, 0, sizeof(out_buf));
	sprintf(out_buf, "%lf", nValue);

	return nRetCode;
}

NEPDLLAPI int __stdcall FB_VRIOMeasurement(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_VRIO};
	int nRetCode = GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);

	unsigned int nHighByte = 0;
	nHighByte += ((unsigned int)out_buf[0]);
	nHighByte = nHighByte<<8;
	unsigned int nLowByte = 0xff;
	nLowByte &= (unsigned int)out_buf[1];

	double nValue = (double)(nHighByte + nLowByte);
	nValue /= 1000.0;
	memset(out_buf, 0, sizeof(out_buf));
	sprintf(out_buf, "%lf", nValue);

	return nRetCode;
}

NEPDLLAPI int __stdcall FB_VMEMMeasurement(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,0};
	int nRetCode = GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);

	unsigned int nHighByte = 0;
	nHighByte += ((unsigned int)out_buf[0]);
	nHighByte = nHighByte<<8;
	unsigned int nLowByte = 0xff;
	nLowByte &= (unsigned int)out_buf[1];

	double nValue = (double)(nHighByte + nLowByte);
	nValue /= 1000.0;
	memset(out_buf, 0, sizeof(out_buf));
	sprintf(out_buf, "%lf", nValue);

	return nRetCode;
}

NEPDLLAPI int __stdcall FB_VRSIMMeasurement(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_VRSIM};
	int nRetCode = GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);

	unsigned int nHighByte = 0;
	nHighByte += ((unsigned int)out_buf[0]);
	nHighByte = nHighByte<<8;
	unsigned int nLowByte = 0xff;
	nLowByte &= (unsigned int)out_buf[1];

	double nValue = (double)(nHighByte + nLowByte);
	nValue /= 1000.0;
	memset(out_buf, 0, sizeof(out_buf));
	sprintf(out_buf, "%lf", nValue);

	return nRetCode;
}

NEPDLLAPI int __stdcall FB_VCAPMeasurement(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_VCAP_3P3V};
	int nRetCode = GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);

	unsigned int nHighByte = 0;
	nHighByte += ((unsigned int)out_buf[0]);
	nHighByte = nHighByte<<8;
	unsigned int nLowByte = 0xff;
	nLowByte &= (unsigned int)out_buf[1];

	double nValue = (double)(nHighByte + nLowByte);
	nValue /= 1000.0;
	nValue *= VOLTAGE_DIVISION_COEFFICIENT;
	memset(out_buf, 0, sizeof(out_buf));
	sprintf(out_buf, "%lf", nValue);

	return nRetCode;
}

NEPDLLAPI int __stdcall FB_Buck1P8VMeasurement(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_BUCK_1P8V};
	int nRetCode = GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);

	unsigned int nHighByte = 0;
	nHighByte += ((unsigned int)out_buf[0]);
	nHighByte = nHighByte<<8;
	unsigned int nLowByte = 0xff;
	nLowByte &= (unsigned int)out_buf[1];

	double nValue = (double)(nHighByte + nLowByte);
	nValue /= 1000.0;
	memset(out_buf, 0, sizeof(out_buf));
	sprintf(out_buf, "%lf", nValue);

	return nRetCode;
}

NEPDLLAPI int __stdcall FB_BLCurrentMeasurement(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_BACKLIGHT};
	return GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);
}

NEPDLLAPI int __stdcall FB_SideKeyLEDCurrentMeasurement(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_SIDE_KEY_LED};
	return GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);
}

NEPDLLAPI int __stdcall FB_StatusKeyCurrentMeasurement(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_STATUS_LED};
	return GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);
}

NEPDLLAPI int __stdcall FB_VibratorCurrentMeasurement(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_VIBRATOR};
	return GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);
}

NEPDLLAPI int __stdcall FB_LoadSpeakerAudioMeasurement(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_LOUD_SPEAKER};
	return GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);
}

NEPDLLAPI int __stdcall FB_ReceiverAudioMeasurement(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_RECEIVER};
	return GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);
}

NEPDLLAPI int __stdcall FB_PressPwrKey(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_PRESS_HS_POWER_KEY};
	return GeneralCmd(port, timeout_ms, fb_cmd);
}

NEPDLLAPI int __stdcall FB_AccessoryInsert(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_ACCIN_INT_ON};
	return GeneralCmd(port, timeout_ms, fb_cmd);
}

NEPDLLAPI int __stdcall FB_AccessoryPullout(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_ACCIN_INT_OFF};
	return GeneralCmd(port, timeout_ms, fb_cmd);
}

NEPDLLAPI int __stdcall FB_PwrSwitchOVP(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_CHG_OVP};
	return GeneralCmd(port, timeout_ms, fb_cmd);
}

NEPDLLAPI int __stdcall FB_PwrSwitchPreCharge(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_PRE_CHG};
	return GeneralCmd(port, timeout_ms, fb_cmd);
}

NEPDLLAPI int __stdcall FB_PwrSwitchFullCharge(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_FULL_CHG};
	return GeneralCmd(port, timeout_ms, fb_cmd);
}

NEPDLLAPI int __stdcall FB_PwrSwitchNormalMode(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_NORMAL_MODE};
	return GeneralCmd(port, timeout_ms, fb_cmd);
}

NEPDLLAPI int __stdcall FB_PwrSwitchNoPwrToHandset(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_NO_PWR_To_HS};
	return GeneralCmd(port, timeout_ms, fb_cmd);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_PwrSwitchUsbCharge(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_USB_CHG};
	return GeneralCmd(port, timeout_ms, fb_cmd);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AlsLedOn(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_ALS_LED_ON};
	return GeneralCmd(port, timeout_ms, fb_cmd);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AlsLedOff(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_ALS_LED_OFF};
	return GeneralCmd(port, timeout_ms, fb_cmd);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_ElectromagnetOn(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_ELECTROMAGNET_ON};
	return GeneralCmd(port, timeout_ms, fb_cmd);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_ElectromagnetOff(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte fb_cmd[4] = {0,0,0,CMD_ELECTROMAGNET_OFF};
	return GeneralCmd(port, timeout_ms, fb_cmd);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_LdoOn4Leakage(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();
	byte fb_cmd[4] = {0,0,0,CMD_LDO_ON};
	return GeneralCmd(port, timeout_ms, fb_cmd);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_LdoOff4Leakage(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();
	byte fb_cmd[4] = {0,0,0,CMD_LDO_OFF};
	return GeneralCmd(port, timeout_ms, fb_cmd);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AdcSpeakPositive(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte fb_cmd[4] = {0,0,0,CMD_SPK_POS};
	return GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AdcSpeakNegative(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte fb_cmd[4] = {0,0,0,CMD_SPK_NEG};
	return GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AdcReceiverPositive(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte fb_cmd[4] = {0,0,0,CMD_REV_POS};
	return GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AdcReceiverNegative(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte fb_cmd[4] = {0,0,0,CMD_REV_NEG};
	return GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AdcAlsLed1(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte fb_cmd[4] = {0,0,0,CMD_ALS_LED1};
	return GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AdcAlsLed2(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte fb_cmd[4] = {0,0,0,CMD_ALS_LED2};
	return GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AdcAlsLed3(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte fb_cmd[4] = {0,0,0,CMD_ALS_LED3};
	return GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AdcHsPositive(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte fb_cmd[4] = {0,0,0,CMD_HS_POS};
	return GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AdcHsNegative(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte fb_cmd[4] = {0,0,0,CMD_HS_NEG};
	return GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AdcLoPositive(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte fb_cmd[4] = {0,0,0,CMD_LO_POS};
	return GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AdcLoNegative(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte fb_cmd[4] = {0,0,0,CMD_LO_NEG};
	return GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);
}

NEPDLLAPI int __stdcall FB_GetVersion(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte fb_cmd[4] = {0,0,0,CMD_GET_CODE_VERSION};
	return GeneralCmdOut(port, timeout_ms, fb_cmd, out_buf);
}

NEPDLLAPI int __stdcall EnableComport(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	// Set Current Directory
	char szEXEPath[MAX_PATH];
	memset(szEXEPath, 0, sizeof(szEXEPath));
	::GetModuleFileName(NULL, szEXEPath, MAX_PATH);

	char szDrive[MAX_PATH];
	char szDir[MAX_PATH];
	_splitpath(szEXEPath, szDrive, szDir, NULL, NULL);

	char szCurDir[MAX_PATH];
	memset(szCurDir, 0, sizeof(MAX_PATH));
	strcat(szCurDir, szDrive);
	strcat(szCurDir, szDir);
	::SetCurrentDirectory(szCurDir);

	char szParam[MAX_PATH];
	memset(szParam, 0, sizeof(szParam));
	strcpy(szParam, "enable ");
	strcat(szParam, in_buf);
	ShellExecute(NULL, "open", "devcon.exe", szParam, NULL, 0); //param: "enable USB\\VID_067B*"

	return HS_OK;
}

NEPDLLAPI int __stdcall DisableComport(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	// Set Current Directory
	char szEXEPath[256];
	memset(szEXEPath, 0, sizeof(szEXEPath));
	::GetModuleFileName(NULL, szEXEPath, 256);

	char szDrive[256];
	char szDir[256];
	_splitpath(szEXEPath, szDrive, szDir, NULL, NULL);

	char szCurDir[256];
	memset(szCurDir, 0, sizeof(256));
	strcat(szCurDir, szDrive);
	strcat(szCurDir, szDir);
	::SetCurrentDirectory(szCurDir);

	char szParam[256];
	memset(szParam, 0, sizeof(szParam));
	strcpy(szParam, "disable ");
	strcat(szParam, in_buf);
	ShellExecute(NULL, "open", "devcon.exe", szParam, NULL, 0); //"disable USB\\VID_067B*"

	return 1;
}

NEPDLLAPI int __stdcall ExecuteBatchFile(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();
	ShellExecute(NULL, "open", in_buf, NULL, NULL, 0); //Execute Batch File
	return 1;
}

// 2009-10-26 Mick: new relay board function
NEPDLLAPI int __stdcall RB_InitDevice(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[port] == NULL) {
		theApp.m_pCmdCtrl[port] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[port]->InitDevice(port);
	if (nRetCode != HS_OK) {
		nRetCode += (port <= QTH_PORT_COM_MAX) ? ERR_TYPE_UART : ERR_TYPE_USB;
	}
	else { // if ok, reset to 0
		RB_Relay2(port, timeout_ms, "0");
		RB_Relay1(port, timeout_ms, "0");
	}
	return nRetCode;
}

NEPDLLAPI int __stdcall RB_FreeDevice(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[port] == NULL) {
		theApp.m_pCmdCtrl[port] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[port]->FreeDevice(port);
	if (nRetCode != HS_OK) {
		nRetCode += (port <= QTH_PORT_COM_MAX) ? ERR_TYPE_UART : ERR_TYPE_USB;
	}
	return nRetCode;
}

// use DTR to control
NEPDLLAPI int __stdcall RB_Relay2(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	if (strlen(in_buf) == 0) {
		return HS_FAIL;
	}

	if (theApp.m_pCmdCtrl[port] == NULL) {
		theApp.m_pCmdCtrl[port] = new CmdCtrl;
	}
	bool isEnable = atoi(in_buf) > 0;
	int nRetCode = theApp.m_pCmdCtrl[port]->ComDtrCtrl(port, timeout_ms, isEnable);
	if (nRetCode != HS_OK) {
		nRetCode += (port <= QTH_PORT_COM_MAX) ? ERR_TYPE_UART : ERR_TYPE_USB;
	}
	return nRetCode;
}

// use RTS to control
NEPDLLAPI int __stdcall RB_Relay1(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	if (strlen(in_buf) == 0) {
		return HS_FAIL;
	}

	if (theApp.m_pCmdCtrl[port] == NULL) {
		theApp.m_pCmdCtrl[port] = new CmdCtrl;
	}
	bool isEnable = atoi(in_buf) > 0;
	int nRetCode = theApp.m_pCmdCtrl[port]->ComRtsCtrl(port, timeout_ms, isEnable);
	if (nRetCode != HS_OK) {
		nRetCode += (port <= QTH_PORT_COM_MAX) ? ERR_TYPE_UART : ERR_TYPE_USB;
	}
	return nRetCode;
}

NEPDLLAPI int __stdcall RB_Relay0(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[port] == NULL) {
		theApp.m_pCmdCtrl[port] = new CmdCtrl;
	}
	byte ch = 0x0f;
	int nRetCode = theApp.m_pCmdCtrl[port]->ComTxCtrl(port, timeout_ms, &ch, 1);
	if (nRetCode != HS_OK) {
		nRetCode += (port <= QTH_PORT_COM_MAX) ? ERR_TYPE_UART : ERR_TYPE_USB;
	}
	return nRetCode;
}

// Relay2
NEPDLLAPI int __stdcall RB_USB_Charging(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();
	return RB_Relay2(port, timeout_ms, in_buf);
}

// Relay1
NEPDLLAPI int __stdcall RB_HC_Charging(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();
	return RB_Relay1(port, timeout_ms, in_buf);
}

// Relay0
NEPDLLAPI int __stdcall RB_PowerKey(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();
	return RB_Relay0(port, timeout_ms);
}
