#include "stdafx.h"
#include "FMtest.h"


RF_IMPLEMENT_DYNCREATE(CFMtest)

bool CFMtest::Run()
{
	bool bRes = MainFunction();

	if (bRes)
		FactoryLog(true, "", "", "", "", m_strLower, m_strUpper, m_strMeasured, "dBm", "FM Test PASS");
	else
		FactoryLog(false, "FM Test", m_strErrCode, "", "", m_strLower, m_strUpper, m_strMeasured, "dBm", m_strMsg);

	return bRes;
}

bool CFMtest::MainFunction()
{
	char szCommand[256];
	memset(szCommand, 0, 256);

	// Reset Tester Device and Initialize
    if (! m_pITesterDevice->Reset()) {
        m_strErrCode = ERR_DEVICE_RESET ;
        m_strMsg = "Fail to Reset Tester device" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
    }
    if (! m_pITesterDevice->Initial()) {
        m_strErrCode = ERR_DEVICE_RESET ;
        m_strMsg = "Fail to Initial Tester device" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
    }

	if (! m_pITesterDevice->Initial_RF_Nonsignaling()) {
        m_strErrCode = ERR_DEVICE_RESET ;
        m_strMsg = "Fail to Initial GSM Tester device" ;
        TraceLog(MSG_ERROR, m_strMsg) ;
        return false;
    }

	int iCMUMode = 1;
	std::string strResponse;

	sprintf_s(szCommand, 255, "%d;OUTP RF%d;", iCMUMode, m_iOutput);
	if (! m_pITesterDevice->GPIB_Write(std::string(szCommand))){
		m_strMsg = "Set Output Port in FM test fail!" ;
		TraceLog(MSG_ERROR, m_strMsg);
		m_strErrCode = ERR_DEVICE_SET_OUTPUT_PORT;
		return false;
	}

	sprintf_s(szCommand, 255, "%d;SOURce:RFGenerator:MODulation FM;*OPC?", iCMUMode);
	if (! m_pITesterDevice->GPIB_Querry(std::string(szCommand), strResponse)){
		m_strMsg = "Set Modulation fail!" ;
		TraceLog(MSG_ERROR, m_strMsg);
		m_strErrCode = ERR_DEVICE_FM_INIT;
		return false;
	}
	
	sprintf_s(szCommand, 255, "%d;SOURce:RFGenerator:MODulation:FM:FREQuency 1000;*OPC?", iCMUMode);
	if (! m_pITesterDevice->GPIB_Querry(std::string(szCommand), strResponse)){
		m_strMsg = "Set Modulation Frequency fail!" ;
		TraceLog(MSG_ERROR, m_strMsg);
		m_strErrCode = ERR_DEVICE_FM_INIT;
		return false;
	}

	sprintf_s(szCommand, 255, "%d;SOURce:RFGenerator:MODulation:FM:DEViation 22500;*OPC?", iCMUMode);
	if (! m_pITesterDevice->GPIB_Querry(std::string(szCommand), strResponse)){
		m_strMsg = "Set Modulation Deviation fail!" ;
		TraceLog(MSG_ERROR, m_strMsg);
		m_strErrCode = ERR_DEVICE_FM_INIT;
		return false;
	}	

	sprintf_s(szCommand, 255, "%d;INITiate:RFGenerator:TX; *OPC?", iCMUMode);
	if (! m_pITesterDevice->GPIB_Querry(std::string(szCommand), strResponse)){
		m_strMsg = "Init RF generator fail!" ;
		TraceLog(MSG_ERROR, m_strMsg);
		m_strErrCode = ERR_DEVICE_FM_INIT;
		return false;
	}	

	bool bIsConnect = false ;

    //int iRetry = 60;
	int iRetry = 10;
    do
    {
        if (m_pIPhone->IsConnected_QMSL())
        {
            bIsConnect = true ;
            break ;
        }

        --iRetry ;
        //Sleep(500) ;
		Sleep(300) ;
    } while ( 0<iRetry ) ;

    if ( !bIsConnect )
    {
        TraceLog(MSG_ERROR, "Fail to Connect Phone.");
        return false ;
    }

    // Change to Online Mode
	if (!m_pIPhone->QMSL_DIAG_CONTROL_F(MODE_ONLINE_F))
	{
		m_strErrCode = ERR_QLIB_SET_ONLINE ;
		m_strMsg = "Fail to Reset Phone to Online" ;
		TraceLog(MSG_ERROR, m_strMsg) ;
		return false ;
	}

	bool ret = m_pIPhone->Disconnect_QMSL();

	if (ret)
	{
		TraceLog(MSG_INFO, "Disconnect_QMSL success");
	}
	else
	{
		m_strErrCode = ERR_PHONE_CONNECT;
		m_strMsg = "Disconnect_QMSL fail";
		TraceLog(MSG_ERROR, "Fail to Disconnect_QMSL");
	}

	if (! m_pIPhone->Initial())
	{
		TraceLog(MSG_ERROR, "Fail to execute m_pIPhone->Initial()");
		m_strErrCode = ERR_PHONE_CONNECT;
		return false;
	}

	/*
	// set online
	BYTE nvdata[128];
	int &iCurrMode = (int&)nvdata;
	iCurrMode = 0;
	if (! m_pIPhone->WriteNVItem(453, nvdata, 128))
	{
		m_strMsg = "Fail to write NV453 with Qisda module";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	bool bRet = m_pIPhone->EFS_Sync_Combo(m_iEFsSyncRetryCount);
	if (bRet)
	{
		TraceLog(MSG_INFO, "Sync combo success");
	}
	else
	{
		m_strErrCode = ERR_CHANGE_MODE;
		m_strMsg = "Sync combo fail";
		TraceLog(MSG_ERROR, "Fail to Sync combo");
		return false;
	}

	if (! m_pIPhone->Reboot())
	{
		m_strMsg = "Fail to reboot phone with Qisda module";
		TraceLog(MSG_ERROR, m_strMsg);
		m_strErrCode = ERR_CHANGE_MODE;
		return false;
	}

	Sleep(m_iWaitTimeForBoot);

	bool bIsConnect = false;
	for (int i = 0; i < m_iRetryConnectTimes; ++i)
	{
		if (m_pIPhone->IsConnected())
		{
			bIsConnect = true;
			break;
		}

		Sleep(500);
	}

	if (! bIsConnect)
	{
		m_strMsg = "Phone is not connected";
		TraceLog(MSG_ERROR, m_strMsg);
		m_strErrCode = ERR_PHONE_CONNECT;
		return false;
	}

	Sleep(m_iWaitTimeAfterConnect);
	*/

	bool bRet=m_pIPhone->BTModuleOnOffSwitch(false);
	if(!bRet)
	{
		TraceLog(MSG_INFO, "Failed to BT Module Off!");		
	}
	//Sleep(1000);
	Sleep(500);

	bRet=m_pIPhone->FM_ConnectFM();
	if(!bRet)
	{
		TraceLog(MSG_INFO, "Failed to FM_ConnectFM!");		
	}
	Sleep(5000);
	
	bRet=m_pIPhone->FM_ConnectFM(false);
	if(!bRet)
	{
		TraceLog(MSG_INFO, "Failed to disconnect FM!");		
	}
	Sleep(1000);

	bRet=m_pIPhone->FM_ConnectFM();
	if(!bRet)
	{
		TraceLog(MSG_INFO, "Failed to FM_ConnectFM!");		
	}
	Sleep(5000);

 
	sprintf_s(szCommand, 255, "%d;SOURce:RFGenerator:TX:LEVel %f; *OPC?", iCMUMode, m_dRFLevel);
	if (! m_pITesterDevice->GPIB_Querry(std::string(szCommand), strResponse)){
		m_strMsg = "Set Tx Level fail!" ;
		TraceLog(MSG_ERROR, m_strMsg);
		m_strErrCode = ERR_DEVICE_FM_INIT;
		return false;
	}	

	sprintf_s(szCommand, 255, "0;CONFigure:USER:CORRection:LOSS:TABLe:ENABle OFF");
	if (! m_pITesterDevice->GPIB_Write(std::string(szCommand))){
		m_strMsg = "Set Correction table in FM test fail!" ;
		TraceLog(MSG_ERROR, m_strMsg);
		m_strErrCode = ERR_DEVICE_SET_OUTPUT_PORT;
		return false;
	}

	double dOutCableLoss;

	dOutCableLoss = GetCableLoss("RX", "FMRssi", "9810");

	sprintf_s(szCommand, 255, "%d;SOUR:CORR:LOSS:OUTPUT%d %8.3f;*OPC?", iCMUMode, m_iOutput, dOutCableLoss);
	if (! m_pITesterDevice->GPIB_Querry(std::string(szCommand), strResponse)){
		m_strMsg = "Set Loss Output fail!" ;
		TraceLog(MSG_ERROR, m_strMsg);
		m_strErrCode = ERR_DEVICE_FM_INIT;
		return false;
	}	

	int iChannel = m_iTestCH;

	iChannel *= 10000;

	sprintf_s(szCommand, 255, "%d;SOURce:RFGenerator:TX:FREQuency %d; *OPC?", iCMUMode, iChannel);
	if (! m_pITesterDevice->GPIB_Querry(std::string(szCommand), strResponse)){
		m_strMsg = "Set Tx Frequency Output fail!" ;
		TraceLog(MSG_ERROR, m_strMsg);
		m_strErrCode = ERR_DEVICE_FM_INIT;
		return false;
	}	

	sprintf_s(szCommand, 255, "%d;INITiate:RFGenerator:TX; *OPC?", iCMUMode);
	if (! m_pITesterDevice->GPIB_Querry(std::string(szCommand), strResponse)){
		m_strMsg = "Init RF generator fail!" ;
		TraceLog(MSG_ERROR, m_strMsg);
		m_strErrCode = ERR_DEVICE_FM_INIT;
		return false;
	}	

	iChannel /= 1000;

	bRet=false;
	int iRetryTimes = 5;
	do{
		if(!(bRet=m_pIPhone->FM_TuneFM(iChannel)))
		{
			TraceLog(MSG_INFO, _T("Failed to FM_TuneFM, try one more time!"));		
			//Sleep(1000);
			Sleep(500);
		}
	}while(--iRetryTimes && !bRet );

	if(!bRet)
	{
		TraceLog(MSG_ERROR, _T("Failed to FM_TuneFM!"));		
		return false;
	}

	//Sleep(4000);

	int  iPoweResult = 0;

	bRet=false;
	iRetryTimes = 5;
	do{
		if(!(bRet=m_pIPhone->FM_GetRSSI(iPoweResult)))
		{
			TraceLog(MSG_ERROR, _T("Failed to FMReceiveRSSI, try next time!"));		
			//Sleep(1000);
			Sleep(500);
		}
		else
		{
			iPoweResult=0-iPoweResult;

			char szTmp[256];
				
			memset(szTmp, 0, 256);
			sprintf_s(szTmp, 255, "%f", m_dLowerRSSI);
			m_strLower = szTmp ;

			memset(szTmp, 0, 256);
			sprintf_s(szTmp, 255, "%f", m_dUpperRSSI);
			m_strUpper = szTmp ;

			memset(szTmp, 0, 256);
			sprintf_s(szTmp, 255, "%d", iPoweResult);
			m_strMeasured = szTmp ;

			if(InRange(m_dLowerRSSI, m_dUpperRSSI, (double)iPoweResult))
			{
				return true;
			}
			else
			{
				bRet=false;
				m_strErrCode = ERR_FM_RSSI_OUT_OF_RANGE;
				TraceLog(MSG_ERROR, _T("RSSI isn't in the range, try next time!"));	
				//Sleep(1000);
				Sleep(500);
			}
		}

	}while(--iRetryTimes && !(bRet));

	if(!bRet)
	{
		return false;
	}

	sprintf_s(szCommand, 255, "%d;ABORt:RFGenerator:TX; *OPC?", iCMUMode);
	if (! m_pITesterDevice->GPIB_Querry(std::string(szCommand), strResponse)){
		m_strMsg = "Abort RF generator fail!" ;
		TraceLog(MSG_ERROR, m_strMsg);
		m_strErrCode = ERR_DEVICE_FM_INIT;
		return false;
	}	
	
	
	bRet=false;
	iRetryTimes = 3;
	do{
		if(!(bRet=m_pIPhone->FM_ConnectFM(false)))
		{
			TraceLog(MSG_ERROR, _T("Failed to FMDisconnect, try next time!"));		
			//Sleep(1000);
			Sleep(500);
		}
	}while(--iRetryTimes && !bRet );

	return bRet;
}

