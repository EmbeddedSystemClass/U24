#ifndef _I_TESTER_DEVICE_H_
#define _I_TESTER_DEVICE_H_


#include "IGPIBDevice.h"


interface ITesterDevice : public IGPIBDevice
{
RF_DECLARE_DYNAMIC(ITesterDevice)


// Data members
public:
protected:
	static int g_iRFInputPort;
	static int g_iRFOutputPort;
	static int g_iMode;
    static int g_iCurrentBand ;
    static int g_iCurrentChannel ;

private:


// Member functions
public:
	ITesterDevice() {}
	virtual ~ITesterDevice() {}

	static void SetRFInputPort(int iInputPort);
	static void SetRFOutputPort(int iOutputPort);
	static void SetMode(int iMode);
    static void SetBand(int iBand) ;
    static void SetChannel(int iChannel) ;

	// Initial functions
	virtual bool Initial() = 0;
	virtual bool Initial_RF_Nonsignaling() = 0;
	virtual bool Initial_GSM() = 0;
    virtual bool Initial_WCDMA() = 0;
    virtual bool SetMaxPower(double dMaxPower) = 0;

	// Get Options of Test Device
	virtual bool GetDeviceOptions(std::string& strOptions) = 0;

	// Reset functions
	virtual bool Reset() = 0;

	// Set RF port functions
	virtual bool SetInputPort(int iMode, int iPort) = 0;
	virtual bool SetOutputPort(int iMode, int iPort) = 0;
	virtual bool SetInputPort(int iMode) = 0;
	virtual bool SetOutputPort(int iMode) = 0;
	virtual bool SetInputPort() = 0;
	virtual bool SetOutputPort() = 0;

	virtual bool SetLossTable(int iNumOfFreqs,double *dFreqs,double *dLoss_RF1,double *dLoss_RF2,double *dLoss_RF3,double *dLoss_RF4) = 0;

	// Set cable loss functions
	virtual bool SetInputLoss(int iMode, int iPort, double dLoss) = 0;
	virtual bool SetOutputLoss(int iMode, int iPort, double dLoss) = 0;
	virtual bool SetInputLoss(int iMode, double dLoss) = 0;
	virtual bool SetOutputLoss(int iMode, double dLoss) = 0;
	virtual bool SetInputLoss(double dLoss) = 0;
	virtual bool SetOutputLoss(double dLoss) = 0;
	virtual bool TurnOnOffLossTable(bool bOnOfff) = 0;

	// Set signal on/off
	virtual bool SetSignalOnOff(int iMode, bool bOn) = 0;
	virtual bool SetSignalOnOff(bool bOn) = 0;

	// RF functions
	virtual bool RF_InitialAnalyzer() = 0;
    virtual bool RF_SetAnalyzerFreq(double dFreq) = 0 ;
    virtual bool RF_SetGeneratorFreq(double dFreqMHz) = 0 ;
    virtual bool RF_SetGeneratorPowerLevel(double dLevel) = 0 ;
    virtual bool RF_TurnOnOffGernerator(bool bOn) = 0 ;
    virtual bool RF_NPowerMeasurement(int iStatistics, std::string& strMeasured) = 0 ;
	virtual bool RF_AnalyzerMeasurement(std::string& strMeasured) = 0;
	virtual bool RF_TurnOnOffAuxTxGenerator(bool bOn) = 0;
	virtual bool RF_TurnOnOffLowSpurModeForGenerator(bool bOn) = 0;
	virtual bool RF_SetModulationMode(std::string strMode) = 0;
	virtual bool RF_SetTriggerSourceType(std::string strType) = 0;
	virtual bool RF_SetTriggerDelay(int iSlotDelay,int iDelayOffset) = 0;
	virtual bool RF_SetIFPowerConfig(double dThreshold,std::string strSlope) = 0;
	virtual bool RF_SetAMModulationRation(int iPercent) = 0 ;
	virtual bool RF_SetAMModulationFrequency(int iHz) = 0 ;

