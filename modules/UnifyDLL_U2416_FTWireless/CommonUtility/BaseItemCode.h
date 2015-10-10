/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *   This is a part of the Qisda RF Calibration and Test library.
 *
 *   This document contains propriety information, and except with written
 *   permission of Qisda Corp., such information shall not be 
 *   published, or disclosed to others, or used for any purpose, and the   
 *   document shall not be duplicated in whole or in part.  
 *
 *   Copyright (C) 2010 Qisda Corporation
 *   All Rights Reserved.
 *
 ***************************************************************************/
/***************************************************************************
************************Item Code Define Rule********************************
Item Code(6 bytes):
2G Calibration Item Code = 2(1) + Band(1) + Channel(1) + PowerLevel(1) + Item Number(2)
3G Calibration Item Code = 3 + Band + Channel + PowerLevel + Item Number
2G GSM Test Item Code = 4 + Band + Channel + PowerLevel + Item Number
2G EDGE Test Item Code = 5 + Band + Channel + PowerLevel + Item Number
3G Test Item Code = 6 + Band + Channel + PowerLevel + Item Number
Antenna Test Item Code = 7 + Band + Channel + PowerLevel + Item Number
Base Band = 1 + 0 + 0 +  Item Number£¨3bytes£©

Band Definition: 
GSM:   1: 850    2: 900   3: 1800   4: 1900   5: 850/900   6: 1800/1900
EDGE: 1: 850    2: 900   3: 1800    4: 1900
WCDMA:  1: 850   2: 900   3: 1700   4: 1900   5: 2100

Channel Definition:  
 1: Low Channel   2: Middle Channel   3: High Channel
 
 Power Level Definition:
  1: Low Power Level   2: High Power Level 
***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
#ifndef _BaseItemCode_H
#define _BaseItemCode_H
//-------------------------------------------------------------------------------------------------
//	2G Calibration BaseItemCode
//------------------------------------------------------------------------------------------------- 
#define GSM_CalGSMInit_BaseItemCode                                      200000
#define GSM_PA_DAC_Search_Calibration_BaseItemCode                       200001
#define GSM_KVCO_Calibration_BaseItemCode                                200002
#define GSM_CS_Calibration_BaseItemcode                                  200003
#define GSM_Polar_Calibration_BaseItemCode                               200004
#define GSM_RxFreqCompensation_Calibration_BaseItemCode                  200005
#define GSM_TimeDelay_Calibration_BaseItemCode                           200006
#define GSM_AFCPhasing_Calibration_BaseItemCode                          200007
#define GSM_TxRxPhasing_Calibration_BaseItemCode                         200008
#define GSM_PrePhasingTask_Calibration_BaseItemCode                      200009
#define GSM_PostPhasingTask_Calibration_BaseItemCode                     200010
#define GSM_CS_Idc_Calibration_BaseItemCode                              200011
#define GSM_CS_Qdc_Calibration_BaseItemCode                              200012
#define GSM_CalGSMEnd_BaseItemCode                                       200099

//-------------------------------------------------------------------------------------------------
//	2G Test BaseItemCode
//------------------------------------------------------------------------------------------------- 
#define GSM_GSMTestInit_BaseItemCode                                     400000
#define GSM_GSMTestCall_BaseItemCode                                     400001
#define GSM_MaxPower_Test_BaseItemCode                                   400002
#define GSM_Burst_Test_BaseItemCode                                      400003
#define GSM_PeakPhase_Test_BaseItemCode                                  400004
#define GSM_RmsPhase_Test_BaseItemCode                                   400005
#define GSM_FrequencyError_Test_BaseItemCode                             400006
#define GSM_Swit_ORFS_Test_BaseItemCode                                  400007
#define GSM_MOD_ORFS_Test_BaseItemCode                                   400008
#define GSM_RXLev_Test_BaseItemCode                                      400009
#define GSM_TxCurrent_Test_BaseItemCode                                  400010
#define GSM_RXBER_Test_BaseItemCode                                      400011
#define GSM_GSMTestEnd_BaseItemCode                                      400099

//-------------------------------------------------------------------------------------------------
//	EDGE Test BaseItemCode
//------------------------------------------------------------------------------------------------- 
#define EDGE_EDGEInit_Test_BaseItemCode                                  500000
#define EDGE_EDGECall_Test_BaseItemCode                                  500001
#define EDGE_MaxPower_Test_BaseItemCode                                  500002
#define EDGE_EDGEEVMRMS_Test_BaseItemCode                                500003
#define EDGE_EDGEEVM95_Test_BaseItemCode                                 500004
#define EDGE_FrequencyError_Test_BaseItemCode                            500005
#define EDGE_EDGEORFSSWITCH_Test_BaseItemCode                            500006
#define EDGE_EDGEORFSMOD_Test_BaseItemCode                               500007
#define EDGE_Burst_Test_BaseItemCode                                     500008
#define EDGE_EDGEPeakEVM_Test_BaseItemCode                               500009
#define EDGE_HandOver_Test_BaseItemCode                                  500098
#define EDGE_EDGETestEnd_BaseItemCode                                    500099

//-------------------------------------------------------------------------------------------------
//	3G Calibration BaseItemCode
//------------------------------------------------------------------------------------------------- 
#define WCDMA_InitWCDMACal_BaseItemCode                                    300000
#define WCDMA_Therm_Calibration_BaseItemCode                               300001
#define WCDMA_DoDC_Calibration_BaseItemCode                                300002
#define WCDMA_WCDMATXLin_Calibration_BaseItemCode                          300003
#define WCDMA_WCDMATXRXAlign_Calibration_BaseItemCode                      300004
#define WCDMA_EndWCDMACal_BaseItemCode                                     300099

//-------------------------------------------------------------------------------------------------
//	3G Test BaseItemCode
//------------------------------------------------------------------------------------------------- 
#define WCDMA_InitWCDMATest_BaseItemCode                                   600000
#define WCDMA_WCDMATestCall_BaseItemCode                                   600001
#define WCDMA_MaxPower_Test_BaseItemCode                                   600002
#define WCDMA_MinPower_Test_BaseItemCode                                   600003
#define WCDMA_EVMRMSAvg_Test_BaseItemCode                                  600004
#define WCDMA_FrequencyError_Test_BaseItemCode                             600005
#define WCDMA_TransTimeError_Test_BaseItemCode                             600006
#define WCDMA_EMASK_Test_BaseItemCode                                      600007
#define WCDMA_OccupiedBandWidth_Test_BaseItemCode                          600010
#define WCDMA_BER_Test_BaseItemCode                                        600011
#define WCDMA_RxLevel_Test_BaseItemCode                                    600012

#define WCDMA_ILPC_E_Test_BaseItemCode                                     600014
#define WCDMA_ILPC_F_Test_BaseItemCode                                     600015

#define WCDMA_ACLR_N10_Test_BaseItemCode                                   600020
#define WCDMA_ACLR_N5_Test_BaseItemCode                                    600021
#define WCDMA_ACLR_P5_Test_BaseItemCode                                    600022
#define WCDMA_ACLR_P10_Test_BaseItemCode                                   600023

#define WCDMA_TxCurrent_Test_BaseItemCode                                  600091
#define WCDMA_Handover_Test_BaseItemCode                                   600098
#define WCDMA_EndWCDMATest_BaseItemCode                                    600099

//-------------------------------------------------------------------------------------------------
//	WireLess Test BaseItemCode
//------------------------------------------------------------------------------------------------- 
#define WireLess_InitGSMTest_BaseItemCode                               700000
#define WireLess_GSMStartCall_BaseItemCode                              700001
#define WireLess_GSM_MaxPower_Test_BaseItemCode                         700002
#define WireLess_GSM_RxLevel_Test_BaseItemCode                          700003
#define WireLess_GSMTest_BaseItemCode                                   700004

#define WireLess_GSM850_MeasureLoop_Test_BaseItemCode                   700048
#define WireLess_EndGSMTest_BaseItemCode                                700049
#define WireLess_InitWCDMATest_BaseItemCode                             700050
#define WireLess_WCDMAStartCall_BaseItemCode                            700051
#define WireLess_WCDMA_MaxPower_Test_BaseItemCode                       700052
#define WireLess_WCDMA_RxLevel_Test_BaseItemCode                        700053
#define WireLess_WCDMATest_BaseItemCode                                 700054

#define WireLess_CDMA2000Test_BaseItemCode                              700060

#define WireLess_WCDMA850_MeasureLoop_Test_BaseItemCode                 700098
#define WireLess_EndWCDMATest_BaseItemCode                              700099





//-------------------------------------------------------------------------------------------------
//	Misc BaseItemCode
//------------------------------------------------------------------------------------------------- 
#define BT_Test_BaseItemCode                                            100001
#define WLAN_Test_BaseItemCode                                          100002
#define FM_Test_BaseItemCode                                            100003
#define GPS_Test_BaseItemCode                                           100004

#define StartPowersupply_BaseItemCode                                   100011
#define StopPowersupply_BaseItemCode                                    100012

#define LeakageCurrent_BaseItemCode                                     100021
#define StandbyCurrent_BaseItemCode                                     100022
#define MobileOnCurrent_BaseItemCode                                    100023
#define LEDCurrent_Test_BaseItemCode                                    100024
#define BootCurrent_BaseItemCode										100025
#define ChargeCurrent_BaseItemCode										100026

#define WritePicasso_BaseItemCode                                       100031
#define ReadPicasso_BaseItemCode                                        100032
#define ComparePiasscoNumber_BaseItemCode                               100033
#define WriteMobileID_BaseItemCode                                      100034
#define ReadMobileID_BaseItemCode                                       100035          
#define WriteSWHWVersion_BaseItemCode                                   100036
#define WriteVersion_BaseItemCode                                       100037
#define CheckVersion_BaseItemCode                                       100038
#define HWSWVersionCheck_BaseItemCode                                   100039
#define Write2GStationInfo_BaseItemCode                                 100040
#define Check2GStationInfo_BaseItemCode                                 100041
#define Write3GStationInfo_BaseItemCode                                 100042
#define Check3GStationInfo_BaseItemCode                                 100043
#define WriteAttenaStationInfo_BaseItemCode                             100044
#define CheckAttenaAtationInfo_BaseItemCode                             100045
#define CheckPicassoMobileID_BaseItemCode                               100046
#define CheckHWID_BaseItemCode											100047
#define CheckPicassoInSMTSys_BaseItemCode                               100048
#define CheckSWVersionMT_BaseItemCode									100049

#define StopCoinCharge_BaseItemCode                                     100061
#define StartCoinCharge_BaseItemCode                                    100062
#define USBChargeStart_BaseItemCode                                     100063
#define USBChargeStop_BaseItemCode                                      100064
#define CoinCellVoltage_BaseItemCode                                    100065
#define BatteryVoltage_BaseItemCode                                     100066
#define BatteryDetectSwitch_BaseItemCode                                100067
#define BatteryTemperature_Test_BaseItemCode                            100068
#define USBIsCharging_BaseItemcode                                      100069
#define USBChargeCurrent_BaseItemcode                                   100070
#define Monitor_BaseItemcode                                   100071

#define WriteQfuse_BaseItemCode                                         100081
#define CheckQfuse_BaseItemCode                                         100082
#define LeopardCheckQfuse_BaseItemCode									100083
#define LeopardWriteQfuse_BaseItemCode									100084


#define IsPhoneConnected_BaseItemCode                                   100091
#define EnterFTM_BaseItemCode                                           100092
#define EnterOnline_BaseItemCode                                        100093
#define ResetHandSet_BaseItemCode                                       100094
#define SetOnlineMode_BaseItemCode                                      100095
#define EnterPowerOffMode_BaseItemCode                                  100096
#define CheckAndChangeMode_BaseItemCode									100097
#define ChangeFTDtoNormal_BaseItemCode									100098

#define SimCardExistence_BaseItemCode                                   100101
#define GsernsorEpassid_BaseItemCode                                    100102
#define BoardID_BaseItemCode                                            100103
#define SDCard_Test_BaseItemCode                                        100104
#define LED_Test_BaseItemCode                                           100105
#define GsernsorXYZ_BaseItemCode                                        100106
#define GsernsorOn_BaseItemCode                                         100107
#define GsernsorOff_BaseItemCode                                        100108
#define SetRTC_BaseItemCode                                             100109
#define BackupNV_BaseItemCode                                           100110
#define LeopardGSensorTest_BaseItemCode									100111
#define ECompass_BaseItemCode											100112
#define GaugeICIDTest_BaseItemCode										100113
#define LeopardSDCardSize_BaseItemCode									100114
#define LeopardSDCardStatus_BaseItemCode								100115
#define WriteOneNV_BaseItemCode											100116
#define WriteEFSFile_BaseItemCode										100117


#define RFConnector_BaseItemCode                                        100120 
#define GSensorTestGetID_BaseItemCode                                   100121
#define GSensorTestIDFunc_BaseItemCode                                  100122
#define StartMotoUSB_BaseItemCode                                       100123
#define StopMotoUSB_BaseItemCode                                        100124
#define SaveHOB_BaseItemCode                                            100125
#define TrackID_BaseItemCode                                            100126
#define WriteFlashID_BaseItemCode                                       100127
#define ReadFlashID_BaseItemCode                                        100128
#define CheckFlashID_BaseItemCode                                       100129
#define WriteFactoryInfo2G_BaseItemCode                                 100130
#define CheckFactoryInfo2G_BaseItemCode                                 100131
#define WriteFactoryInfo3G_BaseItemCode                                 100132
#define CheckFactoryInfo3G_BaseItemCode                                 100133
#define CheckFactoryInfoCIT_BaseItemCode                                100134
#define WriteFactoryInfoWireless_BaseItemCode                           100135
#define WritePCBID_BaseItemCode                                         100136
#define CheckPCBID_BaseItemCode                                         100137
#define WriteSMTTime_BaseItemCode                                       100138
#define WriteServiceByte_BaseItemCode                                   100139
#define WriteTransceiverNum_BaseItemCode                                100140
#define ResetProcessInfo_BaseItemCode									100141

#define AudioLoopBackTestStart_BaseItemCode                             100143
#define AudioLoopBackTestStop_BaseItemCode                              100144
#define LEDTestStart_BaseItemCode                                       100145
#define LEDTestStop_BaseItemCode                                        100146
#define GPIOTest_BaseItemCode                                           100147
#define UARTTest_BaseItemCode                                           100148
#define SIMTest_BaseItemCode                                            100149

#define PopUSB_BaseItemCode												100150
#define PushUSB_BaseItemCode                                            100151

/* 110729 Gilpin add */
#define GYROIDTest_BaseItemCode											100152
#define CAPKEYCHIPTest_BaseItemCode                                     100153
#define CHARGERICTest_BaseItemCode                                      100154
#define EMMCCard_Test_BaseItemCode                                      100155
#define DDR_Test_BaseItemCode											100156
#define LCD_Test_BaseItemCode											100157
#define Backlight_Test_BaseItemCode										100158
#define KEYPAD_Test_BaseItemCode										100159
#define TOUCH_DownloadFirmware_BaseItemCode								100160
#define TOUCHID_Test_BaseItemCode										100161
#define TOUCH_Test_BaseItemCode											100162
#define GSensor_Calibration_BaseItemCode								100163
#define Gauge_DFI_Calibration_BaseItemCode								100164
#define Gauge_DFIIT_Operator_BaseItemCode								100165
#define Gauge_Check_DFI_Version_BaseItemCode							100166

