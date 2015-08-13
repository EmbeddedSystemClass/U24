#include "..\Lib\RequestUtility\InterfaceBase.h"


#ifdef QCOM_DIAG_MODULE_EXPORTS
#define QCOM_DIAG_API extern "C" __declspec(dllexport)
#define QCOM_DIAG_CLASS_API __declspec(dllexport)
#else
#define QCOM_DIAG_API extern "C" __declspec(dllimport)
#define QCOM_DIAG_CLASS_API __declspec(dllimport)
#endif


// CQcomDiagModuleApp
// See QcomDiagModule.cpp for the implementation of this class
//

bool TransferCharToBool(const char* szBool);
bool HexStringToByteArray(const char *szHexData, BYTE *&ByteArray, size_t &Len);
void ByteArrayToHexString (BYTE* pByte, char* chString, long ByteCount);


class QCOM_DIAG_CLASS_API CQcomDiagModule : public CInterfaceBase
{
public:
	CQcomDiagModule(int nCOMPort);
	virtual ~CQcomDiagModule(void);

	// SD_Diag.c 
	bool SD_GetSize (DWORD *dwSize);
	bool SD_GetStatus (DWORD *dwStatus);
	bool SD_GetSizenStatus_Sys (DWORD *dwSize, DWORD *dwStatus);

	// GPS_Diag
	bool GPS_GetIQValue (WORD *pIValue, WORD *pQValue);
	bool GPS_GetCNOvalue (WORD *pCNO);
	bool GPS_SVTrack(BYTE cmd, BYTE SVnumber);
	bool GPS_SVTrack_byLog (BYTE cmd, BYTE SVnumber, WORD Threshold, WORD* pCNOvalue, bool bWaitLog = true);
	bool GPS_EnableLog (bool bEnable, WORD wPacketNo);
	bool GPS_EnterSA_RF_VerifMode (bool bEnable, WORD wPacketNo, 
								const int UpBound=0, const int LoBound=0, const int PacketCount=0, 
								int *pIValue=NULL, int *pQValue=NULL);
	bool GPS_TrackingSwitch(bool bOn);
	bool GPS_PollingTTFFCN0(int& iTTFF, int& iCN0);
	bool GPS_ClearData(int nCommand);
	bool GPS_EnterGPS(bool bEnter);

	// Sensor
	bool Sensor_CheckGsensor();
	bool Sensor_CalibrateGSensor();
	bool Sensor_IsCalibratedGSensor(BYTE& nByteParameter1, BYTE& nByteParameter2, BYTE& nByteParameter3);
	bool Sensor_SelfTestGSensor();
	bool Sensor_CheckECompass();
	bool Sensor_LPSensor(int nFunction,BYTE& nByteParameter1,BYTE& nByteParameter2,BYTE& nByteParameter3,BYTE& nByteParameter4,int& nValue,bool& bSuccess);
	bool Sensor_LPSensor(int nFunction,DWORD *pdata,int& nValue,bool& bSuccess);
	bool Sensor_GetComponentValue(int nID, int nSubID, std::string& strID,bool& bSuccess);

	// FM
	bool FM_ConnectFM (bool bConnect, int nTimeout);
	bool FM_TuneFM (DWORD dwFrequency);
	bool FM_GetRSSI (WORD *pRSSI);

	// BT
	bool BT_ModuleSwitch (bool bOn);
	bool BT_StartPower (int nDegree);
	bool BT_PowerOff() ;
	bool BT_ModeRequest();

	// WiFi
	bool WiFi_ModuleSwitch(bool bOn);
	bool WiFi_ModuleCheck(bool& bOn);
	bool WiFi_ChangeIfToNative();
	bool WiFi_TurnOnTXPower(BYTE nChannel);
	bool WiFi_TurnOnTXPower2(BYTE Rate, BYTE nChannel, BYTE Power);
	bool WiFi_TurnOnTXPower2ChN(BYTE Rate, BYTE nChannel, BYTE Power);
	bool WiFi_TurnOnRXPower2(BYTE Rate, BYTE nChannel, int& Data1, int& Data2, int& Data3);
	bool WiFi_TurnOnRXPower2ChN(BYTE Rate, BYTE nChannel, int& Data1, int& Data2, int& Data3);
	bool Wifi_StartTx(int rate, int channel, int power);
	bool Wifi_ReadMacAddress(char* szAddress);

