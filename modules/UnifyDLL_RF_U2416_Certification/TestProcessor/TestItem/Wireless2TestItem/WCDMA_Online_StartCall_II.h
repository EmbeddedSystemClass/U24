#ifndef _C_WCDMA_ONLINE_START_CALL_II_H_
#define _C_WCDMA_ONLINE_START_CALL_II_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CWCDMA_Online_StartCall_II : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CWCDMA_Online_StartCall_II)


// Data members
public:
protected:
private:
    std::string     m_strBand ;
    std::string     m_strChannel ;
    std::string     m_strMsg ;
    std::string     m_strErrCode ;
	
	int		m_iBand;
	int		m_iChannel;
	double	m_dOutputPower;
	bool	m_bSetAuthenticationON;
	int		m_iWaitForStartUp;

	std::string m_strSKey1, m_strSKey2;

// Member functions
public:
	CWCDMA_Online_StartCall_II() {}
	~CWCDMA_Online_StartCall_II() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool Run();

protected:
private:
	bool MainFunction() ;
    bool WCDMA_StartCall();
	bool SetPhoneSecurity(bool OnOff);
};


#endif // End of #ifndef _C_WCDMA_ONLINE_START_CALL_II_H_
