#include "stdafx.h"
#include "UpLoadRecordToYrstation.h"
#include <algorithm>

using namespace StringUtility;

GP_IMPLEMENT_DYNCREATE(CUploadRecordToYrstation)

CUploadRecordToYrstation::CUploadRecordToYrstation(void)
{
}

CUploadRecordToYrstation::~CUploadRecordToYrstation(void)
{
}

bool CUploadRecordToYrstation::InitData(std::map<std::string,std::string> &paramMap)
{
	//Get
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone =  dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());
	if (!m_pIPhone)
	{
		TraceLog("Fail to get address of phone instance for CUsbTest");
		return false;
	}
	memset(&m_st_UIMessage, 0, sizeof(st_UIMessage));

	pugi::xml_node myParm_node = m_pCGProc->m_pXMLTestItem->child("Configuration").child("Param").child(m_NickName.c_str());
	std::string szLine = myParm_node.child("Line").text().as_string(ERROR_STRING);
	if(szLine == ERROR_STRING)
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"Line is NULL");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		return false;
	}
	memset(m_szPicasso,'\0',10);

	std::string szStation = myParm_node.child("Station").text().as_string(ERROR_STRING);
	if(szStation == ERROR_STRING)
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"Station is NULL");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		return false;
	}
	memset(m_szStation,'\0',20);
	char szStationTemp[20]="\0";
	memcpy(szStationTemp,szStation.c_str(),szStation.length());
	memcpy(m_szStation,szStationTemp,strlen(szStationTemp));

	std::string szStationSerNo = myParm_node.child("StationSerNo").text().as_string(ERROR_STRING);
	if (szStationSerNo == ERROR_STRING)
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"StationSerNo is NULL");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		return false;
	}
	else
	{	//transform(szStationSerNo.begin(),szStationSerNo.end(),szStationSerNo.begin(),toupper);
	}
	szLine.append(szStationSerNo);
	//memset(m_szStationSerNo,'\0',5);
	//memcpy(m_szStationSerNo,szStationSerNo.c_str(),szStationSerNo.length());
	char szLineTemp[5]="\0";
	sprintf_s(szLineTemp,5,"%s",szLine.c_str());
	int ii =strlen(szLineTemp);
	memset(m_szLine,'\0',5);
	memcpy(m_szLine,szLineTemp,ii);

	std::string szModel = myParm_node.child("Model").text().as_string(ERROR_STRING);
	if(szModel == ERROR_STRING)
	{	sprintf_s(m_st_UIMessage.sz_message,4096,"Model is NULL");
		m_pCGProc->m_p_event_callback("UIMessage",(long)&m_st_UIMessage);
		return false;
	}
	memset(m_szModel,'\0',10);
	memcpy(m_szModel,szModel.c_str(),szModel.length());

	//memset(m_szPicasso,'\0',10);
	this->LoadPicasso();

	return true;
}

bool CUploadRecordToYrstation::Run(int i_slot)
{	return MainFunction();
}

bool CUploadRecordToYrstation::MainFunction(void)
{	return UploadData();
}

bool CUploadRecordToYrstation::LoadPicasso(void)
{	char szTemp[10];
	sprintf_s(szTemp,10,"");
	sprintf_s(szTemp,10,"%s",m_pCGProc->g_mapToolInfo["PICASSO"].c_str());
	memcpy(m_szPicasso,szTemp,10);

	return false;
}

bool CUploadRecordToYrstation::UploadData(void)
{	bool bReturn = false;
	CString str_dllF32SERVER2 = F32SERVERDB;

	WCHAR Buf[1000];
	GetCurrentDirectory(1000,Buf);

	//if(_taccess(_TEXT(".\\F32server2.dll"),0) != 0)
	//{	return false;
	//}
	

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

			unsigned char szTrid[5]="";
			unsigned char szOperator[5]="";
			unsigned char szCheckInfo[5]="";
			bReturn = iInsertYrstation(m_szModel,   (unsigned short)strlen((char*)m_szModel),
									   m_szPicasso,	(unsigned short)strlen((char*)m_szPicasso),
									   m_szLine,    (unsigned short)strlen((char*)m_szLine), 
									   m_szStation, (unsigned short)strlen((char*)m_szStation),
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