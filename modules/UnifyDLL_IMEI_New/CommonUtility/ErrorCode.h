#ifndef _ERROR_CODE_H_
#define _ERROR_CODE_H_

#define QLIB_FUNCTION_SUCCESSFUL			  1

#define LOAD_SCRIPT_FAIL								"FT1000"
#define INIT_TESTITEM_FAIL								"FT1001"

#define PHONE_NOT_POWERON								"FT2010"
#define POWER_ON_SHORT									"FT2011"
#define POWERSUPPLY_FAIL								"FT2020"
#define CHANGE_MODE_FAIL								"FT2021"
#define CHANGE_NORMALMODE_FAIL							"FT2022"
#define FTD_READ_NV_FAIL								"FT2023"
#define FTD_WRITE_NV_FAIL								"FT2024"
#define FTD_EFS_SYNC_FAIL								"FT2025"

#define READ_PICASSO_FAIL								"FT3001"
#define WRITE_PICASSO_FAIL								"FT3002"
#define COMPARE_PICASSO_FAIL							"FT3003"
#define WRITE_SWVERSION_FAIL							"FT3004"
#define READ_SWVERSION_FAIL								"FT3005"
#define SWVERSION_ERROR									"FT3006"
#define WRITE_HWVERSION_FAIL							"FT3007"
#define READ_HWVERSION_FAIL								"FT3008"
#define HWVERSION_ERROR									"FT3009"
#define WRITE_PCBAVERSION_FAIL							"FT3010"
#define READ_PCBAVERSION_FAIL							"FT3011"
#define PCBAVERSION_ERROR								"FT3012"
#define WRITE_MEVERSION_FAIL							"FT3013"
#define READ_MEVERSION_FAIL								"FT3014"
#define MEVERSION_ERROR									"FT3015"
#define WRITE_PROCESSID_FAIL							"FT3016"
#define READ_PROCESSID_FAIL								"FT3017"
#define CHECK_PROCESSID_FAIL							"FT3018"
#define CHECK_MFGID_FAIL								"FT3019"
#define FACTORY_BLOCK_FAIL								"FT3020"
#define CHECK_PICASSO_SMT_FAIL							"FT3021"
#define CHECK_MOBILEID_BY_PICASSO_FAIL					"FT3022"
#define BACKUP_NV_FAIL									"FT3023"

#define RTC_FAIL										"FT4000"
#define WRITE_QFUSE_FAIL								"FT4001"
#define CHECK_QFUSE_FAIL								"FT4002"

#define BATTERYID_FAIL									"FT5050"
#define BATTERY_FAIL									"FT5051"
#define BATTERY_TEMPERATURE								"FT5052"
#define BATTERY_VOLTAGE									"FT5053"
#define BATTERY_COVER_DETECT							"FT5054"
#define COINCELL_VOLTAGE_FAIL							"FT5055"
#define GAUGE_BATT_CURR_FAIL							"FT5056"

#define SWITCH_USB_CHARGING_FAIL						"FT5100"
#define USB_CHARGE_FAIL									"FT5101"
#define STOP_COINCELL_CHARGE_FAIL						"FT5102"
#define HIGH_CURRENT_CHARGE_FAIL						"FT5103"

#define BT_DEVICE_FAIL									"FT5250"
#define BT_COMMAND_FAIL									"FT5251"
#define BT_TX_MAX_POWER_FAIL							"FT5252"
#define BT_FM_FAIL										"FT5253"
#define BT_OUTPUT_POWER_FAIL							"FT5254"
#define BT_MODULATION_CHARACTERISTIC_FAIL				"FT5255"
#define BT_ICFT_FAIL									"FT5256"
#define BT_CARRIER_DRIFT_FAIL							"FT5257"
#define BT_EDR_FREQ_STAB_MOD_ACC_FAIL					"FT5258"
#define BT_EDR_DIFF_PHASE_ENCODING_FAIL					"FT5259"
#define BT_SINGLESLOT_SENSITIVITY_FAIL					"FT5260"
#define BT_MULTISLOT_SENSITIVITY_FAIL					"FT5261"
#define BT_MAX_USABLE_SIGNAL_LEVEL_FAIL					"FT5262"
#define BT_EDR_SENSITIVITY_FAIL							"FT5263"
#define BT_EDR_BER_FLOOR_FAIL							"FT5264"
#define BT_EDR_MAX_INPUT_LEVEL_FAIL						"FT5265"
#define BT_POWER_CONTROL_FAIL							"FT5266"
#define BT_EDR_RELATIVE_POWER_FAIL						"FT5267"
#define BT_MODULE_TURN_OFF_FAIL							"FT5268"

