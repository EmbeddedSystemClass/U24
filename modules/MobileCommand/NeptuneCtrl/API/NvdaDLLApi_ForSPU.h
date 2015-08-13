/*********************************************************************************
 * COPYRIGHT Qisda WGC18 2011
 *
 * Description: .h file of Nvda Command DLL module for SPU Script use.
 * FileName   : NvdaDLLApi_ForSPU.h
 * Version    : $Revision:   1.22  $
 * Author     : Gilpin.Xi
 * Time       : 2011/03/11
 * History    :
 *  NO      Date        Updated by				Content of change
 *  1    2011/03/11      Gilpin.Xi					Create.
 *  2    2011/07/22      Gilpin.Xi			Modify for Docunment update
 ********************************************************************************/

#include "NvdaDLLApi.h"

/******************************************************************************
 * Export/import function prototypes
 ******************************************************************************/
NEPDLLAPI bool InitDevice_ForSPU(char* PortNum, char* nTimeout, char* ErrorCode);
NEPDLLAPI bool FreeDevice_ForSPU(char* PortNum, char* nTimeout, char* ErrorCode);

NEPDLLAPI bool EFS_ReadFile_ForSPU(char* sFileSrc, char* sFileDst, char* ErrMsg,char* ErrorCode);

// 1-SD
NEPDLLAPI bool NVDA_SD_DetectStatus_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode); // cmd: 1
NEPDLLAPI bool NVDA_SD_OperateTest_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);  // cmd: 2

// 2-CAMERA
NEPDLLAPI bool NVDA_CAMERA_ReadId_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode); // cmd: 1
NEPDLLAPI bool NVDA_CAMERA_8MTest_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode); // cmd: 2//8M
NEPDLLAPI bool NVDA_CAMERA_1D3MTest_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);			     // cmd: 3//1.3M
NEPDLLAPI bool NVDA_CAMERA_5MTest_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode); // cmd: 4//5M

// 3-Display
NEPDLLAPI bool NVDA_Display_LCDRGB_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode); // cmd: 1
NEPDLLAPI bool NVDA_Display_DetectHDMI_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);			  // cmd: 2
NEPDLLAPI bool NVDA_Display_HDMIRGB_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);				  // cmd: 3
NEPDLLAPI bool NVDA_Display_LCDRLE_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode); // cmd: 4
NEPDLLAPI bool NVDA_Display_CloseLCD_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);				  // cmd: 5

// 4-SENSOR
NEPDLLAPI bool NVDA_SENSOR_GSENSORReadAcc_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);		  // cmd: 1
NEPDLLAPI bool NVDA_SENSOR_GSENSORReadID_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);			  // cmd: 2
NEPDLLAPI bool NVDA_SENSOR_ECompassI2CTest_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);		  // cmd: 3
NEPDLLAPI bool NVDA_SENSOR_TouchID_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);				  // cmd: 4
NEPDLLAPI bool NVDA_SENSOR_LSensorReadID_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);			  // cmd: 5
NEPDLLAPI bool NVDA_SENSOR_LSensorReadValue_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);		  // cmd: 6
NEPDLLAPI bool NVDA_SENSOR_PSensorReadID_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);			  // cmd: 7
NEPDLLAPI bool NVDA_SENSOR_PSensorReadProximity_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);	  // cmd: 8
NEPDLLAPI bool NVDA_SENSOR_GyroReadID_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);			  // cmd: 9
NEPDLLAPI bool NVDA_SENSOR_LSensorCalibration_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);	  // cmd: 10
NEPDLLAPI bool NVDA_SENSOR_PSensorFarCalibration_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);	  // cmd: 11
NEPDLLAPI bool NVDA_SENSOR_PSensorNearCalibration_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);  // cmd: 12
NEPDLLAPI bool NVDA_SENSOR_GSensorCalibration_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);	  // cmd: 13

