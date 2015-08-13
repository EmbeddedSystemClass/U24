#ifndef _C_Sleep_Ctrl_H_
#define _C_Sleep_Ctrl_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CSleepCtrl : public ITestItem
{
RF_DECLARE_DYNCREATE(CSleepCtrl)


// Data members
public:
protected:
private:
	//parameters
	int m_i_sleep;

// Member functions
public:
	CSleepCtrl() {}
	~CSleepCtrl() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool SleepCtrl();
};


#endif // End of #ifndef _C_Sleep_Ctrl_H_
