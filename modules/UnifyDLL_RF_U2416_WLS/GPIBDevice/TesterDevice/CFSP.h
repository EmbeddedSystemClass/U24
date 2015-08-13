#ifndef _C_CFSP_H_
#define _C_CFSP_H_


#include "../../CommonUtility/BaseObject/ITesterDevice.h"
#include "../../CommonUtility/CommonDefine.h"
#include <cctype>
#include <algorithm>
#include <limits.h>
#include <math.h>

class CFSP : public ITesterDevice
{
RF_DECLARE_DYNCREATE(CFSP)
public:
	CFSP(void) {}
	virtual ~CFSP(void) {}

// Initial functions
	virtual bool Initial();
	virtual bool Initial_RF_Nonsignaling() { return false; }
	virtual bool Initial_GSM() { return false; }
    virtual bool Initial_WCDMA() { return false; }
    virtual bool SetMaxPower(double dMaxPower)  { return false; }

	// Get Options of Test Device
	virtual bool GetDeviceOptions(std::string& strOptions)  { return false; }

	// Reset functions
	virtual bool Reset() { return false; }

	// Set RF port functions
	virtual bool SetInputPort(int iMode, int iPort) { return false; }
	virtual bool SetOutputPort(int iMode, int iPort) { return false; }
	virtual bool SetInputPort(int iMode) { return false; }
	virtual bool SetOutputPort(int iMode) { return false; }
	virtual bool SetInputPort() { return false; }
	virtual bool SetOutputPort() { return false; }

	// Set cable loss functions
	virtual bool SetInputLoss(int iMode, int iPort, double dLoss) { return false; }
	virtual bool SetOutputLoss(int iMode, int iPort, double dLoss) { return false; }
	virtual bool SetInputLoss(int iMode, double dLoss) { return false; }
	virtual bool SetOutputLoss(int iMode, double dLoss) { return false; }
	virtual bool SetInputLoss(double dLoss);
	virtual bool SetOutputLoss(double dLoss) { return false; }
	
	virtual bool TurnOnOffLossTable(bool bOnOfff) { return false; }
	virtual bool SetLossTable(int iNumOfFreqs,double *dFreqs,double *dLoss_RF1,double *dLoss_RF2,double *dLoss_RF3,double *dLoss_RF4) { return false; }

	// Set signal on/off
	virtual bool SetSignalOnOff(int iMode, bool bOn) { return false; }
	virtual bool SetSignalOnOff(bool bOn) { return false; }

	// RF analyzer functions
	virtual bool RF_InitialAnalyzer() { return false; }
	virtual bool RF_SetAnalyzerFreq(double dFreqMHz);
    virtual bool RF_SetGeneratorFreq(double dFreqMHz)  { return false; }
    virtual bool RF_SetGeneratorPowerLevel(double dLevel)  { return false; }
    virtual bool RF_TurnOnOffGernerator(bool bOn)  { return false; }
	virtual bool RF_NPowerMeasurement(int iStatistics, std::string& strMeasured);
	virtual bool RF_NPowerMeasurementMAX(int iStatistics, std::string& strMeasured);
	virtual bool RF_NPowerMeasurementNoOFF(int iStatistics, std::string& strMeasured);

	virtual bool RF_AnalyzerMeasurement(std::string& strMeasured) { return false; }
	virtual bool RF_TurnOnOffAuxTxGenerator(bool bOn) { return false; }
	virtual bool RF_TurnOnOffLowSpurModeForGenerator(bool bOn) { return false; }
	virtual bool RF_SetModulationMode(std::string strMode) { return false; }
	virtual bool RF_SetTriggerDelay(int iSlotDelay, int iDelayOffset) { return false; }
	virtual bool RF_SetTriggerSourceType(std::string strType) { return false; }
	virtual bool RF_SetIFPowerConfig(double dThreshold, std::string strSlope) { return false; }
	virtual bool RF_SetAMModulationRation(int iPercent)  { return false; }
	virtual bool RF_SetAMModulationFrequency(int iHz)  { return false; }
	virtual bool RF_SetLevelMode(std::string strLevelMode) { return false; }
	virtual bool RF_SetLevelMaximum(int iLevelRef) { return false; }

