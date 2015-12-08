#include "StdAfx.h"
#include "Function.h"
#include <direct.h>


Function::Function(void)
{
}

Function::~Function(void)
{
}

//YTT 140903 FUNCTION ヘ玡や糷ヘ魁穓碝
bool Function::FindCompressedFile(TCHAR* SearchPath,TCHAR* keyword,TCHAR* CompressedType, TCHAR* OutFileName )
{
	CFileFind finder;
	CString strFileName;
	CString strDirectory= CString(SearchPath);
	strDirectory += _T("\\*.*");

	BOOL bWorking = finder.FindFile(strDirectory);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
//		File_Path = finder.GetFilePath();
		if (finder.IsDots())
			continue;
		if (finder.IsDirectory())
        {

			continue;
        }
		strFileName = finder.GetFileName();
		//AfxMessageBox(strFileName);
		//m_Clog.WriteLog(_T("CheckVerifyResult Failure"),sz_ErrCode,sz_ErrMsg);
		m_Clog.WriteLog(strFileName);
        if(-1 ==strFileName.Find(keyword))
		{
			continue;
		}
		int dotnum = strFileName.ReverseFind(_T('.'));
        int FileExtLen = strFileName.GetLength()- dotnum; 
        

		CString strFileExtName=strFileName.Right(FileExtLen);
		strFileExtName.MakeUpper();
		//AfxMessageBox(strFileExtName);
		if(strFileExtName.Compare(_T(".ZIP"))==0) 
		{
            CString outFilePath = finder.GetFilePath();
			_stprintf(OutFileName, _T("%s"), outFilePath);
           // _tcscpy_s (m_NewEntranceToolFolderPath,OutFileName);
			//AfxMessageBox(outFilePath);
			m_Clog.WriteLog(outFilePath);
		   //  AfxMessageBox(m_NewEntranceToolFolderPath);
			return true;
		}


	}

	return false;
}

bool Function::DoAutoUpdate(TCHAR* ToolVer, TCHAR* ServerPath)
{   
	bool b_res =false;
	m_Clog.WriteLog(_T("DoAutoUpdate"));
    //AfxMessageBox(CString("DoAutoUpdate"),MB_OK);
	//AfxMessageBox(CString(ToolVer), MB_OK);
	//AfxMessageBox(CString(ServerPath), MB_OK);
	
	//1. clean entrance.exe
	b_res = KillEntranceTool();

    //2. combine tool folder path
    //exmaple
	//Toover 2,018
    //ServerPath \\10.85.58.13\VerifySW\07.Tools\Factory_Tool\Entrance 
	//===>Toopath : \\10.85.58.13\VerifySW\07.Tools\Factory_Tool\Entrance\TLFT_Common_v2.018_Entrance
   
    CString strRemoteToolPath = ServerPath + CString(_T("\\TLFT_Common_")) + ToolVer + CString(_T("_Entrance"));
    //AfxMessageBox(CString(strRemoteToolPath), MB_OK);
	m_Clog.WriteLog(strRemoteToolPath);
	CString strLocalToolPath = CString(_T("D:\\ENTRANCE")) + CString(_T("\\TLFT_Common_")) + ToolVer + CString(_T("_Entrance"));
	m_Clog.WriteLog(strLocalToolPath);
    //AfxMessageBox(CString(strLocalToolPath), MB_OK);
	//3. check the folder is exist or not , then copy whole folder to d:\entrance
    b_res = DoCopy((TCHAR*)LPCTSTR(strRemoteToolPath),(TCHAR*)LPCTSTR(strLocalToolPath) );
	if(!b_res)
	{
		m_Clog.WriteLog(CString("DoCopy fail"));
		AfxMessageBox(CString("DoCopy fail"),MB_OK);
		return false;
	}
    
   //4. unzip TLFT_PRJMCommon_GNVRv2.018_BVRTEU_TLCAEntrance_HWSGMPP01.zip
   //	just check if  any zip file with keyword "2.018"
   //   delete zip
	//   FindCompressedFile
	TCHAR outFilePath[MAX_PATH];
	//V2.018 ->2.018
	CString strToolVer = (CString(ToolVer)).Mid(1);
	b_res = FindCompressedFile((TCHAR*)LPCTSTR(strLocalToolPath),(TCHAR*)LPCTSTR(strToolVer),_T("ZIP"),outFilePath );
	if(!b_res)
	{
		m_Clog.WriteLog(CString("FindCompressedFile fail"));
		AfxMessageBox(CString("FindCompressedFile fail"),MB_OK);

		return false;
	}
   

    b_res = DoUnzip(outFilePath,(TCHAR*)LPCTSTR(strLocalToolPath) );
	if(!b_res)
	{
		m_Clog.WriteLog(CString("DoUnzip fail"));
		AfxMessageBox(CString("DoUnzip fail"),MB_OK);
		return false;
	}
	 _tcscpy_s (m_NewEntranceToolFolderPath,(LPCTSTR)strLocalToolPath);

   //5. execute .\TLFT_PRJMCommon_GNVRv2.018_BVRTEU_TLCAEntrance_HWSGMPP01\TLFT_Common_v2.017_TLCAEntrance\entrance tool.exe
   //   enumerate the folder to find entrance.exe and its real path  
	return true;
}




