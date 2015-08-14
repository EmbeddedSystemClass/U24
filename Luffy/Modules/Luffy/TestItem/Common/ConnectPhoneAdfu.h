#ifndef _C_CONNECT_PHONE_ADFU_H_
#define _C_CONNECT_PHONE_ADFU_H_

#include "baseobject/itestitem.h"
#include "FacTestToolInterface.h"
class CConnectPhoneAdfu : public ITestItem
{
	GP_DECLARE_DYNCREATE(CConnectPhoneAdfu)

	// Data members
private:
	std::string m_szImageDir;       //记录Image路径
	std::string m_szWorkingDir;      //记录CMD路径
	char m_szCmdLine[512];          //记录命令的参数
	std::string m_szFwFile;          //fw文件名
	std::string m_szMiscFile;        //MISC文件名
	std::string m_szRecoveryFile;    //RECOVERY文件名
	std::string m_szSystemFile ;     //SYSTEM文件名

	std::string m_strErrCode;
	std::string m_strMsg;

	CAndroidPhone	*m_pIPhone;
	CGPProcessor    *m_pCGProc;

	st_UIMessage m_st_UIMessage;

	// Member functions
public:
	CConnectPhoneAdfu(void);
	~CConnectPhoneAdfu(void);
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool Run(int i_slot = 0);

private:
	bool MainFunction();
};

#endif