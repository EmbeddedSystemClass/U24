#include "StdAfx.h"
#include "MD5_Check.h"


#include "wininet.h "
#pragma comment(lib, "Wininet.lib")
#pragma comment(lib, "Mpr.Lib")

#include "windows.h " 
#include <winsock.h>
#include "afxcmn.h"
#include "afxwin.h"

#include <stdio.h> 
#include <io.h>

#import "msxml3.dll"

MD5_Check::MD5_Check(void)
{
	m_str_serverPath = _T("");
	m_str_dirPath = _T("");
	m_str_hashPath = _T("");
	m_str_StationName = _T("");
	m_str_ModelName = _T("");
	m_str_swversion = _T("");
	m_stra_Toolpath.RemoveAll();
	m_b_checkFlag = false;
}

MD5_Check::~MD5_Check(void)
{
}


/*****************************************************************************
* Function name: InsertStringIntoList     
* Summary      : CListBox
* Parameters   : void
*                void
* Return       : void
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2010/06/09          First version             
*****************************************************************************/
void MD5_Check::InsertStringIntoList(CString str_String)
{
	SYSTEMTIME time;
	CString str_time = _T("");
	int i_count = 0;

	/* Get the local time */
	GetLocalTime(&time);
	str_time.Format(_T("[%04d/%02d/%02d %02d:%02d:%02d]: "), 
		time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	
	/* Get the total number in the list control */
	/*i_count = m_ctl_list.GetCount();
	m_ctl_list.InsertString(i_count, str_time + str_String);
	m_ctl_list.SetCurSel(i_count);*/

	/* Write log */
	obj_logDispose.LogDispose_syslog(str_time + str_String);

	return;
}



/*****************************************************************************
* Function name: ConvertXML     
* Summary      : 转换XML文件中的路径名  
* Parameters   : 
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2010/06/09          First version             
*****************************************************************************/
//ytt 20150207 support fciv xml generator
//examle  1 normal xml format fro tim java MD5 XML
//   c:\tool\MD5         \abc.exe          =>COMBINE TO  c:\tool\MD5\abc.exe then write to new XML  
//example 2  xml generator by fciv      
//   c:\tool\MD5          abc.exe

//将由str_XMLFilePathName指定的路径的MD5.xml文件中的DIRECTORY_PATH之前部分用str_newName来替换
bool MD5_Check::ConvertXML(CString str_XMLFilePathName, CString str_newName, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	/* 检查输入参数 */

	if(str_XMLFilePathName == _T("") || str_newName == _T(""))
	{
		return false;
	}

	if(str_newName.Right(1) == _T("\\"))
	{
		str_newName = str_newName.Left(str_newName.GetLength()-1);
	}

	/* 获取要查找的字符串（m_str_dirPath） */
	CString str_find = m_str_dirPath;

	::CoInitialize(NULL);
	MSXML2::IXMLDOMDocumentPtr pDoc;
	HRESULT hr;
	
	hr = pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument));
	if(FAILED(hr)) 
	{ 
		return FALSE;
	}

	// 加载XML文件
	if(_taccess(str_XMLFilePathName, 0) != 0) {
		return false;
	}
	pDoc->load((LPCTSTR)str_XMLFilePathName);
	if(pDoc == NULL) {
		return false;
	}

	// 获取FCIV节点
	XMLNode pNode_all;
	pNode_all = pDoc->selectSingleNode(_T("//FCIV"));

	if(pNode_all == NULL)
	{
		return false;
	}

	XMLNodeList nodeList;
	pNode_all->get_childNodes(&nodeList);
	if(nodeList == NULL)
	{
		return false;
	}
	long i_NodeNum = 0;
	nodeList->get_length(&i_NodeNum);
	i_NodeNum = i_NodeNum - 1;

	for(int j = i_NodeNum; j >= 0; j--)
	{
		// 获取最后一个FCIV节点
	    XMLNode pItem;
		nodeList->get_item(j,&pItem);
		if(pItem == NULL)
			return false;

		// 获取name节点
		XMLNode pNode;
		pNode = pItem->selectSingleNode(_T("name"));
		if(pNode == NULL) 
		{
			return false;
		}
		CString str_nameValue;
		str_nameValue = (TCHAR *)pNode->text;
		//if(str_nameValue.Find('\\') == 0)
		//{
		//	str_nameValue = str_nameValue.Mid(1);
		//}
		if(str_nameValue.Left(1) != '\\')
		{
			str_nameValue =  CString('\\') + str_nameValue ;	
		}
		str_nameValue = str_newName + str_nameValue;
		//str_nameValue.MakeUpper().Replace(str_find,str_newName);
		pNode->Puttext((_bstr_t)(const TCHAR *)str_nameValue);

	}

	// 保存XML文件
	pDoc->save((LPCTSTR)str_XMLFilePathName);
	//::CoUninitialize();

	return true;
}

/*****************************************************************************
* Function name: GetToolfilespath     
* Summary      : get Tool files path
* Parameters   : IN  LPCTSTR pstr Tool(model) path
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Tom.Li        2011/07/12          First version             
*****************************************************************************/
//bool MD5_Check::GetToolfilespath(LPCTSTR pstr, char* sz_ErrCode, char* sz_ErrMsg)
//{
//	bool b_res = false;
//
//	CFileFind finder;
//	if(pstr == "")
//	{
//		sz_ErrCode = "001";
//		sz_ErrMsg = "The Tool path is NULL, Tool Path 路径为空";
//		return false;
//
//		CString strWildcard(pstr);
//        strWildcard += _T("\\*.*");
//		b_res = finder.FindFile(strWildcard);
//		if(b_res == false)
//		{
//			finder.Close();
//		}
//		else
//		{
//			while(b_res)
//			{
//				bool b_test = finder.FindNextFile();//检查文件是否检查完
//				if(b_test)
//				{
//					m_stra_Toolpath.Add(finder.GetFilePath());
//				}
//
//				if(finder.IsDots())
//				continue;
//
//				if(finder.IsDirectory())
//				{
//					CString str = finder.GetFilePath();
//                    b_res = Recurse(str,sz_ErrCode,sz_ErrMsg);
//					if(b_res)
//						return true;
//				}
//			}
//		}
//		return b_res;
//	}
//
//	return b_res;
//}
//
//
//
//

