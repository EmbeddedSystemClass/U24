#ifndef _C_EMMC_SIZE_H_
#define _C_EMMC_SIZE_H_


#include "BaseObject/ITestItem.h"
#include "ErrorDefine/Function_ErrorDefine.h"


class CEMMCSize : public ITestItem
{
GP_DECLARE_DYNCREATE(CEMMCSize)


// Data members
public:
protected:
private:
	/*int m_iLower;
	int m_iUpper;*/
	int m_iMaterialOptions;
	std::vector<std::string> m_vecSizeType;

	std::string m_strMsg;
	std::string m_strMeasured;

	CGPProcessor    *m_pCGProc;
	CAndroidPhone	*m_pIPhone;

// Member functions
public:
	CEMMCSize() {}
	~CEMMCSize() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run(int iItem = 0);

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_EMMC_SIZE_H_
