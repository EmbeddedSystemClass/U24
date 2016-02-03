/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of AutoJackDetect object.
 * FileName   : JackDetectTest.h
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2012/04/12
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/04/12    Evonne.Hsieh            Create for Detroit project MMI test
 ********************************************************************************/

#ifndef _C_JACK_DETECT_TEST_H_
#define _C_JACK_DETECT_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"

class AutoJackDetect : public ITestItem
{
RF_DECLARE_DYNCREATE(AutoJackDetect)


// Data members
public:
protected:
private:
	//parameters
	int m_i_Type;
	bool m_needPrompt;


// Member functions
public:
	AutoJackDetect() {}
	~AutoJackDetect() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool CheckJackType();
};


#endif // End of #ifndef _C_JACK_DETECT_TEST_H_
