#include "DLLDefine.h"

static const int NV_DT_CMD_MAX_RETRY = 2;

/******************************************************************************
 * Export/import function prototypes
 ******************************************************************************/
NEPDLLAPI bool __stdcall C7_ExecAdbOut(CString command, char* output, char* error_msg);
NEPDLLAPI bool C7_ExecAdbOut_SPU(char* Command, char* output, char* error_code);

NEPDLLAPI int __stdcall C7_TestFtdMode(int port, int timeout_ms, char* error_msg);
NEPDLLAPI int __stdcall C7_AutoFtdMode(int port, int timeout_ms, char* error_msg);
NEPDLLAPI int C7_AutoFtdMode_SPU(char* Port, char* Timeout, char* error_code);
NEPDLLAPI int C7_TestFtdMode_SPU(char* Port, char* Timeout, char* error_code);

// FTD Communication Commands
//01:battery
NEPDLLAPI int __stdcall C7_BATTcoincellVoltage(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 1
NEPDLLAPI int __stdcall C7_BATTcoincellChargeOn(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 2
NEPDLLAPI int __stdcall C7_BATTrtcTime(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 3

//02:power
NEPDLLAPI int __stdcall C7_PWRgaugeID(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall C7_PWRgaugeVoltage(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall C7_PWRgaugeCurrent(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall C7_PWRgaugeTemperature(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall C7_PWRchargerID(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall C7_PWRchargerOn(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall C7_DFIupgrade(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int C7_DFIenable_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf);

//03:memory
NEPDLLAPI int __stdcall C7_MCPflashID(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall C7_MCPddrID(int port, int timeout_ms, char* in_buf, char* out_buf);

//04:LCM - LCD & Touch
NEPDLLAPI int __stdcall C7_TOUCHpoint(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:1
NEPDLLAPI int __stdcall C7_TOUCHtest(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall C7_TOUCHid(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall C7_TOUCHversion(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall C7_LCDonoff(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall C7_TOUCHSelfTest(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:6

//05:audio
NEPDLLAPI int __stdcall C7_AUpathSwitch(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:1
NEPDLLAPI int __stdcall C7_AUplaymp3(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall C7_AUrecordWAV(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall C7_AUplayWAV(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2

//06:camera
NEPDLLAPI int __stdcall C7_CameraID(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:1
NEPDLLAPI int __stdcall C7_CAMshot(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall C7_CAMstart(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall C7_CAMindexReset(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3

//07:keypad
NEPDLLAPI int __stdcall C7_KPtouchTest(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2

// 13-BT
NEPDLLAPI int __stdcall C7_BTid(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall C7_BTTestMode(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall C7_BTTXPower(int port, int timeout_ms, char* in_buf, char* out_buf);

//14:WLAN
NEPDLLAPI int __stdcall C7_WLAN_Mode(int port, int timeout_ms, char* in_buf, char* out_buf);   // cmd: 1
NEPDLLAPI int __stdcall C7_WLAN_TXPower(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 2
NEPDLLAPI int __stdcall C7_WLAN_RXrssi(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 3
NEPDLLAPI int __stdcall C7_WLAN_RXset(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 4
NEPDLLAPI int __stdcall C7_WLAN_LoadFABin(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 5
NEPDLLAPI int __stdcall C7_WLAN_LoadNextBin(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 6
NEPDLLAPI int __stdcall C7_WLAN_SaveBin(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 7

//15:GPS
NEPDLLAPI int __stdcall C7_GPSValueCN0(int port, int timeout_ms, char* in_buf, char* out_buf);

//16:FMR
NEPDLLAPI int __stdcall C7_FMRrssi(int port, int timeout_ms, char* in_buf, char* out_buf);

//17:NFC
NEPDLLAPI int __stdcall C7_NFCid(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall C7_NFCselftest(int port, int timeout_ms, char* in_buf, char* out_buf);

//18:Sensor
NEPDLLAPI int __stdcall C7_SNSRgsensorID(int port, int timeout_ms, char* in_buf, char* out_buf); //cmd:1
NEPDLLAPI int __stdcall C7_SNSRgsensorAcc(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall C7_SNSRgsensorCalib(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall C7_SNSRecompassID(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:4
NEPDLLAPI int __stdcall C7_SNSRecompassSelftest(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:30

NEPDLLAPI int __stdcall C7_SNSRlsensorID(int port, int timeout_ms, char* in_buf, char* out_buf);   //cmd:7
NEPDLLAPI int __stdcall C7_SNSRlsensorValue(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:8
NEPDLLAPI int __stdcall C7_SNSRlsensorCalib(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:9
NEPDLLAPI int __stdcall C7_SNSRpsensorID(int port, int timeout_ms, char* in_buf, char* out_buf);   //cmd:10
NEPDLLAPI int __stdcall C7_SNSRpsensorValue(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:11
NEPDLLAPI int __stdcall C7_SNSRpsensorCalib(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:12
NEPDLLAPI int __stdcall C7_SNSRloadFALPkdata(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:13

//20:LED
NEPDLLAPI int __stdcall C7_LEDid(int port, int timeout_ms, char* in_buf, char* out_buf);

//21:ID
NEPDLLAPI int __stdcall C7_IDcpu(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:1
NEPDLLAPI int __stdcall C7_BoardADC(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall C7_BoardRev(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3

//90:OS
NEPDLLAPI int __stdcall C7_OSgetprop(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:1
NEPDLLAPI int __stdcall C7_OSpwr2home(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall C7_OSkeyBack(int port, int timeout_ms, char* in_buf, char* out_buf); //cmd:3
NEPDLLAPI int __stdcall C7_OSkeyFocus(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:4
NEPDLLAPI int __stdcall C7_OSkeyCamera(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5
NEPDLLAPI int __stdcall C7_OSmodeSleep(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5

//91:version
NEPDLLAPI int __stdcall C7_VERftd(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall C7_VERchange(int port, int timeout_ms, char* in_buf, char* out_buf);

//92:factory
NEPDLLAPI int __stdcall C7_FAC_FARead(int port, int timeout_ms, char* in_buf, char* out_buf); //cmd:1
NEPDLLAPI int C7_FAC_FARead_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code); //cmd:1
NEPDLLAPI int __stdcall C7_FAC_FAWrite(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall C7_FAC_Picasso(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3

NEPDLLAPI int __stdcall C7_FAC_WifiDataPath(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5
NEPDLLAPI int __stdcall C7_FAC_FactoryReset(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5
NEPDLLAPI int C7_FAC_FactoryReset_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code);

//92:factory special
NEPDLLAPI int __stdcall C7_FAC_FARead301(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall C7_FAC_FARead303(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall C7_FAC_FARead304(int port, int timeout_ms, char* in_buf, char* out_buf);

//92.
NEPDLLAPI int __stdcall C7_FAC_ClearNFCVariable(int port, int timeout_ms, char* in_buf, char* out_buf);
