#ifndef _C_BBTEST_H_
#define _C_BBTEST_H_

#include "baseobject/itestitem.h"
#include "FacTestToolInterface.h"

class CBBTest : public ITestItem
{

	GP_DECLARE_DYNCREATE(CBBTest)

	// Data members
private:
	//std::string m_szImageDir;       //记录Image路径
	//std::string m_szWorkingDir;       //记录CMD路径
	//std::string m_szExeName;

	//std::string m_szPMIC;             //PMIC self-test
	//std::string m_szNAND;             //eMMCSizeTest
	//std::string m_szDDR;              //DDRSizeTest
	//std::string m_szHDMI;
	//std::string m_szUart;		      //Uart Loopback
	//std::string m_szEthernet;         //Ethernet Loopback
	//std::string m_szSDWriteProtected;
	//std::string m_szSDPlugin;
	//std::string m_szSDWriteRead;
	//std::string m_szSDeMMC;
	//std::string m_szSDClockRate;

	//char m_szCmdLine[512];            //记录命令的参数

	bool m_enabledPMIC;
	bool m_enabledNAND;
	bool m_enabledDDRSize;
	bool m_enabledHDMI;
	bool m_enabledUart;
	bool m_enabledEthernet;
	bool m_enabledSDWriteProtected;
	bool m_enabledSDPlugin;
	bool m_enabledSDWriteRead;
	bool m_enabledSDeMMCSize;
	bool m_enabledSDClockRate;

	double m_dNandSize; 
	double m_dDDRSize;

	//std::string m_szNandSize;
	//std::string m_szDDRSize;
	//std::string m_szSDeMMCSize;
	//std::string m_szSDType;
	//std::string m_szSDProtected;

	//std::string m_strErrCode;
	//std::string m_strMsg;

	int m_iRetryTime;

	CAndroidPhone	*m_pIPhone;
	CGPProcessor    *m_pCGProc;

	//st_UIMessage m_st_UIMessage;

	// Member functions
public:
	CBBTest(void);
	~CBBTest(void);
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool Run(int i_slot = 0);

private:
	bool MainFunction(void);

};

#endif