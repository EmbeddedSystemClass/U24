/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CCameraTest object.
 * FileName   : CameraTest.h
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2012/04/12
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/04/12    Evonne.Hsieh            Create for Detroit project MMI test
 ********************************************************************************/

#ifndef _C_CAMERA_TEST_H_
#define _C_CAMERA_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CCameraTest : public ITestItem
{
RF_DECLARE_DYNCREATE(CCameraTest)

// Data members
public:
protected:
private:
	std::string m_str_patternType;


// Member functions
public:
	CCameraTest() {}
	~CCameraTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool CameraFrontBack();
};


#endif // End of #ifndef _C_BACKLIGHT_TEST_H_