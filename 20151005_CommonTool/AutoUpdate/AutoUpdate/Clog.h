/******************************************************************************
* Copyright (C) QCS/WGC18                                      
* Module name : Entrance Tool   
* File name   : Clog.h             
* Summary     : Write log                                                              
* Version      Author            Date                 Abstract                      
* 1.0          Tom Li         2011/06/30           First version                        
*****************************************************************************/

#pragma once

#include "atltime.h"

/*============================== Const ======================================*/
#define NAMEDPIPE               _T("\\\\.\\Pipe\\AutoUpdateTool")
/*============================== Const ======================================*/

class Clog
{
public:
	Clog(void);
	virtual ~Clog(void);

public:
	bool WriteLog (CString str_logContent, TCHAR* sz_ErrCode=NULL, TCHAR* sz_ErrMsg=NULL);
	void Setpath(CString& str_path);
	CString GetcurrentTime(void);
	void LogDispose_syslog(CString str_logContent);
	bool OpenFile (TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);

private:
	bool CheckFileExist(CString str_path);
	

private:
	TCHAR m_sz_path[MAX_PATH];
	CString m_str_LogName;
};
