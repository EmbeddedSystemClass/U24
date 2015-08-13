#ifndef _C_CMU200_H_
#define _C_CMU200_H_


#include "../../CommonUtility/BaseObject/ITesterDevice.h"
#include "../../CommonUtility/CommonDefine.h"
#include <cctype>
#include <algorithm>
#include <limits.h>
#include <math.h>


#define _RF_NSIG					1
#define _GSM900MS_NSIG				2
#define _GSM900MS_SIG				3
#define _GSM1800MS_NSIG				4
#define _GSM1800MS_SIG				5
#define _GSM1900MS_NSIG				6
#define _GSM1900MS_SIG				7
#define _AUDIO_NSIG					8
#define _CMUBT						10
#define _CMUBT_SIG					11
#define _GSM850MS_NSIG				15
#define _GSM850MS_SIG				16
#define _WCDMA19UEFDD_SIG			17
#define _WCDMA19UEFDD_NSIG			23
#define _CDMA2KCellMS_NSig			25
#define _CDMA2KCellMS_Sig			26
#define _CDMA2KPCSMS_NSig			27
#define _CDMA2KPCSMS_Sig			28


#define IDSTR_RF_NSIG				"RF_NSig"
#define IDSTR_GSM900MS_NSIG			"GSM900MS_NSig"
#define IDSTR_GSM900MS_SIG			"GSM900MS_Sig"
#define IDSTR_GSM1800MS_NSIG		"GSM1800MS_NSig"
#define IDSTR_GSM1800MS_SIG			"GSM1800MS_Sig"
#define IDSTR_GSM1900MS_NSIG		"GSM1900MS_NSig"
#define IDSTR_GSM1900MS_SIG			"GSM1900MS_Sig"
#define IDSTR_AUDIO_NSIG			"AUDIO_NSig"
#define IDSTR_BT_SIG				"BLUETOOTH_Sig"
#define IDSTR_GSM850MS_NSIG			"GSM850MS_NSig"
#define IDSTR_GSM850MS_SIG			"GSM850MS_Sig"
#define IDSTR_WCDMA19UEFDD_SIG		"WCDMA19UEFDD_Sig"
#define IDSTR_WCDMA19UEFDD_NSIG		"WCDMA19UEFDD_NSig"
#define IDSTR_CDMA2KCellMS_NSig		"CDMA2KCellMS_NSig"
#define IDSTR_CDMA2KCellMS_Sig		"CDMA2KCellMS_Sig"
#define IDSTR_CDMA2KPCSMS_NSig		"CDMA2KPCSMS_NSig"
#define IDSTR_CDMA2KPCSMS_Sig		"CDMA2KPCSMS_Sig"


#define MAP_COMMAND					"SYSTem:REMote:ADDRess:SEC %d,\"%s\""


class CCMU200 : public ITesterDevice
{
RF_DECLARE_DYNCREATE(CCMU200)


// Data members
public:
protected:
private:


// Member functions
public:
	CCMU200() { GPIB_Write(std::string("TRACe:REMote:MODE:DISPlay ON")); }
	virtual ~CCMU200() {}

	// Initial functions
	virtual bool Initial();
	virtual bool Initial_RF_Nonsignaling();
	virtual bool Initial_GSM();
    virtual bool Initial_WCDMA();
    virtual bool SetMaxPower(double dMaxPower) ;

	// Get Options of Test Device
	virtual bool GetDeviceOptions(std::string& strOptions) ;

	// Reset functions
	virtual bool Reset();

	// Set RF port functions
	virtual bool SetInputPort(int iMode, int iPort);
	virtual bool SetOutputPort(int iMode, int iPort);
	virtual bool SetInputPort(int iMode);
	virtual bool SetOutputPort(int iMode);
	virtual bool SetInputPort();
	virtual bool SetOutputPort();

	// Set cable loss functions
	virtual bool SetInputLoss(int iMode, int iPort, double dLoss);
	virtual bool SetOutputLoss(int iMode, int iPort, double dLoss);
	virtual bool SetInputLoss(int iMode, double dLoss);
	virtual bool SetOutputLoss(int iMode, double dLoss);
	virtual bool SetInputLoss(double dLoss);
	virtual bool SetOutputLoss(double dLoss);
	
