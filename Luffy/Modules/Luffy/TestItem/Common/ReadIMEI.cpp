#include "stdafx.h"
#include "ReadIMEI.h"

using namespace StringUtility;

GP_IMPLEMENT_DYNCREATE(CReadIMEI)


CReadIMEI::CReadIMEI()
{
	m_bTestResult = false;
	m_pCGProc = NULL;
	m_pIPhone = NULL;
}

bool CReadIMEI::Run(int i_slot)
{
	MainFunction();

	if (m_pIPhone)
		m_pIPhone = NULL;

	if (m_bTestResult)
	{
		SetUIMessage("Read IMEI PASS");
		SetUIMessage(m_strMsg);
	}
	else
	{
		if (m_strErrCode.empty())
			m_strErrCode = CommErr_Read_IMEI_Fail;

		if (m_strMsg.empty())
			m_strMsg = "Read IMEI FAIL";

		FactoryLog(false, m_NickName, m_strErrCode, "", "", "", "", "", "", m_strMsg);
	}

	//SetResultToPICS
	SetPICSData("IMEI", m_strIMEI);
	if (m_bTestResult)
		SetPICSData(m_NickName, "PASS");
	else
		SetPICSData(m_NickName, "FAIL");

	return m_bTestResult;
}

bool CReadIMEI::MainFunction()
{
	char szIMEI[32];

	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to Initial()";
		TraceLog(m_strMsg);
		m_strErrCode = CommErr_No_Phone_Connected ;
		return false;
	}

	memset(szIMEI, 0, 32);

	if (! m_pIPhone->ReadIMEI(szIMEI, 32))
	{
		m_strMsg = "Read IMEI fail";
		TraceLog(m_strMsg);
		m_strErrCode = CommErr_Read_IMEI_Fail ;
		return false;
	}

	m_bTestResult = true;
	m_strIMEI = szIMEI;
	m_strMsg = MakeSTDstring("IMEI : %s", m_strIMEI.c_str());

	return true;
}


bool CReadIMEI::InitData(std::map<std::string, std::string>& paramMap)
{
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone = dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());

	if (!m_pIPhone)
	{
		TraceLog( "Fail to get address of phone instance for CReadIMEI");
		return false;
	}

	//Reset all value
	m_strErrCode = "";
	m_strMsg = "";
	m_strIMEI = "";
	m_bTestResult = false;

	return true;
}
