#ifndef _C_WIFI_MODULE_ON_OFF_H_
#define _C_WIFI_MODULE_ON_OFF_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"

class CWifiModuleOnOFF : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CWifiModuleOnOFF)

private:
	std::string m_strWifiModule;
	std::string m_strMsg;
	int m_iRetryCount;

public:
	CWifiModuleOnOFF(void);
	virtual ~CWifiModuleOnOFF(void);
	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool Run(void);
};

#endif // End of #ifndef _C_WIFI_MODULE_ON_OFF_H_
