#ifndef _C_LEAKAGE_CURRENT_H_
#define _C_LEAKAGE_CURRENT_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CLeakageCurrent : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CLeakageCurrent)


// Data members
public:
protected:
private:
	double m_dLower, m_dUpper;

	int m_iWaitTimeForShutdown;
	int m_iWaitTimeForFetch;

	//Super4Relay* m_pRBCtrl;

	std::string m_strMsg;
	std::string m_strMeasured;
	std::string m_strErrCode;


// Member functions
public:
	CLeakageCurrent() {}
	~CLeakageCurrent() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_LEAKAGE_CURRENT_H_