// 5-AUDIO
NEPDLLAPI bool NVDA_AUDIO_Headset_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);					// cmd: 1
NEPDLLAPI bool NVDA_AUDIO_Speaker_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);					// cmd: 2
NEPDLLAPI bool NVDA_AUDIO_JackCheck_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);					// cmd: 3
NEPDLLAPI bool NVDA_AUDIO_HookCheck_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);					// cmd: 4
NEPDLLAPI bool NVDA_AUDIO_LineOut_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);					// cmd: 5
NEPDLLAPI bool NVDA_AUDIO_PlayMusic_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);					// cmd: 6
NEPDLLAPI bool NVDA_AUDIO_PlayTone_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);					// cmd: 7
NEPDLLAPI bool NVDA_AUDIO_RecordWav_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);  // cmd: 8
NEPDLLAPI bool NVDA_AUDIO_LoopTest_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);   // cmd: 9
NEPDLLAPI bool NVDA_AUDIO_SwitchRX_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);   // cmd: 10
NEPDLLAPI bool NVDA_AUDIO_SwitchTX_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);   // cmd: 11
NEPDLLAPI bool NVDA_AUDIO_PlayWav_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);	// cmd: 12

// 6-GPS
NEPDLLAPI bool NVDA_GPS_EnterSARFMode_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);		// cmd: 1
NEPDLLAPI bool NVDA_GPS_CWTest_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);				// cmd: 2
NEPDLLAPI bool NVDA_GPS_FreqStability_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);		// cmd: 3
NEPDLLAPI bool NVDA_GPS_ExitSARFMode_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);			// cmd: 4

// 7-BT
NEPDLLAPI bool NVDA_BT_TxPowerOn_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);			// cmd: 1
NEPDLLAPI bool NVDA_BT_TxPowerOff_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);		// cmd: 2

// 8-FMR
NEPDLLAPI bool NVDA_FMR_ReadRSSI_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);// cmd: 1
NEPDLLAPI bool NVDA_FMR_PathTest_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);// cmd: 2

// 9-SIM
NEPDLLAPI bool NVDA_SIM_CheckStatus_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);  // cmd: 1
NEPDLLAPI bool NVDA_SIM_DetectHotPlug_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);// cmd: 2

// 10-CLK

// 11-USB

// 12-TOUCH
NEPDLLAPI bool NVDA_TOUCH_Test_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);				// cmd: 1
NEPDLLAPI bool NVDA_TOUCH_DownloadFirmware_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);	// cmd: 2

// 13-KEYPAD
NEPDLLAPI bool NVDA_KEYPAD_Detect_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);	// cmd: 1
NEPDLLAPI bool NVDA_KEYPAD_GetCapkeyChipID_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);			// cmd: 2

// 14-VIBRATOR
NEPDLLAPI bool NVDA_VIBRATOR_Switch_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode); // cmd: 1
NEPDLLAPI bool NVDA_VIBRATOR_RunningTime_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);			   // cmd: 2

// 15-BATTERY
NEPDLLAPI bool NVDA_BATTERY_BatteryCharge_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);		// cmd: 1
NEPDLLAPI bool NVDA_BATTERY_BatteryCurrent_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);						// cmd: 2
NEPDLLAPI bool NVDA_BATTERY_BatteryTemperature_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);					// cmd: 3
NEPDLLAPI bool NVDA_BATTERY_BatteryVoltage_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);						// cmd: 4
NEPDLLAPI bool NVDA_BATTERY_BatteryCapacity_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);						// cmd: 5
NEPDLLAPI bool NVDA_BATTERY_GaugeDFICalibration_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);  // cmd: 6
NEPDLLAPI bool NVDA_BATTERY_ReadGaugeID_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);							// cmd: 7
NEPDLLAPI bool NVDA_BATTERY_ReadChargerID_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);					    // cmd: 8
NEPDLLAPI bool NVDA_BATTERY_CoinCellCharge_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);		// cmd: 9
NEPDLLAPI bool NVDA_BATTERY_CoinCellVoltage_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);						// cmd: 10
NEPDLLAPI bool NVDA_BATTERY_DFIITEnable_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);		    // cmd: 11

// 16-DOWNLOAD

// 17-OS

// 18-WLAN
NEPDLLAPI bool NVDA_WLAN_EnableTestMode_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);					// cmd: 1
NEPDLLAPI bool NVDA_WLAN_EnableNormalMode_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);				// cmd: 2
NEPDLLAPI bool NVDA_WLAN_Disable_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);							// cmd: 3
NEPDLLAPI bool NVDA_WLAN_TxPowerSwitch_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);   // cmd: 4
NEPDLLAPI bool NVDA_WLAN_ScanAP_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);							// cmd: 5
NEPDLLAPI bool NVDA_WLAN_GetScanResult_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);					// cmd: 6
NEPDLLAPI bool NVDA_WLAN_ConnectAP_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);						// cmd: 7
NEPDLLAPI bool NVDA_WLAN_GetRSSI_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);		    // cmd: 8

