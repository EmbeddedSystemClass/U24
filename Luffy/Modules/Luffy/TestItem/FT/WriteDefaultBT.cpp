#include "stdafx.h"
#include "WriteDefaultBT.h"


using namespace StringUtility;


GP_IMPLEMENT_DYNCREATE(CWriteDefaultBT)


bool CWriteDefaultBT::Run(int iItem)
{
	m_strMeasured = "";
	m_strErrorCode = "";

	bool bRes = MainFunction();

	if (m_pIPhone)
		m_pIPhone = NULL;

	if (bRes)
	{
		FactoryLog(true, "WriteDefaultBTAddr", "", "", "", "", "", m_strMeasured, "", "Write BT Mac address test PASS");
		SetPICSData("WriteDefaultBTAddr", m_strMeasured);

	}
	else
	{
		FactoryLog(false, "WriteDefaultBTAddr", m_strErrorCode, "", "", "", "", m_strMeasured, "", m_strMsg);
		SetPICSData("WriteDefaultBTAddr", "FAIL");
	}

	return bRes;
}

bool CWriteDefaultBT::MainFunction()
{
	char chWiFiAddr[4096] = {0};
	memset(chWiFiAddr, 0, sizeof chWiFiAddr);

	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to connect phone Initial()";
		TraceLog(m_strMsg);
		return false;
	}

	char chWriteAddr[64] = {0};
	sprintf_s(chWriteAddr, 64, "%s", m_strDefaultAddr_comma.c_str());
	// 1. Write default addr. to devices
	if (! m_pIPhone->RWBTAddr(true, chWriteAddr, 64))
	{
		m_strMsg = "Fail to Write BT Address";
		m_strErrorCode = CommErr_Write_BT_Address_Fail;
		TraceLog(m_strMsg);
		return false;
	}


	// 2. Read BT Mac Addr.
	memset(chWiFiAddr, 0, sizeof chWiFiAddr);
	if (! m_pIPhone->RWBTAddr(false, chWiFiAddr, 4096))
	{
		m_strMsg = "Fail to read BT Address";
		m_strErrorCode = CommErr_Read_BT_Address_Fail;
		TraceLog(m_strMsg);
		return false;
	}

	m_strMeasured = chWiFiAddr;

	// 3. Compare with default addr.
	if (m_strMeasured != m_strDefaultAddr)
	{
		m_strMsg = MakeSTDstring("Fail: %s/%s Compare fail!", m_strMeasured.c_str(), m_strDefaultAddr.c_str());
		m_strErrorCode = CommErr_Compare_BT_Address_Fail;
		TraceLog(m_strMsg);
		return false;
	}

	return true;
}


bool CWriteDefaultBT::InitData(std::map<std::string, std::string>& paramMap)
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
	m_strDefaultAddr =  myParm_node.child("WriteDefaultAddr").text().as_string(ERROR_STRING);
	CHECK_PARM("WriteDefaultAddr", m_strDefaultAddr, ERROR_STRING);

	CStringA cstrTmp(m_strDefaultAddr.c_str());
	cstrTmp.Replace(":", ",");
	m_strDefaultAddr_comma = cstrTmp.GetBuffer(0);

	return true;
}
