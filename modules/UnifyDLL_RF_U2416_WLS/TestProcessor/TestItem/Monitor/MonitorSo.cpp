#include "stdafx.h"
#include "MonitorSo.h"

CMonitorSo::CMonitorSo()
{	m_szPartNo = "";
	m_szSoType = "";
	m_szCustomer = "";
	m_szSoVer = "";
	m_szSoInfo = "";
	m_iSoQty = 0;
	m_szSo = "";
	m_szErrMsg = "";
}

CMonitorSo::~CMonitorSo(void)
{
}

bool CMonitorSo::SearchSoInfo(void)
{
	unsigned char szPartNo[30] = "";
	unsigned char szSocateory[30] = "";
	unsigned char szSoInfo[30] = "";
	unsigned char szCustomer[30] = "";
	unsigned char szSoVer[30] = "";
	int   iSoQty;
	unsigned char szSo[30]="";

	m_szPartNo = "";
	m_szSoType = "";
	m_szCustomer = "";
	m_szSoVer = "";
	m_szSoInfo = "";
	m_iSoQty = 0;

	CString str_dllF32SERVER2 = F32SERVERDB;
	HINSTANCE hDll;
	hDll = LoadLibrary(str_dllF32SERVER2);
	if(hDll != NULL)
	{	typedef void(_stdcall *lpGetSoInfo)(unsigned char*,unsigned short,
	                                        unsigned char*,unsigned short,
											int*,
											unsigned char*,unsigned short,
											unsigned char*,unsigned short,
											unsigned char*,unsigned short,
											unsigned char*,unsigned short);
		lpGetSoInfo GetSoInfo;
		GetSoInfo = (lpGetSoInfo)::GetProcAddress(hDll,"GetSoInfo");
		if (GetSoInfo != NULL)
		{	//m_szSo.copy((char*)szSo,m_szSo.length(),0);
			//*szSo =(unsigned char)m_szSo.c_str();
			for(unsigned short i = 0; i < m_szSo.length(); i++)
			{	szSo[i] = m_szSo[i];
			}
			szSo[m_szSo.length()] = '\0';
			GetSoInfo(szSo,30,szPartNo,30,&iSoQty,szSocateory,30,szSoInfo,30,szCustomer,30,szSoVer,30);
			FreeLibrary(hDll);
			m_szPartNo = (char*)szPartNo;
			m_szSoType = (char*)szSocateory;
			m_szCustomer = (char*)szCustomer;
			m_szSoVer = (char*)szSoVer;
			m_szSoInfo = (char*)szSoInfo;
			m_iSoQty = iSoQty;
			return true;
		}
		else
		{	FreeLibrary(hDll);
			m_szErrMsg = "Load address of GetSoInfo fail.";
			return false;
		}
	}
	else
	{	m_szErrMsg = "Load f32server2.dll fail.";
		return false;
	}
}

bool CMonitorSo::SetSo(std::string szSo)
{
	m_szSo = szSo;
	if(SearchSoInfo())
	{	return true;
	}
	else
	{	m_szSo = "";
		return false;
	}
}

bool CMonitorSo::GetSoByRuncard(std::string szRuncard)
{
	int iLen = szRuncard.length();
	if(iLen == 13 || iLen == 15 || iLen == 17)
	{	m_szSo = szRuncard.substr(0,6);
		if(SearchSoInfo())
		{	return true;
		}
		else
		{	m_szSo ="";
			return false;
		}
	}
	else if(iLen == 14 || iLen == 16 || iLen == 18)
	{	m_szSo = szRuncard.substr(0,7);
		if(SearchSoInfo())
		{	return true;
		}
		else
		{	m_szSo ="";
			return false;
		}
	}
	else
	{	m_szSo ="";
		m_szErrMsg = "Runcard lenght error.";
		return false;
	}
}

std::string CMonitorSo::GetSo(void)
{	return m_szSo;
}

std::string CMonitorSo::GetCustomer(void)
{	return m_szCustomer;
}

std::string CMonitorSo::GetSoType(void)
{	return m_szSoType;
}

int CMonitorSo::GetSoQty(void)
{	return m_iSoQty;
}

std::string CMonitorSo::GetSoVer(void)
{	return m_szSoVer;
}

std::string CMonitorSo::GetSoInfo(void)
{	return m_szSoInfo;
}