#ifndef _C_RESET_KEY_TEST_H_
#define _C_RESET_KEY_TEST_H_

#include "baseobject/itestitem.h"
#include "FacTestToolInterface.h"

class CResetKeyTest : public ITestItem
{

	GP_DECLARE_DYNCREATE(CResetKeyTest)

	// Data members
	//std::string m_szWorkingDir;       //¼ÇÂ¼CMDÂ·¾¶
	//std::string m_szExeName;

	std::string m_szIicCmdTemp;

	int iiccmd;
	int iicadd;
	int iicdat;
	int iicdel;

	int m_iTimeForPowerOff;
	int m_iTimeForPowerOn;

	std::string m_strMsg;

	CAndroidPhone	*m_pIPhone;
	CGPProcessor    *m_pCGProc;

	st_UIMessage m_st_UIMessage;

	// Member functions
public:
	CResetKeyTest(void);
	~CResetKeyTest(void);
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool Run(int i_slot = 0);

private:
	bool MainFunction(void);
};

#endif