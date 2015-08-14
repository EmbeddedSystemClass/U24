#include "stdafx.h"
#include "CPKLogItem.h"


//#define _WRITE_HORIZONTAL_CPK //normal, header at top

using namespace StringUtility;


CCPKLogItem::CCPKLogItem(char* logfile)
{
	setFileName (logfile);

}

CCPKLogItem::~CCPKLogItem()
{
}

void CCPKLogItem::setFileName (const char* szLogfile, const bool bAddDate)
{
	std::string strLogpath;

	if (0 == szLogfile[0])
	{
		strLogpath = MakeSTDstring ("%sRF_CPK", DEFAULT_CPKLOG_PATH);
	}
	else
	{
		strLogpath = szLogfile;
	}

	if (bAddDate)
	{
		std::string strTime = GetTimeStamp();
		strLogpath = MakeSTDstring("%s_%s.csv", strLogpath.c_str(), strTime.c_str());
	}

	if (PathIsRelativeA (strLogpath.c_str()))
	{
		char szTemp[MAX_PATH] = {0};
		GetModuleFileNameA(NULL, szTemp, MAX_PATH);
		PathRemoveFileSpecA(szTemp);

		m_strFileName = szTemp;
		m_strFileName += "\\" + strLogpath;
	}
	else
	{
		m_strFileName = strLogpath;
	}
}

bool CCPKLogItem::IsFileExist()
{
	return PathFileExistsA(m_strFileName.c_str()) == TRUE;
}

void CCPKLogItem::UpdateFileNameVersion()
{
	const int bufsize = 524288;
	bool bSame = false;

	int iloop = 9999; //for save
	while (iloop-- > 0)
	{
		if (! PathFileExistsA(m_strFileName.c_str()))
			break;
		
#ifdef _WRITE_HORIZONTAL_CPK

		char *HeaderFromFile = new char [bufsize];
		CStdioFile File_log;
		if(File_log.Open (m_strFileName.c_str(), CFile::modeRead, NULL))
		{
			File_log.ReadString (HeaderFromFile, bufsize);
			File_log.Close();
		
			std::vector<std::string> vFileCol;
			STDStringTokenizer (HeaderFromFile, ",", vFileCol);
			if (vFileCol.size() == m_record.size())
			{		
				bSame = true;
				int colCount = vFileCol.size();// compare per column
				for (int i=0; i < colCount; ++i)
				{
					if (vFileCol[i] != m_record[i].first)
					{
						bSame = false;
						break;
					}
				}
			}

			delete [] HeaderFromFile;
		}

#else

		std::vector<std::string> vFileCol, vTemp;
		TCHAR *HeaderFromFile = new TCHAR [bufsize];
		CStdioFile File_log;
		CA2T strFileName(m_strFileName.c_str());

		if(File_log.Open(strFileName, CFile::modeRead, NULL))
		{
			while (File_log.ReadString(HeaderFromFile, bufsize))
			{
				std::wstring strWstring(HeaderFromFile);
				std::string strHeaderFromFile(strWstring.begin(), strWstring.end());
				STDStringTokenizer (strHeaderFromFile, ",", vTemp);
				vFileCol.push_back (vTemp[0]);
			}
			File_log.Close();
			delete [] HeaderFromFile;


			// compare per column
			if (vFileCol.size() == m_record.size())
			{		
				bSame = true;
				int colCount = vFileCol.size();
				for (int i=0; i < colCount; ++i)
				{
					if (vFileCol[i] != m_record[i].first)
					{
						bSame = false;
						break;
					}
				}
			}

		} // if(File_log.Open

#endif

		if (bSame)
			break;

		// Increase version
		int ver;
		size_t pos = m_strFileName.find ("_v");
		if (pos != std::string::npos) 
		{
			ver = atoi (m_strFileName.substr(pos+2).c_str()) + 1;
		}
		else
		{
			ver = 1;
			pos = m_strFileName.find (".");
		}
		m_strFileName.erase (pos);
		m_strFileName += MakeSTDstring ("_v%d.csv", ver);
	}

}

//string CCPKLogItem::operator[] (LPCTSTR colName)
//{
//	int count = m_record.size();
//	for (int i=0; i<count; ++i)
//	{
//		if (m_record[i].first.compare(colName) != string.npos)
//			return m_record[i].second;
//	}
//
//	return "";
//}
//
//std::string CCPKLogItem::operator[] (const int col)
//{
//	if (col >= (int) m_record.size())
//		return "";
//
//	return m_record[col].second;
//}

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


void CCPKLogItem::setHeader (const int col, const char* colName)
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

/*
void CCPKLogItem::WriteRecordToFile()
{
	UpdateFileNameVersion();

	// CreateDirectory if not exist
	char chDir[MAX_PATH];
	strcpy (chDir, m_strFileName.c_str());
	PathRemoveFileSpecA (chDir);

	if (! CreateDirectoryA (chDir, NULL))
	{
		if (GetLastError() != ERROR_ALREADY_EXISTS)
		{
			// write to MyDocuments folder
			SHGetFolderPathA (NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, chDir);
			PathAppendA (chDir, "QisdaCPK\\");

			char chTemp[256];
			strcpy (chTemp, m_strFileName.c_str());
			m_strFileName = std::string (chDir) + PathFindFileNameA (chTemp);

			strcpy (chDir, m_strFileName.c_str());
			PathRemoveFileSpecA (chDir);
			CreateDirectoryA (chDir, NULL);	
		}
	}

	bool bNewFile = ! PathFileExistsA (m_strFileName.c_str());


#ifdef _WRITE_HORIZONTAL_CPK
	
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

#else 


	//
	// OK -- write header vertically (at left)
	//

	int count;
	CString cstrTemp;
	std::vector<std::string> cpkRecords;
	CStdioFile File_log;
	CString cstrFileName(m_strFileName.c_str());
	// write header if this is a new file
	if (bNewFile)
	{
		
		if(! File_log.Open (cstrFileName, CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate, NULL))
			return;

		count = m_record.size();
		for (int i=0; i<count; ++i)
		{
			cstrTemp = CString(m_record[i].first.c_str()) + CString(_T(",\n"));
			File_log.WriteString (cstrTemp);
		}
		File_log.Close();
	}


	if(! File_log.Open (cstrFileName, CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate, NULL))
		return;

	// read files
	int i=0;
	while (File_log.ReadString (cstrTemp))
		cpkRecords.push_back (CStringA(cstrTemp).GetString());

	// append current record
	count = m_record.size();
	for (int i=0; i<count; i++)
		cpkRecords[i] += m_record[i].second + ",\n";

	// write each line back to file
	File_log.Seek(0, CFile::begin);
	for (int i=0; i<count; ++i)
		File_log.WriteString (CString(cpkRecords[i].c_str()));

	File_log.Close();
	
#endif //_WRITE_HORIZONTAL_CPK
}
*/

std::string CCPKLogItem::GetTimeStamp()
{
	SYSTEMTIME* Systime = new SYSTEMTIME;
	GetLocalTime(Systime);

	char szTime[MAX_PATH];
	memset(szTime, 0, MAX_PATH);

	sprintf_s(szTime, MAX_PATH, "%04d%02d%02d",
		Systime->wYear,
		Systime->wMonth,
		Systime->wDay
		);

	delete Systime;

	//return
	string strTime = szTime;
	return strTime;
}