/*****************************************************************************
* Function name: VerifyHash     
* Summary      : Hash验证  
* Parameters   : 
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2010/06/09          First version             
*****************************************************************************/
bool MD5_Check::VerifyHash(CString str_dirPath, CString str_XMLFilePath, CString str_logFile, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	CString str_log = _T("");
	str_log = _T("函数VerifyHash根据文件") + str_XMLFilePath + _T("验证") + str_dirPath;
	m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);

	/* Check Input */
	if(str_dirPath == _T("") || str_XMLFilePath == _T("") || str_logFile == _T("")) 
	{
		m_Clog.WriteLog(_T("MD5:dirPath || XMLFilePath || logFile -> NULL  "),sz_ErrCode,sz_ErrMsg);
		return false;
	}
	
	/* 检查fciv.exe是否存在 */
	CString str_fcivPathName;
	str_fcivPathName =  _T(".\\fciv.exe");
	if (_taccess(str_fcivPathName, 0) != 0) {
		InsertStringIntoList(_T("C015 - fciv.exe is not found in app directory path.\r\n"));
		//obj_messageBox.SetMessage(_T("C015"));
		//obj_messageBox.DoModal();
		m_Clog.WriteLog(_T("MD5:C015 - fciv.exe is not found in app directory path.\r\n"),sz_ErrCode,sz_ErrMsg);
		return false;
	}

	/* Command */
	CString str_winExecCommand;
	CString str_commandOne;         // cmd.exe
	CString str_commandTwo;         // fciv.exe
	CString str_commandThree;       // log file
	str_commandOne = _T("cmd.exe /c ");
	//str_commandTwo = str_fcivPathName + _T(" -r -v ") + str_dirPath + _T(" -xml ") + str_XMLFilePath + _T("\\") + str_XMLFileName;
	str_commandTwo = str_fcivPathName + _T(" -r -v ") + str_dirPath + _T(" -xml ") + str_XMLFilePath;
	str_commandThree = str_commandThree + _T(" > ") + str_logFile;
	str_winExecCommand = str_commandOne + str_commandTwo + str_commandThree;

	/* Command CString -> char* */
	TCHAR sz_commandLine[1024];
	memset(sz_commandLine, 0, sizeof(sz_commandLine));
	for (int i = 0; i < str_winExecCommand.GetLength(); i ++)
	{
		sz_commandLine[i] = str_winExecCommand[i];
	}

	/* Excute fciv.exe */
	m_Clog.WriteLog(sz_commandLine,sz_ErrCode,sz_ErrMsg);
	InsertStringIntoList(_T("Exucute fciv.exe start.\r\n"));
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb          = sizeof(si);
	si.wShowWindow = SW_HIDE;
	si.dwFlags     = STARTF_USESHOWWINDOW;

	BOOL b_createRes = FALSE;	
	b_createRes = CreateProcess(NULL, sz_commandLine, NULL, NULL, NULL, NULL, NULL, NULL, &si, &pi);
	if(b_createRes == FALSE) 
	{
		InsertStringIntoList(_T("Exucute fciv.exe fail.\r\n"));
		m_Clog.WriteLog(_T("MD5:Exucute fciv.exe fail."),sz_ErrCode,sz_ErrMsg);
		return false;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);

	Sleep(1000);
	InsertStringIntoList(_T("Exucute fciv.exe end.\r\n"));
	return true;
}




/*****************************************************************************
* Function name: CheckVerifyResult     
* Summary      : 解析Hash结果  
* Parameters   : 
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2010/06/09          First version             
*****************************************************************************/
bool MD5_Check::CheckVerifyResult(CString str_logFile, int& i_result, CString& str_errorfile, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	CString str_log = _T("");
	str_log = _T("函数CheckVerifyResult根据文件") + str_logFile + _T("验证MD5的结果");
	m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);

	/* Check Input */
	if(str_logFile == _T("")) 
	{
		m_Clog.WriteLog(_T("MD5:logFile path null"),sz_ErrCode,sz_ErrMsg);
		return false;
	}

	int i_existentResult = 0;
	int i_succeedFlag = 0;
	int i_retry = 30;            // 30秒

	// 检查HASHTABLERESULTFILE是否存在
	while(1) 
	{
		//str_logFile = test_prj_Ini_Files_v1.txt
		i_existentResult = _taccess(str_logFile, 0);
		if(i_existentResult != 0) 
		{
			if(i_retry <= 0)
			{
				break;
			} 
			else
			{
				Sleep(1000);
				i_retry--;
				continue;
			}
		} 
		else 
		{
			break;
		}
	}
	if(i_existentResult != 0) {
		CString str_tmp;
		str_tmp.Format(_T("C016 - %s is not found in the LOG directory.\r\n"), str_logFile);
		InsertStringIntoList(str_tmp);
		m_Clog.WriteLog(str_tmp,sz_ErrCode,sz_ErrMsg);
		return false;
	}

	// 检查文件大小
	DWORD dw_fileSize = 0;
	i_retry = 30;
	while(1) {
		HANDLE h_handle = NULL;
		h_handle = CreateFile(str_logFile, FILE_READ_EA, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
		if(h_handle != INVALID_HANDLE_VALUE){
			dw_fileSize = GetFileSize(h_handle, NULL);
			CloseHandle(h_handle);
			if(dw_fileSize == 0) {
				if(i_retry <= 0) {
					break;
				} else {
					Sleep(1000);
					i_retry--;
					continue;
				}
			} else {
				Sleep(1000);
				break;
			}
		} else {
			break;
		}
	}

	if(dw_fileSize == 0) {
		CString str_tmp;
		str_tmp.Format(_T("C018 - %s file size is 0 .\r\n"), str_logFile);
		m_Clog.WriteLog(str_tmp,sz_ErrCode,sz_ErrMsg);
		InsertStringIntoList(str_tmp);
		return false;
	}

	// 打开文件HASHTABLERESULTFILE
	CStdioFile obj_stdioFile;
	if(!obj_stdioFile.Open(str_logFile, CFile::modeRead
		|CFile::modeNoTruncate|CFile::shareDenyNone, NULL)) {
			CString str_tmp;
			str_tmp.Format(_T("C017 - Open %s failure.\r\n"), str_logFile);
			InsertStringIntoList(str_tmp);
			m_Clog.WriteLog(_T("C017 - Open %s failure."),sz_ErrCode,sz_ErrMsg);
			return false;
	} 

	// 读文件HASHTABLERESULTFILE
	try {
		CString str_string;
		while(obj_stdioFile.ReadString(str_string)) 
		{
			m_Clog.WriteLog((LPCTSTR)str_string,sz_ErrCode,sz_ErrMsg);
			if(str_string.Find(_T(VERIFYSUCCEEDSTRING)) != -1) 
			{
				m_Clog.WriteLog(_T("md5 result=successfully."),sz_ErrCode,sz_ErrMsg);
				i_succeedFlag = 1;
				break;
			}
			if(str_string.Find(_T("List of modified files:")) != -1)
			{
				m_Clog.WriteLog(_T("md5 result=List of modified files:."),sz_ErrCode,sz_ErrMsg);
				i_succeedFlag = 2;
			}

			if(str_string.Find(_T("\\")) != -1 && i_succeedFlag == 2)
			{
				str_errorfile = str_string;
				if(str_errorfile.ReverseFind('\n') == (str_errorfile.GetLength() - 1))
				{
					str_errorfile = str_errorfile.Mid(0,str_errorfile.GetLength() - 1);
				}

				if(str_errorfile.ReverseFind('\r') == (str_errorfile.GetLength() - 1))
				{
					str_errorfile = str_errorfile.Mid(0,str_errorfile.GetLength() - 1);
				}
				break;
			}

			str_string = _T("");
		}
	} catch(CFileException *e) {
		e->Delete();
	}

	i_result = i_succeedFlag;
	obj_stdioFile.Close();

	return true;
}






