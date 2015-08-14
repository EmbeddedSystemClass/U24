#include "StdAfx.h"
#include "ConnectPhoneAdfu.h"

using namespace StringUtility;

GP_IMPLEMENT_DYNCREATE(CConnectPhoneAdfu)

bool CConnectPhoneAdfu::Run(int iItem)
{
	return MainFunction();
}

bool CConnectPhoneAdfu::InitData(std::map<std::string ,std::string>& paramMap)
{
	memset(&m_st_UIMessage, 0, sizeof(st_UIMessage));
	//Get
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone =  dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	if (!m_pIPhone)
	{
		TraceLog("Fail to get address of phone instance for ConnectPhoneAdfu");
		return false;
	}
	m_pCGProc->SetEraseFlag(true);

	TraceLog("PASS to initdata for ConnectPhoneAdfu");
	sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to initdata for ConnectPhoneAdfu");
	m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	return true;
}

bool CConnectPhoneAdfu::MainFunction()
{
	bool bRef = m_pCGProc->AutoDetectDevice();
	if (bRef)
	{
		sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to Connect Phone by ADFU");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	}
	else
	{
		sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to Connect Phone by ADFU");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	}
	return bRef;
}


CConnectPhoneAdfu::CConnectPhoneAdfu(void)
{
}

CConnectPhoneAdfu::~CConnectPhoneAdfu(void)
{
}