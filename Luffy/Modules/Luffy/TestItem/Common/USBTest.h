#ifndef _C_USBTEST_H_
#define _C_USBTEST_H_

#include "baseobject/itestitem.h"
#include "FacTestToolInterface.h"

class CUsbTest : public ITestItem
{

	GP_DECLARE_DYNCREATE(CUsbTest)

	// Data members
	std::string m_szWorkingDir;       //记录CMD路径
	//std::string m_szExeName;

	//std::string m_szIdProductTest;    //IdProduct CMD
	//std::string m_szIdVendorTest;         //IdVendor CMD

	//std::string m_szIdProductSize;
	//std::string m_szIdVendorSize;

	//std::string m_strMsg;

	//char m_szIdProductCmdLine[512];            //记录命令的参数
	//char m_szIdVendorCmdLine[512];            //记录命令的参数

	CAndroidPhone	*m_pIPhone;
	CGPProcessor    *m_pCGProc;

	//st_UIMessage m_st_UIMessage;

	// Member functions
public:
	CUsbTest(void);
	~CUsbTest(void);
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool Run(int i_slot = 0);

private:
	bool MainFunction(void);
};

#endif