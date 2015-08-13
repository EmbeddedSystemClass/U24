/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CCheckGSensorValue object.
 * FileName   : checkGSensorValue.h
 * Version    : $Revision:   1.0  $
 * Author     : Lion.Wu
 * Time       : 2013/02/22 
 ********************************************************************************/

#ifndef _C_CHECK_GSENSOR_VALUE_H_
#define _C_CHECK_GSENSOR_VALUE_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"

class CCheckGSensorValue : public ITestItem
{
RF_DECLARE_DYNCREATE(CCheckGSensorValue)


// Data members
public:
protected:
private:
	//parameters
	std::string m_str_sensorType;
	CString cs_sz_GSensorSpec;
	int m_i_wait_for_k;


// Member functions
public:
	CCheckGSensorValue() {}
	~CCheckGSensorValue() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool checkGSensorValue();
	CRangePair	m_obj_GSensorX;
	CRangePair	m_obj_GSensorY;
	CRangePair	m_obj_GSensorZ;
};


#endif // End of #ifndef _C_GSENSOR_CALIBRATION_H_
