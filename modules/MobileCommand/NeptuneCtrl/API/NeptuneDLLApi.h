#include "DLLDefine.h"
/******************************************************************************
 * Export/import function prototypes
 ******************************************************************************/
NEPDLLAPI int __stdcall GetDLLInfo(int PortNum, int nTimeout, char* OutputFileName);
NEPDLLAPI int __stdcall DDRRun(int nPort, int nTimeout, char* TI2ndFile , char* DDRFile);
NEPDLLAPI int __stdcall P21DDRDump(int PortNum, int nTimeout);

//Exact Control API
NEPDLLAPI int __stdcall ExactReadDevice(int nPort, int Timeout, char*InBuffer, int InSize);
NEPDLLAPI int __stdcall ExactWriteDevice(int nPort, int Timeout, char*OutBuffer, int OutSize);

//Standard API
NEPDLLAPI int __stdcall P21VibratorOn(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21VibratorOff(int nPort, int nTimeout);

//SD card
NEPDLLAPI int __stdcall P21SDCardFileInfo(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21SDCardMoveImgToDDR(int nPort, int nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer);
NEPDLLAPI int __stdcall P21SDCardReadOSVersion(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21SDCardDownloadMSIPL(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21SDCardDownloadEBOOT(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21SDCardDownloadBDTL0(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21SDCardDownloadBDTL1(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21SDCardIO1Init(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21SDCardCMD(int nPort, int nTimeout, char* InBuffer);

//HW
NEPDLLAPI int __stdcall P21HWGetDieID(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWGetProductID(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWGetTritonID(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWADCIN1(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWADCIN2(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWADCIN3(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWADCIN4(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWADCIN5(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWADCIN6(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWADCIN7(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWADCIN8(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWADCIN9(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWADCIN10(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21HWADCIN11(int nPort, int nTimeout, char* OutBuffer);
// 2007-06-11 YTT
NEPDLLAPI int __stdcall P21HWGetHWVersion(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21I2CTest(int nPort, int nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer);

//Version Info
NEPDLLAPI int __stdcall P21VersionGetDTName(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21VersionGetDTVersion(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21VersionGetDTDate(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21VersionOSVersion(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21GetNeptuneCtrlDLLVersion(int nPort, int nTimeout, char* version);
NEPDLLAPI int __stdcall P21GetNeptuneCtrlDLLSubVersion(int nPort, int nTimeout, char* version);
//070611 Yu Teng Tung
NEPDLLAPI int __stdcall P21VersionGetIPLVersion(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21VersionGetIPLName(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21VersionGetIPLDate(int nPort, int nTimeout, char* OutBuffer);

//LCD
NEPDLLAPI int __stdcall P21LCDInit(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LCDOff(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LCDRed(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LCDBlue(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LCDGreen(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LCDBlack(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LCDWhite(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LCDBacklightOn(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LCDBacklightOff(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LCDMagenta(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LCDCyan(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LCDYellow(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LCDSpecialPpattern1(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LCDSpecialPpattern2(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LCDSpecialPpattern3(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LCDSpecialPpattern4(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LCDSpecialPpattern5(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LCDSpecialPpattern6(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LCDSpecialPpattern7(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LCDSpecialPpattern8(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LCDSpecialPpattern9(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LCDGetID(int nPort, int nTimeout, char* OutBuffer);

//Modem
NEPDLLAPI int __stdcall P21ModemSIMPowerOn(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21ModemSIMPowerOff(int nPort, int nTimeout);
// 2007-06-11 YTT
NEPDLLAPI int __stdcall P21ModemSIMTest(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21ModemGetSIMInfo(int nPort, int nTimeout, char* OutBuffer);

//BT
NEPDLLAPI int __stdcall P21BTOn(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21BTOff(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21BTTest(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21BTRelayMode(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21BTConnect(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21BTDisconnect(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21BTAudioOn(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21BTGetID(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21BTRFTest(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21BTFMGetRSSILevel(int nPort, int nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer);

//Clock
NEPDLLAPI int __stdcall P21ClockRTCOn(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21ClockRTCOff(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21ClockReadClock(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21ClockWriteClock(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21ClockReadAlarm(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21ClockWriteAlarm(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21ClockRTCAlarmPowerOn(int nPort, int nTimeout);

//Changer
NEPDLLAPI int __stdcall P21ChargerCVOn(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21ChargerCCOn(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21ChargerOff(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21ChargerBKPBatteryOn(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21ChargerBKPBatteryOff(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21ChargerStatus(int nPort, int nTimeout, char* OutBuffer);

//LED
NEPDLLAPI int __stdcall P21LED1Blink(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LED1On(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LED1Off(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21Sidekey1LEDBlink(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LED2On(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LED2Off(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LEDKeypadLedOn(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LEDKeypadLedOff(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LEDSidekey1LedOn(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LEDSidekey1LedOff(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LEDSidekey2LedOn(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LEDSidekey2LedOff(int nPort, int nTimeout);

//Audio
NEPDLLAPI int __stdcall P21AudioHandsetOnMicRev(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21AudioHeadsetOnHMicHRev(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21AudioHHHFOnMicLS(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21AudioHeadsetDetect(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21AudioAllOff(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21AudioToneToRev(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21AudioToneToHRev(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21AudioToneToLS(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21AudioHookDetect(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21AudioVULPG(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21AudioVDLPG(int nPort, int nTimeout, char* InBuffer);

//DTR control
NEPDLLAPI int __stdcall P21EnableDTR(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21DisableDTR(int nPort, int nTimeout);

//callback function
NEPDLLAPI int __stdcall P21RegisterCallBack(int nPort, int nTimeout, char* RegCallBack);
NEPDLLAPI int __stdcall P21UnregisterCallBack(int nPort, int nTimeout, char* RegCallBack);

//Flash
//Download
NEPDLLAPI int __stdcall P21FlashIPLDownload(int nPort, int nTimeout, char* DLPath);
NEPDLLAPI int __stdcall P21FlashMSIPLDownload(int nPort, int nTimeout, char* DLPath);
NEPDLLAPI int __stdcall P21FlashEBOOTDownload(int nPort, int nTimeout, char* DLPath);
NEPDLLAPI int __stdcall P21FlashDTDownload(int nPort, int nTimeout, char* DLPath);
NEPDLLAPI int __stdcall P21FlashWriteFA(int nPort, int nTimeout, char* DLPath);
NEPDLLAPI int __stdcall P21FlashWriteBDTL0(int nPort, int nTimeout, char* DLPath);
NEPDLLAPI int __stdcall P21FlashWriteBDTL1(int nPort, int nTimeout, char* DLPath);
NEPDLLAPI int __stdcall P21FlashWriteBDTL2(int nPort, int nTimeout, char* DLPath);
NEPDLLAPI int __stdcall P21FlashWriteCFG(int nPort, int nTimeout, char* DLPath);
NEPDLLAPI int __stdcall P21FlashWriteNGSMFFS(int nPort, int nTimeout, char* DLPath);
NEPDLLAPI int __stdcall P21FlashWriteWGSMFFS(int nPort, int nTimeout, char* DLPath);
NEPDLLAPI int __stdcall P21FlashWriteBTDLSectors(int nPort, int nTimeout, char* DLPath, char* parameters);
NEPDLLAPI int __stdcall P21FlashWriteBDTL3(int nPort, int nTimeout, char* DLPath);

//Upload
NEPDLLAPI int __stdcall P21FlashDumpDT(int nPort, int nTimeout, char* block_size);
NEPDLLAPI int __stdcall P21FlashDumpMSIPL(int nPort, int nTimeout, char* block_size);
NEPDLLAPI int __stdcall P21FlashDumpEboot(int nPort, int nTimeout, char* block_size);
NEPDLLAPI int __stdcall P21FlashDumpBDTL0(int nPort, int nTimeout, char* block_size);
NEPDLLAPI int __stdcall P21FlashDumpBDTL1(int nPort, int nTimeout, char* block_size);
NEPDLLAPI int __stdcall P21FlashDumpBDTL2(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21FlashDumpNGSMFFS(int nPort, int nTimeout, char* block_size);
NEPDLLAPI int __stdcall P21FlashDumpWGSMFFS(int nPort, int nTimeout, char* block_size);
NEPDLLAPI int __stdcall P21FlashDumpEbootConfig(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21FlashDumpFA(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21FlashDumpBDTLSectors(int nPort, int nTimeout, char* InBuffer, char* parameters);
NEPDLLAPI int __stdcall P21FlashDumpIPL(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21FlashDumpBDTL3(int nPort, int nTimeout, char* InBuffer);

//MDOC
NEPDLLAPI int __stdcall P21FlashExit(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21FlashFormatAll(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21FlashReadMDocID(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21FlashFormatBDTL8M2M(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21FlashFormatBDTL5M60M(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21FlashMDOCInfo(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21FlashFormatAllPCAssign(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21FlashFormatBDTLPCAssign(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21FlashCheckProtect(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21FlashGetServicePassword(int nPort, int nTimeout, char* OutBuffer);

//Mode Change
NEPDLLAPI int __stdcall P21UARTPacketMode(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21USBPacketMode(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21EngineerMode(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21301Mode(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21UARTConsoleMode(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21USBConsoleMode(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21LCDLogEnable(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21UARTLogEnable(int nPort, int nTimeout);

//WLan
NEPDLLAPI int __stdcall P21WLANPowerOn(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21WLANPowerOff(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21WLANSetWlanTestMode(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21WLANSetCmdLocalFrequecy(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21WLANSetCmdOutputPower(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21WLANSetCmdMACAddrToEEProm(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21WLANGetCmdMACAddrFromEEProm(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21WLANGetMTKChipID(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21WLANGetCmdPacketRxStatus(int nPort, int nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer);

//DL
// 2007-09-20 YTT
NEPDLLAPI int __stdcall P21DLCheckServicePasswd(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21DLVerifyChecksumFromFlash(int nPort, int nTimeout, char* InBuffer);

//OS
NEPDLLAPI int __stdcall P21OSRunNecleusOS(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21OSPowerOff(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21OSWatchDogReset(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21OSEraseNucleusGSMFFS(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21OSNucleusSaveGSMFFS(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21OSJumpToOS(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21OSRunPLDR(int nPort, int nTimeout);

//RD
// 2006-11-09 YTT
//replace bool to int
NEPDLLAPI int __stdcall P21PowerOnToUARTPacketMode(int nPort, int nTimeout, int DTRflag);
NEPDLLAPI int __stdcall GetMEPDUnlockCode(unsigned char * szUnlockCode, const unsigned char * mepd, unsigned int length, unsigned int easyDecode, char *FSN, char *IMEI); //Mark Hsieh 2008.2.27

// 2006-11-15 YTT
//export Load2nd,LoadDDR function to
NEPDLLAPI int __stdcall TILoadFileToDDR(int nPort, int nTimeout, char* TIDDRFile , int StartAddres );
NEPDLLAPI int __stdcall TILoad2nd(int nPort, int nTimeout, char* TI2ndFile  );

// 2006-11-15 YTT
//CallBack for getting data
NEPDLLAPI int __stdcall RegisterCallBackChar(int nPort, int nTimeout, char* RegCallBack);
NEPDLLAPI int __stdcall UnregisterCallBackChar(int nPort, int nTimeout, char* RegCallBack);
NEPDLLAPI int __stdcall ExactUSBTIJumpAddr(int nPort, int nTimeout, unsigned int StartAddr);
NEPDLLAPI int __stdcall ExactUSBTIJumpAddrEx(int nPort, int nTimeout, unsigned int StartAddr, char* workPath);
NEPDLLAPI int __stdcall USB2ndSendCommand(int nPort, int nTimeout, char* InBuffer);

//camera
NEPDLLAPI int __stdcall P21CameraPowerOn(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21CameraPowerOff(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21CameraQQVGA(int nPort, int nTimeout, char* filePath);
NEPDLLAPI int __stdcall P21CameraPreview(int nPort, int nTimeout, char* filePath);
NEPDLLAPI int __stdcall P21CameraVGA(int nPort, int nTimeout, char* filePath);

//SDRAM
NEPDLLAPI int __stdcall P21SDRAMReadWriteFull(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21SDRAMReadWritePartial(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21SDRAMLineTest(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21SDRAMGetID(int nPort, int nTimeout, char* OutBuffer);

//Factory Area
NEPDLLAPI int __stdcall P21FAWriteFactoryArea(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21FAReadFactoryArea(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21FAWriteIMEI(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21FAReadIMEI(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21FAWriteVBATOffset(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21FAReadVBATOffset(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21FAReadCountryCode(int nPort, int nTimeout, char* OutPath);
NEPDLLAPI int __stdcall P21FAWriteCountryCode(int nPort, int nTimeout, char* DLPath);
NEPDLLAPI int __stdcall P21FAWriteBTAddress(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21FAReadBTAddress(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21FAWrite301Flag(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21FARead301Flag(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21FAWriteIMEITemp(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21FAReadIMEITemp(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21FAWriteVersionInfo(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21FAReadVersionInfo(int nPort, int nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer);
NEPDLLAPI int __stdcall P21FABackupGSMFFSToFA(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21FARestoreGSMFFSFromFA(int nPort, int nTimeout);
NEPDLLAPI int __stdcall SetUIInfo(int nPort , char* UIInfo);
NEPDLLAPI int __stdcall P21FAWriteIMEIAndNLockkey(int nPort, int nTimeout, char* cImei, char* cLockKey);
NEPDLLAPI int __stdcall P21FAReadIMEIAndNLockkey(int nPort, int nTimeout, char* cImei, char* cLockKey);
NEPDLLAPI int __stdcall P21FAVBATCalibration(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21FAGetMEPDUnlockCode(int nPort, int nTimeout, char* szUnlockCode); //Mark Hsieh 2008.Feb.27

//Keypad
NEPDLLAPI int __stdcall P21KeypadPress(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21KeypadReadMatrix(int nPort, int nTimeout, char* OutBuffer);

// 2007-07-03 YTT
NEPDLLAPI int __stdcall P21FlashWriteMEPDSimLockFile(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21FlashWriteMEPDSimLockData(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21FlashWriteMEPDSimLockDataEX(int nPort, int nTimeout, char* InBuffer, char *MEPDFlag);

//// Just Test
NEPDLLAPI int __stdcall P21GenCMDTEST(int nPort, int nTimeout);
NEPDLLAPI int __stdcall P21GenCMDOutTEST(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall P21GenCMDInTEST(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall P21GenCMDInOutTEST(int nPort, int nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer);

