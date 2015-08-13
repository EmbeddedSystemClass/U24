#ifndef _C_CHECK_BATT_VOLTAGE_H_
#define _C_CHECK_BATT_VOLTAGE_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"
#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CCheckBattVoltage : public ITestItem
{
RF_DECLARE_DYNCREATE(CCheckBattVoltage)


// Data members
public:
protected:
private:
	//parameters
	double m_dVoltage;

// Member functions
public:
	CCheckBattVoltage();
	~CCheckBattVoltage() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool CheckBattVoltage();

};


#endif // End of #ifndef _C_CHECK_BATT_VOLTAGE_H_
