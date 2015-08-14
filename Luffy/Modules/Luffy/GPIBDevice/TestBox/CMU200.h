#ifndef _C_CMU200_H_
#define _C_CMU200_H_

#include <algorithm>

#include "BaseObject/ITestBox.h"

#include "Utils/CommonDefine.h"

#include "Utils/StringUtil.h"
#include "Utils/GSMUtil.h"


class CCMU200 : public ITestBox
{
GP_DECLARE_DYNCREATE(CCMU200)


// Data members
public:
protected:
private:


// Member functions
public:
	CCMU200() {}
	virtual ~CCMU200() {}

	// Initial functions
	virtual bool Initial();
	virtual bool Initial_GSM();
	virtual bool Initial_RF_Nonsignaling();
	virtual bool Initial_LTE()  {return false; } ;

	// Get Options of Test Device
	virtual bool GetOptions(std::string& strOptions);
    virtual bool GetOptionVersions(std::string& strOptionsVer) {return false; } ;
    virtual bool GetDeviceIdentification(std::string& strIdentification) {return false; } ;

	// Reset functions
	virtual bool Reset();
	virtual bool Preset();

    //
    virtual bool QueryRelIndicator(int iIndicator, std::string& strMsg) { return false; }

	// Set cable loss functions
	virtual bool DeleteAllLossTable() {return false;}
	virtual bool SetMeasureLoss(std::string strPort, std::string strFirmware, double dLoss) {return false;}
	virtual bool SetMeasureLoss(std::string strFirmware, double dLoss) {return false;}
	virtual bool SetGeneratorLoss(std::string strPort, std::string strFirmware, double dLoss) {return false;}
	virtual bool SetGeneratorLoss(std::string strFirmware, double dLoss) {return false;}
	virtual bool SetMeasureLossTable(std::string strPort, std::vector<unsigned int> vec_uiFreq, std::vector<double> vec_dLoss) {return false;}
	virtual bool SetGeneratorLossTable(std::string strPort, std::vector<unsigned int> vec_uiFreq, std::vector<double> vec_dLoss) {return false;}
	virtual bool SetMeasureLossTable(std::vector<unsigned int> vec_uiFreq, std::vector<double> vec_dLoss) {return false;}
	virtual bool SetGeneratorLossTable(std::vector<unsigned int> vec_uiFreq, std::vector<double> vec_dLoss) {return false;}

	// GPRF functions
    virtual bool SetGPRFGeneratorPort(std::string strPort);
    virtual bool SetGPRFMeasurementPort(std::string strPort);
    virtual bool SetCurrentMeasurementPort(std::string strFirmware) {return false;};
	virtual bool SetGPRFGeneratorPort();
	virtual bool SetGPRFMeasurementPort();
	virtual bool SetGPRFFixedMeasureLoss(double dLoss);
	virtual bool SetGPRFFixedGeneratorLoss(double dLoss);
	virtual bool SetGPRFGeneratorBBMode(std::string strMode) {return false;}
	virtual bool SetGPRFGeneratorPlayARB (std::string strArbFile) {return false;}
	virtual bool SetGPRFGeneratorFrequency(unsigned int uiFrequency);
	virtual bool SetGPRFMeasurementFrequency(unsigned int uiFrequency);
	virtual bool SetGPRFGeneratorLevel(double dLevel);
	virtual bool SetGPRFGeneratorOnOff(bool bOn);
	virtual bool GetARBWaveform(bool bIsAutoSearch, std::string& strArbFile, std::string& strArbFilePath) {return false;}

    // Measurement
    virtual bool SetExpectedPower(std::string strFirmware, double dENPower, double dUMARgin) { return false; }
    virtual bool SetMeasurementFrequency(std::string strFirmware, unsigned int uiFrequency) { return false; }
	
	//Generator
	virtual bool SetGeneratorFrequency(std::string strFirmware, unsigned int uiFrequency) { return false; }
	
	// GSM_Measurement functions
	virtual bool SetGSMMeasurementBand (int band, bool bSig = true);

	virtual bool SetInputPort(int iMode = -1, int iPort = -1);
	virtual bool SetOutputPort(int iMode = -1, int iPort = -1);

protected:
private:
	// Set RF port functions
	virtual bool SetInputPort(std::string strFirmware, std::string strScenario, std::string strInputPort) {return false;}
	virtual bool SetOutputPort(std::string strFirmware, std::string strScenario, std::string strInputPort) {return false;}
	virtual bool SetInputPort(std::string strFirmware, std::string strScenario) {return false;}
	virtual bool SetOutputPort(std::string strFirmware, std::string strScenario) {return false;}
	virtual bool SetInputPort(std::string strFirmware) {return false;}
	virtual bool SetOutputPort(std::string strFirmware) {return false;}

	

	

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


	
	// RF analyzer functions
	virtual bool RF_InitialAnalyzer();
	virtual bool RF_SetAnalyzerFreq(double dFreqMHz);
    virtual bool RF_SetGeneratorFreq(double dFreqMHz);
    virtual bool RF_SetGeneratorPowerLevel(double dLevel);
    virtual bool RF_TurnOnOffGernerator(bool bOn);
	virtual bool RF_NPowerMeasurement(int iStatistics, std::string& strMeasured);// { return false; }
	virtual bool RF_AnalyzerMeasurement(std::string& strMeasured) { return false; }
	virtual bool RF_TurnOnOffAuxTxGenerator(bool bOn) { return false; }
	virtual bool RF_TurnOnOffLowSpurModeForGenerator(bool bOn) { return false; }
	virtual bool RF_SetModulationMode(std::string strMode);
	virtual bool RF_SetTriggerDelay(int iSlotDelay, int iDelayOffset) { return false; }
	virtual bool RF_SetTriggerSourceType(std::string strType) { return false; }
	virtual bool RF_SetIFPowerConfig(double dThreshold, std::string strSlope) { return false; }
	virtual bool RF_SetAMModulationRation(int iPercent)  { return false; }
	virtual bool RF_SetAMModulationFrequency(int iHz)  { return false; }

};


#endif // End of #ifndef _C_CMU200_H_
