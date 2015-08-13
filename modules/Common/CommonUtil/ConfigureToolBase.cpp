#include "stdafx.h"
#include "ConfigureToolBase.h"
#include <cassert>

CConfigureToolBase::CConfigureToolBase(LPCTSTR szConfigureTitle, LPCTSTR szFolderName)
:m_strConfigureTitle(szConfigureTitle)
,m_pIConfigure(CreateIConfigure(szConfigureTitle,szFolderName))
{
	if(m_pIConfigure)
		m_pIConfigure->Load();
}

CConfigureToolBase::~CConfigureToolBase()
{
	if(m_pIConfigure)
	{
		m_pIConfigure->Save();
		DeleteIConfigure(m_strConfigureTitle.c_str());
	}
}

TSTRING CConfigureToolBase::GetStringValueByIndex(const TSTRING& strName,int nIndex)
{
	TCHAR szTemp[1024] = {0};

	TCHAR szName[1024] = {0};
	sprintf(szName,_T("%s_%02d"),strName.c_str(),nIndex);
	bool bRes = GetValue(szName,szTemp,szTemp);
	assert(bRes);
	return szTemp;
}

/*
20090729 Alan Tu
For fix crash issue from different CRT version issue (VC6 and VC2005)
Root cause : 
Each CRT has a kind of memory manager. If return type is a class like CString or TSTRING,
the real case will cause the new and delete functions use different memory manager.
And the application will crash.
Solution :
For avoid the case, we can use all projects from the same CRT version.
If we can't do it, we should happen the crash case, the new and delete functions use different memory manager.
So we should check if the memory buffer is created and destoryed in the same CRT version.
If not, return false.

And avoid ti use the old interface "TSTRING CConfigureToolBase::GetStringValueByIndex(const TSTRING& strName,int nIndex)"
*/
bool CConfigureToolBase::GetValue(LPCTSTR szName,LPTSTR szValue,LPCTSTR szDefaultValue)
{
	bool bRes=  false;
	if(m_pIConfigure)
	{
		TSTRING strValue;
		TSTRING::size_type nCapacitySize = strValue.capacity( );
		nCapacitySize = strValue.capacity( );
		int nMaxSize = 1024;
		while(m_pIConfigure->GetCfgVar(szName,szDefaultValue,strValue) == false && nCapacitySize <= nMaxSize)
		{
			//For add buffer
			strValue += _T(" ");
			nCapacitySize = strValue.capacity( );
		}
		if(nCapacitySize <= nMaxSize)
		{
			_tcscpy(szValue,strValue.c_str());
			bRes = true;
		}
	}
	return bRes;
}

bool CConfigureToolBase::GetValue(LPCTSTR szName,int& nValue,int nDefaultValue)
{
	bool bRes=  false;
	if(m_pIConfigure)
	{
		nValue = m_pIConfigure->GetCfgVar(szName,nDefaultValue);
		bRes = true;
	}
	return bRes;
}

int CConfigureToolBase::GetCount(const TSTRING& strName)
{
	int nCount = -1;
	TSTRING strCountName = strName + _T("_Count");
	bool bRes = GetValue(strCountName.c_str(),nCount,nCount);
	assert(bRes);
	return nCount;
}

int CConfigureToolBase::GetValueByIndex(const TSTRING& strName,int nIndex)
{
	int nImageID = -1;

	TCHAR szName[1024] = {0};
	sprintf(szName,_T("%s_%02d"),strName.c_str(),nIndex);
	bool bRes = GetValue(szName,nImageID,nImageID);
	assert(bRes);
	return nImageID;
}


bool CConfigureToolBase::SetValue(LPCTSTR szName,LPCTSTR szValue)
{
	bool bRes = false;
	if(m_pIConfigure)
		bRes = m_pIConfigure->SetCfgVar(szName, szValue);
	return bRes;
}
bool CConfigureToolBase::SetValue(LPCTSTR szName,int nValue)
{
	bool bRes = false;
	if(m_pIConfigure)
		bRes = m_pIConfigure->SetCfgVar(szName, nValue);
	return bRes;
}

bool CConfigureToolBase::SetValueByIndex(const TSTRING& strName, int nIndex, int nValue)
{
	bool bRes = false;
	TCHAR szName[1024] = {0};
	sprintf(szName,_T("%s_%02d"),strName.c_str(),nIndex);
	bRes = SetValue(szName, nValue);
	return bRes;
}
bool CConfigureToolBase::SetStringValueByIndex(const TSTRING& strName, int nIndex, const TSTRING& strValue)
{
	bool bRes = false;
	TCHAR szName[1024] = {0};
	sprintf(szName,_T("%s_%02d"),strName.c_str(),nIndex);
	bRes = SetValue(szName, strValue.c_str());
	return bRes;
}