/* 110818 Alex add */
#define JigRequest_In_BaseItemCode										100170
#define JigRequest_Out_BaseItemCode										100171
#define LSensor_ReadID_BaseItemCode										100172
#define PSensor_ReadID_BaseItemCode										100173
#define LSensor_Calibration_BaseItemCode                                100174
#define PSensor_Calibration_BaseItemCode                                100175
#define HallSensor_Calibration_BaseItemCode                             100405

#define LPSensor_Test_BaseItemCode										100176
#define Vibrator_Vibrate_BaseItemCode									100177
#define Audio_Speaker_BaseItemCode										100178
#define Audio_Headset_BaseItemCode										100179
#define Audio_Receiver_BaseItemCode										100180

#define HDMITest_BaseItemCode											100185
#define FMRrssi_Test_BaseItemCode										100186

/* 111118 Neil add */
#define ReadBTID_BaseItemCode											100190
#define ReadPWRgaugeID_BaseItemCode										100191
#define ReadPWRchargerID_BaseItemCode									100192
#define ReadNFCID_BaseItemCode											100193
#define ReadSNSRgsensorID_BaseItemCode									100194
#define ReadSNSRecompassID_BaseItemCode									100195
#define ReadSNSRlsensorID_BaseItemCode									100196
#define ReadSNSRpsensorID_BaseItemCode									100197
#define ReadCPUid_BaseItemCode											100198
#define ReadVERftd_BaseItemCode											100199
#define ReadBoardADC_BaseItemCode										100200
#define ReadBATTrtcTime_BaseItemCode									100201
#define ReadPWRgaugeVoltage_BaseItemCode								100202
#define ReadPWRgaugeCurrent_BaseItemCode								100203
#define ReadPWRgaugeTemperature_BaseItemCode							100204
#define ReadMCPflashID_BaseItemCode										100205
#define ReadMCPddrID_BaseItemCode										100206
#define ReadGPSValueCN0_BaseItemCode									100207
#define ReadVERchange_BaseItemCode										100208

