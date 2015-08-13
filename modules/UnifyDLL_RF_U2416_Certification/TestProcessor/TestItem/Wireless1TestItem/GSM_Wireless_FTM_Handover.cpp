#include "stdafx.h"
#include "GSM_Wireless_FTM_Handover.h"


RF_IMPLEMENT_DYNCREATE(CGSM_Wireless_FTM_Handover)


bool CGSM_Wireless_FTM_Handover::Run()
{
    m_strErrCode.clear() ;
    bool bRet = MainFunction() ;

    if (bRet) {
        m_strMsg = "Success to Run GSM Handover" ;
        TraceLog(MSG_INFO, m_strMsg) ;

        FactoryLog(bRet, "GSM Handover", "", m_strBand, m_strChannel, "", "", "", "", m_strMsg ) ;
        return true ;
    } else {
        if ( m_strErrCode.empty() )
            m_strErrCode = ERR_GSMTEST_HANDOVER ;

        FactoryLog(bRet, "GSM Handover", m_strErrCode, m_strBand, m_strChannel, "", "", "", "", m_strMsg ) ;
        return false ;
    }
}



bool CGSM_Wireless_FTM_Handover::MainFunction()
{
    // Query Previous Band
    m_iPreBand =  m_pITesterDevice->GSM_QueryBand() ;

    if ( m_iBand != m_iPreBand )
    {
        m_pITesterDevice->GSM_SetMode(m_iBand, 1) ;

        if (! m_pITesterDevice->GSM_SetTCH(m_iChannel))
        {
            m_strMsg = "Fail to set BS Signaling Traffic Channel" ;
            TraceLog(MSG_ERROR, m_strMsg) ;
            return false ;
        }

        if (! m_pITesterDevice->GSM_SetTCHTimeSlot(m_iTCHTimeSlot) )
        {
            m_strMsg = "Fail to Set Traffic Channel Time Slot" ;
            TraceLog(MSG_ERROR, m_strMsg);
            return false;
        }

        m_pITesterDevice->GSM_SetMode(m_iPreBand, 1) ;

        if (! m_pITesterDevice->GSM_SetHANDoverTarget(m_iBand) )
        {
            m_strMsg = "Fail to Set HandOver Target" ;
            TraceLog(MSG_ERROR, m_strMsg) ;
            return false ;
        }

        if (! m_pITesterDevice->GSM_Handover() )
        {
            m_strMsg = "Fail to Handover" ;
            TraceLog(MSG_ERROR, m_strMsg) ;
            return false ;
        }
        Sleep(3000) ;

        m_pITesterDevice->GSM_SetMode(m_iBand, 1) ;
    }
    else
    {
        if (! m_pITesterDevice->GSM_SetTCH_Handover(m_iChannel) )
        {
            m_strMsg = "Fail to set Signaling Traffic Channel during Handover" ;
            TraceLog(MSG_ERROR, m_strMsg) ;
            return false ;
        }
    }


    if (! m_pITesterDevice->GSM_SetTCHTimeSlotLevel_Handover(m_fPowerLevel) )
    {
        m_strMsg = "Fail to Set Traffic Channel Time slot Level, During Handover" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }

    if (! m_pIPhone->QMSL_FTM_GSM_BER_Handover_NoLoopbackConfig(m_iChannel, m_iBand, 2, m_iPCL) )
    {
        Sleep(1000) ;
        if (! m_pIPhone->QMSL_FTM_GSM_BER_Handover_NoLoopbackConfig(m_iChannel, m_iBand, 2, m_iPCL) )
        {
            m_strMsg = "Fail on QMSL_FTM_GSM_BER_Handover" ;
            TraceLog(MSG_ERROR, m_strMsg) ;
            return false ;
        }
    }

    if (! m_pITesterDevice->GSM_SetTCHTimeSlotPCL_Handover(m_iPCL) )
    {
        m_strMsg = "Fail to set Traffic Channel Time Slot PCL during Handover" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }

    std::string strBandName;
    std::string strRange;

    // Set TX/RX Cable Loss
    if (GSM_GetBandNameForCableLoss(m_iBand, strBandName) && GSM_GetRangeForTxRxCableLoss(m_iBand, m_iChannel, strRange))
    {
        if (! m_pITesterDevice->SetInputLoss(GetCableLoss("TX", strBandName, strRange)))
        {
            m_strMsg = "Fail to set GSM TX cable loss" ;
            TraceLog(MSG_ERROR, m_strMsg);
            return false;
        }

        if (! m_pITesterDevice->SetOutputLoss(GetCableLoss("RX", strBandName, strRange)))
        {
            m_strMsg = "Fail to set GSM RX cable loss" ;
            TraceLog(MSG_ERROR, m_strMsg) ;
            return false;
        }
    }
    else
    {
        m_strMsg = "Fail to get GSM band name & range for Tx/Rx cable loss" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
    }

    if (! m_pITesterDevice->GSM_SetMSMaxPower(m_fMaxPower) )
    {
        m_strMsg = "Fail to Set Input Level during Handover" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }

    TraceLog(MSG_INFO, "Success to Set Handover Parameters") ;



    if (! m_pIPhone->QMSL_FTM_GSM_BER_ClearStatus() )
    {
        m_strMsg = "Fail to Clear GSM BER Status" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }

    if (! m_pIPhone->QMSL_FTM_GSM_BER_Handover_NoLoopbackConfig(m_iChannel, m_iBand, m_iTCHTimeSlot, m_iPCL) )
    {
        m_strMsg = "Fail to set QLIB_FTM_GSM_BER_Handover" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }

    if (! m_pITesterDevice->GSM_CheckCallStatusCED() )
    {
        if (! m_pITesterDevice->GSM_CheckCallStatusCEST() )
        {
            m_strMsg = "Fail to Check Call Status" ;
            TraceLog(MSG_ERROR, m_strMsg) ;
            return false ;
        }
    }

    // TrafficVaild
    int iRetry = m_iRetry ;
    bool bRet = false ;
    do
    {
        if (m_pIPhone->QMSL_FTM_GSM_BER_IsTrafficValid()) {
            bRet = true ;
            break ;
        }

        --iRetry ;
        Sleep(500) ;
    } while ( 0<iRetry ) ;

    if (!bRet)
    {
        m_strMsg = "Fail Traffic is not Valid." ;
        TraceLog(MSG_ERROR, m_strMsg);
        return false ;
    }


    TraceLog(MSG_INFO, "GSM Handover Success.") ;

    return true;
}


