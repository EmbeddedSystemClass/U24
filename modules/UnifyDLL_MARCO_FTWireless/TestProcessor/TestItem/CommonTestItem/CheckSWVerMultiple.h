#ifndef _C_CHECK_SWVERSION_MULTIPLE_H_
#define _C_CHECK_SWVERSION_MULTIPLE_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"



class CCheckSWVerMultiple : public ITestItem
{
RF_DECLARE_DYNCREATE(CCheckSWVerMultiple)

// Data members
public:

typedef struct tagQPhoneThreadParam
{
	CCheckSWVerMultiple* pThisObject;
	int nQPhoneNumber;

} THREAD_PARAM, *LPTHREAD_PARAM;

protected:
private:
	int m_msWaitTime;
	std::string m_Adbcommand;
	std::string m_VersionType;

	HANDLE m_hQPhoneProcess[5];
	HANDLE g_MutexSWVerReport;

// Member functions
public:
	CCheckSWVerMultiple() {}
	~CCheckSWVerMultiple() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	void Report(bool bRes, std::string strMessage, int nQPhoneNumber,
				std::string strMeasured = "", std::string strErrorCode = "");
	static UINT AfxQPhoneThread(LPVOID lpParam);
	bool MainFunction(int nQPhoneNumber);
};


#endif // End of #ifndef _C_CHECK_SWVERSION_MULTIPLE_H_
