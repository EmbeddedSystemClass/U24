#ifndef __INI_ACCESS__H__
#define __INI_ACCESS__H__

#pragma once

#include <string>
#include "..\QisdaEmmcDownloadModule\XML\MyMSXML.h"
#include "io.h"

class CIniAccess
{
public:
	CIniAccess (const char* chSection, const char* chFilename);
	bool GetValue(const char* strKey, char* strValue, DWORD dwSize);
	bool GetValue(const char* strKey, LPCTSTR strValue);
	int  GetValue(const char* strKey, int nDefaultValue = 0);
	bool GetToolVersion(const char* strKey, LPCTSTR strValue,  int nDefaultValue = 0);

	CMyMSXML m_ToolVersionXML;
	std::string m_strToolVersion;

	/*
	~CIniAccess(void);
	CIniAccess (const string& strSection, const string& strIniFile = szSetupIni, bool bUseCurrentFolder = true);
	bool GetValue(const string& strKey, string& strValue);
	int  GetValue(const string& strKey, int nDefaultValue = 0);
	bool GetValue(const string& strKey, vector<string>& vValue, bool bSpaceOn = true);
	bool SetValue(const string& strKey, const string& strValue);
	bool SetValue(const string& strKey, int nValue);
	bool SetValue(const string& strKey, const vector<string>& vItems);

	bool IsIniExist(){ return m_bIniExist; }
	void SwitchSection(const string& strSection){ m_strSection = strSection; }
	void GetCurrentSection(string& strSection){ strSection = m_strSection; }
	*/

private:
	char m_strSection [MAX_PATH];
	char m_strIniPath [MAX_PATH];
	//bool   m_bIniExist;
};

#endif //__INI_ACCESS__H__