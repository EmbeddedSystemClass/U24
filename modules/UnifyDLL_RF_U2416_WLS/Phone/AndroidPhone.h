#ifndef _C_ANDROID_PHONE_H_
#define _C_ANDROID_PHONE_H_


#include "../CommonUtility/BaseObject/IPhone.h"
#include "../../../lib/Qualcomm/QDART4823/inc/QLib.h"
#include "../../../lib/Qualcomm/QDART4823/inc/QLib_Defines.h"
#include "Qisda_API.h"


#define FTD_PORT			2000
#define FTD_TIMEOUT         15000
#define FTD_OK				1

#define CPHONE_FTD_CMD_IN_OUT_CREATE(cphone_func_name, dll_api_name)                  \
bool CAndroidPhone::cphone_func_name(int nPort, int nTimeout, char* szInput, char* szOutput) \
{                                                                                     \
	return FTDCmdCtrl(nPort, nTimeout, dll_api_name, szInput, szOutput);              \
}


class CAndroidPhone : public IPhone
{
	RF_DECLARE_DYNCREATE(CAndroidPhone)


	// Data members
public:

protected:
private:
	HMODULE m_hQcomDiagModule;
	HMODULE m_hNeptuneCtrl;

	std::string m_strCOMport;
	std::string m_strKeyword;
	HANDLE m_hQMSLPhone;


	// Member functions
public:
	CAndroidPhone();
	virtual ~CAndroidPhone();

	virtual bool DetectDevice();
	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	// FA functions
	virtual bool GetFAData(char* szValue, int iSize);
	virtual bool SetFAData(char* szValue);
	/*Get/Set FA Sector Data*/
	virtual bool GetFASector(int i_sectorNum, char *sz_sectorData, int i_sectorSize);
	virtual bool SetFASector(int i_sectorNum, char *sz_sectorData, int i_sectorSize);

	virtual bool GetFAData_QMSL(char* szValue, int iSize);
	virtual bool SetFAData_QMSL(char* szValue);

	// Phone functions
	virtual bool Initial();
	virtual bool Disconnect();
	virtual bool IsConnected();
	virtual bool Reboot();
	virtual bool ReadNVItem(WORD NVItem, BYTE* pBuffer, const int BufferSize);
	virtual bool WriteNVItem(WORD NVItem, BYTE* pBuffer, const int BufferSize);
	virtual bool BackupNVpartition() { return true; } ;
	virtual bool ReadHWVersion(char* szVarientVer, char* szMemoryVer, char* szEvtVer, char* szEvtMinorVer, char* szPilotRunVer) { return true; } ;
	virtual bool ReadSWVersion(char* szVersion) { return true; };
	virtual bool GetComponentValue(const int ID, const int SubID, char* szValue) { return true; } ;
	virtual bool GsensorTest() { return true; } ;
	virtual bool ECompassTest() { return true; } ;
	virtual bool GetSDStatus(int* pStatus) { return true; } ;
	virtual bool GetSDSize(int* pSize) { return true; } ;
	virtual bool Adb_Shell_Start();
	virtual bool WifiBinNumberWrite (int offset, int length, int nBin);
	virtual bool ExecAdbCommand(CString str_command, char* pszOutput, char* p_sz_ErrorMsg);

	// AT command
	virtual bool Send_ATCommand(char* szPort, char* szCommand, char* szResponse) ;

	// BT
	virtual bool BTModuleOnOffSwitch(bool bOn) { return true; } ;
	virtual bool BTPowerOff() { return true; } ;
	virtual bool BTStartTxPower(int iDegree) { return true; } ;

	// WiFi
	virtual bool WiFiModuleOnOffSwitch(bool bEnable);
	virtual bool WiFiStartTxPower(int iRate, int iChannel, int iPower);

	virtual bool WLANLoadFABin(char *szOutput);
	virtual bool WLANLoadNextBin(char *szOutput);
	virtual bool WLANSaveBinToFA();

	// GPS
	virtual bool GPS_FunctionOn() { return true; } ;
	virtual bool GPS_FunctionOff() { return true; } ;
	virtual bool GPS_GetCN0value(int& iCN0);
	virtual bool GPS_GetTTFFCN0(int& iTTFF, int& iCN0) { return true; } ;

