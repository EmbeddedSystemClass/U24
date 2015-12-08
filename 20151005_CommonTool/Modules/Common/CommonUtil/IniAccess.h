#ifndef __INI_ACCESS__H__
#define __INI_ACCESS__H__

#pragma once

using namespace std;

#include <vector>
#include <string>



class CIniAccess
{
public:	
	CIniAccess();
	~CIniAccess(void);
	
	//ANSI
	CIniAccess(const string& strSection, const string& strIniFile, bool bUseCurrentFolder = true);
	bool GetValue(const string& strKey, string& strValue, string strDefault = "");
	int  GetValue(const string& strKey, int nDefaultValue = 0);
	bool GetValue(const string& strKey, vector<string>& vValue, bool bSpaceOn = true);
	bool SetValue(const string& strKey, const string& strValue);
	bool SetValue(const string& strKey, int nValue);
	bool SetValue(const string& strKey, const vector<string>& vItems);
	void LoadIni(const string& strSection, const string& strIniFile, bool bUseCurrentFolder = true);
	void SwitchSection(const string& strSection){ this->m_strSection = strSection; }
	void GetCurrentSection(string& strSection){ strSection = this->m_strSection; }

#ifdef _UNICODE
	//Unicode
	CIniAccess(const wstring& wstrSection, const wstring& wstrIniFile, bool bUseCurrentFolder = true);
	bool GetValue(const wstring& wstrKey, wstring& wstrValue, wstring wstrDefault = _T(""));
	int  GetValue(const wstring& wstrKey, int nDefaultValue = 0);
	bool GetValue(const wstring& wstrKey, vector<wstring>& vValue, bool bSpaceOn = true);
	bool SetValue(const wstring& wstrKey, const wstring& wstrValue);
	bool SetValue(const wstring& wstrKey, int nValue);
	bool SetValue(const wstring& wstrKey, const vector<wstring>& vItems);
	void LoadIni(const wstring& wstrSection, const wstring& wstrIniFile, bool bUseCurrentFolder = true);
	void SwitchSection(const wstring& wstrSection){ this->m_wstrSection = wstrSection; }
	void GetCurrentSection(wstring& wstrSection){ wstrSection = this->m_wstrSection; }
#endif
	
	bool IsIniExist(){ return m_bIniExist; }	
private:
	string m_strSection;
	string m_strIniPath;
	
#ifdef _UNICODE
	wstring m_wstrSection;
	wstring m_wstrIniPath;
#endif
	bool   m_bIniExist;
};

#endif //__INI_ACCESS__H__