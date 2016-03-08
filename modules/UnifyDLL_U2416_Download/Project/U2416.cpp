#include "StdAfx.h"
#include "U2416.h"
#include <io.h>
#include <direct.h>


CCriticalSection CU2416::m_obj_CritSection;
CCriticalSection CU2416::m_obj_MD5_CritSection;

CU2416::CU2416(int i_COMPort, CString str_multiDLFlag)
	:m_i_COMPort(i_COMPort), m_str_multiDLFlag(str_multiDLFlag)
{
	m_str_errorCode = _T("");
	m_str_DLMode = _T("");;
	m_i_rework = 0;
	m_i_reboot = 0;
	m_i_DDRCheck = 0;
	m_i_DumpEmmcLog = 0;
	m_i_RebootMode = 0;
	m_i_checkSum = 0;
	m_i_BackupNVPartition = 0;
	m_i_SupportQDownload = 0;
	m_i_eraseUserFat = 0;
	m_b_LogUpload = false;
	m_str_UploadServerIP = _T("10.85.68.13");
}

CU2416::~CU2416(void)
{

}

/*****************************************************************************
* Function name: Event      
* Summary      : CDLCommand::DMSSNop ... -> CDLCommand::AddMsg() ->
*                Fire(EVENT_DL_PROGRESS, (long)&Msg);
* Parameters   : str_Event (EVENT_DL_PROGRESS)  [input]
* Return       : void            
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/18          First version             
*****************************************************************************/
void CU2416::Event(const std::string& str_Event, long nParam)
{
	/* EVENT_DL_PROGRESS */
	if (str_Event == EVENT_DL_PROGRESS) 
	{
		DL_PROGRESS* p_st_progress = (DL_PROGRESS*)nParam;
		if (p_st_progress == NULL) 
		{
			return;
		}

		/* None: Only Show message, not update process */
		if (p_st_progress->nStep == None) 
		{
			SendMessageToUI(p_st_progress->szMsg);
		} 
		else 
		{
			int i_mainProgress = 0;
			int i_subProgress = p_st_progress->nProgress;
			switch (p_st_progress->nStep)
			{
				case ChangeDLMode:
					i_mainProgress = 5;
					break;
				case SaharaProtocol:
					i_mainProgress = 10;
					break;
				case Connection:
					i_mainProgress = 60;
					break;
				case StreamingProtocol:
					i_mainProgress = 65;
					break;
				case ReOpenComport:
					i_mainProgress = 70;
					break;
				case ADBProtocol:
					i_mainProgress = 85;
					break;
				case None:
					i_mainProgress = 90;
					break;
				default:
					break;
			}

			/* SendMessage + SetProgress to UI */
			char sz_temp[512] = {0};
			sprintf(sz_temp, "%s     %d%%", p_st_progress->szMsg, i_subProgress);
			SendMessageToUI(sz_temp);
			SetProgress(i_mainProgress, i_subProgress);
			if (i_subProgress == 100) 
			{
				sprintf(sz_temp, "Main progress:  %d%%", i_mainProgress);
				SendMessageToUI(sz_temp);
			}
		}
	}
}

/*****************************************************************************
* Function name: Run      
* Summary      : Download run.
*              
* Parameters   : 
* Return       :            
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/18          First version   
* 2.0           Alex.Chen       2011/09/19          Modify to: PreDL + ReDL
*****************************************************************************/
bool CU2416::RunMD5Check(){
	/* md5 check */

	CSingleLock singleLock(&m_obj_MD5_CritSection);
	singleLock.Lock();  // Attempt to lock the shared resource

	CGlobal::n_TTestTime = CGlobal::n_TTestTime + 1;

	CString csNtest;
	csNtest.Format("Md5 count = %d ",  CGlobal::n_TTestTime);
	SendMessageToUI(csNtest);
	
	//printf("%d", CGlobal::n_TTestTime);
	//printf("%d", CGlobal::nCheckMd5TimeLimit);

	if ( CGlobal::n_TTestTime >= CGlobal::nCheckMd5TimeLimit ){
		
		if ( !check_IMAGE_MD5())
		{
			SendMessageToUI("check_IMAGE_MD5 Fail in U2416.CPP");
			AfxMessageBox(_T("check_IMAGE_MD5 !! "));

			singleLock.Unlock();
			return false;	
		}else{
			CGlobal::n_TTestTime = 0 ;
			SendMessageToUI("check_IMAGE_MD5 PASS in U2416.CPP");
		}
	}
	singleLock.Unlock();

	return true;
}
bool CU2416::Run()
{
	bool b_res = false;
	/* Read ini file */
	b_res = ReadIniFile();
	
	/*lion*/
	//if ( b_res ) {
	//	SendMessageToUI("RunMD5Check Start");
	//	b_res = RunMD5Check();
	//}
	//
	//if ( !b_res ) {
	//	SendMessageToUI("RunMD5Check Fail");
	//}
	//



	if (b_res) 
	{
		/* PreDL */
		if (m_str_DLMode == DOWNLOAD_MODE_PREDL)
		{
			b_res = PreDLRun();
		}

		/* ReDL */
		else if ((m_str_DLMode == DOWNLOAD_MODE_REDL) 
			||(m_str_DLMode == DOWNLOAD_MODE_REDL_OSDL)
			||(m_str_DLMode == DOWNLOAD_MODE_OSDL1))
		{
			b_res = ReDLRun();
		}
		else
		{
			SendMessageToUI("CU2416::Run() Fail");
			b_res = false;
		}
	}

	Beep(750, 300);
	SetProgress(100, 100);

	return b_res;
}

