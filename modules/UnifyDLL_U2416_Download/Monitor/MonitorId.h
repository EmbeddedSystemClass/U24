#ifndef _C_MONITORID_H_
#define _C_MONITORID_H_
#include <string>
//#include "MonitorRuncard.h"
#include "CommonDefine.h"

class CMonitorId 
{
public:  
	//member

private:
	//member
	std::string m_szId;
	std::string m_szErrMsg;
public:
	//function
    bool ReadId(void);
	std::string GetId(void);
	std::string GetErrMsg(void);
	CMonitorId(void);
	~CMonitorId(void);
};

#endif