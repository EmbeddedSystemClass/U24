#include "stdafx.h"
#include "GSM_Online_StartCall.h"


RF_IMPLEMENT_DYNCREATE(CGSM_Online_StartCall)

#define RRC_Integrity_Enabled		880
#define RRC_Ciphering_Enabled		881
#define RRC_Fake_Security_Enabled	882


bool CGSM_Online_StartCall::Run()
{
    m_strErrCode.clear() ;
    bool bRet = MainFunction() ;

    if (bRet) 
	{
        m_strMsg = "Success to Run GSM StartCall" ;
		m_strErrCode = "Pass";

		SetPICSDate("GSM_StartCall", "PASS");
        return true ;
    } 
	else 
	{
        if ( m_strErrCode.empty() )
		{
            m_strErrCode = FunErr_WirelessGSM_Online_Call_Fail ;
		}
    
		SetPICSDate("GSM_StartCall", "FAIL");
    }

	TraceLog(MSG_INFO, m_strMsg) ;
	FactoryLog(bRet, "GSM StartCall", m_strErrCode, m_strBand, m_strChannel, "", "", "", "", m_strMsg ) ;
	return bRet;
}


bool CGSM_Online_StartCall::MainFunction()
{
	//1.Start CMU TX power
	if (!InitTestCMU())
	{
		return false;
	}
	
	TraceLog(MSG_INFO, "Set CMU parameters OK") ;

	//2. Reboot phone if connect phone is true
	bool bSuccess = false; 
	bool bIsConnected = false;
	int	iTryTimes = 0;
	int iRetryConnectTimes = 30;
	

 	if (m_bConnectPhone)
 	{
		if (m_bReset)
		{
			if (!ResetPhone())
			{
				return false;
			}
			TraceLog(MSG_INFO, "Reset Phone OK") ;
		}	

		if (!GSM_AutoAnswer())
		{
			m_strMsg = "Fail to set auto answer" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
		

		if (! SetPhoneSecurity(!m_bChangeFakeSecurity))
		{
			m_strMsg = "Fail to set NV item for fake security" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
		TraceLog(MSG_INFO, "Set PhoneSecurity OK") ;

 	}
	else
	{
		if (! SetPhoneSecurity(!m_bChangeFakeSecurity))
		{
			m_strMsg = "Fail to set NV item for fake security" ;
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
		TraceLog(MSG_INFO, "Set PhoneSecurity OK") ;

		ShowMsgDlg("拔掉USB，并放置好手机，最后点确定");
	}

	TraceLog(MSG_INFO, "Sleep a while for phone SYN with CMU...") ;
	//wait a while after connect
	Sleep(m_iWaitTimeAfterConnect);

	//3. Start call
	if (!GSM_StartCall())
	{	
		TraceLog(MSG_ERROR, "Fail to make call first, call again!") ;
	
		if (m_bConnectPhone)
		{
			m_strMsg = "Fail to make GSM online call, Reset all and try again" ;

			FactoryLog(false, "Start Call", m_strErrCode, m_strBand, m_strChannel, "", "", "", "", m_strMsg ) ;

			if (!ResetPhone())
			{
				return false;
			}		
			//ShowMsgDlg("Un-Plug USB cable");
		}

		Sleep(m_iWaitTimeAfterConnect);
		if (!GSM_StartCall())
		{
			m_strMsg = "Fail to make call again,please check sim card!";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}	
	}

    TraceLog(MSG_INFO, "Success to Make GSM Call") ;

    return true;
}


bool CGSM_Online_StartCall::InitData(std::map<std::string, std::string>& paramMap)
{
	char szTmp[256];
	memset(szTmp, 0, 256);

    // --- Default Value ---
    m_iTCHTimeSlot = 2 ;


    // --- Get From XML ---
    if (paramMap.find("Band") == paramMap.end()) {
        m_strMsg = "Fail to Get <Band> from XML" ;
        return false; }
    m_iBand = atoi(paramMap["Band"].c_str());

	sprintf_s(szTmp, 255, "WirelessGSM%d", m_iBand);
	strBandName = szTmp;
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
	

	if (paramMap.find ("ConnectPhone") == paramMap.end()) {
		m_bConnectPhone = true ;
	}
	if (paramMap["ConnectPhone"] == "FALSE" || paramMap["ConnectPhone"] == "false")
		m_bConnectPhone = false;
	else
		m_bConnectPhone = true;

	if (paramMap.find("ModemPort") == paramMap.end()) {
		m_strMsg = "Fail to Get <SerialPort> from XML" ;
		return false; }
	m_strModemPort = paramMap["ModemPort"].c_str();

	if (paramMap.find("WaitForStartUp") == paramMap.end()) {
		m_strMsg = "Fail to Get <WaitForStartUp> from XML" ;
		return false; }
	m_iWaitForStartUp = atoi(paramMap["WaitForStartUp"].c_str());

	if (paramMap.find ("RebootFirst") == paramMap.end()) {
		m_bReset = true ;
	}
	if (paramMap["RebootFirst"] == "FALSE" || paramMap["RebootFirst"] == "false")
		m_bReset = false;
	else
		m_bReset = true;

	if (paramMap.find("WaitTimeAfterConnect") == paramMap.end())
	{
		m_strMsg = "Fail to Get <WaitTimeAfterConnect> from XML";
		return false;
	}
	m_iWaitTimeAfterConnect = atoi(paramMap["WaitTimeAfterConnect"].c_str());

	if (paramMap.find("ChangeFakeSecurity") == paramMap.end())
	{
		m_bChangeFakeSecurity = false;
	}
	if (paramMap["ChangeFakeSecurity"] == "FALSE" || paramMap["ChangeFakeSecurity"] == "false")
		m_bChangeFakeSecurity = false;
	else
		m_bChangeFakeSecurity = true;	


    return true;
}

bool CGSM_Online_StartCall::GSM_StartCall()
{
	if (! m_pITesterDevice->GSM_MT_Call())
	{
		m_strMsg = "Fail to GSM MT call" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	TraceLog(MSG_INFO, "MT call OK") ;
	return true;
}

bool CGSM_Online_StartCall::GSM_AutoAnswer()
{
	
	char szCommand[] = "ATS0=1";
	char szResponse[512];

	bool b_ret = false;
	int RetryCount = 0;

	m_strMsg = "";

	while(!b_ret && (RetryCount < 10)) 
	{
		memset(szResponse, 0, 512);
		if (!m_pIPhone->Send_ATCommand((char*)(m_strModemPort.c_str()), (char*)szCommand, (char*)szResponse))
		{
			m_strErrCode = FunErr_WirelessGSM_Test_Set_Command_Fail;
			m_strMsg = "Fail to Open Serial Port";
			TraceLog(MSG_ERROR, m_strMsg) ;
			b_ret = false;
			break;
		}
		else
		{
			if (strstr(std::string(szResponse).c_str(),"OK") == NULL)
			{
				m_strErrCode = FunErr_WirelessGSM_Test_Set_Command_Fail;
				m_strMsg = "Fail to set auto answer for phone. Modem response is " + std::string(szResponse);
				TraceLog(MSG_ERROR, m_strMsg) ;
				Sleep(200);
			}
			else
			{
				m_strMsg = "Set auto answer for phone Pass";
				b_ret = true;
			}
		}
		RetryCount ++;
	}

	FactoryLog(b_ret, "Auto Answer", m_strErrCode, m_strBand, m_strChannel, "", "", "", "", m_strMsg ) ;

	return b_ret;
}

bool CGSM_Online_StartCall::SetAllBandMS()
{
	//900
	m_pITesterDevice->GSM_SetMode(900,1);
	
	if (! m_pITesterDevice->GSM_SetTCHTimeSlotLevel_StartCall(m_fPowerLevel) )
	{
		m_strMsg = "Fail to Set Timeslot Power Level of Traffic Channel in GSM900 before test";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	//1800
	m_pITesterDevice->GSM_SetMode(1800,1);

	if (! m_pITesterDevice->GSM_SetTCHTimeSlotLevel_StartCall(m_fPowerLevel) )
	{
		m_strMsg = "Fail to Set Timeslot Power Level of Traffic Channel in GSM1800 before test";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	//1900
	m_pITesterDevice->GSM_SetMode(1900,1);

	if (! m_pITesterDevice->GSM_SetTCHTimeSlotLevel_StartCall(m_fPowerLevel) )
	{
		m_strMsg = "Fail to Set Timeslot Power Level of Traffic Channel in GSM1900 before test";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	//850
	m_pITesterDevice->GSM_SetMode(850,1);

	if (! m_pITesterDevice->GSM_SetTCHTimeSlotLevel_StartCall(m_fPowerLevel) )
	{
		m_strMsg = "Fail to Set Timeslot Power Level of Traffic Channel in GSM850 before test";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	return true;
}

bool CGSM_Online_StartCall::SetPhoneSecurity(bool OnOff)
{
	map<WORD, int> nvdata;

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
			m_strMsg = "Fail to write NV about set Phone security";
			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
	}

	return true;
}

bool CGSM_Online_StartCall::InitTestCMU()
{
	// Initial Tester, and Remote Mode Display.
	if (! m_pITesterDevice->Initial())
	{
		m_strMsg = "Fail to Initial Tester Device" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	// Initial Tester for GSM (Secondary Address, RF IN/OUT port).
	if (! m_pITesterDevice->Initial_GSM() )
	{
		m_strMsg = "Fail to Initial Tester Device for GSM" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	//Jack Set all band ms signal
	if (!SetAllBandMS())
	{
		m_strMsg = "Fail to set all band ms signal." ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false;
	}

	// Initialize, Set to GSM900 Signaling.
	m_pITesterDevice->GSM_SetMode(m_iBand, 1) ;

	// Set BS Signaling Channel.
	if (! m_pITesterDevice->GSM_SetCCH(m_iChannel))
	{
		m_strMsg = "Fail to set Control Channel" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}

	if (! m_pITesterDevice->GSM_SetTCH(m_iChannel))
	{
		m_strMsg = "Fail to set Traffic Channel" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}

	std::string strRange;

	// Set TX/RX Cable Loss
	if (GSM_GetRangeForTxRxCableLoss(m_iBand, m_iChannel, strRange))
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
		m_strMsg = "Fail to Set Timeslot Power Level of Traffic Channel" ;
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (! m_pITesterDevice->SetSignalOnOff(true) )
	{
		m_strMsg = "Fail to set signal on for GSM" ;
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	
	return true;
}

bool CGSM_Online_StartCall::ResetPhone()
{
	bool bIsConnected = false;
	int	iTryTimes = 0;
	int iRetryConnectTimes = 30;
	bool bSuccess = false; 

	for (int i = 0; (i < 3) && !bSuccess; ++i)
	{
		if (! m_pIPhone->Reboot())
		{
			if (i < 2)
				TraceLog(MSG_ERROR, "Fail to reboot the phone, try again.");
		}
		else
			bSuccess = true;
		Sleep(2000);
	}
	if (! bSuccess)
	{
		m_strMsg = "Fail to reboot the phone.";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	Sleep(m_iWaitForStartUp) ;
	while (! m_pIPhone->IsConnected())
	{
		if (iTryTimes++ > iRetryConnectTimes)
		{
			m_strErrCode = ERR_QLIB_PHONE_CONNECT ;
			m_strMsg = "Fail to connect phone by QMSL" ;

			TraceLog(MSG_ERROR, m_strMsg) ;
			return false;
		}
		Sleep(500);
	}

	if (! GSM_AutoAnswer() )
	{
		m_strMsg = "Fail to set AT command for auto answer." ;
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	return true;
}
