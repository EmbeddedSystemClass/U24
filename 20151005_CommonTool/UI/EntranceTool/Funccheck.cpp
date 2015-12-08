#include "StdAfx.h"
#include "Funccheck.h"
//#include "..\DBAccess\DBAccess.h"
#include "io.h"

#pragma comment(lib, "Version.lib")

Funccheck::Funccheck(void)
{   
	memset(m_sz_Level, 0, sizeof(m_sz_Level));
	memset(m_sz_status,0, sizeof(m_sz_status));
	memset(m_sz_employId, 0, sizeof(m_sz_employId));
	memset(m_sz_PCname, 0, sizeof(m_sz_PCname));
	memset(m_sz_line, 0, sizeof(m_sz_line));
	memset(m_sz_process, 0, sizeof(m_sz_process));
	memset(m_sz_wrkstation,0,sizeof(m_sz_wrkstation));
	memset(m_sz_segment, 0, sizeof(m_sz_segment));
	memset(m_sz_SO, 0, sizeof(m_sz_SO));
	memset(m_sz_Model, 0, sizeof(m_sz_Model));
	//memset(m_sz_SOType, 0, sizeof(m_sz_SOType));
	//memset(m_sz_PN, 0, sizeof(m_sz_PN));
	memset(m_sz_version_get,0,sizeof(m_sz_version_get));
	memset(m_sz_model_get,0,sizeof(m_sz_model_get));
	m_str_process = _T("");
	m_str_status = _T("");
	m_str_ProjectName = _T("");
	m_str_ImagePath = _T("");
	m_bDBConnected = false;
	m_bMD5check = false;
	m_bImageCopy = false;
	m_bWin7 = false;
	m_str_SOType = _T("");
	m_str_ToolStatus = _T("");
	m_str_FactoryStatus = _T("");
	m_str_ToolFolderVersion = _T("");

    m_bfactoryImagestate = true;
	m_btoolstate = true;
	m_buserImagestate = true;
    
	DWORD dwSize = sizeof(m_sz_PCname);
	::GetComputerName(m_sz_PCname,&dwSize);

//	strcpy(m_sz_PCname,"P07D04");
   
	CString str_temp;
	TCHAR sz_buffer[15];
	memset(sz_buffer, 0, sizeof(sz_buffer));
	::GetPrivateProfileString(_T("Image_Copy"), _T("Copy"), _T(""), sz_buffer, sizeof(sz_buffer), _T(".\\INI\\setup.ini"));
	str_temp.Format(_T("%s"),sz_buffer);
	if(str_temp == _T("0"))
	{
		m_bImageCopy = false;
	}
	else
	{
		m_bImageCopy = true;
	}
	memset(sz_buffer, 0, sizeof(sz_buffer));
	::GetPrivateProfileString(_T("MD5_Check"), _T("Check"), _T(""), sz_buffer, sizeof(sz_buffer), _T(".\\INI\\setup.ini"));
	str_temp.Format(_T("%s"),sz_buffer);
	if(str_temp == _T("0"))
	{
		m_bMD5check = false;
	}
	else
	{
		m_bMD5check = true;
	}

	memset(sz_buffer, 0, sizeof(sz_buffer));
	::GetPrivateProfileString(_T("MD5_Check_TOOL"), _T("Check"), _T(""), sz_buffer, sizeof(sz_buffer), _T(".\\INI\\setup.ini"));
	str_temp.Format(_T("%s"),sz_buffer);
	if(str_temp == _T("0"))
	{
		m_bMD5check_tool = false;
	}
	else
	{
		m_bMD5check_tool = true;
	}

	memset(sz_buffer, 0, sizeof(sz_buffer));
	::GetPrivateProfileString(_T("MD5_Check_TOOL_Pre"), _T("Check"), _T(""), sz_buffer, sizeof(sz_buffer), _T(".\\INI\\setup.ini"));
	str_temp.Format(_T("%s"),sz_buffer);
	if(str_temp == _T("0"))
	{
		m_bMD5check_tool_Pre = false;
	}
	else
	{
		m_bMD5check_tool_Pre = true;
	}

	memset(sz_buffer, 0, sizeof(sz_buffer));
	::GetPrivateProfileString(_T("Win7_Support"), _T("Win7"), _T(""), sz_buffer, sizeof(sz_buffer), _T(".\\INI\\setup.ini"));
	str_temp.Format(_T("%s"),sz_buffer);
	if(str_temp == _T("0"))
	{
		m_bWin7 = false;
	}
	else
	{
		m_bWin7 = true;
	}

}

/******************************************************************************
* function name:  FconnectDB()                                       
* Parameters   :  void               
* Summary      :  连接数据库
* Return       :  True 数据库连接成功.False数据库连接失败  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Tom Li          2011/06/26           First version                            
*****************************************************************************/
bool Funccheck::FconnectDB()
{
	if(!this->m_bDBConnected)
	{
		if(m_DBO.GetFunadress())
		{
			if(m_DBO.pConnectDB()&&m_DBO.pVMS_ConnectDB())
			{
				this->m_bDBConnected = true;
			}
		}
	}

	return m_bDBConnected;
}

Funccheck::~Funccheck(void)
{
	if(this->m_bDBConnected)
	{
		m_DBO.pCloseDB();
		m_DBO.pVMS_CloseDB();
		this->m_bDBConnected = false;
	}
		//m_DBO.pCloseDB();
}

bool Funccheck::GetComputername_LT(TCHAR* ch_computername)
{
	TCHAR sz_buffer[15];
	memset(sz_buffer, 0, sizeof(sz_buffer));
	::GetPrivateProfileString(_T("COMPUTER_NAME"), _T("NAME"), _T(""), sz_buffer, sizeof(sz_buffer), _T(".\\INI\\setup.ini"));
	strcpy_s(ch_computername,sizeof(sz_buffer),sz_buffer);
	return true;
}


/******************************************************************************
* function name:  GetFileName(void);             
* Parameters   :  tch_FileName
* Summary      :  Read File name from config file.
* Return       :  File Name  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Tom Li          2011/09/05           First version                            
*****************************************************************************/
CString Funccheck::GetFileName()
{
	CString str_Filename;
	TCHAR sz_buffer[512];
	memset(sz_buffer, 0, sizeof(sz_buffer));
	int iLength = ::GetPrivateProfileString(_T("FILE_NAME"), _T("NAME"), _T(""), sz_buffer, sizeof(sz_buffer), _T(".\\INI\\setup.ini"));
	str_Filename.Format(_T("%s"),sz_buffer);
	//strcpy_s(tch_FileName,sizeof(sz_buffer),sz_buffer);
	return str_Filename;
}

bool Funccheck::GetEmployeeIdOnly()
{   
	CString str_Only;
	TCHAR sz_buffer[15];
	memset(sz_buffer, 0, sizeof(sz_buffer));
	::GetPrivateProfileString(_T("EMPLOYEEID_ONLY"), _T("ONLY"), _T(""), sz_buffer, sizeof(sz_buffer), _T(".\\INI\\setup.ini"));
	str_Only.Format(_T("%s"),sz_buffer);
	if(str_Only==_T("1"))
		return true;
	else
		return false;
}
/******************************************************************************
* function name:  Query_ImployeeID(TCHAR* sz_employId, TCHAR* sz_Level,TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)                                      
* Parameters   :  IN    sz_employId Employee ID，IN   sz_Level   Employee Level               
*                 OUT   sz_ErrCode Error code  ，sz_ErrMsg  Error messgae    
* Summary      :  Query Employee level by Employee ID
* Return       :  True query success.False query fail  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Tom Li          2011/06/26           First version                            
*****************************************************************************/
bool Funccheck::Query_ImployeeID(TCHAR* sz_employId, TCHAR* sz_Level,TCHAR* sz_status,TCHAR* sz_wrkstation,TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	CString str_log = _T("");
	str_log.Format(_T("函数Query_ImployeeID查询ImployeeID %s 开始"),sz_employId);
	m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);
	bool b_res = false;
	if(sz_employId == _T(""))
	{
		return b_res;
	}
	if(!m_bDBConnected)
	{
		//m_bDBConnected = ConnectDB();
		//m_bDBConnected = m_DBO.pConnectDB;
		this->FconnectDB();
	}

	if(m_bDBConnected)
		//b_res = QueryEmployeeID_Right(sz_employId,sz_Level,sz_ErrCode,sz_ErrMsg);
	    //b_res = m_DBO.pQueryEmployeeID_Right(sz_employId,sz_Level,sz_ErrCode,sz_ErrMsg);
		b_res = m_DBO.pQueryEmployeeID_Right_V2(sz_employId,sz_Level,sz_status,sz_wrkstation,sz_ErrCode,sz_ErrMsg);
	else
	{//connect DB fail

	}
	return b_res;
}


/******************************************************************************
* function name:  ProjectName_Return(TCHAR* sz_modelprojList,TCHAR* sz_count,TCHAR * sz_ErrCode,TCHAR * sz_ErrMsg)                                     
* Parameters   :  OUT  sz_modelprojList model_project link name，OUT   sz_count   mode_project total num               
*                 OUT   sz_ErrCode Error code  ，sz_ErrMsg  Error messgae    
* Summary      :  Query model_project
* Return       :  True query success.False query fail  
* Version      :  Author            Date                 Abstract                      
* 2.0          :  Fox.yan          2012/04/25          Add ProjectName_Return function
*****************************************************************************/
bool Funccheck::ProjectName_Return(TCHAR* sz_modelprojList,TCHAR* sz_count,TCHAR * sz_ErrCode,TCHAR * sz_ErrMsg)
{
	bool b_bes=false;

	if(!m_bDBConnected)
	{
		//m_bDBConnected = ConnectDB();
		//m_bDBConnected = m_DBO.pConnectDB;
		this->FconnectDB();
	}

	if(m_bDBConnected)
		b_bes=m_DBO.pVMS_QueryModelProjectList(sz_modelprojList,sz_count,sz_ErrCode,sz_ErrMsg);

	return b_bes;
}

/******************************************************************************
* function name:  Entrance_Tool_Update(TCHAR * Entrance_toolver,TCHAR * sz_SO,TCHAR * sz_ErrCode,TCHAR * sz_ErrMsg)                                     
* Parameters   :  IN  Entrance_toolver EntranceTool version, IN   sz_SO   so               
*                 OUT   sz_ErrCode Error code  ，sz_ErrMsg  Error message  
* Summary      :  EntranceTool auto upgrade
* Return       :  True query success.False query fail  
* Version      :  Author            Date                 Abstract                      
* 2.0          :  Tom Li          2012/04/26           Add Entrance_Tool_Update function
*****************************************************************************/
bool Funccheck::Entrance_Tool_Update(TCHAR * Entrance_toolver,TCHAR * sz_SO,TCHAR * sz_ErrCode,TCHAR * sz_ErrMsg)
{
	bool b_res = false;
    CString str_Server_Path=_T("");
    
	memset(sz_ErrCode,0,ERRORNUMBER);
	memset(sz_ErrMsg,0,ERRORNUMBER);
	memset(m_sz_model_pre2,0,50);//1
	memset(m_sz_partNo_pre2,0,50);//2
	memset(m_sz_soType_pre2,0,50);//3
	memset(m_sz_toolVer_pre2,0,50);//4
	memset(m_sz_facImageVer_SO_pre2,0,50);//5
	memset(m_sz_userImageVer_pre2,0,50);//6
	memset(m_sz_ETtoolVer_pre2,0,50);//7
	memset(m_sz_CRType_pre2,0,50);
	memset(m_sz_sz_Qty,0,50);
	memset(m_sz_segment_pre2,0,50);

	TCHAR sz_Buffer[100];
	CString str_FileServerName=_T("");
	::GetPrivateProfileString(_T("SERVER_INFO"), _T("ENTRANCE_PATH"), _T(""), sz_Buffer, sizeof(sz_Buffer), INIFILEPASH);
	str_FileServerName.Format("%s",sz_Buffer);
    
	char sz_Status[30]={0};
    
	char sz_Fix_Entool_Ver[50]={0};

	char sz_Fix_Img_ver[50]={0};

    b_res = m_DBO.pQuerySO_Info_All_V5(sz_SO,m_sz_model_pre2,m_sz_partNo_pre2,m_sz_soType_pre2,m_sz_toolVer_pre2,m_sz_facImageVer_SO_pre2,m_sz_userImageVer_pre2,m_sz_ETtoolVer_pre2,m_sz_sz_Qty,m_sz_CRType_pre2,m_sz_segment_pre2,sz_Fix_Entool_Ver,sz_Fix_Img_ver,sz_ErrCode,sz_ErrMsg);
    
	if(b_res==false)
	{   CString ErrMSG = sz_ErrMsg;
		AfxMessageBox(ErrMSG);
		return false;
	}

	if(strcmp(sz_Fix_Entool_Ver,"Y")==0)
	{
		m_str_EntranceToolver.Format(_T("%s"),m_sz_ETtoolVer_pre2);
		m_str_EntranceToolver.MakeUpper();
		if (m_str_EntranceToolver==_T(""))
		{
			AfxMessageBox(_T("EntranceTool指定版本被勾起，但版本栏为空，请找PE确认信息"));
			return false;
		}
		CString str_log = _T("");
		str_log.Format(_T("EntrnaceTool当前版本为%s,所需Fix的版本为%s"),Entrance_toolver,m_str_EntranceToolver);
		m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);
	}
	else
	{
		b_res = m_DBO.pVMS_QueryCommonToolVersion("Entrance",m_sz_CRType_pre2,m_sz_toolVer_pre2,sz_Status,sz_ErrCode,sz_ErrMsg );
		m_str_EntranceToolver.Format(_T("%s"),m_sz_toolVer_pre2);
		m_str_EntranceToolver.MakeUpper();

		CString str_log = _T("");
		str_log.Format(_T("EntrnaceTool当前版本为%s,最新release版本为%s"),Entrance_toolver,m_str_EntranceToolver);
		m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);
	}

	if(strcmp(Entrance_toolver,m_str_EntranceToolver)<0)
	{
		CString str_winExecCommand;
		CString str_commandOne;        
		CString str_commandTwo;         
        
		str_commandOne = _T("cmd.exe /c ");

		str_commandTwo = _T("start AutoUpdate.exe");
		str_winExecCommand = str_commandOne + str_commandTwo+_T(" ")+m_str_EntranceToolver+_T(" ")+str_FileServerName;

		CString str_log = _T("");
		str_log.Format(_T("EntrnaceTool start upgrade :toolver %s ,servername %s"),m_str_EntranceToolver,str_FileServerName);
		m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);

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

		bool b_createRes = false;

		b_createRes = CreateProcess(NULL, sz_commandLine, NULL, NULL, NULL, NULL, NULL, NULL, &si, &pi);

		Sleep(1000);

		if (b_createRes==true)
		{
			b_res = false;
		}
		else
		{
			AfxMessageBox(_T("调用自我升级程序失败，请手动copy使用，并联系工程师"));
			return false;
		}
	}
	else
	{
		b_res = true;
	}
return b_res;
}

/******************************************************************************
* function name:  CreateUpdateBat(TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg, CString& str_batpath)                                  
* Parameters   :  BOOL 
* Summary      :  Create Bat file to Update Entrace tool
* Return       :  True  success.False  fail  
* Version      :  Author            Date                 Abstract                      
* 2.0          :  Fox.yan         2012/04/15           Add CreateUpdateBat function
*****************************************************************************/
bool Funccheck::CreateUpdateBat(TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg, CString& str_batpath)
{   
	bool b_res=false;
	CStdioFile fbat;
	CFileFind obj_fileFind;
	CFileException fileException;
	TCHAR sz_Path[MAX_PATH];
	CFileFind finder;
	CString str_filename;
	CString str_fileCurrentname;
	memset(sz_Path,0,sizeof(sz_Path));

	/*********** 获取当前进程的路径 str_fileCurrentname ********/
	::GetCurrentDirectory(MAX_PATH, sz_Path);
	str_fileCurrentname.Format("%s",sz_Path);

	/*********** 从 INI 文件中获取 Entracetool sever路径 ********/
	::GetPrivateProfileString(_T("SERVER_INFO"), _T("ENTRANCE_PATH"), _T(""), sz_Path, sizeof(sz_Path), INIFILEPASH);
	str_filename.Format("%s",sz_Path);


	CString str_temp = _T("C:\\Tool\\");                    
	str_temp = str_temp + _T("*.*");
	b_res = finder.FindFile(str_temp);
	while(b_res)
	{
		b_res = finder.FindNextFile();//检查文件是否检查完
		if(finder.IsDots())
			continue;

		CString str_filePath = finder.GetFilePath();
		if(finder.IsDirectory())
		{
			str_temp = str_filePath;
			break;
		}
		else
		{
			continue;
		}
	}

	CString str_bat_filename = _T("c:\\Tool\\");
	str_bat_filename = str_bat_filename +"UpdateEntranceTool.bat";

	if(obj_fileFind.FindFile(str_bat_filename))
	{
		DeleteFile(str_bat_filename);
		if(fbat.Open (str_bat_filename, CFile::typeText|CFile::modeReadWrite|CFile::modeCreate, &fileException))
		{
			CString str_temp = _T("taskkill /f /im EntranceTool.exe");

			CString str_Local_MD5_Path=_T("C:\\EntranceTool\\MD5\\MD5_TLFT_Common_")+m_str_EntranceToolver+_T("_TLCAEntrance.xml");


			CString str_Local_path = _T("C:\\EntranceTool\\TLFT_Common_")+m_str_EntranceToolver+_T("_Entrance");
			CString str_Local_path_Winto = _T("C:\\EntranceTool\\TLFT_Common_")+m_str_EntranceToolver+_T("_TLCAEntrance");//str_Local_path+_T("\\TLFT_Common_")+m_str_EntranceToolver+_T("_TLCAEntrance");
			CString str_Local_ZIP_Path=_T("C:\\EntranceTool\\TLFT_Common_")+m_str_EntranceToolver+_T("_Entrance\\")+_T("TLFT_PRJMCommon_GNVR")+m_str_EntranceToolver+_T("_BVRTEU_TLCAEntrance_HWSGMPP01.zip");
			str_temp=str_temp+"\n";
			str_temp=str_temp+_T("xcopy")+_T(" ")+str_fileCurrentname+_T("\\winrar.exe")+_T(" ")+_T("C:\\tool");
			str_temp=str_temp+"\n";
			str_temp=str_temp+_T("xcopy")+_T(" ")+str_fileCurrentname+_T("\\ConvertXML.exe")+_T(" ")+_T("C:\\tool");
			str_temp=str_temp+"\n";
			str_temp=str_temp+_T("ping 127.0.0.1 -n 12");
			str_temp=str_temp+"\n";
			str_temp=str_temp+_T("rmdir /s/q")+_T(" ")+str_fileCurrentname;
			str_temp=str_temp+"\n";
			str_temp=str_temp+_T("rmdir /s/q ")+_T("C:\\EntranceTool");
			str_temp=str_temp+"\n";

			str_temp=str_temp+_T("md ")+_T("C:\\EntranceTool");
			str_temp=str_temp+"\n";
			//从服务器将MD5文件和TLFT_Common_v2.001_TLCAEntrance.zip COPY到本地来
			str_temp=str_temp+_T("xcopy")+_T(" ")+str_filename+_T("\\")+_T("*.* /e/k ")+_T("C:\\EntranceTool");
			str_temp=str_temp+"\n";
			str_temp=str_temp+_T("start")+_T(" ")+_T("C:\\tool\\ConvertXML.exe");
			//然后解压.zip到本地C:\Entrance\TLFT_Common_v2.001_TLCAEntrance路径
			str_temp = str_temp+"\n";
			str_temp =str_temp+_T("C:\\tool\\WINRAR.EXE x")+_T(" ")+str_Local_ZIP_Path+_T(" ")+_T("C:\\EntranceTool\\");
			str_temp= str_temp+"\n";
			str_temp =str_temp+_T("taskkill /f /im ConvertXML.exe");

			str_temp = str_temp+"\n";
			str_temp = str_temp +_T("del")+_T(" ")+_T("C:\\tool\\ConvertXML.exe");
			str_temp = str_temp+"\n";
			str_temp = str_temp +_T("del")+_T(" ")+_T("C:\\tool\\WINRAR.exe");
			str_temp = str_temp+"\n";
			//DELETE TLFT_PRJMCommon_GNVRv2.001_BVRTEU_TLCAEntrance_HWSGMPP01.zip
			str_temp = str_temp+_T("rmdir /s/q")+_T(" ")+str_Local_path;
			str_temp = str_temp+"\n";
			str_temp = str_temp+str_Local_path_Winto+_T("\\fciv.exe -r -v")+_T(" ")+str_Local_path_Winto+_T(" -xml")+_T(" ")+str_Local_MD5_Path;
			//fciv.exe -r -v D:\EntranceTool -xml EntranceTool.xml
			//if %errorlevel%==1 pause echo MD5检查失败，拷贝出错
			//str_temp=str_temp+str_fcivCheck;
			str_temp=str_temp+"\n";
			str_temp=str_temp+_T("if %errorlevel%==1 mshta vbscript:msgbox(MD5_CHECK_FAIL,MD5_check_Fail,Update_Fail)");
			str_temp=str_temp+"\n";
			//str_temp=str_temp+_T("start ")+str_Local_path_Winto+_T("\\EntranceTool.exe");
			//str_temp= str_temp + _T("start") + str_Local_path_Winto +_T("\\StartEntrance.bat");
			str_temp=str_temp + _T("cd\\");
			str_temp=str_temp+"\n";
			str_temp=str_temp+_T("cd ")+_T("\\EntranceTool\\TLFT_Common_")+m_str_EntranceToolver+_T("_TLCAEntrance\\");//cd EntranceTool\TLFT_Common_v2.002_Entrance\TLFT_Common_v2.002_TLCAEntrance
			str_temp=str_temp+"\n";
			str_temp=str_temp+_T("start ")+_T("C:\\EntranceTool\\TLFT_Common_")+m_str_EntranceToolver+_T("_TLCAEntrance\\")+_T("StartEntrance.bat");
			str_temp=str_temp+"\n";
			str_temp=str_temp+"exit";

			fbat.WriteString(str_temp);
			fbat.Close();
			str_batpath = str_bat_filename;
			b_res = true;
		}
		else
		{
			b_res = false;
		}
	}
	else
	{
		if(fbat.Open (str_bat_filename, CFile::typeText|CFile::modeReadWrite|CFile::modeCreate, &fileException))
		{

			CString str_temp = _T("taskkill /f /im EntranceTool.exe");

			CString str_Local_MD5_Path=_T("C:\\EntranceTool\\MD5\\MD5_TLFT_Common_")+m_str_EntranceToolver+_T("_TLCAEntrance.xml");


			CString str_Local_path = _T("C:\\EntranceTool\\TLFT_Common_")+m_str_EntranceToolver+_T("_Entrance");
			CString str_Local_path_Winto = _T("C:\\EntranceTool\\TLFT_Common_")+m_str_EntranceToolver+_T("_TLCAEntrance");//str_Local_path+_T("\\TLFT_Common_")+m_str_EntranceToolver+_T("_TLCAEntrance");
			CString str_Local_ZIP_Path=_T("C:\\EntranceTool\\TLFT_Common_")+m_str_EntranceToolver+_T("_Entrance\\")+_T("TLFT_PRJMCommon_GNVR")+m_str_EntranceToolver+_T("_BVRTEU_TLCAEntrance_HWSGMPP01.zip");
			str_temp=str_temp+"\n";
			str_temp=str_temp+_T("xcopy")+_T(" ")+str_fileCurrentname+_T("\\winrar.exe")+_T(" ")+_T("C:\\tool");
			str_temp=str_temp+"\n";
			str_temp=str_temp+_T("xcopy")+_T(" ")+str_fileCurrentname+_T("\\ConvertXML.exe")+_T(" ")+_T("C:\\tool");
			str_temp=str_temp+"\n";
			str_temp=str_temp+_T("ping 127.0.0.1 -n 12");
			str_temp=str_temp+"\n";
			str_temp=str_temp+_T("rmdir /s/q")+_T(" ")+str_fileCurrentname;
			str_temp=str_temp+"\n";
			str_temp=str_temp+_T("rmdir /s/q ")+_T("C:\\EntranceTool");
			str_temp=str_temp+"\n";

			str_temp=str_temp+_T("md ")+_T("C:\\EntranceTool");
			str_temp=str_temp+"\n";
			//从服务器将MD5文件和TLFT_Common_v2.001_TLCAEntrance.zip COPY到本地来
			str_temp=str_temp+_T("xcopy")+_T(" ")+str_filename+_T("\\")+_T("*.* /e/k ")+_T("C:\\EntranceTool");
			str_temp=str_temp+"\n";
			str_temp=str_temp+_T("start")+_T(" ")+_T("C:\\tool\\ConvertXML.exe");
			//然后解压.zip到本地C:\Entrance\TLFT_Common_v2.001_TLCAEntrance路径
			str_temp = str_temp+"\n";
			str_temp =str_temp+_T("C:\\tool\\WINRAR.EXE x")+_T(" ")+str_Local_ZIP_Path+_T(" ")+_T("C:\\EntranceTool\\");
			str_temp= str_temp+"\n";
			str_temp =str_temp+_T("taskkill /f /im ConvertXML.exe");

			str_temp = str_temp+"\n";
			str_temp = str_temp +_T("del")+_T(" ")+_T("C:\\tool\\ConvertXML.exe");
			str_temp = str_temp+"\n";
			str_temp = str_temp +_T("del")+_T(" ")+_T("C:\\tool\\WINRAR.exe");
			str_temp = str_temp+"\n";
			//DELETE TLFT_PRJMCommon_GNVRv2.001_BVRTEU_TLCAEntrance_HWSGMPP01.zip
			str_temp = str_temp+_T("rmdir /s/q")+_T(" ")+str_Local_path;
			str_temp = str_temp+"\n";
			str_temp = str_temp+str_Local_path_Winto+_T("\\fciv.exe -r -v")+_T(" ")+str_Local_path_Winto+_T(" -xml")+_T(" ")+str_Local_MD5_Path;
			//fciv.exe -r -v D:\EntranceTool -xml EntranceTool.xml
			//if %errorlevel%==1 pause echo MD5检查失败，拷贝出错
			//str_temp=str_temp+str_fcivCheck;
			str_temp=str_temp+"\n";
			str_temp=str_temp+_T("if %errorlevel%==1 vbscript:msgbox(MD5_CHECK_FAIL,MD5_check_Fail,Update_Fail)");
			str_temp=str_temp+"\n";
			//str_temp=str_temp+_T("start ")+str_Local_path_Winto+_T("\\EntranceTool.exe");
			//str_temp= str_temp + _T("start") + str_Local_path_Winto +_T("\\StartEntrance.bat");
			str_temp=str_temp + _T("cd\\");
			str_temp=str_temp+"\n";
			str_temp=str_temp+_T("cd ")+_T("\\EntranceTool\\TLFT_Common_")+m_str_EntranceToolver+_T("_TLCAEntrance\\");//cd EntranceTool\TLFT_Common_v2.002_Entrance\TLFT_Common_v2.002_TLCAEntrance
			str_temp=str_temp+"\n";
			str_temp=str_temp+_T("start ")+_T("C:\\EntranceTool\\TLFT_Common_")+m_str_EntranceToolver+_T("_TLCAEntrance\\")+_T("StartEntrance.bat");
			str_temp=str_temp+"\n";
			str_temp=str_temp+"exit";

			fbat.WriteString(str_temp);
			fbat.Close();
			str_batpath = str_bat_filename;
			b_res = true;
		}
		else
		{
			b_res = false;
		}

	}
	return b_res;
}

