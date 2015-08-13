#include "stdafx.h"
#include "LeakageCurrent.h"


RF_IMPLEMENT_DYNCREATE(CLeakageCurrent)


bool CLeakageCurrent::Run()
{
	m_strMeasured = "";

	char szLower[256];
	memset(szLower, 0, 256);
	sprintf_s(szLower, 255, "%.3f", m_dLower);

	char szUpper[256];
	memset(szUpper, 0, 256);
	sprintf_s(szUpper, 255, "%.3f", m_dUpper);

	if (m_pIPS == NULL)
	{
		if (m_strErrCode.empty())
			m_strErrCode = CommErr_PowerSupply_Control_Fail;

		FactoryLog(false, "", m_strErrCode, "", "", szLower, szUpper, m_strMeasured, "mA", "Power supply not detected");
		SetPICSData("LeakageCurrent", "FAIL");
		SetPICSData("LeakageCurrentValue", "0.000");

		return false;
	}

	//m_pRBCtrl = new Super4Relay;

	//if (! m_pRBCtrl->InitialRB())
	//{
	//	FactoryLog(false, "", LEAKAGE_CURRENT_FAIL, "", "", szLower, szUpper, m_strMeasured, "mA", "Fail to initial relay board");
	//	SetPICSData("LeakageCurrent", "FAIL");
	//	SetPICSData("LeakageCurrentValue", "0.000");

	//	delete m_pRBCtrl;
	//	return false;
	//}

	bool bRes = MainFunction();

	if (bRes)
	{
		FactoryLog(true, "LeakageCurrent", "Pass", "", "", szLower, szUpper, m_strMeasured, "mA", "Leakage current PASS");
		SetPICSData("LeakageCurrent", "PASS");
		SetPICSData("LeakageCurrentValue", m_strMeasured);
	}
	else
	{
		if (m_strErrCode.empty())
			m_strErrCode = FunErr_Leakage_Current_Test_Fail;

		FactoryLog(false, "LeakageCurrent", m_strErrCode, "", "", szLower, szUpper, m_strMeasured, "mA", m_strMsg);
		SetPICSData("LeakageCurrent", "FAIL");
		SetPICSData("LeakageCurrentValue", m_strMeasured);
	}

	//for (int i = 0; i < 4; ++i)
	//{
	//	m_pRBCtrl->SetRB(0, i, false);
	//}
	//delete m_pRBCtrl;

	m_pIPS->SetCurrRangeMax();
	//m_pIPS->SetPSOnOff(false);
	//m_pIPS2->SetPSOnOff(false);

	return bRes;
}

bool CLeakageCurrent::MainFunction()
{
	double dCurrent;

	if (! m_pIPS2->SetPSOnOff(false))
	{
		m_strMsg = "Fail to set PS2 off";
		return false;
	}

	//if (! m_pRBCtrl->SetRB(0, 1, true))
	//{
	//	m_strMsg = "Fail to set relay board #0 slot R1 to Normal Open";
	//	return false;
	//}
	if (! m_pIPS->SetPSOnOff(false))
	{
		m_strMsg = "Fail to set PS off";
		return false;
	}

	Sleep(m_iWaitTimeForShutdown);

	if (! m_pIPS->SetCurrRangeMin())
	{
		m_strMsg = "Fail to set power supply current range to minimum";
		return false;
	}

	if (! m_pIPS->SetPSOnOff(true))
	{
		m_strMsg = "Fail to set power supply on";
		return false;
	}

	Sleep(m_iWaitTimeForFetch);

	bool bPass = false;
	int iRetryTimes = 10;
	for (int i = 0; i < iRetryTimes; ++i)
	{
		if (! m_pIPS->GetCurrent(dCurrent))
		{
			m_strMsg = "Fail to get current value from power supply";
			return false;
		}

		dCurrent *= 1000.0;

		char szMeasured[256];
		memset(szMeasured, 0, 256);
		sprintf_s(szMeasured, 255, "%.3f", dCurrent);
		m_strMeasured = szMeasured;

		char szTmp[256];
		memset(szTmp, 0, 256);
		sprintf_s(szTmp, 255, "%0d - Leakage Current : %s", i + 1, szMeasured);
		m_strMsg = szTmp;
		TraceLog(MSG_INFO, m_strMsg);

		if (! InRange(m_dLower, m_dUpper, dCurrent))
		{
			if (i < iRetryTimes - 1)
			{
				m_strMsg = "Leakage current not in range, try again";
				TraceLog(MSG_ERROR, m_strMsg);
			}
			else
			{
				m_strMsg = "Leakage current not in range";
				TraceLog(MSG_ERROR, m_strMsg);
				m_strErrCode = FunErr_Leakage_Current_Test_Fail;
			}
		}
		else
		{
			bPass = true;
			break;
		}
		Sleep(1000);
	}

	return bPass;
}


bool CLeakageCurrent::InitData(std::map<std::string, std::string>& paramMap)
{
	std::vector<std::string> vTmp;

	if (paramMap.find("WaitTimeForShutdown") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter WaitTimeForShutdown for CLeakageCurrent");
		return false;
	}
	m_iWaitTimeForShutdown = atoi(paramMap["WaitTimeForShutdown"].c_str());

	if (paramMap.find("WaitTimeForFetch") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter WaitTimeForFetch for CLeakageCurrent");
		return false;
	}
	m_iWaitTimeForFetch = atoi(paramMap["WaitTimeForFetch"].c_str());

	if (paramMap.find("Range") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CLeakageCurrent");
		return false;
	}
	STDStringTokenizer(paramMap["Range"], ",", vTmp);
	if ((signed)vTmp.size() < 2)
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Range for CLeakageCurrent");
		return false;
	}
	m_dLower = atof(vTmp[0].c_str());
	m_dUpper = atof(vTmp[1].c_str());

	return true;
}
