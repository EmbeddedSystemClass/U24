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
	if (m_str_TestItem == Wifi_RxTest)
	{
	//	m_strItemCode = CStr::IntToStr(Monitor_BaseItemcode);
		m_strErrorCode = FunErr_POST_CMD_Fail;
		int i;
		for (  i = 0 ; i < 60 ; i ++ ) {
			if ( !DetectDevice() ) {
				m_strMsg = "DetectDevice fail";
				FactoryLog(true, "DetectDevice", "fail", "-", "-", "-", "-", m_strMeasured, "dBm", m_strMsg);
			//	return false;
			}
			else
			{
			//	i = 60;
				m_strMsg = "DetectDevice ok";
				FactoryLog(true, "DetectDevice", "pass", "-", "-", "-", "-", m_strMeasured, "dBm", m_strMsg);
				break;
			}
			Sleep(3000);
		}
		if ( i >=60 ) return false;
		

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

			bool brunPostCmd = false;
			m_strErrorCode = FunErr_POST_CMD_Fail;
			brunPostCmd = runPostCmd();

			if (brunPostCmd)
			{
				FactoryLog(true, "runPostCmd", "Pass", "-", "-", "-", "-", m_strMeasured, "dBm", "runPostCmd PASS");
			}
			else
			{
				FactoryLog(false, "runPostCmd", "Fail", "-", "-", "-", "-", m_strMeasured, "dBm", "runPostCmd fail");
			}



		//passFail = runPostCmd();
	}
	else if (m_str_TestItem == Postcmd)
	{
		
		m_strErrorCode = FunErr_POST_CMD_Fail;
		bRes = runPostCmd();

		if (bRes)
		{
			FactoryLog(true, "runPostCmd", "Pass", "-", "-", "-", "-", m_strMeasured, "dBm", "runPostCmd PASS");
		}
		else
		{
			FactoryLog(false, "runPostCmd", "Fail", "-", "-", "-", "-", m_strMeasured, "dBm", "runPostCmd fail");
		}

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

	if (!(isOk = m_pIPhone->Initial(atoi(m_strCcomPort.c_str()))))
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

	if (paramMap.find(std::string("XMLTestItem")) != paramMap.end())
	{
		m_str_TestItem = paramMap[std::string("XMLTestItem")];
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'TestItem' for class 'CWireless_WiFi_CSDWifiTest'"); 
	//return false;
	}

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



bool CWireless_WiFi_CSDWifiTest::DetectDevice()
{

	std::map<std::string, std::string> map_strstrCOMDeviceName;
	m_strKeyword.assign("QUALCOMM");

	if (GetCOMPortDevByWDK(map_strstrCOMDeviceName))
	{
		std::map<std::string, std::string>::iterator itr;

		if (map_strstrCOMDeviceName.size() < 1)
		{
			//CmdLog("No COM device detected");
			return false;
		}
		else
		{
			bool bFound = false;
			for (itr = map_strstrCOMDeviceName.begin(); itr != map_strstrCOMDeviceName.end(); ++itr)
			{
				std::string strDev = itr->first;
				std::string strDevName = itr->second;
				std::transform(strDevName.begin(), strDevName.end(), strDevName.begin(), ::toupper);

				if (strDevName.find(m_strKeyword) !=  std::string::npos)
				{
					bFound = true;
					size_t iPosition;
					if ((iPosition = strDev.find("COM")) != std::string::npos)
						m_strCcomPort = m_strDeviceName = strDev.substr(3, (strDev.size() - 3));
					break;
					//else
					//	m_strCcomPort = m_strDeviceName = strDev;
					
				}
			}

			if (! bFound)
			{
				TraceLog(MSG_INFO, _T("Fail to find  Comport"));
				return false;
			}

		}
	}
	else
	{
		return false;
	}


	return true;
}


bool CWireless_WiFi_CSDWifiTest::GetCOMPortDevByWDK(std::map<std::string, std::string>& map_strstrCOMDevice)
{
	map_strstrCOMDevice.clear();

	BOOL bContinue = FALSE;

	// 1. Retrieves the GUID(s) associated with the specified class name "Ports"
	DWORD uiGuidSize = 0;
	GUID* ptGuidBuf = NULL;

	::SetupDiClassGuidsFromNameA("Ports", NULL, 0, &uiGuidSize);

	if (uiGuidSize > 0)
	{
		ptGuidBuf = new GUID[uiGuidSize];
		ASSERT(ptGuidBuf != NULL);

		if (ptGuidBuf != NULL)
		{
			bContinue = ::SetupDiClassGuidsFromNameA("Ports", ptGuidBuf, uiGuidSize, &uiGuidSize);
		}
	}

	// 2. Returns a device information set that contains all devices of a specified class
	HDEVINFO hDevInfoSet = INVALID_HANDLE_VALUE;

	if (bContinue)
	{
		hDevInfoSet = ::SetupDiGetClassDevsA(ptGuidBuf, NULL, NULL, DIGCF_PRESENT);
		bContinue = (hDevInfoSet != INVALID_HANDLE_VALUE);
	}

	// 3. Start enumeration
	DWORD uiIndex = 0;

	while (bContinue)
	{
		// Returns one context structure for a device information element of a device information set.
		SP_DEVINFO_DATA tDevInfo;
		tDevInfo.cbSize = sizeof(SP_DEVINFO_DATA);
		if ((bContinue = ::SetupDiEnumDeviceInfo(hDevInfoSet, uiIndex, &tDevInfo)))
		{
			// Opens a registry storage key for device-specific configuration information
			HKEY hKey = ::SetupDiOpenDevRegKey(hDevInfoSet, &tDevInfo, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE);

			if (hKey != NULL)
			{
				// Read the name of the port.
				char szDev[256];
				DWORD uiType = 0, uiDevSize = 256;
				memset(szDev, 0, 256);

				if ((::RegQueryValueExA(hKey, "PortName", NULL, &uiType, (LPBYTE)szDev, &uiDevSize) == ERROR_SUCCESS) && (uiType == REG_SZ))
				{
					std::string strTmp = szDev;

					memset(szDev, 0, 256);
					uiDevSize = 256;
					if (SetupDiGetDeviceRegistryPropertyA(hDevInfoSet, &tDevInfo, SPDRP_DEVICEDESC, &uiType, (PBYTE)szDev, uiDevSize, &uiDevSize) && (uiType == REG_SZ))
						map_strstrCOMDevice[strTmp] = std::string(szDev);
					else
						map_strstrCOMDevice[strTmp] = std::string("");
				}

				// Close the key.
				::RegCloseKey(hKey);
			}
		}

		++uiIndex;
	}

	// clean-up.
	if (ptGuidBuf != NULL)
		delete [] ptGuidBuf;

	if (hDevInfoSet != INVALID_HANDLE_VALUE)
		::SetupDiDestroyDeviceInfoList(hDevInfoSet);

	return true;
}



bool CWireless_WiFi_CSDWifiTest::runPostCmd()
{
	CString cs_write_cmd = "";

	bool bRes = true;
	std::string st_readId = "";
	char sz_cmd_in[FTD_BUF_SIZE] ="";
	char sz_cmd_out[FTD_BUF_SIZE] ="";
	char sz_cmd_errcode[FTD_BUF_SIZE] ="";

	bool b_wait_fastboot = false;
	int nLimitTime = 0 ;
	CString csFastboot("fastboot");

	memset(sz_cmd_in, 0, sizeof(sz_cmd_in));
	memset(sz_cmd_out, 0, sizeof(sz_cmd_out));
	memset(sz_cmd_errcode, 0, sizeof(sz_cmd_errcode));


	strcpy(sz_cmd_in, _T("reboot bootloader"));
	if ( !ExecAdbOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("reboot bootloader Fail"));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		bRes = false;
		goto  Exit_ShowResult;
	}	
	ErrMsg = (_T("runPostCmd reboot bootloader ok"));
	TraceLog(MSG_INFO,  ErrMsg);
	Sleep(3000);


	while ( !b_wait_fastboot){
		if (bCallAdbFastbootCMD(_T("fastboot.exe"), _T("devices"), sz_cmd_out, sz_cmd_errcode, csFastboot) ){
			b_wait_fastboot = true;
			ErrMsg = (_T("Get Fastboot Success"));
			//AfxMessageBox( ErrMsg.c_str() );
			TraceLog(MSG_INFO,  ErrMsg);
		//	AddMsg("Get Fastboot Success.", None, 10);
		}
		Sleep(2000);
		nLimitTime ++;
		if ( nLimitTime > 60 ) break;
	}
	
	if ( ! b_wait_fastboot ) {
		ErrMsg = (_T("reboot to fastboot fail "));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		bRes = false;
		goto  Exit_ShowResult;
	}
	ErrMsg = (_T("runPostCmd reboot to fastboot  ok"));
	TraceLog(MSG_INFO,  ErrMsg);

	strcpy(sz_cmd_in, _T("flash passport passport_FactoryDLTool"));
	if ( !ExecFastbootOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("flash passport passport_FactoryDLTool fail"));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		bRes = false;
		goto  Exit_ShowResult;
	}	
	Sleep(200);
	ErrMsg = (_T("runPostCmd flash passport passport_FactoryDLTool ok"));
	TraceLog(MSG_INFO,  ErrMsg);

	strcpy(sz_cmd_in, _T("oem adb Qoff"));
	if ( !ExecFastbootOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("oem ftd adb fail"));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		bRes = false;
		goto  Exit_ShowResult;
	}	
	Sleep(200);
	ErrMsg = (_T("runPostCmd oem ftd adb off ok"));
	TraceLog(MSG_INFO,  ErrMsg);

	strcpy(sz_cmd_in, _T("oem root Qoff"));
	if ( !ExecFastbootOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("oem ftd root fail"));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		bRes = false;
		goto  Exit_ShowResult;
	}	
	Sleep(200);
	ErrMsg = (_T("runPostCmd oem root off ok"));
	TraceLog(MSG_INFO,  ErrMsg);

	
	strcpy(sz_cmd_in, _T("oem permissive Qoff"));
	if ( !ExecFastbootOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("oem ftd permissive fail"));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		bRes = false;
		goto  Exit_ShowResult;
	}	
	Sleep(200);
	ErrMsg = (_T("runPostCmd oem permissive Qoff ok"));
	TraceLog(MSG_INFO,  ErrMsg);


	strcpy(sz_cmd_in, _T("oem ftd Qoff"));
	if ( !ExecFastbootOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("oem ftd Qoff fail"));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		bRes = false;
		goto  Exit_ShowResult;
	}	
	Sleep(200);
	ErrMsg = (_T("runPostCmd oem ftd Qoff ok"));
	TraceLog(MSG_INFO,  ErrMsg);


