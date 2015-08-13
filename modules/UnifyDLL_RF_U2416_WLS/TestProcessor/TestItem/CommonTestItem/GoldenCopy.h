#ifndef _C_GOLDER_COPY_H_
#define _C_GOLDER_COPY_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CGoldenCopy : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CGoldenCopy)

// Data members
public:
protected:
private:

	std::string     m_strErrCode ;
    std::string     m_strMsg ;

// Member functions
public:
	CGoldenCopy() {}
	~CGoldenCopy() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();
};


#endif // End of #ifndef _C_GOLDER_COPY_H_
