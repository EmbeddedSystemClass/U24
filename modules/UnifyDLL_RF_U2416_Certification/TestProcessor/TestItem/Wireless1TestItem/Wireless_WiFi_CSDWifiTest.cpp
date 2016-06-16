#include "stdafx.h"
#include "Wireless_WiFi_CSDWifiTest.h"


// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CWireless_WiFi_CSDWifiTest)

template<typename T>
std::string t_to_string(T i)
{
  //  std::stringstream ss;
	char charr[255];
    std::string s;
  //  ss << i;
 //   s = ss;
	sprintf(charr, "%d", i);
	s = charr;
    return s;
}

bool CWireless_WiFi_CSDWifiTest::Run()
{
	bool bRes = false;
	bRes = getWifiApInfo();
	if (bRes){
		bRes = MainFunction();
	}
	else
	{
		FactoryLog(false, "getWifiApInfo", "Fail", "-", "-", "-", "-", m_strMeasured, "dBm", m_strMsg);
	}

	if (bRes)
	{
		FactoryLog(true, "WiFi_RxTest", "Pass", "-", "-", "-", "-", m_strMeasured, "dBm", "CSD WiFi Rx PASS");
	}
	else
	{
		FactoryLog(false, "WiFi_RxTest", "Fail", "-", "-", "-", "-", m_strMeasured, "dBm", m_strMsg);
	}

	return bRes;
}
bool CWireless_WiFi_CSDWifiTest::getWifiApInfo()
{
  HANDLE hClient = NULL;
    DWORD dwMaxClient = 2;      //    
    DWORD dwCurVersion = 0;
    DWORD dwResult = 0;
    DWORD dwRetVal = 0;
    int iRet = 0;

    WCHAR GuidString[39] = { 0 };

    unsigned int i, k;

    // variables used for WlanEnumInterfaces

    PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
    PWLAN_INTERFACE_INFO pIfInfo = NULL;

    // variables used for WlanQueryInterfaces for opcode = wlan_intf_opcode_current_connection
    PWLAN_CONNECTION_ATTRIBUTES pConnectInfo = NULL;
    DWORD connectInfoSize = sizeof(WLAN_CONNECTION_ATTRIBUTES);
    WLAN_OPCODE_VALUE_TYPE opCode = wlan_opcode_value_type_invalid;

    dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
    if (dwResult != ERROR_SUCCESS) {
        wprintf(L"WlanOpenHandle failed with error: %u\n", dwResult);
        return 1;
        // You can use FormatMessage here to find out why the function failed
    }

    dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
    if (dwResult != ERROR_SUCCESS) {
        wprintf(L"WlanEnumInterfaces failed with error: %u\n", dwResult);
        return 1;
        // You can use FormatMessage here to find out why the function failed
    } else {
        wprintf(L"Num Entries: %lu\n", pIfList->dwNumberOfItems);
	}


   dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
    if (dwResult != ERROR_SUCCESS) {
        wprintf(L"WlanEnumInterfaces failed with error: %u\n", dwResult);
        return 1;
        // You can use FormatMessage here to find out why the function failed
    } else {
        wprintf(L"Num Entries: %lu\n", pIfList->dwNumberOfItems);
        wprintf(L"Current Index: %lu\n", pIfList->dwIndex);
        for (i = 0; i < (int) pIfList->dwNumberOfItems; i++) {
            pIfInfo = (WLAN_INTERFACE_INFO *) & pIfList->InterfaceInfo[i];
            wprintf(L"  Interface Index[%u]:\t %lu\n", i, i);
            iRet =
                StringFromGUID2(pIfInfo->InterfaceGuid, (LPOLESTR) & GuidString,
                                sizeof (GuidString) / sizeof (*GuidString));
            // For c rather than C++ source code, the above line needs to be
            // iRet = StringFromGUID2(&pIfInfo->InterfaceGuid, (LPOLESTR) &GuidString, 
            //     sizeof(GuidString)/sizeof(*GuidString)); 
            if (iRet == 0)
                wprintf(L"StringFromGUID2 failed\n");
            else {
                wprintf(L"  InterfaceGUID[%d]:\t %ws\n", i, GuidString);
            }
            wprintf(L"  Interface Description[%d]: %ws", i, pIfInfo->strInterfaceDescription);
            wprintf(L"\n");
            wprintf(L"  Interface State[%d]:\t ", i);
            switch (pIfInfo->isState) {
            case wlan_interface_state_not_ready:
                wprintf(L"Not ready\n");
                break;
            case wlan_interface_state_connected:
                wprintf(L"Connected\n");
                break;
            case wlan_interface_state_ad_hoc_network_formed:
                wprintf(L"First node in a ad hoc network\n");
                break;
            case wlan_interface_state_disconnecting:
                wprintf(L"Disconnecting\n");
                break;
            case wlan_interface_state_disconnected:
                wprintf(L"Not connected\n");
                break;
            case wlan_interface_state_associating:
                wprintf(L"Attempting to associate with a network\n");
                break;
            case wlan_interface_state_discovering:
                wprintf(L"Auto configuration is discovering settings for the network\n");
                break;
            case wlan_interface_state_authenticating:
                wprintf(L"In process of authenticating\n");
                break;
            default:
                wprintf(L"Unknown state %ld\n", pIfInfo->isState);
                break;
            }
            wprintf(L"\n");
		}}

     if (pIfInfo->isState == wlan_interface_state_connected) {
                dwResult = WlanQueryInterface(hClient,
                                              &pIfInfo->InterfaceGuid,
                                              wlan_intf_opcode_current_connection,
                                              NULL,
                                              &connectInfoSize,
                                              (PVOID *) &pConnectInfo, 
                                              &opCode);

                if (dwResult != ERROR_SUCCESS) {
                    wprintf(L"WlanQueryInterface failed with error: %u\n", dwResult);
                    dwRetVal = 1;
                    // You can use FormatMessage to find out why the function failed
                } else {
                    wprintf(L"  WLAN_CONNECTION_ATTRIBUTES for this interface\n");	
				}
                 wprintf(L"    Receiving Rate:\t %ld\n",
                 pConnectInfo->wlanAssociationAttributes.ulRxRate);
				nRxRate = pConnectInfo->wlanAssociationAttributes.ulRxRate;

	 }
	/*lion*/

	int nlRxRate;
    bool bRxRate = false;
    //pConnectInfo->wlanAssociationAttributes.ulRxRate);
	nlRxRate = pConnectInfo->wlanAssociationAttributes.ulRxRate;
	wprintf(L"    Receiving Rate 1:\t %d\n",nlRxRate);

	for(int i = 0 ; i < 20 ; i ++){
		for (int j = 0 ; j < 2; j ++){
			if (nlRxRate  == arrWlanRate[i][j] ) {
				wprintf(L"    Receiving Rate 2:\t %d, wMode:%d, DataRate:%d\n",nlRxRate, arrWlanRate[i][2]  ,arrWlanRate[i][3]   );
				m_iWlandMode = arrWlanRate[i][2] ;
				m_iRateMask = arrWlanRate[i][3];
				bRxRate = true;
				break;
			}
		}
	}
	if ( ! bRxRate) {
		m_strMsg = "Can't find wMode, DataRate, nlRxRate = ";
		m_strMsg = m_strMsg +  t_to_string(nlRxRate);
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}
	/*lion*/
	ULONG *channel = 0;
	DWORD dwSizeChannel = sizeof(channel);
            if (pIfInfo->isState == wlan_interface_state_connected) {
                dwResult = WlanQueryInterface(hClient,
                                              &pIfInfo->InterfaceGuid,
                                              wlan_intf_opcode_channel_number,
                                              NULL,
                                              &dwSizeChannel,
                                              //&channel, 
											  (PVOID*)&channel, 
                                              &opCode);
				if (dwResult == ERROR_SUCCESS) {
						m_iChannel = *channel;
						if (  m_iChannel < 12 ) {
							m_iChannel = 2412 + 5 * ( m_iChannel - 1);
						}
					 //nChannel = *channel;
					 printf ("Channel: %lu\n", *channel);
				}
			}
    //return dwRetVal;
    if (pConnectInfo != NULL) {
        WlanFreeMemory(pConnectInfo);
        pConnectInfo = NULL;
    }

    if (pIfList != NULL) {
        WlanFreeMemory(pIfList);
        pIfList = NULL;
    }

	return true;
}