bool Funccheck::Update_ImployeeIdStatus(TCHAR* sz_employId/*input*/, TCHAR * sz_status/*input*/,TCHAR* sz_ErrCode/*output*/, TCHAR* sz_ErrMsg/*output*/)
{
	bool b_res=false;
	CString sz_str_employee=_T("");
	CString sz_str_status=_T("");
    sz_str_employee.Format(_T("%s"),sz_employId);
    sz_str_status.Format(_T("%s"),sz_status);


	if(sz_str_employee==_T("")||sz_str_status==_T(""))
	{  
		return false;
	}
	if(!m_bDBConnected)
		this->FconnectDB();
	if(m_bDBConnected)
	{
		//b_res = QueryEmployeeID_Right(sz_employId,sz_Level,sz_ErrCode,sz_ErrMsg);
		b_res = m_DBO.pUpdateEmployeeID_Status(sz_employId,sz_status,m_sz_PCname,sz_ErrCode,sz_ErrMsg);
	}
	else
	{//connect DB fail

	}
	return b_res;
}

/******************************************************************************
* function name:  Query_StationInfo(TCHAR* sz_line, TCHAR* sz_process, TCHAR* sz_segment,TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)                                     
* Parameters   :  IN sz_line   , IN  sz_process  station name
                  OUT sz_ErrCode  Error code  ，  sz_ErrMsg  Error message    
* Summary      :  Query station name by "Computer name".
* Return       :  True query success.False query error  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Tom Li          2011/06/26           First version                            
*****************************************************************************/
bool Funccheck::Query_StationInfo(TCHAR* sz_line, TCHAR* sz_process, TCHAR* sz_segment,TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	bool b_res = false;
    
	//TCHAR sz_Path[50];
	//CString str_Flag=_T("");
	//::GetPrivateProfileString(_T("Test_Flag"), _T("Check"), _T(""), sz_Path, sizeof(sz_Path), INIFILEPASH);
	//str_Flag.Format("%s",sz_Path);
	//if (str_Flag==_T("1"))
	//{
#ifdef RD_MODE
	m_Clog.WriteLog(_T("RD MODE: GetComputername from setup.ini"));
	b_res = GetComputername_LT(m_sz_PCname);   // test use PC name  release need cancel
#endif

	//}
    

	CString str_log = _T("");
	str_log.Format(_T("函数Query_StationInfo根据电脑名 %s 查询process开始"),m_sz_PCname);
	m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);

	if(!m_bDBConnected)
		this->FconnectDB();

	if(m_bDBConnected)
	{
		//b_res = QueryStation_Info(m_sz_PCname,sz_line,sz_process,sz_segment,sz_ErrCode,sz_ErrMsg);
		b_res = m_DBO.pQueryStation_Info(m_sz_PCname,sz_line,sz_process,sz_segment,sz_ErrCode,sz_ErrMsg);
	}
	else
	{//connect DB fail

	}
	return b_res;
}



/******************************************************************************
* function name:  Query_SOInfo(TCHAR sz_SO[], TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)                                    
* Parameters   :  IN sz_SO[] 订单， OUT sz_ErrCode 错误代码  ，  sz_ErrMsg  错误消息    
* Summary      :  根据SO，查询该SO的相关信息
* Return       :  True 查询成功.False查询失败  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Tom Li          2011/06/26           First version                            
*****************************************************************************/
bool Funccheck::Query_SOInfo(TCHAR* sz_SO, TCHAR* sz_Model, TCHAR* sz_PN, TCHAR* sz_SOType,TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	CString str_log = _T("");
	str_log.Format(_T("函数Query_SOInfo根据SO %s 查询SO相关信息开始"),sz_SO);
	m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);

	bool b_res = false;
	if(sz_SO == "")
		return b_res;
	if(!m_bDBConnected)
		this->FconnectDB();

	if(m_bDBConnected)
	{
		//b_res = QuerySO_Info(sz_SO,sz_Model,sz_PN,sz_SOType,sz_ErrCode,sz_ErrMsg);
		b_res = m_DBO.pQuerySO_Info(sz_SO,sz_Model,sz_PN,sz_SOType,sz_ErrCode,sz_ErrMsg);
	}
	else
	{//connect DB fail

	}
	return b_res;
}

/******************************************************************************
* function name:  Query_ProcessList(TCHAR sz_SO[], TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)                                    
* Parameters   :  IN sz_SO[] 订单， OUT sz_ErrCode 错误代码  ，  sz_ErrMsg  错误消息    
* Summary      :  根据SO，查询该SO的相关信息
* Return       :  True 查询成功.False查询失败  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Tom Li          2011/06/26           First version                            
*****************************************************************************/
bool Funccheck::Query_PorcessList(TCHAR* sz_Model, TCHAR* sz_ProcessList, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	CString str_log = _T("");
	str_log.Format(_T("函数Query_PorcessList根据Model %s 查询Process相关信息开始"),sz_Model);
	
	m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);

	bool b_res = false;
	if(sz_Model == "")
		return b_res;

//	m_bDBConnected = m_DBO.pConnectDB;
	if(!m_bDBConnected)
		this->FconnectDB();

	if(m_bDBConnected)
	{
		//b_res = QuerySegmentProcess(sz_Model,_T(""),sz_ProcessList,sz_ErrCode,sz_ErrMsg);
	    b_res = m_DBO.pQuerySegmentProcess(sz_Model,_T(""),sz_ProcessList,sz_ErrCode,sz_ErrMsg);
	}
	else
	{//connect DB fail

	}
	return b_res;
}
/******************************************************************************
* function name:  Query_ConsumeType(TCHAR* sz_Model, TCHAR* sz_Process, TCHAR* sz_Consume,TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)                                    
* Parameters   :      
* Summary      :  Query Consume Type and maxtimes
* Return       :  True 查询成功.False查询失败  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Magan.Mei          2014/05/26           First version                            
*****************************************************************************/
bool Funccheck::Query_ConsumeType(TCHAR* sz_Model, TCHAR* sz_Process, TCHAR* sz_Consume, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	CString str_log = _T("");
	str_log.Format(_T("函数Query_ConsumeType根据Model %s, Process %s,查询consume相关信息开始"),sz_Model, sz_Process);
	
	m_Clog.WriteLog(str_log);

	bool b_res = false;

	if(!m_bDBConnected)
		this->FconnectDB();

	if(m_bDBConnected)
	{
	    b_res = m_DBO.pQueryConsumeType(sz_Model, sz_Process, sz_Consume, sz_ErrCode, sz_ErrMsg);

	}
	return b_res;
}
/******************************************************************************
* function name:  Query_ConsumeType(TCHAR* sz_Model, TCHAR* sz_Process, TCHAR* sz_Consume,TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)                                    
* Parameters   :      
* Summary      :  Query Consume Type and maxtimes
* Return       :  True 查询成功.False查询失败  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Magan.Mei          2014/05/26           First version                            
*****************************************************************************/
bool Funccheck::Query_ConsumeInfo(TCHAR* sz_sn, TCHAR* sz_type, TCHAR* sz_times, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	CString str_log = _T("");
	str_log.Format(_T("函数Query_ConsumeInfo根据sn %s, type %s,查询consume相关信息开始"),sz_sn, sz_type);
	
	m_Clog.WriteLog(str_log);

	bool b_res = false;

	if(!m_bDBConnected)
		this->FconnectDB();

	if(m_bDBConnected)
	{
	    b_res = m_DBO.pQueryConsumeInfo(sz_sn, sz_type, sz_times, sz_ErrCode, sz_ErrMsg);

	}
	return b_res;
}

/******************************************************************************
* function name:  ImageDirectoryDelete(CString str_search, TCHAR* szImageVersion , TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)                           
* Parameters   :      
* Summary      :  删除Image目录下多余的信息
* Return       :  True 成功.False失败  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Tom Li          2011/11/29           First version                            
*****************************************************************************/
bool Funccheck::ImageDirectoryDelete(CString str_search, TCHAR* szImageVersion , TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	CString str_log = _T("");
	str_log = _T("函数ImageDirectoryDelete开始,删除") + str_search + _T("目录下多余的文件");
	m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);

	bool b_res = true;
	CFileFind finder;
	CString str_initialPath = _T("");
	CString str_ModelName = _T("");
	CString str_Imageversion = _T("");
	str_Imageversion.Format(_T("%s"),szImageVersion);
	str_ModelName.Format(_T("%s"),m_sz_Model);

	if(str_search.Right(1) == _T("\\"))
	{
		str_search = str_search.Left(str_search.GetLength() - 1);
	}
	str_initialPath = str_search;
	str_search = str_search + _T("\\");
	str_search = str_search + _T("*.*");

	b_res = finder.FindFile(str_search);
	while(b_res)
	{
		b_res = finder.FindNextFile();//检查文件是否检查完
		if(finder.IsDots())
			continue;

		CString str_filePath = finder.GetFilePath();
		CString str_lastDirectory = _T("");
		if(finder.IsDirectory())
		{
			CString str_lastDirectory = _T("");
			str_lastDirectory = str_filePath.Mid(str_filePath.ReverseFind('\\'));
			if(str_lastDirectory.MakeUpper().Find(m_str_Model.MakeUpper()) >= 0 && str_lastDirectory.MakeUpper().Find(str_Imageversion.MakeUpper()) >= 0)
			{
				m_str_ImagePath = str_filePath;
			}
			else
			{
				  int i_check = m_obj_DirectoryDispose.DeleteDir(str_filePath);
				  if(!i_check)
				  {
					  CString str_ErrCode = _T("ET0004");
					  CString str_ErrMsg = _T("删除本地路径") + str_filePath + _T("失败");
					  strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
					  strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
					  str_ErrCode.ReleaseBuffer();
					  str_ErrMsg.ReleaseBuffer();
					  return false;
				  }
			}
		}
		else
		{
			int nLengthRight = str_filePath.GetLength() - str_filePath.ReverseFind('.') - 1;
			CString str_check = str_filePath.Right(nLengthRight).MakeUpper();
			if(str_check != _T("FFU"))
			{
			  int i_check = DeleteFile(str_filePath);
			  if(!i_check)
			  {
				  CString str_ErrCode = _T("ET0004");
				  CString str_ErrMsg = _T("删除本地文件") + str_filePath + _T("失败");
				  strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
				  strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
				  str_ErrCode.ReleaseBuffer();
				  str_ErrMsg.ReleaseBuffer();
				  return false;
			  }
			}
		}
	}

	return true;
}

bool Funccheck::UnzipImage(CString str_search, TCHAR* szImageVersion , TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	bool b_res = true;
	bool isZip = true;
	CFileFind finder;
	CString str_Imagepath = _T("");
	CString str_destpath = _T("");
	str_Imagepath = DIRECTORYPATH;
	str_Imagepath = str_Imagepath + m_str_Model;
	str_Imagepath = str_Imagepath + _T("\\");
	str_Imagepath = str_Imagepath + szImageVersion;
	//str_Imagepath = str_Imagepath + _T("\\");
	//str_Imagepath = str_Imagepath + _T("*.*");
	m_Clog.WriteLog(_T("enter UnzipImage()"));
	if(str_search.Right(1) == _T("\\"))
	{
		str_search = str_search.Left(str_search.GetLength() - 1);
	}
	str_search = str_search + _T("\\");
	str_search = str_search + _T("*.*");

	b_res = finder.FindFile(str_search);
	while(b_res == true)
	{
		b_res = finder.FindNextFile();//检查文件是否检查完
		if(finder.IsDots())
			continue;

		CString str_filePath = finder.GetFilePath();
		if(finder.IsDirectory())
		{
			return UnzipImage(str_filePath, szImageVersion , sz_ErrCode, sz_ErrMsg);
		}

		if(str_filePath.Find(m_str_Model) >= 0 && str_filePath.Find(szImageVersion) >= 0 && str_filePath.Find(".zip") >= 0)
		{
			str_destpath = str_filePath;
			break;
		}
		//for 7z
		if(str_filePath.Find(m_str_Model) >= 0 && str_filePath.Find(szImageVersion) >= 0 && str_filePath.Find(".7z") >= 0)
		{
			str_destpath = str_filePath;
			isZip = false;
			break;
		}
	}
	finder.Close();

	if(str_destpath.IsEmpty())
	{
		 CString str_ErrCode = _T("ET0015");
		 CString str_ErrMsg = _T("Local Image is not exist\r\n");
		 str_ErrMsg = str_ErrMsg + _T("本地的Image不存在");
		 strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		 strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		 str_ErrCode.ReleaseBuffer();
	     str_ErrMsg.ReleaseBuffer();
		 return false;
	}
	m_Clog.WriteLog(_T("Unziptool"));
	if(isZip)
	{
		b_res = Unziptool(str_destpath, str_Imagepath, sz_ErrCode, sz_ErrMsg);
	}else{
		b_res = DecompressFile(str_destpath, str_Imagepath, sz_ErrCode, sz_ErrMsg);
	}
	if(!b_res)
	{
		  CString str_ErrCode = _T("ET0015");
		  CString str_ErrMsg = _T("Unzip file fail\n");
		  str_ErrMsg = str_ErrMsg + _T("解压文件") + str_destpath + _T("失败");
		  strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		  strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		  str_ErrCode.ReleaseBuffer();
		  str_ErrMsg.ReleaseBuffer();
		  return false;
	}
	m_Clog.WriteLog(_T("ImageDirectoryDelete"));
	b_res = ImageDirectoryDelete(str_Imagepath, szImageVersion , sz_ErrCode, sz_ErrMsg);
	if(!b_res)
	{
		  CString str_ErrCode = _T("ET0015");
		  CString str_ErrMsg = _T("Delete Image file fail\n");
		  str_ErrMsg = str_ErrMsg + _T("删除多余Image文件失败");
		  strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		  strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		  str_ErrCode.ReleaseBuffer();
		  str_ErrMsg.ReleaseBuffer();
		  return false;
	}

	return b_res;
}

/******************************************************************************
* function name:  Unziptool(CString str_Toolpath)                                 
* Parameters   :  CString str_Toolpath   Tool path  
* Summary      :  Unzip tool
* Return       :  True unzip success.False unzip fail  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Tom Li          2011/07/07           First version                            
*****************************************************************************/
bool Funccheck::Unziptool(CString str_Toolpath, CString str_destpath, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	CString str_log = _T("");
	str_log = _T("将文件从") + str_Toolpath + _T("目录下解压到") + str_destpath;
	m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);

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
		sz_ErrCode = _T("ET0015");
		sz_ErrMsg = _T("WinRAR.exe is not found.\r\nWinRAR.exe不存在");
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

	m_Clog.WriteLog(str_winExecCommand,sz_ErrCode,sz_ErrMsg);

	/* Command CString -> char* */
	TCHAR sz_commandLine[1024];
	memset(sz_commandLine, 0, sizeof(sz_commandLine));
	for (int i = 0; i < str_winExecCommand.GetLength(); i ++)
	{
		sz_commandLine[i] = str_winExecCommand[i];
	}

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
		return false;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);

	Sleep(1000);
 
	return b_res;
}


/******************************************************************************
* function name:  DecompressFile                                
* Parameters   :  CString str_Toolpath   Tool path  
* Summary      :  decompress zip/7z by 7Z.exe , to repleace UnzupTool()
* Return       :  True unzip success.False unzip fail  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Yu Teng Tung          2014/10/03           First version                            
*****************************************************************************/
bool Funccheck::DecompressFile(CString str_Toolpath, CString str_destpath, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	CString str_log = _T("");
	str_log = _T("将文件从") + str_Toolpath + _T("目录下解压到") + str_destpath;
	m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);

	bool b_res = false;
	/* Check Input */
	if(str_Toolpath == _T("") || str_destpath == _T("")) 
	{
		return false;
	}

	/* 检查7z.exe是否存在 */
	CString str_7zPathName;
	if(IsWow64())
	{
		str_7zPathName =  _T(".\\7z64.exe");
	}else{
		str_7zPathName =  _T(".\\7z32.exe");
	}
	if (_taccess(str_7zPathName, 0) != 0) 
	{
		sz_ErrCode = _T("ET0015");
		sz_ErrMsg = _T("WinRAR.exe is not found.\r\n7z.exe不存在");
		return false;
	}

	/* Command */
	CString str_winExecCommand;
	CString str_commandOne;         // cmd.exe
	CString str_commandTwo;         // fciv.exe
	CString str_commandThree;       // log file
	CString str_commandType;       // log file
	str_commandOne = _T("cmd.exe /c ");
	//str_commandTwo = _T(".\\WinRAR.exe");
	//str_commandTwo = _T(".\\7z.exe");
	str_commandTwo = str_7zPathName; 
	str_commandType = _T(" x ");
	str_commandThree = _T("\"");
	str_commandThree = str_commandThree + str_Toolpath + _T("\"");
	str_commandThree = str_commandThree + _T(" -o") + str_destpath + _T(" -r");
	str_winExecCommand = str_commandOne + str_commandTwo + str_commandType + str_commandThree;

	m_Clog.WriteLog(str_winExecCommand,sz_ErrCode,sz_ErrMsg);

	/* Command CString -> char* */
	TCHAR sz_commandLine[1024];
	memset(sz_commandLine, 0, sizeof(sz_commandLine));
	for (int i = 0; i < str_winExecCommand.GetLength(); i ++)
	{
		sz_commandLine[i] = str_winExecCommand[i];
	}

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
		return false;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);

	Sleep(1000);
 
	return b_res;
}

/******************************************************************************
* function name:  CString2TCHAR(CString &str)                                 
* Parameters   :  CString &str,  To be converted CString 
* Summary      :  Convert CString to TCHAR
* Return       :  TCHAR
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Tom Li          2011/09/05           First version                            
*****************************************************************************/
TCHAR* Funccheck::CString2TCHAR(CString &str) 
{ 
	if(str.IsEmpty())
		return NULL;
	int iLen = str.GetLength(); 
	TCHAR* szRs = new TCHAR[iLen]; 
	lstrcpy(szRs, str.GetBuffer(iLen)); 
	str.ReleaseBuffer(); 
	return szRs; 
} 


/******************************************************************************
* function name:  Recurse(LPCTSTR pstr)                                   
* Parameters   :  LPCTSTR pstr,Query directory  
* Summary      :  Query *.zip in pstr directory
* Return       :  True query success.False query fail  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Tom Li          2011/07/07           First version                            
*****************************************************************************/
bool Funccheck::Recurse(LPCTSTR pstr, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	bool b_res = false;
	CFileFind finder;
	CString str_errCode = _T("");
	CString str_errMsg = _T("");

	if(pstr == _T(""))
	{
		str_errCode = _T("ET0016");
		str_errMsg = _T("The directory of Image is NULL\nImage路径为空");
		sz_ErrCode = CString2TCHAR(str_errCode);
		sz_ErrMsg = CString2TCHAR(str_errMsg);
		return false;
	}
	// build a string with wildcards,*.zip exist in current directory
   CString strWildcard(pstr);
   if(strWildcard.Right(1) == _T("\\"))
   {
	   strWildcard = strWildcard.Left(strWildcard.GetLength() - 1);
   }
   strWildcard += _T("\\*.zip");

	b_res = finder.FindFile(strWildcard);
	CString str = _T("");
	if(b_res)
	{
		return true;
	}
	else
	{
		//当前目录没有*.zip,查找子目录
		CString strWildcard(pstr);
        strWildcard += _T("\\*.*");
		b_res = finder.FindFile(strWildcard);
		//查找失败
		if(b_res == false)
		{
			str_errCode = _T("ET0017");
			sz_ErrCode = CString2TCHAR(str_errCode);
			CString str_directory = pstr;
			CString str_temp = _T("Search fail,current directory ") + str_directory + _T(" is not exist , 检索失败，当前目录不存在 ");
			//sz_ErrMsg = str_temp;
			//strcpy_s(sz_ErrMsg,str_temp.GetLength(),str_temp.GetBuffer());
			sz_ErrMsg = CString2TCHAR(str_temp);
			finder.Close();
			return false;
		}
		else
		{
			while(b_res)
			{
				//只处理文件夹
				bool b_test = finder.FindNextFile();//检查文件是否检查完
				if(finder.IsDots())
				continue;
				if(finder.IsDirectory())
				{
					CString str = finder.GetFilePath();
                    b_res = Recurse(str,sz_ErrCode,sz_ErrMsg);
					if(b_res)
						return true;
				}
				if(b_test == false)
				{
					sz_ErrCode = _T("ET0026");
		            sz_ErrMsg = _T("Search fail, *.zip file is not exist\n检索失败，*.zip文件不存在");
					b_res = false;
				}
			}

			return b_res;
		}
			
	}

	finder.Close();
	return b_res;
}




/******************************************************************************
* function name:  DoModuleCheck(char* sz_ErrCode, char* sz_ErrMsg)                                  
* Parameters   :  char* sz_ErrCode  ,  char* sz_ErrMsg
* Summary      :  Compare the model from SO and file name
* Return       :  True success.False fail  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Tom Li          2011/07/14           First version                            
*****************************************************************************/
bool Funccheck::DoModuleCheck(TCHAR* sz_Model, TCHAR* sz_Model_get, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	bool b_res = false;
	int n_check = 0;
	CString str_OS_get = _T("");
	CString str_file_get = _T("");
	str_OS_get.Format("%s",sz_Model);
	str_file_get.Format("%s",sz_Model_get);

	n_check = strcmp(sz_Model_get,sz_Model);

	if(n_check == 0)
	{
		b_res = true;
	}
	else
	{
		CString str_ErrCode = _T("ET0027");
		CString str_ErrMsg = _T("SO查询的版本") + str_OS_get + _T("与文件名的版本信息") + str_file_get + _T("不一致");
		b_res = false;
	}

	return b_res;
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
bool Funccheck::ReadIniFile(CString& str_ServerPath, bool b_state)
{
	str_ServerPath = _T("");


	/* 检查ini文件是否存在 */
	int i_existentResult = 0;
	i_existentResult = _taccess(INIFILEPASH, 0);
	if(i_existentResult != 0) {
		return false;
	} 

	/* 读取INI文件 */
	TCHAR sz_buffer[128];
	memset(sz_buffer, 0, sizeof(sz_buffer));
	//::GetPrivateProfileString(_T("SERVER_INFO"), _T("SERVER_PATH"), _T(""), sz_buffer, sizeof(sz_buffer), INIFILEPASH);
	if(b_state)
	{
	  ::GetPrivateProfileString(_T("SERVER_INFO"), _T("NORMAL_PATH"), _T(""), sz_buffer, sizeof(sz_buffer), INIFILEPASH);
	}
	else
	{
		::GetPrivateProfileString(_T("SERVER_INFO"), _T("CR_PATH"), _T(""), sz_buffer, sizeof(sz_buffer), INIFILEPASH);
	}
	str_ServerPath.Format(_T("%s"),sz_buffer);

	if (str_ServerPath == _T("")) 
	{
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

	return true;
}


/*******************************************************************************************
* Function name: FindTooldirectory(CString str_dir,CString& str_directory, CString str_version)   
* Summary      : Look for Tool directory
* Parameters   : IN  CString str_dir,file directory
                 OUT CString& filepath   Contain str_Wildcard file path
*                
* Return       : int
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Tom Li         2012/02/03          First version             
**********************************************************************************************/
int Funccheck::FindTooldirectory(CString str_dir,CString& str_directory, CString str_version)
{
	if(str_dir.IsEmpty())
	{
		return 0;
	}
	int n_number = 0;
	str_directory = _T("");

	CFileFind finder;

	str_directory = str_dir;

	if (str_directory.Right(1) == _T("\\"))
	{
		str_directory = str_directory.Left(str_directory.GetLength()-1);
	}
	str_directory = str_directory;

	BOOL bWorking = finder.FindFile(str_dir);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if(finder.IsDots())
			continue;

		CString str_filePath = finder.GetFilePath();
		CString str_lastDirectory = _T("");
		CString str_model = _T("");
		str_model.Format(_T("%s"),m_sz_model_pre2);
		if(finder.IsDirectory())
		{
			str_lastDirectory = str_filePath.Mid(str_filePath.ReverseFind('\\'));
			if (m_bcommonTool)
			{
				if(str_lastDirectory.MakeUpper().Find(str_version.MakeUpper()) >= 0)
				{
					str_directory = str_filePath;
					n_number++;
				}

			}
			else if(str_lastDirectory.MakeUpper().Find(str_version.MakeUpper()) >= 0&&str_lastDirectory.MakeUpper().Find(str_model.MakeUpper()) >= 0)
			{
				str_directory = str_filePath;
				n_number++;
			}
		}
	}
	return n_number;
}

int Funccheck::FindResetTooldirectory(CString str_dir,CString& str_directory, CString str_version)
{
	if(str_dir.IsEmpty())
	{
		return 0;
	}
	int n_number = 0;
	str_directory = _T("");

	CFileFind finder;

	str_directory = str_dir;

	if (str_directory.Right(1) == _T("\\"))
	{
		str_directory = str_directory.Left(str_directory.GetLength()-1);
	}
	str_directory = str_directory;

	BOOL bWorking = finder.FindFile(str_dir);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if(finder.IsDots())
			continue;

		CString str_filePath = finder.GetFilePath();
		CString str_lastDirectory = _T("");
		CString str_model = _T("");
		str_model.Format(_T("%s"),m_sz_model_pre2);
		if(finder.IsDirectory())
		{
			str_lastDirectory = str_filePath.Mid(str_filePath.ReverseFind('\\'));
            if(str_lastDirectory.MakeUpper().Find(str_version.MakeUpper()) >= 0&&str_lastDirectory.MakeUpper().Find(str_model.MakeUpper()) >= 0)
			{
				str_directory = str_filePath;
				n_number++;
			}
		}
	}
	return n_number;
}


