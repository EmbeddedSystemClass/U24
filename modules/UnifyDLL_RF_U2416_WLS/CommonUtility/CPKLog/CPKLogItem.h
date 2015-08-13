#ifndef _CPK_LOGITEM_H
#define _CPK_LOGITEM_H

#include <string>
#include <vector>
#include "../CommonFunction.h"

#define DEFAULT_CPKLOG_PATH  "D:\\LOG\\CPK\\"

using namespace std;



class CCPKLogItem
{

public:
	CCPKLogItem (LPCTSTR logfile = "");
	~CCPKLogItem();

	string operator[] (LPCTSTR colName);
	string operator[] (const int col);
	//CCPKLogItem& operator= (const CCPKLogItem& rValue);

	void assign (string colName, string value);
	void assign (string colName, long value);
	void assign (string colName, double value);

	void setFileName (LPCTSTR filename, const bool addDate = true);
	void setHeader (const int col, LPCTSTR colName);
	void ResetRecord();
	void ResetAll();
	string GetLogString ();
	string GetLogHeader ();
	void WriteRecordToFile();
	


protected:
	std::string m_strFileName;
	vector <pair<string, string>> m_record;


};



#endif //_CPK_LOGITEM_H