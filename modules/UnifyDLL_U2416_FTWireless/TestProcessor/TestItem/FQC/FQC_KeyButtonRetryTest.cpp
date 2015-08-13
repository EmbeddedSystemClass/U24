/*********************************************************************************
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1   2014    Lion.Wu            Create for Detroit project MMI test
 ********************************************************************************/

#include "stdafx.h"
#include "FQC_KeyButtonRetryTest.h"


RF_IMPLEMENT_DYNCREATE(CFQC_KeyButtonRetryTest)

CFQC_KeyButtonRetryTest* pButtonTest;

bool CFQC_KeyButtonRetryTest::PreRun()
{
	m_strResult = "-";
	m_strItemCode = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMessage = "";
	m_strErrorCode = "-";
	return true;
}


bool CFQC_KeyButtonRetryTest::PostRun()
{
	return true;
}

bool CFQC_KeyButtonRetryTest::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find(std::string("XMLTestItem")) != paramMap.end())
	{
		m_str_TestItem = paramMap[std::string("XMLTestItem")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter XMLTestItem for CFQC_KeyButtonRetryTest");
		return false;
	}

	if (paramMap.find(std::string("XMLStation")) != paramMap.end())
	{
		m_str_XMLStation = paramMap[std::string("XMLStation")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter XMLStation for CFQC_KeyButtonRetryTest");
		return false;
	}

	
	std::string strTimeOut;
	if (paramMap.find(std::string("XMLTimeOut")) != paramMap.end())
	{
		strTimeOut = paramMap[std::string("XMLTimeOut")];
		TimeOut = CStr::StrToInt(strTimeOut);
	}
	else
	{
		TraceLog(MSG_ERROR, "Failed to Get XMLTimeOut for class CFQC_KeyButtonRetryTest");
		return false;
	}

	std::string str_MexRetryTime;
	if (paramMap.find(std::string("XMLRetryTime")) != paramMap.end())
	{
		str_MexRetryTime = paramMap[std::string("XMLRetryTime")];
		m_i_MexRetryTime = CStr::StrToInt(str_MexRetryTime);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter RetryTime for CFQC_KeyButtonRetryTest");
		return false;
	}


	return true;
}

bool CFQC_KeyButtonRetryTest::Run()
{
	bool passFail = false;
	CString cs_TestItem = m_str_TestItem.c_str();
	if (cs_TestItem.Compare("SideKeyTest") == 0)
	{
		m_strItemCode = CStr::IntToStr(KEYPAD_Test_BaseItemCode);
		passFail = runLoop_SideKeyTest();
	}
	//else if (cs_TestItem.Compare("HeadsetKeyTest") == 0)
	//{
	//	m_strItemCode = CStr::IntToStr(Audio_Headset_BaseItemCode);
	//	passFail = runLoop_HeadsetKeyTest();
	//}
	return passFail;
}

bool CFQC_KeyButtonRetryTest::runLoop_SideKeyTest()
{
	m_i_retriedTime = 0;
	CString cs_TestTime;
	char sz_pattern[FTD_BUF_SIZE] = {0};
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	do
	{
		b_res_msg_try = false;
		b_res_msg_retry = false;

		b_res_msg_try = getSideKey();

		if (b_res_msg_try)
		{
			m_strErrorCode = "-";
			str_msg = "Loop_SideKeyTest pass!";
			m_i_retriedTime = m_i_MexRetryTime + 1 ;
		}
		else
		{
			if (m_i_retriedTime < m_i_MexRetryTime)
			{
				b_res_msg_retry = showMsg(CW2A(L"please check USB cable\n 请确认USB线有接好?\n\n Retry?是否重测?\n "));
				if (!b_res_msg_retry) m_i_retriedTime = m_i_MexRetryTime + 1 ;
			}
			str_msg = "Loop_SideKeyTest fail!";
			m_i_retriedTime += 1;
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);
		FactoryLog();
	} while (m_i_retriedTime <= m_i_MexRetryTime);

	if (b_res_msg_try)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Loop_SideKeyTest PASS!");
		SetPICSData(_T("KEY_TEST"), "PASS");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "Loop_SideKeyTest FAIL!");
		SetPICSData("KEY_TEST", "FAIL");
	}
	FactoryLog();
	return b_res_msg_try;
}

