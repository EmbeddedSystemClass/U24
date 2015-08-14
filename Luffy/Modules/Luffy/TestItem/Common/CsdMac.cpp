#include "stdafx.h"
#include "CsdMac.h"
#include <algorithm>

using namespace StringUtility;

GP_IMPLEMENT_DYNCREATE(CCsdMac)

CCsdMac::CCsdMac()
{

}

CCsdMac::~CCsdMac()
{
}

bool CCsdMac::InitData(std::map<std::string,std::string> &paramMap)
{
	memset(&m_st_UIMessage, 0, sizeof(st_UIMessage));

	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone =  dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	if (!m_pIPhone)
	{	TraceLog("Fail to get address of phone instance for CCsdMac");
		SendUiMsg("Fail to get address of phone instance for CCsdMac");
		return false;
	}

	TraceLog("============= CCsdMac ================");
	SendUiMsg("============= CCsdMac ================");

	TraceLog("Start to initdata for CCsdMac");
	SendUiMsg("Start to initdata for CCsdMac");
	m_pCGProc->SetEraseFlag(false);
	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());
	m_szImageDir =  myParm_node.child("ImageDir").text().as_string();
	m_szPadToolDir = myParm_node.child("WorkingDir").text().as_string();
	m_szFwFile = myParm_node.child("fw").text().as_string();
	m_szExeName = myParm_node.child("WriteSnExe").text().as_string();

	TraceLog("PASS to initdata for CCsdMac");
	SendUiMsg("PASS to initdata for CCsdMac");
	return true;
}

bool CCsdMac::MainFunction()
{
	return true;
}

