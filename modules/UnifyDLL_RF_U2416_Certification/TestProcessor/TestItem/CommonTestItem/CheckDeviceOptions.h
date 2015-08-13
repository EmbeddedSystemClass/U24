#ifndef _C_CHECK_DEVICE_PTIONS_H_
#define _C_CHECK_DEVICE_PTIONS_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CCheckDeviceOptions : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CCheckDeviceOptions)


// Data members
public:
protected:
private:
	std::vector<std::string> m_vec_strDeviceOptions;

	std::string m_strMsg;
	std::string m_strErrCode;
	

// Member functions
public:
	CCheckDeviceOptions() {}
	~CCheckDeviceOptions() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_CHECK_DEVICE_PTIONS_H_