Exit_ShowResult:
	if ( !bRes) {
		m_strResult = "FAIL";
	}
	else
	{
		m_strErrorCode = "-";
		m_strResult = "PASS";
	}

	m_strMessage = ErrMsg;
	TraceLog(MSG_INFO,  ErrMsg);
	return bRes;
}



bool CWireless_WiFi_CSDWifiTest::ExecFastbootOut(CString Command, char* output, char* ErrorCode)
{
	bool isOk = false;
	DWORD nPipeSize = 1024 * 1024; //1M pipeline

	CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));
	CString path_adb = pthToolDir + _T("\\fastboot.exe");
	if (_taccess(path_adb, 0) == -1)
	{
		strcpy(ErrorCode, "ERROR: No adb.exe exist!");
		return false;
	}

	HANDLE hRead, hWrite;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, nPipeSize))
	{
		strcpy(ErrorCode, "ERROR: CreatePipe fail!");
		return false;
	}

	//HANDLE hProcess = NULL;
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupInfo.wShowWindow	= SW_HIDE;
	startupInfo.hStdError = hWrite;
	startupInfo.hStdOutput = hWrite;

	Command = _T("\"") + path_adb + _T("\" ") + Command;
	TRACE(_T("Cmd: %s\n"), Command);
	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo))
	{
		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, 10 * 1000); // timeout in 10 seconds

		CloseHandle(hWrite);
		hWrite = NULL;
		//if timeout then exit the process
		if (TimeOutSignal == WAIT_TIMEOUT)
		{
			isOk = false;
			TerminateProcess(processInfo.hProcess, 0);
			strcpy(ErrorCode, "ERROR: Adb timeout");
		}
		else
		{

			isOk = true;
			DWORD bytesRead;
			std::string	std_out = "";
			std::string std_find_string = "error";
			char* message = new char[nPipeSize];
			memset(message, 0, sizeof(message));
			::ReadFile(hRead, message, nPipeSize, &bytesRead, NULL);
			message[bytesRead] = '\0';

			std_out = message;
			if ( std_out.find( std_find_string )  != string::npos) 
			{
				isOk = false;
				strcpy(ErrorCode, "ERROR: adb com fail!");
			}
			else
			{
				strcpy(output, message);
				strcpy(ErrorCode, "Adb command ok");
			}
			delete [] message;

		}
	}
	else
	{
		isOk = false;
		strcpy(ErrorCode, "ERROR: Execute adb.exe fail!");
	}
	Command.ReleaseBuffer();
	CloseHandle(hRead);
	if (hWrite)CloseHandle(hWrite);
	//CloseHandle(hProcess);

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	//hProcess = NULL;

	return isOk;
}



