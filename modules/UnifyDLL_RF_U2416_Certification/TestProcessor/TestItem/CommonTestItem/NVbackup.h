#ifndef _C_NV_BACKUP_H_
#define _C_NV_BACKUP_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CNVbackup : public ITestItem
{
RF_DECLARE_DYNCREATE(CNVbackup)

// Data members
public:
protected:
private:


// Member functions
public:
	CNVbackup() {}
	~CNVbackup() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
};


#endif // End of #ifndef _C_NV_BACKUP_H_
