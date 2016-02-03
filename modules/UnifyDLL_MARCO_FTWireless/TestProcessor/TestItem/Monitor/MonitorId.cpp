#include "StdAfx.h"
#include "MonitorId.h"

CMonitorId::CMonitorId(void)
{	m_szId = "";
	m_szErrMsg = "";
}

CMonitorId::~CMonitorId(void)
{
}

bool CMonitorId::ReadId (void)
{
	char szBuf[20]="";
	CString str_dllIIC = IICDLL;
	HINSTANCE hDll;
	hDll = LoadLibrary(str_dllIIC);
	if (hDll != NULL )
	{
		typedef void(_stdcall *lpReadId)(char*,int);
		lpReadId ReadId;
		ReadId =(lpReadId)::GetProcAddress(hDll,"ReadId");
		if( ReadId != NULL)
		{
			ReadId(szBuf,20);
			FreeLibrary(hDll);
			m_szErrMsg = "";
			m_szId = szBuf;
			//AfxMessageBox(m_szId.c_str());
			return true;
		}
		else
		{	FreeLibrary(hDll);
			m_szErrMsg = "Load address of ReadId fail.";
			m_szId = szBuf;
			AfxMessageBox(m_szErrMsg.c_str());
			return false;
		}
	}
	else
	{	m_szErrMsg = "Load IIC.DLL fail.";
		m_szId = szBuf;
		AfxMessageBox(m_szErrMsg.c_str());
		return false;
	}
}

std::string CMonitorId::GetId(void)
{	return m_szId;
}

std::string CMonitorId::GetErrMsg(void)
{	return m_szErrMsg;
}