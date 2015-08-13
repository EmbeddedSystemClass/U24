/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CDublin2KeyTest object.
 * FileName   : BostonKeyTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     : LION.WU
 * Time       : 2013.02.26
 ********************************************************************************/

#include "stdafx.h"
#include "SideKeyTest.h"


RF_IMPLEMENT_DYNCREATE(CDublin2KeyTest)

CDublin2KeyTest *pKeyTest;

CDublin2KeyTest::CDublin2KeyTest()
{
	b_StopkKeyTest = false;
	pKeyTest = NULL;
	pKeyTestDlg = NULL;
	KeyTestThreadHandle = NULL;
}

CDublin2KeyTest::~CDublin2KeyTest()
{
}

bool CDublin2KeyTest::PreRun()
{
	m_strResult = "-";
	m_strItemCode = CStr::IntToStr(KEYPAD_Test_BaseItemCode);
	m_strErrorCode = FunErr_Keypad_Test_Fail;
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMessage = "";

	return true;
}

bool CDublin2KeyTest::Run()
{
	return KeyTest();
}

bool CDublin2KeyTest::PostRun()
{
	bool b_res = true;
	if(!m_pIPhone->ExactCloseDevice(m_nFtdPort, m_nFtdTimeOut))
	{
		m_strMessage = "Device Close Command fail!";
		TraceLog(MSG_ERROR, "Device Close Command fail!");
		b_res = false;
	}
	else
	{
		TraceLog(MSG_INFO, "Device Close Command Succeed.");
	}
	return b_res;
}

bool CDublin2KeyTest::InitData(std::map<std::string, std::string>& paramMap)
{
	std::string strTimeOut;
	if (paramMap.find(std::string("TimeOut")) != paramMap.end())
	{
		strTimeOut = paramMap[std::string("TimeOut")];
		TimeOut = CStr::StrToInt(strTimeOut);
	}
	else
	{
		TraceLog(MSG_ERROR, "Failed to Get TimeOut for class CDublin2KeyTest");
		return false;
	}

	return true;
}

