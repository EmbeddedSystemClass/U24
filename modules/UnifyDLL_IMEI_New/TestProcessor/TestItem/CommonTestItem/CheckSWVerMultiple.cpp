#include "stdafx.h"
#include "CheckSWVerMultiple.h"


RF_IMPLEMENT_DYNCREATE(CCheckSWVerMultiple)

bool CCheckSWVerMultiple::InitData(std::map<std::string, std::string>& paramMap)
{
	std::string str;

	// VersionType //
	if (paramMap.find(std::string("VersionType")) != paramMap.end())
	{
		m_VersionType = paramMap[std::string("VersionType")].c_str();
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to Get filed 'VersionType' for class 'CCheckSWVerMultiple'"));
		return false;
	}

	return true;
}

bool CCheckSWVerMultiple::PreRun()
{
	m_strResult = "-";
	m_strItemCode = "";
	m_strErrorCode = "";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";

	return true;
}

bool CCheckSWVerMultiple::Run()
{
	g_MutexSWVerReport  = ::CreateMutex(NULL, FALSE, "MUTEX_SW_VER_REPORT");

	CWinThread* pThread[5];//single:0,multi:1,2,3,4
	for (int i = 1; i <= 4; ++i)
	{
		m_hQPhoneProcess[i] = NULL;
		if (m_b_UsableSlot[i] && m_b_isSlotPass[i])
		{
			LPTHREAD_PARAM lpParam = new THREAD_PARAM;
			lpParam->nQPhoneNumber = i;
			lpParam->pThisObject = this;
			pThread[i] = ::AfxBeginThread(CCheckSWVerMultiple::AfxQPhoneThread, lpParam, THREAD_PRIORITY_HIGHEST, 0, 0, NULL);
			pThread[i]->m_bAutoDelete = false;
			m_hQPhoneProcess[i] = pThread[i]->m_hThread;
		}
	}

	for (int i = 1; i <= 4; ++i)
	{
		if (m_b_UsableSlot[i] && m_b_isSlotPass[i])
		{
			DWORD ret = WAIT_OBJECT_0;
			DWORD d;
			d = WaitForSingleObject(m_hQPhoneProcess[i], 30000);
			if (WAIT_TIMEOUT == d)
			{
				ret = WAIT_TIMEOUT;
				m_b_isSlotPass[i] = false;
			}
			else if (WAIT_ABANDONED == d)
			{
				ret = WAIT_ABANDONED_0 + i;
				m_b_isSlotPass[i] = false;
			}
			if (WAIT_OBJECT_0 != ret)
			{
				CString csMsg;
				csMsg.Format(_T("QPhone%d Timeout! (type: %d)"), i, ret);
				TraceLog(MSG_INFO, std::string(csMsg));
			}
		}
		if (m_hQPhoneProcess[i] != NULL)
		{
			delete pThread[i];
			m_hQPhoneProcess[i] = NULL;
		}
	}
	return m_b_isSlotPass[1] || m_b_isSlotPass[2] || m_b_isSlotPass[3] || m_b_isSlotPass[4];
}

bool CCheckSWVerMultiple::PostRun()
{
	return true;
}

UINT CCheckSWVerMultiple::AfxQPhoneThread(LPVOID lpParam)
{
	LPTHREAD_PARAM pParam = (LPTHREAD_PARAM)lpParam;
	THREAD_PARAM stParam;
	stParam.pThisObject = pParam->pThisObject;
	stParam.nQPhoneNumber = pParam->nQPhoneNumber;

	CCheckSWVerMultiple* pQPhoneProc = reinterpret_cast<CCheckSWVerMultiple*>(stParam.pThisObject);
	ASSERT(pQPhoneProc != NULL);
	pQPhoneProc->MainFunction(stParam.nQPhoneNumber);
	delete pParam;

	return TRUE;
}

bool CCheckSWVerMultiple::MainFunction(int nQPhoneNumber)
{
	char output[100] = {0};
	char error[100]  = {0};
	CString cstrAdbCommand;
	cstrAdbCommand.Format(_T("-s QPHONE%d shell getprop ro.build.oemversion.main"), nQPhoneNumber);
	if (!m_pIPhone->ExecAdbCommand(cstrAdbCommand, output, error))
	{
		Report(false, _T("Get Software Version fail"), nQPhoneNumber, output, CommErr_Get_Software_Version_Fail);
		m_b_isSlotPass[nQPhoneNumber] = false;
		return true;
	}

	for (int i = 0; i < 100; i++)
	{
		if ((output[i] == 0x0A) || (output[i] == 0x0D))
		{
			output[i] = 0x00;
		}
	}

	//-- Check SW Version --//
	char SWver[1024];
	if (strcmp(m_VersionType.c_str(), "Factory") == 0)
	{
		GetFactoryVersion(SWver);
	}
	else if (strcmp(m_VersionType.c_str(), "Ship") == 0)
	{
		GetSWVersion(SWver);
	}
	else
	{
		Report(false, "Get XML Version Type fail", nQPhoneNumber, CommErr_Get_Software_Version_Fail);
		m_b_isSlotPass[nQPhoneNumber] = false;
		return true;
	}

	std::string strCheck_SW;
	StrVtr vToken;
	CStr::ParseString(SWver, ",", vToken);
	std::string strMsg;
	bool b_match = false;
	for (size_t i = 0; i < vToken.size(); i++)
	{
		strCheck_SW = vToken[i];
		if ( strCheck_SW == "0" )
		{
			strMsg = "Skip check: " + strCheck_SW;
			b_match = true;
			break;
		}
		if ( strstr(output, strCheck_SW.c_str()) != NULL )
		{
			strMsg = "Match: " + std::string(SWver);
			b_match = true;
			break;
		}
	}

	if (!b_match)
	{
		strMsg = "Not match: " + std::string(SWver);
		Report(false, strMsg, nQPhoneNumber, output, CommErr_Check_Software_Version_Fail);
		m_b_isSlotPass[nQPhoneNumber] = false;
		return true; // neil: always return true
	}
	Report(true, strMsg, nQPhoneNumber, output);
	return true;
}

void CCheckSWVerMultiple::Report(bool bRes, std::string strMessage, int nQPhoneNumber,
								 std::string strMeasured, std::string strErrorCode)
{
	::WaitForSingleObject(g_MutexSWVerReport, 5000);

	CString csTraceMsg;
	CString csQPhoneNumber;
	csQPhoneNumber.Format(_T("%d"), nQPhoneNumber);
	if (bRes)
	{
		m_strResult = "PASS";
		if (nQPhoneNumber > 0 && m_i_QPhoneMultiCtrl >= 1)
		{
			m_strResult += ",#SLOT#" + csQPhoneNumber;
			csTraceMsg.Format(_T("Slot-%d: "), nQPhoneNumber);
		}
		TraceLog(MSG_INFO, std::string(csTraceMsg) + strMessage);
	}
	else
	{
		m_strResult = "FAIL";
		if (nQPhoneNumber > 0 && m_i_QPhoneMultiCtrl >= 1)
		{
			m_strResult += ",#SLOT#" + csQPhoneNumber;
			csTraceMsg.Format(_T("Slot-%d: "), nQPhoneNumber);
		}
		TraceLog(MSG_ERROR, std::string(csTraceMsg) + strMessage);
	}
	m_strErrorCode = strErrorCode;
	m_strMessage   = strMessage;
	m_strMeasured  = strMeasured;
	FactoryLog();

	::ReleaseMutex(g_MutexSWVerReport);
}