	// WP7
	bool WP7_WritePVK();
	bool WP7_WritePVK_Wolf(const char* pFileSrc, char* szError);
	bool WP7_PVKisWritten_Wolf(char *szStatus);
	bool WP7_DelPVK();
	bool WP7_ClearPVK_Wolf();
	bool WP7_CheckPVK(int &status);
	bool WP7_CheckPVK_Wolf(char* szError);
	bool WP7_BackUpPVK_Wolf(char* szError);
	bool WP7_RestorePVK_Wolf(char* szError);
	bool WP7_MasterClear();
	bool WP7_Reset() ;
	bool WP7_ReadHWVersion(int& nVariant, int& nMem, int& nEvt, int& nEvtMinor, int& nPilotRun);
	bool WP7_ReadSWVersion(char* SWversion);
	bool WP7_ReadHWVersion_Wolf (int& nVariant, int& nPCBStage, int& nMajorSN, int& nMinorSN, int& nMem, int& neMMC, int& nTouchPanel, int& nModule);
	bool WP7_PowerOffMode(int nMode);
	bool WP7_CIT_SpecifyFileName(const char* szFileDest);
	bool WP7_CIT_Write(const char* szFileSrc, char* szError);
	bool WP7_CIT_Read(const char* szFileDest, char* szError);
	bool WP7_Setup_Sleep(const char* szSleep);

	// PW
	bool PW_ReadBatteryVoltage(int& nVoltage);
	bool PW_USBChargeStop();
	bool PW_USBChargeStart();
	bool PW_USBChargingStatus(bool &isCharging);
	bool PW_GetGaugeDeviceName(std::string &DevName);
	bool PW_GetChargeICDeviceID(BYTE& byID);
	bool PW_GetCoinCellVoltage(int& iVoltage);
	bool PW_PowerKeyEvent();

	// EFs
	bool EFs_GoldenCopy();
	bool EFS_SyncNV_NoWait(DWORD &token);
	bool EFS_Sync_Status(DWORD token);
	bool EFs_BackupCheck();
	bool EFs_GoldenCopyCheck(int *status);

	// eMMC
	bool eMMC_GetSize (DWORD *dwSize);

    // FA_Station

	// NFC
	bool NFC_IsConnected();

	// LED
	bool NLED_IsConnected();

	// Audio
	bool Audio_SpeakerAmpTest();
    bool Audio_IsHandsetPlugged();
    bool Audio_VibratorOnOff(bool bOn);

	// Camera
	bool Camera_WolfRequest (BYTE index);
	bool Camera_WolfAFStatus();
	bool Camera_DrawPoint(CString FilePath, CString InputPoints);

	// Touch
	bool Touch_UpdateTouchFW();
	bool Touch_CalibrateTouch();
	bool Touch_GetTouchStream (char *szTouchStream);
	bool Touch_GetTouchDelta (char *szTouchDelta);
	bool Touch_GetTouchIDAC (char *szTouchIDAC);
	bool Touch_GetTouchBaseLine (char *szTouchBaseLine);
	bool Touch_EnableSensor();
	bool Touch_DisableSensor();
	bool Touch_EnterTestPage();
    bool Touch_CheckVersion(char* szTouchVersion);
	bool Touch_ClearStreamBuffer();

	// LCD
	bool Wolf_LCDOnOffSwitch(bool bOn);
	
	//Temp
	bool Temp_UpdateDFIFW(char* szErrCode);
	bool CreateBatFile (const TCHAR* pFileName, const TCHAR* pFFUFile, bool bList);
	void AppendToCurrentPath (TCHAR* szNewBuffer, DWORD dwBufSize, const TCHAR* szFileName);
	void GetDirFullPath (const TCHAR* szFilePath, TCHAR* szDirName, long desSize);

	// Phone Info
	bool CQcomDiagModule::PhoneInfo_GetPINStatus();

	//PWP
	bool Wolf_Ini_PWPData(BYTE Op) ;
	bool Wolf_WriteSerialNumber(char* szDevNum) ;
	bool Wolf_ReadSerialNumber(char* szDevNumber) ;
	bool Wolf_WritePublicKey(const char* szFileSrc) ;
	bool Wolf_ReadPublicKey(char* szPubKey) ;
	
