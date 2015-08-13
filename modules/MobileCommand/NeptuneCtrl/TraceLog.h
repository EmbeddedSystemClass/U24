#ifndef _TRACE_LOG_H
#define _TRACE_LOG_H

#define TRACER_DEFAULT_FILE		_T("_TraceLog.txt")
#define TRACER_MAX_BUFFER		1024
#define TRACER_NUMSIZE			24
#define TRACER_MAX_FILESIZE		1024*1024 // 1 MB

class CTraceLog  
{
public:
	CTraceLog();
	virtual ~CTraceLog();
	void Initialize(LPCTSTR pszFile);
	void Print(LPCTSTR szFormat, ...);
	void Print(LPCTSTR szType, LPCTSTR szFormat, ...);

	CTraceLog operator<<(int nValue);
	CTraceLog operator<<(unsigned int unValue);
	CTraceLog operator<<(short sValue);
	CTraceLog operator<<(float fValue);
	CTraceLog operator<<(double dfValue);
	CTraceLog operator<<(bool bValue);
	CTraceLog operator<<(char cValue);
	CTraceLog operator<<(unsigned char byValue);
	CTraceLog operator<<(wchar_t wcValue);
	CTraceLog operator<<(char * pszValue);
	CTraceLog operator<<(wchar_t * pszValue);

private:
	static CTraceLog* m_pTraceLog;
public:
	static void Release();
	static CTraceLog* GetTracer();

private:
	TCHAR m_szFile[MAX_PATH];
	bool m_isLogOn;
	void LogFile(LPCTSTR szData);
};


class CTraceFunction
{
private:
	TCHAR m_szFunction[TRACER_MAX_BUFFER];
public:
	CTraceFunction(LPCTSTR szFunction);
	~CTraceFunction();
};

#define TRACER (*(CTraceLog::GetTracer()))

#ifdef TRACER_ENABLED
	#define TRACER_INIT(FilePath)	TRACER.Initialize(FilePath);
	#define TRACER_FUNCTION_IN()	CTraceFunction trace_func(_T(__FUNCTION__));
	#define TRACER_STREAM(arg)		( TRACER << arg );
	#define TRACER_EXIT()			TRACER.Release();
#else
	#define TRACER_INIT(FilePath)
	#define TRACER_FUNCTION_IN()
	#define TRACER_STREAM(arg)
#endif // TRACER_ENABLED

#endif // _TRACE_LOG_H