	// Power
	virtual bool GetGaugeName(char* szName) { return true; } ;
	virtual bool ChargeICTest() { return true; } ;
	virtual bool USBIsCharging() { return true; } ;
	virtual bool USBChargeSwitch(bool bEnable) { return true; } ;
	virtual bool ReadBatteryVoltage(int* piVoltage) { return true; } ;
	virtual bool GetCoinCellVoltage(int* piVoltage) { return true; } ;
	bool Detroit_PWRchargerOn(int nPort, int nTimeout, char* szInput, char* szOutput);
	bool Detroit_PWRWriteRsenseData(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output);// cmd: 13
	bool Detroit_PWRAverageGaugeCurrent(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output);// cmd: 14
	bool FTD_PWRgaugeVoltage(int nPort, int nTimeout, char* pszInput, char* pszOutput); //cmd: 2

	//bool FTD_Set_Model(int nPort, int nTimeout, char* pszInput, char* pszOutput); //cmd: 2


	bool FTDCmdCtrl(int nPort, int nTimeout, char* szFuncName, char* pszInput, char* pszOutput);
	//93 - 6
	virtual bool FTD_Set_Model(int nPort, int nTimeout, char* p_sz_Input, char* p_sz_Output) ; //cmd: 2

	// NFC
	virtual bool NFC_IsConnected() { return true; } ;
	virtual bool NFCHCITypeAReader(char *szInput, char *szOutput);

	// NLED
	virtual bool NLED_IsConnected() { return true; } ;

	// Speaker
	virtual bool Audio_SpeakerAmpTest() { return true; } ;

	// EFs
	virtual bool EFS_Sync_Combo(int iRetryTime) { return true; } ;
	virtual bool EFs_GoldenCopy() { return true; } ;

	// FM
	virtual bool FM_TuneFM(int iChannel) { return true; } ;
	virtual bool FM_ConnectFM(bool bConnect=true,int iTimeout=5000) { return true; } ;
	virtual bool FM_GetRSSI(int &iRSSI) { return true; } ;

	// Detroit NV Backup
	virtual bool NVBackup();

	// Detroit FTD basic commands
	virtual bool Initial_FTD();
	virtual bool Disconnect_FTD();
	virtual bool IsConnected_FTD();

	// Detroit BT
	virtual bool BTModuleOnOffSwitch(bool bEnable, int idelay);
	virtual bool BTEnterTestMode();
	virtual bool BTStartTxPower(char *szInput);
	virtual bool Nordic2G4_PowerOn(char *szInput);
	virtual bool Nordic2G4_PowerMode(char *szInput);
	virtual bool Nordic2G4_Consumer(char *szInput);
	virtual bool BTStartTxLtePower(int iDegree) { return true;};
	virtual bool BTStartTxLtePower(char *szBTInput);
	
	virtual bool BTReset();

	// Detroit QMI
	virtual bool Detroit_QmiRfTxTest();
	virtual bool Detroit_QmiRfTxOff();

	// Detroit WiFi
	virtual bool WifiModuleOn (bool Enable, int nBin=-1);
	virtual bool WifiPowerOnTx (int iRate, int iChannel, int iPower);
	virtual bool WifiPowerOffTx ( int m_iChanne );
	virtual bool WifiPowerOnTx(int m_iRateBitIndex, int m_iChannel,int m_iPower,int m_iWlanMode,int m_iTxChain, int m_iLoadBin);
	virtual bool WifiPowerOnCW(int m_iRateBitIndex, int m_iChannel,int m_iPower,int m_iWlanMode,int m_iTxChain, int m_iLoadBin);
	virtual bool Wifi5GPowerOnTx (int iRate, int iChannel, int iPower, int iRegulatorFixed, int iPower_control_mode);
	virtual bool WifiPowerOnCw (int iChannel, int iGain, long iTone, unsigned long iAmpl);

	// Detroit GPS
	virtual bool GPS_EnterGPS(bool bEnter);
	virtual bool GPS_GetCN0value(double& dCN0, int& nFreq, unsigned char iTestMode, unsigned short iCaptureSize_kSamples, unsigned char iNumberOfIntegrations);

	// Detroit FMR
	virtual bool FMR_Rssi(char *szInput, char *szOutput);

