#ifndef _C_GSM_ONLINE_HANDOVER_H_
#define _C_GSM_ONLINE_HANDOVER_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CGSM_Online_Handover : public ITestProcessor
{
    RF_DECLARE_DYNCREATE(CGSM_Online_Handover)


        // Data members
public:
protected:
private:
    std::string     m_strBand ;
    std::string     m_strChannel ;
    std::string     m_strErrCode ;
    std::string     m_strMsg ;
	std::string     m_strTestItem ;
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
    CGSM_Online_Handover() {}
    ~CGSM_Online_Handover() {}

    virtual bool InitData(std::map<std::string, std::string>& paramMap);
    virtual bool Run();

protected:
private:
    bool MainFunction();
	bool CheckConnectionAndReCall();
	int GetCMUMode(int iBand);
};


#endif // End of #ifndef _C_GSM_ONLINE_HANDOVER_H_
