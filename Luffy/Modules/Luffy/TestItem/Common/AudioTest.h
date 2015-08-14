#ifndef _C_AUDIO_TEST_H_
#define _C_AUDIO_TEST_H_

#include "baseobject/itestitem.h"
#include "FacTestToolInterface.h"

class CAudioTest : public ITestItem
{
	GP_DECLARE_DYNCREATE(CAudioTest)

private:
	CAndroidPhone	*m_pIPhone;
	CGPProcessor    *m_pCGProc;

	//st_UIMessage m_st_UIMessage;

	std::string m_szWorkingDir;
	std::string m_szCommand;
	std::string m_strMsg;

	// Member functions
public:
	CAudioTest(void);
	~CAudioTest(void);
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool Run(int i_slot = 0);

private:
	bool MainFunction(void);
};

#endif