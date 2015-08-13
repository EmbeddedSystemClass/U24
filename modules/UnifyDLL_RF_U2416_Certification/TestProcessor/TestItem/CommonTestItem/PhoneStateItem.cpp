#include "stdafx.h"
#include "PhoneStateItem.h"


RF_IMPLEMENT_DYNCREATE(CPhoneStateItem)


bool CPhoneStateItem::Run()
{
	bool bRes = MainFunction();

	if (bRes)
	{
		FactoryLog(true, "IsPhoneConnected", "--", "--", "--", "--", "--", "Pass", "--", "Connect phone PASS");
	}
	else
	{
		FactoryLog(false, "", CommErr_No_Phone_Connected, "--", "--", "--", "--", "Fail", "--", m_strMsg);
	}

	return bRes;
}

bool CPhoneStateItem::MainFunction()
{
	bool bIsInitial = false;
	for (int i = 0; i < m_iRetryConnectTimes; ++i)
	{
		if (m_pIPhone->Initial())
		{
			bIsInitial = true;
			break;
		}

		Sleep(500);
	}

	if (! bIsInitial)
	{
		m_strMsg = "Fail to initial phone";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	Sleep(m_iWaitTimeForBoot);

	bool bIsConnect = false;
	for (int i = 0; i < m_iRetryConnectTimes; ++i)
	{
		if (m_pIPhone->IsConnected_FTD())
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
		return false;
	}

	Sleep(m_iWaitTimeAfterConnect);
	return true;
}


bool CPhoneStateItem::InitData(std::map<std::string, std::string>& paramMap)
{
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

	return true;
}