bool Function::KillEntranceTool(void)
{   
	CString str_winExecCommand;
	CString str_commandOne;        
	CString str_commandTwo;         

	str_commandOne = _T("cmd.exe /c ");

	str_commandTwo = _T("taskkill /f /im EntranceTool.exe");
	str_winExecCommand = str_commandOne + str_commandTwo;

	/* Command CString -> char* */
	TCHAR sz_commandLine[1024];
	memset(sz_commandLine, 0, sizeof(sz_commandLine));
	for (int i = 0; i < str_winExecCommand.GetLength(); i ++)
	{
		sz_commandLine[i] = str_winExecCommand[i];
	}


	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb          = sizeof(si);
	si.wShowWindow = SW_HIDE;
	si.dwFlags     = STARTF_USESHOWWINDOW;

	BOOL b_createRes = FALSE;

	b_createRes = CreateProcess(NULL, sz_commandLine, NULL, NULL, NULL, NULL, NULL, NULL, &si, &pi);

	Sleep(1000);

	if(b_createRes == FALSE) 
	{   
        AfxMessageBox(_T("无法关闭EntranceTool.EXE的进程，请联系工程师"));
		return false;
	}
	else
	{  
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
		return true;
	}
}

bool Function::UpOnEntranceTool(void)
{
	return false;
}

bool Function::DoUnzip(TCHAR* str_Toolpath, TCHAR*  str_destpath )
{
	//check sub filename to support 7z/zip  
	//or directly using 7z.exe to unzip 7z/zip ?

	//CString str_log = _T("");
	//str_log = _T("将文件从") + str_Toolpath + _T("目录下解压到") + str_destpath;
	//m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);
	//AfxMessageBox(CString(_T("DoUnzip")), MB_OK);
	//AfxMessageBox(CString(str_Toolpath), MB_OK);
	//AfxMessageBox(CString(str_destpath), MB_OK);
	m_Clog.WriteLog(CString(_T("DoUnzip")));
	bool b_res = false;
	/* Check Input */
	if(str_Toolpath == _T("") || str_destpath == _T("")) 
	{
		return false;
	}

	/* 检查WinRAR.exe是否存在 */
	CString str_WinRARPathName;
	str_WinRARPathName =  _T(".\\WinRAR.exe");
	if (_taccess(str_WinRARPathName, 0) != 0) 
	{
		//sz_ErrCode = _T("ET0015");
		//sz_ErrMsg = _T("WinRAR.exe is not found.\r\nWinRAR.exe不存在");
		m_Clog.WriteLog(CString("WinRAR.exe is not found.WinRAR.exe不存在"));
		AfxMessageBox(_T("WinRAR.exe is not found.WinRAR.exe不存在"));
		
		return false;
	}

	/* Command */
	CString str_winExecCommand;
	CString str_commandOne;         // cmd.exe
	CString str_commandTwo;         // fciv.exe
	CString str_commandThree;       // log file
	CString str_commandType;       // log file
	str_commandOne = _T("cmd.exe /c ");
	str_commandTwo = _T(".\\WinRAR.exe");
	str_commandType = _T(" x ");
	str_commandThree = _T("\"");
	str_commandThree = str_commandThree + str_Toolpath + _T("\"");
	str_commandThree = str_commandThree + _T(" ") + str_destpath;
	str_winExecCommand = str_commandOne + str_commandTwo + str_commandType + str_commandThree;

	/* Command CString -> char* */
	TCHAR sz_commandLine[1024];
	memset(sz_commandLine, 0, sizeof(sz_commandLine));
	for (int i = 0; i < str_winExecCommand.GetLength(); i ++)
	{
		sz_commandLine[i] = str_winExecCommand[i];
	}
	m_Clog.WriteLog(sz_commandLine);
	/* Excute WinRAR.exe */
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb          = sizeof(si);
	si.wShowWindow = SW_HIDE;
	si.dwFlags     = STARTF_USESHOWWINDOW;

	b_res = CreateProcess(NULL, sz_commandLine, NULL, NULL, NULL, NULL, NULL, NULL, &si, &pi);
	if(b_res == FALSE) 
	{
		m_Clog.WriteLog(CString("无法关闭WinRAR.EXE的进程，请联系工程师"));
		AfxMessageBox(_T("无法关闭WinRAR.EXE的进程，请联系工程师"));
		return false;
	}
	WaitForSingleObject(pi.hProcess, 5000);
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );

	Sleep(1000);
	return true;
}



