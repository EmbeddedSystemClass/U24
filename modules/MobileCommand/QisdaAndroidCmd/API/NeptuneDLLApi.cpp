#include "stdafx.h"
#include <fstream>
#include <cstring>
#include "../QisdaAndroidCmdCtrlV2.h"
#include "GeneralDLLApi.h"
#include "NeptuneDLLApi.h"
#include "P21Type.h"
#include "../Utility.h" // _httoi


using namespace std;

extern CQisdaAndroidCmdCtrlApp theApp;


NEPDLLAPI int __stdcall P21ExactOpenDevice(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	return OpenDevice(port, timeout_ms);
}

NEPDLLAPI int __stdcall P21ExactCloseDevice(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	return CloseDevice(port, timeout_ms);
}


NEPDLLAPI int __stdcall P21VibratorOn(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_VIBRATOR,1,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

NEPDLLAPI int __stdcall P21VibratorOff(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_VIBRATOR,2,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

NEPDLLAPI int __stdcall P21VersionGetDTVersion(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_VERSION,1,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: GetDieID 0912
NEPDLLAPI int __stdcall P21HWGetDieID(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_HARDWARE,2,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

// 2007-06-11 YTT GetHWVersion by DT according to some hardware inormation
NEPDLLAPI int __stdcall P21HWGetHWVersion(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {P21_DT_HARDWARE,4,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

// 2008-01-16 Mark Hsieh
NEPDLLAPI int __stdcall P21SDCardMoveImgToDDR(int port, int timeout_ms, char* in_buf, size_t nInLength, char* out_buf)
{
	TRACER_FUNCTION_IN();
	byte dt_cmd[4] = {P21_DT_SD,16,0,0};
	return GeneralCmdInOut(port, timeout_ms, dt_cmd, in_buf, nInLength, out_buf);
}

NEPDLLAPI int __stdcall P21SDCardReadOSVersion(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_SD,17,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: ShowSDFileInfo 0912
NEPDLLAPI int __stdcall P21SDCardFileInfo(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_SD,1,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

NEPDLLAPI int __stdcall P21SDCardIO1Init(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_SD,10,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

NEPDLLAPI int __stdcall P21SDCardCMD(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_SD,11,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

//FF: 0920
NEPDLLAPI int __stdcall P21LCDInit(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LCD,1,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: 0920
NEPDLLAPI int __stdcall P21LCDRed(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LCD,P21_DT_LCD_COLORTEST,1,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: 0920
NEPDLLAPI int __stdcall P21LCDGreen(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LCD,P21_DT_LCD_COLORTEST,2,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: 0920
NEPDLLAPI int __stdcall P21LCDBlue(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LCD,P21_DT_LCD_COLORTEST,3,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

// 2006-11-09 YTT
// LCDBacklight/SIMPower
NEPDLLAPI int __stdcall P21LCDBlack(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LCD,P21_DT_LCD_COLORTEST,4,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

NEPDLLAPI int __stdcall P21LCDWhite(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LCD,P21_DT_LCD_COLORTEST,5,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

NEPDLLAPI int __stdcall P21LCDBacklightOn(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LCD,3,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

NEPDLLAPI int __stdcall P21LCDBacklightOff(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LCD,4,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

NEPDLLAPI int __stdcall P21ModemSIMPowerOn(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_MODEM,1,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

NEPDLLAPI int __stdcall P21ModemSIMPowerOff(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_MODEM,2,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

// 2007-06-11 YTT
NEPDLLAPI int __stdcall P21ModemSIMTest(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_MODEM,3,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF:clock RTC on
NEPDLLAPI int __stdcall P21ClockRTCOn(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_CLOCK,1,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF:RTC Off
NEPDLLAPI int __stdcall P21ClockRTCOff(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_CLOCK,2,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF:RTC read clock
NEPDLLAPI int __stdcall P21ClockReadClock(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_CLOCK,3,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF:need 20sec to timeout
NEPDLLAPI int __stdcall P21BTConnect(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_BT,5,0,0};
	int nRetCode = HS_FAIL;
	for (int i = 0; i < 3; i++) { // retry 3 times.
		nRetCode = GeneralCmdIn(port, 20000, dt_cmd, in_buf, strlen(in_buf));
		if (nRetCode == HS_OK) {
			break;
		}
	}
	return nRetCode;
}

/////////////////////////////Audio
//FF: 0920
NEPDLLAPI int __stdcall P21AudioHandsetOnMicRev(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_AUDIO,1,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: 0920
NEPDLLAPI int __stdcall P21AudioHeadsetOnHMicHRev(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_AUDIO,2,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: 0920
NEPDLLAPI int __stdcall P21AudioHHHFOnMicLS(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_AUDIO,3,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: 0920
NEPDLLAPI int __stdcall P21AudioAllOff(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_AUDIO,4,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: 0920
NEPDLLAPI int __stdcall P21AudioHeadsetDetect(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_AUDIO,5,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: 0920
NEPDLLAPI int __stdcall P21AudioToneToRev(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_AUDIO,6,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: 0920
NEPDLLAPI int __stdcall P21AudioToneToHRev(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_AUDIO,7,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

// 2007-01-12 YTT
NEPDLLAPI int __stdcall P21AudioHookDetect(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_AUDIO,12,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

///////////////////////////LED
//add by FF 0920
NEPDLLAPI int __stdcall P21LED1On(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LED,1,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}
//FF: 0920
NEPDLLAPI int __stdcall P21LED1Blink(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LED,2,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: 0920
NEPDLLAPI int __stdcall P21LED1Off(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LED,3,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}
//FF: 0920
NEPDLLAPI int __stdcall P21LED2On(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LED,4,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: 0920
NEPDLLAPI int __stdcall P21LED2Off(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LED,6,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: 0920
NEPDLLAPI int __stdcall P21LEDKeypadLedOn(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LED,7,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: 0920
NEPDLLAPI int __stdcall P21LEDKeypadLedOff(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LED,8,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: 0920
NEPDLLAPI int __stdcall P21LEDSidekey1LedOn(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LED,9,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: 0920
NEPDLLAPI int __stdcall P21LEDSidekey1LedOff(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LED,10,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: 0920
NEPDLLAPI int __stdcall P21LEDSidekey2LedOn(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LED,11,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: 0920
NEPDLLAPI int __stdcall P21LEDSidekey2LedOff(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LED,12,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

/////////////////////////////////////////ID
//FF: 0920
NEPDLLAPI int __stdcall P21HWGetTritonID(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_HARDWARE,1,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: 0920
NEPDLLAPI int __stdcall P21HWGetProductID(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_HARDWARE,3,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

///////////////////////////Read DT
//FF: 0920
NEPDLLAPI int __stdcall P21VersionGetDTDate(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_VERSION,2,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: 0920
NEPDLLAPI int __stdcall P21VersionGetDTName(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_VERSION,3,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

NEPDLLAPI int __stdcall P21VersionGetIPLVersion(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_VERSION,4,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

NEPDLLAPI int __stdcall P21VersionGetIPLDate(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_VERSION,5,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

NEPDLLAPI int __stdcall P21VersionGetIPLName(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_VERSION,6,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//callback function by YT 0921 intergrate by FF 0926
NEPDLLAPI int __stdcall P21RegisterCallBack(int port, int timeout_ms, char* RegCallBack)
{
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[port] == NULL) {
		theApp.m_pCmdCtrl[port] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[port]->RegisterCallBack((void*)RegCallBack);
	if (nRetCode != HS_OK) {
		nRetCode += (port <= QTH_PORT_COM_MAX) ? ERR_TYPE_UART : ERR_TYPE_USB;
	}

	return nRetCode;
}
//callback function by YT 0921 intergrate by FF 0926
NEPDLLAPI int __stdcall P21UnregisterCallBack(int port, int timeout_ms, char* RegCallBack)
{
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[port] == NULL) {
		theApp.m_pCmdCtrl[port] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[port]->UnregisterCallBack((void*)RegCallBack);
	if (nRetCode != HS_OK) {
		nRetCode += (port <= QTH_PORT_COM_MAX) ? ERR_TYPE_UART : ERR_TYPE_USB;
	}
	return nRetCode;
}

//callback function by YT 061112
NEPDLLAPI int __stdcall RegisterCallBackChar(int port, int timeout_ms, char* RegCallBack)
{
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[port] == NULL) {
		theApp.m_pCmdCtrl[port] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[port]->RegisterCallBackChar((void*)RegCallBack);
	if (nRetCode != HS_OK) {
		nRetCode += (port <= QTH_PORT_COM_MAX) ? ERR_TYPE_UART : ERR_TYPE_USB;
	}
	return nRetCode;
}
//callback function by YT 0921 intergrate by FF 0926
NEPDLLAPI int __stdcall UnregisterCallBackChar(int port, int timeout_ms, char* RegCallBack)
{
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[port] == NULL) {
		theApp.m_pCmdCtrl[port] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[port]->UnregisterCallBackChar((void*)RegCallBack);
	if (nRetCode != HS_OK) {
		nRetCode += (port <= QTH_PORT_COM_MAX) ? ERR_TYPE_UART : ERR_TYPE_USB;
	}

	return nRetCode;
}

//FF: 0928
NEPDLLAPI int __stdcall P21FlashFormatAll(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FLASH,1,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

NEPDLLAPI int __stdcall P21FlashGetServicePassword(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	char szIMEI[MAX_PATH];
	char szDieID[MAX_PATH];
	memset(szIMEI, 0, sizeof(szIMEI));
	memset(szDieID, 0, sizeof(szDieID));

	int nRetCode = HS_FAIL;
	nRetCode = P21FAReadIMEI(port, timeout_ms, szIMEI);
	if (nRetCode == HS_OK) {
		nRetCode = P21HWGetDieID(port, timeout_ms, szDieID);
	}
	if (nRetCode == HS_OK) {
		char* szIMEITemp;
		char* szDieIDTemp;
		szIMEITemp	= _strrev(_strdup(szIMEI));
		szDieIDTemp = _strrev(_strdup(szDieID));
		memset(szIMEI, 0, sizeof(szIMEI));
		memset(szDieID, 0, sizeof(szDieID));
		strncpy(szIMEI, szIMEITemp, 4);
		strncpy(szDieID, szDieIDTemp, 5);
		long temp = (atoi(szIMEI) + atoi(szDieID)) * 9578;

		char szPasswordTemp[MAX_PATH];
		memset(szPasswordTemp, 0, sizeof(szPasswordTemp));
		for (int i = 0; i < 6; i++) {
			szPasswordTemp[i] = (char)((temp % 10) + 48);
			temp /= 10;
		}
		memcpy(out_buf , szPasswordTemp , sizeof(szPasswordTemp));
	}

	return nRetCode;
}

//FF: 1019
NEPDLLAPI int __stdcall P21EnableDTR(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[port] == NULL) {
		theApp.m_pCmdCtrl[port] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[port]->EnableDTR(port, timeout_ms) ;
	if (nRetCode != HS_OK) {
		nRetCode += (port <= QTH_PORT_COM_MAX) ? ERR_TYPE_UART : ERR_TYPE_USB;
	}

	return nRetCode;
}

//FF: 1019
NEPDLLAPI int __stdcall P21DisableDTR(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[port] == NULL) {
		theApp.m_pCmdCtrl[port] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[port]->DisableDTR(port, timeout_ms) ;
	if (nRetCode != HS_OK) {
		nRetCode += (port <= QTH_PORT_COM_MAX) ? ERR_TYPE_UART : ERR_TYPE_USB;
	}

	return nRetCode;

}

//add by FF 1023
//power on mode for UART
//send ASCII 'A' for 5 second
//recieve zzz is success
NEPDLLAPI int __stdcall P21PowerOnToUARTPacketMode(int port, int timeout_ms, int DTRFlag)
{
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[port] == NULL) {
		theApp.m_pCmdCtrl[port] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[port]->NormalCmdPowerOnToUARTPacketMode(port, timeout_ms, DTRFlag);
	if (nRetCode != HS_OK) {
		nRetCode += (port <= QTH_PORT_COM_MAX) ? ERR_TYPE_UART : ERR_TYPE_USB;
	}
	return nRetCode;
}

//FF: 1025
NEPDLLAPI int __stdcall P21GetQisdaAndroidCmdCtrlDLLVersion(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	char Command[6] = {'A', '1', '.', '0', '2', '1'};
	memcpy(out_buf, Command, sizeof(Command));
	return HS_OK;
}

//FF: 1026
NEPDLLAPI int __stdcall P21UARTPacketMode(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_CHANGEMODE,5,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: 1026
NEPDLLAPI int __stdcall P21USBPacketMode(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_CHANGEMODE,6,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: 1026
NEPDLLAPI int __stdcall P21GetQisdaAndroidCmdCtrlDLLSubVersion(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	CString cstrVersion;
	CString cstrMainVersion;
	CString cstrVersion2;
	char cSubVer[64];
	memset(cSubVer, 0, sizeof(cSubVer));

	cstrVersion.Format("%s", __DATE__);
	int nRetCode = PreDefMacroToDigitString((char*)(LPCTSTR)cstrVersion, cSubVer, PREDEF_DATE);
	cstrVersion = CString(cSubVer);
	memset(cSubVer, 0, sizeof(cSubVer));

	cstrVersion2.Format("%s", __TIME__);
	nRetCode = PreDefMacroToDigitString((char*)(LPCTSTR)cstrVersion2, cSubVer, PREDEF_TIME);
	cstrVersion = cstrVersion + "." + CString(cSubVer);
	memcpy(out_buf, (LPCTSTR)cstrVersion, cstrVersion.GetLength());

	return HS_OK;
}

//FF: change to Engineer mode 1026
NEPDLLAPI int __stdcall P21EngineerMode(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_CHANGEMODE,1,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: change to 301 mode 1026
NEPDLLAPI int __stdcall P21301Mode(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_CHANGEMODE,2,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: change to uart console mode 1026
NEPDLLAPI int __stdcall P21UARTConsoleMode(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_CHANGEMODE,3,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: change to usb console mode 1026
NEPDLLAPI int __stdcall P21USBConsoleMode(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_CHANGEMODE,4,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: enable LCD log 1026
NEPDLLAPI int __stdcall P21LCDLogEnable(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_CHANGEMODE,7,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: enable uart log 1026
NEPDLLAPI int __stdcall P21UARTLogEnable(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_CHANGEMODE,8,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: Format Mdoc(8M,2M) for LPR2 use only 1030
NEPDLLAPI int __stdcall P21FlashFormatBDTL8M2M(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FLASH,13,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: run necleus os 1030
NEPDLLAPI int __stdcall P21OSRunNecleusOS(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_RUNOS,5,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: Wlan Power On 1030
NEPDLLAPI int __stdcall P21WLANPowerOn(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_WLAN,1,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: Wlan Power Off 1030
NEPDLLAPI int __stdcall P21WLANPowerOff(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_WLAN,2,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: setWlanTestMode 1030
NEPDLLAPI int __stdcall P21WLANSetWlanTestMode(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_WLAN,3,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: Tx Local frequency 1030
NEPDLLAPI int __stdcall P21WLANSetCmdLocalFrequecy(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_WLAN,4,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

//FF: Tx Output power 1030
NEPDLLAPI int __stdcall P21WLANSetCmdOutputPower(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_WLAN,5,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

//FF: Write MAC Address 1030
NEPDLLAPI int __stdcall P21WLANSetCmdMACAddrToEEProm(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_WLAN,7,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

//FF: Read MAC Address 1030
NEPDLLAPI int __stdcall P21WLANGetCmdMACAddrFromEEProm(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_WLAN,8,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: Get Wanla Chip ID 1030
NEPDLLAPI int __stdcall P21WLANGetMTKChipID(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_WLAN,9,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: Get Rssi 1030
NEPDLLAPI int __stdcall P21WLANGetCmdPacketRxStatus(int port, int timeout_ms, char* in_buf, size_t i_InLength, char* out_buf)
{
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[port] == NULL) {
		theApp.m_pCmdCtrl[port] = new CmdCtrl;
	}
	byte dt_cmd[4] = {P21_DT_WLAN,6,0,0};
	int nRetCode = theApp.m_pCmdCtrl[port]->GeneralCmdInOut(port, timeout_ms, dt_cmd, in_buf, i_InLength, out_buf);
	if (nRetCode != HS_OK) {
		nRetCode += (port <= QTH_PORT_COM_MAX) ? ERR_TYPE_UART : ERR_TYPE_USB;
	}

	return nRetCode;
}

//////////////////camera
//FF: camera power On 1106
NEPDLLAPI int __stdcall P21CameraPowerOn(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_CAMERA,1,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: camera power Off 1106
NEPDLLAPI int __stdcall P21CameraPowerOff(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_CAMERA,2,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: Read MDoc ID 1106
NEPDLLAPI int __stdcall P21FlashReadMDocID(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FLASH,46,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

/////////SDRAM
//FF: SDRAM read write full 1106
NEPDLLAPI int __stdcall P21SDRAMReadWriteFull(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_SDRAM,1,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: SDRAM read write partial 1106
NEPDLLAPI int __stdcall P21SDRAMReadWritePartial(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_SDRAM,2,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}


/////////batery
//FF: ADCIN 1 1106
NEPDLLAPI int __stdcall P21HWADCIN1(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_HARDWARE,P21_DT_HARDWARE_ADC,1,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: ADCIN 2 1106
NEPDLLAPI int __stdcall P21HWADCIN2(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_HARDWARE,P21_DT_HARDWARE_ADC,2,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: ADCIN 3 1106
NEPDLLAPI int __stdcall P21HWADCIN3(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_HARDWARE,P21_DT_HARDWARE_ADC,3,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}


//FF: ADCIN 4 1106
NEPDLLAPI int __stdcall P21HWADCIN4(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_HARDWARE,P21_DT_HARDWARE_ADC,4,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: ADCIN 5 1106
NEPDLLAPI int __stdcall P21HWADCIN5(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_HARDWARE,P21_DT_HARDWARE_ADC,5,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: ADCIN 6 1106
NEPDLLAPI int __stdcall P21HWADCIN6(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_HARDWARE,P21_DT_HARDWARE_ADC,6,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: ADCIN 7 1106
NEPDLLAPI int __stdcall P21HWADCIN7(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_HARDWARE,P21_DT_HARDWARE_ADC,7,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: ADCIN 8 1106
NEPDLLAPI int __stdcall P21HWADCIN8(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_HARDWARE,P21_DT_HARDWARE_ADC,8,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: ADCIN 9 1106
NEPDLLAPI int __stdcall P21HWADCIN9(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_HARDWARE,P21_DT_HARDWARE_ADC,9,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: ADCIN 10 1106
NEPDLLAPI int __stdcall P21HWADCIN10(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_HARDWARE,P21_DT_HARDWARE_ADC,10,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: ADCIN 11 1106
NEPDLLAPI int __stdcall P21HWADCIN11(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_HARDWARE,P21_DT_HARDWARE_ADC,11,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//OS related
//FF: P21 power off 0109 modify
NEPDLLAPI int __stdcall P21OSPowerOff(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_RUNOS,3,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

//FF: warch dog reset 0109 modify
NEPDLLAPI int __stdcall P21OSWatchDogReset(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_RUNOS,4,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

//FF: write factory area 1106
NEPDLLAPI int __stdcall P21FAWriteFactoryArea(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FACTORYAREA,34,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

//FF: read factory area 1106
NEPDLLAPI int __stdcall P21FAReadFactoryArea(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FACTORYAREA,33,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//YTT: write LocalSetting
NEPDLLAPI int __stdcall P21FAWriteCountryCode(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FACTORYAREA,52,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

//YTT: read LocalSetting 1106
NEPDLLAPI int __stdcall P21FAReadCountryCode(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	memset(out_buf, 0, sizeof(out_buf));
	byte dt_cmd[4] = {P21_DT_FACTORYAREA,51,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//for  P21FAWriteMEPDSimLockFile
int load_file_to_memory(const char * filename, char** result) //BenQ Mark Hsieh + Dec.22.2006
{
	unsigned int size = 0;
	FILE* fp = fopen(filename, "rb");
	if (fp == NULL) {
		*result = NULL;
		return -1; // -1 means file opening fail
	}
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	*result = (char*)malloc(size + 1);
	if (size != fread(*result, sizeof(char), size, fp)) {
		free(*result);
		return -2; // -2 means file reading fail
	}
	fclose(fp);
	(*result)[size] = '\0';
	return size;
}

//for  P21FAWriteMEPDSimLockFile
int write_file_from_memory(const char * filename, char* buf, int size)
{
	FILE* fp = fopen(filename, "wb");
	if (fp == NULL) {
		return -1; // -1 means file opening fail
	}

	int nByteWritten = (int)fwrite(buf, sizeof(char), size, fp);
	fclose(fp);

	return nByteWritten;
}

//FF: erase necleus GSMFFS 1107
NEPDLLAPI int __stdcall P21OSEraseNucleusGSMFFS(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_RUNOS,7,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: Format Mdoc(5M,60M) 1226
NEPDLLAPI int __stdcall P21FlashFormatBDTL5M60M(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FLASH,8,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: write IMEI 1205
NEPDLLAPI int __stdcall P21FAWriteIMEI(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FACTORYAREA,38,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

//FF: Read IMEI 1205
NEPDLLAPI int __stdcall P21FAReadIMEI(int port, int timeout_ms, char* cImei)
{
	TRACER_FUNCTION_IN();
	TRACER_STREAM("[Port:" << port << "] => IMEI: " << cImei << "\r\n");

	byte dt_cmd[4] = {P21_DT_FACTORYAREA,37,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, cImei);
}

// Mark Hsieh 2007.Dec.4th BEGIN
NEPDLLAPI int __stdcall P21FAVBATCalibration(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FACTORYAREA,60,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}
// Mark Hsieh 2007.Dec.4th END

NEPDLLAPI int __stdcall P21FAWriteVBATOffset(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FACTORYAREA,50,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

NEPDLLAPI int __stdcall P21FAReadVBATOffset(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FACTORYAREA,49,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

// 2007-06-11 YTT Write IMEI NetworkLock Key to OTP
NEPDLLAPI int __stdcall P21FAWriteIMEIAndNLockKey(int port, int timeout_ms, char* in_buf, char * cLockKey)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FACTORYAREA,42,0,0};
	char cin_buf[32];
	memset(cin_buf, 0, sizeof(cin_buf));
	memcpy(cin_buf, in_buf, 16); //16 for imei and 4 for network key
	memcpy(cin_buf + 16, cLockKey, 4); //repeat copy network lock key to byte 21-24
	memcpy(cin_buf + 20, cLockKey, 4); //repeat copy network lock key to byte 21-24
	return GeneralCmdIn(port, timeout_ms, dt_cmd, cin_buf, strlen(in_buf));
}

// 2007-06-11 YTT Read IMEI NetworkLock Key to OTP
NEPDLLAPI int __stdcall P21FAReadIMEIAndNLockKey(int port, int timeout_ms, char* cImei, char* cLockKey)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FACTORYAREA,41,0,0};

	char cout_buf[256];
	memset(cout_buf, 0, sizeof(cout_buf));

	int nRetCode = GeneralCmdOut(port, timeout_ms, dt_cmd, cout_buf);

	memcpy(cImei, cout_buf, 16);
	memcpy(cLockKey, cout_buf + 16, 4);
	//check if key 0-3  equals to key 4-7
	for (int i = 0; i < 4; i++) {
		if (cout_buf[16+i] != cout_buf[20+i] ) {
			return nRetCode + 1 ;//for special check
		}
	}

	return nRetCode;
}

//FF: LCD Off 1221
NEPDLLAPI int __stdcall P21LCDOff(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LCD,2,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: LCD show yellow 1221
NEPDLLAPI int __stdcall P21LCDYellow(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LCD,P21_DT_LCD_COLORTEST,6,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: LCD show Magenta 1221
NEPDLLAPI int __stdcall P21LCDMagenta(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LCD,P21_DT_LCD_COLORTEST,7,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: LCD show Cyan 1221
NEPDLLAPI int __stdcall P21LCDCyan(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LCD,P21_DT_LCD_COLORTEST,8,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: LCD backlight test 1221
NEPDLLAPI int __stdcall P21LCDBacklightTest(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LCD,6,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: LCD Special Ppattern 1221
NEPDLLAPI int __stdcall P21LCDSpecialPpattern1(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LCD,P21_DT_LCD_SpecialPpattern,1,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: LCD Special Ppattern 1221
NEPDLLAPI int __stdcall P21LCDSpecialPpattern2(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LCD,P21_DT_LCD_SpecialPpattern,2,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: LCD Special Ppattern 1221
NEPDLLAPI int __stdcall P21LCDSpecialPpattern3(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LCD,P21_DT_LCD_SpecialPpattern,3,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: LCD Special Ppattern 1221
NEPDLLAPI int __stdcall P21LCDSpecialPpattern4(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LCD,P21_DT_LCD_SpecialPpattern,4,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: LCD Special Ppattern 1221
NEPDLLAPI int __stdcall P21LCDSpecialPpattern5(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LCD,P21_DT_LCD_SpecialPpattern,5,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: LCD Special Ppattern 1221
NEPDLLAPI int __stdcall P21LCDSpecialPpattern6(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LCD,P21_DT_LCD_SpecialPpattern,6,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: LCD Special Ppattern 1221
NEPDLLAPI int __stdcall P21LCDSpecialPpattern7(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LCD,P21_DT_LCD_SpecialPpattern,7,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: LCD Special Ppattern 1221
NEPDLLAPI int __stdcall P21LCDSpecialPpattern8(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LCD,P21_DT_LCD_SpecialPpattern,8,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: LCD Special Ppattern 1221
NEPDLLAPI int __stdcall P21LCDSpecialPpattern9(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LCD,P21_DT_LCD_SpecialPpattern,9,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}
//FF: LCD ID 1221
NEPDLLAPI int __stdcall P21LCDGetID(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LCD,9,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: Audio Tone to LS 1221
NEPDLLAPI int __stdcall P21AudioToneToLS(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_AUDIO,8,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}
//FF: get sim ID 1221
NEPDLLAPI int __stdcall P21ModemGetSIMInfo(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_MODEM,3,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF:BT on 1221
NEPDLLAPI int __stdcall P21BTOn(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_BT,1,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF:BT off 1221
NEPDLLAPI int __stdcall P21BTOff(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_BT,2,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF:BT Test 1221
NEPDLLAPI int __stdcall P21BTTest(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_BT,3,0,0};
	return GeneralCmdOut(port, 20000, dt_cmd, out_buf);
}

//FF:BT relay mode 1221
NEPDLLAPI int __stdcall P21BTRelayMode(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_BT,4,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF:BT disconnect 1221
NEPDLLAPI int __stdcall P21BTDisconnect(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_BT,6,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF:BT audio on 1221
NEPDLLAPI int __stdcall P21BTAudioOn(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_BT,7,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: get BT ID 1221
NEPDLLAPI int __stdcall P21BTGetID(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_BT,9,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

NEPDLLAPI int __stdcall P21BTRFTest(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_BT,12,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

NEPDLLAPI int __stdcall P21BTFMGetRSSILevel(int port, int timeout_ms, char* in_buf, size_t i_InLength, char* out_buf)
{
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[port] == NULL) {
		theApp.m_pCmdCtrl[port] = new CmdCtrl;
	}
	byte dt_cmd[4] = {P21_DT_BT,13,0,0};
	int nRetCode = theApp.m_pCmdCtrl[port]->GeneralCmdInOut(port, timeout_ms, dt_cmd, in_buf, i_InLength, out_buf);
	if (nRetCode != HS_OK) {
		nRetCode += (port <= QTH_PORT_COM_MAX) ? ERR_TYPE_UART : ERR_TYPE_USB;
	}

	return nRetCode;
}

//FF: SDRAM line test 1221
NEPDLLAPI int __stdcall P21SDRAMLineTest(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_SDRAM,3,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: SDRAM ID 1221
NEPDLLAPI int __stdcall P21SDRAMGetID(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_SDRAM,9,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: Write clock 1221
NEPDLLAPI int __stdcall P21ClockWriteClock(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_CLOCK,4,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, in_buf);
}

//FF:RTC Read alarm
NEPDLLAPI int __stdcall P21ClockReadAlarm(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_CLOCK,5,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: Write alarm 1221
NEPDLLAPI int __stdcall P21ClockWriteAlarm(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_CLOCK,6,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

//FF:clock RTC on 1221
NEPDLLAPI int __stdcall P21ClockRTCAlarmPowerOn(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_CLOCK,7,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF:Charger CV On 1221
NEPDLLAPI int __stdcall P21ChargerCVOn(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_CHARGER,1,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

//FF:Charger CC On 1221
NEPDLLAPI int __stdcall P21ChargerCCOn(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_CHARGER,2,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

//FF:Charger off 1221
NEPDLLAPI int __stdcall P21ChargerOff(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_CHARGER,3,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF:Charge BKP battery On 1221
NEPDLLAPI int __stdcall P21ChargerBKPBatteryOn(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_CHARGER,4,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF:Charge BKP battery Off 1221
NEPDLLAPI int __stdcall P21ChargerBKPBatteryOff(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_CHARGER,5,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF:Charger status
NEPDLLAPI int __stdcall P21ChargerStatus(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_CHARGER,6,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF:Read OS version 1221
NEPDLLAPI int __stdcall P21VersionOSVersion(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_VERSION,4,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: read MDOC partition info
NEPDLLAPI int __stdcall P21FlashMDOCInfo(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FLASH,9,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: key pad 1227
NEPDLLAPI int __stdcall P21KeypadPress(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_KEYPAD,2,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: key pad 1227
NEPDLLAPI int __stdcall P21KeypadReadMatrix(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_KEYPAD,3,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: save necleus GSMFFS 0104
NEPDLLAPI int __stdcall P21OSNucleusSaveGSMFFS(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_RUNOS,6,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: jump ot OS 0109
//FF: DT load OS
NEPDLLAPI int __stdcall P21OSJumpToOS(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_RUNOS,1,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: write BT address to FA 0215
NEPDLLAPI int __stdcall P21FAWriteBTAddress(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FACTORYAREA,40,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

//FF: read BT address from FA 0215
NEPDLLAPI int __stdcall P21FAReadBTAddress(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FACTORYAREA,39,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: Audio Volume Up 0305
NEPDLLAPI int __stdcall P21AudioVULPG(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_AUDIO,13,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

//FF: Audio Volume Down  0305
NEPDLLAPI int __stdcall P21AudioVDLPG(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_AUDIO,14,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

//FF: write 301 flag 0308
NEPDLLAPI int __stdcall P21FAWrite301Flag(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[port] == NULL) {
		theApp.m_pCmdCtrl[port] = new CmdCtrl;
	}
	byte dt_cmd[4] = {P21_DT_FACTORYAREA,44,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

//FF: read 301 flag 0308
NEPDLLAPI int __stdcall P21FARead301Flag(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FACTORYAREA,43,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

//FF: Run PLDR 0313
//FF:DT set a flag to DDR, then load PLDR
NEPDLLAPI int __stdcall P21OSRunPLDR(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_RUNOS,9,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: write IMEI temp 0322
//FF: write IMEI to BDTL0
NEPDLLAPI int __stdcall P21FAWriteIMEITemp(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FACTORYAREA,46,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

//FF: Read IMEI temp 0322
//FF: Read IMEI from BDTL0
NEPDLLAPI int __stdcall P21FAReadIMEITemp(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FACTORYAREA,45,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

NEPDLLAPI int __stdcall P21FAWriteVersionInfo(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FACTORYAREA,36,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

NEPDLLAPI int __stdcall P21FAReadVersionInfo(int port, int timeout_ms, char* in_buf, size_t i_InLength, char* out_buf)
{
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[port] == NULL) {
		theApp.m_pCmdCtrl[port] = new CmdCtrl;
	}
	byte dt_cmd[4] = {P21_DT_FACTORYAREA,35,0,0};
	int nRetCode = theApp.m_pCmdCtrl[port]->GeneralCmdInOut(port, timeout_ms, dt_cmd, in_buf, i_InLength, out_buf);
	if (nRetCode != HS_OK) {
		nRetCode += (port <= QTH_PORT_COM_MAX) ? ERR_TYPE_UART : ERR_TYPE_USB;
	}
	return nRetCode;
}

NEPDLLAPI int __stdcall P21FlashFormatAllPCAssign(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FLASH,44,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

//FF: format BDTL by PC assign parameter 0322
NEPDLLAPI int __stdcall P21FlashFormatBDTLPCAssign(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FLASH,45,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

//FF:0327
NEPDLLAPI int __stdcall P21Sidekey1LEDBlink(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_LED,13,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: copy clibration data from BDTL0 to BDK1:Bin3 (FA)
//FF: but DT will not check that calibration data is on BDTL0 or not
NEPDLLAPI int __stdcall P21FABackupGSMFFSToFA(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FACTORYAREA,47,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: copy clibration data from BDK1:Bin3 (FA) to BDTL0
//FF: but DT will not check that calibration data is on FA or not
NEPDLLAPI int __stdcall P21FARestoreGSMFFSFromFA(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FACTORYAREA,48,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//FF: check protect 0426
//FF:check protect statuse of flash
NEPDLLAPI int __stdcall P21FlashCheckProtect(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FLASH,60,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

NEPDLLAPI int __stdcall P21DLCheckServicePasswd(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_DOWNLOAD,35,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

NEPDLLAPI int __stdcall P21DLVerifyChecksumFromFlash(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_DOWNLOAD,37,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

NEPDLLAPI int __stdcall P21I2CTest(int port, int timeout_ms, char* in_buf, size_t i_InLength, char* out_buf)
{
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[port] == NULL) {
		theApp.m_pCmdCtrl[port] = new CmdCtrl;
	}
	byte dt_cmd[4] = {P21_DT_HARDWARE,5,0,0};
	int nRetCode = theApp.m_pCmdCtrl[port]->GeneralCmdInOut(port, timeout_ms, dt_cmd, in_buf, i_InLength, out_buf);
	if (nRetCode != HS_OK) {
		nRetCode += (port <= QTH_PORT_COM_MAX) ? ERR_TYPE_UART : ERR_TYPE_USB;
	}

	return nRetCode;
}

NEPDLLAPI int __stdcall GetMEPDUnlockCode(unsigned char *szUnlockCode, const unsigned char *mepd, unsigned int length, unsigned int easyDecode, char *FSN, char *IMEI)
{
	TRACER_FUNCTION_IN();

	// 2010-06-21 Evonne: load SimEncoder.dll
	typedef UINT (__stdcall *LPDLLFUNC)(unsigned char *, const unsigned char *, unsigned int, unsigned int, char *, char *);
	LPDLLFUNC lpfnDllFunc = NULL;
	HMODULE m_hmLib = ::LoadLibrary("SimEncoder.dll");

	if(m_hmLib) {
		lpfnDllFunc = (LPDLLFUNC)::GetProcAddress(m_hmLib, "GetUnlockCodeFromMEPD");

		if (!lpfnDllFunc) {
			AfxMessageBox("Function not found in DLL");
			FreeLibrary(m_hmLib);
			return HS_FAIL;
		}
		lpfnDllFunc(szUnlockCode, mepd, length, 0, FSN, IMEI);

	} else {
		CString msg = _T("Load: SimEncoder.dll Fail!");
		AfxMessageBox(msg);
	}

	for (int i = 0; i < 8; i++) {
		if (!isdigit(szUnlockCode[i])) {
			TRACER_STREAM("UnlockCode digit[" << i << "] is not a number!\r\n");
			return HS_FAIL;
		}
	}

	// 2010-06-21 Evonne: free library
	if (m_hmLib) {
		::FreeLibrary(m_hmLib);
		m_hmLib = NULL;
	}

	return HS_OK;
}

NEPDLLAPI int __stdcall P21FlashExit(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_FLASH,64,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

//// Just Test
NEPDLLAPI int __stdcall P21GenCMDOutTEST(int port, int timeout_ms, char* out_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_DOWNLOAD,4,0,0};
	return GeneralCmdOut(port, timeout_ms, dt_cmd, out_buf);
}

NEPDLLAPI int __stdcall P21GenCMDInTEST(int port, int timeout_ms, char* in_buf)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_DOWNLOAD,4,0,0};
	return GeneralCmdIn(port, timeout_ms, dt_cmd, in_buf, strlen(in_buf));
}

NEPDLLAPI int __stdcall P21GenCMDTEST(int port, int timeout_ms)
{
	TRACER_FUNCTION_IN();

	byte dt_cmd[4] = {P21_DT_DOWNLOAD,61,0,0};
	return GeneralCmd(port, timeout_ms, dt_cmd);
}

NEPDLLAPI int __stdcall P21GenCMDInOutTEST(int port, int timeout_ms, char* in_buf, size_t i_InLength, char* out_buf)
{
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[port] == NULL) {
		theApp.m_pCmdCtrl[port] = new CmdCtrl;
	}
	byte dt_cmd[4] = {P21_DT_DOWNLOAD,4,0,0};
	int nRetCode = theApp.m_pCmdCtrl[port]->GeneralCmdInOut(port, timeout_ms, dt_cmd, in_buf, i_InLength, out_buf);
	if (nRetCode != HS_OK) {
		nRetCode += (port <= QTH_PORT_COM_MAX) ? ERR_TYPE_UART : ERR_TYPE_USB;
	}
	return nRetCode;
}
