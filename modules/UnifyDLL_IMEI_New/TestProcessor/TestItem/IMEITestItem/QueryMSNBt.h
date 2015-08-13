#ifndef _C_QUERY_MSN_BT_H_
#define _C_QUERY_MSN_BT_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CQueryMSNBt : public ITestItem
{
RF_DECLARE_DYNCREATE(CQueryMSNBt)

// Data members
public:
protected:
private:
	int m_iRetry;
	std::string m_strStation;

	HMODULE m_hSSSO2Handle;

// Member functions
public:
	CQueryMSNBt() {}
	~CQueryMSNBt() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();

	bool CheckIMEIStatus(char *szStatus_SSSO2, char *szIMEI_Scanner, char *szIMEI_Mobile, char *szSave_SAVE_PICS_FILE_FLAG, char *szErrorCode, char *szErrorString);
	bool CheckBTLength(char *szBT_Addr,char* szLength, char *szErrorCode, char *szErrorString);
	bool CheckMACLength(char *szMAC_Addr,char* szLength, char *szErrorCode, char *szErrorString);

	typedef bool (__cdecl * PFQueryIMEI_SSSO2_V2)(char *szStationtState,
												  char *pIMEI, char *szMSN, char *szBTAddr, char *szMACAddr,
												  char *szSWver, char *szHWver, char *szPVK,
												  char *szErrCode, char *szErrMsg, char *szStatusOutput);
};


#endif // End of #ifndef _C_QUERY_MSN_BT_H_
