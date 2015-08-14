#ifndef _C_CMW_H_
#define _C_CMW_H_


#include <Shlwapi.h>

#include "BaseObject/ITestBox.h"

#include "Utils/CommonDefine.h"

#include "Utils/StringUtil.h"


class CCMW : public ITestBox
{
GP_DECLARE_DYNCREATE(CCMW)


// Data members
public:
protected:
private:


// Member functions
public:
	CCMW() {}
	virtual ~CCMW() {}

	// Initial functions
	virtual bool Initial();
	virtual bool Initial_GSM();
	virtual bool Initial_RF_Nonsignaling();
	virtual bool Initial_LTE();

	// Get Options of Test Device
	virtual bool GetOptions(std::string& strOptions);
    virtual bool GetOptionVersions(std::string& strOptionsVer) ;
    virtual bool GetDeviceIdentification(std::string& strIdentification) ;

	// Reset functions
	virtual bool Reset();
	virtual bool Preset();

    //
    virtual bool QueryRelIndicator(int iIndicator, std::string& strMsg) ;

	// Set cable loss functions
	virtual bool DeleteAllLossTable();
	virtual bool SetMeasureLoss(std::string strPort, std::string strFirmware, double dLoss);
	virtual bool SetMeasureLoss(std::string strFirmware, double dLoss);
	virtual bool SetGeneratorLoss(std::string strPort, std::string strFirmware, double dLoss);
	virtual bool SetGeneratorLoss(std::string strFirmware, double dLoss);
	virtual bool SetMeasureLossTable(std::string strPort, std::vector<unsigned int> vec_uiFreq, std::vector<double> vec_dLoss);
	virtual bool SetGeneratorLossTable(std::string strPort, std::vector<unsigned int> vec_uiFreq, std::vector<double> vec_dLoss);
	virtual bool SetMeasureLossTable(std::vector<unsigned int> vec_uiFreq, std::vector<double> vec_dLoss);
	virtual bool SetGeneratorLossTable(std::vector<unsigned int> vec_uiFreq, std::vector<double> vec_dLoss);

	// GPRF functions
	virtual bool SetGPRFGeneratorPort(std::string strPort);
	virtual bool SetGPRFMeasurementPort(std::string strPort);
    virtual bool SetCurrentMeasurementPort(std::string strFirmware);
	virtual bool SetGPRFGeneratorPort();
	virtual bool SetGPRFMeasurementPort();
	virtual bool SetGPRFFixedMeasureLoss(double dLoss);
	virtual bool SetGPRFFixedGeneratorLoss(double dLoss);
	virtual bool SetGPRFGeneratorBBMode(std::string strMode);
	virtual bool SetGPRFGeneratorPlayARB (std::string strArbFile);
	virtual bool SetGPRFGeneratorFrequency(unsigned int uiFrequency);
	virtual bool SetGPRFMeasurementFrequency(unsigned int uiFrequency);
	virtual bool SetGPRFGeneratorLevel(double dLevel);
	virtual bool SetGPRFGeneratorOnOff(bool bOn);
	virtual bool GetARBWaveform(bool bIsAutoSearch, std::string& strArbFile, std::string& strArbFilePath);

    // Measure
    virtual bool SetExpectedPower(std::string strFirmware, double dENPower, double dUMARgin) ;
    virtual bool SetMeasurementFrequency(std::string strFirmware, unsigned int uiFrequency) ;

	//Generator
	virtual bool SetGeneratorFrequency(std::string strFirmware, unsigned int uiFrequency) ;

	// GSM_Measurement functions
	virtual bool SetGSMMeasurementBand (int band, bool bSig=true);
protected:
private:

	// Set RF port functions
	virtual bool SetInputPort(std::string strFirmware, std::string strScenario, std::string strInputPort);
	virtual bool SetOutputPort(std::string strFirmware, std::string strScenario, std::string strInputPort);
	virtual bool SetInputPort(std::string strFirmware, std::string strScenario);
	virtual bool SetOutputPort(std::string strFirmware, std::string strScenario);
	virtual bool SetInputPort(std::string strFirmware);
	virtual bool SetOutputPort(std::string strFirmware);
	virtual bool SetInputPort(int iMode, int iPort1);
	virtual bool SetOutputPort(int iMode, int iPort);

	virtual bool DeactivateLossTable(std::string strPort);
	virtual bool DeactivateLossTable();
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
	virtual bool SetFreq_QryAtmpt(double d_Freq, std::string& strMeasured) { return false; }
	virtual bool SetSweepCount(int iSwepCount){return false;}

	// RF functions
	virtual bool RF_InitialAnalyzer() { return false; }
    virtual bool RF_SetAnalyzerFreq(double dFreq) { return false; }
    virtual bool RF_SetGeneratorFreq(double dFreqMHz) { return false; }
    virtual bool RF_SetGeneratorPowerLevel(double dLevel) { return false; }
    virtual bool RF_TurnOnOffGernerator(bool bOn) { return false; }
    virtual bool RF_NPowerMeasurement(int iStatistics, std::string& strMeasured) { return false; }
	virtual bool RF_AnalyzerMeasurement(std::string& strMeasured) { return false; }
	virtual bool RF_TurnOnOffAuxTxGenerator(bool bOn) { return false; }
	virtual bool RF_TurnOnOffLowSpurModeForGenerator(bool bOn) { return false; }
	virtual bool RF_SetModulationMode(std::string strMode) { return false; }
	virtual bool RF_SetTriggerSourceType(std::string strType) { return false; }
	virtual bool RF_SetTriggerDelay(int iSlotDelay,int iDelayOffset) { return false; }
	virtual bool RF_SetIFPowerConfig(double dThreshold,std::string strSlope) { return false; }
	virtual bool RF_SetAMModulationRation(int iPercent) { return false; }
	virtual bool RF_SetAMModulationFrequency(int iHz) { return false; }
};


#endif // End of #ifndef _C_CMW_H_