/*******************************************************************************************
* Function name: FindInformation(CString str_dir,CString& filepath, CString str_Wildcard)   
* Summary      : Look for MD5 files in str_dir directory
* Parameters   : IN  CString str_dir,file directory
                 OUT CString& filepath   Contain str_Wildcard file path
*                
* Return       : int
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Tom Li         2011/07/06          First version             
**********************************************************************************************/
int Funccheck::FindInformation(CString str_dir,CString& filepath, CString str_Wildcard)
{   
	if(str_dir.IsEmpty())
	{
		return 0;
	}
	int n_number = 0;
	filepath = _T("");

	CFileFind finder;

	CString str_directory = _T("");
	str_directory = str_dir;

	if (str_directory.Right(1) == _T("\\"))
	{
		str_directory = str_directory.Left(str_directory.GetLength()-1);
	}
	str_directory = str_directory + _T("\\") + str_Wildcard;;

	BOOL bWorking = finder.FindFile(str_directory);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		//if(str_directory.MakeUpper().Find(m_str_Model.MakeUpper()) >= 0)
		n_number++;
		filepath = finder.GetFilePath();
	}
	return n_number;
}


int Funccheck::FindInformationToolMD5(CString str_dir,CString& filepath, CString str_Wildcard)
{   
	CString str_model = _T("");
	str_model.Format(_T("%s"),m_sz_model_pre2);

	if(str_dir.IsEmpty())
	{
		return 0;
	}
	int n_number = 0;
	filepath = _T("");
    CString File_Path = _T("");
	CFileFind finder;

	CString str_directory = _T("");
	str_directory = str_dir;

	if (str_directory.Right(1) == _T("\\"))
	{
		str_directory = str_directory.Left(str_directory.GetLength()-1);
	}
	str_directory = str_directory + _T("\\") + str_Wildcard;;

	BOOL bWorking = finder.FindFile(str_directory);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		File_Path = finder.GetFilePath();
		if (m_bcommonTool)
		{
			filepath = File_Path;
			n_number++;
		}
		if(File_Path.MakeUpper().Find(str_model.MakeUpper()) >= 0)
		{
			filepath = File_Path;
			n_number++;
		}


	}
	return n_number;
}

int Funccheck::FindInformationResetToolMD5(CString str_dir,CString& filepath, CString str_Wildcard)
{   
	CString str_model = _T("");
	str_model.Format(_T("%s"),m_sz_model_pre2);
    CString File_Path = _T("");

	if(str_dir.IsEmpty())
	{
		return 0;
	}
	int n_number = 0;
	filepath = _T("");

	CFileFind finder;

	CString str_directory = _T("");
	str_directory = str_dir;

	if (str_directory.Right(1) == _T("\\"))
	{
		str_directory = str_directory.Left(str_directory.GetLength()-1);
	}
	str_directory = str_directory + _T("\\") + str_Wildcard;;

	BOOL bWorking = finder.FindFile(str_directory);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		File_Path = finder.GetFilePath();
		if(File_Path.MakeUpper().Find(str_model.MakeUpper()) >= 0)
		{
			filepath = File_Path;
			n_number++;
		}

	}
	return n_number;
}
/*******************************************************************************************
* Function name: Check_strLevel(CString strLevel,CString strProcess,CString strSegment,TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)   
* Summary      : Check Level 权限
* Parameters   : IN  CString strLevel,strProcess，strSegment
                 OUT 
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 2.0           Fox.yan        2012/05/03          Second version             
**********************************************************************************************/
bool Funccheck::Check_strLevel(CString strLevel,CString strProcess,CString strSegment,TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{   
	bool b_res = true;
	memset(sz_ErrCode,0,ERRORNUMBER);
	memset(sz_ErrMsg,0,ERRORNUMBER);
    CString str_log=_T("");
	str_log.Format(_T("检查权限开始，根据SO查询出来的权限为%s"),m_sz_Level);
	m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);
	str_log = _T("");
    

	if(strLevel== _T("24"))
	{
		if (strProcess.MakeUpper()!=_T("RUN_IN"))
		{
			m_Clog.WriteLog(_T("RUN_IN权限不能操作除RUN_IN之外的站别"),sz_ErrCode,sz_ErrMsg);
			CString str_Errcode = _T("ET0120");
			CString str_ErrMsg = _T("USER LEVEL IS RUN_IN, NOT OPERATE ") + strProcess;
			str_ErrMsg = str_ErrMsg + _T("\n用户RUN_IN权限不能操作") + strProcess;
			strcpy_s(sz_ErrCode,ERRORNUMBER,str_Errcode.GetBuffer());
			strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			str_Errcode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			return false;
		}
	}
	else if(strLevel== _T("30"))                //QA权限只能打开QA Tool
	{
		if(strProcess.MakeUpper()!=_T("FQC")&&strProcess.MakeUpper()!=_T("OOB"))
		{   
            m_Clog.WriteLog(_T("QA权限不能操作除FQC和OOB之外的站别"),sz_ErrCode,sz_ErrMsg);
			CString str_Errcode = _T("ET0108");
			CString str_ErrMsg = _T("User level not QA, can not operate ") + strProcess;
			str_ErrMsg = str_ErrMsg + _T("\n用户非QA权限不能操作") + strProcess;
			strcpy_s(sz_ErrCode,ERRORNUMBER,str_Errcode.GetBuffer());
			strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			str_Errcode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			return false;
		}
	}
	else if(strLevel == _T("21"))
	{
		if(strSegment.MakeUpper() != _T("TLINE"))
		{
			CString str_Errcode = _T("ET0030");
			CString str_ErrMsg = _T("User level TL, can not operate ") + strSegment;
			str_ErrMsg = str_ErrMsg + _T("\n用户TL权限不能操作") + strSegment;
			strcpy_s(sz_ErrCode,ERRORNUMBER,str_Errcode.GetBuffer());
			strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			str_Errcode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			return false;
		}
	}
	else if(strLevel == _T("22"))
	{
		if(strSegment.MakeUpper() != _T("FA"))
		{
			CString str_Errcode = _T("ET0031");
			CString str_ErrMsg = _T("User level FA, can not operate ") + strSegment;
			str_ErrMsg = str_ErrMsg + _T("\n用户FA权限不能操作") + strSegment;
			strcpy_s(sz_ErrCode,ERRORNUMBER,str_Errcode.GetBuffer());
			strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			str_Errcode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			return false;
		}
	}
	else if(strLevel == _T("23"))
	{
		if(strSegment.MakeUpper() != _T("PACKAGE"))
		{
			CString str_Errcode = _T("ET0032");
			CString str_ErrMsg = _T("User level PACKAGE, can not operate ") + strSegment;
			str_ErrMsg = str_ErrMsg + _T("\n用户PACKAGE权限不能操作") + strSegment;

			strcpy_s(sz_ErrCode,ERRORNUMBER,str_Errcode.GetBuffer());
			strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			str_Errcode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			return false;
		}
	}
	else if(strLevel == _T("20"))
	{
		return true;
	}
	else if(strLevel == _T("10"))
	{
		return true;
	}
	else
	{   
		CString str_Errcode = _T("ET0034");
		CString str_ErrMsg = strLevel +(" is not exist,please confirm\n");
		str_ErrMsg = str_ErrMsg + _T("不存在该权限 ") + strLevel +(" ,请您确认一下该权限");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_Errcode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_Errcode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}
return true;
}

bool Funccheck::PrCheck1(TCHAR* sz_SO,TCHAR* sz_employId, TCHAR* sz_Station, TCHAR* sz_Model, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	bool b_res = false;
	CString str_log = _T("");
    
	memset(sz_ErrCode,0,ERRORNUMBER);
	memset(sz_ErrMsg,0,ERRORNUMBER);

	strcpy(m_sz_employId,sz_employId);
	strcpy(m_sz_SO,sz_SO);
	m_Clog.WriteLog(_T("enter PrCheck1"));
	//b_res = Query_SOInfo(m_sz_SO,m_sz_Model,m_sz_PN,m_sz_SOType,sz_ErrCode,sz_ErrMsg);

	//initial class member
	memset(m_sz_model_pre2,0,50);//1
	memset(m_sz_partNo_pre2,0,50);//2
	memset(m_sz_soType_pre2,0,50);//3
	memset(m_sz_toolVer_pre2,0,50);//4
	memset(m_sz_facImageVer_SO_pre2,0,50);//5
	memset(m_sz_userImageVer_pre2,0,50);//6
	memset(m_sz_ETtoolVer_pre2,0,50);//7
	memset(m_sz_starttime_pre2,0,50);//8
	memset(m_sz_endtime_pre2,0,50);//9
	memset(m_sz_Status_factory_pre2,0,50);//10
	memset(m_sz_SWVersion_pre2,0,50);//11
	memset(m_sz_Status_Tool_pre2,0,50);//12
	memset(m_sz_toolVer_pr2,0,50);//13
	memset(m_sz_facImageVer_Tool_pre2,0,50);
	memset(m_sz_CRType_pre2,0,50);//so_info table, cloumn='CRType', the value is in 'C,B,D, ""'
	memset(m_sz_sz_Qty,0,50);
	memset(m_sz_segment_pre2,0,50);
	m_str_SOType = _T("");//so_info table:column='rework',the value in 'R' and 'N',that's mean the so is for reworkor normal
	m_str_ToolStatus = _T("");
	m_str_FactoryStatus = _T("");
	m_str_CRType = _T("");
	m_str_FacResetToolver = _T("");


	b_res = m_DBO.pQuerySO_Info_All_V4(sz_SO,m_sz_model_pre2,m_sz_partNo_pre2,m_sz_soType_pre2,m_sz_toolVer_pre2,m_sz_facImageVer_SO_pre2,m_sz_userImageVer_pre2,m_sz_ETtoolVer_pre2,m_sz_sz_Qty,m_sz_CRType_pre2,m_sz_segment_pre2,sz_ErrCode,sz_ErrMsg);

	//   strcpy(m_sz_partNo_pre2,"6Tja");
	//   strcpy(m_sz_toolVer_pre2,"v111.222");
	//strcpy(m_sz_facImageVer_SO_pre2,"i333.444");
	//   strcpy(m_sz_userImageVer_pre2,"select");

	//=================================================================Fox.Yan

	//if(strcmp(m_sz_soType_pre2,"")==0)
	//{
	if (b_res)
	{   

		m_Clog.WriteLog(_T("查找PART NUM , IMAGE , TOOL VERSION 在表VRCM中的记录，同时查看status值"),sz_ErrCode,sz_ErrMsg);

		CString str_PN = m_sz_partNo_pre2;
		CString str_PN_Type = str_PN.Left(2);
		char sz_status[10]={0};
		char sz_record_status[10]={0};
		if (str_PN_Type.MakeUpper()==_T("5E") || str_PN_Type.MakeUpper()==_T("5G") || str_PN_Type.MakeUpper()==_T("CS") || str_PN_Type.MakeUpper()==_T("9J")\
			|| str_PN_Type.MakeUpper()==_T("5J") || str_PN_Type.MakeUpper()==_T("6K") || str_PN_Type.MakeUpper()==_T("9B"))
		{
			b_res = m_DBO.pVMS_QueryImagePNToolVersionStatus(m_sz_model_pre2,m_sz_partNo_pre2,m_sz_facImageVer_SO_pre2,m_sz_userImageVer_pre2,m_sz_toolVer_pre2,sz_status,sz_record_status,sz_ErrCode,sz_ErrMsg);
		}
		else
		{
			CString str_Errcode = _T("ET0126");
			CString str_ErrMsg = _T("此工单查询得到的料号，非正确格式5E/5G/9J/CS/5J/6K/9B");
			strcpy_s(sz_ErrCode,ERRORNUMBER,str_Errcode.GetBuffer());
			strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			str_Errcode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			b_res = false;
		}

		if(!b_res)
		{   
			if (strcmp(sz_ErrCode,"NoRecord")==0)
			{   
				memset(sz_ErrCode,0,ERRORNUMBER);
				m_Clog.WriteLog("No record in VRCM, skip checking VRCM",sz_ErrCode,sz_ErrMsg);
				b_res = true;
			}
			else
			{
				b_res = false;
			}

		}
		else
		{
			if (strcmp(sz_status,"1")==0&&strcmp(sz_record_status,"2")==0)
			{
				b_res = true;
			}
			else
			{ 
				CString str_Errcode = _T("ET0125");
				CString str_ErrMsg = _T("在VRCM表中找到相应的记录，但status值不为1或Record_status不为2，无法用于生产");
				strcpy_s(sz_ErrCode,ERRORNUMBER,str_Errcode.GetBuffer());
				strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
				str_Errcode.ReleaseBuffer();
				str_ErrMsg.ReleaseBuffer();
				b_res = false;
			}
		}
	}


	//m_sz_Model = sz_Model;

	//strcpy_s(m_sz_model_pre2,15,sz_Model);
	//str_log = _T("函数PrCheck1开始执行");
	//m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);

	if(b_res)
	{   

		str_log.Format(_T("比较模块开始，用户输入的模块为%s,根据SO查询出来的模块为%s"),sz_Model,m_sz_model_pre2);
		m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);

		str_log = _T("");
		strcpy_s(m_sz_Model,15,sz_Model);

		if(strcmp(m_sz_model_pre2,sz_Model) != 0)
		{	//Model Name不一致
			CString str_Errcode = _T("ET0028");
			CString str_ErrMsg = _T("");
			str_ErrMsg.Format("模块名ModelName不一致SO查询得到为%s,输入值为%s",m_sz_model_pre2,sz_Model);
			strcpy_s(sz_ErrCode,ERRORNUMBER,str_Errcode.GetBuffer());
			strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			str_Errcode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			b_res = false;
		}
	}

	if(b_res)
	{
		m_str_FactoryImage.Format(_T("%s"),m_sz_facImageVer_SO_pre2);
		m_str_UserImage.Format(_T("%s"),m_sz_userImageVer_pre2);
		m_str_Toolver.Format(_T("%s"),m_sz_toolVer_pre2);
		m_str_FacResetToolver.Format(_T("%s"),m_sz_toolVer_pre2);
		str_log.Format(_T("根据SO查询得到UserImageVersion为%s,ToolVersion为%s,factoryImageVersion为%s"),m_str_UserImage,m_str_Toolver,m_str_FactoryImage);
		m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);
		str_log = _T("");

		if(strcmp(sz_Station,"RE_DL")==0)
		{   
			strcpy(m_sz_process,"RE_DL");
			m_str_process.Format(_T("%s"),sz_Station);
			return true;
		}
		if (strcmp(m_sz_CRType_pre2,"")==0)//normal so
		{
			if (strcmp(m_sz_segment_pre2,"FA")==0||strcmp(m_sz_segment_pre2,"TLINE")==0)
			{
				if (strcmp(m_sz_toolVer_pre2,"")==0||strcmp(m_sz_facImageVer_SO_pre2,"")==0||strcmp(m_sz_ETtoolVer_pre2,"")==0)
				{
					m_Clog.WriteLog(_T("Normal的工单在ALine和TLine必须有tool version ，factory version and  entrance tool verison存在"),sz_ErrCode,sz_ErrMsg);
					CString str_segment =_T("");
					str_segment.Format(_T("%s"),m_sz_segment_pre2);
					CString str_Errcode = _T("ET0130");
					CString str_ErrMsg = _T("Normal工单在")+str_segment+_T("站有信息为空\n");
					str_ErrMsg = str_ErrMsg + _T("ToolVersion为空或者\nFactoryImageVersion为空或者\nEntranceToolVersion为空");
					strcpy_s(sz_ErrCode,ERRORNUMBER,str_Errcode.GetBuffer());
					strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
					str_Errcode.ReleaseBuffer();
					str_ErrMsg.ReleaseBuffer();
					b_res = false;
				}
			}
			else if (strcmp(m_sz_segment_pre2,"PACKAGE")==0)
			{
				if (strcmp(m_sz_toolVer_pre2,"")==0||strcmp(m_sz_facImageVer_SO_pre2,"")==0||strcmp(m_sz_ETtoolVer_pre2,"")==0||strcmp(m_sz_userImageVer_pre2,"")==0)
				{
					m_Clog.WriteLog(_T("Normal工单在PLine站有信息为空"),sz_ErrCode,sz_ErrMsg);

					CString str_Errcode = _T("ET0131");
					CString str_ErrMsg = _T("Normal工单在PLine站有信息为空\nToolVersion为空或者\nImageVersion为空或者\nEntranceToolVersion为空或者\nUserImageVersion为空");
					strcpy_s(sz_ErrCode,ERRORNUMBER,str_Errcode.GetBuffer());
					strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
					str_Errcode.ReleaseBuffer();
					str_ErrMsg.ReleaseBuffer();
					b_res =  false;
				}
			}

		}

		if(b_res && (!m_str_UserImage.IsEmpty() && (m_str_Toolver.IsEmpty()||m_str_FactoryImage.IsEmpty())))
		{
			m_Clog.WriteLog(_T("查询得到的Toolversion或者imageversion为空,但是User Image不为空, 不合法"),sz_ErrCode,sz_ErrMsg);

			CString str_Errcode = _T("ET0111");
			CString str_ErrMsg = _T("PVCS系统中有ImageVersion但\nToolVersion为空或者\nFactoryImageVersion为空");
			strcpy_s(sz_ErrCode,ERRORNUMBER,str_Errcode.GetBuffer());
			strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			str_Errcode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			b_res = false;
		}
	}

	if(b_res)
	{
		b_res = Query_StationInfo(m_sz_line,m_sz_process,m_sz_segment,sz_ErrCode,sz_ErrMsg);
	m_str_process.Format(_T("%s"),sz_Station);
	}

	if(b_res)
	{
		str_log.Format(_T("检查站开始，根据电脑名查询出来的站为%s,用户输入的站为%s"),sz_Station,m_sz_process);
		m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);
		str_log = _T("");

		if(strcmp(m_sz_process,sz_Station) != 0)
		{//Station不一致
			CString str_Errcode = _T("ET0029");
			CString str_ErrMsg = _T("测试站别不一致,通过PCname查询站别为");
			str_ErrMsg = str_ErrMsg + m_sz_process;
			str_ErrMsg = str_ErrMsg + _T(" Process输入站别为");
			str_ErrMsg = str_ErrMsg + sz_Station;
			strcpy_s(sz_ErrCode,ERRORNUMBER,str_Errcode.GetBuffer());
			strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			str_Errcode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			b_res = false;
		}
	}
	if(b_res)
	{

		str_log.Format(_T("检查segment开始，根据电脑名查询出来的站为%s,用户输入的查询得到为%s"),m_sz_segment,m_sz_segment_pre2);
		m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);
		str_log = _T("");

		//if((str_Segment.MakeUpper()==_T("FA")&&str_segment_pc.MakeUpper()==_T("FQC"))||(str_Segment.MakeUpper()==_T("PACKAGE")&&str_segment_pc.MakeUpper()==_T("OOB")))
		//{

		//}
		//else if(str_Segment.MakeUpper()==_T("FA")&&m_str_process.MakeUpper()==_T("RUN_IN")||str_segment_pc.MakeUpper()==_T("RUN_IN"))
		//{

		//}
		//else
		if (strcmp(m_sz_segment,m_sz_segment_pre2)!=0)
		{//segment不一致
			if((strcmp(m_sz_segment_pre2, "FA") == 0 && strcmp(m_sz_segment, "FQC") == 0) ||
				(strcmp(m_sz_segment_pre2, "PACKAGE") == 0 && strcmp(m_sz_segment, "OOB") == 0) ||
				(strcmp(m_sz_segment_pre2, "FA") == 0 && strcmp(m_sz_segment, "RUN_IN") == 0) )
			{
			}
			else
			{

				CString str_Errcode = _T("ET0106");
				CString str_ErrMsg = _T("段别不一致,通过PCname查询得到当前段segment为：");
				str_ErrMsg = str_ErrMsg + m_sz_segment;
				str_ErrMsg = str_ErrMsg + _T("用户输入段segment为：");
				str_ErrMsg = str_ErrMsg + m_sz_segment_pre2;
				strcpy_s(sz_ErrCode,ERRORNUMBER,str_Errcode.GetBuffer());
				strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
				str_Errcode.ReleaseBuffer();
				str_ErrMsg.ReleaseBuffer();
				return false;
			}
		}


		//if(strProcess.MakeUpper()==_T("FQC"))
		if(strcmp(m_sz_process, "FQC") == 0)
		{  
			str_log.Format(_T("检查FQC||00B开始，根据电脑名查询出来的站为%s,用户输入的SO查询得到segment为%s"),m_sz_process,m_sz_segment_pre2);
			m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);
			str_log = _T("");

			//if(str_Segment.MakeUpper() != _T("FA"))
			if(strcmp(m_sz_segment_pre2, "FA") != 0)
			{   
				CString str_Errcode = _T("ET0109");
				CString str_ErrMsg = _T("FQC需在FA段操作");
				//str_ErrMsg = str_ErrMsg + _T("\n站不能操作") + str_Segment;
				strcpy_s(sz_ErrCode,ERRORNUMBER,str_Errcode.GetBuffer());
				strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
				str_Errcode.ReleaseBuffer();
				str_ErrMsg.ReleaseBuffer();
				return false;
			}

		}
		else if(strcmp(m_sz_process, "OOB") == 0)
//		else if(strProcess.MakeUpper()==_T("OOB"))
		{
			str_log.Format(_T("检查FQC||00B开始，根据电脑名查询出来的站为%s,用户输入的SO查询得到segment为%s"),m_sz_process,m_sz_segment_pre2);
			m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);
			str_log = _T("");
			//if(str_Segment.MakeUpper() != _T("PACKAGE"))
			if(strcmp(m_sz_segment_pre2, "PACKAGE") != 0)
			{   
				CString str_Errcode = _T("ET0110");
				CString str_ErrMsg = _T("OOB站需在PACKAGE段");
				//str_ErrMsg = str_ErrMsg + _T("\n站不能操作") + str_Segment;
				strcpy_s(sz_ErrCode,ERRORNUMBER,str_Errcode.GetBuffer());
				strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
				str_Errcode.ReleaseBuffer();
				str_ErrMsg.ReleaseBuffer();
				return false;
			}
		}

	}
	if(b_res)
		b_res = Query_ImployeeID(m_sz_employId,m_sz_Level,m_sz_status,m_sz_wrkstation,sz_ErrCode,sz_ErrMsg);
	m_str_status.Format(_T("%s"),m_sz_status);

	if(b_res)
	{   
		CString strSegment = m_sz_segment;
		CString strLevel = m_sz_Level;
		CString strProcess = m_sz_process;

		b_res=GetEmployeeIdOnly();

		str_log.Format(_T("检查EmployeeID唯一性开始，根据employeeID查询出来的ID状态为%s"),m_sz_status);
		m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);
		str_log = _T("");

		if(b_res&&m_str_status==_T("N"))
		{
			if(strcmp(m_sz_PCname,m_sz_wrkstation)==0)
			{
				b_res=true;
			}
			else
			{//PCname不一致
				m_Clog.WriteLog(_T("当前EMPLOYEEID是唯一性的，pcNAME不一致，导致fail"),sz_ErrCode,sz_ErrMsg);
				CString str_Errcode = _T("ET0107");
				CString str_ErrMsg = _T("当前PCname");
				str_ErrMsg = str_ErrMsg + m_sz_PCname;
				str_ErrMsg = str_ErrMsg + _T("employeeID查询得到PCname ");
				str_ErrMsg = str_ErrMsg + m_sz_wrkstation;
				str_ErrMsg = str_ErrMsg + _T("不一致且employeeID是唯一的不能重用");
				strcpy_s(sz_ErrCode,ERRORNUMBER,str_Errcode.GetBuffer());
				strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
				str_Errcode.ReleaseBuffer();
				str_ErrMsg.ReleaseBuffer();
				return false;
			}
		}
		else
		{
			b_res=true;
		}
		b_res = Check_strLevel(strLevel,strProcess,strSegment,sz_ErrCode,sz_ErrMsg);
	}
	return b_res;
}