bool Function::DoCopy(TCHAR* str_RemotePath,TCHAR* strLocalToolPath )
{ 
	//AfxMessageBox(CString("DoCopy"), MB_OK);
    //AfxMessageBox(CString(str_RemotePath), MB_OK);
	m_Clog.WriteLog(CString("DoCopy"));
	//1.check 舦籔囊琌

	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = _T("open");
	ShExecInfo.lpFile = _T("cmd");
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;

	//char str_RemotePath[1000];
	//char str_LocalPath[1000] = "D:\\Log";
	//char str_BackupPath[1000] = "D:\\LogUpload";
	//char temp[2000];

	////-- ミ环狠隔畖
	//strcpy(str_RemotePath, _T("\\\\"));
	//strcat(str_RemotePath, m_str_UploadServerIP.c_str());
	//strcat(str_RemotePath, _T("\\TestLog"));

	//TCHAR str_account[100] = _T("mpics\\TestLog");
	//TCHAR str_pswd[100] = _T("qisda2013");
	TCHAR str_account[100] = _T("mpics\\user");
	TCHAR str_pswd[100] = _T("12345678");
    TCHAR temp[2000];
	_stprintf(temp, _T("/C net use %s /user:%s %s"), str_RemotePath, str_account, str_pswd);
	//m_Clog.WriteLog(CString(temp));
	//AfxMessageBox(CString(temp));

	ShExecInfo.lpParameters = temp;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, 3000);

	//-- 代刚ミ硈絬琌Θ
	_stprintf(temp,_T("/C net use %s"), str_RemotePath);
	//AfxMessageBox(CString(temp));
	m_Clog.WriteLog(CString(temp));
	ShExecInfo.lpParameters = temp;
	ShellExecuteEx(&ShExecInfo);
	if (WaitForSingleObject(ShExecInfo.hProcess, 2000) == WAIT_TIMEOUT)
	{
		//m_strResult = _T("FAIL");
		//m_strMessage = "Fail to connect server uploading.";
		//FactoryLog();
		m_Clog.WriteLog(CString(_T("Fail to connect server ")));
		AfxMessageBox(CString(_T("Fail to connect server ")));
		return false;
	}
	//AfxMessageBox(CString(_T("connect server ok")));

	//CString strLocalToolPath = CString(_T("D:\\ENTRANCE")) ;
	_tmkdir(CString(_T("D:\\ENTRANCE")));
	_tmkdir(strLocalToolPath);
	//-- 狡籹┮Τ郎环狠
	memset(temp,0,sizeof(temp));
	_stprintf(temp, _T("/C xcopy /e /y %s %s"), str_RemotePath,strLocalToolPath);
	//AfxMessageBox(CString(temp));
	m_Clog.WriteLog(CString(temp));

	ShExecInfo.lpParameters = temp;
	ShellExecuteEx(&ShExecInfo);
	if(WaitForSingleObject(ShExecInfo.hProcess, 10000) == WAIT_TIMEOUT)
	{
		//CString csMsg;
		//csMsg.Format(_T("Log uploading timeout with %d sec"), nUploadTimeout / 1000);
		//m_strMessage = _T(csMsg);
		//FactoryLog();
		AfxMessageBox(CString(_T("copy timeout")));
		m_Clog.WriteLog(CString(_T("copy timeout")));
		return false;
	}
	return true;
}

