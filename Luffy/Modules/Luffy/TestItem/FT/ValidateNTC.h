#ifndef _C_VALIDATE_NTC_H_
#define _C_VALIDATE_NTC_H_


#include "BaseObject/ITestItem.h"
#include "ErrorDefine/Function_ErrorDefine.h"
#include "Utils/InspectionUtil.h"


class CValidateNTC : public ITestItem
{
GP_DECLARE_DYNCREATE(CValidateNTC)

		// Data members
public:
protected:
private:
	std::string		m_strPICSName;

	short           m_nCmdCode;
	std::string     m_strErrCode;
	std::string     m_strMsg;

	BYTE			m_byteEnable;

	std::string		m_strNTC_1, m_strNTC_1_Lower, m_strNTC_1_Upper;
	std::string		m_strNTC_2, m_strNTC_2_Lower, m_strNTC_2_Upper;
	std::string		m_strNTC_3, m_strNTC_3_Lower, m_strNTC_3_Upper;
	std::string		m_strNTC_4, m_strNTC_4_Lower, m_strNTC_4_Upper;
	std::string		m_strNTC_5, m_strNTC_5_Lower, m_strNTC_5_Upper;

	double			m_dNTC1, m_dNTC_1_Lower, m_dNTC_1_Upper;
	double			m_dNTC2, m_dNTC_2_Lower, m_dNTC_2_Upper;
	double			m_dNTC3, m_dNTC_3_Lower, m_dNTC_3_Upper;
	double			m_dNTC4, m_dNTC_4_Lower, m_dNTC_4_Upper;
	double			m_dNTC5, m_dNTC_5_Lower, m_dNTC_5_Upper;

	std::string		m_strMeasured;

	CGPProcessor    *m_pCGProc;
	CAndroidPhone	*m_pIPhone;

	// Member functions
public:
	CValidateNTC() {}
	~CValidateNTC() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap) ;
	virtual bool Run(int iItem = 0) ;

protected:
private:
	bool MainFunction();
	
};


#endif // End of #ifndef _C_VALIDATE_NTC_H_