    // GSM signaling functions
    virtual bool Initial_GSM_Param()  { return false; }
    virtual bool GSM_SetMode(int iBand, int iMode)  { return false; }
    virtual bool GSM_SetBandIndicator(int iBand)  { return false; }
    virtual bool GSM_SetCCH(int iChannel)  { return false; }
    virtual bool GSM_SetTCH(int iChannel)  { return false; }
    virtual bool GSM_SetTCH_Handover(int iChannel)  { return false; }
    virtual bool GSM_SetTCHTimeSlot(int iSlot)  { return false; }
    virtual bool GSM_SetTCHTimeSlotLevel_StartCall(float fLevel)  { return false; }
    virtual bool GSM_SetTCHTimeSlotLevel_Handover (float fLevel)  { return false; }
    virtual bool GSM_SetTCHTimeSlotPCL(int iPCL)  { return false; }
    virtual bool GSM_SetTCHTimeSlotPCL_Handover(int iPCL)  { return false; }
    virtual bool GSM_MTCall()  { return false; }
    virtual bool GSM_Handover()  { return false; }
    virtual bool GSM_CheckCallStatusCED()  { return false; }
    virtual bool GSM_CheckCallStatusCEST()  { return false; }
	virtual bool GSM_QuerryCampStatus() { return false; }
	virtual bool GSM_MT_Call() { return false; }
    virtual int  GSM_QueryBand()  { return false; }
    virtual int  GSM_QueryChannel()  { return false; }

    // GSM signaling Test functions
	virtual bool GSM_FetchBERRSSI(std::string& strMeasured) { return false; }
    virtual bool GSM_SetMSMaxPower(float fInputLevel)  { return false; }
    virtual bool GSM_SetHANDoverTarget(int iTargetBand)  { return false; }
    virtual bool GSM_FetchRXQualityBER(std::string& strMeasured, int iBursts)  { return false; }
    virtual bool GSM_FetchRXQualityRFER(std::string& strMeasured, int iBursts)  { return false; }
    virtual bool GSM_FetchGMSKPower(std::string& strMeasured, int iBursts)  { return false; }
    virtual bool GSM_FetchGMSKPhaseError(std::string& strMeasured, int iBursts)  { return false; }
    virtual bool GSM_FetchModulationSpectrum(std::string& strMeasured, int iBursts)  { return false; }
    virtual bool GSM_SampleModulationSpectrum(float *out_pfPowerMOD)  { return false; }
    virtual bool GSM_FetchPowerVsTimeMask(std::string& strMeasured, int iBursts)  { return false; }
    virtual bool GSM_PowerVsTimeMaskPaser(std::vector<std::string> vData, int iArea, double fPowerUpper, std::string& strMeasured)  { return false; }
    virtual bool GSM_FetchSwitchingSpectrum(std::string& strMeasured, int iBursts)  { return false; }

    // GPRS signaling functions
    virtual bool GPRS_NeworkSelect()  { return false; }
    virtual bool GPRS_SetCS(int iCS)  { return false; }
    virtual bool GPRS_SetCS_Handover(int iCS)  { return false; }

    // EDGE signaling functions
    virtual bool EDGE_NeworkSelect()  { return false; }
    virtual bool EDGE_SetSignalOnOff(bool bOn)  { return false; }
    virtual bool Initial_EDGE_Param()  { return false; }
    virtual bool EDGE_SetTCH(int iChannel)  { return false; }
    virtual bool EDGE_SetTCH_Handover(int iChannel)  { return false; }
    virtual bool EDGE_SetTCHMainTimeslot(int iSlot)  { return false; }
    virtual bool EDGE_SetBCCHAbsLevel(float fLevel)  { return false; }
    virtual bool EDGE_SetTCHLevel(float fLevel)  { return false; }
    virtual bool EDGE_SetTimeslotConfig_Downlink(std::string strEnable, std::string strLevel)  { return false; }
    virtual bool EDGE_SetTimeslotConfig_Uplink(std::string strEnable, std::string strLevel)  { return false; }
    virtual bool EDGE_SetTimeslotConfig_ULDL_Handover(int iMainSlot,
        std::string strDLEnable, std::string strDLLevel,
        std::string strULEnable, std::string strULGamma)  { return false; }
    virtual bool EDGE_SetTCHPzero(int iPzero)  { return false; }
    virtual bool EDGE_ConnectRES()  { return false; }
    virtual bool EDGE_ConnectTestModeA()  { return false; }
    virtual bool EDGE_ConnectBLER()  { return false; }
    virtual bool EDGE_Handover()  { return false; }
    virtual bool EDGE_DisconnectMobile()  { return false; }
    virtual bool EDGE_QueryAttachStatus()  { return false; }
    virtual bool EDGE_QueryService(std::string& strService)  { return false; }
    virtual bool EDGE_CheckCallStatus()  { return false; }
    virtual bool EDGE_SetMCS(int iMCS)  { return false; }
    virtual bool EDGE_SetMCS_Handover(int iMCS)  { return false; }


