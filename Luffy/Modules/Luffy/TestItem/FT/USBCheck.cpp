#include "stdafx.h"
#include "USBCheck.h"


using namespace StringUtility;


GP_IMPLEMENT_DYNCREATE(CCheckUSB)


bool CCheckUSB::Run(int iItem)
{
	m_strMsg = "";

	bool bRes = MainFunction();

	if (m_pIPhone)
		m_pIPhone = NULL;

	if (bRes)
	{
		FactoryLog(true, "CheckUSB", "", "", "", "", "", "", "", "CheckUSB test PASS");
		SetPICSData("CheckUSB", "PASS");
	}
	else    //TODO: update error code
	{
		FactoryLog(false, "CheckUSB", FunErr_Check_CPU_Version_Fail, "", "", "", "", "", "", m_strMsg);
		SetPICSData("CheckUSB", "FAIL");
	}

	return bRes;
}

bool CCheckUSB::MainFunction()
{
	char szOutput[512];
	std::string szUSBType;
	std::string szUSBPresent;
	std::string szUSBMaxCurrent;

	memset(szOutput, 0, 512);

	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to connect phone with Qisda module";
		TraceLog(m_strMsg);
		return false;
	}

	//check present

	if (! m_pIPhone->ReadUSBPresent(szOutput, sizeof(szOutput)))
	{
		m_strMsg = "Read USB Present from phone fail";
		TraceLog(m_strMsg);
		return false;
	}

	szUSBPresent = szOutput;

	m_strMsg = "Read USB Present from phone success : ";

	m_strMsg += szUSBPresent;

	TraceLog(m_strMsg);

	if(szUSBPresent.find(m_strUSBPresent) == -1)
	{
		m_strMsg = "Compare USB Present fail";
		TraceLog(m_strMsg);

		return false;
	}

	//check type

	memset(szOutput, 0, 512);

	if (! m_pIPhone->ReadUSBType(szOutput, sizeof(szOutput)))
	{
		m_strMsg = "Read USB Type from phone fail";
		TraceLog(m_strMsg);
		return false;
	}

	szUSBType = szOutput;

	m_strMsg = "Read USB Present from phone success : ";

	m_strMsg += szUSBType;

	TraceLog(m_strMsg);

	if(szUSBType.find(m_strUSBType) == -1)
	{
		m_strMsg = "Compare USB Type fail";
		TraceLog(m_strMsg);

		return false;
	}

	// check MaxCurrent

	memset(szOutput, 0, 512);

	if (! m_pIPhone->ReadUSBCurrent(szOutput, sizeof(szOutput)))
	{
		m_strMsg = "Read USB Type from phone fail";
		TraceLog(m_strMsg);
		return false;
	}

	szUSBMaxCurrent = szOutput;

	m_strMsg = "Read USB MaxCurrent from phone success : ";

	m_strMsg += szUSBMaxCurrent;

	TraceLog(m_strMsg);

	if(szUSBMaxCurrent.find(m_strUSBMaxCurrent) == -1)
	{
		m_strMsg = "Compare USB MaxCurrent fail";
		TraceLog(m_strMsg);

		return false;
	}

	return true;
}


bool CCheckUSB::InitData(std::map<std::string, std::string>& paramMap)
{
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone = dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	if (!m_pIPhone)
	{
		TraceLog( "Fail to get address of phone instance for CCheckUSB");
		return false;
	}

	//Get myParam_node
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());

	//Get parameter 

	m_strUSBType =  myParm_node.child("USBType").text().as_string(ERROR_STRING);
	CHECK_PARM("USBType", m_strUSBType, ERROR_STRING);

	m_strUSBPresent =  myParm_node.child("USBPresent").text().as_string(ERROR_STRING);
	CHECK_PARM("USBPresent", m_strUSBPresent, ERROR_STRING);

	m_strUSBMaxCurrent =  myParm_node.child("USBMaxCurrent").text().as_string(ERROR_STRING);
	CHECK_PARM("USBMaxCurrent", m_strUSBMaxCurrent, ERROR_STRING);

	return true;
}
