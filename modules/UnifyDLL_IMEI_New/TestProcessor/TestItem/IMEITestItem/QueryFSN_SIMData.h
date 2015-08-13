#ifndef _C_QUERY_FSN_SIMDATA_H_
#define _C_QUERY_FSN_SIMDATA_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CQueryFSN_SIMData : public ITestItem
{
RF_DECLARE_DYNCREATE(CQueryFSN_SIMData)

// Data members
public:
protected:
private:
	int m_iRetry;
	bool m_bNameServer;
	std::string m_strXMLName;
	std::string m_strSIMType;
	char m_szSIMType[32];
	char m_szIMEI[32];
	char m_szFSN[64];

	HMODULE m_hSSSO2Handle;

// Member functions
public:
	CQueryFSN_SIMData() {}
	~CQueryFSN_SIMData() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();
	bool QueryFSNFromPICS();
	bool QuerySIMData();
	bool DealFSN();
	bool GetFSNFromPICS();
	bool DealPICS_FSN(char *szFSN_D32_Pics, char *szFSN_Base64_Pics, char *szFSN_D16_Pics, char* szLength, char *szErrorCode, char *szErrorString);
	
	typedef bool (__cdecl * PFQueryFSNCode_PICS)(char *szInIMEI,char * szFSN,char *szRes);
	typedef bool (__cdecl * PFGetSIMData_PICS)(char* szSimType,char *szInIMEI,char * szFSN,char *szSimDataFile ,char *szSimDataFromPICS,char *szSimDataSize,char *szRes);
	typedef bool (__cdecl * PFGetFSNCode_PICS)(char *szInIMEI,char * szFSN,char *szRes);
};


#endif // End of #ifndef _C_QUERY_FSN_SIMDATA_H_
