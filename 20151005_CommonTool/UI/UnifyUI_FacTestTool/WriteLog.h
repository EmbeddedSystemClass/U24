/******************************************************************************
* Copyright (C) : QCS/WGC18                                      
* Module name	: Log
* File name     : WriteLog.h             
* Summary       : By pipe to write log.                                                        
* Version       Author             Date                 Abstract                      
* 1.0           Alex.Chen          2011/09/01           First version                           
*****************************************************************************/

#pragma once

/*================================= Macro =================================*/
#ifdef FACTORY_MODE
#define LOG_PATH              _T("D:\\Log\\UI")                  // Log directory
#else
#define LOG_PATH              _T("D:\\Log\\UI_RD")                  // Log directory
#endif

#define LOG_LINEMAX           10000                         // Log max line
#define NAMEDPIPE             _T("\\\\.\\Pipe\\WriteLog")   // Pipe name
#define BUFFER_SIZE           4096
/*================================= Macro =================================*/


/* ============================= Structure ================================*/
struct LogFile 
{
	CStdioFile obj_file;        // File handle
	CString    str_logFileName; // File name
	int        i_line;          // File Line
	bool       b_openFlag;      // File open status(true: opened)
};
/* ============================= Structure ================================*/


class CWriteLog
{
public:
	CWriteLog(void);
	virtual ~CWriteLog(void);

public:
	
	void SaveLog(CString str_logContent);
	void SetLogPath(CString str_logpath);
private:
	bool OpenLogFile(void);
	void WriteLogFile(CString str_logContent);
	bool SendLog(CString str_logContent);
	static UINT StartupLogServer(LPVOID lpParam);
	bool SetLogFileName(CString str_logFileName);
private:
	static bool      m_b_threadFlag;
	static CString   m_str_logPath;
	static int		 m_i_today;
	
private:	
	CString          m_str_logFileName;
	LogFile			 m_st_logFile;
	

};
