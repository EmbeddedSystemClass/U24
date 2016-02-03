#ifndef _C_MONITORWBCMODEL_H_
#define _C_MONITORWBCMODEL_H_
#include <string>
#include "../../../CommonUtility/CommonDefine.h"

class CMonitorWbcModel
{
private:  //member
	std::string m_szWbcFileName;
	std::string m_szDdcFileName;
	std::string m_szMonitorPort;
	std::string m_szInfo;
	std::string m_szErrMsg;
public:  //member

public:  //function
	//bool GetMonitorInfoByPartNo(void);
	bool SetWbcFileName(std::string szWbcFileName);
	std::string GetWbcFileName(void);
	std::string GetDdcFileName(void);
	std::string GetMonitorPort(void);
	bool SearchWbcFileName(std::string szPartNo);
	std::string GetMonitorEep(void);
	std::string GetErrMsg(void);
	CMonitorWbcModel(void);
	 virtual ~CMonitorWbcModel(void);
};

#endif