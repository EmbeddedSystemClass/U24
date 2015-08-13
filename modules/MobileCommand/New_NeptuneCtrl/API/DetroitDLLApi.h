#include "DLLDefine.h"

static const int NV_DT_CMD_MAX_RETRY = 2;

/******************************************************************************
 * Export/import function prototypes
 ******************************************************************************/
NEPDLLAPI bool __stdcall Detroit_ExecAdbOut(CString Command, char* output, char* ErrorCode);

NEPDLLAPI int __stdcall Detroit_TestFtdMode(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall Detroit_AutoFtdMode(int nPort, int nTimeout, char* ErrorCode);

// FTD Communication Commands
//01:battery
NEPDLLAPI int __stdcall Detroit_BATTcoincellVoltage(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 1
NEPDLLAPI int __stdcall Detroit_BATTcoincellChargeOn(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 2
NEPDLLAPI int __stdcall Detroit_BATTrtcTime(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);		// cmd: 3

//02:power
NEPDLLAPI int __stdcall Detroit_PWRgaugeID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); //cmd:1
NEPDLLAPI int __stdcall Detroit_PWRgaugeVoltage(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:2
NEPDLLAPI int __stdcall Detroit_PWRgaugeCurrent(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:3
NEPDLLAPI int __stdcall Detroit_PWRgaugeTemperature(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:4
NEPDLLAPI int __stdcall Detroit_PWRchargerID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:5
NEPDLLAPI int __stdcall Detroit_PWRchargerOn(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:6
NEPDLLAPI int __stdcall Detroit_DFIupgrade(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:7
NEPDLLAPI int __stdcall Detroit_ITenable(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:8
NEPDLLAPI int __stdcall Detroit_Regualator(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:9
NEPDLLAPI int __stdcall Detroit_PWRbattID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:10
NEPDLLAPI int __stdcall Detroit_PWRbattCapacity(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:12

//03:memory
NEPDLLAPI int __stdcall Detroit_MCPflashID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall Detroit_MCPddrID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall Detroit_MCPddrSize(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);

//04:LCM - LCD & Touch
NEPDLLAPI int __stdcall Detroit_TOUCHpoint(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:1
NEPDLLAPI int __stdcall Detroit_TOUCHtest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:2
NEPDLLAPI int __stdcall Detroit_TOUCHid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:3
NEPDLLAPI int __stdcall Detroit_TOUCHversion(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:4
NEPDLLAPI int __stdcall Detroit_TOUCHSelfTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:5
NEPDLLAPI int __stdcall Detroit_LCMbacklight(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:6
NEPDLLAPI int __stdcall Detroit_LCMbacklightColor(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:7
NEPDLLAPI int __stdcall Detroit_TouchModel(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:10


//05:audio
NEPDLLAPI int __stdcall Detroit_AUpathOut(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:1
NEPDLLAPI int __stdcall Detroit_AUplaymp3(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:2
NEPDLLAPI int __stdcall Detroit_AUrecordWAV(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:3
NEPDLLAPI int __stdcall Detroit_AUplayWAV(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:4
NEPDLLAPI int __stdcall Detroit_AUdetectHeadset(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:5
NEPDLLAPI int __stdcall Detroit_AUpathRec(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:6
NEPDLLAPI int __stdcall Detroit_AUloopbackTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:9
NEPDLLAPI int __stdcall Detroit_AUButtonCheck(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:10
NEPDLLAPI int __stdcall Detroit_AUinitADC(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:11
NEPDLLAPI int __stdcall Detroit_AUADCCali(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:12
NEPDLLAPI int __stdcall Detroit_AUheadsetInfo(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:13

//06:camera
NEPDLLAPI int __stdcall Detroit_CAMReadID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:1
NEPDLLAPI int __stdcall Detroit_CAMshot(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:2
NEPDLLAPI int __stdcall Detroit_CAMstart(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:3
NEPDLLAPI int __stdcall Detroit_CAMend(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:4
NEPDLLAPI int __stdcall Detroit_CAMFlashLED(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:5
NEPDLLAPI int __stdcall Detroit_CAMFlashLEDID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:6
NEPDLLAPI int __stdcall Detroit_CAM_OTP_AWB(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:7
NEPDLLAPI int __stdcall Detroit_CAM_OTP_AFMacro(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:8
NEPDLLAPI int __stdcall Detroit_CAM_OTP_AFInfinity(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:9
NEPDLLAPI int __stdcall Detroit_CAM_OTP_FrontFuseID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:10

//07:keypad
NEPDLLAPI int __stdcall Detroit_KeyTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:2

//08:SIM
NEPDLLAPI int __stdcall Detroit_SIMtest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:1
NEPDLLAPI int __stdcall Detroit_SIMdetect(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:2

//10:Vibrator
NEPDLLAPI int __stdcall Detroit_VibRun(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:1

//12:Modem
NEPDLLAPI int __stdcall Detroit_TxRxPower(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:1
NEPDLLAPI int __stdcall Detroit_QmiRfTxTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:3
NEPDLLAPI int __stdcall Detroit_QmiRfTxOff(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:4

//13-BT
NEPDLLAPI int __stdcall Detroit_BTid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall Detroit_BTTestMode(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall Detroit_BTTXPower(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall Detroit_BTRXrssi(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);

NEPDLLAPI int __stdcall Detroit_BTSelfTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall Detroit_BTLteTx(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall Detroit_BTLteRx(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);

//14:WLAN
NEPDLLAPI int __stdcall Detroit_WLAN_Mode(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);   // cmd: 1
NEPDLLAPI int __stdcall Detroit_WLAN_TXPower(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 2
NEPDLLAPI int __stdcall Detroit_WLAN_RXrssi(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 3
NEPDLLAPI int __stdcall Detroit_WLAN_RXset(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 4
NEPDLLAPI int __stdcall Detroit_WLAN_ScanAP(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 5
NEPDLLAPI int __stdcall Detroit_WLAN_ConnectAP(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 6
NEPDLLAPI int __stdcall Detroit_WLANid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 7

//15:GPS
NEPDLLAPI int __stdcall Detroit_GPSValueCN0(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);

//16:FMR
NEPDLLAPI int __stdcall Detroit_FMRrssi(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);// cmd: 1
NEPDLLAPI int __stdcall Detroit_FMRModuleOn(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);// cmd: 4

//17:NFC
NEPDLLAPI int __stdcall Detroit_NFCid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall Detroit_NFCFWVersion(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall Detroit_NFCAntennaSelftest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); //cmd:3
NEPDLLAPI int __stdcall Detroit_NFCSwpSelftest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); //cmd:4

//18:Sensor
NEPDLLAPI int __stdcall Detroit_SNSRgsensorID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); //cmd:1
NEPDLLAPI int __stdcall Detroit_SNSRgsensorAcc(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:2
NEPDLLAPI int __stdcall Detroit_SNSRgsensorCalib(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:3
NEPDLLAPI int __stdcall Detroit_SNSRecompassID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:4
NEPDLLAPI int __stdcall Detroit_SNSRecompassValue(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:5
NEPDLLAPI int __stdcall Detroit_SNSRecompassValue(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:4

NEPDLLAPI int __stdcall Detroit_SNSRlsensorID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);   //cmd:7
NEPDLLAPI int __stdcall Detroit_SNSRlsensorValue(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:8
NEPDLLAPI int __stdcall Detroit_SNSRlsensorCalib(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:9
NEPDLLAPI int __stdcall Detroit_SNSRpsensorID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);   //cmd:10
NEPDLLAPI int __stdcall Detroit_SNSRpsensorValue(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:11
NEPDLLAPI int __stdcall Detroit_SNSRpsensorCalib(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:12
NEPDLLAPI int __stdcall Detroit_SNSRgyroscopeID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:13
NEPDLLAPI int __stdcall Detroit_SNSRgyroscopeValue(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:14
NEPDLLAPI int __stdcall Detroit_SNSRgyroscopeCalib(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:15
NEPDLLAPI int __stdcall Detroit_SNSRGyroscopeSelfTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:16
NEPDLLAPI int __stdcall Detroit_SNSRgsensorSelfTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:17
NEPDLLAPI int __stdcall Detroit_SNSREcompassSelfTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:18


NEPDLLAPI int __stdcall Detroit_SNSRloadFALPkdata(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:13

//19:LED
NEPDLLAPI int __stdcall Detroit_LEDid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall Detroit_LEDon(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);

//20:SD
NEPDLLAPI int __stdcall Detroit_SDdetect(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:1
NEPDLLAPI int __stdcall Detroit_SDtestStart(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:2
NEPDLLAPI int __stdcall Detroit_SDtestStop(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:3
NEPDLLAPI int __stdcall Detroit_SDinfo(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:4
NEPDLLAPI int __stdcall Detroit_SDdetectPin(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:5

//21:ID
NEPDLLAPI int __stdcall Detroit_CPUid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:1
NEPDLLAPI int __stdcall Detroit_BoardADC(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:2
NEPDLLAPI int __stdcall Detroit_BoardRev(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:3
NEPDLLAPI int __stdcall Detroit_RFid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:4
NEPDLLAPI int __stdcall Detroit_MHLid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:5
NEPDLLAPI int __stdcall Detroit_RFICid(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:8

//90:OS
NEPDLLAPI int __stdcall Detroit_OSgetprop(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:1
NEPDLLAPI int __stdcall Detroit_OSkeyPower(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:2
NEPDLLAPI int __stdcall Detroit_OSpwr2home(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:3
NEPDLLAPI int __stdcall Detroit_ShutDown(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); //cmd:4
NEPDLLAPI int __stdcall Detroit_OSkeyFocus(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:5
NEPDLLAPI int __stdcall Detroit_OSkeyCamera(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:6

//91:version
NEPDLLAPI int __stdcall Detroit_VERftd(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall Detroit_VERchange(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);

//92:factory
NEPDLLAPI int __stdcall Detroit_FAC_FARead(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); //cmd:1
NEPDLLAPI int __stdcall Detroit_FAC_FAWrite(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:2
NEPDLLAPI int __stdcall Detroit_FAC_Picasso(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:3
NEPDLLAPI int __stdcall Detroit_FAC_OTPRead(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:4
NEPDLLAPI int __stdcall Detroit_FAC_OTPWrite(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:5
NEPDLLAPI int __stdcall Detroit_FAC_OTPEnable(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:6
NEPDLLAPI int __stdcall Detroit_FAC_CFGRead(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:7
NEPDLLAPI int __stdcall Detroit_FAC_CFGWrite(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//cmd:8

//Special Control Functions
NEPDLLAPI bool __stdcall Detroit_SplitSimData(char* input, char* output, char* ErrorCode);
NEPDLLAPI bool __stdcall Detroit_SplitPublicKey(char* input, char* output, char* ErrorCode);


//SPU
NEPDLLAPI int Detroit_FAC_FactoryReset_SPU(char* Port, char* Timeout, char* InBuffer, char* OutBuffer, char* ErrorCode);
NEPDLLAPI int Detroit_PWRbattCapacity_SPU(char* Port, char* Timeout, char* InBuffer, char* OutBuffer, char* ErrorCode); //cmd:2,12
NEPDLLAPI int Detroit_FAC_FARead_SPU(char* Port, char* Timeout, char* InBuffer, char* OutBuffer, char* ErrorCode); //cmd:1
NEPDLLAPI int Detroit_AutoFtdMode_SPU(char* Port, char* Timeout, char* ErrorCode);
NEPDLLAPI int Detroit_TestFtdMode_SPU(char* Port, char* Timeout, char* ErrorCode);  
NEPDLLAPI bool Detroit_ExecAdbOut_SPU(char* Command, char* output, char* ErrorCode);
NEPDLLAPI int Detroit_FAC_OTPRead_SPU(char* Port, char* Timeout, char* InBuffer, char* OutBuffer, char* ErrorCode);
NEPDLLAPI int Detroit_FAC_OTPWrite_SPU(char* Port, char* Timeout, char* InType,char* InValue, char* OutBuffer, char* ErrorCode);
NEPDLLAPI int Detroit_FAC_OTPEnable_SPU(char* Port, char* Timeout, char* InType,char* InValue, char* OutBuffer, char* ErrorCode);
NEPDLLAPI bool Detroit_CheckOTPExist(char* Port, char* Timeout, char* InType, char* InValue, char* OutBuffer);
NEPDLLAPI int Detroit_FAC_ShutDown_SPU(char* Port, char* Timeout, char* InValue, char* OutBuffer, char* ErrorCode);//90-4
NEPDLLAPI int Detroit_CheckIMEIStatus_SPU(char* Port, char* Timeout, char* InBuffer, char* OutBuffer, char* ErrorCode);//Lion20121206

NEPDLLAPI bool Detroit_SplitSimData_SPU(char* input, char* output, char* ErrorCode);
NEPDLLAPI bool Detroit_SplitPublicKey_SPU(char* input, char* output, char* ErrorCode);