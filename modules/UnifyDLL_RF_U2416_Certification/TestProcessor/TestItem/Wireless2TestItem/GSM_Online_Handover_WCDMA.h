#ifndef _C_GSM_ONLINE_HANDOVER_WCDMA_H_
#define _C_GSM_ONLINE_HANDOVER_WCDMA_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CGSM_Online_Handover_WCDMA : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CGSM_Online_Handover_WCDMA)


// Data members
public:
protected:
private:
    std::string     m_strBand ;
    std::string     m_strChannel ;
    std::string     m_strMsg ;
	std::string     m_strErrCode ;
	std::string		m_strSKey1, m_strSKey2;

	bool			m_bSetAuthenticationON;
	
	double			m_dOutputPower;
	double			m_dMaxMSPower;
	double			m_dMSTargetPower;
	int				m_iTargetBand;
	int				m_iTargetChannel;
	int				m_iPreBand ;


// Member functions
public:
	CGSM_Online_Handover_WCDMA() {}
	~CGSM_Online_Handover_WCDMA() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);
    virtual bool Run();

protected:
private:
    bool MainFunction() ;
	bool WCDMA_StartCall();
	bool MainFunction2();

};


#endif // End of #ifndef _C_GSM_ONLINE_HANDOVER_WCDMA_H_
