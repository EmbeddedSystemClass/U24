#ifndef _C_RECOVER_KEY_TEST_H_
#define _C_RECOVER_KEY_TEST_H_

#include "baseobject/itestitem.h"
#include "FacTestToolInterface.h"

class CRecoverKeyTest : public ITestItem
{

	GP_DECLARE_DYNCREATE(CRecoverKeyTest)

	// Data members
	std::string m_szWorkingDir;       //记录CMD路径
	std::string m_szExeName;

	std::string m_szRecoverKey;             //RecoverKey CMD

	std::string m_szRecoverKeySize;

	std::string m_szIicCmdTemp;

	bool m_iicFunction;
	bool m_adfucmd;

	int iiccmd;
	int iicadd;
	int iicdat;
	int iicdel;

	int m_iRetryTime;

	int m_iIicCmdCount;
	char m_szCmdLine[512];            //记录命令的参数

	CWinThread* m_p_h_iicthread;

	CAndroidPhone	*m_pIPhone;
	CGPProcessor    *m_pCGProc;

	st_UIMessage m_st_UIMessage;

	// Member functions
public:
	CRecoverKeyTest(void);
	~CRecoverKeyTest(void);
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool Run(int i_slot = 0);

private:
	bool MainFunction(void);
	static UINT iicFunction(LPVOID pParam);
};

#endif