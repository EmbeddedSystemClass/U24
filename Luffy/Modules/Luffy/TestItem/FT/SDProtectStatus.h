#ifndef _C_SD_PROTECT_STATUS_H_
#define _C_SD_PROTECT_STATUS_H_


#include "BaseObject/ITestItem.h"
#include "ErrorDefine/Function_ErrorDefine.h"


class CSDProtectStatus : public ITestItem
{
GP_DECLARE_DYNCREATE(CSDProtectStatus)

		// Data members
public:
protected:
private:
	std::string		m_strPICSName;
	std::string     m_strStatus;

	short           m_nCmdCode;
	std::string     m_strErrCode;
	std::string     m_strMsg;
	std::string m_strMeasured;
	int m_ID;

	CGPProcessor    *m_pCGProc;
	CAndroidPhone	*m_pIPhone;

	// Member functions
public:
	CSDProtectStatus() {}
	~CSDProtectStatus() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap) ;
	virtual bool Run(int iItem = 0) ;

protected:
private:
	bool MainFunction();
	
};


#endif // End of #ifndef _C_SD_PROTECT_STATUS_H_
