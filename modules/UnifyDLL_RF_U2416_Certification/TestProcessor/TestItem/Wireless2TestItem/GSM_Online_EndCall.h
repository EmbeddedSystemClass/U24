#ifndef _C_GSM_ONLINE_END_CALL_H_
#define _C_GSM_ONLINE_END_CALL_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CGSM_Online_EndCall : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CGSM_Online_EndCall)


        // Data members
public:
protected:
private:
    std::string     m_strBand ;
    std::string     m_strChannel ;
    std::string     m_strErrCode ;
    std::string     m_strMsg ;

    // Member functions
public:
    CGSM_Online_EndCall() {}
    ~CGSM_Online_EndCall() {}

    virtual bool InitData(std::map<std::string, std::string>& paramMap);
    virtual bool Run() ;

protected:
private:
    bool MainFunction();
};


#endif // End of #ifndef _C_GSM_ONLINE_END_CALL_H_
