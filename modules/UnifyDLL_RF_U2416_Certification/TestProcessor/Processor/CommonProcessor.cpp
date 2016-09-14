#include "stdafx.h"
#include "CommonProcessor.h"


RF_IMPLEMENT_DYNCREATE(CCommonProcessor)


// --------------------- Test sequence ---------------------

bool CCommonProcessor::Begin()
{
	CToolRuntimeClass* pcObjectFactory = RF_RUNTIME_CLASS(CToolBaseObject);
	CToolBaseObject* pcObject = NULL;

	// 6. Load phone parameters
	if (! LoadPhoneParams())
	{
		TraceLog(MSG_ERROR, "Fail to load phone parameters");
		FactoryLog(false, "Begin", "--", "--", "--", "--", "--", "Fail", "--", "Fail to load phone parameters");
		return false;
	}

	// 7. Create phone object
	if (m_pIPhone == NULL)
	{
		CT2A szPhone(m_cstrPhone);
		std::string strPhone(szPhone);

		if ((pcObject = pcObjectFactory->CreateObject(strPhone)) == NULL)
		{
			TraceLog(MSG_ERROR, "Fail to create phone object");
			FactoryLog(false, "Begin", "--", "--", "--", "--", "--", "Fail", "--", "Fail to create phone object");
			return false;
		}

		if (! pcObject->InitData(m_mapPhoneParams))
		{
			TraceLog(MSG_ERROR, "Fail to execute InitDate() of phone object");
			FactoryLog(false, "Begin", "--", "--", "--", "--", "--", "Fail", "--", "Fail to execute InitDate() of phone object");
			return false;
		}

		m_pIPhone = dynamic_cast<IPhone*>(pcObject);
	}
	else
	{
		if (! m_pIPhone->InitData(m_mapPhoneParams))
		{
			TraceLog(MSG_ERROR, "Fail to execute InitDate() of phone object");
			FactoryLog(false, "Begin", "--", "--", "--", "--", "--", "Fail", "--", "Fail to execute InitDate() of phone object");
			return false;
		}
	}

	// 8. Load test item parameters
	if (! LoadTestItemParameters())
	{
		TraceLog(MSG_ERROR, "Fail to load test item parameters");
		FactoryLog(false, "Begin", "--", "--", "--", "--", "--", "Fail", "--", "Fail to load test item parameters");
		return false;
	}

	// 9. Create test item objects
	std::map<std::string, std::string> mapObjectParam;

	for (unsigned int i = 0; i < m_vTestItemList.size(); ++i)
	{
		// Create object
		if (m_mapTestItemParams.find(m_vTestItemList[i]) == m_mapTestItemParams.end())
			return false;

		mapObjectParam = m_mapTestItemParams[m_vTestItemList[i]];
		if ((pcObject = pcObjectFactory->CreateObject(mapObjectParam[m_vTestItemList[i]])) == NULL)
		{
			std::string strTmp = "Fail to create " + mapObjectParam[m_vTestItemList[i]] + " object";
			TraceLog(MSG_ERROR, strTmp);
			FactoryLog(false, "Begin", "--", "--", "--", "--", "--", "Fail", "--", strTmp);
			return false;
		}

		ITestProcessor* piTestItem = dynamic_cast<ITestProcessor*>(pcObject);

		// Enable Notification mechanism
		piTestItem->Register(m_piToolNotify, DLL_PATTERN);
		piTestItem->Register(m_piToolNotify, DLL_TESTING_ITEM);
		piTestItem->Register(m_piToolNotify, DLL_MESSAGE);
		piTestItem->Register(m_piToolNotify, DLL_TEST_RESULT);
		piTestItem->Register(m_piToolNotify, DLL_PICSDATA);
		piTestItem->Register(m_piToolNotify, DLL_JIG_CONTROL);
		piTestItem->Register(m_piToolNotify, DLL_SHOW_MSG_DLG);
		piTestItem->Register(m_piToolNotify, DLL_LOG);

		// Run the object's InitData() API
		if (pcObject->InitData(mapObjectParam) != true)
		{
			pcObject->Release();
			std::string strTmp = "Fail to execute InitData() of " + mapObjectParam[m_vTestItemList[i]] + " object";
			TraceLog(MSG_ERROR, strTmp);
			FactoryLog(false, "Begin", "--", "--", "--", "--", "--", "Fail", "--", strTmp);
			return false;
		}

		m_vpiTestItem.push_back(piTestItem);
	}


	return true;
}

