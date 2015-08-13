#include "stdafx.h"
#include "GSM_Online_EndCall.h"


RF_IMPLEMENT_DYNCREATE(CGSM_Online_EndCall)


bool CGSM_Online_EndCall::Run()
{
    m_strErrCode.clear() ;
    bool bRet = MainFunction() ;

    if (bRet) {
        m_strMsg = "Success to Run GSM EndCall" ;
        TraceLog(MSG_INFO, m_strMsg) ;

        FactoryLog(bRet, "GSM EndCall", "", "", "", "", "", "", "", m_strMsg ) ;
        return true ;
    } else {
        if ( m_strErrCode.empty() )
            m_strErrCode = FunErr_WirelessGSM_Online_EndCall_Fail ;

        FactoryLog(bRet, "GSM EndCall", m_strErrCode, "", "", "", "", "", "", m_strMsg ) ;
        return false ;
    }
}


bool CGSM_Online_EndCall::MainFunction()
{

    if (! m_pITesterDevice->SetSignalOnOff(false) )
    {
        m_strMsg = "Fail to Turn Off Signal." ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }

	TraceLog(MSG_INFO, "Success to End GSM Call") ;

    return true;
}


bool CGSM_Online_EndCall::InitData(std::map<std::string, std::string>& paramMap)
{
    return true;
}
