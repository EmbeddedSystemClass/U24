#include "stdafx.h"
#include "CommonProcessor.h"


RF_IMPLEMENT_DYNCREATE(CCommonProcessor)


// ------------------- Run tool progress -------------------
bool CCommonProcessor::Begin()
{
	TraceLog(MSG_INFO, "-------------------------------- Start Items Test-----------------------------------");
	m_strResult = "";
	m_strItemCode = "";
	m_strErrorCode = "";
	m_strLower = "";
	m_strUpper = "";
	m_strUnit = "";
	m_strMeasured = "";
	m_strMessage = "";

	// 1. Load cable loss value
	if (!LoadCableLossXML())
	{
		m_strMessage = "Fail to load Cable Loss XML";
		TraceLog(MSG_ERROR, m_strMessage);
		return false;
	}
	TraceLog(MSG_INFO, "Load cable loss value success");

	// 2. Parse each test item's parameters
	if (!ParseTestItemParameters())
	{
		m_strMessage = "Fail to parse XML for test items";
		TraceLog(MSG_ERROR, m_strMessage);
		return false;
	}
	TraceLog(MSG_INFO, "Parse each test item's parameters success");

	// 3. Generate all test item objects and set all parameters including NotifyBase
	CToolRuntimeClass* pcObjectFactory = RF_RUNTIME_CLASS(CToolBaseObject);
	CToolBaseObject* pcObject = NULL;
	std::map<std::string, std::string> mapObjectParam;
	for (size_t i = 0; i < m_vTestItemList.size(); ++i)
	{
		if (m_mapTestItemParams.find(m_vTestItemList[i]) == m_mapTestItemParams.end())
		{
			TraceLog(MSG_ERROR, "Fail to load find test item definition");
			return false;
		}

		// Create object
		mapObjectParam = m_mapTestItemParams[m_vTestItemList[i]];
		if ((pcObject = pcObjectFactory->CreateObject(mapObjectParam[m_vTestItemList[i]])) == NULL)
		{
			m_strMessage = "Fail to create " + mapObjectParam[m_vTestItemList[i]] + " object";
			TraceLog(MSG_ERROR, m_strMessage);
			return false;
		}

		// Run the object's InitData() API
		if (pcObject->InitData(mapObjectParam) != true)
		{
			pcObject->Release();
			m_strMessage = "Fail to execute InitData() of " + mapObjectParam[m_vTestItemList[i]] + " object";
			TraceLog(MSG_ERROR, m_strMessage);
			return false;
		}

		// Type cast the object to ITestItem*, and push into a vector
		ITestItem* piTestItem = dynamic_cast<ITestItem*>(pcObject);
		piTestItem->Register(m_piToolNotify, TRACE_LOG);
		piTestItem->Register(m_piToolNotify, FACTORY_LOG);
		piTestItem->Register(m_piToolNotify, PICS_DATA);
		piTestItem->Register(m_piToolNotify, SHOW_PICTURE);
		piTestItem->Register(m_piToolNotify, JIG_REQUEST);
		piTestItem->Register(m_piToolNotify, SHOW_DIALOG);
		m_vpiTestItem.push_back(piTestItem);
	}
	TraceLog(MSG_INFO, "Generate all test item objects and set all parameters success");

	// 2013.01.17 Neil: Notice for start items test
	m_strMessage = "Start Items Test";
	FactoryLog();
	m_strMessage = "";
	return true;
}

bool CCommonProcessor::IsConnected()
{
	/* 1. Connect phone */
	int maxRetry = m_i_WaitTimeStartingUp / 1000;
	bool isConnected = false;
	for (int i = 0; i < maxRetry; i++)
	{
		if (m_pIPhone->IsConnected())
		{
			TraceLog(MSG_INFO, "Find adb device and connected phone success");
			isConnected = true;
			break;
		}
		TraceLog(MSG_ERROR, "Fail to find adb device of phone, Maybe it is starting up. Try again...");
		Sleep(1000);
	}
	if (!isConnected)
	{
		m_strErrorCode = CommErr_Get_ADB_Fail;
		m_strMessage = "Fail to find ADB device of phone.";
		TraceLog(MSG_ERROR, m_strMessage);
		return false;
	}

	/* 2. Set mobile FTD mode if need */
	if (m_b_SetFTDMode && !MobileSetFTDMode())
		return false;

	/* 3. Auto enter FTD mode if need */
	/* 111114 Gilpin.Xi add for service tool start */
	if (m_b_AutoFTDMode && MobileAutoFTDMode())
		return false;
	/* 111114 Gilpin.Xi add for service tool start */

	return true;
}

bool CCommonProcessor::IsConnectedMulti()
{
	int maxRetry = m_i_WaitTimeStartingUp / 5000;
	for (int i = 0; i < maxRetry; i++)
	{
		if (m_pIPhone->IsConnectedMulti())
		{
			TraceLog(MSG_INFO, "Find adb device and connected phone success");
			return true;
		}
		TraceLog(MSG_ERROR, "Fail to find adb device of phone, Maybe it is starting up. Try again...");
		KillADBThread();
	}
	m_strErrorCode = CommErr_Get_ADB_Fail;
	m_strMessage = "Fail to find ADB device of phone.";
	TraceLog(MSG_ERROR, m_strMessage);
	return false;
}