/*****************************************************************************
* Function name: ReadIniFile     
* Summary      : Read .\\INI\\setup.ini
* Parameters   : IN  null,    OUT sz_ModelName,str_SoftVersion,str_ServerPath
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Tom Li         2011/07/06          First version             
*****************************************************************************/
bool MD5_Check::ReadIniFile(CString& str_ModelName, CString& str_SoftVersion, CString& str_ServerPath)
{

	str_SoftVersion = _T("");
	str_ServerPath = _T("");
	str_ModelName = _T("");

	/* 检查ini文件是否存在 */
	int i_existentResult = 0;
	i_existentResult = _taccess(INIFILEPASH, 0);
	if(i_existentResult != 0) {
		InsertStringIntoList(_T("C001 - .\\INI\\setup.ini is not existent.\r\n"));
		return false;
	} 

	/* 读取INI文件 */
	TCHAR sz_buffer[128];
	memset(sz_buffer, 0, sizeof(sz_buffer));
	::GetPrivateProfileString(_T("Model_Name"), _T("MODEL"), _T(""), sz_buffer, sizeof(sz_buffer), INIFILEPASH);
	str_ModelName.Format(_T("%s"),sz_buffer);

	memset(sz_buffer, 0, sizeof(sz_buffer));
	::GetPrivateProfileString(_T("SW_VERSION"), _T("VERSION"), _T(""), sz_buffer, sizeof(sz_buffer), INIFILEPASH);
	str_SoftVersion.Format(_T("%s"),sz_buffer);

	memset(sz_buffer, 0, sizeof(sz_buffer));
	::GetPrivateProfileString(_T("SERVER_INFO"), _T("SERVER_PATH"), _T(""), sz_buffer, sizeof(sz_buffer), INIFILEPASH);
	str_ServerPath.Format(_T("%s"),sz_buffer);

	if (str_ModelName == _T("") || str_SoftVersion == _T("") || str_ServerPath == _T("")) 
	{
		InsertStringIntoList(_T("C002 - Get the pathname fail from .\\INI\\setup.ini.\r\n"));
		return false;
	}

	/* 去除Path最后的"\\". 目的：确保路径最后没有"\\",后面会统一添加"\\" */
	if (str_ServerPath.GetAt(str_ServerPath.GetLength() - 1) == _T('\\')) {
		try {
			str_ServerPath = str_ServerPath.Left(str_ServerPath.GetLength() - 1);
		} catch (CMemoryException e) {
			e.Delete();
			return false;
		}
	}

	if (str_SoftVersion.GetAt(str_SoftVersion.GetLength() - 1) == _T('\\')) {
		try {
			str_SoftVersion = str_SoftVersion.Left(str_SoftVersion.GetLength() - 1);
		} catch (CMemoryException e) {
			e.Delete();
			return false;
		}
	}
	return true;
}