	// add for orange [7/26/2011 Kevin.Dong]
	bool ReadHWID(char* szHWID);
	bool ReadFA(char* szOffset,char* szFA);
	bool WriteFA(char* szOffset,char* szData);
	bool CheckSWVersion(char* szData);
	bool CoinCharge(bool bFlag);
	bool BatteryCharge(bool bFlag);
	bool FileSystemBackup(char* szUserData);


	bool ReadFA_Sector(char* szOffset,char* szFA,char* szSectorSize);
	bool WriteFA_Sector(char* szOffset,char* szFA,char* szSectorSize);
	bool ReadCoinCellVoltage(char* szVoltage);


	//Sim lock
	bool GetSimLockName(char *szLockName);
	bool GetSimLockNameNew(char *szLockName);


	// QFuse
	bool Wolf_WriteQFuse();
	bool Wolf_ReadQFuse (BYTE* pBuffer, long bufsize);
	bool Wolf_VerifyQFuse();

	// For Chicago
	bool C7_NVBackup() ;
	bool C7_NVRestore() ;
	bool C7_DRMBackup();

	// For A9
	bool A9_OS_ReadModemVersion(char* szModemVersion);
	bool A9_OS_IsPhoneReady(char* szMaxWaitTime, char* szRetryPeriod, char* szErrMsg,
							char* szIdentify = "Acer_", char* szIdentifyLength = "5") ;

};


// BT
QCOM_DIAG_API bool BT_ModuleOn_SPU(const char* szPort);
QCOM_DIAG_API bool BT_ModuleOff_SPU(const char* szPort);
QCOM_DIAG_API bool BT_StartPower_SPU (const char* szPort, char* szDegree) ;
QCOM_DIAG_API bool BT_PowerOff_SPU(const char* szPort) ;
QCOM_DIAG_API bool BT_ModeRequest_SPU(const char* szPort) ;
QCOM_DIAG_API bool BT_GetAddr_SPU(const char* szPort, char* szAddr);

// EFs
QCOM_DIAG_API bool EFs_GoldenCopy_SPU(char* szPort);
QCOM_DIAG_API bool EFS_DeleteFile_SPU(char*szPort, char *sFileDst);
QCOM_DIAG_API bool EFS_SyncNV_NoWait_SPU(char*szPort, char *szToken, char *szRetryTime);
QCOM_DIAG_API bool EFS_Sync_Status_SPU(char*szPort, char *szToken, char *szRetryTime);
QCOM_DIAG_API bool EFS_Sync_Combo_SPU(char *szPort, char *szRetryTime);
QCOM_DIAG_API bool EFs_BackupCheck_SPU(char* szPort);
QCOM_DIAG_API bool EFs_GoldenCopyCheck_SPU(char* szPort, char* szStatus);

// eMMC
QCOM_DIAG_API bool eMMC_GetSize_SPU (const char* szPort, char* szSize) ;

// FA_Station
QCOM_DIAG_API bool FA_Station_Setup (char* szPort, char* szStationAmount, char* szStations, char* szErrCode, char* szErrMsg) ;
QCOM_DIAG_API bool FA_ReadStationInfo_SPU (char* szPort, char* szStationIndex, char* szLineType, char* szTestResult, char* szTestCount,
                                           char* szTestDateTime, char* szTestLineStation, char* szReserve, char* szErrCode, char* szErrMsg ) ;
QCOM_DIAG_API bool FA_WriteStationInfo_SPU (char* szPort, char* szStationIndex, char* szLineType,
                                           char* szTestResult, char* szErrCode, char* szErrMsg) ;
QCOM_DIAG_API bool FA_ReadAllStationInfo (char* szPort, char* szErrCode, char* szErrMsg) ;
QCOM_DIAG_API bool FA_EraseTargetStationInfo_SPU (char* szPort, char* szIndex, char* szErrCode, char* szErrMsg) ;
QCOM_DIAG_API bool FA_Read (const char* szPort, char* szFAData, char* szErrCode, char* szErrMsg);
QCOM_DIAG_API bool FA_Write (const char* szPort, char* szFAData, char* szErrCode, char* szErrMsg);

// FM
QCOM_DIAG_API bool FM_ConnectFM_SPU(const char* szPort, char* szbConnect, char* szTimeout);
QCOM_DIAG_API bool FM_TuneFM_SPU(const char* szPort, char* szFMFrequence);
QCOM_DIAG_API bool FM_GetRSSI_SPU(const char* szPort, char* szRSSI);

