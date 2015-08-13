#include "stdafx.h"
#include "BootCurrent.h"


RF_IMPLEMENT_DYNCREATE(CBootCurrent)


bool CBootCurrent::PreRun()
{
	m_strResult = "-";
	m_strItemCode = CStr::IntToStr(BootCurrent_BaseItemCode);
	m_strErrorCode = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "A";
	m_strMessage = "";

	return true;
}

bool CBootCurrent::Run()
{
	bool b_res = BootCurrentTest();

	/* Set PICS Data */
	//SetPICSData("BOOT_CURRENT", m_strMeasured);

	return b_res;
}

bool CBootCurrent::PostRun()
{
	return true;
}

bool CBootCurrent::InitData(std::map<std::string, std::string>& paramMap)
{

	std::string pstrRange;
	//-- LeakageCurrent Spec --//
	if (paramMap.find(std::string("LeakageCurrent_Spec")) != paramMap.end())
	{
		pstrRange = paramMap[std::string("LeakageCurrent_Spec")];
		m_obj_LeakageCurrent_Spec.SetRange(pstrRange);
	}
	else
	{
		TraceLog(MSG_INFO, _T("Get LeakageCurrent_Spec Fail."));
		return false;
	}

	//-- PowerOnShort Spec --//
	if (paramMap.find(std::string("PowerOnShort_Spec")) != paramMap.end())
	{
		pstrRange = paramMap[std::string("PowerOnShort_Spec")];
		m_obj_PowerOnShort_Spec.SetRange(pstrRange);
	}
	else
	{
		TraceLog(MSG_INFO, _T("Get PowerOnShort_Spec Fail."));
		return false;
	}

	//-- WaitTimeForFetch --//
	std::string str_fetchTime;
	if (paramMap.find(std::string("WaitTimeForFetch")) != paramMap.end())
	{
		str_fetchTime = paramMap[std::string("WaitTimeForFetch")];
		m_i_WaitTimeForFetch = CStr::StrToInt(str_fetchTime);
	}
	else
	{
		m_i_WaitTimeForFetch = 1000;
		TraceLog(MSG_INFO, _T("Set WaitTimeForFetch by Default 1000ms."));
	}

	//-- DelayTime --//
	std::string str_delayTime;
	if (paramMap.find(std::string("Delay_Time_ms")) != paramMap.end())
	{
		str_delayTime = paramMap[std::string("Delay_Time_ms")];
		m_i_delayTimeMS = CStr::StrToInt(str_delayTime);
	}
	else
	{
		m_i_delayTimeMS = 2000;
		TraceLog(MSG_INFO, _T("Set delayTimeMS by Default 2000ms."));
	}

	return true;
}

bool CBootCurrent::BootCurrentTest()
{

	if (!m_pIPS)
	{
		m_strResult = "FAIL";
		m_strMessage = "Fail to find powersupply, please check GPIB connection.";
		TraceLog(MSG_INFO, _T("Fail to find powersupply, please check GPIB connection."));
		return false;
	}
/*
	//-- Set power supply range to min --//
	if( !m_pIPS->SetCurrRangeMin())
	{
		m_strResult = "FAIL";
		m_strMessage = "Failed to PowerSupply SetCurrRangeMin";
		TraceLog(MSG_ERROR, _T("Failed to PowerSupply SetCurrRangeMin"));
		return false;
	}

	Sleep(m_i_WaitTimeForFetch);
*/
	int i_retryTestDelay = 100;
	int i_testTotalTime = m_i_delayTimeMS;
	int i_retry = 0;
	bool b_res = false;

	//-- Test Leakage Current --//
	double f_LeakageCurrent;
	b_res = false;
	do {
		i_retry++;
		Sleep(i_retryTestDelay);

		m_pIPS->GetCurrent(f_LeakageCurrent);

		if(m_obj_LeakageCurrent_Spec.InRange(f_LeakageCurrent)) {
			b_res = true;
		}
		CString str_trace;
		str_trace.Format(_T("Leakage Current (%d): %f"), i_retry, f_LeakageCurrent);
		TraceLog(MSG_INFO, std::string(CT2A(str_trace)));
	} while ( (i_retry < (i_testTotalTime/i_retryTestDelay)) && (b_res == false) );
/*
	//-- Set power supply range to max --//
	if(!m_pIPS->SetCurrRangeMax())
	{
		m_strResult = "FAIL";
		m_strMessage = "Failed to PowerSupply SetCurrRangeMax";
		TraceLog(MSG_ERROR,_T("Failed to PowerSupply SetCurrRangeMax"));
		return false;
	}
*/
	//-- LeakageCurrent Compare Spec --//
	m_strMeasured = CStr::DblToStr(f_LeakageCurrent);
	m_strUpper = CStr::FloatToStr((float)m_obj_LeakageCurrent_Spec.GetMax());
	m_strLower = CStr::FloatToStr((float)m_obj_LeakageCurrent_Spec.GetMin());
	if(b_res)
	{
		m_strResult = "PASS";
		m_strMessage = "LeakageCurrent in the spec.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("LEAKAGE_CURRENT", m_strMeasured);
		FactoryLog();
	}
	else
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Leakage_Current_Test_Fail;
		m_strMessage = "LeakageCurrent is not in the spec.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("LEAKAGE_CURRENT", m_strMeasured);
		FactoryLog();
		return false;
	}
	//-- Compare Spec End --//

	if (!m_pIPhone->USB4702_SetDOBit(1, 3))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_SetDOBit3");
		return false;
	}
	Sleep(m_i_WaitTimeForFetch);

	//-- Test Power On Short Current --//
	double f_PowerOnShort;
	b_res = false;

	do {
		i_retry++;
		Sleep(i_retryTestDelay);
		m_pIPS->GetCurrent(f_PowerOnShort);
		if(m_obj_PowerOnShort_Spec.InRange(f_PowerOnShort))
		{
			b_res = true;
		}
		CString str_trace;
		str_trace.Format(_T("Power On Short Current (%d): %f"), i_retry, f_PowerOnShort);
		TraceLog(MSG_INFO, std::string(CT2A(str_trace)));
	} while (i_retry < (i_testTotalTime / i_retryTestDelay) && !b_res);

	//-- PowerOnShort Current Compare Spec --//
	m_strMeasured = CStr::DblToStr(f_PowerOnShort);
	m_strUpper = CStr::FloatToStr((float)m_obj_PowerOnShort_Spec.GetMax());
	m_strLower = CStr::FloatToStr((float)m_obj_PowerOnShort_Spec.GetMin());
	if (b_res)
	{
		m_strResult = "PASS";
		m_strMessage = "PowerOnShort Current in the spec.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("POWER_ON_SHORT", m_strMeasured);
		FactoryLog();
	}
	else
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Boot_Current_Test_Fail;
		m_strMessage = "PowerOnShort Current is not in the spec.";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("POWER_ON_SHORT", m_strMeasured);
		FactoryLog();
		return false;
	}
	//-- Compare Spec End --//

	if (!m_pIPhone->USB4702_ClearDOBit(1, 3))
	{
		TraceLog(MSG_INFO, "Fail to execute m_pIPhone->USB4702_ClearDOBit3");
		return false;
	}
	return true;
}
