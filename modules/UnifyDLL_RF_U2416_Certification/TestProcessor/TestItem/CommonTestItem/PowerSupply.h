#ifndef _C_POWER_SUPPLY_ITEM_H_
#define _C_POWER_SUPPLY_ITEM_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CPowerSupplyItem : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CPowerSupplyItem)


// Data members
public:
protected:
private:
	int m_iPS_Index;
	double m_dVoltage;
	double m_dCurrent;
	bool m_bPS_OnOff;

	std::string m_strMsg;


// Member functions
public:
	CPowerSupplyItem() {}
	~CPowerSupplyItem() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_POWER_SUPPLY_ITEM_H_
