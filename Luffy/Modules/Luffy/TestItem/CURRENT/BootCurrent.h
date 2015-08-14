#ifndef _C_BOOT_CURRENT_H_
#define _C_BOOT_CURRENT_H_

#include "BaseObject/ITestItem.h"
#include "Utils/StringUtil.h"
#include "ErrorDefine/Function_ErrorDefine.h"
#include "Processor/GPProcessor.h"
#include "Utils/InspectionUtil.h"


class CBootCurrent : public ITestItem
{
GP_DECLARE_DYNCREATE(CBootCurrent)


// Data members
public:
protected:
private:
	int m_iWaitTimeForShutdown;
	int m_iWaitTimeForBoot;
	int m_iMeasureCount;
	int m_iWaitTimeAfterBoot;
	double m_dLower, m_dUpper;

	//Super4Relay* m_pRBCtrl;

	std::string m_strMsg;
	std::string m_strMeasured;

	//cast
	CGPProcessor    *m_pCGProc;
	IPowerSupply    *m_pIPS;
	IPowerSupply    *m_pIPS2;


// Member functions
public:
	//CBootCurrent() {}
	//~CBootCurrent() {}
	CBootCurrent(void);
	virtual ~CBootCurrent(void);

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run(int iItem);

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_BOOT_CURRENT_H_
