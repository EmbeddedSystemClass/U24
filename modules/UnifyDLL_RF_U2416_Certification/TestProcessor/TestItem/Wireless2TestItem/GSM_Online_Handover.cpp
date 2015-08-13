#include "stdafx.h"
#include "GSM_Online_Handover.h"


RF_IMPLEMENT_DYNCREATE(CGSM_Online_Handover)


bool CGSM_Online_Handover::Run()
{
    m_strErrCode.clear() ;
    bool bRet = MainFunction() ;

    if (bRet) 
	{
        m_strMsg = "Success to Run GSM Handover" ;
		m_strErrCode = "Pass";
    } 
	else 
	{
        if ( m_strErrCode.empty() )
		{
            m_strErrCode = FunErr_WirelessGSM_Online_Handover_Fail ;
		}
    }

	TraceLog(MSG_INFO, m_strMsg) ;
	FactoryLog(bRet, m_strTestItem, m_strErrCode, m_strBand, m_strChannel, "", "", "", "", m_strMsg ) ;
	SetPICSDate(m_strTestItem,m_strErrCode);
	return bRet;
}



bool CGSM_Online_Handover::MainFunction()
{
	// Query Previous Band
	m_iPreBand =  m_pITesterDevice->GSM_QueryBand() ;

	if (! m_pITesterDevice->GSM_SetBandIndicator(m_iBand) )
	{
		m_strMsg = "Fail to Set Band Indicator, During Handover" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}

	/*if (! m_pITesterDevice->GSM_SetTCHTimeSlotLevel_Handover(m_fPowerLevel) )
	{
		m_strMsg = "Fail to Set Traffic Channel Time slot Level, During Handover" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}*/

	std::string strRange;

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

		if (! m_pITesterDevice->GSM_SetTCHTimeSlotPCL(m_iPCL) )
		{
			m_strMsg = "Fail to Set PCL" ;
			TraceLog(MSG_ERROR, m_strMsg);
			return false;
		}

		// Set TX/RX Cable Loss
		if (GSM_GetRangeForTxRxCableLoss(m_iBand, m_iChannel, strRange))
		{
			if (! m_pITesterDevice->SetInputLoss(GetCableLoss("TX", "Wireless" + m_strBand, strRange)))
			{
				m_strMsg = "Fail to set GSM TX cable loss" ;
				TraceLog(MSG_ERROR, m_strMsg);
				return false;
			}

			if (! m_pITesterDevice->SetOutputLoss(GetCableLoss("RX", "Wireless" + m_strBand, strRange)))
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

        Sleep(6000) ;

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

	bool b_online = true;

	if (! m_pITesterDevice->GSM_CheckCallStatusCED() )
	{
		if (! m_pITesterDevice->GSM_CheckCallStatusCEST() )
		{
			m_strMsg = "Fail to Check Call Status" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			b_online = false ;;
		}
	}	

	if (!b_online)
	{
		if (!CheckConnectionAndReCall())
		{
			return false;
		}
		else
		{
			b_online = true;
		}

	}

	if (! m_pITesterDevice->GSM_SetTCHTimeSlotPCL_Handover(m_iPCL) )
	{
		m_strMsg = "Fail to set Traffic Channel Time Slot PCL during Handover" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}

	if (! m_pITesterDevice->GSM_SetMSMaxPower(m_fMaxPower) )
	{
		m_strMsg = "Fail to Set Input Level during Handover" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}
    

    TraceLog(MSG_INFO, "Success to Set Handover Parameters") ;

    if (! m_pITesterDevice->GSM_CheckCallStatusCED() )
    {
        if (! m_pITesterDevice->GSM_CheckCallStatusCEST() )
        {
            m_strMsg = "Fail to Check Call Status" ;
            TraceLog(MSG_ERROR, m_strMsg) ;
            b_online = false ;
        }
    }

	if (!b_online)
	{
		if (!CheckConnectionAndReCall())
		{
			return false;
		}
		
	}
	
    TraceLog(MSG_INFO, "GSM Handover Success.") ;

    return true;
}

bool CGSM_Online_Handover::CheckConnectionAndReCall()
{
	bool bRet = true;

	//m_pITesterDevice->GSM_SetMode(m_iPreBand, 1) ;

	if (! m_pITesterDevice->SetSignalOnOff(GetCMUMode(m_iPreBand),false))
	{
		m_strMsg = "Fail to Turn off phone call." ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}


	m_pITesterDevice->GSM_SetMode(m_iBand, 1) ;

	if (! m_pITesterDevice->GSM_SetCCH(m_iChannel))
	{
		m_strMsg = "Fail to set Control Channel" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}

	if (! m_pITesterDevice->GSM_SetTCH(m_iChannel))
	{
		m_strMsg = "Fail to set Traffic Channel";
		TraceLog(MSG_ERROR,m_strMsg);
		return false ;
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

	if (! m_pITesterDevice->SetSignalOnOff(GetCMUMode(m_iBand),true))
	{
		m_strMsg = "Fail to Turn signal on before Re-Make phone call." ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	/*if (! m_pITesterDevice->GSM_MTCall() )
	{
		TraceLog(MSG_ERROR, "Fail on GSM_MTCall") ;
		return false ;
	}*/

	if (! m_pITesterDevice->GSM_MT_Call())
	{
		if (! m_pITesterDevice->GSM_MT_Call())
		{
			m_strMsg = "Fail to Re-make GSM call" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			bRet = false;
		}
		else
			bRet = true;
	}


	return bRet;
}


bool CGSM_Online_Handover::InitData(std::map<std::string, std::string>& paramMap)
{
    // Default Value
    m_iRetry = 20 ;
    m_iTCHTimeSlot = 2 ;
    m_iPreBand = 0 ;


    // Get From XML
    if (paramMap.find("Band") == paramMap.end()) 
	{
        m_strMsg = "Fail to Get <Band> from XML" ;
        return false; 
	}
    m_iBand = atoi(paramMap["Band"].c_str());
	m_strBand = "GSM" + paramMap["Band"];
	m_strTestItem = m_strBand + "_Handover";

    if (paramMap.find("Channel") == paramMap.end()) 
	{
        m_strMsg = "Fail to Get <Channel> from XML" ;
        return false; 
	}
    m_iChannel = atoi(paramMap["Channel"].c_str());
    m_strChannel = paramMap["Channel"];

	

    if (paramMap.find("PCL") == paramMap.end()) 
	{
        m_strMsg = "Fail to Get <PCL> from XML" ;
        return false; 
	}
    m_iPCL = atoi(paramMap["PCL"].c_str());

    if (paramMap.find("MaxPower") == paramMap.end()) 
	{
        m_strMsg = "Fail to Get <MaxPower> from XML" ;
        return false; 
	}
    m_fMaxPower = (float)atof(paramMap["MaxPower"].c_str());

    if (paramMap.find("PowerLevel") == paramMap.end()) 
	{
        m_strMsg = "Fail to Get <PowerLevel> from XML" ;
        return false; 
	}
    m_fPowerLevel = (float)atof(paramMap["PowerLevel"].c_str());


    TraceLog(MSG_INFO, "Success to GSM_Handover InitData") ;

    return true;
}

int CGSM_Online_Handover::GetCMUMode(int iBand)
{
	switch (iBand)
	{
	case 850:
		return 16;//_GSM850MS_SIG;
	case 900:
		return 3;//_GSM900MS_SIG;
	case 1800:
		return 5;//_GSM1800MS_SIG;
	default:
		return 7;//_GSM1900MS_SIG;
	}
}