#ifndef _C_UPDATE_IMEI_STATUS_H_
#define _C_UPDATE_IMEI_STATUS_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CUpdateIMEIStatus : public ITestItem
{
RF_DECLARE_DYNCREATE(CUpdateIMEIStatus)

// Data members
public:
protected:
private:
	int m_iRetry;
	string m_strStatus;

	HMODULE m_hSSSO2Handle;

// Member functions
public:
	CUpdateIMEIStatus() {}
	~CUpdateIMEIStatus() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();
	bool Update_I();
	bool Update_F();

	typedef bool (__cdecl * PFUpdateIMEIStatus_I)(char *pIMEI,char *szSWver,char *szErrCode, char *szErrMsg);
	typedef bool (__cdecl * PFUpdateIMEIStatus_F)(char *pIMEI,char *szSWver,char *szHWver,char *szMal,char *szErrCode, char *szErrMsg);
};


#endif // End of #ifndef _C_UPDATE_IMEI_STATUS_H_
