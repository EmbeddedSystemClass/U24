/*********************************************************************************
 * COPYRIGHT Qisda WE02 2013
 *
 * Description: .cpp file of CBatteryImpedanceResult object.
 * FileName   : BatteryImpedanceResult.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2014/12/4
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2014/12/4       Neil.Du                 Create for Boston BB test
 ********************************************************************************/

#ifndef _C_BATTERY_IMPENDANCE_RESULT_H_
#define _C_BATTERY_IMPENDANCE_RESULT_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class CBatteryImpedanceResult : public ITestItem
{
RF_DECLARE_DYNCREATE(CBatteryImpedanceResult)


// Data members
public:
protected:
private:
	//parameters
	std::string m_str_BIResultPath;
	std::string m_str_LogFolderPath;


// Member functions
public:
	CBatteryImpedanceResult() {}
	~CBatteryImpedanceResult() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool GetBIResult();
};


#endif // End of #ifndef _C_BATTERY_IMPENDANCE_RESULT_H_
