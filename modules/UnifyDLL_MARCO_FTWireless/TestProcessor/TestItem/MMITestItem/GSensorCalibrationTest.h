/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CGSensorCalibration object.
 * FileName   : GSensorCalibrationTest.h
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh 
 * Time       : 2012/06/12
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2012/06/12    Evonne.Hsieh          Create.
 ********************************************************************************/

#ifndef _C_GSENSOR_CALIBRATION_H_
#define _C_GSENSOR_CALIBRATION_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"

class CGSensorCalibration : public ITestItem
{
RF_DECLARE_DYNCREATE(CGSensorCalibration)


// Data members
public:
protected:
private:
	//parameters
	std::string m_str_sensorType;
	int m_i_wait_for_k;


// Member functions
public:
	CGSensorCalibration() {}
	~CGSensorCalibration() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool GSensorCalibrationTest();
	bool ExecAdbOut(CString Command, char* output, char* ErrorCode);

};


#endif // End of #ifndef _C_GSENSOR_CALIBRATION_H_
