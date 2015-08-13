#include "stdafx.h"
#include "CheckBattVoltage.h"

// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


RF_IMPLEMENT_DYNCREATE(CCheckBattVoltage)

CCheckBattVoltage::CCheckBattVoltage()
{
}

bool CCheckBattVoltage::Run()
{
	bool bRes = CheckBattVoltage();

	if (bRes)
	{
		FactoryLog(true, "Check_Battery_Voltage", "Pass", "", "", "", "", m_strMeasured, "V", "Check Battery Voltage PASS");
	}
	else
	{
		FactoryLog(false, "Check_Battery_Voltage", FunErr_Check_Battery_Capacity_Fail, "", "", "", "", m_strMeasured, "V", m_strMessage);
	}
	SetPICSData(_T("CHECK_BATTERY_VOLTAGE"), m_strMeasured);

	return bRes;
}

bool CCheckBattVoltage::InitData(std::map<std::string, std::string>& paramMap)
{
	std::string str_Voltage;
	if (paramMap.find(std::string("Range")) != paramMap.end())
	{
		str_Voltage = paramMap[std::string("Range")];
		m_dVoltage = CStr::StrToDouble(str_Voltage);
	}
	else
	{
		TraceLog(MSG_INFO, _T("Fail to find parameter 'Voltage' for 'CCheckBattVoltage'"));
		return false;
	}

	return true;
}

bool CCheckBattVoltage::CheckBattVoltage()
{
	char szInput[FTD_BUF_SIZE] = {0};
	strcpy(szInput, "internal");
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_PWRgaugeVoltage(m_nFtdPort, m_nFtdTimeOut, szInput, szOutput))
	{
		m_strMessage = "Read PWRgaugeVoltage from mobile fail";
		TraceLog(MSG_INFO, m_strMessage);
		return false;
	}

	double d_output = atof(szOutput);
	d_output /= 1000000;

	char tmp[FTD_BUF_SIZE] = {0};
	sprintf(tmp, "%f", d_output);
	m_strMeasured = tmp;

	if(d_output <= m_dVoltage) {
		m_strMessage = "Voltage is less than 3.8V";
		return false;
	}


	return true;
}