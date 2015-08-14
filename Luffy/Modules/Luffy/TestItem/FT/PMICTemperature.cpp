#include "stdafx.h"
#include "PMICTemperature.h"


using namespace InspectionUtility;
using namespace StringUtility;


GP_IMPLEMENT_DYNCREATE(CPMICTemperature)


bool CPMICTemperature::Run(int iItem)
{
	bool bRes = MainFunction();

	if (m_pIPhone)
		m_pIPhone = NULL;

	if (bRes)
	{
		FactoryLog(true, "PMIC Temperature", "", "", "", m_strLower, m_strUpper, m_strMeasured, "", "Success to verify PMIC Temperature");

		if (! m_strPICSName.empty())
			SetPICSData(m_strPICSName, "PASS");
	}
	else
	{
		if ( m_strErrCode.empty() )
			m_strErrCode = FunErr_Check_Battery_Temperature_Fail;

		FactoryLog(false, "PMIC Temperature", m_strErrCode, "", "", m_strLower, m_strUpper, m_strMeasured, "", m_strMsg);

		if (! m_strPICSName.empty())
			SetPICSData(m_strPICSName, "FAIL");
	}

	return bRes;
}

bool CPMICTemperature::MainFunction()
{
	char chTemperature[256] = {0};
	memset(chTemperature, 0, sizeof chTemperature);

	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to connect phone with Qisda module";
		return false;
	}

	if (! m_pIPhone->ReadPMICTemperature(chTemperature, 256))
	{
		m_strMsg = "Fail to do ReadPMICTemperature()";
		TraceLog(m_strMsg);
		return false;
	}

	m_strMeasured = chTemperature;
	double dMeasured = atof(m_strMeasured.c_str());

	char szTmp[512];
	memset(szTmp, 0, 512);
	sprintf_s(szTmp, 512, "%.3f", dMeasured);
	m_strMeasured = szTmp;
	if (! InRange(m_dLower, m_dUpper, dMeasured))
	{
		m_strMsg = MakeSTDstring("Temperature(%.3f) not in range(%.3f,%.3f)", dMeasured, m_dLower, m_dUpper);
		TraceLog(m_strMsg);
		return false;
	}
	else
	{
		m_strMsg = MakeSTDstring("Temperature(%.3f) in range(%.3f,%.3f)", dMeasured, m_dLower, m_dUpper);
		TraceLog(m_strMsg);
	}

	return true;
}

bool CPMICTemperature::InitData(std::map<std::string, std::string>& paramMap)
{
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone = dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	if (!m_pIPhone)
	{
		TraceLog( "Fail to get address of phone instance for CPMICTemperature");
		return false;
	}

	//Get myParam_node
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());

	//Get parameter 
	m_strPICSName =  myParm_node.child("PICSName").text().as_string(ERROR_STRING);
	CHECK_PARM("PICSName", m_strPICSName, ERROR_STRING);

	std::string strTemperatureRangel;
	std::vector<std::string> vTmp;
	strTemperatureRangel =  myParm_node.child("TemperatureRange").text().as_string(ERROR_STRING);
	CHECK_PARM("TemperatureRange", strTemperatureRangel, ERROR_STRING);
	STDStringTokenizer(strTemperatureRangel, ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog("Fail to find parameter SizeRange for CPMICTemperature");
		return false;
	}
	m_dLower = atof(vTmp[0].c_str());
	m_dUpper = atof(vTmp[1].c_str());
	m_strLower = vTmp[0].c_str();
	m_strUpper = vTmp[1].c_str();

    return true ;
}