bool CWireless_WiFi_CSDWifiTest::MainFunction()
{
	bool isOk = true;
	unsigned long rxFrameCounter = 0;
	char* szRxFrameCounter = new char[1000];

	if (!(isOk = m_pIPhone->Initial()))
	{
		m_strMsg = "Fail to connect phone with Qisda module  in CWireless_WiFi_CSDWifiTest";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	// Start WiFi power
	Sleep(m_iCommandDelay);
	char szTmp[256] = {0};
	char szMeasuredTmp[256] = {0};

	if (isOk)
	{
		if (!(isOk = m_pIPhone->WifiModuleOnCertification(true)))
		{
			m_strMsg = "Fail to Switch WIFI module on";
			TraceLog(MSG_ERROR, m_strMsg);
		}

		if (isOk)
		{
			//WifiPowerOnRxGetPacket
			CString csWifiPowerOnRxCertification;
			csWifiPowerOnRxCertification.Format("m_iChannel = %d ,m_iWlandMode = %d, m_iRateMask = %d  , m_iChain = %d", m_iChannel, m_iWlandMode, m_iRateMask, m_iChain);
			m_strMsg = csWifiPowerOnRxCertification.GetBuffer(0);
			csWifiPowerOnRxCertification.ReleaseBuffer();
			//m_strMsg = m_strMsg +  t_to_string(nlRxRate);
			TraceLog(MSG_ERROR, m_strMsg);

			if (!(isOk = m_pIPhone->WifiPowerOnRxCertification(m_iChannel, m_iWlandMode, m_iRateMask, m_iChain)))
			{
				m_strMsg = "Fail to execute WifiPowerOnRxGetPacket";
				TraceLog(MSG_ERROR, m_strMsg);
			}
			//Sleep(m_iCommandDelay);

			//int ret = MessageBox(NULL, _T("Get Packet Counter?"), _T("Info."), 
			//	MB_ICONQUESTION ); 

			//int ret = ::MessageBox(NULL, _T("Get Packet Counter?"), _T("Info."), 
			//	MB_ICONQUESTION );
			TraceLog(MSG_ERROR, "Wait for Get Result");
			Sleep(m_iResultDelay);

			if (!(isOk = m_pIPhone->WifiPowerOnRxGetRssi(szRxFrameCounter))){
				m_strMsg = "Fail to execute WifiPowerOnRxGetPacket";
				TraceLog(MSG_ERROR, m_strMsg);
				isOk = false;
			}else{
				sprintf(szMeasuredTmp, "%s", szRxFrameCounter);
				m_iMeasure = atoi(szRxFrameCounter);
			}

			if (!m_pIPhone->WifiModuleOnCertification(false))
			{
				m_strMsg = "Fail to Switch WIFI module off";
				TraceLog(MSG_ERROR, m_strMsg);
				isOk = false;
			}
		}
	}

	if ( isOk ) {

		if( ( m_iMeasure > m_iCheckMin ) && ( m_iMeasure < m_iCheckMax) ) 
		{
			m_strMsg = " Check RxGetPacket  range OK";
			TraceLog(MSG_ERROR, m_strMsg);
		}
		else 
		{
			m_strMsg = " Check RxGetPacket over range";
			TraceLog(MSG_ERROR, m_strMsg);
			isOk = false;
		}

		CString csCheckValue;
		csCheckValue.Format( "CHECKMIN = %d, CHECKMAX = %d, RxGetPacket  = %d ", m_iCheckMin, m_iCheckMax, m_iMeasure);
		m_strMsg = csCheckValue.GetBuffer(0);
		csCheckValue.ReleaseBuffer();
		TraceLog(MSG_ERROR, m_strMsg);
	}
	m_strMeasured = szMeasuredTmp;

	return isOk;
}


bool CWireless_WiFi_CSDWifiTest::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find("CommandDelay") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CommandDelay for CWireless_WiFi_CSDWifiTest");
		return false;
	}
	m_iCommandDelay = atoi(paramMap["CommandDelay"].c_str());

	if (paramMap.find("CHECKMIN") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CHECKMIN for CWireless_WiFi_CSDWifiTest");
		return false;
	}
	m_iCheckMin = atoi(paramMap["CHECKMIN"].c_str());

	if (paramMap.find("CHECKMAX") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CHECKMAX for CWireless_WiFi_CSDWifiTest");
		return false;
	}
	m_iCheckMax = atoi(paramMap["CHECKMAX"].c_str());



	if (paramMap.find("RESULTDELAY") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RESULTDELAY for CWireless_WiFi_CSDWifiTest");
		return false;
	}
	m_iResultDelay = atoi(paramMap["RESULTDELAY"].c_str());

	

	if (paramMap.find("CHAINANTE") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter CHAINANTE for CWireless_WiFi_CSDWifiTest");
		return false;
	}
	m_iChain = atoi(paramMap["CHAINANTE"].c_str());


	return true;
}

bool CWireless_WiFi_CSDWifiTest::PostRun()
{
	return true;
}
