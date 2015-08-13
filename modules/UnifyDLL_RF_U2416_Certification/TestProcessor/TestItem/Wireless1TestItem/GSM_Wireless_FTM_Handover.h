#ifndef _C_GSM_WIRELESS_FTM_HANDOVER_H_
#define _C_GSM_WIRELESS_FTM_HANDOVER_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CGSM_Wireless_FTM_Handover : public ITestProcessor
{
    RF_DECLARE_DYNCREATE(CGSM_Wireless_FTM_Handover)


        // Data members
public:
protected:
private:
    std::string     m_strBand ;
    std::string     m_strChannel ;
    std::string     m_strErrCode ;
    std::string     m_strMsg ;
    int     m_iBand;
    int     m_iChannel;
    int     m_iPCL ;
    float   m_fPowerLevel ;
    float   m_fMaxPower ;
    int     m_iPreBand ;
    int     m_iRetry ;
    int     m_iTCHTimeSlot ;


    // Member functions
public:
    CGSM_Wireless_FTM_Handover() {}
    ~CGSM_Wireless_FTM_Handover() {}

    virtual bool InitData(std::map<std::string, std::string>& paramMap);
    virtual bool Run();

protected:
private:
    bool MainFunction();
};


#endif // End of #ifndef _C_GSM_WIRELESS_FTM_HANDOVER_H_
