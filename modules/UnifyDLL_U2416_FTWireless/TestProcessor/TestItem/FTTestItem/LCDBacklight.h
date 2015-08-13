/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of LCDBacklight object.
 * FileName   : LCDBacklight.h
 * Version    : $Revision:   1.0  $
 * Author     : CY.Lee
 * Time       : 2012/05/21
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/05/21     CY.Lee                      Create.
 ********************************************************************************/

#pragma once

#include "../../../CommonUtility/BaseObject/ITestItem.h"

class LCDBacklight : public ITestItem
{
RF_DECLARE_DYNCREATE(LCDBacklight)

public:
	LCDBacklight() {}
	~LCDBacklight() {}

public:
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

private:
	bool BacklightTestFunc(void);
	int m_i_Delay;
	std::string m_str_lightLevel;
	CRangePair m_obj_Backlight_Spec;
};