bool CCommonProcessor::Run()
{
	// 0. Move to here from Begin() in order to get g_strPicasso
	std::string strTmp = "";



#ifdef _ToolInterface
		bool isOk = false;



		if (!(isOk = m_pIPhone->Initial_FTD()))
		{
			TraceLog(MSG_ERROR,  "Fail to connect phone with Qisda module  in CWireless_WiFi_CSDWifiTest");
			return false;
		}

		bool b_Res;
		if (! (b_Res = CsdFastbootOpenExe()))
		{
			TraceLog(MSG_ERROR, "Fail to CsdFastbootOpenExe");
			//m_pITool->GetTestResult(0, &st_Result);
			//Fire(UI_RESULT, (long)&st_Result);
			return false;
		}
		TraceLog(MSG_ERROR, "Success to CsdFastbootOpenExe");
		Sleep(1000);


#endif

		if (string::npos != g_strPicasso.find(_T("RFGOLDEN")))
		{
			strTmp = "This is GOLDEN sample !!";
			TraceLog(MSG_ERROR, strTmp);
			FactoryLog(false, "WifiModuleOnOFF", CommErr_Is_Golden_Sample, "", "", "", "", "", "", strTmp);
			return false;
		}
	//}

	// 1. Run all test items
	bool bRes = true;
	bool bResult = true;

	for (int i = 0; i < (signed)m_vpiTestItem.size(); ++i)
	{
		StartTestItemDurationCounter();

		if (! m_bForceToPass)
		{
			if (! bResult)
			{
				if (m_vTestItemTypeList[i] != "MUST")
					continue;
			}
		}

		std::string strTmp = ">>>>>>>>>> Start handling test item " + m_vTestItemList[i] + " <<<<<<<<<<";
		TraceLog(MSG_INFO, strTmp);

		sTestingItem firedData;
		firedData.strTestingItem = m_vTestItemList[i];
		Fire(DLL_TESTING_ITEM, (long)&firedData);

		if (! m_bStopped)
			bRes = m_vpiTestItem[i]->Run();
		else
		{
			TraceLog(MSG_WARNING, ">>>>>>>>>> Stop triggered <<<<<<<<<<");
			return true;
		}

		if (bRes)
		{
			strTmp = m_vTestItemList[i] + " Run() PASS";
			TraceLog(MSG_INFO, strTmp);
		}
		else
		{
			strTmp = m_vTestItemList[i] + " Run() FAIL";
			TraceLog(MSG_ERROR, strTmp);
			bResult = false;
		}

		strTmp = ">>>>>>>>>> End of test item " + m_vTestItemList[i] + " <<<<<<<<<<";
		TraceLog(MSG_INFO, strTmp);

		if (! bRes)
		{
			if (! m_bForceToPass)
			{
				bool bContinue = false;
				for (int j = i + 1; j < (signed)m_vTestItemTypeList.size(); ++j)
				{
					if (m_vTestItemTypeList[j] == "MUST")
						bContinue = true;
				}

				if (! bContinue)
					break;
			}
		}
	}

	SetTestResult(bResult);

	return bResult;
}

bool CCommonProcessor::Stop()
{
	m_bStopped = true;

	return true;
}

// bool CCommonProcessor::Disconnect()
// {
// 	return true;
// }

bool CCommonProcessor::End()
{	
	// 2. Delete all test item objects
	for (int i = 0; i < (signed)m_vpiTestItem.size(); ++i)
	{
		m_vpiTestItem[i]->Unregister(m_piToolNotify, DLL_PATTERN);
		m_vpiTestItem[i]->Unregister(m_piToolNotify, DLL_TESTING_ITEM);
		m_vpiTestItem[i]->Unregister(m_piToolNotify, DLL_MESSAGE);
		m_vpiTestItem[i]->Unregister(m_piToolNotify, DLL_TEST_RESULT);
		m_vpiTestItem[i]->Unregister(m_piToolNotify, DLL_PICSDATA);
		m_vpiTestItem[i]->Unregister(m_piToolNotify, DLL_JIG_CONTROL);
		m_vpiTestItem[i]->Unregister(m_piToolNotify, DLL_SHOW_MSG_DLG);
		m_vpiTestItem[i]->Unregister(m_piToolNotify, DLL_LOG);
		m_vpiTestItem[i]->Release();
	}

	// 3. Clear all data
	m_vpiTestItem.clear();
	m_mapTestItemParams.clear();
//	g_mapTxCableLossData.clear();
//	g_mapRxCableLossData.clear();
	g_strPicasso.clear();

	// 4. Disconnect phone
	if (m_pIPhone != NULL)
	{
		//m_pIPhone->Disconnect();
		m_pIPhone->Disconnect_FTD();
		m_pIPhone->Disconnect_QMSL();
	}

	return true;
}

