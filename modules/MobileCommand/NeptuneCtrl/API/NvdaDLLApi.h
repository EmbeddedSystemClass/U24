#include "DLLDefine.h"

static const int NV_DT_CMD_MAX_RETRY = 2;

/******************************************************************************
 * Export/import function prototypes
 ******************************************************************************/
NEPDLLAPI bool __stdcall ExecAdbOut(CString Command, char* output, char* ErrorCode);

NEPDLLAPI int __stdcall NVDA_TestFtdMode(int nPort, int nTimeout, char* ErrorCode);
NEPDLLAPI int __stdcall NVDA_AutoFtdMode(int nPort, int nTimeout, char* ErrorCode);

NEPDLLAPI int __stdcall NVDA_CheckStationFlag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall NVDA_PwrOn2UartPktMode(int nPort, int nTimeout, char* ErrorCode);

/*********************************** FTD Communication Commands *******************************************/
// 1-SD
NEPDLLAPI int __stdcall NVDA_SD_DetectStatus(int nPort, int nTimeout, char* OutBuffer);		 // cmd: 1
NEPDLLAPI int __stdcall NVDA_SD_OperateTest(int nPort, int nTimeout, char* OutBuffer);		 // cmd: 2
NEPDLLAPI int __stdcall NVDA_SD_Mount(int nPort, int nTimeout, char* OutBuffer);			 // cmd: 3
NEPDLLAPI int __stdcall NVDA_SD_Unmount(int nPort, int nTimeout, char* OutBuffer);			 // cmd: 4
NEPDLLAPI int __stdcall NVDA_SD_CheckeMMCSetting(int nPort, int nTimeout, char* OutBuffer);  // cmd: 5
NEPDLLAPI int __stdcall NVDA_SD_GetInfo(int nPort, int nTimeout, char* OutBuffer);           // cmd: 6
NEPDLLAPI int __stdcall NVDA_SD_DeleteDCIM(int nPort, int nTimeout, char* OutBuffer);        // cmd: 7

