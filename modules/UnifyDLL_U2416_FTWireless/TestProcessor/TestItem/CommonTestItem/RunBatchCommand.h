#ifndef _C_RUN_BATCH_FILE_H_
#define _C_RUN_BATCH_FILE_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CRunBatchCommand : public ITestItem
{
RF_DECLARE_DYNCREATE(CRunBatchCommand)

// Data members
public:
protected:
private:
	int m_msWaitTime;
	std::string m_str_file;

// Member functions
public:
	CRunBatchCommand() {}
	~CRunBatchCommand() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_RUN_BATCH_FILE_H_
