/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: L-Sensor Lux Check.
 * FileName   : BostonLSensorLuxCheck.h
 * Version    : $Revision:   1.0  $
 * Author     : Lion.Wu
 * Time       : 2012/07/30
 * History    :
 *  NO      Date            Updated by            Content of change
 *	1		2012/07/30		Lion.Wu				  Create.
 ********************************************************************************/

#pragma once

#include "../../../CommonUtility/BaseObject/ITestItem.h"

class CBostonAdbLogcat : public ITestItem
{
RF_DECLARE_DYNCREATE(CBostonAdbLogcat)

public:
	CBostonAdbLogcat() {}
	virtual ~CBostonAdbLogcat() {}

public:
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

private:
	bool runAdbLogcat(bool b_IsPass);

private:
};
