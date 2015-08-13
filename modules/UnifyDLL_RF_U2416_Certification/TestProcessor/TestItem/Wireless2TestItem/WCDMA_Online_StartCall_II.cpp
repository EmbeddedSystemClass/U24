#include "stdafx.h"
#include "WCDMA_Online_StartCall_II.h"


RF_IMPLEMENT_DYNCREATE(CWCDMA_Online_StartCall_II)

#define RRC_Integrity_Enabled		880
#define RRC_Ciphering_Enabled		881
#define RRC_Fake_Security_Enabled	882


bool CWCDMA_Online_StartCall_II::Run()
{
    m_strErrCode.clear() ;
    bool bRet = MainFunction() ;

    if (bRet) {
        m_strMsg = "Success to Run CWCDMA_Online_StartCall" ;
        TraceLog(MSG_INFO, m_strMsg) ;

        FactoryLog(bRet, "WCDMA Online StartCall", "", m_strBand, m_strChannel, "", "", "", "", m_strMsg ) ;
        return true ;
    } else {
        if ( m_strErrCode.empty() )
            m_strErrCode = FunErr_WirelessWCDMA_Online_Call_Fail ;

        FactoryLog(bRet, "WCDMA Online StartCall", m_strErrCode, m_strBand, m_strChannel, "", "", "", "", m_strMsg ) ;
        return false ;
    }
}


bool CWCDMA_Online_StartCall_II::MainFunction()
{
	std::string strBandName;

	char szTmp[256];
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "WirelessUMTS%d", m_iBand);
	m_strBand = szTmp;

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%d", m_iChannel);
	m_strChannel = szTmp;

	/* Neil: dont reboot phone */
	/*	
	if (! m_pIPhone->Reboot())
	{
		m_strMsg = "Fail to reboot device" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	*/
	/* Neil: dont reboot phone */

	/* Neil: disable Reset cmu200 */
	/*
	if (! m_pITesterDevice->Reset())
	{
        m_strMsg = "Fail to reset tester device" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	*/
	/* Neil: disable Reset cmu200 */

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

	if (! m_pITesterDevice->WCDMA_SetBand(m_iBand))
	{
		m_strMsg = "Fail to set WCDMA band" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	int iDownlinkChannel = WCDMA_GetDLChannel(m_iBand, m_iChannel);
	if (iDownlinkChannel == 0)
	{
		m_strMsg = "Fail to get WCDMA downlink channel, check XML parameters" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	if (! m_pITesterDevice->WCDMA_SetDownlinkChannel(iDownlinkChannel))
	{
		m_strMsg = "Fail to set WCDMA channel" ;
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
		m_strMsg = "Fail to set WCDMA BS output power" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	//std::string strBandName;
	std::string strRange;

	if (WCDMA_GetBandNameForCableLoss(m_iBand, strBandName) && WCDMA_GetRangeForTxCableLoss(m_iBand, m_iChannel, strRange))
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

	if (! m_pITesterDevice->SetSignalOnOff(true))
	{
		m_strMsg = "Fail to set signal on for WCDMA online make call" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	TraceLog(MSG_INFO, "Success to Send WCDMA Signal") ;

	/* Neil: dont check phone connection */
	/*
	Sleep(m_iWaitForStartUp) ;
	bool bIsConnected = false;
	int	iTryTimes = 0;

	while (! m_pIPhone->IsConnected())
	{
		if(iTryTimes++ > 50)
		{
			m_strErrCode = ERR_QLIB_PHONE_CONNECT ;
			m_strMsg = "Fail to connect phone by QMSL" ;

			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
		Sleep(500);
	}
	*/
	/* Neil: dont check phone connection */

	if (! m_bSetAuthenticationON)
	{
		if (! SetPhoneSecurity(m_bSetAuthenticationON))
		{
			m_strMsg = "Fail to set NV item for fake security" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
	}

	ShowMsgDlg("Un-plug USB cable, then put the phone in.");

	//bool bIsConnected = false;
	bool bIsConnected = false;
	int iRetry = 20;

	if (! WCDMA_StartCall())
	{
		m_strMsg = "Fail to WCDMA online make call, try again" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
/*
		if (! m_pIPhone->Reboot())
		{
			m_strMsg = "Fail to reboot handset" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
		else
		{
			Sleep(25000);

			bIsConnected = false;
			iRetry = 20;
			do
			{
				if (m_pIPhone->IsConnected())
				{
					bIsConnected = true;
					break;
				}

				Sleep(500);
				--iRetry;
			} while (iRetry > 0);

			if (! bIsConnected)
			{
				m_strMsg = "Fail to connect handset after reboot" ;
			    TraceLog(MSG_ERROR, m_strMsg) ;
				return false;
			}
*/
			if (! WCDMA_StartCall())
			{
				m_strMsg = "Fail to WCDMA online make call" ;
			    TraceLog(MSG_ERROR, m_strMsg) ;
				return false;
			}
//		}
	}

	//ShowMsgDlg("Un-plug USB cable");

	return true;
}


bool CWCDMA_Online_StartCall_II::InitData(std::map<std::string, std::string>& paramMap)
{
    if (paramMap.find("Band") == paramMap.end()) {
        m_strMsg = "Fail to Get <Band> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
    }
	m_iBand = atoi(paramMap["Band"].c_str());

    if (paramMap.find("Channel") == paramMap.end()) {
        m_strMsg = "Fail to Get <Channel> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
    }
	m_iChannel = atoi(paramMap["Channel"].c_str());

    if (paramMap.find("BSOutputPower") == paramMap.end()) {
        m_strMsg = "Fail to Get <BSOutputPower> from XML" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
    }
	m_dOutputPower = atof(paramMap["BSOutputPower"].c_str());

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

	if (paramMap.find("WaitForStartUp") == paramMap.end()){
		m_strMsg = "Fail to Get <WaitForStartUp> from XML" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}
	m_iWaitForStartUp = atoi(paramMap["WaitForStartUp"].c_str());

	return true;
}


bool CWCDMA_Online_StartCall_II::WCDMA_StartCall()
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

bool CWCDMA_Online_StartCall_II::SetPhoneSecurity(bool OnOff)
{
	map< WORD, int > nvdata;

	if (OnOff)
	{
		nvdata[RRC_Integrity_Enabled] = 1;
		nvdata[RRC_Ciphering_Enabled] = 1;
		nvdata[RRC_Fake_Security_Enabled] = 0;
	}
	else
	{
		nvdata[RRC_Integrity_Enabled] = 0;
		nvdata[RRC_Ciphering_Enabled] = 0;
		nvdata[RRC_Fake_Security_Enabled] = 1;
	}

	map< WORD, int >::iterator iter;   
	for( iter = nvdata.begin(); iter != nvdata.end(); iter++ )
	{
		BYTE data[8];
		int &iStatus = (int&)data;
		iStatus = iter->second;
		if (! m_pIPhone->WriteNVItem(iter->first, data, 8))
		{
			return false;
		}
	}

	return true;

}
