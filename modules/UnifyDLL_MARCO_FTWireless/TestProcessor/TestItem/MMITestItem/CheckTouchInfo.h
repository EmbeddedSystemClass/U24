/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CCheckTouch object.
 * FileName   : BostonTouch.h
 * Version    : $Revision:   1.0  $
 * Author     : Lion.Wu
 * Time       : 2012/04/12
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/04/12    Lion.Wu            Create for Detroit project MMI test
 ********************************************************************************/

#ifndef _C_Boston_Touch_TEST_H_
#define _C_Boston_Touch_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CCheckTouch : public ITestItem
{
RF_DECLARE_DYNCREATE(CCheckTouch)

// Data members
public:
protected:
private:

	std::string m_str_TouchType;
	std::string m_str_TouchData;
	CString cs_TouchData[2];
	CString cs_TouchData1;

	//std::string m_str_TouchData_B;
// Member functions
public:
	CCheckTouch() {}
	~CCheckTouch() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool readTouchModel();
	bool readTouchModelLimitLength();
	bool readTouchFWVersion();
	
};


#endif // End of #ifndef _C_GYRO_SELF_TEST_H_