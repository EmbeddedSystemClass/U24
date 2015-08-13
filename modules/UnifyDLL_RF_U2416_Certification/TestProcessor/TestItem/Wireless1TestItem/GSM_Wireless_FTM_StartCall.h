#ifndef _GSM_WIRELESS_FTM_START_CALL_H_
#define _GSM_WIRELESS_FTM_START_CALL_H_

#include "../../../CommonUtility/BaseObject/ITestProcessor.h"

class CGSM_Wireless_FTM_StartCall : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CGSM_Wireless_FTM_StartCall)

// Data members
public:
protected:
private:
    std::string     m_strErrCode ;
    std::string     m_strMsg ;
    int   m_iBand;
    int   m_iChannel;
    int   m_iPCL ;
    float m_fPowerLevel ;
    float m_fMaxPower ;
    int   m_iRetry ;
    int   m_iTCHTimeSlot ;


// Member functions
public:
	CGSM_Wireless_FTM_StartCall() {}
    ~CGSM_Wireless_FTM_StartCall() {}

    virtual bool InitData(std::map<std::string, std::string>& paramMap);
    virtual bool Run();

protected:
private:
	bool MainFunction();
};

#endif // End of #ifndef _GSM_WIRELESS_FTM_START_CALL_H_