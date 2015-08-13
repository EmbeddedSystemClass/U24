#ifndef _C_FACTORY_RESET_H_
#define _C_FACTORY_RESET_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CFactoryReset : public ITestItem
{
RF_DECLARE_DYNCREATE(CFactoryReset)

// Data members
public:
protected:
private:

	bool m_bBackupFA;
	bool m_bBackupCFG;
	bool m_bBackupModemCFG;
// Member functions
public:
	CFactoryReset() {}
	~CFactoryReset() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();
	bool ExecAdbOut(CString Command, char* output, char* ErrorCode);
};


#endif // End of #ifndef _C_FACTORY_RESET_H_
