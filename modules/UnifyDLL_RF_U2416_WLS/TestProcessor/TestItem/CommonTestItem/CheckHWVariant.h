#ifndef _C_CHECK_HW_VARIANT_H_
#define _C_CHECK_HW_VARIANT_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CCheckHWVariant : public ITestItem
{
RF_DECLARE_DYNCREATE(CCheckHWVariant)

// Data members
public:
protected:
private:


// Member functions
public:
	CCheckHWVariant() {}
	~CCheckHWVariant() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	void SetItemError(const char* msg);
};


#endif // End of #ifndef _C_CHECK_HW_VARIANT_H_
