#ifndef _C_LOGGER_H_
#define _C_LOGGER_H_


#include <string>
#include <afxmt.h>
#include <iostream>
#include <fstream>

using namespace std;

class CLogger
{
// Data member
public:
protected:
private:
	// Log Dir 
	std::string m_strLogDir;

	// current log file name
	std::string m_strLogFileName;

	// lock for trace log and cmd log
	CCriticalSection m_CritSection_Trace;
	CCriticalSection m_CritSection_Cmd;
	CCriticalSection m_CritSection_Nv;

	// output file stream , trace log
	ofstream m_ofs_trace;
	ofstream m_ofs_cmd;
	ofstream m_ofs_nv;

	//for time header
	double m_timebase;
	
// Member functions
public:
	CLogger();
	virtual ~CLogger();

	// Logger file name
	bool SetLogName(const char* szLogName );
	string GetLogName();

	//Get TimeStamp
	static string GetTimeStamp();

	//Reset timebase
	void ResetTimebase();

	// basic Log functions
	void TraceLog(const char* szLog , int iSlot=-1);
	void CmdLog(const char* szLog, int iSlot=-1);

protected:
private:
		void InitializeLogFolder();
		bool SetTraceLogName(const char* szLogName);
		bool SetCmdLogName(const char* szLogName);
};


#endif // End of #ifndef _C_LOGGER_H_
