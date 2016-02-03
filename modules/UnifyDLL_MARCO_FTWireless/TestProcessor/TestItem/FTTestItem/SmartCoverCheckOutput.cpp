
#include "stdafx.h"
#include "SmartCoverCheckOutput.h"

RF_IMPLEMENT_DYNCREATE(CSmartCoverCheckOutput)

bool CSmartCoverCheckOutput::InitData(std::map<std::string, std::string>& paramMap)
{
	std::string str_Range;
	if (paramMap.find(std::string("SmartCoverOutputRange")) != paramMap.end())
	{
		str_Range = paramMap[std::string("SmartCoverOutputRange")];
		m_obj_SmartCoverOutputSpec.SetRange(str_Range);
	}
	else
	{
		TraceLog(MSG_INFO, "Get SmartCoverOutputRange fail.");
		return false;
	}

	return true;
}

bool CSmartCoverCheckOutput::PreRun()
{
	m_strResult    = "";
	m_strItemCode  = "";
	m_strErrorCode = FunErr_SmartCover_Enable3V_Fail;
	m_strBand      = "-";
	m_strChannel   = "-";
	m_strUpper     = CStr::FloatToStr((float)m_obj_SmartCoverOutputSpec.GetMax());
	m_strLower     = CStr::FloatToStr((float)m_obj_SmartCoverOutputSpec.GetMin());
	m_strMeasured  = "-";
	m_strUnit      = "-";
	m_strMessage   = "";

	return true;
}

bool CSmartCoverCheckOutput::Run()
{
	if (m_pIPhone == NULL)
	{
		return false;
	}

	return SmartCoverCheckOutput();
}

bool CSmartCoverCheckOutput::PostRun()
{
	return true;
}

bool CSmartCoverCheckOutput::SmartCoverCheckOutput()
{
	if (m_pIPhone == NULL)
	{
		TraceLog(MSG_INFO, "m_pIPhone is NULL.");
		return false;
	}

	TraceLog(MSG_INFO, "Smart cover check output 3.3V on.");
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->Sapporo_SCoverEnable3V(m_nFtdPort, m_nFtdTimeOut, "1", sz_outBuffer))
	{
		m_strResult = "FAIL";
		m_strMessage = "Sapporo_SCoverEnable3V on fail.";
		TraceLog(MSG_INFO, "Sapporo_SCoverEnable3V on fail.");
		FactoryLog();
		return false;
	}
	TraceLog(MSG_INFO, "Sapporo_SCoverEnable3V on success.");

	//Switch 4702 path: DO D5; ADC:channel 1
	if (!m_pIPhone->USB4702_ClearDOBit(1, 0))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit0");
		return false;
	}
	if (!m_pIPhone->USB4702_SetDOBit(1, 1))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit1");
		return false;
	}
	if (!m_pIPhone->USB4702_ClearDOBit(1, 2))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit2");
		return false;
	}
	if (!m_pIPhone->USB4702_ClearDOBit(1, 3))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit3");
		return false;
	}
	if (!m_pIPhone->USB4702_ClearDOBit(1, 4))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit4");
		return false;
	}
	if (!m_pIPhone->USB4702_SetDOBit(1, 5))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit5");
		return false;
	}

	//GetADCVol
	char sz_SmartCoverOutput[10] = {0};
	if (! m_pIPhone->USB4702_GetADCVol(1, 1, sz_SmartCoverOutput))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_GetADCVol");
		return false;
	}

	//Compare pass criteria
	m_strMeasured = sz_SmartCoverOutput;
	double d_rMeasuredValue = 0;
	d_rMeasuredValue = CStr::StrToDouble(m_strMeasured);
	bool b_res = false;
	if (b_res = m_obj_SmartCoverOutputSpec.InRange(d_rMeasuredValue))
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "Smart cover 3.3V output is in range.";
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "Smart cover 3.3V output is not in range.";
		SetPICSData("SC_CHECK_OUTPUT", m_strMeasured);
	}

	TraceLog(MSG_INFO, m_strMessage);
	SetPICSData(_T("SC_CHECK_OUTPUT"), m_strMeasured);
	FactoryLog();

	// Set D5,D2,D1,D0 to 0,1,1,1 prevent audio noise
	if (!m_pIPhone->USB4702_ClearDOBit(1, 5))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit5");
		return false;
	}
	if (!m_pIPhone->USB4702_SetDOBit(1, 2))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit2");
		return false;
	}
	if (!m_pIPhone->USB4702_SetDOBit(1, 1))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit1");
		return false;
	}
	if (!m_pIPhone->USB4702_SetDOBit(1, 0))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit0");
		return false;
	}
	return b_res;
}