/*******************************************************************************************
* Function name: MD5_CopyCheck(CString str_server_path, CString str_LOC_Path, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
* Summary      : Do MD5 check
* Parameters   : IN  CString str_server_path   Server MD5 directory
                 IN  CString str_LOC_Path      LOC MD5 directory
*                
* Return       : int
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Tom Li         2011/09/09          First version             
**********************************************************************************************/
bool Funccheck::MD5_CopyCheck(CString str_server_path, CString str_LOC_Path, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{  
    

	m_Clog.WriteLog(_T("进入MD5copy"),sz_ErrCode,sz_ErrMsg);
    m_Clog.WriteLog(_T("路径分别为："),sz_ErrCode,sz_ErrMsg);
	m_Clog.WriteLog(str_server_path,sz_ErrCode,sz_ErrMsg);
	m_Clog.WriteLog(str_LOC_Path,sz_ErrCode,sz_ErrMsg);

	if(str_server_path.IsEmpty())
	{
		CString str_ErrCode = _T("ET0001");
		CString str_ErrMsg = _T("Server Path is empty./n 服务器路径为空");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}

	if(str_LOC_Path.IsEmpty())
	{
		CString str_ErrCode = _T("ET0002");
		CString str_ErrMsg = _T("Local path of is empty. /n 本地路径为空");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}

	int i_check = -1;
	i_check = m_obj_DirectoryDispose.IsDirectoryExist(str_server_path);
	if(!i_check)
	{
		CString str_ErrCode = _T("ET0003");
		CString str_ErrMsg = _T("Server Path") + str_server_path + _T("is not exist");
		str_ErrMsg = str_ErrMsg + _T("Server路径") + str_server_path + _T("不存在");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
	    return false;
	}

	i_check = m_obj_DirectoryDispose.IsDirectoryExist(str_LOC_Path);
	if(i_check)
	{
	  i_check = m_obj_DirectoryDispose.DeleteDir(str_LOC_Path);
	  if(!i_check)
	  {
		  CString str_ErrCode = _T("ET0004");
		  CString str_ErrMsg = _T("Delete local path fail\n");
		  str_ErrMsg = str_ErrMsg + _T("删除本地路径") + str_LOC_Path + _T("失败");
		  strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		  strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		  str_ErrCode.ReleaseBuffer();
		  str_ErrMsg.ReleaseBuffer();
		  return false;
	  }
	}

	i_check = m_obj_DirectoryDispose.CreateDirectory(str_LOC_Path);
	if(!i_check)
	{
		CString str_ErrCode = _T("ET0005");
		CString str_ErrMsg = _T("Create local path fail\n");
		str_ErrMsg = str_ErrMsg + _T("创建本地路径") + str_LOC_Path + _T("失败");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}

	i_check = m_obj_DirectoryDispose.CopyDirectory(str_server_path,str_LOC_Path);
	if(i_check)
	{
		return true;
	}
	else
	{
		CString str_ErrCode = _T("ET0006");
		CString str_ErrMsg = _T("Copy from server to local fail\n");
		str_ErrMsg = str_ErrMsg + _T("从服务器") + str_server_path +  _T("拷贝到本地") +  str_LOC_Path + _T("失败");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}
}

CString Funccheck::GetProjectName()
{
   return m_str_ProjectName;
}

CString Funccheck::GetProcessName()
{
   return m_str_process;
}

/******************************************************************************
* function name:  GetToolName(CString& str_Toolpath, CString str_process)                               
* Parameters   :  OUT  CString& str_Toolpath  Tool path.
                  IN   CString str_process    process
* Summary      :  Output Toolpath according process
* Return       :  True  success.False  fail  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Tom Li          2011/09/09           First version                            
*****************************************************************************/
bool Funccheck::GetToolName(CString& str_Toolpath, CString str_process, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	bool b_res = true;
	if(str_process.IsEmpty())
		return false;
    CString strLevel = m_sz_Level;
	/* 读取INI文件 */
	TCHAR sz_buffer[512];
	memset(sz_buffer, 0, sizeof(sz_buffer));

	//if(str_process.MakeUpper() == _T("OS_DL"))
	//{
	//	::GetPrivateProfileString(_T("Tool_Path"), _T("OS_DL"), _T(""), sz_buffer, sizeof(sz_buffer), TOOLPATHCONFG);
	//    str_Toolpath.Format(_T("%s"),sz_buffer);
	//	memset(sz_buffer, 0, sizeof(sz_buffer));
	//	b_res = true;
	//}
	//else 
	//{
	//	::GetPrivateProfileString(_T("Tool_Path"), str_process.MakeUpper(), _T(""), sz_buffer, sizeof(sz_buffer), TOOLPATHCONFG);
	//	str_Toolpath.Format(_T("%s"),sz_buffer);
	//	memset(sz_buffer, 0, sizeof(sz_buffer));
	//	b_res = true;
	//}

	if(str_process.MakeUpper() == _T("VI")||str_process.MakeUpper() == _T("SHIPPINGOS")||str_process.MakeUpper() == _T("P_VI"))
	{
		str_Toolpath = _T("\\VI.exe");
	}
	//else if(str_process.MakeUpper() == _T("FALINK") || str_process.MakeUpper() == _T("PPIDLINK"))
	else if(strncmp("FALINK",m_sz_process,6)==0||strncmp("PPIDLINK",m_sz_process,8)==0)
	{
		str_Toolpath = _T("\\KeypartsLink.exe");
	}
	else if(str_process.MakeUpper() == _T("OOB") || str_process.MakeUpper() == _T("FQC"))
	{
		str_Toolpath = _T("\\QA.exe");
	}
	else if(str_process.MakeUpper() == _T("RUN_IN"))
	{
		str_Toolpath = _T("\\Run_in.exe");
	}
	else if(str_process.MakeUpper() == _T("ShippingQualityCtrl"))
	{
		str_Toolpath = _T("\\Run_in.exe");
	}
	else if(strLevel==_T("10"))
	{
       str_Toolpath = _T("\\") + str_process + _T("\\UnifyUI_TS.exe");
	}
	else
	{
		str_Toolpath = _T("\\") + str_process + _T("\\UnifyUI_FacTestTool.exe");
	}


	if(str_Toolpath.IsEmpty())
	{
		return false;
	}

	CString str_log = _T("");
	str_log = _T("函数GetToolName开始执行，传入参数") + str_process + _T("传出参数") + str_Toolpath;
	m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);

	return b_res;
}


/*******************************************************************************************
* Function name: ToolCopy(CString str_ServerPath, CString str_LocPath)  
* Summary      : Copy Tool from server to Local disk.
* Parameters   : IN   CString str_ServerPath   Server Path
                 IN   CString str_LocPath      Local Path
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Tom Li         2011/09/09          First version             
**********************************************************************************************/
bool Funccheck::ToolCopy(CString str_ServerPath, CString str_LocPath, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{

	bool b_res = false;

	if(str_ServerPath.IsEmpty())
	{
		CString str_ErrCode = _T("ET0001");
		CString str_ErrMsg = _T("Server路径为空");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}

	if(str_LocPath.IsEmpty() )
	{
		CString str_ErrCode = _T("ET0002");
		CString str_ErrMsg = _T("LOC路径为空");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}

	b_res = m_obj_DirectoryDispose.IsDirectoryExist(str_ServerPath);
	if(!b_res)
	{
		CString str_ErrCode = _T("ET0003");
		CString str_ErrMsg = _T("Server路径") + str_ServerPath + _T("不存在");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}

	b_res = m_obj_DirectoryDispose.IsDirectoryExist(str_LocPath);
	int i_checkresult = -1;
	if(b_res)
	{
		i_checkresult = m_obj_DirectoryDispose.DeleteDir(str_LocPath);
		if(i_checkresult == 0)
		{
			CString str_ErrCode = _T("ET0004");
			CString str_ErrMsg = _T("删除本地路径 ") + str_LocPath + _T("失败");
			strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
			strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			str_ErrCode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			return false;
		}
	}

	i_checkresult = m_obj_DirectoryDispose.CreateDirectory(str_LocPath);
	if(i_checkresult == 0)
	{
		CString str_ErrCode = _T("ET0005");
		CString str_ErrMsg = _T("创建本地路径") + str_LocPath + _T("失败");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}
	else
	{
	  b_res = true;
	}

	i_checkresult = m_obj_DirectoryDispose.CopyDirectory(str_ServerPath,str_LocPath);

	CString str_log = _T("");
	str_log = _T("将Tool从服务器") + str_ServerPath + _T("拷贝到本地") + str_LocPath;
	m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);

	if(i_checkresult != 1)
	{
		CString str_ErrCode = _T("ET0006");
		CString str_ErrMsg = _T("从SERVER") + str_ServerPath + _T("拷贝到本地") + str_LocPath + _T("失败");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}

	int n_check = 0;
	CString str_filepath = _T("");
	CString str_Wildcard = _T("*.zip");
	bool IsZip = true;
	n_check = FindInformation(str_LocPath,str_filepath, str_Wildcard);
	if(n_check != 1)
	{
		CString str_Wildcard = _T("*.7z");
		IsZip = false;
		n_check = FindInformation(str_LocPath,str_filepath, str_Wildcard);
		if(n_check != 1)
		{
			CString str_ErrCode = _T("ET0006");
			CString str_ErrMsg = _T("寻找Tool包失败");
			strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
			strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			str_ErrCode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			return false;
		}


	}
    //ytt
	if(IsZip)
	{
		b_res = Unziptool(str_filepath, str_LocPath, sz_ErrCode, sz_ErrMsg);
	}else{
		b_res = DecompressFile(str_filepath, str_LocPath, sz_ErrCode, sz_ErrMsg);
	}
	if(b_res)
	{
		b_res = DeleteExtrl_Tool(str_LocPath, sz_ErrCode, sz_ErrMsg);
	}

	return b_res;
}

/*******************************************************************************************
* Function name: DeleteExtrl_Tool(CString str_LocPath, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
* Summary      : Delete extral files.
* Parameters   : IN   CString str_LocPath    Tool path
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Tom Li         2012/02/02          First version             
**********************************************************************************************/
bool Funccheck::DeleteExtrl_Tool(CString str_LocPath, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	bool b_res = true;

	CFileFind finder;

	if(str_LocPath.Right(1) != _T("\\"))
	{
		str_LocPath = str_LocPath + _T("\\");
	}
	str_LocPath = str_LocPath + _T("*.*");

	b_res = finder.FindFile(str_LocPath);
	while(b_res)
	{
		b_res = finder.FindNextFile();//检查文件是否检查完
		if(finder.IsDots())
			continue;

		CString str_filePath = finder.GetFilePath();
		CString str_lastDirectory = _T("");
		if(finder.IsDirectory())
		{
			str_LocPath = str_filePath;
			continue;
		}
		else
		{
		  int i_check = DeleteFile(str_filePath);
		  if(!i_check)
		  {
			  CString str_ErrCode = _T("ET0004");
			  CString str_ErrMsg = _T("删除本地文件") + str_filePath + _T("失败");
			  strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
			  strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			  str_ErrCode.ReleaseBuffer();
			  str_ErrMsg.ReleaseBuffer();
			  return false;
		  }
		}
	}

	return true;
}

/*******************************************************************************************
* Function name: FcheckImageDirectory(TCHAR* szImageVersion,TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg) 
* Summary      : Check local directory.
* Parameters   : IN   TCHAR* szImageVersion    Image version
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Tom Li         2011/09/22          First version             
**********************************************************************************************/
bool Funccheck::FcheckImageDirectory(TCHAR* szImageVersion, CString& str_locPath, bool b_state, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{   
	m_Clog.WriteLog(_T("进入FcheckImageDirectory查找"),sz_ErrCode,sz_ErrMsg);
	CString str_ModelName = _T("");
	CString str_FindResult = _T("");
	CString str_ServerPath = _T("");
	CString str_ImageVersion = _T("");
	CString str_locDir = DIRECTORYPATH;
	str_ImageVersion.Format(_T("%s"),szImageVersion);
	if(str_ImageVersion.IsEmpty())
	{
		CString str_ErrCode = _T("ET0050");
		CString str_ErrMsg = _T("Image version is empty\n");
		str_ErrMsg = str_ErrMsg + _T("Image的版本信息为空");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}
	ReadIniFile(str_ServerPath, b_state);
	str_ModelName.Format(_T("%s"),m_sz_Model);
	if(str_ModelName.IsEmpty())
	{
		CString str_ErrCode = _T("ET0049");
		CString str_ErrMsg = _T("The model in config file Setup.ini is empty\n");
		str_ErrMsg = str_ErrMsg + _T("配置文件Setup.ini文件中的Model为空");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}
	str_locDir = str_locDir + str_ModelName;
	str_locDir = str_locDir + _T("\\");
	str_locDir = str_locDir + str_ImageVersion;
	//str_locDir = str_locDir;

    m_Clog.WriteLog(str_locDir,sz_ErrCode,sz_ErrMsg);  //log msg

	//CString strSerachrFolder = str_locDir + _T("*.*");
	CString strSerachrFolder = str_locDir;
    
	CFileFind finder;
	bool b_res = false;
	b_res = finder.FindFile(strSerachrFolder);
	while(b_res == true)
	{
		b_res = finder.FindNextFile();//检查文件是否检查完
		if(finder.IsDots())
			continue;
		if(finder.IsDirectory())
		{
			CString str_filePath = finder.GetFilePath();
			if(str_filePath.Find(str_ModelName) >= 0 && str_filePath.Find(str_ImageVersion) >= 0)
			{
				str_FindResult = str_filePath;
				break;
			}
		}
	}

	if(str_FindResult.IsEmpty())
	{   
        m_Clog.WriteLog(_T("imageDirectory为空"),sz_ErrCode,sz_ErrMsg);  //log msg
		return false;
	}
	else
	{   
		m_Clog.WriteLog(_T("imageDirectory为:"),sz_ErrCode,sz_ErrMsg);  //log msg
		m_Clog.WriteLog(str_FindResult,sz_ErrCode,sz_ErrMsg);  //log msg
		str_locPath = str_FindResult;
		return true;
	}
}

/*******************************************************************************************
* Function name: DeleteDirectory(CString str_Directory, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg) 
* Summary      : Delete specify directory.
* Parameters   : CString str_Directory    Directory path
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Tom Li         2011/10/12          First version             
**********************************************************************************************/
bool Funccheck::DeleteDirectory(CString str_Directory, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	bool b_res = false;
	if(str_Directory.IsEmpty())
	{
		CString str_ErrCode = _T("ET0019");
		CString str_ErrMsg = _T("The deleted file is empty\n");
		str_ErrMsg = str_ErrMsg + _T("要删除的路径为空");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}

	b_res = m_obj_DirectoryDispose.IsDirectoryExist(str_Directory);

	int i_checkresult = -1;

	if(b_res)
	{
		i_checkresult = m_obj_DirectoryDispose.DeleteDir(str_Directory);
		if(i_checkresult == 0)
		{
			CString str_ErrCode = _T("ET0019");
			CString str_ErrMsg = _T("Delete ") + str_Directory + _T("fail\n");
			str_ErrMsg = str_ErrMsg + _T("删除目录 ") + str_Directory + _T(" 失败");
			strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
			strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			str_ErrCode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			return false;
		}
	}

	return true;
}

bool Funccheck::DirectoryCopy(CString str_Dest, CString str_Source, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
   bool b_res = false;
   if(str_Dest.IsEmpty() || str_Source.IsEmpty())
   {
	   CString str_ErrCode = _T("ET0019");
	   CString str_ErrMsg = _T("The path passed by SD_COPY is empty\n");
	   str_ErrMsg =str_ErrMsg + _T("SD_COPY传进来的路径为空");
	   strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
	   strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
	   str_ErrCode.ReleaseBuffer();
	   str_ErrMsg.ReleaseBuffer();
	   return false;
   }

   b_res = m_obj_DirectoryDispose.IsDirectoryExist(str_Dest);

   int i_checkresult = -1;

   if(b_res)
   {
	   i_checkresult = m_obj_DirectoryDispose.DeleteDir(str_Dest);
	   if(i_checkresult == 0)
	   {
		   CString str_ErrCode = _T("ET0019");
		   CString str_ErrMsg = _T("Delete  ") + str_Dest + _T(" fail /n");
		   str_ErrMsg = str_ErrMsg + _T("删除目录 ") + str_Dest + _T(" 失败");
		   strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		   strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		   str_ErrCode.ReleaseBuffer();
		   str_ErrMsg.ReleaseBuffer();
		   return false;
	   }
   }

   i_checkresult = m_obj_DirectoryDispose.CreateDirectory(str_Dest);
   if(i_checkresult == 0)
   {
	   CString str_ErrCode = _T("ET0020");
	   CString str_ErrMsg = _T("create file fail\n ");
	   str_ErrMsg = str_ErrMsg + _T("在 ") + str_Dest + _T(" 下创建目录失败");
	   strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
	   strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
	   str_ErrCode.ReleaseBuffer();
	   str_ErrMsg.ReleaseBuffer();
	   return false;
   }

   i_checkresult = m_obj_DirectoryDispose.CopyDirectory(str_Source,str_Dest);

   	CString str_log = _T("");
	str_log = _T("将") + str_Source + _T("拷贝到") + str_Dest;
	m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);

   if(i_checkresult != 1)
   {
	   CString str_ErrCode = _T("ET0021");
	   CString str_ErrMsg = _T("Copy Image fail \n");
	   str_ErrMsg = str_ErrMsg + _T("拷贝Image失败");
	   strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
	   strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
	   str_ErrCode.ReleaseBuffer();
	   str_ErrMsg.ReleaseBuffer();
	   return false;
   }

   return true;
}

bool Funccheck::ImageCopyCheck(TCHAR* szProject, TCHAR* szImageVersion, CString& str_replace, bool b_state, bool b_ImageType, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{   
    m_Clog.WriteLog(_T("进入imagecopyCheck"),sz_ErrCode,sz_ErrMsg);

	CString str_ModelName = _T("");
//	CString str_ServerPath = _T("");

	CString str_Imageversion = _T("");
	str_Imageversion.Format(_T("%s"),szImageVersion);
	m_Clog.WriteLog(str_Imageversion,sz_ErrCode,sz_ErrMsg);

//	ReadIniFile(str_ServerPath, b_state);
//	str_ServerPath = m_str_FImageServerPath;
	str_ModelName.Format(_T("%s"),m_sz_Model);
	//if(str_ServerPath.Right(1) != _T("\\"))
	//	str_ServerPath += _T("\\");
	CString strFolder = m_str_FImageServerPath;//str_ServerPath + m_str_ModelProjectName;
	if(strFolder.Right(1) != _T("\\"))
		strFolder += _T("\\");

	//CString strtemp = _T("");
	//strtemp.Format("%s",szImageVersion);

	CString strVersion = _T("");
	CString strSource = _T("");
	CString strdest = _T("");
	strdest = DIRECTORYPATH;

	if(str_ModelName.IsEmpty())
	{
		CString str_ErrCode = _T("ET0049");
		CString str_ErrMsg = _T("配置文件Setup.ini文件中的Model为空");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}
	//strdest = strdest + str_ModelName;
	strdest = strdest + m_str_Model;             //add 1
	if(str_Imageversion.IsEmpty())
	{
		CString str_ErrCode = _T("ET0050");
		CString str_ErrMsg = _T("Image的版本信息为空");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}
	strdest = strdest + _T("\\") + str_Imageversion;

	strVersion = str_Imageversion.Mid(1,str_Imageversion.Find('.') - 1) + str_Imageversion.Right(str_Imageversion.GetLength() - str_Imageversion.Find('.') - 1);
	//strVersion = strtemp.Right(strtemp.GetLength() - strtemp.Find('.') - 1);
	//strVersion = strVersion

	//if(szProject == _T("Luna_LG7"))  //special case
	//if(strcmp(szProject,"Luna_LG7") == 0)
	//{	strFolder += _T("\\");
	//	strFolder += szImageVersion;
	//	strFolder += _T("\\");
	//}
	//else
	//{
	//	//strFolder += _T("\\SW_Load\\");
	//	if(b_ImageType)
	//	{
	//		strFolder += _T("\\Factory_SW_Load\\*.*");
	//	}
	//	else
	//	{
	//		strFolder += _T("\\SW_Load\\*.*");
	//	}
	//}
    m_Clog.WriteLog(_T("由ImageType得到image的server地址"),sz_ErrCode,sz_ErrMsg);
 	m_Clog.WriteLog(strFolder,sz_ErrCode,sz_ErrMsg);

	m_Clog.WriteLog(m_str_Model,sz_ErrCode,sz_ErrMsg);

	m_Clog.WriteLog(strVersion,sz_ErrCode,sz_ErrMsg);


	//strSource = strFolder;
	CString str_ServerImagepath = _T("");

	CFileFind finder;
	bool b_res = false;
	strFolder += _T("*.*");
	b_res = finder.FindFile(strFolder);
	while(b_res == true)
	{
		b_res = finder.FindNextFile();//检查文件是否检查完
		if(finder.IsDots())
			continue;
		if(finder.IsDirectory())
		{
			CString str_filePath = finder.GetFilePath();
			m_Clog.WriteLog(_T("检查server文件是否存在"),sz_ErrCode,sz_ErrMsg);
			m_Clog.WriteLog(str_filePath,sz_ErrCode,sz_ErrMsg);
			m_Clog.WriteLog(str_ModelName,sz_ErrCode,sz_ErrMsg);
			m_Clog.WriteLog(str_Imageversion,sz_ErrCode,sz_ErrMsg);

			if(str_filePath.MakeUpper().Find(m_str_Model.MakeUpper()) >= 0 && str_filePath.MakeUpper().Find(str_Imageversion.MakeUpper()) >= 0)
			{
				str_ServerImagepath = str_filePath;
				break;
			}
		}
	}


	//b_res = FcheckImageDirectory(szImageVersion, strSource, b_state,sz_ErrCode, sz_ErrMsg);


	if(str_ServerImagepath.IsEmpty())
	{
		CString str_ErrCode = _T("ET0018");
		CString str_ErrMsg = _T("Image file is empty\n");
	    str_ErrMsg = str_ErrMsg + _T("Image 路径 ") + str_ServerImagepath + _T(" 为空");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}

	if(!m_obj_DirectoryDispose.IsDirectoryExist(str_ServerImagepath))
	{
		CString str_ErrCode = _T("ET0018");
		CString str_ErrMsg = _T("Image file is not exist\n");
		str_ErrMsg = str_ErrMsg + _T("Image 路径 ") + str_ServerImagepath + _T(" 不存在");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}

	b_res = m_obj_DirectoryDispose.IsDirectoryExist(strdest);

	int i_checkresult = -1;

	if(b_res)
	{
		i_checkresult = m_obj_DirectoryDispose.DeleteDir(strdest);
		if(i_checkresult == 0)
		{
			CString str_ErrCode = _T("ET0019");
			CString str_ErrMsg = _T("删除目录 ") + strdest + _T(" 失败");
			strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
			strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			str_ErrCode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			return false;
		}
	}
	i_checkresult = m_obj_DirectoryDispose.CreateDirectory(strdest);
	if(i_checkresult == 0)
	{
		CString str_ErrCode = _T("ET0020");
		CString str_ErrMsg = _T("在 ") + strdest + _T(" 下创建目录失败");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}

	// strSource = \\10.85.58.13\Official\01.Project_related\Luna_LG7\V9.09\GLUNB1A190900
	if(str_ServerImagepath.Right(1) == _T("\\"))
	{
		str_ServerImagepath = str_ServerImagepath.Left(str_ServerImagepath.GetLength() - 1);
	}
	str_replace = str_ServerImagepath.Mid(strSource.ReverseFind('\\') + 1);
	//strdest = strdest + _T("\\") + str_replace;
	i_checkresult = m_obj_DirectoryDispose.CopyDirectory(str_ServerImagepath,strdest);


	CString str_log = _T("");
	str_log.Format(_T("拷贝Image开始，将Image从%s,拷贝到%s"),str_ServerImagepath,strdest);
	m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);

	if(i_checkresult != 1)
	{
		CString str_ErrCode = _T("ET0021");
		CString str_ErrMsg = _T("拷贝Image失败");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}


	return true;
}

bool Funccheck::ToolUpgrade(TCHAR* szProject, TCHAR* szToolVersion, TCHAR* szErrCode, TCHAR* szErrMsg)
{
	return true;
}

