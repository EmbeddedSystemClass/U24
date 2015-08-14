#include "stdafx.h"
#include "TouchSelfTest.h"


using namespace StringUtility;


GP_IMPLEMENT_DYNCREATE(CTouchSelfTest)


bool CTouchSelfTest::Run(int iItem)
{
	bool bRes = MainFunction();

	if (m_pIPhone)
		m_pIPhone = NULL;

	if (bRes) 
	{
		FactoryLog(true, "Touch selfTest", "", "", "", "", "", "", "", "Success to Touch selfTest");

		if (! m_strPICSName.empty())
			SetPICSData(m_strPICSName, "PASS");
	}
	else
	{
		if ( m_strErrCode.empty() )
			m_strErrCode = FunErr_Touch_Self_Test_Fail;

		FactoryLog(false, "Touch selfTest", m_strErrCode, "", "", "", "", "", "", m_strMsg);

		if (! m_strPICSName.empty())
			SetPICSData(m_strPICSName, "FAIL");
	}

	return bRes;
}

bool CTouchSelfTest::MainFunction()
{
	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to connect phone with Qisda module";
		return false;
	}

	if (! m_pIPhone->TouchSelfTest())
	{
		m_strMsg = "Fail to do TouchSelfTest()";
		TraceLog(m_strMsg);
		return false;
	}


	return true;
}


bool CTouchSelfTest::InitData(std::map<std::string, std::string>& paramMap)
{
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone =  dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	if (!m_pIPhone)
	{
		TraceLog( "Fail to get address of phone instance for CTouchSelfTest");
		return false;
	}

	//Get myParam_node
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());

	m_strPICSName = myParm_node.child("PICSName").text().as_string(ERROR_STRING);
	CHECK_PARM("PICSName", m_strPICSName, ERROR_STRING);

	return true;
}
