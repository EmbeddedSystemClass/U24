#include "stdafx.h"
#include "CLogger.h"
#include <Shlwapi.h>

#define DEFAULT_LOG_DIR             "D:\\LOG\\DLL\\"
#define ANOTHER_LOG_DIR				"C:\\LOG\\DLL\\"
#define ANOTHER_LOG_DIR_BASE		"C:\\LOG\\"
#define MSGBUFF				1024
#define TIMESTAMP_BUFF		32

CLogger::CLogger()
{
	m_timebase=0;
	InitializeLogFolder();
}

void CLogger::InitializeLogFolder()
{
	m_strLogDir = DEFAULT_LOG_DIR;

	// CreateDirectory if not exist
	char szDir[MAX_PATH] = {0};
	strcpy_s(szDir, MAX_PATH, m_strLogDir.c_str());
	if (! CreateDirectoryA (szDir, NULL))
	{
		if (GetLastError() != ERROR_ALREADY_EXISTS)
		{
			m_strLogDir = "";
		}
	}

	// Create ANOTHER_LOG_DIR_BASE
	if (m_strLogDir == "")
	{
		m_strLogDir = ANOTHER_LOG_DIR;

		std::string strLogDir;
		strLogDir = ANOTHER_LOG_DIR_BASE;
		memset(szDir, 0, MAX_PATH);
		strcpy_s(szDir, MAX_PATH, strLogDir.c_str());

		if (! CreateDirectoryA (szDir, NULL))
		{
			if (GetLastError() != ERROR_ALREADY_EXISTS)
			{
				m_strLogDir = "";
				return;
			}
		}
		else
		{
			// Create ANOTHER_LOG_DIR
			strLogDir = ANOTHER_LOG_DIR;
			memset(szDir, 0, MAX_PATH);
			strcpy_s(szDir, MAX_PATH, strLogDir.c_str());

			//Creates all the directories in the specified path, beginning with the root.
			if (! CreateDirectoryA (szDir, NULL))
			{
				if (GetLastError() != ERROR_ALREADY_EXISTS)
				{
					m_strLogDir = "";
				}
			}
		}
	}
}


CLogger::~CLogger()
{
	if (m_ofs_trace.is_open()) 
	{
		m_ofs_trace.close();
	}

	if (m_ofs_cmd.is_open())
	{
		m_ofs_cmd.close();
	}

	if (m_ofs_nv.is_open())
	{
		m_ofs_nv.close();
	}
}


// --------------------------------------------------------------
// ---------------------Log name functions ---------------------
// --------------------------------------------------------------

bool CLogger::SetLogName(const char* szLogName)
{
	//if the same do nothing
	string strLogName= szLogName;
	if ( m_strLogFileName == strLogName)
	{
		return true;
	}

	//if different , save it , start clock
	m_strLogFileName = szLogName;
	m_timebase = clock();				//start new clock for log

	// init every log sink , trace log and cmd log
	SetTraceLogName(szLogName);
	SetCmdLogName(szLogName);

	return true;
}

bool CLogger::SetTraceLogName(const char* szLogName)
{
	//log name different current on , close old one
	if ( m_ofs_trace.is_open())
	{
		m_ofs_trace.close();
	}

	// change to new log
	char szTraceFullPath [MAX_PATH];
	sprintf_s(szTraceFullPath, MAX_PATH,"%s%s_TRACE.txt", m_strLogDir.c_str(), szLogName);

	m_ofs_trace.open(szTraceFullPath , ios::out | ios::app	);

	return true;
}

bool CLogger::SetCmdLogName(const char* szLogName)
{
	//log name different current on , close old one
	if ( m_ofs_cmd.is_open())
	{
		m_ofs_cmd.close();
	}

	// change to new log 
	char szCmdFullPath [MAX_PATH];
	sprintf_s(szCmdFullPath, MAX_PATH,"%s%s_CMD.txt", m_strLogDir.c_str(), szLogName);

	m_ofs_cmd.open(szCmdFullPath, ios::out | ios::app );

	return true;
}


// --------------------------------------------------------------
// ---------------------Basic log functions ---------------------
// --------------------------------------------------------------

void CLogger::TraceLog(const char* szLog, int iSlot)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&this->m_CritSection_Trace), TRUE);

	if (m_ofs_trace.is_open() )
	{
		//time in second
		char szTraceTime[TIMESTAMP_BUFF];
		sprintf_s(szTraceTime, TIMESTAMP_BUFF,"%.3f", ((double)clock()-m_timebase)/CLOCKS_PER_SEC);
		m_ofs_trace << "[" << szTraceTime << "][" << iSlot <<"] ";

		m_ofs_trace<< szLog << "\r\n";
		m_ofs_trace.flush();
	}
}

void CLogger::CmdLog(const char* szLog, int iSlot)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&this->m_CritSection_Cmd), TRUE);

	if (m_ofs_cmd.is_open() )
	{	
		//time in second
		char szTraceTime[TIMESTAMP_BUFF];

		sprintf_s(szTraceTime, TIMESTAMP_BUFF, "%.3f", ((double)clock()-m_timebase)/CLOCKS_PER_SEC);
		m_ofs_cmd << "[" << szTraceTime << "][" << iSlot << "] ";

		m_ofs_cmd<< szLog << "\r\n";
		m_ofs_cmd.flush();
	}
}

std::string CLogger::GetTimeStamp()
{
	SYSTEMTIME* Systime = new SYSTEMTIME;
	GetLocalTime(Systime);

	char szTime[MAX_PATH];
	memset(szTime, 0, MAX_PATH);

	sprintf_s(szTime, MAX_PATH, "%04d%02d%02d_%02d%02d%02d",
		Systime->wYear,
		Systime->wMonth,
		Systime->wDay,
		Systime->wHour,
		Systime->wMinute,
		Systime->wSecond
		);

	delete Systime;

	//return
	string strTime = szTime;
	return strTime;
}

std::string CLogger::GetLogName()
{
	return m_strLogFileName;
}

void CLogger::ResetTimebase()
{
	m_timebase = clock();	
}
