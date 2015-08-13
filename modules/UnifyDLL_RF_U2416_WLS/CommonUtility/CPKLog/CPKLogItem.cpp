#include "stdafx.h"
#include "CPKLogItem.h"



CCPKLogItem::CCPKLogItem(LPCTSTR logfile)
{
	setFileName (logfile);

}

CCPKLogItem::~CCPKLogItem()
{
}

void CCPKLogItem::setFileName (LPCTSTR logfile, const bool addDate)
{
	string logpath;

	if (0 == logfile[0])
	{
		logpath = MakeSTDstring ("%sBTWifi_CPK", DEFAULT_CPKLOG_PATH);
	}
	else
	{
		logpath = logfile;
	}

	if (addDate)
	{
		CString csTime = CTime::GetCurrentTime().Format(_T("%Y%m%d"));
		logpath += string ("_") + string(csTime) + string(".csv");
	}

	if (PathIsRelative (logpath.c_str()))
	{
		char szTemp[MAX_PATH] = {0};
		GetModuleFileName(NULL, szTemp, MAX_PATH);
		PathRemoveFileSpec(szTemp);

		m_strFileName = szTemp;
		m_strFileName += "\\" + logpath;
	}
	else
	{
		m_strFileName = logpath;
	}

}

string CCPKLogItem::operator[] (LPCTSTR colName)
{
	int count = m_record.size();
	for (int i=0; i<count; ++i)
	{
		if (m_record[i].first.compare(colName) != string.npos)
			return m_record[i].second;
	}

	return "";
}

std::string CCPKLogItem::operator[] (const int col)
{
	if (col >= (int) m_record.size())
		return "";

	return m_record[col].second;
}

void CCPKLogItem::assign (std::string colName, std::string value)
{
	int count = m_record.size();
	for (int i=0; i<count; ++i)
	{
		if (m_record[i].first == colName)
		{
			m_record[i].second = value;
			break;
		}
	}
}

void CCPKLogItem::assign (std::string colName, long value)
{
	int count = m_record.size();
	for (int i=0; i<count; ++i)
	{
		if (m_record[i].first == colName)
		{
			m_record[i].second = MakeSTDstring ("%d", value);
			break;
		}
	}
}

void CCPKLogItem::assign (std::string colName, double value)
{
	int count = m_record.size();
	for (int i=0; i<count; ++i)
	{
		if (m_record[i].first == colName)
		{
			m_record[i].second = MakeSTDstring ("%f", value);
			break;
		}
	}
}


void CCPKLogItem::setHeader (const int col, LPCTSTR colName)
{
	pair<string, string> temp;
	temp.first = colName;
	temp.second = "";

	m_record.push_back (temp);
}

void CCPKLogItem::ResetRecord()
{
	int count = m_record.size();
	for (int i=0; i < count; ++i)
	{
		m_record[i].second = "";
	}
}

void CCPKLogItem::ResetAll()
{
	m_record.clear();
}

string CCPKLogItem::GetLogString ()
{
	string strFull = "";

	int count = m_record.size();
	for (int i=0; i < count; ++i)
	{
		strFull += m_record[i].second + ",";
	}

	strFull += string("\n");
	return strFull;
}

string CCPKLogItem::GetLogHeader()
{
	string strFull = "";

	int count = m_record.size();
	for (int i=0; i < count; ++i)
	{
		strFull += m_record[i].first + ",";
	}

	strFull += string("\n");
	return strFull;
}

void CCPKLogItem::WriteRecordToFile()
{
	// CreateDirectory if not exist
	char chDir[MAX_PATH];
	strcpy_s (chDir, m_strFileName.c_str());
	PathRemoveFileSpec (chDir);

	if (! CreateDirectory (chDir, NULL))
	{
		if (GetLastError() != ERROR_ALREADY_EXISTS)
		{
			strcpy_s (chDir, "C:\\");

			char chTemp[256];
			strcpy_s (chTemp, m_strFileName.c_str());
			m_strFileName = std::string (chDir) + PathFindFileName (chTemp);
		}
	}

	bool bNewFile = ! PathFileExists (m_strFileName.c_str());

	/*
	// OK -- write header horizontally
	CStdioFile File_log;
	if(File_log.Open (m_strFileName.c_str(), CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate, NULL))
	{
		// write header if this is a new file
		if (bNewFile)
			File_log.WriteString (GetLogHeader().c_str());

		File_log.SeekToEnd();
		File_log.WriteString(GetLogString().c_str());
		File_log.Close();
	}
	*/


	// write header vertically

	int count;
	CString cstrTemp;
	std::vector<std::string> cpkRecords;
	CStdioFile File_log;

	// write header if this is a new file
	if (bNewFile)
	{
		if(! File_log.Open (m_strFileName.c_str(), CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate, NULL))
			return;

		count = m_record.size();
		for (int i=0; i<count; ++i)
		{
			cstrTemp = m_record[i].first.c_str() + CString(",\n");
			File_log.WriteString (cstrTemp);
		}
		File_log.Close();
	}


	if(! File_log.Open (m_strFileName.c_str(), CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate, NULL))
		return;

	// read files
	int i=0;
	while ( File_log.ReadString (cstrTemp) )
		cpkRecords.push_back (cstrTemp.GetString());

	// append current record
	count = m_record.size();
	for (int i=0; i<count; i++)
		cpkRecords[i] += m_record[i].second + ",\n";

	// write each line back to file
	File_log.Seek(0, CFile::begin);
	for (int i=0; i<count; ++i)
		File_log.WriteString (cpkRecords[i].c_str());

	File_log.Close();
	
}
