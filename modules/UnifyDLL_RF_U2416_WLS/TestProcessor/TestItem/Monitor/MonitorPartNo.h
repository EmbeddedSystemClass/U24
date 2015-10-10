#ifndef _C_MONITORPARTNO_H_
#define _C_MONITORPARTNO_H_
#include <string>
//#include "..\..\UnifyInterface\FacTestToolInterface.h"
#include "../../../CommonUtility/CommonDefine.h"

class CMonitorPartNo
{
private:  //member
	std::string m_szPartNo;
	std::string m_szErrMsg;
public:

public:  //function
	//bool GetMonitorInfoByPartNo(void);
	bool SetPartNo(std::string szPartNo);
	std::string GetPartNo(void);
	bool SearchPartNo(std::string szSo);
	std::string GetErrMsg(void);
	CMonitorPartNo(void);
	 virtual ~CMonitorPartNo(void);
};

#endif