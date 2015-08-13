/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 *
 * Description: .h file of CAutoMMIBBHandsetButtonTest object.
 * FileName   : BostonKeyTest.cpp
 * Version    : $Revision:   1.0  $
 * Author     : LION.WU
 * Time       : 2013.02.26
 ********************************************************************************/

#include "stdafx.h"
#include "AutoMMIBBHandsetButtonTest.h"


RF_IMPLEMENT_DYNCREATE(CAutoMMIBBHandsetButtonTest)

//CAutoMMIBBHandsetButtonTest *pKeyTest;

CAutoMMIBBHandsetButtonTest::CAutoMMIBBHandsetButtonTest()
{
	b_StopkKeyTest = false;

}

CAutoMMIBBHandsetButtonTest::~CAutoMMIBBHandsetButtonTest()
{
}

bool CAutoMMIBBHandsetButtonTest::PreRun()
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

bool CAutoMMIBBHandsetButtonTest::Run()
{
	return HandsetKeyTest();
}

bool CAutoMMIBBHandsetButtonTest::PostRun()
{
	bool b_res = true;
	if (!m_pIPhone->ExactCloseDevice(m_nFtdPort, m_nFtdTimeOut))
	{
		m_strMessage = "Device Close Command fail! in AutoMMIBBHandsetButtonTest";
		TraceLog(MSG_ERROR, "Device Close Command fail!  in AutoMMIBBHandsetButtonTest");
		/* Set PICS Data */
		//SetPICSData(_T("TOUCH_PANEL_TEST"), "FAIL");
		b_res = false;
	}
	else
	{
		TraceLog(MSG_INFO, "Device Close Command Succeed. in AutoMMIBBHandsetButtonTest");
	}
	return b_res;
}

bool CAutoMMIBBHandsetButtonTest::InitData(std::map<std::string, std::string>& paramMap)
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
		TraceLog(MSG_ERROR, "Failed to Get TimeOut for class CAutoMMIBBHandsetButtonTest");
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
		TraceLog(MSG_INFO, "Get ButtonID fail. in CAutoMMIBBHandsetButtonTest");
		return false;
	}

	if (paramMap.find(std::string("Retry")) != paramMap.end())
	{
		strRetry = paramMap[std::string("Retry")];
		nRetry = CStr::StrToInt(strRetry);
	}
	else
	{
		TraceLog(MSG_ERROR, "Failed to Get Retry for class CAutoMMIBBHandsetButtonTest");
		return false;
	}

	if (paramMap.find(std::string("SleepJIG")) != paramMap.end())
	{
		m_str_SleepJIG = paramMap[std::string("SleepJIG")];
		m_i_sleep_for_Jig = atoi(m_str_SleepJIG.c_str());
	}
	else
	{
		TraceLog(MSG_INFO, "Get SleepJIG fail. in CAutoMMIBBHandsetButtonTest");
		return false;
	}

	if (paramMap.find(std::string("Check")) != paramMap.end())
	{
		m_str_Check = paramMap[std::string("Check")];
		m_i_sleep_for_Check = atoi(m_str_Check.c_str());
	}
	else
	{
		TraceLog(MSG_INFO, "Get Check fail. in CAutoMMIBBHandsetButtonTest");
		return false;
	}
	return true;
}

