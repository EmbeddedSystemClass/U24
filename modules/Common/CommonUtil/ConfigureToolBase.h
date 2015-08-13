#pragma once

#include "..\Components\ConfigureTool\IConfigure.h"

using namespace QSYNC_LIB;

class CConfigureToolBase
{
public:
	bool IsWork() const {return m_pIConfigure != NULL;}
protected:
	CConfigureToolBase(LPCTSTR szConfigureTitle, LPCTSTR szFolderName);
	virtual ~CConfigureToolBase();
	bool GetValue(LPCTSTR szName,LPTSTR szValue,LPCTSTR szDefaultValue);
	bool GetValue(LPCTSTR szName,int& nValue,int nDefaultValue);
	int GetCount(const TSTRING& strName);
	int GetValueByIndex(const TSTRING& strName,int nIndex);
	TSTRING GetStringValueByIndex(const TSTRING& strName,int nIndex);

	bool SetValue(LPCTSTR szName,LPCTSTR szValue);
	bool SetValue(LPCTSTR szName,int nValue);
	bool SetValueByIndex(const TSTRING& strName, int nIndex, int nValue);
	bool SetStringValueByIndex(const TSTRING& strName, int nIndex, const TSTRING& strValue);
private:
	IConfigure *m_pIConfigure;
	TSTRING m_strConfigureTitle;
};
