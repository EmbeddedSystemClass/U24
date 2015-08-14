#ifndef _C_COINCELLVOL_H_
#define _C_COINCELLVOL_H_
#include "baseobject/itestitem.h"
#include "FacTestToolInterface.h"

class CCoincellVol : public ITestItem
{

	GP_DECLARE_DYNCREATE(CCoincellVol)

	// Data members
	std::string m_szWorkingDir;       //记录CMD路径
	//std::string m_szExeName;

	std::string m_szCoincellVol;             //CoincellVol CMD

	int m_iCoincellVolSize;             //  Vol(mV) = m_szCoincellVolSize * 5.86(mV)

	std::string m_strMsg;

	char m_szCmdLine[512];            //记录命令的参数

	CAndroidPhone	*m_pIPhone;
	CGPProcessor    *m_pCGProc;

	//st_UIMessage m_st_UIMessage;

	// Member functions
public:
	CCoincellVol(void);
	~CCoincellVol(void);
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool Run(int i_slot = 0);

private:
	bool MainFunction(void);
};

#endif