bool CFQC_KeyButtonRetryTest::runLoop_HeadsetKeyTest()
{
	m_i_retriedTime = 0;
	CString cs_TestTime;
	char sz_pattern[FTD_BUF_SIZE] = {0};
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	do
	{
		b_res_msg_try = false;
		b_res_msg_retry = false;

		b_res_msg_try = getButtonCount();

		if (b_res_msg_try)
		{
			m_strErrorCode = "-";
			str_msg = "runLoop_HeadsetKeyTest pass!";
			m_i_retriedTime = m_i_MexRetryTime + 1 ;
		}
		else
		{
			if (m_i_retriedTime < m_i_MexRetryTime)
			{
				b_res_msg_retry = showMsg(CW2A(L"please check headset cable\n 请确认耳机有插到位?\n\n Retry?是否重测?\n "));
				if (!b_res_msg_retry) m_i_retriedTime = m_i_MexRetryTime + 1 ;
			}
			str_msg = "runLoop_HeadsetKeyTest fail!";
			m_i_retriedTime += 1;
		}
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, str_msg);
	} while (m_i_retriedTime <= m_i_MexRetryTime);

	if (b_res_msg_try)
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
		m_strErrorCode = FunErr_Headset_Send_Key_Fail;
		TraceLog(MSG_INFO, "Check ButtonCheck FAIL!");
		SetPICSData("Headset_Button_CHECK", "FAIL");
	}
	FactoryLog();
	return b_res_msg_try;
}

bool CFQC_KeyButtonRetryTest::getButtonCount()
{
	bool b_res;
	int n_ButtoncountResult = 0;
	CString cs_Result;
	b_res = false;

	/* initial ButtonCount */
	char sz_ButtonTypePress[FTD_BUF_SIZE] = {0};
	b_res = m_pIPhone->FTD_AUButtonCount(m_nFtdPort, m_nFtdTimeOut, sz_ButtonTypePress);
	if (!b_res)
	{
		str_msg = "getButtonCount initial fail";
		TraceLog(MSG_INFO, str_msg);
	}

	b_res = InitialBounceWindow(0) ;
	if (b_res)
	{
		n_ButtoncountResult = getButtonCountProcess();
	}
	else
	{
		str_msg = "InitialBounceWindow(0) fail";
		TraceLog(MSG_INFO, str_msg);
	}
	EndButtonTestDlg();

	if (n_ButtoncountResult == 0) b_res = false;

	cs_Result.Format("%s (%s, %d)", "ButtonCheck Value = ", sz_ButtonTypePress, n_ButtoncountResult);
	m_strMessage = cs_Result;
	FactoryLog();
	return b_res;
}

bool CFQC_KeyButtonRetryTest::getSideKey()
{
	bool b_Res_Cmd;
	bool b_SideKeyResult;

	b_Res_Cmd  = false;
	b_SideKeyResult = false;

	b_Res_Cmd = m_pIPhone->ExactOpenDevice(m_nFtdPort, m_nFtdTimeOut);
	if (b_Res_Cmd)
	{
		TraceLog(MSG_INFO, "getSideKey - Device Open Command Succeed.");
	}
	else
	{
		TraceLog(MSG_ERROR, "getSideKey - Device Open Command fail!");
		b_SideKeyResult = false;
	}

	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	b_Res_Cmd = m_pIPhone->FTD_KeyTest(m_nFtdPort, m_nFtdTimeOut, "1", sz_outBuffer);
	if (b_Res_Cmd)
	{
		TraceLog(MSG_INFO, "getSideKey - SideKey command Pass");
	}
	else
	{
		TraceLog(MSG_INFO, "getSideKey - SideKey command fail");
	}

	b_Res_Cmd = InitialBounceWindow(1) ;
	if (b_Res_Cmd)
	{
		b_SideKeyResult = getSideKeyProcess();
	}
	EndButtonTestDlg();

	TraceLog(MSG_INFO, "getSideKey - Start SideKey Stop Command !");
	b_Res_Cmd = m_pIPhone->FTD_KeyTest(m_nFtdPort, 1000, "0", sz_outBuffer);
	if (b_Res_Cmd)
	{
		TraceLog(MSG_INFO, "getSideKey - SideKey Stop Command Pass!");
	}
	else
	{
		TraceLog(MSG_INFO, "getSideKey - SideKey Stop Command fail!");
	}

	b_Res_Cmd = m_pIPhone->ExactCloseDevice(m_nFtdPort, m_nFtdTimeOut);
	if (b_Res_Cmd)
	{
		TraceLog(MSG_INFO, "getSideKey - Device Close Command Succeed.");
	}
	else
	{
		TraceLog(MSG_ERROR, "getSideKey - Device Close Command fail!");
		b_SideKeyResult = false;
	}
	return b_SideKeyResult;
}

bool CFQC_KeyButtonRetryTest::getSideKeyProcess()
{
	bool b_Stop = true;
	b_StopkKeyTest = false;
	clock_t start = clock(); // Count start time
	clock_t finish;
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	char command[4] = {7, 1, 0, 0};
	while (b_Stop)
	{
		if (m_pIPhone->ReceiveCommandData(m_nFtdPort, 5000, command, sz_outBuffer))
		{
			std::string str_Id = sz_outBuffer;
			TraceLog(MSG_INFO, sz_outBuffer);
			pButtonWindow->Check_SideKey_Position(sz_outBuffer);
			if (pButtonWindow->Check_SideKey_Result(m_strErrorCode))
			{
				b_Stop = false;
				break;
			}
		}

		finish = clock();
		if (((finish - start) / CLK_TCK) > TimeOut)
		{
			b_StopkKeyTest = true;
			b_Stop = false;
			break;
		}
	}
	m_strErrorCode = m_strErrorCode;
	//stop key test
	if (b_StopkKeyTest)
	{
		m_strMessage = "SideKey Test time out!";
		TraceLog(MSG_INFO, "SideKey Test time out!");
		return false;
	}
	return true;
}