// 2-CAMERA
NEPDLLAPI int __stdcall NVDA_CAMERA_ReadId(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 1
NEPDLLAPI int __stdcall NVDA_CAMERA_8MTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 2//8M
NEPDLLAPI int __stdcall NVDA_CAMERA_1D3MTest(int nPort, int nTimeout, char* OutBuffer);               // cmd: 3//1.3M
NEPDLLAPI int __stdcall NVDA_CAMERA_5MTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 4//5M
NEPDLLAPI int __stdcall NVDA_CAMERA_StoreInSDCard(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 5 select store location
NEPDLLAPI int __stdcall NVDA_CAMERA_ChooseWBMode(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 6 select WBMode
NEPDLLAPI int __stdcall NVDA_CAMERA_1D3MTest_New(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);  //cmd:3//2:capture,3:shutdown
NEPDLLAPI int __stdcall NVDA_CAMERA_ClearIndex(int nPort, int nTimeout, char* OutBuffer);  //cmd 7
// 3-Display
NEPDLLAPI int __stdcall NVDA_Display_LCDRGB(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);			// cmd: 1
NEPDLLAPI int __stdcall NVDA_Display_DetectHDMI(int nPort, int nTimeout, char* OutBuffer);						// cmd: 2
NEPDLLAPI int __stdcall NVDA_Display_HDMIRGB(int nPort, int nTimeout, char* OutBuffer);							// cmd: 3
NEPDLLAPI int __stdcall NVDA_Display_LCDRLE(int nPort, int nTimeout, char* InBuffer, char* p_sz_OutBuffer);		// cmd: 4
NEPDLLAPI int __stdcall NVDA_Display_CloseLCD(int nPort, int nTimeout, char* p_sz_OutBuffer);					// cmd: 5

// 4-SENSOR
NEPDLLAPI int __stdcall NVDA_SENSOR_GSENSORReadAcc(int nPort, int nTimeout, char* OutBuffer);			// cmd: 1
NEPDLLAPI int __stdcall NVDA_SENSOR_GSENSORReadID(int nPort, int nTimeout, char* OutBuffer);			// cmd: 2
NEPDLLAPI int __stdcall NVDA_SENSOR_ECompassI2CTest(int nPort, int nTimeout, char* OutBuffer);			// cmd: 3
NEPDLLAPI int __stdcall NVDA_SENSOR_TouchID(int nPort, int nTimeout, char* OutBuffer);					// cmd: 4
NEPDLLAPI int __stdcall NVDA_SENSOR_LSensorReadID(int nPort, int nTimeout, char* OutBuffer);			// cmd: 5
NEPDLLAPI int __stdcall NVDA_SENSOR_LSensorReadValue(int nPort, int nTimeout, char* OutBuffer);			// cmd: 6
NEPDLLAPI int __stdcall NVDA_SENSOR_PSensorReadID(int nPort, int nTimeout, char* OutBuffer);			// cmd: 7
NEPDLLAPI int __stdcall NVDA_SENSOR_PSensorReadProximity(int nPort, int nTimeout, char* OutBuffer);		// cmd: 8
NEPDLLAPI int __stdcall NVDA_SENSOR_GyroReadID(int nPort, int nTimeout, char* OutBuffer);				// cmd: 9
NEPDLLAPI int __stdcall NVDA_SENSOR_LSensorCalibration(int nPort, int nTimeout, char* OutBuffer);		// cmd: 10
NEPDLLAPI int __stdcall NVDA_SENSOR_PSensorFarCalibration(int nPort, int nTimeout, char* OutBuffer);	// cmd: 11
NEPDLLAPI int __stdcall NVDA_SENSOR_PSensorNearCalibration(int nPort, int nTimeout, char* OutBuffer);	// cmd: 12
NEPDLLAPI int __stdcall NVDA_SENSOR_GSensorCalibration(int nPort, int nTimeout, char* OutBuffer);		// cmd: 13
NEPDLLAPI int __stdcall NVDA_SENSOR_FcheckCalibration(int nPort, int nTimeout, char* OutBuffer);        // cmd: 14

// 5-AUDIO
NEPDLLAPI int __stdcall NVDA_AUDIO_Headset(int nPort, int nTimeout, char* OutBuffer);					// cmd: 1
NEPDLLAPI int __stdcall NVDA_AUDIO_Speaker(int nPort, int nTimeout, char* OutBuffer);					// cmd: 2
NEPDLLAPI int __stdcall NVDA_AUDIO_JackCheck(int nPort, int nTimeout, char* OutBuffer);					// cmd: 3
NEPDLLAPI int __stdcall NVDA_AUDIO_HookCheck(int nPort, int nTimeout, char* OutBuffer);					// cmd: 4
NEPDLLAPI int __stdcall NVDA_AUDIO_LineOut(int nPort, int nTimeout, char* OutBuffer);					// cmd: 5
NEPDLLAPI int __stdcall NVDA_AUDIO_PlayMusic(int nPort, int nTimeout, char* OutBuffer);					// cmd: 6
NEPDLLAPI int __stdcall NVDA_AUDIO_PlayTone(int nPort, int nTimeout, char* OutBuffer);					// cmd: 7
NEPDLLAPI int __stdcall NVDA_AUDIO_RecordWav(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 8
NEPDLLAPI int __stdcall NVDA_AUDIO_LoopTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);  // cmd: 9
NEPDLLAPI int __stdcall NVDA_AUDIO_SwitchRX(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);  // cmd: 10
NEPDLLAPI int __stdcall NVDA_AUDIO_SwitchTX(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);  // cmd: 11
NEPDLLAPI int __stdcall NVDA_AUDIO_Playwav(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);   // cmd: 12
NEPDLLAPI int __stdcall NVDA_AUDIO_ReadRCID(int nPort, int nTimeout, char* OutBuffer);					// cmd: 13

// 6-GPS
NEPDLLAPI int __stdcall NVDA_GPS_EnterSARFMode(int nPort, int nTimeout, char* OutBuffer);	    // cmd:1
NEPDLLAPI int __stdcall NVDA_GPS_CWTest(int nPort, int nTimeout, char* OutBuffer);				// cmd:2
NEPDLLAPI int __stdcall NVDA_GPS_FreqStability(int nPort, int nTimeout, char* OutBuffer);		// cmd:3
NEPDLLAPI int __stdcall NVDA_GPS_ExitSARFMode(int nPort, int nTimeout, char* OutBuffer);        // cmd:4

// 7-BT
NEPDLLAPI int __stdcall NVDA_BT_TxPowerOn(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall NVDA_BT_TxPowerOnChannel(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall NVDA_BT_TxPowerOff(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall NVDA_BT_TestModeOn(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall NVDA_BT_TestModeOff(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall NVDA_BT_ReadBTID(int nPort, int nTimeout, char* OutBuffer);


// 8-FMR
NEPDLLAPI int __stdcall NVDA_FMR_ReadRSSI(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);// cmd: 1
NEPDLLAPI int __stdcall NVDA_FMR_PathTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);// cmd: 2

// 9-SIM
NEPDLLAPI int __stdcall NVDA_SIM_CheckStatus(int nPort, int nTimeout, char* OutBuffer);				// cmd: 1
NEPDLLAPI int __stdcall NVDA_SIM_DetectHotPlug(int nPort, int nTimeout, char* OutBuffer);			// cmd: 2

// 10-CLK

// 11-USB

// 12-TOUCH
NEPDLLAPI int __stdcall NVDA_TOUCH_Test(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);					// cmd: 1
NEPDLLAPI int __stdcall NVDA_TOUCH_DownloadFirmware(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);      // cmd: 2
NEPDLLAPI int __stdcall NVDA_TOUCH_Cal_Test(int nPort, int nTimeout, char* OutBuffer);      // cmd: 3
NEPDLLAPI int __stdcall NVDA_TOUCH_Self_Test(int nPort, int nTimeout, char* OutBuffer);      // cmd: 5
NEPDLLAPI int __stdcall NVDA_TOUCH_ReadFlag(int nPort, int nTimeout, char* OutBuffer);		//cmd: 6
NEPDLLAPI int __stdcall NVDA_TOUCH_WriteFlag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); //cmd 7

// 13-KEYPAD
NEPDLLAPI int __stdcall NVDA_KEYPAD_Detect(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);   // cmd: 1
NEPDLLAPI int __stdcall NVDA_KEYPAD_GetCapkeyChipID(int nPort, int nTimeout, char* OutBuffer);			// cmd: 2
NEPDLLAPI int __stdcall NVDA_KEYPAD_GetCapkeyImpendanceValue(int nPort, int nTimeout, char* OutBuffer);	// cmd: 3

// 14-VIBRATOR
NEPDLLAPI int __stdcall NVDA_VIBRATOR_Switch(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 1
NEPDLLAPI int __stdcall NVDA_VIBRATOR_RunningTime(int nPort, int nTimeout, char* OutBuffer);			// cmd: 2

// 15-BATTERY
NEPDLLAPI int __stdcall NVDA_BATTERY_BatteryCharge(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);		 // cmd: 1
NEPDLLAPI int __stdcall NVDA_BATTERY_BatteryCurrent(int nPort, int nTimeout, char* OutBuffer);						 // cmd: 2
NEPDLLAPI int __stdcall NVDA_BATTERY_BatteryTemperature(int nPort, int nTimeout, char* OutBuffer);					 // cmd: 3
NEPDLLAPI int __stdcall NVDA_BATTERY_BatteryVoltage(int nPort, int nTimeout, char* OutBuffer);						 // cmd: 4
NEPDLLAPI int __stdcall NVDA_BATTERY_BatteryCapacity(int nPort, int nTimeout, char* OutBuffer);						 // cmd: 5
NEPDLLAPI int __stdcall NVDA_BATTERY_GaugeDFICalibration(int nPort, int nTimeout, char* OutBuffer);					 // cmd: 6
NEPDLLAPI int __stdcall NVDA_BATTERY_ReadGaugeID(int nPort, int nTimeout, char* OutBuffer);						     // cmd: 7
NEPDLLAPI int __stdcall NVDA_BATTERY_ReadChargerID(int nPort, int nTimeout, char* OutBuffer);					     // cmd: 8
NEPDLLAPI int __stdcall NVDA_BATTERY_CoinCellCharge(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);		 // cmd: 9
NEPDLLAPI int __stdcall NVDA_BATTERY_CoinCellVoltage(int nPort, int nTimeout, char* OutBuffer);						 // cmd: 10
NEPDLLAPI int __stdcall NVDA_BATTERY_DFIITEnable(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);			 // cmd: 11
NEPDLLAPI int __stdcall NVDA_BATTERY_DFIVersion(int nPort, int nTimeout, char* OutBuffer);			                 // cmd: 12
NEPDLLAPI int __stdcall NVDA_BATTERY_DFIITEnableStatus(int nPort, int nTimeout, char* OutBuffer);			         // cmd: 13

// 16-DOWNLOAD

// 17-OS

// 18-WLAN
NEPDLLAPI int __stdcall NVDA_WLAN_EnableTestMode(int nPort, int nTimeout, char* OutBuffer);						// cmd: 1
NEPDLLAPI int __stdcall NVDA_WLAN_EnableNormalMode(int nPort, int nTimeout, char* OutBuffer);					// cmd: 2
NEPDLLAPI int __stdcall NVDA_WLAN_Disable(int nPort, int nTimeout, char* OutBuffer);							// cmd: 3
NEPDLLAPI int __stdcall NVDA_WLAN_TxPowerSwitch(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);		// cmd: 4
NEPDLLAPI int __stdcall NVDA_WLAN_ScanAP(int nPort, int nTimeout, char* OutBuffer);								// cmd: 5
NEPDLLAPI int __stdcall NVDA_WLAN_GetScanResult(int nPort, int nTimeout, char* OutBuffer);						// cmd: 6
NEPDLLAPI int __stdcall NVDA_WLAN_ConnectAP(int nPort, int nTimeout, char* OutBuffer);							// cmd: 7
NEPDLLAPI int __stdcall NVDA_WLAN_GetRSSI(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);		    // cmd: 8
//NEPDLLAPI int __stdcall NVDA_WLAN_CheckWlanData(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall NVDA_WLAN_EnableRx(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall NVDA_WLAN_GetRxResult(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall NVDA_WLAN_LoadBin(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall NVDA_WLAN_SaveBin(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);

// 19-LED
NEPDLLAPI int __stdcall NVDA_LED_BacklightLEDTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);	// cmd: 1
NEPDLLAPI int __stdcall NVDA_LED_LEDRGBTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);			// cmd: 2
NEPDLLAPI int __stdcall NVDA_LED_CapKeyLEDTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);		// cmd: 3
NEPDLLAPI int __stdcall NVDA_LED_FlashLEDTest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);		// cmd: 4
NEPDLLAPI int __stdcall NVDA_LED_ReadFlashID(int nPort, int nTimeout, char* OutBuffer);

// 20-ID
NEPDLLAPI int __stdcall NVDA_ID_ReadHWID(int nPort, int nTimeout, char* OutBuffer);	   // cmd: 2

// 21-Regulator
NEPDLLAPI int __stdcall NVDA_REGULATOR_ReadREGValue(int nPort, int nTimeout, char* OutBuffer); // cmd:1

// 22-RF
NEPDLLAPI int __stdcall NVDA_WIRELESS_GSMTxPower(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);		// cmd: 1
NEPDLLAPI int __stdcall NVDA_WIRELESS_WCDMATxPower(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);   // cmd: 2
NEPDLLAPI int __stdcall NVDA_WIRELESS_TXPowerOFF(int nPort, int nTimeout, char* OutBuffer);						// cmd: 3

// 23-WWAN

// 24-TV_MOUDLE
NEPDLLAPI int __stdcall NVDA_TV_ModuleDetect(int nPort, int nTimeout, char* OutBuffer);				   // cmd: 1
NEPDLLAPI int __stdcall NVDA_TV_InBandPower(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 2

// 25-VERSION
NEPDLLAPI int __stdcall NVDA_VERSION_ReadSWVersion(int nPort, int nTimeout, char* OutBuffer);     // cmd: 1
NEPDLLAPI int __stdcall NVDA_VERSION_ReadDDRInfo(int nPort, int nTimeout, char* OutBuffer);       // cmd: 2
NEPDLLAPI int __stdcall NVDA_VERSION_ReadeMMCInfo(int nPort, int nTimeout, char* OutBuffer);      // cmd: 3
NEPDLLAPI int __stdcall NVDA_VERSION_ReadSDInfo(int nPort, int nTimeout, char* OutBuffer);        // cmd: 4
NEPDLLAPI int __stdcall NVDA_VERSION_ReadTouchInfo(int nPort, int nTimeout, char* OutBuffer);     // cmd: 5
NEPDLLAPI int __stdcall NVDA_VERSION_ReadCameraInfo(int nPort, int nTimeout, char* OutBuffer);    // cmd: 6
NEPDLLAPI int __stdcall NVDA_VERSION_ReadBTVersion(int nPort, int nTimeout, char* OutBuffer);     // cmd: 7
NEPDLLAPI int __stdcall NVDA_VERSION_ReadWIFIVersion(int nPort, int nTimeout, char* OutBuffer);   // cmd: 8
NEPDLLAPI int __stdcall NVDA_VERSION_ReadLCDInfo(int nPort, int nTimeout, char* OutBuffer);       // cmd: 9
NEPDLLAPI int __stdcall NVDA_VERSION_ReadGyroFWVersion(int nPort, int nTimeout, char* OutBuffer); // cmd: 10
NEPDLLAPI int __stdcall NVDA_VERSION_ReadRFVersion(int nPort, int nTimeout, char* OutBuffer);	  // cmd: 11
NEPDLLAPI int __stdcall NVDA_VERSION_ReadBBVersion(int nPort, int nTimeout, char* OutBuffer);     // cmd: 12
NEPDLLAPI int __stdcall NVDA_VERSION_ReadPMVersion(int nPort, int nTimeout, char* OutBuffer);     // cmd: 13

// 26-FACTORY
/* Alex.Chen Modify Start 2011/05/30 */
NEPDLLAPI int __stdcall NVDA_FACTORY_FARead(int nPort, int nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer);		// cmd: 1
NEPDLLAPI int __stdcall NVDA_FACTORY_FAWrite(int nPort, int nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer);		// cmd: 2
/* Alex.Chen Modify End 2011/05/30 */
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadPicasso(int nPort, int nTimeout, char* OutBuffer);										// cmd: 3
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadPicassoCheck(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadWifiMac(int nPort, int nTimeout, char* OutBuffer);										// cmd: 4
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteWifiMac(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);					// cmd: 5
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadBtMac(int nPort, int nTimeout, char* OutBuffer);										// cmd: 6
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteBtMac(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);						// cmd: 7
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadPPID(int nPort, int nTimeout, char* OutBuffer);										// cmd: 8
NEPDLLAPI int __stdcall NVDA_FACTORY_WritePPID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);						// cmd: 9
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadServiceTag(int nPort, int nTimeout, char* OutBuffer);									// cmd: 10
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteServiceTag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);					// cmd: 11
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteDLFlag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);						// cmd: 14
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteFTFlag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);						// cmd: 15
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteMMIBBFlag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);					// cmd: 16
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteMMIRFFlag(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);					// cmd: 17
NEPDLLAPI int __stdcall NVDA_FACTORY_CheckAndMoveFA(int nPort, int nTimeout, char* OutBuffer);									// cmd: 19
NEPDLLAPI int __stdcall NVDA_FACTORY_WritePicasso(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);					// cmd: 24
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadIMEI(int nPort, int nTimeout, char* OutBuffer);										// cmd: 28
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteIMEI(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);						// cmd: 29
NEPDLLAPI int __stdcall NVDA_FACTORY_ResetPhone(int nPort, int nTimeout, char* OutBuffer);										// cmd: 30
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteFCFlag(int nPort,int nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer);	// cmd: 31
NEPDLLAPI int __stdcall NVDA_FACTORY_SetBootMode(int nPort, int nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer);	// cmd: 34
NEPDLLAPI int __stdcall NVDA_FACTORY_EFS_Sync(int nPort,int nTimeout,char* OutBuffer);											// cmd:	35					
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadLCSN(int nPort, int nTimeout, char* OutBuffer);										// cmd: 36
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteLCSN(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);						// cmd: 37
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteOTP(int nPort,int nTimeout,char* OutBuffer);											// cmd: 39
NEPDLLAPI int __stdcall NVDA_FACTORY_CheckOTP(int nPort,int nTimeout,char* OutBuffer);											// cmd: 40
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadMEID(int nPort, int nTimeout, char* OutBuffer);										// cmd: 41
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteMEID(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);						// cmd: 42
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadPhoneTime(int nPort, int nTimeout, char* OutBuffer);									// cmd: 43
NEPDLLAPI int __stdcall NVDA_FACTORY_WritePhoneTime(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);					// cmd: 44
NEPDLLAPI int __stdcall NVDA_FACTORY_UnlockPartition(int nPort, int nTimeout, char* OutBuffer);                                 // cmd: 45
NEPDLLAPI int __stdcall NVDA_FACTORY_ClearProductionTime(int nPort, int nTimeout, char* OutBuffer);                             // cmd: 46
NEPDLLAPI int __stdcall NVDA_FACTORY_ClearItemOnNORMALPartition(int nPort, int nTimeout, char* OutBuffer);                      // cmd: 47
NEPDLLAPI int __stdcall NVDA_FACTORY_TriggerNVrestore(int nPort, int nTimeout, char* OutBuffer);                                // cmd: 48
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadSN(int nPort, int nTimeout, char* OutBuffer);										    // cmd: 49
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteSN(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);						    // cmd: 50
NEPDLLAPI int __stdcall NVDA_FACTORY_ResetPhone_Dubbai(int nPort, int nTimeout, char* OutBuffer);                               // cmd: 51
NEPDLLAPI int __stdcall NVDA_FACTORY_UnlockSPC_ForSPU(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);                // cmd: 52
NEPDLLAPI int __stdcall NVDA_FACTORY_ProgramSPC_ForSPU(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);               // cmd: 53
NEPDLLAPI int __stdcall NVDA_FACTORY_IsSPCLocked_ForSPU (int nPort, int nTimeout, char* OutBuffer);                             // cmd: 54
NEPDLLAPI int __stdcall NVDA_FACTORY_FileOperation(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);                   // cmd: 55
NEPDLLAPI int __stdcall NVDA_FACTORY_ReadIMEI_Second(int nPort, int nTimeout, char* OutBuffer);                                 // cmd: 56
NEPDLLAPI int __stdcall NVDA_FACTORY_WriteIMEI_Second(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);                // cmd: 57
// 27-HouseKeeping
NEPDLLAPI int __stdcall NVDA_HOUSEKEEP_PowerGlueLogicCheck(int nPort, int nTimeout, char* OutBuffer);			// cmd: 1