#define WLAN_DEVICE_FAIL								"FT5300"
#define WLAN_COMMAND_FAIL								"FT5301"
#define WLAN_TX_MAX_POWER_FAIL							"FT5302"
#define WLAN_OUTPUTPOWER_FAIL							"FT5303"
#define WLAN_EVM_FAIL									"FT5304"
#define WLAN_CARRIER_SUPPRESION_FAIL					"FT5305"
#define WLAN_FREQ_ERROR_FAIL							"FT5306"
#define WLAN_DEMOD_FAIL									"FT5307"
#define WLAN_POWER_RAMP_FAIL							"FT5308"
#define WLAN_SPECTRAL_MASK_FAIL							"FT5309"
#define WLAN_SPECTRAL_FLATNESS_FAIL						"FT5310"
#define WLAN_RX_MINSENS_FAIL							"FT5311"

#define GPS_DEVICE_FAIL									"FT5450"
#define GPS_COMMAND_FAIL								"FT5451"
#define GPS_TEST_FAIL									"FT5452"
#define GPS_CN0_TEST_FAIL								"FT5453"
#define GPS_TTFF_TEST_FAIL								"FT5454"

#define FM_DEVICE_FAIL									"FT5500"
#define FM_COMMAND_FAIL									"FT5501"
#define FM_TEST_FAIL									"FT5502"
#define FM_ID_FAIL										"FT5503"

#define LED_TEST_FAIL									"FT5600"
#define SIM_CARD_TEST_FAIL								"FT5601"
#define SD_CARD_TEST_FAIL								"FT5602"
#define DETECT_USB_STORAGE_FAIL							"FT5603"
#define SET_CONTRAST_ERROR								"FT5604"
#define SET_LCD_TYPE_ERROR								"FT5605"
#define AMBIENT_LIGHT_SENSOR							"FT5606"
#define GSENSOR_FAIL									"FT5607"
#define ECOMPASS_FAIL									"FT5608"
#define PROXIMITY_FAIL									"FT5609"
#define FLASH_ID_READ_WRITE_FAIL						"FT5610"
#define CAMERA_ID_FAIL									"FT5611"
#define CAP_KEY_FAIL									"FT5612"
#define AUDIO_FAIL										"FT5613"
#define TV_FAIL											"FT5614"
#define HOUSEKEEP_POWERG_GLUE_LOGIC						"FT5615"
#define REGULATOR										"FT5616"
#define READ_GAUGEIC_NAME_FAIL							"FT5617"
#define LEAKAGE_CURRENT_FAIL							"FT5618"
#define VIBRATOR_FAIL									"FT5621"
#define GYRO_FAIL										"FT5622"
#define SIM_LOCK_CHECK_FAIL								"FT5623"
#define CHARGE_IC_FAIL									"FT5624"
#define EMMC_SIZE_FAIL									"FT5625"
#define CHARGE_CURRENT_FAIL								"FT5626"
#define LCD_OFF_CURRENT_FAIL							"FT5627"
#define SLEEP_CURRENT_FAIL								"FT5628"
#define BOOT_CURRENT_FAIL								"FT5629"
#define GAUGE_IC_FAIL									"FT5630"
#define GAUGE_DFI_CALIBRATION_FAIL						"FT5631"
#define DDR_TEST_FAIL									"FT5632"
#define MHL_TEST_FAIL									"FT5633"
#define NFC_TEST_FAIL									"FT5634"
#define Codec_TEST_FAIL									"FT5635"
#define CHECK_eMMC_SETTING_FAIL							"FT5636"
#define CPU_ID_FAIL										"FT5637"
//FT add by CY.Lee 2012/06/05
#define CAMERA_LED_FAIL									"FT5638"
#define R_LED_TEST_FAIL									"FT5639"
#define G_LED_TEST_FAIL									"FT5640"
#define B_LED_TEST_FAIL									"FT5641"

