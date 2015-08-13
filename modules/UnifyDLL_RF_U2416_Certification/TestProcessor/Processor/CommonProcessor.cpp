#include "stdafx.h"
#include "CommonProcessor.h"


RF_IMPLEMENT_DYNCREATE(CCommonProcessor)


// --------------------- Test sequence ---------------------

bool CCommonProcessor::Begin()
{
	CToolRuntimeClass* pcObjectFactory = RF_RUNTIME_CLASS(CToolBaseObject);
	CToolBaseObject* pcObject = NULL;

	// 0. Check if calibration mode or not
	//CheckCalibrationMode();

	//// 1. Load cable loss value
	//if (! LoadCableLoss())
	//{	
	//	g_strErrCode = CommErr_XML_Cableloss_Load_Fail;
	//	TraceLog(MSG_ERROR, "Fail to load cable loss values from XML file");
	//	FactoryLog(false, "Load cable loss values from XML", g_strErrCode, "--", "--", "--", "--", "Fail", "--", "Fail to load cable loss values from XML file");
	//	return false;
	//}
/* Move to begining of Run() in order to get g_strPicasso
	// 1.1 Load ref result value
	if (m_bCalibrationMode)
	{
		if (! LoadRefResult())
		{	
			g_strErrCode = CommErr_XML_Cableloss_Load_Fail;
			TraceLog(MSG_ERROR, "Fail to load cable loss values from XML file");
			FactoryLog(false, "Load cable loss values from XML", g_strErrCode, "--", "--", "--", "--", "Fail", "--", "Fail to load cable loss values from XML file");
			return false;
		}
	}
*/
	// 2. Load test processor parameters
	//if (! LoadTestProcessorParams())
	//{
	//	g_strErrCode = CommErr_XML_Testitem_Load_Fail;
	//	TraceLog(MSG_ERROR, "Fail to load test processor parameters");
	//	FactoryLog(false, "Load test processor parameters", g_strErrCode, "--", "--", "--", "--", "Fail", "--", "Fail to load test processor parameters");
	//	return false;
	//}

	//if (m_pITesterDevice == NULL)
	//{
	//	// 3. Load GPIB instruments parameters
	//	if (! LoadGPIBDeviceParams())
	//	{
	//		g_strErrCode = CommErr_XML_Deviceitem_Load_Fail;
	//		TraceLog(MSG_ERROR, "Fail to load GPIB devices parameters");
	//		FactoryLog(false, "Fail to load GPIB devices parameters", g_strErrCode, "--", "--", "--", "--", "Fail", "--", "Fail to load GPIB devices parameters");
	//		return false;
	//	}

	//	// 4. Detect GPIB instruments
	//	if (! DetectGPIBDevice())
	//	{
	//		g_strErrCode = CommErr_Detect_RF_Tester_Fail;
	//		TraceLog(MSG_ERROR, "Fail to detect GPIB devices");
	//		FactoryLog(false, "Fail to detect GPIB devices", g_strErrCode, "--", "--", "--", "--", "Fail", "--", "Fail to detect GPIB devices");
	//		return false;
	//	}

	//	// 5. Create power supply objects & tester device object
	//	std::vector<std::string> vDeviceList;
	//	if (! IGPIBDevice::GetDeviceList(vDeviceList))
	//	{
	//		TraceLog(MSG_ERROR, "Fail to get GPIB device list");
	//		FactoryLog(false, "Fail to get GPIB device list", "--", "--", "--", "--", "--", "Fail", "--", "Fail to get GPIB device list");
	//		return false;
	//	}

	//	for (unsigned int i = 0; i < vDeviceList.size(); ++i)
	//	{
	//		for (std::map<std::string, std::string>::iterator itr = m_mapTesterDeviceParams.begin();
	//			itr != m_mapTesterDeviceParams.end();
	//			++itr)
	//		{
	//			if (vDeviceList[i].find(itr->first) != std::string::npos)
	//			{
	//				if ((pcObject = pcObjectFactory->CreateObject(itr->second)) == NULL)
	//				{
	//					TraceLog(MSG_ERROR, "Fail to create tester device object");
	//					FactoryLog(false, "Fail to create tester device object", "--", "--", "--", "--", "--", "Fail", "--", "Fail to create tester device object");
	//					return false;
	//				}

	//				if (0 == itr->second.compare("CCMU200"))
	//				{
	//					m_pITesterDevice = dynamic_cast<ITesterDevice*>(pcObject);

	//					if (m_pITesterDevice->InitData(vDeviceList[i],
	//						m_pITesterDevice->GetNI488Address(i),
	//						m_pITesterDevice->GetPrimaryAddress(i),
	//						m_pITesterDevice->GetSecondaryAddress(i))
	//						!= true)
	//					{
	//						m_pITesterDevice->Release();
	//						TraceLog(MSG_ERROR, "Fail to execute InitDate() of tester device object");
	//						FactoryLog(false, "Fail to execute InitDate() of tester", "--", "--", "--", "--", "--", "Fail", "--", "Fail to execute InitDate() of tester device object");
	//						return false;
	//					}
	//				}
	//				else if (0 == itr->second.compare("CFSP"))
	//				{
	//					m_pITesterDeviceFSP = dynamic_cast<ITesterDevice*>(pcObject);

	//					if (m_pITesterDeviceFSP->InitData(vDeviceList[i],
	//						m_pITesterDeviceFSP->GetNI488Address(i),
	//						m_pITesterDeviceFSP->GetPrimaryAddress(i),
	//						m_pITesterDeviceFSP->GetSecondaryAddress(i))
	//						!= true)
	//					{
	//						m_pITesterDeviceFSP->Release();
	//						TraceLog(MSG_ERROR, "Fail to execute InitDate() of tester device object");
	//						FactoryLog(false, "Fail to execute InitDate() of tester", "--", "--", "--", "--", "--", "Fail", "--", "Fail to execute InitDate() of tester device object");
	//						return false;
	//					}
	//				}
	//			}
	//		}

	//		for (std::map<std::string, std::string>::iterator itr = m_mapPowerSupplyParams.begin();
	//			itr != m_mapPowerSupplyParams.end();
	//			++itr)
	//		{
	//			if (vDeviceList[i].find(itr->first) != std::string::npos)
	//			{
	//				if ((pcObject = pcObjectFactory->CreateObject(itr->second)) == NULL)
	//				{
	//					TraceLog(MSG_ERROR, "Fail to create power supply object");
	//					FactoryLog(false, "Fail to create power supply", "--", "--", "--", "--", "--", "Fail", "--", "Fail to create power supply object");
	//					return false;
	//				}

	//				IPowerSupply* pIPS = dynamic_cast<IPowerSupply*>(pcObject);

	//				if (m_vPowerSupplyAddr[0] == pIPS->GetPrimaryAddress(i))
	//					m_pIPS = dynamic_cast<IPowerSupply*>(pcObject);
	//				else if (m_vPowerSupplyAddr[1] == pIPS->GetPrimaryAddress(i))
	//					m_pIPS2 = dynamic_cast<IPowerSupply*>(pcObject);
	//				else if (m_vPowerSupplyAddr[2] == pIPS->GetPrimaryAddress(i))
	//					m_pIPS3 = dynamic_cast<IPowerSupply*>(pcObject);
	//				else
	//				{
	//					pIPS->Release();
	//					continue;
	//				}

	//				if (pIPS->InitData(vDeviceList[i],
	//					pIPS->GetNI488Address(i),
	//					pIPS->GetPrimaryAddress(i),
	//					pIPS->GetSecondaryAddress(i))
	//					!= true)
	//				{
	//					pIPS->Release();
	//					TraceLog(MSG_ERROR, "Fail to execute InitDate() of power supply object");
	//					FactoryLog(false, "Begin", "--", "--", "--", "--", "--", "Fail", "--", "Fail to execute InitDate() of power supply object");
	//					return false;
	//				}
	//			}
	//		}
	//	}
	//}

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
	if (m_bCalibrationMode)
	{
		if (! LoadRefResult())
		{	
			strTmp = "Fail to load golden sample values from XML file";
			TraceLog(MSG_ERROR, strTmp);
			FactoryLog(false, "Load golden sample values from XML", CommErr_XML_GoldenSample_Load_Fail, "--", "--", "--", "--", "Fail", "--", strTmp);
			return false;
		}

		if (g_mapTxRefResultData.empty() && g_mapRxRefResultData.empty())
		{
			strTmp = "This is not GOLDEN sample";
			TraceLog(MSG_ERROR, strTmp);
			FactoryLog(false, "Load golden sample values from XML", CommErr_Is_Not_Golden_Sample, "--", "--", "--", "--", "Fail", "--", strTmp);
			return false;
		}
	}
	else
	{
		if (string::npos != g_strPicasso.find(_T("RFGOLDEN")))
		{
			strTmp = "This is GOLDEN sample !!";
			TraceLog(MSG_ERROR, strTmp);
			FactoryLog(false, "WifiModuleOnOFF", CommErr_Is_Golden_Sample, "", "", "", "", "", "", strTmp);
			return false;
		}
	}

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
	g_mapTxCableLossData.clear();
	g_mapRxCableLossData.clear();
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

bool CCommonProcessor::PostRun()
{
#ifndef _ToolInterface
	// 1. Set PICASSO & ToolVer to CIM PICS, Clear picasso after set value
	if (g_mapToolInfo.find("PICASSO") == g_mapToolInfo.end())
	{
		TraceLog(MSG_ERROR, "Fail to find PICASSO in parameters map");
		//FactoryLog(false, "", "", "", "", "", "", "", "", "Fail to find PICASSO in parameters map");
		return false;
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
		//Sleep(3000);
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

bool CCommonProcessor::GetFASector( int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize )
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
		if (! m_pIPhone->GetFASector(i_sectorNum,sz_sectorData,i_sectorSize))
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
