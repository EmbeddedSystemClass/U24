/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CSapporoFtdTest object.
 * FileName   : CSapporoFtdTest.h
 * Version    : $Revision:   1.0  $
 * Author     : Lion.Wu	
 * Time       : 2012.03.15

 ********************************************************************************/

#ifndef _C_BOSTON_BUTTON_CHECK_TEST_H_
#define _C_BOSTON_BUTTON_CHECK_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"

class CSapporoFtdTest : public ITestItem
{
RF_DECLARE_DYNCREATE(CSapporoFtdTest)


// Data members
public:
protected:
private:
	std::string std_FtdFunction;
	

// Member functions
public:
	CSapporoFtdTest() {}
	~CSapporoFtdTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool runUsbDetectStart();
	bool runUsbDetectEnd();
};


#endif // End of #ifndef _C_JACK_DETECT_TEST_H_