bool CCommonProcessor::PreRun()
{
	m_bStopped = false;
	StartTotalDurationCounter();
	return true;
}

bool CCommonProcessor::CsdFastbootOpenExe()
{
	TraceLog(MSG_INFO, "start CsdFastbootOpenExe"); 
	bool b_wait_fastboot = false;
	bool bRes = false;
	int nLimitTime = 0 ;

	CString str_command;
	char errMsg[128] = {0};
	char cmdOut[1024] = {0};
	

	str_command = ("devices");
	while ( !b_wait_fastboot){
		if (!m_pIPhone->ExecFastbootCommand(str_command, cmdOut, errMsg))
		{
			TraceLog(MSG_INFO, m_strMessage);
			goto  Exit_ShowResult;
		}

		CString csBuf = cmdOut;
		CString csFastboot("fastboot");
		if(csBuf.Find(csFastboot) != -1) {
		  b_wait_fastboot = true; //get it
		  break;
		}
		Sleep(1000);
		nLimitTime ++;
		if ( nLimitTime > 60 ) break; //time out
	}


	if ( ! b_wait_fastboot ) {
		m_strMessage = _T("reboot to fastboot fail ");
		goto  Exit_ShowResult;
	}
	m_strMessage = "CsdFastbootOpenExe reboot to fastboot ok";
	TraceLog(MSG_INFO, m_strMessage);

	str_command = ("flash passport passport_FactoryDLTool");
	if (!m_pIPhone->ExecFastbootCommand(str_command, cmdOut, errMsg))
	{
		m_strMessage = "Fail to flash passport passport_FactoryDLTool";
		goto  Exit_ShowResult;
	}
	m_strMessage = "CsdFastbootOpenExe flash passport passport_FactoryDLTool ok";
	TraceLog(MSG_INFO, m_strMessage);

	Sleep(1000);

	str_command = ("oem adb Qon");
	if (!m_pIPhone->ExecFastbootCommand(str_command, cmdOut, errMsg))
	{
		m_strMessage = "Fail to oem adb Qon";
		goto  Exit_ShowResult;
	}
	m_strMessage = "CsdFastbootOpenExe oem adb Qon ok";
	TraceLog(MSG_INFO, m_strMessage);
	Sleep(200);

	str_command =  _T("oem root Qon");
	if (!m_pIPhone->ExecFastbootCommand(str_command, cmdOut, errMsg))
	{
		m_strMessage = "Fail to oem root Qon";
		goto  Exit_ShowResult;
	}
	m_strMessage = "CsdOpenFtd oem root Qon ok";
	TraceLog(MSG_INFO, m_strMessage);
	Sleep(200);

	str_command = _T("oem permissive Qon");
	if (!m_pIPhone->ExecFastbootCommand(str_command, cmdOut, errMsg))
	{
		m_strMessage = "Fail to oem permissive Qon";
		goto  Exit_ShowResult;
	}
	m_strMessage = "CsdOpenFtd oem permissive Qon ok";
	TraceLog(MSG_INFO, m_strMessage);
	Sleep(200);

	str_command = _T("oem ftd Qon");
	if (!m_pIPhone->ExecFastbootCommand(str_command, cmdOut, errMsg))
	{
		m_strMessage = "Fail to oem ftd Qon";
		goto  Exit_ShowResult;
	}
	m_strMessage = "CsdFastbootOpenExe oem ftd Qon ok";
	TraceLog(MSG_INFO, m_strMessage);
	Sleep(1000);

	str_command = _T("reboot");
	if (!m_pIPhone->ExecFastbootCommand(str_command, cmdOut, errMsg))
	{
		m_strMessage = "Fail to reboot";
		goto  Exit_ShowResult;
	}
	m_strMessage = "CsdFastbootOpenExe fastboot reboot ok";
	TraceLog(MSG_INFO, m_strMessage);
	Sleep(3000);
	
	bRes = true;

Exit_ShowResult:
	if ( !bRes) {
		m_strResult = "FAIL";
	}
	else
	{
		m_strMessage = (_T("CsdDetectFastboot  ok  \n"));
		m_strErrorCode = "-";
	//	m_strResult = "PASS";
	}

	TraceLog(MSG_INFO, m_strMessage);
	//FactoryLog();
	return bRes;

}