#define HEAD_PHONE_L									"FT5700"
#define HEAD_PHONE_R									"FT5701"
#define LINE_OUT_L										"FT5702"
#define LINE_OUT_R										"FT5703"
#define MIC_JACK_BIAS									"FT5704"
#define SPEAKER_L										"FT5705"
#define SPEAKER_R										"FT5706"
#define SPEAKER_AMP_TEST_FAIL							"FT5707"
//Audio add by CY.Lee	2012/06/04
#define MIC_FORNT_BIAS									"FT5708"
#define MIC_BACK_BIAS									"FT5709"
#define MIC_FORNT										"FT5710"
#define MIC_BACK										"FT5711"

#define GSM_TX_MAX_POWER_FAIL							"FT5850"

//Wireless add by jackiesen	2011/06/01
#define ERR_WIRELESS_SET_PDM						    "FT5851"
#define ERR_WIRELESS_SET_PHONEID					    "FT5852"
#define ERR_WIRELESS_GET_PHONEMODE						"FT5853"
#define ERR_WIRELESS_SET_CHANNEL						"FT5854"
#define ERR_WIRELESS_SET_WCDMAWAVE						"FT5855"
#define ERR_WIRELESS_SET_PARANGE						"FT5856"
#define ERR_WIRELESS_SET_BAND_CH						"FT5857"
#define ERR_WIRELESS_SET_TX_ONOFF						"FT5858"
#define ERR_WIRELESS_SET_MODE							"FT5859"
#define ERR_WIRELESS_SET_MAXPOWER						"FT5860"
#define ERR_WIRELESS_FETCH_POWER						"FT5861"
#define ERR_GSM_SET_RGI					                "FT5862"
//#define ERR_QLIB_PHONE_CONNECT	                        "FT5863"
//#define ERR_QLIB_SET_PHONE_BAND						    "FT5864"
#define ERR_QLIB_FUNCTION_FAILED					    "FT5865"
#define ERR_QLIB_WRONG_VALUE						    "FT5866"
#define ERR_GSM_SET_RF_CHANNEL						    "FT5867"
#define ERR_DEVICE_WCDMATEST_FETCH_MAXPOW				"FT5868"
#define ERR_WIRELESS_READ_NV							"FT5869"
#define ERR_WIRELESS_GET_PDM							"FT5870"
#define ERR_DEVICE_SET_CABLELOSS						"FT5871"
#define ERR_DEVICE_INIT_GSM								"FT5872"
#define ERR_DEVICE_GSMCAL_SET_BAND	                    "FT5873"
//#define ERR_QLIB_SET_FTM								"FT5874"
#define ERR_GSM_WIRELESS								"FT5875"
#define ERR_WCDMA_WIRELESS								"FT5876"
#define ERR_SET_WCDMA_FREQ								"FT5877"
#define ERR_WCDMA_POWER_FAIL							"FT5878"
#define ERR_GSM_POWER_FAIL								"FT5879"
#define ERR_QLIB_COMMAND_CODE							"FT5880"
#define ERR_WIRELESS_SET_CDMA2000WAVE					"FT5881"

