#include "stdafx.h"
#include "ConnectPhone_QMSL.h"


RF_IMPLEMENT_DYNCREATE(CConnectPhone_QMSL)

bool CConnectPhone_QMSL::PreRun()
{
	m_strResult = "-";
	m_strItemCode = "-";
	m_strErrorCode = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool CConnectPhone_QMSL::Run()
{
	bool bRes = MainFunction();
	if (bRes)
	{
		m_strResult = _T("TRUE");
		m_strMessage = "Connect phone PASS.";
		FactoryLog();
		SetPICSData(m_strPICSName, "PASS");
	}
	else
	{
		m_strResult = _T("FLASE");
		FactoryLog();
		SetPICSData(m_strPICSName, "FAIL");
	}
	return bRes;
}

bool CConnectPhone_QMSL::MainFunction()
{
	Sleep(m_iWaitTimeForBoot);

	// 1. Initial QMSL
	if (!m_pIPhone->Initial_QMSL())
	{
		m_strMessage = "Fail to initial QMSL";
		TraceLog(MSG_INFO, m_strMessage);
		m_strErrorCode = ERR_QLIB_ConnectServer;
		return false;
	}

	// 2. Connect phone
	m_strErrorCode = ERR_QLIB_PHONE_CONNECT;
	bool bSuccess = false;
	for (int i = 0; i < m_iRetryConnectTimes; ++i)
	{
		if (m_pIPhone->IsConnected_QMSL())
		{
			bSuccess = true;
			break;
		}
		Sleep(500);
	}
	if (!bSuccess)
	{
		CString cstrErrorHandling = _T("CErrorHandling");
		CToolRuntimeClass* pcObjectFactory = RF_RUNTIME_CLASS(CToolBaseObject);
		CToolBaseObject* pcObject = NULL;

		CT2A szErrorHandling(cstrErrorHandling);
		std::string strErrorHandling(szErrorHandling);
		m_pIErrorHandling = NULL;
		if ((pcObject = pcObjectFactory->CreateObject(strErrorHandling)) != NULL)
		{
			m_pIErrorHandling = dynamic_cast<IErrorHandling*>(pcObject);
		}
		if (m_pIErrorHandling != NULL)
		{
			st_UIResult st_Result;
			m_pIErrorHandling->SearchPortUsb(5);
			m_pIErrorHandling->GetTestResult(0, &st_Result);
			m_strErrorCode = st_Result.sz_errCode;
			m_strResult = _T("FALSE");
			m_strMessage = st_Result.sz_msg;
			FactoryLog();
			m_pIErrorHandling = NULL;
		}

		m_strMessage = "Fail to connect phone with QMSL";
		TraceLog(MSG_INFO, m_strMessage);
		return false;
	}

	// 3. Check phone mode
	unsigned long ulPhoneMode;
	bSuccess = false;
	for (int i = 0; i < 10; ++i)
	{
		if (m_pIPhone->QMSL_GetPhoneOperatingMode(&ulPhoneMode))
		{
			bSuccess = true;
			break;
		}
		Sleep(1000);
	}
	if (!bSuccess)
	{
		m_strMessage = "Fail to get phone operating mode with QMSL";
		TraceLog(MSG_INFO, m_strMessage);
		return false;
	}
	if (ulPhoneMode == SYS_OPRT_MODE_LPM || ulPhoneMode == SYS_OPRT_MODE_PWROFF)
	{
		if(ulPhoneMode == SYS_OPRT_MODE_LPM)	TraceLog(MSG_INFO, "Phone is in LPM mode");
		if(ulPhoneMode == SYS_OPRT_MODE_PWROFF)	TraceLog(MSG_INFO, "Phone is in POWERING OFF mode");
/*		TraceLog(MSG_INFO, "Phone is in LPM or POWERING OFF mode");
		if (!m_pIPhone->QMSL_DIAG_CONTROL_F(MODE_FTM_F))
		{
			m_strMessage = "Fail to switch phone to FTM mode with QMSL";
			TraceLog(MSG_INFO, m_strMessage);
			return false;
		}
*/
	}

	Sleep(m_iWaitTimeAfterConnect);
	m_strErrorCode = "";
	return true;
}

bool CConnectPhone_QMSL::PostRun()
{	return true;	}

bool CConnectPhone_QMSL::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find("WaitTimeForBoot") == paramMap.end())
	{
		TraceLog(MSG_INFO, "Fail to find parameter WaitTimeForBoot for CConnectPhone_QMSL");
		return false;
	}
	m_iWaitTimeForBoot = atoi(paramMap["WaitTimeForBoot"].c_str());

	if (paramMap.find("RetryConnectTimes") == paramMap.end())
	{
		TraceLog(MSG_INFO, "Fail to find parameter RetryConnectTimes for CConnectPhone_QMSL");
		return false;
	}
	m_iRetryConnectTimes = atoi(paramMap["RetryConnectTimes"].c_str());

	if (paramMap.find("WaitTimeAfterConnect") == paramMap.end())
	{
		TraceLog(MSG_INFO, "Fail to find parameter WaitTimeAfterConnect for CConnectPhone_QMSL");
		return false;
	}
	m_iWaitTimeAfterConnect = atoi(paramMap["WaitTimeAfterConnect"].c_str());

	if (paramMap.find("PICSName") == paramMap.end())
	{
		//TraceLog(MSG_INFO, "Fail to find parameter PICSName for CConnectPhone_QMSL");
		m_strPICSName.clear();
	}
	else
	{
		m_strPICSName = paramMap["PICSName"];
	}

	return true;
}
