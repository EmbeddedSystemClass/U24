/*********************************************************************************
 * COPYRIGHT Qisda WR24 2013
 *
 * Description: .h file of SmartCoverChargeCurrent object.
 * FileName   : SmartCoverChargeCurrent.h
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2013/07/30
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2013/07/30    Evonne.Hsieh           Create.
 ********************************************************************************/

#ifndef _C_Charge_Current_H_
#define _C_Charge_Current_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CSmartCoverChargeCurrent : public ITestItem
{
RF_DECLARE_DYNCREATE(CSmartCoverChargeCurrent)


// Data members
private:
	int m_i_WaitTimeForFetch;
	int m_i_TotalTime;
	CRangePair m_rp_currentSpec;

// Member functions
public:
	CSmartCoverChargeCurrent() {}
	~CSmartCoverChargeCurrent() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

private:
	bool SmartCoverChargeCurrentTest();
};


#endif // End of #ifndef _C_Charge_Current_H_