bool Funccheck::FDCSCheck(TCHAR* sz_SO, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	bool b_res = true;

    char sz_info[30]={0};
	m_str_CRType.Format(_T("%s"),m_sz_CRType_pre2);
	m_str_UserImage.Format(_T("%s"),m_sz_userImageVer_pre2);
	m_str_FactoryImage.Format(_T("%s"),m_sz_facImageVer_SO_pre2);

	m_str_Toolver.Format(_T("%s"),m_sz_toolVer_pre2);

	if(m_str_UserImage.IsEmpty())
	{
		if(m_str_Toolver.IsEmpty()||m_str_FactoryImage.IsEmpty())
		{   
            m_Clog.WriteLog(_T("工单查询得到的信息中，没有swVersion等信息，直接启用Tool，无copy等动作，需手动copy至本地目录"),sz_ErrCode,sz_ErrMsg);
			AfxMessageBox(_T("Image和Tool信息，系统均未准备，请手动copy至本地"));
			return true;
		}
	}

	//(*VMS_QueryFactoryImageStatus)(char * sz_Model, char * sz_ImageVer,char * sz_Status, char * sz_starttime, char * sz_endtime,char * sz_ErrCode, char * sz_ErrMsg );
	//input/Model, imageVer; output/status, error code, error message
	if(!m_str_FactoryImage.IsEmpty())
	{
		b_res = m_DBO.pVMS_QueryFactoryImageStatus(m_sz_model_pre2,m_sz_facImageVer_SO_pre2,m_sz_Status_factory_pre2,m_sz_starttime_pre2,m_sz_endtime_pre2,sz_ErrCode,sz_ErrMsg);
		if(!b_res)
		{   
			m_Clog.WriteLog(_T("VMS系统查询factoryImage出现fail"),sz_ErrCode,sz_ErrMsg);
			return false;
		}
	}
	////typedef bool (*VMS_QuerySQNinfo_V2)(char * sz_Model, char * sz_Partno, char * sz_SWver, st_9JPNData & st_PNinfo, char * sz_ErrCode, char * sz_ErrMsg );
	////input model/part no/sw ver; output/SQN_id, startTime, endTime
	if(!m_str_UserImage.IsEmpty()&&(strcmp(m_sz_process,"RE_DL")!=0))
	{
		//b_res = m_DBO.pVMS_QuerySQNinfo_V2(m_sz_model_pre2,m_sz_partNo_pre2,m_sz_userImageVer_pre2,st_PNinfo,sz_ErrCode,sz_ErrMsg);
		
		char sz_model[50]={0};
		char sz_partno[50]={0};
		char sz_swver[50]={0};

		char *pi;
		char sz_count[10]={0};
		char sz_9Jinfo[4096]={0};

		strcpy(sz_model,m_sz_model_pre2);
		strcpy(sz_partno,m_sz_partNo_pre2);
		strcpy(sz_swver,m_sz_userImageVer_pre2);

		b_res = m_DBO.pVMS_QuerySQNinfo_All(sz_model,sz_partno,sz_swver,sz_count,sz_9Jinfo,sz_ErrCode,sz_ErrMsg);
		
		if(!b_res)
		{   
			m_Clog.WriteLog(_T("VMS系统查询SQN出现fail"),sz_ErrCode,sz_ErrMsg);
			return false;
		}

		pi = strstr(sz_9Jinfo,"SQN_release_status");
		for (int i=0;i<25;i++)
		{  
			if(i==19)
				sz_info[0]=*(pi+i);

		}

	}
	if(m_str_FactoryImage.IsEmpty() && m_str_UserImage.IsEmpty())
	{
		strcpy_s(sz_ErrCode,ERRORNUMBER,_T("ET0102"));
		strcpy_s(sz_ErrMsg,ERRORNUMBER,_T("facImageVer 和UseImage不能同时为空"));
		return false;
	}
	if(m_bcommonTool)
	{
		//(char * sz_Model, char * sz_ToolVer,char * sz_Status, char * sz_ErrCode, char * sz_ErrMsg );
		CString str_model = _T("");
		b_res = GetCommonToolPath(m_str_process, str_model, sz_ErrCode, sz_ErrMsg);
		b_res = m_DBO.pVMS_QueryCommonToolVersion(str_model.GetBuffer(),m_sz_CRType_pre2,m_sz_toolVer_pre2, m_sz_Status_Tool_pre2, sz_ErrCode, sz_ErrMsg);
		str_model.ReleaseBuffer();
		if(b_res)
		{   
			m_str_SOType.Format(_T("%s"),m_sz_soType_pre2);
			m_str_ToolStatus.Format(_T("%s"),m_sz_Status_Tool_pre2);
			m_str_FactoryStatus.Format(_T("%s"),m_sz_Status_factory_pre2);
			m_str_UseStatus.Format(_T("%s"),sz_info);
			if(m_str_FactoryStatus == _T("1"))
			{
				m_bfactoryImagestate = true;
			}
			else
			{
				m_bfactoryImagestate = false;
			}

			if(m_str_ToolStatus == _T("1"))
			{
				m_btoolstate = true;
			}
			else
			{
				m_btoolstate = false;
			}

			if(m_str_UseStatus == _T("2"))
			{
				m_buserImagestate = true;
			}
			else
			{
				m_buserImagestate = false;
			}
			return true;
		}
		else
		{   
			return false;
		}
	}
	else
	{
		//input model/tool ver; output/image ver, status
	    b_res = m_DBO.pVMS_QueryToolStatus(m_sz_model_pre2,m_sz_toolVer_pre2,m_sz_facImageVer_Tool_pre2,m_sz_Status_Tool_pre2,m_sz_Reserve1_pre2,sz_ErrCode,sz_ErrMsg);
        
	}
	if(!b_res)
	{
		return false;
	}

	m_str_SOType.Format(_T("%s"),m_sz_soType_pre2);
	m_str_ToolStatus.Format(_T("%s"),m_sz_Status_Tool_pre2);
	m_str_FactoryStatus.Format(_T("%s"),m_sz_Status_factory_pre2);
	m_str_UseStatus.Format(_T("%s"),sz_info);
	if(m_str_FactoryStatus == _T("1"))
	{
		m_bfactoryImagestate = true;
	}
	else
	{
		m_bfactoryImagestate = false;
	}

	if(m_str_ToolStatus == _T("1"))
	{
		m_btoolstate = true;
	}
	else
	{
		m_btoolstate = false;
	}

	if(m_str_UseStatus == _T("2"))
	{
		m_buserImagestate = true;
	}
	else
	{
		m_buserImagestate = false;
	}

	if(m_str_CRType.IsEmpty())
	{   
		m_Clog.WriteLog(_T("CRType为空, normal SO"),sz_ErrCode,sz_ErrMsg);
		if(m_str_ToolStatus != _T("1"))
		{
			CString str_ErrCode = _T("ET0101");
			CString str_ErrMsg = _T("CRType is empty,means it's normal SO, ToolStatus is not 'R'\n CRType为空,正常工单，ToolStatus状态不为R");
			strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
			strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			str_ErrCode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			return false;
		}
		if(!m_str_FactoryImage.IsEmpty())
		{
          /*   
			TCHAR rule[]=",";
			TCHAR *result;

			CString str_find=_T("");
			int len=0;
			char m_sz_facImageVer_Tool_pre2[50]={"v50.013.01.g_00,v51.002.14.b_01"};
			result = strtok(m_sz_facImageVer_Tool_pre2, rule);

			while (result!=NULL) 
			{   
				TCHAR find[512]={0};
				len=strlen(result);
				memcpy(find,result,len);
				str_find.Format(_T("%s"),find);
				result = strtok(NULL, rule); 
			}

            char m_sz_facImageVer_Tool_pre2[50]={"v50.013.01.g_00,v51.002.13.b_01"};
			TCHAR *result;
			result = strstr(m_sz_facImageVer_Tool_pre2,m_sz_facImageVer_SO_pre2);
			if(result==NULL)
			{
				CString str_ErrCode = _T("ET0102");
				CString str_ErrMsg = _T("facImageVer query by Tool");
				str_ErrMsg = str_ErrMsg + m_sz_facImageVer_Tool_pre2;
				str_ErrMsg = str_ErrMsg + _T("和SO查询出来的不一致");
				str_ErrMsg = str_ErrMsg + m_sz_facImageVer_SO_pre2;
				strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
				strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
				str_ErrCode.ReleaseBuffer();
				str_ErrMsg.ReleaseBuffer();
				return false;
			}
*/          
			//char m_sz_facImageVer_Tool_pre2[50]={"g_21,v52.002.14.b_01"};

			//if(strstr(m_sz_facImageVer_Tool_pre2,m_sz_facImageVer_SO_pre2) == NULL)
			//{   
			//	CString str_ErrCode = _T("ET0102");
			//	CString str_ErrMsg = _T("facImageVer query by Tool");
			//	str_ErrMsg = str_ErrMsg + m_sz_facImageVer_Tool_pre2;
			//	str_ErrMsg = str_ErrMsg + _T("和SO查询出来的不一致,PVCS系统中tool版本维护不一致");
			//	str_ErrMsg = str_ErrMsg + m_sz_facImageVer_SO_pre2;
			//	strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
			//	strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			//	str_ErrCode.ReleaseBuffer();
			//	str_ErrMsg.ReleaseBuffer();
			//	return false;
			//}

			if(m_str_FactoryStatus != _T("1"))
			{
				CString str_ErrCode = _T("ET0103");
				CString str_ErrMsg = _T("CRType is empty,FactoryStatus is not 'R' \n CRType为空,正常工单,FactoryStatus状态不为R");
				strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
				strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
				str_ErrCode.ReleaseBuffer();
				str_ErrMsg.ReleaseBuffer();
				return false;
			}
		}
		if(!m_str_UserImage.IsEmpty()&&(strcmp(m_sz_process,"RE_DL")!=0))
		{
			if(m_str_UseStatus !=_T("2"))
			{
				// str_ErrCode = _T("ET0104");
				//CString str_ErrMsg = _T("CRType为空,end user image status状态不为R");
				strcpy_s(sz_ErrCode,ERRORNUMBER,_T("ET0104"));
				strcpy_s(sz_ErrMsg,ERRORNUMBER,_T("CRType is empty,and user image status is not 'R' CRType为空,正常工单,and user image status状态不为R"));
				return false;
			}
		}

		b_res = true;
	}
	else
	{   
		
		if(m_str_CRType.MakeUpper() == _T("C") || m_str_CRType.MakeUpper() == _T("B")|| m_str_CRType.MakeUpper() == _T("D"))
		{   
			m_Clog.WriteLog(_T("CRType为C或B或D"),sz_ErrCode,sz_ErrMsg);
            m_Clog.WriteLog(m_str_CRType,sz_ErrCode,sz_ErrMsg);
//mark
			/*
			if(m_str_ToolStatus == _T("0") || m_str_FactoryStatus == _T("0") || st_PNinfo.SQN_status == 1)
			{
			}
			else
			{   
				m_Clog.WriteLog(_T("CRType为C或者B时，toolStauts，factory status, os Image status的状态不为0不匹配"),sz_ErrCode,sz_ErrMsg);
				CString str_ErrCode = _T("ET0105");
				CString str_ErrMsg = _T("Tool or Factory status in not 'V'\n Tool或Factory状态不为V");
				strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
				strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
				str_ErrCode.ReleaseBuffer();
				str_ErrMsg.ReleaseBuffer();
				return false;
			}
			*/
			//if(m_str_ToolStatus == _T("0") || st_PNinfo.SQN_status == 0)
			//{
			//}
			//else
			//{   
			//	m_Clog.WriteLog(_T("CRType为C或者B时PNinfo.SQN_status的状态不为0，显示出错"),sz_ErrCode,sz_ErrMsg);
			//	CString str_ErrCode = _T("ET0105");
			//	CString str_ErrMsg = _T("Tool or SQN status is not 'V'\n Tool或SQN状态不为V");
			//	strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
			//	strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			//	str_ErrCode.ReleaseBuffer();
			//	str_ErrMsg.ReleaseBuffer();
			//	return false;
			//}
			b_res = true;
		}
		else
		{   
			m_Clog.WriteLog(_T("未定义的 CRType"),sz_ErrCode,sz_ErrMsg);
			CString str_ErrCode = _T("ET0100");
			CString str_ErrMsg = _T("SOType status is fault\n此SO Type未定义");
			strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
			strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			str_ErrCode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			return false;
		}
	}

	return b_res;
}


/******************************************************************************
* function name:  Pre2_check(TCHAR* szErrCode, TCHAR* szErrMsg)                               
* Parameters   :  BOOL 
* Summary      :  Check userImage,factoryImage,model,EntranceTool version status
* Return       :  True  success.False  fail  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Tom Li          2012/01/04           First version                            
*****************************************************************************/
bool Funccheck::Pre2_check(CString str_ETVersion, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	bool b_res = true;
	m_Clog.WriteLog(_T("enter Pre2_check"));
	  m_str_UserImage.Format(_T("%s"),m_sz_userImageVer_pre2);
	  m_str_FactoryImage.Format(_T("%s"),m_sz_facImageVer_SO_pre2);
	  m_str_EntranceToolver.Format(_T("%s"),m_sz_ETtoolVer_pre2);
	  m_str_Toolver.Format(_T("%s"),m_sz_toolVer_pre2);
      m_str_Model.Format(_T("%s"),m_sz_model_pre2);


	if(m_str_Model.IsEmpty())
	{
		CString str_ErrCode = _T("ET0054");
		CString str_ErrMsg = _T("Model Name is empty\n Model Name 的信息为空");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}
	if(m_str_EntranceToolver.IsEmpty())
	{
		CString str_ErrCode = _T("ET0052");
		CString str_ErrMsg = _T("EntranceTool version is empty \nEntranceTool 的版本信息为空");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}

 //   TCHAR sz_buffer[50];
	//CString str_check=_T("");
 //   ::GetPrivateProfileString(_T("pre2_EntraceTool_Check_Version"), _T("check"), _T(""), sz_buffer, sizeof(sz_buffer), INIFILEPASH);
 //   str_check.Format(_T("%s"),sz_buffer);
 //   if(str_check==_T("1"))
	//{
	//	if(str_ETVersion.MakeUpper() != m_str_EntranceToolver.MakeUpper())
	//	{
	//		CString str_ErrCode = _T("ET0052");
	//		CString str_ErrMsg = _T("EntranceTool version are not the same between DB and tool \n从数据库获得的EntranceTool的版本信息与工具的版本不一致");
	//		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
	//		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
	//		str_ErrCode.ReleaseBuffer();
	//		str_ErrMsg.ReleaseBuffer();
	//		return false;
	//	}

	//}


    if(m_str_UserImage.IsEmpty())
	{
		if(m_str_FactoryImage.IsEmpty()||m_str_Toolver.IsEmpty())
		{
			return true;
		}
	}
	if(m_str_Toolver.IsEmpty())
	{
		CString str_ErrCode = _T("ET0051");
		CString str_ErrMsg = _T("Tool version is empty\n");
		str_ErrMsg = str_ErrMsg + _T("工具的版本信息为空");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}
		return b_res;
}

/******************************************************************************
* function name:  DO_SD_Copy(TCHAR* szErrCode, TCHAR* szErrMsg)                         
* Parameters   :  BOOL 
* Summary      :  copy SD and do MD5 check of SD_Copy
* Return       :  True  success.False  fail  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Tom Li          2012/01/04           First version                            
*****************************************************************************/
bool Funccheck::DO_SD_Copy(CString str_ProjectName, bool b_state, TCHAR* szErrCode, TCHAR* szErrMsg)
{
	CString str_log = _T("");
	str_log = _T("函数DO_SD_Copy开始执行");
	m_Clog.WriteLog(str_log,szErrCode,szErrMsg);

	bool b_res = true;
	CString str_Server = _T("");
	CString str_Server_MD5 = _T("");
	CString str_Loc = _T("");
	CString str_Loc_MD5 = _T("");
	TCHAR sz_buffer[128];
	memset(sz_buffer, 0, sizeof(sz_buffer));
	if(b_state)
	{
	  ::GetPrivateProfileString(_T("SERVER_INFO"), _T("NORMAL_PATH"), _T(""), sz_buffer, sizeof(sz_buffer), INIFILEPASH);
	}
	else
	{
		::GetPrivateProfileString(_T("SERVER_INFO"), _T("CR_PATH"), _T(""), sz_buffer, sizeof(sz_buffer), INIFILEPASH);
	}
	str_Server.Format(_T("%s"),sz_buffer);// Server MD5 directory
	str_Server_MD5 = str_Server;
	str_Server = str_Server + str_ProjectName + _T("\\") + _T("SD_Content") + _T("\\");
	str_Server_MD5 = str_Server_MD5 + str_ProjectName + _T("\\")  + _T("MD5") + _T("\\") + _T("SD_Content") + _T("\\");

	CString str_wild = _T("");
	str_wild.Format(_T("%s"),_T("*"));
	str_wild =str_wild  + st_PNinfo.SD_version;
	CString str_SerDetail = _T("");
	int i_checknumber = FindInformation(str_Server,str_SerDetail,str_wild);

	str_Loc = _T("C:\\");
	str_Loc = str_Loc + m_str_Model + _T("\\MICROSD_CONTENT\\");
	str_Loc = str_Loc + st_PNinfo.SD_version;

	str_Loc_MD5 = _T("C:\\Image\\");
	str_Loc_MD5 = str_Loc_MD5 + m_str_Model;
	str_Loc_MD5 = str_Loc_MD5 + _T("\\");
	str_Loc_MD5 = str_Loc_MD5 + _T("MD5");
	str_Loc_MD5 = str_Loc_MD5 + _T("\\");
	str_Loc_MD5 = str_Loc_MD5 + _T("SD_Content");

	bool b_DBresult = DirectoryCopy(str_Loc, str_SerDetail, szErrCode, szErrMsg);

	str_log = _T("");
	str_log = _T("将SD的内容从") + str_SerDetail + _T("拷贝到本地") + str_Loc;
	m_Clog.WriteLog(str_log,szErrCode,szErrMsg);

	if(!b_DBresult)
	{
		return false;
	}

	b_DBresult = DirectoryCopy(str_Loc_MD5, str_Server_MD5, szErrCode, szErrMsg);

	str_log = _T("");
	str_log = _T("将SD的MD5内容从") + str_Server_MD5 + _T("拷贝到本地") + str_Loc_MD5;
	m_Clog.WriteLog(str_log,szErrCode,szErrMsg);

	if(!b_DBresult)
	{
		return false;
	}
	str_wild = _T("");
	str_wild.Format(_T("%s"),_T("*"));
	str_wild =str_wild + st_PNinfo.SD_version;
	str_wild = str_wild + _T(".xml");
	CString str_MD5_File = _T("");
	i_checknumber = FindInformation(str_Loc_MD5,str_MD5_File,str_wild);

	CString str_replace = _T("");
	str_replace = _T("C:\\");

	str_replace = str_replace + m_str_Model;

	b_DBresult = m_obj_MD5check.Do_MD5_Check(str_Loc, str_MD5_File,str_replace, szErrCode, szErrMsg);
	if(!b_DBresult)
	{
		return false;
	}
	else
	{
		return true;
	}

	return b_res;
}