// GPS_Diag.c
QCOM_DIAG_API bool GPS_GetIQValue_SPU (const char* szPort, char* Ivalue, char* Qvalue);
QCOM_DIAG_API bool GPS_GetCNOvalue_SPU (const char* szPort, char* szCNOvalue);
QCOM_DIAG_API bool GPS_SVTrack_SPU (const char*szPort, char* szCmd, char* szSVnumber);
QCOM_DIAG_API bool GPS_SVTrack_byLog_SPU (const char*szPort, char* szCmd, char* szSVnumber, const char* szThreshold, char* szCNOvalue);
QCOM_DIAG_API bool GPS_EnableLog_SPU (const char* szPort, const char* szbEnable,const char* packetNo);
QCOM_DIAG_API bool GPS_EnterSA_RF_VerifMode_SPU (const char* szPort, const char* szbEnable, const char* packetNo, 
												 const char* szUpbound = NULL, const char* szLoBound = NULL, const char* szPacketCount = NULL, //IQ value input
												 char* IValue = NULL, char* QValue = NULL); //IQ value output
QCOM_DIAG_API bool GPS_TrackingSwitch_SPU(const char* szPort, const char* szOnOffSwitch);
QCOM_DIAG_API bool GPS_PollingTTFFCN0_SPU(const char* szPort, char* TTFF, char* CN0);
QCOM_DIAG_API bool GPS_ClearData_SPU(const char* szPort, const char* szStartClear);
QCOM_DIAG_API bool GPS_EnterGPS_SPU (const char* szPort, const char* bEnter);

// NV
QCOM_DIAG_API bool NV_IsConnect_SPU (const char* szPort);
QCOM_DIAG_API bool NV_ResetHandset_SPU (const char* szPort);
QCOM_DIAG_API bool NV_ReadItem (const int port, const WORD NVItem, BYTE* pBuffer, long BufferSize, WORD *pnv_stat);
QCOM_DIAG_API bool NV_ReadItem_SPU (const char* szPort, const char* NVItem, char* pBuffer, char* BufferSize, char *pnv_stat);
QCOM_DIAG_API bool NV_WriteItem (const int port, const WORD NVItem, BYTE* pBuffer, long BufferSize, WORD *pnv_stat);
QCOM_DIAG_API bool NV_WriteItem_Radix_SPU (const char* szPort, const char* NVItem, char* pBuffer, char* pradix, char* BufferSize, char *pnv_stat);
QCOM_DIAG_API bool NV_ReadNV453_SPU (const char* szPort, char* value);
QCOM_DIAG_API bool NV_WriteNV453_SPU (const char* szPort, char* value);
QCOM_DIAG_API bool NV_BackupPartition (const char* szPort);
QCOM_DIAG_API bool NV_RestorePartition (const char* szPort);
QCOM_DIAG_API bool NV_ResetProcessInfo (char* szPort,char* szProcessIndex);
QCOM_DIAG_API bool NV_ReadProcessInfo (char* szPort,char* szProcessIndex,char* szYear,char* szMonth,char* szDay,char* szLine,char* szStation);
QCOM_DIAG_API bool NV_WriteItem_64Base_SPU (const char* szPort, const char* szNVItem, char* szWriteData_Base64, char* sznDataLen,char* szNV_stat);
QCOM_DIAG_API bool NV_PowerOff_SPU (const char* szPort);
QCOM_DIAG_API bool NV_WriteItem22CodeSN_Wolf_SPU (const char* szPort, const char* szBuffer, char* szError);
QCOM_DIAG_API bool NV_ReadItem22CodeSN_Wolf_SPU (const char* szPort, char* szBuffer);
QCOM_DIAG_API bool NV_ReadQCNName_SPU (const char* szPort, char* value);
QCOM_DIAG_API bool NV_ReadNV543_SPU (const char* szPort, char* value);
QCOM_DIAG_API bool NV_WriteNV543_SPU (const char* szPort, char* value);
QCOM_DIAG_API bool Write_Public_Key_C7_SPU(const char* szPort, const char* szFileSrc);
QCOM_DIAG_API bool Read_Public_Key_C7_SPU(const char* szPort, char* szPubKey);
QCOM_DIAG_API bool NV_Check_SPU(const char* szPort, char *szNV30015, char *szNV30016, char *szNV30017, char *szErrorCode);

QCOM_DIAG_API bool NV_WriteItem22CodeSN_C7_SPU (const char* szPort, const char* szBuffer, char* szError);
QCOM_DIAG_API bool NV_ReadItem22CodeSN_C7_SPU (const char* szPort, char* szBuffer);

