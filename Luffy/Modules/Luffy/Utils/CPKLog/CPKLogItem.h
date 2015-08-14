#ifndef _CPK_LOGITEM_H
#define _CPK_LOGITEM_H

#include <string>
#include <vector>
#include <Shlwapi.h>

#include "Utils/StringUtil.h"

#define DEFAULT_CPKLOG_PATH  "D:\\Log\\DLL\\CPK\\"
#define MAX_TESTITEM 16

using namespace std;



class CCPKLogItem
{

public:
	CCPKLogItem (char* logfile = "");
	~CCPKLogItem();

	//string operator[] (LPCTSTR colName);
	//string operator[] (const int col);
	//CCPKLogItem& operator= (const CCPKLogItem& rValue);

	void assign (string colName, string value);
	void assign (string colName, long value);
	void assign (string colName, double value);

	bool IsFileExist ();
	std::string getFileName () {return m_strFileName;}
	void UpdateFileNameVersion();
	void setFileName (const char* szLogfile, const bool bAddDate = true);
	void setHeader (const int col,const char* colName);
	void ResetRecord();
	void ResetAll();
	string GetLogString ();
	string GetLogHeader ();
	void WriteRecordToFile();
	static string GetTimeStamp();
	


protected:
	std::string m_strFileName;
	vector <pair<string, string>> m_record;


};



#endif //_CPK_LOGITEM_H