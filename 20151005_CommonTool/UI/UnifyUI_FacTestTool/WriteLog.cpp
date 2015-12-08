
#include "StdAfx.h"
#include "WriteLog.h"

bool CWriteLog::m_b_threadFlag = false;
CString CWriteLog::m_str_logPath = LOG_PATH;
int CWriteLog::m_i_today = 0;


CWriteLog::CWriteLog(void)
{
	/* Init LogFile Structure */
	m_st_logFile.str_logFileName = _T("");
	m_st_logFile.i_line          = 0;
	m_st_logFile.b_openFlag      = false;
	
	m_str_logFileName = _T("");
}

CWriteLog::~CWriteLog(void)
{
	if (m_st_logFile.b_openFlag == true)
	{
		m_st_logFile.obj_file.Close();
	}
}


/*****************************************************************************
* Function name: SetLogFileName      
* Summary      : Set log filename and Open.
* Parameters   : 
* Return       : 
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/09/13          First version             
*****************************************************************************/
bool CWriteLog::SetLogFileName(CString str_logFileName)
{
	/* Check input */
	if (str_logFileName == _T(""))
	{
		return false;
	}
	m_str_logFileName = str_logFileName;

	/* Check log file Open status */
	if (m_st_logFile.b_openFlag == true)
	{
		m_st_logFile.obj_file.Close();
		m_st_logFile.b_openFlag = false;
	}

	/* Open log file */
	if (OpenLogFile() != true)
	{
		return false;
	}

	return true;
}


/*****************************************************************************
* Function name: SaveLog      
* Summary      : Save log to file.
* Parameters   : 
* Return       : 
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/09/13          First version             
*****************************************************************************/
void CWriteLog::SaveLog(CString str_logContent)
{
	/* Check input */
	if (str_logContent == _T(""))
	{
		return;
	}

	CString str_tempLog;
	SYSTEMTIME st_systemTime;
	GetLocalTime(&st_systemTime);
	str_tempLog.Format(_T("[%4d-%02d-%02d %02d:%02d:%02d.%03d]: %s"),st_systemTime.wYear, st_systemTime.wMonth, st_systemTime.wDay,st_systemTime.wHour,st_systemTime.wMinute,st_systemTime.wSecond,st_systemTime.wMilliseconds,str_logContent);

	//if another day
	if (m_i_today != (int)st_systemTime.wDay)
	{
		/* default log file name */
		m_i_today = (int)st_systemTime.wDay;

		CString str_logName;
		str_logName.Format(_T("UILog_%4d%02d%02d.log"),st_systemTime.wYear, st_systemTime.wMonth, st_systemTime.wDay);

		SetLogFileName(str_logName);

		/* Startup Log Receive Thread */
		if (m_b_threadFlag == false)
		{		
			if (::AfxBeginThread(StartupLogServer, this) == NULL)
			{
				return;
			}
			
			m_b_threadFlag = true;
		}
		
		Sleep(1000);
	}
	

	/* Send log */
	SendLog(str_tempLog);

	return;	
}


