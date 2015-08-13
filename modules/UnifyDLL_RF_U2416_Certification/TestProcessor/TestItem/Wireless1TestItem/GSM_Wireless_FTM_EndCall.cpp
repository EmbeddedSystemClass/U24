#include "stdafx.h"
#include "GSM_Wireless_FTM_EndCall.h"


RF_IMPLEMENT_DYNCREATE(CGSM_Wireless_FTM_EndCall)


bool CGSM_Wireless_FTM_EndCall::Run()
{
    m_strErrCode.clear() ;
    bool bRet = MainFunction() ;

    if (bRet) {
        m_strMsg = "Success to Run GSM EndCall" ;
        TraceLog(MSG_INFO, m_strMsg) ;

        FactoryLog(bRet, "GSM EndCall", "", m_strBand, m_strChannel, "", "", "", "", m_strMsg ) ;
        return true ;
    } else {
        if ( m_strErrCode.empty() )
            m_strErrCode = ERR_GSMTEST_END ;

        FactoryLog(bRet, "GSM EndCall", m_strErrCode, m_strBand, m_strChannel, "", "", "", "", m_strMsg ) ;
        return false ;
    }
}


bool CGSM_Wireless_FTM_EndCall::MainFunction()
{
    if (! m_pIPhone->QMSL_FTM_GSM_BER_ReleaseTCH() )
    {
        m_strMsg = "Fail to release Traffic Channel." ;
        TraceLog(MSG_ERROR, m_strMsg);
        return false;
    }

	Sleep(500);

    if (! m_pIPhone->QMSL_FTM_GSM_BER_StopSession() )
    {
        m_strMsg = "Fail to Stop Session." ;
        TraceLog(MSG_ERROR, m_strMsg);
        return false;
    }

    if (! m_pIPhone->Disconnect_QMSL() )
    {
        m_strMsg = "Fail to Disconnect QMSL." ;
        TraceLog(MSG_ERROR, m_strMsg);
        return false;
    }

    if (! m_pIPhone->Disconnect() )
    {
        m_strMsg = "Fail to Disconnect Phone" ;
        TraceLog(MSG_ERROR, m_strMsg);
        return false;
    }

    if (! m_pITesterDevice->SetSignalOnOff(false) )
    {
        m_strMsg = "Fail to Turn Off Signal." ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }


    TraceLog(MSG_INFO, "Success to End GSM Call") ;

    return true;
}


bool CGSM_Wireless_FTM_EndCall::InitData(std::map<std::string, std::string>& paramMap)
{
    return true;
}