bool CWireless_WiFi_CSDWifiTest::ExecAdbOut(CString Command, char* output, char* ErrorCode)
{
	bool isOk = false;
	DWORD nPipeSize = 1024 * 1024; //1M pipeline

	CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));
	CString path_adb = pthToolDir + _T("\\adb.exe");
	if (_taccess(path_adb, 0) == -1)
	{
		strcpy(ErrorCode, "ERROR: No adb.exe exist!");
		return false;
	}

	HANDLE hRead, hWrite;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, nPipeSize))
	{
		strcpy(ErrorCode, "ERROR: CreatePipe fail!");
		return false;
	}

	//HANDLE hProcess = NULL;
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupInfo.wShowWindow	= SW_HIDE;
	startupInfo.hStdError = hWrite;
	startupInfo.hStdOutput = hWrite;

	Command = _T("\"") + path_adb + _T("\" ") + Command;
	TRACE(_T("Cmd: %s\n"), Command);
	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo))
	{
		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, 10 * 1000); // timeout in 10 seconds

		CloseHandle(hWrite);
		hWrite = NULL;
		//if timeout then exit the process
		if (TimeOutSignal == WAIT_TIMEOUT)
		{
			isOk = false;
			TerminateProcess(processInfo.hProcess, 0);
			strcpy(ErrorCode, "ERROR: Adb timeout");
		}
		else
		{

			isOk = true;
			DWORD bytesRead;
			std::string	std_out = "";
			std::string std_find_string = "error";
			char* message = new char[nPipeSize];
			memset(message, 0, sizeof(message));
			::ReadFile(hRead, message, nPipeSize, &bytesRead, NULL);
			message[bytesRead] = '\0';

			std_out = message;
			if ( std_out.find( std_find_string )  != string::npos) 
			{
				isOk = false;
				strcpy(ErrorCode, "ERROR: adb com fail!");
			}
			else
			{
				strncpy(output, message, 4096);
				strcpy(ErrorCode, "Adb command ok");
			}
			delete [] message;

		}
	}
	else
	{
		isOk = false;
		strcpy(ErrorCode, "ERROR: Execute adb.exe fail!");
	}
	Command.ReleaseBuffer();
	CloseHandle(hRead);
	if (hWrite)CloseHandle(hWrite);
	//CloseHandle(hProcess);

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	//hProcess = NULL;

	return isOk;
}


