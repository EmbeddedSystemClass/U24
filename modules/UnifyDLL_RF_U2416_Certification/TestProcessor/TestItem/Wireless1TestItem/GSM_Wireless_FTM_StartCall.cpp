#include "stdafx.h"
#include "GSM_Wireless_FTM_StartCall.h"

#include "QLib.h"
#include "QLib_Defines.h"

RF_IMPLEMENT_DYNCREATE(CGSM_Wireless_FTM_StartCall)

bool CGSM_Wireless_FTM_StartCall::Run()
{
	m_strErrCode.clear() ;
    bool bRet = MainFunction() ;

    if (bRet) 
	{
        m_strMsg = "Success to Start GSM Call" ;
        TraceLog(MSG_INFO, m_strMsg) ;
        
        FactoryLog(bRet, "GSM StartCall", "Pass", m_strBand, m_strChannel, "", "", "", "", m_strMsg ) ;
        return true ;
    } 
	else 
	{
        if ( m_strErrCode.empty() )
		{
            m_strErrCode = ERR_GSMTEST_STARTCALL ;
		}

        FactoryLog(bRet, "GSM StartCall", m_strErrCode, m_strBand, m_strChannel, "", "", "", "", m_strMsg ) ;
        return false ;
    }
}

bool CGSM_Wireless_FTM_StartCall::MainFunction()
{
	// Reset GSM Tester.
    if (! m_pITesterDevice->Reset())
    {
        m_strMsg = "Fail to Reset Tester Device" ;
        TraceLog(MSG_ERROR, m_strMsg);
        return false;
    }

    // Initial Tester, and Remote Mode Display.
    if (! m_pITesterDevice->Initial())
    {
        m_strMsg = "Fail to Initial Tester Device" ;
        TraceLog(MSG_ERROR, m_strMsg);
        return false;
    }

    // Initial Tester for GSM (Secondary Address, RF IN/OUT port).
    if (! m_pITesterDevice->Initial_GSM())
    {
        m_strMsg = "Fail to Initial Tester Device for GSM" ;
        TraceLog(MSG_ERROR, m_strMsg);
        return false;
    }

    // Initialize, Set to GSM900 Signaling.
    m_pITesterDevice->GSM_SetMode(m_iBand, 1) ;

    // Initialize Parameters.
    if (! m_pITesterDevice->Initial_GSM_Param() )
    {
        m_strMsg = "Fail to set GSM Channel Parameters" ;
        TraceLog(MSG_ERROR, m_strMsg);
        return false;
    }

    // Set BS Signaling Channel.
    if (! m_pITesterDevice->GSM_SetCCH(m_iChannel))
    {
        m_strMsg = "Fail to set Control Channel" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false ;
    }

    if (! m_pITesterDevice->GSM_SetTCH(m_iChannel))
    {
        TraceLog(MSG_ERROR, "Fail to set Traffic Channel") ;
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
        m_strMsg = "Fail to Set MS Maximum Power" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
    }

    if (! m_pITesterDevice->GSM_SetTCHTimeSlot(m_iTCHTimeSlot) )
    {
        m_strMsg = "Fail to Set Traffic Channel Timeslot" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
    }

    if (! m_pITesterDevice->GSM_SetTCHTimeSlotPCL(m_iPCL) )
    {
        m_strMsg = "Fail to Set Timeslot PCL of Traffic Channel" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
    }

    if (! m_pITesterDevice->GSM_SetTCHTimeSlotLevel_StartCall(m_fPowerLevel) )
    {
		m_strMsg = "Fail to Set Timeslot Power Level of Traffic Channel";
        TraceLog(MSG_ERROR, m_strMsg);
        return false;
    }

    if (! m_pITesterDevice->SetSignalOnOff(true) )
    {
		m_strMsg = "Fail to set signal on for GSM";
        TraceLog(MSG_ERROR, m_strMsg );
        return false;
    }

    TraceLog(MSG_INFO, "Success to Send GSM Signal") ;



    int iRetry = m_iRetry ;
    bool bIsConnect = false ;

    do
    {
        if (m_pIPhone->IsConnected_QMSL())
        {
            bIsConnect = true ;
            break ;
        }

        --iRetry ;
        Sleep(500) ;
    } while ( 0<iRetry ) ;

    if (!bIsConnect)
    {
		m_strMsg = "Fail to Connect QMSL Phone.";
        TraceLog(MSG_ERROR, m_strMsg);
        return false ;
    }


    if (! m_pIPhone->QMSL_FTM_GSM_BER_StartSession(10000) )
    {
        Sleep(1000) ;
        if (! m_pIPhone->QMSL_FTM_GSM_BER_StartSession(10000) )
        {
			m_strMsg = "Fail to Start FTM_GSM_BER Session";
            TraceLog(MSG_ERROR, m_strMsg);
            return false ;
        }
    }

    if (! m_pIPhone->QMSL_FTM_GSM_BER_AssignBCCH(m_iChannel, m_iBand) )
    {
        Sleep(1000) ;
        if (! m_pIPhone->QMSL_FTM_GSM_BER_AssignBCCH(m_iChannel, m_iBand) )
        {
			m_strMsg = "Fail to Assign BCChannel";
            TraceLog(MSG_ERROR, m_strMsg);
            return false ;
        }
    }

    if (! m_pITesterDevice->GSM_MTCall() )
    {
		m_strMsg = "Fail on GSM_MTCall";
        TraceLog(MSG_ERROR,m_strMsg ) ;
        return false ;
    }
    
    if (! m_pIPhone->QMSL_FTM_GSM_BER_Handover_NoLoopbackConfig(m_iChannel, m_iBand, m_iTCHTimeSlot, m_iPCL) )
    {
        Sleep(1000) ;
        if (! m_pIPhone->QMSL_FTM_GSM_BER_Handover_NoLoopbackConfig(m_iChannel, m_iBand, m_iTCHTimeSlot, m_iPCL) )
        {
			m_strMsg = "Fail on QMSL_FTM_GSM_BER_Handover";
            TraceLog(MSG_ERROR, m_strMsg) ;
            return false ;
        }
    }

    // TrafficVaild
    iRetry = m_iRetry ;
    bIsConnect = false ;
    do
    {
        if (m_pIPhone->QMSL_FTM_GSM_BER_IsTrafficValid())
        {
            bIsConnect = true ;
            break ;
        }

        --iRetry ;
        Sleep(500) ;
    } while ( 0<iRetry ) ;

    if (!bIsConnect)
    {
		m_strMsg = "Fail Traffic is not Valid.";
        TraceLog(MSG_ERROR, m_strMsg);
        return false ;
    }


    TraceLog(MSG_INFO, "Success to Make GSM Call") ;

	return true;
}

