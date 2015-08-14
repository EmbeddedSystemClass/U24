#include "stdafx.h"
#include "BatteryCharging.h"


using namespace StringUtility;


GP_IMPLEMENT_DYNCREATE(CBatteryCharging)


bool CBatteryCharging::Run(int iItem)
{
	bool bRes = MainFunction();

	if (m_pIPhone)
		m_pIPhone = NULL;

	if (bRes) 
	{
		FactoryLog(true, "Charging Status", "", "", "", "", "", "", "", "Success to verify Charging status");

		if (! m_strPICSName.empty())
			SetPICSData(m_strPICSName, "PASS");
	}
	else
	{
		if ( m_strErrCode.empty() )
			m_strErrCode = FunErr_USB_Charging_Current_Test_Fail;

		FactoryLog(false, "Charging Status", m_strErrCode, "", "", "", "", "", "", m_strMsg);

		if (! m_strPICSName.empty())
			SetPICSData(m_strPICSName, "FAIL");
	}

	return bRes;
}

bool CBatteryCharging::MainFunction()
{
	char chChargingStatus[4096] = {0};
	memset(chChargingStatus, 0, sizeof chChargingStatus);
	std::string strChargingStatus;

	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to connect phone with Qisda module";
		return false;
	}

	if (! m_pIPhone->Charger(m_bOnOff))
	{
		m_strMsg = "Fail to do Charger()";
		TraceLog(m_strMsg);
		return false;
	}

	if (! m_pIPhone->ChargerStatus(chChargingStatus, 4096))
	{
		m_strMsg = "Fail to do ChargerStatus()";
		TraceLog(m_strMsg);
		return false;
	}

	strChargingStatus = chChargingStatus;
	if (strChargingStatus != m_strStatus)
	{
		m_strMsg = "Fail to verify ChargerStatus()";
		TraceLog(m_strMsg);
		return false;
	}
	else
	{
		m_strMsg = MakeSTDstring("The charing status is %s", m_strStatus.c_str());
		TraceLog(m_strMsg);
	}

	return true;
}


bool CBatteryCharging::InitData(std::map<std::string, std::string>& paramMap)
{
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone =  dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	if (!m_pIPhone)
	{
		TraceLog( "Fail to get address of phone instance for CGPS_CN0Test_CMU");
		return false;
	}

	//Get myParam_node
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());

	m_strPICSName = myParm_node.child("PICSName").text().as_string(ERROR_STRING);
	CHECK_PARM("PICSName", m_strPICSName, ERROR_STRING);

	CHECK_PARM("ChargingOnOff", myParm_node.child("ChargingOnOff").text().as_string(ERROR_STRING), ERROR_STRING);
	if (myParm_node.child("ChargingOnOff").text().as_int() || (myParm_node.child("ChargingStatus").text().as_string() == "TRUE") || (myParm_node.child("ChargingStatus").text().as_string() == "true"))
		m_bOnOff = true;
	else
		m_bOnOff = false;

	if (m_bOnOff)
		m_strStatus = "Charging";
	else
		m_strStatus = "Discharging";

	return true;
}
