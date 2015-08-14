#include "stdafx.h"
#include "ConnectPhone.h"


GP_IMPLEMENT_DYNCREATE(CConnectPhone)


bool CConnectPhone::Run(int iItem)
{
	bool bRes = MainFunction();

	if (m_pIPhone)
		m_pIPhone = NULL;

	if (bRes)
	{
		SetUIMessage("ConnectPhone PASS");
		SetUIMessage(m_strMsg);

		if (! m_strPICSName.empty())
			SetPICSData(m_strPICSName, "PASS");
	}
	else
	{
		FactoryLog(false, "ConnectPhone", CommErr_No_Phone_Connected, "", "", "", "", "", "", m_strMsg);

		if (! m_strPICSName.empty())
			SetPICSData(m_strPICSName, "FAIL");
	}

	return bRes;
}

bool CConnectPhone::MainFunction()
{
	Sleep(m_iWaitTimeForBoot);

	// 1. Initial Qisda module
	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to initial Qisda module";
		TraceLog(m_strMsg);
		return false;
	}

	// 2. Connect phone
	bool bSuccess = false;
	for (int i = 0; i < m_iRetryConnectTimes; ++i)
	{
		if (m_pIPhone->IsConnected())
		{
			bSuccess = true;
			break;
		}

		Sleep(500);
	}
	if (! bSuccess)
	{
		m_strMsg = "Fail to connect phone with Qisda module";
		TraceLog(m_strMsg);
		return false;
	}

	Sleep(2000);
	bSuccess = true;

	if(! bSuccess)
	{
		m_strMsg = "Fail to active Ftd mode";
		TraceLog(m_strMsg);
		return false;
	}
	Sleep(m_iWaitTimeAfterConnect);
	return true;
}


bool CConnectPhone::InitData(std::map<std::string, std::string>& paramMap)
{
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone = dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());

	if (!m_pIPhone)
	{
		TraceLog( "Fail to get address of phone instance");
		return false;
	}

	//Get myParam_node
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());


	m_iWaitTimeForBoot = myParm_node.child("WaitTimeForBoot").text().as_int(ERROR_INT);
	if (m_iWaitTimeForBoot == ERROR_INT)
	{
		TraceLog( "Fail to find parameter WaitTimeForBoot for CConnectPhone");
		return false;
	}

	m_iRetryConnectTimes = myParm_node.child("RetryConnectTimes").text().as_int(ERROR_INT);
	if (m_iRetryConnectTimes == ERROR_INT)
	{
		TraceLog( "Fail to find parameter RetryConnectTimes for CConnectPhone");
		return false;
	}

	m_iWaitTimeAfterConnect = myParm_node.child("WaitTimeAfterConnect").text().as_int(ERROR_INT);
	if (m_iWaitTimeAfterConnect == ERROR_INT)
	{
		TraceLog( "Fail to find parameter WaitTimeAfterConnect for CConnectPhone");
		return false;
	}

	return true;
}
