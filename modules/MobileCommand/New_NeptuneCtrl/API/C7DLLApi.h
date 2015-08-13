#include "DLLDefine.h"

static const int NV_DT_CMD_MAX_RETRY = 2;

/******************************************************************************
 * Export/import function prototypes
 ******************************************************************************/
NEPDLLAPI bool __stdcall C7_ExecAdbOut(CString Command, char* output, char* ErrorCode);
NEPDLLAPI bool C7_ExecAdbOut_SPU(char* Command, char* output, char* ErrorCode);

NEPDLLAPI int __stdcall C7_TestFtdMode(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall C7_AutoFtdMode(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int C7_AutoFtdMode_SPU(char* Port, char* Timeout, char* ErrorCode);
NEPDLLAPI int C7_TestFtdMode_SPU(char* Port, char* Timeout, char* ErrorCode);  

// FTD Communication Commands
//01:battery
NEPDLLAPI int __stdcall C7_BATTcoincellVoltage(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 1
NEPDLLAPI int __stdcall C7_BATTcoincellChargeOn(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 2
NEPDLLAPI int __stdcall C7_BATTrtcTime(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);		// cmd: 3

//02:power
NEPDLLAPI int __stdcall C7_PWRgaugeID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall C7_PWRgaugeVoltage(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall C7_PWRgaugeCurrent(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall C7_PWRgaugeTemperature(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall C7_PWRchargerID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall C7_PWRchargerOn(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall C7_DFIupgrade(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int C7_DFIenable_SPU(char* Port, char* Timeout, char* InBuffer, char* OutBuffer);


//03:memory
NEPDLLAPI int __stdcall C7_MCPflashID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall C7_MCPddrID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);


//04:LCM - LCD & Touch
NEPDLLAPI int __stdcall C7_TOUCHpoint(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:1
NEPDLLAPI int __stdcall C7_TOUCHtest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:2
NEPDLLAPI int __stdcall C7_TOUCHid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:3
NEPDLLAPI int __stdcall C7_TOUCHversion(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:3
NEPDLLAPI int __stdcall C7_LCDonoff(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:3
NEPDLLAPI int __stdcall C7_TOUCHSelfTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:6


//05:audio
NEPDLLAPI int __stdcall C7_AUpathSwitch(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:1
NEPDLLAPI int __stdcall C7_AUplaymp3(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:2
NEPDLLAPI int __stdcall C7_AUrecordWAV(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:3
NEPDLLAPI int __stdcall C7_AUplayWAV(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:2


//06:camera
NEPDLLAPI int __stdcall C7_CameraID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:1
NEPDLLAPI int __stdcall C7_CAMshot(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:2
NEPDLLAPI int __stdcall C7_CAMstart(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:3
NEPDLLAPI int __stdcall C7_CAMindexReset(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:3

//07:keypad
NEPDLLAPI int __stdcall C7_KPtouchTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:2

// 13-BT
NEPDLLAPI int __stdcall C7_BTid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall C7_BTTestMode(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall C7_BTTXPower(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);

//14:WLAN
NEPDLLAPI int __stdcall C7_WLAN_Mode(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);   // cmd: 1
NEPDLLAPI int __stdcall C7_WLAN_TXPower(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 2
NEPDLLAPI int __stdcall C7_WLAN_RXrssi(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 3
NEPDLLAPI int __stdcall C7_WLAN_RXset(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 4
NEPDLLAPI int __stdcall C7_WLAN_LoadFABin(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 5
NEPDLLAPI int __stdcall C7_WLAN_LoadNextBin(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 6
NEPDLLAPI int __stdcall C7_WLAN_SaveBin(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 7

//15:GPS
NEPDLLAPI int __stdcall C7_GPSValueCN0(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);

//16:FMR
NEPDLLAPI int __stdcall C7_FMRrssi(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);


//17:NFC
NEPDLLAPI int __stdcall C7_NFCid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall C7_NFCselftest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);


//18:Sensor
NEPDLLAPI int __stdcall C7_SNSRgsensorID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); //cmd:1
NEPDLLAPI int __stdcall C7_SNSRgsensorAcc(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:2
NEPDLLAPI int __stdcall C7_SNSRgsensorCalib(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:3
NEPDLLAPI int __stdcall C7_SNSRecompassID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:4
NEPDLLAPI int __stdcall C7_SNSRecompassSelftest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:30

NEPDLLAPI int __stdcall C7_SNSRlsensorID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);   //cmd:7
NEPDLLAPI int __stdcall C7_SNSRlsensorValue(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:8
NEPDLLAPI int __stdcall C7_SNSRlsensorCalib(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:9
NEPDLLAPI int __stdcall C7_SNSRpsensorID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);   //cmd:10
NEPDLLAPI int __stdcall C7_SNSRpsensorValue(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:11
NEPDLLAPI int __stdcall C7_SNSRpsensorCalib(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:12
NEPDLLAPI int __stdcall C7_SNSRloadFALPkdata(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:13


//20:LED
NEPDLLAPI int __stdcall C7_LEDid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);


//21:ID
NEPDLLAPI int __stdcall C7_IDcpu(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:1
NEPDLLAPI int __stdcall C7_BoardADC(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:2
NEPDLLAPI int __stdcall C7_BoardRev(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:3


//90:OS
NEPDLLAPI int __stdcall C7_OSgetprop(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:1
NEPDLLAPI int __stdcall C7_OSpwr2home(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:2
NEPDLLAPI int __stdcall C7_OSkeyBack(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); //cmd:3
NEPDLLAPI int __stdcall C7_OSkeyFocus(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:4
NEPDLLAPI int __stdcall C7_OSkeyCamera(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:5
NEPDLLAPI int __stdcall C7_OSmodeSleep(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:5


//91:version
NEPDLLAPI int __stdcall C7_VERftd(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall C7_VERchange(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);


//92:factory
NEPDLLAPI int __stdcall C7_FAC_FARead(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); //cmd:1
NEPDLLAPI int C7_FAC_FARead_SPU(char* Port, char* Timeout, char* InBuffer, char* OutBuffer, char* ErrorCode); //cmd:1
NEPDLLAPI int __stdcall C7_FAC_FAWrite(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:2
NEPDLLAPI int __stdcall C7_FAC_Picasso(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:3

NEPDLLAPI int __stdcall C7_FAC_WifiDataPath(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:5
NEPDLLAPI int __stdcall C7_FAC_FactoryReset(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:5
NEPDLLAPI int C7_FAC_FactoryReset_SPU(char* Port, char* Timeout, char* InBuffer, char* OutBuffer, char* ErrorCode);


//92:factory special
NEPDLLAPI int __stdcall C7_FAC_FARead301(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall C7_FAC_FARead303(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall C7_FAC_FARead304(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);

//92.
NEPDLLAPI int __stdcall C7_FAC_ClearNFCVariable(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