bool CGSM_Wireless_FTM_Handover::InitData(std::map<std::string, std::string>& paramMap)
{
    // Default Value
    m_iRetry = 20 ;
    m_iTCHTimeSlot = 2 ;
    m_iPreBand = 0 ;


    // Get From XML
    if (paramMap.find("Band") == paramMap.end()) {
        m_strMsg = "Fail to Get <Band> from XML" ;
        return false; }
    m_iBand = atoi(paramMap["Band"].c_str());
    m_strBand = "GSM" + paramMap["Band"];

    if (paramMap.find("Channel") == paramMap.end()) {
        m_strMsg = "Fail to Get <Channel> from XML" ;
        return false; }
    m_iChannel = atoi(paramMap["Channel"].c_str());
    m_strChannel = paramMap["Channel"];

    if (paramMap.find("PCL") == paramMap.end()) {
        m_strMsg = "Fail to Get <PCL> from XML" ;
        return false; }
    m_iPCL = atoi(paramMap["PCL"].c_str());

    if (paramMap.find("MaxPower") == paramMap.end()) {
        m_strMsg = "Fail to Get <MaxPower> from XML" ;
        return false; }
    m_fMaxPower = (float)atof(paramMap["MaxPower"].c_str());

    if (paramMap.find("PowerLevel") == paramMap.end()) {
        m_strMsg = "Fail to Get <PowerLevel> from XML" ;
        return false; }
    m_fPowerLevel = (float)atof(paramMap["PowerLevel"].c_str());


    TraceLog(MSG_INFO, "Success to GSM_Handover InitData") ;

    return true;
}

