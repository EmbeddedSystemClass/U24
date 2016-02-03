/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of ChargeCurrent object.
 * FileName   : ChargeCurrent.h
 * Version    : $Revision:   1.1  $
 * Author     : CY.Lee
 * Time       : 2012/05/11
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2012/05/11      CY.Lee                Create.
 ********************************************************************************/

#ifndef _C_Charge_Current_H_
#define _C_Charge_Current_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class ChargeCurrent : public ITestItem
{
RF_DECLARE_DYNCREATE(ChargeCurrent)


// Data members
private:
	int m_i_WaitTimeForFetch;
	int m_i_delayTimeMS;
	CRangePair m_rp_currentSpec;

// Member functions
public:
	ChargeCurrent() {}
	~ChargeCurrent() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

private:
	bool ChargeCurrentTest();
};


#endif // End of #ifndef _C_Charge_Current_H_
