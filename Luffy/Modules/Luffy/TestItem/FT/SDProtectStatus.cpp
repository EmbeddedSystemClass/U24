#include "stdafx.h"
#include "SDProtectStatus.h"


using namespace StringUtility;


GP_IMPLEMENT_DYNCREATE(CSDProtectStatus)


bool CSDProtectStatus::Run(int iItem)
{
	bool bRes = MainFunction();

	if (m_pIPhone)
		m_pIPhone = NULL;

	if (bRes)
	{
		FactoryLog(true, "SD Write Protect status", "", "", "", "", "", m_strMeasured, "", "Success to SD Write Protect status");

		if (! m_strPICSName.empty())
			SetPICSData(m_strPICSName, "PASS");
	}
	else
	{
		if ( m_strErrCode.empty() )
			m_strErrCode = FunErr_SD_Write_Protect_Test_Fail;

		FactoryLog(false, "SD Write Protect status", m_strErrCode, "", "", "", "", m_strMeasured, "", m_strMsg);

		if (! m_strPICSName.empty())
			SetPICSData(m_strPICSName, "FAIL");
	}

	return bRes;
}

bool CSDProtectStatus::MainFunction()
{
	char chReadStatus[4096] = {0};
	memset(chReadStatus, 0, sizeof chReadStatus);
	std::string strReadStatus;

	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to connect phone with Initial()";
		TraceLog(m_strMsg);
		return false;
	}

	if (! m_pIPhone->SDProtectStatus(chReadStatus, sizeof(chReadStatus)))
	{
		m_strMsg = "Fail to do SDProtectStatus()";
		TraceLog(m_strMsg);
		return false;
	}

	strReadStatus = chReadStatus;

	if(strReadStatus.find(m_strStatus) == -1)
		return false;

	return true;
}


bool CSDProtectStatus::InitData(std::map<std::string, std::string>& paramMap)
{
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone = dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	if (!m_pIPhone)
	{
		TraceLog( "Fail to get address of phone instance for CSDProtectStatus");
		return false;
	}

	//Get myParam_node
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());

	//Get parameter 
	m_strPICSName =  myParm_node.child("PICSName").text().as_string(ERROR_STRING);
	CHECK_PARM("PICSName", m_strPICSName, ERROR_STRING);

	m_strStatus =  myParm_node.child("Status").text().as_string(ERROR_STRING);
	CHECK_PARM("Status", m_strStatus, ERROR_STRING);

    return true ;
}
