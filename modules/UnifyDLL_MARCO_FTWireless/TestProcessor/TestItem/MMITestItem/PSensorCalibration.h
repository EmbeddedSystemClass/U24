/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: P-Sensor Calibration.
 * FileName   : BostonPSenssorCalibration.h
 * Version    : $Revision:   1.0  $
 * Author     :Lion.Wu
 * Time       : 2012/04/15
 * History    :
 *  NO      Date            Updated by            Content of change
 *	1		2013/01/25		Lion.Wu				  Create.
 ********************************************************************************/

#pragma once

#include "../../../CommonUtility/BaseObject/ITestItem.h"

class CBostonPSensorCalibration : public ITestItem
{
RF_DECLARE_DYNCREATE(CBostonPSensorCalibration)

public:
	CBostonPSensorCalibration() {}
	virtual ~CBostonPSensorCalibration() {}

public:
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

private:
	bool PSensorCalibrationProc(void);
	bool checkPSensorValue(std::string cs_farValue, std::string cs_nearValue);
	bool SendJigRequest(CString str_command);
	void Log_Value(CString message);
private:
	CRangePair	m_obj_PSensorNearSpec;
	CRangePair	m_obj_PSensorFarSpec;
	int         m_i_sleep_for_Jig;
	int			m_i_wait_for_k;
	int			m_i_retry;
	int			 m_i_EnableJIG;
};
