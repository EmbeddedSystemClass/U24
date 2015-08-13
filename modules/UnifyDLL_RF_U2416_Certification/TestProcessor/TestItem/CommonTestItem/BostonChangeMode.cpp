#include "stdafx.h"
#include "BostonChangeMode.h"


RF_IMPLEMENT_DYNCREATE(CBostonChangeMode)


bool CBostonChangeMode::Run()
{
	bool bRes;
	// Start WiFi power
	/*lionAdded*/

	if (!(bRes = m_pIPhone->Initial()))
	{
		m_strMsg = "Fail to connect phone with Qisda module";
		TraceLog(MSG_ERROR, m_strMsg);
		FactoryLog(false, "WifiModuleOnOFF", FunErr_WLAN_Test_Init_Device_Fail, "", "", "", "", "", "", m_strMsg);
		return false;
	}

	Sleep(500);
	/*lionAdded*/


	//bool 
		bRes = MainFunction();

	if (bRes)
	{
		m_strMsg = "Check and change phone mode PASS";
		FactoryLog(true, "BostonChangeMode", "--", "--", "--", "--", "--", "Pass", "--", m_strMsg.c_str());
	}
	else
	{
		m_strMsg = "Check and change phone mode Fail";
		FactoryLog(false, "--", m_strErrCode, "--", "--", "--", "--", "Fail", "--", m_strMsg.c_str());
	}
	
	return bRes;
}

bool CBostonChangeMode::MainFunction()
{	
	unsigned long iPhoneMode;
	bool b_done = false;

			for (int i=1; i< 10; i++)
			{
				if (! m_pIPhone->QMSL_ChangeFTM_Mode(m_iRequiredMode))
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

	////out 1 FTM,out 0 online
	//for (int i=1; i< 10; i++)
	//{
	//	if (!m_pIPhone->QMSL_PHONE_GET_MODE(&iPhoneMode))
	//	{
	//		m_strMsg = "Fail in attempt to Get phone mode with QMSL";
	//		TraceLog(MSG_ERROR, m_strMsg);
	//		Sleep(1000);
	//	}
	//	else
	//	{
	//		b_done = true;
	//		TraceLog(MSG_INFO, "Get phone mode success");
	//		break;
	//	}
	//}

	//if (b_done)
	//{
	//	int iFTMmode = (iPhoneMode == 1)? 1:0;

	//	//0=online, 1=FTM

	//	if (iFTMmode != m_iRequiredMode)
	//	{
	//		int iSetMode;
	//		if (m_iRequiredMode == 0)
	//		{
	//			iSetMode = 4; //online
	//			m_strErrCode = ERR_QLIB_SET_ONLINE;
	//		}
	//		else
	//		{
	//			iSetMode = 3; //FTM
	//			m_strErrCode = ERR_QLIB_SET_FTM;
	//		}

	//		b_done = false;
	//		for (int i=1; i< 10; i++)
	//		{
	//			if (! m_pIPhone->QMSL_ChangeFTM_Mode(iSetMode))
	//			{
	//				if (m_iRequiredMode == 0)
	//				{
	//					m_strMsg = "Fail in attempt to switch phone to Online with QMSL";
	//				}
	//				else
	//				{
	//					m_strMsg = "Fail in attempt to switch phone to FTM with QMSL";
	//				}

	//				TraceLog(MSG_ERROR, m_strMsg);
	//				Sleep(1000);
	//			}
	//			else
	//			{
	//				b_done = true;
	//				TraceLog(MSG_INFO, "Set phone mode success");
	//				break;
	//			}
	//		}
	//		if (!b_done)
	//		{
	//			if (m_iRequiredMode == 0)
	//			{
	//				m_strMsg = "Fail to switch phone to Online with QMSL finally";
	//			}
	//			else
	//			{
	//				m_strMsg = "Fail to switch phone to FTM with QMSL finally";
	//			}

	//			TraceLog(MSG_ERROR, m_strMsg);
	//			return false;
	//		}
	//	}	
	//}
	//else
	//{
	//	m_strMsg = "Fail to get phone mode with QMSL finally";
	//	m_strErrCode = ERR_QLIB_ConfigureTimeOut;
	//	TraceLog(MSG_ERROR, m_strMsg);
	//}

	//return b_done;
}




bool CBostonChangeMode::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find ("RequiredMode") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter RequiredMode for CBostonChangeMode");
		return false;
	}
	m_iRequiredMode = atoi(paramMap["RequiredMode"].c_str());

	return true;
}
