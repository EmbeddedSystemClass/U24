#ifndef _C_SYNC_COMBO_H_
#define _C_SYNC_COMBO_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CSyncCombo : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CSyncCombo)

// Data members
public:
protected:
private:

	std::string     m_strErrCode ;
    std::string     m_strMsg ;

	int m_iEFsSyncRetryCount;

// Member functions
public:
	CSyncCombo() {}
	~CSyncCombo() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_SYNC_COMBO_H_
