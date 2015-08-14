#ifndef _C_BT_CHIPID_H_
#define _C_BT_CHIPID_H_


#include "BaseObject/ITestItem.h"
#include "ErrorDefine/Function_ErrorDefine.h"


class CReadWiFiAddr : public ITestItem
{
GP_DECLARE_DYNCREATE(CReadWiFiAddr)


// Data members
public:
protected:
private:
	std::string m_strMsg;
	std::string m_strMeasured;
	std::string m_strPICSName;

	CGPProcessor    *m_pCGProc;
	CAndroidPhone	*m_pIPhone;

// Member functions
public:
	CReadWiFiAddr() {}
	~CReadWiFiAddr() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run(int iItem = 0);

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_EMMC_SIZE_H_