#define VDD												"FT6000"
#define VDD_LDO0										"FT6001"	
#define VDD_LDO1										"FT6002"	
#define VDD_LDO2										"FT6003"	
#define VDD_LDO3										"FT6004"	
#define VDD_LDO4										"FT6005"	
#define VDD_LDO5										"FT6006"	
#define VDD_LDO6										"FT6007"	
#define VDD_LDO7										"FT6008"	
#define VDD_LDO8										"FT6009"	
#define VDD_LDO9										"FT6010"	
#define VDD_SM0											"FT6011"	
#define VDD_SM1											"FT6012"	
#define VDD_1V5											"FT6013"	
#define VDD_1V8											"FT6014"	
#define VDD_2V2											"FT6015"	
#define VDD_3V3											"FT6016"	
#define VDD_3V3_2										"FT6017"	
#define VDD_1V2											"FT6018"	
#define HDMI_5V											"FT6019"	
#define VLED1											"FT6021"	
#define VLED2											"FT6022"	
#define VCAPKEYLED										"FT6023"	

/*********************** FT-Wireless station error code ***********************/


/*********************** MMI(EM﹜Audio﹜Camara﹜Touch) stations error code ************************/

#define ERR_INIT_TESTITEM_FAIL							"MM0001" 

#define ERR_MOBILEID_NOT_EXIST							"MM0011" 
#define ERR_MOBILEID_INCORRECT							"MM0012" 
#define ERR_FACTORY_AREA_NOT_ACTIVE						"MM0013" 
#define ERR_WRITE_FACTORY_AREA_FAIL						"MM0014" 
#define ERR_READ_FACTORY_AREA_FAIL						"MM0015" 
#define ERR_MOBILEID_EXIST_MODE_ERROR					"MM0016" 
#define ERR_PHONE_IN_DLMODE								"MM0017" 
#define ERR_HANDSET_NOT_INSERTED						"MM0018" 
#define ERR_PICASSO_FAIL								"MM0019"
#define ERR_CHECK_SWVERSION_FAIL						"MM0020"
#define ERR_CHECK_HWVERSION_FAIL						"MM0021"
#define ERR_WRONG_NV2497								"MM0022"
#define ERR_GET_FILE_FROM_DEVICE_FAIL					"MM0023"
#define ERR_PARSE_FILE_FAIL								"MM0024"
#define ERR_WRITE_PICS_FAIL								"MM0025"
#define ERR_WRITE_PROCESSINFO_FAIL						"MM0026"
#define ERR_BACK_UP_NV_FAIL								"MM0027"

#define ERR_MODEM_VERSION_CHECK_FAIL					"MM1004"
#define ERR_TOUCH_VERSION_CHECK_FAIL					"MM1005"
#define ERR_TOUCH_CALIBRATION_CHECK_FAIL				"MM1006"
#define ERR_FASTBOOT_VERSION_CHECK_FAIL					"MM1007"
#define ERR_RC_VERSION_CHECK_FAIL						"MM1008"
#define ERR_RTC_YEAR_CHECK_FAIL							"MM1009"
#define ERR_GAUGE_VERSION_CHECK_FAIL					"MM1010"
#define ERR_IMEI_CHECK_FAIL								"MM1011"
#define ERR_WLAN_CHECK_FAIL								"MM1012"
#define ERR_BT_CHECK_FAIL								"MM1013"
#define ERR_SIMLOCK_CHECK_FAIL							"MM1014"
#define ERR_SD_INFO_CHECK_FAIL							"MM1015"
#define ERR_PICASSO_CHECK_FAIL							"MM1016"
#define ERR_DFI_VERSION_CHECK_FAIL					    "MM1017"

#define ERR_VIBRATOR_FAIL								"MM1051"

#define ERR_SIM_NOT_DETECTED							"MM1071"
#define ERR_SIM_NO_3GMODULE								"MM1072"
#define ERR_SIM_NOT_DETECTED_WIMAX						"MM1073"
#define ERR_SIM_NOT_DETECTED_CDMA						"MM1074"

#define ERR_CAMERA_TEST_FAIL							"MM1091"
#define ERR_CAMERA_CHECK_EPPROM_FAIL					"MM1092"