bool Funccheck::PrCheck2(CString str_ModelProjectName, CString str_ProjectName, CString& str_ImagePath, TCHAR* sz_SO, CString str_ETVersion, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	m_Clog.WriteLog(_T("enter PrCheck2"));
	m_bcommonTool = CheckCommonTool(m_str_process,sz_ErrCode,sz_ErrMsg);

	if(m_bcommonTool)
	{
		m_Clog.WriteLog(_T("CheckCommonTool得到的结果是True为commonTool"),sz_ErrCode,sz_ErrMsg);
	}
	else
	{
		m_Clog.WriteLog(_T("CheckCommonTool得到的结果是false不为commonTool"),sz_ErrCode,sz_ErrMsg);
	}

	if(str_ProjectName.IsEmpty())
	{   
		m_Clog.WriteLog(_T("projectName为空"),sz_ErrCode,sz_ErrMsg);
		return false;
	}
	m_str_ModelProjectName = str_ModelProjectName;
	if(m_str_ModelProjectName.IsEmpty())
	{   
		m_Clog.WriteLog(_T("ModelProjectName为空"),sz_ErrCode,sz_ErrMsg);
		return false;
	}

	//AfxMessageBox(_T("Check2 开始"), MB_OK);

	m_str_ProjectName = str_ProjectName;
	bool b_res = false;
	CString str_model = _T("");
	CString str_station = _T("");
	CString str_version = _T("");
	//CString str_version_ET = _T("");// EntranceTool Version
	CString str_Server = _T("");
	CString str_replace = _T("");  //This parameter will be used to convert *MD5.XML 
	CString str_status = _T("");
	char sz_model[50] = {0};
	char sz_partNo[50] = {0};
	char sz_soType[50] = {0};
	char sz_toolVer[50] = {0};
	char sz_ETtoolVer[50] = {0};
	char sz_facImageVer[50] = {0};
	char sz_userImageVer[50] = {0};
	char sz_ImageVer[50] = {0};
	char sz_Status[50] = {0};
	char sz_Status_factory[50] = {0};
	char sz_Status_Tool[50] = {0};
	char sz_starttime[50] = {0};
	char sz_endtime[50] = {0};
	char sz_SWVersion[50] = {0};
	//ReadIniFile(str_Server,b_state);
	//Query_SOInfo(sz_SO,m_sz_Model,m_sz_PN,m_sz_SOType,sz_ErrCode,sz_ErrMsg);
	//Query_StationInfo(m_sz_line,m_sz_process,m_sz_segment,sz_ErrCode,sz_ErrMsg);
	str_station.Format(_T("%s"),m_sz_process);
	m_str_HashFilepath_Loc = _T("");

	memset(sz_ErrCode,0,ERRORNUMBER);
	memset(sz_ErrMsg,0,ERRORNUMBER);
	m_Clog.WriteLog(_T("FDCSCheck"));
	b_res = FDCSCheck(sz_SO, sz_ErrCode, sz_ErrMsg);
	if(!b_res)
	{
		return false;
	}

	/*b_res = m_DBO.pQuerySO_Info_All(sz_SO,m_sz_model_pre2,m_sz_partNo_pre2,m_sz_soType_pre2,m_sz_toolVer_pre2,m_sz_facImageVer_SO_pre2,m_sz_userImageVer_pre2,m_sz_ETtoolVer_pre2,sz_ErrCode,sz_ErrMsg);
	if(!b_res)
	{
	return false;
	}*/

	b_res = Pre2_check(str_ETVersion,sz_ErrCode, sz_ErrMsg);
	if(!b_res)
	{
		return false;
	}

	m_Clog.WriteLog(_T("check user image"));
	if(m_str_UserImage.IsEmpty())
	{
		if(m_str_FactoryImage.IsEmpty()||m_str_Toolver.IsEmpty())
		{
			m_Clog.WriteLog(_T("FactoryImage or Toolver empty ->return true"));
			return true;
		}
	}


	CString strTemp = m_str_Model.Mid(4,2);
	if(strTemp == _T("CS"))
	{
		CString strNum = m_str_Model.Right(1);
		CString strTemp1 = m_str_Model.Left(4) + _T("B") + strNum + _T("A");
		m_str_Model = strTemp1;
		strTemp1 = m_str_ModelProjectName.Left(4) + _T("B") + strNum + _T("A") + m_str_ModelProjectName.Mid(7);
		m_str_ModelProjectName = strTemp1;
	}
	else
	{
		strTemp = m_str_Model.Left(1);
		if(strTemp == _T("T") || strTemp == _T("S"))
		{
			m_str_Model.SetAt(0,'G');
			m_str_ModelProjectName.SetAt(0, 'G');
		}
	}
	m_Clog.WriteLog(_T("Model Name = ")+m_str_Model,sz_ErrCode,sz_ErrMsg);

	m_Clog.WriteLog(_T("Model Project Name = ")+m_str_ModelProjectName,sz_ErrCode,sz_ErrMsg);

/*	if(m_str_Model.Right(3).MakeUpper() == _T("CS1"))
	{
		m_str_Model.MakeUpper().Replace(_T("CS1"),_T("B1A"));
		m_str_ModelProjectName.MakeUpper().Replace(_T("CS1"),_T("B1A"));
	}
	else if(m_str_Model.Right(3).MakeUpper() == _T("CS2"))
	{
		m_str_Model.MakeUpper().Replace(_T("CS2"),_T("B2A"));
		m_str_ModelProjectName.MakeUpper().Replace(_T("CS2"),_T("B2A"));
	}
	else if(m_str_Model.Right(3).MakeUpper() == _T("CS3"))
	{
		m_str_Model.MakeUpper().Replace(_T("CS3"),_T("B3A"));
		m_str_ModelProjectName.MakeUpper().Replace(_T("CS3"),_T("B3A"));
	}
	else if(m_str_Model.Right(3).MakeUpper() == _T("CS4"))
	{
		m_str_Model.MakeUpper().Replace(_T("CS4"),_T("B4A"));
		m_str_ModelProjectName.MakeUpper().Replace(_T("CS4"),_T("B4A"));
	}
*/

	CString strProject = _T("");
	CString strImageType = _T("");
	CString strVariant = _T("");
	CString strModel = _T("");
	CString strQTTOfficialURL = _T("");
	CString strQTTVerifyURL = _T("");
	CString strQCSOfficialURL = _T("");
	CString strQCSVerifyURL = _T("");
	CString strFactoryImageFolder = _T("");
	CString strFactoryToolFolder = _T("");
	CString strSWImageFolder = _T("");
	CString strServiceToolFolder = _T("");
	CString strDataIOFolder = _T("");
	CString strFactoryImageMD5Folder = _T("");
	CString strSWMD5Folder = _T("");
	CString strFactoryToolMD5Folder = _T("");
	CString strDataIOMD5Folder = _T("");
	bool bNewFolderStructure = false;
	TCHAR sz_MapPath[128] ={0};

	CString strCheckVariantImage;
	if(str_station.MakeUpper() == _T("TRIGGER") || str_station.MakeUpper() == _T("DL")||str_station.MakeUpper()==_T("RE_DL")||str_station.MakeUpper() == _T("OSDL"))
	{ 
		strCheckVariantImage = m_str_FactoryImage;
	}else{
		strCheckVariantImage = m_str_UserImage;
	}


	::GetPrivateProfileString(_T("SERVER_INFO"), _T("MAP_PATH"), _T("\\\\10.85.58.13\\Official\\01.Project_related\\01_ProjectModelPathTable\\"), sz_MapPath, sizeof(sz_MapPath), INIFILEPASH);

	CString strMapPath = sz_MapPath;
	strMapPath += _T("ProjectModelPathTable.csv");

	m_Clog.WriteLog(_T("Map Path: ") + strMapPath);
	CStdioFile MapFile;
	if(MapFile.Open(strMapPath.GetBuffer(), CFile::modeRead))
	{
		m_Clog.WriteLog(_T("Open Map file success!"));
		CString strMapContent;
		while(MapFile.ReadString(strMapContent))
		{
			if(strMapContent.Find(_T("QTT_Offical_URL")) != -1) // first line
				continue;

			int nIndex = strMapContent.Find(this->m_str_Model) ;
			if(nIndex == -1 ) //not this model
				continue;
			else // find this model
			{
				//SO_INFO -> SW VERSION variant match mapping table variant ?
				// ONE_00_04.xxx.xx.xx  => get 00
				CString strCheckVariant;
				int nUnderlineLoc = strCheckVariantImage.Find(_T("_")) +1;
				if(nUnderlineLoc>0)
				{
					 strCheckVariant = strCheckVariantImage.Mid(nUnderlineLoc,2);
					 strCheckVariant = _T("[")+strCheckVariant+_T("]");
					 m_Clog.WriteLog(_T("Variant for SO_INFO: ") +  strCheckVariant);
				}

				bNewFolderStructure = true;
				int nDotIndex = strMapContent.Find(_T(","));
				strProject = strMapContent.Left(nDotIndex); //Project
				m_Clog.WriteLog(_T("Project : ") + strProject);

				CString strTemp = strMapContent.Mid(nDotIndex + 1);
				nDotIndex = strTemp.Find(_T(","));
				strImageType = strTemp.Left(nDotIndex); //ImageType;
				m_Clog.WriteLog(_T("ImageType: ") + strImageType);

				strTemp = strTemp.Mid(nDotIndex+1);
				nDotIndex = strTemp.Find(_T(","));
				strVariant = strTemp.Left(nDotIndex);//Variant
				m_Clog.WriteLog(_T("Variant: ") + strVariant);
          
				if(0!=strVariant.Compare(strCheckVariant))
				{
					 m_Clog.WriteLog(_T("Variant not match ") );
					continue;
				}

				strTemp = strTemp.Mid(nDotIndex+1);
				nDotIndex = strTemp.Find(_T(","));
				strModel = strTemp.Left(nDotIndex); //Model
				m_Clog.WriteLog(_T("Model: ") + strModel);

				strTemp = strTemp.Mid(nDotIndex+1);
				nDotIndex = strTemp.Find(_T(","));
				strQTTOfficialURL = strTemp.Left(nDotIndex); //QTT Officail URL
				m_Clog.WriteLog(_T("QTT Official URL: ") + strQTTOfficialURL);

				strTemp = strTemp.Mid(nDotIndex+1);
				nDotIndex = strTemp.Find(_T(","));
				strQTTVerifyURL = strTemp.Left(nDotIndex); //QTT Verify URL
				m_Clog.WriteLog(_T("QTT Verify URL: ") + strQTTVerifyURL);

				strTemp = strTemp.Mid(nDotIndex+1);
				nDotIndex = strTemp.Find(_T(","));
				strQCSOfficialURL = strTemp.Left(nDotIndex); //QCS Official URL
				m_Clog.WriteLog(_T("QCS Official URL: ") + strQCSOfficialURL);

				strTemp = strTemp.Mid(nDotIndex+1);
				nDotIndex = strTemp.Find(_T(","));
				strQCSVerifyURL = strTemp.Left(nDotIndex); //QCS Verify URL
				m_Clog.WriteLog(_T("QCS Verify URL: ") + strQCSVerifyURL);

				strTemp = strTemp.Mid(nDotIndex+1);
				nDotIndex = strTemp.Find(_T(","));
				strFactoryImageFolder = strTemp.Left(nDotIndex); //Factory Image folder
				m_Clog.WriteLog(_T("Factory Image Folder: ") + strFactoryImageFolder);

				strTemp = strTemp.Mid(nDotIndex+1);
				nDotIndex = strTemp.Find(_T(","));
				strSWImageFolder = strTemp.Left(nDotIndex); //SW Load Folder
				m_Clog.WriteLog(_T("SW Load Folder: ") + strSWImageFolder);

				strTemp = strTemp.Mid(nDotIndex+1);
				nDotIndex = strTemp.Find(_T(","));
				strFactoryToolFolder = strTemp.Left(nDotIndex); //Factory Tool Folder
				m_Clog.WriteLog(_T("Factory Tool Folder: ") + strFactoryToolFolder);

				strTemp = strTemp.Mid(nDotIndex+1);
				nDotIndex = strTemp.Find(_T(","));
				strServiceToolFolder = strTemp.Left(nDotIndex); //Service Tool Folder
				m_Clog.WriteLog(_T("Service Tool Folder : ") + strServiceToolFolder);

				strTemp = strTemp.Mid(nDotIndex+1);
				nDotIndex = strTemp.Find(_T(","));
				strDataIOFolder = strTemp.Left(nDotIndex); //DataIO Folder
				m_Clog.WriteLog(_T("DataIO Folder: ") + strDataIOFolder);

				strTemp = strTemp.Mid(nDotIndex+1);
				nDotIndex = strTemp.Find(_T(","));
				strFactoryImageMD5Folder = strTemp.Left(nDotIndex); //FactoryImage MD5 Folder
				m_Clog.WriteLog(_T("Factory Image MD5 Folder: ") + strDataIOFolder);

				strTemp = strTemp.Mid(nDotIndex+1);
				nDotIndex = strTemp.Find(_T(","));
				strSWMD5Folder = strTemp.Left(nDotIndex); //SW MD5 Folder
				m_Clog.WriteLog(_T("SW MD5 Folder: ") + strDataIOFolder);

				strTemp = strTemp.Mid(nDotIndex+1);
				nDotIndex = strTemp.Find(_T(","));
				strFactoryToolMD5Folder = strTemp.Left(nDotIndex); //Factory Tool MD5 Folder
				m_Clog.WriteLog(_T("Factory Tool MD5 Folder: ") + strDataIOFolder);

				strTemp = strTemp.Mid(nDotIndex+1);
				nDotIndex = strTemp.Find(_T(","));
				strDataIOMD5Folder = strTemp; //DataIO MD5 Folder
				m_Clog.WriteLog(_T("DataIO MD5 Folder: ") + strDataIOFolder);
				break;
			}
		}
		MapFile.Close();
	}
	else
	{
		m_Clog.WriteLog(_T("Open Map file fail!"));
		CString str_ErrCode = _T("ET1051");
		CString str_ErrMsg = _T("打开Map 文件失败");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}
	if(str_station.MakeUpper() == _T("TRIGGER") || str_station.MakeUpper() == _T("DL")||str_station.MakeUpper()==_T("RE_DL")||str_station.MakeUpper() == _T("OSDL"))
	{   
		m_Clog.WriteLog(_T("进入TRIGGER或者DL/OSDL或者RE_DL，检查factoryImage的内容"),sz_ErrCode,sz_ErrMsg);

		m_str_ImagePath_Factory_LOC = _T("C:\\Image\\");
		m_str_ImagePath_Factory_LOC = m_str_ImagePath_Factory_LOC + m_str_Model + _T("\\") + m_str_FactoryImage;
		m_str_ImagePath = m_str_ImagePath_Factory_LOC;

		b_res = FcheckImageDirectory(m_sz_facImageVer_SO_pre2, m_str_ImagePath_Factory_LOC, m_bfactoryImagestate,sz_ErrCode, sz_ErrMsg);

		CString str_server_path = _T("");
		m_str_MD5Path_Factory_LOC = _T("C:\\Image\\");// Loc MD5 directory
		m_str_MD5Path_Factory_LOC = m_str_MD5Path_Factory_LOC + m_str_Model + _T("\\") + _T("MD5");

		TCHAR sz_buffer[128];
		memset(sz_buffer, 0, sizeof(sz_buffer));

		if(!bNewFolderStructure)
		{
			if(m_bfactoryImagestate)
			{
				::GetPrivateProfileString(_T("SERVER_INFO"), _T("NORMAL_PATH"), _T(""), sz_buffer, sizeof(sz_buffer), INIFILEPASH);
			}
			else
			{
				::GetPrivateProfileString(_T("SERVER_INFO"), _T("CR_PATH"), _T(""), sz_buffer, sizeof(sz_buffer), INIFILEPASH);
			}

			str_server_path.Format(_T("%s"),sz_buffer);// Server MD5 directory
			str_server_path = str_server_path + m_str_ModelProjectName + _T("\\MD5\\Factory_SW_Load\\");
			m_Clog.WriteLog(_T("获取tool的serverPath的路径"),sz_ErrCode,sz_ErrMsg);
			m_Clog.WriteLog(str_server_path,sz_ErrCode,sz_ErrMsg);
			m_str_FImageServerPath.Format(_T("%s"),sz_buffer);
			m_str_FImageServerPath = m_str_FImageServerPath + m_str_ModelProjectName+ _T("\\Factory_SW_Load\\");
		}
		else
		{
			if(m_bfactoryImagestate)
			{
				str_server_path = strQCSOfficialURL + _T("\\") + strFactoryImageMD5Folder;
				m_str_FImageServerPath = strQCSOfficialURL + _T("\\") + strFactoryImageFolder;

			}
			else
			{
				str_server_path = strQCSVerifyURL + _T("\\") + strFactoryImageMD5Folder;
				m_str_FImageServerPath = strQCSVerifyURL + _T("\\") + strFactoryImageFolder;

			}
			m_Clog.WriteLog(_T("Factory Image MD5 Folder: ")+str_server_path);
			m_Clog.WriteLog(_T("Factory Image Folder: ") + m_str_FImageServerPath);
		}



		if(!b_res && m_bImageCopy)
		{
			// Local directory is not exist
			m_Clog.WriteLog(_T("Local directory is not exist"));
			m_Clog.WriteLog(_T("DeleteDirectory C:\\Image\\"));
			b_res = DeleteDirectory(_T("C:\\Image\\"), sz_ErrCode, sz_ErrMsg);
			if(!b_res)
			{
				return false;
			}
			m_Clog.WriteLog(_T("ImageCopyCheck"));
			b_res = ImageCopyCheck(str_ProjectName.GetBuffer(), m_sz_facImageVer_SO_pre2, str_replace, m_bfactoryImagestate, true, sz_ErrCode, sz_ErrMsg);
			str_ProjectName.ReleaseBuffer();
			if(!b_res)
			{
				return false;
			}
			m_Clog.WriteLog(_T("UnzipImage"));
			b_res = UnzipImage(m_str_ImagePath, m_sz_facImageVer_SO_pre2 , sz_ErrCode, sz_ErrMsg);
			if(!b_res)
			{
				return false;
			}
		}
		else
		{
			// Local directory exist,do MD5 check
			m_Clog.WriteLog(_T(" Local directory exist,do MD5 check"));
			if(m_bMD5check)
			{
				m_Clog.WriteLog(_T("MD5_CopyCheck"));
				b_res = MD5_CopyCheck(str_server_path, m_str_MD5Path_Factory_LOC,sz_ErrCode, sz_ErrMsg);
				if(b_res == false)
				{
					return false;
				}
				CString str_wild = _T("");
				str_wild = _T("*") + m_str_FactoryImage + _T("*");

				m_Clog.WriteLog(_T("进入factoryimage的FindInformation查找内容为"),sz_ErrCode,sz_ErrMsg);
				m_Clog.WriteLog(m_str_MD5Path_Factory_LOC,sz_ErrCode,sz_ErrMsg);

				int i_checknumber = FindInformation(m_str_MD5Path_Factory_LOC,m_str_HashFilepath_Loc,str_wild);

				m_Clog.WriteLog(m_str_HashFilepath_Loc,sz_ErrCode,sz_ErrMsg);

				//ytt: do not understand .....need review 
				if(i_checknumber == 1)
				{
					m_Clog.WriteLog(_T("i_checknumber =1 => Do_MD5_Check" ));
					b_res = m_obj_MD5check.Do_MD5_Check(m_str_ImagePath_Factory_LOC, m_str_HashFilepath_Loc,str_replace, sz_ErrCode, sz_ErrMsg);
					if(b_res)
					{
						m_Clog.WriteLog(_T("check MD5 ok => do ImageDirectoryDelete "));
						b_res = ImageDirectoryDelete(m_str_ImagePath, m_sz_facImageVer_SO_pre2 , sz_ErrCode, sz_ErrMsg);
						return b_res;
					}
				}
				m_Clog.WriteLog(_T("DeleteDirectory C:\\Image\\"));
				b_res = DeleteDirectory(_T("C:\\Image\\"), sz_ErrCode, sz_ErrMsg);
				if(!b_res)
				{
					return false;
				}
				m_Clog.WriteLog(_T("ImageCopyCheck"));
				b_res = ImageCopyCheck(str_ProjectName.GetBuffer(), m_sz_facImageVer_SO_pre2, str_replace, m_bfactoryImagestate, true, sz_ErrCode, sz_ErrMsg);
				str_ProjectName.ReleaseBuffer();
				if(!b_res)
				{
					return false;
				}
				m_Clog.WriteLog(_T("UnzipImage"));
				b_res = UnzipImage(m_str_ImagePath, m_sz_facImageVer_SO_pre2 , sz_ErrCode, sz_ErrMsg);
				if(!b_res)
				{
					return false;
				}
			}
		}
		m_Clog.WriteLog(_T("MD5_CopyCheck for all image files"));
		bool b_check = MD5_CopyCheck(str_server_path, m_str_MD5Path_Factory_LOC,sz_ErrCode, sz_ErrMsg);
		if(!b_check)
		{
			return false;
		}
		CString str_wild = _T("");
		str_wild = _T("*") + m_str_FactoryImage;
		str_wild = str_wild + _T("*");
		int i_checknumber = FindInformation(m_str_MD5Path_Factory_LOC,m_str_HashFilepath_Loc,str_wild);
		if(i_checknumber == 1)
		{
			if(m_bMD5check)
			{   
				m_Clog.WriteLog(_T("进入factoryimage的Do_MD5_Check进行check"),sz_ErrCode,sz_ErrMsg);
				m_Clog.WriteLog(_T("check的本地文件和哈斯文件分别为"),sz_ErrCode,sz_ErrMsg);
				m_Clog.WriteLog(m_str_ImagePath_Factory_LOC,sz_ErrCode,sz_ErrMsg);
				m_Clog.WriteLog(m_str_HashFilepath_Loc,sz_ErrCode,sz_ErrMsg);

				b_res = m_obj_MD5check.Do_MD5_Check(m_str_ImagePath_Factory_LOC, m_str_HashFilepath_Loc,str_replace, sz_ErrCode, sz_ErrMsg);
				if(b_res == false)
				{
					m_Clog.WriteLog(_T("Do_MD5_Check fail =>return"));
					return false;
				}
				else
				{
					m_Clog.WriteLog(_T("Do_MD5_Check pass =>return"));
					return true;
				}
			}
			else
			{
				return true;
			}
		}
		else
		{   
			m_Clog.WriteLog(_T("没有找到相互对应的MD5文件，请检查server路径的MD5文件是否正确"),sz_ErrCode,sz_ErrMsg);
			CString str_ErrCode = _T("ET0023");
			CString str_ErrMsg = _T("Can not find HashFile\n");
			str_ErrMsg = str_ErrMsg + _T("找不到HashFile");
			strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
			strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			str_ErrCode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			return false;
		}	
		//ytt after code will never execte???????  need review
		m_Clog.WriteLog(_T("YTT this code never execute???"));
		if(b_res)
		{
			return true;
		}
		else
		{   
			m_Clog.WriteLog(_T("MD5检查失败，请检查MD5或者IMAGE的server路径是否正确"),sz_ErrCode,sz_ErrMsg);
			CString str_ErrCode = _T("ET0024");
			CString str_ErrMsg = _T("MD5 check fail\nMD5检查失败");
			strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
			strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			str_ErrCode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			return false;
		}
	}
	else
	{
		/////User Image
		m_Clog.WriteLog(_T("进入userImage进行"),sz_ErrCode,sz_ErrMsg);
		//AfxMessageBox(_T("User Image 开始"), MB_OK);
		CString str_segment = _T("");
		m_str_ImagePath_User_LOC = _T("C:\\Image\\");
		m_str_ImagePath_User_LOC = m_str_ImagePath_User_LOC + m_str_Model + _T("\\") + m_str_UserImage;
		m_str_ImagePath = m_str_ImagePath_User_LOC;

		bool b_res = Query_StationInfo(m_sz_line,m_sz_process,m_sz_segment,sz_ErrCode,sz_ErrMsg);
		str_segment.Format(_T("%s"),m_sz_segment);
		if(!b_res)
		{
			m_Clog.WriteLog(_T("Query_StationInfo fail =>return"));
			return false;
		}

		if(str_segment.MakeUpper() == _T("PACKAGE") || str_station.MakeUpper() == _T("SD_COPY") || str_station.MakeUpper() == _T("FCHECK"))
		{   

			m_Clog.WriteLog(_T("当前为PACKAGE或则SD_COPY或者FCHECK"),sz_ErrCode,sz_ErrMsg);

			CString str_Server = _T("");
			CString str_Server_MD5 = _T("");
			CString str_Loc = _T("");
			CString str_Loc_MD5 = _T("");
			/*m_DBO.pVMS_ConnectDB();
			bool b_DBresult = m_DBO.pVMS_QuerySQNinfo_V2(m_sz_Model, m_sz_PN, sz_userImageVer, st_PNinfo, sz_ErrCode, sz_ErrMsg);
			m_DBO.pVMS_CloseDB();
			if(!b_DBresult)
			{
			return false;
			}

			if(st_PNinfo.SQN_status == 1)
			{
			b_state = false;
			}
			else
			{
			b_state = true;
			}*/

			if(str_station.MakeUpper() == _T("SD_COPY"))
			{
				b_res = DO_SD_Copy(str_ProjectName,m_buserImagestate, sz_ErrCode, sz_ErrMsg);
				if(!b_res)
					return false;
			}

			if (str_station.MakeUpper() != _T("OS_DL") && str_station.MakeUpper() != _T("FCHECK"))
			{
				return true;
			}

			if(str_station.MakeUpper() == _T("FCHECK"))
			{
				if(!m_bWin7)
				{
					return true;
				}
			}

			b_res = true;
			CString str_locUserImagePath = _T("");
			CString str_server_path = _T("");
			CString str_Hashpath = _T("");

			m_str_MD5DirectoryPath_User_LOC = _T("C:\\Image\\");//Local MD5 directory
			m_str_MD5DirectoryPath_User_LOC = m_str_MD5DirectoryPath_User_LOC + m_str_Model + _T("\\") + _T("MD5");


			b_res = FcheckImageDirectory(m_sz_userImageVer_pre2, m_str_ImagePath_User_LOC, m_buserImagestate,sz_ErrCode, sz_ErrMsg);
			CString str_path = _T("C:\\Image\\");//Local Image directory
			str_path = str_path + m_str_Model + "\\" + m_str_UserImage;


			TCHAR sz_buffer[128];
			memset(sz_buffer, 0, sizeof(sz_buffer));
		if(!bNewFolderStructure)
		{
			if(m_buserImagestate)
			{
				::GetPrivateProfileString(_T("SERVER_INFO"), _T("NORMAL_PATH"), _T(""), sz_buffer, sizeof(sz_buffer), INIFILEPASH);
			}
			else
			{
				::GetPrivateProfileString(_T("SERVER_INFO"), _T("CR_PATH"), _T(""), sz_buffer, sizeof(sz_buffer), INIFILEPASH);
			}

			str_server_path.Format(_T("%s"),sz_buffer);// Server MD5 directory
			str_server_path = str_server_path + m_str_ModelProjectName + _T("\\MD5\\SW_Load\\");
			m_Clog.WriteLog(_T("获取tool的serverPath的路径"),sz_ErrCode,sz_ErrMsg);
			m_Clog.WriteLog(str_server_path,sz_ErrCode,sz_ErrMsg);
			m_str_FImageServerPath.Format(_T("%s"),sz_buffer);
			m_str_FImageServerPath = m_str_FImageServerPath + m_str_ModelProjectName+ _T("\\SW_Load\\");
		}
		else
		{
			if(m_buserImagestate)
			{
				str_server_path = strQCSOfficialURL + _T("\\") + strSWMD5Folder;
				m_str_FImageServerPath = strQCSOfficialURL + _T("\\") + strSWImageFolder;

			}
			else
			{
				str_server_path = strQCSVerifyURL + _T("\\") + strSWMD5Folder;
				m_str_FImageServerPath = strQCSVerifyURL + _T("\\") + strSWImageFolder;

			}
			m_Clog.WriteLog(_T("Factory Image MD5 Folder: ")+str_server_path);
			m_Clog.WriteLog(_T("Factory Image Folder: ") + m_str_FImageServerPath);
		}
			//if(m_buserImagestate)
			//{
			//	::GetPrivateProfileString(_T("SERVER_INFO"), _T("NORMAL_PATH"), _T(""), sz_buffer, sizeof(sz_buffer), INIFILEPASH);
			//}
			//else
			//{
			//	::GetPrivateProfileString(_T("SERVER_INFO"), _T("CR_PATH"), _T(""), sz_buffer, sizeof(sz_buffer), INIFILEPASH);
			//}
			//str_server_path.Format(_T("%s"),sz_buffer);
			//if(str_server_path.IsEmpty())
			//{
			//	CString str_ErrCode = _T("ET0056");
			//	CString str_ErrMsg = _T("The servel path in config file is empty\n配置文件中的服务器路径为空");
			//	strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
			//	strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			//	str_ErrCode.ReleaseBuffer();
			//	str_ErrMsg.ReleaseBuffer();
			//	return false;
			//}
			//str_server_path = str_server_path + m_str_ModelProjectName + _T("\\") + _T("MD5") + _T("\\");
			//str_server_path = str_server_path + _T("SW_Load") + _T("\\");

			if(!b_res && m_bImageCopy)
			{
				// Local directory is not exist
				b_res = DeleteDirectory(_T("C:\\Image\\"), sz_ErrCode, sz_ErrMsg);
				if(!b_res)
				{
					return false;
				}
				b_res = ImageCopyCheck(str_ProjectName.GetBuffer(), m_sz_userImageVer_pre2, str_replace, m_buserImagestate, false,sz_ErrCode, sz_ErrMsg);
				str_ProjectName.ReleaseBuffer();
				if(!b_res)
				{
					return false;
				}
				b_res = UnzipImage(m_str_ImagePath, m_sz_userImageVer_pre2 , sz_ErrCode, sz_ErrMsg);
				if(!b_res)
				{
					return false;
				}
			}
			else
			{
				// Local directory exist,do MD5 check
				m_Clog.WriteLog(_T(" Local directory exist,do MD5 check"));
				if(m_bMD5check)
				{
					b_res = MD5_CopyCheck(str_server_path, m_str_MD5DirectoryPath_User_LOC,sz_ErrCode, sz_ErrMsg);
					if(!b_res)
					{
						return false;
					}
					CString str_wild = _T("");
					str_wild = _T("*") + m_str_UserImage + _T("*");



					int i_checknumber = FindInformation(m_str_MD5DirectoryPath_User_LOC,m_str_MD5File_User_LOC,str_wild);
					if(i_checknumber == 1)
					{
						b_res = m_obj_MD5check.Do_MD5_Check(m_str_ImagePath_User_LOC, m_str_MD5File_User_LOC,str_replace, sz_ErrCode, sz_ErrMsg);
						if(b_res)
						{
							b_res = ImageDirectoryDelete(m_str_ImagePath, m_sz_userImageVer_pre2, sz_ErrCode, sz_ErrMsg);
							return b_res;
						}
					}
				}
				b_res = DeleteDirectory(_T("C:\\Image\\"), sz_ErrCode, sz_ErrMsg);
				if(!b_res)
				{
					return false;
				}
				b_res = ImageCopyCheck(str_ProjectName.GetBuffer(), m_sz_userImageVer_pre2, str_replace, m_buserImagestate, false,sz_ErrCode, sz_ErrMsg);
				str_ProjectName.ReleaseBuffer();
				if(!b_res)
				{
					return false;
				}
				b_res = UnzipImage(m_str_ImagePath, m_sz_userImageVer_pre2 , sz_ErrCode, sz_ErrMsg);
				if(!b_res)
				{
					return false;
				}
			}
			bool b_check = MD5_CopyCheck(str_server_path, m_str_MD5DirectoryPath_User_LOC,sz_ErrCode, sz_ErrMsg);
			if(!b_check)
			{
				return false;
			}

			CString str_wild = _T("");
			str_wild = _T("*") + m_str_UserImage + _T("*");

			m_Clog.WriteLog(_T("进入USERImage的FindInformation查找内容为"),sz_ErrCode,sz_ErrMsg);
			m_Clog.WriteLog(m_str_MD5DirectoryPath_User_LOC,sz_ErrCode,sz_ErrMsg);

			int i_checknumber = FindInformation(m_str_MD5DirectoryPath_User_LOC,m_str_MD5File_User_LOC,str_wild);
			if(i_checknumber == 1)
			{
				if(m_bMD5check)
				{   
					m_Clog.WriteLog(_T("进入USER的Do_MD5_Check进行check"),sz_ErrCode,sz_ErrMsg);
					m_Clog.WriteLog(_T("check的本地文件和哈斯文件分别为"),sz_ErrCode,sz_ErrMsg);
					m_Clog.WriteLog(m_str_ImagePath_User_LOC,sz_ErrCode,sz_ErrMsg);
					m_Clog.WriteLog(m_str_MD5File_User_LOC,sz_ErrCode,sz_ErrMsg);

					b_res = m_obj_MD5check.Do_MD5_Check(m_str_ImagePath_User_LOC, m_str_MD5File_User_LOC,str_replace, sz_ErrCode, sz_ErrMsg);
					if(b_res)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					b_res = true;
				}
			}
			else
			{   
				m_Clog.WriteLog(_T("没有找到相互对应的MD5文件，请检查server路径的MD5文件是否正确"),sz_ErrCode,sz_ErrMsg);
				CString str_ErrCode = _T("ET0023");
				CString str_ErrMsg = _T("找不到HashFile");
				strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
				strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
				str_ErrCode.ReleaseBuffer();
				str_ErrMsg.ReleaseBuffer();
				return false;
			}
		}
		return b_res;
	}
	return true;
}

bool Funccheck::CheckCommonTool(CString str_Process, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	bool b_res = true;
	if (str_Process.MakeUpper() == _T("P_VI"))
	{
		str_Process = _T("VI");
	}
	//if(str_Process.MakeUpper() == "FALINK" || str_Process.MakeUpper() == "VI" || str_Process.MakeUpper() == "FQC" || str_Process.MakeUpper() == "RUN_IN" || str_Process.MakeUpper() == "OOB" || str_Process.MakeUpper() == "PPIDLINK"||str_Process.MakeUpper() == "SHIPPINGOS")
	if(strncmp("FALINK",m_sz_process,6)==0||strncmp("PPIDLINK",m_sz_process,8)==0|| str_Process.MakeUpper() == "VI" || str_Process.MakeUpper() == "FQC" || str_Process.MakeUpper() == "RUN_IN" || str_Process.MakeUpper() == "OOB" || str_Process.MakeUpper() == "SHIPPINGOS")
	{   
		b_res = true;
		return b_res;
	}
	else
	{
		b_res = false;
		return b_res;
	}
}

bool Funccheck::GetCommonToolPath(CString str_Process, CString& str_path, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	CString str_log = _T("");
	str_log = _T("函数GetCommonToolPath开始执行");
	m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);

	bool b_res = true;
	//if(str_Process.MakeUpper() == "FALINK")
	if(strncmp("FALINK",m_sz_process,6)==0)
	{
		str_path = "KeypartsLink";
	}	
	else if(str_Process.MakeUpper() == "VI"||str_Process.MakeUpper() == "SHIPPINGOS"||str_Process.MakeUpper() == "P_VI")
	{
		str_path = "VI";
	}
	else if(str_Process.MakeUpper() == "FQC")
	{
		str_path = "QA";
	}
	else if (str_Process.MakeUpper() == "RUN_IN")
	{
		str_path = "RunIn";
	}
	else if(str_Process.MakeUpper() == "OOB")
	{
		str_path = "QA";
	}
	//else if(str_Process.MakeUpper() == "PPIDLINK")
	else if(strncmp("PPIDLINK",m_sz_process,8)==0)
	{
		str_path = "KeypartsLink";
	}
	else
	{
		CString str_ErrCode = _T("ET0051");
		CString str_ErrMsg = _T("");
		str_ErrMsg.Format(_T("Common Tool is not exist\n不存在Common工具%s"),str_Process);
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}
	return b_res;
}

