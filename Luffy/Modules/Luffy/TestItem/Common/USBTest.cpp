#include "StdAfx.h"
#include "USBTest.h"
#include "FacTestToolInterface.h"

using namespace StringUtility;

GP_IMPLEMENT_DYNCREATE(CUsbTest)

CUsbTest::CUsbTest()
{
}

CUsbTest::~CUsbTest()
{
}

bool CUsbTest::InitData(std::map<std::string,std::string> &paramMap)
{
	//memset(&m_st_UIMessage, 0, sizeof(st_UIMessage));
	//std::string szString;
	//Get
	TraceLog("Start initdata for CUsbTest");

	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone =  dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	if (!m_pIPhone)
	{
		TraceLog("Fail to get address of phone instance for CUsbTest");
		return false;
	}
	
	m_pCGProc->SetEraseFlag(true);

	/*
	//Load XML   D:\source\C++\Luffy_ModifyCurrentSpec_AddCPKLog\Luffy\Solution\Debug\Qisda\GLUFB1A_BBTEST_TestItem.xml
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());
	m_szWorkingDir = myParm_node.child("WorkingDir").text().as_string();
	//m_szExeName = myParm_node.child("TESTEXE").text().as_string();
	m_szIdProductTest = myParm_node.child("idProductCMD").text().as_string();
	m_szIdVendorTest = myParm_node.child("idVendorCMD").text().as_string();

	if (m_szIdProductTest.empty() || m_szIdVendorTest.empty())
	{
		TraceLog("FAIL to initdata for CUsbTest");
		sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to initdata for CUsbTest");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		return false;
	}

	m_szIdProductSize = myParm_node.child("idProductSize").text().as_string();
	m_szIdVendorSize =  myParm_node.child("idVendorSize").text().as_string();
	if (m_szIdProductSize.empty() || m_szIdVendorSize.empty())
	{
		TraceLog("FAIL to initdata for CUsbTest");
		sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to initdata for CUsbTest");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		return false;
	}
	//m_szIicCmdTemp = myParm_node.child("IICCMD").text().as_string(); 

	sprintf_s(m_szIdProductCmdLine,512,m_szIdProductTest.c_str());
	sprintf_s(m_szIdVendorCmdLine,512,m_szIdVendorTest.c_str());

	TraceLog("PASS to initdata for CUsbTest");
	sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to initdata for CUsbTest");
	m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	TraceLog(m_szIdProductCmdLine);
	TraceLog(m_szIdVendorCmdLine);
	*/
	TraceLog("PASS to initdata for CUsbTest");
	m_pCGProc->SendMsgToUi("PASS to initdata for CUsbTest");
	return true;
}

bool CUsbTest::Run(int i_slot)
{
	return MainFunction();
}

bool CUsbTest::MainFunction()
{
	//Set buffer to get result of CMD
	//string Buf="";

	//sprintf_s(m_st_UIMessage.sz_message,4096,"CUsbTest is running......");
	//m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

	TraceLog("CUsbTest is running");
	m_pCGProc->SendMsgToUi("CUsbTest is running......");
	
	std::string szReturn = "";
	szReturn= m_pIPhone->CheckIdProduct();
	if("PASS" == szReturn)
	{	//TraceLog(szReturn.c_str());
		TraceLog("IdProduct pass");
		m_pCGProc->SendMsgToUi("IdProduct pass");
	}
	else
	{	m_pCGProc->SetFailMsg("IdProduct fail");
		TraceLog(szReturn.c_str());
		TraceLog("IdProduct fail");
		m_pCGProc->SendMsgToUi(szReturn.c_str());
		m_pCGProc->SendMsgToUi("IdProduct fail");
		return false;
	}

	szReturn = m_pIPhone->CheckIdVendor();
	if( "PASS" == szReturn )
	{	//TraceLog(szReturn.c_str());
		TraceLog("IdVendor pass");
		m_pCGProc->SendMsgToUi("IdVendor pass");
	}
	else
	{	m_pCGProc->SetFailMsg("IdVendor fail");
		TraceLog(szReturn.c_str());
		TraceLog("IdVendor fail");
		m_pCGProc->SendMsgToUi(szReturn.c_str());
		m_pCGProc->SendMsgToUi("IdVendor fail");
		return false;
	}
	/*
	//Set working dirszWorkingDir
	char szWorkingDir[512]={0};
	sprintf_s(szWorkingDir,512,m_szWorkingDir.c_str());
	//sprintf_s(szWorkingDir,512,m_pCGProc->g_mapToolInfo["WORKING_DIR"].c_str());

	ExecCmdUtil::ExecuteCmdBlocking("ADB.exe",m_szIdProductCmdLine,szWorkingDir, &Buf);
	if (!Buf.empty())
	{
		std::string::size_type i = 0;
		i = Buf.find(m_szIdProductSize);
		if (i == string::npos )
		{	//Fail
			TraceLog("FAIL to IdProduct of USBTest");
			sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to IdProduct of USBTest");
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
			m_pCGProc->SetFailMsg("USB IdProduct os fail");
			return false;
		}
		else
		{	//PASS
			TraceLog("PASS to IdProduct of USBTest");
			sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to IdProduct of USBTest");
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		}
	}
	
	Sleep(100);
	ExecCmdUtil::ExecuteCmdBlocking("ADB.exe",m_szIdVendorCmdLine,szWorkingDir, &Buf);
	if (!Buf.empty())
	{
		std::string::size_type i = 0;
		i = Buf.find(m_szIdVendorSize);
		if (i == string::npos )
		{	//Fail
			TraceLog("FAIL to IdVendor of USBTest");
			sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to IdVendor of USBTest");
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
			m_pCGProc->SetFailMsg("USB IdVendor os fail");
			return false;
		}
		else
		{	//PASS
			TraceLog("PASS to IdVendor of USBTest");
			sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to IdVendor of USBTest");
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		}
	}
	*/

	return true;
}