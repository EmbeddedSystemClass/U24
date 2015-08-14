#include "stdafx.h"
#include "ChargeCurrent.h"

using namespace StringUtility;
using namespace InspectionUtility;

GP_IMPLEMENT_DYNCREATE(CChargeCurrent)

CChargeCurrent::CChargeCurrent(void)
: m_dChargeVoltage(0.0)
,m_dNormalVoltage(0.0)
,m_iWaitTimeForFetch(0)
,m_dLower(0.0)
,m_dUpper(0.0)
,m_pCGProc(NULL)
,m_pIPS(NULL)
,m_pIPS2(NULL)
{
}

CChargeCurrent::~CChargeCurrent(void)
{
}

bool CChargeCurrent::Run(int iItem)
{
	m_pIPS = m_pCGProc->m_pIPS;

	m_pIPS2 = m_pCGProc->m_pIPS2;

	m_strMeasured = "";

	char szLower[256];
	memset(szLower, 0, 256);
	sprintf_s(szLower, 255, "%.3f", m_dLower);

	char szUpper[256];
	memset(szUpper, 0, 256);
	sprintf_s(szUpper, 255, "%.3f", m_dUpper);

	if (m_pIPS == NULL || m_pIPS2 == NULL)
	{
		FactoryLog(false, "", FunErr_USB_Charging_Current_Test_Fail, "", "", szLower, szUpper, m_strMeasured, "mA", "Power supply not detected");
		SetPICSData("ChargeCurrent", "FAIL");
		SetPICSData("ChargeCurrentValue", "0.000");
		return false;
	}

	bool bRes = MainFunction();

	if (bRes)
	{
		FactoryLog(true, "", "", "", "", szLower, szUpper, m_strMeasured, "mA", "Charge current PASS");
		SetPICSData("ChargeCurrent", "PASS");
		SetPICSData("ChargeCurrentValue", m_strMeasured);
	}
	else
	{
		FactoryLog(false, "", FunErr_USB_Charging_Current_Test_Fail, "", "", szLower, szUpper, m_strMeasured, "mA", m_strMsg);
		SetPICSData("ChargeCurrent", "FAIL");
		SetPICSData("ChargeCurrentValue", m_strMeasured);
	}

	m_pIPS->SetVoltage(m_dNormalVoltage);

	return bRes;
}

bool CChargeCurrent::MainFunction()
{
	double dBatteryCurrent, dUSBCurrent, dSystemCurrent;

	if (! m_pIPS->SetVoltage(m_dChargeVoltage))
	{
		m_strMsg = "Fail to set battery PS to charging voltage";
		TraceLog(m_strMsg);
		return false;
	}

	Sleep(m_iWaitTimeForFetch);

	if (! m_pIPS->GetCurrent(dBatteryCurrent))
	{
		m_strMsg = "Fail to fetch battery PS current";
		TraceLog(m_strMsg);
		return false;
	}

	if (! m_pIPS2->GetCurrent(dUSBCurrent))
	{
		m_strMsg = "Fail to fetch USB PS current";
		TraceLog(m_strMsg);
		return false;
	}

	dBatteryCurrent *= -1000.0;
	dUSBCurrent *= 1000.0;
	dSystemCurrent = dUSBCurrent - dBatteryCurrent;

	char szTmp[256];
	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "Charging current is : %.3f", dBatteryCurrent);
	TraceLog(szTmp);

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "USB output current is : %.3f", dUSBCurrent);
	TraceLog(szTmp);

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "System current is : %.3f", dSystemCurrent);
	TraceLog(szTmp);

	memset(szTmp, 0, 256);
	sprintf_s(szTmp, 255, "%.3f", dBatteryCurrent);
	m_strMeasured = szTmp;

	if (! InRange(m_dLower, m_dUpper, dBatteryCurrent))
	{
		m_strMsg = "Charge current not in range";
		TraceLog(m_strMsg);
		return false;
	}

	return true;
}


bool CChargeCurrent::InitData(std::map<std::string, std::string>& paramMap)
{
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);

	std::vector<std::string> vTmp;
	std::string szString;

	//Get myParam_node
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());

	//Get Parameter

	m_dChargeVoltage = myParm_node.child("ChargeVoltage").text().as_double(ERROR_DOUBLE);
	CHECK_PARM("ChargeVoltage", m_dChargeVoltage, ERROR_DOUBLE);
	

	m_dNormalVoltage = myParm_node.child("NormalVoltage").text().as_double(ERROR_DOUBLE);
	CHECK_PARM("NormalVoltage", m_dNormalVoltage, ERROR_DOUBLE);

	m_iWaitTimeForFetch = myParm_node.child("WaitTimeForFetch").text().as_int(ERROR_INT);
	CHECK_PARM("WaitTimeForFetch", m_iWaitTimeForFetch, ERROR_INT);

	szString =  myParm_node.child("Range").text().as_string(ERROR_STRING);
	CHECK_PARM("Range", szString, ERROR_STRING);

	STDStringTokenizer(szString, ",", vTmp);

	if ((signed)vTmp.size() < 2)
	{
		TraceLog("Fail to find parameter Range for CChargeCurrent");
		return false;
	}
	m_dLower = atof(vTmp[0].c_str());
	m_dUpper = atof(vTmp[1].c_str());

	return true;
}