    // GSM signaling functions
    virtual bool Initial_GSM_Param() = 0 ;
    virtual bool GSM_SetMode(int iBand, int iMode) = 0 ;
    virtual bool GSM_SetBandIndicator(int iBand) = 0 ;
    virtual bool GSM_SetCCH(int iChannel) = 0 ;
    virtual bool GSM_SetTCH(int iChannel) = 0 ;
    virtual bool GSM_SetTCH_Handover(int iChannel) = 0 ;
    virtual bool GSM_SetTCHTimeSlot(int iSlot) = 0 ;
    virtual bool GSM_SetTCHTimeSlotLevel_StartCall(float fLevel) = 0 ;
    virtual bool GSM_SetTCHTimeSlotLevel_Handover (float fLevel) = 0 ;
    virtual bool GSM_SetTCHTimeSlotPCL(int iPCL) = 0 ;
    virtual bool GSM_SetTCHTimeSlotPCL_Handover(int iPCL) = 0 ;
    virtual bool GSM_MTCall() = 0 ;
    virtual bool GSM_Handover() = 0 ;
    virtual bool GSM_CheckCallStatusCED() = 0 ;
    virtual bool GSM_CheckCallStatusCEST() = 0 ;
	virtual bool GSM_QuerryCampStatus() = 0;
	virtual bool GSM_MT_Call() = 0;
    virtual int  GSM_QueryBand() = 0 ;
    virtual int  GSM_QueryChannel() = 0 ;
    virtual bool GSM_SetMSMaxPower(float fInputLevel) = 0 ;
    virtual bool GSM_SetHANDoverTarget(int iTargetBand) = 0 ;

    // GSM signaling Test functions
	virtual bool GSM_FetchBERRSSI(std::string& strMeasured) = 0 ;
    virtual bool GSM_FetchRXQualityBER(std::string& strMeasured, int iBursts) = 0 ;
    virtual bool GSM_FetchRXQualityRFER(std::string& strMeasured, int iBursts) = 0 ;
    virtual bool GSM_FetchGMSKPower(std::string& strMeasured, int iBursts) = 0 ;
    virtual bool GSM_FetchGMSKPhaseError(std::string& strMeasured, int iBursts) = 0 ;
    virtual bool GSM_FetchModulationSpectrum(std::string& strMeasured, int iBursts) = 0 ;
    virtual bool GSM_SampleModulationSpectrum(float *out_pfPowerMOD) = 0 ;
    virtual bool GSM_FetchPowerVsTimeMask(std::string& strMeasured, int iBursts) = 0 ;
    virtual bool GSM_FetchSwitchingSpectrum(std::string& strMeasured, int iBursts) = 0 ;
    virtual bool GSM_PowerVsTimeMaskPaser(std::vector<std::string> vData,
                                          int iArea, double fPowerUpper,
                                          std::string& strMeasured) = 0 ;

    // GPRS signaling functions
    virtual bool GPRS_NeworkSelect() = 0 ;
    virtual bool GPRS_SetCS(int iCS) = 0 ;
    virtual bool GPRS_SetCS_Handover(int iCS) = 0 ;

    // EDGE signaling functions
    virtual bool EDGE_NeworkSelect() = 0 ;
    virtual bool EDGE_SetSignalOnOff(bool bOn) = 0 ;
    virtual bool Initial_EDGE_Param() = 0 ;
    virtual bool EDGE_SetTCH(int iChannel) = 0 ;
    virtual bool EDGE_SetTCH_Handover(int iChannel) = 0 ;
    virtual bool EDGE_SetTCHMainTimeslot(int iSlot) = 0 ;
    virtual bool EDGE_SetBCCHAbsLevel(float fLevel) = 0 ;
    virtual bool EDGE_SetTCHLevel(float fLevel) = 0 ;
    virtual bool EDGE_SetTimeslotConfig_Downlink(std::string strEnable, std::string strLevel) = 0 ;
    virtual bool EDGE_SetTimeslotConfig_Uplink(std::string strEnable, std::string strLevel) = 0 ;
    virtual bool EDGE_SetTimeslotConfig_ULDL_Handover(int iMainSlot, 
                                                      std::string strDLEnable, std::string strDLLevel,
                                                      std::string strULEnable, std::string strULGamma) = 0 ;
    virtual bool EDGE_SetTCHPzero(int iPzero) = 0 ;
    virtual bool EDGE_ConnectRES() = 0 ;
    virtual bool EDGE_ConnectTestModeA() = 0 ;
    virtual bool EDGE_ConnectBLER() = 0 ;
    virtual bool EDGE_Handover() = 0 ;
    virtual bool EDGE_DisconnectMobile() = 0 ;
    virtual bool EDGE_QueryAttachStatus() = 0 ;
    virtual bool EDGE_QueryService(std::string& strService) = 0 ;
    virtual bool EDGE_CheckCallStatus() = 0 ;
    virtual bool EDGE_SetMCS(int iMCS) = 0 ;
    virtual bool EDGE_SetMCS_Handover(int iMCS) = 0 ;