bool CDublin2KeyTest::KeyTest()
{
	bool b_res = true;

	/* Init dialog response flag to false */
	m_b_DialogResponse = false;
	m_b_waitFlag = false;

	/* Notify UI to show message dialog */
	std::string str_msg = CW2A(L"Please press key to start test.\n请依照号码顺序按按键测试");
	ShowDialog(str_msg.c_str());

	/* Wait UI response(Max:10s) */
	int i_waitCount = 0;
	while (m_b_waitFlag != true)
	{
		if (i_waitCount == 50)
		{
			str_msg = "Wait UI response time out.";
			TraceLog(MSG_INFO, "Wait UI response time out.");
			b_res = false;
			break;
		}
		i_waitCount++;
		Sleep(200);
	}

	if (b_res)
	{
		/* Get UI dialog operator response */
		if (m_b_DialogResponse == true)
		{
			str_msg = "Start key test.";
			TraceLog(MSG_INFO, str_msg);
		}
		else
		{
			b_res = false;
			str_msg = "Stop key test.";
			TraceLog(MSG_INFO, str_msg);
		}
	}

	if (!(b_res = m_pIPhone->ExactOpenDevice(m_nFtdPort, m_nFtdTimeOut)))
	{
		m_strMessage = "Device Open Command fail!";
		TraceLog(MSG_ERROR, "Device Open Command fail!");
		b_res = false;
	}
	else
	{
		TraceLog(MSG_INFO, "Device Open Command Succeed.");
	}

	if (b_res)
	{
		char sz_outBuffer[FTD_BUF_SIZE] = {0};
		if (!(b_res = m_pIPhone->FTD_KeyTest(m_nFtdPort, m_nFtdTimeOut, "1", sz_outBuffer)))
		{
			b_res = false;
			TraceLog(MSG_INFO, "SideKey test command fail");
		}
		else
		{
			TraceLog(MSG_INFO, "SideKey test command success");
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	b_res = InitialKeyTestDlg();
	if (b_res)
	{
		b_res = RunKeyTestProcess();
	}
	EndKeyTestDlg();
	///////////////////////////////////////////////////////////////////////////////

	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	if (!m_pIPhone->FTD_KeyTest(m_nFtdPort, 1000, "0", sz_outBuffer))
	{
		TraceLog(MSG_ERROR, "SideKey Test Stop Command fail!");
	}
	else
	{
		TraceLog(MSG_INFO, "SideKey Test Stop Command Succeed.");
	}

	if(!m_pIPhone->ExactCloseDevice(m_nFtdPort, m_nFtdTimeOut))
	{
		m_strMessage = "Device Close Command fail!";
		TraceLog(MSG_ERROR, "Device Close Command fail!");
		b_res = false;
	}
	else
	{
		TraceLog(MSG_INFO, "Device Close Command Succeed.");
	}

	/* test result */
	if(b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = "SideKey test pass!";
		TraceLog(MSG_INFO, m_strMessage);
		SetPICSData("KEY_TEST", "PASS");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "SideKey test fail!";
		TraceLog(MSG_INFO, "Key test fail!");
		SetPICSData("KEY_TEST", "FAIL");
	}
	FactoryLog();
	Sleep(1000);//fixed UI can't show Pass
	return b_res;
}

bool CDublin2KeyTest::InitialKeyTestDlg()
{
	pKeyTestDlg = new CDublin2KeyTestDlg; //2KEY,  volumn up, volumn down
	int iPanelHPixel = 1100;
	int iPanelVPixel = 1900;
	int iBlockLenght = 100;
	int iBlockWidth = 100;
	int iDisplayRate = 2;
	if(pKeyTestDlg->Initial_Parameter(iPanelHPixel, iPanelVPixel, iBlockLenght, iBlockWidth, iDisplayRate) != true)
	{
		m_strMessage = "Initial key test dialog fail!";
		TraceLog(MSG_ERROR, "Initial key test dialog fail!");
		return false;
	}
	else
	{
		TraceLog(MSG_INFO, "Initial key test dialog success!");
	}

	KeyTestThreadHandle = AfxBeginThread(CDublin2KeyTest::AfxKeyTestDlgThread,this,THREAD_PRIORITY_NORMAL,0,0,NULL);
	Sleep(200);//for Thread Handle Initial Dialog
	if(KeyTestThreadHandle == NULL)
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

bool CDublin2KeyTest::RunKeyTestProcess()
{
	clock_t start = clock(); // Count start time
	clock_t finish;
	char command[4] = {7,1,0,0};
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	while (1)
	{
		if (m_pIPhone->ReceiveCommandData(m_nFtdPort, 5000, command, sz_outBuffer) == true)
		{
			std::string str_Id = sz_outBuffer;
			TraceLog(MSG_INFO, sz_outBuffer);
			pKeyTestDlg->Check_Key_Position(sz_outBuffer);
			if (pKeyTestDlg->Check_Key_Result(m_strErrorCode))
				break;
		}

		finish = clock();
		if(((finish-start)/CLK_TCK) > TimeOut)
		{
			b_StopkKeyTest = true;
			break;
		}
	}

	//stop key test
	if (b_StopkKeyTest)
	{
		m_strMessage = "SideKey Test time out!";
		TraceLog(MSG_INFO, "SideKey Test time out!");
		return false;
	}
	return true;
}

UINT CDublin2KeyTest::AfxKeyTestDlgThread(LPVOID pParam)
{
	pKeyTest = (CDublin2KeyTest *)pParam;
	pKeyTest->pKeyTestDlg->DoModal();
	return true;
}

bool CDublin2KeyTest::EndKeyTestDlg()
{
	pKeyTestDlg->OnDestroy();
	Sleep(1000);
	pKeyTest->pKeyTestDlg->FreeResource();
	delete pKeyTest->pKeyTestDlg;
	pKeyTest->pKeyTestDlg = NULL;
	return true;
}
