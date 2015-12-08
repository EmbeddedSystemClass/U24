#include "StdAfx.h"
#include "DirectoryDispose.h"

DirectoryDispose::DirectoryDispose(void)
{
}

DirectoryDispose::~DirectoryDispose(void)
{
}


/*
	Function:
		Copy a folder to the other folder.
	Input:
		CString str_srcPath      
		CString str_destPath
	Output:
		void
	Return:
		OK      // Copy success
		NG      // Copy failure
	Note:
	    1. 对于服务器目录路径：至少需3层，否则SHFileOperation会失败。
		   例如："\\\\10.85.40.48\\cgf\\123"
		2. 如果只有2层，则保证2点，SHFileOperation才能成功。
		   2.1 添加通配符，"\\\\10.85.40.48\\cgf\\*"
		   2.2 目标目录路径，必须存在
*/
int DirectoryDispose::CopyDirectory(CString str_srcPath, CString str_destPath)
{   

	// Check Input
	if(str_srcPath == _T("") || str_destPath == _T("")) 
	{
		return NG;
	}

	// 如果路径最后一个字符是"\\",则须去掉，不然SHFileOperation会失败。
	// 例如e:\\123\\  -> e:\\123
	if(str_srcPath.GetAt(str_srcPath.GetLength() - 1) == _T('\\')) 
	{
		try 
		{
			str_srcPath = str_srcPath.Left(str_srcPath.GetLength() - 1);
		}
		catch (CMemoryException e)
		{
			e.Delete();
			return NG;
		}
	}

	if(str_destPath.GetAt(str_destPath.GetLength() - 1) == _T('\\')) 
	{
		try 
		{
			str_destPath = str_destPath.Left(str_destPath.GetLength() - 1);
		}
		catch (CMemoryException e)
		{
			e.Delete();
			return NG;
		}
	}

	// 将CString转换成TCHAR
	TCHAR sz_srcDir[256];
	memset(sz_srcDir, '\0', sizeof(sz_srcDir));
	TCHAR* p_srcDir = sz_srcDir;
	for(int i = 0; i < str_srcPath.GetLength(); i++) {
		p_srcDir[i] = str_srcPath[i];
	}
	p_srcDir[str_srcPath.GetLength()] = '\0';

	TCHAR sz_destDir[256];
	memset(sz_destDir, '\0', sizeof(sz_destDir));
	TCHAR* p_destDir = sz_destDir;
	for(int i = 0; i < str_destPath.GetLength(); i++) 
	{
		p_destDir[i] = str_destPath[i];
	}
	p_destDir[str_destPath.GetLength()] = '\0';

	// Check dest dir if is exist
	int i_dirExistRes = 0;
	i_dirExistRes = IsDirectoryExist(str_destPath);
	if(i_dirExistRes == OK)
	{
		// Exist -- Delete it
		// Check if is disk sign
		int i_checkDiskSignRes = 0;
		i_checkDiskSignRes = IsDiskSignDirectory(str_destPath);
		if(i_checkDiskSignRes != OK) 
		{
			// Not disk sign --- Delete
			SHFILEOPSTRUCT st_fileOp; 
			ZeroMemory((void*)&st_fileOp, sizeof(SHFILEOPSTRUCT));

			st_fileOp.fFlags            = FOF_SIMPLEPROGRESS | FOF_NOCONFIRMMKDIR | FOF_NOCONFIRMATION; 
			st_fileOp.hNameMappings     = NULL; 
			st_fileOp.hwnd              = NULL; 
			st_fileOp.lpszProgressTitle = NULL; 
			st_fileOp.pFrom             = sz_destDir; 
			st_fileOp.pTo               = NULL;
			st_fileOp.wFunc             = FO_DELETE;
			//st_fileOp.wFunc             = FO_COPY;

			if(SHFileOperation(&st_fileOp) != 0) 
			{
				// 用户取消了删除操作
				if(st_fileOp.fAnyOperationsAborted == TRUE)
				{
					return CANCEL;
				}
				return NG;
			}
			if(st_fileOp.fAnyOperationsAborted == TRUE) {
				return CANCEL;
			}
		} 
	}

	// Copy directory
	SHFILEOPSTRUCT st_fileOp; 
	ZeroMemory((void*)&st_fileOp, sizeof(SHFILEOPSTRUCT));

	st_fileOp.fFlags            = FOF_SIMPLEPROGRESS | FOF_NOCONFIRMMKDIR | FOF_NOCONFIRMATION; 
	st_fileOp.hNameMappings     = NULL; 
	st_fileOp.hwnd              = NULL; 
	st_fileOp.lpszProgressTitle = NULL; 
	st_fileOp.pFrom             = sz_srcDir; 
	st_fileOp.pTo               = sz_destDir;
	st_fileOp.wFunc             = FO_COPY; 

	// Excute to copy directory
	int i_shfileResult = 0;
	i_shfileResult = SHFileOperation(&st_fileOp);
	if(i_shfileResult != 0){
		// 用户取消了复制操作
		if(st_fileOp.fAnyOperationsAborted == TRUE) {
			return CANCEL;
		} else {
			return NG;
		}
	}
	if(st_fileOp.fAnyOperationsAborted == TRUE) {
		return CANCEL;
	}

	return OK;
}


