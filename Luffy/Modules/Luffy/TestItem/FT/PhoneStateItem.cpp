#include "stdafx.h"
#include "PhoneStateItem.h"


using namespace StringUtility;


GP_IMPLEMENT_DYNCREATE(CPhoneStateItem)


bool CPhoneStateItem::Run(int iItem)
{
	bool bRes = MainFunction();

	if (m_pIPhone)
		m_pIPhone = NULL;

	if (bRes)
	{
		FactoryLog(true, "", "", "", "", "", "", "", "", "Connect phone PASS");
		SetPICSData("IsPhoneConnected", "PASS");
	}
	else
	{
		FactoryLog(false, "", CommErr_No_Phone_Connected, "", "", "", "", "", "", m_strMsg);
		SetPICSData("IsPhoneConnected", "FAIL");
	}

	return bRes;
}

bool CPhoneStateItem::MainFunction()
{
	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to connect phone with Initial()";
		TraceLog(m_strMsg);
		return false;
	}

	Sleep(m_iWaitTimeForBoot);


	bool bIsConnect = false;
	for (int i = 0; i < m_iRetryConnectTimes; ++i)
	{
		/*if (m_pIPhone->FtdModeActive())
		{
			bIsConnect = true;
			break;
		}*/

		Sleep(m_iWaitTimeAfterCommand);
	}

	if (! bIsConnect)
	{
		m_strMsg = "Fail to connect phone with FtdModeActive()";
		TraceLog(m_strMsg);
		return false;
	}

	char szErrMsg[MAX_PATH] = {0};

	Sleep(m_iWaitTimeAfterConnect);

	return true;
}


bool CPhoneStateItem::InitData(std::map<std::string, std::string>& paramMap)
{
	//Get
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone =  dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	if (!m_pIPhone)
	{
		TraceLog("Fail to get address of phone instance for CPhoneStateItem");
		return false;
	}

	//Get myParam_node
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());

	//Get parameter 
	m_iWaitTimeForBoot =  myParm_node.child("WaitTimeForBoot").text().as_int(ERROR_INT);
	CHECK_PARM("WaitTimeForBoot", m_iWaitTimeForBoot, ERROR_INT);

	m_iRetryConnectTimes = myParm_node.child("RetryConnectTimes").text().as_int(ERROR_INT);
	CHECK_PARM("RetryConnectTimes", m_iRetryConnectTimes, ERROR_INT);

	m_iWaitTimeAfterCommand = myParm_node.child("WaitTimeAfterCommand").text().as_int(ERROR_INT);
	CHECK_PARM("WaitTimeAfterCommand", m_iWaitTimeAfterCommand, ERROR_INT);

	m_iWaitTimeAfterConnect = myParm_node.child("WaitTimeAfterConnect").text().as_int(ERROR_INT);
	CHECK_PARM("WaitTimeAfterConnect", m_iWaitTimeAfterConnect, ERROR_INT);

	return true;
}
