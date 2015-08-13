
#include "stdafx.h"
#include "GLog.h"

CGLog::CGLog():m_str_logPath(""), m_b_init(false)
{

}

CGLog::~CGLog()
{
	if (m_b_init)
	{
		google::ShutdownGoogleLogging();
	}
}

void CGLog::InitializeGLog(const char* sz_logPath)
{
	/* Init */
	google::InitGoogleLogging("");
	m_b_init = true;

	if ((sz_logPath == NULL) || (0 == strcmp(sz_logPath,"")))
	{
		m_str_logPath = DEFAULT_LOG_DIR;
		return;
	}

	CString str_logPath;
	str_logPath.Format("%s", sz_logPath);
	if (str_logPath.GetAt(str_logPath.GetLength() - 1) != _T('\\')) 
	{
		str_logPath = str_logPath + _T("\\");
	}

	/* Check path */
	if (str_logPath.Find(":\\") != -1)
	{
		/* Absolute path */
		m_str_logPath = str_logPath;

	}
	else
	{
		char sz_currentPath[MAX_PATH] = { 0 };
		::GetModuleFileName(NULL, sz_currentPath, MAX_PATH);
		::PathRemoveFileSpec(sz_currentPath);

		if (str_logPath.Left(2) == _T(".\\"))
		{
			str_logPath = str_logPath.Right(str_logPath.GetLength() - 2);
		}

		m_str_logPath.Format("%s\\%s", sz_currentPath, str_logPath);
	}

	/* CreateDirectory if not exist */
	if (! CreateLogDirectory(m_str_logPath)){
		m_str_logPath = DEFAULT_LOG_DIR;
	}

	return;
}

void CGLog::SetGLogFileName_Info(const char* sz_logName)
{
	CString str_fullPath = m_str_logPath + sz_logName;
	google::SetLogDestination (google::_INFO, str_fullPath);
}

void CGLog::SetGLogFileName_Error(const char* sz_logName)
{
	CString str_fullPath = m_str_logPath + sz_logName;
	google::SetLogDestination (google::_ERROR, str_fullPath);
}

void CGLog::SetGLogFileName_Waring(const char* sz_logName)
{
	CString str_fullPath = m_str_logPath + sz_logName;
	google::SetLogDestination(google::_WARNING, str_fullPath);
}

void CGLog::Log(const int i_severity, const char* sz_logMessage)
{
	switch(i_severity)
	{
	case __INFO:
		LOG(_INFO) << sz_logMessage;
		break;
	case __ERROR:
		LOG(_ERROR) << sz_logMessage;
		break;
	case __WARNING:
		LOG(_WARNING) << sz_logMessage;
		break;
	default:
		LOG(_ERROR) << sz_logMessage;
		break;
	}

	google::FlushLogFiles(i_severity);
}

void CGLog::ShutdownGLog(void)
{
	google::ShutdownGoogleLogging();
	m_b_init = false;
}

bool CGLog::CreateLogDirectory(CString str_directoryName)
{
	/* Check Input */
	if(str_directoryName == _T("")) 
	{
		return false;
	}

	CStringArray stra_dirName;           // The directory path name
	CString      str_tmpDirName;         // The directory name
	int i_dirNum = 0;                    // The directory number
	int i_resultFlag = 0;                // Split result

	/* Spilt the string by the "\\" */
	if(SpiltString(str_directoryName, _T("\\"), stra_dirName) != true) 
	{
		return false;
	}

	/* Get the directory name */
	if(stra_dirName.GetSize() < 1) 
	{
		return false;
	}

	i_dirNum = stra_dirName.GetSize();
	for(int i = 0; i < i_dirNum; i++) 
	{
		if(i == 0) 
		{
			str_tmpDirName = stra_dirName.GetAt(i);
			continue;
		} 
		else 
		{
			str_tmpDirName = str_tmpDirName + _T("\\")+ stra_dirName.GetAt(i);
		}

		if(!::CreateDirectory(str_tmpDirName, NULL)) 
		{
			if (GetLastError() != ERROR_ALREADY_EXISTS)
			{
				return false;
			} else
			{
				continue;
			}
		}
	}

	return true;
}

bool CGLog::SpiltString(CString str_srcString, CString str_delimiter, CStringArray& stra_strArg)
{
	/* Check Input */
	if ((str_srcString == _T("")) || (str_delimiter == _T(""))) 
	{
		return false;
	}

	stra_strArg.RemoveAll();

	/* Case1: Delimiter is Space */
	if (str_delimiter == _T(" ") )
	{
		str_srcString.TrimLeft();
		str_srcString.TrimRight();
	}
		
	/* Case2: The source string did not have delimiter */
	int i_position = 0;
	i_position = str_srcString.FindOneOf(str_delimiter);
	if (i_position == -1) 
	{
		stra_strArg.Add(str_srcString);		
		return true;
	}

	/* Split string */
	CString str_temp;
	int i_strLength = 0;
	while (str_srcString != _T("")) 
	{
		i_position = str_srcString.FindOneOf(str_delimiter);
		i_strLength = str_srcString.GetLength();
		if (i_position == -1 )
		{
			stra_strArg.Add(str_srcString);
			break;
		}

		try
		{
			/* Get the first string */
			str_temp = str_srcString.Mid(0, i_position);
		} 
		catch(CMemoryException e) 
		{
			e.Delete();
			return false;
		}

		/* Delete right space of string */
		if (str_delimiter == _T(" "))
		{
			str_temp.TrimRight();
		}

		stra_strArg.Add(str_temp);

		try
		{
			/* Get the next string */
			str_srcString = str_srcString.Mid(i_position + 1);
		} 
		catch(CMemoryException e) 
		{
			e.Delete();
			return false;
		}

		if (str_delimiter == _T(" "))
		{
			str_srcString.TrimLeft();
		}

		/* The end string */
		if(i_position == i_strLength - 1) {
			stra_strArg.Add(str_srcString);
		}
	}	
 
	return true;
}