int DirectoryDispose::DeleteDir(CString str_directoryName)
{
	// Check Input
	if(str_directoryName == _T("")) {
		return NG;
	}

	if(str_directoryName.GetAt(str_directoryName.GetLength() - 1) == _T('\\')) 
	{
		try 
		{
			str_directoryName = str_directoryName.Left(str_directoryName.GetLength() - 1);
		} 
		catch (CMemoryException e)
		{
			e.Delete();
			return NG;
		}
	}

	// 将CString转换成TCHAR
	TCHAR sz_destDir[256];
	memset(sz_destDir, '\0', sizeof(sz_destDir));
	TCHAR* p_destDir = sz_destDir;
	for(int i = 0; i < str_directoryName.GetLength(); i++) 
	{
		p_destDir[i] = str_directoryName[i];
	}
	p_destDir[str_directoryName.GetLength()] = '\0';
	if(str_directoryName.GetLength() <255)
	{
		p_destDir[str_directoryName.GetLength() + 1] = '\0';
	}

	// Check dest dir if is exist
	int i_dirExistRes = 0;
	i_dirExistRes = IsDirectoryExist(str_directoryName);
	if(i_dirExistRes == OK) {
		// Exist -- Delete it
		// Check if is disk sign
		int i_checkDiskSignRes = 0;
		i_checkDiskSignRes = IsDiskSignDirectory(str_directoryName);
		if(i_checkDiskSignRes != OK)
		{
			// Not disk sign --- Delete
			SHFILEOPSTRUCT st_fileOp; 
			ZeroMemory((void*)&st_fileOp, sizeof(SHFILEOPSTRUCT));

			st_fileOp.fFlags            = FOF_SIMPLEPROGRESS | FOF_NOCONFIRMMKDIR | FOF_NOCONFIRMATION | FOF_NOERRORUI; 
			st_fileOp.hNameMappings     = NULL; 
			st_fileOp.hwnd              = NULL; 
			st_fileOp.lpszProgressTitle = NULL; 
			st_fileOp.pFrom             = sz_destDir; 
			st_fileOp.pTo               = NULL;
			st_fileOp.wFunc             = FO_DELETE; 

			if(SHFileOperation(&st_fileOp) != 0) 
			{
				// 用户取消了删除操作
				if(st_fileOp.fAnyOperationsAborted == TRUE) 
				{
					return CANCEL;
				}
				return NG;
			}
			if(st_fileOp.fAnyOperationsAborted == TRUE)
			{
				return CANCEL;
			}
		} 
	}
	return OK;
}