// 19-LED
NEPDLLAPI bool NVDA_LED_BacklightLEDTest_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);		// cmd: 1
NEPDLLAPI bool NVDA_LED_LEDRGBTest_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);			// cmd: 2
NEPDLLAPI bool NVDA_LED_CapKeyLEDTest_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);		// cmd: 3
NEPDLLAPI bool NVDA_LED_FlashLEDTest_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);			// cmd: 4
NEPDLLAPI bool NVDA_LED_ReadFlashID_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);

// 20-ID
NEPDLLAPI bool NVDA_ID_ReadHWID_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);	// cmd: 2

// 21-Regulator
NEPDLLAPI bool NVDA_REGULATOR_ReadREGValue_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode); // cmd: 1

// 22-RF
NEPDLLAPI bool NVDA_WIRELESS_GSMTxPower_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer,char* ErrorCode);		// cmd: 1
NEPDLLAPI bool NVDA_WIRELESS_WCDMAxPower_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer,char* ErrorCode);		// cmd: 2
NEPDLLAPI bool NVDA_WIRELESS_TXPowerOFF_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);						// cmd: 3

// 23-WWAN

// 24-TV_MOUDLE
NEPDLLAPI bool NVDA_TV_ModuleDetect_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);					// cmd: 1
NEPDLLAPI bool NVDA_TV_InBandPower_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);   // cmd: 2

// 25-VERSION
NEPDLLAPI bool NVDA_VERSION_ReadSWVersion_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);     // cmd: 1
NEPDLLAPI bool NVDA_VERSION_ReadDDRInfo_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);       // cmd: 2
NEPDLLAPI bool NVDA_VERSION_ReadeMMCInfo_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);      // cmd: 3
NEPDLLAPI bool NVDA_VERSION_ReadSDInfo_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);        // cmd: 4
NEPDLLAPI bool NVDA_VERSION_ReadTouchInfo_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);     // cmd: 5
NEPDLLAPI bool NVDA_VERSION_ReadCameraInfo_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);    // cmd: 6
NEPDLLAPI bool NVDA_VERSION_ReadBTVersion_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);     // cmd: 7
NEPDLLAPI bool NVDA_VERSION_ReadWIFIVersion_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);   // cmd: 8
NEPDLLAPI bool NVDA_VERSION_ReadLCDInfo_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);       // cmd: 9
NEPDLLAPI bool NVDA_VERSION_ReadGyroFWVersion_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode); // cmd: 10
NEPDLLAPI bool NVDA_VERSION_ReadRFVersion_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);     // cmd: 11
NEPDLLAPI bool NVDA_VERSION_ReadBBVersion_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);     // cmd: 12
NEPDLLAPI bool NVDA_VERSION_ReadPMVersion_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);     // cmd: 13

