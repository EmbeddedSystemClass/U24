#include "StdAfx.h"
#include "DLFactoryOs.h"
#include "FacTestToolInterface.h"
using namespace StringUtility;

GP_IMPLEMENT_DYNCREATE(CDlFactoryOs)

bool CDlFactoryOs::Run (int iItem)
{
	//PS Off
	m_pCGProc->SendMsgToUi("Luffy重启中");
	m_pCGProc->SendMsgToUi("Power supply off.");
	TraceLog("Power supply off.");
	m_pCGProc->m_pIPS->SetPowerSupplyOff();

	//waiting 1s
	Sleep(1000);

	//PS ON
	m_pCGProc->SendMsgToUi("Power supply on.");
	TraceLog("Power supply on.");
	m_pCGProc->m_pIPS->SetPowerSupplyOn();

	//Waiting 5s
	Sleep(5000);

	//detect ADFU devices
	m_pCGProc->SendMsgToUi("自动侦测ADFU设备中...");
	TraceLog("Auto detect ADFU device.");
	if(false == m_pCGProc->AutoDetectDevice())
	{	m_pCGProc->SendMsgToUi("侦测不到ADFU设备,请检查机台是否已连接或通电");
		TraceLog("Detect device fail.");
		m_pCGProc->SetFailMsg("侦测不到ADFU设备");
		return false;
	}
	bool blnTemp=MainFunction();
	return blnTemp;
}

bool CDlFactoryOs::InitData(std::map<std::string ,std::string>& paramMap)
{
	//memset(&m_st_UIMessage, 0, sizeof(st_UIMessage));
	//Get
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone =  dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	if (!m_pIPhone)
	{
		TraceLog("Fail to get address of phone instance for CDlFactoryOs");
		return false;
	}
	m_pCGProc->SetEraseFlag(false);
	////m_szImageDir={0};
	//char szWorkingDir[512]={0};
	//pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());
	//m_szImageDir =  myParm_node.child("ImageDir").child_value();
	//m_szWorkingDir = myParm_node.child("WorkingDir").text().as_string();
	//m_szFwFile = myParm_node.child("fw").child_value();
	//m_szMiscFile = myParm_node.child("MISC").child_value();
	//m_szRecoveryFile = myParm_node.child("RECOVERY").child_value();
	//m_szSystemFile = myParm_node.child("SYSTEM").child_value();

	//std::string strTmp = myParm_node.child("Erase").text().as_string(ERROR_STRING);
	//CHECK_PARM("Erase", strTmp , ERROR_STRING);
	//if ((strTmp == "TRUE") || (strTmp == "true"))// || (strTmp != "0"))
	//	m_blnErase = true;
	//else
	//	m_blnErase = false;

	////sprintf_s(szCmdLine, 1024, "\"%s\" %s", szExePath, szCmdLineOrFileName);
	//if (m_blnErase)
	//	sprintf_s(m_szCmdLine,512,"-fw=\"%s%s\" -dev=3 -mode=1 -flag=2",m_szImageDir.c_str(),m_szFwFile.c_str());
	//else
	//	sprintf_s(m_szCmdLine,512,"-fw=\"%s%s\" device=3 flash_erase=1 MISC=\"%s%s\" RECOVERY=\"%s%s\" SYSTEM=\"%s%s\" BOOT_MSG=FMT DATA=FMT CACHE=FMT",m_szImageDir.c_str(),m_szFwFile.c_str(),m_szImageDir.c_str(),m_szMiscFile.c_str(),m_szImageDir.c_str(),m_szRecoveryFile.c_str(),m_szImageDir.c_str(),m_szSystemFile.c_str());

	//m_szCmdLine={0};

	TraceLog("PASS to initdata for CDlFactoryOs");
	m_pCGProc->SendMsgToUi("CDlFactoryOs初始化完成");
	//sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to initdata for CDlFactoryOs");
	//m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	//TraceLog(m_szCmdLine);
	return true;
}
bool CDlFactoryOs::MainFunction()
{
	//Set buffer to get result of CMD
	//string Buf="";

	//sprintf_s(m_st_UIMessage.sz_message,4096,"Download is running......");
	//m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	
	TraceLog("Download is running......");
	m_pCGProc->SendMsgToUi("正在下载工厂版系统,请稍等...");
	if( true == m_pIPhone->LoadFactoryOs())
	{	TraceLog("Complete download factory image.");
		m_pCGProc->SendMsgToUi("下载工厂版系统完成");
		return true;
	}
	else
	{	TraceLog("Download factory image fail.");
		m_pCGProc->SendMsgToUi("下载工厂版系统失败");
		m_pCGProc->SetFailMsg("下载OS失败");
		return false;
	}

	////Set working dirszWorkingDir
	//char szWorkingDir[512]={0};
	////sprintf_s(szWorkingDir,512,"C:\\windows\\system32");
	//sprintf_s(szWorkingDir,512,m_szWorkingDir.c_str());

	////char szCmdLine[512]={0};
	////sprintf_s(szCmdLine,512,"-fw=\"D:\\Luffy\\20140106_7039_image\\140106.fw\" device=3 flash_erase=1 MISC=\"IMG:D:\\Luffy\\20140106_7039_image\\misc.img\" RECOVERY=\"IMG:D:\\Luffy\\20140106_7039_image\\recovery.img\" SYSTEM=\"IMG:D:\\Luffy\\20140106_7039_image\\system.img\" BOOT_MSG=FMT DATA=FMT CACHE=FMT");
	////ExecCmdUtil::ExecuteCmdBlocking("ping.exe", "127.0.0.1", &szWorkingDir[0], &Buf);
	//TraceLog(szWorkingDir);
	//ExecCmdUtil::ExecuteCmdBlocking("Command.exe",m_szCmdLine, szWorkingDir, &Buf);

	//if (!Buf.empty())
	//{	
	//	std::string::size_type i=Buf.rfind("Successful");
	//	std::string::size_type j=Buf.rfind("OnComplete");
	//	if (i == string::npos || j == string::npos )
	//	{
	//		TraceLog("FAIL to Download OS");
	//		sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to Download OS");
	//		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	//		return false;
	//	}
	//	else
	//	{
	//		TraceLog("PASS to Download OS");
	//		sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to Download OS");
	//		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	//		return true;
	//	}
	//}
	//else
	//{
	//	TraceLog("FAIL to Download OS;ExecuteCMDBlocking ERROR");
	//	return false;
	//}

}


CDlFactoryOs::CDlFactoryOs(void)
{

}

CDlFactoryOs::~CDlFactoryOs(void)
{

}
