#include "StdAfx.h"
#include "IniAccess.h"
#include <sstream>
#include "Winbase.h"
#include "PIMStringUtility.h"

#define MAX_PATH          260

CIniAccess::CIniAccess():
			m_strSection(""),
			m_strIniPath(""),
			m_bIniExist(false)
#ifdef _UNICODE
			, m_wstrSection(_T("")),
			  m_wstrIniPath(_T(""))
#endif
{}

CIniAccess::~CIniAccess(void)
{}


//Unicode
#ifdef _UNICODE

CIniAccess::CIniAccess(const wstring& wstrSection, const wstring& wstrIniFile, bool bUseCurrentFolder)
{
	this->LoadIni(wstrSection, wstrIniFile, bUseCurrentFolder);
}

void CIniAccess::LoadIni(const wstring& wstrSection, const wstring& wstrIniFile, bool bUseCurrentFolder)
{
	this->m_strSection = "";
	this->m_strIniPath = "";

	this->m_wstrSection = wstrSection;
	this->m_bIniExist = false;

	if(bUseCurrentFolder){
		WCHAR szTemp[MAX_PATH] = {0};
		GetModuleFileName(NULL, szTemp, MAX_PATH);
		PathRemoveFileSpec(szTemp);
		this->m_wstrIniPath = szTemp;
		this->m_wstrIniPath += _T("\\") + wstrIniFile;
	}
	else{
		this->m_wstrIniPath = wstrIniFile;
	}

	if(::_taccess(this->m_wstrIniPath.c_str(), 0) == 0)
		this->m_bIniExist = true;
}

bool CIniAccess::GetValue(const wstring& wstrKey, wstring& wstrValue, wstring wstrDefault)
{
	bool bRes = false;
	if(this->m_bIniExist)
	{
		WCHAR szTemp[4096] = {0};
		DWORD nSize = ::GetPrivateProfileString(this->m_wstrSection.c_str(), 
							wstrKey.c_str(), 
							wstrDefault.c_str(), 
							szTemp, 
							sizeof(szTemp), 
							this->m_wstrIniPath.c_str());
		if(nSize!=0)
		{
			wstrValue = szTemp;
			bRes = true;
		}
	}
	return bRes;
}
int CIniAccess::GetValue(const wstring& wstrKey, int nDefaultValue)
{
	int nValue = nDefaultValue;
	if(this->m_bIniExist)
	{
		nValue = ::GetPrivateProfileInt(this->m_wstrSection.c_str(), 
						wstrKey.c_str(), 
						nDefaultValue, 
						this->m_wstrIniPath.c_str());
	}
	return nValue;
}
bool CIniAccess::GetValue(const wstring& wstrKey, vector<wstring>& vValue, bool bSpaceOn)
{
	bool bRes = false;
	wstring wstrValue;
	if(this->GetValue(wstrKey, wstrValue))
	{
		bRes = QSYNC_LIB::Tokenize<wstring>(_T(";"), wstrValue, vValue, bSpaceOn);
	}
	return bRes;
}
bool CIniAccess::SetValue(const wstring& wstrKey, const wstring& wstrValue)
{
	bool bRes = false;
	if(this->m_bIniExist)
	{
		bRes = TRUE == ::WritePrivateProfileString(this->m_wstrSection.c_str(), 
							   wstrKey.c_str(), 
							   wstrValue.c_str(), 
							   this->m_wstrIniPath.c_str());			
	}
	return bRes;
}
bool CIniAccess::SetValue(const wstring& wstrKey, int nValue)
{
	bool bRes = false;
	if(this->m_bIniExist)
	{
		wstring wstrValue;
		wstringstream wssValue(wstrValue);
		wssValue << nValue;

		bRes = this->SetValue(wstrKey, wssValue.str());
	}
	return bRes;
}

