#include "StdAfx.h"
#include "ResetKeyTest.h"
//#include "d:\source\C++\MyLuffy\Luffy\Modules\UnifyInterface\FacTestToolInterface.h"

using namespace StringUtility;

GP_IMPLEMENT_DYNCREATE(CResetKeyTest)

CResetKeyTest::CResetKeyTest()
{
}

CResetKeyTest::~CResetKeyTest()
{
}

bool CResetKeyTest::InitData(std::map<std::string,std::string> &paramMap)
{
	memset(&m_st_UIMessage, 0, sizeof(st_UIMessage));
	std::string szString;
	//Get
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone =  dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	if (!m_pIPhone)
	{
		TraceLog("Fail to get address of phone instance for CRecoverKeyTest");
		return false;
	}

	m_pCGProc->SetEraseFlag(true);
	//Load XML
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());
	//m_szWorkingDir = myParm_node.child("WorkingDir").text().as_string();

	m_iTimeForPowerOff = myParm_node.child("WaitTimeForPowerOff").text().as_int(ERROR_INT);
	CHECK_PARM("WaitTimeForPowerOff", m_iTimeForPowerOff, ERROR_INT);

	m_iTimeForPowerOn = myParm_node.child("WaitTimeForPowerOn").text().as_int(ERROR_INT);
	CHECK_PARM("WaitTimeForPowerOn", m_iTimeForPowerOn, ERROR_INT);

	m_szIicCmdTemp = myParm_node.child("IICCMD").text().as_string(); 
	std::string szTemp = m_szIicCmdTemp.substr(0,4);
	iiccmd = (int)strtol(szTemp.c_str(),0,0);
	szTemp = m_szIicCmdTemp.substr(5,4);
	iicadd = (int)strtol(szTemp.c_str(),0,0);
	szTemp = m_szIicCmdTemp.substr(10,4);
	iicdat = (int)strtol(szTemp.c_str(),0,0);
	szTemp = m_szIicCmdTemp.substr(15,m_szIicCmdTemp.length() - 14);
	iicdel = (int)strtol(szTemp.c_str(),0,0);

	TraceLog("PASS to initdata for CRecoverKeyTest");
	sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to initdata for CRecoverKeyTest");
	m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	return true;
}

bool CResetKeyTest::Run(int i_slot)
{
	m_st_UIMessage.i_slot = i_slot;
	return MainFunction();
}

bool CResetKeyTest::MainFunction()
{

	unsigned short status = 0;
	int iRetryConnectTimes = 10;
	// 1. Initial Qisda module
	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to initial Qisda module";
		TraceLog(m_strMsg);
		return false;
	}

	//m_pIPhone->SetWorkingDir(m_szWorkingDir.c_str());
	if (status == 0)
	{	//检查Port存在
		// 2. Connect phone
		sprintf_s(m_st_UIMessage.sz_message,4096,"Connect phone...");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		for (int i = 0; i < iRetryConnectTimes; ++i)
		{   if (m_pIPhone->IsConnected())
			{   status = 1;
				break;
			}
			Sleep(100);
		}
		if (status == 0)
		{	m_strMsg = "Fail to connect phone with Qisda module";
			TraceLog(m_strMsg);
			return false;
		}
	}

	if (status == 1)
	{	//下 command 0xF8 0x01
		sprintf_s(m_st_UIMessage.sz_message,4096,"Write reset IIC CMD...");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		HINSTANCE hDll = LoadLibrary(_T("I2C.dll"));
		if(hDll != NULL)
		{	//Load 6e to 7c
			typedef void(_stdcall *lpSlaveAddress7C)(void);
			lpSlaveAddress7C SlaveAddress7C;
			SlaveAddress7C = (lpSlaveAddress7C)GetProcAddress(hDll,"SlaveAddress7C");
			if (SlaveAddress7C != NULL)
			{	SlaveAddress7C();
			}

			//Load writecmd
			typedef int(_stdcall *lpWriteCommand)(int,int,int,int,int);
			lpWriteCommand WriteCommand;
			WriteCommand = (lpWriteCommand)GetProcAddress(hDll,"WriteCommand");
			if (WriteCommand != NULL)
			{	for( int i = 0; i < 1; i++)
				{	int iRet = WriteCommand(iiccmd ,iicadd ,iicdat ,iicdel ,1);
					//if ( iRet == iicdat )
					//{	status = 2;
					//	break;
					//}
					status = 2;
				}
			}
			FreeLibrary(hDll);
			if (status == 1)
			{	TraceLog("FAIL to sned of I2C CMD");
				sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to sned of I2C CMD");
				m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
				return false;
			}
		}
		else
		{	TraceLog("FAIL to load of I2C.DLL");
			sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to load of I2C.DLL");
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
			return false;
		}
	}

	if (status == 2)
	{	Sleep(m_iTimeForPowerOff);
		//检查Port不存在
		sprintf_s(m_st_UIMessage.sz_message,4096,"Disconnect phone...");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		for (int i = 0; i < iRetryConnectTimes; ++i)
		{   if (m_pIPhone->IsConnected())
			{   Sleep(500);
			}
			else
			{	status = 3;
				break;
			}
		}
		if (status == 2)
		{	TraceLog("FAIL to power off of monitor");
			sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to power off of monitor");
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
			return false;
		}
	}

	if (status == 3)
	{	//等待
		Sleep(m_iTimeForPowerOn);

		//检查Port存在
		sprintf_s(m_st_UIMessage.sz_message,4096,"Connect phone...");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		for (int i = 0; i < iRetryConnectTimes; ++i)
		{   if (m_pIPhone->IsConnected())
			{   status = 4;
				break;
			}
			else
			{	Sleep(500);
			}
		}
		if (status == 3)
		{	TraceLog("FAIL to power on of monitor");
			sprintf_s(m_st_UIMessage.sz_message,4096,"Fail to power on of monitor");
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		}
	}

	if(status == 4)
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"Pass to reset monitor");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		return true;
	}
	else
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"Fail to reset monitor");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		m_pCGProc->SetFailMsg("Restart monitor fail");
		return false;
	}

}