#include "stdafx.h"
#include "AudioTest.h"
#include "FacTestToolInterface.h"

using namespace StringUtility;

GP_IMPLEMENT_DYNCREATE(CAudioTest)

CAudioTest::CAudioTest(void)
{
}

CAudioTest::~CAudioTest(void)
{
}

bool CAudioTest::InitData(std::map<std::string,std::string> &paramMap)
{
	//memset(&m_st_UIMessage, 0, sizeof(st_UIMessage));
	//std::string szString;

	TraceLog("Start initdata for CAudioTest");
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone =  dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	if (!m_pIPhone)
	{
		TraceLog("Fail to get address of phone instance for CAudioTest");
		return false;
	}
	
	m_pCGProc->SetEraseFlag(true);
	/*
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());
	m_szWorkingDir = myParm_node.child("WorkingDir").text().as_string(ERROR_STRING);
	if (ERROR_STRING == m_szWorkingDir)
	{	TraceLog("Fail to load WorkingDir");
		sprintf_s(m_st_UIMessage.sz_message,4096,"Fail to load WorkingDir");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		return false;
	}
	TraceLog("WorkingDir = " + m_szWorkingDir);

	m_szCommand = myParm_node.child("ITEMCMD").text().as_string(ERROR_STRING);
	if (ERROR_STRING == m_szCommand)
	{	TraceLog("Fail to load ITEMCMD");
		sprintf_s(m_st_UIMessage.sz_message,4096,"Fail to load ITEMCMD");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		return false;
	}
	TraceLog("ITEMCMD = " + m_szCommand);

	TraceLog("PASS to initdata for CAudioTest");
	sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to initdata for CAudioTest");
	m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	*/
	TraceLog("PASS to initdata for CAudioTest");
	m_pCGProc->SendMsgToUi("PASS to initdata for CAudioTest");
	return true;
}

bool CAudioTest::Run(int i_slot)
{
	//sprintf_s(m_st_UIMessage.sz_message,4096,"CAudioTest is running......");
	//m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	TraceLog("CAudioTest is running");
	m_pCGProc->SendMsgToUi("CAudioTest is running......");
	//m_pIPhone->SetWorkingDir(m_szWorkingDir.c_str());
	return MainFunction();
}

bool CAudioTest::MainFunction(void)
{
	//m_pIPhone->GetRunInResult();
	//m_pIPhone->pullRunInLog();
	//m_pIPhone->PlayMusic();
	m_pIPhone->PlayMovie();
	Sleep(1000);
	int i = MessageBox(NULL,TEXT("若刚才听到一段音乐,请选择是; 否则选否"),TEXT("确认是否听到音乐"),MB_YESNO);
	if (i == IDYES)
	{	//m_strMsg = MakeSTDstring("CAudioTest PASS");
		//sprintf_s(m_st_UIMessage.sz_message,4096,m_strMsg.c_str());
		//m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		TraceLog("HDMI pass");
		m_pCGProc->SendMsgToUi("AudioTest pass");
		return true;
	}
	else
	{	//m_strMsg = MakeSTDstring("CAudioTest Fail");
		//sprintf_s(m_st_UIMessage.sz_message,4096,m_strMsg.c_str());
		//m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		m_pCGProc->SetFailMsg("HDMI fail");
		TraceLog("HDMI fail");
		m_pCGProc->SendMsgToUi("HDMI fail");
		return false;
	}

}