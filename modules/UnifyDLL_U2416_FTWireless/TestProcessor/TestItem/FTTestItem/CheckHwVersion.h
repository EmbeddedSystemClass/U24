/*********************************************************************************
 * COPYRIGHT Qisda WR24 2013
 *
 * Description: .h file of CCheckHwVersion object.
 * FileName   : CheckHwVersion.h
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2013/06/05
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2013/06/05    Evonne.Hsieh            Create for Boston BB test
 ********************************************************************************/

#ifndef _C_HW_VERSION_CHECK_H_
#define _C_HW_VERSION_CHECK_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class CCheckHwVersion : public ITestItem
{
RF_DECLARE_DYNCREATE(CCheckHwVersion)


// Data members
public:
protected:
private:
	//parameters
	std::string m_str_HwVersion;
	std::string m_str_CheckStation;
	std::string m_str_ModelName;


// Member functions
public:
	CCheckHwVersion() {}
	~CCheckHwVersion() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool CheckHwVersion();
	string trim(const string &str);
	string ltrim(const string &str);
	string rtrim(const string &str);
};


#endif // End of #ifndef _C_HW_VERSION_CHECK_H_