// 26-FACTORY
NEPDLLAPI bool NVDA_FACTORY_FARead_ForSPU(char* nPort, char* nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer, char* ErrorCode);			// cmd: 1
NEPDLLAPI bool NVDA_FACTORY_FAWrite_ForSPU(char* nPort, char* nTimeout, char* InBuffer,size_t i_InLength,  char* OutBuffer, char* ErrorCode);			// cmd: 2
NEPDLLAPI bool NVDA_FACTORY_ReadPicasso_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);											// cmd: 3
NEPDLLAPI bool NVDA_FACTORY_ReadPicassoCheck_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);
NEPDLLAPI bool NVDA_FACTORY_ReadWifiMac_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);											// cmd: 4
NEPDLLAPI bool NVDA_FACTORY_WriteWifiMac_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);							// cmd: 5
NEPDLLAPI bool NVDA_FACTORY_ReadBtMac_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);											// cmd: 6
NEPDLLAPI bool NVDA_FACTORY_WriteBtMac_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);							// cmd: 7
NEPDLLAPI bool NVDA_FACTORY_TriggerNVrestore_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);
NEPDLLAPI bool NVDA_FACTORY_ReadPPID_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);												// cmd: 8
NEPDLLAPI bool NVDA_FACTORY_WritePPID_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);							// cmd: 9
NEPDLLAPI bool NVDA_FACTORY_ReadServiceTag_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);										// cmd: 10
NEPDLLAPI bool NVDA_FACTORY_WriteServiceTag_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);						// cmd: 11
NEPDLLAPI bool NVDA_FACTORY_WriteDLFlag_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);							// cmd: 14
NEPDLLAPI bool NVDA_FACTORY_WriteFTFlag_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);							// cmd: 15
NEPDLLAPI bool NVDA_FACTORY_WriteMMIBBFlag_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);						// cmd: 16
NEPDLLAPI bool NVDA_FACTORY_WriteMMIRFFlag_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);						// cmd: 17
NEPDLLAPI bool NVDA_FACTORY_CheckAndMoveFA_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);										// cmd: 19
NEPDLLAPI bool NVDA_FACTORY_WritePicasso_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);							// cmd: 24
NEPDLLAPI bool NVDA_FACTORY_ReadIMEI_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);												// cmd: 28
NEPDLLAPI bool NVDA_FACTORY_WriteIMEI_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);							// cmd: 29
NEPDLLAPI bool NVDA_FACTORY_ResetPhone_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);											// cmd: 30
NEPDLLAPI bool NVDA_FACTORY_WriteFCFlag_ForSPU(char* nPort, char* nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer, char* ErrorCode);		// cmd: 31
NEPDLLAPI bool NVDA_FACTORY_SetBootMode_ForSPU(char* nPort, char* nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer,char* ErrorCode);		// cmd: 34
NEPDLLAPI bool NVDA_FACTORY_EFS_Sync_ForSPU(char* nPort, char* nTimeout, char* OutBuffer,char* ErrorCode);												// cmd: 35
NEPDLLAPI bool NVDA_FACTORY_ReadLCSN_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);												// cmd: 36
NEPDLLAPI bool NVDA_FACTORY_WriteLCSN_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer,char* ErrorCode);								// cmd: 37
NEPDLLAPI bool NVDA_FACTORY_WriteOTP_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);												// cmd: 39
NEPDLLAPI bool NVDA_FACTORY_ReadMEID_ForSPU(char* nPort, char* OutBuffer);																				// cmd: 41
NEPDLLAPI bool NVDA_FACTORY_WriteMEID_ForSPU(char* nPort, char* InBuffer);																				// cmd: 42
NEPDLLAPI bool NVDA_FACTORY_ReadPhoneTime_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);										// cmd: 43
NEPDLLAPI bool NVDA_FACTORY_WritePhoneTime_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer,char* ErrorCode);						// cmd: 44

// 27-HouseKeeping
NEPDLLAPI bool NVDA_HOUSEKEEP_PowerGlueLogicCheck_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode); //cmd:1

// 28-EFS

// 29-NV
NEPDLLAPI bool NVDA_NV_ReadNV_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);							// cmd: 1
NEPDLLAPI bool NVDA_NV_WriteNV_ForSPU(char* nPort, char* nTimeout, char* InBuffer, size_t i_InLength, char* OutBuffer, char* ErrorCode);		// cmd: 2
NEPDLLAPI bool NVDA_NV_WriteInitNV_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);		                                // cmd: 3
//BackupNV: you can input NULL or station, date, time into inbuffer in order to record nv infor.
NEPDLLAPI bool NVDA_NV_BackupNV_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);							// cmd: 3

// 30-DIAG
NEPDLLAPI bool NVDA_DIAG_SetRequest_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);   // cmd: 1
NEPDLLAPI bool NVDA_RTC_Read_ForSPU(char* nPort, char* nTimeout,char* OutBuffer, char* ErrorCode);// cmd: 1
NEPDLLAPI bool NVDA_RTC_Write_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer, char* ErrorCode);// cmd: 2

NEPDLLAPI bool NVDA_FACTORY_ReadSN_ForSPU(char* nPort, char* nTimeout, char* OutBuffer, char* ErrorCode);												// cmd: 36
NEPDLLAPI bool NVDA_FACTORY_WriteSN_ForSPU(char* nPort, char* nTimeout, char* InBuffer, char* OutBuffer,char* ErrorCode);								// cmd: 37
