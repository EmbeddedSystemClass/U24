/**
 * COPYRIGHT QCS LIMITED 2010
 *
 * Description: Write log class 
 * Version    : $Revision:   1.1  $
 * Author     : QCS/WGC18/Alex
 * Time       : 2010/06/07
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2010/06/07     Alex              princeps edition.
 * 
 */

#pragma once

#define LOG_FILEPATH            ".\\LOG"
#define LOG_FILENAME		    "SDCardCopy"                // the file name of log file
#define TRY_COUNT				10                          // the circle number 
#define NAMEDPIPE               "\\\\.\\Pipe\\SDCardCopy"   // the pipe correspond
#define MAXSIZE                 1024
#define OK                       1
#define NG                       0

class LogDispose  
{
private:
	 CFile obj_file;
public:
	static UINT LogDispose_startupLogServer(LPVOID lpParam);
	void LogDispose_syslog(CString str_logContent);
	LogDispose();
	virtual ~LogDispose();

private:
	void LogDispose_init();
	void LogDispose_writeLog(CString str_logInfo);

};

