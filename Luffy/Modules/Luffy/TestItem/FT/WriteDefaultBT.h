#ifndef _C_WR_BTADDR_H_
#define _C_WR_BTADDR_H_


#include "BaseObject/ITestItem.h"


class CWriteDefaultBT : public ITestItem
{
GP_DECLARE_DYNCREATE(CWriteDefaultBT)


// Data members
public:
protected:
private:
	std::string m_strBTMacAddr;
	std::string m_strMsg;
	std::string m_strMeasured;
	std::string m_strErrorCode;
	std::string m_strPICSName;
	std::string m_strDefaultAddr; // split by : 
	std::string m_strDefaultAddr_comma; // split by

	CGPProcessor    *m_pCGProc;
	CAndroidPhone	*m_pIPhone;

// Member functions
public:
	CWriteDefaultBT() {}
	~CWriteDefaultBT() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run(int iItem = 0);

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_EMMC_SIZE_H_