	virtual bool TurnOnOffLossTable(bool bOnOfff);
	virtual bool SetLossTable(int iNumOfFreqs,double *dFreqs,double *dLoss_RF1,double *dLoss_RF2,double *dLoss_RF3,double *dLoss_RF4);

	// Set signal on/off
	virtual bool SetSignalOnOff(int iMode, bool bOn);
	virtual bool SetSignalOnOff(bool bOn);

	// RF analyzer functions
	virtual bool RF_InitialAnalyzer();
	virtual bool RF_SetAnalyzerFreq(double dFreqMHz) ;
    virtual bool RF_SetGeneratorFreq(double dFreqMHz) ;
    virtual bool RF_SetGeneratorPowerLevel(double dLevel) ;
    virtual bool RF_TurnOnOffGernerator(bool bOn) ;
	virtual bool RF_NPowerMeasurement(int iStatistics, std::string& strMeasured);
	virtual bool RF_AnalyzerMeasurement(std::string& strMeasured);
	virtual bool RF_TurnOnOffAuxTxGenerator(bool bOn);
	virtual bool RF_TurnOnOffLowSpurModeForGenerator(bool bOn);
	virtual bool RF_SetModulationMode(std::string strMode);
	virtual bool RF_SetTriggerDelay(int iSlotDelay, int iDelayOffset);
	virtual bool RF_SetTriggerSourceType(std::string strType);
	virtual bool RF_SetIFPowerConfig(double dThreshold, std::string strSlope);
	virtual bool RF_SetAMModulationRation(int iPercent) ;
	virtual bool RF_SetAMModulationFrequency(int iHz) ;

    // GSM signaling functions
    virtual bool Initial_GSM_Param() ;
    virtual bool GSM_SetMode(int iBand, int iMode) ;
    virtual bool GSM_SetBandIndicator(int iBand) ;
    virtual bool GSM_SetCCH(int iChannel) ;
    virtual bool GSM_SetTCH(int iChannel) ;
    virtual bool GSM_SetTCH_Handover(int iChannel) ;
    virtual bool GSM_SetTCHTimeSlot(int iSlot) ;
    virtual bool GSM_SetTCHTimeSlotLevel_StartCall(float fLevel) ;
    virtual bool GSM_SetTCHTimeSlotLevel_Handover (float fLevel) ;
    virtual bool GSM_SetTCHTimeSlotPCL(int iPCL) ;
    virtual bool GSM_SetTCHTimeSlotPCL_Handover(int iPCL) ;
    virtual bool GSM_MTCall() ;
    virtual bool GSM_Handover() ;
    virtual bool GSM_CheckCallStatusCED() ;
    virtual bool GSM_CheckCallStatusCEST() ;
	virtual bool GSM_QuerryCampStatus();
	virtual bool GSM_MT_Call();
    virtual int  GSM_QueryBand() ;
    virtual int  GSM_QueryChannel() ;

    // GSM signaling Test functions
	virtual bool GSM_FetchBERRSSI(std::string& strMeasured);
    virtual bool GSM_SetMSMaxPower(float fInputLevel) ;
    virtual bool GSM_SetHANDoverTarget(int iTargetBand) ;
    virtual bool GSM_FetchRXQualityBER(std::string& strMeasured, int iBursts) ;
    virtual bool GSM_FetchRXQualityRFER(std::string& strMeasured, int iBursts) ;
    virtual bool GSM_FetchGMSKPower(std::string& strMeasured, int iBursts) ;
    virtual bool GSM_FetchGMSKPhaseError(std::string& strMeasured, int iBursts) ;
    virtual bool GSM_FetchModulationSpectrum(std::string& strMeasured, int iBursts) ;
    virtual bool GSM_SampleModulationSpectrum(float *out_pfPowerMOD) ;
    virtual bool GSM_FetchPowerVsTimeMask(std::string& strMeasured, int iBursts) ;
    virtual bool GSM_PowerVsTimeMaskPaser(std::vector<std::string> vData, int iArea, double fPowerUpper, std::string& strMeasured) ;
    virtual bool GSM_FetchSwitchingSpectrum(std::string& strMeasured, int iBursts) ;

    // GPRS signaling functions
    virtual bool GPRS_NeworkSelect() ;
    virtual bool GPRS_SetCS(int iCS) ;
    virtual bool GPRS_SetCS_Handover(int iCS) ;