#define ERR_SD_DETECT_FAIL								"MM1101"
#define ERR_SD_DETECT_FAIL2								"MM1102"

#define ERR_AUDIO_MIC_TO_SPEAKER						"MM1110"
#define ERR_AUDIO_MIC_TO_RECEIVER						"MM1112"
#define ERR_AUDIO_HEADSET_LOOPBACK						"MM1114"
#define ERR_AUDIO_MUSIC_TO_SPEAKER						"MM1118"
#define ERR_AUDIO_GENERAL_ERROR							"MM1119"
#define ERR_HEADSET_UNDETECTED							"MM1120"
#define ERR_HEADSET_MISDETECTED							"MM1121"
#define ERR_HEADSET_SENDKEY_FAIL						"MM1122"

#define ERR_AUDIO_SWITCH_TO_MIC						    "MM1123"
#define ERR_AUDIO_MIC_RECORD						    "MM1124"
#define ERR_AUDIO_MUSIC_TO_RECEIVER						"MM1125"
#define ERR_HEADSET_MIC									"MM1126"
#define ERR_HEADSET_SPEAKER								"MM1127"

/* Alex.Chen Add Start */
#define ERR_PC_PLAY_WAV_FAIL							"MM1129"
#define ERR_COLLECT_DATA_FAIL							"MM1130"
#define ERR_ANALYSE_DATA_FAIL							"MM1131"
/* Alex.Chen Add End */

#define ERR_BATTERY_VOLTAGE_FAIL						"MM1155" 
#define ERR_CHARGING_FAIL								"MM1156"
#define ERR_BATTERY_FAIL								"MM1157" 
#define ERR_HC_CHARGING_CURRENT_FAIL					"MM1158"
#define ERR_USB_CHARGING_CURRENT_FAIL					"MM1159" 

#define ERR_LCD_DISPLAY_FAIL							"MM1160"
#define ERR_LCD_BACKLIGHT_FAIL							"MM1170"

#define ERR_DFI_CALIBRATION_FAIL						"MM1179"
#define ERR_GAUGE_CHECK_FAIL							"MM1180"
#define ERR_GAUGE_ENABLE_DFIIT_FAIL						"MM1181"
#define ERR_CHECK_ENABLE_DFIIT_FAIL						"MM1182"

#define ERR_TOUCH_DOWNLOADFIRMWARE_FAIL					"MM1183"
#define ERR_TOUCH_GENERAL_FAIL							"MM1184"
#define ERR_TOUCH_CHECK_RANGE_FAIL						"MM1185"
#define ERR_TOUCH_AUTO_CHECK_FAIL						"MM1186"
#define ERR_TOUCH_PIN_FAIL								"MM1187"

#define ERR_LED_FAIL									"MM1190"
#define ERR_ECOMPASS_FAIL								"MM1191"
#define ERR_POWERLED_GREEN_FAIL							"MM1192"
#define ERR_POWERLED_RED_FAIL							"MM1193"
#define ERR_CAPKEYLED_FAIL								"MM1194"
#define ERR_FLASHLED_FAIL								"MM1195"

#define ERR_KEYPAD_FAIL									"MM1220"
#define ERR_CAPKEY_FAIL									"MM1221"
#define ERR_CAPKEY_SENSITIVITY_CHECK_FAIL				"MM1222"

#define ERR_ALS_LSENSOR_CHECK_ID_FAIL					"MM1249"
#define ERR_ALS_PSENSOR_CHECK_ID_FAIL					"MM1250"
#define ERR_ALS_LSENSOR_RANGE_CHECK_FAIL				"MM1251"
#define ERR_ALS_PSENSOR_RANGE_CHECK_FAIL				"MM1252"
#define ERR_ALS_PSENSOR_SAMEDATA_AT_DIFF_SITUATION		"MM1253"
#define ERR_ALS_LSENSOR_SAMEDATA_AT_DIFF_SITUATION		"MM1254"
#define ERR_ALS_LSENSOR_CALIBRATION_FAIL				"MM1255"
#define ERR_ALS_PSENSOR_CALIBRATION_FAIL				"MM1256" 
#define ERR_ALS_LSENSOR_VERIFICATION_FAIL				"MM1257"
#define ERR_ALS_PSENSOR_VERIFICATION_FAIL				"MM1258" 
#define ERR_ALS_ENABLE_SENSOR							"MM1259"
#define ERR_ALS_DISABLE_SENSOR							"MM1260"

