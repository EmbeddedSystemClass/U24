#include "stdafx.h"
#include "SleepDelay.h"
//#include "FacTestToolInterface.h"

GP_IMPLEMENT_DYNCREATE(CSleepDelay)
														

CSleepDelay::CSleepDelay()
{
	m_bTestResult = false;
	m_pCGProc = NULL;

	m_iWaitTime = 0;
}

bool CSleepDelay::Run(int iItem)
{

	sprintf_s(m_st_UIMessage.sz_message,4096,"Sleep......");
	m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	
	if (m_blnShowMsgbox)
		MessageBox(NULL,TEXT("请按住治具ADFU键,再按Enter"),TEXT("确认进入ADFU模式"),MB_OK);
	else
		Sleep(m_iWaitTime);

	m_bTestResult = true;

	SetUIMessage("Sleep Delay PASS");

	SetPICSData(m_NickName, "PASS");

	sprintf_s(m_st_UIMessage.sz_message,4096,"Sleep PASS.");
	m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	return m_bTestResult;
}


bool CSleepDelay::InitData(std::map<std::string, std::string>& paramMap)
{
	memset(&m_st_UIMessage, 0, sizeof(st_UIMessage));
	//cast interface to a real object
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	
	//Get myParam_node
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());

	//Get parameter 
	m_iWaitTime = myParm_node.child("WaitTime").text().as_int(ERROR_INT);
	CHECK_PARM("WaitTime", m_iWaitTime , ERROR_INT);

	if (myParm_node.child("ShowMsgBox").text().as_bool(false) != false)
		m_blnShowMsgbox = myParm_node.child("ShowMsgBox").text().as_bool();
	else
		m_blnShowMsgbox = false;

	TraceLog("PASS to initdata for CSleepDelay");
	sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to initdata for CSleepDelay");
	m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

	return true;
}
