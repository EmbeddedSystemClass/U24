#include "stdafx.h"
#include "ChangeBootMode.h"


RF_IMPLEMENT_DYNCREATE(CChangeBootMode)


bool CChangeBootMode::Run()
{
	bool bRes = MainFunction();

	if (bRes)
	{
		m_strMsg = "Change phone boot mode PASS";
		FactoryLog(true, "ChangeBootMode", "--", "--", "--", "--", "--", "Pass", "--", m_strMsg.c_str());
	}
	else
	{
		m_strMsg = "Change phone boot mode Fail";
		FactoryLog(false, "--", m_strErrCode, "--", "--", "--", "--", "Fail", "--", m_strMsg.c_str());
	}
	
	return bRes;
}

bool CChangeBootMode::MainFunction()
{
	bool b_done = false;
	for (int i=1; i< 10; i++)
	{
		if (! m_pIPhone->QMSL_ChangeFTM_BootMode(m_iRequiredMode))
		{
			if (m_iRequiredMode == 0)
			{
				m_strMsg = "Fail to Set Boot Mode as Online with QMSL";
			}
			else
			{
				m_strMsg = "Fail to Set Boot Mode as FTM with QMSL";
			}

			TraceLog(MSG_ERROR, m_strMsg);
			Sleep(1000);
		}
		else
		{
			b_done = true;
			if (m_iRequiredMode == 0)
			{
				m_strMsg = "Success to set Boot Mode as Online with QMSL";
			}
			else
			{
				m_strMsg = "Success to set Boot Mode as FTM with QMSL";
			}
			TraceLog(MSG_INFO, m_strMsg);
			break;
		}
	}
	if (!b_done)
	{
		if (m_iRequiredMode == 0)
		{
			m_strMsg = "Fail to Boot Mode as Online with QMSL finally";
			m_strErrCode = ERR_QLIB_SET_ONLINE;
		}
		else
		{
			m_strMsg = "Fail to Boot Mode as FTM with QMSL finally";
			m_strErrCode = ERR_QLIB_SET_FTM;
		}

		TraceLog(MSG_ERROR, m_strMsg);
	}	

	return b_done;
}




bool CChangeBootMode::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find ("RequiredMode") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RequiredMode for CChangeBootMode");
		return false;
	}
	m_iRequiredMode = atoi(paramMap["RequiredMode"].c_str());

	return true;
}
