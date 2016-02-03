
/************************************************************/
#include "stdafx.h"
#include "AutoMMIBBFlashLED.h"


RF_IMPLEMENT_DYNCREATE(CAutoMMIBBFlashLED)

bool CAutoMMIBBFlashLED::InitData(std::map<std::string, std::string>& paramMap)
{
	d_writeBufferSize = 10;
	d_readBufferSize = 10;
	if (paramMap.find(std::string("Comport")) != paramMap.end())
	{
		m_str_comport = paramMap[std::string("Comport")];
	}
	else
	{
		TraceLog(MSG_INFO, "Get Comport fail.");
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

	if (paramMap.find(std::string("SleepJIG")) != paramMap.end())
	{
		m_str_SleepJIG = paramMap[std::string("SleepJIG")];
		m_i_sleep_for_Jig = atoi(m_str_SleepJIG.c_str());
	}
	else
	{
		TraceLog(MSG_INFO, "Get SleepJIG fail.");
		return false;
	}

	if (paramMap.find(std::string("Check")) != paramMap.end())
	{
		m_str_Check = paramMap[std::string("Check")];
		m_i_sleep_for_Check = atoi(m_str_Check.c_str());
	}
	else
	{
		TraceLog(MSG_INFO, "Get Check fail. in CAutoMMIBBFlashLED");
		return false;
	}
	return true;
}

bool CAutoMMIBBFlashLED::JigGO(char* sz_number)
{
	bool bRes = false ;
	int nRes = -1;/* PASS = 0 */
	CString csResult_JigGO;

	nRes = m_obj_Communicate.SetCOMPort(m_str_comport.c_str());
	if (nRes == 0)
	{
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
			TraceLog(MSG_INFO, "JigGO OpenCOM PASS");
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
		if (bRes)
		{
			memset(c_ReadBuffer, 0, 10);
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

		/*check return value = out08onOK*/
		if (bRes)
		{
			CString cs_ReadBuffer = c_ReadBuffer;
			cs_ReadBuffer.Trim();
			if (cs_ReadBuffer.Compare(_T("out08onOK")) == 0)
			{
				CString cs_Result(c_ReadBuffer);
				TraceLog(MSG_INFO, cs_Result.GetString());
				csResult_JigGO.Format(_T("check return value = out08onOK PASS : %s"), cs_ReadBuffer);
				TraceLog(MSG_INFO, csResult_JigGO.GetString());
				bRes = true;
			}
			else
			{
				csResult_JigGO.Format(_T("check return value = out08onOK FAIL : %d"), cs_ReadBuffer);
				TraceLog(MSG_INFO, csResult_JigGO.GetString());
				bRes = false;
			}
		}
		m_obj_Communicate.CloseCOM();
	}
	return bRes;
}

bool CAutoMMIBBFlashLED::JigBACK(char* sz_number)
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
			TraceLog(MSG_INFO, "JigBACK OpenCOM PASS");
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

bool CAutoMMIBBFlashLED::PreRun()
{
	m_strResult    = "";
	m_strItemCode  = CStr::IntToStr(Flash_LED_BaseItemCode);
	m_strErrorCode = "";
	m_strBand      = "-";
	m_strChannel   = "-";
	m_strUpper     = "";
	m_strLower     = "";
	m_strMeasured  = "-";
	m_strUnit      = "-";
	m_strMessage   = "";
	return true;
}

bool CAutoMMIBBFlashLED::Run()
{
	if (m_pIPhone == NULL)
		return false;

	bool b_res = false;
	b_res = AutoMMIBBFlashLEDTest();
	return b_res;
}

bool CAutoMMIBBFlashLED::PostRun()
{
	return true;
}

bool CAutoMMIBBFlashLED::AutoMMIBBFlashLEDTest()
{
	if (m_pIPhone == NULL)
	{
		TraceLog(MSG_INFO, "m_pIPhone is NULL.");
		return false;
	}

	CString cs_OnOff;
	bool b_res_Boston = true;
	bool b_res_JIG = false;
	bool b_res = true;

	/* FlashLED Start */
	char sz_patternCameraLLED[FTD_BUF_SIZE] = {0};
	strcpy(sz_patternCameraLLED, "1");
	char sz_outBuffer[FTD_BUF_SIZE] = {0};
	b_res = m_pIPhone->FTD_CAMFlashLED(m_nFtdPort, m_nFtdTimeOut, sz_patternCameraLLED, sz_outBuffer);
	if (b_res)
	{
		m_strMessage = "FlashLED ON Fail.";
		SetPICSData("VIBRATOR_TEST", "FAIL");
		TraceLog(MSG_INFO, "AutoMMIBBFlashLED FlashLED ON PASS.");
	}
	else
	{
		m_strResult = "FAIL";
		m_strErrorCode = FunErr_Flash_LED_Test_Fail;
		m_strMessage = "FlashLED ON Fail.";
		SetPICSData("VIBRATOR_TEST", "FAIL");
		TraceLog(MSG_INFO, "AutoMMIBBFlashLED FlashLED ON Fail.");
		FactoryLog();
		return false;
	}
	Sleep(m_i_sleep_for_Jig);

	char sz_Number[8] = {0};
	sscanf(m_str_command.c_str(), "%c,%s", sz_Number, cs_OnOff);
	if (cs_OnOff.Compare("on") == 0)
	{
		TraceLog(MSG_INFO, "JigGO ing.");
		b_res_JIG = JigGO(sz_Number);
	}
	else
	{
		TraceLog(MSG_INFO, "JigBACK ing.");
		b_res_JIG = JigBACK(sz_Number);
	}

	/* FlashLED Stop */
	strcpy(sz_patternCameraLLED, "0");
	b_res = m_pIPhone->FTD_CAMFlashLED(m_nFtdPort, m_nFtdTimeOut, sz_patternCameraLLED, sz_outBuffer);
	if (b_res)
	{
		TraceLog(MSG_INFO, "Detroit_CAMFlashLED FlashLED OFF PASS.");
	}
	else
	{
		SetPICSData("VIBRATOR_TEST", "FAIL");
		TraceLog(MSG_INFO, "AutoMMIBBFlashLED FlashLED OFF Fail.");
	}
	if (!m_i_sleep_for_Check) b_res_JIG = true;   // no check

	if (b_res_Boston && b_res_JIG)
	{
		m_strResult = "PASS";
		m_strMessage = "FlashLED Test Pass.";
		TraceLog(MSG_INFO, "FlashLED Test Pass.");
		FactoryLog();
		SetPICSData("VIBRATOR_TEST", "PASS");
		return true;
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = "FlashLED Test FAIL";
		m_strErrorCode = FunErr_Flash_LED_Test_Fail;
		TraceLog(MSG_INFO, "FlashLED Test Fail.");
		FactoryLog();
		SetPICSData("VIBRATOR_TEST", "FAIL");
		return false;
	}
	return true;;
}
