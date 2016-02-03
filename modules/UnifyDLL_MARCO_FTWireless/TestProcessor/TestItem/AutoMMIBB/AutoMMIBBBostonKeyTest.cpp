/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CAutoMMIBBBostonKeyTest object.
 * FileName   : BostonKeyTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     : LION.WU
 * Time       : 2013.08.20
 ********************************************************************************/

#include "stdafx.h"
#include "AutoMMIBBBostonKeyTest.h"

RF_IMPLEMENT_DYNCREATE(CAutoMMIBBBostonKeyTest)

bool b_JIG_Thread;
bool b_Result_Thread;

CAutoMMIBBBostonKeyTest::CAutoMMIBBBostonKeyTest()
{
	b_StopkKeyTest = false;
}

CAutoMMIBBBostonKeyTest::~CAutoMMIBBBostonKeyTest()
{
}

bool CAutoMMIBBBostonKeyTest::PreRun()
{
	m_strResult = "-";
	m_strItemCode = CStr::IntToStr(KEYPAD_Test_BaseItemCode);
	m_strErrorCode = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "-";
	m_strUnit = "-";
	m_strMessage = "";

	return true;
}

bool CAutoMMIBBBostonKeyTest::Run()
{
	return RunAnalysisThread();
}

bool CAutoMMIBBBostonKeyTest::PostRun()
{
	bool b_res = true;
	if (!m_pIPhone->ExactCloseDevice(m_nFtdPort, m_nFtdTimeOut))
	{
		m_strMessage = "Device Close Command fail! in AutoMMIBBBostonKeyTest";
		TraceLog(MSG_ERROR, "Device Close Command fail!  in AutoMMIBBBostonKeyTest");
		b_res = false;
	}
	else
	{
		TraceLog(MSG_INFO, "Device Close Command Succeed. in AutoMMIBBBostonKeyTest");
	}
	return b_res;
}

bool CAutoMMIBBBostonKeyTest::InitData(std::map<std::string, std::string>& paramMap)
{
	std::string strTimeOut;
	std::string strRetry;
	d_writeBufferSize = 10;
	d_readBufferSize = 10;

	if (paramMap.find(std::string("TimeOut")) != paramMap.end())
	{
		strTimeOut = paramMap[std::string("TimeOut")];
		TimeOut = CStr::StrToInt(strTimeOut);
	}
	else
	{
		TraceLog(MSG_ERROR, "Failed to Get TimeOut for class CAutoMMIBBBostonKeyTest");
		return false;
	}

	if (paramMap.find(std::string("Comport")) != paramMap.end())
	{
		m_str_comport = paramMap[std::string("Comport")];
	}
	else
	{
		TraceLog(MSG_INFO, "Get Comport fail. in AutoMMIBBBostonKeyTest");
		return false;
	}

	if (paramMap.find(std::string("Command")) != paramMap.end())
	{
		m_str_command = paramMap[std::string("Command")];
	}
	else
	{
		TraceLog(MSG_INFO, "Get Command fail.");
		return false;
	}

	if (paramMap.find(std::string("ButtonID")) != paramMap.end())
	{
		m_str_buttonID = paramMap[std::string("ButtonID")];
	}
	else
	{
		TraceLog(MSG_INFO, "Get ButtonID fail. in AutoMMIBBBostonKeyTest");
		return false;
	}

	if (paramMap.find(std::string("Retry")) != paramMap.end())
	{
		strRetry = paramMap[std::string("Retry")];
		nRetry = CStr::StrToInt(strRetry);
	}
	else
	{
		TraceLog(MSG_ERROR, "Failed to Get Retry for class CAutoMMIBBBostonKeyTest");
		return false;
	}


	if (paramMap.find(std::string("SleepJIG")) != paramMap.end())
	{
		m_str_SleepJIG = paramMap[std::string("SleepJIG")];
		m_i_sleep_for_Jig = atoi(m_str_SleepJIG.c_str());
	}
	else
	{
		TraceLog(MSG_INFO, "Get SleepJIG fail. in AutoMMIBBBostonKeyTest");
		return false;
	}
	return true;
}