bool Function::TraverseFolder(LPCTSTR lpPath)
{
    CFileFind finder;
	CString strFileName;
    CString keyword = _T("EntranceTool.exe");
	CString strDirectory= CString(lpPath);
	strDirectory += _T("\\*.*");

	BOOL bWorking = finder.FindFile(strDirectory);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
//		File_Path = finder.GetFilePath();
		if (finder.IsDots())
			continue;
		if (finder.IsDirectory())
        {
			 CString str = finder.GetFilePath();
         //Recurse(str);	 
			// AfxMessageBox(str);
			if( TraverseFolder((LPCTSTR)str))
			{
				return true;
			}
			continue;
        }
		strFileName = finder.GetFileName();
		//AfxMessageBox(strFileName);
		m_Clog.WriteLog(strFileName);
        if(-1 ==strFileName.Find(keyword))
		{
			continue;
		}
		CString strFilePath = finder.GetFilePath();
		//strFilePath = strFilePath +strFileName;

		_tcscpy_s (m_NewEntranceToolFilePath,strFilePath);
		//_tcscpy_s (m_NewEntranceToolFolderPath,strFilePath );
		//AfxMessageBox(CString(_T("EntranceTool.exe found")));
		m_Clog.WriteLog(CString(_T("EntranceTool.exe found")));
		m_Clog.WriteLog(m_NewEntranceToolFilePath);
		//AfxMessageBox(CString(m_NewEntranceToolFilePath));
		//AfxMessageBox(CString(m_NewEntranceToolFolderPath));
		return true;
	}
	return false;
}

bool Function::ExecuteEntranceTool()
{
	//1.find entrancetool.exe 
	m_Clog.WriteLog(CString("ExecuteEntranceTool"));
	//AfxMessageBox(CString("ExecuteEntranceTool"));
	//AfxMessageBox(CString(m_NewEntranceToolFolderPath));
	bool bRes = TraverseFolder(m_NewEntranceToolFolderPath);
	if(!bRes)
	{
		m_Clog.WriteLog(CString("Entrancetool.exe not found"));
		AfxMessageBox(CString(_T("Entrancetool.exe not found")));
		return false;
	}
	//AfxMessageBox(CString(m_NewEntranceToolFilePath));

	/* Command */
	CString str_winExecCommand;
	CString str_commandOne;         // cmd.exe
	CString str_commandTwo;         // fciv.exe
	CString str_commandThree;       // log file
	CString str_commandType;       // log file
	str_commandOne = _T("cmd.exe /c ");
	//str_commandTwo = _T(".\\WinRAR.exe");
	//str_commandType = _T(" x ");
	//str_commandThree = _T("\"");
	//str_commandThree = str_commandThree + str_Toolpath + _T("\"");
	//str_commandThree = str_commandThree + _T(" ") + str_destpath;
	//str_winExecCommand = str_commandOne + str_commandTwo + str_commandType + str_commandThree;
	str_winExecCommand = str_commandOne + m_NewEntranceToolFilePath;

	/* Command CString -> char* */
	TCHAR sz_commandLine[1024];
	memset(sz_commandLine, 0, sizeof(sz_commandLine));
	for (int i = 0; i < str_winExecCommand.GetLength(); i ++)
	{
		sz_commandLine[i] = str_winExecCommand[i];
	}

	/* Execute EntranceTool.exe */
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb          = sizeof(si);
	si.wShowWindow = SW_HIDE;
	si.dwFlags     = STARTF_USESHOWWINDOW;
	m_Clog.WriteLog(sz_commandLine);
	bool b_res = CreateProcess(NULL, sz_commandLine, NULL, NULL, NULL, NULL, NULL, NULL, &si, &pi);
	if(b_res == FALSE) 
	{
		AfxMessageBox(_T("无法关闭EntranceTool.EXE的进程，请联系工程师"));
		return false;
	}
	WaitForSingleObject(pi.hProcess, 3000);
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );
	return true;
}