    // EDGE signaling Test functions
    virtual bool EDGE_FetchRXQualityBLER(std::string& strMeasured, int iBursts)  { return false; }
    virtual bool EDGE_FetchEPSKPower(std::string& strMeasured, int iBursts)  { return false; }
    virtual bool EDGE_FetchEVMEPSKPhaseError(std::string& strMeasured, int iBursts)  { return false; }
    virtual bool EDGE_FetchPowerVsTimeMask(std::string& strMeasured, int iBursts)  { return false; }
    virtual bool EDGE_PowerVsTimeMaskPaser(std::vector<std::string> vData, int iArea, double fPowerUpper, std::string& strMeasured)  { return false; }
    virtual bool EDGE_FetchPowerVsTimeMask_MultiSlot(std::string& strMeasured, int iBursts, int iSlotAmount)  { return false; }


    // WCDMA signaling functions
    virtual bool Initial_WCDMA_Online() { return false; }
    virtual bool WCDMA_SetBand(int iBand) { return false; }
    virtual bool WCDMA_SetDownlinkChannel(int iChannel) { return false; }
    virtual bool WCDMA_SetDCHType(std::string strType) { return false; }
    virtual bool WCDMA_SetBSOutputPower(double dPower) { return false; }
    virtual bool WCDMA_SetMaxMSPower(double dPower) { return false; }
    virtual bool WCDMA_SetMSTargetPower(double dPower) { return false; }
    virtual bool WCDMA_SetTPCPattern(std::string strPattern) { return false; }
    virtual bool WCDMA_SetTPCPatternType(std::string strType) { return false; }
	virtual bool WCDMA_SetAuthentication(bool bOnOff) { return false; }
    virtual bool WCDMA_SetSecurityKey(std::string strKey1, std::string strKey2) { return false; }
    virtual bool WCDMA_QuerryCampStatus() { return false; }
    virtual bool WCDMA_MTCall() { return false; }
    virtual bool WCDMA_SetBandAndDownlinkChannel(int iBand, int iChannel) { return false; }
    virtual bool WCDMA_QuerryCallStatus() { return false; }
    virtual bool WCDMA_InitMaxPowerMeasurement() { return false; }
    virtual bool WCDMA_FetchMaxPower(std::string& strMeasured) { return false; }
    virtual bool WCDMA_InitMinPowerMeasurement() { return false; }
    virtual bool WCDMA_FetchMinPower(std::string& strMeasured) { return false; }
    virtual bool WCDMA_InitModulationMeasurement() { return false; }
    virtual bool WCDMA_FetchModulation(std::string& strMeasured) { return false; }
    virtual bool WCDMA_InitACLR_OBW_Measurement() { return false; }
    virtual bool WCDMA_FetchACLR_OBW(std::string& strMeasured) { return false; }
    virtual bool WCDMA_InitSEMMeasurement() { return false; }
    virtual bool WCDMA_FetchSEM(std::string& strMeasured) { return false; }
    virtual bool WCDMA_InitILPC() { return false; }
    virtual bool WCDMA_FetchILPC(std::string strPattern, std::string& strMeasured) { return false; }
    virtual bool WCDMA_InitOpenLoop_OffPower(double dBSPower, double dULInterference, double dConstantValue, double dPCPICHPower) { return false; }
    virtual bool WCDMA_FetchOpenLoop_OffPower(std::string& strMeasured) { return false; }
    virtual bool WCDMA_EndCall() { return false; }
    virtual bool WCDMA_InitBERMMeasurement() { return false; }
    virtual bool WCDMA_FetchBERMMeasurement(std::string& strMeasured) { return false; }
	virtual bool WCDMA_FetchRSCPMeasurement(std::string& strMeasured) { return false; }
	virtual bool WCDMA_SetHANDoverTarget() { return false; }

