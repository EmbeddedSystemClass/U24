/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: L-Sensor Calibration.
 * FileName   : LSenssorCalibration.h
 * Version    : $Revision:   1.0  $
 * Author     : CY.Lee
 * Time       : 2012/04/15
 * History    :
 *  NO      Date            Updated by            Content of change
 *	1		2012/04/15		CY.Lee				  Create.
 ********************************************************************************/

#pragma once

#include "../../../CommonUtility/BaseObject/ITestItem.h"

#define HallSensorOpenTest			"HallSensorOpenTest"
#define HallSensorCloseTest          "HallSensorCloseTest"

class CHallSensor : public ITestItem
{
RF_DECLARE_DYNCREATE(CHallSensor)

public:
	CHallSensor() {}
	virtual ~CHallSensor() {}

public:
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

private:
	bool runHallSensorOpenTest(void);
	bool runHallSensorCloseTest(void);
	bool SendJigRequest(CString str_command);

private:
	std::string m_str_TestItem;
	
	std::string str_Pics ;
	int         m_i_sleep;
	int			m_i_retry;
	int			m_i_EnableJIG;
};
