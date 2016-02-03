/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of VibratorRun object.
 * FileName   : VibratorRun.h
 * Version    : $Revision:   1.0  $
 * Author     : CY.Lee
 * Time       : 2012/05/21
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/05/21     CY.Lee                      Create.
 ********************************************************************************/

#pragma once

#include "../../../CommonUtility/BaseObject/ITestItem.h"

class VibratorRun : public ITestItem
{
RF_DECLARE_DYNCREATE(VibratorRun)

public:
	VibratorRun() {}
	~VibratorRun() {}

public:
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

private:
	bool VibrateTestFunc(void);
	int m_i_VibratorDelay;
	CRangePair m_obj_VIB_TESTSpec;
};
