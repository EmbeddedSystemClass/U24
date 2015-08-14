#include "stdafx.h"
#include "ReadWiFiAddr.h"


GP_IMPLEMENT_DYNCREATE(CReadWiFiAddr)


bool CReadWiFiAddr::Run(int iItem)
{
	m_strMeasured = "";

	bool bRes = MainFunction();

	if (m_pIPhone)
		m_pIPhone = NULL;

	if (bRes)
	{
		FactoryLog(true, "ReadWiFiAddr", "", "", "", "", "", m_strMeasured, "", "WiFi Mac address test PASS");
		SetPICSData("WiFiAddr", m_strMeasured);

	}
	else
	{
		FactoryLog(false, "ReadWiFiAddr", CommErr_Read_WLAN_Address_Fail, "", "", "", "", m_strMeasured, "", m_strMsg);
		SetPICSData("WiFiAddr", "FAIL");
	}

	return bRes;
}

bool CReadWiFiAddr::MainFunction()
{
	char chWiFiAddr[4096] = {0};
	memset(chWiFiAddr, 0, sizeof chWiFiAddr);

	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to connect phone Initial()";
		TraceLog(m_strMsg);
		return false;
	}

	// 1. WiFi module on
	bool bRes = false;
	for(int i=0; i<3; ++i)
	{
		if (!(bRes = m_pIPhone->WifiModuleOn(true)))
		{
			m_strMsg = "SwitchWifiModule ON FAIL (CReadWiFiAddr)";
			TraceLog( m_strMsg);
			FactoryLog(false, "WifiModuleOn", FunErr_WLAN_Module_ON_Fail, "", "", "", "", "", "", m_strMsg);	
			continue;
		}

		if(bRes)
			break;	
	}
	if (!bRes)
		return false;

	// 2. Read WiFi Mac Addr.
	if (! m_pIPhone->RWWiFiAddr(false, chWiFiAddr, 4096))
	{
		m_strMsg = "Fail to read WiFi Address";
		TraceLog(m_strMsg);
		return false;
	}

	m_strMeasured = chWiFiAddr;

	if ( m_strMeasured.empty())
	{
		m_strMsg = "WiFi Mac Address is Empty";
		TraceLog(m_strMsg);
		return false;
	}

	return true;
}


bool CReadWiFiAddr::InitData(std::map<std::string, std::string>& paramMap)
{
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone = dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	if (!m_pIPhone)
	{
		TraceLog( "Fail to get address of phone instance for CSpeakerTest");
		return false;
	}

	//Get myParam_node
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());

	//Get parameter 
	m_strPICSName =  myParm_node.child("PICSName").text().as_string(ERROR_STRING);
	CHECK_PARM("PICSName", m_strPICSName, ERROR_STRING);

	return true;
}
