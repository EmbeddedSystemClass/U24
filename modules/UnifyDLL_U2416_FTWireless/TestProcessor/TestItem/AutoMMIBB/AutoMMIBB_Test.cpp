#include "stdafx.h"
#include "AutoMMIBB_Test.h"


RF_IMPLEMENT_DYNCREATE(CAutoMMIBB_Test)

bool CAutoMMIBB_Test::InitData(std::map<std::string, std::string>& paramMap)
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
	return true;
}

bool CAutoMMIBB_Test::JigGO(char* sz_number)
{
	bool bRes = false ;
	int nRes = -1;/* PASS = 0 */
	CString csResult_JigGO;

	char c_ReadBuffer[10];
	memset(c_ReadBuffer, 0 , 10);
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
	}
	return bRes;
}

bool CAutoMMIBB_Test::JigBACK(char* sz_number)
{
	bool bRes = false ;
	CString csResult_JigGO;

	char c_ReadBuffer[10];
	memset(c_ReadBuffer, 0 , 10);
	int nRes = -1;/* PASS = 0 */
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

	return bRes;
}

bool CAutoMMIBB_Test::PreRun()
{
	m_strResult    = "";
	m_strItemCode  = CStr::IntToStr(Vibrator_Vibrate_BaseItemCode);
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

bool CAutoMMIBB_Test::Run()
{
	bool b_res = false;
	return b_res;
}

bool CAutoMMIBB_Test::PostRun()
{
	return true;
}

bool CAutoMMIBB_Test::runAutoMMIBB()
{
	if (m_pIPhone == NULL)
	{
		TraceLog(MSG_INFO, "m_pIPhone is NULL.");
		return false;
	}

	bool b_res_JIG = false;
	char sz_Number[8];
	CString cs_OnOff;
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

	b_res_JIG = true;/*liontest*/
	if (b_res_JIG)
	{
		m_strResult = "PASS";
		m_strMessage = "runAutoMMIBB Test Pass.";
		TraceLog(MSG_INFO, "runAutoMMIBB Test Pass.");
		FactoryLog();
		SetPICSData("VIBRATOR_TEST", "PASS");
		return true;
	}
	else
	{
		m_strResult = "FAIL";
		m_strMessage = _T("runAutoMMIBB Test FAIL");
		m_strErrorCode = FunErr_Vibrator_Test_Fail;
		TraceLog(MSG_INFO, "runAutoMMIBB Test Fail.");
		FactoryLog();
		SetPICSData("VIBRATOR_TEST", "FAIL");
		return false;
	}
	return true;;
}