bool CWireless_WiFi_CSDWifiTest::bCallAdbFastbootCMD(CString csAdbFastboot, CString Command, char* output, char* ErrorCode, CString cs_FindData)
{
	bool isOk = false;
	CString pthToolDir;

	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));
	CString path_adb_fastboot = pthToolDir + _T("\\") + csAdbFastboot;
	
	if (_taccess(path_adb_fastboot, 0) == -1)
	{
		strcpy(ErrorCode, "ERROR: No adb.exe exist!");
		return false;
	}

	HANDLE hRead, hWrite;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, BUFFER_SIZE))
	{
		strcpy(ErrorCode, "ERROR: CreatePipe fail!");
		return false;
	}

	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupInfo.wShowWindow	= SW_HIDE;
	startupInfo.hStdError = hWrite;
	startupInfo.hStdOutput = hWrite;

	Command = _T("\"") + path_adb_fastboot + _T("\" ") + Command;
	TRACE(_T("Cmd: %s\n"), Command);

	DWORD dwRead;
	CHAR chBuf[4096]; 
	bool bSuccess = FALSE;

	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo))
	{
		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, 100 * 1000); // timeout in 10 seconds

		CloseHandle(hWrite);
		hWrite = NULL;
		//if timeout then exit the process
		if (TimeOutSignal == WAIT_TIMEOUT)
		{
			isOk = false;
			TerminateProcess(processInfo.hProcess, 0);
			strcpy(ErrorCode, "ERROR: fastboot timeout");
		}
		else
		{
			if ( (cs_FindData.Find(DREAD) != -1) )// neet to read resolt
			{
				bSuccess = ReadFile( hRead, chBuf, BUFFER_SIZE, &dwRead, NULL);
				chBuf[dwRead] = '\0';
				strncpy(output, chBuf, dwRead);

				if ( bSuccess ) 
				{
					strcpy(ErrorCode, _T("Adb command ok"));
					isOk = true; //don't not need to check
				}
			}
			else if ( ( cs_FindData.Find(DNULL) == -1) )  //need to check return payload
			{
			   for (int i = 0; i < 60; i++) //get QPHONE
			   { 
				  bSuccess = ReadFile( hRead, chBuf, BUFFER_SIZE, &dwRead, NULL);
				  chBuf[dwRead] = '\0';
				  CString csBuf = chBuf;
				  if(csBuf.Find(cs_FindData) != -1) {
					  isOk = true; //get it
					  break;
				  }
				  if( ! bSuccess || dwRead == 0 ) break; 
			   }
			    strcpy(ErrorCode, _T("Adb command ok"));


			}
			else /*do not neet to find anything*/
			{

				isOk = true; //don't not need to check
			}
		}
	}
	else
	{
		isOk = false;
		strcpy(ErrorCode, "ERROR: Execute fastboot.exe fail!");
	}

	Command.ReleaseBuffer();
	if(hRead){
		CloseHandle(hRead);
		hRead = NULL;
	}
	if (hWrite){
		CloseHandle(hWrite);
		hWrite = NULL;
	}

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	processInfo.hProcess = NULL;
	processInfo.hThread = NULL;


	return isOk;
}