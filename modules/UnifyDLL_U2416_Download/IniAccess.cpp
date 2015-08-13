#include "stdafx.h"
#include "IniAccess.h"

CIniAccess::CIniAccess (const char* chSection, const char* chFilename)
{
	strcpy (m_strSection, chSection);
	strcpy (m_strIniPath, chFilename);

}

bool CIniAccess::GetValue(const char* strKey, char* strValue, DWORD dwSize)
{
	return GetPrivateProfileString (m_strSection, strKey, "", strValue, dwSize, m_strIniPath) > 0;
}

bool CIniAccess::GetValue(const char* strKey, LPCTSTR strValue)
{
	char chTemp[4096];
	bool ret = GetValue (strKey, chTemp, 4096);
	strcpy ((char*)strValue, chTemp);
	return ret;
}

int CIniAccess::GetValue(const char* strKey, int nDefaultValue)
{
	return GetPrivateProfileInt (m_strSection, strKey, nDefaultValue, m_strIniPath);
}

bool CIniAccess::GetToolVersion(const char* strKey, LPCTSTR strValue,  int nDefaultValue )
{
	

	if (_taccess(m_strIniPath, 0) != 0) 
	{
		return false;
	
	}

	if (m_ToolVersionXML.Load (m_strIniPath) != ERROR_SUCCESS)
	{
		return false;
	}
	XMLNode XMLNode_ToolVersionXML = m_ToolVersionXML.SearchNode(_T("//Configuration//ToolInfo//TOOL_VERSION"));
	CString cstrTestProcessor = m_ToolVersionXML.GetNodeText(XMLNode_ToolVersionXML);
	strcpy ((char*)strValue, cstrTestProcessor.GetBuffer(0));
	cstrTestProcessor.ReleaseBuffer();


	return true;
}