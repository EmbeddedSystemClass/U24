#ifndef _C_GSM_WIRELESS_FTM_END_CALL_H_
#define _C_GSM_WIRELESS_FTM_END_CALL_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"

class CGSM_Wireless_FTM_EndCall : public ITestProcessor
{
    RF_DECLARE_DYNCREATE(CGSM_Wireless_FTM_EndCall)


        // Data members
public:
protected:
private:
    std::string     m_strErrCode ;
    std::string     m_strMsg ;

    // Member functions
public:
    CGSM_Wireless_FTM_EndCall() {}
    ~CGSM_Wireless_FTM_EndCall() {}

    virtual bool InitData(std::map<std::string, std::string>& paramMap);
    virtual bool Run() ;

protected:
private:
    bool MainFunction();
};


#endif // End of #ifndef _C_GSM_WIRELESS_FTM_END_CALL_H_