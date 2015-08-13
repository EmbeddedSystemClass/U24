#ifndef _C_FT2_GSM_START_CALL_H_
#define _C_FT2_GSM_START_CALL_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CGSM_Online_StartCall : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CGSM_Online_StartCall)

// Data members
public:
protected:
private:
    std::string     m_strBand ;
	std::string		strBandName;
    std::string     m_strChannel ;
    std::string     m_strErrCode ;
    std::string     m_strMsg ;
	std::string		m_strModemPort;

    int		m_iBand;
    int		m_iChannel;
    int		m_iPCL ;
    float	m_fPowerLevel ;
    float	m_fMaxPower ;
    int		m_iRetry ;
    int		m_iTCHTimeSlot ;
	int		m_iWaitForStartUp;
	bool	m_bReset ;
	bool    m_bConnectPhone;

	int		m_iWaitTimeAfterConnect;
	bool	m_bChangeFakeSecurity;

    // Member functions
public:
    CGSM_Online_StartCall() {}
    ~CGSM_Online_StartCall() {}

    virtual bool InitData(std::map<std::string, std::string>& paramMap);
    virtual bool Run();

protected:
private:
    bool MainFunction();
	bool GSM_StartCall();
	bool GSM_AutoAnswer();
	bool SetAllBandMS();
	bool SetPhoneSecurity(bool OnOff);
	bool InitTestCMU();
	bool ResetPhone();

};


#endif // End of #ifndef _C_GSM_START_CALL_H_