#define ERR_GSENSOR_CALIBRATION_FAIL					"MM1311"
#define ERR_GSENSOR_TEST_FAIL							"MM1312"

#define ERR_SARSENSOR_TEST_FAIL							"MM1350"

#define ERR_WLAN_FAIL									"MM1500"
#define ERR_BT_FAIL										"MM1510"

#define ERR_HDMI_FAIL									"MM1600"
#define ERR_SLIDER_FAIL									"MM1610"
#define ERR_RTC_TEST_FAIL								"MM1620"

/*Lion*/
#define ERR_CARDMODE_FAIL								"MM1630"
#define ERR_READMODE_FAIL								"MM1631"
#define ERR_FM_REDIO_FAIL								"MM1640"
#define ERR_UNKNOW_FAIL							    	"MM2000"
/**/

#define ERR_PUSH_RUNIN_XML_FAIL							"MM1700"
#define ERR_PUSH_RUNIN_APK_FAIL							"MM1701"


#define ERR_SUBSYSTEM_STATION_FAIL						"MM4000"
#define ERR_2G_STATION_FAIL								"MM4001"
#define ERR_3G_STATION_FAIL								"MM4002"
#define ERR_CIT_STATION_FAIL							"MM4003"
#define ERR_WIRELESS_STATION_FAIL						"MM4004"
#define ERR_IMEI_STATION_FAIL							"MM4005"
#define ERR_PREDL_STATION_FAIL							"MM4006"
#define ERR_FT_STATION_FAIL								"MM4007"
#define ERR_MMIBB_STATION_FAIL							"MM4008"
#define ERR_REDL_STATION_FAIL							"MM4009"
#define ERR_301_FLAG_FAIL								"MM4010"

/* Evonne.Hsieh Add */
#define ERR_PHONE_PLAY_WAV_FAIL							"MM4011"
#define ERR_PHONE_SPEAKER_TEST_FAIL						"MM4012"
#define ERR_PHONE_HANDSET_TEST_FAIL						"MM4013"
#define ERR_PHONE_HEADSET_TEST_FAIL						"MM4014"
#define ERR_PHONE_LOOPBACK_TEST_FAIL					"MM4015"
#define ERR_PHONE_JACK_DETECT_FAIL						"MM4016"
#define ERR_PHONE_LOOPBACK_HEADMIC_TO_HEADSET_FAIL		"MM4017"
#define ERR_PHONE_LOOPBACK_FRONTMIC_TO_HEADSET_FAIL		"MM4018"
#define ERR_PHONE_LOOPBACK_BACKMIC_TO_HEADSET_FAIL		"MM4019"

/* Alex.Chen Add */
#define ERR_JIG_IN								        "MM9000"
#define ERR_JIG_OUT								        "MM9001"
#define ERR_JIG_ON								        "MM9002"
#define ERR_JIG_OFF								        "MM9003"

/*********************** MMI(EM﹜Audio﹜Camara﹜Touch) stations error code ************************/

/*CAMARA ERROR CODE, QTH LION 2012.5.16*/

