#ifndef _C_ERASE_OS_H_
#define _C_ERASE_OS_H_


#include "BaseObject/ITestItem.h"
#include "ErrorDefine/Common_ErrorDefine.h"
#include "FacTestToolInterface.h"

class CEraseOs : public ITestItem
{
GP_DECLARE_DYNCREATE(CEraseOs)


// Data members
public:
protected:
private:
	//ITestBox		*m_pITestBox;
	CAndroidPhone	*m_pIPhone;
	CGPProcessor    *m_pCGProc;

	//st_UIMessage m_st_UIMessage;

	//std::string m_szImageDir;       //记录Image路径
	//std::string m_szWorkingDir;      //记录CMD路径
	//char m_szCmdLine[512];          //记录命令的参数
	//std::string m_szFwFile;          //fw文件名
	//std::string m_szMiscFile;        //MISC文件名
	//std::string m_szRecoveryFile;    //RECOVERY文件名
	//std::string m_szSystemFile ;     //SYSTEM文件名

	//std::string m_strErrCode;

	//std::string m_strPICSName;
	//std::string m_strMsg;


	// Member functions
public:
	CEraseOs() {}
	~CEraseOs() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run(int iItem = 0);

protected:
private:
	bool MainFunction();
};



#endif // End of #ifndef _C_ERASE_OS_H_
