#ifndef _C_FM_TEST_H_
#define _C_FM_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CFMtest : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CFMtest)

// Data members
public:
protected:
private:

	std::string     m_strErrCode ;
    std::string     m_strMsg ;
	std::string     m_strLower, m_strUpper, m_strMeasured;

	int						m_iOutput;
	int						m_iTestCHCount;
	int 					m_iTestCH;
	double					m_dLowerRSSI, m_dUpperRSSI;
	double					m_dRFLevel;

	int m_iWaitTimeForBoot;
	int m_iRetryConnectTimes;
	int m_iWaitTimeAfterConnect;
	int m_iEFsSyncRetryCount;

// Member functions
public:
	CFMtest() {}
	~CFMtest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_FM_TEST_H_
