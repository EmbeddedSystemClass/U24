#ifndef _C_CONNECT_PHONE_ADFU_H_
#define _C_CONNECT_PHONE_ADFU_H_

#include "baseobject/itestitem.h"
#include "FacTestToolInterface.h"
class CConnectPhoneAdfu : public ITestItem
{
	GP_DECLARE_DYNCREATE(CConnectPhoneAdfu)

	// Data members
private:
	std::string m_szImageDir;       //��¼Image·��
	std::string m_szWorkingDir;      //��¼CMD·��
	char m_szCmdLine[512];          //��¼����Ĳ���
	std::string m_szFwFile;          //fw�ļ���
	std::string m_szMiscFile;        //MISC�ļ���
	std::string m_szRecoveryFile;    //RECOVERY�ļ���
	std::string m_szSystemFile ;     //SYSTEM�ļ���

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