// PW
QCOM_DIAG_API bool PW_ReadBatteryVoltage_SPU (char* szPort, char* szVolt);
QCOM_DIAG_API bool PW_USBChargeStop_SPU (char* szPort);
QCOM_DIAG_API bool PW_USBChargeStart_SPU (char* szPort);
QCOM_DIAG_API bool PW_USBChargingStatus_SPU (char *szPort, char* szStatus);
QCOM_DIAG_API bool PW_GetGaugeDeviceName_SPU (char *szPort, char *szDevName);
QCOM_DIAG_API bool PW_GetChargeICDeviceID_SPU (const char* szPort, char* szBuffer);
QCOM_DIAG_API bool PW_GetCoinCellVoltage_SPU (const char* szPort, char* szVoltage);
QCOM_DIAG_API bool PW_PowerKeyEvent_SPU (char* szPort);

// SD_Diag.c 
QCOM_DIAG_API bool SD_GetSize_SPU (const char* szPort, char* szSize);
QCOM_DIAG_API bool SD_GetStatus_SPU (const char* szPort, char* szStatus);
QCOM_DIAG_API bool SD_GetSizenStatus_Sys_SPU (const char* szPort, char* dwSize, char *dwStatus);

// Sensor
QCOM_DIAG_API bool Sensor_CheckGSensor_SPU (const char* szPort);
QCOM_DIAG_API bool Sensor_CalibrateGSensor_SPU (const char* szPort);
QCOM_DIAG_API bool Sensor_CheckECompass_SPU (const char* szPort);
QCOM_DIAG_API bool Sensor_GetComponentValue_SPU (const char* szPort, const char* szID, const char* szSubID, char* szValue);
QCOM_DIAG_API bool Sensor_LPSensor_SPU (const char* szPort,char* szFunction,char* szPara1,char* szPara2,char* szPara3,char* szValue);

// WIFI
QCOM_DIAG_API bool WiFi_ModuleSwitchOn_SPU (const char* szPort);
QCOM_DIAG_API bool WiFi_ModuleSwitchOff_SPU (const char* szPort);
QCOM_DIAG_API bool WiFi_ModuleCheck_SPU (const char* szPort, char* szOn);
QCOM_DIAG_API bool WiFi_ChangeIfToNative_SPU (const char* szPort);
QCOM_DIAG_API bool WiFi_TurnOnTXPower_SPU (const char* szPort, char* szChannel);
QCOM_DIAG_API bool WiFi_TurnOnTXPower2_SPU (const char* szPort, char* szRate, char* szChannel, char* szPower);
QCOM_DIAG_API bool WiFi_TurnOnTXPower2ChN_SPU (const char* szPort, char* szRate, char* szChannel, char* szPower);
QCOM_DIAG_API bool WiFi_TurnOnRXPower2_SPU (const char* szPort, char* szRate, char* szChannel, char* szData1, char* szData2, char* szData3);
QCOM_DIAG_API bool WiFi_TurnOnRXPower2ChN_SPU (const char* szPort, char* szRate, char* szChannel, char* szData1, char* szData2, char* szData3);
QCOM_DIAG_API bool Wifi_StartTx_SPU (const char* szPort, char* szRate, char* szChannel, char* szPower);
QCOM_DIAG_API bool Wifi_GetAddrFromNV_SPU (const char* szPort, char* szAddr);
QCOM_DIAG_API bool Wifi_WriteAddrFromNV_SPU (const char* szPort, char* szAddr);
QCOM_DIAG_API bool Wifi_ReadMacAddress_SPU (const char* szPort, char* szAddr);

