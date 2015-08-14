#include "StdAfx.h"
#include "RecoverKeyTest.h"
#include "FacTestToolInterface.h"

using namespace StringUtility;

GP_IMPLEMENT_DYNCREATE(CRecoverKeyTest)

CRecoverKeyTest ::CRecoverKeyTest(void)
{
}

CRecoverKeyTest::~CRecoverKeyTest(void)
{
}

bool CRecoverKeyTest::InitData(std::map<std::string, std::string>& paramMap)
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
	m_pCGProc->SetEraseFlag(false);

	//Load XML
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());

	//m_szWorkingDir = myParm_node.child("WorkingDir").text().as_string(ERROR_STRING);
	//CHECK_PARM("WorkingDir", m_szWorkingDir, ERROR_STRING);

	//m_szExeName = myParm_node.child("TESTEXE").text().as_string(ERROR_STRING);
	//CHECK_PARM("TESTEXE", m_szExeName, ERROR_STRING);

	//m_szRecoverKey = myParm_node.child("ITEMCMD").text().as_string(ERROR_STRING);
	//CHECK_PARM("ITEMCMD", m_szRecoverKey, ERROR_STRING);

	m_szRecoverKeySize = myParm_node.child("KeyColumn").text().as_string(ERROR_STRING);
	CHECK_PARM("KeyColumn", m_szRecoverKeySize, ERROR_STRING);

	m_szIicCmdTemp = myParm_node.child("IICCMD").text().as_string(ERROR_STRING);
	CHECK_PARM("IICCMD", m_szIicCmdTemp, ERROR_STRING);

	m_iIicCmdCount = myParm_node.child("IICCMDCount").text().as_int(ERROR_INT);
	CHECK_PARM("IICCMDCount", m_iIicCmdCount, ERROR_INT);

	std::string szTemp = m_szIicCmdTemp.substr(0,4);
	iiccmd = (int)strtol(szTemp.c_str(),0,0);
	szTemp = m_szIicCmdTemp.substr(5,4);
	iicadd = (int)strtol(szTemp.c_str(),0,0);
	szTemp = m_szIicCmdTemp.substr(10,4);
	iicdat = (int)strtol(szTemp.c_str(),0,0);
	szTemp = m_szIicCmdTemp.substr(15,m_szIicCmdTemp.length() - 14);
	iicdel = (int)strtol(szTemp.c_str(),0,0);

	m_iRetryTime = myParm_node.child("RetryTime").text().as_int();

	sprintf_s(m_szCmdLine,"%s %s",m_szExeName.c_str(),m_szRecoverKey.c_str());

	TraceLog("PASS to initdata for CRecoverKeyTest");
	sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to initdata for CRecoverKeyTest");
	m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
	TraceLog(m_szCmdLine);
	return true;
}

bool CRecoverKeyTest::Run(int i_slot)
{
	return MainFunction();
}

bool CRecoverKeyTest::MainFunction()
{
	//Set buffer to get result of CMD

	std::string szResult = "";
	int i = 0;
	bool bResult = false;
	do
	{
		i++;
		if( i > m_iRetryTime )
		{	break;
		}

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
			continue;
		}

		sprintf_s(m_st_UIMessage.sz_message,4096,"CRecoverKeyTest is running......");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);

		//Send IIC CMD
		m_p_h_iicthread = ::AfxBeginThread(iicFunction, (void*)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		if (m_p_h_iicthread == NULL)
		{	TraceLog("FAIL to run iicFunction");
			sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to run iicFunction");
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
			continue;
		}
		else
		{	TraceLog("PASS to run iicFunction");
			sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to run iicFunction");
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		}
		Sleep(50);

		m_adfucmd = false;
		//szResult = m_pIPhone->GetRecoverKeyStatus(m_szExeName,m_szWorkingDir);
		szResult = m_pIPhone->GetRecoverKeyStatus();
		m_adfucmd = true;
		TraceLog(szResult);
		if ( "NULL" == szResult )
		{	TraceLog("FAIL to get ADFU comand return. BUF=NULL");
			sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to get ADFU comand return. BUF=NULL");
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
			continue;
		}
		else if ( "ERROR" == szResult )
		{	TraceLog("FAIL to execute ADFU comand");
			sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to execute ADFU comand");
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
			continue;
		}

		while(m_iicFunction)
		{	Sleep(10);
		}

		if (!szResult.empty())
		{	std::string::size_type i = szResult.find(m_szRecoverKeySize);
			if(i == string::npos)
			{	TraceLog("FAIL to KeyColumn of CRecoverKeyTest");
				sprintf_s(m_st_UIMessage.sz_message,4096,"FAIL to KeyColumn of CRecoverKeyTest");
				m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);	
				continue;
			}
			else
			{	TraceLog("PASS to KeyColumn of CRecoverKeyTest");
				sprintf_s(m_st_UIMessage.sz_message,4096,"PASS to KeyColumn of CRecoverKeyTest");
				m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
				bResult = true;
			}
		}
	}
	while(false == bResult);

	if(false == bResult)
	{	m_pCGProc->SetFailMsg("RecoverKey fail");
		m_pCGProc->SendMsgToUi("RecoverKey fail");
		//TraceLog(szResult.c_str());
		TraceLog("RecoverKey fail.");
		return false;
	}
	else
	{	m_pCGProc->SendMsgToUi("RecoverKey pass.");
		TraceLog("RecoverKey pass.");
		return true;
	}
	//Free I2C.dll
}

UINT CRecoverKeyTest::iicFunction(LPVOID pParam)
{
	CRecoverKeyTest* p_RecoverKeyTest = (CRecoverKeyTest*)pParam;
	p_RecoverKeyTest->m_iicFunction = true;
	//Load iic.dll
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
		{	int i;
			for( i = 0; i < p_RecoverKeyTest->m_iIicCmdCount; i++)
			{	int iRet = WriteCommand(p_RecoverKeyTest->iiccmd ,p_RecoverKeyTest->iicadd ,p_RecoverKeyTest->iicdat ,p_RecoverKeyTest->iicdel ,1);

				sprintf_s(p_RecoverKeyTest->m_st_UIMessage.sz_message,4096,"I2C command return %d",iRet);
				p_RecoverKeyTest->m_pCGProc->m_p_event_callback("UIMessage",(long)&p_RecoverKeyTest->m_st_UIMessage);

				if(p_RecoverKeyTest->m_adfucmd)
				{	break;
				}
			}
		}
		FreeLibrary(hDll);
	}
	else
	{
		sprintf_s(p_RecoverKeyTest->m_st_UIMessage.sz_message,4096,"Load I2C fail...");
		p_RecoverKeyTest->m_pCGProc->m_p_event_callback("UIMessage",(long)&p_RecoverKeyTest->m_st_UIMessage);
	}

	p_RecoverKeyTest->m_iicFunction = false;

	return 1;
}