/*****************************************************************************
* Function name: PostRun      
* Summary      : Download PostRun.
*              
* Parameters   : 
* Return       :            
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Evonne.Hsieh  2012/05/05          First version   
*****************************************************************************/
bool CU2416::PostRun(int i_slot)
{
	bool b_res = true;

	return true;
}


/*****************************************************************************
* Function name: PreDLRun      
* Summary      : PreDL.
*              
* Parameters   : 
* Return       :            
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/09/19          First version             
*****************************************************************************/
bool CU2416::PreDLRun(void)
{
	bool b_res = true;

	/* Download */
	if (b_res)
	{
		b_res = runDownload();
	}

	/* Sleep 1s */
	if (b_res) 
	{
		Sleep(1000);
	}

	/* Write PICS */
	SetAllPICSData();

	return b_res;
}


/*****************************************************************************
* Function name: ReDLRun      
* Summary      : ReDL.
*              
* Parameters   : 
* Return       :            
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/09/19          First version             
*****************************************************************************/
bool CU2416::ReDLRun(void)
{
	bool b_res = true;

	/* Download */
	if (b_res)
	{
		b_res = runDownload();
	}

	/* Sleep 1s */
	if (b_res) 
	{
		Sleep(1000);
	}

	/* Write PICS */
	SetAllPICSData();

	return b_res;
}


/*****************************************************************************
* Function name: CheckConnection      
* Summary      : Check handset is connected
*              
* Parameters   : 
* Return       :            
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/18          First version             
*****************************************************************************/
//bool CU2416::CheckConnection(CCFCDiagCommandPtr pDiag)
//{
//	/* Check Input */
//	if (pDiag == NULL)
//	{
//		SendMessageToUI("Check DIAG Connection Fail.");
//		return false;
//	}
//
//	/* Check Connected */
//	bool b_res = false;
//	int i_retryCount = 5;
//	while (i_retryCount)
//	{
//		b_res = pDiag->IsConnected();
//		if (b_res) 
//		{
//			SendMessageToUI("Check handset connection Succeed.");
//			break;
//		} 
//		--i_retryCount;
//		Sleep(1000);
//	}
//	if (!b_res) 
//	{
//		SendMessageToUI("Check handset connection Fail(Not connected or Get mode Fail).");
//		SetError(DLERR_TARGET_ERROR);
//	}
//
//	return b_res;
//}


/*****************************************************************************
* Function name: IsDLMode      
* Summary      : Check is DL mode
*              
* Parameters   : 
* Return       :            
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/18          First version             
*****************************************************************************/
//bool CU2416::IsDLMode(CCFCDiagCommandPtr pDiag)
//{
//	/* Check Input */
//	if (pDiag == NULL) 
//	{
//		SendMessageToUI("Check DIAG Connection Fail.");
//		return false;
//	}
//
//	bool b_isDLMode = pDiag->IsDLMode();
//	SendMessageToUI(b_isDLMode ? "DL mode" : "DIAG mode");
//
//	if (b_isDLMode == false)
//	{
//		SetError(DLERR_PHONE_IN_DL_MODE);
//	}
//
//	return b_isDLMode;
//}
//

/*****************************************************************************
* Function name: Download      
* Summary      : Download start
*              
* Parameters   : 
* Return       :            
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/18          First version             
*****************************************************************************/
bool CU2416::runDownload(void)
{
	SendMessageToUI("CU2416 Download Start.");

	bool b_res = DLchipset();
	if (b_res)
	{
		SendMessageToUI("CU2416 Download Success.");
	} 
	else 
	{
		SendMessageToUI("CU2416 Download Failure.");
		SetError(m_str_errorCode);
	}

	return b_res;
}


