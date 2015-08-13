
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
	std::string m_strMessage;
	std::string m_strMeasured;
	double m_dVoltage;

// Member functions
public:
	CCheckBattVoltage();
	~CCheckBattVoltage() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
	bool CheckBattVoltage();

};


#endif // End of #ifndef _C_CHECK_BATT_VOLTAGE_H_
