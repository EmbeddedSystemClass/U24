#ifndef _C_GSM_WIRELESS_FTM_MEASURELOOP_H_
#define _C_GSM_WIRELESS_FTM_MEASURELOOP_H_

#include "../../../CommonUtility/BaseObject/ITestProcessor.h"
#include "../../../CommonUtility/CommonUtil.h"

class CGSM_Wireless_FTM_MeasureLoop : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CGSM_Wireless_FTM_MeasureLoop)

//Data members
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
	std::string		m_strPicsMsg ;

	bool    m_bItemResult ;
	bool    m_bResult ;
	int     m_iBand ;
	int     m_iChannel ;
	bool	m_bLoopbackType0;

	std::string m_strTestList;
	std::vector<std::string> m_vTestList;
	std::vector<FPClassFunc> m_vTestFunction;
	std::vector<FPClassFunc> m_vInitDataFunction ;
	std::map<std::string, std::string> m_paramMap ;

	// 1. TxPower
	std::vector<std::string> m_vID_TxPower ;
	std::vector<std::string> m_vPCLTxPower ;
	std::vector<std::string> m_vLowerTxPower ;
	std::vector<std::string> m_vUpperTxPower ;
	int m_iBurstTxPower ;

// Member functions
public:
	CGSM_Wireless_FTM_MeasureLoop(){}
	~CGSM_Wireless_FTM_MeasureLoop(){}

	virtual bool	InitData(std::map<std::string, std::string>& paramMap);
	virtual bool	Run();

protected:
private:
	bool			InitFunction();
	bool			MainFunction();
};

#endif // End of #ifndef _C_GSM_WIRELESS_FTM_MEASURELOOP_H_