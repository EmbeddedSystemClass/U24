/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CPlayWav object.
 * FileName   : PlayWavTest.h
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2012/04/12
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2012/04/12    Evonne.Hsieh            Create for Detroit project MMI test
 ********************************************************************************/

#ifndef _C_PLAY_WAV_TEST_H_
#define _C_PLAY_WAV_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"

class CPlayWav : public ITestItem
{
RF_DECLARE_DYNCREATE(CPlayWav)

// Data members
public:
protected:
private:
	std::string m_str_OnOff;
	std::string m_str_Device;
	std::string m_str_VolumeVal;
	int m_i_SleepTime;


// Member functions
public:
	CPlayWav() {}
	~CPlayWav() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool PlayWavTest();
};


#endif // End of #ifndef _C_PLAY_WAV_TEST_H_
