#ifndef _C_SAMPLE_PROCESSOR_H_
#define _C_SAMPLE_PROCESSOR_H_


#include "../../CommonUtility/BaseObject/ITestProcessor.h"
#include "../../CommonUtility/BaseObject/ITestItem.h"


class CCommonProcessor : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CCommonProcessor)


// Data members
public:
protected:
private:


// Member functions
public:
	CCommonProcessor() {}
	virtual ~CCommonProcessor() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap) { return true; }

	// Run tool progress
	virtual bool Begin();
	virtual bool IsConnected();
	virtual bool IsConnectedMulti();
	virtual bool IsMobileInFTDMode();
	virtual bool IsSocketReady();
	virtual bool IsSocketReadyMulti();
	virtual bool PreRun(int nIndex);
	virtual bool Run(int nIndex);
	virtual bool PostRun(int nIndex);
	virtual bool Disconnect();
	virtual bool End();

protected:
private:
};


#endif // End of #ifndef _C_SAMPLE_PROCESSOR_H_
