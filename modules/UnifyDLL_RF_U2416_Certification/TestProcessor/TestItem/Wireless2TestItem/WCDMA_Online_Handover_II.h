#ifndef _C_WCDMA_ONLINE_HANDOVER_II_H_
#define _C_WCDMA_ONLINE_HANDOVER_II_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"


class CWCDMA_Online_Handover_II : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CWCDMA_Online_Handover_II)


// Data members
public:
protected:
private:
    std::string     m_strBand ;
    std::string     m_strChannel ;
    std::string     m_strMsg ;
    std::string     m_strErrCode ;
	int m_iTargetBand;
	int m_iTargetChannel;


// Member functions
public:
	CWCDMA_Online_Handover_II() {}
	~CWCDMA_Online_Handover_II() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);
    virtual bool Run();

protected:
private:
    bool MainFunction() ;
	bool WCDMA_StartCall();
};


#endif // End of #ifndef _C_WCDMA_ONLINE_HANDOVER_H_
