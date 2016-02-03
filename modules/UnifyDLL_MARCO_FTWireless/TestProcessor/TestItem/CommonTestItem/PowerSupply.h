#ifndef _C_POWER_SUPPLY_H_
#define _C_POWER_SUPPLY_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CPowerSupply : public ITestItem
{
RF_DECLARE_DYNCREATE(CPowerSupply)


// Data members
public:
protected:
private:
	double m_d_voltage;
	double m_d_current;
	bool m_b_PS_OnOff;


// Member functions
public:
	CPowerSupply() {}
	~CPowerSupply() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool StartPowerSupply();
	bool StopPowerSupply();

};


#endif // End of #ifndef _C_POWER_SUPPLY_ITEM_H_
