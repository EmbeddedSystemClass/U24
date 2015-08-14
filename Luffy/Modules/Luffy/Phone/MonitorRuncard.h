#ifndef _C_MONITORRUNCARD_H_
#define _C_MONITORRUNCARD_H_
#include <string>

class CMonitorRuncard
{
public:
	//member
private:
	//member
	std::string m_szRuncard;
	std::string m_szErrMsg;
public:
	//function
	CMonitorRuncard(void);
	~CMonitorRuncard(void);
	bool GetRunCardById(std::string szId);
	std::string GetRuncard(void);
	std::string GetErrMsg(void);
};

#endif