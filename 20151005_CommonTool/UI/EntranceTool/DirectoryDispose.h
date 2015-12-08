/**
 * COPYRIGHT QCS LIMITED 2010
 *
 * Description: Some Method of dispose directory 
 * Version    : $Revision:   1.0  $
 * Author     : QCS/WGC18/Alex
 * Time       : 2010/06/07
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2010/06/07     Alex              princeps edition.
 * 
 */

#pragma once

#define    OK                                  1
#define    NG                                  0
#define    CANCEL                              -1

#define    FILENAME_ILLEGIMATE_CHARACTER	   _T("/ *?<>|\\\"")
#define    MAXLENGTH                           256  

class DirectoryDispose
{
public:
	DirectoryDispose(void);
	virtual ~DirectoryDispose(void);

public:
	int CheckDirectoryValid(CString str_directoryName);
	int IsDirectoryExist(CString str_directoryName);
	int CreateDirectory(CString str_directoryName);
	int CopyDirectory(CString str_srcPath, CString str_destPath);
	int CheckDiskFreeSpace(CString str_directoryName);
	int SpiltString(CString str_sourceString, CString str_delimiter, CStringArray& stra_strArg);
	int IsDiskSignDirectory(CString str_directoryName);
	int GetFileNameFromDir(CString str_directoryName, CStringArray& stra_fileName);
	void GetAllDirName(CString str_directoryName, CStringArray& stra_strDirName);
	int DeleteDir(CString str_directoryName);
	
};