	// Detroit WCDMA
	virtual bool WCDMA_Rssi(unsigned short iChannel, unsigned short* iRX_AGC, unsigned short* iRX_Power_dBm, unsigned short* iFTM_Error_Code);

	//Sapporo FSG_Enable
	virtual bool FSG_Enable(char *szInput, char *szOutput);

	//add for Sapporo FSG lock 20130909
	virtual bool FTD_FAC_FSGProtect(int nPort, int nTimeout, char* pszInput, char* pszOutput);

	// QMSL phone functions
	virtual bool Initial_QMSL();
	virtual bool IsConnected_QMSL();
	virtual bool Disconnect_QMSL();
	virtual bool QMSL_IsFTM_Mode(unsigned char* pMode);
	virtual bool QMSL_ChangeFTM_Mode(int ucFTMMode);
	virtual bool QMSL_ChangeFTM_BootMode(int iMode);
	virtual bool QMSL_CheckPhoneConnect(int iRetryConnect);
	virtual bool QMSL_SendSync(short iRequestSize, BYTE* piRequestBytes, BYTE* piResponseBytes, short* piResponseSize);


	virtual bool QMSL_PHONE_GET_MODE(unsigned long *piPhoneMode);
	virtual bool ReadNV_QMSL(unsigned short unIndex, unsigned char* pData, int iSize);
	virtual bool QMSL_FTM_SET_MODE_ID(short eModeId);
	virtual bool QMSL_FTM_SET_PATH_DELAY(short *piDelay);
	virtual bool QMSL_FTM_SET_MODE(short nMode);
	virtual bool QMSL_FTM_GSM_POLAR_TX_SWEEP_CAL(unsigned long iNumDAC, unsigned long* piDACList, unsigned long* piDACDuration);
	virtual bool QMSL_FTM_SET_CHAN(unsigned short unChannel);
	virtual bool QMSL_DIAG_CONTROL_F(int iMode);
	virtual bool QMSL_FTM_SET_GSM_LINEAR_PA_RANGE(unsigned short unSlot, unsigned short unPARange);
	virtual bool QMSL_FTM_GSM_SET_LINEAR_RGI(unsigned short unSlot, unsigned short unRGI, unsigned char ucModType);
	virtual bool QMSL_FTM_SET_TRANSMIT_BURST(
		unsigned char ucSlot,
		short nDataSource,
		unsigned char ucTSCIndex,
		unsigned long ulNumBursts,
		unsigned char ucIsInfiniteDuration
		);
	virtual bool QMSL_FTM_SET_TX_FRAME_MATRIX(
		unsigned char iSlotNum, 
		unsigned char iTurnSlotOn, 
		unsigned short iPwrLevel, 
		unsigned char iModScheme
		);
	virtual bool QMSL_FTM_SET_TX_ON();
	virtual bool QMSL_FTM_SET_TX_OFF();
	virtual bool QMSL_FTM_SET_PA_RANGE(unsigned short unRange);
	virtual bool QMSL_FTM_SET_PA_STATE(unsigned short unState, unsigned short* iFTM_Error_Code);

	
	virtual bool QMSL_FTM_SET_PDM(unsigned short unPDM);
	virtual bool QMSL_FTM_SET_PDM_V2(unsigned short iPDMtype, unsigned short unPDM);
	virtual bool QMSL_FTM_SET_PA_DAC_INPUT(unsigned short iDACinput);
	virtual bool QMSL_FTM_TX_CS_SWEEP(unsigned char iListSize, short* aiI_DC_Correction, short* aiQ_DC_Correction,
		unsigned short* aiSegmentLength, unsigned short* piStatus);
	virtual bool QMSL_ConfigureTimeOut(unsigned long unTimoutID, unsigned long unTimeout) ;
	virtual bool QMSL_FTM_SET_COMMAND_CODE(short nCmdCode) ;
	virtual bool QMSL_FTM_SET_SECONDARY_CHAIN(unsigned short unMode) ;
	virtual bool QMSL_FTM_SET_LNA_RANGE(unsigned char iLNArange) ;
	virtual bool QMSL_FTM_DO_ENH_XO_DC_CAL(void* pFTM_ENH_XO_Cal_Request, void* pFTM_XO_Cal_Response) ;
	virtual bool QMSL_FTM_DO_ENH_XO_FT_COLLECT_SAMPLE(void* pFTM_ENH_XO_Cal_Request, void* pFTM_XO_Cal_Response) ;
	virtual bool QMSL_FTM_CDMA_CW_WAVEFORM(unsigned char bSelectCW) ;
	virtual bool QMSL_FTM_DO_ENH_XO_FT_CURVE_CAL(void* pFTM_ENH_XO_Cal_Request, void* pFTM_XO_Cal_Response) ;
	virtual bool QMSL_FTM_DA_CAL(unsigned short iListSize, unsigned short *aiSegLength, unsigned char *aiRgi, unsigned char *aiGsmMode, unsigned char *aiPaRange) ;
	virtual bool QMSL_RFCAL_GSM_Tx_DA_Cal_Results(unsigned short iNvModeId, void *aGSM_DA_Cal, void *aGSM_DA_Cal_NV, unsigned char bCachedByNVManager) ;
	virtual bool QMSL_FTM_TX_CFG2_AMAM_SWEEP(unsigned long iPreDcDuration, unsigned long iEdgeDuration, unsigned long iCalRgi, unsigned long *piDigScale) ;
	virtual bool QMSL_RFCAL_GSM_Tx_Linear_Process_PreDist_Cal_Results(unsigned short iNvModeId, void *aGSM_PreDist_Cal, void *aGSM_PreDist_Cal_NV, unsigned char bCachedByNVManager);
	virtual bool QMSL_RFCAL_NV_Manager_WriteToPhone(void *pWriteToPhoneStatus) ;
	virtual bool QMSL_FTM_GSM_DO_TX_Kvco(unsigned short iLowChan,unsigned short iHiChan, unsigned short *piLowChanKvco, 
		unsigned short *piHiChanKvco);
	virtual bool QMSL_FTM_SET_RX_BURST(unsigned char iSlotNum, short iRxFunction);
	virtual bool QMSL_FTM_RX_GAIN_RANGE_CAL(unsigned char iRxRangeBitMask, unsigned char iNumAverages, unsigned char bToggleRx, 
		unsigned long aiRSSI_Result[8]);
	virtual bool QMSL_RFCAL_GSM_Rx_Gain_Range_Results(unsigned short iNvModeId, void* aGSM_Rx_Gain_Range_Result, void* aGSM_Rx_Gain_Range_NV, 
		unsigned char bCachedByNVManager);
	virtual bool QMSL_RFCAL_GSM_CAL_ARFCN(unsigned short iNvModeId, 
		unsigned short* aGSM_Cal_ARFCN_List, 
		unsigned char iNumOfChannel, 
		void * pGSM_Cal_ARFCN_NV, 
		unsigned char bCachedByNVManager);
	virtual bool QMSL_FTM_LOAD_RF_NV(unsigned short* pusFTM_Error_Code);
	virtual bool QMSL_DIAG_NV_WRITE_F(unsigned short iItemID, unsigned char* szValue, int iLength, unsigned short* iStatus);