    // EDGE signaling functions
    virtual bool EDGE_NeworkSelect() ;
    virtual bool EDGE_SetSignalOnOff(bool bOn) ;
    virtual bool Initial_EDGE_Param() ;
    virtual bool EDGE_SetTCH(int iChannel) ;
    virtual bool EDGE_SetTCH_Handover(int iChannel) ;
    virtual bool EDGE_SetTCHMainTimeslot(int iSlot) ;
    virtual bool EDGE_SetBCCHAbsLevel(float fLevel) ;
    virtual bool EDGE_SetTCHLevel(float fLevel) ;
    virtual bool EDGE_SetTimeslotConfig_Downlink(std::string strEnable, std::string strLevel) ;
    virtual bool EDGE_SetTimeslotConfig_Uplink(std::string strEnable, std::string strLevel) ;
    virtual bool EDGE_SetTimeslotConfig_ULDL_Handover(int iMainSlot,
        std::string strDLEnable, std::string strDLLevel,
        std::string strULEnable, std::string strULGamma) ;
    virtual bool EDGE_SetTCHPzero(int iPzero) ;
    virtual bool EDGE_ConnectRES() ;
    virtual bool EDGE_ConnectTestModeA() ;
    virtual bool EDGE_ConnectBLER() ;
    virtual bool EDGE_Handover() ;
    virtual bool EDGE_DisconnectMobile() ;
    virtual bool EDGE_QueryAttachStatus() ;
    virtual bool EDGE_QueryService(std::string& strService) ;
    virtual bool EDGE_CheckCallStatus() ;
    virtual bool EDGE_SetMCS(int iMCS) ;
    virtual bool EDGE_SetMCS_Handover(int iMCS) ;


    // EDGE signaling Test functions
    virtual bool EDGE_FetchRXQualityBLER(std::string& strMeasured, int iBursts) ;
    virtual bool EDGE_FetchEPSKPower(std::string& strMeasured, int iBursts) ;
    virtual bool EDGE_FetchEVMEPSKPhaseError(std::string& strMeasured, int iBursts) ;
    virtual bool EDGE_FetchPowerVsTimeMask(std::string& strMeasured, int iBursts) ;
    virtual bool EDGE_PowerVsTimeMaskPaser(std::vector<std::string> vData, int iArea, double fPowerUpper, std::string& strMeasured) ;
    virtual bool EDGE_FetchPowerVsTimeMask_MultiSlot(std::string& strMeasured, int iBursts, int iSlotAmount) ;


    // WCDMA signaling functions
    virtual bool Initial_WCDMA_Online();
    virtual bool WCDMA_SetBand(int iBand);
    virtual bool WCDMA_SetDownlinkChannel(int iChannel);
    virtual bool WCDMA_SetDCHType(std::string strType);
    virtual bool WCDMA_SetBSOutputPower(double dPower);
    virtual bool WCDMA_SetMaxMSPower(double dPower);
    virtual bool WCDMA_SetMSTargetPower(double dPower);
    virtual bool WCDMA_SetTPCPattern(std::string strPattern);
    virtual bool WCDMA_SetTPCPatternType(std::string strType);
	virtual bool WCDMA_SetAuthentication(bool bOnOff);
    virtual bool WCDMA_SetSecurityKey(std::string strKey1, std::string strKey2);
    virtual bool WCDMA_QuerryCampStatus();
    virtual bool WCDMA_MTCall();
    virtual bool WCDMA_SetBandAndDownlinkChannel(int iBand, int iChannel);
    virtual bool WCDMA_QuerryCallStatus();
    virtual bool WCDMA_InitMaxPowerMeasurement();
    virtual bool WCDMA_FetchMaxPower(std::string& strMeasured);
    virtual bool WCDMA_InitMinPowerMeasurement();
    virtual bool WCDMA_FetchMinPower(std::string& strMeasured);
    virtual bool WCDMA_InitModulationMeasurement();
    virtual bool WCDMA_FetchModulation(std::string& strMeasured);
    virtual bool WCDMA_InitACLR_OBW_Measurement();
    virtual bool WCDMA_FetchACLR_OBW(std::string& strMeasured);
    virtual bool WCDMA_InitSEMMeasurement();
    virtual bool WCDMA_FetchSEM(std::string& strMeasured);
    virtual bool WCDMA_InitILPC();
    virtual bool WCDMA_FetchILPC(std::string strPattern, std::string& strMeasured);
    virtual bool WCDMA_InitOpenLoop_OffPower(double dBSPower, double dULInterference, double dConstantValue, double dPCPICHPower);
    virtual bool WCDMA_FetchOpenLoop_OffPower(std::string& strMeasured);
    virtual bool WCDMA_EndCall();
    virtual bool WCDMA_InitBERMMeasurement();
    virtual bool WCDMA_FetchBERMMeasurement(std::string& strMeasured);
	virtual bool WCDMA_FetchRSCPMeasurement(std::string& strMeasured);
	virtual bool WCDMA_SetHANDoverTarget();