bool CGSM_Wireless_FTM_StartCall::InitData(std::map<std :: string,std :: string> &paramMap)
{
	m_iRetry = 20 ;
    m_iTCHTimeSlot = 2 ;

    if (paramMap.find("Band") == paramMap.end())
	{
		m_strMsg = "<Band> is not found in xml";
		TraceLog(MSG_ERROR, m_strMsg);
        return false;
	}
    m_iBand = atoi(paramMap["Band"].c_str());
    m_strBand = "GSM" + paramMap["Band"];

    if (paramMap.find("Channel") == paramMap.end())
	{
		m_strMsg = "<Channel> is not found in xml";
		TraceLog(MSG_ERROR, m_strMsg);
        return false;
	}
    m_iChannel = atoi(paramMap["Channel"].c_str());
    m_strChannel = paramMap["Channel"];

    if (paramMap.find("PCL") == paramMap.end())
	{
		m_strMsg = "<PCL> is not found in xml";
		TraceLog(MSG_ERROR, m_strMsg);
        return false;
	}
    m_iPCL = atoi(paramMap["PCL"].c_str());

    if (paramMap.find("MaxPower") == paramMap.end())
	{
		m_strMsg = "<MaxPower> is not found in xml";
		TraceLog(MSG_ERROR, m_strMsg);
        return false;
	}
    m_fMaxPower = (float)atof(paramMap["MaxPower"].c_str());

    if (paramMap.find("PowerLevel") == paramMap.end())
	{
		m_strMsg = "<PowerLevel> is not found in xml";
		TraceLog(MSG_ERROR, m_strMsg);
        return false;
	}
    m_fPowerLevel = (float)atof(paramMap["PowerLevel"].c_str());

    TraceLog(MSG_INFO, "Success to GSM_StartCall InitData");

    return true;
}