    // EDGE signaling Test functions
    virtual bool EDGE_FetchRXQualityBLER(std::string& strMeasured, int iBursts) = 0 ;
    virtual bool EDGE_FetchEPSKPower(std::string& strMeasured, int iBursts) = 0 ;
    virtual bool EDGE_FetchEVMEPSKPhaseError(std::string& strMeasured, int iBursts) = 0 ;
    virtual bool EDGE_FetchPowerVsTimeMask(std::string& strMeasured, int iBursts) = 0 ;
    virtual bool EDGE_PowerVsTimeMaskPaser(std::vector<std::string> vData, int iArea, double fPowerUpper, std::string& strMeasured) = 0 ;
    virtual bool EDGE_FetchPowerVsTimeMask_MultiSlot(std::string& strMeasured, int iBursts, int iSlotAmount) = 0 ;

    // WCDMA signaling functions
    virtual bool Initial_WCDMA_Online() = 0;
    virtual bool WCDMA_SetBand(int iBand) = 0;
    virtual bool WCDMA_SetDownlinkChannel(int iChannel) = 0;
    virtual bool WCDMA_SetDCHType(std::string strType) = 0;
    virtual bool WCDMA_SetBSOutputPower(double dPower) = 0;
    virtual bool WCDMA_SetMaxMSPower(double dPower) = 0;
    virtual bool WCDMA_SetMSTargetPower(double dPower) = 0;
    virtual bool WCDMA_SetTPCPattern(std::string strPattern) = 0;
    virtual bool WCDMA_SetTPCPatternType(std::string strType) = 0;
	virtual bool WCDMA_SetAuthentication(bool bOnOff) = 0;
    virtual bool WCDMA_SetSecurityKey(std::string strKey1, std::string strKey2) = 0;
    virtual bool WCDMA_QuerryCampStatus() = 0;
    virtual bool WCDMA_MTCall() = 0;
    virtual bool WCDMA_SetBandAndDownlinkChannel(int iBand, int iChannel) = 0;
    virtual bool WCDMA_QuerryCallStatus() = 0;
    virtual bool WCDMA_InitMaxPowerMeasurement() = 0;
    virtual bool WCDMA_FetchMaxPower(std::string& strMeasured) = 0;
    virtual bool WCDMA_InitMinPowerMeasurement() = 0;
    virtual bool WCDMA_FetchMinPower(std::string& strMeasured) = 0;
    virtual bool WCDMA_InitModulationMeasurement() = 0;
    virtual bool WCDMA_FetchModulation(std::string& strMeasured) = 0;
    virtual bool WCDMA_InitACLR_OBW_Measurement() = 0;
    virtual bool WCDMA_FetchACLR_OBW(std::string& strMeasured) = 0;
    virtual bool WCDMA_InitSEMMeasurement() = 0;
    virtual bool WCDMA_FetchSEM(std::string& strMeasured) = 0;
    virtual bool WCDMA_InitILPC() = 0;
    virtual bool WCDMA_FetchILPC(std::string strPattern, std::string& strMeasured) = 0;
    virtual bool WCDMA_InitOpenLoop_OffPower(double dBSPower = -51.7, double dULInterference = -80.0,
                                             double dConstantValue = -29.0, double dPCPICHPower = 31.0) = 0;
    virtual bool WCDMA_FetchOpenLoop_OffPower(std::string& strMeasured) = 0;
    virtual bool WCDMA_EndCall() = 0;
    virtual bool WCDMA_InitBERMMeasurement() = 0;
    virtual bool WCDMA_FetchBERMMeasurement(std::string& strMeasured) = 0;
	virtual bool WCDMA_FetchRSCPMeasurement(std::string& strMeasured) = 0;
	virtual bool WCDMA_SetHANDoverTarget() = 0;

    // HSDPA signaling functions
    virtual bool HSDPA_QuerryCampStatus() = 0;
    virtual bool HSDPA_QuerryCallStatus() = 0;
    virtual bool HSDPA_MTCall() = 0;
    virtual bool HSDPA_EndCall() = 0;
    virtual bool HSDPA_SetGainFactor(int iBetaC, int iBetaD, int iDeltaACK = 5, int iDeltaNACK = 5, int iDeltaCQI = 2) = 0;

