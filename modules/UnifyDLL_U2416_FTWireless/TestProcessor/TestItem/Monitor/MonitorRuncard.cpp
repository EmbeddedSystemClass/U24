#include "StdAfx.h"
#include "MonitorRuncard.h"

CMonitorRuncard::CMonitorRuncard(void)
{
	m_szRuncard = "";
	m_szErrMsg = "";
}

CMonitorRuncard::~CMonitorRuncard(void)
{
}

bool CMonitorRuncard::GetRunCardById(std::string szId)
{
	unsigned char szBuf[30] = "";
	unsigned char szIdBuf[30] = "";

	for(unsigned i = 0; i < szId.length(); i++)
		szIdBuf[i] = szId[i];
	szIdBuf[szId.length()] = '\0';

	HINSTANCE hDll;
	CString str_dllF32SERVER2 = F32SERVERDB;
	hDll = LoadLibrary(str_dllF32SERVER2);
	if(hDll != NULL)
	{	typedef void(_stdcall *lpGetRuncardById)(unsigned char*,unsigned short,unsigned char*,unsigned short);
		lpGetRuncardById GetRuncardById;
		GetRuncardById = (lpGetRuncardById)::GetProcAddress(hDll,"GetRunCardById");
		if (GetRuncardById != NULL)
		{	GetRuncardById(szIdBuf,30,szBuf,30);
			FreeLibrary(hDll);
			m_szErrMsg = "";
			m_szRuncard = (char*)szBuf;
			return true;
		}
		else
		{	m_szErrMsg = "Load address of GetRunCardById fail.";
			FreeLibrary(hDll);
			m_szRuncard = (char*)szBuf;
			return false;
		}
	}
	else
	{	m_szErrMsg = "Load f32server2.dll fail.";
		m_szRuncard = (char*)szBuf;
		return false;
	}
}

std::string CMonitorRuncard::GetRuncard(void)
{	return m_szRuncard;
}

std::string CMonitorRuncard::GetErrMsg(void)
{	return m_szErrMsg;
}