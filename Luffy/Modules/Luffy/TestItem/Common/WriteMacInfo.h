#define PATH1 _T(".\\DB\\INI\\VMS_DBConfig.ini")

#ifndef _C_WRITE_MAC_INFO_H_
#define _C_WRITE_MAC_INFO_H_

#include "baseobject/itestitem.h"
#include "FacTestToolInterface.h"
#include <io.h>

class CWriteMacInfo : public ITestItem
{
	GP_DECLARE_DYNCREATE(CWriteMacInfo)
private:
	//member
	CGPProcessor    *m_pCGProc;
	CAndroidPhone	*m_pIPhone;

	std::string m_szWorkingDir;      //��¼CMD·��
	std::string m_szADBWorkingDir;
	std::string m_szExeName;
	std::string m_szStep;

	std::string     m_szPartNo;
	std::string     m_szMSN;
	std::string     m_szBTAddr;
	std::string     m_szMACAddr;
	std::string     m_szSWver;
	std::string     m_szHWver;
	std::string     m_szPVK;
	std::string     m_szErrCode;
	std::string     m_szErrMsg;
	std::string     m_szStatusOutput;
	std::string     m_szWifiAdd;
	std::string     m_szRemotServerPath;

	char     m_sz9JPartNo[31];
	char     m_szImagerVer[31];
	char     m_szCmdLine[512];

	bool	m_blnNeedPushOtaInfo;

	st_UIMessage m_st_UIMessage;
	std::string m_strMsg;

public:
	//function
	CWriteMacInfo(void);
	virtual ~CWriteMacInfo(void);

private:
	//function
	bool MainFunction(void);
	bool InitData(std::map<std::string, std::string>& paramMap);
	bool Run(int i_slot = 0);
	bool GetSWVersion(void);
	bool GetMacAddr(void);
	bool CheckImeiAndPartNo(void);
	bool UpdateIMEIStatus_I(void);
	bool UploadYrstation(void);
	bool ReadWifiAdd(void);
	bool WriteSn(void);
	bool ReadSn(void);
	bool CreatedVersionFile(void);
	bool PushDownLoadFile(void);
};

#endif