bool CAutoMMIBBHandsetButtonTest::HandsetKeyTest()
{
	std::string str_msg;
	CString cs_OnOff;
	CString csLoopTime;
	CString cs_Result;

	bool b_res_JIG = false;
	bool b_res ;
	bool b_Res_PressButton, b_Res_ReleaseButton;

	///////////////////////////////////////////////////////////////////////////////
	if (!AutoMMIBBSetComport()) return false;
	char sz_Number[8] = {0};
	sscanf(m_str_command.c_str(), "%c,%s", sz_Number, cs_OnOff);
	TraceLog(MSG_INFO, "JigGO ing.");
	///////////////////////////////////////////////////////////////////////////////

	for (int i = 0 ; i < nRetry ; i ++)
	{
		b_Res_PressButton  = false;
		b_Res_ReleaseButton  = false;
		b_res = false;

		/*set JIG Default*/
		TraceLog(MSG_INFO, "JigBACK ing.");
		b_res_JIG = JigBACK(sz_Number);/*always keepgoing*/

		/* read ReleaseButton */
		char sz_ButtonTypeRelease[FTD_BUF_SIZE] = {0};
		if (!m_pIPhone->FTD_AUButtonCheck(m_nFtdPort, m_nFtdTimeOut, sz_ButtonTypeRelease))
		{
			str_msg = "ButtonCheck_ReleaseButton from mobile fail";
			TraceLog(MSG_INFO, str_msg);
			return false;
		}

		/* Compare */
		m_strMeasured = sz_ButtonTypeRelease;
		if (CStr::StrToInt(m_strMeasured) == 255)
		{
			str_msg = "ButtonCheck_ReleaseButton pass = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
			b_Res_ReleaseButton = true;
		}
		else
		{
			csLoopTime.Format("[%d]ButtonTypeRelease Fail Value = [%s], Retrying", i , m_strMeasured.c_str());
			str_msg = csLoopTime.GetString();
			TraceLog(MSG_INFO, str_msg);
		}
		///////////////////////////////////////////////////////////////////////////////
		b_res_JIG = JigGO(sz_Number);/*always keepgoing*/
		Sleep(m_i_sleep_for_Jig);

		/* read PressButton */
		char sz_ButtonTypePress[FTD_BUF_SIZE] = {0};
		if (!m_pIPhone->FTD_AUButtonCheck(m_nFtdPort, m_nFtdTimeOut, sz_ButtonTypePress))
		{
			str_msg = "HandsetButtonTest from mobile fail";
			TraceLog(MSG_INFO, str_msg);
			return false;
		}

		/* Compare */
		m_strMeasured = sz_ButtonTypePress;
		if (CStr::StrToInt(m_strMeasured) == 0)
		{
			str_msg = "HandsetButtonTest pass = " + m_strMeasured;
			TraceLog(MSG_INFO, str_msg);
			b_Res_PressButton = true;
		}
		else
		{
			csLoopTime.Format("[%d] ButtonTypePress Fail Value = [%s], Retrying", i , m_strMeasured.c_str());
			str_msg = csLoopTime.GetString();
			TraceLog(MSG_INFO, str_msg);
		}
		TraceLog(MSG_INFO, "JigBACK ing.");
		b_res_JIG = JigBACK(sz_Number);/*always keepgoing*/

		if ((b_Res_PressButton == true) && (b_Res_ReleaseButton == true))
		{
			cs_Result.Format("%s (%s, %s)", "ButtonCheck PASS = ", sz_ButtonTypePress, sz_ButtonTypeRelease);
			str_msg = cs_Result;
			b_res = true;
			break; /*exit while loop*/
		}
	}/*end of for, retry */
	///////////////////////////////////////////////////////////////////////////////

	if (!m_i_sleep_for_Check) b_res = true;   // no check

	/* test result */
	if (b_res)
	{
		CString csResult;
		TraceLog(MSG_INFO, csResult.GetString());
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, csResult.GetString());
		SetPICSData("HandsetKeyTest", "PASS");
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "HandsetButtonTest fail!";
		TraceLog(MSG_INFO, "HandsetButtonTest  fail!");
		SetPICSData("HandsetKeyTest", "FAIL");
	}
	FactoryLog();
	return b_res;
}

bool CAutoMMIBBHandsetButtonTest::AutoMMIBBSetComport()
{
	if (m_obj_Communicate.SetCOMPort(m_str_comport.c_str()) != 0)
	{
		TraceLog(MSG_INFO, "m_obj_Communicate.SetCOMPort FAIL,  in AutoMMIBBHandsetButtonTest");
		return false;
	}
	return true;
}

bool CAutoMMIBBHandsetButtonTest::JigGO(char* sz_number)
{
	bool bRes = false ;
	int nRes = -1;/* PASS = 0 */
	CString csResult_JigGO;

	//nRes = m_obj_Communicate.SetCOMPort(m_str_comport.c_str());

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
		TraceLog(MSG_INFO, "JigGO OpenCOM PASS, in AutoMMIBBHandsetButtonTest");
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

	if (bRes)
	{
		char c_ReadBuffer[10] = {0};
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

bool CAutoMMIBBHandsetButtonTest::JigBACK(char* sz_number)
{
	bool bRes = false ;
	int nRes = -1;/* PASS = 0 */
	CString csResult_JigGO;

	nRes = m_obj_Communicate.SetCOMPort(m_str_comport.c_str());
	if (nRes == 0)
	{
		BYTE b_command[10] = {"out08off#"};//
		BYTE buff2[2] = {"0"};
		buff2[0] = (BYTE)(LPCTSTR)sz_number[0];
		b_command[4] = buff2[0];

		CString cs_log ;
		cs_log.Format("JigBACK = %s", b_command);
		TraceLog(MSG_INFO, cs_log.GetString());

		nRes = m_obj_Communicate.OpenCOM();
		if (nRes == 0)
		{
			TraceLog(MSG_INFO, "JigBACK OpenCOM PASS,  in AutoMMIBBHandsetButtonTest");
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