bool CCommonProcessor::PostRun()
{
#ifndef _ToolInterface
	// 1. Set PICASSO & ToolVer to CIM PICS, Clear picasso after set value
	if (g_mapToolInfo.find("PICASSO") == g_mapToolInfo.end())
	{
		TraceLog(MSG_ERROR, "Fail to find PICASSO in parameters map");
		//FactoryLog(false, "", "", "", "", "", "", "", "", "Fail to find PICASSO in parameters map");
		//return false;//LIONTEST
	}
	else
	{
		SetPICSDate("PICASSO", g_mapToolInfo["PICASSO"]);
		g_mapToolInfo.erase("PICASSO");
	}

	if (g_mapToolInfo.find("TOOL_VERSION") == g_mapToolInfo.end())
	{
		TraceLog(MSG_ERROR, "Fail to find ToolVersion in parameters map");
		//FactoryLog(false, "", "", "", "", "", "", "", "", "Fail to find ToolVersion in parameters map");
		return false;
	}
	else
	{
		SetPICSDate("ToolVersion", g_mapToolInfo["TOOL_VERSION"]);
	}
#endif
	//2. disconnect with phone/adb server/QMSL
	if (m_pIPhone)
	{
		m_pIPhone->Disconnect();
		m_pIPhone->Disconnect_QMSL();
		Sleep(500);
	}

	TraceLog(MSG_INFO, "PostRun success!");

	return true;

}

bool CCommonProcessor::SetFAData(int i_slot, char* sz_value)
{
	if (! m_pIPhone)
	{
		if (! CreatePhoneObject())
		{
			TraceLog(MSG_ERROR, "Fail to create phone object for SetFAData()");
			FactoryLog(false, "SetFAData", "--", "--", "--", "--", "--", "Fail", "--", "Fail to create phone object for SetFAData()");
			return false;
		}
	}

	if (! m_pIPhone->Initial())
	{
		TraceLog(MSG_ERROR, "Fail to initial Qisda module for SetFAData()");
		FactoryLog(false, "SetFAData", "--", "--", "--", "--", "--","Fail", "--", "Fail to initial Qisda module for SetFAData()");
		return false;
	}

	bool bIsConnected = false;
	int iCounter = 0;
	int iRetry = 16;
	do
	{
		if (m_pIPhone->IsConnected())
		{
			bIsConnected = true;
			break;
		}

		++iCounter;
		Sleep(500);
	} while (iCounter < iRetry);

	if (bIsConnected)
	{
		if (! m_pIPhone->SetFAData(sz_value))
		{
			TraceLog(MSG_ERROR, "Fail to set FA with Qisda module");
			FactoryLog(false, "SetFAData", "--", "--", "--", "--", "--", "Fail", "--", "Fail to set FA with Qisda module");
		}
		else
		{
			m_pIPhone->EFS_Sync_Combo(10);
			return true;
		}
	}
	else
	{
		TraceLog(MSG_ERROR, "Fail to initial phone connection with Qisda module for SetFAData()");
		FactoryLog(false, "SetFAData", "--", "--", "--", "--", "--","Fail", "--", "Fail to initial phone connection with Qisda module for SetFAData()");
	}

	return false;
}

bool CCommonProcessor::GetFAData(int i_slot, char* sz_value, int i_size)
{
	if (! m_pIPhone)
	{
		if (! CreatePhoneObject())
		{
			TraceLog(MSG_ERROR, "Fail to create phone object for GetFAData()");
			FactoryLog(false, "GetFAData", "--", "--", "--", "--", "--", "Fail", "--", "Fail to create phone object for GetFAData()");
			return false;
		}
	}

	if (! m_pIPhone->Initial())
	{
		TraceLog(MSG_ERROR, "Fail to initial Qisda module for GetFAData()");
		FactoryLog(false, "GetFAData", "--", "--", "--", "--", "--","Fail", "--", "Fail to initial Qisda module for GetFAData()");
		return false;
	}

	bool bIsConnected = false;
	int iCounter = 0;
	int iRetry = 16;
	do
	{
		if (m_pIPhone->IsConnected())
		{
			bIsConnected = true;
			break;
		}

		++iCounter;
		Sleep(500);
	} while (iCounter < iRetry);

	if (bIsConnected)
	{
		if (! m_pIPhone->GetFAData(sz_value,i_size))
		{
			TraceLog(MSG_ERROR, "Fail to get FA with Qisda module");
			FactoryLog(false, "GetFAData", "--", "--", "--", "--", "--", "Fail", "--", "Fail to get FA with Qisda module");
		}
		else
		{
			m_pIPhone->EFS_Sync_Combo(10);
			return true;
		}
	}
	else
	{
		TraceLog(MSG_ERROR, "Fail to initial phone connection with Qisda module for GetFAData()");
		FactoryLog(false, "GetFAData", "--", "--", "--", "--", "--","Fail", "--", "Fail to initial phone connection with Qisda module for GetFAData()");
	}

	return false;
}

