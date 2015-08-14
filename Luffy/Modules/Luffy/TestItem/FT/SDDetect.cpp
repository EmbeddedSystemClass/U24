#include "stdafx.h"
#include "SDDetect.h"


using namespace StringUtility;


GP_IMPLEMENT_DYNCREATE(CSDDetect)


bool CSDDetect::Run(int iItem)
{
	bool bRes = MainFunction();

	if (m_pIPhone)
		m_pIPhone = NULL;

	if (bRes)
	{
		FactoryLog(true, "Detect SD Card", "", "", "", "", "", m_strMeasured, "", "Success to Detect SD Card");

		if (! m_strPICSName.empty())
			SetPICSData(m_strPICSName, "PASS");
	}
	else
	{
		if ( m_strErrCode.empty() )
			m_strErrCode = FunErr_Detect_SD_Fail;

		FactoryLog(false, "Detect SD Card", m_strErrCode, "", "", "", "", m_strMeasured, "", m_strMsg);

		if (! m_strPICSName.empty())
			SetPICSData(m_strPICSName, "FAIL");
	}

	return bRes;
}

bool CSDDetect::MainFunction()
{
	char chReadID[4096] = {0};
	memset(chReadID, 0, sizeof chReadID);
	std::string strReadID;

	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to connect phone with Initial()";
		TraceLog(m_strMsg);
		return false;
	}

	if (! m_pIPhone->SDDetect())
	{
		m_strMsg = "Fail to do SDDetect()";
		TraceLog(m_strMsg);
		return false;
	}

	return true;
}


bool CSDDetect::InitData(std::map<std::string, std::string>& paramMap)
{
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone = dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	if (!m_pIPhone)
	{
		TraceLog( "Fail to get address of phone instance for CSDDetect");
		return false;
	}

	//Get myParam_node
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());

	//Get parameter 
	m_strPICSName =  myParm_node.child("PICSName").text().as_string(ERROR_STRING);
	CHECK_PARM("PICSName", m_strPICSName, ERROR_STRING);

    return true ;
}
