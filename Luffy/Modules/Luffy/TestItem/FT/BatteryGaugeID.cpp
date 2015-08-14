#include "stdafx.h"
#include "BatteryGaugeID.h"


using namespace StringUtility;


GP_IMPLEMENT_DYNCREATE(CBatteryGaugeID)


bool CBatteryGaugeID::Run(int iItem)
{
	bool bRes = MainFunction();

	if (m_pIPhone)
		m_pIPhone = NULL;

	if (bRes)
	{
		FactoryLog(true, "Battery Gauge ID", "", "", "", "", "", m_strMeasured, "", "Success to Battery Gauge ID");

		if (! m_strPICSName.empty())
			SetPICSData(m_strPICSName, "PASS");
	}
	else
	{
		if ( m_strErrCode.empty() )
			m_strErrCode = FunErr_Check_Battery_ID_Fail;

		FactoryLog(false, "Battery Gauge ID", m_strErrCode, "", "", "", "", m_strMeasured, "", m_strMsg);

		if (! m_strPICSName.empty())
			SetPICSData(m_strPICSName, "FAIL");
	}

	return bRes;
}

bool CBatteryGaugeID::MainFunction()
{
	char chReadID[256] = {0};
	memset(chReadID, 0, sizeof chReadID);
	std::string strReadID;

	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to connect phone with Initial()";
		TraceLog(m_strMsg);
		return false;
	}

	if (! m_pIPhone->ReadBatteryGaugeID(chReadID, 256))
	{
		m_strMsg = "Fail to do ReadBatteryGaugeID()";
		TraceLog(m_strMsg);
		return false;
	}

	m_strMeasured = chReadID;

	if ( m_strMeasured.find(m_strBatteryID) == -1)
	{
		m_strMsg = MakeSTDstring("Fail:BatteryID is not match(%s), target is %s ", m_strMeasured.c_str(), m_strBatteryID.c_str());
		TraceLog(m_strMsg);
		return false;
	}

	return true;
}


bool CBatteryGaugeID::InitData(std::map<std::string, std::string>& paramMap)
{
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone = dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	if (!m_pIPhone)
	{
		TraceLog( "Fail to get address of phone instance for CBatteryGaugeID");
		return false;
	}

	//Get myParam_node
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());

	//Get parameter 
	m_strPICSName =  myParm_node.child("PICSName").text().as_string(ERROR_STRING);
	CHECK_PARM("PICSName", m_strPICSName, ERROR_STRING);

	m_strBatteryID =  myParm_node.child("BatteryGaugeID").text().as_string(ERROR_STRING);
	CHECK_PARM("BatteryGaugeID", m_strBatteryID, ERROR_STRING);

    return true ;
}