	// 3G cal
	virtual bool QMSL_FTM_SET_TX_ENH_CAL_ACTION(int iState);
	virtual bool QMSL_FTM_LOAD_TX_LINEARIZER_CAL_DATA(void* pFTM_Tx_Lin_CalData_Req, void* pFTM_Tx_Lin_CalData_Res) ;
	virtual bool QMSL_FTM_LOAD_TX_FREQCOMP_CAL_DATA(void* pFTM_Tx_Freqcomp_CalData_Req, void* pFTM_Tx_Freqcomp_CalData_Res) ;
	virtual bool QMSL_FTM_SET_SMPS_PA_BIAS_OVERRIDE(bool bOnOff) ;
	virtual bool QMSL_FTM_SET_SMPS_PA_BIAS_VAL(int iPA_Bias_Value) ;
	virtual bool QMSL_FTM_SET_CALIBRATION_STATE(int iState);
	virtual bool QMSL_FTM_SET_TX_SWEEP_STEP_SIZE(unsigned char ucStepSize);
	virtual bool QMSL_FTM_CONFIGURE_TX_SWEEP_CAL(unsigned short iTimeInterval, unsigned short iNumofHDETReadingsPerStep);
	virtual bool QMSL_FTM_TX_SWEEP_CAL(bool bOnOff);
	virtual bool QMSL_FTM_GET_HDET_FROM_TX_SWEEP_CAL(unsigned short usIndex,unsigned short* usHDET);
	virtual bool QMSL_FTM_TX_RX_FREQ_CAL_SWEEP(FTM_Tx_Rx_Freq_Cal_Sweep_Request_Response *pFTM_Tx_Rx_Freq_Cal_Sweep_Request_Response);
	virtual bool QMSL_DIAG_NV_WRITE_SetBatchMode();
	virtual bool QMSL_DIAG_NV_WRITE_SetBatchMode_False();
	virtual bool QMSL_DIAG_NV_WRITE_FlushBatchQueue();
	virtual bool QMSL_FTM_GET_ENH_THERM(unsigned long* iThermValue);
	virtual bool QMSL_FTM_GET_THERM(unsigned char* ucThermValue);
	virtual bool QMSL_RFCAL_Internal_Thermistor_Calibration_Results(void*  pInt_Therm_Cal_Meas, void*  pInt_Therm_Cal_NV, unsigned char  bCachedByNVManager) ;
	virtual bool QMSL_FTM_DO_DC_CAL() ;
	virtual bool QMSL_FTM_GET_DVGA_OFFSET(short iExpectedRX_AGC, short* iDVGAoffset) ;
	virtual bool QMSL_FTM_GET_CDMA_IM2(unsigned char* piI_Value, unsigned char* piQ_Value, unsigned char* piTransConductance_Value) ;

