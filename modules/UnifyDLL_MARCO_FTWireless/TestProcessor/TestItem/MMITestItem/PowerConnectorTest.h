/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CPowerConnectorTest object.
 * FileName   : PowerConnectorTest.h
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2012/06/04
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/04/12    Evonne.Hsieh            Create for Detroit project MMI test
 ********************************************************************************/

#pragma once

#include "../../../CommonUtility/BaseObject/ITestItem.h"

class CPowerConnectorTest : public ITestItem
{
RF_DECLARE_DYNCREATE(CPowerConnectorTest)

// Data members
public:
protected:
private:
	CRangePair m_obj_PowerVoltageSpec;
	CRangePair m_obj_PowerBattIDSpec;
	CRangePair m_obj_PowerGaugeTemperatureSpec;

// Member functions
public:
	CPowerConnectorTest() {}
	~CPowerConnectorTest() {}

public:
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

private:
	bool PowerGaugeVoltage(void);
	bool PowerBattID(void);
	bool PowerGaugeTemperature(void);
};
