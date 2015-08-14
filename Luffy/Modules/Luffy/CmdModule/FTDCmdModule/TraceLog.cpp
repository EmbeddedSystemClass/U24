#include "stdafx.h"
#include "TraceLog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CTraceLog* CTraceLog::m_pTraceLog = CTraceLog::GetTracer();

CTraceFunction::CTraceFunction(LPCTSTR szFunction)
{
	_tcscpy(m_szFunction, szFunction);
	TCHAR szBuffer[2*TRACER_MAX_BUFFER];
	_stprintf(szBuffer, _T("+%s"), m_szFunction);
	TRACER.Print(szBuffer);
}

CTraceFunction::~CTraceFunction()
{
	TCHAR szBuffer[2*TRACER_MAX_BUFFER];
	_stprintf(szBuffer, _T("-%s"), m_szFunction);
	TRACER.Print(szBuffer);
}

CTraceLog::CTraceLog()
: m_isLogOn(false)
{
	_tcscpy(m_szFile, TRACER_DEFAULT_FILE);
}

CTraceLog::~CTraceLog()
{
}

void CTraceLog::Initialize(LPCTSTR pszFile)
{
	m_isLogOn = true;
	if (NULL != pszFile) {
		_tcscpy(m_szFile, pszFile);
	}
}

void CTraceLog::Release()
{
	delete m_pTraceLog;
	m_pTraceLog = NULL;
}

CTraceLog* CTraceLog::GetTracer()
{
	if (NULL == m_pTraceLog) {
		m_pTraceLog = new CTraceLog();
	}
	return m_pTraceLog;
}

void CTraceLog::Print(LPCTSTR szType, LPCTSTR szFormat, ...)
{
	if (!m_isLogOn) {
		return;
	}

	TCHAR szBuffer[8*TRACER_MAX_BUFFER] = {0};
	va_list args;
	va_start(args, szFormat);
	_vsntprintf(szBuffer, sizeof(szBuffer), szFormat, args);
	va_end(args);

	TCHAR szLog[3*TRACER_MAX_BUFFER];
	_stprintf(szLog, _T("%s%s\r\n"), szType, szBuffer);
	LogFile(szLog);
}

void CTraceLog::Print(LPCTSTR szFormat, ...)
{
	if (!m_isLogOn) {
		return;
	}

	TCHAR Buffer[8*TRACER_MAX_BUFFER] = {0};
	va_list args;
	va_start(args, szFormat);
	_vsntprintf(Buffer, sizeof(Buffer), szFormat, args);
	va_end(args);

	_tcscat(Buffer, _T("\r\n"));
	LogFile(Buffer);
}

void CTraceLog::LogFile(LPCTSTR szData)
{
	if (!m_isLogOn) {
		return;
	}

	HANDLE hLog = INVALID_HANDLE_VALUE;
	while (1) { //// To control logging size;
		hLog = CreateFile(m_szFile, GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hLog) {
			return;
		}

		DWORD nSize = GetFileSize(hLog, NULL);
		if (nSize > TRACER_MAX_FILESIZE) { //// Size Checking.
			CloseHandle(hLog);
			DeleteFile(m_szFile);
			continue;
		}
		break;
	}

	SetFilePointer(hLog, 0, NULL, FILE_END);
	size_t nBytesWrite = _tcslen(szData) * sizeof(TCHAR);
	DWORD nBytesWritten = 0;
	WriteFile(hLog, szData, static_cast<DWORD>(nBytesWrite), &nBytesWritten, NULL);
	if (INVALID_HANDLE_VALUE != hLog) {
		CloseHandle(hLog);
	}
}

CTraceLog CTraceLog::operator<<(int nValue)
{
	if (m_isLogOn) {
		TCHAR szLog[TRACER_NUMSIZE] = {0};
		_stprintf(szLog, _T("%d"), nValue);
		LogFile(szLog);
	}

	return (*this);
}