    // WCDMA Cal
	virtual	bool WCDMA_SetMode(bool bSignalling) = 0;
	virtual bool WCDMA_SetGeneratorPower(double dOutputPower) = 0;
	virtual bool WCDMA_SetPowerVsSlotMeasuredMode(std::string strMode) = 0;
	virtual bool WCDMA_SetPowerVsSlotMeasuredConfigure(double dPowerMeasuredWidth,int iStepCount,int iStepWidth,int iStepDelay) = 0;
	virtual bool WCDMA_SetPowerVsSlotMeasuredRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode) = 0;
	virtual bool WCDMA_FetchPowerVsSlotMeasurment(double* dMearsurment) = 0;
	
	virtual bool WCDMA_SetFastPowerMearsuredConfig(std::string strRepetition,std::string strStopCondition,std::string strStepMode) = 0;
	virtual bool WCDMA_SetFastPowerMearsuredConfig(std::string strStatics,std::string strRepetition,std::string strStopCondition,std::string strStepMode) = 0;
	virtual bool WCDMA_FetchFastPowerMeasurement(double& dAvgPwr_Current,double& dMinPwr_Current,double& dMaxPwr_Current,double& dAvgPwr_LastCycle,double& dMinPwr_Entire,double& dMaxPwr_Entire)=0;
	virtual bool WCDMA_InitiatePowerVsSlotMeasurment() = 0;

	virtual bool WCDMA_SetSmartAlignmentRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode) = 0;
	virtual bool WCDMA_SetSmartAlignmentConfig(int iFilterType,int iUnitType,double dStepWidth) = 0;
	virtual bool WCDMA_SetSmartAlignmentConfigOfPowers(int iNumOfPower,double *dOutputPower,double *dMaxLevel) = 0;
	virtual bool WCDMA_SetSmartAlignmentConfigOfFreqs(int iNumOfFreq,double *dGenFreq,double *dAnalyzerFreq) = 0;
	virtual bool WCDMA_InitiateSmartAlignmentMeasurement() = 0;
	virtual bool WCDMA_FetchSmartAlignmentMeasurement(double* dMeasurements) = 0;

	// WCDMA FTM Test
	virtual bool WCDMA_ReferenceChannelType(std::string strType) = 0;
	virtual bool WCDMA_SetScramblingCode(int iPrimaryScramblingCode,int iSecondaryScramblingCode) = 0;
	virtual bool WCDMA_SetBSSynchronizationChannelLevel(double dPCPICH_Level,double dPSCH_Level,double dSSCH_Level,double SCPICH_Level) = 0;
	virtual bool WCDMA_SetBSPhysicalChannelLevel(double dP_CPICH,double dP_SCH,double dS_SCH,double dP_CCPCH,double dPICH,double dDPDCH,double dPowerOffset) = 0;
	virtual bool WCDMA_SetBSDedicatedBroadcastChannelCode(int iDPDCHCode,int iPICHCode) = 0;
	virtual bool WCDMA_SetUEScramblingCode(int iSramblingCode) = 0;
	virtual bool WCDMA_SetBS_TPCControlType(int iPatternSetNumber,std::string strType) = 0;
	virtual bool WCDMA_SetBS_TPC_PatternSet(int iPatternSetNumber) = 0;
	virtual bool WCDMA_SetBS_TPC_PatternSet(std::string strPatternSet) = 0;
	virtual bool WCDMA_SetBS_TPC_AlgorithmMode(int iAlg) = 0;
	virtual bool WCDMA_TurnBS_TPC_PatternOn() = 0;
	virtual bool WCDMA_TurnOnILPC_Measurement(bool bOn) = 0;

	virtual bool WCDMA_FetchILPCMearsurement(std::string strILPCMode,double dMaxInputPower,double* dPowerVsSlotArray) = 0;

	virtual bool WCDMA_SetACLRNumberOfBurstsToMearsured(int iNumberOfBursts) = 0;
	virtual bool WCDMA_SetACLRMearsuredRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode) = 0;
	virtual bool WCDMA_SetACLRMearsuredLength(int iChipNumbers) = 0;
	virtual bool WCDMA_TurnOnOffACLRMearsurement(bool bOn) = 0;
	virtual bool WCDMA_FetchACLRMearsurement(
											double& dCarrierPower_PeakCurrent,	double& dCarrierPower_RMSCurrent,
											double& dACLR_PeakCurrent_Minus10MHz,	double& dACLR_PeakCurrent_Minus5MHz,double& dACLR_PeakCurrent_5MHz,	double& dACLR_PeakCurrent_10MHz,
											double& dACLR_RMSCurrent_Minus10MHz,	double& dACLR_RMSCurrent_Minus5MHz,	double& dACLR_RMSCurrent_5MHz,	double& dACLR_RMSCurrent_10MHz,
											double& dACLR_RMSAvg_Minus10MHz,		double& dACLR_RMSAvg_Minus5MHz,		double& dACLR_RMSAvg_5MHz,		double& dACLR_RMSAvg_10MHz,
											double& dACLR_PeakMax_Minus10MHz,		double& dACLR_PeakMax_Minus5MHz,	double& dACLR_PeakMax_5MHz,		double& dACLR_PeakMax_10MHz,
											double& dACLR_RMSMax_Minus10MHz,		double& dACLR_RMSMax_Minus5MHz,		double& dACLR_RMSMax_5MHz,		double& dACLR_RMSMax_10MHz,
											double& dUEPower_RMSCurrent,			double& dOutOfTolerance
											) = 0;

	virtual bool WCDMA_SetOBWNumberOfBurstsToMearsured(int iNumberOfBursts) = 0;
	virtual bool WCDMA_SetOBWMeasuredRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode) = 0;
	virtual bool WCDMA_SetOBWMeasuredType(std::string strType) = 0;
	virtual bool WCDMA_SetOBWMearsuredLength(int iChipNumbers) = 0;
	virtual bool WCDMA_SetDefaultOBWMeasurement(bool bOn) = 0;
	virtual bool WCDMA_TurnOnOffMaxDynamicRangeForACLR_OBW(bool bOn) = 0;
	virtual bool WCDMA_FetchOBWMearsurement(
											double& dCarrierPower_PeakCurrent,		double& dCarrierPower_RMSCurrent,
											double& dACLR_RMSCurrent_Minus10MHz,	double& dACLR_RMSCurrent_Minus5MHz,	double& dACLR_RMSCurrent_5MHz,	double& dACLR_RMSCurrent_10MHz,
											double& dACLR_RMSAvg_Minus10MHz,		double& dACLR_RMSAvg_Minus5MHz,		double& dACLR_RMSAvg_5MHz,		double& dACLR_RMSAvg_10MHz,
											double& dACLR_RMSMax_Minus10MHz,		double& dACLR_RMSMax_Minus5MHz,		double& dACLR_RMSMax_5MHz,		double& dACLR_RMSMax_10MHz,
											double& dOBW_Current,					double& dOBW_Avg,					double& dOBW_Max,				double& dOBW_Left,
											double& dOBW_Right,						double& dUEPower_RMSCurrent,		double& dOutOfTolerance
											) = 0;
	virtual bool WCDMA_SetSEMNumberOfBurstsToMearsured(int iNumberOfBursts) = 0;
	virtual bool WCDMA_SetSEMMearsuredRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode) = 0;
	virtual bool WCDMA_SetSEMMearsuredLength(int iChipNumbers) = 0;
	virtual bool WCDMA_SetSEMMeasuredType(std::string strType) = 0;
	virtual bool WCDMA_SetDefaultSEMMeasurement(bool bOn) = 0;
	virtual bool WCDMA_SetSEMMeasurementLowBoundaryCheck(bool bOn_Max,bool bOn_Average) = 0;
	virtual	bool WCDMA_SetSEMLineMaskPower(double dMaskPwr_12MHz,double dMaskPwr_8_5MHz,double dMaskPwr_7_5MHz,double dMaskPwr_4MHz,double dMaskPwr_3_845MHz,double dMaskPwr_2_515MHz) = 0;
	virtual bool WCDMA_FetchSEMMearsurement(
											std::string strMeasuredType,
											double& dReferencePower_Current,double& dReferencePower_Avg,double& dReferencePower_Max,
											double& dSEM_Minus_12_8p5MHz,	double& dSEM_Minus_8p5_7p5MHz,	double& dSEM_Minus_7p5_4MHz,double& dSEM_Minus_3p845_2p515MHz,
											double& dSEM_12_8p5MHz,			double& dSEM_8p5_7p5MHz,		double& dSEM_7p5_4MHz,		double& dSEM_3p845_2p515MHz,
											double& dUEPower_RMSCurrent,	double& dOutOfTolerance
											) = 0;

	virtual bool WCDMA_SetBERMearsuredRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode) = 0;
	virtual bool WCDMA_SetBERMearsuredDelayTime(int iChipNumbers) = 0;
	virtual bool WCDMA_SetSameBlockSize_DownLink_UpLink(bool bSame) = 0;
	virtual bool WCDMA_SetBERNumberOfMearsuredBlock(std::string strMeasuredType,int iNumOfBlocks) = 0;
	virtual bool WCDMA_FetchBERMearsurement(
											double& dBER,
											double& dBLER,
											double &dDBLER,
											int& iLostTransportBlocks,
											int& iAlignmentDLUL,
											double& dFDR,
											double& dTFCI_ULFaults,
											double&	dPNDiscontinuity,
											int& iTransportBlockDelay) = 0;

	virtual bool WCDMA_SetModulationMearsuredRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode) = 0;
	virtual bool WCDMA_SetModulationMeasuredLength(int iChipNumbers) = 0;
	virtual bool WCDMA_SetModulationNumberOfBurstsToMearsured(int iNumberOfBursts) = 0;
	virtual bool WCDMA_FetchModulationMearsurement(
											double& dEVM_PeakCurrent,			double& dEVM_PeakAvg,			double& dEVM_PeakMax,
											double& dEVM_RMSCurrent,			double& dEVM_RMSAvg,			double& dEVM_RMSMax,
											double& dMagnErr_PeakCurrent,		double& dMagnErr_PeakAvg,		double& dMagnErr_PeakMax,
											double& dMagnErr_RMSCurrent,		double& dMagnErr_RMSAvg,		double& dMagnErr_RMSMax,
											double& dPhaseErr_PeakCurrent,		double& dPhaseErr_PeakAvg,		double& dPhaseErr_PeakMax,
											double& dPhaseErr_RMSCurrent,		double& dPhaseErr_RMSAvg,		double& dPhaseErr_RMSMax,
											double& dIQ_OriginOffset_Current,	double& dIQ_OriginOffset_Avg,	double& dIQ_OriginOffset_Max,
											double& dIQ_Imbalance_Current,		double& dIQ_Imbalance_Avg,		double& dIQ_Imbalance_Max,
											double& dCarrierFreqErr_Current,	double& dCarrierFreqErr_Avg,	double& dCarrierFreqErr_Max,
											double& dWaveQuality_Current,		double& dWaveQuality_Avg,		double& dWaveQuality_Max,
											double& dPeakCodeDomErr_Current,	double& dPeakCodeDomErr_Avg,	double& dPeakCodeDomErr_Max,
											double& dTransmitTimeErr_Current,	double& dTransmitTimeErr_Avg,	double& dTransmitTimeErr_Max,		
											double& dUEPower_RMSCurrent,		double& dOutOfTolerance,		int&	iSlotNumber
											) = 0;

	virtual bool WCDMA_SetBSOutputPower_NonSignaling(double dOutputPower) = 0;
	// GSM Cal
	virtual bool GSM_FetchTxSweepRez(float *fTxPowerArray,float *fTxPhasArray,int iSizeofTotalTable) = 0;
	virtual bool GSM_FetchIQRecorderPL(float *fTxPowerArray, float *fTxPhaseArray, int iSizeofTotalTable) = 0;

	// FSP
	virtual bool SetResolutionBandwidth(double dRBWMHz) = 0;
	virtual bool SetVideoBandwidth(double dVBWMHz) = 0;
	virtual bool SetFrequencySpan(double dSpanMHz) = 0;
	virtual bool SwitchTraceMode(std::string strTraceMode) = 0;
	virtual bool SetDetector(std::string strDetector) = 0;
	virtual bool SwitcheMeasurementWindow(std::string strDiagramFullSize) = 0;
	virtual bool AdjustReferenceLevel() = 0;
	virtual bool SetAttenuation(bool bEnable) = 0;
	virtual bool SetSweepTime(int iSweepTime) = 0;
	virtual bool SetLimitLines(std::string strLineName, std::string strDomain, int iTrace, 
		std::string strXscalingMode, std::string strUnit, std::string strYscalingMode, 
		std::string strSpacing, int iMargin, int iThreshold,
		std::string strContData, std::string strUppData) = 0;
	virtual bool SpectrumMaskMeasurement(int iStatistics, std::string& strMeasured) = 0;
	virtual bool FrequencyErrorMeasurement(int iStatistics, std::string& strMeasured) = 0;
	virtual bool SetAttRefLevel(int iAttenuationManual, int iRefLevel) = 0;
	virtual bool SetChannelBandwidth(double dChannelBWMHz) = 0;
	virtual bool SetFrequencyError(double dPeakExcursion, int iTrace, double iFreqCounterRes) = 0;
	virtual bool SwitchChannelPower(std::string strChannelPower) = 0;
	virtual bool SetMaxLevel() = 0;

protected:
private:
};


#endif // End of #ifndef _I_TESTER_DEVICE_H_