/*****************************************************************************
* Function name: CheckServerPathValid     
* Summary      : 检查Server path是否有效path
* Parameters   : void
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2010/06/09          First version             
*****************************************************************************/
bool MD5_Check::CheckServerPathValid(void)
{
	/* 分割m_str_serverPath */
	int i_splitStringRes = 0;
	CStringArray stra_pathName;
	i_splitStringRes = obj_dirDispose.SpiltString(m_str_serverPath, _T("\\"), stra_pathName);
	if (i_splitStringRes != OK)
	{
		CString str_tempInfo;
		str_tempInfo.Format(_T("C003 - %s is a invalid server path.\r\n"), m_str_serverPath);
		InsertStringIntoList(str_tempInfo);
		//obj_messageBox.SetMessage(_T("C003"));
		//obj_messageBox.DoModal();
		return false;
	}

	/* 检查m_str_serverPath是不是以\\开头的路径 */
	/*if (stra_pathName.GetSize() < 2) {
		CString str_tempInfo;
		str_tempInfo.Format(_T("C003 - %s is a invalid server path.\r\n"), m_str_serverPath);
		InsertStringIntoList(str_tempInfo);
		obj_messageBox.SetMessage(_T("C003"));
		obj_messageBox.DoModal();
		return false;
	}*/
	/*if(stra_pathName.GetAt(0) != _T("") && stra_pathName.GetAt(1) != _T("")) {
		CString str_tempInfo;
		str_tempInfo.Format(_T("C003 - %s is a invalid server path.\r\n"), m_str_serverPath);
		InsertStringIntoList(str_tempInfo);
		obj_messageBox.SetMessage(_T("C003"));
		obj_messageBox.DoModal();
		return false;
	}*/

	/* 判断本地PC是否能连接上m_str_serverPath（相当于ping）--- 通过获取服务器的主机名或IP,是否成功的方法 */
	CString str_tempName;
	str_tempName = stra_pathName.GetAt(1);
	//str_tempName = stra_pathName.GetAt(2);
	/*CString str_IPAddr;
	CString str_hostName;
	GetIPByHostName(str_tempName, str_IPAddr);
	GetHostNameByIP(str_tempName, str_hostName);
	if (str_IPAddr == _T("") && str_hostName == _T("")) {
		CString str_tempInfo;
		str_tempInfo.Format(_T("W001 - Server \\\\%s is not connected to Internet.\r\n"), str_tempName);
		InsertStringIntoList(str_tempInfo);
		obj_messageBox.SetMessage(_T("W001"));
		obj_messageBox.DoModal();
		return false;
	}
	CString str_tempInfo;
	str_tempInfo.Format(_T("Server \\\\%s have been connected to Internet.\r\n"), str_tempName);
	InsertStringIntoList(str_tempInfo);

	/* 检查m_str_dirPath和m_str_hashPath是否存在 */
	CString str_tempPath = _T("");
	str_tempPath = m_str_serverPath + m_str_hashPath;
	int i_existDir = 0;
	i_existDir = obj_dirDispose.IsDirectoryExist(str_tempPath);
	if (i_existDir != OK) 
	{
		CString str_tempInfo;
		str_tempInfo.Format(_T("C004 - %s is not existent.\r\n"), str_tempPath);
		InsertStringIntoList(str_tempInfo);
		return false;
	}

	str_tempPath = m_str_serverPath + m_str_dirPath;
	i_existDir = obj_dirDispose.IsDirectoryExist(str_tempPath);
	if(i_existDir != OK) 
	{
		CString str_tempInfo;
		str_tempInfo.Format(_T("C004 - %s is not existent.\r\n"), str_tempPath);
		InsertStringIntoList(str_tempInfo);
		return false;
	}

	return true;
}
bool MD5_Check::AddMapDiskServer(CString str_disk, CString str_server)
{
	if ((str_disk == _T("")) || (str_server == _T("")))
	{
		return false;
	}

	//定义网络资源
	NETRESOURCE nsource;
	nsource.dwScope = RESOURCE_GLOBALNET;
	nsource.dwType = RESOURCETYPE_DISK;
	nsource.dwDisplayType = RESOURCEDISPLAYTYPE_GENERIC;
	nsource.dwUsage = RESOURCEUSAGE_CONNECTABLE;
	nsource.lpLocalName = (LPSTR)(LPCSTR)str_disk;
	nsource.lpRemoteName = (LPSTR)(LPCSTR)str_server;
	nsource.lpProvider = NULL;
	nsource.lpComment = NULL;

	//添加网络资源
	DWORD ret;
	ret =  WNetAddConnection2(&nsource, NULL, NULL, 0);
	if (ret == NO_ERROR)
	{
		return true;
	} 
	else 
	{
		return false;
	}

	return true;
}

bool MD5_Check::CheckDiskMapServer(CString str_path, CString& str_destPath)
{
	if (str_path == _T("")) 
	{
		return false;
	}

	CString str_serverDir;
	try {
		CString str_temp;
		str_temp = str_path.Mid(0, 2);
		if (str_temp == _T("\\\\")) 
		{
			CStringArray stra_dir;
			if (obj_dirDispose.SpiltString(str_path, _T("\\"), stra_dir) != OK) 
			{
				return NG;
			} 
			else
			{
				int i_num = stra_dir.GetSize();
				if (stra_dir.GetSize() < 4)
				{
					return NG;
				}
				str_serverDir = _T("\\");
				for (int i = 2; i <= 3; i++) 
				{
					str_serverDir = str_serverDir + _T("\\") + stra_dir.GetAt(i);
				}
				str_path.Replace(str_serverDir, DISKMAP);
				str_destPath = str_path;
				if (m_b_checkFlag != true) {
					if (AddMapDiskServer(DISKMAP, str_serverDir) != true)
					{
						if (CancelMapDiskServer(DISKMAP) != true) 
						{
							return false;
						} 
						else 
						{
							if (AddMapDiskServer(DISKMAP, str_serverDir) != true)
							{
								return false;
							} 
							else 
							{
								m_b_checkFlag = true;
							}
						}
					} 
					else 
					{
						m_b_checkFlag = true;
					}
				}
			}
		} 
		else 
		{
			str_destPath = str_path;
		}
	} 
	catch(...)
	{
		return NG;
	}

	return true;
}


bool MD5_Check::CancelMapDiskServer(CString str_disk)
{
	if (str_disk == _T("")) 
	{
		return false;
	}

	DWORD ret;
	ret =  WNetCancelConnection2(str_disk, CONNECT_UPDATE_PROFILE, FALSE);
	if (ret == NO_ERROR)
	{
		return true;
	} 
	else
	{
		return false;
	}

	return true;
}



