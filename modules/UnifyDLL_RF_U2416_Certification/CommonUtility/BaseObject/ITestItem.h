#ifndef _I_TEST_ITEM_H_
#define _I_TEST_ITEM_H_


#include "ITestProcessor.h"
#include "../CommonDefine.h"


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

	/* Set/Get FA Data */
	virtual bool SetFAData(int i_slot, char* sz_value);
	virtual bool GetFAData(int i_slot, char* sz_value, int i_size);

	/*Get/Set FA Sector Data*/
	virtual bool GetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize);
	virtual bool SetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize);

	virtual bool Initial();
	virtual bool IsConnected();
	//virtual bool GetFA(char* szValue, int iSize);
	virtual bool Begin();
	virtual bool PreRun();
	virtual bool PostRun();
	virtual bool Stop();
	virtual bool Disconnect();
	virtual bool End();

	virtual bool InitData(std::map<std::string, std::string>& paramMap) = 0;
	virtual bool Run() = 0;

protected:
	//double GetCableLoss(const std::string strRfType, const std::string strRfBand, std::string strRfRange);
	//bool UpdateCableLoss(const std::string strRfType, const std::string strRfBand, std::string strRfRange, std::string strNewLoss);
	bool InRange(int iLowerBound, int iUpperBound, int iMeasured);
	bool InRange(double dLowerBound, double dUpperBound, double dMeasured);

private:
};


#endif // End of #ifndef _I_TEST_ITEM_H_
