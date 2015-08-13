/*********************************************************************************
 * COPYRIGHT Qisda WR24 2013
 *
 * Description: .h file of SmartCoverInterruptPin object.
 * FileName   : SmartCoverInterruptPin.h
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2013/07/31
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2013/07/31    Evonne.Hsieh                    Create.
 ********************************************************************************/

#pragma once

#include "../../../CommonUtility/BaseObject/ITestItem.h"

class CSmartCoverInterruptPin : public ITestItem
{
RF_DECLARE_DYNCREATE(CSmartCoverInterruptPin)

// Data members
public:
protected:
private:
	//parameters
	std::string m_str_InterruptPin;

public:
	CSmartCoverInterruptPin() {}
	~CSmartCoverInterruptPin() {}

public:
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

private:
	bool SmartCoverCheckInterruptPin(void);
};
