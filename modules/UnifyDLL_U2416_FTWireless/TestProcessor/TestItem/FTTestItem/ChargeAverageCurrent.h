

#ifndef _C_Charge_Average_Current_H_
#define _C_Charge_Average_Current_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"

#define ChargingAverageCurrent  _T("ChargingAverageCurrent")


class ChargeAverageCurrent : public ITestItem
{
RF_DECLARE_DYNCREATE(ChargeAverageCurrent)


// Data members

private:

	std::string m_str_Name;
	std::string m_str_Value;
	CRangePair m_rp_Value;
	bool m_b_CheckMatch;
	int m_iCheckMode;// 0 = QC1 MODE , 1 = T47Mode
	int m_iReadMode; // 0 = read Chip , 1 = read PS1
	int m_i_sleep;
	int m_i_ChargeTime;


// Member functions
public:

	ChargeAverageCurrent() {}
	~ChargeAverageCurrent() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

private:
	bool CheckSystemChargingAverageCurrent();
};


#endif // End of #ifndef _C_Charge_Average_Current_H_
