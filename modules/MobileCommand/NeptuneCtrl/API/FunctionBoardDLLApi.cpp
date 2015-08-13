#include "stdafx.h"
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include "io.h"
#include "../NeptuneCtrl.h"
#include "GeneralDLLApi.h" // GeneralCmd
#include "FunctionBoardDLLApi.h"
#include "FunctionBoardType.h"

using namespace std;

extern CNeptuneCtrlApp theApp;

NEPDLLAPI int __stdcall FB_InitDevice(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->InitDevice(nPort);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	return nRetCode;
}

NEPDLLAPI int __stdcall FB_FreeDevice(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->FreeDevice(nPort);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	return nRetCode;
}

NEPDLLAPI int __stdcall FB_VBatMeasurement(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_VBAT};
	return GeneralCmdOut(nPort, nTimeout, cGenCommand, OutBuffer);
}

NEPDLLAPI int __stdcall FB_VRDBBMeasurement(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_VRDBB};
	int nRetCode = GeneralCmdOut(nPort, nTimeout, cGenCommand, OutBuffer);

	unsigned int nHighByte = 0;
	nHighByte += ((unsigned int)OutBuffer[0]);
	nHighByte = nHighByte<<8;
	unsigned int nLowByte = 0xff;
	nLowByte &= (unsigned int)OutBuffer[1];

	double nValue = (double)(nHighByte + nLowByte);
	nValue /= 1000.0;
	memset(OutBuffer, 0, sizeof(OutBuffer));
	sprintf(OutBuffer, "%lf", nValue);

	return nRetCode;
}

NEPDLLAPI int __stdcall FB_BackupBatMeasurement(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_BACKUP_BATTERY};
	int nRetCode = GeneralCmdOut(nPort, nTimeout, cGenCommand, OutBuffer);

	unsigned int nHighByte = 0;
	nHighByte += ((unsigned int)OutBuffer[0]);
	nHighByte = nHighByte<<8;
	unsigned int nLowByte = 0xff;
	nLowByte &= (unsigned int)OutBuffer[1];

	double nValue = (double)(nHighByte + nLowByte);
	nValue /= 1000.0;
	memset(OutBuffer, 0, sizeof(OutBuffer));
	sprintf(OutBuffer, "%lf", nValue);

	return nRetCode;
}

NEPDLLAPI int __stdcall FB_VRIOMeasurement(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_VRIO};
	int nRetCode = GeneralCmdOut(nPort, nTimeout, cGenCommand, OutBuffer);

	unsigned int nHighByte = 0;
	nHighByte += ((unsigned int)OutBuffer[0]);
	nHighByte = nHighByte<<8;
	unsigned int nLowByte = 0xff;
	nLowByte &= (unsigned int)OutBuffer[1];

	double nValue = (double)(nHighByte + nLowByte);
	nValue /= 1000.0;
	memset(OutBuffer, 0, sizeof(OutBuffer));
	sprintf(OutBuffer, "%lf", nValue);

	return nRetCode;
}

NEPDLLAPI int __stdcall FB_VMEMMeasurement(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,0};
	int nRetCode = GeneralCmdOut(nPort, nTimeout, cGenCommand, OutBuffer);

	unsigned int nHighByte = 0;
	nHighByte += ((unsigned int)OutBuffer[0]);
	nHighByte = nHighByte<<8;
	unsigned int nLowByte = 0xff;
	nLowByte &= (unsigned int)OutBuffer[1];

	double nValue = (double)(nHighByte + nLowByte);
	nValue /= 1000.0;
	memset(OutBuffer, 0, sizeof(OutBuffer));
	sprintf(OutBuffer, "%lf", nValue);

	return nRetCode;
}

NEPDLLAPI int __stdcall FB_VRSIMMeasurement(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_VRSIM};
	int nRetCode = GeneralCmdOut(nPort, nTimeout, cGenCommand, OutBuffer);

	unsigned int nHighByte = 0;
	nHighByte += ((unsigned int)OutBuffer[0]);
	nHighByte = nHighByte<<8;
	unsigned int nLowByte = 0xff;
	nLowByte &= (unsigned int)OutBuffer[1];

	double nValue = (double)(nHighByte + nLowByte);
	nValue /= 1000.0;
	memset(OutBuffer, 0, sizeof(OutBuffer));
	sprintf(OutBuffer, "%lf", nValue);

	return nRetCode;
}

NEPDLLAPI int __stdcall FB_VCAPMeasurement(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_VCAP_3P3V};
	int nRetCode = GeneralCmdOut(nPort, nTimeout, cGenCommand, OutBuffer);

	unsigned int nHighByte = 0;
	nHighByte += ((unsigned int)OutBuffer[0]);
	nHighByte = nHighByte<<8;
	unsigned int nLowByte = 0xff;
	nLowByte &= (unsigned int)OutBuffer[1];

	double nValue = (double)(nHighByte + nLowByte);
	nValue /= 1000.0;
	nValue *= VOLTAGE_DIVISION_COEFFICIENT;
	memset(OutBuffer, 0, sizeof(OutBuffer));
	sprintf(OutBuffer, "%lf", nValue);

	return nRetCode;
}

