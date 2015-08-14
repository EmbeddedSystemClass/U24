#ifndef _C_MONITORSO_H_
#define _C_MONITORSO_H_
#include <string>

class CMonitorSo
{
private:  //member
	std::string m_szSo;
	std::string m_szPartNo;
	int m_iSoQty;
	std::string m_szSoType;
	std::string m_szCustomer;
	std::string m_szSoVer;
	std::string m_szSoInfo;
	std::string m_szErrMsg;
public:
	//member

public:  //function
	bool SetSo(std::string szSo);
	bool GetSoByRuncard(std::string szRuncard);
	 CMonitorSo();
	 virtual ~CMonitorSo(void);

	 std::string GetSo(void);
	 //std::string GetPartNo(void);
	 int         GetSoQty(void);
	 std::string GetSoType(void);
	 std::string GetCustomer(void);
	 std::string GetSoVer(void);
	 std::string GetSoInfo(void);
private:
	//function
	bool SearchSoInfo(void);
};

#endif