#include "DLLDefine.h"

static const int NV_DT_CMD_MAX_RETRY = 2;
HANDLE hProcess;
CString m_ModelName;



/******************************************************************************
 * Export/import function prototypes
 ******************************************************************************/
NEPDLLAPI int __stdcall NVDA_TestFtdMode(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall NVDA_AutoFtdMode(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall NVDA_CheckStationFlag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall NVDA_PwrOn2UartPktMode(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall NVDA_ClearTLineFlag(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall NVDA_UpdateImeiPackState_R(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall NVDA_KillAdb(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall NVDA_QueryMobileIDByPicasso(int nPort, int nTimeout, char* ErrorCode);

// 1-SD
//ytt
//// 2-CAMERA
//NEPDLLAPI int __stdcall NVDA_CAMERA_GetModuleId(int nPort, int nTimeout, char* OutBuffer); // cmd: 9
//NEPDLLAPI int __stdcall NVDA_CAMERA_GetSensorId(int nPort, int nTimeout, char* OutBuffer); // cmd: 10

//// 3-LCD
NEPDLLAPI int __stdcall NVDA_LCD_DisplayRGB(int nPort, int nTimeout, char* OutBuffer); // cmd: 1
NEPDLLAPI int __stdcall NVDA_LCD_Backlight_Test(int nPort, int nTimeout, char* OutBuffer); // cmd: 2
NEPDLLAPI int __stdcall NVDA_LCD_Backlight_On(int nPort, int nTimeout, char* OutBuffer); // cmd: 3
NEPDLLAPI int __stdcall NVDA_LCD_Backlight_Off(int nPort, int nTimeout, char* OutBuffer); // cmd: 4
NEPDLLAPI int __stdcall NVDA_LCD_Touch_Test(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 5
NEPDLLAPI int __stdcall NVDA_LCD_Touch_SelfTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 6
NEPDLLAPI int __stdcall NVDA_LCD_Touch_VddTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 7
NEPDLLAPI int __stdcall NVDA_LCD_Touch_PinFaultTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 8

// 4-SENSOR
NEPDLLAPI int __stdcall NVDA_SENSOR_GSENSORReadAcc(int nPort, int nTimeout, char* OutBuffer); // cmd:1
NEPDLLAPI int __stdcall NVDA_SENSOR_GSENSORReadID(int nPort, int nTimeout, char* OutBuffer);  // cmd:2
NEPDLLAPI int __stdcall NVDA_SENSOR_ECompassI2CTest(int nPort, int nTimeout, char* OutBuffer);// cmd:3
NEPDLLAPI int __stdcall NVDA_SENSOR_TouchID(int nPort, int nTimeout, char* OutBuffer);// cmd:4

// 5-AUDIO
NEPDLLAPI int __stdcall NVDA_AUDIO_Mic(int nPort, int nTimeout, char* OutBuffer);    //cmd:1
NEPDLLAPI int __stdcall NVDA_AUDIO_Headset(int nPort, int nTimeout, char* OutBuffer);//cmd:2
NEPDLLAPI int __stdcall NVDA_AUDIO_Speaker(int nPort, int nTimeout, char* OutBuffer);//cmd:3
NEPDLLAPI int __stdcall NVDA_AUDIO_I2CTest(int nPort, int nTimeout, char* OutBuffer);//cmd:4
NEPDLLAPI int __stdcall NVDA_AUDIO_JackCheck(int nPort, int nTimeout, char* OutBuffer);//cmd:5
NEPDLLAPI int __stdcall NVDA_AUDIO_HookCheck(int nPort, int nTimeout, char* OutBuffer);//cmd:6
NEPDLLAPI int __stdcall NVDA_AUDIO_LineOut(int nPort, int nTimeout, char* OutBuffer);//cmd:7
NEPDLLAPI int __stdcall NVDA_AUDIO_PlayMusic(int nPort, int nTimeout, char* OutBuffer);//cmd:8

// 6-MODEM
NEPDLLAPI int __stdcall NVDA_MODEM_GPSCWTest(int nPort, int nTimeout, char* OutBuffer); // cmd: 1
NEPDLLAPI int __stdcall NVDA_MODEM_GPSFreqStability(int nPort, int nTimeout, char* OutBuffer); // cmd: 2

// 7-BT
NEPDLLAPI int __stdcall NVDA_BT_TxOnAt(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 1
NEPDLLAPI int __stdcall NVDA_BT_TxOff(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 2

//// 8-FLASH
//NEPDLLAPI int __stdcall NVDA_FLASH_ReadWriteLoopTest(int nPort, int nTimeout, char* OutBuffer);
//NEPDLLAPI int __stdcall NVDA_FLASH_ReadPage(int nPort, int nTimeout, char* OutBuffer);
//NEPDLLAPI int __stdcall NVDA_FLASH_WritePage(int nPort, int nTimeout, char* OutBuffer);
//NEPDLLAPI int __stdcall NVDA_FLASH_ReadID(int nPort, int nTimeout, char* OutBuffer);
//NEPDLLAPI int __stdcall NVDA_FLASH_EraceBlock(int nPort, int nTimeout, char* OutBuffer);
//NEPDLLAPI int __stdcall NVDA_FLASH_FindImageStartingPage(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
//NEPDLLAPI int __stdcall NVDA_FLASH_MCPTest(int nPort, int nTimeout, char* OutBuffer);

//// 9-DDR
//NEPDLLAPI int __stdcall NVDA_DDR_SetEBI1Clkto99MHZ(int nPort, int nTimeout, char* OutBuffer);
//NEPDLLAPI int __stdcall NVDA_DDR_SetEBI1Clkto128MHZ(int nPort, int nTimeout, char* OutBuffer);
//NEPDLLAPI int __stdcall NVDA_DDR_ReadWriteTest(int nPort, int nTimeout, char* OutBuffer);
//NEPDLLAPI int __stdcall NVDA_DDR_ReadWriteLoopTest(int nPort, int nTimeout, char* OutBuffer);
//NEPDLLAPI int __stdcall NVDA_DDR_ReadAddr(int nPort, int nTimeout, char* OutBuffer);
//NEPDLLAPI int __stdcall NVDA_DDR_RoughTest(int nPort, int nTimeout, char* OutBuffer);

//// 10-RTC
//NEPDLLAPI int __stdcall NVDA_RTC_ReadYear(int nPort, int nTimeout, char* OutBuffer);
//NEPDLLAPI int __stdcall NVDA_RTC_ReadMonth(int nPort, int nTimeout, char* OutBuffer);
//NEPDLLAPI int __stdcall NVDA_RTC_ReadDay(int nPort, int nTimeout, char* OutBuffer);
//NEPDLLAPI int __stdcall NVDA_RTC_ReadHour(int nPort, int nTimeout, char* OutBuffer);
//NEPDLLAPI int __stdcall NVDA_RTC_ReadMinute(int nPort, int nTimeout, char* OutBuffer);
//NEPDLLAPI int __stdcall NVDA_RTC_ReadSecond(int nPort, int nTimeout, char* OutBuffer);
//NEPDLLAPI int __stdcall NVDA_RTC_ReadDayOfWeek(int nPort, int nTimeout, char* OutBuffer);
//NEPDLLAPI int __stdcall NVDA_RTC_WriteYear(int nPort, int nTimeout, char* InBuffer);
//NEPDLLAPI int __stdcall NVDA_RTC_WriteMonth(int nPort, int nTimeout, char* InBuffer);
//NEPDLLAPI int __stdcall NVDA_RTC_WriteDay(int nPort, int nTimeout, char* InBuffer);
//NEPDLLAPI int __stdcall NVDA_RTC_WriteHour(int nPort, int nTimeout, char* InBuffer);
//NEPDLLAPI int __stdcall NVDA_RTC_WriteMinute(int nPort, int nTimeout, char* InBuffer);
//NEPDLLAPI int __stdcall NVDA_RTC_WriteSecond(int nPort, int nTimeout, char* InBuffer);
//NEPDLLAPI int __stdcall NVDA_RTC_WriteDayOfWeek(int nPort, int nTimeout, char* InBuffer);
//NEPDLLAPI int __stdcall NVDA_RTC_Alert(int nPort, int nTimeout, char* InBuffer);
//NEPDLLAPI int __stdcall NVDA_RTC_Reset_Second(int nPort, int nTimeout, char* OutBuffer);

//// 11-USB

//// 12-CHARGER
//NEPDLLAPI int __stdcall NVDA_CHARGER_PreCharge(int nPort, int nTimeout, char* OutBuffer);  // cmd: 1
//NEPDLLAPI int __stdcall NVDA_CHARGER_FullCharge(int nPort, int nTimeout, char* OutBuffer); // cmd: 2
//NEPDLLAPI int __stdcall NVDA_CHARGER_StopCharge(int nPort, int nTimeout, char* OutBuffer); // cmd: 3
//NEPDLLAPI int __stdcall NVDA_CHARGER_Verify_IC(int nPort, int nTimeout, char* OutBuffer);       // cmd: 4
//NEPDLLAPI int __stdcall NVDA_CHARGER_Verify_TPS65023(int nPort, int nTimeout, char* OutBuffer); // cmd: 5
//NEPDLLAPI int __stdcall NVDA_CHARGER_EnableCharing(int nPort, int nTimeout, char* OutBuffer); // cmd: 6
//NEPDLLAPI int __stdcall NVDA_CHARGER_ReadVersion(int nPort, int nTimeout, char* OutBuffer); // cmd: 7
//NEPDLLAPI int __stdcall NVDA_CHARGER_500mACharge(int nPort, int nTimeout, char* OutBuffer); // cmd: 8

//// 13-KEYPAD
//NEPDLLAPI int __stdcall NVDA_KEYPAD_Detect(int nPort, int nTimeout, char* OutBuffer);

//// 14-VIBRATOR
NEPDLLAPI int __stdcall NVDA_VIBRATOR_Switch(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 1
NEPDLLAPI int __stdcall NVDA_VIBRATOR_RunningTime(int nPort, int nTimeout, char* OutBuffer); // cmd: 2

// 15-BATTERY
NEPDLLAPI int __stdcall NVDA_BATTERY_BattChargeOn(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 1
NEPDLLAPI int __stdcall NVDA_BATTERY_ChargingRecovery(int nPort, int nTimeout, char* OutBuffer); // cmd: 2
NEPDLLAPI int __stdcall NVDA_BATTERY_BattTemp(int nPort, int nTimeout, char* OutBuffer); // cmd: 3
NEPDLLAPI int __stdcall NVDA_BATTERY_PMUBattVoltage(int nPort, int nTimeout, char* OutBuffer); // cmd: 4
NEPDLLAPI int __stdcall NVDA_BATTERY_PMUBattCapacity(int nPort, int nTimeout, char* OutBuffer); // cmd: 5
NEPDLLAPI int __stdcall NVDA_BATTERY_GAGBattVoltage(int nPort, int nTimeout, char* OutBuffer); // cmd: 6
NEPDLLAPI int __stdcall NVDA_BATTERY_GAGBattCapacity(int nPort, int nTimeout, char* OutBuffer); // cmd: 7

//// 16-DOWNLOAD

//// 17-OS
//NEPDLLAPI int __stdcall NVDA_OS_JumpToAPPSBL(int nPort, int nTimeout, char* OutBuffer);

// 18-WLAN
NEPDLLAPI int __stdcall NVDA_WLAN_TxMaxPowerOnCh(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); //cmd:1
NEPDLLAPI int __stdcall NVDA_WLAN_TxMaxPowerOff(int nPort, int nTimeout, char* OutBuffer);                  //cmd:2
NEPDLLAPI int __stdcall NVDA_WLAN_LocalFreqOnCh(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);  //cmd:3
NEPDLLAPI int __stdcall NVDA_WLAN_LocalFreqOff(int nPort, int nTimeout, char* OutBuffer);                   //cmd:4
NEPDLLAPI int __stdcall NVDA_WLAN_TempTxMaxPowerOnCh(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); //cmd:5
NEPDLLAPI int __stdcall NVDA_WLAN_TempTxMaxPowerOff(int nPort, int nTimeout, char* OutBuffer);                  //cmd:6
NEPDLLAPI int __stdcall NVDA_WLAN_TempLocalFreqOnCh(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);  //cmd:7
NEPDLLAPI int __stdcall NVDA_WLAN_TempLocalFreqOff(int nPort, int nTimeout, char* OutBuffer);                   //cmd:8

// 19-LED
NEPDLLAPI int __stdcall NVDA_LED_BacklightOn(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); //cmd:1
NEPDLLAPI int __stdcall NVDA_LED_L01On(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); //cmd:2
NEPDLLAPI int __stdcall NVDA_LED_L02On(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); //cmd:3
NEPDLLAPI int __stdcall NVDA_LED_CapKeyOn(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); //cmd:4
NEPDLLAPI int __stdcall NVDA_LED_FlashOn(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); //cmd:5


// 20-ID
NEPDLLAPI int __stdcall NVDA_ID_ReadBoardID(int nPort, int nTimeout, char* OutBuffer); //cmd:1
NEPDLLAPI int __stdcall NVDA_ID_ReadHWID(int nPort, int nTimeout, char* OutBuffer); //cmd:2

// 21-Regulator
NEPDLLAPI int __stdcall NVDA_REGULATOR_LDO0On(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd:1
NEPDLLAPI int __stdcall NVDA_REGULATOR_LDO6On(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd:2
NEPDLLAPI int __stdcall NVDA_REGULATOR_LDO7On(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd:3
NEPDLLAPI int __stdcall NVDA_REGULATOR_LDO8On(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd:4
NEPDLLAPI int __stdcall NVDA_REGULATOR_GPIO1On(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd:5
NEPDLLAPI int __stdcall NVDA_REGULATOR_GPIO2On(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd:6
NEPDLLAPI int __stdcall NVDA_REGULATOR_GPIO3On(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd:7
NEPDLLAPI int __stdcall NVDA_REGULATOR_LDORecover(int nPort, int nTimeout, char* OutBuffer); // cmd:8
NEPDLLAPI int __stdcall NVDA_REGULATOR_LDOLog(int nPort, int nTimeout, char* OutBuffer); // cmd:9
NEPDLLAPI int __stdcall NVDA_REGULATOR_LDOAllOn(int nPort, int nTimeout, char* OutBuffer); //cmd:10
NEPDLLAPI int __stdcall NVDA_REGULATOR_LDO1V2(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd:11


// 22-WWAN
NEPDLLAPI int __stdcall NVDA_WWAN_3GCardDetect(int nPort, int nTimeout, char* OutBuffer); //cmd:1
NEPDLLAPI int __stdcall NVDA_WWAN_ReadIMEI(int nPort, int nTimeout, char* OutBuffer); //cmd:2
NEPDLLAPI int __stdcall NVDA_WWAN_ReadRSSI(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); //cmd:3
NEPDLLAPI int __stdcall NVDA_WWAN_Read3GModuleVersion(int nPort, int nTimeout, char* OutBuffer);  //cmd:4
NEPDLLAPI int __stdcall NVDA_WWAN_CheckSimLock(int nPort, int nTimeout, char* OutBuffer);  //cmd:5
NEPDLLAPI int __stdcall NVDA_WWAN_CheckNetworkLock(int nPort, int nTimeout, char* OutBuffer);  //cmd:6
NEPDLLAPI int __stdcall NVDA_WWAN_Read3GModuleType(int nPort, int nTimeout, char* OutBuffer);  //cmd:7
NEPDLLAPI int __stdcall NVDA_WWAN_IMEIBackUp(int nPort, int nTimeout, char* OutBuffer);  //cmd:8
NEPDLLAPI int __stdcall NVDA_WWAN_ReadFAIMEIBackUp(int nPort, int nTimeout, char* OutBuffer);  //cmd:14

// 23-RD

// 24-TV_MOUDLE
NEPDLLAPI int __stdcall NVDA_TV_ModuleDetect(int nPort, int nTimeout, char* OutBuffer); //cmd:1
NEPDLLAPI int __stdcall NVDA_TV_InBandPower(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); //cmd:2

// 25-VERSION
NEPDLLAPI int __stdcall NVDA_VERSION_ReadTestVersion(int nPort, int nTimeout, char* OutBuffer); //cmd:1

//// 26-FACTORY
NEPDLLAPI int __stdcall NVDA_FACTORY_FARead(int nPort, int nTimeout, char* OutBuffer); //cmd:1
NEPDLLAPI int __stdcall NVDA_FACTORY_FAWrite(int nPort, int nTimeout, char* InBuffer); //cmd:2
NEPDLLAPI int __stdcall NVDA_FACTORY_WritePicasso(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);  //cmd:24
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadPicasso(int nPort, int nTimeout, char* OutBuffer); //cmd:3
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadPicassoCheck(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadWifiMac(int nPort, int nTimeout, char* OutBuffer); //cmd:4
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteWifiMac(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); //cmd:5
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadBtMac(int nPort, int nTimeout, char* OutBuffer); //cmd:6
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteBtMac(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); //cmd:7
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadPPID(int nPort, int nTimeout, char* OutBuffer); //cmd:8
NEPDLLAPI int __stdcall NVDA_FACTORY_WritePPID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); //cmd:9
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadServiceTag(int nPort, int nTimeout, char* OutBuffer); //cmd:10
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteServiceTag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); //cmd:11
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteDLFlag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);  //cmd:14
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteFTFlag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);  //cmd:15
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteMMIBBFlag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);  //cmd:16
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteMMIRFFlag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);  //cmd:17
NEPDLLAPI int __stdcall NVDA_FACTORY_CheckAndMoveFA(int nPort, int nTimeout, char* OutBuffer); //cmd:19
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadRC(int nPort, int nTimeout, char* OutBuffer);  //cmd:25
NEPDLLAPI int __stdcall NVDA_FACTORY_ClearFA(int nPort, int nTimeout, char* OutBuffer);  //cmd:26
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadMmiErrorCode(int nPort, int nTimeout, char* OutBuffer); //cmd:29

// 27-HouseKeeping
NEPDLLAPI int __stdcall NVDA_HOUSEKEEP_PowerGlueLogicCheck(int nPort, int nTimeout, char* OutBuffer); //cmd:1