#define ReadSNSRgsensorAcc_BaseItemCode									100209
#define ReadSNSRlsensorValue_BaseItemCode								100210
#define ReadSNSRpsensorValue_BaseItemCode								100211
#define ReadLEDid_BaseItemCode											100212
#define ReadBATTCoinCellVoltage_BaseItemCode							100213

#define ReadNFCselftest_BaseItemCode									100214
#define ReadBoardRev_BaseItemCode										100215
#define DFIupgrade_BaseItemCode											100216
#define ALS_ReadID_BaseItemCode											100217

/* 120413 Evonne add */
#define Camera_Test_BaseItemCode										100218
#define PlayWav_Test_BaseItemCode										100219
#define Loopback_Test_BaseItemCode										100220

/* 120604 Evonne add */
#define PowerConnector_Test_BaseItemCode								100221
#define TOUCHMODEL_Test_BaseItemCode									100222
#define NFCMODEL_Test_BaseItemCode										100223


/*   Lion Add */
#define SensorLog_BaseItemCode											100300
#define Flash_LED_BaseItemCode											100301
#define GYROSelfTest_BaseItemCode										100302
#define GYROTest_BaseItemCode										    100303
#define BatteryCapacity_BaseItemCode									100304
#define Smart_Cover_BaseItemCode										100305
#define Indicator_LED_BaseItemCode										100305


