#include "DLLDefine.h"

/******************************************************************************
 * Export/import function prototypes
 ******************************************************************************/
NEPDLLAPI bool __stdcall Detroit_ExecAdbOut(CString Command, char* output, char* error_msg);
NEPDLLAPI bool __stdcall Detroit_ExecFastbootOut(CString Command, char* output, char* error_msg);
NEPDLLAPI int __stdcall Detroit_TestFtdMode(int port, int timeout_ms, char* error_msg);
NEPDLLAPI int __stdcall Detroit_AutoFtdMode(int port, int timeout_ms, char* error_msg);

// FTD Communication Commands
//01:battery
NEPDLLAPI int __stdcall Detroit_BATTcoincellVoltage(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 1
NEPDLLAPI int __stdcall Detroit_BATTcoincellChargeOn(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 2
NEPDLLAPI int __stdcall Detroit_BATTrtcTime(int port, int timeout_ms, char* in_buf, char* out_buf);		// cmd: 3

//02:power
NEPDLLAPI int __stdcall Detroit_PWRgaugeID(int port, int timeout_ms, char* in_buf, char* out_buf); //cmd:1
NEPDLLAPI int __stdcall Detroit_PWRgaugeVoltage(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall Detroit_PWRgaugeCurrent(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall Detroit_PWRgaugeTemperature(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:4
NEPDLLAPI int __stdcall Detroit_PWRchargerID(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5
NEPDLLAPI int __stdcall Detroit_PWRchargerOn(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:6
NEPDLLAPI int __stdcall Detroit_DFIupgrade(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:7
NEPDLLAPI int __stdcall Detroit_ITenable(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:8
NEPDLLAPI int __stdcall Detroit_Regualator(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:9
NEPDLLAPI int __stdcall Detroit_PWRbattID(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:10
NEPDLLAPI int __stdcall Detroit_PWRbattCapacity(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:12
NEPDLLAPI int __stdcall Detroit_PWRWriteRsenseData(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:13
NEPDLLAPI int __stdcall Detroit_PWRAverageGaugeCurrent(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:14

//03:memory
NEPDLLAPI int __stdcall Detroit_MCPflashID(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall Detroit_MCPddrID(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall Detroit_MCPddrSize(int port, int timeout_ms, char* in_buf, char* out_buf);

//04:LCM - LCD & Touch
NEPDLLAPI int __stdcall Detroit_TOUCHpoint(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:1
NEPDLLAPI int __stdcall Detroit_TOUCHtest(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall Detroit_TOUCHid(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall Detroit_TOUCHversion(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:4
NEPDLLAPI int __stdcall Detroit_TOUCHSelfTest(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5
NEPDLLAPI int __stdcall Detroit_LCMbacklight(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:6
NEPDLLAPI int __stdcall Detroit_LCMbacklightColor(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:7
NEPDLLAPI int __stdcall Detroit_TouchModel(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:10

//05:audio
NEPDLLAPI int __stdcall Detroit_AUpathOut(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:1
NEPDLLAPI int __stdcall Detroit_AUplaymp3(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall Detroit_AUrecordWAV(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall Detroit_AUplayWAV(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:4
NEPDLLAPI int __stdcall Detroit_AUdetectHeadset(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5
NEPDLLAPI int __stdcall Detroit_AUpathRec(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:6
NEPDLLAPI int __stdcall Detroit_AUloopbackTest(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:9
NEPDLLAPI int __stdcall Detroit_AUButtonCheck(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:10
NEPDLLAPI int __stdcall Detroit_AUinitADC(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:11
NEPDLLAPI int __stdcall Detroit_AUADCCali(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:12
NEPDLLAPI int __stdcall Detroit_AUheadsetInfo(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:13

//06:camera
NEPDLLAPI int __stdcall Detroit_CAMReadID(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:1
NEPDLLAPI int __stdcall Detroit_CAMshot(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall Detroit_CAMstart(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall Detroit_CAMend(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:4
NEPDLLAPI int __stdcall Detroit_CAMFlashLED(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5
NEPDLLAPI int __stdcall Detroit_CAMFlashLEDID(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:6
NEPDLLAPI int __stdcall Detroit_CAM_OTP_AWB(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:7
NEPDLLAPI int __stdcall Detroit_CAM_OTP_AFMacro(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:8
NEPDLLAPI int __stdcall Detroit_CAM_OTP_AFInfinity(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:9
NEPDLLAPI int __stdcall Detroit_CAM_OTP_FrontFuseID(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:10
NEPDLLAPI int __stdcall Detroit_CAM_OTP_WriteFrontFuseID(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:10

//07:keypad
NEPDLLAPI int __stdcall Detroit_KeyTest(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2

//08:SIM
NEPDLLAPI int __stdcall Detroit_SIMtest(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:1
NEPDLLAPI int __stdcall Detroit_SIMdetect(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2

//10:Vibrator
NEPDLLAPI int __stdcall Detroit_VibRun(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:1

//12:Modem
NEPDLLAPI int __stdcall Detroit_TxRxPower(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:1
NEPDLLAPI int __stdcall Detroit_QmiRfTxTest(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall Detroit_QmiRfTxOff(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:4

//13-BT
NEPDLLAPI int __stdcall Detroit_BTid(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall Detroit_BTTestMode(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall Detroit_BTTXPower(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall Detroit_BTRXrssi(int port, int timeout_ms, char* in_buf, char* out_buf);

NEPDLLAPI int __stdcall Detroit_BTSelfTest(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall Detroit_BTLteTx(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall Detroit_BTLteRx(int port, int timeout_ms, char* in_buf, char* out_buf);

NEPDLLAPI int __stdcall Detroit_BTReset(int port, int timeout_ms, char* in_buf, char* out_buf);

//14:WLAN
NEPDLLAPI int __stdcall Detroit_WLAN_Mode(int port, int timeout_ms, char* in_buf, char* out_buf);   // cmd: 1
NEPDLLAPI int __stdcall Detroit_WLAN_RXrssi(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 3
NEPDLLAPI int __stdcall Detroit_WLAN_RXset(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 4
NEPDLLAPI int __stdcall Detroit_WLAN_ScanAP(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 5
NEPDLLAPI int __stdcall Detroit_WLAN_ConnectAP(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 6
NEPDLLAPI int __stdcall Detroit_WLANid(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 7

//15:GPS

//16:FMR
NEPDLLAPI int __stdcall Detroit_FMRrssi(int port, int timeout_ms, char* in_buf, char* out_buf);// cmd: 1
NEPDLLAPI int __stdcall Detroit_FMRModuleOn(int port, int timeout_ms, char* in_buf, char* out_buf);// cmd: 4

//17:NFC
NEPDLLAPI int __stdcall Detroit_NFCid(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall Detroit_NFCFWVersion(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall Detroit_NFCAntennaSelftest(int port, int timeout_ms, char* in_buf, char* out_buf); //cmd:3
NEPDLLAPI int __stdcall Detroit_NFCSwpSelftest(int port, int timeout_ms, char* in_buf, char* out_buf); //cmd:4

//18:Sensor
NEPDLLAPI int __stdcall Detroit_SNSRgsensorID(int port, int timeout_ms, char* in_buf, char* out_buf); //cmd:1
NEPDLLAPI int __stdcall Detroit_SNSRgsensorAcc(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall Detroit_SNSRgsensorCalib(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall Detroit_SNSRecompassID(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:4
NEPDLLAPI int __stdcall Detroit_SNSRecompassValue(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5
NEPDLLAPI int __stdcall Detroit_SNSRecompassValue(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:4

NEPDLLAPI int __stdcall Detroit_SNSRlsensorID(int port, int timeout_ms, char* in_buf, char* out_buf);   //cmd:7
NEPDLLAPI int __stdcall Detroit_SNSRlsensorValue(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:8
NEPDLLAPI int __stdcall Detroit_SNSRlsensorCalib(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:9
NEPDLLAPI int __stdcall Detroit_SNSRpsensorID(int port, int timeout_ms, char* in_buf, char* out_buf);   //cmd:10
NEPDLLAPI int __stdcall Detroit_SNSRpsensorValue(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:11
NEPDLLAPI int __stdcall Detroit_SNSRpsensorCalib(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:12
NEPDLLAPI int __stdcall Detroit_SNSRgyroscopeID(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:13
NEPDLLAPI int __stdcall Detroit_SNSRgyroscopeValue(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:14
NEPDLLAPI int __stdcall Detroit_SNSRgyroscopeCalib(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:15
NEPDLLAPI int __stdcall Detroit_SNSRGyroscopeSelfTest(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:16
NEPDLLAPI int __stdcall Detroit_SNSRgsensorSelfTest(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:17
NEPDLLAPI int __stdcall Detroit_SNSREcompassSelfTest(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:18

NEPDLLAPI int __stdcall Detroit_SNSRloadFALPkdata(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:13

//19:LED
NEPDLLAPI int __stdcall Detroit_LEDid(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall Detroit_LEDon(int port, int timeout_ms, char* in_buf, char* out_buf);

//20:SD
NEPDLLAPI int __stdcall Detroit_SDdetect(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:1
NEPDLLAPI int __stdcall Detroit_SDtestStart(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall Detroit_SDtestStop(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall Detroit_SDinfo(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:4
NEPDLLAPI int __stdcall Detroit_SDdetectPin(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5

//21:ID
NEPDLLAPI int __stdcall Detroit_CPUid(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:1
NEPDLLAPI int __stdcall Detroit_BoardADC(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall Detroit_BoardRev(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall Detroit_RFid(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:4
NEPDLLAPI int __stdcall Detroit_MHLid(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5
NEPDLLAPI int __stdcall Detroit_RFICid(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:8

//90:OS
NEPDLLAPI int __stdcall Detroit_OSgetprop(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:1
NEPDLLAPI int __stdcall Detroit_OSkeyPower(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall Detroit_OSpwr2home(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall Detroit_ShutDown(int port, int timeout_ms, char* in_buf, char* out_buf); //cmd:4
NEPDLLAPI int __stdcall Detroit_OSkeyFocus(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5
NEPDLLAPI int __stdcall Detroit_OSkeyCamera(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:6

//91:version
NEPDLLAPI int __stdcall Detroit_VERftd(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall Detroit_VERchange(int port, int timeout_ms, char* in_buf, char* out_buf);

//92:factory
NEPDLLAPI int __stdcall Detroit_FAC_FARead(int port, int timeout_ms, char* in_buf, char* out_buf); //cmd:1
NEPDLLAPI int __stdcall Detroit_FAC_FAWrite(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall Detroit_FAC_Picasso(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall Detroit_FAC_OTPRead(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:4
NEPDLLAPI int __stdcall Detroit_FAC_OTPWrite(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5
NEPDLLAPI int __stdcall Detroit_FAC_OTPEnable(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:6
NEPDLLAPI int __stdcall Detroit_FAC_CFGRead(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:7
NEPDLLAPI int __stdcall Detroit_FAC_CFGWrite(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:8
NEPDLLAPI int __stdcall Detroit_FAC_FSGProtect(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:9
NEPDLLAPI int __stdcall Detroit_FAC_BackupFA(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:10
NEPDLLAPI int __stdcall Detroit_FAC_BackupCFG(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:11
NEPDLLAPI int __stdcall Detroit_FAC_RestoreFA(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:12
NEPDLLAPI int __stdcall Detroit_FAC_RestoreCFG(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:13

//Special Control Functions
NEPDLLAPI bool __stdcall Detroit_SplitSimData(char* input, char* output, char* error_code);
NEPDLLAPI bool __stdcall Detroit_SplitPublicKey(char* input, char* output, char* error_code);
NEPDLLAPI bool Detroit_CheckOTPExist(char* Port, char* Timeout, char* InType, char* InValue, char* out_buf);

//Normal Functions SPU
NEPDLLAPI int Detroit_PWRbattCapacity_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code); //cmd:2,12
NEPDLLAPI int Detroit_FAC_FARead_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code); //cmd:1
NEPDLLAPI int Detroit_AutoFtdMode_SPU(char* Port, char* Timeout, char* error_code);
NEPDLLAPI int Detroit_TestFtdMode_SPU(char* Port, char* Timeout, char* error_code);
NEPDLLAPI bool Detroit_ExecAdbOut_SPU(char* Command, char* output, char* error_code);
NEPDLLAPI int Detroit_FAC_OTPRead_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code);
NEPDLLAPI int Detroit_FAC_OTPWrite_SPU(char* Port, char* Timeout, char* InType,char* InValue, char* out_buf, char* error_code);
NEPDLLAPI int Detroit_FAC_OTPEnable_SPU(char* Port, char* Timeout, char* InType,char* InValue, char* out_buf, char* error_code);
NEPDLLAPI int Detroit_FAC_ShutDown_SPU(char* Port, char* Timeout, char* InValue, char* out_buf, char* error_code);//90-4
NEPDLLAPI int Detroit_CheckIMEIStatus_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code);//Lion20121206
NEPDLLAPI int Detroit_FAC_FSGProtect_SPU(char* Port, char* Timeout, char* InType, char* InValue, char* out_buf, char* error_code);//92-9
NEPDLLAPI int Detroit_FAC_BackupFA_SPU(char* Port, char* Timeout, char* InValue, char* out_buf, char* error_code);//92-10
NEPDLLAPI int Detroit_FAC_BackupCFG_SPU(char* Port, char* Timeout, char* InValue, char* out_buf, char* error_code);//92-11
NEPDLLAPI int Detroit_FAC_BestoreFA_SPU(char* Port, char* Timeout, char* InValue, char* out_buf, char* error_code);//92-12
NEPDLLAPI int Detroit_FAC_RestoreCFG_SPU(char* Port, char* Timeout, char* InValue, char* out_buf, char* error_code);//92-13

//Special Control Functions SPU
NEPDLLAPI bool Detroit_SplitSimData_SPU(char* input, char* output, char* error_code);
NEPDLLAPI bool Detroit_SplitPublicKey_SPU(char* input, char* output, char* error_code);