UINT CAutoMMIBBBostonKeyTest::JIG_Thread(LPVOID pVar)
{
	CAutoMMIBBBostonKeyTest* pAutoMMIBBBostonKeyTest = (CAutoMMIBBBostonKeyTest*)pVar;
	bool bRes_JIG = pAutoMMIBBBostonKeyTest->AutoMMIBBSetComport();
	b_JIG_Thread = true;
	CString cs_OnOff;
	int n_Count = 0 ;
	char sz_Number[8];
	while (b_JIG_Thread)
	{
		if (bRes_JIG)
		{
			memset(sz_Number, 0, sizeof(sz_Number));
			sscanf(pAutoMMIBBBostonKeyTest->m_str_command.c_str(), "%c,%s", sz_Number, cs_OnOff);

			BYTE b_command[10] = {"out08off#"};//
			BYTE buff2[2] = {"0"};
			buff2[0] = (BYTE)(LPCTSTR)sz_Number[0];
			b_command[4] = buff2[0];
			pAutoMMIBBBostonKeyTest->TraceLog(MSG_INFO, "[JIG_Thread] JigBACK ing.Set Default First");
			bRes_JIG = pAutoMMIBBBostonKeyTest->JigBACK(sz_Number);/*always keepgoing*/
		}

		if (bRes_JIG)
		{
			Sleep(200);
			pAutoMMIBBBostonKeyTest->TraceLog(MSG_INFO, "[JIG_Thread] JigGO ing.");
			bRes_JIG = pAutoMMIBBBostonKeyTest->JigGO(sz_Number);/*always keepgoing*/
			Sleep(200);
		}
		else
		{
			pAutoMMIBBBostonKeyTest->TraceLog(MSG_INFO, "[JIG_Thread] JigBACK Fail");
		}

		if (!bRes_JIG)
			pAutoMMIBBBostonKeyTest->TraceLog(MSG_INFO, "[JIG_Thread] JigGO Fail.");

		n_Count++;
		if (n_Count == (pAutoMMIBBBostonKeyTest->nRetry)) b_JIG_Thread = false;	//retry  3 times, times up
	}
	pAutoMMIBBBostonKeyTest->TraceLog(MSG_INFO, "[JIG_Thread] JigBACK ing.Exit & Set Default");
	bRes_JIG = pAutoMMIBBBostonKeyTest->JigBACK(sz_Number);
	return 0;
}

