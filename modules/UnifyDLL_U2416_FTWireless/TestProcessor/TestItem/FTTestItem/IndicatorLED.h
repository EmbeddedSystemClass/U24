/*********************************************************************************
 * COPYRIGHT Qisda WR24 2013
 *
 * Description: .h file of IndicatorLED object.
 * FileName   : IndicatorLED.h
 * Version    : $Revision:   1.0  $
 * Author     : CY.Lee
 * Time       : 2013/08/02
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2013/08/02    Evonne.Hsieh                      Create.
 ********************************************************************************/

#pragma once

#include "../../../CommonUtility/BaseObject/ITestItem.h"

class CIndicatorLED : public ITestItem
{
RF_DECLARE_DYNCREATE(CIndicatorLED)

public:
	CIndicatorLED() {}
	~CIndicatorLED() {}

public:
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

private:
	bool FlashLEDTestFunc(void);
	CRangePair m_obj_IND_LED_R_Spec;
	CRangePair m_obj_IND_LED_G_Spec;
	CRangePair m_obj_IND_LED_B_Spec;
	std::string m_str_IND_LED_R_Input;
	std::string m_str_IND_LED_G_Input;
	std::string m_str_IND_LED_B_Input;
};