// 28-EFS

// 29-NV
NEPDLLAPI int __stdcall NVDA_NV_ReadNV(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);							// cmd: 1
NEPDLLAPI int __stdcall NVDA_NV_WriteNV(int nPort, int nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer);       // cmd: 2
NEPDLLAPI int __stdcall NVDA_NV_WriteInitNV(int nPort, int nTimeout, char* OutBuffer); // add by Tom.Li                                     // cmd: 3
//BackupNV: you can input NULL or station, date, time into in buffer in order to record NV info.
NEPDLLAPI int __stdcall NVDA_NV_BackupNV(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);							// cmd: 3
NEPDLLAPI int __stdcall NVDA_NV_CheckBackupNV(int nPort, int nTimeout, char* OutBuffer);									// cmd: 5

// 30-DIAG
NEPDLLAPI int __stdcall NVDA_DIAG_SetRequest(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);   // cmd: 1
NEPDLLAPI int __stdcall NVDA_RTC_Read(int nPort, int nTimeout,char* OutBuffer);                      // cmd: 1
NEPDLLAPI int __stdcall NVDA_RTC_Write(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);   // cmd: 2

//32-DRM
NEPDLLAPI int __stdcall NVDA_DRM_Write(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
NEPDLLAPI int __stdcall NVDA_DRM_Read(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);

//33-Efuse
NEPDLLAPI int __stdcall NVDA_EFUSE_Read(int nPort, int nTimeout, char* OutBuffer);
NEPDLLAPI int __stdcall NVDA_EFUSE_Write(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);


NEPDLLAPI bool ExecFastbootOut(CString Command,char *OutMsg, char* ErrorCode, int i_timeout);
