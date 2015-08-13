/*********************************************************************************
 * COPYRIGHT Qisda WR24 2013
 *
 * Description: .h file of SmartCoverDetectPin object.
 * FileName   : SmartCoverDetectPin.h
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2013/08/01
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2013/08/01    Evonne.Hsieh                    Create.
 ********************************************************************************/

#pragma once

#include "../../../CommonUtility/BaseObject/ITestItem.h"

class CSmartCoverDetectPin : public ITestItem
{
RF_DECLARE_DYNCREATE(CSmartCoverDetectPin)

// Data members
public:
protected:
private:
	//parameters
	std::string m_str_DetectPin;

public:
	CSmartCoverDetectPin() {}
	~CSmartCoverDetectPin() {}

public:
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

private:
	bool SmartCoverDetectPin(void);
};