bool CIniAccess::SetValue(const wstring& wstrKey, const vector<wstring>& vItems)
{
	bool bRes = false;
	if(this->m_bIniExist)
	{
		wstring wstrValue;
		for(vector<wstring>::const_iterator iter= vItems.begin(); iter != vItems.end(); iter++)
		{
			if(iter != vItems.begin())
				wstrValue += _T(";");
			wstrValue += (*iter);
		}
		bRes = SetValue(wstrKey, wstrValue);
	}
	return bRes;
}
#else

//ANSI
CIniAccess::CIniAccess(const string& strSection, const string& strIniFile, bool bUseCurrentFolder)
{
	this->LoadIni(strSection, strIniFile, bUseCurrentFolder);
}

void CIniAccess::LoadIni(const string& strSection, const string& strIniFile, bool bUseCurrentFolder)
{
#ifdef _UNICODE
	this->m_wstrSection = _T("");
	this->m_wstrIniPath = _T("");
#endif
	this->m_strSection = strSection;
	this->m_bIniExist = false;

	if(bUseCurrentFolder){
		char szTemp[MAX_PATH] = {0};
		GetModuleFileNameA(NULL, szTemp, MAX_PATH);
		PathRemoveFileSpecA(szTemp);
		this->m_strIniPath = szTemp;
		this->m_strIniPath += "\\" + strIniFile;
	}
	else{
		this->m_strIniPath = strIniFile;
	}

	if(_access(this->m_strIniPath.c_str(), 0) == 0)
		this->m_bIniExist = true;
}

bool CIniAccess::GetValue(const string& strKey, string& strValue, string strDefault)
{
	bool bRes = false;
	if(this->m_bIniExist)
	{
		char szTemp[4096] = {0};
		DWORD nSize = ::GetPrivateProfileStringA(this->m_strSection.c_str(), 
							 strKey.c_str(), 
							 strDefault.c_str(), 
							 szTemp, 
							 sizeof(szTemp), 
							 this->m_strIniPath.c_str());
		if(nSize!=0)
		{
			strValue = szTemp;
			bRes = true;
		}
	}
	return bRes;
}
int CIniAccess::GetValue(const string& strKey, int nDefaultValue)
{
	int nValue = nDefaultValue;
	if(m_bIniExist)
	{
		nValue = ::GetPrivateProfileIntA(this->m_strSection.c_str(), 
						 strKey.c_str(), 
						 nDefaultValue, 
						 this->m_strIniPath.c_str());
	}
	return nValue;
}
bool CIniAccess::GetValue(const string& strKey, vector<string>& vValue, bool bSpaceOn)
{
	bool bRes = false;
	string strValue;
	if(this->GetValue(strKey, strValue))
	{
		bRes = QSYNC_LIB::TokenizeA(";", strValue, vValue, bSpaceOn);
	}
	return bRes;
}
bool CIniAccess::SetValue(const string& strKey, const string& strValue)
{
	bool bRes = false;
	if(this->m_bIniExist)
	{
		bRes = TRUE == ::WritePrivateProfileStringA(this->m_strSection.c_str(), 
							    strKey.c_str(), 
							    strValue.c_str(), 
							    this->m_strIniPath.c_str());			
	}
	return bRes;
}
bool CIniAccess::SetValue(const string& strKey, int nValue)
{
	bool bRes = false;
	if(this->m_bIniExist)
	{
		std::string strValue;
		std::stringstream ssValue(strValue);
		ssValue << nValue;

		bRes = SetValue(strKey, ssValue.str());
	}
	return bRes;
}

bool CIniAccess::SetValue(const string& strKey, const vector<string>& vItems)
{
	bool bRes = false;
	if(this->m_bIniExist)
	{
		string strValue;
		for(vector<string>::const_iterator iter= vItems.begin(); iter != vItems.end(); iter++)
		{
			if(iter != vItems.begin())
				strValue += ";";
			strValue += (*iter);
		}
		bRes = SetValue(strKey, strValue);
	}
	return bRes;
}


#endif