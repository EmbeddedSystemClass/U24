#include "stdafx.h"
#include "CheckAndChangeMode.h"


RF_IMPLEMENT_DYNCREATE(CCheckAndChangeMode)


bool CCheckAndChangeMode::Run()
{
	bool bRes = MainFunction();

	if (bRes)
	{
		m_strMsg = "Check and change phone mode PASS";
		FactoryLog(true, "CheckAndChangeMode", "--", "--", "--", "--", "--", "Pass", "--", m_strMsg.c_str());
	}
	else
	{
		m_strMsg = "Check and change phone mode Fail";
		FactoryLog(false, "--", m_strErrCode, "--", "--", "--", "--", "Fail", "--", m_strMsg.c_str());
	}
	
	return bRes;
}

bool CCheckAndChangeMode::MainFunction()
{	
	unsigned long iPhoneMode;
	bool b_done = false;
	//out 1 FTM,out 0 online
	for (int i=1; i< 10; i++)
	{
		if (!m_pIPhone->QMSL_PHONE_GET_MODE(&iPhoneMode))
		{
			m_strMsg = "Fail in attempt to Get phone mode with QMSL";
			TraceLog(MSG_ERROR, m_strMsg);
			Sleep(1000);
		}
		else
		{
			b_done = true;
			TraceLog(MSG_INFO, "Get phone mode success");
			break;
		}
	}

	if (b_done)
	{
		if(1 == m_iRequiredMode)
		{
			m_strErrCode = ERR_QLIB_SET_FTM;

			if (! m_pIPhone->QMSL_DIAG_CONTROL_F(MODE_ONLINE_F))
			{
				m_strMsg = "Fail to change to ONLINE mode";
				TraceLog(MSG_ERROR, m_strMsg);
				return false;
			}

			Sleep(2000);

			if (! m_pIPhone->QMSL_DIAG_CONTROL_F(MODE_FTM_F))
			{
				m_strMsg = "Fail to change to FTM mode";
				TraceLog(MSG_ERROR, m_strMsg);
				return false;
			}

			Sleep(3000);
		}
		else
		{
			m_strErrCode = ERR_QLIB_SET_ONLINE;

			if (! m_pIPhone->QMSL_DIAG_CONTROL_F(MODE_ONLINE_F))
			{
				m_strMsg = "Fail to change to ONLINE mode";
				TraceLog(MSG_ERROR, m_strMsg);
				return false;
			}
		}
	}
	else
	{
		m_strMsg = "Fail to get phone mode with QMSL finally";
		m_strErrCode = ERR_QLIB_ConfigureTimeOut;
		TraceLog(MSG_ERROR, m_strMsg);
	}
/*
	if (b_done)
	{
		int iFTMmode = (iPhoneMode == 1)? 1:0;

		//0=online, 1=FTM
		if (iFTMmode != m_iRequiredMode)
		{
			int iSetMode;
			if (m_iRequiredMode == 0)
			{
				iSetMode = MODE_ONLINE_F;
				m_strErrCode = ERR_QLIB_SET_ONLINE;
			}
			else
			{
				iSetMode = MODE_FTM_F;
				m_strErrCode = ERR_QLIB_SET_FTM;
			}

			b_done = false;
			for (int i=1; i< 10; i++)
			{
				if (! m_pIPhone->QMSL_ChangeFTM_Mode(iSetMode))
				{
					if (m_iRequiredMode == 0)
					{
						m_strMsg = "Fail in attempt to switch phone to Online with QMSL";
					}
					else
					{
						m_strMsg = "Fail in attempt to switch phone to FTM with QMSL";
					}

					TraceLog(MSG_ERROR, m_strMsg);
					Sleep(1000);
				}
				else
				{
					b_done = true;
					TraceLog(MSG_INFO, "Set phone mode success");
					break;
				}
			}
			if (!b_done)
			{
				if (m_iRequiredMode == 0)
				{
					m_strMsg = "Fail to switch phone to Online with QMSL finally";
				}
				else
				{
					m_strMsg = "Fail to switch phone to FTM with QMSL finally";
				}

				TraceLog(MSG_ERROR, m_strMsg);
				return false;
			}
		}	
	}
	else
	{
		m_strMsg = "Fail to get phone mode with QMSL finally";
		m_strErrCode = ERR_QLIB_ConfigureTimeOut;
		TraceLog(MSG_ERROR, m_strMsg);
	}
*/
	return b_done;
}




bool CCheckAndChangeMode::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find ("RequiredMode") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RequiredMode for CCheckAndChangeMode");
		return false;
	}
	m_iRequiredMode = atoi(paramMap["RequiredMode"].c_str());

	return true;
}
