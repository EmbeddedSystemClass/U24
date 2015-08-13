#ifndef __CGLOG_H__
#define __CGLOG_H__

#include "..\gLog\src\windows\glog\logging.h"

const int __INFO = 0, __WARNING = 1, __ERROR = 2, __FATAL = 3, NUM__SEVERITIES = 4;

#define DEFAULT_LOG_DIR              "LOG\\"

class CGLog
{
public:
	CGLog();
	~CGLog();
	void InitializeGLog(const char *szLogPath);
	void SetGLogDestination_Error(const char *szErrLogName);
	void SetGLogDestination_Warning(const char *szErrLogName);
	void SetGLogDestination_Info(const char *szErrLogName);
	void SetGLogDestination_All(const char *szErrLogName, const char *szWarLogName, const char *szInfoLogName);
	void Log(const int Severity, const char *szLogMsg);
	void ShutdownGLog();
private:
	std::string m_strGLogPath;
	static bool m_Initialized;
};

#endif