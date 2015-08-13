#include "DLLDefine.h"

static const int QC_DT_CMD_MAX_RETRY = 3;



/******************************************************************************
 * Export/import function prototypes
 ******************************************************************************/

NEPDLLAPI int __stdcall QUAL_QueryMobileIDByPicasso(int nPort, int nTimeout, char* ErrorCode);

NEPDLLAPI int __stdcall QUAL_PwrOn2UartPktMode(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_DummyInOut(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);

// 1-SD
NEPDLLAPI int __stdcall QUAL_SD_CheckExternal(int nPort, int nTimeout, char* OutBuffer); // cmd: 1

// 2-CAMERA
NEPDLLAPI int __stdcall QUAL_CAMERA_GetModuleId(int nPort, int nTimeout, char* OutBuffer); // cmd: 9
NEPDLLAPI int __stdcall QUAL_CAMERA_GetSensorId(int nPort, int nTimeout, char* OutBuffer); // cmd: 10

// 3-LCD
NEPDLLAPI int __stdcall QUAL_LCD_MDDIBootSplashScreen(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_LCD_MDDIDisplayRGB(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall QUAL_LCD_MDDIReset(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_LCD_MDDIInitLCD(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_LCD_ReadTPCoordinate(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_LCD_ReadTPCoordinate_new(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_LCD_ReadTPCoordinate_new_activelow(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_LCD_DisplayGIF(int nPort, int nTimeout, char* ErrorCode);

// 4-SENSOR
NEPDLLAPI int __stdcall QUAL_SENSOR_ALSOn(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_SENSOR_ALSOff(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_SENSOR_ALSRead(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_SENSOR_Proximity(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_SENSOR_PSSDEnable(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_SENSOR_PSSDDisable(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_SENSOR_GSENSORReadAcc(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_SENSOR_GSENSORReadID(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_SENSOR_InitCapSenseKey(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_SENSOR_ReadCapSenseKey(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_SENSOR_ECompassI2CTest(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_SENSOR_ECompassX(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_SENSOR_ECompassY(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_SENSOR_ECompassZ(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_SENSOR_GSensorCalibration(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_SENSOR_GSensorAverage(int nPort, int nTimeout, char* OutBuffer);

// 5-AUDIO
NEPDLLAPI int __stdcall QUAL_AUDIO_HandsetLoopback(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_AUDIO_HeadsetLoopback(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_AUDIO_LoudspeakerLoopback(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_AUDIO_HookKeyDetection(int nPort, int nTimeout, char* ErrorCode);

// 6-MODEM
NEPDLLAPI int __stdcall QUAL_MODEM_GSMPowerOn(int nPort, int nTimeout, char* ErrorCode); // cmd: 1
NEPDLLAPI int __stdcall QUAL_MODEM_GSMPowerOff(int nPort, int nTimeout, char* ErrorCode); // cmd: 2
NEPDLLAPI int __stdcall QUAL_MODEM_SIMPowerOn(int nPort, int nTimeout, char* ErrorCode); // cmd: 3
NEPDLLAPI int __stdcall QUAL_MODEM_SIMPowerOff(int nPort, int nTimeout, char* ErrorCode); // cmd: 4
NEPDLLAPI int __stdcall QUAL_MODEM_SIMTest(int nPort, int nTimeout, char* ErrorCode); // cmd: 5
NEPDLLAPI int __stdcall QUAL_MODEM_GPSTest(int nPort, int nTimeout, char* OutBuffer); // cmd: 6
NEPDLLAPI int __stdcall QUAL_MODEM_GPS_Read_I(int nPort, int nTimeout, char* OutBuffer); // cmd: 7
NEPDLLAPI int __stdcall QUAL_MODEM_GPS_Read_Q(int nPort, int nTimeout, char* OutBuffer); // cmd: 8

// 7-BT
NEPDLLAPI int __stdcall QUAL_BT_BTPowerOn(int nPort, int nTimeout, char* ErrorCode); // cmd: 1
NEPDLLAPI int __stdcall QUAL_BT_TxPower_2402MHz(int nPort, int nTimeout, char* ErrorCode); // cmd: 2
NEPDLLAPI int __stdcall QUAL_BT_TxPower_2441MHz(int nPort, int nTimeout, char* ErrorCode); // cmd: 3
NEPDLLAPI int __stdcall QUAL_BT_TxPower_2480MHz(int nPort, int nTimeout, char* ErrorCode); // cmd: 4
NEPDLLAPI int __stdcall QUAL_BT_TxPower_2438MHz(int nPort, int nTimeout, char* ErrorCode); // cmd: 5
NEPDLLAPI int __stdcall QUAL_BT_FMOn(int nPort, int nTimeout, char* ErrorCode); // cmd: 10
NEPDLLAPI int __stdcall QUAL_BT_BTRFTest(int nPort, int nTimeout, char* ErrorCode); // cmd: 12
NEPDLLAPI int __stdcall QUAL_BT_TxStop(int nPort, int nTimeout, char* ErrorCode); // cmd: 13
NEPDLLAPI int __stdcall QUAL_BT_FMReadRSSI(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);//14

// 8-FLASH
NEPDLLAPI int __stdcall QUAL_FLASH_ReadWriteLoopTest(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_FLASH_ReadPage(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_FLASH_WritePage(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_FLASH_ReadID(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_FLASH_EraceBlock(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_FLASH_FindImageStartingPage(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_FLASH_MCPTest(int nPort, int nTimeout, char* ErrorCode);

// 9-DDR
NEPDLLAPI int __stdcall QUAL_DDR_SetEBI1Clkto99MHZ(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_DDR_SetEBI1Clkto128MHZ(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_DDR_ReadWriteTest(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_DDR_ReadWriteLoopTest(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_DDR_ReadAddr(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_DDR_RoughTest(int nPort, int nTimeout, char* ErrorCode);

// 10-RTC
NEPDLLAPI int __stdcall QUAL_RTC_ReadYear(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_RTC_ReadMonth(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_RTC_ReadDay(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_RTC_ReadHour(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_RTC_ReadMinute(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_RTC_ReadSecond(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_RTC_ReadDayOfWeek(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_RTC_WriteYear(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall QUAL_RTC_WriteMonth(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall QUAL_RTC_WriteDay(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall QUAL_RTC_WriteHour(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall QUAL_RTC_WriteMinute(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall QUAL_RTC_WriteSecond(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall QUAL_RTC_WriteDayOfWeek(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall QUAL_RTC_Alert(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall QUAL_RTC_Reset_Second(int nPort, int nTimeout, char* ErrorCode);

// 11-USB

// 12-CHARGER
NEPDLLAPI int __stdcall QUAL_CHARGER_PreCharge(int nPort, int nTimeout, char* ErrorCode);  // cmd: 1
NEPDLLAPI int __stdcall QUAL_CHARGER_FullCharge(int nPort, int nTimeout, char* ErrorCode); // cmd: 2
NEPDLLAPI int __stdcall QUAL_CHARGER_StopCharge(int nPort, int nTimeout, char* ErrorCode); // cmd: 3
NEPDLLAPI int __stdcall QUAL_CHARGER_Verify_IC(int nPort, int nTimeout, char* OutBuffer);       // cmd: 4
NEPDLLAPI int __stdcall QUAL_CHARGER_Verify_TPS65023(int nPort, int nTimeout, char* OutBuffer); // cmd: 5
NEPDLLAPI int __stdcall QUAL_CHARGER_EnableCharing(int nPort, int nTimeout, char* ErrorCode); // cmd: 6
NEPDLLAPI int __stdcall QUAL_CHARGER_ReadVersion(int nPort, int nTimeout, char* OutBuffer); // cmd: 7
NEPDLLAPI int __stdcall QUAL_CHARGER_500mACharge(int nPort, int nTimeout, char* OutBuffer); // cmd: 8

// 13-KEYPAD
NEPDLLAPI int __stdcall QUAL_KEYPAD_Detect(int nPort, int nTimeout, char* ErrorCode);

// 14-VIBRATOR
NEPDLLAPI int __stdcall QUAL_VIBRATOR_On(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_VIBRATOR_Off(int nPort, int nTimeout, char* ErrorCode);

// 15-BATTERY
NEPDLLAPI int __stdcall QUAL_BATTERY_GAUGE_ReadTemperature(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_BATTERY_GAUGE_ReadVoltage(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_BATTERY_GAUGE_ReadStateOfCharge(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_BATTERY_GAUGE_ReadCurrent(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_BATTERY_GAUGE_Reset(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_BATTERY_BackupBatteryPowerLevel(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_BATTERY_BackupBatteryChargerOn(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_BATTERY_BackupBatteryChargerOff(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_BATTERY_BatteryCoverDetect(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_BATTERY_GAUGE_ReadVersion(int nPort, int nTimeout, char* OutBuffer); // cmd: 11
NEPDLLAPI int __stdcall QUAL_BATTERY_GAUGE_ReadDFIVersion(int nPort, int nTimeout, char* OutBuffer); // cmd: 12

// 16-DOWNLOAD
NEPDLLAPI int __stdcall QUAL_DOWNLOAD_FASTBOOT_MODE(int nPort, int nTimeout, char* ErrorCode);
//NEPDLLAPI int __stdcall QUAL_DOWNLOAD_USBDownloadImage(int nPort, int nTimeout, char* InBuffer)
NEPDLLAPI int __stdcall QUAL_DOWNLOAD_NewFlashWrite0XFF(int nPort, int nTimeout, char* InBuffer);
NEPDLLAPI int __stdcall QUAL_DOWNLOAD_NewFlashRead(int nPort, int nTimeout, char* OutBuffer);

// 17-OS
NEPDLLAPI int __stdcall QUAL_OS_JumpToAPPSBL(int nPort, int nTimeout, char* ErrorCode);

// 18-WLAN
NEPDLLAPI int __stdcall QUAL_WLAN_TxMaxPower(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_WLAN_TxMaxPower_CH01_2412MHz(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_WLAN_TxMaxPower_CH07_2442MHz(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_WLAN_TxMaxPower_CH13_2472MHz(int nPort, int nTimeout, char* ErrorCode);

// 19-LED
NEPDLLAPI int __stdcall QUAL_LED_KeypadOn(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_LED_KeypadOff(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_LED_BacklightOn(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_LED_BacklightOff(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_LED_FlashOn(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_LED_FlashOff(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_LED_CapSenseKeyOn(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall QUAL_LED_CapSenseKeyOff(int nPort, int nTimeout, char* ErrorCode);

// 20-ID
NEPDLLAPI int __stdcall QUAL_ID_ReadBoardID(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_ID_ReadHWID(int nPort, int nTimeout, char* OutBuffer);

// 23-RD
NEPDLLAPI int __stdcall QUAL_RD_LoadAPPSBL(int nPort, int nTimeout, char* ErrorCode);   // cmd: 1
NEPDLLAPI int __stdcall QUAL_RD_EnterUARTPktMode(int nPort, int nTimeout, char* ErrorCode); // cmd: 2
NEPDLLAPI int __stdcall QUAL_RD_EVT0AutoTest(int nPort, int nTimeout, char* ErrorCode); // cmd: 3
NEPDLLAPI int __stdcall QUAL_RD_SendSMEMCmd(int nPort, int nTimeout, char* ErrorCode);  // cmd: 4
NEPDLLAPI int __stdcall QUAL_RD_AudioAMP(int nPort, int nTimeout, char* ErrorCode);     // cmd: 5
NEPDLLAPI int __stdcall QUAL_RD_ReadMemoryAddress(int nPort, int nTimeout, char* OutBuffer); // cmd: 6
NEPDLLAPI int __stdcall QUAL_RD_ChangeHDMIAddress(int nPort, int nTimeout, char* ErrorCode); // cmd: 7
NEPDLLAPI int __stdcall QUAL_RD_ReadHDMIAddress(int nPort, int nTimeout, char* OutBuffer);   // cmd: 8
NEPDLLAPI int __stdcall QUAL_RD_GetPartitionInfo(int nPort, int nTimeout, char* OutBuffer);  // cmd: 9
NEPDLLAPI int __stdcall QUAL_RD_JumpToAPPSBL_new(int nPort, int nTimeout, char* ErrorCode);  // cmd: 10
NEPDLLAPI int __stdcall QUAL_RD_Reboot(int nPort, int nTimeout, char* ErrorCode);   // cmd: 11
NEPDLLAPI int __stdcall QUAL_RD_SetGP2Vol(int nPort, int nTimeout, char* InBuffer); // cmd: 12
NEPDLLAPI int __stdcall QUAL_RD_Enter301Mode(int nPort, int nTimeout, char* ErrorCode);  // cmd: 13
NEPDLLAPI int __stdcall QUAL_RD_JumpToAPPSBL_WriteFlag(int nPort, int nTimeout, char* ErrorCode); // cmd: 14
NEPDLLAPI int __stdcall QUAL_RD_ReadI2CStatus(int nPort, int nTimeout, char* OutBuffer); // cmd: 15
NEPDLLAPI int __stdcall QUAL_RD_I2CStressTest(int nPort, int nTimeout, char* ErrorCode); // cmd: 16
NEPDLLAPI int __stdcall QUAL_RD_GPIO7678_High(int nPort, int nTimeout, char* ErrorCode); // cmd: 17
NEPDLLAPI int __stdcall QUAL_RD_GPIO7678_Low(int nPort, int nTimeout, char* ErrorCode);  // cmd: 18
NEPDLLAPI int __stdcall QUAL_RD_LoopReadTouchCoordinate(int nPort, int nTimeout, char* ErrorCode); // cmd: 19
NEPDLLAPI int __stdcall QUAL_RD_PowerDown(int nPort, int nTimeout, char* ErrorCode); // cmd: 20
NEPDLLAPI int __stdcall QUAL_RD_HdmiTest(int nPort, int nTimeout, char* ErrorCode);  // cmd: 21
NEPDLLAPI int __stdcall QUAL_RD_PictureTest(int nPort, int nTimeout, char* ErrorCode);  // cmd: 22
NEPDLLAPI int __stdcall QUAL_RD_RecoveryMenu(int nPort, int nTimeout, char* ErrorCode); // cmd: 23
NEPDLLAPI int __stdcall QUAL_RD_AutoSetEbi1KernelAddr(int nPort, int nTimeout, char* ErrorCode); // cmd: 24
NEPDLLAPI int __stdcall QUAL_RD_ReadUartLogStatus(int nPort, int nTimeout, char* OutBuffer); // cmd: 25

// 25-VERSION
NEPDLLAPI int __stdcall QUAL_VERSION_ReadDTVersion(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_VERSION_ReadBoardID(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_VERSION_ReadAccessaryID(int nPort, int nTimeout, char* OutBuffer);

// 26-FACTORY
NEPDLLAPI int __stdcall QUAL_FACTORY_FARead(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall QUAL_FACTORY_FAWrite(int nPort, int nTimeout, char* InBuffer);

