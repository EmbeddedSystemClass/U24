/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of BostonButtonCheck object.
 * FileName   : BostonButtonCheck.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Lion.Wu
 * Time       : 2012.03.15
 ********************************************************************************/

#include "stdafx.h"
#include "headsetButtonCheck.h"

RF_IMPLEMENT_DYNCREATE(BostonButtonCheck)

BostonButtonCheck* pButtonTest;

bool BostonButtonCheck::PreRun()
{
	m_strResult = "-";
	m_strItemCode = "-";
	m_strErrorCode = FunErr_Headset_Send_Key_Fail;
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool BostonButtonCheck::Run()
{
	return runButtonCount();
}

bool BostonButtonCheck::PostRun()
{
	return true;
}

bool BostonButtonCheck::InitData(std::map<std::string, std::string>& paramMap)
{
	std::string strTimeOut;
	if (paramMap.find(std::string("TimeOut")) != paramMap.end())
	{
		strTimeOut = paramMap[std::string("TimeOut")];
		TimeOut = CStr::StrToInt(strTimeOut);
	}
	else
	{
		TraceLog(MSG_ERROR, "Failed to Get TimeOut for class BostonButtonCheck");
		return false;
	}
	return true;
}

bool BostonButtonCheck::runButtonCount()
{
	std::string str_msg;

	/* Init dialog response flag to false */
	bool b_res = false;

	/* initial ButtonCount */
	char sz_ButtonTypePress[FTD_BUF_SIZE] = {0};
	b_res = m_pIPhone->FTD_AUButtonCount(m_nFtdPort, m_nFtdTimeOut, sz_ButtonTypePress);
	if (!b_res)
	{
		str_msg = "runButtonCount initial fail";
		TraceLog(MSG_INFO, str_msg);
	}

	int n_ButtoncountResult = 0;
	b_res = InitialKeyTestDlg() ;
	if (b_res)
	{
		n_ButtoncountResult = RunButtonTestProcess();
	}
	EndButtonTestDlg();

	if (n_ButtoncountResult == 0) b_res = false;

	CString cs_Result;
	cs_Result.Format("%s (%s, %d)", "ButtonCheck Value = ", sz_ButtonTypePress, n_ButtoncountResult);
	str_msg = cs_Result;

	/* test result */
	if (b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check ButtonCheck PASS!");
		SetPICSData("Headset_Button_CHECK", "PASS");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Check ButtonCheck FAIL!");
		SetPICSData("Headset_Button_CHECK", "FAIL");
	}
	FactoryLog();
	return b_res;
}

UINT BostonButtonCheck::AfxKeyTestDlgThread(LPVOID pParam)
{
	pButtonTest = (BostonButtonCheck*)pParam;
	int nRet = pButtonTest->pButtonTestDlg->DoModal();
	if (nRet == -1)
	{
		pButtonTest->TraceLog(MSG_ERROR, "AfxKeyTestDlgThread Create Error");
	}
	return true;
}

bool BostonButtonCheck::InitialKeyTestDlg()
{
	pButtonTestDlg = new CButtonCountTestDlg;
	int iPanelHPixel = 1100;
	int iPanelVPixel = 1900;
	int iBlockLenght = 300;
	int iBlockWidth = 100;
	int iDisplayRate = 2;
	if (!pButtonTestDlg->Initial_Parameter(iPanelHPixel, iPanelVPixel, iBlockLenght, iBlockWidth, iDisplayRate))
	{
		m_strMessage = "Initial key test dialog fail!";
		TraceLog(MSG_ERROR, "Initial key test dialog fail!");
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Initial key test dialog success!");
	}

	KeyTestThreadHandle = AfxBeginThread(BostonButtonCheck::AfxKeyTestDlgThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	Sleep(200);//for Thread Handle Initial Dialog
	if (KeyTestThreadHandle == NULL)
	{
		m_strMessage = "Create Touch point dialog fail!";
		TraceLog(MSG_ERROR, "Create Touch point dialog fail!");
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Create touch point test dialog success!");
	}
	return true;
}

int BostonButtonCheck::RunButtonTestProcess()
{
	b_StopkKeyTest = false;
	bool b_res = false;
	clock_t start = clock(); // Count start time
	clock_t finish;

	int nCount;
	CString cs_Result;
	char sz_ButtonTypePress[FTD_BUF_SIZE] = {0};
	while (1)
	{
		nCount = 0 ;
		b_res = m_pIPhone->FTD_AUButtonCount(m_nFtdPort, m_nFtdTimeOut, sz_ButtonTypePress);
		if (!b_res)
		{
			TraceLog(MSG_INFO, "runButtonCount fail in RunButtonTestProcess");
		}

		nCount = atoi(sz_ButtonTypePress);
		if (nCount > 0)
		{
			cs_Result.Format("%s %s", "ButtonCheck PASS in RunButtonTestProcess = ", sz_ButtonTypePress);
			TraceLog(MSG_INFO, cs_Result.GetString());
			break;
		}
		else
		{
			cs_Result.Format("%s %s", "ButtonCheck Fail in RunButtonTestProcess = ", sz_ButtonTypePress);
			TraceLog(MSG_INFO, cs_Result.GetString());
		}
		Sleep(1000);

		finish = clock();
		if (((finish - start) / CLK_TCK) > TimeOut)
		{
			b_StopkKeyTest = true;
			break;
		}
	}

	//stop key test
	if (b_StopkKeyTest)
	{
		m_strMessage = "Sapporo ButtonCheck time out!";
		TraceLog(MSG_INFO, "Sapporo ButtonCheck time out!");
		return false;
	}
	return nCount;
}

bool BostonButtonCheck::EndButtonTestDlg()
{
	pButtonTestDlg->OnDestroy();
	Sleep(1000);
	pButtonTestDlg->FreeResource();
	pButtonTestDlg->CDiagDestroyWindow();
	delete pButtonTest->pButtonTestDlg;
	pButtonTest->pButtonTestDlg = NULL;
	pButtonTest = NULL;
	KeyTestThreadHandle  = NULL ;
	return true;
}
