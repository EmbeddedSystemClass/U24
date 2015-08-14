#ifndef _C_CHECK_USB_H_
#define _C_CHECK_USB_H_


#include "BaseObject/ITestItem.h"
#include "ErrorDefine/Function_ErrorDefine.h"


class CCheckUSB: public ITestItem
{
GP_DECLARE_DYNCREATE(CCheckUSB)


// Data members
public:
protected:
private:

	std::string m_strMsg;
	std::string m_strUSBType;
	std::string m_strUSBPresent;
	std::string m_strUSBMaxCurrent;

	CGPProcessor    *m_pCGProc;
	CAndroidPhone	*m_pIPhone;

// Member functions
public:
	CCheckUSB() {}
	~CCheckUSB() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run(int iItem = 0);

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_CHECK_MCU_VERSION__H_