/*****************************************************************************
* Function name: OpenLogFile      
* Summary      : Open file.
* Parameters   : CString str_logContent: log content.
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/09/13          First version             
*****************************************************************************/
bool CWriteLog::OpenLogFile(void)
{
	/* Check and Create .\\LOG directory */
	WIN32_FIND_DATA fd_fileInformation;
	HANDLE h_searchFind = NULL;
	CString str_path = _T("");
	int i_start = 0;
	
	while(str_path != m_str_logPath)
	{
		i_start = m_str_logPath.Find(_T("\\"),i_start + 1);
		if (i_start != -1)
		{
			str_path = m_str_logPath.Left(i_start);		
		}
		else
		{
			str_path = m_str_logPath;
		}

		if (str_path.Right(1) == _T(":"))
		{
			continue;;
		}

		h_searchFind = ::FindFirstFile(str_path, &fd_fileInformation);
		if ((h_searchFind != INVALID_HANDLE_VALUE) && (fd_fileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			::FindClose(h_searchFind); 
		} 
		else 
		{
			::FindClose(h_searchFind); 

			/* create a null directory */
			if(::CreateDirectory(str_path, NULL) == 0) 
			{
				return false;
			} 
		}
		
	}
	
	/* Open log file */
	CString str_logFilePathName;
	str_logFilePathName.Format(_T("%s\\%s"), m_str_logPath, m_str_logFileName);
	if (!m_st_logFile.obj_file.Open(str_logFilePathName, CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate|CFile::shareDenyNone, NULL)) 
	{
		return false;
	}
	m_st_logFile.str_logFileName = str_logFilePathName;
	m_st_logFile.b_openFlag = true;

	/* Count line */
	try 
	{
		int i_line = 0;
		CString str_string;
		while (m_st_logFile.obj_file.ReadString(str_string)) 
		{
			i_line++;
		}
		m_st_logFile.i_line = i_line;
	} 
	catch(CFileException *e) 
	{
		e->Delete();
		return false;
	}

	return true;
}


/*****************************************************************************
* Function name: WriteLogFile      
* Summary      : Write file.
* Parameters   : CString str_logContent: log content.
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/09/13          First version             
*****************************************************************************/
void CWriteLog::WriteLogFile(CString str_logContent)
{
	/* Check input */
	if (str_logContent == _T(""))
	{
		return;
	}

	/* Check file if opened*/
	if (m_st_logFile.b_openFlag == false)
	{
		return;
	}

	/* New log file name */
	SYSTEMTIME st_systemTime;
	GetLocalTime(&st_systemTime);
	/* Check file lines */
	if (m_st_logFile.i_line >= LOG_LINEMAX)
	{
		/* Close file */
		m_st_logFile.obj_file.Close();
		
		CString str_logTime;
		str_logTime.Format(_T("%4d%02d%02d%02d%02d%02d"), st_systemTime.wYear, st_systemTime.wMonth, st_systemTime.wDay, st_systemTime.wHour, st_systemTime.wMinute, st_systemTime.wSecond);
		CString str_oldLogFileName;
		CString str_newLogFileName;
		str_oldLogFileName = m_st_logFile.str_logFileName;
		str_newLogFileName.Format(_T("%s_%s"), m_st_logFile.str_logFileName, str_logTime);

		/* Rename the old file name */
		if (_trename(str_oldLogFileName, str_newLogFileName) != 0) 
		{
			return;
		}

		/* ReOpen the file */
		if (m_st_logFile.obj_file.Open(str_oldLogFileName, CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate|CFile::shareDenyNone|CFile::typeBinary, NULL )) 
		{
			m_st_logFile.b_openFlag = true;
			m_st_logFile.i_line = 0;
			m_st_logFile.str_logFileName = str_oldLogFileName;
		} 
		else
		{
			return;
		}
	}
	else if (m_i_today != (int)st_systemTime.wDay)
	{
		/* Close file */
		m_st_logFile.obj_file.Close();

		CString str_logName;
		str_logName.Format(_T("UILog_%4d%02d%02d.log"),st_systemTime.wYear, st_systemTime.wMonth, st_systemTime.wDay);

		SetLogFileName(str_logName);
	}
	

	CStringA str_tempLog;
	str_tempLog = str_logContent + _T("\r\n");

	/* Write to file */
	try 
	{
		m_st_logFile.obj_file.SeekToEnd();
		m_st_logFile.obj_file.Write(str_tempLog, str_tempLog.GetLength());
		m_st_logFile.obj_file.Flush();
		m_st_logFile.i_line++;
	} 
	catch (CFileException *e)
	{
		e->Delete();
		return;
	}

	return;
}


/*****************************************************************************
* Function name: SendLog      
* Summary      : Send log to pipe server(receiver thread).
* Parameters   : CString str_logContent: log content.
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/09/13          First version             
*****************************************************************************/
bool CWriteLog::SendLog(CString str_logContent)
{
	/* Check input */
	if (str_logContent == _T(""))
	{
		return false;
	}

	/* Wait the connecting from the server */
	if (::WaitNamedPipe(NAMEDPIPE, NMPWAIT_USE_DEFAULT_WAIT) == FALSE) 
	{
		return false;
	}

	/* Create pipe connection */
	HANDLE h_client = NULL; 
	h_client = ::CreateFile(NAMEDPIPE, GENERIC_WRITE|GENERIC_READ, 0, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);
	if (h_client == INVALID_HANDLE_VALUE) 
	{
		return false;	
	}

	/* Send to pipe */
	DWORD dw_writeNumber = 0;
#ifdef UNICODE
	::WriteFile(h_client, str_logContent, str_logContent.GetLength() * 2, &dw_writeNumber, NULL);
#else
	::WriteFile(h_client, str_logContent, str_logContent.GetLength(), &dw_writeNumber, NULL);
#endif

	/* Close this connection */
	CloseHandle(h_client);

	return true;
}


/*****************************************************************************
* Function name: StartupLogServer      
* Summary      : Receive the log content from pipe server.(Thread function)
* Parameters   : 
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/09/13          First version             
*****************************************************************************/
UINT CWriteLog::StartupLogServer(LPVOID lpParam) 
{
	/* Thread param */
	CWriteLog* p_this = (CWriteLog*)lpParam;
	if (p_this == NULL)
	{
		return 0;
	}

	TCHAR sz_buffer[BUFFER_SIZE];
	memset(sz_buffer, 0, sizeof(sz_buffer));
	DWORD dw_readNum   = 0;
	int i_circleNumber = 0;

	/* Create server pipe */
	HANDLE h_server = NULL;
	h_server = ::CreateNamedPipe(NAMEDPIPE, PIPE_ACCESS_DUPLEX|FILE_FLAG_OVERLAPPED, PIPE_TYPE_BYTE|PIPE_READMODE_BYTE|PIPE_WAIT, 100, BUFFER_SIZE, BUFFER_SIZE, 1000 , NULL);
	if (h_server == INVALID_HANDLE_VALUE) 
	{
		/* Retry CreateNamedPipe again */
		h_server = ::CreateNamedPipe(NAMEDPIPE, PIPE_ACCESS_DUPLEX|FILE_FLAG_OVERLAPPED, PIPE_TYPE_BYTE|PIPE_READMODE_BYTE|PIPE_WAIT, 100, BUFFER_SIZE, BUFFER_SIZE, 1000 , NULL);
		if (h_server == INVALID_HANDLE_VALUE) 
		{
			DWORD dw_errerNo = 0;
			dw_errerNo = GetLastError();
			CString str_logContent;
			str_logContent.Format(_T("CreateNamedPipe Fail,GetLastError=%ld"), dw_errerNo);
			p_this->WriteLogFile(str_logContent);

			return 0;
		}
	}

	/* Listen the pipe and Wait the connection from the Client */
	while(1) 
	{
		if(::ConnectNamedPipe(h_server, NULL)) 
		{
			/* Read data from pipe */
			memset(sz_buffer, 0, sizeof(sz_buffer));
			if(::ReadFile(h_server, sz_buffer, sizeof(sz_buffer), &dw_readNum, NULL) == FALSE) 
			{
				DWORD dw_errerNo = 0;
				dw_errerNo = GetLastError();
				CString str_logContent;
				str_logContent.Format(_T("ReadFile Fail,GetLastError=%ld"), dw_errerNo);
				p_this->WriteLogFile(str_logContent);
			} 
			else 
			{
				/* Read Success -> Save to file */
				p_this->WriteLogFile(sz_buffer);
			}

			/* Close this connection */
			DisconnectNamedPipe(h_server);
		} 
		else 
		{
			/* Retry ConnectNamedPipe 10 */
			i_circleNumber++;
			if (i_circleNumber > 10)
			{
				i_circleNumber = 0;
				Sleep(1000);
			}
		}	
	}

	::CloseHandle(h_server);

	return 1;
}

void CWriteLog::SetLogPath( CString str_logpath )
{
	if (str_logpath != _T(""))
	{
		m_str_logPath = str_logpath;
	}
}
