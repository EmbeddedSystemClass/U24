#include "stdafx.h"
#include "WCDMA_Online_Handover_II.h"


RF_IMPLEMENT_DYNCREATE(CWCDMA_Online_Handover_II)


bool CWCDMA_Online_Handover_II::Run()
{
    m_strErrCode.clear() ;
    bool bRet = MainFunction() ;

    if (bRet) {
        m_strMsg = "Success to Run CWCDMA_Online_Handover" ;
        TraceLog(MSG_INFO, m_strMsg) ;

        FactoryLog(bRet, "WCDMA Online Handover", "", m_strBand, m_strChannel, "", "", "", "", m_strMsg ) ;
        return true ;
    } else {
        if ( m_strErrCode.empty() )
            m_strErrCode = FunErr_WirelessWCDMA_Online_Handover_Fail ;

        FactoryLog(bRet, "WCDMA Online Handover", m_strErrCode, m_strBand, m_strChannel, "", "", "", "", m_strMsg ) ;
        return false ;
    }
}


bool CWCDMA_Online_Handover_II::MainFunction()
{
	char szTmp[256];
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "UMTS%d", m_iTargetBand);
	m_strBand = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%d", m_iTargetChannel);
	m_strChannel = szTmp;

	if (! m_pITesterDevice->WCDMA_SetBSOutputPower(-60.0))
	{
		m_strMsg = "Fail to set WCDMA BS output power to -60.0" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMaxMSPower(33.0))
	{
		m_strMsg = "Fail to set WCDMA maximum allow power" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMSTargetPower(-20.0))
	{
		m_strMsg = "Fail to set WCDMA MS target power to tester device" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	std::string strBandName;
	std::string strRange;

	if (WCDMA_GetBandNameForCableLoss(m_iTargetBand, strBandName) && WCDMA_GetRangeForTxCableLoss(m_iTargetBand, m_iTargetChannel, strRange))
	{
		if (! m_pITesterDevice->SetInputLoss(GetCableLoss("TX", "Wireless" + strBandName, strRange)))
		{
			m_strMsg = "Fail to set WCDMA input cable loss" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		if (! m_pITesterDevice->SetOutputLoss(GetCableLoss("RX", "Wireless" +strBandName, strRange)))
		{
			m_strMsg = "Fail to set WCDMA input cable loss" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
	}
	else
	{
		m_strMsg = "Fail to get WCDMA band name & range for Tx cable loss" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}


	TraceLog(MSG_INFO, "Success to setup test device for WCDMA handover") ;


	int iDownlinkChannel = WCDMA_GetDLChannel(m_iTargetBand, m_iTargetChannel);
	if (iDownlinkChannel == 0)
	{
		m_strMsg = "Fail to get WCDMA downlink channel, check XML parameters" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetBandAndDownlinkChannel(m_iTargetBand, iDownlinkChannel))
	{
		m_strMsg = "Fail to set band and channel for WCDMA handover" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_QuerryCallStatus())
	{
		m_strMsg = "Fail to handover, starting make call procedure" ;
		TraceLog(MSG_ERROR, m_strMsg) ;

        if (!WCDMA_StartCall())
        {
			m_strMsg = "Fail to WCDMA MT call, try again" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
        }
        else
        {
			return true;
        }

		if (! m_pIPhone->Reboot())
		{
			m_strMsg = "Fail to reboot handset" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		Sleep(25000);

		int iRetry = 30;
		bool bIsConnected = false;
		do
		{
			if (m_pIPhone->IsConnected())
			{
				bIsConnected = true;
				break;
			}

			Sleep(500);
			--iRetry;
		} while (iRetry >= 0);

		if (! bIsConnected)
		{
			m_strMsg = "Fail to connect handset after reboot" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		if (! WCDMA_StartCall())
		{
			m_strMsg = "Fail to WCDMA MT call" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
	}

	return true;
}


bool CWCDMA_Online_Handover_II::InitData(std::map<std::string, std::string>& paramMap)
{
    if (paramMap.find("TargetBand") == paramMap.end()) {
        m_strMsg = "Fail to Get <TargetBand> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
    }
	m_iTargetBand = atoi(paramMap["TargetBand"].c_str());

    if (paramMap.find("TargetChannel") == paramMap.end()) {
        m_strMsg = "Fail to Get <TargetChannel> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
    }
	m_iTargetChannel = atoi(paramMap["TargetChannel"].c_str());

    TraceLog(MSG_INFO, "Success to InitData for WCDMA_Online_Handover");
	return true;
}


bool CWCDMA_Online_Handover_II::WCDMA_StartCall()
{
	if (! m_pITesterDevice->WCDMA_QuerryCampStatus())
	{
		m_strMsg = "Fail to camp on BS, try MT call without register" ;
		TraceLog(MSG_ERROR, m_strMsg) ;

		if (! m_pITesterDevice->WCDMA_MTCall())
		{
			m_strMsg = "Fail to WCDMA MT call" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
		else
			return true;
	}

	if (! m_pITesterDevice->WCDMA_MTCall())
	{
		m_strMsg = "Fail to WCDMA MT call" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	return true;
}