// WP7
QCOM_DIAG_API bool WP7_WritePVK_SPU (const char*szPort);
QCOM_DIAG_API bool WP7_WritePVK_Wolf_SPU (const char* szPort, const char* szFileSrc, char* szError);
QCOM_DIAG_API bool WP7_PVKisWritten_Wolf_SPU (const char* szPort, char *szStatus);
QCOM_DIAG_API bool WP7_DelPVK_SPU (const char*szPort);
QCOM_DIAG_API bool WP7_ClearPVK_Wolf_SPU (const char*szPort);
QCOM_DIAG_API bool WP7_CheckPVK_SPU (const char*szPort, char* checkstatus);
QCOM_DIAG_API bool WP7_CheckPVK_Wolf_SPU (const char* szPort, char* szError);
QCOM_DIAG_API bool WP7_BackUpPVK_Wolf_SPU (const char* szPort, char* szError);
QCOM_DIAG_API bool WP7_RestoreUpPVK_Wolf_SPU (const char* szPort, char* szError);
QCOM_DIAG_API bool WP7_ReadHWversion_SPU (const char* szPort, char* szHWVer, char* szVarientVer, char* szMemoryVer, char* szEvtVer, char* szEvtMinorVer, char* szPilotRunVer);
QCOM_DIAG_API bool WP7_ReadSWversion_SPU (const char* szPort, char* szVersion);
QCOM_DIAG_API bool WP7_ReadHWversion_Wolf_SPU (const char* szPort, char* szHWVer , char* szVariant, char* szPCBStage, char* szMajorSN, char* szMinorSN, char* szMem, char* szeMMC, char* szTouchPanel, char* szModule);
QCOM_DIAG_API bool WP7_MasterReset_SPU (const char* szPort);
QCOM_DIAG_API bool WP7_PowerOffMode_SPU (const char* szPort, char* sznPowerOffMode);
QCOM_DIAG_API bool WP7_CIT_SpecifyFileName_SPU (const char* szPort, const char* szFileDest);
QCOM_DIAG_API bool WP7_CIT_Write_SPU (const char* szPort, const char* szFileSrc, char* szError);
QCOM_DIAG_API bool WP7_CIT_Read_SPU (const char* szPort, const char* szFileDest, char* szError);
QCOM_DIAG_API bool WP7_Setup_Sleep_SPU (const char* szPort, char* szTime);

// Phone Info
QCOM_DIAG_API bool PhoneInfo_GetIMEI_SPU (const char* szPort, char* szAddr);
QCOM_DIAG_API bool PhoneInfo_WriteIMEI_SPU (const char* szPort, char* szAddr);
QCOM_DIAG_API bool PhoneInfo_FFU_List_SPU(char* szSN, char* szKITLName, char* szManufacturer);
QCOM_DIAG_API bool PhoneInfo_GetPINStatus_SPU(const char* szPort);
QCOM_DIAG_API bool PhoneInfo_C7FreezeOTP_SPU(const char* szPort, char *szStatus);

// NFC
QCOM_DIAG_API bool NFC_IsConnected_SPU (const char* szPort);

// NLED
QCOM_DIAG_API bool NLED_IsConnected_SPU (const char* szPort);

// Audio
QCOM_DIAG_API bool Audio_SpeakerAmpTest_SPU (const char* szPort);
QCOM_DIAG_API bool Audio_IsHandsetPlugged_SPU(const char* szPort) ;
QCOM_DIAG_API bool Audio_VibratorOnOff_SPU (const char* szPort, bool bOn);

// Touch
QCOM_DIAG_API bool Touch_UpdateTouchFW_SPU (const char* szPort);
QCOM_DIAG_API bool Touch_EnterTestPage_SPU(const char* szPort);
QCOM_DIAG_API bool Touch_CalibrateTouch_SPU (const char* szPort);
QCOM_DIAG_API bool Touch_GetTouchStream_SPU (const char* szPort, char *szTouchStream);
QCOM_DIAG_API bool Touch_GetTouchDelta_SPU (const char* szPort, char *szTouchDelta);
QCOM_DIAG_API bool Touch_GetTouchIDAC_SPU (const char* szPort, char *szTouchIDAC);
QCOM_DIAG_API bool Touch_GetTouchBaseLine_SPU (const char* szPort, char *szTouchBaseLine);
QCOM_DIAG_API bool Touch_EnableSensor_SPU(const char* szPort);
QCOM_DIAG_API bool Touch_DisableSensor_SPU(const char* szPort);
QCOM_DIAG_API bool Touch_CheckVersion_SPU(const char* szPort, char* szTouchVersion);
QCOM_DIAG_API bool Touch_ClearStreamBuffer_SPU(const char* szPort);

// LCD
QCOM_DIAG_API bool Wolf_LCDOnOffSwitch_SPU(const char* szPort, char* szOnOff);

// AT Control
QCOM_DIAG_API bool ATControl_SPU(const char* szPort, char* szCommand, char* szResponse);