    // HSDPA signaling functions
    virtual bool HSDPA_QuerryCampStatus();
    virtual bool HSDPA_QuerryCallStatus();
    virtual bool HSDPA_MTCall();
    virtual bool HSDPA_EndCall();
    virtual bool HSDPA_SetGainFactor(int iBetaC, int iBetaD, int iDeltaACK = 5, int iDeltaNACK = 5, int iDeltaCQI = 2);


    // WCDMA Cal
	virtual bool WCDMA_SetMode(bool bSignalling);
	virtual bool WCDMA_SetGeneratorPower(double dOutputPower);
	
	virtual bool WCDMA_SetPowerVsSlotMeasuredMode(std::string strMode);
	virtual bool WCDMA_SetPowerVsSlotMeasuredConfigure(double dPowerMeasuredWidth,int iStepCount,int iStepWidth,int iStepDelay);
	virtual bool WCDMA_SetPowerVsSlotMeasuredRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode);
	virtual bool WCDMA_InitiatePowerVsSlotMeasurment();
	virtual bool WCDMA_FetchPowerVsSlotMeasurment(double* dMearsurment);
	
	virtual bool WCDMA_SetFastPowerMearsuredConfig(std::string strRepetition,std::string strStopCondition,std::string strStepMode);
	virtual bool WCDMA_SetFastPowerMearsuredConfig(std::string strStatics,std::string strRepetition,std::string strStopCondition,std::string strStepMode);
	virtual bool WCDMA_FetchFastPowerMeasurement(double& dAvgPwr_Current,double& dMinPwr_Current,double& dMaxPwr_Current,double& dAvgPwr_LastCycle,double& dMinPwr_Entire,double& dMaxPwr_Entire);
	
	virtual bool WCDMA_SetSmartAlignmentRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode);
	virtual bool WCDMA_SetSmartAlignmentConfig(int iFilterType,int iUnitType,double dStepWidth);
	virtual bool WCDMA_SetSmartAlignmentConfigOfPowers(int iNumOfPower,double *dOutputPower,double *dMaxLevel);
	virtual bool WCDMA_SetSmartAlignmentConfigOfFreqs(int iNumOfFreq,double *dGenFreq,double *dAnalyzerFreq);
	virtual bool WCDMA_InitiateSmartAlignmentMeasurement();
	virtual bool WCDMA_FetchSmartAlignmentMeasurement(double* dMeasurements);

	// WCDMA FTM Test
	virtual bool WCDMA_ReferenceChannelType(std::string strType);
	virtual bool WCDMA_SetScramblingCode(int iPrimaryScramblingCode,int iSecondaryScramblingCode);
	virtual bool WCDMA_SetBSSynchronizationChannelLevel(double dPCPICH_Level,double dPSCH_Level,double dSSCH_Level,double SCPICH_Level);
	virtual bool WCDMA_SetBSPhysicalChannelLevel(double dP_CPICH,double dP_SCH,double dS_SCH,double dP_CCPCH,double dPICH,double dDPDCH,double dPowerOffset);
	virtual bool WCDMA_SetBSDedicatedBroadcastChannelCode(int iDPDCHCode,int iPICHCode);
	virtual bool WCDMA_SetUEScramblingCode(int iSramblingCode);
	virtual bool WCDMA_SetBS_TPCControlType(int iPatternSetNumber,std::string strType);
	virtual bool WCDMA_SetBS_TPC_PatternSet(int iPatternSetNumber);
	virtual bool WCDMA_SetBS_TPC_PatternSet(std::string strPatternSet);
	virtual bool WCDMA_SetBS_TPC_AlgorithmMode(int iAlg);
	virtual bool WCDMA_TurnBS_TPC_PatternOn();
	virtual bool WCDMA_TurnOnILPC_Measurement(bool bOn);

	virtual bool WCDMA_FetchILPCMearsurement(std::string strILPCMode,double dMaxInputPower,double* dPowerVsSlotArray);

	virtual bool WCDMA_SetACLRNumberOfBurstsToMearsured(int iNumberOfBursts);
	virtual bool WCDMA_SetACLRMearsuredRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode);
	virtual bool WCDMA_SetACLRMearsuredLength(int iChipNumbers);
	virtual bool WCDMA_FetchACLRMearsurement(
											double& dCarrierPower_PeakCurrent,	double& dCarrierPower_RMSCurrent,
											double& dACLR_PeakCurrent_Minus10MHz,	double& dACLR_PeakCurrent_Minus5MHz,double& dACLR_PeakCurrent_5MHz,	double& dACLR_PeakCurrent_10MHz,
											double& dACLR_RMSCurrent_Minus10MHz,	double& dACLR_RMSCurrent_Minus5MHz,	double& dACLR_RMSCurrent_5MHz,	double& dACLR_RMSCurrent_10MHz,
											double& dACLR_RMSAvg_Minus10MHz,		double& dACLR_RMSAvg_Minus5MHz,		double& dACLR_RMSAvg_5MHz,		double& dACLR_RMSAvg_10MHz,
											double& dACLR_PeakMax_Minus10MHz,		double& dACLR_PeakMax_Minus5MHz,	double& dACLR_PeakMax_5MHz,		double& dACLR_PeakMax_10MHz,
											double& dACLR_RMSMax_Minus10MHz,		double& dACLR_RMSMax_Minus5MHz,		double& dACLR_RMSMax_5MHz,		double& dACLR_RMSMax_10MHz,
											double& dUEPower_RMSCurrent,			double& dOutOfTolerance
											);
	virtual bool WCDMA_TurnOnOffACLRMearsurement(bool bOn);

	virtual bool WCDMA_SetOBWNumberOfBurstsToMearsured(int iNumberOfBursts);
	virtual bool WCDMA_SetOBWMeasuredRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode);
	virtual bool WCDMA_SetOBWMeasuredType(std::string strType);
	virtual bool WCDMA_SetOBWMearsuredLength(int iChipNumbers);
	virtual bool WCDMA_SetDefaultOBWMeasurement(bool bOn);
	virtual bool WCDMA_FetchOBWMearsurement(
											double& dCarrierPower_PeakCurrent,		double& dCarrierPower_RMSCurrent,
											double& dACLR_RMSCurrent_Minus10MHz,	double& dACLR_RMSCurrent_Minus5MHz,	double& dACLR_RMSCurrent_5MHz,	double& dACLR_RMSCurrent_10MHz,
											double& dACLR_RMSAvg_Minus10MHz,		double& dACLR_RMSAvg_Minus5MHz,		double& dACLR_RMSAvg_5MHz,		double& dACLR_RMSAvg_10MHz,
											double& dACLR_RMSMax_Minus10MHz,		double& dACLR_RMSMax_Minus5MHz,		double& dACLR_RMSMax_5MHz,		double& dACLR_RMSMax_10MHz,
											double& dOBW_Current,					double& dOBW_Avg,					double& dOBW_Max,				double& dOBW_Left,
											double& dOBW_Right,						double& dUEPower_RMSCurrent,		double& dOutOfTolerance
											);

	virtual bool WCDMA_TurnOnOffMaxDynamicRangeForACLR_OBW(bool bOn);

	virtual bool WCDMA_SetSEMNumberOfBurstsToMearsured(int iNumberOfBursts);
	virtual bool WCDMA_SetSEMMearsuredRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode);
	virtual bool WCDMA_SetSEMMeasuredType(std::string strType);
	virtual bool WCDMA_SetSEMMearsuredLength(int iChipNumbers);
	virtual bool WCDMA_SetDefaultSEMMeasurement(bool bOn);
	virtual	bool WCDMA_SetSEMLineMaskPower(double dMaskPwr_12MHz,double dMaskPwr_8_5MHz,double dMaskPwr_7_5MHz,double dMaskPwr_4MHz,double dMaskPwr_3_845MHz,double dMaskPwr_2_515MHz);
	virtual bool WCDMA_SetSEMMeasurementLowBoundaryCheck(bool bOn_Max,bool bOn_Average);
	virtual bool WCDMA_FetchSEMMearsurement(
		std::string strMeasuredType,
		double& dReferencePower_Current,double& dReferencePower_Avg,double& dReferencePower_Max,
		double& dSEM_Minus_12_8p5MHz,	double& dSEM_Minus_8p5_7p5MHz,	double& dSEM_Minus_7p5_4MHz,double& dSEM_Minus_3p845_2p515MHz,
		double& dSEM_12_8p5MHz,			double& dSEM_8p5_7p5MHz,		double& dSEM_7p5_4MHz,		double& dSEM_3p845_2p515MHz,
		double& dUEPower_RMSCurrent,	double& dOutOfTolerance
		);

	virtual bool WCDMA_SetBERMearsuredRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode);
	virtual bool WCDMA_SetBERMearsuredDelayTime(int iChipNumbers);
	virtual bool WCDMA_SetSameBlockSize_DownLink_UpLink(bool bSame);
	virtual bool WCDMA_SetBERNumberOfMearsuredBlock(std::string strMeasuredType,int iNumOfBlocks);
	virtual bool WCDMA_FetchBERMearsurement(
										double& dBER,
										double& dBLER,
										double &dDBLER,
										int& iLostTransportBlocks,
										int& iAlignmentDLUL,
										double& dFDR,
										double& dTFCI_ULFaults,
										double&	dPNDiscontinuity,
										int& iTransportBlockDelay);

	virtual bool WCDMA_SetModulationMearsuredRepetition(std::string strRepetition,std::string strStopCondition,std::string strStepMode);
	virtual bool WCDMA_SetModulationMeasuredLength(int iChipNumbers);
	virtual bool WCDMA_SetModulationNumberOfBurstsToMearsured(int iNumberOfBursts);
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
										);

	virtual bool WCDMA_SetBSOutputPower_NonSignaling(double dOutputPower);
	
	// GSM Cal
	virtual bool GSM_FetchTxSweepRez(float *fTxPowerArray,float *fTxPhasArray,int iSizeofTotalTable);
	virtual bool GSM_FetchIQRecorderPL(float *fTxPowerArray, float *fTxPhaseArray, int iSizeofTotalTable);
    
	// FSP
	virtual bool SetResolutionBandwidth(double dRBWMHz) { return false; }
	virtual bool SetVideoBandwidth(double dVBWMHz) { return false; }
	virtual bool SetFrequencySpan(double dSpanMHz) { return false; }
	virtual bool SwitchTraceMode(std::string strTraceMode) { return false; }
	virtual bool SetDetector(std::string strDetector) { return false; }
	virtual bool SwitcheMeasurementWindow(std::string strDiagramFullSize) { return false; }
	virtual bool AdjustReferenceLevel() { return false; }
	virtual bool SetAttenuation(bool bEnable) { return false; }
	virtual bool SetSweepTime(int iSweepTime) { return false; }
	virtual bool SetLimitLines(std::string strLineName, std::string strDomain, int iTrace, 
		std::string strXscalingMode, std::string strUnit, std::string strYscalingMode, 
		std::string strSpacing, int iMargin, int iThreshold,
		std::string strContData, std::string strUppData) { return false; }
	virtual bool SpectrumMaskMeasurement(int iStatistics, std::string& strMeasured) { return false; }
	virtual bool FrequencyErrorMeasurement(int iStatistics, std::string& strMeasured) { return false; }
	virtual bool SetAttRefLevel(int iAttenuationManual, int iRefLevel) { return false; }
	virtual bool SetChannelBandwidth(double dChannelBWMHz) { return false; }
	virtual bool SetFrequencyError(double dPeakExcursion, int iTrace, double iFreqCounterRes) { return false; }
	virtual bool SwitchChannelPower(std::string strChannelPower) { return false; }
	virtual bool SetMaxLevel() { return false; }

protected:
private:
};


#endif // End of #ifndef _C_CMU200_H_
