#ifndef _C_FSG_PROTECT_H_
#define _C_FSG_PROTECT_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"


class CFSGProtect : public ITestItem
{
	RF_DECLARE_DYNCREATE(CFSGProtect)


	// Data members
public:
protected:
private:
	int m_iWaitTime;
	std::string m_strPICSName;
	char m_szCommand[1024];


	// Member functions
public:
	CFSGProtect() {}
	~CFSGProtect() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
};


#endif // End of #ifndef _C_FSG_PROTECT_H_