	//	wcdma ftm test
	virtual bool QMSL_FTM_WCDMA_BER_RMC_DCH_TFCS_CONFIG_REQ(unsigned char ucTFCS_Select, unsigned char* ucTFCS_Config);
	virtual bool QMSL_FTM_UMTS_BER_ForceStopped();
	virtual bool QMSL_FTM_WCDMA_BER_StartSession(unsigned long ulTimeOut_ms);
	virtual bool QMSL_FTM_WCDMA_BER_AcquirePilot(unsigned short usChannel);
	virtual bool QMSL_FTM_WCDMA_BER_StopSession();
	virtual bool QMSL_FTM_WCDMA_BER_Handover(unsigned char iRMC_Type, unsigned char iUL_MaxTxPwr, unsigned long iUL_DPCH_ScrCode, unsigned char iUL_PCA, unsigned char iUL_TPC_StepSize,  
		unsigned short iDL_PriCPICH_Scr_Code, unsigned short iDL_ChanCode, unsigned char iBetaC, unsigned char iBetaD, unsigned short iChannel);
	virtual bool QMSL_FTM_WCDMA_BER_Handover_V2B(
		unsigned char	iRMC_Type, 
		unsigned char	iUL_MaxTxPwr, 
		unsigned long	iUL_DPCH_ScrCode, 
		unsigned char	iUL_PCA,
		unsigned char	iUL_TPC_StepSize, 
		unsigned short iDL_PriCPICH_Scr_Code, 
		unsigned short iDL_ChanCode,
		unsigned char	iBetaC, 
		unsigned char	iBetaD,
		short			iDPCCH_PowerOffset,
		unsigned char	n_312,
		unsigned char	n_313,
		unsigned char	n_315,
		unsigned char	t_312,
		unsigned char	t_313,
		unsigned short iChannel);

	// GSM Function
	virtual bool QMSL_FTM_GSM_BER_StartSession(unsigned long iTimeOut) ;
	virtual bool QMSL_FTM_GSM_BER_AssignBCCH (unsigned short iChannel, int iBand) ;
	virtual bool QMSL_FTM_GSM_BER_Handover_NoLoopbackConfig (unsigned short iChannel,
		int iBand,
		unsigned char iSlot,
		unsigned char iPowerLevel) ;
	virtual bool QMSL_FTM_GSM_BER_IsTrafficValid() ;
	virtual bool QMSL_FTM_GSM_BER_ClearStatus() ;
	virtual bool QMSL_FTM_GSM_BER_StopSession() ;
	virtual bool QMSL_FTM_GSM_BER_ReleaseTCH() ;