bool CFMtest::PostRun()
{
	TraceLog(MSG_INFO, "FM Test PASS");
	return true;
}

bool CFMtest::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find ("OutputPort") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter OutputPort for CFMtest");
		return false;
	}
	m_iOutput = atoi(paramMap["OutputPort"].c_str());

	if (paramMap.find ("TestCHCount") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter TestCHCount for CFMtest");
		return false;
	}
	m_iTestCHCount = atoi(paramMap["TestCHCount"].c_str());

	if (paramMap.find ("TestCHList") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter TestCHList for CFMtest");
		return false;
	}
	m_iTestCH = atoi(paramMap["TestCHList"].c_str());

	if (paramMap.find ("RFLevel") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RFLevel for CFMtest");
		return false;
	}
	m_dRFLevel = atoi(paramMap["RFLevel"].c_str());
	
	std::string strTmp;
    std::vector<std::string> vTmp;

    if (paramMap.find("RSSISpec") == paramMap.end())
        return false;
    strTmp = paramMap["RSSISpec"];
    STDStringTokenizer(strTmp, ",", vTmp);
    if (vTmp.size() < 2)
        return false;
    m_dLowerRSSI = atof(vTmp[0].c_str());
	m_dUpperRSSI = atof(vTmp[1].c_str());

	if (paramMap.find ("WaitTimeForBoot") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter WaitTimeForBoot for CPhoneStateItem");
		return false;
	}
	m_iWaitTimeForBoot = atoi(paramMap["WaitTimeForBoot"].c_str());

	if (paramMap.find ("RetryConnectTimes") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RetryConnectTimes for CPhoneStateItem");
		return false;
	}
	m_iRetryConnectTimes = atoi(paramMap["RetryConnectTimes"].c_str());

	if (paramMap.find ("WaitTimeAfterConnect") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter WaitTimeAfterConnect for CPhoneStateItem");
		return false;
	}
	m_iWaitTimeAfterConnect = atoi(paramMap["WaitTimeAfterConnect"].c_str());

	if (paramMap.find ("EFsSyncRetryCount") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter EFsSyncRetryCount for CSyncCombo");
		return false;
	}
	m_iEFsSyncRetryCount = atoi(paramMap["EFsSyncRetryCount"].c_str());

	return true;
}
