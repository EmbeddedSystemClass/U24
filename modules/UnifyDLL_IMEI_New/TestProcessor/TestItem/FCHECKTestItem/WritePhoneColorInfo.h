#ifndef _C_WRITE_PHONE_COLOR_INFO_H_
#define _C_WRITE_PHONE_COLOR_INFO_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CWritePhoneColorInfo : public ITestItem
{
RF_DECLARE_DYNCREATE(CWritePhoneColorInfo)

// Data members
public:
protected:
private:
	int m_iRetry;
	std::map<std::string, std::string> m_mapCoverColor;


// Member functions
public:
	CWritePhoneColorInfo() {}
	~CWritePhoneColorInfo() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();
	bool ExecAdbOut(CString Command, char* output, char* ErrorCode);

};


#endif // End of #ifndef _C_WRITE_PHONE_COLOR_INFO_H_