CTraceLog CTraceLog::operator<<(unsigned int unValue)
{
	if (m_isLogOn) {
		TCHAR szLog[TRACER_NUMSIZE] = {0};
		_stprintf(szLog, _T("%u"), unValue);
		LogFile(szLog);
	}

	return (*this);
}

CTraceLog CTraceLog::operator<<(unsigned long unValue)
{
	if (m_isLogOn) {
		TCHAR szLog[TRACER_NUMSIZE] = {0};
		_stprintf(szLog, _T("%lu"), unValue);
		LogFile(szLog);
	}

	return (*this);
}

CTraceLog CTraceLog::operator<<(short sValue)
{
	if (m_isLogOn) {
		TCHAR szLog[TRACER_NUMSIZE] = {0};
		_stprintf(szLog, _T("%s"), sValue);
		LogFile(szLog);
	}
	return (*this);
}

CTraceLog CTraceLog::operator<<(float fValue)
{
	if (m_isLogOn) {
		TCHAR szLog[TRACER_NUMSIZE] = {0};
		_stprintf(szLog, _T("%f"), fValue);
		LogFile(szLog);
	}

	return (*this);
}

CTraceLog CTraceLog::operator<<(double dfValue)
{
	if (m_isLogOn) {
		TCHAR szLog[TRACER_NUMSIZE] = {0};
		_stprintf(szLog, _T("%f"), dfValue);
		LogFile(szLog);
	}
	return (*this);
}

CTraceLog CTraceLog::operator<<(bool bValue)
{
	if (m_isLogOn) {
		TCHAR szLog[TRACER_NUMSIZE] = {0};
		_stprintf(szLog, _T("%d"), bValue);
		LogFile(szLog);
	}
	return (*this);
}

CTraceLog CTraceLog::operator<<(unsigned char byValue)
{
	if (m_isLogOn) {
		TCHAR szLog[TRACER_NUMSIZE] = {0};
		_stprintf(szLog, _T("%d"), byValue);
		LogFile(szLog);
	}
	return (*this);
}

CTraceLog CTraceLog::operator<<(char cValue)
{
	if (m_isLogOn) {
		TCHAR szLog[TRACER_NUMSIZE] = {0};
		_stprintf(szLog, _T("%c"), cValue);
		LogFile(szLog);
	}
	return (*this);
}

CTraceLog CTraceLog::operator<<(wchar_t wcValue)
{
	if (m_isLogOn) {
		TCHAR szLog[TRACER_NUMSIZE] = {0};
		_stprintf(szLog, _T("%c"), wcValue);
		LogFile(szLog);
	}
	return (*this);
}

CTraceLog CTraceLog::operator<<(char* pszValue)
{
	if (m_isLogOn) {
		if ( !pszValue ) return (*this);

		TCHAR szLog[TRACER_MAX_BUFFER * 2] = {0};
		TCHAR* pLodAddr = (TCHAR*)pszValue;

#if defined (UNICODE) || defined (_UNICODE)
		wchar_t szSource[TRACER_MAX_BUFFER * 2] = {0};
		mbstowcs(szSource, pszValue, strlen(pszValue));
		pLodAddr = (TCHAR*)szSource;
#endif	// (_UNICODE || UNICODE)
		_stprintf(szLog, _T("%s"), pLodAddr);
		LogFile(szLog);
	}

	return (*this);
}

CTraceLog CTraceLog::operator<<(wchar_t *pszValue)
{
	if (m_isLogOn) {
		if (!pszValue) {
			return (*this);
		}

		TCHAR szLog[TRACER_MAX_BUFFER * 2] = {0};
		char  szSource[TRACER_MAX_BUFFER * 2] = {0};
		TCHAR* pLodAddr = (TCHAR*)pszValue;
#if !defined (UNICODE) && !defined (_UNICODE)
		wcstombs(szSource, pszValue, wcslen(pszValue));
		pLodAddr = (TCHAR*)szSource;
#endif	// (_UNICODE || UNICODE)

		_stprintf(szLog, _T("%s"), pLodAddr);
		LogFile(szLog);
	}

	return (*this);
}

