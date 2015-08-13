/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of ADCCalibration object.
 * FileName   : ADCCalibration.h
 * Version    : $Revision:   1.0  $
 * Author     : CY.Lee
 * Time       : 2012/09/10
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/09/10       CY.Lee                          Create.
 ********************************************************************************/

#ifndef _C_ADC_CALIBRATION_H_
#define _C_ADC_CALIBRATION_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"

class ADCCalibration : public ITestItem
{
RF_DECLARE_DYNCREATE(ADCCalibration)


// Data members
private:
	//parameters
	std::string m_str_Timeout;
	std::string m_str_Cycle;
	std::string m_str_Filter;
	std::string m_str_Delta;

// Member functions
public:
	ADCCalibration() {}
	~ADCCalibration() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool ADC_Calibration();
	bool Check_SWversion();
};


#endif // End of #ifndef _C_ADC_CALIBRATION_H_
