#ifndef _I_PHONE_H_
#define _I_PHONE_H_


#include "../CommonDefine.h"
#include "CToolBaseObject.h"
#include "QLib_Defines.h"


interface IPhone : public CToolBaseObject
{
RF_DECLARE_DYNAMIC(IPhone)


// Data members
public:
	std::string			m_strDeviceName;
	std::string			m_strQisdaCommandModuleName;

protected:
private:


// Member functions
public:
	IPhone() {}
	virtual ~IPhone() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap) = 0;

	// FA functions
	virtual bool GetFAData(char* szValue, int iSize) = 0;
	virtual bool SetFAData(char* szValue) = 0;
	/*Get/Set FA Sector Data*/
	virtual bool GetFASector(int i_sectorNum, char *sz_sectorData, int i_sectorSize) = 0;
	virtual bool SetFASector(int i_sectorNum, char *sz_sectorData, int i_sectorSize) = 0;

	virtual bool GetFAData_QMSL(char* szValue, int iSize) = 0;
	virtual bool SetFAData_QMSL(char* szValue) = 0;

	// Phone functions
	virtual bool Initial() = 0;
	virtual bool Disconnect() = 0;
	virtual bool IsConnected() = 0;
	virtual bool Reboot() = 0;
	virtual bool ReadNVItem(WORD NVItem, BYTE* pBuffer, const int BufferSize) = 0;
	virtual bool WriteNVItem(WORD NVItem, BYTE* pBuffer, const int BufferSize) = 0;
	virtual bool BackupNVpartition() = 0;
	virtual bool ReadHWVersion(char* szVarientVer, char* szMemoryVer, char* szEvtVer, char* szEvtMinorVer, char* szPilotRunVer) = 0;
	virtual bool ReadSWVersion(char* szVersion) = 0;
	virtual bool GetComponentValue(const int ID, const int SubID, char* szValue) = 0;
	virtual bool GsensorTest() = 0;
	virtual bool ECompassTest() = 0;
	virtual bool GetSDStatus(int* pStatus) = 0;
	virtual bool GetSDSize(int* pSize) = 0;
	virtual bool Adb_Shell_Start() = 0;
	virtual bool WifiBinNumberWrite (int offset, int length, int nBin) = 0;
	virtual bool ExecAdbCommand(CString str_command, char* pszOutput, char* p_sz_ErrorMsg) =0;
	virtual bool ResetBinFile() = 0;

	// AT Control
	virtual bool Send_ATCommand(char* szPort, char* szCommand, char* szResponse) = 0;

	// BT
	virtual bool BTModuleOnOffSwitch(bool bOn) = 0;
	virtual bool BTPowerOff() = 0;
	virtual bool BTStartTxPower(int iDegree) = 0;
	virtual bool BTStartTxLtePower(int iDegree) = 0;
	virtual bool Nordic2G4_Consumer(char *szInput) = 0;
	virtual bool Nordic2G4_PowerMode(char *szInput) = 0;
	virtual bool Nordic2G4_PowerOn(char *szInput) = 0;
	

	// WiFi
	virtual bool WiFiModuleOnOffSwitch(bool bEnable) = 0;
	virtual bool WiFiStartTxPower(int iRate, int iChannel, int iPower) = 0;
 	virtual bool WLANLoadFABin(char *szOutput) = 0;
 	virtual bool WLANLoadNextBin(char *szOutput) = 0;
 	virtual bool WLANSaveBinToFA() = 0;

	// GPS
	virtual bool GPS_FunctionOn() = 0;
	virtual bool GPS_FunctionOff() = 0;
	virtual bool GPS_GetCN0value(int& iCN0) = 0;
	virtual bool GPS_GetTTFFCN0(int& iTTFF, int& iCN0) = 0;

	// Power
	virtual bool GetGaugeName(char* szName) = 0;
	virtual bool ChargeICTest() = 0;
	virtual bool USBIsCharging() = 0;
	virtual bool USBChargeSwitch(bool bEnable) = 0;
	virtual bool ReadBatteryVoltage(int* piVoltage) = 0;
	virtual bool GetCoinCellVoltage(int* piVoltage) = 0;

	// NFC
	virtual bool NFC_IsConnected() = 0;

	// NLED
	virtual bool NLED_IsConnected() = 0;

	// Speaker
	virtual bool Audio_SpeakerAmpTest() = 0;

	// EFs
	virtual bool EFS_Sync_Combo(int iRetryTime) = 0;
	virtual bool EFs_GoldenCopy() = 0;

	// Detroit NV Backup
	virtual bool NVBackup() = 0;

	// FM
	virtual bool FM_TuneFM(int iChannel) = 0;
	virtual bool FM_ConnectFM(bool bConnect=true, int iTimeout=5000) = 0;
	virtual bool FM_GetRSSI(int &iRSSI) = 0;

	// Detroit FTD basic commands
	virtual bool Initial_FTD() = 0;
	virtual bool Disconnect_FTD() = 0;
	virtual bool IsConnected_FTD() = 0;

	// Detroit BT
	virtual bool BTModuleOnOffSwitch(bool bEnable, int idelay) = 0;
	virtual bool BTEnterTestMode() = 0;
	virtual bool BTStartTxPower(int ichannel, int ipower, int itype) = 0;
	virtual bool BTStartTxLtePower(int ichannel, int iDataLength, int itype) = 0;

	// Detroit QmiRf
	virtual bool Detroit_QmiRfTxTest() = 0;
	virtual bool Detroit_QmiRfTxOff() = 0;

	// Detroit WiFi
	virtual bool WifiModuleOn (bool Enable, int nBin=-1) = 0;
	virtual bool WifiModuleOnCertification(bool Enable, int nBin=-1) = 0;
	virtual bool WifiPowerOnTx (int iRate, int iChannel, int iPower) = 0;
	virtual bool WifiPowerOnTxCertification (int iRate, int iChannel, int iPower, int iPreamble, int iPayloadSize, int iSpacing,int iChain) = 0;
	virtual bool WifiPowerOn5GTxCertification (int iRate, int iChannel, int iPower, int iPreamble, int iPayloadSize, int iSpacing, int iBond,int iChain) = 0;
	virtual bool WifiPowerStopTx() = 0;
	virtual bool WifiPowerStopTxCw() = 0;
	virtual bool WifiPowerOnTxCwCertification (int iChannel, int iRfGain) = 0;
	virtual bool WifiPowerOn5GTxCwCertification (int iChannel, int iRfGain, int iBond) = 0;
	//virtual bool WifiPowerOnRxGetPacket (unsigned long* rxFrameCounter,int iChain) = 0;
	virtual bool WifiPowerOnRxGetPacket (int iChannel,int iPreamble, unsigned long* rxFrameCounter,int iChain) = 0;
	virtual bool WifiPowerOnRxSetChannel(int iChannel) = 0;
	virtual bool WifiPowerOn5GRxGetPacket(unsigned long* rxFrameCounter) = 0;
	virtual bool WifiPowerOn5GRxSetChannel(int iChannel, int iBond) = 0;
	virtual bool WifiPowerOnRxCertificationReset () = 0;
	virtual bool WifiPowerOnTx (int iRate, int iChannel, int iPower, int iRegulatorFixed) = 0;
	virtual bool WifiPowerOnCw (int iChannel, int iGain, long iTone, unsigned long iAmpl) = 0;

	// Detroit GPS
	virtual bool GPS_EnterGPS(bool bEnter) = 0;
	virtual bool GPS_GetCN0value(double& dCN0, int& nFreq, unsigned short iTestMode, unsigned short iCaptureSize_kSamples, unsigned char iNumberOfIntegrations) = 0;

	// Detroit FMR
	virtual bool FMR_Rssi(char *szInput, char *szOutput) = 0;
	
	// Detroit WCDMA
	virtual bool WCDMA_Rssi(unsigned short iChannel, unsigned short* iRX_AGC, unsigned short* iRX_Power_dBm, unsigned short* iFTM_Error_Code) = 0;

	//Sapporo FSG_Enable
	virtual bool FSG_Enable(char *szInput, char *szOutput) = 0;

	// QMSL phone functions
	virtual bool Initial_QMSL() = 0;
	virtual bool IsConnected_QMSL() = 0;
	virtual bool Disconnect_QMSL() = 0;
	virtual bool QMSL_IsFTM_Mode(unsigned char* pMode) = 0;
    virtual bool QMSL_ChangeFTM_Mode(int ucFTMMode) = 0 ;
	virtual bool QMSL_ChangeFTM_BootMode(int iMode) = 0 ;
    virtual bool QMSL_CheckPhoneConnect(int iRetryConnect) = 0 ;


	virtual bool ReadNV_QMSL(unsigned short unIndex, unsigned char* pData, int iSize) = 0;
	virtual bool QMSL_FTM_SET_MODE_ID(short nMode) = 0;
	virtual bool QMSL_FTM_SET_PATH_DELAY(short *piDelay) = 0;
	virtual bool QMSL_FTM_SET_MODE(short nMode) = 0;
	virtual bool QMSL_PHONE_GET_MODE(unsigned long *piPhoneMode) = 0;

	virtual bool QMSL_FTM_GSM_POLAR_TX_SWEEP_CAL(unsigned long iNumDAC, unsigned long* piDACList, unsigned long* piDACDuration) = 0;
	virtual bool QMSL_FTM_SET_CHAN(unsigned short unChannel) = 0;
	virtual bool QMSL_FTM_SET_GSM_LINEAR_PA_RANGE(unsigned short unSlot, unsigned short unPARange) = 0;
	virtual bool QMSL_FTM_GSM_SET_LINEAR_RGI(unsigned short unSlot, unsigned short unRGI, unsigned char ucModType) = 0;
	virtual bool QMSL_FTM_SET_TRANSMIT_BURST(
		unsigned char ucSlot,
		short nDataSource,
		unsigned char ucTSCIndex,
		unsigned long ulNumBursts,
		unsigned char ucIsInfiniteDuration
		) = 0;
	virtual bool QMSL_FTM_SET_TX_FRAME_MATRIX(
		unsigned char iSlotNum, 
		unsigned char iTurnSlotOn, 
		unsigned short iPwrLevel, 
		unsigned char iModScheme
		) = 0;
	virtual bool QMSL_FTM_SET_TX_ON() = 0;
	virtual bool QMSL_FTM_SET_TX_OFF() = 0;
	virtual bool QMSL_FTM_SET_PA_RANGE(unsigned short unRange) = 0;
	virtual bool QMSL_FTM_SET_PDM(unsigned short unPDM) = 0;
	virtual	bool QMSL_FTM_SET_PDM_V2(unsigned short iPDMtype, unsigned short unPDM) = 0;
	virtual bool QMSL_FTM_SET_PA_DAC_INPUT(unsigned short iDACinput) = 0;
	virtual bool QMSL_FTM_TX_CS_SWEEP(unsigned char iListSize, short* aiI_DC_Correction, short* aiQ_DC_Correction,
									  unsigned short* aiSegmentLength, unsigned short* piStatus) = 0;
    virtual bool QMSL_ConfigureTimeOut(unsigned long unTimoutID, unsigned long unTimeout) = 0 ;
    virtual bool QMSL_FTM_SET_COMMAND_CODE(short nCmdCode) = 0 ;
    virtual bool QMSL_FTM_SET_SECONDARY_CHAIN(unsigned short unMode) = 0 ;
    virtual bool QMSL_FTM_SET_LNA_RANGE(unsigned char iLNArange) = 0 ;
    virtual bool QMSL_FTM_DO_ENH_XO_DC_CAL(void* pFTM_ENH_XO_Cal_Request, void* pFTM_XO_Cal_Response) = 0 ;
    virtual bool QMSL_FTM_DO_ENH_XO_FT_COLLECT_SAMPLE(void* pFTM_ENH_XO_Cal_Request, void* pFTM_XO_Cal_Response) = 0 ;
    virtual bool QMSL_FTM_CDMA_CW_WAVEFORM(unsigned char bSelectCW) = 0 ;
    virtual bool QMSL_FTM_DO_ENH_XO_FT_CURVE_CAL(void* pFTM_ENH_XO_Cal_Request, void* pFTM_XO_Cal_Response) = 0 ;
	virtual bool QMSL_FTM_DA_CAL(unsigned short iListSize, unsigned short *aiSegLength, unsigned char *aiRgi, unsigned char *aiGsmMode, unsigned char *aiPaRange) = 0 ;
	virtual bool QMSL_RFCAL_GSM_Tx_DA_Cal_Results(unsigned short iNvModeId, void *aGSM_DA_Cal, void *aGSM_DA_Cal_NV, unsigned char bCachedByNVManager) = 0 ;
	virtual bool QMSL_FTM_TX_CFG2_AMAM_SWEEP(unsigned long iPreDcDuration, unsigned long iEdgeDuration, unsigned long iCalRgi, unsigned long *piDigScale) = 0 ;
	virtual bool QMSL_RFCAL_GSM_Tx_Linear_Process_PreDist_Cal_Results(unsigned short iNvModeId, void *aGSM_PreDist_Cal, void *aGSM_PreDist_Cal_NV, unsigned char bCachedByNVManager) = 0 ;
	virtual bool QMSL_RFCAL_NV_Manager_WriteToPhone(void *pWriteToPhoneStatus) = 0 ;
	virtual bool QMSL_FTM_GSM_DO_TX_Kvco(unsigned short iLowChan,unsigned short iHiChan, unsigned short *piLowChanKvco,unsigned short *piHiChanKvco) = 0;
	virtual bool QMSL_FTM_SET_RX_BURST(unsigned char iSlotNum, short iRxFunction) = 0 ;
	virtual bool QMSL_FTM_RX_GAIN_RANGE_CAL(unsigned char iRxRangeBitMask, unsigned char iNumAverages, unsigned char bToggleRx, 
							unsigned long aiRSSI_Result[8]) = 0 ;
	virtual bool QMSL_RFCAL_GSM_Rx_Gain_Range_Results(unsigned short iNvModeId, void* aGSM_Rx_Gain_Range_Result, void* aGSM_Rx_Gain_Range_NV, 
							unsigned char bCachedByNVManager) = 0 ;
	virtual bool QMSL_RFCAL_GSM_CAL_ARFCN(unsigned short iNvModeId, 
								unsigned short* aGSM_Cal_ARFCN_List, 
								unsigned char iNumOfChannel, 
								void * pGSM_Cal_ARFCN_NV, 
								unsigned char bCachedByNVManager) = 0 ; 
	virtual bool QMSL_DIAG_CONTROL_F(int iMode) = 0;
	virtual bool QMSL_FTM_LOAD_RF_NV(unsigned short* pusFTM_Error_Code) = 0 ;
	virtual bool QMSL_DIAG_NV_WRITE_F(unsigned short iItemID, unsigned char* szValue, int iLength, unsigned short* iStatus) = 0 ;
	
    // 3G cal
	virtual bool QMSL_FTM_SET_TX_ENH_CAL_ACTION(int iState) = 0;
    virtual bool QMSL_FTM_LOAD_TX_LINEARIZER_CAL_DATA(void* pFTM_Tx_Lin_CalData_Req, void* pFTM_Tx_Lin_CalData_Res) = 0 ;
    virtual bool QMSL_FTM_LOAD_TX_FREQCOMP_CAL_DATA(void* pFTM_Tx_Freqcomp_CalData_Req, void* pFTM_Tx_Freqcomp_CalData_Res) = 0 ;
	virtual bool QMSL_FTM_SET_SMPS_PA_BIAS_OVERRIDE(bool bOnOff) = 0 ;
	virtual bool QMSL_FTM_SET_SMPS_PA_BIAS_VAL(int iPA_Bias_Value) = 0 ;
	virtual bool QMSL_FTM_SET_CALIBRATION_STATE(int iState) = 0;
	virtual bool QMSL_FTM_SET_TX_SWEEP_STEP_SIZE(unsigned char ucStepSize) = 0;
	virtual bool QMSL_FTM_CONFIGURE_TX_SWEEP_CAL(unsigned short iTimeInterval, unsigned short iNumofHDETReadingsPerStep) = 0;
	virtual bool QMSL_FTM_TX_SWEEP_CAL(bool bOnOff) = 0;
	virtual bool QMSL_FTM_GET_HDET_FROM_TX_SWEEP_CAL(unsigned short usIndex,unsigned short* usHDET) = 0;
	virtual bool QMSL_FTM_TX_RX_FREQ_CAL_SWEEP(FTM_Tx_Rx_Freq_Cal_Sweep_Request_Response *pFTM_Tx_Rx_Freq_Cal_Sweep_Request_Response) = 0;
	virtual bool QMSL_DIAG_NV_WRITE_SetBatchMode() = 0;
	virtual bool QMSL_DIAG_NV_WRITE_SetBatchMode_False() = 0;
	virtual bool QMSL_DIAG_NV_WRITE_FlushBatchQueue() = 0;
	virtual bool QMSL_FTM_GET_ENH_THERM(unsigned long* iThermValue) = 0;
	virtual bool QMSL_FTM_GET_THERM(unsigned char* ucThermValue) = 0;
	virtual bool QMSL_RFCAL_Internal_Thermistor_Calibration_Results(void*  pInt_Therm_Cal_Meas, void*  pInt_Therm_Cal_NV, unsigned char  bCachedByNVManager) = 0 ;
    virtual bool QMSL_FTM_DO_DC_CAL() = 0 ;
	virtual bool QMSL_FTM_GET_DVGA_OFFSET(short iExpectedRX_AGC, short* iPVGAoffset) = 0 ;
	virtual bool QMSL_FTM_GET_CDMA_IM2(unsigned char* piI_Value, unsigned char* piQ_Value, unsigned char* piTransConductance_Value) = 0 ;

	//	WCDMA Test
	virtual bool QMSL_FTM_WCDMA_BER_RMC_DCH_TFCS_CONFIG_REQ(unsigned char ucTFCS_Select, unsigned char* ucTFCS_Config) = 0;
	virtual bool QMSL_FTM_WCDMA_BER_StartSession(unsigned long ulTimeOut_ms) = 0;
	virtual bool QMSL_FTM_WCDMA_BER_AcquirePilot(unsigned short usChannel) = 0;
	virtual bool QMSL_FTM_WCDMA_BER_StopSession() = 0;
	virtual bool QMSL_FTM_UMTS_BER_ForceStopped() = 0;
	virtual bool QMSL_FTM_WCDMA_BER_Handover(unsigned char iRMC_Type, unsigned char iUL_MaxTxPwr, unsigned long iUL_DPCH_ScrCode, unsigned char iUL_PCA, unsigned char iUL_TPC_StepSize,  
											 unsigned short iDL_PriCPICH_Scr_Code, unsigned short iDL_ChanCode, unsigned char iBetaC, unsigned char iBetaD, unsigned short iChannel) = 0;
	virtual bool QMSL_FTM_WCDMA_BER_Handover_V2B(
												unsigned char	iRMC_Type, 
												unsigned char	iUL_MaxTxPwr, 
												unsigned long	iUL_DPCH_ScrCode, 
												unsigned char	iUL_PCA,
												unsigned char	iUL_TPC_StepSize, 
												unsigned short	iDL_PriCPICH_Scr_Code, 
												unsigned short	iDL_ChanCode,
												unsigned char	iBetaC, 
												unsigned char	iBetaD,
												short			iDPCCH_PowerOffset,
												unsigned char	n_312,
												unsigned char	n_313,
												unsigned char	n_315,
												unsigned char	t_312,
												unsigned char	t_313,
												unsigned short	iChannel) = 0;


    // GSM Function
    virtual bool QMSL_FTM_GSM_BER_StartSession(unsigned long iTimeOut) = 0 ;
    virtual bool QMSL_FTM_GSM_BER_AssignBCCH(unsigned short iChannel, int iBand) = 0 ;
    virtual bool QMSL_FTM_GSM_BER_Handover_NoLoopbackConfig(unsigned short iChannel,
                                                            int iBand,
                                                            unsigned char iSlot,
                                                            unsigned char iPowerLevel) = 0 ;
    virtual bool QMSL_FTM_GSM_BER_IsTrafficValid() = 0 ;
    virtual bool QMSL_FTM_GSM_BER_ClearStatus() = 0 ;
    virtual bool QMSL_FTM_GSM_BER_ReleaseTCH() = 0 ;
    virtual bool QMSL_FTM_GSM_BER_StopSession() = 0 ;

    // EDGE Function
    virtual bool QMSL_FTM_EGPRS_BER_AssignBCCH (unsigned short iChannel, int iBand) = 0 ;
    virtual bool QMSL_FTM_EGPRS_BER_Establish_SRB_LOOPBACK() = 0 ;
    virtual bool QMSL_FTM_EGPRS_BER_Configure_DL_TBF (unsigned short usChannel, int iBand) = 0 ;
    virtual bool QMSL_FTM_EGPRS_BER_Configure_UL_TBF (unsigned short iChannel, int iBand, unsigned char iGamma) = 0 ;
    virtual bool QMSL_FTM_EGPRS_BER_Release_All_TBF() = 0 ;
    virtual bool QMSL_FTM_GSM_ConfigLoopbackType(unsigned short usLookbackType) = 0 ;
    virtual bool QMSL_FTM_GSM_BER_GetRxMetrics(unsigned char* szRxLevel, unsigned char* szRxQual) = 0 ;
    virtual bool QMSL_FTM_SET_TX_POWER_LEVEL(unsigned char nPowerLevel) = 0 ;

protected:
private:
};

typedef IPhone* PIPhone;

#endif // End of #ifndef _I_PHONE_H_