/*****************************************************************************
* Function name: DLchipset      
* Summary      : Download process
*              
* Parameters   : 
* Return       :            
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/18          First version             
*****************************************************************************/
bool CU2416::DLchipset(void)
{
	SendMessageToUI("SetImgae.");
	bool b_res = true;
	int i_COMPort = m_i_COMPort;

	/* CDownload8994 */
	CDownload8994 obj_download8994(i_COMPort, m_str_multiDLFlag);
	obj_download8994.Register(this, EVENT_DL_PROGRESS);

	/* Set DL Parameter */

	obj_download8994.SetDLMode(m_str_DLMode);
	CString str_imagePath;
	str_imagePath.Format("%s", m_vImage[0].str_imagePath.c_str());
	obj_download8994.SetImagePath(str_imagePath);

	//obj_download8994.SetFactoryVersion(m_str_fatoryVersion);
	//obj_download8994.GetOemMainVersion(m_str_fatoryVersionDevice);
	

	/*lion*/
	
	if (obj_download8994.setIMGPath( m_vImage[0].str_imagePath.c_str()) ) {
		SendMessageToUI("setIMGPath. ok");
	}
	else{
		SendMessageToUI("setIMGPath. fail");
	}
	Sleep(500);
	if (obj_download8994.ReadIMG( m_vImage[0].str_imagePath.c_str()) ) {
		SendMessageToUI("ReadIMG. ok");
	}
	else{
		SendMessageToUI("ReadIMG. fail");
	}
	if (obj_download8994.ReadIMG_qfil( m_vImage[0].str_imagePath.c_str())) {
		SendMessageToUI("setIMGPathReadIMG_qfil ok");
	}
	else{
		SendMessageToUI("ReadIMG_qfil. fail");
	}

	SendMessageToUI("SetImgae. ok");

	//b_res = SetImageFile(obj_download8994, IMAGE_TYPE_8916::Q8K_HEX, _T("hex"));
	//if (b_res) 
	//{
	//	b_res = SetImageFile(obj_download8994, IMAGE_TYPE_8916::Q8K_MSIMAGE, _T("msimage"));
	//}

	/* Download */
	if (b_res) 
	{
		SendMessageToUI("Start DL");
		 DownloadProtocol nDLPROTOCOL = QTT_FASTDOWNLOAD;
		b_res = obj_download8994.MultiDownload(NULL, NULL,  nDLPROTOCOL, m_i_idtype);
		SendMessageToUI("DL OK");
	}

	/* Get Error code */
	m_str_errorCode = obj_download8994.GetErrorCode();

	obj_download8994.Unregister(this, EVENT_DL_PROGRESS);

	
	return b_res;
	/*********************new new new ******************************************/


	/* Set DL Parameter */
	if (b_res) 
	{
		obj_download8994.SetDLMode(m_str_DLMode);
	/*	obj_download8994.SetReworkFlag(m_i_rework);
		obj_download8994.SetRebootFlag(m_i_reboot);
		obj_download8994.SetDDRCheckFlag(m_i_DDRCheck);
		obj_download8994.SetDumpEmmcLogFlag(m_i_DumpEmmcLog);
		obj_download8994.SetCheckSumFlag(m_i_checkSum);
		obj_download8994.SetBackupNVFlag(m_i_BackupNVPartition);*/

		CString str_imagePath;
		str_imagePath.Format("%s", m_vImage[0].str_imagePath.c_str());
		
	}

	/* Download */
	if (b_res) 
	{
		b_res = obj_download8994.MultiDownload();
	}

	/* Get Error code */
	m_str_errorCode = obj_download8994.GetErrorCode();

	obj_download8994.Unregister(this, EVENT_DL_PROGRESS);

	return b_res;
}

bool CU2416::check_IMAGE_MD5(){
	
	std::string str_fileName;
	vector<st_Image>::iterator vIter;

	genMD5(  m_vImage[0].str_imagePath.c_str());//lion
	/*gen new MD5*/
	CString str_logFile ;

	/*read new md5*/
	str_logFile = _T("c:\\NewImgMD5.txt");
	readMD5( &mapNewMd5, str_logFile );

	/*read old md5*/
	str_logFile.Format("%s%s",m_vImage[0].str_imagePath.c_str() , _T("\\md5.lst"));
	readMD5( &mapExistMd5, str_logFile );


	for (vIter = m_vImage.begin(); vIter != m_vImage.end(); vIter++)
	{
		CString str_image = (*vIter).str_imageName.c_str();
		if ( !compareMD5(str_image))  return false;
	
	}

	return true;;

}
bool CU2416::genMD5(CString folderPath){

	CString str_dirPath;
	CString str_XMLFilePathName;
	CString str_logFile = _T("C:\\NewImgMD5.txt");

	mapExistMd5.clear();
	mapExistMd5.clear();

	/* Check fciv.exe exist */
	char sz_currentPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, sz_currentPath, MAX_PATH);
	::PathRemoveFileSpec(sz_currentPath);
	CString str_fcivPathName;
	str_fcivPathName.Format(_T("%s\\MD5\\fciv.exe"), sz_currentPath);
	if (_taccess(str_fcivPathName, 0) != 0) 
	{
		return false;
	}

	/* Command */
	CString str_command;
	CString str_commandOne;
	CString str_commandTwo;
	CString str_commandThree;
	str_commandOne = _T("cmd.exe /c ");
	str_commandTwo = str_fcivPathName + _T(" -r ") + folderPath;
	str_commandThree = str_commandThree + _T(" > ") + str_logFile;
	str_command = str_commandOne + str_commandTwo + str_commandThree;

	/* Run */
	TCHAR sz_commandLine[1024];
	memset(sz_commandLine, 0, sizeof(sz_commandLine));
	for (int i = 0; i < str_command.GetLength(); i ++) 
	{
		sz_commandLine[i] = str_command[i];
	}
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
		return false;
	}
	DWORD dw_waitResult = 0;
	DWORD dw_timeout;
	dw_timeout = 10000;
	dw_waitResult = ::WaitForSingleObject(pi.hProcess, dw_timeout);
	switch(dw_waitResult)
	{
	case WAIT_FAILED:
		::CloseHandle(pi.hProcess);
		pi.hProcess = NULL;
		return false;

	case WAIT_TIMEOUT:
		::CloseHandle(pi.hProcess);
		pi.hProcess = NULL;
		return false;

	case WAIT_OBJECT_0:
		break;
	}
	::CloseHandle(pi.hProcess);
	pi.hProcess = NULL;


	return true;
}

