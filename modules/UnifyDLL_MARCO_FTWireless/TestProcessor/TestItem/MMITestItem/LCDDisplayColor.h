/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CDisplay object.
 * FileName   : LCDDisplayColor.h
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2012/04/12
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/04/12    Evonne.Hsieh            Create for Detroit project MMI test
 ********************************************************************************/

#ifndef _C_LCD_DISPLAY_TEST_H_
#define _C_LCD_DISPLAY_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"

class CDisplay : public ITestItem
{
RF_DECLARE_DYNCREATE(CDisplay)

// Data members
public:
protected:
private:
	std::string m_str_patternIndex;


// Member functions
public:
	CDisplay() {}
	~CDisplay() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool LCDDisplayTest();
};


#endif // End of #ifndef _C_LCD_DISPLAY_TEST_H_
