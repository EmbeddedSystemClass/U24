#include "StdAfx.h"
#include "CoincellVol.h"
#include "FacTestToolInterface.h"

using namespace StringUtility;

GP_IMPLEMENT_DYNCREATE(CCoincellVol)

CCoincellVol::CCoincellVol(void)
{
}

CCoincellVol::~CCoincellVol(void)
{
}

bool CCoincellVol::InitData(std::map<std::string, std::string>& paramMap)
{
	//memset(&m_st_UIMessage, 0, sizeof(st_UIMessage));
	//std::string szString;
	//Get
	TraceLog("Start initdata for CCoincellVol");

	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone =  dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	if (!m_pIPhone)
	{
		TraceLog("Fail to get address of phone instance for CCoincellVol");
		return false;
	}
	
	m_pCGProc->SetEraseFlag(true);
	/*
	//Load XML   D:\source\C++\Luffy_ModifyCurrentSpec_AddCPKLog\Luffy\Solution\Debug\Qisda\GLUFB1A_BBTEST_TestItem.xml
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());
	m_szWorkingDir = myParm_node.child("WorkingDir").text().as_string();
	//m_szExeName = myParm_node.child("TESTEXE").text().as_string();
	m_szCoincellVol = myParm_node.child("ITEMCMD").text().as_string();

	std::string szTemp = myParm_node.child("Voltage").text().as_string(); 
	if (szTemp.empty())
	{
		TraceLog("FAIL to initdata for CCoincellVol");
		sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to initdata for CCoincellVol");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		return false;
	}
	m_iCoincellVolSize = (int)strtol(szTemp.c_str(),0,0);
	//m_szIicCmdTemp = myParm_node.child("IICCMD").text().as_string(); 

	sprintf_s(m_szCmdLine,512,"shell cat /sys/class/hwmon/hwmon1/device/backupbat_voltage");

	TraceLog("PASS to initdata for CCoincellVol");
	sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to initdata for CCoincellVol");
	m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	TraceLog(m_szCmdLine);
	*/
	TraceLog("PASS to initdata for CCoincellVol");
	m_pCGProc->SendMsgToUi("PASS to initdata for CCoincellVol");
	return true;
}

bool CCoincellVol::Run(int i_slot)
{
	return MainFunction();
}

bool CCoincellVol::MainFunction()
{
	//Set buffer to get result of CMD
	//char Buf[100]={0};

	//sprintf_s(m_st_UIMessage.sz_message,4096,"CCoincellVol is running......");
	//m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

	TraceLog("CCoincellVol is running");
	m_pCGProc->SendMsgToUi("CCoincellVol is running......");

	std::string szReturn = m_pIPhone->CheckCoincellVol();
	if( "PASS" == szReturn )
	{	TraceLog(szReturn.c_str());
		TraceLog("Coincell Voltage pass");
		m_pCGProc->SendMsgToUi("Coincell Voltage pass");
		return true;
	}
	else
	{	m_pCGProc->SetFailMsg("Coincell Voltage fail");
		TraceLog(szReturn.c_str());
		TraceLog("Coincell Voltage fail");
		m_pCGProc->SendMsgToUi(szReturn.c_str());
		m_pCGProc->SendMsgToUi("Coincell Voltage fail");
		return false;
	}
	/*
	//Set working dirszWorkingDir
	char szWorkingDir[512]={0};
	sprintf_s(szWorkingDir,512,m_szWorkingDir.c_str());

	//ExecCmdUtil::ExecuteCmdBlocking("ADB.exe",m_szCmdLine,szWorkingDir, &Buf);
	
	char* BUf = NULL;
	char** BUF=NULL;
	BUf = Buf;
	BUF = &BUf;
    if(ExecCmdUtil::ExecuteCmdBlockingWithTimeout("ADB.exe",m_szCmdLine, szWorkingDir, BUF, 4000) != ERROR_SUCCESS)
	{
		
		TraceLog("FAIL to  CCoincellVol test");
         sprintf_s(m_st_UIMessage.sz_message,4096,"CCoincellVol failed......");
	     m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
        return false;
	}

	std::string Buf1 = Buf;
	if (!Buf1.empty())
	{
		int iVol = (int)strtol(Buf1.c_str(),0,0);
		char szVol[50]="";
		char szLower[50] = "";
		sprintf_s(szVol, 50, "%.2f", iVol * 5.86);
		sprintf_s(szLower, 50, "%.2f", m_iCoincellVolSize * 5.86);
		if (iVol >= m_iCoincellVolSize)
		{
			m_strMsg = MakeSTDstring("Lower: %s mV,CoincellVoltage MeasureResult: %s mV, CoincellVoltage PASS", szLower, szVol);
			sprintf_s(m_st_UIMessage.sz_message,4096,m_strMsg.c_str());
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
			return true;
		}
		else
		{
			m_strMsg = MakeSTDstring("Lower: %s mV,CoincellVoltage MeasureResult: %s mV, CoincellVoltage Fail", szLower, szVol);
			sprintf_s(m_st_UIMessage.sz_message,4096,m_strMsg.c_str());
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
			return false;
		}

	}
	*/
	//return true;
}