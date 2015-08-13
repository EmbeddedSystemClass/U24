/***************************************************************************************
* Copyright (C) : QCS/WGC18                                      
* Module name	: LOG  
* File name     : GLog.h             
* Summary       : Unify DL dll.                                                  
* Version		Author             Date                Abstract                      
* 1.0           Alex.Chen          2011/07/15          First version  
****************************************************************************************/

#pragma once

#include "Shlwapi.h"
#include "..\Common\Glog\src\windows\glog\logging.h"

const int __INFO = 0, __WARNING = 1, __ERROR = 2, __FATAL = 3, NUM__SEVERITIES = 4;

#define  DEFAULT_LOG_DIR		"D:\\LOG\\DLL\\"

class CGLog
{
public:
	CGLog();
	~CGLog();

public:
	void InitializeGLog(const char* sz_logPath);
	void SetGLogFileName_Info(const char* sz_logName);
	void SetGLogFileName_Error(const char* sz_logName);
	void SetGLogFileName_Waring(const char* sz_logName);
	void Log(const int i_severity, const char* sz_logMessage);
	void ShutdownGLog(void);

private:
	bool CreateLogDirectory(CString str_directoryName);
	bool SpiltString(CString str_srcString, CString str_delimiter, CStringArray& stra_strArg);

private:
	CString  m_str_logPath;
	bool     m_b_init;

};