bool CCsdMac::Run(int i_slot)
{

	TCHAR szMsg[]=TEXT("请按住治具ADFU键,再按Enter");
	TCHAR szTitle[] = TEXT("确认进入ADFU模式");
	m_pCGProc->m_pIPS->SetPowerSupplyOff();
	TraceLog("luffy power on for CCsdMac");
	SendUiMsg("Luffy power on for CCsdMac");
	m_pCGProc->m_pIPS->SetPowerSupplyOn();
	TraceLog("Power supply on for CCsdMac");
	SendUiMsg("Power supply on for CCsdMac");
	m_szStep = "WRITE";
	TraceLog("Start to get ethernet address for CCsdMac");
	SendUiMsg("Start to get ethernet address for CCsdMac");
	//MessageBox(NULL,szMsg,szTitle,MB_OK);
	if( false == GetMacAddr())
	{	TraceLog("Fail to get mac address in sss for CCsdMac");
		sprintf_s(m_st_UIMessage.sz_message,4096,"Fail to get mac address in sss for CCsdMac"); 
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage); 
		return false;
		//MessageBox(NULL,TEXT("ERROR"),szTitle,MB_OK);
	}
	else
	{	//MessageBox(NULL,szMsg,szTitle,MB_OK);
		TraceLog("Pass to get mac address in sss for CCsdMac");
		sprintf_s(m_st_UIMessage.sz_message,4096,"Pass to get mac address in sss for CCsdMac"); 
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage); 
	}
	Sleep(85000);
	m_pIPhone->SetPhoneMacAddress(m_szMACAddr);
	//MessageBox(NULL,szMsg,szTitle,MB_OK);
	if( false == m_pIPhone->WriteMacAddress(m_szExeName, m_szPadToolDir))
	{	//MessageBox(NULL,szMsg,szTitle,MB_OK);
		TraceLog("Fail to write mac address of phone for CCsdMac");
		sprintf_s(m_st_UIMessage.sz_message,4096,"Fail to write mac address of phone for CCsdMac"); 
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage); 
		return false;
	}
	else
	{	//MessageBox(NULL,szMsg,szTitle,MB_OK);
		TraceLog("Pass to write mac address of phone for CCsdMac");
		sprintf_s(m_st_UIMessage.sz_message,4096,"Pass to write mac address of phone for CCsdMac"); 
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage); 
	}

	TraceLog("Restart luffy for CCsdMac");
	SendUiMsg("Restart luffy for CCsdMac");
	m_pCGProc->m_pIPS->SetPowerSupplyOff();
	TraceLog("Power supply off for CCsdMac");
	SendUiMsg("Power supply off for CCsdMac");
	//MessageBox(NULL,TEXT("请按住治具ADFU键,再按Enter"),TEXT("确认进入ADFU模式"),MB_OK);

	MessageBox(NULL,szMsg,szTitle,MB_OK);

	TraceLog("Power supply off for CCsdMac");
	SendUiMsg("Power supply off for CCsdMac");

	m_pCGProc->m_pIPS->SetPowerSupplyOn();
	TraceLog("Power supply on for CCsdMac");
	SendUiMsg("Power supply on for CCsdMac");
	Sleep(4000);
	MessageBox(NULL,TEXT("请放开治具ADFU键,再按Enter"),TEXT("确认进入ADFU模式"),MB_OK);
	if( false == m_pCGProc->AutoDetectDevice())
	{	TraceLog("Fail to detect ADFU device for CCsdMac");
		SendUiMsg("Fail to detect ADFU device for CCsdMac");
		return false;
	}
	else
	{	TraceLog("Pass to detect ADFU device for CCsdMac");
		SendUiMsg("Pass to detect ADFU device for CCsdMac");
	}

	TraceLog("Start to download User Image for CCsdMac");
	SendUiMsg("Start to download User Image for CCsdMac");

	if( false == m_pIPhone->LoadOs(m_szFwFile, m_szImageDir, m_szPadToolDir))
	{	TraceLog("Fail to load user image of phone for CCsdMac");
		sprintf_s(m_st_UIMessage.sz_message,4096,"Fail to load user image of phone for CCsdMac"); 
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage); 
		return false;
	}
	else
	{	TraceLog("Pass to load user image of phone for CCsdMac");
		sprintf_s(m_st_UIMessage.sz_message,4096,"Pass to load user image of phone for CCsdMac"); 
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage); 
	}

	TraceLog("Power supply on for CCsdMac");
	SendUiMsg("Power supply on for CCsdMac");
	m_pCGProc->m_pIPS->SetPowerSupplyOff();
	TraceLog("Power supply off for CCsdMac");
	SendUiMsg("Power supply off for CCsdMac");
	Sleep(5000);
	m_pCGProc->m_pIPS->SetPowerSupplyOn();
	TraceLog("Power supply on for CCsdMac");
	SendUiMsg("Power supply on for CCsdMac");
	TraceLog("Start to upload result for CCsdMac");
	SendUiMsg("Start to upload result for CCsdMac");
	sprintf_s(m_szImagerVer,31,"v01.012.2");
	if( false == UpdateIMEIStatus_I())
	{	TraceLog("Fail to upload sssdb record for CCsdMac");
		SendUiMsg("Fail to upload sssdb record for CCsdMac");
		return false;
	}
	else
	{	TraceLog("Pass to upload sssdb record for CCsdMac");
		SendUiMsg("Pass to upload sssdb record for CCsdMac");
	}
	if(false == UploadData())
	{	TraceLog("Fail to upload yrstation record for CCsdMac");
		SendUiMsg("Fail to upload yrstation record for CCsdMac");
		return false;
	}
	else
	{	TraceLog("Pass to upload yrstation record for CCsdMac");
		SendUiMsg("Pass to upload yrstation record for CCsdMac");
	}
	return true;
}