int CFQC_KeyButtonRetryTest::getButtonCountProcess()
{
	bool b_res = false;
	b_StopkKeyTest = false;
	int nCount;
	CString cs_Result;
	clock_t start = clock(); // Count start time
	clock_t finish;
	char sz_ButtonTypePress[FTD_BUF_SIZE] = {0};
	while (1)
	{
		nCount = 0 ;
		b_res = m_pIPhone->FTD_AUButtonCount(m_nFtdPort, m_nFtdTimeOut, sz_ButtonTypePress);
		if (!b_res)
		{
			TraceLog(MSG_INFO, "getButtonCount fail in getButtonCountProcess");
		}

		nCount = atoi(sz_ButtonTypePress);
		if (nCount > 0)
		{
			cs_Result.Format("%s %s", "ButtonCheck PASS in getButtonCountProcess = ", sz_ButtonTypePress);
			TraceLog(MSG_INFO, cs_Result.GetString());
			break;
		}
		else
		{
			cs_Result.Format("%s %s", "ButtonCheck Fail in getButtonCountProcess = ", sz_ButtonTypePress);
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

UINT CFQC_KeyButtonRetryTest::AfxBounceWindowThread(LPVOID pParam)
{
	pButtonTest = (CFQC_KeyButtonRetryTest*)pParam;
	int nRet = pButtonTest->pButtonWindow->DoModal();
	if (nRet == -1)
	{
		pButtonTest->TraceLog(MSG_ERROR, "AfxBounceWindowThread Create Error");
	}
	return true;
}

bool CFQC_KeyButtonRetryTest::InitialBounceWindow(int nStyle)
{
	if ( m_str_XMLStation == "MMIBB")
	{
		pButtonWindow = new CFQC_KeyButtonRetryTestWindow(NULL,0);// 0 = mmibb, 1 = FQC
	}
	else
	{
		pButtonWindow = new CFQC_KeyButtonRetryTestWindow(NULL,1);// 0 = mmibb, 1 = FQC
	}
	int iPanelHPixel = 1100;
	int iPanelVPixel = 1900;
	int iBlockLenght;
	int iBlockWidth = 100;
	int iDisplayRate = 2;
	if (nStyle == 0) iBlockLenght = 300;  /*headset key*/
	else iBlockLenght = 100;/*sidekey*/
	if (pButtonWindow->Initial_Parameter(iPanelHPixel, iPanelVPixel, iBlockLenght, iBlockWidth, iDisplayRate, nStyle) != true)
	{
		m_strMessage = "InitialBounceWindow fail!";
		TraceLog(MSG_ERROR, "InitialBounceWindow fail!");
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "InitialBounceWindow Pass!");
	}

	KeyTestThreadHandle = AfxBeginThread(CFQC_KeyButtonRetryTest::AfxBounceWindowThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	Sleep(200);//for Thread Handle Initial Dialog
	if (KeyTestThreadHandle == NULL)
	{
		m_strMessage = "Create BounceWindowfail!";
		TraceLog(MSG_ERROR, "Create BounceWindow fail!");
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Create BounceWindow Pass");
	}
	return true;
}

bool CFQC_KeyButtonRetryTest::EndButtonTestDlg()
{
	pButtonWindow->OnDestroy();
	Sleep(1000);
	pButtonWindow->FreeResource();
	pButtonWindow->CDiagDestroyWindow();
	delete pButtonTest->pButtonWindow;
	pButtonTest->pButtonWindow = NULL;
	pButtonTest = NULL;
	KeyTestThreadHandle  = NULL ;
	TraceLog(MSG_INFO, "EndButtonTestDlg Pass");
	return true;
}

bool CFQC_KeyButtonRetryTest::showMsg(char* szMsg)
{
	/* Init dialog response flag to false */
	m_b_DialogResponse = false;
	m_b_waitFlag = false;

	/* Notify UI to show message dialog */
	ShowDialog(szMsg);

	/* Wait UI response(Max:10s) */
	int i_waitCount = 0;
	while (m_b_waitFlag != true)
	{
		if (i_waitCount == 50)
		{
			str_msg = "Wait UI response time out.";
			TraceLog(MSG_INFO, "Wait UI response time out.");
			break;
		}
		i_waitCount++;
		Sleep(200);
	}

	/* Get UI dialog operator response */
	if (m_b_DialogResponse == true)
	{
		str_msg = "Pressed PASS";
		TraceLog(MSG_INFO, str_msg);
	}
	else
	{
		str_msg = "Pressed FAIL";
		TraceLog(MSG_INFO, str_msg);
	}
	return m_b_DialogResponse;
}