UINT CAutoMMIBBBostonKeyTest::KeyTest_Thread(LPVOID pVar)
{
	CAutoMMIBBBostonKeyTest* pAutoMMIBBBostonKeyTest = (CAutoMMIBBBostonKeyTest*)pVar;
	bool b_res = pAutoMMIBBBostonKeyTest->m_pIPhone->ExactOpenDevice(pAutoMMIBBBostonKeyTest->m_nFtdPort, pAutoMMIBBBostonKeyTest->m_nFtdTimeOut);
	if (b_res)
		pAutoMMIBBBostonKeyTest->TraceLog(MSG_ERROR, "[KeyTest_Thread] ExactOpenDevice...ok");
	else
		pAutoMMIBBBostonKeyTest->TraceLog(MSG_ERROR, "[KeyTest_Thread] ExactOpenDevice...fail");
	Sleep(200);

	char szOutput[FTD_BUF_SIZE] = {0};
	b_res = pAutoMMIBBBostonKeyTest->m_pIPhone->FTD_KeyTest(m_nFtdPort, m_nFtdTimeOut, "1", szOutput);
	if (b_res)
		pAutoMMIBBBostonKeyTest->TraceLog(MSG_ERROR, "[KeyTest_Thread] Detroit_KeyTest...ok");
	else
		pAutoMMIBBBostonKeyTest->TraceLog(MSG_ERROR, "[KeyTest_Thread] Detroit_KeyTest...fail");
	Sleep(200);

	clock_t start = clock();// Count start time
	clock_t finish;
	char command[4] = {7, 1, 0, 0};
	int nButtonID = 0;
	CString cs_sz_outBuffer;
	bool bReceiveData = true;
	bool bReceiveDataPass = false;
	int nXMLButtonID = atoi(pAutoMMIBBBostonKeyTest->m_str_buttonID.c_str());
	CString cs_Button;
	if (nXMLButtonID == 115)
	{
		m_strErrorCode = FunErr_VolumeUpKey_Test_Fail;
		cs_Button = _T("VolumeUpKey");
	}
	else if (nXMLButtonID == 114)
	{
		m_strErrorCode = FunErr_VolumeDownKey_Test_Fail;
		cs_Button = _T("VolumeDownKey");
	}
	else if (nXMLButtonID == 116)
	{
		m_strErrorCode = FunErr_PowerKey_Test_Fail;
		cs_Button = _T("PowerKey");
	}
	else
	{
		m_strErrorCode = FunErr_Sidekey_Test_Fail;
	}
	pAutoMMIBBBostonKeyTest->TraceLog(MSG_INFO, "[KeyTest_Thread] start RunKeyTestProcess");
	b_Result_Thread = false;
	while (bReceiveData)
	{
		if (pAutoMMIBBBostonKeyTest->m_pIPhone->ReceiveCommandData(pAutoMMIBBBostonKeyTest->m_nFtdPort, pAutoMMIBBBostonKeyTest->m_nFtdTimeOut, command, szOutput))
		{
			Sleep(200);
			std::string str_Id = szOutput;
			sscanf(szOutput, "%d", &nButtonID);
			//if (nButtonID == 116 || nButtonID == 115 || nButtonID == 114)
			if (nButtonID == nXMLButtonID)
			{
				cs_sz_outBuffer.Format(_T("%s PASS,ID =  %s"), cs_Button, szOutput);
				pAutoMMIBBBostonKeyTest->TraceLog(MSG_INFO, cs_sz_outBuffer.GetString());
				bReceiveData = false ;//exit
				bReceiveDataPass = true;// Pass
				b_Result_Thread = true;// Pass
				b_JIG_Thread = false;// stop JIG retry
			}
			else
			{
				b_Result_Thread = false;
				cs_sz_outBuffer.Format(_T("Button FAIL,ID =  %s"), szOutput);
				pAutoMMIBBBostonKeyTest->TraceLog(MSG_INFO, cs_sz_outBuffer.GetString());
			}
		}

		finish = clock();
		if (((finish - start) / CLK_TCK) > pAutoMMIBBBostonKeyTest->TimeOut)
		{
			pAutoMMIBBBostonKeyTest->b_StopkKeyTest = true;
			break;
		}
	}

	//stop key test
	if (pAutoMMIBBBostonKeyTest->b_StopkKeyTest)
	{
		m_strMessage = "[KeyTest_Thread] time out!";
		pAutoMMIBBBostonKeyTest->TraceLog(MSG_INFO, "[KeyTest_Thread] Key Test time out");
	}
	pAutoMMIBBBostonKeyTest->TraceLog(MSG_INFO, "[KeyTest_Thread] End RunKeyTestProcess");
	Sleep(200);

	b_res = pAutoMMIBBBostonKeyTest->m_pIPhone->ExactCloseDevice(pAutoMMIBBBostonKeyTest->m_nFtdPort, pAutoMMIBBBostonKeyTest->m_nFtdTimeOut);
	if (b_res)
		pAutoMMIBBBostonKeyTest->TraceLog(MSG_ERROR, "[KeyTest_Thread] ExactCloseDevice...ok");
	else
		pAutoMMIBBBostonKeyTest->TraceLog(MSG_ERROR, "[KeyTest_Thread] ExactCloseDevice...fail");
	Sleep(200);

	if (bReceiveDataPass)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = cs_sz_outBuffer.GetString();
		pAutoMMIBBBostonKeyTest->TraceLog(MSG_INFO, cs_sz_outBuffer.GetString());
		pAutoMMIBBBostonKeyTest->SetPICSData("SAPPORO_KEY_TEST", "PASS");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "SAPPORO_KEY_TEST  FAIL!";
		pAutoMMIBBBostonKeyTest->TraceLog(MSG_INFO, "SAPPORO_KEY_TEST fail!");
		pAutoMMIBBBostonKeyTest->SetPICSData("SAPPORO_KEY_TEST", "FAIL");
	}
	pAutoMMIBBBostonKeyTest->FactoryLog();
	return 0;
}