bool Funccheck::ExecuteCommonTool(CString str_path_Server, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	CString str_log = _T("");
	str_log = _T("函数ExecuteCommonTool开始执行");
	m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);

	bool b_res = true;
	if(str_path_Server.IsEmpty())
	{
		CString str_ErrCode = _T("ET0051");
		CString str_ErrMsg = _T("Common Tool servel path is empty\n Common工具的服务器路径为空");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}

	/*==================================================================================================================
	// Config Tool Path
	==================================================================================================================*/
	CString str_path_loc = _T("C:\\Tool\\");// Loc Tool Path
	str_path_loc = str_path_loc + m_str_process;
	str_path_loc = str_path_loc + _T("\\");
	

	if(str_path_Server.Right(1) != '\\')
	{
		str_path_Server = str_path_Server + _T("\\");
	}

	CString str_common_Path = _T("");
	b_res = GetCommonToolPath(m_str_process, str_common_Path, sz_ErrCode, sz_ErrMsg);
	if(!b_res)
	{
		return false;
	}
	str_path_Server = str_path_Server + str_common_Path;
	str_path_Server = str_path_Server + _T("\\");
	str_path_Server = str_path_Server + _T("*.*");
	int n_TooldirectoryCheck = 0;
	CString str_temp = _T("");

	//find tool folder by Tool Version and assign ToolVersion to path
	n_TooldirectoryCheck = FindTooldirectory(str_path_Server,str_temp, m_str_Toolver);
	if(n_TooldirectoryCheck != 1)
	{
		CString str_ErrCode = _T("ET0007");
		CString str_ErrMsg = str_path_Server + _T("can not be found\n");
		str_ErrMsg = str_ErrMsg + str_path_Server + _T("找不到工具文件夹");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}

	str_path_Server = str_temp;

	/*==================================================================================================================
	// Config MD5 Path
	==================================================================================================================*/
	CString str_Toolpath_MD5 = _T("");
	CString str_Serever_path_MD5 = _T("");
	CString str_Hashpath = _T("");
	CString str_LOC_Path = _T("C:\\TOOL\\");
	str_LOC_Path = str_LOC_Path + _T("MD5");

	str_Serever_path_MD5 = str_path_Server;

	if(str_Serever_path_MD5.Right(1) == '\\')
	{
		str_Serever_path_MD5 = str_Serever_path_MD5.Left(str_Serever_path_MD5.GetLength() - 1);
	}
	
	str_Serever_path_MD5 = str_Serever_path_MD5.Left(str_Serever_path_MD5.ReverseFind('\\'));
	str_Serever_path_MD5 = str_Serever_path_MD5 + _T("\\");
	str_Serever_path_MD5 = str_Serever_path_MD5 + _T("MD5");
    
	m_Clog.WriteLog(_T("Common Tool COPY 开始，服务器地址和本地地址为："),sz_ErrCode,sz_ErrMsg);
	m_Clog.WriteLog(str_path_Server,sz_ErrCode,sz_ErrMsg);
	m_Clog.WriteLog(str_path_loc,sz_ErrCode,sz_ErrMsg);

	//YTT: need check the code ???? ex:str_path_loc:C:\Tool\GA3I12A_T3\   but i think we need to check C:\Tool\GA3I12A_T3\TLFT_GA3I12A_v1.005
	b_res = m_obj_DirectoryDispose.IsDirectoryExist(str_path_loc);
	if(!b_res)
	{
		//Tool did not exist
		b_res = ToolCopy(str_path_Server, str_path_loc, sz_ErrCode, sz_ErrMsg);
		if(!b_res)
		{
			return false;
		}
	}
	else
	{
		//Tool exist, Do MD5 check
		if(m_bMD5check_tool_Pre)
		{
			b_res = MD5_CopyCheck(str_Serever_path_MD5, str_LOC_Path,sz_ErrCode, sz_ErrMsg);
			if(!b_res)
			{
				return false;
			}

			CString str_wild = _T("");
			str_wild = _T("*") + m_str_Toolver + _T("*");
            
			//int i_checknumber = FindInformation(str_LOC_Path,str_Hashpath,str_wild);

            int i_checknumber = FindInformationToolMD5(str_LOC_Path,str_Hashpath,str_wild);

			CString str_replace = _T("");
			str_replace = m_str_Toolver;
			if(i_checknumber == 1)
			{
				if(m_bMD5check_tool)
				{
					b_res = m_obj_MD5check.Do_MD5_Check(str_path_loc, str_Hashpath,str_replace, sz_ErrCode, sz_ErrMsg);
					if(b_res)
					{   
						m_Clog.WriteLog(_T("MD5 CHECK OK"),sz_ErrCode,sz_ErrMsg);
						return true;
					}
				}
				else
				{
					return true;
				}
			}
		}
		b_res = ToolCopy(str_path_Server, str_path_loc, sz_ErrCode, sz_ErrMsg);
	}

	bool b_check = false;
	CString str_wild = _T("");
	int i_checknumber = 0;
	CString str_replace = _T("");
	if(m_bMD5check_tool)
	{
		b_check = MD5_CopyCheck(str_Serever_path_MD5, str_LOC_Path,sz_ErrCode, sz_ErrMsg);
		if(!b_check)
		{
			return false;
		}
		str_wild = _T("*") + m_str_Toolver + _T("*");
		//i_checknumber = FindInformation(str_LOC_Path,str_Hashpath,str_wild);

        int i_checknumber = FindInformationToolMD5(str_LOC_Path,str_Hashpath,str_wild);

		if (i_checknumber==2)
		{
			int i_checknumber = FindTooldirectory(str_LOC_Path,str_Hashpath,str_wild);
		}

		str_replace = m_str_Toolver;
		if(i_checknumber == 1)
		{
			str_path_loc = str_path_loc + _T("\\");
			if(m_bMD5check_tool)
			{
				b_res = m_obj_MD5check.Do_MD5_Check(str_path_loc, str_Hashpath,str_replace, sz_ErrCode, sz_ErrMsg);
				if(b_res == false)
				{
					return false;
				}
			}
			else
			{
				return true;
			}
		}
		else
		{
			CString str_ErrCode = _T("ET0008");
			CString str_ErrMsg = _T("Can not find MD5 file in ") + str_LOC_Path;
			str_ErrMsg = str_ErrMsg + _T("\n在 ") + str_LOC_Path + _T(" 找不到MD5文件");
			strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
			strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			str_ErrCode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			return false;
		}
	}

	return b_res;
}

bool Funccheck::PrCheck3(CString str_ProjectName, TCHAR* sz_SO, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	bool b_res = true;
	CString str_Server = _T("");
	bool bNewFolderStructure = false;


	memset(sz_ErrCode,0,ERRORNUMBER);
	memset(sz_ErrMsg,0,ERRORNUMBER);

	TCHAR sz_ServerPath[128];
	memset(sz_ServerPath, 0, sizeof(sz_ServerPath));
	TCHAR sz_MapPath[128] ={0};

	CString strProject = _T("");
	CString strImageType = _T("");
	CString strVariant = _T("");
	CString strModel = _T("");
	CString strQTTOfficialURL = _T("");
	CString strQTTVerifyURL = _T("");
	CString strQCSOfficialURL = _T("");
	CString strQCSVerifyURL = _T("");
	CString strFactoryImageFolder = _T("");
	CString strFactoryToolFolder = _T("");
	CString strSWImageFolder = _T("");
	CString strServiceToolFolder = _T("");
	CString strDataIOFolder = _T("");
	CString strFactoryImageMD5Folder = _T("");
	CString strSWMD5Folder = _T("");
	CString strFactoryToolMD5Folder = _T("");
	CString strDataIOMD5Folder = _T("");

    if(m_str_UserImage.IsEmpty())
	{
		if(m_str_Toolver.IsEmpty())
		{
			return true;
		}
	}

	if(m_str_Toolver.IsEmpty())
	{
		CString str_ErrCode = _T("ET0051");
		CString str_ErrMsg = _T("工具的版本信息为空");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}


	if(m_str_Model.IsEmpty())
	{
		CString str_ErrCode = _T("ET0051");
		CString str_ErrMsg = _T("Tool model is empty\n,工具的Model信息为空");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}

    if(!m_bcommonTool)
	{
		::GetPrivateProfileString(_T("SERVER_INFO"), _T("MAP_PATH"), _T("\\\\10.85.58.13\\Official\\01.Project_related\\01_ProjectModelPathTable\\"), sz_MapPath, sizeof(sz_MapPath), INIFILEPASH);

		CString strMapPath = sz_MapPath;
		strMapPath += _T("ProjectModelPathTable.csv");

		m_Clog.WriteLog(_T("Map Path: ") + strMapPath);
		CStdioFile MapFile;
		if(MapFile.Open(strMapPath.GetBuffer(), CFile::modeRead))
		{
			m_Clog.WriteLog(_T("Open Map file success!"));
			CString strMapContent;
			while(MapFile.ReadString(strMapContent))
			{
				if(strMapContent.Find(_T("QTT_Offical_URL")) != -1) // first line
					continue;

				int nIndex = strMapContent.Find(this->m_str_Model) ;
				if(nIndex == -1 ) //not this model
					continue;
				else // find this model
				{
					bNewFolderStructure = true;
					int nDotIndex = strMapContent.Find(_T(","));
					strProject = strMapContent.Left(nDotIndex); //Project
					m_Clog.WriteLog(_T("Project : ") + strProject);

					CString strTemp = strMapContent.Mid(nDotIndex + 1);
					nDotIndex = strTemp.Find(_T(","));
					strImageType = strTemp.Left(nDotIndex); //ImageType;
					m_Clog.WriteLog(_T("ImageType: ") + strImageType);

					strTemp = strTemp.Mid(nDotIndex+1);
					nDotIndex = strTemp.Find(_T(","));
					strVariant = strTemp.Left(nDotIndex);//Variant
					m_Clog.WriteLog(_T("Variant: ") + strVariant);

					strTemp = strTemp.Mid(nDotIndex+1);
					nDotIndex = strTemp.Find(_T(","));
					strModel = strTemp.Left(nDotIndex); //Model
					m_Clog.WriteLog(_T("Model: ") + strModel);

					strTemp = strTemp.Mid(nDotIndex+1);
					nDotIndex = strTemp.Find(_T(","));
					strQTTOfficialURL = strTemp.Left(nDotIndex); //QTT Officail URL
					m_Clog.WriteLog(_T("QTT Official URL: ") + strQTTOfficialURL);

					strTemp = strTemp.Mid(nDotIndex+1);
					nDotIndex = strTemp.Find(_T(","));
					strQTTVerifyURL = strTemp.Left(nDotIndex); //QTT Verify URL
					m_Clog.WriteLog(_T("QTT Verify URL: ") + strQTTVerifyURL);

					strTemp = strTemp.Mid(nDotIndex+1);
					nDotIndex = strTemp.Find(_T(","));
					strQCSOfficialURL = strTemp.Left(nDotIndex); //QCS Official URL
					m_Clog.WriteLog(_T("QCS Official URL: ") + strQCSOfficialURL);

					strTemp = strTemp.Mid(nDotIndex+1);
					nDotIndex = strTemp.Find(_T(","));
					strQCSVerifyURL = strTemp.Left(nDotIndex); //QCS Verify URL
					m_Clog.WriteLog(_T("QCS Verify URL: ") + strQCSVerifyURL);

					strTemp = strTemp.Mid(nDotIndex+1);
					nDotIndex = strTemp.Find(_T(","));
					strFactoryImageFolder = strTemp.Left(nDotIndex); //Factory Image folder
					m_Clog.WriteLog(_T("Factory Image Folder: ") + strFactoryImageFolder);

					strTemp = strTemp.Mid(nDotIndex+1);
					nDotIndex = strTemp.Find(_T(","));
					strSWImageFolder = strTemp.Left(nDotIndex); //SW Load Folder
					m_Clog.WriteLog(_T("SW Load Folder: ") + strSWImageFolder);

					strTemp = strTemp.Mid(nDotIndex+1);
					nDotIndex = strTemp.Find(_T(","));
					strFactoryToolFolder = strTemp.Left(nDotIndex); //Factory Tool Folder
					m_Clog.WriteLog(_T("Factory Tool Folder: ") + strFactoryToolFolder);

					strTemp = strTemp.Mid(nDotIndex+1);
					nDotIndex = strTemp.Find(_T(","));
					strServiceToolFolder = strTemp.Left(nDotIndex); //Service Tool Folder
					m_Clog.WriteLog(_T("Service Tool Folder : ") + strServiceToolFolder);

					strTemp = strTemp.Mid(nDotIndex+1);
					nDotIndex = strTemp.Find(_T(","));
					strDataIOFolder = strTemp.Left(nDotIndex); //DataIO Folder
					m_Clog.WriteLog(_T("DataIO Folder: ") + strDataIOFolder);

					strTemp = strTemp.Mid(nDotIndex+1);
					nDotIndex = strTemp.Find(_T(","));
					strFactoryImageMD5Folder = strTemp.Left(nDotIndex); //FactoryImage MD5 Folder
					m_Clog.WriteLog(_T("Factory Image MD5 Folder: ") + strDataIOFolder);

					strTemp = strTemp.Mid(nDotIndex+1);
					nDotIndex = strTemp.Find(_T(","));
					strSWMD5Folder = strTemp.Left(nDotIndex); //SW MD5 Folder
					m_Clog.WriteLog(_T("SW MD5 Folder: ") + strDataIOFolder);

					strTemp = strTemp.Mid(nDotIndex+1);
					nDotIndex = strTemp.Find(_T(","));
					strFactoryToolMD5Folder = strTemp.Left(nDotIndex); //Factory Tool MD5 Folder
					m_Clog.WriteLog(_T("Factory Tool MD5 Folder: ") + strDataIOFolder);

					strTemp = strTemp.Mid(nDotIndex+1);
					nDotIndex = strTemp.Find(_T(","));
					strDataIOMD5Folder = strTemp; //DataIO MD5 Folder
					m_Clog.WriteLog(_T("DataIO MD5 Folder: ") + strDataIOFolder);
					break;
				}
			}
			MapFile.Close();
		}
		else
		{
			m_Clog.WriteLog(_T("Open Map file fail!"));
			CString str_ErrCode = _T("ET1051");
			CString str_ErrMsg = _T("打开Map 文件失败");
			strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
			strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			str_ErrCode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			return false;
		}
		if(!bNewFolderStructure)
		{
			if(m_btoolstate)
			{
				::GetPrivateProfileString(_T("SERVER_INFO"), _T("NORMAL_PATH"), _T(""), sz_ServerPath, sizeof(sz_ServerPath), INIFILEPASH);
			}
			else
			{
				::GetPrivateProfileString(_T("SERVER_INFO"), _T("CR_PATH"), _T(""), sz_ServerPath, sizeof(sz_ServerPath), INIFILEPASH);
			}
			m_Clog.WriteLog(_T("获取tool的serverPath的路径"),sz_ErrCode,sz_ErrMsg);
			m_Clog.WriteLog(sz_ServerPath,sz_ErrCode,sz_ErrMsg);
			str_Server.Format(_T("%s"),sz_ServerPath);
		}
	}
	else
	{
		if(m_btoolstate)
		{
			::GetPrivateProfileString(_T("SERVER_INFO"), _T("COMMON_PATH"), _T(""), sz_ServerPath, sizeof(sz_ServerPath), INIFILEPASH);
		}
		else
		{
			::GetPrivateProfileString(_T("SERVER_INFO"), _T("CR_COMMON_PATH"), _T(""), sz_ServerPath, sizeof(sz_ServerPath), INIFILEPASH);
		}
		m_Clog.WriteLog(_T("获取tool的serverPath的路径"),sz_ErrCode,sz_ErrMsg);
		m_Clog.WriteLog(sz_ServerPath,sz_ErrCode,sz_ErrMsg);
		str_Server.Format(_T("%s"),sz_ServerPath);
		b_res = ExecuteCommonTool(str_Server, sz_ErrCode, sz_ErrMsg);
		if (m_str_process.MakeUpper()==_T("OOB"))
	   {   
		   b_res = m_DBO.pVMS_QueryToolStatus(m_sz_model_pre2,m_sz_toolVer_pre2,m_sz_facImageVer_Tool_pre2,m_sz_Status_Tool_pre2,m_sz_Reserve1_pre2,sz_ErrCode,sz_ErrMsg);
           CString str_status_tool=_T("");
           str_status_tool.Format(_T("%s"),m_sz_Status_Tool_pre2);
		   if(str_status_tool==_T("1"))
		   {
			   ::GetPrivateProfileString(_T("SERVER_INFO"), _T("NORMAL_PATH"), _T(""), sz_ServerPath, sizeof(sz_ServerPath), INIFILEPASH);
		   }
		   else
		   {
			   ::GetPrivateProfileString(_T("SERVER_INFO"), _T("CR_PATH"), _T(""), sz_ServerPath, sizeof(sz_ServerPath), INIFILEPASH);
		   }
		   //::GetPrivateProfileString(_T("SERVER_INFO"), _T("CR_PATH"), _T(""), sz_ServerPath, sizeof(sz_ServerPath), INIFILEPASH);
           str_Server.Format(_T("%s"),sz_ServerPath);
		   b_res = ExecuteFactoryResetTool(str_Server, sz_ErrCode, sz_ErrMsg);
	   }
	   return b_res;
	}


/*==================================================================================================================
// Config Tool Path
==================================================================================================================*/
	CString str_path_loc = _T("C:\\Tool\\");// Loc Tool Path
	str_path_loc = str_path_loc + m_str_ModelProjectName;
	str_path_loc = str_path_loc + _T("\\");
	

	CString str_path_Server = _T("");
	if(bNewFolderStructure)
	{
		if(m_btoolstate) //Normal Official
		{
			str_path_Server = strQCSOfficialURL;
		}
		else
		{
			str_path_Server = strQCSVerifyURL;
		}
		if(str_path_Server.Right(1) != '\\')
		{
			str_path_Server = str_path_Server + _T("\\");
		}
		//str_path_Server = str_path_Server + m_str_ModelProjectName;
		//str_path_Server = str_path_Server + _T("\\");

		//str_path_Server = str_path_Server + _T("Factory_Tool");
		str_path_Server += strFactoryToolFolder;
		m_Clog.WriteLog(_T("Tool path1: ") + str_path_Server);
		str_path_Server = str_path_Server + _T("\\");
		str_path_Server = str_path_Server + _T("*.*");
		m_Clog.WriteLog(_T("Tool path2: ") + str_path_Server);	
	}
	else
	{
		str_path_Server = str_Server;
		if(str_path_Server.Right(1) != '\\')
		{
			str_path_Server = str_path_Server + _T("\\");
		}
		str_path_Server = str_path_Server + m_str_ModelProjectName;
		str_path_Server = str_path_Server + _T("\\");

		str_path_Server = str_path_Server + _T("Factory_Tool");
		m_Clog.WriteLog(_T("Tool path1: ") + str_path_Server);
		str_path_Server = str_path_Server + _T("\\");
		str_path_Server = str_path_Server + _T("*.*");
		m_Clog.WriteLog(_T("Tool path2: ") + str_path_Server);		
	}

	int n_TooldirectoryCheck = 0;
	CString str_temp = _T("");

	//AfxMessageBox(str_path_Server, MB_OK);

	n_TooldirectoryCheck = FindTooldirectory(str_path_Server,str_temp, m_str_Toolver);
	if(n_TooldirectoryCheck != 1)
	{   
        m_Clog.WriteLog(str_path_Server,sz_ErrCode,sz_ErrMsg);
        m_Clog.WriteLog(_T("FindToolDirectory的结果错误不为1,查看文件夹是否存在相同的model或者version"),sz_ErrCode,sz_ErrMsg);
		CString str_ErrCode = _T("ET0007");
		CString str_ErrMsg = str_path_Server + _T("找不到Tool文件夹");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}

	str_path_Server = str_temp;

/*==================================================================================================================
// Config MD5 Path
==================================================================================================================*/
	CString str_Toolpath_MD5 = _T("");
	CString str_Serever_path_MD5 = _T("");
	CString str_Hashpath = _T("");
	CString str_LOC_Path = _T("C:\\TOOL\\");
	//str_LOC_Path = str_LOC_Path + m_str_ProjectName + _T("\\") + str_TOOL_version + _T("\\") + _T("MD5");
	str_LOC_Path = str_LOC_Path + m_str_ProjectName + _T("\\") + _T("MD5");

	TCHAR sz_buffer[128];
	memset(sz_buffer, 0, sizeof(sz_buffer));
	//::GetPrivateProfileString(_T("SERVER_INFO"), _T("SERVER_PATH"), _T(""), sz_buffer, sizeof(sz_buffer), INIFILEPASH);
	if(bNewFolderStructure)
	{
		if(m_btoolstate) //Normal Official
		{
			str_Serever_path_MD5 = strQCSOfficialURL;
		}
		else
		{
			str_Serever_path_MD5 = strQCSVerifyURL;
		}
		if(str_Serever_path_MD5.Right(1) != '\\')
		{
			str_Serever_path_MD5 = str_Serever_path_MD5 + _T("\\");
		}

		str_Serever_path_MD5 = str_Serever_path_MD5 + strFactoryToolMD5Folder + _T("\\");
		m_Clog.WriteLog(_T("MD5 Path: ") + str_Serever_path_MD5);
	}
	else
	{
		str_Serever_path_MD5 = str_Server;
		if(str_Serever_path_MD5.Right(1) != '\\')
		{
			str_Serever_path_MD5 = str_Serever_path_MD5 + _T("\\");
		}
		str_Serever_path_MD5 = str_Serever_path_MD5 + m_str_ModelProjectName + _T("\\MD5\\Factory_Tool\\");	
		m_Clog.WriteLog(_T("MD5 Path: ") + str_Serever_path_MD5);
	}
	//if(m_btoolstate)
	//{
	//	::GetPrivateProfileString(_T("SERVER_INFO"), _T("NORMAL_PATH"), _T(""), sz_buffer, sizeof(sz_buffer), INIFILEPASH);
	//}
	//else
	//{
	//	::GetPrivateProfileString(_T("SERVER_INFO"), _T("CR_PATH"), _T(""), sz_buffer, sizeof(sz_buffer), INIFILEPASH);
	//}
	//str_Serever_path_MD5.Format(_T("%s"),sz_buffer);
    
    

	//b_res = DeleteDirectory(_T("C:\\Tool\\"), sz_ErrCode, sz_ErrMsg);



	b_res = m_obj_DirectoryDispose.IsDirectoryExist(str_path_loc);


	m_Clog.WriteLog(_T("Factory Tool COPY 开始，服务器地址和本地地址为："),sz_ErrCode,sz_ErrMsg);
	m_Clog.WriteLog(str_path_Server,sz_ErrCode,sz_ErrMsg);
	m_Clog.WriteLog(str_path_loc,sz_ErrCode,sz_ErrMsg);

	if(!b_res)
	{
		//Tool did not exist
		m_Clog.WriteLog(_T("Factory Tool did not exist"),sz_ErrCode,sz_ErrMsg);
		m_Clog.WriteLog(_T("start tool copy"),sz_ErrCode,sz_ErrMsg);
		b_res = ToolCopy(str_path_Server, str_path_loc, sz_ErrCode, sz_ErrMsg);
		if(!b_res)
		{
			m_Clog.WriteLog(_T("Tool copy fail"),sz_ErrCode,sz_ErrMsg);
			return false;
		}
	}
	else
	{
		//Tool exist, Do MD5 check
		m_Clog.WriteLog(_T("Factory Tool exist"),sz_ErrCode,sz_ErrMsg);
		if(m_bMD5check_tool_Pre)
		{
			b_res = MD5_CopyCheck(str_Serever_path_MD5, str_LOC_Path,sz_ErrCode, sz_ErrMsg);
			if(!b_res)
			{
				m_Clog.WriteLog(_T("MD5_CopyChek fail"),sz_ErrCode,sz_ErrMsg);
				return false;
			}

			CString str_wild = _T("");
			str_wild = _T("*") + m_str_Toolver + _T("*");

			int i_checknumber = FindInformationToolMD5(str_LOC_Path,str_Hashpath,str_wild);

			CString str_replace = _T("");
			str_replace = m_str_Toolver;
			if(i_checknumber == 1)
			{
				if(m_bMD5check_tool)
				{
					b_res = m_obj_MD5check.Do_MD5_Check(str_path_loc, str_Hashpath,str_replace, sz_ErrCode, sz_ErrMsg);
					if(b_res)
					{
						m_Clog.WriteLog(_T("Do_MD5_Check pass"),sz_ErrCode,sz_ErrMsg);
						return true;
					}
					m_Clog.WriteLog(_T("MD5 check fail"),sz_ErrCode,sz_ErrMsg);
					//ytt
					//m_Clog.WriteLog(_T("fail return "),sz_ErrCode,sz_ErrMsg);
					//return false;
					
				}
				else
				{
					return true;
				}
			}
		}
		m_Clog.WriteLog(_T("start Tool copy"),sz_ErrCode,sz_ErrMsg);
		b_res = ToolCopy(str_path_Server, str_path_loc, sz_ErrCode, sz_ErrMsg);
	}

	bool b_check = false;
	CString str_wild = _T("");
	int i_checknumber = 0;
	CString str_replace = _T("");
	if(m_bMD5check_tool)
	{
		b_check = MD5_CopyCheck(str_Serever_path_MD5, str_LOC_Path,sz_ErrCode, sz_ErrMsg);
		if(!b_check)
		{
			return false;
		}
		str_wild = _T("*") + m_str_Toolver + _T("*");
		i_checknumber = FindInformationToolMD5(str_LOC_Path,str_Hashpath,str_wild);
		str_replace = m_str_Toolver;
		if(i_checknumber == 1)
		{
			str_path_loc = str_path_loc + _T("\\");
			//str_path_loc = str_path_loc + str_temp;
			if(m_bMD5check_tool)
			{
				b_res = m_obj_MD5check.Do_MD5_Check(str_path_loc, str_Hashpath,str_replace, sz_ErrCode, sz_ErrMsg);
				if(b_res == false)
				{
					return false;
				}
			}
			else
			{
				return true;
			}
		}
		else
		{   
			CString str_ErrCode = _T("ET0007");
			CString str_ErrMsg = _T("在 ") + str_LOC_Path + _T(" 找不到MD5文件");
			strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
			strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			str_ErrCode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			return false;
		}
	}
	return b_res;
}




CString Funccheck::GetUnify_UI_Version(CString& lpszFileName)
{   
	CString str_Version;   
	TCHAR szSrcfn[MAX_PATH]; // 要获取信息的文件   
	lstrcpy(szSrcfn, lpszFileName);   

	// 读文件信息   
	DWORD dwVerHnd = 0;   
	DWORD dwVerInfoSize = ::GetFileVersionInfoSize(szSrcfn, &dwVerHnd);         //判断容纳版本信息，需要多大缓存区
	if(dwVerInfoSize)   
	{   
   
		HANDLE hMem;   
		LPVOID lpvMem;   
		unsigned int uInfoSize = 0;   
		VS_FIXEDFILEINFO * pFileInfo;   
        /**********指定内存的分配,并转换为指针***************/
		hMem = ::GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);    
		lpvMem = ::GlobalLock(hMem);   
		::GetFileVersionInfo(szSrcfn, dwVerHnd, dwVerInfoSize, lpvMem);   
		::VerQueryValue(lpvMem, (LPTSTR)_T("\\"), (void**)&pFileInfo, &uInfoSize);   

		WORD nVer[4];   
		nVer[0] = HIWORD(pFileInfo->dwProductVersionMS);   
		nVer[1] = LOWORD(pFileInfo->dwProductVersionMS);   
		nVer[2] = HIWORD(pFileInfo->dwProductVersionLS);   
		nVer[3] = LOWORD(pFileInfo->dwProductVersionLS);   
		str_Version.Format(_T("v%d.%d%d"), nVer[1], nVer[2],nVer[3]);   

		::GlobalUnlock(hMem);   
		::GlobalFree(hMem);   
	}   

	return str_Version;   
} 


