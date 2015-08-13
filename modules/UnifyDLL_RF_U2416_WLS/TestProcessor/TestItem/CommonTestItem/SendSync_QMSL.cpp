#include "StdAfx.h"
#include "SendSync_QMSL.h"

RF_IMPLEMENT_DYNCREATE(CSendSync_QMSL)

bool CSendSync_QMSL::Run()
{
	bool bRes = MainFunction();

	if (bRes)
	{
		FactoryLog(true, "", "", "", "", "", "", "", "", m_strMsg);
		SetPICSData(m_strPICSName, "PASS");
	}
	else
	{
		FactoryLog(false, "", m_strErrCode, "", "", "", "", "", "", m_strMsg);
		SetPICSData(m_strPICSName, "FAIL");
	}

	return bRes;
}

bool CSendSync_QMSL::MainFunction()
{
	if (!m_pIPhone->Initial_QMSL())
	{
		m_strMsg = "Fail to initial QMSL";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	if (!m_pIPhone->IsConnected_QMSL())
	{
		m_strMsg = "Fail to connect phone with QMSL";
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	bool bSuccess = true;
	short iResponseSize;
	BYTE uc_Response[100] = {0};
	BYTE* uc_Request = new BYTE[m_vec_strDIAGCommnad.size()];

	for (size_t i = 0; i < m_vec_strDIAGCommnad.size(); ++i)
	{
		uc_Request[i] = atoi(m_vec_strDIAGCommnad[i].c_str());
	}

	TraceLog(MSG_ERROR, MakeStr("m_vec_strDIAGCommnad.size:%d", m_vec_strDIAGCommnad.size()));
	m_strMsg.clear();
	if (!m_pIPhone->QMSL_SendSync(m_vec_strDIAGCommnad.size(), uc_Request, uc_Response, &iResponseSize))
	{
		m_strMsg = "Fail to Execute QMSL_SendSync.";
		m_strErrCode = CommErr_Backup_NV_Fail;
		TraceLog(MSG_ERROR, m_strMsg);
		return false;
	}

	for (size_t i = 0; i < m_vec_strResponse.size(); i++)
	{
		if (atoi(m_vec_strResponse[i].c_str()) != uc_Response[i])
		{
			bSuccess = false;
			break;
		}
	}

	m_strMsg = "";
	for (size_t i = 0; i < m_vec_strDIAGCommnad.size(); ++i)
	{
		m_strMsg += MakeStr("%x ", uc_Request[i]);
	}
	TraceLog(MSG_ERROR, m_strMsg + "request");

	m_strMsg = "";
	for (size_t i = 0; i < m_vec_strResponse.size(); ++i)
	{
		m_strMsg += MakeStr("%x ", uc_Response[i]);
	}

	if (!bSuccess)
	{
		m_strMsg += ", Fail to Send Sync.";
		m_strErrCode = CommErr_Backup_NV_Fail;
		TraceLog(MSG_ERROR, m_strMsg);
	}
	else
	{
		TraceLog(MSG_INFO, m_strMsg);
		m_strMsg = "Send Sync PASS.";
	}
	delete []uc_Request;
	return bSuccess;
}

bool CSendSync_QMSL::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find("DIAGCommand") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter DIAGCommand for CSendSync_QMSL");
		return false;
	}
	STDStringTokenizer(paramMap["DIAGCommand"], " ", m_vec_strDIAGCommnad);

	if (paramMap.find("Response") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter Response for CSendSync_QMSL");
		return false;
	}
	STDStringTokenizer(paramMap["Response"], " ", m_vec_strResponse);

	if (paramMap.find("PICSName") == paramMap.end())
	{
		TraceLog(MSG_ERROR, "Fail to find parameter PICSName for CSendSync_QMSL");
		return false;
	}
	m_strPICSName = paramMap["PICSName"];

	return true;
}