#define	Front_Camera_Test_Macro_Capture_Fail			"CAMA0001" //,FunErr_Front_Camera_Test_Macro_Capture_Fail,前攝像頭微距拍照失敗,Front Camera Macro Capture fail
#define	Front_Camera_Test_Capture_Fail					"CAMA0002" //FunErr_Front_Camera_Test_Capture_Fail,前攝像頭拍照失敗,Front Camera Capture fail
#define	Front_Camera_Test_Shutdown_Fail					"CAMA0003" //,FunErr_Front_Camera_Test_Shutdown_Fail,前攝像頭關閉失敗,Front Camera shut down fail
#define	Front_Camera_Test_Read_ID_Fail					"CAMA0004" //,FunErr_Front_Camera_Test_Read_ID_Fail,前攝像頭設備讀取失敗,Read front camera ID Fail
#define	Front_Camera_Test_Shading_Fail					"CAMA0005" //,FunErr_Front_Camera_Test_Shading_Fail,前攝像頭拍攝照片亮度均值偏低,front camera shading fail
#define	Front_Camera_Test_Blemish_Fail					"CAMA0006" //,FunErr_Front_Camera_Test_Blemish_Fail,前攝像頭存在黑斑,front camera Blemish Fail
#define	Front_Camera_Test_MTF_Fail						"CAMA0007" //,FunErr_Front_Camera_Test_MTF_Fail,前攝像頭黑白不均,front camera MTF Fail
#define	Front_Camera_Test_Tile_Fail						"CAMA0008" //,FunErr_Front_Camera_Test_Tile_Fail,前攝像頭安裝過度傾斜,front camera Tile Fail
#define	Front_Camera_Test_Rotation_Fail					"CAMA0009" //,FunErr_Front_Camera_Test_Rotation_Fail,前攝像頭安裝過度旋轉,front camera Rotation Fail
#define	Front_Camera_Test_Color_Fail					"CAMA0010" //,FunErr_Front_Camera_Test_Color_Fail,前攝像頭色彩不正常,front camera Color Fail
#define	Rear_Camera_Test_Capture_Fail					"CAMA0011" //,FunErr_Rear_Camera_Test_Capture_Fail,後攝像頭拍照失敗,Rear Camera Capture fail
#define	Rear_Camera_Test_Read_ID_Fail					"CAMA0012" //,FunErr_Rear_Camera_Test_Read_ID_Fail,後攝像頭設備讀取失敗,Read rear camera ID Fail
#define	Rear_Camera_Test_Shading_Fail					"CAMA0013" //,FunErr_Rear_Camera_Test_Shading_Fail,後攝像頭拍攝照片亮度均值偏低,rear camera shading fail
#define	Rear_Camera_Test_Blemish_Fail					"CAMA0014" //,FunErr_Rear_Camera_Test_Blemish_Fail,後攝像頭存在黑斑,rear camera Blemish Fail
#define	Rear_Camera_Test_MTF_Fail						"CAMA0015" //,FunErr_Rear_Camera_Test_MTF_Fail,後攝像頭黑白不均,rear camera MTF Fail
#define	Rear_Camera_Test_Tile_Fail						"CAMA0016" //,FunErr_Rear_Camera_Test_Tile_Fail,後攝像頭安裝過度傾斜,rear camera Tile Fail
#define	Rear_Camera_Test_Rotation_Fail					"CAMA0017" //,FunErr_Rear_Camera_Test_Rotation_Fail,後攝像頭安裝過度旋轉,rear camera Rotation Fail
#define	Rear_Camera_Test_Color_Fail						"CAMA0018" //,FunErr_Rear_Camera_Test_Color_Fail,後攝像頭色彩不正常,rear camera Color Fail
#define	Reset_PictureIndex_Fail							"CAMA0019" //,FunErr_Camera_Reset_PictureIndex_Fail,重置照片索引失敗,Reset picture index fail


/*********************** MMI(EM﹜Audio﹜Camara﹜Touch) stations error code ************************/

/*QTH LION 2012.5.36*/