bool CCommonProcessor::IsMobileInFTDMode()
{
	const int maxRetry = 5;
	for (int i = 0; i < maxRetry; i++)
	{
		if (m_pIPhone->IsMobileInFTDMode(m_nFtdPort, m_nFtdTimeOut))
		{
			TraceLog(MSG_INFO, "Check phone FTD mode success");
			return true;
		}
		TraceLog(MSG_ERROR, "Mobile is not in FTD mode, try again...");
		Sleep(1000);
	}
	m_strMessage = "Mobile is not in FTD mode.";
	TraceLog(MSG_ERROR, m_strMessage);
	return false;
}

bool CCommonProcessor::IsSocketReady()
{
	if (m_i_QPhoneMultiCtrl != 0)
		return IsSocketReadyMulti();

	CString csCommand;
	char cmdOut[1024] = {0};
	char errMsg[128] = {0};
	const int MAX_RETRY = 20;
	for (int i = 0; i < MAX_RETRY; i++)
	{
		for (size_t p = 0, sock_num = 0; p < m_pIPhone->m_devices.size(); p++)
		{
			csCommand = _T("");
			if (!m_pIPhone->m_devices[p].empty())
				csCommand.Format(_T("-s %s "), m_pIPhone->m_devices[p].c_str());
			csCommand += _T("shell getprop ro.ftd.socket");
			if (m_pIPhone->ExecAdbCommand(csCommand, cmdOut, errMsg))
			{
				if (strchr(cmdOut, '1') != NULL)
				{
					TraceLog(MSG_INFO, "Check socket success: " + m_pIPhone->m_devices[p]);
					sock_num++;
				}
				if (sock_num == m_pIPhone->m_devices.size())
					return true;
			}
		}
		TraceLog(MSG_ERROR, "Socket is not ready, try again...");
		Sleep(500);
	}
	m_strMessage = "Socket timeout.";
	TraceLog(MSG_ERROR, m_strMessage);
	return false;
}

bool CCommonProcessor::IsSocketReadyMulti()
{
	bool bIsReady = false;
	CString csTraceMsg;
	CString str_command;
	char cmdOut[1024] = {0};
	char errMsg[128] = {0};
	const int MAX_RETRY = 80;
	for (int i = 0; i < MAX_RETRY; i++)
	{
		if (m_b_UsableSlot[1] && m_b_UsableSlot[2] && m_b_UsableSlot[3] && m_b_UsableSlot[4]) break;

		int nIndex = (i % m_i_QPhoneMultiCtrl) + 1;
		if (m_b_UsableSlot[nIndex])
			continue;

		str_command.Format(_T("-s QPHONE%d shell getprop ro.ftd.socket"), m_nQPhoneOrderSlot[nIndex]);
		if (m_pIPhone->ExecAdbCommand(str_command, cmdOut, errMsg))
		{
			if (strchr(cmdOut, '1') != NULL)
			{
				bIsReady = true;
				m_b_UsableSlot[nIndex] = true;
				m_b_isSlotPass[nIndex] = true;
				csTraceMsg.Format(_T("QPhone%d Socket is ready"), nIndex);
				TraceLog(MSG_INFO, std::string(csTraceMsg));
			}
			else
			{
				csTraceMsg.Format(_T("QPhone%d Socket is not in ready"), nIndex);
				TraceLog(MSG_ERROR, std::string(csTraceMsg));
			}
		}
		else
		{
			csTraceMsg.Format(_T("Fail to execute \"%s\""), str_command);
			TraceLog(MSG_ERROR, std::string(csTraceMsg));
		}
		Sleep(100);
	}

	if (!bIsReady)
	{
		m_strMessage = "Socket timeout.";
		TraceLog(MSG_ERROR, m_strMessage);
	}
	return bIsReady;
}

bool CCommonProcessor::PreRun(int nIndex)
{
	if (nIndex >= (signed)m_vTestItemList.size()) return false;

	TraceLog(MSG_INFO, std::string(">>>>>>>>>> Start handling test item " + m_vTestItemList[nIndex] + " <<<<<<<<<<"));
	return m_vpiTestItem[nIndex]->PreRun();
}

bool CCommonProcessor::Run(int nIndex)
{
	if (nIndex >= (signed)m_vTestItemList.size()) return false;

	return m_vpiTestItem[nIndex]->Run();
}

bool CCommonProcessor::PostRun(int nIndex)
{
	if (nIndex >= (signed)m_vTestItemList.size()) return false;

	bool b_res = m_vpiTestItem[nIndex]->PostRun();
	TraceLog(MSG_INFO, std::string(">>>>>>>>>> End handling test item " + m_vTestItemList[nIndex] + " <<<<<<<<<<\n"));
	return b_res;
}