/*************************************************************************************************************************
* Function name: GenerateHash(CString str_dirPath, CString str_XMLFilePath, CString str_XMLFileName, CString str_logFile)     
* Summary      : 生成Hash文件
* Parameters   : IN       CString str_dirPath               The directory to generate Hash files.
IN       CString str_XMLFilePath           The directory of Hash file. 
IN       CString str_XMLFileName           The name of Hash file.
IN       CString str_logFile               The name of Log File.
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Tom.Li        2011/09/08          First version             
*****************************************************************************************************************************/
bool MD5_Check::GenerateHash(CString str_dirPath, CString str_XMLFilePath, CString str_XMLFileName, CString str_logFile, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	CString str_log = _T("");
	str_log = _T("函数GenerateHash根据文件") + str_XMLFileName + _T("验证MD5的结果");
	m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);

	/* Check Input */
	if(str_dirPath == _T("") || str_XMLFilePath == _T("") || str_XMLFileName == _T("") || str_logFile == _T("")) 
	{
		return false;
	}


	/* 检查fciv.exe是否存在 */
	CString str_fcivPathName;
	str_fcivPathName =  _T(".\\fciv.exe");
	if (_taccess(str_fcivPathName, 0) != 0) 
	{
		InsertStringIntoList(_T("C015 - fciv.exe is not found in app directory path.\r\n"));
		CString str_ErrCode = _T("ET0038");
		CString str_ErrMsg = _T("fciv.exe is not found in app directory path.\n 当前工具目录中没有fciv.exe");
		strcpy_s(sz_ErrCode,str_ErrCode.GetLength(),str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,str_ErrMsg.GetLength(),str_ErrMsg.GetBuffer());
		return false;
	}

	//ytt support MD5 gen by fciv.exe  , ex: path from C:\Tool\GT05B1A_F52\ to  C:\Tool\GT05B1A_F52
    //if(str_dirPath.Right(1) == '\\')
	//	str_dirPath = str_dirPath.Left(str_dirPath.GetLength()-1);
	/* Command */
	CString str_winExecCommand;
	CString str_commandOne;     // cmd.exe
	CString str_commandTwo;     // fciv.exe
	CString str_commandThree;   // log file
	str_commandOne = _T("cmd.exe /c ");
	str_commandTwo = str_fcivPathName + _T(" -r ") + str_dirPath + _T(" -xml ") + str_XMLFilePath + _T("\\") + str_XMLFileName;
	str_commandThree = str_commandThree + _T(" > ") + str_logFile;
	str_winExecCommand = str_commandOne + str_commandTwo + str_commandThree;


	CString str_MD5_File = str_XMLFilePath + _T("\\") + str_XMLFileName;
	if( (_access(str_MD5_File, 0)) != -1 )
	{
		//File exist, delete file
		int n_check = 0;
		n_check = DeleteFile(str_MD5_File);
		if(n_check == 0)
			return false;
	}

	/* Command CString -> char* */
	TCHAR sz_commandLine[1024];
	memset(sz_commandLine, 0, sizeof(sz_commandLine));
	for (int i = 0; i < str_winExecCommand.GetLength(); i ++) 
	{
		sz_commandLine[i] = str_winExecCommand[i];
	}


	/* Excute fciv.exe */
	m_Clog.WriteLog(sz_commandLine,sz_ErrCode,sz_ErrMsg);
	InsertStringIntoList(_T("Exucute fciv.exe start.\r\n"));
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb          = sizeof(si);
	si.wShowWindow = SW_HIDE;
	si.dwFlags     = STARTF_USESHOWWINDOW;

	BOOL b_createRes = FALSE;	
	b_createRes = CreateProcess(NULL, sz_commandLine, NULL, NULL, NULL, NULL, NULL, NULL, &si, &pi);
	if(b_createRes == FALSE) {
		InsertStringIntoList(_T("Exucute fciv.exe fail.\r\n"));
		return false;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);

	Sleep(1000);
	InsertStringIntoList(_T("Exucute fciv.exe end.\r\n"));

	return true;

}