bool CU2416::compareMD5(CString csIMGName){

    set <CString >::iterator m_ExistIMGMD5Iter;
	set <CString >::iterator m_NewIMGMD5Iter;

	CString csExistIMGMD5, csNewIMGMD5;
	int nTokenPos = 0;
	int nResult = 0;


	nResult = 0;
    for ( m_ExistIMGMD5Iter = mapExistMd5.begin() ; m_ExistIMGMD5Iter != mapExistMd5.end() ; m_ExistIMGMD5Iter++ )
    {
		csExistIMGMD5 = *m_ExistIMGMD5Iter;
		if (  (csExistIMGMD5.Find( csIMGName ) != -1) && (csExistIMGMD5.Find(_T("ramdisk-recovery.img")) == -1)){
			/*get it*/
			break;
		}

    }

	/*get md5*/
	CString strToken_ExistIMGMD;
	if ( m_ExistIMGMD5Iter == mapExistMd5.end()){
		/*not find*/
		return false;
	}else{
		 nTokenPos = 0;
		 strToken_ExistIMGMD = csExistIMGMD5.Tokenize(_T(" "), nTokenPos);
	}

	/*******************************************************/

	nResult = 0;
    for ( m_NewIMGMD5Iter = mapNewMd5.begin() ; m_NewIMGMD5Iter != mapNewMd5.end() ; m_NewIMGMD5Iter++ )
    {
		csNewIMGMD5 = *m_NewIMGMD5Iter ;
		nResult = csNewIMGMD5.Find( csIMGName );
		if ( nResult != -1 ){//get it
			break;
		}
    }

	/*get md5*/
	CString strToken_NewIMGMD5 ;
	if ( m_NewIMGMD5Iter == mapNewMd5.end()){
		/*not find*/
		return false;
	}else{
		nTokenPos = 0;
		strToken_NewIMGMD5 = csNewIMGMD5.Tokenize(_T(" "), nTokenPos);
	}

	strToken_ExistIMGMD.Trim();
	strToken_NewIMGMD5.Trim();
	CString csMD5Result;
	if (strToken_ExistIMGMD.Compare( strToken_NewIMGMD5 ) == 0){
		//check pass
		csMD5Result.Format( _T("check %s Success"), csNewIMGMD5);
		SendMessageToUI( csMD5Result );
		csMD5Result.ReleaseBuffer();
		return true;
	}else
	{
		//check fail
		csMD5Result.Format( _T("check %s Fail"), csNewIMGMD5);
		SendMessageToUI( csMD5Result );
		csMD5Result.ReleaseBuffer();
		return false;
	}

	csMD5Result.Format( _T("check %s Fail"), csMD5Result);
	//SetErrorMessage( csMD5Result.GetBuffer(0) , 0);
	csMD5Result.ReleaseBuffer();

	return false;
}

