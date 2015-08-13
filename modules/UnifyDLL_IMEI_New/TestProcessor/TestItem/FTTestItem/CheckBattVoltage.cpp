#include "stdafx.h"
#include "CheckBattVoltage.h"

RF_IMPLEMENT_DYNCREATE(CCheckBattVoltage)

CCheckBattVoltage::CCheckBattVoltage()
{
}

bool CCheckBattVoltage::PreRun()
{
	m_strItemCode = "";
	m_strErrorCode = "";
	m_strResult = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "";
	m_strMessage = "";
	return true;
}

bool CCheckBattVoltage::Run()
{
	bool bRes = CheckBattVoltage();

	SetPICSData(_T("BattaryVoltageCheck"), m_strMeasured);

	return bRes;
}

bool CCheckBattVoltage::PostRun()
{
	return true;
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
	char szOutput[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_PWRgaugeVoltage(m_nFtdPort, m_nFtdTimeOut, "internal", szOutput))
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

	if(d_output <= m_dVoltage)
	{
		m_strResult = "FAIL";
		m_strMessage = MakeStr("Voltage is less than 3.8V (%.2fV)", d_output);
		m_strErrorCode = FunErr_Check_Battery_Capacity_Fail;
		TraceLog(MSG_INFO, m_strMessage);
		FactoryLog();
		//return false;
		return true;
	}

	m_strResult = "PASS";
	m_strMessage = MakeStr("Check Battery Voltage PASS (%.2fV)", d_output);
	TraceLog(MSG_INFO, m_strMessage);
	FactoryLog();
	return true;
}