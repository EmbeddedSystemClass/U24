/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CSapporoFtdTestMulti object.
 * FileName   : CSapporoFtdTestMulti.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Neil.Du
 * Time       : 2013.10.01
 ********************************************************************************/

#include "stdafx.h"
#include "SapporoFtdTestMulti.h"


RF_IMPLEMENT_DYNCREATE(CSapporoFtdTestMulti)


bool CSapporoFtdTestMulti::InitData(std::map<std::string, std::string>& paramMap)
{
	/* Name */
	if (paramMap.find(std::string("Name")) != paramMap.end())
	{
		std_FtdFunction = paramMap[std::string("Name")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Fail to find parameter std_FtdFunction for CSapporoFtdTestMulti"));
		return false;
	}

	return true;
}

bool CSapporoFtdTestMulti::PreRun()
{
	m_strResult = "-";
	m_strItemCode = "-";
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

bool CSapporoFtdTestMulti::Run()
{
	CString cs_FtdFunction = std_FtdFunction.c_str();
	bool b_res = false;

	if (cs_FtdFunction.Compare("runUsbDetectStart") == 0)
		b_res = MultiThreadExecution(USB_DETECT_START);
	else if (cs_FtdFunction.Compare("runUsbDetectEnd") == 0)
		b_res = MultiThreadExecution(USB_DETECT_END);

	return b_res;
}

bool CSapporoFtdTestMulti::PostRun()
{
	return true;
}

bool CSapporoFtdTestMulti::MultiThreadExecution(int nQisdaProcess)
{
	g_MutexUsbDetectReport = ::CreateMutex(NULL, FALSE, "MUTEX_USB_DETECT_REPORT");
	g_MutexUsbDetectFtd = ::CreateMutex(NULL, FALSE, "MUTEX_USB_DETECT_FTD");

	CWinThread* pThread[5];//single:0,multi:1,2,3,4
	for (int i = 1; i <= 4; ++i)
	{
		m_hQPhoneProcess[i] = NULL;
		if( m_b_UsableSlot[i] && m_b_isSlotPass[i] ) {
			LPTHREAD_PARAM lpParam = new THREAD_PARAM;
			lpParam->pThisObject = this;
			lpParam->nQPhoneNumber = i;
			lpParam->nQisdaProcess = nQisdaProcess;
			pThread[i] = ::AfxBeginThread( CSapporoFtdTestMulti::AfxQPhoneThread, lpParam, THREAD_PRIORITY_HIGHEST, 0, 0, NULL );
			pThread[i]->m_bAutoDelete = false;
			m_hQPhoneProcess[i] = pThread[i]->m_hThread;
		}
	}

	for (int i = 1; i <= 4; ++i)
	{
		if( m_b_UsableSlot[i] && m_b_isSlotPass[i] ) {
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
			if ( WAIT_OBJECT_0 != ret) {
				CString csMsg;
				csMsg.Format(_T("QPhone%d Timeout! (type: %d)"), i, ret);
				TraceLog(MSG_INFO, std::string(csMsg));
			}
		}
		if ( m_hQPhoneProcess[i] != NULL ) {
			delete pThread[i];
			m_hQPhoneProcess[i] = NULL;
		}
	}
	return m_b_isSlotPass[1]||m_b_isSlotPass[2]||m_b_isSlotPass[3]||m_b_isSlotPass[4];
}

UINT CSapporoFtdTestMulti::AfxQPhoneThread(LPVOID lpParam)
{
	LPTHREAD_PARAM pParam = (LPTHREAD_PARAM)lpParam;
	THREAD_PARAM stParam;
	stParam.pThisObject = pParam->pThisObject;
	stParam.nQPhoneNumber = pParam->nQPhoneNumber;
	stParam.nQisdaProcess = pParam->nQisdaProcess;

	CSapporoFtdTestMulti* pQPhoneProc = reinterpret_cast<CSapporoFtdTestMulti*>(stParam.pThisObject);
	ASSERT(pQPhoneProc != NULL);
	pQPhoneProc->runUsbDetect( stParam.nQPhoneNumber, stParam.nQisdaProcess );
	delete pParam;

	return TRUE;
}

void CSapporoFtdTestMulti::runUsbDetect(int nQPhoneNumber, int nQisdaProcess)
{
	if ( USB_DETECT_START == nQisdaProcess ) {
		runUsbDetectStart(nQPhoneNumber);
	}else if ( USB_DETECT_END == nQisdaProcess ) {
		runUsbDetectEnd(nQPhoneNumber);
	}
}

bool CSapporoFtdTestMulti::runUsbDetectStart(int nQPhoneNumber)
{
	char sz_Input[FTD_BUF_SIZE] = {0};
	strncpy(sz_Input, "1", 1);
	char sz_Output[FTD_BUF_SIZE] = {0};
	::WaitForSingleObject(g_MutexUsbDetectFtd, 5000);
	bool bRes = m_pIPhone->Sapporo_ftdStartEnd(m_nFtdPort+nQPhoneNumber, m_nFtdTimeOut, sz_Input, sz_Output);
	::ReleaseMutex(g_MutexUsbDetectFtd);

	if (!bRes)
	{
		Report( false, "runUsbDetectStart fail", nQPhoneNumber, "", FunErr_USB_Detect_Start_Fail);
		m_b_isSlotPass[nQPhoneNumber] = false;
	}
	else
	{
		Report( true, "runUsbDetectStart pass", nQPhoneNumber );
	}
	return true;
}

bool CSapporoFtdTestMulti::runUsbDetectEnd(int nQPhoneNumber)
{
	char sz_Input[FTD_BUF_SIZE] = {0};
	strncpy(sz_Input, "0", 1);
	char sz_Output[FTD_BUF_SIZE] = {0};
	::WaitForSingleObject(g_MutexUsbDetectFtd, 5000);
	bool bRes = m_pIPhone->Sapporo_ftdStartEnd(m_nFtdPort+nQPhoneNumber, m_nFtdTimeOut, sz_Input, sz_Output);
	::ReleaseMutex(g_MutexUsbDetectFtd);

	if (!bRes)
	{
		Report( true, "runUsbDetectEnd fail", nQPhoneNumber, "", FunErr_USB_Detect_End_Fail);
		m_b_isSlotPass[nQPhoneNumber] = false;
	}
	else
	{
		Report( true, "runUsbDetectEnd pass", nQPhoneNumber );
	}
	return true;
}

void CSapporoFtdTestMulti::Report(bool bRes, string strMessage, int nQPhoneNumber, string strMeasured, string strErrorCode)
{
	::WaitForSingleObject(g_MutexUsbDetectReport, 5000);

	CString csTraceMsg;
	CString csQPhoneNumber;
	csQPhoneNumber.Format(_T("%d"), nQPhoneNumber);
	if(bRes)
	{
		m_strResult = "PASS";
		if( nQPhoneNumber > 0 && m_i_QPhoneMultiCtrl >= 1) {
			m_strResult += _T(",#SLOT#") + csQPhoneNumber;
			csTraceMsg.Format( _T("Slot-%d: "), nQPhoneNumber);
		}
		TraceLog( MSG_INFO, std::string(csTraceMsg) + strMessage );
	}
	else
	{
		m_strResult = "FAIL";
		if( nQPhoneNumber > 0 && m_i_QPhoneMultiCtrl >= 1) {
			m_strResult += _T(",#SLOT#") + csQPhoneNumber;
			csTraceMsg.Format( _T("Slot-%d: "), nQPhoneNumber);
		}
		TraceLog( MSG_ERROR, std::string(csTraceMsg) + strMessage );
	}
	m_strErrorCode = strErrorCode;
	m_strMessage = strMessage;
	m_strMeasured = strMeasured;
	FactoryLog();

	::ReleaseMutex(g_MutexUsbDetectReport);
}