bool CU2416::readMD5(set <CString> *readMapMd5,  CString csPath){

	//CString str_logFile = _T("c:\\NewImgMD5.txt");
	/* Check Input */
	if(csPath == _T("")) 
	{
		return false;
	}

	/* Check file exist */
	int i_existentResult = 0;
	int i_retry = 10;
	while(1)
	{
		i_existentResult = _taccess(csPath, 0);
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
	if(i_existentResult != 0) 
	{
		return false;
	}

	/* Check file Size */
	DWORD dw_fileSize = 0;
	i_retry = 10;
	while(1) 
	{
		HANDLE h_handle = NULL;
		h_handle = CreateFile(csPath, FILE_READ_EA, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
		if(h_handle != INVALID_HANDLE_VALUE)
		{
			dw_fileSize = GetFileSize(h_handle, NULL);
			CloseHandle(h_handle);
			if(dw_fileSize == 0)
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
				Sleep(1000);
				break;
			}
		} 
		else {
			break;
		}
	}
	if(dw_fileSize == 0)
	{
		return false;
	}


	int i_succeedFlag = 0;

	CStdioFile obj_stdioFile;
	if (!obj_stdioFile.Open(csPath, CFile::modeRead|CFile::modeNoTruncate|CFile::shareDenyNone, NULL))
	{
		return false;
	} 
	try 
	{
		CString str_string;
		while(obj_stdioFile.ReadString(str_string)) 
		{
			readMapMd5->insert(str_string);

			if(str_string.Find(_T("All files verified successfully")) != -1) 
			{
				i_succeedFlag = 1;
				break;
			}
			str_string = _T("");
		}
	} 
	catch(CFileException *e) 
	{
		e->Delete();
	}

	obj_stdioFile.Close();

	if (i_succeedFlag == 1)
	{
		return true;
	}
	else
	{
		return false;
	}


	return true;
}


bool CU2416::ReadIniFile(void)
{
	CSingleLock singleLock(&m_obj_CritSection, TRUE);

	m_str_dllIniFile.Format(_T("%s"), m_sz_iniFile);
	if (m_str_dllIniFile == _T(""))
	{
		/* Get current DLL path */
		char sz_currentPath[MAX_PATH] = { 0 };
		::GetModuleFileName(NULL, sz_currentPath, MAX_PATH); 
		::PathRemoveFileSpec(sz_currentPath);

		/* Check data file exist */
		
		CDLInstance csCDLInstance;
		CString str_iniFile;
		str_iniFile.Format(_T("%s\\QISDA\\%s_%s_TestItem.xml"), sz_currentPath, csCDLInstance.GetProjectName(),csCDLInstance.GetProjectName() ); 
		if (_taccess(str_iniFile, 0) != 0) 
		{
			SendMessageToUI("TestItem.xml file is not exist.");
			SetError(DLERR_UNKNOWN);
			return false;
		}

		m_str_dllIniFile = str_iniFile;
	}
	if (m_str_dllIniFile == _T(""))
	{
		return false;
	}

	/* Get DL Mode */
	bool b_res = false;
	b_res = GetDLMode();


	/* Get GetIDType flag */
	if (b_res) 
	{
		b_res = GetIDType();
	}

	///* Get Reboot flag */
	//if (b_res) 
	//{
	//	b_res = GetReboot();
	//}

	///* Get DDR Check flag */
	//if (b_res) 
	//{
	//	b_res = GetDDRCheck();
	//}

	///* Get DumpEmmcLog flag */ 
	//if (b_res) 
	//{
	//	b_res = GetDumpEmmcLog();
	//}

	///* Get DumpEmmcLog flag */ 
	//if (b_res) 
	//{
	//	b_res = GetRebootMode();
	//}
	//

	///* Get CheckSum flag */
	//if (b_res) 
	//{
	//	b_res = GetCheckSum();
	//}

	///* Get Backup NV flag */
	//if (b_res) 
	//{
	//	b_res = GetBackupNVPartition();
	//}

	///* Get Support for QDownload */
	//if (b_res) 
	//{
	//	b_res = GetSupportQDownload();
	//}

	///* Get Erase user fat flag */
	//if (b_res) 
	//{
	//	b_res = GetEraseUserFat();
	//}

	///* Get Log upload info flag */
	//if (b_res) 
	//{
	//	b_res = GetLogUploadInfo();
	//}


	return b_res;
}

bool CU2416::GetDLMode(void)
{
	CString str_DLMode;

	char sz_mode[128] = {0};
	CIniAccess obj_dllIni(_T("Current_Mode"), m_str_dllIniFile);
	obj_dllIni.GetValue(_T("Mode"), sz_mode, sizeof(sz_mode));

	str_DLMode.Format(_T("%s"), sz_mode);

	if (str_DLMode == _T(""))
	{
		SendMessageToUI("Get DL Mode Fail from QISDA.");
		SetError(DLERR_UNKNOWN);
		return false;
	}

	m_str_DLMode = str_DLMode;

	return true;
}

bool CU2416::GetIDType(void)
{
	int i_idtype = -1;

	CIniAccess obj_dllIni("IDTYPE", m_str_dllIniFile);
	i_idtype = obj_dllIni.GetValue("IDTYPE", 2); 

	if ((i_idtype != 1) && (i_idtype != 2))
	{
		SendMessageToUI("GetIDType Fail from QISDA.");
		SetError(DLERR_UNKNOWN);
		return false;
	}

	m_i_idtype = i_idtype;

	return true;
}

//bool CU2416::GetRework(void)
//{
//	int i_rework = -1;
//
//	CIniAccess obj_dllIni("Setting", m_str_dllIniFile);
//	i_rework = obj_dllIni.GetValue("Rework", -1); 
//
//	if ((i_rework != 0) && (i_rework != 1))
//	{
//		SendMessageToUI("Get Rework Fail from QISDA.");
//		SetError(DLERR_UNKNOWN);
//		return false;
//	}
//
//	m_i_rework = i_rework;
//
//	return true;
//}
//
//bool CU2416::GetReboot(void)
//{
//	int i_reboot = -1;
//
//	CIniAccess obj_dllIni("Setting", m_str_dllIniFile);
//	i_reboot = obj_dllIni.GetValue("Reboot", -1);
//
//	if ((i_reboot != 0) && (i_reboot != 1))
//	{
//		SendMessageToUI("Get Reboot Fail from QISDA.");
//		SetError(DLERR_UNKNOWN);
//		return false;
//	}
//
//	m_i_reboot = i_reboot;
//
//	return true;
//}
//
//bool CU2416::GetDDRCheck(void)
//{
//	int i_DDRCheck = -1;
//
//	CIniAccess obj_dllIni("Setting", m_str_dllIniFile);
//	i_DDRCheck = obj_dllIni.GetValue("DDRCheck", -1);
//
//	if ((i_DDRCheck != 0) && (i_DDRCheck != 1))
//	{
//		SendMessageToUI("Get DDRCheck Fail from QISDA.");
//		SetError(DLERR_UNKNOWN);
//		return false;
//	}
//
//	m_i_DDRCheck = i_DDRCheck;
//
//	return true;
//}
//
//bool CU2416::GetDumpEmmcLog(void)
//{
//	int i_DumpEmmcLog = -1;
//
//	CIniAccess obj_dllIni("Setting", m_str_dllIniFile);
//	i_DumpEmmcLog = obj_dllIni.GetValue("DumpEmmcLog", -1);
//
//	if ((i_DumpEmmcLog != 0) && (i_DumpEmmcLog != 1))
//	{
//		SendMessageToUI("GetDumpLog Fail from QISDA.");
//		//SetError(DLERR_UNKNOWN);
//		//return false;
//	}
//
//	m_i_DumpEmmcLog = i_DumpEmmcLog;
//
//	return true;
//}
//
//bool CU2416::GetRebootMode(void)
//{
//	int i_RebootMode = -1;
//
//	CIniAccess obj_dllIni("Setting", m_str_dllIniFile);
//	i_RebootMode = obj_dllIni.GetValue("RebootMode", -1);
//
//	if (i_RebootMode == -1 )
//	{
//		SendMessageToUI("GetRebootMode Fail from QISDA");
//		//return false;
//	}
//
//	m_i_RebootMode = i_RebootMode;
//	return true;
//}
//
//
//
//bool CU2416::GetCheckSum(void)
//{
//	int i_checkSum = -1;
//
//	CIniAccess obj_dllIni("Setting", m_str_dllIniFile);
//	i_checkSum = obj_dllIni.GetValue("CheckSum", -1);
//
//	if ((i_checkSum != 0) && (i_checkSum != 1))
//	{
//		SendMessageToUI("Get CheckSum Fail from QISDA.");
//		SetError(DLERR_UNKNOWN);
//		return false;
//	}
//
//	m_i_checkSum = i_checkSum;
//
//	return true;
//}
//
//bool CU2416::GetBackupNVPartition(void)
//{
//	int i_BackupNVPartition = -1;
//
//	CIniAccess obj_dllIni("Setting", m_str_dllIniFile);
//	i_BackupNVPartition = obj_dllIni.GetValue("BackupNVPartition", -1);
//
//	if ((i_BackupNVPartition != 0) && (i_BackupNVPartition != 1))
//	{
//		SendMessageToUI("Get BackupNVPartition Fail from QISDA.");
//		SetError(DLERR_UNKNOWN);
//		return false;
//	}
//
//	m_i_BackupNVPartition = i_BackupNVPartition;
//
//	return true;
//}

//bool CU2416::GetSupportQDownload(void)
//{
//	int i_SupportQDownload = -1;
//
//	CIniAccess obj_dllIni("Setting", m_str_dllIniFile);
//	i_SupportQDownload = obj_dllIni.GetValue("SupportQDownload", -1);
//
//	if ((i_SupportQDownload != 0) && (i_SupportQDownload != 1))
//	{
//		SendMessageToUI("Get SupportQDownload Fail from QISDA.");
//		SetError(DLERR_UNKNOWN);
//		return false;
//	}
//
//	m_i_SupportQDownload = i_SupportQDownload;
//
//	return true;
//}

//bool CU2416::GetEraseUserFat(void)
//{
//	int i_eraseUserFat = -1;
//
//	CIniAccess obj_dllIni("Setting", m_str_dllIniFile);
//	i_eraseUserFat = obj_dllIni.GetValue("EraseUserFat", -1);
//
//	if ((i_eraseUserFat != 0) && (i_eraseUserFat != 1))
//	{
//		SendMessageToUI("Get EraseUserFat Fail from QISDA.");
//		SetError(DLERR_UNKNOWN);
//		return false;
//	}
//
//	m_i_eraseUserFat = i_eraseUserFat;
//
//	return true;
//}
//
//bool CU2416::GetLogUploadInfo(void)
//{	
//	CString str_LogUpload;
//	CString str_UploadServerIP;
//	char sz_outBuffer[64] = {0};
//
//	CIniAccess obj_dllIni("Setting", m_str_dllIniFile);
//	if (!obj_dllIni.GetValue ("LogUpload", sz_outBuffer, 64)) 
//	{
//		m_b_LogUpload = false;
//	}
//	else
//	{
//		str_LogUpload.Format(_T("%s"), sz_outBuffer);
//		m_b_LogUpload = (str_LogUpload.Compare(_T("TRUE")) == 0) ? true : false;
//	}
//	
//	memset(sz_outBuffer, 0, 64);
//	if (!obj_dllIni.GetValue ("UploadServerIP", sz_outBuffer, 64)) 
//	{
//		m_str_UploadServerIP = _T("10.85.68.13");
//	}
//	else
//	{
//		str_UploadServerIP.Format(_T("%s"), sz_outBuffer);
//	}
//
//	return true;
//}
//
//bool CU2416::ForceToEnterDLMode(void)
//{
//	/* Read ini file */
//	if (ReadIniFile() != true)
//	{
//		return false;
//	}
//
//	bool b_res = false;
//	int i_COMPort = m_i_COMPort;
//	CDownload8994 obj_download8994(i_COMPort, m_str_multiDLFlag);
//	obj_download8994.Register(this, EVENT_DL_PROGRESS);
//
//	obj_download8994.SetDLMode(m_str_DLMode);
//	//obj_download8994.SetSupportQDownloadFlag(m_i_SupportQDownload);
//
//	/* Erase sbl1 to enter emergence DL mode */
////	b_res = obj_download8994.EraseSBL1();
//	if (b_res)
//	{
//		b_res = obj_download8994.Reboot();
//		if (b_res)
//		{
//			Sleep(3000);
//		}
//	}
//
//	obj_download8994.Unregister(this, EVENT_DL_PROGRESS);
//
//	return b_res;
//}

bool CU2416::ReadFA(char* sz_FAData)
{
	/* Read ini file */
	if (ReadIniFile() != true)
	{
		return false;
	}

	bool b_res = false;
	int i_COMPort = m_i_COMPort;
	CDownload8994 obj_download8994(i_COMPort, m_str_multiDLFlag);
	obj_download8994.Register(this, EVENT_DL_PROGRESS);

//	obj_download8994.SetDLMode(m_str_DLMode);
	//obj_download8994.SetSupportQDownloadFlag(m_i_SupportQDownload);

	/* Read FA */
	char sz_data[512] = {0};
	b_res = obj_download8994.ReadFA(sz_data);
	memcpy(sz_FAData, sz_data, 512);

	obj_download8994.Unregister(this, EVENT_DL_PROGRESS);

	return b_res;
}

bool CU2416::WriteFA(char* sz_FAData)
{	
	/* Read ini file */
	if (ReadIniFile() != true)
	{
		return false;
	}

	bool b_res = false;
	int i_COMPort = m_i_COMPort;
	CDownload8994 obj_download8994(i_COMPort, m_str_multiDLFlag);
	obj_download8994.Register(this, EVENT_DL_PROGRESS);

	obj_download8994.SetDLMode(m_str_DLMode);
	//obj_download8994.SetSupportQDownloadFlag(m_i_SupportQDownload);

	/* Write FA */
	b_res = obj_download8994.WriteFA(sz_FAData);

	obj_download8994.Unregister(this, EVENT_DL_PROGRESS);

	return b_res;
}

bool CU2416::ReadFASector(int i_sectorNum, char *sz_sectorData, int i_sectorSize, int i_idtype)
{
	/* Read ini file */
	if (ReadIniFile() != true)
	{
		return false;
	}

	bool b_res = false;
	int i_COMPort = m_i_COMPort;
	CDownload8994 obj_download8994(i_COMPort, m_str_multiDLFlag);
	obj_download8994.Register(this, EVENT_DL_PROGRESS);

	obj_download8994.SetDLMode(m_str_DLMode);
	//obj_download8994.SetSupportQDownloadFlag(m_i_SupportQDownload);

	/* Read FA */
	b_res = obj_download8994.ReadFASector(i_sectorNum, sz_sectorData, i_sectorSize, i_idtype);

	obj_download8994.Unregister(this, EVENT_DL_PROGRESS);

	return b_res;
}

bool CU2416::WriteFASector(int i_sectorNum, char *sz_sectorData, int i_sectorSize)
{
	/* Read ini file */
	if (ReadIniFile() != true)
	{
		return false;
	}

	bool b_res = false;
	int i_COMPort = m_i_COMPort;
	CDownload8994 obj_download8994(i_COMPort, m_str_multiDLFlag);
	obj_download8994.Register(this, EVENT_DL_PROGRESS);

	obj_download8994.SetDLMode(m_str_DLMode);
	//obj_download8994.SetSupportQDownloadFlag(m_i_SupportQDownload);

	/* Write FA */
	b_res = obj_download8994.WriteFASector(i_sectorNum, sz_sectorData, i_sectorSize);

	//obj_download8994.Unregister(this, EVENT_DL_PROGRESS);

	return b_res;
}

//upload log to server by BAT
bool CU2416::LogUploadByBAT(char* str_modelname)
{
	if (!m_b_LogUpload) return true;

	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = "open";
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;

	char szModulePath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szModulePath, MAX_PATH);
	PathRemoveFileSpec(szModulePath);

	char str_ComputerName[50];
	DWORD dwRet = 50;
	GetComputerName(str_ComputerName, &dwRet);

	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	char sysdate[100];
	sprintf(sysdate, _T("%04d%02d%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay);

	char path_bat[2000];
	sprintf(path_bat, "%s\\Qisda\\Uplog.bat", szModulePath);
	ShExecInfo.lpFile = path_bat;

	char bat_param[2000];
	sprintf(bat_param, "%s %s %s %s", m_str_UploadServerIP.c_str(), str_modelname, str_ComputerName, sysdate);
	ShExecInfo.lpParameters = bat_param;

	ShellExecuteEx(&ShExecInfo);
	return true;
}


bool CU2416::LogUpload(char *str_modelname)
{
	//bool b_res = ReadIniFile();
	//if(!b_res)
	//	return false;

	if(!m_b_LogUpload)
		return true;

	//砞﹚Shell Execute把计
	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = "open";
	ShExecInfo.lpFile = "cmd";	
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;

	char str_RemotePath[1000];  
	char str_LocalPath[1000] = _T("D:\\Log");  
	char str_BackupPath[1000] = _T("D:\\LogUpload"); 
	char temp[2000];

	//-- ミ环狠隔畖
	strcpy(str_RemotePath, _T("\\\\"));
	strcat(str_RemotePath, m_str_UploadServerIP.c_str());
	strcat(str_RemotePath, _T("\\TestLog"));

	char str_account[100] = _T("mpics\\TestLog");
	char str_pswd[100] = _T("qisda2013");
	sprintf(temp, "/C net use %s /user:%s %s", str_RemotePath, str_account, str_pswd );
	ShExecInfo.lpParameters = temp;	
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, 3000);

	//-- 代刚ミ硈絬琌Θ
	sprintf(temp, "/C net use %s", str_RemotePath);
	ShExecInfo.lpParameters = temp;	
	ShellExecuteEx(&ShExecInfo);
	if(WaitForSingleObject(ShExecInfo.hProcess, 2000) == WAIT_TIMEOUT)
	{
		SendMessageToUI("Fail to connect server uploading.");
		return false;
	}

	_mkdir(str_RemotePath);
	strcat(str_RemotePath, _T("\\"));
	strcat(str_RemotePath, str_modelname);
	_mkdir(str_RemotePath);

	char str_ComputerName[50];
	DWORD dwRet = 50;
	GetComputerName(str_ComputerName, &dwRet);
	strcat(str_RemotePath, _T("\\"));
	strcat(str_RemotePath, str_ComputerName);
	_mkdir(str_RemotePath);

	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	char systime[100];
	sprintf(systime, _T("%04d%02d%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay );
	strcat(str_RemotePath, _T("\\"));
	strcat(str_RemotePath, systime);

	_mkdir(str_RemotePath);
	
	//-- 狡籹┮Τ郎环狠
	sprintf(temp, "/C xcopy /e /y %s %s", str_LocalPath, str_RemotePath);
	ShExecInfo.lpParameters = temp;	
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, 20000); 

	//-- 絋粄戈Ч俱肚癳环狠
	bool b_ok = DFSCheckFile(str_LocalPath, str_RemotePath, _T(""));

	//-- 狡籹logセ﹚ヘ魁埃
	if(b_ok)
	{
		_mkdir(str_BackupPath);
		sprintf(temp, "/C xcopy /e /y /q %s %s", str_LocalPath, str_BackupPath);
		ShExecInfo.lpParameters = temp;
		ShellExecuteEx(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE); 
		
		DFSDeleteFile(str_LocalPath, _T(""));
		//sprintf(temp, "/C rmdir /s /q %s", str_LocalPath);
		//ShExecInfo.lpParameters = temp;
		//ShellExecuteEx(&ShExecInfo);
		//WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
		SendMessageToUI("Check and Upload Pass.");
		return true;
	}
	else
	{
		SendMessageToUI("Copy and Check upload file Fail.");
		return false;
	}
}

/* Check upload file */
bool CU2416::DFSCheckFile(char *str_LocalPath, char *str_RemotePath, char *str_TracePath)
{
	char szDir[1000];
	char str_tempPath[1000];
    HANDLE hList;
    WIN32_FIND_DATA FileData;

	sprintf(szDir, _T("%s\\%s\\*"), str_LocalPath, str_TracePath);
	if ( (hList = FindFirstFile(szDir, &FileData)) == INVALID_HANDLE_VALUE )	//隔畖⊿糷隔畖,Τ郎
	{	
		sprintf(szDir, _T("%s\\%s"), str_RemotePath, str_TracePath);			//絋粄环狠隔畖Τ郎
		FindClose(hList);
		if(_taccess(szDir, 0) == -1){											//тぃ环狠隔畖郎
			SendMessageToUI(str_TracePath);
			return false;
		}
		else{																	//т环狠隔畖郎
			return true;
		}
	}
	while (1)
	{
        if (!FindNextFile(hList, &FileData)) {
			if (GetLastError() == ERROR_NO_MORE_FILES){
				FindClose(hList);
                return true;
			}
        }
		if(strcmp(FileData.cFileName, _T("..")) != 0){
			strcpy(str_tempPath, str_TracePath);
			if(strcmp(str_tempPath, "") != 0)
				strcat(str_tempPath, _T("\\"));
			strcat(str_tempPath, FileData.cFileName);
			if(!DFSCheckFile(str_LocalPath, str_RemotePath, str_tempPath)){
				FindClose(hList);
				return false;
			}
		}
    }
}
/* Delete local file */
bool CU2416::DFSDeleteFile(char *str_LocalPath, char *str_TracePath)
{
	char szDir[1000];
	sprintf(szDir, "%s\\%s\\*", str_LocalPath, str_TracePath);
	HANDLE hList;
	WIN32_FIND_DATA FileData;
	if ((hList = FindFirstFile(szDir, &FileData)) == INVALID_HANDLE_VALUE)//此路徑沒下層路徑,有可能為檔案
	{
		bool b_DelFile = true;
		//.csv檔不刪
		if(strstr(str_TracePath, ".CSV") != NULL)	b_DelFile = false;

		SYSTEMTIME systemTime;
		GetLocalTime(&systemTime);
		char TodayKeepLog[1000];

		//DB_Log今日檔不刪
		sprintf(TodayKeepLog, "DB_Log\\%04d_%02d_%02d", systemTime.wYear, systemTime.wMonth, systemTime.wDay);
		if (strstr(szDir, TodayKeepLog) != NULL)	b_DelFile = false;

		//VMS_Log今日檔不刪
		sprintf(TodayKeepLog, "VMS_Log\\%04d_%02d_%02d", systemTime.wYear, systemTime.wMonth, systemTime.wDay);
		if (strstr(szDir, TodayKeepLog) != NULL)	b_DelFile = false;

		//UI_Log今日檔不刪
		sprintf(TodayKeepLog, _T("UILog_%04d%02d%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay);
		if (strstr(szDir, TodayKeepLog) != NULL) b_DelFile = false;

		//統?csv今日檔不刪
		sprintf(TodayKeepLog, "%04d%02d%02d.csv", systemTime.wYear, systemTime.wMonth, systemTime.wDay);
		if (strstr(szDir, TodayKeepLog) != NULL)	b_DelFile = false;

		////本站DLL資料夾今日log不刪
		//sprintf(TodayKeepLog, "DLL\\%s\\%04d%02d%02d\\", g_str_station.c_str(), systemTime.wYear, systemTime.wMonth, systemTime.wDay);
		//if (strstr(szDir, TodayKeepLog) != NULL)	b_DelFile = false;

		//Android logcat今日檔不刪
		sprintf(TodayKeepLog, "Android\\%04d%02d%02d\\", systemTime.wYear, systemTime.wMonth, systemTime.wDay);
		if (strstr(szDir, TodayKeepLog) != NULL)	b_DelFile = false;

		//EntranceTool_Log今日檔不刪
		sprintf(TodayKeepLog, "EntranceTool_Log\\%04d_%02d_%02d", systemTime.wYear, systemTime.wMonth, systemTime.wDay);
		if (strstr(szDir, TodayKeepLog) != NULL)	b_DelFile = false;


		if (b_DelFile)
		{
			sprintf(szDir, "%s\\%s", str_LocalPath, str_TracePath);
			remove(szDir);
		}
		//TraceLog(MSG_INFO, szDir);
		FindClose(hList);
		return true;
	}
	char str_tempPath[1000];
	for (;;)
	{
		if (!FindNextFile(hList, &FileData))
		{
			if (GetLastError() == ERROR_NO_MORE_FILES)
			{
				FindClose(hList);
				return true;
			}
		}
		if (strcmp(FileData.cFileName, _T("..")) != 0)
		{
			strcpy(str_tempPath, str_TracePath);
			if (strcmp(str_tempPath, "") != 0)
				strcat(str_tempPath, "\\");
			strcat(str_tempPath, FileData.cFileName);
			if (!DFSDeleteFile(str_LocalPath, str_tempPath))
			{
				FindClose(hList);
				return false;
			}
		}
	}
}