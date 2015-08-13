#ifndef _C_DISCONNECT_QMSL_H_
#define _C_DISCONNECT_QMSL_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CDisconnectQMSL : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CDisconnectQMSL)

// Data members
public:
protected:
private:

	std::string m_strErrCode ;
	std::string m_strMsg;


// Member functions
public:
	CDisconnectQMSL() {}
	~CDisconnectQMSL() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_DISCONNECT_QMSL_H_