	// EDGE Function
	virtual bool QMSL_FTM_EGPRS_BER_AssignBCCH (unsigned short iChannel, int iBand) ;
	virtual bool QMSL_FTM_EGPRS_BER_Establish_SRB_LOOPBACK() ;
	virtual bool QMSL_FTM_EGPRS_BER_Configure_DL_TBF (unsigned short usChannel,
		int iBand) ;
	virtual bool QMSL_FTM_EGPRS_BER_Configure_UL_TBF (unsigned short iChannel,
		int iBand,
		unsigned char iGamma) ;
	virtual bool QMSL_FTM_EGPRS_BER_Release_All_TBF() ;
	virtual bool QMSL_FTM_GSM_ConfigLoopbackType(unsigned short usLookbackType) ;
	virtual bool QMSL_FTM_GSM_BER_GetRxMetrics(unsigned char* szRxLevel, unsigned char* szRxQual) ;
	virtual bool QMSL_FTM_SET_TX_POWER_LEVEL(unsigned char nPowerLevel) ;

	// LTE
	virtual bool QMSL_FTM_LTE_SET_TX_BANDWIDTH(unsigned char irxChanBW);
	virtual bool QMSL_FTM_LTE_SET_RX_BANDWIDTH(unsigned char irxChanBW);
	virtual bool QMSL_FTM_LTE_SET_TX_WAVEFORM(unsigned char iTxWaveform, unsigned char inumRBsPUSCH, unsigned char inumRBsPUCCH,unsigned char iPUSCHStartRBIndex);
	virtual bool QMSL_FTM_LTE_STOP_TX_WAVEFORM();
	virtual bool QMSL_FTM_SET_TX_GAIN_INDEX(unsigned short iTxGainIndex);
	virtual bool QMSL_FTM_LTE_SET_NET_SIG_VALUE(unsigned char iNS, unsigned short* iFTM_Error_Code);
	virtual bool QMSL_FTM_SET_TX_POWER_DBM(unsigned char iEnable, short iPowerLevel, unsigned short* iFTM_Error_Code);
	virtual bool QMSL_FTM_LTE_SET_TX_MODULATION_TYPE(unsigned long iModulationType);
	virtual bool LTE_B3_RxRssi (unsigned short iChannel, unsigned short* iRX_AGC, unsigned short* iRX_Power_dBm, unsigned short* iFTM_Error_Code);
	virtual bool LTE_B7_RxRssi (unsigned short iChannel, unsigned short* iRX_AGC, unsigned short* iRX_Power_dBm, unsigned short* iFTM_Error_Code);
	virtual bool LTE_B20_RxRssi (unsigned short iChannel, unsigned short* iRX_AGC, unsigned short* iRX_Power_dBm, unsigned short* iFTM_Error_Code);

	virtual bool LTE_Band_RxRssi ( unsigned short* iRX_AGC, unsigned short* iRX_Power_dBm, unsigned short* iFTM_Error_Code);

	virtual bool QMSL_FTM_GSM_GET_MEAS_PWR (unsigned short* iMeasured_Dbm16_Value, unsigned long*  iRaw_RSSI, unsigned short* iFTM_Error_Code);
	virtual bool QMSL_FTM_GSM_SETUP_RX_BURST_FOR_EXPECTED_PWR (unsigned short iExpected_RX_Power_dbm16, unsigned short  iNumber_of_Averages, unsigned short* iFTM_Error_Code);
	virtual bool QMSL_FTM_RFFE_READWRITE_CMD(unsigned char* szTunerIC);
	virtual bool QMSL_FTM_DO_TUNER_SELF_CAL();
	virtual bool QMSL_EFS2_SyncWithWait(unsigned char* ucStatus, int iTimeout = 5000);

	virtual bool QMSL_FTM_LTE_SET_LNA_STATE_DBM ( short iExpected_RX_Power_dBm );

	virtual bool QMSL_FTM_GET_RX_AGC(unsigned short* iRX_AGC);




protected:
private:
	bool MyExecAdbCmd(const char* chCmd, char **chReturn = NULL, bool bWithShell = true);
};


#endif // End of #ifndef _C_ANDROID_PHONE_H_
