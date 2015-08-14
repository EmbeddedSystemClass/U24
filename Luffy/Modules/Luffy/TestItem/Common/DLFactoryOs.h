#ifndef _C_DL_FACTORY_OS_H_
#define _C_DL_FACTORY_OS_H_

#include "baseobject/itestitem.h"
#include "FacTestToolInterface.h"

class CDlFactoryOs : public ITestItem
{
	GP_DECLARE_DYNCREATE(CDlFactoryOs)

	// Data members
private:
	//std::string m_szImageDir;       //记录Image路径
	//std::string m_szWorkingDir;      //记录CMD路径
	//char m_szCmdLine[512];          //记录命令的参数
	//std::string m_szFwFile;          //fw文件名
	//std::string m_szMiscFile;        //MISC文件名
	//std::string m_szRecoveryFile;    //RECOVERY文件名
	//std::string m_szSystemFile ;     //SYSTEM文件名

	//std::string m_strErrCode;
	//std::string m_strMsg;
	
	//bool        m_blnErase;

	CAndroidPhone	*m_pIPhone;
	CGPProcessor    *m_pCGProc;

	// Member functions
public:
	CDlFactoryOs(void);
	~CDlFactoryOs(void);
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool Run(int i_slot = 0);

private:
	bool MainFunction();
};

#endif