#ifndef _I_TEST_BOX_H_
#define _I_TEST_BOX_H_


#include "IGPIBDevice.h"

interface ITestBox : public IGPIBDevice
{
GP_DECLARE_DYNAMIC(ITestBox)


// Data members
public:
protected:
	static std::string g_strRFInputPort;
	static std::string g_strRFOutputPort;
	static int g_iMode;

private:


// Member functions
public:
	ITestBox() {}
	virtual ~ITestBox() {}

	static void SetRFInputPort(const char* szInputPort) { g_strRFInputPort = szInputPort; }
	static void SetRFOutputPort(const char* szOutputPort) { g_strRFOutputPort = szOutputPort; }
	static void SetCurrentMode(const int iCurrentMode) { g_iMode = iCurrentMode; }
	static std::string GetRFInputPort() { return g_strRFInputPort; }
	static std::string GetRFOutputPort() { return g_strRFOutputPort; }
	static int  GetCurrentMode () { return g_iMode; }

	// Initial functions
	virtual bool Initial() = 0;
	virtual bool Initial_GSM() = 0;
	virtual bool Initial_RF_Nonsignaling() = 0;
	virtual bool Initial_LTE() = 0;

	// Get Options of Test Device
    virtual bool GetOptions(std::string& strOptions) = 0 ;
    virtual bool GetOptionVersions(std::string& strOptionsVer) = 0 ;
    virtual bool GetDeviceIdentification(std::string& strIdentification) = 0 ;

	// Reset functions
	virtual bool Reset() = 0;
	virtual bool Preset() = 0;
    
    //
    virtual bool QueryRelIndicator(int iIndicator, std::string& strMsg) = 0 ;

	// Set cable loss functions
	virtual bool DeleteAllLossTable() = 0;
	virtual bool SetMeasureLoss(std::string strPort, std::string strFirmware, double dLoss) = 0;
	virtual bool SetMeasureLoss(std::string strFirmware, double dLoss) = 0;
	virtual bool SetGeneratorLoss(std::string strPort, std::string strFirmware, double dLoss) = 0;
	virtual bool SetGeneratorLoss(std::string strFirmware, double dLoss) = 0;
	virtual bool SetMeasureLossTable(std::string strPort, std::vector<unsigned int> vec_uiFreq, std::vector<double> vec_dLoss) = 0;
	virtual bool SetGeneratorLossTable(std::string strPort, std::vector<unsigned int> vec_uiFreq, std::vector<double> vec_dLoss) = 0;
	virtual bool SetMeasureLossTable(std::vector<unsigned int> vec_uiFreq, std::vector<double> vec_dLoss) = 0;
	virtual bool SetGeneratorLossTable(std::vector<unsigned int> vec_uiFreq, std::vector<double> vec_dLoss) = 0;

	// GPRF functions
	virtual bool SetGPRFGeneratorPort(std::string strPort) = 0;
	virtual bool SetGPRFMeasurementPort(std::string strPort) = 0;
	virtual bool SetCurrentMeasurementPort(std::string strFirmware) = 0;
	virtual bool SetGPRFGeneratorPort() = 0;
	virtual bool SetGPRFMeasurementPort() = 0;
	virtual bool SetGPRFFixedMeasureLoss(double dLoss) = 0;
	virtual bool SetGPRFFixedGeneratorLoss(double dLoss) = 0;
	virtual bool SetGPRFGeneratorBBMode(std::string strMode) = 0;
	virtual bool SetGPRFGeneratorPlayARB (std::string strArbFile) = 0;
	virtual bool SetGPRFGeneratorFrequency(unsigned int uiFrequency) = 0;
	virtual bool SetGPRFMeasurementFrequency(unsigned int uiFrequency) = 0;
	virtual bool SetGPRFGeneratorLevel(double dLevel) = 0;
    virtual bool SetGPRFGeneratorOnOff(bool bOn) = 0;
	virtual bool GetARBWaveform(bool bIsAutoSearch, std::string& strArbFile, std::string& strArbFilePath) = 0;

    // Measure
    virtual bool SetExpectedPower(std::string strFirmware, double dENPower, double dUMARgin) = 0 ;
    virtual bool SetMeasurementFrequency(std::string strFirmware, unsigned int uiFrequency) = 0 ;

	//Generator
	virtual bool SetGeneratorFrequency(std::string strFirmware, unsigned int uiFrequency) = 0 ;
	
	// GSM functions
	virtual bool SetGSMMeasurementBand (int band, bool bSig = true) = 0;

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
	virtual bool SetFreq_QryAtmpt(double d_Freq, std::string& strMeasured) = 0;
	virtual bool SetSweepCount(int iSwepCount) = 0;

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

	//
	virtual bool SetInputPort(int iMode, int iPort) = 0;
	virtual bool SetOutputPort(int iMode, int iPort) = 0;

protected:
private:
};


#endif // End of #ifndef _I_TEST_BOX_H_