/*   Eason Add */
//DDR
#define MCPddrmr8_BaseItemCode									     	100350
#define MCPemmcsize_BaseItemCode								     	100351
#define MCPemmcFWversion_BaseItemCode							     	100352
#define MCPddrmr6_BaseItemCode									     	100353
//ID


#define AudioID_BaseItemCode							     			100370
#define RF_PA_QFE_ID_BaseItemCode							     		100371
#define RF_ASM_ID_BaseItemCode							     			100372
#define Board_ID_BaseItemCode							     			100373
#define SKU_ID_BaseItemCode							     				100374
#define SOCversion_BaseItemCode							     			100375
#define SOCfeature_ID_BaseItemCode							     		100376

//YTT
#define PMid_BaseItemCode							     		        100388
#define PCBVerMPP3_BaseItemCode							     		    100389
#define PCBVerMPP2_BaseItemCode							     		    100390
#define PMIid_BaseItemCode							     		        100391
#define APQ_UART_BaseItemCode							     		    100392
#define	HDMI_OUTPUT_BaseItemCode							     		100393
#define	WLAN_MODULE_BaseItemCode							     		100394
#define	BT_INQUIRY_BaseItemCode							     		100394

//RF
#define RF_Band_ID_Temperature_BaseItemCode								100377
#define PMICID_BaseItemCode							     				100378
//WLAN
#define WLAN_SelfTest_BaseItemCode							     		100380
//Current

#define Charging_Average_Current_BaseItemCode							100385
#define Power_Max_Current_BaseItemCode									100386


#define ReadFPGA_ID_BaseItemCode										100387
//SelfTest

#define Gsensor_SelfTest_BaseItemCode                               100400
#define ECompass_SelfTest_BaseItemCode                              100401

#define FM_SelfTest_BaseItemCode										100402
#define BT_SelfTest_BaseItemCode										100403



#define ReadBuckID_BaseItemCode											100500
#define ReadBuckfeature_BaseItemCode									100501
#define ReadBacklightDriver_BaseItemCode								100502


#endif