bool CCsdMac::GetMacAddr()
{
	bool bReturn = false;
	char szStationState[4] = "B";  //"P"
	char szPartNo[31] = "";
	char szIMEI[31] = "";    //QCS340000011
	char szMSN[31] = "";
	char szBTAddr[31] = "";
	char szMACAddr[31] = "";
	char szSWver[31] = "";
	char szHWver[31] = "";
	char szPVK[31] = "";
	char szErrCode[31] = "";
	char szErrMsg[1024] = "";
	char szStatusOutput[31] = "";

	CString str_dllPICS_S3_FACTORY = PICS_S3_FACTORY;
	HMODULE hDll = ::LoadLibrary(str_dllPICS_S3_FACTORY);
	if(hDll != NULL)
	{	SendUiMsg("Pass to load Pics_S3_Factory.dll");
		typedef bool (*fpSSS_QueryIMEI_SSS02_V2)(char *szStationState, 
	                                             char *pIMEI, 
												 char *szMSN, 
												 char *szBTAddr, 
												 char *szMACAddr, 
												 char *szSWver, 
												 char *szHWver, 
												 char *szPVK, 
												 char *szErrCode, 
												 char *szErrMsg, 
												 char *szStatusOutput);
		fpSSS_QueryIMEI_SSS02_V2 iSSS_QueryIMEI_SSS02_V2 = (fpSSS_QueryIMEI_SSS02_V2)::GetProcAddress(hDll,"QueryIMEI_SSSO2_V2");
		if (iSSS_QueryIMEI_SSS02_V2 != NULL)
		{	SendUiMsg("Pass to load QueryIMEI_SSSO2_V2");
			//sprintf_s(szStationState,4 "%s","B");
			if(m_szStep == "WRITE")
			{	sprintf_s(szStationState,4 ,"B");
			}
			else
			{	sprintf_s(szStationState,4 ,"P");
			}
			sprintf_s(szIMEI,31, "%s", m_pIPhone->m_szIMEI.c_str());
			if(iSSS_QueryIMEI_SSS02_V2(szStationState,szIMEI,szMSN,szBTAddr,szMACAddr,szSWver,szHWver,szPVK,szErrCode,szErrMsg,szStatusOutput))
			{	TraceLog("Pass to load MacEthernet");
				SendUiMsg("Pass to load MacEthernet");
				m_szMSN = szMSN;
				m_szBTAddr = szBTAddr;
				m_szMACAddr = szMACAddr;
				m_szMACAddr.insert(2,1,':');
				m_szMACAddr.insert(5,1,':');
				m_szMACAddr.insert(8,1,':');
				m_szMACAddr.insert(11,1,':');
				m_szMACAddr.insert(14,1,':');
				//m_szSWver = m_szImagerVer;
				m_szHWver = szHWver;
				m_szPVK = szPVK;
				m_szErrCode = szErrCode;
				m_szErrMsg = szErrMsg;
				m_szStatusOutput = szStatusOutput;
				bReturn = true;
			}
			else
			{	TraceLog("fail to load MacEthernet");
				SendUiMsg("fail to load MacEthernet");
				SendUiMsg(szErrMsg);
				goto Exit_FreeLibrary;
			}
		}
		else
		{	TraceLog("fail to load QueryIMEI_SSSO2_V2");
			SendUiMsg("fail to load QueryIMEI_SSSO2_V2");
			goto Exit_FreeLibrary;
		}
	} 
	else
	{	TraceLog("fail to load Pics_S3_Factory.dll");
		SendUiMsg("fail to load Pics_S3_Factory.dll");
		goto Exit;
	}
Exit_FreeLibrary:
	FreeLibrary(hDll);
Exit:
	return bReturn;
}