bool CCommonProcessor::GetFASector( int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize, int i_idType )
{
	if (! m_pIPhone)
	{
		if (! CreatePhoneObject())
		{
			TraceLog(MSG_ERROR, "Fail to create phone object for GetFAData()");
			FactoryLog(false, "Initial_Phone", "--", "--", "--", "--", "--", "Fail", "--", "Fail to create phone object for GetFASector()");
			return false;
		}
	}
	FactoryLog(true, "Initial_Phone", "--", "--", "--", "--", "--", "", "--", "Phone booting and check connection");

	bool bIsInitial = false;
	int iCounter = 0;
	int iInitialRetry = 5;
	do
	{
		if (m_pIPhone->Initial())
		{
			bIsInitial = true;
			break;
		}

		++iCounter;
		Sleep(1000);
	} while (iCounter < iInitialRetry);

	if (!bIsInitial)
	{
		TraceLog(MSG_ERROR, "Fail to initial phone for GetFASector()");
		FactoryLog(false, "Initial_Phone", "--", "--", "--", "--", "--","Fail", "--", "Fail to initial phone for GetFASector()");
		return false;
	}



	bool bIsConnected = false;
	iCounter = 0;
	int iConnectedRetry = 10;
	do
	{
		if (m_pIPhone->IsConnected_FTD())
		{
			bIsConnected = true;
			break;
		}

		++iCounter;
		Sleep(500);
	} while (iCounter < iConnectedRetry);

	if (bIsConnected)
	{
		if (! m_pIPhone->GetFASector(i_sectorNum,sz_sectorData,i_sectorSize, 1))
		{
			TraceLog(MSG_ERROR, "Fail to get FA with Qisda module");
			FactoryLog(false, "GetFASector", "--", "--", "--", "--", "--", "Fail", "--", "Fail to get FA with Qisda module");
		}
		else
		{
			FactoryLog(true, "GetFASector", "--", "--", "--", "--", "--", "Pass", "--", "Success to check ftd connection after phone booting");
			return true;
		}
	}
	else
	{
		TraceLog(MSG_ERROR, "Fail to initial phone connection with Qisda module for GetFASector()");
		FactoryLog(false, "GetFASector", "--", "--", "--", "--", "--","Fail", "--", "Fail to initial phone connection with Qisda module for GetFASector()");
	}

	return false;
}

bool CCommonProcessor::SetFASector( int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize )
{
	if (! m_pIPhone)
	{
		if (! CreatePhoneObject())
		{
			TraceLog(MSG_ERROR, "Fail to create phone object for SetFASector()");
			FactoryLog(false, "SetFASector", "--", "--", "--", "--", "--", "Fail", "--", "Fail to create phone object for SetFASector()");
			return false;
		}
	}

	if (! m_pIPhone->Initial())
	{
		TraceLog(MSG_ERROR, "Fail to initial Qisda module for SetFASector()");
		FactoryLog(false, "SetFASector", "--", "--", "--", "--", "--","Fail", "--", "Fail to initial Qisda module for SetFASector()");
		return false;
	}

	bool bIsConnected = false;
	int iCounter = 0;
	int iRetry = 16;
	do
	{
		if (m_pIPhone->IsConnected())
		{
			bIsConnected = true;
			break;
		}

		++iCounter;
		Sleep(500);
	} while (iCounter < iRetry);

	if (bIsConnected)
	{
		if (! m_pIPhone->SetFASector(i_sectorNum,sz_sectorData,i_sectorSize))
		{
			TraceLog(MSG_ERROR, "Fail to set FA with Qisda module");
			FactoryLog(false, "SetFASector", "--", "--", "--", "--", "--", "Fail", "--", "Fail to set FA with Qisda module for SetFASector()");
		}
		else
		{
			m_pIPhone->EFS_Sync_Combo(10);
			return true;
		}
	}
	else
	{
		TraceLog(MSG_ERROR, "Fail to initial phone connection with Qisda module for SetFASector()");
		FactoryLog(false, "SetFASector", "--", "--", "--", "--", "--","Fail", "--", "Fail to initial phone connection with Qisda module for SetFASector()");
	}

	return false;
}
  