//**FSN,,,,
#define FSN_Siez_fail									"FSN00001"  //CommErr_FSN_Length_Fail,FSN長度不對,Check FSN Length fail
#define Compare_FSN_Fail								"FSN00002" //CommErr_Compare_MSN_Fail,比較手機裡寫入的FSN和系統不一致,Compare FSN fail
#define Read_FSN_Fail									"FSN00003" //ommErr_Read_FSN_Fail,讀FSN失敗,Write FSN fail
#define Write_FSN_Fail									"FSN00004" //CommErr_Write_FSN_Fail,寫FSN失敗,Read FSN fail
#define NO_FSN_Record_IN_DB								"FSN00005" //CommErr_No_FSN_Record_in_DB,系統沒有FSN數據,No FSN record in DB

//**BT Address,,
#define Read_BT_Address_Fail							"BTMC0001" //CommErr_Read_BT_Address_Fail,讀BT失敗,Write BT address fail
#define Write_BT_Address_Fail							"BTMC0002" //CommErr_Write_BT_Address_Fail,寫BT失敗,Read BT address fail
#define Compare_BT_Address_Fail							"BTMC0003" //CommErr_Compare_BT_Address_Fail,比較手機裡寫入的BT地址和系統不一致,Compare BT address fail
#define Check_BT_Address_Length_Fail					"BTMC0004" //CommErr_Check_BT_Address_Length_Fail,BT地址長度不正確,Check BT address length fail

//**WLAN Address,,,,
#define Check_WLAN_Address_Length_Fail					"WFMC0001" //CommErr_Check_WLAN_Address_Length_Fail,WLAN地址長度不正確,Check WLAN address length fail
#define Read_WLAN_Address_Fail							"WFMC0002" //CommErr_Read_WLAN_Address_Fail,讀WLAN失敗,Write WLAN address fail
#define Write_WLAN_Address_Fail							"WFMC0003" //CommErr_Write_WLAN_Address_Fail,寫WLAN失敗,Read WLAN address fail
#define Compare_WLAN_Address_Fail						"WFMC0004" //CommErr_Compare_WLAN_Address_Fail,比較手機裡寫入的WLAN地址和系統不一致,Compare WLAN address fail

//**IMEI,,,,
#define Check_IMEI_Rule_Fail							"IMEI0001" //CommErr_Check_IMEI_Rule_Fail,IMEI不符合規則,Check IMEI Length & Rule fail
#define Check_IMEI_Fail_In_Rework						"IMEI0002" //CommErr_Check_IMEI_Fail_In_Rework,Rework時手機裡的IMEI和掃瞄的不一致,Check IMEI fail in rework mode
#define IMEI_Default_In_Phone_Error						"IMEI0003" //CommErr_IMEI_Default_In_Phone_Error,手機的default IMEI不正確,Default IMEI error
#define No_IMEI_In_Phone_When_rework					"IMEI0004" //CommErr_No_IMEI_In_Phone_When_rework,rework是手機裡沒有IMEI存在,No IMEI when in rework
#define IMEI_Status_In_P_S_Error						"IMEI0005" //CommErr_IMEI_Status_In_P_S_Error,IMEI狀態是P或S錯誤,IMEI status in P or S
#define IMEI_Status_Error								"IMEI0006" //CommErr_IMEI_Status_Error,IMEI狀態錯誤,IMEI status fail
#define Check_IMEI_Sequence_Fail						"IMEI0007" //CommErr_Check_IMEI_Sequence_Fail,IMEI不連續,Check IMEI sequence fail
#define Read_IMEI_Fail									"IMEI0008" //CommErr_Read_IMEI_Fail,讀IMEI失敗,Write IMEI fail
#define Write_IMEI_Fail									"IMEI0009" //CommErr_Write_IMEI_Fail,寫IMEI失敗,Read IMEI fail
#define Compare_IMEI_Fail								"IMEI0010" //CommErr_Check_IMEI_Fail,比較手機裡寫入的IMEI和系統不一致,Compare IMEI fail

/*********************** PreMMI stations error code ************************/
#define Power_Gauge_Voltage_Fail						"PWGA0001"
#define Power_Batt_ID_Fail								"PWGA0002"
#define Power_Gauge_Temperature_Fail					"PWGA0003"

#endif