bool CCsdMac::UpdateIMEIStatus_I(void)
{
	bool bReturn = false;
	char szIMEI[31] = "";  //QCS340000011
	char szId[15] = "";
	char szErrCode[31] = "";
	char szErrMsg[1024] = "";

	CString str_dllPICS_S3_FACTORY = PICS_S3_FACTORY;
	HMODULE hDll = ::LoadLibrary(str_dllPICS_S3_FACTORY);
	if(hDll != NULL)
	{	typedef bool (*fpUpdateIMEIStatus_I_V2)(char *pIMEI,char *szSWver,char *szHWver,char *szMal,char *szErrCode, char *szErrMsg);
		fpUpdateIMEIStatus_I_V2 iUpdateIMEIStatus_I = (fpUpdateIMEIStatus_I_V2)::GetProcAddress(hDll,"UpdateIMEIStatus_I_V2");
		if(iUpdateIMEIStatus_I != NULL)
		{	sprintf_s(szIMEI,31,"%s",m_pIPhone->m_szIMEI.c_str());
		sprintf_s(szId,15,"%s",m_pIPhone->m_szSN.c_str());
			char szMal[16] = "";
			sprintf_s(szMal,16,"%s", m_szWifiAdd.c_str());
			if(iUpdateIMEIStatus_I(szIMEI,m_szImagerVer,szId,szMal,szErrCode,szErrMsg))
			{	bReturn = true;
			}
			else
			{	SendUiMsg("fail to upload status");
				SendUiMsg(szErrMsg);
				goto Exit_FreeLibrary;
			}
		}
		else
		{	SendUiMsg("fail to load UpdateIMEIStatus_I_V2");
			goto Exit_FreeLibrary;
		}
	}
	else
	{	SendUiMsg("fail to load Pics_S3_Factory.dll");
		goto Exit;
	}
Exit_FreeLibrary:
	FreeLibrary(hDll);
Exit:
	return bReturn;
}

void CCsdMac::SendUiMsg(const char *szMsg)
{
	sprintf_s(m_st_UIMessage.sz_message,4096,szMsg); 
	m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage); 
}

bool CCsdMac::UploadData(void)
{	bool bReturn = false;
	CString str_dllF32SERVER2 = F32SERVERDB;

	HMODULE hDll ;
	hDll = ::LoadLibrary(str_dllF32SERVER2);

	if( hDll != NULL )
	{	typedef bool (_stdcall *lpInsertYrstation)(const unsigned char* Model,     unsigned short ModelLen,
				   		        				   const unsigned char* Id,        unsigned short IdLen,
												   const unsigned char* Shift,     unsigned short ShiftLen,
												   const unsigned char* Station,   unsigned short StationLen,
												   const unsigned char* Trid,      unsigned short TridLen,
												   const unsigned char* Operator,  unsigned short OperatorLen,
												   unsigned short CycleTime,
												   const unsigned char* CheckInfo, unsigned short CheckInfoLen);
		lpInsertYrstation iInsertYrstation = (lpInsertYrstation)::GetProcAddress(hDll,"InsertYrstation");
		if( iInsertYrstation != NULL)
		{	
			sprintf_s(m_st_UIMessage.sz_message,4096,"Start InsertYrstation...");
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
			unsigned char szPicasso[10];
			unsigned char szLine[5]="B21";
			unsigned char szStation[20]="CSD_IMEI";
			unsigned char szModel[10]="QS2214R";
			unsigned char szTrid[5]="";
			unsigned char szOperator[5]="";
			unsigned char szCheckInfo[5]="";
			char szTemp[10];
			sprintf_s(szTemp,10,"");
			sprintf_s(szTemp,10,"%s",m_pIPhone->m_szSN.c_str());
			memcpy(szPicasso,szTemp,10);
			bReturn = iInsertYrstation(szModel,   (unsigned short)strlen((char*)szModel),
									   szPicasso,	(unsigned short)strlen((char*)szPicasso),
									   szLine,    (unsigned short)strlen((char*)szLine), 
									   szStation, (unsigned short)strlen((char*)szStation),
									   szTrid,     5,
									   szOperator,   5,
									   0,
									   szCheckInfo,  5);
			goto Exit_FreeLibrary;
		}
		else
		{	sprintf_s(m_st_UIMessage.sz_message,4096,"Load InsertYrstation fail");
			m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
			goto Exit_FreeLibrary;
		}
	}
	else
	{	//AnsiString str ="";
		//str+=SysErrorMessage(GetLastError());
		//MessageBox(NULL,str.c_str(),"ERROR",MB_OK);
		sprintf_s(m_st_UIMessage.sz_message,4096,"Load f32server2.dll fail");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		goto Exit;
	}
Exit_FreeLibrary:
	FreeLibrary(hDll);
Exit:
	return bReturn;
}