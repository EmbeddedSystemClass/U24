#include "DLLDefine.h"

/******************************************************************************
 * Export/import function prototypes
 ******************************************************************************/
NEPDLLAPI bool __stdcall FTD_ExecAdbOut(CString Command, char* output, char* error_msg);
NEPDLLAPI bool __stdcall FTD_ExecFastbootOut(CString Command, char* output, char* error_msg);
NEPDLLAPI int __stdcall FTD_TestFtdMode(int port, int timeout_ms, char* error_msg);
NEPDLLAPI int __stdcall FTD_AutoFtdMode(int port, int timeout_ms, char* error_msg);

// FTD Communication Commands
//01:battery
NEPDLLAPI int __stdcall FTD_BATTcoincellVoltage(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 1
NEPDLLAPI int __stdcall FTD_BATTcoincellChargeOn(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 2
NEPDLLAPI int __stdcall FTD_BATTrtcTime(int port, int timeout_ms, char* in_buf, char* out_buf);		// cmd: 3

//02:power
NEPDLLAPI int __stdcall FTD_PWRgaugeID(int port, int timeout_ms, char* in_buf, char* out_buf); //cmd:1
NEPDLLAPI int __stdcall FTD_PWRgaugeVoltage(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall FTD_PWRgaugeCurrent(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall FTD_PWRgaugeTemperature(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:4
NEPDLLAPI int __stdcall FTD_PWRchargerID(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5
NEPDLLAPI int __stdcall FTD_PWRchargerOn(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:6
NEPDLLAPI int __stdcall FTD_DFIupgrade(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:7
NEPDLLAPI int __stdcall FTD_ITenable(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:8
NEPDLLAPI int __stdcall FTD_Regualator(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:9
NEPDLLAPI int __stdcall FTD_PWRbattID(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:10
NEPDLLAPI int __stdcall FTD_PWRRFBandIDTemperature(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:11 //Eason
NEPDLLAPI int __stdcall FTD_PWRbattCapacity(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:12
NEPDLLAPI int __stdcall FTD_PWRWriteRsenseData(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:13
NEPDLLAPI int __stdcall FTD_PWRAverageGaugeCurrent(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:14
NEPDLLAPI int __stdcall FTD_WriteMaxChargeCurrent(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:15 //Eason

NEPDLLAPI int __stdcall FTD_ReadPowerMaxCurrent(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:16 //Eason


NEPDLLAPI int __stdcall FTD_ReadBuckID(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:18 ////[F80] 導入 Eason
NEPDLLAPI int __stdcall FTD_ReadBuckfeature(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:19 ////[F80] 導入 Eason

//03:memory
NEPDLLAPI int __stdcall FTD_MCPflashID(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall FTD_MCPddrID(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall FTD_MCPddrSize(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall FTD_MCPCsdInfo(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:7
NEPDLLAPI int __stdcall FTD_MCPemmcsize(int port, int timeout_ms, char* in_buf, char* out_buf);		//cmd:8 //Eason
NEPDLLAPI int __stdcall FTD_MCPemmcFWversion(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:9 //Eason
NEPDLLAPI int __stdcall FTD_MCPddrmr8(int port, int timeout_ms, char* in_buf, char* out_buf);		//cmd:10 //Eason
NEPDLLAPI int __stdcall FTD_MCPddrmr6(int port, int timeout_ms, char* in_buf, char* out_buf);		//cmd:11 //Eason


//04:LCM - LCD & Touch
NEPDLLAPI int __stdcall FTD_TOUCHpoint(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:1
NEPDLLAPI int __stdcall FTD_TOUCHtest(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall FTD_TOUCHid(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall FTD_TOUCHversion(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:4
NEPDLLAPI int __stdcall FTD_TOUCHSelfTest(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5
NEPDLLAPI int __stdcall FTD_LCMbacklight(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:6
NEPDLLAPI int __stdcall FTD_LCMbacklightColor(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:7
NEPDLLAPI int __stdcall FTD_TouchModel(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:10
NEPDLLAPI int __stdcall FTD_TouchCheck(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:13

NEPDLLAPI int __stdcall FTD_OLEDTest(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:14

NEPDLLAPI int __stdcall FTD_BacklightDriver(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:14 ////[F80] 導入 Eason 指令重複但是 OLED BSP 說只有QC1用到

//05:audio
NEPDLLAPI int __stdcall FTD_AUpathOut(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:1
NEPDLLAPI int __stdcall FTD_AUplaymp3(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall FTD_AUrecordWAV(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall FTD_AUplayWAV(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:4
NEPDLLAPI int __stdcall FTD_AUdetectHeadset(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5
NEPDLLAPI int __stdcall FTD_AUpathRec(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:6
NEPDLLAPI int __stdcall FTD_AUloopbackTest(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:9
NEPDLLAPI int __stdcall FTD_AUButtonCheck(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:10
NEPDLLAPI int __stdcall FTD_AUinitADC(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:11
NEPDLLAPI int __stdcall FTD_AUADCCali(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:12
NEPDLLAPI int __stdcall FTD_AUheadsetInfo(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:13

//06:camera
NEPDLLAPI int __stdcall FTD_CAMReadID(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:1
NEPDLLAPI int __stdcall FTD_CAMshot(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall FTD_CAMstart(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall FTD_CAMend(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:4
NEPDLLAPI int __stdcall FTD_CAMFlashLED(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5
NEPDLLAPI int __stdcall FTD_CAMFlashLEDID(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:6
NEPDLLAPI int __stdcall FTD_CAM_OTP_AWB(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:7
NEPDLLAPI int __stdcall FTD_CAM_OTP_AFMacro(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:8
NEPDLLAPI int __stdcall FTD_CAM_OTP_AFInfinity(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:9
NEPDLLAPI int __stdcall FTD_CAM_OTP_FrontFuseID(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:10
NEPDLLAPI int __stdcall FTD_CAM_OTP_WriteFrontFuseID(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:11
NEPDLLAPI int __stdcall FTD_CAM_Read_Bridge_ID(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:12
NEPDLLAPI int __stdcall FTD_CAM_EEPROM_Power(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:13
NEPDLLAPI int __stdcall FTD_CAM_EEPROM_Write(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:14
NEPDLLAPI int __stdcall FTD_CAM_EEPROM_Read(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:15
NEPDLLAPI int __stdcall FTD_CAM_MIPI_LVDS_Status(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:16

//07:keypad
NEPDLLAPI int __stdcall FTD_KeyTest(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2

//08:SIM
NEPDLLAPI int __stdcall FTD_SIMtest(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:1
NEPDLLAPI int __stdcall FTD_SIMdetect(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2

//09: USB
NEPDLLAPI int __stdcall FTD_USBtest(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:1

//10:Vibrator
NEPDLLAPI int __stdcall FTD_VibRun(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:1

//12:Modem
NEPDLLAPI int __stdcall FTD_TxRxPower(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:1
NEPDLLAPI int __stdcall FTD_QmiRfTxTest(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall FTD_QmiRfTxOff(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:4

//13-BT
NEPDLLAPI int __stdcall FTD_BTid(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall FTD_BTTestMode(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall FTD_BTTXPower(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall FTD_BTRXrssi(int port, int timeout_ms, char* in_buf, char* out_buf);

NEPDLLAPI int __stdcall FTD_BTSelfTest(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall FTD_BTLteTx(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall FTD_BTLteRx(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall FTD_BT_MAC(int port, int timeout_ms, char* in_buf, char* out_buf);  //cmd:9
NEPDLLAPI int __stdcall FTD_BT_INQUIRY(int port, int timeout_ms, char* in_buf, char* out_buf);  //cmd:10
NEPDLLAPI int __stdcall FTD_2_4GPowerMode(int port, int timeout_ms, char* in_buf, char* out_buf);  //cmd:14
NEPDLLAPI int __stdcall FTD_2_4GConsumer(int port, int timeout_ms, char* in_buf, char* out_buf);  //cmd:15

//14:WLAN
NEPDLLAPI int __stdcall FTD_WLAN_Mode(int port, int timeout_ms, char* in_buf, char* out_buf);   // cmd: 1
NEPDLLAPI int __stdcall FTD_WLAN_RXrssi(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 3
NEPDLLAPI int __stdcall FTD_WLAN_RXset(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 4
NEPDLLAPI int __stdcall FTD_WLAN_ScanAP(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 5
NEPDLLAPI int __stdcall FTD_WLAN_ConnectAP(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 6
NEPDLLAPI int __stdcall FTD_WLANid(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 7

NEPDLLAPI int __stdcall FTD_WLAN_selfTest(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 8
NEPDLLAPI int __stdcall FTD_WLAN_MODULE_Test(int port, int timeout_ms, char* in_buf, char* out_buf); // cmd: 10


//15:GPS

//16:FMR
NEPDLLAPI int __stdcall FTD_FMRrssi(int port, int timeout_ms, char* in_buf, char* out_buf);// cmd: 1
NEPDLLAPI int __stdcall FTD_FMRModuleOn(int port, int timeout_ms, char* in_buf, char* out_buf);// cmd: 4

NEPDLLAPI int __stdcall FTD_FMR_SelfTest(int port, int timeout_ms, char* in_buf, char* out_buf);// cmd: 3 

//17:NFC
NEPDLLAPI int __stdcall FTD_NFCid(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall FTD_NFCFWVersion(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall FTD_NFCAntennaSelftest(int port, int timeout_ms, char* in_buf, char* out_buf); //cmd:3
NEPDLLAPI int __stdcall FTD_NFCSwpSelftest(int port, int timeout_ms, char* in_buf, char* out_buf); //cmd:4
NEPDLLAPI int __stdcall FTD_NFCHCITypeAReader(int port, int timeout_ms, char* in_buf, char* out_buf); //cmd:5

//18:Sensor
NEPDLLAPI int __stdcall FTD_SNSRgsensorID(int port, int timeout_ms, char* in_buf, char* out_buf); //cmd:1
NEPDLLAPI int __stdcall FTD_SNSRgsensorAcc(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall FTD_SNSRgsensorCalib(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall FTD_SNSRecompassID(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:4
NEPDLLAPI int __stdcall FTD_SNSRecompassValue(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5
NEPDLLAPI int __stdcall FTD_SNSRecompassValue(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:4

NEPDLLAPI int __stdcall FTD_SNSRlsensorID(int port, int timeout_ms, char* in_buf, char* out_buf);   //cmd:7
NEPDLLAPI int __stdcall FTD_SNSRlsensorValue(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:8
NEPDLLAPI int __stdcall FTD_SNSRlsensorCalib(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:9
NEPDLLAPI int __stdcall FTD_SNSRpsensorID(int port, int timeout_ms, char* in_buf, char* out_buf);   //cmd:10
NEPDLLAPI int __stdcall FTD_SNSRpsensorValue(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:11
NEPDLLAPI int __stdcall FTD_SNSRpsensorCalib(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:12
NEPDLLAPI int __stdcall FTD_SNSRgyroscopeID(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:13
NEPDLLAPI int __stdcall FTD_SNSRgyroscopeValue(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:14
NEPDLLAPI int __stdcall FTD_SNSRgyroscopeCalib(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:15
NEPDLLAPI int __stdcall FTD_SNSRGyroscopeSelfTest(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:16
NEPDLLAPI int __stdcall FTD_SNSRgsensorSelfTest(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:17
NEPDLLAPI int __stdcall FTD_SNSREcompassSelfTest(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:18

NEPDLLAPI int __stdcall FTD_SNSRloadFALPkdata(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:13

//19:LED
NEPDLLAPI int __stdcall FTD_LEDid(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall FTD_LEDon(int port, int timeout_ms, char* in_buf, char* out_buf);

//20:SD
NEPDLLAPI int __stdcall FTD_SDdetect(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:1
NEPDLLAPI int __stdcall FTD_SDtestStart(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall FTD_SDtestStop(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall FTD_SDinfo(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:4
NEPDLLAPI int __stdcall FTD_SDdetectPin(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5

//21:ID
NEPDLLAPI int __stdcall FTD_CPUid(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:1
NEPDLLAPI int __stdcall FTD_BoardADC(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall FTD_BoardRev(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall FTD_RFid(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:4
NEPDLLAPI int __stdcall FTD_MHLid(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5
NEPDLLAPI int __stdcall FTD_PMid(int port, int timeout_ms, char* in_buf, char* out_buf);		//cmd:6 //YTT
NEPDLLAPI int __stdcall FTD_SOCversion(int port, int timeout_ms, char* in_buf, char* out_buf);	//cmd:7 //YTT
NEPDLLAPI int __stdcall FTD_RFICid(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:8
NEPDLLAPI int __stdcall FTD_PCBVerMPP3(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:9 //YTT U2417
NEPDLLAPI int __stdcall FTD_PCBVerMPP2(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:10 //YTT U2417
NEPDLLAPI int __stdcall FTD_PMIid(int port, int timeout_ms, char* in_buf, char* out_buf);	//cmd:11 //YTT
NEPDLLAPI int __stdcall FTD_RFASMid(int port, int timeout_ms, char* in_buf, char* out_buf);		//cmd:12 //Eason

NEPDLLAPI int __stdcall FTD_AntennaIC(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:13


NEPDLLAPI int __stdcall FTD_Boardid(int port, int timeout_ms, char* in_buf, char* out_buf);		//cmd:14 //Eason
NEPDLLAPI int __stdcall FTD_SKUid(int port, int timeout_ms, char* in_buf, char* out_buf);		//cmd:15 //Eason

NEPDLLAPI int __stdcall FTD_SOCfeature(int port, int timeout_ms, char* in_buf, char* out_buf);	//cmd:17 //Eason
//90:OS
NEPDLLAPI int __stdcall FTD_OSgetprop(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:1
NEPDLLAPI int __stdcall FTD_OSkeyPower(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall FTD_OSpwr2home(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall FTD_ShutDown(int port, int timeout_ms, char* in_buf, char* out_buf); //cmd:4
NEPDLLAPI int __stdcall FTD_OSkeyFocus(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5
NEPDLLAPI int __stdcall FTD_OSkeyCamera(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:6

//91:version
NEPDLLAPI int __stdcall FTD_VERftd(int port, int timeout_ms, char* in_buf, char* out_buf);
NEPDLLAPI int __stdcall FTD_VERchange(int port, int timeout_ms, char* in_buf, char* out_buf);

//92:factory
NEPDLLAPI int __stdcall FTD_FAC_FARead(int port, int timeout_ms, char* in_buf, char* out_buf); //cmd:1
NEPDLLAPI int __stdcall FTD_FAC_FAWrite(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall FTD_FAC_Picasso(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall FTD_FAC_OTPRead(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:4
NEPDLLAPI int __stdcall FTD_FAC_OTPWrite(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5
NEPDLLAPI int __stdcall FTD_FAC_OTPEnable(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:6
NEPDLLAPI int __stdcall FTD_FAC_CFGRead(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:7
NEPDLLAPI int __stdcall FTD_FAC_CFGWrite(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:8
NEPDLLAPI int __stdcall FTD_FAC_FSGProtect(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:9
NEPDLLAPI int __stdcall FTD_FAC_BackupFA(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:10
NEPDLLAPI int __stdcall FTD_FAC_BackupCFG(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:11
NEPDLLAPI int __stdcall FTD_FAC_RestoreFA(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:12
NEPDLLAPI int __stdcall FTD_FAC_RestoreCFG(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:13
NEPDLLAPI int __stdcall FTD_FAC_EraseL1Log(int port, int timeout_ms, char* in_buf, char* out_buf); //cmd:14
NEPDLLAPI int __stdcall FTD_FAC_BackupModemCFG(int port, int timeout_ms, char* in_buf, char* out_buf); //cmd:15
NEPDLLAPI int __stdcall FTD_FAC_RestoreModemCFG(int port, int timeout_ms, char* in_buf, char* out_buf); //cmd:16

//93:misc
NEPDLLAPI int __stdcall FTD_MISC_CallDrop(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:2
NEPDLLAPI int __stdcall FTD_Get_GPIO_Value(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:3
NEPDLLAPI int __stdcall FTD_Check_APQ_UART(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5


NEPDLLAPI int __stdcall FTD_Set_Model(int port, int timeout_ms, char* in_buf, char* out_buf);//cmd:5


//Special Control Functions
NEPDLLAPI bool __stdcall FTD_SplitSimData(char* input, char* output, char* error_code);
NEPDLLAPI bool __stdcall FTD_SplitPublicKey(char* input, char* output, char* error_code);
NEPDLLAPI bool FTD_CheckOTPExist(char* Port, char* Timeout, char* InType, char* InValue, char* out_buf);

//Normal Functions SPU
NEPDLLAPI int FTD_PWRgaugeVoltage_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code); //cmd:2-2
NEPDLLAPI int FTD_PWRbattCapacity_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code); //cmd:2,12
NEPDLLAPI int FTD_OSgetprop_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code); //cmd:90,1
NEPDLLAPI int FTD_FAC_FARead_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code); //cmd:1
NEPDLLAPI int FTD_AutoFtdMode_SPU(char* Port, char* Timeout, char* error_code);
NEPDLLAPI int FTD_TestFtdMode_SPU(char* Port, char* Timeout, char* error_code);
NEPDLLAPI bool FTD_ExecAdbOut_SPU(char* Command, char* output, char* error_code);
NEPDLLAPI bool FTD_ExecAdbPull_SPU(char* From, char* To, char* output, char* error_code);
NEPDLLAPI bool FTD_ExecAdbPush_SPU(char* From, char* To, char* output, char* error_code);
NEPDLLAPI bool FTD_TouchCheck_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code); //cmd:4-13
NEPDLLAPI bool FTD_OLEDTest_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code); //cmd:4-14
NEPDLLAPI bool FTD_AUplayWAV_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code); //cmd:5-4
NEPDLLAPI bool FTD_AUloopbackTest_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code); //cmd:5-9
NEPDLLAPI bool FTD_CAMshot_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code); //cmd:6-2
NEPDLLAPI bool FTD_CAMstart_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code); //cmd:6-3
NEPDLLAPI bool FTD_CAMend_SPU(char* Port, char* Timeout, char* out_buf, char* error_code); //cmd:6-4
NEPDLLAPI bool FTD_CAM_OTP_WriteFrontFuseID_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code); //cmd:6-11
NEPDLLAPI bool FTD_CAM_Read_Bridge_ID_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code); //cmd:6-12
NEPDLLAPI bool FTD_CAM_EEPROM_Power_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code); //cmd:6-13
NEPDLLAPI bool FTD_CAM_EEPROM_Write_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code); //cmd:6-14
NEPDLLAPI bool FTD_CAM_EEPROM_Read_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code); //cmd:6-15
NEPDLLAPI bool FTD_CAM_MIPI_LVDS_Status_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code); //cmd:6-16
NEPDLLAPI bool FTD_LEDon_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code); //cmd:19-2
NEPDLLAPI bool FTD_Boardid_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code); //cmd:21-14
NEPDLLAPI int FTD_FAC_OTPRead_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code);
NEPDLLAPI int FTD_FAC_OTPWrite_SPU(char* Port, char* Timeout, char* InType,char* InValue, char* out_buf, char* error_code);
NEPDLLAPI int FTD_FAC_OTPEnable_SPU(char* Port, char* Timeout, char* InType,char* InValue, char* out_buf, char* error_code);
NEPDLLAPI int FTD_FAC_ShutDown_SPU(char* Port, char* Timeout, char* InValue, char* out_buf, char* error_code);//90-4
NEPDLLAPI int FTD_CheckIMEIStatus_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code);//Lion20121206
NEPDLLAPI int FTD_FAC_FSGProtect_SPU(char* Port, char* Timeout, char* InType, char* InValue, char* out_buf, char* error_code);//92-9
NEPDLLAPI int FTD_FAC_BackupFA_SPU(char* Port, char* Timeout, char* InValue, char* out_buf, char* error_code);//92-10
NEPDLLAPI int FTD_FAC_BackupCFG_SPU(char* Port, char* Timeout, char* InValue, char* out_buf, char* error_code);//92-11
NEPDLLAPI int FTD_FAC_BestoreFA_SPU(char* Port, char* Timeout, char* InValue, char* out_buf, char* error_code);//92-12
NEPDLLAPI int FTD_FAC_RestoreCFG_SPU(char* Port, char* Timeout, char* InValue, char* out_buf, char* error_code);//92-13
NEPDLLAPI int FTD_FAC_EraseL1Log_SPU(char* Port, char* Timeout, char* InValue, char* out_buf, char* error_code);//92-14
NEPDLLAPI int FTD_FAC_BackupModemCFG_SPU(char* Port, char* Timeout, char* InValue, char* out_buf, char* error_code);//92-15
NEPDLLAPI int FTD_FAC_RestoreModemCFG_SPU(char* Port, char* Timeout, char* InValue, char* out_buf, char* error_code);//92-16

//Special Control Functions SPU
NEPDLLAPI bool FTD_SplitSimData_SPU(char* input, char* output, char* error_code);
NEPDLLAPI bool FTD_SplitPublicKey_SPU(char* input, char* output, char* error_code);
NEPDLLAPI bool FTD_WDS_Contorl_SPU(char* in_buf, char* out_buf, char* error_code);
NEPDLLAPI bool FTD_CheckDeviceNotInOSMode_SPU(char* Port, char* Timeout, char* in_buf, char* out_buf, char* error_code);
NEPDLLAPI bool FTD_SaveBIResult_SPU(char* BIResult, char* LogPath, char* TargetID, char* out_buf, char* error_code);