bool CCommonProcessor::Disconnect()
{
	return true;
}

bool CCommonProcessor::End()
{
	// 1. Delete all test item objects
	for (int i = 0; i < (signed)m_vpiTestItem.size(); ++i)
	{
		m_vpiTestItem[i]->Unregister(m_piToolNotify, TRACE_LOG);
		m_vpiTestItem[i]->Unregister(m_piToolNotify, FACTORY_LOG);
		m_vpiTestItem[i]->Unregister(m_piToolNotify, PICS_DATA);
		m_vpiTestItem[i]->Unregister(m_piToolNotify, SHOW_PICTURE);
		m_vpiTestItem[i]->Unregister(m_piToolNotify, JIG_REQUEST);
		m_vpiTestItem[i]->Unregister(m_piToolNotify, SHOW_DIALOG);
		m_vpiTestItem[i]->Release();
	}
	TraceLog(MSG_INFO, "Delete all test item objects success");

	if (m_b_AdbShellStart)
	{
		if (m_pIPhone)
		{
			if (!m_pIPhone->Initial())
			{
				m_strMessage = "Fail to Initial IPhone,Loading Qcomdiag fail!";
				TraceLog(MSG_ERROR, m_strMessage);
				return false;
			}
			if (!m_pIPhone->IsConnected())
			{
				m_strMessage = "Fail to connect phone with Qcomdiag dll" ;
				TraceLog(MSG_ERROR, m_strMessage) ;
				return false;
			}
		}
		char cmdOut[1024] = {0};
		char errMsg[1024] = {0};
		if (!m_pIPhone->ExecAdbCommand("shell start", cmdOut, errMsg))
		{
			m_strMessage = "Fail to Adb_Shell_Start!";
			TraceLog(MSG_ERROR, m_strMessage);
			return false;
		}
		Sleep(3000);//wait 3 sec for Android framework start
	}

	/* 6. Stop power supply 1 */
	if (m_pIPS && m_b_PS1_OnOff)
	{
		if (m_pIPS->PowerSupplyIsAvaliable())
		{
			if (m_pIPS->SetPSOnOff(false))
			{
				TraceLog(MSG_INFO, "Stop power supply 1 success");
			}
			else
			{
				m_strErrorCode = CommErr_PowerSupply_Control_Fail;
				m_strMessage = "Fail to set power supply 1 off";
				TraceLog(MSG_ERROR, m_strMessage);
			}
		}
	}

	// 3. Kill adb thread in Task Manager
	if (KillADBThread())
		TraceLog(MSG_INFO, "Kill adb.exe thread success");
	else
		TraceLog(MSG_ERROR, "Fail to kill adb.exe thread");

	/* 4. Disconnect phone, delete phone object */
	if (m_pIPhone)
	{
		/* Free command driver */
		if (m_i_QPhoneMultiCtrl == 0)
		{
			m_pIPhone->FreeDevice(m_nFtdPort, m_nFtdTimeOut);
			for (size_t i = 0; m_pIPhone->m_devices.size() > 1 && i < m_pIPhone->m_devices.size(); i++)
			{
				int qphone_port = atoi(m_pIPhone->m_devices[i].c_str() + 6);//"QPHONE1"
				int port = (m_pIPhone->m_devices.size() > 1) ? m_nFtdPort + qphone_port : m_nFtdPort;
				m_pIPhone->FreeDevice(port, m_nFtdTimeOut);
			}
		}
		else
		{
			for (int i = 1; i <= m_i_QPhoneMultiCtrl; i++)
			{
				m_pIPhone->FreeDevice((2000 + m_nQPhoneOrderSlot[i]), m_nFtdTimeOut);
			}
		}

		/* Disconnect phone */
		m_pIPhone->Disconnect();
		m_pIPhone->Disconnect_QMSL();
		m_pIPhone->Release();
		m_pIPhone = NULL;
		TraceLog(MSG_INFO, "Disconnect phone, delete phone object success");
	}

	// 5. Clear all data
	m_vpiTestItem.clear();
	m_mapTestItemParams.clear();
	g_mapTxCableLossData.clear();
	g_mapRxCableLossData.clear();
	//g_strPicasso.clear();
	m_csArrPicasso[0].Empty();
	m_csArrPicasso[1].Empty();
	m_csArrPicasso[2].Empty();
	m_csArrPicasso[3].Empty();
	m_csArrPicasso[4].Empty();
	m_nFtdPort = 0;

	m_b_UsableSlot[0] = false;
	m_b_UsableSlot[1] = false;
	m_b_UsableSlot[2] = false;
	m_b_UsableSlot[3] = false;
	m_b_UsableSlot[4] = false;

	m_b_isSlotPass[0] = false;
	m_b_isSlotPass[1] = false;
	m_b_isSlotPass[2] = false;
	m_b_isSlotPass[3] = false;
	m_b_isSlotPass[4] = false;

	TraceLog(MSG_INFO, "Clear all data success");
	TraceLog(MSG_INFO, "------------------------------------ End ----------------------------------------\n\n");
	return true;
}
