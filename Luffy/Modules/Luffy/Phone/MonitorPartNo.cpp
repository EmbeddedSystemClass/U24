#include "stdafx.h"
#include "MonitorPartNo.h"
#include <algorithm>

CMonitorPartNo::CMonitorPartNo(void)
{	m_szErrMsg = "";
}

CMonitorPartNo::~CMonitorPartNo(void)
{
}

bool CMonitorPartNo::SetPartNo(std::string szPartNo) 
{
	m_szPartNo = szPartNo;
	return true;
}

std::string CMonitorPartNo::GetPartNo(void)
{	return m_szPartNo;
}

bool CMonitorPartNo::SearchPartNo(std::string szSo)
{
	unsigned char szPartNo[30] = "";
	unsigned char szSocateory[30] = "";
	unsigned char szSoInfo[30] = "";
	unsigned char szCustomer[30] = "";
	unsigned char szSoVer[30] = "";
	int   iSoQty;
	unsigned char szSO[30]="";

	m_szErrMsg = "";
	HINSTANCE hDll;
	CString str_dllF32SERVER2 = F32SERVERDB;
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
			for(unsigned short i = 0; i < szSo.length(); i++)
			{	szSO[i] = szSo[i];
			}
			szSO[szSo.length()] = '\0';
			GetSoInfo(szSO,30,szPartNo,30,&iSoQty,szSocateory,30,szSoInfo,30,szCustomer,30,szSoVer,30);
			FreeLibrary(hDll);
			m_szPartNo = (char*)szPartNo;
			transform(m_szPartNo.begin(),m_szPartNo.end(),m_szPartNo.begin(),toupper);
			return true;
		}
		else
		{	m_szErrMsg = "Load address of GetSoInfo fail.";
			FreeLibrary(hDll);
			return false;
		}
	}
	else
	{	m_szErrMsg = "Load f32server2.dll fail.";
		return false;
	}
}

std::string CMonitorPartNo::GetErrMsg(void)
{	return m_szErrMsg;
}