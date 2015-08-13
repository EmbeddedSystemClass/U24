#include "stdafx.h"
#include "WCDMA_Online_EndCall_II.h"


RF_IMPLEMENT_DYNCREATE(CWCDMA_Online_EndCall_II)


bool CWCDMA_Online_EndCall_II::Run()
{
    m_strErrCode.clear() ;
    bool bRet = MainFunction() ;

    if (bRet) 
	{
		m_strErrCode = "Pass" ;
        m_strMsg = "Success to Run CWCDMA_Online_EndCall" ;
        TraceLog(MSG_INFO, m_strMsg) ;

        FactoryLog(bRet, "WCDMA Online EndCall", "", "", "", "", "", "", "", m_strMsg ) ;
    } 
	else 
	{
        if ( m_strErrCode.empty() )
            m_strErrCode = FunErr_WirelessWCDMA_Online_EndCall_Fail ;

        FactoryLog(bRet, "WCDMA Online EndCall", m_strErrCode, "", "", "", "", "", "", m_strMsg ) ;
    }
	return bRet;
}


bool CWCDMA_Online_EndCall_II::MainFunction()
{
	if (! m_pITesterDevice->WCDMA_EndCall())
	{
		m_strMsg = "Fail to WCDMA online end call" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	TraceLog(MSG_INFO, "Success to WCDMA online end call") ;

	if (! m_pITesterDevice->SetSignalOnOff(false) )
	{
		m_strMsg = "Fail to Turn Off Signal." ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}
	TraceLog(MSG_INFO, "Success to Set Signal Off") ;
	Sleep(2000);

	if (! m_pITesterDevice->SetSignalOnOff(true) )
	{
		m_strMsg = "Fail to Turn On Signal." ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}
	TraceLog(MSG_INFO, "Success to Set Signal On") ;

	return true;
}


bool CWCDMA_Online_EndCall_II::InitData(std::map<std::string, std::string>& paramMap)
{
	return true;
}
