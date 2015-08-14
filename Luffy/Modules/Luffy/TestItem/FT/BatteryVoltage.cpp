#include "stdafx.h"
#include "BatteryVoltage.h"


using namespace InspectionUtility;
using namespace StringUtility;


GP_IMPLEMENT_DYNCREATE(CBatteryVoltage)


bool CBatteryVoltage::Run(int iItem)
{
	bool bRes = MainFunction();

	if (m_pIPhone)
		m_pIPhone = NULL;

	if (bRes)
	{
		FactoryLog(true, "Battery Voltage", "", "", "", m_Range_RTCVoltage.strLower, m_Range_RTCVoltage.strUpper, m_strMeasured, "V", "Success to Battery Voltage");

		if (! m_strPICSName.empty())
			SetPICSData(m_strPICSName, "PASS");
	}
	else
	{
		if ( m_strErrCode.empty() )
			m_strErrCode = FunErr_Check_Battery_Voltage_Fail;

		FactoryLog(false, "Battery Voltage", m_strErrCode, "", "", m_Range_RTCVoltage.strLower, m_Range_RTCVoltage.strUpper, m_strMeasured, "V", m_strMsg);

		if (! m_strPICSName.empty())
			SetPICSData(m_strPICSName, "FAIL");
	}

	return bRes;
}

bool CBatteryVoltage::MainFunction()
{
	/*char szInputCmd[4096] = {0};
	memset(szInputCmd, 0, sizeof szInputCmd);*/

	char chReadBatteryVoltage[256] = {0};
	memset(chReadBatteryVoltage, 0, sizeof chReadBatteryVoltage);

	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to connect phone with Initial()";
		TraceLog(m_strMsg);
		return false;
	}

	/*sprintf(szInputCmd, "%s", "1;1;0;0");*/
	if (! m_pIPhone->CoincellVoltage(chReadBatteryVoltage, 256))
	{
		m_strMsg = "Fail to do CoincellVoltage()";
		TraceLog(m_strMsg);
		return false;
	}

	m_strMeasured = chReadBatteryVoltage;
	m_strMsg = MakeSTDstring("RTCVoltage is %s uV",m_strMeasured.c_str());
	TraceLog(m_strMsg);

	double dMeasured = atof(m_strMeasured.c_str()) / 1e6;
	m_strMeasured = MakeSTDstring("%.3f", dMeasured);

	bool bRet = InRange(m_Range_RTCVoltage.dLower, m_Range_RTCVoltage.dUpper, dMeasured);

	if (! bRet)
	{
		m_strMsg = MakeSTDstring("Fail:RTCVoltage is out of range match, target is %s ",m_strMeasured.c_str());
		TraceLog(m_strMsg);
		return false;
	}
	else
	{
		m_strMsg = MakeSTDstring("Pass:RTCVoltage is in range, value is %s ",m_strMeasured.c_str());
		TraceLog(m_strMsg);
	}

	return true;
}


bool CBatteryVoltage::InitData(std::map<std::string, std::string>& paramMap)
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

	std::string strRTCVoltage_Range;
	std::vector<std::string> vTmp;
	strRTCVoltage_Range =  myParm_node.child("RTCVoltage_Range").text().as_string(ERROR_STRING);
	CHECK_PARM("RTCVoltage_Range", strRTCVoltage_Range, ERROR_STRING);
	STDStringTokenizer(strRTCVoltage_Range, ",", vTmp);
	m_Range_RTCVoltage.dLower = atof(vTmp[0].c_str());
	m_Range_RTCVoltage.dUpper = atof(vTmp[1].c_str());

	m_Range_RTCVoltage.strLower = vTmp[0];
	m_Range_RTCVoltage.strUpper = vTmp[1];

    return true ;
}
