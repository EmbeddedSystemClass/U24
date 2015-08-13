#ifndef _C_QUERY_IMEI_STATUS_H_
#define _C_QUERY_IMEI_STATUS_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CQueryIMEIStatus : public ITestItem
{
RF_DECLARE_DYNCREATE(CQueryIMEIStatus)

// Data members
public:
protected:
private:
	int m_iRetry;

	HMODULE m_hSSSO2Handle;

// Member functions
public:
	CQueryIMEIStatus() {}
	~CQueryIMEIStatus() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();

	typedef bool (__cdecl * PFQueryIMEI_SSSO2)(char *szStationtState,
												  char *pIMEI, char *szMSN, char *szBTAddr, char *szMACAddr,
												  char *szSWver, char *szHWver,
												  char *szErrCode, char *szErrMsg, char *szStatusOutput);
};


#endif // End of #ifndef _C_QUERY_IMEI_STATUS_H_
