#include "DLLDefine.h"

HANDLE hProcess;
CString m_ModelName;


/******************************************************************************
 * Export/import function prototypes
 ******************************************************************************/
NEPDLLAPI int __stdcall NVDA_TestFtdMode(int port, int timeout_ms, char* error_msg);
NEPDLLAPI int __stdcall NVDA_AutoFtdMode(int port, int timeout_ms, char* error_msg);
NEPDLLAPI int __stdcall NVDA_CheckStationFlag(int port, int timeout_ms, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall NVDA_PwrOn2UartPktMode(int port, int timeout_ms, char* ErrorCode);
NEPDLLAPI int __stdcall NVDA_ClearTLineFlag(int port, int timeout_ms, char* ErrorCode);
NEPDLLAPI int __stdcall NVDA_UpdateImeiPackState_R(int port, int timeout_ms, char* ErrorCode);
NEPDLLAPI int __stdcall NVDA_KillAdb(int port, int timeout_ms, char* ErrorCode);
NEPDLLAPI int __stdcall NVDA_QueryMobileIDByPicasso(int port, int timeout_ms, char* ErrorCode);

// 1-SD
//ytt
// 2-CAMERA

//// 3-LCD
NEPDLLAPI int __stdcall NVDA_LCD_DisplayRGB(int port, int timeout_ms, char* OutBuffer); // cmd: 1
NEPDLLAPI int __stdcall NVDA_LCD_Backlight_Test(int port, int timeout_ms, char* OutBuffer); // cmd: 2
NEPDLLAPI int __stdcall NVDA_LCD_Backlight_On(int port, int timeout_ms, char* OutBuffer); // cmd: 3
NEPDLLAPI int __stdcall NVDA_LCD_Backlight_Off(int port, int timeout_ms, char* OutBuffer); // cmd: 4
NEPDLLAPI int __stdcall NVDA_LCD_Touch_Test(int port, int timeout_ms, char* InBuffer, char* OutBuffer); // cmd: 5
NEPDLLAPI int __stdcall NVDA_LCD_Touch_SelfTest(int port, int timeout_ms, char* InBuffer, char* OutBuffer); // cmd: 6
NEPDLLAPI int __stdcall NVDA_LCD_Touch_VddTest(int port, int timeout_ms, char* InBuffer, char* OutBuffer); // cmd: 7
NEPDLLAPI int __stdcall NVDA_LCD_Touch_PinFaultTest(int port, int timeout_ms, char* InBuffer, char* OutBuffer); // cmd: 8

// 4-SENSOR
NEPDLLAPI int __stdcall NVDA_SENSOR_GSENSORReadAcc(int port, int timeout_ms, char* OutBuffer); // cmd:1
NEPDLLAPI int __stdcall NVDA_SENSOR_GSENSORReadID(int port, int timeout_ms, char* OutBuffer);  // cmd:2
NEPDLLAPI int __stdcall NVDA_SENSOR_ECompassI2CTest(int port, int timeout_ms, char* OutBuffer);// cmd:3
NEPDLLAPI int __stdcall NVDA_SENSOR_TouchID(int port, int timeout_ms, char* OutBuffer);// cmd:4

// 5-AUDIO
NEPDLLAPI int __stdcall NVDA_AUDIO_Mic(int port, int timeout_ms, char* OutBuffer);    //cmd:1
NEPDLLAPI int __stdcall NVDA_AUDIO_Headset(int port, int timeout_ms, char* OutBuffer);//cmd:2
NEPDLLAPI int __stdcall NVDA_AUDIO_Speaker(int port, int timeout_ms, char* OutBuffer);//cmd:3
NEPDLLAPI int __stdcall NVDA_AUDIO_I2CTest(int port, int timeout_ms, char* OutBuffer);//cmd:4
NEPDLLAPI int __stdcall NVDA_AUDIO_JackCheck(int port, int timeout_ms, char* OutBuffer);//cmd:5
NEPDLLAPI int __stdcall NVDA_AUDIO_HookCheck(int port, int timeout_ms, char* OutBuffer);//cmd:6
NEPDLLAPI int __stdcall NVDA_AUDIO_LineOut(int port, int timeout_ms, char* OutBuffer);//cmd:7
NEPDLLAPI int __stdcall NVDA_AUDIO_PlayMusic(int port, int timeout_ms, char* OutBuffer);//cmd:8

// 6-MODEM
NEPDLLAPI int __stdcall NVDA_MODEM_GPSCWTest(int port, int timeout_ms, char* OutBuffer); // cmd: 1
NEPDLLAPI int __stdcall NVDA_MODEM_GPSFreqStability(int port, int timeout_ms, char* OutBuffer); // cmd: 2

// 7-BT
NEPDLLAPI int __stdcall NVDA_BT_TxOnAt(int port, int timeout_ms, char* InBuffer, char* OutBuffer); // cmd: 1
NEPDLLAPI int __stdcall NVDA_BT_TxOff(int port, int timeout_ms, char* InBuffer, char* OutBuffer); // cmd: 2

// 8-FLASH
// 9-DDR
// 10-RTC
// 11-USB
// 12-CHARGER
// 13-KEYPAD

// 14-VIBRATOR
NEPDLLAPI int __stdcall NVDA_VIBRATOR_Switch(int port, int timeout_ms, char* InBuffer, char* OutBuffer); // cmd: 1
NEPDLLAPI int __stdcall NVDA_VIBRATOR_RunningTime(int port, int timeout_ms, char* OutBuffer); // cmd: 2

// 15-BATTERY
NEPDLLAPI int __stdcall NVDA_BATTERY_BattChargeOn(int port, int timeout_ms, char* InBuffer, char* OutBuffer); // cmd: 1
NEPDLLAPI int __stdcall NVDA_BATTERY_ChargingRecovery(int port, int timeout_ms, char* OutBuffer); // cmd: 2
NEPDLLAPI int __stdcall NVDA_BATTERY_BattTemp(int port, int timeout_ms, char* OutBuffer); // cmd: 3
NEPDLLAPI int __stdcall NVDA_BATTERY_PMUBattVoltage(int port, int timeout_ms, char* OutBuffer); // cmd: 4
NEPDLLAPI int __stdcall NVDA_BATTERY_PMUBattCapacity(int port, int timeout_ms, char* OutBuffer); // cmd: 5
NEPDLLAPI int __stdcall NVDA_BATTERY_GAGBattVoltage(int port, int timeout_ms, char* OutBuffer); // cmd: 6
NEPDLLAPI int __stdcall NVDA_BATTERY_GAGBattCapacity(int port, int timeout_ms, char* OutBuffer); // cmd: 7

// 16-DOWNLOAD
// 17-OS

// 18-WLAN
NEPDLLAPI int __stdcall NVDA_WLAN_TxMaxPowerOnCh(int port, int timeout_ms, char* InBuffer, char* OutBuffer); //cmd:1
NEPDLLAPI int __stdcall NVDA_WLAN_TxMaxPowerOff(int port, int timeout_ms, char* OutBuffer);                  //cmd:2
NEPDLLAPI int __stdcall NVDA_WLAN_LocalFreqOnCh(int port, int timeout_ms, char* InBuffer, char* OutBuffer);  //cmd:3
NEPDLLAPI int __stdcall NVDA_WLAN_LocalFreqOff(int port, int timeout_ms, char* OutBuffer);                   //cmd:4
NEPDLLAPI int __stdcall NVDA_WLAN_TempTxMaxPowerOnCh(int port, int timeout_ms, char* InBuffer, char* OutBuffer); //cmd:5
NEPDLLAPI int __stdcall NVDA_WLAN_TempTxMaxPowerOff(int port, int timeout_ms, char* OutBuffer);                  //cmd:6
NEPDLLAPI int __stdcall NVDA_WLAN_TempLocalFreqOnCh(int port, int timeout_ms, char* InBuffer, char* OutBuffer);  //cmd:7
NEPDLLAPI int __stdcall NVDA_WLAN_TempLocalFreqOff(int port, int timeout_ms, char* OutBuffer);                   //cmd:8

// 19-LED
NEPDLLAPI int __stdcall NVDA_LED_BacklightOn(int port, int timeout_ms, char* InBuffer, char* OutBuffer); //cmd:1
NEPDLLAPI int __stdcall NVDA_LED_L01On(int port, int timeout_ms, char* InBuffer, char* OutBuffer); //cmd:2
NEPDLLAPI int __stdcall NVDA_LED_L02On(int port, int timeout_ms, char* InBuffer, char* OutBuffer); //cmd:3
NEPDLLAPI int __stdcall NVDA_LED_CapKeyOn(int port, int timeout_ms, char* InBuffer, char* OutBuffer); //cmd:4
NEPDLLAPI int __stdcall NVDA_LED_FlashOn(int port, int timeout_ms, char* InBuffer, char* OutBuffer); //cmd:5

// 20-ID
NEPDLLAPI int __stdcall NVDA_ID_ReadBoardID(int port, int timeout_ms, char* OutBuffer); //cmd:1
NEPDLLAPI int __stdcall NVDA_ID_ReadHWID(int port, int timeout_ms, char* OutBuffer); //cmd:2

// 21-Regulator
NEPDLLAPI int __stdcall NVDA_REGULATOR_LDO0On(int port, int timeout_ms, char* InBuffer, char* OutBuffer); // cmd:1
NEPDLLAPI int __stdcall NVDA_REGULATOR_LDO6On(int port, int timeout_ms, char* InBuffer, char* OutBuffer); // cmd:2
NEPDLLAPI int __stdcall NVDA_REGULATOR_LDO7On(int port, int timeout_ms, char* InBuffer, char* OutBuffer); // cmd:3
NEPDLLAPI int __stdcall NVDA_REGULATOR_LDO8On(int port, int timeout_ms, char* InBuffer, char* OutBuffer); // cmd:4
NEPDLLAPI int __stdcall NVDA_REGULATOR_GPIO1On(int port, int timeout_ms, char* InBuffer, char* OutBuffer); // cmd:5
NEPDLLAPI int __stdcall NVDA_REGULATOR_GPIO2On(int port, int timeout_ms, char* InBuffer, char* OutBuffer); // cmd:6
NEPDLLAPI int __stdcall NVDA_REGULATOR_GPIO3On(int port, int timeout_ms, char* InBuffer, char* OutBuffer); // cmd:7
NEPDLLAPI int __stdcall NVDA_REGULATOR_LDORecover(int port, int timeout_ms, char* OutBuffer); // cmd:8
NEPDLLAPI int __stdcall NVDA_REGULATOR_LDOLog(int port, int timeout_ms, char* OutBuffer); // cmd:9
NEPDLLAPI int __stdcall NVDA_REGULATOR_LDOAllOn(int port, int timeout_ms, char* OutBuffer); //cmd:10
NEPDLLAPI int __stdcall NVDA_REGULATOR_LDO1V2(int port, int timeout_ms, char* InBuffer, char* OutBuffer); // cmd:11

// 22-WWAN
NEPDLLAPI int __stdcall NVDA_WWAN_3GCardDetect(int port, int timeout_ms, char* OutBuffer); //cmd:1
NEPDLLAPI int __stdcall NVDA_WWAN_ReadIMEI(int port, int timeout_ms, char* OutBuffer); //cmd:2
NEPDLLAPI int __stdcall NVDA_WWAN_ReadRSSI(int port, int timeout_ms, char* InBuffer, char* OutBuffer); //cmd:3
NEPDLLAPI int __stdcall NVDA_WWAN_Read3GModuleVersion(int port, int timeout_ms, char* OutBuffer);  //cmd:4
NEPDLLAPI int __stdcall NVDA_WWAN_CheckSimLock(int port, int timeout_ms, char* OutBuffer);  //cmd:5
NEPDLLAPI int __stdcall NVDA_WWAN_CheckNetworkLock(int port, int timeout_ms, char* OutBuffer);  //cmd:6
NEPDLLAPI int __stdcall NVDA_WWAN_Read3GModuleType(int port, int timeout_ms, char* OutBuffer);  //cmd:7
NEPDLLAPI int __stdcall NVDA_WWAN_IMEIBackUp(int port, int timeout_ms, char* OutBuffer);  //cmd:8
NEPDLLAPI int __stdcall NVDA_WWAN_ReadFAIMEIBackUp(int port, int timeout_ms, char* OutBuffer);  //cmd:14

// 23-RD

// 24-TV_MOUDLE
NEPDLLAPI int __stdcall NVDA_TV_ModuleDetect(int port, int timeout_ms, char* OutBuffer); //cmd:1
NEPDLLAPI int __stdcall NVDA_TV_InBandPower(int port, int timeout_ms, char* InBuffer, char* OutBuffer); //cmd:2

// 25-VERSION
NEPDLLAPI int __stdcall NVDA_VERSION_ReadTestVersion(int port, int timeout_ms, char* OutBuffer); //cmd:1

// 26-FACTORY
NEPDLLAPI int __stdcall NVDA_FACTORY_FARead(int port, int timeout_ms, char* OutBuffer); //cmd:1
NEPDLLAPI int __stdcall NVDA_FACTORY_FAWrite(int port, int timeout_ms, char* InBuffer); //cmd:2
NEPDLLAPI int __stdcall NVDA_FACTORY_WritePicasso(int port, int timeout_ms, char* InBuffer, char* OutBuffer);  //cmd:24
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadPicasso(int port, int timeout_ms, char* OutBuffer); //cmd:3
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadPicassoCheck(int port, int timeout_ms, char* OutBuffer);
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadWifiMac(int port, int timeout_ms, char* OutBuffer); //cmd:4
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteWifiMac(int port, int timeout_ms, char* InBuffer, char* OutBuffer); //cmd:5
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadBtMac(int port, int timeout_ms, char* OutBuffer); //cmd:6
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteBtMac(int port, int timeout_ms, char* InBuffer, char* OutBuffer); //cmd:7
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadPPID(int port, int timeout_ms, char* OutBuffer); //cmd:8
NEPDLLAPI int __stdcall NVDA_FACTORY_WritePPID(int port, int timeout_ms, char* InBuffer, char* OutBuffer); //cmd:9
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadServiceTag(int port, int timeout_ms, char* OutBuffer); //cmd:10
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteServiceTag(int port, int timeout_ms, char* InBuffer, char* OutBuffer); //cmd:11
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteDLFlag(int port, int timeout_ms, char* InBuffer, char* OutBuffer);  //cmd:14
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteFTFlag(int port, int timeout_ms, char* InBuffer, char* OutBuffer);  //cmd:15
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteMMIBBFlag(int port, int timeout_ms, char* InBuffer, char* OutBuffer);  //cmd:16
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteMMIRFFlag(int port, int timeout_ms, char* InBuffer, char* OutBuffer);  //cmd:17
NEPDLLAPI int __stdcall NVDA_FACTORY_CheckAndMoveFA(int port, int timeout_ms, char* OutBuffer); //cmd:19
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadRC(int port, int timeout_ms, char* OutBuffer);  //cmd:25
NEPDLLAPI int __stdcall NVDA_FACTORY_ClearFA(int port, int timeout_ms, char* OutBuffer);  //cmd:26
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadMmiErrorCode(int port, int timeout_ms, char* OutBuffer); //cmd:29

// 27-HouseKeeping
NEPDLLAPI int __stdcall NVDA_HOUSEKEEP_PowerGlueLogicCheck(int port, int timeout_ms, char* OutBuffer); //cmd:1
