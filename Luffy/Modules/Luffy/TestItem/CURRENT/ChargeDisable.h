#ifndef _C_CHARGE_DISABLE_H_
#define _C_CHARGE_DISABLE_H_


#include "BaseObject/ITestItem.h"
#include "Phone/AndroidPhone.h"
#include "Processor/GPProcessor.h"

#include "ErrorDefine/Function_ErrorDefine.h"


class CChargeDisable : public ITestItem
{
GP_DECLARE_DYNCREATE(CChargeDisable)


// Data members
public:
protected:
private:
	std::string m_strMsg;

	CGPProcessor    *m_pCGProc;
	CAndroidPhone	*m_pIPhone;


// Member functions
public:
	//CChargeDisable() {}
	//~CChargeDisable() {}
	CChargeDisable(void);
	virtual ~CChargeDisable(void);

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run(int iItem = 0);

protected:
private:
	bool MainFunction();

};


#endif // End of #ifndef _C_CHARGE_DISABLE_H_
