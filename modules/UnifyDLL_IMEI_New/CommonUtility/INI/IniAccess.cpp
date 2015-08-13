
// IniAccess.cpp : implementation file
//

#include "stdafx.h"
#include "Winbase.h"
#include <Shlwapi.h>
#include <tchar.h>
#include <wchar.h>
#include <io.h>


#include "IniAccess.h"


#define MAX_PATH 260


CIniAccess::CIniAccess(const string& strSection, const string& strIniFile)
{
	m_strSection = strSection;
	m_bIniExist = false;

	if (PathIsRelative (strIniFile.c_str()))
	{
		char szTemp[MAX_PATH] = {0};
		GetModuleFileName(NULL, szTemp, MAX_PATH);
		PathRemoveFileSpec(szTemp);
		m_strIniPath = szTemp;
		m_strIniPath += "\\" + strIniFile;
	}
	else
	{
		m_strIniPath = strIniFile;
	}

	if (PathFileExists(m_strIniPath.c_str()))
		m_bIniExist = true;
		
}

CIniAccess::~CIniAccess(void)
{}

bool CIniAccess::GetValue(const string& strKey, string& strValue)
{
	if (m_bIniExist)
	{
		char szTemp[4096] = {0};
		DWORD nSize = ::GetPrivateProfileStringA(m_strSection.c_str(), strKey.c_str(), "", szTemp, sizeof(szTemp), m_strIniPath.c_str());
		if (nSize != 0)
		{
			strValue = szTemp;
			return true;
		}
	}

	return false;
}

bool CIniAccess::SetValue(const string& strKey, const string& strValue)
{
	if (m_bIniExist)
	{
		if (::WritePrivateProfileString(m_strSection.c_str(), strKey.c_str(), strValue.c_str(), m_strIniPath.c_str()) == TRUE)
			return true;
	}

	return false;
}
