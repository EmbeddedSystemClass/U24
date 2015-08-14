#include "StdAfx.h"
#include "ADBReboot.h"

using namespace StringUtility;


GP_IMPLEMENT_DYNCREATE(CADBReboot)


bool CADBReboot::Run(int iItem)
{
	bool bRes = MainFunction();

	if (m_pIPhone)
		m_pIPhone = NULL;

	if (bRes)
	{
		SetUIMessage("ADB Reboot phone PASS");
		SetUIMessage(m_strMsg);
		
		SetPICSData("ADBReboot", "PASS");
	}
	else
	{
		if (m_strErrCode.empty())
			m_strErrCode = CommErr_Phone_Restart_Fail;

		if (m_strMsg.empty())
			m_strMsg = "Reboot Device Fail";

		FactoryLog(false, "", m_strErrCode, "", "", "", "", "", "", m_strMsg);
		//SetPICSData("ADBReboot", "FAIL");
	}

	return bRes;
}

bool CADBReboot::MainFunction()
{
	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to connect phone with Initial()";
		TraceLog(m_strMsg);
		return false;
	}

	bool bresult = m_pIPhone->Reboot();

	if(!bresult)
	{
		m_strErrCode = CommErr_Phone_Restart_Fail;
		m_strMsg = "Reboot Device Fail";
		TraceLog(m_strMsg);
		return false;
	}

	return bresult;
}


bool CADBReboot::InitData(std::map<std::string, std::string>& paramMap)
{
	//Get
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone =  dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	if (!m_pIPhone)
	{
		TraceLog("Fail to get address of phone instance for CADBReboot");
		return false;
	}

	//Get myParam_node
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());

	//Get parameter 
	m_iWaitTimeForBoot =  myParm_node.child("WaitTimeForBoot").text().as_int(ERROR_INT);
	CHECK_PARM("WaitTimeForBoot", m_iWaitTimeForBoot, ERROR_INT);

	return true;
}
