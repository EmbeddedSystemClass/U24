#include "stdafx.h"
#include "CommDef.h"


CCriticalSection m_obj_FileLog;

bool Check_Picasso_Rule_All(CString str_picasso)
{
	CSingleLock singleLock(&m_obj_FileLog, TRUE);

	if (str_picasso.GetLength() != PICASSO_LENGTH)
	{
		return false;
	}

	if (!IsStringAlphaNumber(str_picasso))
	{
		return false;
	}

	return true;
}

CString ConvertString(const char* sz_buffer)
{
	CSingleLock singleLock(&m_obj_FileLog, TRUE);
	/* Check input */
	if (*sz_buffer == NULL)
	{
		return _T("");
	}

	/* Convert char* to CString */
	CString str_temp;
	int i_len = ::MultiByteToWideChar(CP_ACP, 0, sz_buffer, -1, NULL, 0);

	TCHAR *sz_buf = new TCHAR[i_len + 2];
	::MultiByteToWideChar(CP_ACP, 0, sz_buffer, -1, sz_buf, i_len);
	str_temp.Format(_T("%s"), sz_buf);
	delete[] sz_buf;

	return str_temp;
}

bool IsStringAlphaNumber(CString &str_text)
{
	CSingleLock singleLock(&m_obj_FileLog, TRUE);
	for (int i_index = 0; i_index < str_text.GetLength(); i_index++)
	{
		if (!isalnum(str_text.GetAt(i_index)))
		{
			return false;	
		}
	}

	return true;
}

bool SpiltString(CString str_srcString, CString str_delimiter, CStringArray& stra_strArg)
{
	CSingleLock singleLock(&m_obj_FileLog, TRUE);
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


void CStringToChar( CString str_In, char *sz_Out )
{
	CSingleLock singleLock(&m_obj_FileLog, TRUE);
	::WideCharToMultiByte(CP_ACP, 0, str_In, -1, sz_Out, str_In.GetLength() * 2, NULL, NULL);
}


// void LogMsg(CString strMsg)
// {
// 	CSingleLock singleLock(&m_obj_FileLog, TRUE);
// 	if (strMsg == _T("")) 
// 	{
// 		return;
// 	}
// 
// 	/* Get current time */
// 	SYSTEMTIME time;
// 	::GetLocalTime(&time);
// 
// 	/* Get current app directory */
// 	TCHAR sz_currentDir[256];
// 	memset(sz_currentDir, 0, sizeof(sz_currentDir));
// 	::GetCurrentDirectory(sizeof(sz_currentDir), sz_currentDir);
// 	CString str_currentPath;
// 	str_currentPath.Format(_T("%s"), sz_currentDir);
// 
// 	/* Check LOG\\DB_Log exist and create */
// 	CString str_logDir;
// 	str_logDir = str_currentPath;
// 
// 	WIN32_FIND_DATA fd_fileInformation;
// 	HANDLE h_searchFind;
// 	h_searchFind = FindFirstFile(str_logDir, &fd_fileInformation);
// 	if ((h_searchFind != INVALID_HANDLE_VALUE) && (fd_fileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
// 	{
// 		FindClose(h_searchFind); 
// 	}
// 	else 
// 	{
// 		FindClose(h_searchFind ); 
// 		if (CreateDirectory(str_logDir, NULL) == 0) 
// 		{
// 			return;
// 		} 
// 	} 
// 
// 	str_logDir += "\\Log";
// 	h_searchFind = FindFirstFile(str_logDir, &fd_fileInformation);
// 	if ((h_searchFind != INVALID_HANDLE_VALUE) && (fd_fileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
// 	{
// 		FindClose(h_searchFind); 
// 	} 
// 	else 
// 	{
// 		FindClose(h_searchFind ); 
// 		if (CreateDirectory(str_logDir, NULL) == 0) 
// 		{
// 			return;
// 		} 
// 	} 
// 
// 	/* Open log file */
// 	CStdioFile obj_file;
// 	CString str_logFile;
// 	CString str_temp;
// 	str_temp.Format(_T("[%04d/%02d/%02d %02d:%02d:%02d:%03d]:%s "),time.wYear,time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond,time.wMilliseconds,strMsg);
// 	str_logFile.Format(_T("%s\\%04d_%02d_%02d.log"),str_logDir, time.wYear, time.wMonth, time.wDay);
// 
// 	if (obj_file.Open(str_logFile, CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate, NULL)) 
// 	{
// 		obj_file.SeekToEnd();
// 		obj_file.WriteString(str_temp);
// 		obj_file.WriteString(_T("\r\n"));
// 		obj_file.Close();
// 	}
// }
// 

CString ConvertToVisibleString(char *sz_buf, int i_len)
{
	CSingleLock singleLock(&m_obj_FileLog, TRUE);
	CString str_print = _T("");
	CString str_tmp = _T("");
	for (int i = 0; i < i_len; i++)
	{
		if ((sz_buf[i] >= 32) && (sz_buf[i] <= 126))
		{
			str_tmp.Format(_T("%c "),sz_buf[i]);
		}
		else
		{
			str_tmp.Format(_T("%.2d "),sz_buf[i]);
		}

		str_print += str_tmp;
	}	

	return str_print;
}

