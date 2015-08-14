#include "stdafx.h"
#include "SDColckRate.h"


using namespace StringUtility;


GP_IMPLEMENT_DYNCREATE(CSDClockRate)


bool CSDClockRate::Run(int iItem)
{
	bool bRes = MainFunction();

	if (m_pIPhone)
		m_pIPhone = NULL;

	if (bRes)
	{
		FactoryLog(true, "SD ClockRate", "", "", "", "", "", m_strMeasured, "", "Success to SD ClockRate");

		if (! m_strPICSName.empty())
			SetPICSData(m_strPICSName, "PASS");
	}
	else
	{
		if ( m_strErrCode.empty() )
			m_strErrCode = FunErr_SD_ClockRate_Test_Fail;

		FactoryLog(false, "SD ClockRate", m_strErrCode, "", "", "", "", m_strMeasured, "", m_strMsg);

		if (! m_strPICSName.empty())
			SetPICSData(m_strPICSName, "FAIL");
	}

	return bRes;
}

bool CSDClockRate::MainFunction()
{
	char chClockRate[4096] = {0};
	memset(chClockRate, 0, sizeof chClockRate);
	std::string strClockRate;

	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to connect phone with Initial()";
		TraceLog(m_strMsg);
		return false;
	}

	if (! m_pIPhone->SDClockRate(chClockRate, sizeof(chClockRate)))
	{
		m_strMsg = "Fail to do SDClockRate()";
		TraceLog(m_strMsg);
		return false;
	}

	strClockRate = chClockRate;

	if(m_strClockRate != strClockRate)
		return false;

	return true;
}


bool CSDClockRate::InitData(std::map<std::string, std::string>& paramMap)
{
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone = dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	if (!m_pIPhone)
	{
		TraceLog( "Fail to get address of phone instance for CSDClockRate");
		return false;
	}

	//Get myParam_node
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());

	//Get parameter 
	m_strPICSName =  myParm_node.child("PICSName").text().as_string(ERROR_STRING);
	CHECK_PARM("PICSName", m_strPICSName, ERROR_STRING);

	m_strClockRate =  myParm_node.child("ClockRate").text().as_string(ERROR_STRING);
	CHECK_PARM("ClockRate", m_strClockRate, ERROR_STRING);

    return true ;
}
