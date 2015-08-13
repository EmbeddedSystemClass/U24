#pragma once

#include "LogPolicy.h"
#include "Cloud.h"
#include <Tchar.h>

#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#else
#define TSTRING std::string
#endif
#endif


static LPCTSTR g_szCFCErrorCodeTitle = _T("CFC_Error_");

class CErrorCode
{
public:
	CErrorCode(){}
	virtual ~CErrorCode(){}

	static const TSTRING& GetErrorString(const TSTRING& strModule, int nErrorCode)
	{
		static TSTRING strErrorMsg;
		TSTRING strIniPath = _T(".\\ErrorCode.ini");

		if(::_taccess_s(strIniPath.c_str(),0) == 0)
		{
			TCHAR cpErrorCode[10] = {0};
			_itot_s(nErrorCode, cpErrorCode, 10);

			TCHAR cpErrorMsg[200] ={0};
			DWORD lRetValue = ::GetPrivateProfileString(strModule.c_str(),cpErrorCode,NULL,cpErrorMsg, 200, strIniPath.c_str());
			strErrorMsg = cpErrorMsg;
		}
		return strErrorMsg;
	}
};

class CLastError
{
public:
	CLastError(const TSTRING& strDevice)
		:m_strInvaildDevice(_T("InvaildDevice"))
		,m_strKey(g_szCFCErrorCodeTitle + (strDevice.size() > 5 ? strDevice.substr(4) : m_strInvaildDevice))
	{
	}
	virtual ~CLastError(){}
public:
	void SetCFCLastError(int nLastError)
	{
		CStringA cstrError;
		cstrError.Format("%d", nLastError);
		CCloud::Put(key(), cstrError.GetLength(), cstrError.GetBuffer());
		cstrError.ReleaseBuffer();
	}

	int GetCFCLastError()
	{
		char szError[10] = {0};
		int nSize = 0;
		FILETIME time;
		CCloud::Get(key(), nSize, szError, time);
		return atoi(szError);
	}

private:
	const TSTRING& key() const
	{
		return m_strKey;
	}
	const TSTRING m_strInvaildDevice;
	const TSTRING m_strKey;
};



/************************************************************************
Class: CLog
Purpose: For everyone adding their Log easily
Instruction:
1. inherit public CLog
2. add your own Log function which should insert DISABLE LOG Flag
3. (If your don't want to log in COMxx.txt, please set your own FileName inside your function)
************************************************************************/
class CLog
{
public:
	CLog(const TSTRING& strDevice)
		:m_strDevice(strDevice)
	{
		if(m_strDevice.size() > 5)
		{
			m_cstrFileName = m_strDevice.substr(4).c_str();
			m_cstrFileName += _T(".txt");
		}
	}
	virtual ~CLog(){}
public:
	void AddDLErrorLog(const TSTRING& strLog)
	{
#ifndef DISABLE_DL_LOG
		LogPolicy::FunctionLog(strLog.c_str(), _T("DL_")+m_cstrFileName);
#endif
	}
	void AddNVItemLog(const TSTRING& strLog)
	{
#ifndef DISABLE_NVITEM_LOG
		LogPolicy::FunctionLog(strLog.c_str(), _T("NVItem_")+m_cstrFileName);
#endif
	}

	void AddDLLog(bool bSuccess, const CString& strMessage,const CString& strFileName)
	{
#ifdef DownLoad_LOG
		if(m_strDevice.size() > 5)
		{
			CString strTmp = strMessage;
			strTmp += bSuccess ? _T(" Success") : _T(" Fail");

			CString strTitle;
			strTitle.Format(_T("%s.txt"), m_strDevice.substr(4),c_str());
			LogPolicy::Log(strTmp, strFileName + strTitle);
		}
#endif
	}

	void AddDLLog(const CString& strMessage,const CString& strFileName)
	{
#ifdef DownLoad_LOG		
		if(m_strDevice.size() > 5)
		{
			CString strTitle;
			strTitle.Format(_T("%s.txt"), m_strDevice.substr(4),c_str());
			LogPolicy::Log(strMessage, strFileName + strTitle);
		}
#endif
	}
private:
	CString m_cstrFileName;
	const TSTRING& m_strDevice;
};