NEPDLLAPI int __stdcall FB_Buck1P8VMeasurement(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_BUCK_1P8V};
	int nRetCode = GeneralCmdOut(nPort, nTimeout, cGenCommand, OutBuffer);

	unsigned int nHighByte = 0;
	nHighByte += ((unsigned int)OutBuffer[0]);
	nHighByte = nHighByte<<8;
	unsigned int nLowByte = 0xff;
	nLowByte &= (unsigned int)OutBuffer[1];

	double nValue = (double)(nHighByte + nLowByte);
	nValue /= 1000.0;
	memset(OutBuffer, 0, sizeof(OutBuffer));
	sprintf(OutBuffer, "%lf", nValue);

	return nRetCode;
}

NEPDLLAPI int __stdcall FB_BLCurrentMeasurement(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_BACKLIGHT};
	return GeneralCmdOut(nPort, nTimeout, cGenCommand, OutBuffer);
}

NEPDLLAPI int __stdcall FB_SideKeyLEDCurrentMeasurement(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_SIDE_KEY_LED};
	return GeneralCmdOut(nPort, nTimeout, cGenCommand, OutBuffer);
}

NEPDLLAPI int __stdcall FB_StatusKeyCurrentMeasurement(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_STATUS_LED};
	return GeneralCmdOut(nPort, nTimeout, cGenCommand, OutBuffer);
}

NEPDLLAPI int __stdcall FB_VibratorCurrentMeasurement(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_VIBRATOR};
	return GeneralCmdOut(nPort, nTimeout, cGenCommand, OutBuffer);
}

NEPDLLAPI int __stdcall FB_LoadSpeakerAudioMeasurement(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_LOUD_SPEAKER};
	return GeneralCmdOut(nPort, nTimeout, cGenCommand, OutBuffer);
}

NEPDLLAPI int __stdcall FB_ReceiverAudioMeasurement(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_RECEIVER};
	return GeneralCmdOut(nPort, nTimeout, cGenCommand, OutBuffer);
}

NEPDLLAPI int __stdcall FB_PressPwrKey(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_PRESS_HS_POWER_KEY};
	return GeneralCmd(nPort, nTimeout, cGenCommand);
}

NEPDLLAPI int __stdcall FB_AccessoryInsert(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_ACCIN_INT_ON};
	return GeneralCmd(nPort, nTimeout, cGenCommand);
}

NEPDLLAPI int __stdcall FB_AccessoryPullout(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_ACCIN_INT_OFF};
	return GeneralCmd(nPort, nTimeout, cGenCommand);
}

NEPDLLAPI int __stdcall FB_PwrSwitchOVP(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_CHG_OVP};
	return GeneralCmd(nPort, nTimeout, cGenCommand);
}

NEPDLLAPI int __stdcall FB_PwrSwitchPreCharge(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_PRE_CHG};
	return GeneralCmd(nPort, nTimeout, cGenCommand);
}

NEPDLLAPI int __stdcall FB_PwrSwitchFullCharge(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_FULL_CHG};
	return GeneralCmd(nPort, nTimeout, cGenCommand);
}

NEPDLLAPI int __stdcall FB_PwrSwitchNormalMode(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_NORMAL_MODE};
	return GeneralCmd(nPort, nTimeout, cGenCommand);
}

