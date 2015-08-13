#include "stdafx.h"
#include "DisconnectQMSL.h"


RF_IMPLEMENT_DYNCREATE(CDisconnectQMSL)


bool CDisconnectQMSL::Run()
{
	bool bRes = MainFunction();

	if (bRes)
	{
		m_strMsg = "Disconnect QMSL PASS";
		FactoryLog(true, "DisconnectQMSL", "--", "--", "--", "--", "--", "Pass", "--", m_strMsg.c_str());
		SetPICSData("DisconnectQMSL", "PASS");
	}
	else
	{
		m_strMsg = "Disconnect QMSL Fail";
		FactoryLog(false, "", ERR_QLIB_DisconnectServer, "--", "--", "--", "--", "Fail", "--", m_strMsg.c_str());
		SetPICSData("DisconnectQMSL", "FAIL");

	}

	return bRes;
}

bool CDisconnectQMSL::MainFunction()
{
	bool ret = m_pIPhone->Disconnect_QMSL();

	if (ret)
	{
		TraceLog(MSG_INFO, "Disconnect_QMSL success");
	}
	else
	{
		m_strErrCode = ERR_QLIB_DisconnectServer;
		m_strMsg = "Disconnect_QMSL fail";
		TraceLog(MSG_ERROR, "Fail to Disconnect_QMSL");
	}

	return ret;
}


bool CDisconnectQMSL::InitData(std::map<std::string, std::string>& paramMap)
{
	return true;
}
