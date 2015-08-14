#ifndef _C_DDR_SIZE_H_
#define _C_DDR_SIZE_H_


#include "BaseObject/ITestItem.h"
#include "ErrorDefine/Function_ErrorDefine.h"


class CDDRSize : public ITestItem
{
GP_DECLARE_DYNCREATE(CDDRSize)


// Data members
public:
protected:
private:

	int m_iMaterialOptions;
	std::vector<std::string> m_vecSizeType;

	std::string m_strMsg;
	std::string m_strMeasured;

	CGPProcessor    *m_pCGProc;
	CAndroidPhone	*m_pIPhone;

// Member functions
public:
	CDDRSize() {}
	~CDDRSize() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run(int iItem = 0);

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_DDR_SIZE_H_
