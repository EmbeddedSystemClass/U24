/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CNFCAntennaSelfTest object.
 * FileName   : NFCAntennaSelfTest.h
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2013/01/16
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2013/01/16    Evonne.Hsieh            Create for Boston project BB test
 ********************************************************************************/

#ifndef _C_NFC_ANTENNA_SELF_TEST_H_
#define _C_NFC_ANTENNA_SELF_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CNFCAntennaSelfTest : public ITestItem
{
RF_DECLARE_DYNCREATE(CNFCAntennaSelfTest)

// Data members
public:
protected:
private:
	double m_f_UpperSpec[4];
	double m_f_LowerSpec[4];


// Member functions
public:
	CNFCAntennaSelfTest() {}
	~CNFCAntennaSelfTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool NFCAntennaSelfTest();
};


#endif // End of #ifndef _C_NFC_ANTENNA_SELF_TEST_H_