/*
	Function:
		Check a directory path if is exist.
	Input:
		CString str_directoryName      
	Output:
		void
	Return:
		OK      // Exist
		NG      // Not exist
*/
int DirectoryDispose::IsDirectoryExist(CString str_directoryName)
{
	// Check Input
	if(str_directoryName == _T("")) {	
		return NG;
	}

	WIN32_FIND_DATA fd_fileInformation;
	HANDLE h_searchFind;
	memset(&fd_fileInformation, 0, sizeof(WIN32_FIND_DATA));
	h_searchFind = 0;

	// 如果路径最后一个字符是"\\",则须去掉，不然FindFirstFile会失败。
	// 例如e:\\123\\  -> e:\\123
	if(str_directoryName.GetAt(str_directoryName.GetLength() - 1) == _T('\\')) {
		try {
			str_directoryName = str_directoryName.Left(str_directoryName.GetLength() - 1);
		} catch (CMemoryException e) {
			e.Delete();
			return NG;
		}
	}

	// 对于目录只是盘符的，FindFirstFile有可能会失败
	int i_checkDiskSignRes = 0;
	i_checkDiskSignRes = IsDiskSignDirectory(str_directoryName);
	if(i_checkDiskSignRes == OK) {
		return OK;
	}

	// 检查只是Server IP的情况
	try {
		CString str_tempOne;
		str_tempOne = str_directoryName.Mid(0, 2);
		if (str_tempOne == "\\\\") {
			CStringArray stra_dir;
			if (SpiltString(str_directoryName, "\\", stra_dir) != OK) {
				return NG;
			} else {
				if (stra_dir.GetSize() <= 4) {
					return OK;
				}
			}
	
		}
	} catch(...) {
		return NG;
	}


	// Find the directory and return a search handle
	h_searchFind = FindFirstFile(str_directoryName, &fd_fileInformation);

	// The handle is invalid and the attribute is directory
    if((h_searchFind != INVALID_HANDLE_VALUE) && (fd_fileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){
		// Find success and close the specified search handle
		FindClose(h_searchFind); 
		return OK;    
    } else {
		// Find failure and close the specified search handle
		FindClose(h_searchFind); 
		return NG; 
	} 

	return OK;
}
 

/*
	Function:
		Create a directory.
	Input:
		CString str_directoryName
	Output:
		void
	Return:
		OK      // Suceed
		NG      // Fail
*/
int DirectoryDispose:: CreateDirectory(CString str_directoryName)
{
	// Check Input
	if(str_directoryName == _T("")) 
	{
		return NG;
	}

	CStringArray stra_dirName;           // The directory path name
	CString      str_tmpDirName;         // The directory name
	int i_dirNum = 0;                    // The directory number
	int i_resultFlag = 0;                // Split result

	// Spilt the string by the "\\"
	i_resultFlag = SpiltString(str_directoryName, _T("\\"), stra_dirName);
	if(i_resultFlag != OK)
	{
		return NG;
	}

	// Get the directory name
	if(stra_dirName.GetSize() < 1) 
	{
		return NG;
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
			if (str_tmpDirName == "\\")
			{
				continue;
			}
		}
		if(IsDirectoryExist(str_tmpDirName) != OK)
		{
			// Not exist and create the directory
			if(::CreateDirectory(str_tmpDirName, NULL) == 0)
			{
				// Create directory failure
				return NG;
			} 
			else 
			{
				// Create directory succeed
				continue;
			}
		} 
		else
		{
			// Exist
			continue;
		}
	}

	return OK;
}


/*
	Function:
		Check the disk free space.
	Input:
		CString str_directoryName      
	Output:
		void
	Return:
		OK      // The free space > 10K
		NG      // The free space < 10K  
*/
int DirectoryDispose:: CheckDiskFreeSpace(CString str_directoryName)
{
	// Check Input
	if(str_directoryName == _T("")) {
		return NG;
	}

	CStringArray stra_dirName;
	CString      str_tmpDirName;
	int i_resultFlag = 0;

	// Spilt the string by the "\\"
	i_resultFlag = SpiltString(str_directoryName, _T("\\"), stra_dirName);
	if(i_resultFlag != OK) {
		return NG;
	}

	// Get the directory name
	if(stra_dirName.GetSize() < 1) {
		return NG;
	}

	// Get disk drive
	str_tmpDirName = stra_dirName.GetAt(0);

	DWORD            dwTotalDiskSpace = 0;
	DWORD            dwFreeDiskSpace  = 0;
	DWORD            dwUsedDiskSpace  = 0;   
	ULARGE_INTEGER   uiFreeBytesAvailableToCaller;
	ULARGE_INTEGER   uiTotalNumberOfBytes;						
	ULARGE_INTEGER   uiTotalNumberOfFreeBytes;
	memset(&uiFreeBytesAvailableToCaller, 0, sizeof(ULARGE_INTEGER));
	memset(&uiTotalNumberOfBytes, 0, sizeof(ULARGE_INTEGER));
	memset(&uiTotalNumberOfFreeBytes, 0, sizeof(ULARGE_INTEGER));

	// Get the free space of disk
	if(GetDiskFreeSpaceEx(str_tmpDirName, &uiFreeBytesAvailableToCaller, &uiTotalNumberOfBytes, &uiTotalNumberOfFreeBytes))   
	{   // Get the total size of disk
		dwTotalDiskSpace   =   (DWORD)(uiTotalNumberOfBytes.QuadPart/1024);   
		// Get the free space of disk
		dwFreeDiskSpace    =   (DWORD)(uiFreeBytesAvailableToCaller.QuadPart >> 10);  
		// Get the space having used
		dwUsedDiskSpace    =   dwTotalDiskSpace - dwFreeDiskSpace;

		// The free space is less than 10 K
		if(dwFreeDiskSpace < 10){
			return NG;
		}	
	} else {
		return NG;
	}

	return OK;
}