    // HSDPA signaling functions
    virtual bool HSDPA_QuerryCampStatus() { return false; }
    virtual bool HSDPA_QuerryCallStatus() { return false; }
    virtual bool HSDPA_MTCall() { return false; }
    virtual bool HSDPA_EndCall() { return false; }
    virtual bool HSDPA_SetGainFactor(int iBetaC, int iBetaD, int iDeltaACK = 5, int iDeltaNACK = 5, int iDeltaCQI = 2) { return false; }


    // WCDMA Cal
	virtual bool WCDMA_SetMode(bool bSignalling) { return false; }
	virtual bool WCDMA_SetGeneratorPower(double dOutputPower) { return false; }
	
	virtual bool WCDMA_SetPowerVsSlotMeasuredMode(std::string strMode) { return false; }
	virtual bool WCDMA_SetPowerVsSlotMeasuredConfigure(double dPowerMeasuredWidth,int iStepCount,int iStepWidth,int iStepDelay) { return false; }
	virtual bool WCDMA_SetPowerVsSlotMeasuredRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode) { return false; }
	virtual bool WCDMA_InitiatePowerVsSlotMeasurment() { return false; }
	virtual bool WCDMA_FetchPowerVsSlotMeasurment(double* dMearsurment) { return false; }
	
	virtual bool WCDMA_SetFastPowerMearsuredConfig(std::string strRepetition,std::string strStopCondition,std::string strStepMode) { return false; }
	virtual bool WCDMA_SetFastPowerMearsuredConfig(std::string strStatics,std::string strRepetition,std::string strStopCondition,std::string strStepMode) { return false; }
	virtual bool WCDMA_FetchFastPowerMeasurement(double& dAvgPwr_Current,double& dMinPwr_Current,double& dMaxPwr_Current,double& dAvgPwr_LastCycle,double& dMinPwr_Entire,double& dMaxPwr_Entire) { return false; }
	
	virtual bool WCDMA_SetSmartAlignmentRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode) { return false; }
	virtual bool WCDMA_SetSmartAlignmentConfig(int iFilterType,int iUnitType,double dStepWidth) { return false; }
	virtual bool WCDMA_SetSmartAlignmentConfigOfPowers(int iNumOfPower,double *dOutputPower,double *dMaxLevel) { return false; }
	virtual bool WCDMA_SetSmartAlignmentConfigOfFreqs(int iNumOfFreq,double *dGenFreq,double *dAnalyzerFreq) { return false; }
	virtual bool WCDMA_InitiateSmartAlignmentMeasurement() { return false; }
	virtual bool WCDMA_FetchSmartAlignmentMeasurement(double* dMeasurements) { return false; }

	// WCDMA FTM Test
	virtual bool WCDMA_ReferenceChannelType(std::string strType) { return false; }
	virtual bool WCDMA_SetScramblingCode(int iPrimaryScramblingCode,int iSecondaryScramblingCode) { return false; }
	virtual bool WCDMA_SetBSSynchronizationChannelLevel(double dPCPICH_Level,double dPSCH_Level,double dSSCH_Level,double SCPICH_Level) { return false; }
	virtual bool WCDMA_SetBSPhysicalChannelLevel(double dP_CPICH,double dP_SCH,double dS_SCH,double dP_CCPCH,double dPICH,double dDPDCH,double dPowerOffset) { return false; }
	virtual bool WCDMA_SetBSDedicatedBroadcastChannelCode(int iDPDCHCode,int iPICHCode) { return false; }
	virtual bool WCDMA_SetUEScramblingCode(int iSramblingCode) { return false; }
	virtual bool WCDMA_SetBS_TPCControlType(int iPatternSetNumber,std::string strType) { return false; }
	virtual bool WCDMA_SetBS_TPC_PatternSet(int iPatternSetNumber) { return false; }
	virtual bool WCDMA_SetBS_TPC_PatternSet(std::string strPatternSet) { return false; }
	virtual bool WCDMA_SetBS_TPC_AlgorithmMode(int iAlg) { return false; }
	virtual bool WCDMA_TurnBS_TPC_PatternOn() { return false; }
	virtual bool WCDMA_TurnOnILPC_Measurement(bool bOn) { return false; }

	virtual bool WCDMA_FetchILPCMearsurement(std::string strILPCMode,double dMaxInputPower,double* dPowerVsSlotArray) { return false; }

	virtual bool WCDMA_SetACLRNumberOfBurstsToMearsured(int iNumberOfBursts) { return false; }
	virtual bool WCDMA_SetACLRMearsuredRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode) { return false; }
	virtual bool WCDMA_SetACLRMearsuredLength(int iChipNumbers) { return false; }
	virtual bool WCDMA_FetchACLRMearsurement(
											double& dCarrierPower_PeakCurrent,	double& dCarrierPower_RMSCurrent,
											double& dACLR_PeakCurrent_Minus10MHz,	double& dACLR_PeakCurrent_Minus5MHz,double& dACLR_PeakCurrent_5MHz,	double& dACLR_PeakCurrent_10MHz,
											double& dACLR_RMSCurrent_Minus10MHz,	double& dACLR_RMSCurrent_Minus5MHz,	double& dACLR_RMSCurrent_5MHz,	double& dACLR_RMSCurrent_10MHz,
											double& dACLR_RMSAvg_Minus10MHz,		double& dACLR_RMSAvg_Minus5MHz,		double& dACLR_RMSAvg_5MHz,		double& dACLR_RMSAvg_10MHz,
											double& dACLR_PeakMax_Minus10MHz,		double& dACLR_PeakMax_Minus5MHz,	double& dACLR_PeakMax_5MHz,		double& dACLR_PeakMax_10MHz,
											double& dACLR_RMSMax_Minus10MHz,		double& dACLR_RMSMax_Minus5MHz,		double& dACLR_RMSMax_5MHz,		double& dACLR_RMSMax_10MHz,
											double& dUEPower_RMSCurrent,			double& dOutOfTolerance
											) { return false; }
	virtual bool WCDMA_TurnOnOffACLRMearsurement(bool bOn) { return false; }

	virtual bool WCDMA_SetOBWNumberOfBurstsToMearsured(int iNumberOfBursts) { return false; }
	virtual bool WCDMA_SetOBWMeasuredRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode) { return false; }
	virtual bool WCDMA_SetOBWMeasuredType(std::string strType) { return false; }
	virtual bool WCDMA_SetOBWMearsuredLength(int iChipNumbers) { return false; }
	virtual bool WCDMA_SetDefaultOBWMeasurement(bool bOn) { return false; }
	virtual bool WCDMA_FetchOBWMearsurement(
											double& dCarrierPower_PeakCurrent,		double& dCarrierPower_RMSCurrent,
											double& dACLR_RMSCurrent_Minus10MHz,	double& dACLR_RMSCurrent_Minus5MHz,	double& dACLR_RMSCurrent_5MHz,	double& dACLR_RMSCurrent_10MHz,
											double& dACLR_RMSAvg_Minus10MHz,		double& dACLR_RMSAvg_Minus5MHz,		double& dACLR_RMSAvg_5MHz,		double& dACLR_RMSAvg_10MHz,
											double& dACLR_RMSMax_Minus10MHz,		double& dACLR_RMSMax_Minus5MHz,		double& dACLR_RMSMax_5MHz,		double& dACLR_RMSMax_10MHz,
											double& dOBW_Current,					double& dOBW_Avg,					double& dOBW_Max,				double& dOBW_Left,
											double& dOBW_Right,						double& dUEPower_RMSCurrent,		double& dOutOfTolerance
											) { return false; }

	virtual bool WCDMA_TurnOnOffMaxDynamicRangeForACLR_OBW(bool bOn) { return false; }

	virtual bool WCDMA_SetSEMNumberOfBurstsToMearsured(int iNumberOfBursts) { return false; }
	virtual bool WCDMA_SetSEMMearsuredRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode) { return false; }
	virtual bool WCDMA_SetSEMMeasuredType(std::string strType) { return false; }
	virtual bool WCDMA_SetSEMMearsuredLength(int iChipNumbers) { return false; }
	virtual bool WCDMA_SetDefaultSEMMeasurement(bool bOn) { return false; }
	virtual	bool WCDMA_SetSEMLineMaskPower(double dMaskPwr_12MHz,double dMaskPwr_8_5MHz,double dMaskPwr_7_5MHz,double dMaskPwr_4MHz,double dMaskPwr_3_845MHz,double dMaskPwr_2_515MHz) { return false; }
	virtual bool WCDMA_SetSEMMeasurementLowBoundaryCheck(bool bOn_Max,bool bOn_Average) { return false; }
	virtual bool WCDMA_FetchSEMMearsurement(
		std::string strMeasuredType,
		double& dReferencePower_Current,double& dReferencePower_Avg,double& dReferencePower_Max,
		double& dSEM_Minus_12_8p5MHz,	double& dSEM_Minus_8p5_7p5MHz,	double& dSEM_Minus_7p5_4MHz,double& dSEM_Minus_3p845_2p515MHz,
		double& dSEM_12_8p5MHz,			double& dSEM_8p5_7p5MHz,		double& dSEM_7p5_4MHz,		double& dSEM_3p845_2p515MHz,
		double& dUEPower_RMSCurrent,	double& dOutOfTolerance
		) { return false; }

	virtual bool WCDMA_SetBERMearsuredRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode) { return false; }
	virtual bool WCDMA_SetBERMearsuredDelayTime(int iChipNumbers) { return false; }
	virtual bool WCDMA_SetSameBlockSize_DownLink_UpLink(bool bSame) { return false; }
	virtual bool WCDMA_SetBERNumberOfMearsuredBlock(std::string strMeasuredType,int iNumOfBlocks) { return false; }
	virtual bool WCDMA_FetchBERMearsurement(
										double& dBER,
										double& dBLER,
										double &dDBLER,
										int& iLostTransportBlocks,
										int& iAlignmentDLUL,
										double& dFDR,
										double& dTFCI_ULFaults,
										double&	dPNDiscontinuity,
										int& iTransportBlockDelay) { return false; }

	virtual bool WCDMA_SetModulationMearsuredRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode) { return false; }
	virtual bool WCDMA_SetModulationMeasuredLength(int iChipNumbers) { return false; }
	virtual bool WCDMA_SetModulationNumberOfBurstsToMearsured(int iNumberOfBursts) { return false; }
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
										) { return false; }

	virtual bool WCDMA_SetBSOutputPower_NonSignaling(double dOutputPower) { return false; }
	
	// GSM Cal
	virtual bool GSM_FetchTxSweepRez(float *fTxPowerArray,float *fTxPhasArray,int iSizeofTotalTable) { return false; }
	virtual bool GSM_FetchIQRecorderPL(float *fTxPowerArray, float *fTxPhaseArray, int iSizeofTotalTable) { return false; }

	// FSP
	virtual bool SetResolutionBandwidth(double dRBWMHz);
	virtual bool SetVideoBandwidth(double dVBWMHz);
	virtual bool SetFrequencySpan(double dSpanMHz);
	virtual bool SwitchTraceMode(std::string strTraceMode);
	virtual bool SetDetector(std::string strDetector);
	virtual bool SwitcheMeasurementWindow(std::string strDiagramFullSize);
	virtual bool AdjustReferenceLevel();
	virtual bool SetAttenuation(bool bEnable);
	virtual bool SetSweepTime(int iSweepTime);
	virtual bool SetLimitLines(std::string strLineName, std::string strDomain, int iTrace, 
		std::string strXscalingMode, std::string strUnit, std::string strYscalingMode, 
		std::string strSpacing, int iMargin, int iThreshold,
		std::string strContData, std::string strUppData);
	virtual bool SpectrumMaskMeasurement(int iStatistics, std::string& strMeasured);
	virtual bool FrequencyErrorMeasurement(int iStatistics, std::string& strMeasured);
	virtual bool SetAttRefLevel(int iAttenuationManual, int iRefLevel);
	virtual bool SetChannelBandwidth(double dChannelBWMHz);
	virtual bool SetFrequencyError(double dPeakExcursion, int iTrace, double iFreqCounterRes);
	virtual bool SwitchChannelPower(std::string strChannelPower);
	virtual bool SetMaxLevel();
	virtual bool SetMaxLevelForFreErr();
	virtual bool ResetFSP();
	virtual bool SetBlankOFF();
	virtual bool MaxYOffset();

	double dRefLevel;
};

#endif // End of #ifndef _C_CFSP_H_