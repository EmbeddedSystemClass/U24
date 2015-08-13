#include "stdafx.h"
#include "GSM_Online_Handover_WCDMA.h"


RF_IMPLEMENT_DYNCREATE(CGSM_Online_Handover_WCDMA)


bool CGSM_Online_Handover_WCDMA::Run()
{
    m_strErrCode.clear() ;
    bool bRet = MainFunction2() ;

    if (bRet) 
	{
        m_strMsg = "Success to Run CWCDMA_Online_Handover" ;
		m_strErrCode = "Pass";
    }
	else 
	{
        if ( m_strErrCode.empty() )
		{
            m_strErrCode = FunErr_WirelessWCDMA_Online_Handover_Fail ;
		}
    }

	TraceLog(MSG_INFO, m_strMsg) ;
	FactoryLog(bRet, "GSM_Handover_WCDMA", m_strErrCode, m_strBand, m_strChannel, "", "", "", "", m_strMsg ) ;
	SetPICSDate("GSM_Handover_WCDMA",m_strErrCode);
	return bRet;
}


bool CGSM_Online_Handover_WCDMA::MainFunction()
{
	std::string strBandName;

	char szTmp[256];
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "UMTS%d", m_iTargetBand);
	m_strBand = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%d", m_iTargetChannel);
	m_strChannel = szTmp;

	/*memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "WirelessWCDMA%d", m_iTargetBand);
	strBandName = szTmp;*/

	m_iPreBand =  m_pITesterDevice->GSM_QueryBand() ;

	m_pITesterDevice->SetMode(17);

	if (! m_pITesterDevice->WCDMA_SetBSOutputPower(m_dOutputPower))
	{
		m_strMsg = "Fail to set WCDMA BS output power to OutputPower" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMaxMSPower(m_dMaxMSPower))
	{
		m_strMsg = "Fail to set WCDMA maximum allow power" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMSTargetPower(m_dMSTargetPower))
	{
		m_strMsg = "Fail to set WCDMA MS target power to tester device" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetAuthentication(m_bSetAuthenticationON))
	{
		m_strMsg = "Fail to turn Authentication ON for WCDMA online make call" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (m_bSetAuthenticationON)
	{
		if (! m_pITesterDevice->WCDMA_SetSecurityKey(m_strSKey1, m_strSKey2))
		{
			m_strMsg = "Fail to set secret key for WCDMA online make call" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
	}

	if (! m_pITesterDevice->WCDMA_SetDCHType("RMC"))
	{
		m_strMsg = "Fail to set DCH type to RMC for WCDMA online test" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	

	std::string strRange;

	if (WCDMA_GetBandNameForCableLoss(m_iTargetBand, strBandName) && WCDMA_GetRangeForTxCableLoss(m_iTargetBand, m_iTargetChannel, strRange))
	{ 
		if (! m_pITesterDevice->SetInputLoss(GetCableLoss("TX", "Wireless" + strBandName, strRange)))
		{
			m_strMsg = "Fail to set WCDMA input cable loss" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		if (! m_pITesterDevice->SetOutputLoss(GetCableLoss("RX", "Wireless" + strBandName, strRange)))
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

	if ( m_iTargetBand != m_iPreBand )
	{

		m_pITesterDevice->GSM_SetMode(m_iPreBand, 1) ;

		if (! m_pITesterDevice->WCDMA_SetHANDoverTarget() )
		{
			m_strMsg = "Fail to Set HandOver Target" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false ;
		}

		if (! m_pITesterDevice->GSM_Handover() )
		{
			m_strMsg = "Fail to Handover" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			//return false ;
		}

		Sleep(3000) ;

		m_pITesterDevice->SetMode(17) ;
	}

	if (! m_pITesterDevice->SetSignalOnOff(true))
	{
		m_strMsg = "Fail to set signal on for WCDMA online make call" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}


	if (! m_pITesterDevice->WCDMA_QuerryCallStatus())
	{
		m_strMsg = "Fail to handover, starting make call procedure" ;
		TraceLog(MSG_ERROR, m_strMsg) ;

		Sleep(3000);

        if (!WCDMA_StartCall())
        {
			m_strMsg = "Fail to WCDMA MT call, try again" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
        }     
	}
	else
	{
		if (! m_pITesterDevice->WCDMA_MTCall())
		{
			m_strMsg = "Fail to WCDMA MT call" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
	}

	return true;
}


bool CGSM_Online_Handover_WCDMA::InitData(std::map<std::string, std::string>& paramMap)
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

	if (paramMap.find("BSOutputPower") == paramMap.end()) {
		m_strMsg = "Fail to Get <BSOutputPower> from XML" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	m_dOutputPower = atof(paramMap["BSOutputPower"].c_str());

	if (paramMap.find("MaxMSPower") == paramMap.end())
		return false;
	m_dMaxMSPower = atof(paramMap["MaxMSPower"].c_str());

	if (paramMap.find("MSTargetPower") == paramMap.end())
		return false;
	m_dMSTargetPower = atof(paramMap["MSTargetPower"].c_str());

	if (paramMap["SetAuthenticationON"] == "TRUE" || paramMap["SetAuthenticationON"] == "true")
	{
		m_bSetAuthenticationON = true;

		if (paramMap.find("SecretKey1") == paramMap.end()) {
			m_strMsg = "Fail to Get <SecretKey1> from XML" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
		m_strSKey1 = paramMap["SecretKey1"];

		if (paramMap.find("SecretKey2") == paramMap.end()) {
			m_strMsg = "Fail to Get <SecretKey2> from XML" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
		m_strSKey2 = paramMap["SecretKey2"];
	}
	else
		m_bSetAuthenticationON = false;

    TraceLog(MSG_INFO, "Success to InitData for Online_GSM_Handover_WCDMA");
	return true;
}


bool CGSM_Online_Handover_WCDMA::WCDMA_StartCall()
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
	}
	else
	{
		if (! m_pITesterDevice->WCDMA_MTCall())
		{
			m_strMsg = "Fail to WCDMA MT call" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
	}
	return true;
}

bool CGSM_Online_Handover_WCDMA::MainFunction2()
{
	std::string strBandName;

	char szTmp[256];
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "UMTS%d", m_iTargetBand);
	m_strBand = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%d", m_iTargetChannel);
	m_strChannel = szTmp;

	/*memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "WirelessWCDMA%d", m_iTargetBand);
	strBandName = szTmp;*/

	//end GSM call
	if (! m_pITesterDevice->SetSignalOnOff(false) )
	{
		m_strMsg = "Fail to Turn Off Signal." ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}

	TraceLog(MSG_INFO, "Success to End GSM Call") ;
	//begin to initial WCDMA call

	if (! m_pITesterDevice->Initial())
	{
		m_strMsg = "Fail to initial tester device" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->Initial_WCDMA())
	{
		m_strMsg = "Fail to initial tester device for WCDMA" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	m_pITesterDevice->SetMode(17);

	if (! m_pITesterDevice->WCDMA_SetBand(m_iTargetBand))
	{
		m_strMsg = "Fail to set WCDMA band" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

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

	if (! m_pITesterDevice->WCDMA_SetDCHType("RMC"))
	{
		m_strMsg = "Fail to set DCH type to RMC for WCDMA online test" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}


	if (! m_pITesterDevice->WCDMA_SetBSOutputPower(m_dOutputPower))
	{
		m_strMsg = "Fail to set WCDMA BS output power to OutputPower" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMaxMSPower(m_dMaxMSPower))
	{
		m_strMsg = "Fail to set WCDMA maximum allow power" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetMSTargetPower(m_dMSTargetPower))
	{
		m_strMsg = "Fail to set WCDMA MS target power to tester device" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetAuthentication(m_bSetAuthenticationON))
	{
		m_strMsg = "Fail to turn Authentication ON for WCDMA online make call" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (m_bSetAuthenticationON)
	{
		if (! m_pITesterDevice->WCDMA_SetSecurityKey(m_strSKey1, m_strSKey2))
		{
			m_strMsg = "Fail to set secret key for WCDMA online make call" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
	}

	std::string strRange;

	if (WCDMA_GetBandNameForCableLoss(m_iTargetBand, strBandName) && WCDMA_GetRangeForTxCableLoss(m_iTargetBand, m_iTargetChannel, strRange))
	{ 
		if (! m_pITesterDevice->SetInputLoss(GetCableLoss("TX", "Wireless" + strBandName, strRange)))
		{
			m_strMsg = "Fail to set WCDMA input cable loss" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}

		if (! m_pITesterDevice->SetOutputLoss(GetCableLoss("RX", "Wireless" + strBandName, strRange)))
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


	if (! m_pITesterDevice->SetSignalOnOff(true))
	{
		m_strMsg = "Fail to set signal on for WCDMA online make call" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}


	if (!WCDMA_StartCall())
	{
		Sleep(3000);
		TraceLog(MSG_ERROR, "Fail to WCDMA MT call, try again") ;
		if (!WCDMA_StartCall())
		{
			m_strMsg = "Fail to WCDMA MT call finally" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}	
	}  

	return true;
}