/*
	Function:
		Check a directory path if have invalid char.
	Input:
		CString str_directoryName      
	Output:
		void
	Return:
		OK      // Valid
		NG      // Invalid  
*/
int DirectoryDispose::CheckDirectoryValid(CString str_directoryName)
{
	// Check Input
	if(str_directoryName == _T("")) {	
		return NG;
	}

	CStringArray stra_dirName;
	CString      directoryName; 
	int          i_resultFlag = 0;

	// Spilt the string by the "\\"
	i_resultFlag = SpiltString(str_directoryName, _T("\\"), stra_dirName);
	if(i_resultFlag != OK) {
		return NG;
	}

	// Get the directory name
	if(stra_dirName.GetSize() < 1) {
		return NG;
	}

	for(int i = 0; i < stra_dirName.GetSize(); i++) {
		// Check file name is illegimate
		directoryName =	stra_dirName.GetAt(i);
		if(directoryName.FindOneOf(FILENAME_ILLEGIMATE_CHARACTER) != -1) {
			// Contain one of "/*?<>|\\\""
			return NG;
		}
	}

	return OK;
}


/*
	Function:
		Spilt the String.
	Input:
		CString        str_sourceString   // Source string
		CString        str_delimiter      // delimiter string
		CStringArray&  stra_strArg        // Save delimitered strings
	Output:
		void
	Return:
		OK      // Suceed
		NG      // Fail
*/
int DirectoryDispose::SpiltString(CString str_sourceString, CString str_delimiter, CStringArray& stra_strArg)
{
	// Check Input
	if(str_sourceString == _T("")) 
	{
		return NG ;
	}

	stra_strArg.RemoveAll();

	CString str_temp;
	int i_position = 0;		   // The position of the character in string 
	int i_strLength = 0;       // The length of str_sourceString

	// Search Tab character in string
	if(str_sourceString.Find(_T("	")) != -1 ) 
	{
		// Replace the Tab Character to space
		str_sourceString.Replace(_T("	"), _T(" "));
	}

	if(str_sourceString.Find(str_delimiter) == 0)
	{
		str_sourceString = str_sourceString.Mid(1);
	}

	// Delimiter is space case
	if(str_delimiter == _T(" ") )
	{
		// Delete left and right space of input parameter
		str_sourceString.TrimLeft();
		str_sourceString.TrimRight();
	}

	// Find the first delimiter
	i_position = str_sourceString.FindOneOf(str_delimiter);
	if(i_position == -1) 
	{
		// The source string did not have delimiter
		stra_strArg.Add(str_sourceString);		
		return OK;
	}

	while(str_sourceString != _T("")) 
	{
		// Get the first of position of delimiter in sourcestring
		i_position = str_sourceString.FindOneOf(str_delimiter);
		i_strLength = str_sourceString.GetLength();
		if(i_position == -1 )
		{
			// The source string did not have delimiter
			stra_strArg.Add(str_sourceString);
			break ;
		}
		try
		{
			// Get the first string
			str_temp = str_sourceString.Mid(0, i_position);
		} 
		catch(CMemoryException e) {
			e.Delete();
			return NG;
		}

		// Delete right space of string
		if(str_delimiter == _T(" ")){
			str_temp.TrimRight();
		}

		stra_strArg.Add(str_temp);

		try{
			// Get the next string
			str_sourceString = str_sourceString.Mid(i_position + 1);
		} catch(CMemoryException e) {
			e.Delete();
			return NG;
		}

		if(str_delimiter == _T(" ")) {
			// delete left space of string
			str_sourceString.TrimLeft();
		}

		// The end string
		if(i_position == i_strLength - 1) {
			stra_strArg.Add(str_sourceString);
		}
	}	
 
	return OK;
}