bool CAutoMMIBBBostonKeyTest::RunAnalysisThread()
{

	TraceLog(MSG_INFO, "[RunAnalysisThread] Start KeyTest_Thread Thread ");
	thKeyTest_Thread = AfxBeginThread(KeyTest_Thread , (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

	TraceLog(MSG_INFO, "[RunAnalysisThread] Start JIG_Thread Thread ");
	thJIG_Thread = AfxBeginThread(JIG_Thread , (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

	WaitForSingleObject(thKeyTest_Thread->m_hThread , 20000);
	WaitForSingleObject(thJIG_Thread->m_hThread , 20000);
	TraceLog(MSG_INFO, "[RunAnalysisThread] END Analysis Thread ");

	//thKeyTest_Thread->PostQuitMessage();
	DWORD dwExitCode;
	CString csExitCode;
	GetExitCodeThread(thKeyTest_Thread->m_hThread, &dwExitCode);
	csExitCode.Format("thKeyTest_Thread dwExitCode = %d", dwExitCode);
	GetExitCodeThread(thJIG_Thread->m_hThread, &dwExitCode);
	csExitCode.Format("JIG_Thread dwExitCode = %d", dwExitCode);

	if (!b_Result_Thread)
		return false; //test fail
	return true;
}


bool CAutoMMIBBBostonKeyTest::AutoMMIBBSetComport()
{
	if (m_obj_Communicate.SetCOMPort(m_str_comport.c_str()) != 0)
	{
		TraceLog(MSG_INFO, "m_obj_Communicate.SetCOMPort FAIL,  in AutoMMIBBBostonKeyTest");
		return false;
	}
	return true;
}

bool CAutoMMIBBBostonKeyTest::JigGO(char* sz_number)
{
	bool bRes = false ;
	int nRes = -1;/* PASS = 0 */
	CString csResult_JigGO;

	BYTE buff2[2] = {"0"};
	BYTE b_command[10] = {"out08on#"};//

	buff2[0] = (BYTE)(LPCTSTR)sz_number[0];/* char* to byte */
	b_command[4] = buff2[0];

	CString cs_log ;
	cs_log.Format("JigGO = %s", b_command);
	TraceLog(MSG_INFO, cs_log.GetString()); /*CString to Std::String*/

	nRes = m_obj_Communicate.OpenCOM();
	if (nRes == 0)
	{
		TraceLog(MSG_INFO, "JigGO OpenCOM PASS, in AutoMMIBBBostonKeyTest");
		bRes = true;
	}
	else
	{
		csResult_JigGO.Format("JigGO OpenCOM FAIL : %d", nRes);
		TraceLog(MSG_INFO, csResult_JigGO.GetString());
		bRes = false;
	}

	if (bRes)
	{
		//nRes = m_obj_Communicate.WriteCOM(b_command,d_writeBufferSize);
		if (m_obj_Communicate.WriteCOM(b_command, d_writeBufferSize) == d_writeBufferSize)
		{
			csResult_JigGO.Format("JigGO WriteCOM PASS : %d", nRes);
			TraceLog(MSG_INFO, csResult_JigGO.GetString());
			bRes = true;
		}
		else
		{
			csResult_JigGO.Format("JigGO WriteCOM FAIL : %d", nRes);
			TraceLog(MSG_INFO, csResult_JigGO.GetString());
			bRes = false;
		}
	}
	Sleep(m_i_sleep_for_Jig);

	char c_ReadBuffer[10];
	memset(c_ReadBuffer, 0 , 10);
	if (bRes)
	{
		nRes = m_obj_Communicate.ReadCOM(c_ReadBuffer, d_readBufferSize);
		if (nRes == 0)
		{
			CString cs_Result(c_ReadBuffer);
			TraceLog(MSG_INFO, cs_Result.GetString());
			csResult_JigGO.Format("JigGO ReadCOM PASS : %s", c_ReadBuffer);
			TraceLog(MSG_INFO, csResult_JigGO.GetString());
			bRes = true;
		}
		else
		{
			csResult_JigGO.Format("JigGO ReadCOM FAIL : %d", nRes);
			TraceLog(MSG_INFO, csResult_JigGO.GetString());
			bRes = false;
		}
	}

	m_obj_Communicate.CloseCOM();

	return bRes;
}

bool CAutoMMIBBBostonKeyTest::JigBACK(char* sz_number)
{
	bool bRes = false ;
	int nRes = -1;/* PASS = 0 */
	CString csResult_JigGO;


	nRes = m_obj_Communicate.SetCOMPort(m_str_comport.c_str());
	if (nRes == 0)
	{
		BYTE b_command[10] = {"out08off#"};
		BYTE buff2[2] = {"0"};
		buff2[0] = (BYTE)(LPCTSTR)sz_number[0];
		b_command[4] = buff2[0];

		CString cs_log ;
		cs_log.Format("JigBACK = %s", b_command);
		TraceLog(MSG_INFO, cs_log.GetString());

		nRes = m_obj_Communicate.OpenCOM();
		if (nRes == 0)
		{
			TraceLog(MSG_INFO, "JigBACK OpenCOM PASS,  in AutoMMIBBBostonKeyTest");
			bRes = true;
		}
		else
		{
			csResult_JigGO.Format("JigBACK OpenCOM FAIL : %d", nRes);
			TraceLog(MSG_INFO, csResult_JigGO.GetString());
			bRes = false;
		}

		if (bRes)
		{
			//nRes = m_obj_Communicate.WriteCOM(b_command,d_writeBufferSize);
			if (m_obj_Communicate.WriteCOM(b_command, d_writeBufferSize) == d_writeBufferSize)
			{
				csResult_JigGO.Format("JigBACK WriteCOM PASS : %d", nRes);
				TraceLog(MSG_INFO, csResult_JigGO.GetString());
				bRes = true;
			}
			else
			{
				csResult_JigGO.Format("JigBACK WriteCOM FAIL : %d", nRes);
				TraceLog(MSG_INFO, csResult_JigGO.GetString());
				bRes = false;
			}
		}
		Sleep(m_i_sleep_for_Jig);

		if (bRes)
		{
			char c_ReadBuffer[10] = {0};
			nRes = m_obj_Communicate.ReadCOM(c_ReadBuffer, d_readBufferSize);
			if (nRes == 0)
			{
				csResult_JigGO.Format("JigBACK ReadCOM PASS : %s", c_ReadBuffer);
				TraceLog(MSG_INFO, csResult_JigGO.GetString());
				bRes = true;
			}
			else
			{
				csResult_JigGO.Format("JigBACK ReadCOM FAIL : %d", nRes);
				TraceLog(MSG_INFO, csResult_JigGO.GetString());
				bRes = false;
			}
		}
		m_obj_Communicate.CloseCOM();
	}
	//Sleep(m_i_sleep_for_Jig);
	return bRes;
}
