#ifndef _C_SEND_SYNC_QMSL_H_
#define _C_SEND_SYNC_QMSL_H_

#include "../../../CommonUtility/BaseObject/ITestItem.h"

class CSendSync_QMSL : public ITestItem
{
	RF_DECLARE_DYNCREATE(CSendSync_QMSL)

private:
	std::string m_strPICSName;
	std::string m_strMsg;
	std::string m_strErrCode;
	std::vector<std::string> m_vec_strDIAGCommnad;
	std::vector<std::string> m_vec_strResponse;

public:
	CSendSync_QMSL() {}
	~CSendSync_QMSL() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
	bool MainFunction();
};

#endif // End of #ifndef _C_SEND_SYNC_QMSL_H_
