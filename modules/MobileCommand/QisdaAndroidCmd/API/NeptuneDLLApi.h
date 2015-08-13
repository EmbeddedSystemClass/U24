#include "DLLDefine.h"
/******************************************************************************
 * Export/import function prototypes
 ******************************************************************************/
//Standard API
NEPDLLAPI int __stdcall P21VibratorOn(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21VibratorOff(int port, int timeout_ms);

//SD card
NEPDLLAPI int __stdcall P21SDCardFileInfo(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21SDCardMoveImgToDDR(int port, int timeout_ms, char* InBuffer, size_t nInLength, char* OutBuffer);
NEPDLLAPI int __stdcall P21SDCardReadOSVersion(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall P21SDCardIO1Init(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall P21SDCardCMD(int port, int timeout_ms, char* InBuffer);

//HW
NEPDLLAPI int __stdcall P21HWGetDieID(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWGetProductID(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWGetTritonID(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWADCIN1(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWADCIN2(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWADCIN3(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWADCIN4(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWADCIN5(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWADCIN6(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWADCIN7(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWADCIN8(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWADCIN9(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWADCIN10(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWADCIN11(int port, int timeout_ms, char* OutBuffer);
// 2007-06-11 YTT
NEPDLLAPI int __stdcall P21HWGetHWVersion(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21I2CTest(int port, int timeout_ms, char* InBuffer, size_t nInLength, char* OutBuffer);

//Version Info
NEPDLLAPI int __stdcall P21VersionGetDTName(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21VersionGetDTVersion(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21VersionGetDTDate(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21VersionOSVersion(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21GetQisdaAndroidCmdCtrlDLLVersion(int port, int timeout_ms, char* version);
NEPDLLAPI int __stdcall P21GetQisdaAndroidCmdCtrlDLLSubVersion(int port, int timeout_ms, char* version);
//070611 Yu Teng Tung
NEPDLLAPI int __stdcall P21VersionGetIPLVersion(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21VersionGetIPLName(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21VersionGetIPLDate(int port, int timeout_ms, char* OutBuffer);

//LCD
NEPDLLAPI int __stdcall P21LCDInit(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LCDOff(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LCDRed(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LCDBlue(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LCDGreen(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LCDBlack(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LCDWhite(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LCDBacklightOn(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LCDBacklightOff(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LCDMagenta(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LCDCyan(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LCDYellow(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LCDSpecialPpattern1(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LCDSpecialPpattern2(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LCDSpecialPpattern3(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LCDSpecialPpattern4(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LCDSpecialPpattern5(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LCDSpecialPpattern6(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LCDSpecialPpattern7(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LCDSpecialPpattern8(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LCDSpecialPpattern9(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LCDGetID(int port, int timeout_ms, char* OutBuffer);

//Modem
NEPDLLAPI int __stdcall P21ModemSIMPowerOn(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21ModemSIMPowerOff(int port, int timeout_ms);
// 2007-06-11 YTT
NEPDLLAPI int __stdcall P21ModemSIMTest(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21ModemGetSIMInfo(int port, int timeout_ms, char* OutBuffer);

//BT
NEPDLLAPI int __stdcall P21BTOn(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21BTOff(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21BTTest(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21BTRelayMode(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21BTConnect(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall P21BTDisconnect(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21BTAudioOn(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21BTGetID(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21BTRFTest(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21BTFMGetRSSILevel(int port, int timeout_ms, char* InBuffer, size_t nInLength, char* OutBuffer);

//Clock
NEPDLLAPI int __stdcall P21ClockRTCOn(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21ClockRTCOff(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21ClockReadClock(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21ClockWriteClock(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall P21ClockReadAlarm(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21ClockWriteAlarm(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall P21ClockRTCAlarmPowerOn(int port, int timeout_ms);

//Changer
NEPDLLAPI int __stdcall P21ChargerCVOn(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall P21ChargerCCOn(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall P21ChargerOff(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21ChargerBKPBatteryOn(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21ChargerBKPBatteryOff(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21ChargerStatus(int port, int timeout_ms, char* OutBuffer);

//LED
NEPDLLAPI int __stdcall P21LED1Blink(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LED1On(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LED1Off(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21Sidekey1LEDBlink(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LED2On(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LED2Off(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LEDKeypadLedOn(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LEDKeypadLedOff(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LEDSidekey1LedOn(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LEDSidekey1LedOff(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LEDSidekey2LedOn(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21LEDSidekey2LedOff(int port, int timeout_ms);

//Audio
NEPDLLAPI int __stdcall P21AudioHandsetOnMicRev(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21AudioHeadsetOnHMicHRev(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21AudioHHHFOnMicLS(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21AudioHeadsetDetect(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21AudioAllOff(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21AudioToneToRev(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21AudioToneToHRev(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21AudioToneToLS(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21AudioHookDetect(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21AudioVULPG(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall P21AudioVDLPG(int port, int timeout_ms, char* InBuffer);

//DTR control
NEPDLLAPI int __stdcall P21EnableDTR(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21DisableDTR(int port, int timeout_ms);

//callback function
NEPDLLAPI int __stdcall P21RegisterCallBack(int port, int timeout_ms, char* RegCallBack);
NEPDLLAPI int __stdcall P21UnregisterCallBack(int port, int timeout_ms, char* RegCallBack);

//MDOC
NEPDLLAPI int __stdcall P21FlashExit(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21FlashFormatAll(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21FlashReadMDocID(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21FlashFormatBDTL8M2M(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21FlashFormatBDTL5M60M(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21FlashMDOCInfo(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21FlashFormatAllPCAssign(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall P21FlashFormatBDTLPCAssign(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall P21FlashCheckProtect(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21FlashGetServicePassword(int port, int timeout_ms, char* OutBuffer);

//Mode Change
NEPDLLAPI int __stdcall P21UARTPacketMode(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21USBPacketMode(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21EngineerMode(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21301Mode(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21UARTConsoleMode(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21USBConsoleMode(int port, int timeout_ms);

//WLan
NEPDLLAPI int __stdcall P21WLANPowerOn(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21WLANPowerOff(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21WLANSetWlanTestMode(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21WLANSetCmdLocalFrequecy(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall P21WLANSetCmdOutputPower(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall P21WLANSetCmdMACAddrToEEProm(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall P21WLANGetCmdMACAddrFromEEProm(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21WLANGetMTKChipID(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21WLANGetCmdPacketRxStatus(int port, int timeout_ms, char* InBuffer, size_t nInLength, char* OutBuffer);

//DL
// 2007-09-20 YTT
NEPDLLAPI int __stdcall P21DLCheckServicePasswd(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall P21DLVerifyChecksumFromFlash(int port, int timeout_ms, char* InBuffer);

//OS
NEPDLLAPI int __stdcall P21OSRunNecleusOS(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21OSPowerOff(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall P21OSWatchDogReset(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall P21OSEraseNucleusGSMFFS(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21OSNucleusSaveGSMFFS(int port, int timeout_ms);

//RD
// 2006-11-09 YTT
//replace bool to int
NEPDLLAPI int __stdcall P21PowerOnToUARTPacketMode(int port, int timeout_ms, int DTRflag);
NEPDLLAPI int __stdcall GetMEPDUnlockCode(unsigned char * szUnlockCode, const unsigned char * mepd, unsigned int length, unsigned int easyDecode, char *FSN, char *IMEI); //Mark Hsieh 2008.2.27

// 2006-11-15 YTT
//CallBack for getting data
NEPDLLAPI int __stdcall RegisterCallBackChar(int port, int timeout_ms, char* RegCallBack);
NEPDLLAPI int __stdcall UnregisterCallBackChar(int port, int timeout_ms, char* RegCallBack);

//camera
NEPDLLAPI int __stdcall P21CameraPowerOn(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21CameraPowerOff(int port, int timeout_ms);

//SDRAM
NEPDLLAPI int __stdcall P21SDRAMReadWriteFull(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21SDRAMReadWritePartial(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21SDRAMLineTest(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21SDRAMGetID(int port, int timeout_ms, char* OutBuffer);

//Factory Area
NEPDLLAPI int __stdcall P21FAWriteFactoryArea(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21FAReadFactoryArea(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall P21FAWriteIMEI(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall P21FAReadIMEI(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21FAWriteVBATOffset(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall P21FAReadVBATOffset(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21FAReadCountryCode(int port, int timeout_ms, char* OutPath);
NEPDLLAPI int __stdcall P21FAWriteCountryCode(int port, int timeout_ms, char* DLPath);
NEPDLLAPI int __stdcall P21FAWriteBTAddress(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall P21FAReadBTAddress(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21FAWrite301Flag(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall P21FARead301Flag(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21FAWriteIMEITemp(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall P21FAReadIMEITemp(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21FAWriteVersionInfo(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall P21FAReadVersionInfo(int port, int timeout_ms, char* InBuffer, size_t nInLength, char* OutBuffer);
NEPDLLAPI int __stdcall P21FABackupGSMFFSToFA(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21FARestoreGSMFFSFromFA(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21FAWriteIMEIAndNLockkey(int port, int timeout_ms, char* cImei, char* cLockKey);
NEPDLLAPI int __stdcall P21FAReadIMEIAndNLockkey(int port, int timeout_ms, char* cImei, char* cLockKey);
NEPDLLAPI int __stdcall P21FAVBATCalibration(int port, int timeout_ms, char* OutBuffer);

//Keypad
NEPDLLAPI int __stdcall P21KeypadPress(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21KeypadReadMatrix(int port, int timeout_ms, char* OutBuffer);

//// Just Test
NEPDLLAPI int __stdcall P21GenCMDTEST(int port, int timeout_ms);
NEPDLLAPI int __stdcall P21GenCMDOutTEST(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall P21GenCMDInTEST(int port, int timeout_ms, char* InBuffer);
NEPDLLAPI int __stdcall P21GenCMDInOutTEST(int port, int timeout_ms, char* InBuffer, size_t nInLength, char* OutBuffer);
