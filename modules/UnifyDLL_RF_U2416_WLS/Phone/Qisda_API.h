#ifndef _QISDA_API_H_
#define _QISDA_API_H_

typedef int (__stdcall * PFGenCmdInOut)(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);

typedef bool (*pNV_IsConnect_SPU)(const char*szPort);
typedef bool (*pNV_ResetHandset_SPU)(const char* szPort);
typedef bool (*pNV_ReadItem)(const int port, const WORD NVItem, BYTE* pBuffer, long BufferSize, WORD *pnv_stat);
typedef bool (*pNV_WriteItem)(const int port, const WORD NVItem, BYTE* pBuffer, long BufferSize, WORD *pnv_stat);
typedef bool (*pNV_BackupPartition)(const char* szPort);
typedef bool (*pNV_RestorePartition)(const char* szPort);

typedef bool (*pSensor_GetComponentValue_SPU)(const char* szPort, const char* szID, const char* szSubID,char* szValue);
typedef bool (*pSensor_CheckGsensor_SPU)(const char* szPort);
typedef bool (*pSensor_CheckECompass_SPU)(const char* szPort);

typedef bool (*pSD_GetSize_SPU)(const char* szPort, char* szSize);
typedef bool (*pSD_GetStatus_SPU)(const char* szPort, char* szSize);

typedef bool (*pWP7_ReadHWversion_SPU)(const char* szPort, char* szVarientVer, char* szMemoryVer, char* szEvtVer, char* szEvtMinorVer, char* szPilotRunVer);
typedef bool (*pWP7_ReadSWversion_SPU)(const char* szPort, char* szVersion);

// AT command
typedef bool (*pATControl_SPU)(char* szPort, char* szCommand, char* szResponse);

// FA
typedef bool (*pFA_Read)(const char* szPort, char* szFAData, char* szErrCode, char* szErrMsg);
typedef bool (*pFA_Write)(const char* szPort, char* szFAData, char* szErrCode, char* szErrMsg);

// BT
typedef bool (*pBT_ModuleOn_SPU)(const char* szPort);
typedef bool (*pBT_ModuleOff_SPU)(const char* szPort);
typedef bool (*pBT_PowerOff_SPU)(const char* szPort);
typedef bool (*pBT_StartPower_SPU)(const char* szPort, char* szDegree);

// WiFi
typedef bool (*pWiFi_ModuleSwitchOn_SPU)(const char* szPort);
typedef bool (*pWiFi_ModuleSwitchOff_SPU)(const char* szPort);
typedef bool (*pWiFi_TurnOnTXPower2_SPU)(const char* szPort, char* szRate, char* szChannel, char* szPower);

// GPS
typedef bool (*pGPS_TrackingSwitch_SPU)(const char* szPort, const char* szOnOffSwitch);
typedef bool (*pGPS_GetCNOvalue_SPU)(const char* szPort, char* szCNOvalue);
typedef bool (*pGPS_PollingTTFFCN0_SPU)(const char* szPort, char* TTFF, char* CN0);

// Power
typedef bool (*pPW_GetGaugeDeviceName_SPU)(const char *szPort, char *szDevName);
typedef bool (*pPW_GetChargeICDeviceID_SPU)(const char* szPort, char* szBuffer);
typedef bool (*pPW_USBChargingStatus_SPU)(const char *szPort, char* szStatus);
typedef bool (*pPW_USBChargeStart_SPU)(const char* szPort);
typedef bool (*pPW_USBChargeStop_SPU)(const char* szPort);
typedef bool (*pPW_ReadBatteryVoltage_SPU)(const char* szPort, char* szVolt);
typedef bool (*pPW_GetCoinCellVoltage_SPU)(const char* szPort, char* szVoltage);

// NFC
typedef bool (*pNFC_IsConnected_SPU)(const char* szPort);

// NLED
typedef bool (*pNLED_IsConnected_SPU)(const char* szPort);

// Speaker
typedef bool (*pAudio_SpeakerAmpTest_SPU)(const char* szPort);

// EFs
typedef bool (*pEFS_Sync_Combo_SPU)(char *szPort, char *szRetryTime);
typedef bool (*pEFS_GoldenCopy_SPU)(char *szPort);

// FM
typedef bool (*pFM_TuneFM_SPU)(char* szPort, char* szFMFrequency);
typedef bool (*pFM_ConnectFM_SPU)(char* szPort, char* szbConnect, char* szTimeout);
typedef bool (*pFM_GetRSSI_SPU)(char* szPort, char* szRSSI);

// LCD
typedef bool (*pWolf_LCDOnOffSwitch_SPU)(const char* szPort, char* szOnOff);

// Detroit FTD basic commands
typedef int (__stdcall *pInitDevice)(int PortNum, int nTimeout);
typedef int (__stdcall *pFreeDevice)(int PortNum, int nTimeout);
typedef int (__stdcall *pDetroit_TestFtdMode)(int nPort, int nTimeout, char* ErrorCode);
typedef int  (__stdcall * pDetroit_FAC_CFGWrite)(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
typedef bool (__stdcall * PFExecAdbOutFunc)(CString Command, char* output, char* ErrorCode);

// Detroit GPS
typedef int  (__stdcall * pDetroit_GPSValueCN0)(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);

// Detroit FMR
typedef int  (__stdcall * pDetroit_FMRrssi)(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);

typedef int  (__stdcall * pSapporo_FSG_Enable)(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);

// Detroit WiFi
typedef int (__stdcall *pDetroit_WLAN_Mode)(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
typedef int (__stdcall *pDetroit_WLAN_TXPower)(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
typedef int  (__stdcall * pDetroit_WLAN_RXrssi)(int nPort, int nTimeout, char* InBuffer, char* OutBuffer); // cmd: 3
typedef int  (__stdcall * pDetroit_WLAN_RXset)(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);

typedef int  (__stdcall * pDetroit_WLANLoadFABin)(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
typedef int  (__stdcall * pDetroit_WLANLoadNextBin)(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
typedef int  (__stdcall * pDetroit_WLANSaveBinToFA)(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
typedef int  (__stdcall * pDetroit_WLAN_Mode)(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);

// Detroit BT
typedef int (__stdcall *pDetroit_BTTestMode)(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
typedef int (__stdcall *pDetroit_BTTXPower)(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);

typedef int (__stdcall *pDetroit_BTReset)(int nPort, int nTimeout,char* InBuffer,char* ErrorCode);

// Detroit QmiRf
typedef int (__stdcall *pDetroit_QmiRfTxTest)(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);
typedef int (__stdcall *pDetroit_QmiRfTxOff)(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);

// Detroit NV Backup
typedef bool (*pDetroit_NVBackup_SPU)(char *szPort);

// Detroit Read from FA
typedef int (__stdcall *pDetroit_FAC_FARead)(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) ;
typedef int (__stdcall *pDetroit_FAC_FAWrite)(int nPort, int nTimeout, char* InBuffer, char* OutBuffer) ;

// NFC
typedef int  (__stdcall * pFTD_NFCHCITypeAReader)(int nPort, int nTimeout, char* InBuffer, char* OutBuffer);

#endif // End of #ifndef _QISDA_API_H_