NEPDLLAPI int __stdcall FB_PwrSwitchNoPwrToHandset(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_NO_PWR_To_HS};
	return GeneralCmd(nPort, nTimeout, cGenCommand);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_PwrSwitchUsbCharge(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_USB_CHG};
	return GeneralCmd(nPort, nTimeout, cGenCommand);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AlsLedOn(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	byte cGenCommand[4] = {0,0,0,CMD_ALS_LED_ON};
	return GeneralCmd(nPort, nTimeout, (char*)cGenCommand);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AlsLedOff(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	byte cGenCommand[4] = {0,0,0,CMD_ALS_LED_OFF};
	return GeneralCmd(nPort, nTimeout, (char*)cGenCommand);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_ElectromagnetOn(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	byte cGenCommand[4] = {0,0,0,CMD_ELECTROMAGNET_ON};
	return GeneralCmd(nPort, nTimeout, (char*)cGenCommand);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_ElectromagnetOff(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	byte cGenCommand[4] = {0,0,0,CMD_ELECTROMAGNET_OFF};
	return GeneralCmd(nPort, nTimeout, (char*)cGenCommand);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_LdoOn4Leakage(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	byte cGenCommand[4] = {0,0,0,CMD_LDO_ON};
	return GeneralCmd(nPort, nTimeout, (char*)cGenCommand);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_LdoOff4Leakage(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	byte cGenCommand[4] = {0,0,0,CMD_LDO_OFF};
	return GeneralCmd(nPort, nTimeout, (char*)cGenCommand);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AdcSpeakPositive(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_SPK_POS};
	return GeneralCmdOut(nPort, nTimeout, (char*)cGenCommand, OutBuffer);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AdcSpeakNegative(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_SPK_NEG};
	return GeneralCmdOut(nPort, nTimeout, (char*)cGenCommand, OutBuffer);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AdcReceiverPositive(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_REV_POS};
	return GeneralCmdOut(nPort, nTimeout, (char*)cGenCommand, OutBuffer);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AdcReceiverNegative(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_REV_NEG};
	return GeneralCmdOut(nPort, nTimeout, (char*)cGenCommand, OutBuffer);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AdcAlsLed1(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_ALS_LED1};
	return GeneralCmdOut(nPort, nTimeout, (char*)cGenCommand, OutBuffer);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AdcAlsLed2(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_ALS_LED2};
	return GeneralCmdOut(nPort, nTimeout, (char*)cGenCommand, OutBuffer);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AdcAlsLed3(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_ALS_LED3};
	return GeneralCmdOut(nPort, nTimeout, (char*)cGenCommand, OutBuffer);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AdcHsPositive(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_HS_POS};
	return GeneralCmdOut(nPort, nTimeout, (char*)cGenCommand, OutBuffer);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AdcHsNegative(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_HS_NEG};
	return GeneralCmdOut(nPort, nTimeout, (char*)cGenCommand, OutBuffer);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AdcLoPositive(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_LO_POS};
	return GeneralCmdOut(nPort, nTimeout, (char*)cGenCommand, OutBuffer);
}

// 2009-09-15 Mick: for Austin EVT3
NEPDLLAPI int __stdcall FB_AdcLoNegative(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	char cGenCommand[4] = {0,0,0,CMD_LO_NEG};
	return GeneralCmdOut(nPort, nTimeout, (char*)cGenCommand, OutBuffer);
}

NEPDLLAPI int __stdcall FB_GetVersion(int nPort, int nTimeout, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	unsigned char cGenCommand[4] = {0,0,0,CMD_GET_CODE_VERSION};
	return GeneralCmdOut(nPort, nTimeout, (char*)cGenCommand, OutBuffer);
}

NEPDLLAPI int __stdcall EnableComport(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
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
	strcat(szParam, InBuffer);
	ShellExecute(NULL, "open", "devcon.exe", szParam, NULL, 0); //param: "enable USB\\VID_067B*"

	return HS_OK;
}

NEPDLLAPI int __stdcall DisableComport(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
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
	strcat(szParam, InBuffer);
	ShellExecute(NULL, "open", "devcon.exe", szParam, NULL, 0); //"disable USB\\VID_067B*"

	return 1;
}

NEPDLLAPI int __stdcall ExecuteBatchFile(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	ShellExecute(NULL, "open", InBuffer, NULL, NULL, 0); //Execute Batch File
	return 1;
}

// 2009-10-26 Mick: new relay board function
NEPDLLAPI int __stdcall RB_InitDevice(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->InitDevice(nPort);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}
	else { // if ok, reset to 0
		RB_Relay2(nPort, nTimeout, "0");
		RB_Relay1(nPort, nTimeout, "0");
	}

	return nRetCode;
}

NEPDLLAPI int __stdcall RB_FreeDevice(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->FreeDevice(nPort);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	return nRetCode;
}

// use DTR to control
NEPDLLAPI int __stdcall RB_Relay2(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (strlen(InBuffer) == 0) {
		return HS_FAIL;
	}

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	bool isEnable = atoi(InBuffer) > 0;
	int nRetCode = theApp.m_pCmdCtrl[nPort]->ComDtrCtrl(nPort, nTimeout, isEnable);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	return nRetCode;
}

// use RTS to control
NEPDLLAPI int __stdcall RB_Relay1(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (strlen(InBuffer) == 0) {
		return HS_FAIL;
	}

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	bool isEnable = atoi(InBuffer) > 0;
	int nRetCode = theApp.m_pCmdCtrl[nPort]->ComRtsCtrl(nPort, nTimeout, isEnable);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	return nRetCode;
}

NEPDLLAPI int __stdcall RB_Relay0(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	byte ch = 0x0f;
	int nRetCode = theApp.m_pCmdCtrl[nPort]->ComTxCtrl(nPort, nTimeout, &ch, 1);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	return nRetCode;
}

// Relay2
NEPDLLAPI int __stdcall RB_USB_Charging(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	return RB_Relay2(nPort, nTimeout, InBuffer);
}

// Relay1
NEPDLLAPI int __stdcall RB_HC_Charging(int nPort, int nTimeout, char* InBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	return RB_Relay1(nPort, nTimeout, InBuffer);
}

// Relay0
NEPDLLAPI int __stdcall RB_PowerKey(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	return RB_Relay0(nPort, nTimeout);
}