/*************************************************************************************************************************
* Function name: VerifyGeneratedHash(CString str_Hash_Generate, CString str_Hash_Server, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
* Summary      : Compare Hash files between generated files and file got from server
* Parameters   : IN       CString str_Hash_Generate         The directory of generated Hash files.
				 IN       CString str_Hash_Server           The directory of Hash file from server. 
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Tom.Li        2011/09/08          First version             
*****************************************************************************************************************************/
bool MD5_Check::VerifyGeneratedHash(CString str_Hash_Generate, CString str_Hash_Server, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	bool bres = false;
	if(m_obj_XML_GenerateHash.Load(str_Hash_Generate) == ERROR_SUCCESS)
	{
		bres = true;
	}
	else
	{
		return false;
	}

	if(m_obj_XML_ServerHash.Load(str_Hash_Server) == ERROR_SUCCESS)
	{
		bres = true;
	}
	else
	{
		return false;
	}

	//获取Doc
	XMLDoc Doc_generate;
	XMLDoc Doc_Server;
	Doc_generate = m_obj_XML_GenerateHash.GetDoc();
	Doc_Server = m_obj_XML_ServerHash.GetDoc();

	// 获取FCIV节点
	XMLNode pNode_all_Generate;
	XMLNode pNode_all_Server;
	long l_num_Generate = 0;
	long l_num_Server = 0;
	pNode_all_Generate = Doc_generate->selectSingleNode(_T("//FCIV"));
	if(pNode_all_Generate == NULL)
	{
		m_Clog.WriteLog(_T("check FCIV word from generated XML"),sz_ErrCode,sz_ErrMsg);
		return false;
	}

	pNode_all_Server = Doc_Server->selectSingleNode(_T("//FCIV"));
	if(pNode_all_Server == NULL)
	{
		m_Clog.WriteLog(_T("check FCIV word from server XML"),sz_ErrCode,sz_ErrMsg);
		return false;
	}


	bres = m_obj_XML_GenerateHash.GetDocNum(pNode_all_Generate,l_num_Generate);
	if(!bres)
	{
		m_Clog.WriteLog(_T("get node from generated XML"),sz_ErrCode,sz_ErrMsg);
		return false;
	}

	bres = m_obj_XML_ServerHash.GetDocNum(pNode_all_Server,l_num_Server);
	if(!bres)
	{
		m_Clog.WriteLog(_T("get node from server XML"),sz_ErrCode,sz_ErrMsg);
		return false;
	}

	if(l_num_Generate != l_num_Server)
	{
		m_Clog.WriteLog(_T("node number not match between two xml "),sz_ErrCode,sz_ErrMsg);
		return false;
	}
   
	//CString[] stra_Generate = 
	CStringArray stra_Generate_Name;
	CStringArray stra_Generate_MD5;
	CStringArray stra_Server_Name;
	CStringArray stra_Server_MD5;
	stra_Generate_Name.RemoveAll();
	stra_Generate_MD5.RemoveAll();
	stra_Server_Name.RemoveAll();
	stra_Server_MD5.RemoveAll();

	XMLNodeList nodeList;
	pNode_all_Generate->get_childNodes(&nodeList);
	if(nodeList == NULL)
	{
		m_Clog.WriteLog(_T("child nodes not found in generated xml"),sz_ErrCode,sz_ErrMsg);
		return false;
	}
	for(int i = 0 ; i < l_num_Generate; i++)
	{
	
		XMLNode pNode;
		XMLNodeList subnodeList;
		CString str_Name = _T("");
		CString str_MD5 = _T("");

		nodeList->get_item(i,&pNode);
		if(pNode == NULL)
			continue;
		
		//subnodeList
		pNode->get_childNodes(&subnodeList);
		if(subnodeList == NULL)
			continue;
		m_obj_XML_GenerateHash.GetNodeListItem(subnodeList,str_Name,str_MD5);
		stra_Generate_Name.Add(str_Name);
		stra_Generate_MD5.Add(str_MD5);
	}


	XMLNodeList nodeList_Server;
	pNode_all_Server->get_childNodes(&nodeList_Server);
	if(nodeList == NULL)
	{
		m_Clog.WriteLog(_T("child nodes not found in server xml"),sz_ErrCode,sz_ErrMsg);
		return false;
	}
	for(int i = 0 ; i < l_num_Server; i++)
	{
	
		XMLNode pNode;
		XMLNodeList subnodeList;
		CString str_Name = _T("");
		CString str_MD5 = _T("");

		nodeList_Server->get_item(i,&pNode);
		if(pNode == NULL)
			continue;
		
		//subnodeList
		pNode->get_childNodes(&subnodeList);
		if(subnodeList == NULL)
			continue;
		m_obj_XML_ServerHash.GetNodeListItem(subnodeList,str_Name,str_MD5);
		stra_Server_Name.Add(str_Name);
		stra_Server_MD5.Add(str_MD5);
	}

	int i_array_Generated = 0;
	int i_array_Server = 0;
	i_array_Generated = stra_Generate_Name.GetSize();
	i_array_Server = stra_Server_Name.GetSize();

	if(i_array_Generated != i_array_Server)
	{
		m_Clog.WriteLog(_T("name number not match between two xml"),sz_ErrCode,sz_ErrMsg);
		
		return false;
	}
	for(int ij = 0; ij < i_array_Generated; ij++)
	{
		CString str_temp_generated = _T("");
		str_temp_generated = stra_Generate_MD5.GetAt(ij);
		for(int ik = 0; ik < i_array_Server; ik++)
		{
			CString str_temp_Server = _T("");
			str_temp_Server = stra_Server_MD5.GetAt(ik);
			if(str_temp_generated.MakeUpper() == str_temp_Server.MakeUpper())
			{
				CString str_name_generate = _T("");
				CString str_name_Server = _T("");
				str_name_generate = stra_Generate_Name.GetAt(ij);
				str_name_Server = stra_Server_Name.GetAt(ik);
				if(str_name_generate.MakeUpper() == str_name_Server.MakeUpper())
				{
					CString str_log = _T("");
				    str_log.Format(_T("文件名%s,MD5值为服务器%s，本地产生的%s"),str_name_generate,str_temp_Server,str_temp_generated);
		            m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);

					break;
				}

			}

			if(ik == (i_array_Server - 1))
			{
				CString str_ErrCode = _T("ET0022");
				CString str_ErrMsg = _T("The MD5 files are not the same between Server and generate\n");
				str_ErrMsg = str_ErrMsg + _T("服务器上的MD5文件") + stra_Server_Name.GetAt(ij) + _T("与生成的MD5文件不一致");
				strcpy_s(sz_ErrCode,str_ErrCode.GetLength() + 1,str_ErrCode.GetBuffer());
				strcpy_s(sz_ErrMsg,str_ErrMsg.GetLength() + 1,str_ErrMsg.GetBuffer());
				str_ErrCode.ReleaseBuffer();
				str_ErrMsg.ReleaseBuffer();
				return false;
			}
		}
	}




	return bres;
}



bool MD5_Check::ReadUiXML(CString str_XMLFilePathName,CString& str_newName,TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	if(str_XMLFilePathName == _T(""))
	{
		return false;
	}
	::CoInitialize(NULL);
	MSXML2::IXMLDOMDocumentPtr pDoc;
	HRESULT hr;

	hr = pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument26));
	if(FAILED(hr)) 
	{ 
		return FALSE;
	}

	// 加载XML文件
	if(_taccess(str_XMLFilePathName, 0) != 0) {
		return false;
	}
	pDoc->load((LPCTSTR)str_XMLFilePathName);
	if(pDoc == NULL) {
		return false;
	}

	// 获取FCIV节点
	XMLNode pNode_all;
	pNode_all = pDoc->selectSingleNode(_T("//FCIV"));

	if(pNode_all == NULL)
	{
		return false;
	}

	XMLNodeList nodeList;
	pNode_all->get_childNodes(&nodeList);
	if(nodeList == NULL)
	{
		return false;
	}
	long i_NodeNum = 0;
	nodeList->get_length(&i_NodeNum);
	i_NodeNum = i_NodeNum - 1;

	for(int j = i_NodeNum; j >= 0; j--)
	{
		// 获取最后一个FCIV节点
		XMLNode pItem;
		nodeList->get_item(j,&pItem);
		if(pItem == NULL)
			return false;

		// 获取md5节点
		XMLNode pNode;
		pNode = pItem->selectSingleNode(_T("MD5"));
		if(pNode == NULL) 
		{
			return false;
		}
		str_newName = (TCHAR *)pNode->text;

	}
	return true;
}

