/*********************************************************************************
 * COPYRIGHT Qisda WR24 2013
 *
 * Description: .h file of CCheckNfcFwVersion object.
 * FileName   : CheckNfcFwVersion.h
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2013/03/15
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2013/03/15    Evonne.Hsieh            Create for Boston BB test
 ********************************************************************************/

#ifndef _C_NFC_FW_VERSION_CHECK_H_
#define _C_NFC_FW_VERSION_CHECK_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class CCheckNfcFwVersion : public ITestItem
{
RF_DECLARE_DYNCREATE(CCheckNfcFwVersion)


// Data members
public:
protected:
private:
	//parameters
	std::string m_str_FwVersion;


// Member functions
public:
	CCheckNfcFwVersion() {}
	~CCheckNfcFwVersion() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool CheckNfcFwVersion();
};


#endif // End of #ifndef _C_NFC_FW_VERSION_CHECK_H_
