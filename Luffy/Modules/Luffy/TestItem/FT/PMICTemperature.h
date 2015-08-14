#ifndef _C_PMIC_TEMPERATURE_H_
#define _C_PMIC_TEMPERATURE_H_


#include "BaseObject/ITestItem.h"
#include "ErrorDefine/Function_ErrorDefine.h"
#include "Utils/InspectionUtil.h"


class CPMICTemperature : public ITestItem
{
GP_DECLARE_DYNCREATE(CPMICTemperature)

		// Data members
public:
protected:
private:
	std::string		m_strPICSName;

	short           m_nCmdCode;
	std::string     m_strErrCode;
	std::string     m_strMsg;

	std::string		m_strMeasured;
	double			m_dLower;
	double			m_dUpper;
	std::string		m_strLower;
	std::string		m_strUpper;

	CGPProcessor    *m_pCGProc;
	CAndroidPhone	*m_pIPhone;

	// Member functions
public:
	CPMICTemperature() {}
	~CPMICTemperature() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap) ;
	virtual bool Run(int iItem = 0) ;

protected:
private:
	bool MainFunction();
	
};


#endif // End of #ifndef _C_PMIC_TEMPERATURE_H_