/*************************************************************************************************************************
* Function name: Do_MD5_Check(CString str_filecheck, CString str_Hashpath, CString str_Replace, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
* Summary      : Do MD5 check
* Parameters   : IN       CString str_filecheck             The directory of files to be checked.
				 IN       CString str_Hashpath              The directory of Hash file in LOC. 
				 IN       CString str_Replace               The replaced string.
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Tom.Li        2011/09/08          First version             
*****************************************************************************************************************************/
bool MD5_Check::Do_MD5_Check(CString str_filecheck, CString str_Hashpath, CString str_Replace, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	//::MessageBox(NULL,_T("MD5_CHECK成"),_T("提示"),MB_OK);
	if(str_filecheck.IsEmpty() || str_Hashpath.IsEmpty())
	{
		CString str_ErrCode = _T("ET0007");
		CString str_ErrMsg = _T("MD5传入路径为空");
		strcpy_s(sz_ErrCode,512,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,512,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}

	/* 判断本地目录路径是否有效路径 */
	int i_checkDirValidRes = 0;
	i_checkDirValidRes = obj_dirDispose.CheckDirectoryValid(str_filecheck);
	if(i_checkDirValidRes != OK)
	{
		CString str_tmp;
		str_tmp.Format(_T("C007 - Local directory path contains invalid chars: %s\r\n"), FILENAME_ILLEGIMATE_CHARACTER);
		InsertStringIntoList(str_tmp);
		m_Clog.WriteLog(str_tmp,sz_ErrCode,sz_ErrMsg);
		sz_ErrCode = _T("C007");
		sz_ErrMsg = _T("Local directory path is empty.\r\n");
		return false;
	}


	/* 去掉str_filecheck最后的"\\" */
	if (str_filecheck.GetAt(str_filecheck.GetLength() - 1) == _T('\\'))
	{
		try 
		{
			str_filecheck = str_filecheck.Left(str_filecheck.GetLength() - 1);
		} 
		catch (CMemoryException e)
		{
			e.Delete();
			return false;
		}
	}

	/* 判断本地目录路径是否存在 */
	int i_existDir = 0;
	i_existDir = obj_dirDispose.IsDirectoryExist(str_filecheck);
	if(i_existDir != OK) 
	{
		// Local directory path is not exist
		CString str_tempInfo;
		str_tempInfo.Format(_T("Local directroy path %s is not existent.\r\n"), str_filecheck);
		InsertStringIntoList(str_tempInfo);

		CString str_ErrCode = _T("ET0008");
		CString str_ErrMsg = _T("The checked file is not exist\r\n");
		str_ErrMsg = str_ErrMsg + _T("检查的路径") + str_filecheck + _T("不存在\r\n");
		strcpy_s(sz_ErrCode,512,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,512,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}


	CString str_hashFilePath = str_Hashpath;

	InsertStringIntoList(_T("Converting the hash file......, please wait.\r\n"));

	//str_hashFilePath = str_serverHashAbsolutePath + _T("//") +  sz_StationName +  _T("_") + _T("_MD5.xml");
	bool b_convertRes = false;
	//CString str_replace = _T(""); 
	m_str_dirPath = str_Replace;
	//CString str_newPath = _T("");
	b_convertRes = ConvertXML(str_hashFilePath, str_filecheck,sz_ErrCode, sz_ErrMsg);
	if(b_convertRes == false)
	{
		InsertStringIntoList(_T("Convert the hash file fail.\r\n"));
		CString str_ErrCode = _T("ET0012");
		CString str_ErrMsg = _T("Convert Hash file XML fail\n");
		str_ErrMsg = str_ErrMsg + _T("转换Hash文件XML失败");
		strcpy_s(sz_ErrCode,512,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,512,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}

	InsertStringIntoList(_T("Convert the hash file success.\r\n"));
	/* ===============================Convert XML End========================================================== */

	/* ===============================Verify hash Start===================================== */
	InsertStringIntoList(_T("Verifying the hash file......, please wait fciv.exe finish(Long time,Please wait...)\r\n"));
	CString str_verifyResult;

	CString str_pathName;

	/* Hash log filename */
	CString str_hashLogFileName = _T(".\\LOG\\");
	str_hashLogFileName = str_hashLogFileName + _T("MD5_log");
	str_hashLogFileName = str_hashLogFileName + _T(".txt");
	if(!obj_dirDispose.IsDirectoryExist(_T(".\\LOG")))
	{
		if(!obj_dirDispose.CreateDirectory(_T(".\\LOG")))
		{
			CString str_ErrCode = _T("ET0009");
			CString str_ErrMsg = _T("Create MD5 log file fail\n");
			str_ErrMsg = str_ErrMsg + _T("创建MD5 Log文件夹失败");
			strcpy_s(sz_ErrCode,512,str_ErrCode.GetBuffer());
			strcpy_s(sz_ErrMsg,512,str_ErrMsg.GetBuffer());
			str_ErrCode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			return false;
		}
	}
	else
	{
		if(!obj_dirDispose.DeleteDir(_T(".\\LOG")))
		{
			CString str_ErrCode = _T("ET0010");
			CString str_ErrMsg = _T("Delete MD5 Log file fail\n");
			str_ErrMsg = _T("删除MD5 Log文件夹失败");
			strcpy_s(sz_ErrCode,512,str_ErrCode.GetBuffer());
			strcpy_s(sz_ErrMsg,512,str_ErrMsg.GetBuffer());
			str_ErrCode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			return false;
		}
		else
		{
			if(!obj_dirDispose.CreateDirectory(_T(".\\LOG")))
			{
				CString str_ErrCode = _T("ET0009");
				CString str_ErrMsg = _T("创建MD5 Log文件夹失败");
				strcpy_s(sz_ErrCode,512,str_ErrCode.GetBuffer());
				strcpy_s(sz_ErrMsg,512,str_ErrMsg.GetBuffer());
				str_ErrCode.ReleaseBuffer();
				str_ErrMsg.ReleaseBuffer();
				return false;
			}
		}
	}

	/* Verify hash */
	bool b_verifyRes = false;
	b_verifyRes = VerifyHash(str_filecheck, str_hashFilePath, str_hashLogFileName,sz_ErrCode,sz_ErrMsg);
	if(b_verifyRes == false) 
	{
		obj_dirDispose.DeleteDir(str_verifyResult);
		obj_dirDispose.DeleteDir(str_verifyResult);
		InsertStringIntoList(_T("VerifyHash Failure.\r\n"));
		m_Clog.WriteLog(_T("VerifyHash Failure"),sz_ErrCode,sz_ErrMsg);
		return false;
	}
	else
	{
		/* Save hash result */
		int i_result = 0;
		bool b_result = false;
		CString str_ErrorFile;
		b_result = CheckVerifyResult(str_hashLogFileName, i_result, str_ErrorFile,sz_ErrCode,sz_ErrMsg);
		CString str_tmp;
		str_tmp.Format(_T("%d"), i_result);
		str_verifyResult = str_tmp;
		if(b_result == false) 
		{
			InsertStringIntoList(_T("CheckVerifyResult Failure.\r\n"));
			m_Clog.WriteLog(_T("CheckVerifyResult Failure"),sz_ErrCode,sz_ErrMsg);
			return false;
		}

		/* Check hash result */
		int i_count = 0;
		if(i_result == 1) 
		{
			CString str_copyInfo;
			//str_copyInfo.Format(_T("Verify hash %s success.\r\n"), str_localCopyAbsolutePath);
			InsertStringIntoList(str_copyInfo);

			CString str_log = _T("");
			str_log = _T("MD5检查成功");
			m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);
		} 
		else
		{
			if(i_result == 2)
			{
				if(!str_ErrorFile.IsEmpty())
				{
					CString str_copyInfo;
					InsertStringIntoList(str_copyInfo);
					CString str_Errcode = _T("ET0013");
					CString str_ErrMsg = str_ErrorFile + _T(" MD5检查失败");
					strcpy_s(sz_ErrCode,512,str_Errcode.GetBuffer());
					strcpy_s(sz_ErrMsg,512,str_ErrMsg.GetBuffer());
					str_Errcode.ReleaseBuffer();
					str_ErrMsg.ReleaseBuffer();
					return false;
				}

				CString str_log = _T("");
			    str_log = _T("MD5检查失败");
			    m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);
			 
				CString str_copyInfo;
				InsertStringIntoList(str_copyInfo);
				CString str_Errcode = _T("ET0012");
				CString str_ErrMsg = _T("Get MD5 fail\n");
				str_ErrMsg = str_ErrMsg + _T("MD5文件获取失败");
				strcpy_s(sz_ErrCode,512,str_Errcode.GetBuffer());
				strcpy_s(sz_ErrMsg,512,str_ErrMsg.GetBuffer());
				str_Errcode.ReleaseBuffer();
				str_ErrMsg.ReleaseBuffer();
				return false;
			}
			else
			{
					CString str_copyInfo;
					InsertStringIntoList(str_copyInfo);
					CString str_Errcode = _T("ET0011");
					CString str_ErrMsg = _T("MD5 check fail\n");
					str_ErrMsg = str_ErrMsg + _T("MD5检查失败");
					strcpy_s(sz_ErrCode,512,str_Errcode.GetBuffer());
					strcpy_s(sz_ErrMsg,512,str_ErrMsg.GetBuffer());
					str_Errcode.ReleaseBuffer();
					str_ErrMsg.ReleaseBuffer();
					return false;
			}

		}


	}

	/* ===============================Verify hash End===================================== */

	/* ===============================Generate hash Start===================================== */
	InsertStringIntoList(_T("Generating the hash file......, please wait fciv.exe finish\r\n"));
	m_Clog.WriteLog(_T("Generating the hash file......, please wait fciv.exe finish"),sz_ErrCode,sz_ErrMsg);
	/* Hash log filename */
	CString str_hashLogFileName_generate;
	str_hashLogFileName_generate.Format(_T("%s"), _T(".\\LOG\\"));
	str_hashLogFileName_generate = str_hashLogFileName_generate + _T("MD5_log_Generate.txt");


	/* Generate hash */
	bool b_genRes = false;
	CString str_localXmlFile;
	str_localXmlFile = "LocalGenerate_MD5.xml";
	//GenerateHash(CString str_dirPath, CString str_XMLFilePath, CString str_XMLFileName, CString str_logFile)
	CString str_Hash_Generate = _T("");
	str_Hash_Generate = str_hashFilePath.Left(str_hashFilePath.ReverseFind('\\'));
	b_genRes = GenerateHash(str_filecheck, str_Hash_Generate, str_localXmlFile, str_hashLogFileName_generate, sz_ErrCode, sz_ErrMsg);
	if(b_genRes == false) 
	{
		InsertStringIntoList(_T("GenerateHash Failure.\r\n"));
		m_Clog.WriteLog(_T("GenerateHash Failure"),sz_ErrCode,sz_ErrMsg);
		return false;
	}

	str_Hash_Generate = str_Hash_Generate + _T("\\") + str_localXmlFile;
	b_genRes = VerifyGeneratedHash(str_Hash_Generate, str_hashFilePath, sz_ErrCode, sz_ErrMsg);
	if(b_genRes == false) 
	{
		m_Clog.WriteLog(_T("VerifyGeneratedHash Failure"),sz_ErrCode,sz_ErrMsg);
		return false;
	}


	return true;

}