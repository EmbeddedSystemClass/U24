#ifndef _C_GSM_ONLINE_MEASURE_LOOP_H_
#define _C_GSM_ONLINE_MEASURE_LOOP_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"
#include "../../../CommonUtility/CommonUtil.h"


class CGSM_Online_MeasureLoop : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CGSM_Online_MeasureLoop)

// Data members
public:
protected:
private:
    std::string     m_strBand ;
    std::string     m_strChannel ;
    std::string     m_strItem ;
    std::string     m_strUpper ;
    std::string     m_strLower ;
    std::string     m_strMeasured ;
    std::string     m_strUnit ;
    std::string     m_strMsg ;
    std::string     m_strErrCode ;
	bool    m_bItemResult ;
    bool    m_bResult ;
    int     m_iBand ;
    int     m_iChannel ;
	int		m_iSleepTime;

    std::string m_strTestList;
    std::vector<std::string> m_vTestList;
	std::vector<std::string> m_vChannelList;
    std::vector<FPClassFunc> m_vTestFunction;
    std::vector<FPClassFunc> m_vInitDataFunction ;
    std::map<std::string, std::string> m_paramMap ;

    // 1. TxPower
    std::vector<std::string> m_vID_TxPower ;
    std::vector<std::string> m_vPCLTxPower ;
    std::vector<std::string> m_vLowerTxPower ;
    std::vector<std::string> m_vUpperTxPower ;

    // 2. P/t
    std::vector<std::string> m_vID_PVT ;
    std::vector<std::string> m_vPCLPVT ;
    std::vector<std::string> m_vUpperAPVT ;
    std::vector<std::string> m_vUpperBPVT ;
    std::vector<std::string> m_vUpperCPVT ;
    int   m_iBurstPVT ;
    float m_fPowerLevelPVT ;

    // 3. Phase Error RMS
    std::vector<std::string> m_vID_PhErrRMS ;
    std::vector<std::string> m_vPCLPhErrRMS ;
    std::vector<std::string> m_vLowerPhErrRMS ;
    std::vector<std::string> m_vUpperPhErrRMS ;
    int    m_iBurstPhErrRMS ;
    float  m_fPowerLevelPhErrRMS ;

    // 4. Phase Error Peak
    std::vector<std::string> m_vID_PhErrPeak ;
    std::vector<std::string> m_vPCLPhErrPeak ;
    std::vector<std::string> m_vLowerPhErrPeak ;
    std::vector<std::string> m_vUpperPhErrPeak ;
    int    m_iBurstPhErrPeak ;
    float  m_fPowerLevelPhErrPeak ;

    // 5. Frequency Error
    std::vector<std::string> m_vID_FreqErr ;
    std::vector<std::string> m_vPCLFreqErr ;
    std::vector<std::string> m_vLowerFreqErr ;
    std::vector<std::string> m_vUpperFreqErr ;
    int   m_iBurstFreqErr ;
    float m_fPowerLevelFreqErr ;

    // 6. Modulation
    std::vector<std::string> m_vID_Mod ;
    std::vector<std::string> m_vPCLMod ;
    std::vector<std::string> m_vOffsetMod ;
    std::vector<std::string> m_vUpperMod ;
    int  m_iBurstMod ;
    float m_fPowerLevelMod ;

    // 7. Switch
    std::vector<std::string> m_vPCLSwitch ;
    std::vector<std::string> m_vOffsetSwitch ;
    std::vector<std::string> m_vUpperSwitch ;
    int  m_iBurstSwitch ;
    float m_fPowerLevelSwitch ;

    // 8. RxLevel
    std::vector<std::string> m_vID_RxLv ;
    std::vector<std::string> m_vLevelRxLv ;
    std::vector<std::string> m_vUpperRxLv ;
    std::vector<std::string> m_vLowerRxLv ;
    int m_iPCLRxLv ;

    // 9. RFER
    std::vector<std::string> m_vID_RFER ;
    double m_dLowerRBER, m_dUpperRBER ;
    double m_dLowerFER, m_dUpperFER ;
    int    m_iBurstRFER ;
    int    m_iPCLRFER ;
    float  m_fPowerLevelRFER ;

    //11. Sensitivity
    std::vector<std::string> m_vID_Sensitivity ;
    int    m_iPCLSensitivity ;
    double m_dLowerSensitivity, m_dUpperSensitivity ;
    int    m_iBurstSensitivity ;
    float  m_fPowerLevelSensitivity_Start ;

    //12. BER
    std::vector<std::string> m_vID_BER ;
    double m_dLowerBER, m_dUpperBER ;
    int    m_iBurstBER ;
    int    m_iPCLBER ;
	int    m_iRetryTimes;
    float  m_fPowerLevelBER_Low,m_fPowerLevelBER_Mid,m_fPowerLevelBER_High,m_fPowerLevelBER ;
	float  m_fPowerLevelStep;



// Member functions
public:
	CGSM_Online_MeasureLoop() ;
	~CGSM_Online_MeasureLoop() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool Run();


protected:
private:
    bool InitData_TxPower();
    bool InitData_PowerVsTimeMask();
    bool InitData_PhaseErrorRMS();
    bool InitData_PhaseErrorPeak();
    bool InitData_FrequencyError();
    bool InitData_Modulation();
    bool InitData_Switching();
    bool InitData_RxLevel();
    bool InitData_RFER();
    bool InitData_Sensitivity();
    bool InitData_BER();
	bool InitData_CPK();

    bool TxPower() ;
    bool PowerVsTimeMask() ;
    bool PhaseErrorRMS() ;
    bool PhaseErrorPeak() ;
    bool FrequencyError() ;
    bool Modulation() ;
    bool Switching() ;
    bool RxLevel() ;
    bool RFER();
    bool Sensitivity() ;
    bool BER();
	bool CPK();
	bool HandOverChannel(int i_channel);
	bool GSM_StartCall();
};


#endif // End of #ifndef _C_GSM_ONLINE_MEASURE_LOOP_H_
