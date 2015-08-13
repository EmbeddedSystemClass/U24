#ifndef _C_WCDMA_ONLINE_MEASURE_LOOP_II_H_
#define _C_WCDMA_ONLINE_MEASURE_LOOP_II_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CWCDMA_Online_MeasureLoop_II : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CWCDMA_Online_MeasureLoop_II)


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
	std::string     m_strTestList;
	std::vector<std::string> m_vTestList;
	std::vector<std::string> m_vChannelList;
	std::vector<FPClassFunc> m_vTestFunction;
	std::vector<FPClassFunc> m_vInitDataFunction ;
	std::map<std::string, std::string> m_paramMap ;

    bool    m_bTestResult ;
	bool	m_bResult;
	int     m_iBand, m_iChannel;

	double m_dBSOutputPower;
	double m_dMaxMSPower;
	double m_dMSTargetPower;

	// 1.  Maximum Power
	double m_dLowerMaxPower, m_dUpperMaxPower;

	// 2.  Minimum Power
	double m_dLowerMinPower, m_dUpperMinPower;

	// 3.  EVM
	std::vector<std::string> m_vMSTargetPower_EVM;
	double m_dLowerEVM, m_dUpperEVM;

	// 4.  Frequency Error
	double m_dBSOutputPower_FE;
	double m_dMSTargetPower_FE;
	double m_dLowerFE, m_dUpperFE;

	// 5.  Peak Code Domain Error
	std::vector<std::string> m_vMSTargetPower_PCDE;
	double m_dLowerPCDE, m_dUpperPCDE;

	// 6.  ACLR
	double m_dMSTargetPower_ACLR;
	double m_dLowerACLR_5MHz, m_dUpperACLR_5MHz;
	double m_dLowerACLR_10MHz, m_dUpperACLR_10MHz;

	// 7.  OBW
	double m_dMSTargetPower_OBW;
	double m_dLowerOBW, m_dUpperOBW;

	// 8.  SEM
	double m_dMSTargetPower_SEM;
	std::vector<std::string> m_vFreqList_SEM;
	std::vector<std::string> m_vRange_SEM;

	// 9.  InnerLoop
	double m_dMaxMSPower_ILPC;

	// 10. Reference BER
	double m_dBSOutputPower_RefBER;
	double m_dMSTargetPower_RefBER;
	double m_dLowerRefBER, m_dUpperRefBER;
	int m_iRetryTimes;

	// 11. Maximum BER
	double m_dBSOutputPower_MaxBER;
	double m_dMSTargetPower_MaxBER;
	double m_dLowerMaxBER, m_dUpperMaxBER;

	// 12. Minimum BER
	double m_dBSOutputPower_MinBER;
	double m_dMSTargetPower_MinBER;
	double m_dLowerMinBER, m_dUpperMinBER;

	// 13. OffPower
	double m_dBSOutputPower_Off;
	double m_dPCPICHPower_Off;
	double m_dULInterference_Off;
	double m_dConstantValue_Off;
	int m_iWaitTimeForFetch_Off;
	double m_dLowerOff, m_dUpperOff;

	// 14. OpenLoop
	double m_dMaxMSPower_OL;
	std::vector<std::string> m_vBSOutputPower_OL;
	std::vector<std::string> m_vPCPICHPower_OL;
	std::vector<std::string> m_vULInterference_OL;
	std::vector<std::string> m_vConstantValue_OL;
	int m_iWaitTimeForFetch_OL;
	double m_dLowerOL, m_dUpperOL;

	//15. CPK
	double m_fPowerLevelStep;
// Member functions
public:
	CWCDMA_Online_MeasureLoop_II();
	~CWCDMA_Online_MeasureLoop_II() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool Run();

protected:
private:
	bool CheckCallStatus();
	bool WCDMA_StartCall();

	bool InitData_MaximunPower();
	bool InitData_MinimunPower();
	bool InitData_ErrorVectorMagnitude();
	bool InitData_FrequencyError();
	bool InitData_PeakCodeDomainError();
	bool InitData_AdjacentChannelLeakageRatio();
	bool InitData_OccupiedBandwidth();
	bool InitData_SpectrumEmissionMask();
	bool InitData_InnerLoopPowerControl();
	bool InitData_RefBER();
	bool InitData_MaxBER();
	bool InitData_MinBER();
	bool InitData_OffPower();
	bool InitData_OpenLoopPowerControl();
	bool InitData_CPK();

	bool MaximunPower();
	bool MinimunPower();
	bool ErrorVectorMagnitude();
	bool FrequencyError();
	bool PeakCodeDomainError();
	bool AdjacentChannelLeakageRatio();
	bool OccupiedBandwidth();
	bool SpectrumEmissionMask();
	bool InnerLoopPowerControl();
	bool RefBER();
	bool MaxBER();
	bool MinBER();
	bool OffPower();
	bool OpenLoopPowerControl();
	bool CPK();

	bool FetchOff_OL(std::vector<std::string>& vResult);

	bool HandoverUMTS();
};


#endif // End of #ifndef _C_WCDMA_ONLINE_MEASURE_LOOP_II_H_
