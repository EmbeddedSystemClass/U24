/*********************************************************************************
 * COPYRIGHT Qisda WR24 2013
 *
 * Description: .h file of CRSenseCalibration object.
 * FileName   : RSenseCalibration.h
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2013/07/29
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2013/07/29    Evonne.Hsieh            Create for Saporro BB test
 ********************************************************************************/

#ifndef _C_RSENSE_CALIBRATION_H_
#define _C_RSENSE_CALIBRATION_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class CRSenseCalibration : public ITestItem
{
RF_DECLARE_DYNCREATE(CRSenseCalibration)


// Data members
public:
protected:
private:
	//parameters
	int m_i_WaitTime;
	int m_i_Bias;
	int m_i_RetryCount;
	std::string m_str_SampleNum;
	std::string m_strMessage;
	std::string m_strMeasured;
	CRangePair m_rp_RSenseRange;
	double m_dLower, m_dUpper;


// Member functions
public:
	CRSenseCalibration() {}
	~CRSenseCalibration() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool Run();

protected:
private:
	bool RSenseCalibration();
};


#endif // End of #ifndef _C_RSENSE_CALIBRATION_H_