// SIMLock
QCOM_DIAG_API bool NV_WriteFSN_SPU (const char* szPort, const char* szFSN_base64, const char* sznTimeOut);
QCOM_DIAG_API bool NV_WriteFSN_HexString_SPU (const char* szPort, const char* szFSN, const char* sznTimeOut);
QCOM_DIAG_API bool NV_ReadFSN_SPU (const char* szPort, char* szFSN_base64, char* sznTimeOut) ;
QCOM_DIAG_API bool NV_ReadFSN_HexString_SPU (const char* szPort, char* szFSN, char* sznTimeOut);
QCOM_DIAG_API bool LockSPC_ForSPU(char* szPort);
QCOM_DIAG_API bool IsSPCLocked_ForSPU(char* szPort, char* szStatus) ;
QCOM_DIAG_API bool UnlockSPC_ForSPU(char* szPort, char* szLock, char* szUnlockOK) ;
QCOM_DIAG_API bool ProgramSPC_ForSPU(char* szPort, char* szLock) ;
QCOM_DIAG_API bool WriteSimDataV2_SPU(char* szPort, char* szSimDataBuf) ;
QCOM_DIAG_API bool GetSimLockName_SPU(char* szPort, char* szSimLockName) ;
QCOM_DIAG_API bool NV_ReadPublicKey_SPU (const char* szPort, char* szPublicKey_base64);
QCOM_DIAG_API bool NV_ReadPublicKey_HexString_SPU (const char* szPort, char* szPublicKey);
QCOM_DIAG_API bool WriteSimData_ForSPU(char* szPort, char* szSimDataBuf_Base64, char* szSimDataBufLen);


QCOM_DIAG_API bool WriteSimDataNew_SPU(char* szPort, char* szSimDataBuf) ;

//PWP
QCOM_DIAG_API bool IniPWP_SPU (const char* szPort, char* szOperation) ;
QCOM_DIAG_API bool Write_Dev_Num_SPU (const char* szPort, char* szDevNum) ;//, const char* szFileSrc);
QCOM_DIAG_API bool Read_Dev_Num_SPU (const char* szPort, char* szDevNum) ;
QCOM_DIAG_API bool Write_Public_Key_SPU (const char* szPort, const char* szFileSrc) ;
QCOM_DIAG_API bool Read_Public_Key_SPU (const char* szPort, char* szPubKey) ;

// Temp
QCOM_DIAG_API bool Temp_IsConnected_Zune_SPU (const char* szCycleTimes);
QCOM_DIAG_API bool Temp_FFU_List_SPU (const char* szBatFile, const char* szFFUFile, char* szSN, char* szKITLName, char* szManufacturer);
QCOM_DIAG_API bool Temp_UpdateDFIFW_SPU(const char* szPort, char* szErrCode);
QCOM_DIAG_API bool GetPVKFileName_SPU(char* szListFileName, char* szPVKFileName, char* szError);

// QFUSE
QCOM_DIAG_API bool Wolf_WriteQFuse_SPU (const char* szPort);
QCOM_DIAG_API bool Wolf_VerifyQFuse_SPU (const char* szPort);


// add for orange [7/26/2011 Kevin.Dong]
QCOM_DIAG_API bool ReadHWID_SPU(char* szPort,char* szHWID);
QCOM_DIAG_API bool ReadFA_SPU(char* szPort,char* szOffset,char* szFA);
QCOM_DIAG_API bool WriteFA_SPU(char* szPort,char* szOffset,char* szData);
QCOM_DIAG_API bool CheckSWVersion_SPU(char* szPort,char* szData);
QCOM_DIAG_API bool CoinCharge_SPU(char* szPort,char* bFlag);
QCOM_DIAG_API bool BatteryCharge_SPU(char* szPort,char* bFlag);
QCOM_DIAG_API bool FileSystemBackup_SPU(char* szPort,char* szUserData);

// For Chicago
QCOM_DIAG_API bool C7_NVBackup_SPU(char* szPort) ;
QCOM_DIAG_API bool C7_NVRestore_SPU(char* szPort);
QCOM_DIAG_API bool C7_DRMBackup_SPU(char* szPort) ;
QCOM_DIAG_API bool ReadFA_Sector(char* szPort,char* szOffset,char* szFA,char* szSectorSize);
QCOM_DIAG_API bool WriteFA_Sector(char* szPort,char* szOffset,char* szFA,char* szSectorSize);


QCOM_DIAG_API bool ReadCoinCellVoltage_SPU(char* szPort,char* szVoltage);
