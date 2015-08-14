#ifndef _C_CODEC_TEST_H_
#define _C_CODEC_TEST_H_


#include "BaseObject/ITestItem.h"
#include "ErrorDefine/Function_ErrorDefine.h"


class CCodecTest : public ITestItem
{
GP_DECLARE_DYNCREATE(CCodecTest)

		// Data members
public:
protected:
private:
	std::string		m_strPICSName;

	short           m_nCmdCode;
	std::string     m_strErrCode;
	std::string     m_strMsg;
	int m_ID;
	std::string		m_strMeasured;
	std::string		m_strAudioCodecID;

	CGPProcessor    *m_pCGProc;
	CAndroidPhone	*m_pIPhone;

	// Member functions
public:
	CCodecTest() {}
	~CCodecTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap) ;
	virtual bool Run(int iItem = 0) ;

protected:
private:
	bool MainFunction();
	
};


#endif // End of #ifndef _C_CODEC_TEST_H_
