
// IniAccess.h : header file
//

#ifndef _INI_ACCESS_H_
#define _INI_ACCESS_H_


#pragma once


#include <string>


using namespace std;


const char szSetupIni[] = "Qisda\\FT_Wireless_Instance.ini";


class CIniAccess
{
// Member functions
public:
	CIniAccess(const string& strSection, const string& strIniFile = szSetupIni);
	~CIniAccess(void);

	bool GetValue(const string& strKey, string& strValue);
	bool SetValue(const string& strKey, const string& strValue);

	bool IsIniExist() { return m_bIniExist; }
	void SwitchSection(const string& strSection) { m_strSection = strSection; }
	void GetCurrentSection(string& strSection) { strSection = m_strSection; }

protected:
private:

// Data member
public:
protected:
private:
	string m_strSection;
	string m_strIniPath;
	bool m_bIniExist;
};


#endif // End of #ifndef _INI_ACCESS_H_
