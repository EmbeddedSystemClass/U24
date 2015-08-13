#ifndef _C_CHECK_CREATE_IMEI_H_
#define _C_CHECK_CREATE_IMEI_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CCheckCreateIMEI : public ITestItem
{
RF_DECLARE_DYNCREATE(CCheckCreateIMEI)

// Data members
public:
protected:
private:
	int m_iRetry;

	HMODULE m_hSSSO2Handle;

// Member functions
public:
	CCheckCreateIMEI() {}
	~CCheckCreateIMEI() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();

	typedef bool (__cdecl * PFCheck_MAL_PN_SSSO2)(char *szInIMEI,char *szPort,char *szInMal,char *szErrCode, char *szErrMsg);
	typedef bool (__cdecl * PFCreate_IMEI_PICS)(char *szInIMEI,char * szRes);
};


#endif // End of #ifndef _C_CHECK_CREATE_IMEI_H_
