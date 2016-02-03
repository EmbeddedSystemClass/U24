/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CCameraEnd object.
 * FileName   : CameraEnd.h
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2012/04/12
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/04/12    Evonne.Hsieh            Create for Detroit project MMI test
 ********************************************************************************/

#ifndef _C_CAMERA_END_H_
#define _C_CAMERA_END_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CCameraEnd : public ITestItem
{
RF_DECLARE_DYNCREATE(CCameraEnd)

// Data members
public:
protected:
private:


// Member functions
public:
	CCameraEnd() {}
	~CCameraEnd() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool CameraEnd();
};


#endif // End of #ifndef _C_BACKLIGHT_TEST_H_