/*
	Function:
		Check the directory if is disk sign(盘符).
	Input:
		CString        str_directoryName   // Source string
	Output:
		void
	Return:
		OK      // Disk sign
		NG      // Directory
*/
int DirectoryDispose::IsDiskSignDirectory(CString str_directoryName)
{
	// Check Input
	if(str_directoryName == _T("")) {
		return NG ;
	}

	// Delete the end "\\"
	if(str_directoryName.GetAt(str_directoryName.GetLength() - 1) == _T('\\')) {
		try {
			str_directoryName = str_directoryName.Left(str_directoryName.GetLength() - 1);
		} catch (CMemoryException e) {
			e.Delete();
			return NG;
		}
	}

	// Check disk sign
	if(str_directoryName.GetLength() == 2) {

		TCHAR sz_logicString[128];
		CString str_diskString;
		CStringArray stra_diskName;
		int i_splitResult = 0;
		memset(sz_logicString, '\0', sizeof(sz_logicString));

		// Get the all disk logical sign of the computer
		if(GetLogicalDriveStrings(sizeof(sz_logicString), sz_logicString) == 0) {
			return NG;
		}

		for(int i = 0; i < sizeof(sz_logicString); i++){
			if(sz_logicString[i] != '\0'){
				str_diskString = str_diskString + sz_logicString[i] ;
			}
		}

		// Split the str_logicString by "\\"
		i_splitResult = SpiltString(str_diskString , "\\", stra_diskName);
		if(i_splitResult != OK){
			return NG;
		}

		if(stra_diskName.GetSize() < 1) {
			return NG;
		}

		// Check disk
		for(int i = 0; i < stra_diskName.GetSize(); i++) {
			CString str_temp = stra_diskName.GetAt(i);
			int i_res = -1;
			i_res = str_directoryName.CompareNoCase(str_temp);
			if(i_res == 0) {
				return OK;
			}
		}

	}

	return NG;
}


/*
	Function:
		Check the directory if is disk sign(盘符).
	Input:
		CString        str_directoryName   // Source string
		CStringArray&  stra_fileName       // File name
	Output:
		void
	Return:
		OK      // Succeed
		NG      // Failure
*/
int DirectoryDispose::GetFileNameFromDir(CString str_directoryName, CStringArray& stra_fileName)
{
	// Check Input
	if(str_directoryName == _T("")) {
		return NG ;
	}

	stra_fileName.RemoveAll();

	CFileFind obj_tempFind;               
	TCHAR sz_tempFileFind[MAXLENGTH];
	memset( sz_tempFileFind , 0 ,sizeof( sz_tempFileFind ) );

	// The all file of directory
	sprintf_s(sz_tempFileFind , "%s\\*.*", str_directoryName);

	// Lookup the first file
	BOOL b_isFinded = (BOOL)obj_tempFind.FindFile(sz_tempFileFind);
	while(b_isFinded) 
	{
		// Lookup anther files
		b_isFinded = ( BOOL )obj_tempFind.FindNextFile();

		// Judge the directory wether is a point directory or not
		if(!obj_tempFind.IsDots()) {

			if(!obj_tempFind.IsDirectory()) {
				// It is not directory
				CString str_tempFileName;
				str_tempFileName = obj_tempFind.GetFileName().GetBuffer(MAXLENGTH);
				stra_fileName.Add(str_tempFileName);
			}
		}
	}
	// close the object
	obj_tempFind.Close();

	return OK;
}


/*
	Function:
		Get all directory name
	Input:
		CStringArray& stra_strDirName
	Output:
		void
	Return:
		OK      // Succeed
		NG      // Failure
*/
void DirectoryDispose::GetAllDirName(CString str_directoryName, CStringArray& stra_strDirName)
{
	// Check Input
	if(str_directoryName == _T("")) {
		return;
	}
	stra_strDirName.RemoveAll();

	CFileFind obj_tempFind;               
	char sz_tempFileFind[MAXLENGTH];
	memset(sz_tempFileFind, 0, sizeof(sz_tempFileFind));

	// The all file of directory
	sprintf_s(sz_tempFileFind, "%s\\*.*", str_directoryName);

	// Lookup the first file
	BOOL b_isFinded = (BOOL)obj_tempFind.FindFile(sz_tempFileFind);
	while(b_isFinded) 
	{
		// Lookup anther files
		b_isFinded = ( BOOL )obj_tempFind.FindNextFile();

		// Judge the directory wether is a point directory or not
		if(!obj_tempFind.IsDots()) {

			if(obj_tempFind.IsDirectory()) {
				// It is not directory
				CString str_tempFileName;
				str_tempFileName = obj_tempFind.GetFileName().GetBuffer(MAXLENGTH);
				stra_strDirName.Add(str_tempFileName);
			}
		}
	}
	// close the object
	obj_tempFind.Close();

	return;
}
