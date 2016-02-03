#ifndef _I_TEST_ITEM_H_
#define _I_TEST_ITEM_H_


#include "ITestProcessor.h"
#include "../CommonDefine.h"
#include "../BaseItemCode.h"//110406 gilpin add for base item
#include "../ErrorCode.h"//110406 gilpin add for error code
#include "../../../Common/ErrorDefine/ErrorDefine.h"

interface ITestItem : public ITestProcessor
{
RF_DECLARE_DYNAMIC(ITestItem)


// Data members
public:
protected:
private:


// Member functions
public:
	ITestItem() {}
	virtual ~ITestItem() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap) = 0;

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

	// Run test item
	virtual bool PreRun() = 0;
	virtual bool Run() = 0;
	virtual bool PostRun() = 0;

protected:
	double GetCableLoss(const std::string strRfType, const std::string strRfBand, std::string strRfRange);
	bool InRange(int iLowerBound, int iUpperBound, int iMeasured);
	bool InRange(double dLowerBound, double dUpperBound, double dMeasured);

private:
};


#endif // End of #ifndef _I_TEST_ITEM_H_
