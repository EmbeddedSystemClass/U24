/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CNFCSwpSelfTest object.
 * FileName   : NFCSwpSelfTest.h
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2013/01/16
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2013/01/16    Evonne.Hsieh            Create for Boston project BB test
 ********************************************************************************/

#ifndef _C_NFC_SWP_SELF_TEST_H_
#define _C_NFC_SWP_SELF_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CNFCSwpSelfTest : public ITestItem
{
RF_DECLARE_DYNCREATE(CNFCSwpSelfTest)

// Data members
public:
protected:
private:


// Member functions
public:
	CNFCSwpSelfTest() {}
	~CNFCSwpSelfTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	std::string m_str_SwpResult;
	bool NFCSwpSelfTest();
};


#endif // End of #ifndef _C_NFC_SWP_SELF_TEST_H_