bool Funccheck::DO_UiMD5Check(TCHAR* sz_ErrCode,TCHAR* sz_ErrMsg,CString& str_batpath,CString& str_checkPath)
{   
	bool b_res=true;
	CString str_CheckFlag=_T("");
	TCHAR sz_path[20];
	::GetPrivateProfileString(_T("Unify_UI_MD5CHECK"), _T("CHECK"), _T(""), sz_path, sizeof(sz_path), INIFILEPASH);
	str_CheckFlag.Format(_T("%s"),sz_path);
	if(!m_bcommonTool&&(str_CheckFlag==_T("1")))
	{
		CString str_Unify_Version=_T("");
		CString str_UiMD5=_T("");
		char m_Unify_Ver[6];
		str_UiMD5=UiMD5_ReturnValue(sz_ErrCode,sz_ErrMsg,str_batpath,str_checkPath);//CHECK Uniffy_ui得到的MD5
		str_Unify_Version=GetUnify_UI_Version(str_checkPath);  //格式为CString类型 v0.x.x
		strcpy(m_Unify_Ver,str_Unify_Version);
		b_res=m_DBO.pVMS_QueryToolStatus("Unify_UI",m_Unify_Ver,m_sz_facImageVer_Tool_pre2,m_sz_Status_Tool_pre2,m_sz_Reserve1_pre2,sz_ErrCode,sz_ErrMsg);
		CString str_log = _T("");
		str_log.Format(_T("fciv.exe check得到Unify_UI.exeMD5为%s,PVCS查询版本得到MD5为%s"),str_UiMD5,m_sz_Reserve1_pre2);
		m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);

		if(b_res==false)
		{
			CString str_Errcode = _T("ET0121");
			CString str_ErrMsg = _T("查询系统得到MD5值FAIL\n");
			str_ErrMsg = str_ErrMsg + _T("MD5系统返回值为空\n");
			strcpy_s(sz_ErrCode,ERRORNUMBER,str_Errcode.GetBuffer());
			strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			str_Errcode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			return false;
		}

		if(strcmp(str_UiMD5,m_sz_Reserve1_pre2)!=0)
		{
			CString str_Errcode = _T("ET0120");
			CString str_ErrMsg = _T("MD5 is not the same\n");
			str_ErrMsg = str_ErrMsg + _T("UI的MD5检查结果不一样");
			strcpy_s(sz_ErrCode,ERRORNUMBER,str_Errcode.GetBuffer());
			strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			str_Errcode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			return false;
		}
	}
	return b_res;
}

CString Funccheck::UiMD5_ReturnValue(TCHAR* sz_ErrCode,TCHAR* sz_ErrMsg,CString& str_batpath,CString& str_checkPath)
{   
	CString str_UIMD5_VALUE=_T("");
	CFileFind obj_fileFind;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb          = sizeof(si);
	si.wShowWindow = SW_HIDE;
	si.dwFlags     = STARTF_USESHOWWINDOW;
    bool b_res=false;
	b_res=CreateDoMd5(sz_ErrCode,sz_ErrMsg,str_batpath,str_checkPath);
	if(!b_res)
	{
		return FALSE;
	}
	if(b_res)
	{
		if(str_batpath == _T(""))
		{
			return false;
		}

		CString str_cmd = str_batpath;

		TCHAR sz_commandLine[1024];
		memset(sz_commandLine, 0, sizeof(sz_commandLine));
		for (int i = 0; i < str_cmd.GetLength(); i ++) 
		{
			sz_commandLine[i] = str_cmd[i];
		}
		b_res = CreateProcess(NULL, sz_commandLine, NULL, NULL, NULL, NULL, NULL, NULL, &si, &pi);
		Sleep(1000);
	}
    
	b_res=m_obj_MD5check.ReadUiXML("C:\\UI.xml",str_UIMD5_VALUE,sz_ErrCode,sz_ErrMsg);

	if(obj_fileFind.FindFile("C:\\UI.xml")&&b_res)
	{
		DeleteFile("C:\\UI.xml");
	}
    
	return str_UIMD5_VALUE;
}

bool Funccheck::CreateDoMd5(TCHAR* sz_ErrCode,TCHAR* sz_ErrMsg,CString& str_batpath,CString& str_checkPath)
{   
	CString str_filename;
	CStdioFile fbat;
	bool b_res=false;
	CFileFind obj_fileFind;
	CFileException fileException;
	TCHAR sz_Path[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, sz_Path);
	str_filename.Format("%s",sz_Path);
	str_filename = str_filename + _T("\\") +_T("DouiMD5.bat");
	//m_str_filename = str_filename;

	if(obj_fileFind.FindFile(str_filename))
	{
		DeleteFile(str_filename);

		if(fbat.Open (str_filename, CFile::typeText|CFile::modeReadWrite|CFile::modeCreate, &fileException))
		{
			CString str_temp = _T("");
			str_temp=_T("fciv.exe -r ")+str_checkPath+_T(" -xml ")+_T("c:\\UI.xml");
			fbat.WriteString(str_temp);
			fbat.Close();
			str_batpath = str_filename;
			b_res = true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if(fbat.Open (str_filename, CFile::typeText|CFile::modeReadWrite|CFile::modeCreate, &fileException))
		{

			CString str_temp = _T("");
			str_temp=_T("fciv.exe -r ")+str_checkPath+_T(" -xml ")+_T("c:\\UI.xml");
			fbat.WriteString(str_temp);
			fbat.Close();
			str_batpath = str_filename;
			b_res = true;
		}
		else
		{
			return false;
		}
	}
	return b_res;
}

/******************************************************************************
* function name:  CreateBat(TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg, CString& str_batpath)                                  
* Parameters   :  BOOL 
* Summary      :  Create Bat file to UnifyTool invoke tool
* Return       :  True  success.False  fail  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Tom Li          2011/07/28           First version                            
*****************************************************************************/
bool Funccheck::CreateBat(TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg, CString& str_batpath, CString str_process, CString str_ProjectName, CString str_AccessoryList)
{
	bool b_res =false;
	if(str_process.IsEmpty())
		return false;

	char sz_model[50] = {0};
	char sz_partNo[50] = {0};
	char sz_soType[50] = {0};
	char sz_toolVer[50] = {0};
	char sz_facImageVer[50] = {0};
	char sz_userImageVer[50] = {0};
	char sz_ETtoolVer[50] = {0};

	CString str_RunToolPath = _T("");
	CString str_SWVersion = _T("");
	CString str_FactoryResetPath = _T("");
	CString str_toolpath = _T("");
	CString str_cmd = _T("");
	CString str_SO = _T("");
	CString str_EmployeeID = _T("");
	CString str_ToolVersion = _T("");

	CFileFind finder;

	str_SO.Format("%s", m_sz_SO);
	str_EmployeeID.Format("%s", m_sz_employId);


	b_res = GetToolName(str_RunToolPath,str_process,sz_ErrCode,sz_ErrMsg);
	if(!b_res)
	{
		CString str_Errcode = _T("ET0013");
		CString str_ErrMsg = _T("配置文档没有") + str_process + _T("工具");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_Errcode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_Errcode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}

	CString str_temp = _T("C:\\Tool\\");
	if(m_bcommonTool)
	{
		str_temp = str_temp + m_str_process;
	}
	else
	{
		str_temp = str_temp + m_str_ModelProjectName;
	}
	str_temp = str_temp + _T("\\");
	str_temp = str_temp + _T("*.*");
	b_res = finder.FindFile(str_temp);
	while(b_res)
	{
		b_res = finder.FindNextFile();//检查文件是否检查完
		if(finder.IsDots())
			continue;

		CString str_filePath = finder.GetFilePath();
		if(finder.IsDirectory())
		{
			str_temp = str_filePath;
			break;
		}
		else
		{
			continue;
		}
	}

	CStdioFile fbat;
	CFileFind obj_fileFind;
	CFileException fileException;
	CString str_filename = _T("");
	TCHAR sz_Path[MAX_PATH];
	memset(sz_Path,0,sizeof(sz_Path));

	if(str_process==_T("RE_DL"))
	{
		CString str_RunTool_Fst = _T("");
		CString str_RunTool_Snd = _T("");
		str_RunTool_Fst = str_temp +_T("\\RE_DL\\UnifyUI_FacTestTool.exe");
		str_RunTool_Snd = str_temp +_T("\\RE-DL\\UnifyUI_FacTestTool.exe");
		if(obj_fileFind.FindFile(str_RunTool_Fst))
		{
			str_RunToolPath = _T("RE_DL\\UnifyUI_FacTestTool.exe");
		}
		else if(obj_fileFind.FindFile(str_RunTool_Snd))
		{
			str_RunToolPath = _T("RE-DL\\UnifyUI_FacTestTool.exe");
		}
	}

	str_temp = str_temp + _T("\\") + str_RunToolPath;
	str_RunToolPath = str_temp;

	if (m_str_process==_T("OOB"))
	{
		str_FactoryResetPath = GetFactoryResetPath();
	}

	m_DBO.pQuerySO_Info_All(m_sz_SO,sz_model,sz_partNo,sz_soType,sz_toolVer,sz_facImageVer,sz_userImageVer,sz_ETtoolVer,sz_ErrCode,sz_ErrMsg);

	//::GetPrivateProfileString(_T("USER_VERSION"), _T("VERSION"), _T(""), sz_userImageVer, sizeof(sz_toolVer), INIFILEPASH);

	if(str_process.MakeUpper() == _T("OS_DL") || str_process.MakeUpper() == _T("IMEI") || str_process.MakeUpper() == _T("FCHECK") ||str_process.MakeUpper() == _T("SD_COPY"))
	{
		str_SWVersion.Format("%s",sz_userImageVer);
		//if(str_SWVersion.IsEmpty())
		//{
		//	::GetPrivateProfileString(_T("USER_VERSION"), _T("VERSION"), _T(""), sz_userImageVer, sizeof(sz_toolVer), INIFILEPASH);
		//	str_SWVersion.Format("%s",sz_userImageVer);
		//}
	}
	else
	{
		str_SWVersion.Format("%s",sz_facImageVer);
		//if(str_SWVersion.IsEmpty())
		//{
		//	::GetPrivateProfileString(_T("FACTORY_VERSION"), _T("VERSION"), _T(""), sz_facImageVer, sizeof(sz_toolVer), INIFILEPASH);
		//	str_SWVersion.Format("%s",sz_facImageVer);
		//}
	}

	if(m_bcommonTool)
	{
		str_SWVersion = _T("CommonTool");
	}


	str_ToolVersion.Format("%s",sz_toolVer);
	//if(str_ToolVersion.IsEmpty())
	//{
	//	str_ToolVersion = _T("V1.00");
	//}


	CString str_checkPath = str_temp;

	m_Clog.WriteLog(_T("调用的即将执行的tool的exe文档的地址为："),sz_ErrCode,sz_ErrMsg);
	m_Clog.WriteLog(str_checkPath,sz_ErrCode,sz_ErrMsg);

	if(!obj_fileFind.FindFile(str_checkPath))
	{
		CString str_Errcode = _T("ET0013");
		CString str_ErrMsg = _T("Execute file is not exist\n");
		str_ErrMsg = str_ErrMsg + _T("找不到可执行文件") + str_checkPath;
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_Errcode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_Errcode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}

	//在找到Unify_UI这个可执行文件后，获取他的VERSION，写入"Unify_UI",查询得到MD5值，作MD5 CHECK对这个可执行文件。
	//b_res=m_DBO.pVMS_QueryToolStatus("Unify_UI","v0.47",m_sz_facImageVer_Tool_pre2,m_sz_Status_Tool_pre2,m_sz_Reserve1_pre2,sz_ErrCode,sz_ErrMsg);

	b_res = DO_UiMD5Check(sz_ErrCode,sz_ErrMsg,str_batpath,str_checkPath);
	if(!b_res)
	{
		return false;
	}

	//========================================================================================================

	/*20110913*/
	CString str_Firstpart = str_RunToolPath.Left(str_RunToolPath.ReverseFind('\\'));

	str_Firstpart += "\n";

	CString str_Lastpart = str_RunToolPath.Mid(str_RunToolPath.ReverseFind('\\') + 1);

	str_RunToolPath = str_Firstpart + _T("start ") + str_Lastpart;
	/*20110913*/


	::GetCurrentDirectory(MAX_PATH, sz_Path);
	str_filename.Format("%s",sz_Path);
	str_filename = str_filename + "\\" + "EntranceTool.bat";
	m_str_filename = str_filename;


	//memset(sz_ErrCode,0,ERRORNUMBER);
	//memset(sz_ErrMsg,0,ERRORNUMBER);


	if(obj_fileFind.FindFile(str_filename))
	{
		DeleteFile(str_filename);

		if(fbat.Open (str_filename, CFile::typeText|CFile::modeReadWrite|CFile::modeCreate, &fileException))
		{
			CString str_temp = _T("");

			if(m_str_UserImage.IsEmpty())
			{
				if(m_str_Toolver.IsEmpty()||m_str_FactoryImage.IsEmpty())
				{
					m_str_ImagePath = _T("-");
					//m_str_ImagePath = _T("C:\\Image\\");
					//m_str_ImagePath = m_str_ImagePath + m_str_Model + _T("\\");
				}
			}

			//C:\TOOL\Luna_LG7\V304.01.S08A01\V304.01.S08A01\06_reDL\NVDLTool_0.034_reDL\NVDLTool.exe
			str_temp = str_RunToolPath;

			//str_toolpath = _T("cd /d ");
			str_toolpath = _T("cd /d ");
			str_toolpath = str_toolpath + str_temp;
			if (m_str_process==_T("OOB"))
			{
				str_cmd = str_toolpath + _T(" ") + str_SO + _T(" ") + str_EmployeeID + _T(" ") + str_SWVersion + _T(" ") + str_ToolVersion + _T(" ") + str_FactoryResetPath + _T(" ") + str_AccessoryList;
			}
			else
			{
				str_cmd = str_toolpath + _T(" ") + str_SO + _T(" ") + str_EmployeeID + _T(" ") + str_SWVersion + _T(" ") + str_ToolVersion + _T(" ") + m_str_ImagePath + _T(" ") + str_AccessoryList;
			}
			m_Clog.WriteLog(str_cmd,sz_ErrCode,sz_ErrMsg);
			fbat.WriteString(str_cmd);
			fbat.Close();
			str_batpath = str_filename;
			b_res = true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if(fbat.Open (str_filename, CFile::typeText|CFile::modeReadWrite|CFile::modeCreate, &fileException))
		{

			CString str_temp = _T("");

			if(m_str_UserImage.IsEmpty())
			{
				if(m_str_Toolver.IsEmpty()||m_str_FactoryImage.IsEmpty())
				{   
					m_str_ImagePath = _T("-");
					//m_str_ImagePath = _T("C:\\Image\\");
					//m_str_ImagePath = m_str_ImagePath + m_str_Model + _T("\\");
				}
			}

			str_temp = str_RunToolPath;


			str_toolpath = _T("cd /d ");
			str_toolpath = str_toolpath + str_temp;
			if(m_str_process==_T("OOB"))
			{
				str_cmd = str_toolpath + _T(" ") + str_SO + _T(" ") + str_EmployeeID + _T(" ") + str_SWVersion + _T(" ") + str_ToolVersion + _T(" ") + str_FactoryResetPath + _T(" ") + str_AccessoryList;
			}
			else
			{
				str_cmd = str_toolpath + _T(" ") + str_SO + _T(" ") + str_EmployeeID + _T(" ") + str_SWVersion + _T(" ") + str_ToolVersion + _T(" ") + m_str_ImagePath + _T(" ") + str_AccessoryList;
			}
			m_Clog.WriteLog(str_cmd,sz_ErrCode,sz_ErrMsg);
			fbat.WriteString(str_cmd);
			fbat.Close();
			str_batpath = str_filename;
			b_res = true;
		}
		else
		{
			return false;
		}
	}
	return b_res;
}

/******************************************************************************
* function name:  RunTool()                                  
* Parameters   :  VOID 
* Summary      :  Run Unify UI Tool
* Return       :  True  success.False  fail  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Tom Li          2011/07/22           First version                            
*****************************************************************************/
bool Funccheck::RunTool(TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg, CString str_process, CString str_ProjectName, CString str_AccessoryList)
{
	bool b_res = false;

	//str_commandOne = _T("cmd.exe /c ");
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb          = sizeof(si);
	si.wShowWindow = SW_HIDE;
	si.dwFlags     = STARTF_USESHOWWINDOW;

	//CreateBat(TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg, CString& str_batpath, CString str_process, CString str_ProjectName);
	CString str_batpath = _T("");
	b_res = CreateBat(sz_ErrCode, sz_ErrMsg, str_batpath, str_process, str_ProjectName, str_AccessoryList);
	if(!b_res)
	{
	  return FALSE;
	}

	if(b_res)
	{
		if(str_batpath == _T(""))
		{
			return false;
		}

		CString str_cmd = str_batpath;

		TCHAR sz_commandLine[1024];
		memset(sz_commandLine, 0, sizeof(sz_commandLine));
		for (int i = 0; i < str_cmd.GetLength(); i ++) 
		{
			sz_commandLine[i] = str_cmd[i];
		}
		b_res = CreateProcess(NULL, sz_commandLine, NULL, NULL, NULL, NULL, NULL, NULL, &si, &pi);
	}
	if(!b_res)
	{
		CString str_ErrCode = _T("ET0014");
		CString str_ErrMsg = _T("Create UnifyUI_FacTestTool process fail\n创建用户UnifyUI_FacTestTool进程失败");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}
	else
	{
		// Wait until child process exits.
		WaitForSingleObject( pi.hProcess, 4000);
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );

		const int WM_EntranceToolToUnifyUI = WM_USER+0X103;
		HWND hFind = NULL;
		for(int i=0;i<4;i++)
		{
			hFind = ::FindWindow(NULL,"UnifyUI");
			if(hFind)
			{
				i=4;
			}else
			{
				Sleep(1000);
			}

		}

		if(hFind)
		{
			for(int i=0;i<5;i++)
			{
				::PostMessage(hFind,WM_EntranceToolToUnifyUI,1,1);
				Sleep(1000);
			}
        }else
        {
               m_Clog.WriteLog(_T("UnifyUI window not found"));
        }



		//DeleteFile(str_batpath);
		DeleteFile(m_str_filename);
	}
	return b_res;
}

bool Funccheck::KillAdbProcess(TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	CString str_winExecCommand;
	CString str_commandOne;        
	CString str_commandTwo;         

	str_commandOne = _T("cmd.exe /c ");

	str_commandTwo = _T("taskkill /f /im adb.exe");
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
		m_Clog.WriteLog(_T("无法删除adb.exe请手动删除"),sz_ErrCode,sz_ErrMsg);

		CString str_Errcode = _T("ET0122");
		CString str_ErrMsg = _T("无法删除adb.exe请手动删除\n");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_Errcode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_Errcode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}
	else
	{  
		return true;
	}

}


bool Funccheck::ExecuteFactoryResetTool(CString str_path_Server, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{   
	CString str_log = _T("");
	str_log = _T("函数ExecuteFactoryResetTool开始执行");
	m_Clog.WriteLog(str_log,sz_ErrCode,sz_ErrMsg);

	bool b_res = true;
    
	CString str_path_Sever_MD5 = _T("");
    
	str_path_Sever_MD5 = str_path_Server;
	/*==================================================================================================================
	// Config Tool Path
	==================================================================================================================*/
	CString str_path_loc = _T("C:\\Tool\\");// Loc Tool Path
	str_path_loc = str_path_loc + m_str_ModelProjectName;
	str_path_loc = str_path_loc + _T("\\");

	if(str_path_Server.Right(1) != '\\')
	{
		str_path_Server = str_path_Server + _T("\\");
	}
	str_path_Server = str_path_Server + m_str_ModelProjectName;
	str_path_Server = str_path_Server + _T("\\");

	str_path_Server = str_path_Server + _T("Factory_Tool");
	str_path_Server = str_path_Server + _T("\\");
	str_path_Server = str_path_Server + _T("*.*");
	int n_TooldirectoryCheck = 0;
	CString str_temp = _T("");
    CString str_Server_Path = _T("");
    //m_str_FacResetToolver = _T("v1.012");
	n_TooldirectoryCheck = FindResetTooldirectory(str_path_Server,str_temp, m_str_FacResetToolver);
	if(n_TooldirectoryCheck != 1)
	{
		CString str_ErrCode = _T("ET0007");
		CString str_ErrMsg = str_path_Server + _T("can not be found\n");
		str_ErrMsg = str_ErrMsg + str_path_Server + _T("找不到工具文件夹");
		strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}

	str_Server_Path = str_temp;

	/*==================================================================================================================
	// Config MD5 Path
	==================================================================================================================*/


	CString str_Toolpath_MD5 = _T("");
	CString str_Serever_path_MD5 = _T("");
	CString str_Hashpath = _T("");
	CString str_LOC_Path = _T("C:\\TOOL\\");
	//str_LOC_Path = str_LOC_Path + m_str_ProjectName + _T("\\") + str_TOOL_version + _T("\\") + _T("MD5");
	str_LOC_Path = str_LOC_Path + m_str_ProjectName + _T("\\") + _T("MD5");

	//b_res = DeleteDirectory(_T("C:\\Tool\\"), sz_ErrCode, sz_ErrMsg);

	if(str_path_Server.Right(1) != '\\')
	{
		str_Serever_path_MD5 = str_path_Sever_MD5 + _T("\\");
	}
	str_Serever_path_MD5 = str_Serever_path_MD5 + m_str_ModelProjectName + _T("\\") + _T("MD5") + _T("\\");
	str_Serever_path_MD5 = str_Serever_path_MD5 + _T("Factory_Tool") + _T("\\");

    
	m_Clog.WriteLog(_T("FactoryReset TOOL COPY 开始，服务器地址和本地地址为："),sz_ErrCode,sz_ErrMsg);
	m_Clog.WriteLog(str_path_Server,sz_ErrCode,sz_ErrMsg);
	m_Clog.WriteLog(str_path_loc,sz_ErrCode,sz_ErrMsg);

	b_res = m_obj_DirectoryDispose.IsDirectoryExist(str_path_loc);

	if(!b_res)
	{
		//Tool did not exist
		b_res = ToolCopy(str_Server_Path, str_path_loc, sz_ErrCode, sz_ErrMsg);
		if(!b_res)
		{
			return false;
		}
	}
	else
	{
		////Tool exist, Do MD5 check
		//if(m_bMD5check_tool_Pre)
		//{
		//	b_res = MD5_CopyCheck(str_Serever_path_MD5, str_LOC_Path,sz_ErrCode, sz_ErrMsg);
		//	if(!b_res)
		//	{
		//		return false;
		//	}

		//	CString str_wild = _T("");
		//	str_wild = _T("*") + m_str_FacResetToolver + _T("*");
  //          
		//	//int i_checknumber = FindInformation(str_LOC_Path,str_Hashpath,str_wild);

  //          int i_checknumber = FindInformationToolMD5(str_LOC_Path,str_Hashpath,str_wild);

		//	CString str_replace = _T("");
		//	str_replace = m_str_FacResetToolver;
		//	if(i_checknumber == 1)
		//	{
		//		if(m_bMD5check_tool)
		//		{
		//			b_res = m_obj_MD5check.Do_MD5_Check(str_path_loc, str_Hashpath,str_replace, sz_ErrCode, sz_ErrMsg);
		//			if(b_res)
		//			{
		//				return true;
		//			}
		//		}
		//		else
		//		{
		//			return true;
		//		}
		//	}
		//}
		b_res = ToolCopy(str_Server_Path, str_path_loc, sz_ErrCode, sz_ErrMsg);
	}

	bool b_check = false;
	CString str_wild = _T("");
	int i_checknumber = 0;
	CString str_replace = _T("");
	if(m_bMD5check_tool)
	{
		b_check = MD5_CopyCheck(str_Serever_path_MD5, str_LOC_Path,sz_ErrCode, sz_ErrMsg);
		if(!b_check)
		{
			return false;
		}
		str_wild = _T("*") + m_str_FacResetToolver + _T("*");
		//i_checknumber = FindInformation(str_LOC_Path,str_Hashpath,str_wild);

        int i_checknumber = FindInformationResetToolMD5(str_LOC_Path,str_Hashpath,str_wild);

		str_replace = m_str_FacResetToolver;
		if(i_checknumber == 1)
		{
			str_path_loc = str_path_loc + _T("\\");
			if(m_bMD5check_tool)
			{
				b_res = m_obj_MD5check.Do_MD5_Check(str_path_loc, str_Hashpath,str_replace, sz_ErrCode, sz_ErrMsg);
				if(b_res == false)
				{
					return false;
				}
				else
				{
					m_Clog.WriteLog(_T("MD5 CHECK OK"),sz_ErrCode,sz_ErrMsg);
				}
			}
			else
			{
				return true;
			}
		}
		else
		{
			CString str_ErrCode = _T("ET1008");
			CString str_ErrMsg = _T("Can not find MD5 file in ") + str_LOC_Path;
			str_ErrMsg = str_ErrMsg + _T("\n在 ") + str_LOC_Path + _T(" 找不到MD5文件");
			strcpy_s(sz_ErrCode,ERRORNUMBER,str_ErrCode.GetBuffer());
			strcpy_s(sz_ErrMsg,ERRORNUMBER,str_ErrMsg.GetBuffer());
			str_ErrCode.ReleaseBuffer();
			str_ErrMsg.ReleaseBuffer();
			return false;
		}
	}

	return b_res;
}


CString Funccheck::GetFactoryResetPath()
{   
	CFileFind finder;
	bool b_res =false;
	CString str_temp = _T("C:\\Tool\\");
	str_temp = str_temp + m_str_ModelProjectName;

	str_temp = str_temp + _T("\\");
	str_temp = str_temp + _T("*.*");
	b_res = finder.FindFile(str_temp);
	while(b_res)
	{
		b_res = finder.FindNextFile();//检查文件是否检查完
		if(finder.IsDots())
			continue;

		CString str_filePath = finder.GetFilePath();
		if(finder.IsDirectory())
		{
			str_temp = str_filePath;
			break;
		}
		else
		{
			continue;
		}
	}

	str_temp = str_temp + _T("\\") + _T("FactoryReset");
  
	return str_temp;

}

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
LPFN_ISWOW64PROCESS fnIsWow64Process;

bool Funccheck::IsWow64()
{
    BOOL bIsWow64 = false;

    //IsWow64Process is not available on all supported versions of Windows.
    //Use GetModuleHandle to get a handle to the DLL that contains the function
    //and GetProcAddress to get a pointer to the function if available.

    fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
        GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

    if(NULL != fnIsWow64Process)
    {
        if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
        {
            //handle error
        }
    }
    return bIsWow64;
}
