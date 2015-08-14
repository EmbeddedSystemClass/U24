#include "StdAfx.h"
#include "EraseOs.h"
#include "PowerSupplyControl.h"
#include "FacTestToolInterface.h"

using namespace StringUtility;

GP_IMPLEMENT_DYNCREATE(CEraseOs)

bool CEraseOs::InitData(std::map<std::string,std::string> &paramMap)
{
	//memset(&m_st_UIMessage, 0, sizeof(st_UIMessage));
	//Get
	TraceLog("Start initdata for CEraseOs");
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone =  dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	if (!m_pIPhone)
	{
		TraceLog("Fail to get address of phone instance for CEraseOs");
		return false;
	}
	m_pCGProc->SetEraseFlag(false);

	/*
	//m_szImageDir={0};
	char szWorkingDir[512]={0};
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());
	m_szImageDir =  myParm_node.child("ImageDir").child_value();
	m_szWorkingDir = myParm_node.child("WorkingDir").text().as_string();
	m_szFwFile = myParm_node.child("fw").child_value();
	//m_szMiscFile = myParm_node.child("MISC").child_value();
	//m_szRecoveryFile = myParm_node.child("RECOVERY").child_value();
	//m_szSystemFile = myParm_node.child("SYSTEM").child_value();

	//sprintf_s(szCmdLine, 1024, "\"%s\" %s", szExePath, szCmdLineOrFileName);
	sprintf_s(m_szCmdLine,512,"-fw=\"%s%s\" -dev=3 -mode=1 -flag=2",m_szImageDir.c_str(),m_szFwFile.c_str());

	//m_szCmdLine={0};

	TraceLog("PASS to initdata for CEraseOs");
	sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to initdata for CEraseOs");
	m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	TraceLog(m_szCmdLine);
	*/
	TraceLog("PASS to initdata for CEraseOs");
	m_pCGProc->SendMsgToUi("PASS to initdata for CEraseOs");
	return true;
}

bool CEraseOs::Run(int iItem)
{

	return MainFunction();
}

bool CEraseOs::MainFunction(void)
{
	/*
	//Set buffer to get result of CMD
	//power off
	CPowerSupplyControl ControlPower;
	ControlPower.m_bPS_OnOff = false;
	ControlPower.Run();
	
	MessageBox(NULL,TEXT("请按住治具ADFU键,再按Enter"),TEXT("确认进入ADFU模式"),MB_OK);
	//power on
	ControlPower.m_bPS_OnOff = true;
	ControlPower.Run();

	string Buf="";

	sprintf_s(m_st_UIMessage.sz_message,4096,"Erasure is been running......");
	m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

	//Set working dirszWorkingDir
	char szWorkingDir[512]={0};
	//sprintf_s(szWorkingDir,512,"C:\\windows\\system32");
	sprintf_s(szWorkingDir,512,m_szWorkingDir.c_str());

	//char szCmdLine[512]={0};
	//sprintf_s(szCmdLine,512,"-fw=\"D:\\Luffy\\20140106_7039_image\\140106.fw\" device=3 flash_erase=1 MISC=\"IMG:D:\\Luffy\\20140106_7039_image\\misc.img\" RECOVERY=\"IMG:D:\\Luffy\\20140106_7039_image\\recovery.img\" SYSTEM=\"IMG:D:\\Luffy\\20140106_7039_image\\system.img\" BOOT_MSG=FMT DATA=FMT CACHE=FMT");
	//ExecCmdUtil::ExecuteCmdBlocking("ping.exe", "127.0.0.1", &szWorkingDir[0], &Buf);
	TraceLog(szWorkingDir);
	*/
	//PS Off
	m_pCGProc->SendMsgToUi("Luffy重启中");
	m_pCGProc->SendMsgToUi("Power supply off.");
	TraceLog("Power supply off.");
	m_pCGProc->m_pIPS->SetPowerSupplyOff();

	//waiting 1s
	Sleep(1000);
	
	//按ADFU键进入ADFU模式
	MessageBox(NULL,TEXT("请按住治具ADFU键,再按Enter"),TEXT("确认进入ADFU模式"),MB_OK);

	//PS ON
	m_pCGProc->SendMsgToUi("Power supply on.");
	TraceLog("Power supply on.");
	m_pCGProc->m_pIPS->SetPowerSupplyOn();

	//Waiting 6s
	Sleep(6000);

	//detect ADFU devices
	m_pCGProc->SendMsgToUi("请松开ADFU按键");
	Sleep(2000);
	m_pCGProc->SendMsgToUi("自动侦测ADFU设备中...");
	TraceLog("Auto detect ADFU device.");
	if(false == m_pCGProc->AutoDetectDevice())
	{	m_pCGProc->SendMsgToUi("侦测不到ADFU设备,请检查机台是否已连接或通电");
		TraceLog("Detect device fail.");
		return false;
	}

	if( false ==m_pIPhone->EraseFactoryOs() )
	{	TraceLog("FAIL to Erase OS");
		m_pCGProc->SendMsgToUi("Erase OS fail");
		return false;
	}
	else
	{	TraceLog("PASS to Erase OS");
		m_pCGProc->SendMsgToUi("Erase OS pass");
		return false;
	}

	/*
	ExecCmdUtil::ExecuteCmdBlocking("Command.exe",m_szCmdLine, szWorkingDir, &Buf);

	if (!Buf.empty())
	{	
		std::string::size_type i=Buf.rfind("Successful");
		std::string::size_type j=Buf.rfind("OnComplete");
		if (i == string::npos || j == string::npos )
		{
			TraceLog("FAIL to Erase OS");
			sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to Erase OS");
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
			return false;
		}
		else
		{
			TraceLog("PASS to Erase OS");
			sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to Erase OS");
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
			return true;
		}
	}
	else
	{
		TraceLog("FAIL to Download OS;ExecuteCMDBlocking ERROR");
		return false;
	}
	*/
}