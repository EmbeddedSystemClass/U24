/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 * Author     : Lion.Wu
 ********************************************************************************/

#include "stdafx.h"
#include "CMonitor.h"
#include <string> 

template <class T> 
std::string ConvertToString(T);

RF_IMPLEMENT_DYNCREATE(CMonitor)

template <class T> 
std::string ConvertToString(T value) {
  std::stringstream ss;
  ss << value;
  return ss.str();
}

bool CMonitor::PreRun()
{
	m_strResult = "-";
	m_strItemCode = "-";
	m_strErrorCode = "-";
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";
	return true;
}

bool CMonitor::Run()
{
	bool passFail = false;

	if (m_str_TestItem == ReadScalarID)
	{
		m_strItemCode = CStr::IntToStr(Monitor_BaseItemcode);
		m_strErrorCode = FunErr_Check_PMid_Fail;
		//str_Pics = CW2A(L"Fine_Wifi_AP");
		char *sz_value = new char[ID_SIZE_BUFFER]  ;
		passFail = runReadScalarID( sz_value, ID_SIZE);
		delete[] sz_value;
	}
	
	else if (m_str_TestItem == InsertData)
	{
		m_strItemCode = CStr::IntToStr(Monitor_BaseItemcode);
		m_strErrorCode = FunErr_INSERT_DATA_Fail;
	//	char *sz_value = new char[ID_SIZE_BUFFER]  ;
	//	int i_id_type = CStr::StrToInt(m_str_CMD);
		int i_id_type = CStr::StrToInt(m_str_CMD);
		char *sz_value = new char[ID_SIZE_BUFFER]  ;
		if ( i_id_type == 2) {
			passFail = runReadScalarID( sz_value, ID_SIZE);
		}
		delete[] sz_value;
		passFail = runInsertData( i_id_type);
	}
	else if (m_str_TestItem == CheckFlow)
	{
		m_strItemCode = CStr::IntToStr(Monitor_BaseItemcode);
		m_strErrorCode = FunErr_CHECK_FLOW_Fail;
		char *sz_value = new char[ID_SIZE_BUFFER]  ;
		int i_id_type = CStr::StrToInt(m_str_CMD);
		if ( i_id_type == 2) {
			if (passFail = runReadScalarID( sz_value, ID_SIZE)){//check by scalar id
				passFail = runCheckFlow( i_id_type);
			}
			else
			{
			//	return false;
			}
		}
		else//check by picasso(pcba id)
		{
			passFail = runCheckFlow( i_id_type);
		}
		delete[] sz_value;
		
	}
	else if (m_str_TestItem == CheckAllFlow)
	{
		m_strItemCode = CStr::IntToStr(Monitor_BaseItemcode);
		m_strErrorCode = FunErr_CHECK_FLOW_Fail;
		char *sz_value = new char[ID_SIZE_BUFFER]  ;
		int i_id_type = CStr::StrToInt(m_str_CMD);
		if ( i_id_type == 2) {
			passFail = runReadScalarID( sz_value, ID_SIZE);
		}
		delete[] sz_value;
		passFail = runCheckFlowAllStation( i_id_type);
	}
	else if (m_str_TestItem == GetHDCPKEY)
	{
		m_strItemCode = CStr::IntToStr(Monitor_BaseItemcode);
		m_strErrorCode = FunErr_GET_HDCP_KEY_Fail;
		passFail = runGetHDCPKEY();
		//passFail = bGetNewHDCPKEY( sz_value);
	}else if (m_str_TestItem == WriteHDCP) 
	{
		m_strItemCode = CStr::IntToStr(Monitor_BaseItemcode);
		m_strErrorCode = FunErr_GET_HDCP_KEY_Fail;
		passFail = runWriteHDCPKEY();
		//passFail = bGetNewHDCPKEY( sz_value);
	}
	
	else if (m_str_TestItem == CheckSWVersion)
	{
		m_strItemCode = CStr::IntToStr(Monitor_BaseItemcode);
		m_strErrorCode = FunErr_Check_SWVERSION_Fail;
		passFail = runCheckSWversion();
	}	
	else if (m_str_TestItem == CheckSWversionByDB)
	{
		m_strItemCode = CStr::IntToStr(Monitor_BaseItemcode);
		m_strErrorCode = FunErr_Check_SWVERSION_Fail;
		passFail = runCheckSWversionByDB();
	}	
	else if (m_str_TestItem == CheckModel)
	{
		m_strItemCode = CStr::IntToStr(Monitor_BaseItemcode);
		m_strErrorCode = FunErr_CHECK_MODEL_Fail;
		char *sz_value = new char[ID_SIZE_BUFFER]  ;
		runReadScalarID( sz_value, ID_SIZE);
		delete[] sz_value;
		passFail = runCheckModel();
	}	
	else if (m_str_TestItem == WriteTag)
	{
		m_strItemCode = CStr::IntToStr(Monitor_BaseItemcode);
		m_strErrorCode = FunErr_WRITE_TAG_Fail;
		passFail = runWriteTag();
	}	
	else if (m_str_TestItem == Precmd)
	{
		m_strItemCode = CStr::IntToStr(Monitor_BaseItemcode);
		m_strErrorCode = FunErr_PRE_CMD_Fail;
		passFail = runPreCmd();
	}
	else if (m_str_TestItem == Postcmd)
	{
		m_strItemCode = CStr::IntToStr(Monitor_BaseItemcode);
		m_strErrorCode = FunErr_POST_CMD_Fail;
		passFail = runPostCmd();
	}
	else if (m_str_TestItem == PlayVideo)
	{
		m_strItemCode = CStr::IntToStr(Monitor_BaseItemcode);
		m_strErrorCode = "-";
		passFail = runPlayVideo();
	}	
	
	else
	{
		m_strMessage = "The Type of ID is not defined";
		TraceLog(MSG_INFO, "The Type of ID is not defined");
		return false;
	}

	//}else	if (m_str_TestItem == CheckPCBAID)
	//{
	//	m_str_TestItem = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
	//	m_strErrorCode = FunErr_Check_PMid_Fail;
	//	//str_Pics = CW2A(L"Fine_Wifi_AP");
	//	char *sz_value = new char[ID_SIZE_BUFFER]  ;
	//	passFail = runCheckPCBAID( sz_value);
	//}

	return passFail;
}

bool CMonitor::PostRun()
{
	return true;
}

bool CMonitor::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find(std::string("XMLTestItem")) != paramMap.end())
	{
		m_str_TestItem = paramMap[std::string("XMLTestItem")];
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'TestItem' for class 'CMonitor'"); 
		return false;
	}

	std::string str_SleepTime;
	if (paramMap.find(std::string("XMLSleepTime")) != paramMap.end())
	{
		str_SleepTime = paramMap[std::string("XMLSleepTime")];
		m_i_SleepTime = CStr::StrToInt(str_SleepTime);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter OnSleepTime for CMonitor");
		return false;
	}

	if (paramMap.find(std::string("XMLCMDItem")) != paramMap.end())
	{
		m_str_CMD = paramMap[std::string("XMLCMDItem")]; 
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter XMLCMDItem for CMonitor");
		return false;
	}

	if (paramMap.find(std::string("XMLOffCMDItem")) != paramMap.end())
	{
		m_str_OffCMD = paramMap[std::string("XMLOffCMDItem")];
		checkStation = m_str_OffCMD;
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter XMLOffCMDItem for CMonitor");
		return false;
	}

	std::string str_MexRetryTime;
	if (paramMap.find(std::string("XMLRetryTime")) != paramMap.end())
	{
		str_MexRetryTime = paramMap[std::string("XMLRetryTime")];
		m_i_MexRetryTime = CStr::StrToInt(str_MexRetryTime);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter RetryTime for CMonitor");
		return false;
	}

	return true;
}


bool CMonitor::runReadScalarID( char *szvalue, int iSize )
{
	bool b_res = true;
	std::string ErrMsg;

	memset(szvalue,'\0',iSize);
	if(Id.ReadId())
	{	
		//Id.GetId();
		std_ScalarId = Id.GetId();
		g_strScalarID = std_ScalarId;
		if(std_ScalarId.empty() || std_ScalarId.length() != ID_SIZE)
		{	
			
			ErrMsg = "Fail to read ID, ID = ";//  + std_ScalarId.c_str();
			ErrMsg = ErrMsg + std_ScalarId.c_str();
			AfxMessageBox( ErrMsg.c_str());
			TraceLog(MSG_INFO,  ErrMsg);
			b_res = false;
		}
		else
		{	
			ErrMsg = "Pass to read ID, ID = ";//  + std_ScalarId.c_str();
			ErrMsg = ErrMsg + std_ScalarId.c_str();
		//	ErrMsg = "Pass to read ID, ID = " + std_ScalarId.c_str();
			TraceLog(MSG_INFO,  ErrMsg);
			sprintf_s( szvalue , ID_SIZE_BUFFER, "%s", std_ScalarId.c_str());
		}
	}
	else
	{
		ErrMsg = "Read ID fail.";
		AfxMessageBox( ErrMsg.c_str());
		TraceLog(MSG_INFO,  ErrMsg);
		b_res = false;
	}
	
//F1008B28888
	//sprintf_s( szvalue , 12, "%s", "F1008B28887" );
	//if  ( ! (IfRepeated( szvalue ) ) ){
	//	ErrMsg = "Single ID verify OK , ID = " +  std_ScalarId;
	//	TraceLog(MSG_INFO,  ErrMsg);
	//}
	//else{
	//	ErrMsg = "Fail duplicate ID , ID = " +  std_ScalarId;
	//	//AfxMessageBox("ID duplicate");
	//	b_res = false;
	//}
	

	/* test result */
	if(b_res)
	{
		str_msg = CW2A(L"ReadScalarID PASS");
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "ReadScalarID PASS!");
		SetPICSData(str_Pics, "PASS");
	}
	else
	{
	//	str_msg = CW2A(L"ReadScalarID ");
		m_strResult = "FAIL";
		m_strMessage = ErrMsg ;
		TraceLog(MSG_INFO, ErrMsg );
		SetPICSData(str_Pics, "FAIL");
	}
	
	FactoryLog();
	return b_res;
}



bool CMonitor::showMsg(const char* szMsg)
{
	/* Init dialog response flag to false */
	m_b_DialogResponse = false;
	m_b_waitFlag = false;

	/* Notify UI to show message dialog */
	ShowDialog(szMsg);

	/* Wait UI response(Max:10s) */
	int i_waitCount = 0;
	while (m_b_waitFlag != true)
	{
		if (i_waitCount == 50)
		{
			str_msg = "Wait UI response time out.";
			TraceLog(MSG_INFO, "Wait UI response time out.");
			break;
		}
		i_waitCount++;
		Sleep(200);
	}
	/* Get UI dialog operator response */
	if (m_b_DialogResponse == true)
	{
		str_msg = "Pressed PASS";
		TraceLog(MSG_INFO, str_msg);
	}
	else
	{
		str_msg = "Pressed FAIL";
		TraceLog(MSG_INFO, str_msg);
	}
	return m_b_DialogResponse;
}

bool CMonitor::ExecAdbOut(CString Command, char* output, char* ErrorCode)
{
	bool isOk = false;
	DWORD nPipeSize = 1024 * 1024; //1M pipeline

	CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));
	CString path_adb = pthToolDir + _T("\\adb.exe");
	if (_taccess(path_adb, 0) == -1)
	{
		strcpy(ErrorCode, "ERROR: No adb.exe exist!");
		return false;
	}

	HANDLE hRead, hWrite;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, nPipeSize))
	{
		strcpy(ErrorCode, "ERROR: CreatePipe fail!");
		return false;
	}

	//HANDLE hProcess = NULL;
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupInfo.wShowWindow	= SW_HIDE;
	startupInfo.hStdError = hWrite;
	startupInfo.hStdOutput = hWrite;

	Command = _T("\"") + path_adb + _T("\" ") + Command;
	TRACE(_T("Cmd: %s\n"), Command);
	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo))
	{
		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, 10 * 1000); // timeout in 10 seconds

		CloseHandle(hWrite);
		hWrite = NULL;
		//if timeout then exit the process
		if (TimeOutSignal == WAIT_TIMEOUT)
		{
			isOk = false;
			TerminateProcess(processInfo.hProcess, 0);
			strcpy(ErrorCode, "ERROR: Adb timeout");
		}
		else
		{

			isOk = true;
			DWORD bytesRead;
			std::string	std_out = "";
			std::string std_find_string = "error";
			char* message = new char[nPipeSize];
			memset(message, 0, sizeof(message));
			::ReadFile(hRead, message, nPipeSize, &bytesRead, NULL);
			message[bytesRead] = '\0';

			std_out = message;
			if ( std_out.find( std_find_string )  != string::npos) 
			{
				isOk = false;
				strcpy(ErrorCode, "ERROR: adb com fail!");
			}
			else
			{
				strcpy(output, message);
				strcpy(ErrorCode, "Adb command ok");
			}
			delete [] message;

		}
	}
	else
	{
		isOk = false;
		strcpy(ErrorCode, "ERROR: Execute adb.exe fail!");
	}
	Command.ReleaseBuffer();
	CloseHandle(hRead);
	if (hWrite)CloseHandle(hWrite);
	//CloseHandle(hProcess);

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	//hProcess = NULL;

	return isOk;
}


bool CMonitor::bCallAdbFastbootCMD(CString csAdbFastboot, CString Command, char* output, char* ErrorCode, CString cs_FindData)
{
	bool isOk = false;
	CString pthToolDir;

	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));
	CString path_adb_fastboot = pthToolDir + _T("\\") + csAdbFastboot;
	
	if (_taccess(path_adb_fastboot, 0) == -1)
	{
		strcpy(ErrorCode, "ERROR: No adb.exe exist!");
		return false;
	}

	HANDLE hRead, hWrite;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, BUFFER_SIZE))
	{
		strcpy(ErrorCode, "ERROR: CreatePipe fail!");
		return false;
	}

	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupInfo.wShowWindow	= SW_HIDE;
	startupInfo.hStdError = hWrite;
	startupInfo.hStdOutput = hWrite;

	Command = _T("\"") + path_adb_fastboot + _T("\" ") + Command;
	TRACE(_T("Cmd: %s\n"), Command);

	DWORD dwRead;
	CHAR chBuf[4096]; 
	bool bSuccess = FALSE;

	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo))
	{
		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, 100 * 1000); // timeout in 10 seconds

		CloseHandle(hWrite);
		hWrite = NULL;
		//if timeout then exit the process
		if (TimeOutSignal == WAIT_TIMEOUT)
		{
			isOk = false;
			TerminateProcess(processInfo.hProcess, 0);
			strcpy(ErrorCode, "ERROR: fastboot timeout");
		}
		else
		{
			if ( (cs_FindData.Find(DREAD) != -1) )// neet to read resolt
			{
				bSuccess = ReadFile( hRead, chBuf, BUFFER_SIZE, &dwRead, NULL);
				chBuf[dwRead] = '\0';
				strncpy(output, chBuf, dwRead);

				if ( bSuccess ) 
				{
					strcpy(ErrorCode, _T("Adb command ok"));
					isOk = true; //don't not need to check
				}
			}
			else if ( ( cs_FindData.Find(DNULL) == -1) )  //need to check return payload
			{
			   for (int i = 0; i < 60; i++) //get QPHONE
			   { 
				  bSuccess = ReadFile( hRead, chBuf, BUFFER_SIZE, &dwRead, NULL);
				  chBuf[dwRead] = '\0';
				  CString csBuf = chBuf;
				  if(csBuf.Find(cs_FindData) != -1) {
					  isOk = true; //get it
					  break;
				  }
				  if( ! bSuccess || dwRead == 0 ) break; 
			   }
			    strcpy(ErrorCode, _T("Adb command ok"));


			}
			else /*do not neet to find anything*/
			{

				isOk = true; //don't not need to check
			}
		}
	}
	else
	{
		isOk = false;
		strcpy(ErrorCode, "ERROR: Execute fastboot.exe fail!");
	}

	Command.ReleaseBuffer();
	if(hRead){
		CloseHandle(hRead);
		hRead = NULL;
	}
	if (hWrite){
		CloseHandle(hWrite);
		hWrite = NULL;
	}

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	processInfo.hProcess = NULL;
	processInfo.hThread = NULL;


	return isOk;
}

bool CMonitor::ExecFastbootOut(CString Command, char* output, char* ErrorCode)
{
	bool isOk = false;
	DWORD nPipeSize = 1024 * 1024; //1M pipeline

	CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));
	CString path_adb = pthToolDir + _T("\\fastboot.exe");
	if (_taccess(path_adb, 0) == -1)
	{
		strcpy(ErrorCode, "ERROR: No adb.exe exist!");
		return false;
	}

	HANDLE hRead, hWrite;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, nPipeSize))
	{
		strcpy(ErrorCode, "ERROR: CreatePipe fail!");
		return false;
	}

	//HANDLE hProcess = NULL;
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupInfo.wShowWindow	= SW_HIDE;
	startupInfo.hStdError = hWrite;
	startupInfo.hStdOutput = hWrite;

	Command = _T("\"") + path_adb + _T("\" ") + Command;
	TRACE(_T("Cmd: %s\n"), Command);
	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo))
	{
		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, 10 * 1000); // timeout in 10 seconds

		CloseHandle(hWrite);
		hWrite = NULL;
		//if timeout then exit the process
		if (TimeOutSignal == WAIT_TIMEOUT)
		{
			isOk = false;
			TerminateProcess(processInfo.hProcess, 0);
			strcpy(ErrorCode, "ERROR: Adb timeout");
		}
		else
		{

			isOk = true;
			DWORD bytesRead;
			std::string	std_out = "";
			std::string std_find_string = "error";
			char* message = new char[nPipeSize];
			memset(message, 0, sizeof(message));
			::ReadFile(hRead, message, nPipeSize, &bytesRead, NULL);
			message[bytesRead] = '\0';

			std_out = message;
			if ( std_out.find( std_find_string )  != string::npos) 
			{
				isOk = false;
				strcpy(ErrorCode, "ERROR: adb com fail!");
			}
			else
			{
				strcpy(output, message);
				strcpy(ErrorCode, "Adb command ok");
			}
			delete [] message;

		}
	}
	else
	{
		isOk = false;
		strcpy(ErrorCode, "ERROR: Execute adb.exe fail!");
	}
	Command.ReleaseBuffer();
	CloseHandle(hRead);
	if (hWrite)CloseHandle(hWrite);
	//CloseHandle(hProcess);

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	//hProcess = NULL;

	return isOk;
}


//bool _stdcall GetExistHDCPKEY(const unsigned char* id,      unsigned short IDLen,
//                                 unsigned char* Outkeysn, unsigned short keysnLen)

bool CMonitor::brunGetExistHDCPKEY(char *scalarID)
{
	unsigned char szHDCPKEY[30] = "";

	int   i_scalarID = 30;
	int  i_HDCPKEY = 30;

	//std::string stid = scalarID;
	//AfxMessageBox(stid.c_str());

	CString str_dllF32SERVER2 = F32SERVERDB;
	HINSTANCE hDll;
	hDll = ::LoadLibrary(str_dllF32SERVER2);
	if(hDll != NULL)
	{	
		typedef bool (_stdcall *lpGetExistHDCPKEY)(unsigned char*, unsigned short ,
	                                        unsigned char*,unsigned short);
		lpGetExistHDCPKEY GetExistHDCPKEY;
		GetExistHDCPKEY = (lpGetExistHDCPKEY)::GetProcAddress(hDll,"GetExistHDCPKEY");
		if (GetExistHDCPKEY != NULL)
		{	
		//	std::string stt = " GetExistHDCPKEY TRUE " ;
			
			if (GetExistHDCPKEY( (unsigned char*)scalarID, i_scalarID, szHDCPKEY, i_HDCPKEY) ){
				std_Key_Id = (char*)szHDCPKEY;
				//stt =  stt + scalarID;
				//AfxMessageBox(stt.c_str());
				ErrMsg = "GetExistHDCPKEY TRUE  " ;
				ErrMsg = ErrMsg + "ID = " + scalarID;
				TraceLog(MSG_INFO,  ErrMsg);
				return true;
			}
			else
			{
				ErrMsg = "GetExistHDCPKEY False, key not exist" ;
				ErrMsg = ErrMsg + "ID = " + scalarID;
			//	AfxMessageBox(ErrMsg.c_str());
				TraceLog(MSG_INFO,  ErrMsg);
				return false;
			}

			return true;
		}
		else
		{	
			ErrMsg = "Load GetExistHDCPKEY fail" ;
			AfxMessageBox(ErrMsg.c_str());
			TraceLog(MSG_INFO,  ErrMsg);
			FreeLibrary(hDll);
			return false;
		}
	}
	else
	{
		ErrMsg = "Load f32server2.dll fai" ;
		AfxMessageBox(ErrMsg.c_str());
		TraceLog(MSG_INFO,  ErrMsg);
		return false;
	}
}

bool CMonitor::runCheckSWversionByDB()
{
	bool bRes = false;;
	//std::string st_readId = "";
	char sz_cmd_in[FTD_BUF_SIZE] ="";
	char sz_cmd_out[FTD_BUF_SIZE] ="";
	char sz_cmd_errcode[FTD_BUF_SIZE] ="";
	char *sz_value = new char[ID_SIZE_BUFFER]  ;

	if ( ! runReadScalarID( sz_value, ID_SIZE) ) return false; //read monitor id
	if (!GetPartNo()) return false; //get partNo by id
	if (!GetModelByPartNo()) return false;

	std::string std_SoftWareVersion = "";
	CString cs_SoftWareVersion= "";
	//CString cs_xmlModelNamel = "";

	memset(sz_cmd_in, 0, sizeof(sz_cmd_in));
	memset(sz_cmd_out, 0, sizeof(sz_cmd_out));
	memset(sz_cmd_errcode, 0, sizeof(sz_cmd_errcode));


	if ( m_ModelName.length() < 1 ){
			ErrMsg = (_T("runCheckSWversionByDB ModelName  Fail, Model = ")) + m_ModelName;
			AfxMessageBox( ErrMsg.c_str() );
			TraceLog(MSG_INFO,  ErrMsg);
			goto Exit_ShowResult;
	}

	cs_DBModelNamel = m_ModelName.c_str();
	cs_DBModelNamel.Trim();

	if (!changeModel()) return false;
	if (!GetSWVersionFromDB()) return false;	

	strcpy(sz_cmd_in, _T("shell getprop ro.build.oemversion.main"));
	if ( !ExecAdbOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("adb shell getprop ro.build.oemversion.main fail"));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}	
	//Sleep(200);
	std_SoftWareVersion = (char*)sz_cmd_out;
	cs_SoftWareVersion.Format(_T("%s"),  sz_cmd_out);
	cs_SoftWareVersion.Trim();

	//cs_xmlModelNamel = m_str_CMD.c_str();
	//cs_xmlModelNamel.Trim();

	if (cs_SoftWareVersion.Find(m_szSWver.c_str()) == -1 ){
		CString cs;
		cs.Format(_T("Check SoftWare Version Fail，DB Version = %s  Current DL Version = %s"), m_szSWver.c_str(),  cs_SoftWareVersion);
		::MessageBox(NULL, cs.GetBuffer(0), _T("Warnning!!"), MB_TASKMODAL|MB_TOPMOST);
		ErrMsg = cs;
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}
	else
	{
		CString cs;
		cs.Format(_T("Check SoftWare Version pass，DB Version = %s  Current DL Version = %s"), m_szSWver.c_str(),  cs_SoftWareVersion);
		ErrMsg = cs;
		TraceLog(MSG_INFO,  ErrMsg);
		bRes = true;
	}


//[ro.build.variant]: [dels2317w] gbrob2a
//[ro.build.variant]: [delu2417w] gbrob1a

	//compare ,	m_str_CMD, sz_cmd_out

	
Exit_ShowResult:
	if ( !bRes) {
		m_strResult = "FAIL";
	}
	else
	{
		m_strErrorCode = "-";
		m_strResult = "PASS";
	}

	delete[] sz_value;
	str_msg = ErrMsg;
	m_strMessage = str_msg;
	FactoryLog();
	return bRes;
}

bool CMonitor::GetSWVersionFromDB(void)
{    
     bool bReturn = false;

     char szModel[ID_SIZE_BUFFER] = "";
     char szSWVersion[30] = "";
     char szErrCode[128] = "";
     char szErrMsg[512] = "";
     char szPartNoInfo[128] = "";
	 char m_sz9JPartNo[31] ="";//9J.2VM72.DLU
	 char m_szImagerVer[31] = "";


     CString str_dllVMS_DBACCESS = VMS_DBACCESS;
     HMODULE hDll = ::LoadLibrary(str_dllVMS_DBACCESS);
     if(hDll == NULL)
     {    
		 goto Exit_FreeLibrary;
     }

     typedef bool (*fpVMS_ConnectDB)(void);
     typedef bool (*fpVMS_CloseDB)(void);
     fpVMS_CloseDB iVMS_CloseDB = (fpVMS_CloseDB)::GetProcAddress (hDll,"VMS_CloseDB");
     if(iVMS_CloseDB == NULL)
     {    //没有VMS_ConnectDB
		ErrMsg = "iVMS_CloseDB = NULL" ;
		AfxMessageBox(ErrMsg.c_str());
         goto Exit_FreeLibrary;
     }
     //char PartNo9J[31] = "9J.2EE12.33E";

     fpVMS_ConnectDB iVMS_ConnectDB = (fpVMS_ConnectDB)::GetProcAddress(hDll,"VMS_ConnectDB");
     if (iVMS_ConnectDB != NULL)
     {  
		 if(iVMS_ConnectDB())
         {    
			 typedef bool (*fpVMS_QuerySQNinfo_S3)(char *szModel, char *szPartNo, char *szSWVersion, char *st_PartnoInfo, char *szErrCode, char *szErrMsg);
              fpVMS_QuerySQNinfo_S3 iVMS_QuerySQNinfo_S3 = (fpVMS_QuerySQNinfo_S3)::GetProcAddress(hDll,"VMS_QuerySQNinfo_S3");
              if (iVMS_QuerySQNinfo_S3 != NULL )
              {  

				 if ( cs_modelName_cmonitor.GetLength() < 1 ) {
						ErrMsg = "fail  m_str_modelName  = 0 ";
						ErrMsg = ErrMsg + cs_modelName_cmonitor.GetBuffer(0);
						AfxMessageBox(ErrMsg.c_str());
						cs_modelName_cmonitor.ReleaseBuffer();
						goto Exit_FreeLibrary;
				 }

				 if(m_szPartNo.empty() || m_szPartNo.length() < 1){
						ErrMsg = "fail  m_szPartNo  =  " + m_szPartNo;
						//ErrMsg = ErrMsg + m_szPartNo;
						AfxMessageBox(ErrMsg.c_str());
						goto Exit_FreeLibrary;
				 }

				 	//AfxMessageBox(m_szPartNo.c_str());
				  sprintf_s(szModel, ID_SIZE_BUFFER, "%s", cs_modelName_cmonitor);
				  sprintf_s(m_sz9JPartNo, ID_SIZE_BUFFER, "%s", m_szPartNo.c_str());

				  bool bRef = iVMS_QuerySQNinfo_S3(szModel, m_sz9JPartNo, m_szImagerVer, szPartNoInfo, szErrCode, szErrMsg);
                   if(bRef)
                   {  
					   m_szSWver = m_szImagerVer;
						ErrMsg = "iVMS_QuerySQNinfo_S3  ok, m_szSWver = " + m_szSWver;
						//SetErrorMessage(ErrMsg.c_str() , 0);
                       bReturn = true;
                   }
                   else
                   {    //查询失败
						ErrMsg = "iVMS_QuerySQNinfo_S3 fail " ;//+ std_ScalarId;
						AfxMessageBox(ErrMsg.c_str());
				//		sprintf_s( szvalue , ID_SIZE_BUFFER, "%s", std_ScalarId.c_str());
                       goto Exit_CloseDB;
                   }
			  }
              else
              {    //没有VMS_QuerySQNinfo
					ErrMsg = "iVMS_QuerySQNinfo_S3 NULL fail " ;
					AfxMessageBox(ErrMsg.c_str());
                   goto Exit_CloseDB;
			  }
		 }
         else
         {    //链接VMS数据失败
			  ErrMsg = "iVMS_ConnectDB  fail " ;
			  AfxMessageBox(ErrMsg.c_str());
              goto Exit_FreeLibrary;
         }
     }
     else
     {    //没有VMS_ConnectDB
		  ErrMsg = "iVMS_ConnectDB  NULL " ;
		  AfxMessageBox(ErrMsg.c_str());
         goto Exit_FreeLibrary;
     }

Exit_CloseDB:
     iVMS_CloseDB();
Exit_FreeLibrary:
	// SetErrorMessage(ErrMsg.c_str() , 0);
	 TraceLog(MSG_INFO,  ErrMsg);
     FreeLibrary(hDll);
Exit:
     return bReturn;
}


bool CMonitor::runCheckSWversion()
{
	bool bRes = false;;
	std::string st_readId = "";
	char sz_cmd_in[FTD_BUF_SIZE] ="";
	char sz_cmd_out[FTD_BUF_SIZE] ="";
	char sz_cmd_errcode[FTD_BUF_SIZE] ="";
	std::string std_ModelNamel = "";
	CString cs_getModelNamel = "";
	CString cs_xmlModelNamel = "";

	memset(sz_cmd_in, 0, sizeof(sz_cmd_in));
	memset(sz_cmd_out, 0, sizeof(sz_cmd_out));
	memset(sz_cmd_errcode, 0, sizeof(sz_cmd_errcode));


	strcpy(sz_cmd_in, _T("shell getprop ro.build.oemversion.main"));
	if ( !ExecAdbOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("adb shell getprop ro.build.oemversion.main fail"));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}	
	//Sleep(200);
	std_ModelNamel = (char*)sz_cmd_out;
	cs_getModelNamel.Format(_T("%s"),  sz_cmd_out);
	cs_getModelNamel.Trim();
	cs_xmlModelNamel = m_str_CMD.c_str();
	cs_xmlModelNamel.Trim();

	if (cs_xmlModelNamel.Compare( cs_getModelNamel ) == 0 )
	{
		ErrMsg = _T("runCheckSWversion check  ok  , get data = ") + std_ModelNamel; 
		bRes = true;
	}
	else
	{
		ErrMsg = _T("runCheckSWversion check  Fail xml setting = ") + m_str_CMD + _T(" get data = ") + std_ModelNamel; 
		TraceLog(MSG_INFO,  ErrMsg);
	}

//[ro.build.variant]: [dels2317w] gbrob2a
//[ro.build.variant]: [delu2417w] gbrob1a

	//compare ,	m_str_CMD, sz_cmd_out

	
Exit_ShowResult:
	if ( !bRes) {
		m_strResult = "FAIL";
	}
	else
	{
		m_strErrorCode = "-";
		m_strResult = "PASS";
	}


	str_msg = ErrMsg;
	m_strMessage = str_msg;
	FactoryLog();
	return bRes;
}

bool CMonitor::runCheckModel()
{
	bool bRes = false;
	//std::string st_readId = "";
	std::string	st_XMLSetting = "";
	char sz_cmd_in[FTD_BUF_SIZE] ="";
	char sz_cmd_out[FTD_BUF_SIZE] ="";
	char sz_cmd_errcode[FTD_BUF_SIZE] ="";
	std::string std_ModelNamel = "";
	CString cs_getModelNamel = "";
	CString cs_DBModelNamel = "";
	CString cs_XMLModelNamel = "";

	memset(sz_cmd_in, 0, sizeof(sz_cmd_in));
	memset(sz_cmd_out, 0, sizeof(sz_cmd_out));
	memset(sz_cmd_errcode, 0, sizeof(sz_cmd_errcode));

	if (!GetPartNo()) return false;
	if (!GetModelByPartNo()) return false;

	if ( m_ModelName.length() < 1 ){
			ErrMsg = (_T("ModelName  Fail, Model = ")) + m_ModelName;
			goto Exit_ShowResult;
	}

	strcpy(sz_cmd_in, _T("shell getprop ro.build.variant"));
	if ( !ExecAdbOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("shell getprop ro.build.variant Fail"));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}	
	//Sleep(200);
	std_ModelNamel = (char*)sz_cmd_out;
	cs_getModelNamel.Format(_T("%s"),  sz_cmd_out);
	cs_getModelNamel.Trim();

	cs_DBModelNamel = m_ModelName.c_str();
	cs_DBModelNamel.Trim();

	//cs_DBModelNamel = "S2317HWi"; //liontest
	//image setting delu2417w, dels2317w
	//db setting U2417HWi , S2317HWi
	if (cs_DBModelNamel.Compare( _T("U2417HWi") ) == 0 )
	{
		cs_XMLModelNamel = m_str_CMD.c_str();
		st_XMLSetting = m_str_CMD;
	}
	else 	if (cs_DBModelNamel.Compare( _T("S2317HWi") ) == 0 )
	{
		cs_XMLModelNamel = m_str_OffCMD.c_str();
		st_XMLSetting = m_str_OffCMD;
	}
	else
	{
		ErrMsg = (_T("cant find cs_DBModelNamel  = "));
		ErrMsg  = ErrMsg + m_ModelName;
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}

	cs_XMLModelNamel.Trim();
	if (cs_XMLModelNamel.Compare( cs_getModelNamel ) == 0 )
	{
		ErrMsg = _T("runCheckModel check  ok  , get data = ") + std_ModelNamel; 
		bRes = true;
	}
	else
	{
		ErrMsg = _T("runCheckModel check  Fail DB setting = ") + m_ModelName + _T(" get data = ") + std_ModelNamel + _T(" XMLSetting = ")  + st_XMLSetting;
		TraceLog(MSG_INFO,  ErrMsg);
	}

//[ro.build.variant]: [dels2317w] gbrob2a
//[ro.build.variant]: [delu2417w] gbrob1a

	//compare ,	m_str_CMD, sz_cmd_out

	
Exit_ShowResult:
	if ( !bRes) {
		m_strResult = "FAIL";
	}
	else
	{
		m_strErrorCode = "-";
		m_strResult = "PASS";
	}


	str_msg = ErrMsg;
	m_strMessage = str_msg;
	FactoryLog();
	return bRes;
}

bool CMonitor::GetPartNo()
{
	bool bReturn = false;
	unsigned char szPartNo[27] = {0};
	CString str_dllF32SERVER2 = F32SERVERDB;

	if(std_ScalarId.empty() || std_ScalarId.length() != ID_SIZE){
	  //  m_szPartNo =  (char*) szPartNo;
		ErrMsg = "GetPartNo ScalarId fail id  = " + std_ScalarId;
		goto Exit_FreeLibrary;
	}

	HMODULE hDll ;
	hDll = ::LoadLibrary(str_dllF32SERVER2);

	if( hDll != NULL )
	{	
		typedef unsigned short (_stdcall *lpGetPartNoById)(const unsigned char* Id,unsigned short IdLen,unsigned char* PartNo,unsigned short PartNoLen);
		lpGetPartNoById iGetPartNoById = (lpGetPartNoById)::GetProcAddress(hDll,"GetPartNoById");
		if ( NULL != iGetPartNoById )
		{	
			//scalar id
			unsigned char sz_ID[ID_SIZE_BUFFER] ="";
			sprintf_s((char*)sz_ID, ID_SIZE_BUFFER,"%s", std_ScalarId.c_str() );

			if( 0 != iGetPartNoById( sz_ID , 11, szPartNo, 13))
			{	
				//cout<<szPartNo<<endl;
			    m_szPartNo =  (char*) szPartNo;
				ErrMsg = "iGetPartNoById  ok, m_szPartNo = " + m_szPartNo;
				ErrMsg = ErrMsg + " ScalarId = " +  std_ScalarId ;
				bReturn = true;
			}
			else
			{	
				ErrMsg = "GetPartNo.  iGetPartNoById Fail ";
				AfxMessageBox(ErrMsg.c_str());
				goto Exit_FreeLibrary;
			}
		}
		else
		{
				ErrMsg = "GetPartNo. Load str_dllF32SERVER2 Fail ";
				AfxMessageBox(ErrMsg.c_str());
				goto Exit_FreeLibrary;
		}
	}
	else
	{
			ErrMsg = "GetPartNo. iGetPartNoById NULL ";
			AfxMessageBox(ErrMsg.c_str());
			goto Exit_FreeLibrary;
	}

Exit_FreeLibrary:
	TraceLog(MSG_INFO,  ErrMsg);
    FreeLibrary(hDll);

	return bReturn;
}

bool CMonitor::GetModelByPartNo()
{
	bool bReturn = false;
	CString str_dllF32SERVER2 = F32SERVERDB;

	HMODULE hDll ;
	hDll = ::LoadLibrary(str_dllF32SERVER2);

	if( hDll != NULL )
	{	
		typedef void (_stdcall *lpGetMonitorInfoByPartNo)(const unsigned char* PartNo,    unsigned short PartNoLen,
                                                  unsigned char* WBCFileName, unsigned short WBCFileNameLen,
                                                  unsigned char* ModelName,   unsigned short ModelNameLen,
                                                  unsigned char* DDCFileName, unsigned short DDCFileNameLen,
                                                  unsigned char* Info,        unsigned short InfoLen,
                                                  unsigned char* SWInfo,      unsigned short SWInfoLen,
                                                  unsigned char* Port,        unsigned short PortLen
                                                  );
         lpGetMonitorInfoByPartNo iGetMonitorInfoByPartNo = (lpGetMonitorInfoByPartNo)::GetProcAddress(hDll,"GetMonitorInfoByPartNo");

		//unsigned char sz_ID[ID_SIZE_BUFFER] ="";
	//	unsigned char szStation[ID_SIZE_BUFFER] ="";

         if(NULL != iGetMonitorInfoByPartNo)
         {    
			 unsigned char szPartNo[]="9j.2vm72.dlu";
              unsigned char szWbcFileName[30]= {0};
              unsigned char szModelName[30] = {0};
              unsigned char szDdcFileName[30] = {0};
              unsigned char szInfo[30] = {0};
              unsigned char szSwInfo[30] = {0};
              unsigned char szPort[30] = {0};

			 sprintf_s((char*)szPartNo, 13,"%s", m_szPartNo.c_str() );

			 iGetMonitorInfoByPartNo(szPartNo,13,szWbcFileName,30,szModelName,30,szDdcFileName,30,szInfo,30,szSwInfo,30,szPort,30);

			m_ModelName =  (char*) szModelName;

			if ( m_ModelName.empty() || m_ModelName.length() <1 ){
				ErrMsg = "iGetMonitorInfoByPartNo  fail, m_ModelName = " + m_ModelName;
				ErrMsg = ErrMsg + " szScalarId = " +  std_ScalarId ;
				goto Exit_FreeLibrary;
			}
			else
			{
				ErrMsg = "iGetMonitorInfoByPartNo  ok, m_ModelName = " + m_ModelName;
				ErrMsg = ErrMsg + " std_ScalarId = " +  std_ScalarId ;
				bReturn = true;
			}
		}
		else
		{
			ErrMsg = ("GetModelByPartNo Load iGetMonitorInfoByPartNo Fail");
			AfxMessageBox( ErrMsg.c_str() );
			goto Exit_FreeLibrary;
		 }
	}
	else
	{
			ErrMsg = ("GetModelByPartNo Load str_dllF32SERVER2 Fail");
			AfxMessageBox( ErrMsg.c_str() );
			goto Exit_FreeLibrary;
	}

Exit_FreeLibrary:
	TraceLog(MSG_INFO,  ErrMsg);
    FreeLibrary(hDll);

	return bReturn;
}

bool CMonitor::changeModel(){
//	CString cs_DBModelNamel = "";
//	cs_DBModelNamel = m_ModelName.c_str();
	cs_DBModelNamel.Trim();

	if (cs_DBModelNamel.Compare( _T("U2417HWi") ) == 0 )
	{
		cs_modelName_cmonitor = _T("GBROB1A");
		ErrMsg = (_T("set  m_str_modelName  = "));
	//	ErrMsg  = ErrMsg + m_str_modelName;
	//	st_XMLSetting = m_str_CMD;
	}
	else 	if (cs_DBModelNamel.Compare( _T("S2317HWi") ) == 0 )
	{
		cs_modelName_cmonitor = _T("GBROB2A");
		ErrMsg = (_T("set  m_str_modelName  = GBROB2A"));
	//	ErrMsg  = ErrMsg + m_str_modelName;
	//	st_XMLSetting = m_str_OffCMD;
	}
	else
	{
		ErrMsg = (_T("cant find cs_DBModelNamel  = "));
		ErrMsg  = ErrMsg + m_ModelName;
		AfxMessageBox( ErrMsg.c_str() );
		return false;
	}

	TraceLog(MSG_INFO,  ErrMsg);
	return true;
}


/*old math for xml setting*/
//bool CMonitor::runCheckModel()
//{
//	bool bRes = false;;
//	std::string st_readId = "";
//	char sz_cmd_in[FTD_BUF_SIZE] ="";
//	char sz_cmd_out[FTD_BUF_SIZE] ="";
//	char sz_cmd_errcode[FTD_BUF_SIZE] ="";
//	std::string std_ModelNamel = "";
//	CString cs_getModelNamel = "";
//	CString cs_xmlModelNamel = "";
//
//	memset(sz_cmd_in, 0, sizeof(sz_cmd_in));
//	memset(sz_cmd_out, 0, sizeof(sz_cmd_out));
//	memset(sz_cmd_errcode, 0, sizeof(sz_cmd_errcode));
//
//
//	strcpy(sz_cmd_in, _T("shell getprop ro.build.variant"));
//	if ( !ExecAdbOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
//		ErrMsg = (_T("shell getprop ro.build.variant Fail"));
//		AfxMessageBox( ErrMsg.c_str() );
//		TraceLog(MSG_INFO,  ErrMsg);
//		goto  Exit_ShowResult;
//	}	
//	//Sleep(200);
//	std_ModelNamel = (char*)sz_cmd_out;
//	cs_getModelNamel.Format(_T("%s"),  sz_cmd_out);
//	cs_getModelNamel.Trim();
//	cs_xmlModelNamel = m_str_CMD.c_str();
//	cs_xmlModelNamel.Trim();
//	//sprintf
//	//std_ModelNamel = "qisda";
//	if (cs_xmlModelNamel.Compare( cs_getModelNamel ) == 0 )
//	{
//		ErrMsg = _T("runCheckModel check  ok  , get data = ") + std_ModelNamel; 
//		bRes = true;
//	}
//	else
//	{
//		ErrMsg = _T("runCheckModel check  Fail xml setting = ") + m_str_CMD + _T(" get data = ") + std_ModelNamel; 
//		TraceLog(MSG_INFO,  ErrMsg);
//	}
//
////[ro.build.variant]: [dels2317w] gbrob2a
////[ro.build.variant]: [delu2417w] gbrob1a
//
//	//compare ,	m_str_CMD, sz_cmd_out
//
//	
//Exit_ShowResult:
//	if ( !bRes) {
//		m_strResult = "FAIL";
//	}
//	else
//	{
//		m_strErrorCode = "-";
//		m_strResult = "PASS";
//	}
//
//
//	str_msg = ErrMsg;
//	m_strMessage = str_msg;
//	FactoryLog();
//	return bRes;
//}

bool CMonitor::runPlayVideo() 
{
	CString cs_write_cmd = "";

	bool bRes = true;
	char sz_cmd_in[FTD_BUF_SIZE] ="";
	char sz_cmd_out[FTD_BUF_SIZE] ="";
	char sz_cmd_errcode[FTD_BUF_SIZE] ="";

	memset(sz_cmd_in, 0, sizeof(sz_cmd_in));
	memset(sz_cmd_out, 0, sizeof(sz_cmd_out));
	memset(sz_cmd_errcode, 0, sizeof(sz_cmd_errcode));

	//adb shell touch /data/data/com.oem.runin/skipCheckMMI

	strcpy(sz_cmd_in, _T("shell stop ftd"));
	if ( !ExecAdbOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("shell stop ftd Fail"));
	//	AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}	
	Sleep(m_i_SleepTime);

	strcpy(sz_cmd_in, _T("shell start"));
	if ( !ExecAdbOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("shell start"));
	//	AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}	
	Sleep(m_i_SleepTime);

	strcpy(sz_cmd_in, _T("shell  input keyevent 26"));
	if ( !ExecAdbOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("shell  input keyevent 26 Fail"));
	//	AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}	
	Sleep(m_i_SleepTime);

	strcpy(sz_cmd_in, _T("shell am start -n com.oem.runin/com.oem.runin.TesterVideo")); 
	if ( !ExecAdbOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("shell am start -n com.oem.runin/com.oem.runin.TesterVideo fail"));
	//	AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}	
	Sleep(m_i_SleepTime);


	
Exit_ShowResult:
	if ( !bRes) {
		m_strResult = "FAIL";
	}
	else
	{
		m_strErrorCode = "-";
		m_strResult = "PASS";
	}


	str_msg = ErrMsg;
	m_strMessage = str_msg;
	FactoryLog();
	return true;
}

bool CMonitor::runPreCmd()
{
	CString cs_write_cmd = "";

	bool bRes = true;
	std::string st_readId = "";
	char sz_cmd_in[FTD_BUF_SIZE] ="";
	char sz_cmd_out[FTD_BUF_SIZE] ="";
	char sz_cmd_errcode[FTD_BUF_SIZE] ="";

	bool b_wait_fastboot = false;
	int nLimitTime = 0 ;
	CString csFastboot("fastboot");

	memset(sz_cmd_in, 0, sizeof(sz_cmd_in));
	memset(sz_cmd_out, 0, sizeof(sz_cmd_out));
	memset(sz_cmd_errcode, 0, sizeof(sz_cmd_errcode));


	strcpy(sz_cmd_in, _T("reboot bootloader"));
	if ( !ExecAdbOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("reboot bootloader Fail"));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}	
	Sleep(3000);


	while ( !b_wait_fastboot){
		if (bCallAdbFastbootCMD(_T("fastboot.exe"), _T("devices"), sz_cmd_out, sz_cmd_errcode, csFastboot) ){
			b_wait_fastboot = true;
			ErrMsg = (_T("Get Fastboot Success"));
			//AfxMessageBox( ErrMsg.c_str() );
			TraceLog(MSG_INFO,  ErrMsg);
		//	AddMsg("Get Fastboot Success.", None, 10);
		}
		Sleep(1000);
		nLimitTime ++;
		if ( nLimitTime > 60 ) break;
	}
	
	if ( ! b_wait_fastboot ) {
		ErrMsg = (_T("reboot to fastboot fail "));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}

	strcpy(sz_cmd_in, _T("flash passport passport_FactoryDLTool"));
	if ( !ExecFastbootOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("flash passport passport_FactoryDLTool"));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}	
	Sleep(1000);

	strcpy(sz_cmd_in, _T("oem adb Qon"));
	if ( !ExecFastbootOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("oem adb Qon"));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}	
	Sleep(1000);


	strcpy(sz_cmd_in, _T("oem root Qon"));
	if ( !ExecFastbootOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("oem root Qon"));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}	
	Sleep(1000);

	strcpy(sz_cmd_in, _T("oem permissive Qon"));
	if ( !ExecFastbootOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("oem permissive Qon"));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}	
	Sleep(1000);

	strcpy(sz_cmd_in, _T("oem ftd Qon"));
	if ( !ExecFastbootOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("oem ftd Qon"));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}	
	Sleep(1000);

	strcpy(sz_cmd_in, _T("reboot"));
	if ( !ExecFastbootOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("reboot"));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}	
	Sleep(5000);
Exit_ShowResult:
	if ( !bRes) {
		m_strResult = "FAIL";
	}
	else
	{
		ErrMsg = (_T("PreCmd  Success"));
		m_strErrorCode = "-";
		m_strResult = "PASS";
	}


	str_msg = ErrMsg;
	m_strMessage = str_msg;
	FactoryLog();
	return bRes;
}


bool CMonitor::runPostCmd()
{
	CString cs_write_cmd = "";

	bool bRes = true;
	std::string st_readId = "";
	char sz_cmd_in[FTD_BUF_SIZE] ="";
	char sz_cmd_out[FTD_BUF_SIZE] ="";
	char sz_cmd_errcode[FTD_BUF_SIZE] ="";

	bool b_wait_fastboot = false;
	int nLimitTime = 0 ;
	CString csFastboot("fastboot");

	memset(sz_cmd_in, 0, sizeof(sz_cmd_in));
	memset(sz_cmd_out, 0, sizeof(sz_cmd_out));
	memset(sz_cmd_errcode, 0, sizeof(sz_cmd_errcode));


	strcpy(sz_cmd_in, _T("reboot bootloader"));
	if ( !ExecAdbOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("reboot bootloader Fail"));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}	
	Sleep(3000);


	while ( !b_wait_fastboot){
		if (bCallAdbFastbootCMD(_T("fastboot.exe"), _T("devices"), sz_cmd_out, sz_cmd_errcode, csFastboot) ){
			b_wait_fastboot = true;
			ErrMsg = (_T("Get Fastboot Success"));
			//AfxMessageBox( ErrMsg.c_str() );
			TraceLog(MSG_INFO,  ErrMsg);
		//	AddMsg("Get Fastboot Success.", None, 10);
		}
		Sleep(1000);
		nLimitTime ++;
		if ( nLimitTime > 60 ) break;
	}
	
	if ( ! b_wait_fastboot ) {
		ErrMsg = (_T("reboot to fastboot fail "));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}

	strcpy(sz_cmd_in, _T("flash passport passport_FactoryDLTool"));
	if ( !ExecFastbootOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("flash passport passport_FactoryDLTool"));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}	
	Sleep(1000);

	strcpy(sz_cmd_in, _T("oem ftd Qoff"));
	if ( !ExecFastbootOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("oem ftd Qoff"));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}	
	Sleep(1000);

	
Exit_ShowResult:
	if ( !bRes) {
		m_strResult = "FAIL";
	}
	else
	{
		m_strErrorCode = "-";
		m_strResult = "PASS";
	}


	str_msg = ErrMsg;
	m_strMessage = str_msg;
	FactoryLog();
	return bRes;
}

bool CMonitor::runWriteTag()
{
	bool bRes = false;
	std::string st_readId = "";
	char sz_ID[ID_SIZE_BUFFER] ="";
	char szAddress[FTD_BUF_SIZE] = "1056,8";// dell tag
	char m_szFAData[FTD_BUF_SIZE];
	memset(m_szFAData, 0, sizeof(m_szFAData));

//CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_FAC_CFGRead, "FTD_FAC_CFGRead")
//CPHONE_FTD_CMD_IN_OUT_CREATE(FTD_FAC_CFGWrite, "FTD_FAC_CFGWrite")
//	char sz_outBuffer[FTD_BUF_SIZE] = {0};
//	if (! m_pIPhone->FTD_CAMFlashLED(m_nFtdPort, m_nFtdTimeOut, "1", sz_outBuffer))
	
	if ( g_strTag.empty() ){
	//	AfxMessageBox("fail, tag is empty");
		ErrMsg = (_T("fail, tag is empty"));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		goto Exit_ShowResult;
	}

	sprintf_s((char*)sz_ID, ID_SIZE_BUFFER,"1056,8,%s", g_strTag.c_str() );

	if (!m_pIPhone->FTD_FAC_CFGWrite(m_nFtdPort, m_nFtdTimeOut, sz_ID, m_szFAData))
	{
			ErrMsg = (_T("FTD_FAC_CFGWrite Fail"));
			AfxMessageBox( ErrMsg.c_str() );
			TraceLog(MSG_INFO,  ErrMsg);
			goto Exit_ShowResult;
	}

	if (!m_pIPhone->FTD_FAC_CFGRead(m_nFtdPort, m_nFtdTimeOut, szAddress, m_szFAData))
	{
			ErrMsg = (_T("FTD_FAC_CFGRead Fail"));
			AfxMessageBox( ErrMsg.c_str() );
			TraceLog(MSG_INFO,  ErrMsg);
			goto Exit_ShowResult;
	}

	st_readId = m_szFAData;
	if  ( g_strTag.compare(st_readId) == 0 ){
			ErrMsg = (_T("tag compare  ok"));
			TraceLog(MSG_INFO,  ErrMsg);
			m_strErrorCode = "-";
			bRes = true;
	}
	else
	{
			ErrMsg = (_T("tag compare  Fail"));
			//AfxMessageBox( ErrMsg.c_str() );
			TraceLog(MSG_INFO,  ErrMsg);
			goto Exit_ShowResult;
	}

Exit_ShowResult:
	if ( !bRes) {
		m_strResult = "FAIL";
	}
	else
	{
		m_strErrorCode = "-";
		m_strResult = "PASS";
	}

	str_msg = ErrMsg;
	m_strMessage = str_msg;
	FactoryLog();
	return bRes;
}

bool CMonitor::runWriteHDCPKEY()
{
	CString cs_write_cmd = "";
	//FTD_HDCPKEY
	bool bRes = false;
	std::string st_readId = "";
	char sz_cmd_in[FTD_BUF_SIZE] ="";
	char sz_cmd_out[FTD_BUF_SIZE] ="";
	char sz_cmd_errcode[FTD_BUF_SIZE] ="";

	memset(sz_cmd_in, 0, sizeof(sz_cmd_in));
	memset(sz_cmd_out, 0, sizeof(sz_cmd_out));
	memset(sz_cmd_errcode, 0, sizeof(sz_cmd_errcode));

	if ( !(runGetHDCPKEY()) ){
		ErrMsg = (_T("runWriteHDCPKEY - runGetHDCPKEY Fail"));
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}


	strcpy(sz_cmd_in, _T("remount"));
	if ( !ExecAdbOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("remount Fail"));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}	
	
	strcpy(sz_cmd_in, _T("push cek.dat /cache/cek.dat"));
	if ( !ExecAdbOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("push cek.da Fail"));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}	

	cs_write_cmd.Format(_T("push %s /cache/pm.out"), cs_local_key_path);
	sprintf_s((char*)sz_cmd_in, MAX_PATH, "%s", cs_write_cmd);
	
	//strcpy(sz_cmd_in, _T("push 00001_PM.out  /cache/pm.out"));
	if ( !ExecAdbOut(sz_cmd_in, sz_cmd_out, sz_cmd_errcode) ){
		ErrMsg = (_T("push cek.dat Fail"));
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		goto  Exit_ShowResult;
	}	

	if (!(bRes = m_pIPhone->FTD_HDCPKEY(m_nFtdPort, m_nFtdTimeOut, sz_cmd_in, sz_cmd_out)))
	{
		ErrMsg = _T("Check FTD_HDCPKEY Fail");
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO, ErrMsg);
		goto  Exit_ShowResult;
	}
	else
	{
		ErrMsg = _T("Check FTD_HDCPKEY PASS");
		TraceLog(MSG_INFO, ErrMsg);
	}

	Sleep(200);
	/*update key , mac/wifi address*/

	if ( !(bRes = bUpdateKEYWrite() ))
	{
		ErrMsg = _T("bUpdateKEYWrite Fail");
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO, ErrMsg);
		goto  Exit_ShowResult;
	}
	else
	{
		ErrMsg = _T("bUpdateKEYWrite PASS");
		TraceLog(MSG_INFO, ErrMsg);
	}
	
Exit_ShowResult:
	if ( !bRes) {
		m_strResult = "FAIL";
	}
	else
	{
		m_strErrorCode = "-";
		m_strResult = "PASS";
	}


	str_msg = ErrMsg;
	m_strMessage = str_msg;
	FactoryLog();
	return bRes;
}
bool CMonitor::runGetHDCPKEY()
{
	bool bRes = false;
	char *sz_value = new char[ID_SIZE_BUFFER]  ;
	char sz_copy_cmd[MAX_PATH] = "";
	CString csKeyPath = "";
	CString pthToolDir = "";
	cs_local_key_path = "";
	
	sz_Hdcp_key_path.clear();
	std_Key_Name.clear();
	ZeroMemory(sz_local_file_path, sizeof(sz_local_file_path)/sizeof(char));
	ZeroMemory(sz_remote_file_path, sizeof(sz_remote_file_path)/sizeof(char));

	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = "open";
	ShExecInfo.lpFile = "cmd";
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;


	/*ok but skip*/
	if ( !(runReadScalarID( sz_value, ID_SIZE))) {
		ErrMsg = _T("runGetHDCPKEY fail");
		TraceLog(MSG_INFO,  ErrMsg);		
		goto Exit_ShowResult;
	}
	
//	std_ScalarId = _T("F1008B28887");

	sprintf_s((char*)sz_value, ID_SIZE_BUFFER, "%s", std_ScalarId.c_str());//station name (before this station)

	if (  (brunGetExistHDCPKEY( sz_value)) ){
		ErrMsg = _T("brunGetExistHDCPKEY  true, already exist  key");
		TraceLog(MSG_INFO,  ErrMsg);		 
	}
	else
	{
		/*get new key*/
		if ( bGetNewHDCPKEY(sz_value) ){
			ErrMsg = _T("bGetNewHDCPKEY  ok");
			TraceLog(MSG_INFO,  ErrMsg);			
		}
		else
		{
			/*get new key fail*/
			ErrMsg = _T("bGetNewHDCPKEY  fail");
			TraceLog(MSG_INFO,  ErrMsg);		
			goto Exit_ShowResult;
		}
	}

	/*access HDCP key*/

	std_Key_Name = std_Key_Id + _T("_PM.out");
	csKeyPath.Format(_T("\\\\f32server7\\HDCPWrite\\%s"), std_Key_Name.c_str());
	//csKeyPath.Format(_T("\\\\f32server7\\HDCPWrite\\%s_PM.out"), std_Key_Id.c_str());
	std_key_path = csKeyPath;

	if ( !checkFileExist(csKeyPath)){
		goto Exit_ShowResult;
	}


	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));

	/*check cek.dat file */
	cs_local_key_path = pthToolDir + _T("\\cek.dat");
	sprintf_s((char*)sz_local_file_path, MAX_PATH, "%s", cs_local_key_path);

	if ( !checkFileExist(cs_local_key_path)){
		goto Exit_ShowResult;
	}

	/*make folder*/
	cs_local_key_path = pthToolDir + _T("\\HDCPKEY\\");
	sprintf_s((char*)sz_local_file_path, MAX_PATH, "%s", cs_local_key_path);
	_mkdir(sz_local_file_path);


	/*remove key first */
	sprintf(sz_copy_cmd, "/C del %s /f /s /q /a", sz_local_file_path );
	ShExecInfo.lpParameters = sz_copy_cmd;
	ShellExecuteEx(&ShExecInfo);
	if (WaitForSingleObject(ShExecInfo.hProcess, 100000) == WAIT_TIMEOUT)
	{
		ErrMsg = _T("remove HDCPKEY folder fail");
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);		
		goto Exit_ShowResult;
	}

	/*copy key */
	cs_local_key_path = pthToolDir + _T("\\HDCPKEY\\") + std_Key_Name.c_str();

	sprintf_s((char*)sz_local_file_path, MAX_PATH, "%s", cs_local_key_path); //locate path
	sprintf_s((char*)sz_remote_file_path, MAX_PATH, "%s", csKeyPath); //remote path

	sprintf(sz_copy_cmd, "/C copy %s %s", sz_remote_file_path, sz_local_file_path );
	ShExecInfo.lpParameters = sz_copy_cmd;
	ShellExecuteEx(&ShExecInfo);
//	CString cs_local_key_path = pthToolDir + _T("\\HDCPKEY\\") + m_str_file.c_str();
	if (WaitForSingleObject(ShExecInfo.hProcess, 100000) == WAIT_TIMEOUT)
	{
		ErrMsg = _T("fail to copy hdcp key from F32SERVER7");
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);		
		goto Exit_ShowResult;
	}


	if ( !checkFileExist(sz_local_file_path)){
		goto Exit_ShowResult;
	}
	else bRes = true;

Exit_ShowResult:
	if ( !bRes) {
		m_strResult = "FAIL";
	}
	else
	{
		m_strErrorCode = "-";
		m_strResult = "PASS";
	}

	str_msg = ErrMsg.c_str();
	m_strMessage = str_msg;
	FactoryLog();
	delete[] sz_value;
	return bRes;


	brunGetExistHDCPKEY("F1008B28888" );

	bGetNewHDCPKEY("F1008B28887" );
	bGetNewHDCPKEY("F1008B28888" );
	//stKEY = szKEY;
	//AfxMessageBox("Get F1008B28887");
	//AfxMessageBox( std_Key_Id.c_str());

//	stKEY = szKEY;
	//AfxMessageBox("Get F1008B28888");
//	AfxMessageBox( std_Key_Id.c_str());
	
	//return bRes;
}
bool  CMonitor::checkFileExist(CString csFilePath){
	std::string std_filePath = csFilePath;
	if(access(csFilePath, 0) == 0)
	{
		ErrMsg = _T("access file ok, file path = ");
		ErrMsg = ErrMsg + std_filePath ;
		TraceLog(MSG_INFO,  ErrMsg);		
		return true;
	}
	else
	{
		ErrMsg = _T("can't not access file , key path = ");
		ErrMsg = ErrMsg  + std_filePath ;
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);		
		return false;
	}
}

bool CMonitor::bGetNewHDCPKEY(char *scalarID)
{
	unsigned char szHDCPKEY[30] = "";

	int   i_scalarID = 30;
	int  i_HDCPKEY = 30;
	std_Key_Name.clear();

//	std::string stid = scalarID;
	//AfxMessageBox(stid.c_str());

	CString str_dllF32SERVER2 = F32SERVERDB;
	HINSTANCE hDll;
	hDll = ::LoadLibrary(str_dllF32SERVER2);
	if(hDll != NULL)
	{	
		typedef bool (_stdcall *lpGetNewHDCPKEY)(unsigned char*, unsigned short ,
	                                        unsigned char*,unsigned short);
		lpGetNewHDCPKEY GetNewHDCPKEY;
		GetNewHDCPKEY = (lpGetNewHDCPKEY)::GetProcAddress(hDll,"GetNewHDCPKEY");
		if (GetNewHDCPKEY != NULL)
		{				
			if (GetNewHDCPKEY( (unsigned char*)scalarID, i_scalarID, szHDCPKEY, i_HDCPKEY) ){
				std_Key_Id = (char*)szHDCPKEY;
				ErrMsg = _T("GetNewHDCPKEY OK");
				ErrMsg = ErrMsg +  "New Key = " + std_Key_Id ;
				TraceLog(MSG_INFO,  ErrMsg);
				return true;
			}
			else
			{
				ErrMsg = " GetNewHDCPKEY fail " ;
				AfxMessageBox(ErrMsg.c_str());
				return false;
			}
		}
		else
		{				
			
			ErrMsg = "Load GetNewHDCPKEY Fail" ;
			AfxMessageBox(ErrMsg.c_str());
			TraceLog(MSG_INFO,  ErrMsg);
			FreeLibrary(hDll);
			return false;
		}
	}
	else
	{
		ErrMsg = ("Load str_dllF32SERVER2 Fail");
		AfxMessageBox( ErrMsg.c_str() );
		TraceLog(MSG_INFO,  ErrMsg);
		return false;
	}
}

bool CMonitor::bUpdateKEYWrite(){
	bool bRes = false;
		
	CString str_dllF32SERVER2 = F32SERVERDB;

	HMODULE hDll ;
	hDll = ::LoadLibrary(str_dllF32SERVER2);

	if( hDll != NULL )
	{	
		typedef bool (_stdcall *lpUpdateKEYWrite)(const unsigned char* Keysn,     unsigned short Keysnlen,
							  const unsigned char* Id,        unsigned short Idlen,
							  const unsigned char* Wireless,   unsigned short WirelessLen,
							  const unsigned char* BTwifi,      unsigned short BTwifiLen,
                              const unsigned char* ServiceTag,      unsigned short ServiceTagLen);

		lpUpdateKEYWrite iUpdateKEYWrite = (lpUpdateKEYWrite)::GetProcAddress(hDll,"UpdateKEYWrite");

		//unsigned char sz_ID[ID_SIZE_BUFFER] ="";
	//	unsigned char szStation[ID_SIZE_BUFFER] ="";
		
		if( iUpdateKEYWrite != NULL)
		{	
			/*get bt wifi mac*/
			char szInput[FTD_BUF_SIZE] = {0};
			unsigned char szBTOutput[FTD_BUF_SIZE] = {0};
			unsigned char szWifiOutput[FTD_BUF_SIZE] = {0};
			unsigned char szKeyID[ID_SIZE_BUFFER] = {0};
			unsigned char szScarlarId[ID_SIZE_BUFFER] = {0};
			unsigned char szServerId[ID_SIZE_BUFFER] = {0};
			if (!(m_pIPhone->FTD_BT_MAC(m_nFtdPort, 30000, szInput, (char*)szBTOutput)))
			{
				ErrMsg = "Read FTD_BT_MAC from mobile fail";
				AfxMessageBox( ErrMsg.c_str() );
				TraceLog(MSG_INFO, str_msg);
				goto Exit_ShowResult;
			}
			//sprintf(sz_copy_cmd, "/C copy %s %s", sz_remote_file_path, sz_local_file_path );

			if (!(m_pIPhone->FTD_WLAN_MAC(m_nFtdPort, 30000, szInput, (char*)szWifiOutput)))
			{
				ErrMsg = "Read FTD_WLAN_MAC from mobile fail";
				AfxMessageBox( ErrMsg.c_str() );
				TraceLog(MSG_INFO, str_msg);
				goto Exit_ShowResult;
			}

			//std_Key_Id
			sprintf_s((char*)szScarlarId , ID_SIZE_BUFFER, "%s", std_ScalarId.c_str());
			sprintf_s((char*)szKeyID, ID_SIZE_BUFFER, "%s", std_Key_Id.c_str());
			sprintf_s((char*)szServerId, ID_SIZE_BUFFER, "%s", g_strTag.c_str());

			CString iUpdateKEYWriteCmd;
			iUpdateKEYWriteCmd.Format(_T("szKeyID = %s, szScarlarId = %s, szWifiOutput = %s , szBTOutput = %s, szServerId = %s "), szKeyID,  szScarlarId, szWifiOutput, szBTOutput, szServerId);
			ErrMsg = "iUpdateKEYWrite cmd = " + iUpdateKEYWriteCmd;
			TraceLog(MSG_INFO,  ErrMsg);

			//sprintf(szKeyID, "/C copy %s %s", sz_remote_file_path, sz_local_file_path );
			if ( !iUpdateKEYWrite(szKeyID, ID_SIZE_BUFFER, szScarlarId, ID_SIZE_BUFFER, szWifiOutput, ID_SIZE_BUFFER, szBTOutput, ID_SIZE_BUFFER,szServerId ,10)){
				ErrMsg = ("iUpdateKEYWrite Fail");
				AfxMessageBox( ErrMsg.c_str() );
				TraceLog(MSG_INFO,  ErrMsg);
				goto Exit_ShowResult;
			}
			else
			{
				ErrMsg = ("iUpdateKEYWrite pass");
				TraceLog(MSG_INFO,  ErrMsg);
				bRes = true;
			}
		}
		else{
			ErrMsg = ("Load iUpdateKEYWrite Fail");
			AfxMessageBox( ErrMsg.c_str() );
			TraceLog(MSG_INFO,  ErrMsg);
			goto Exit_ShowResult;
		}
	}
	else
	{
			ErrMsg = ("Load str_dllF32SERVER2 Fail");
			AfxMessageBox( ErrMsg.c_str() );
			TraceLog(MSG_INFO,  ErrMsg);
			goto Exit_ShowResult;
	}
	

Exit_ShowResult:
	if ( !bRes) {
		m_strResult = "FAIL";
	}
	else
	{
		m_strErrorCode = "-";
		m_strResult = "PASS";
	}

	str_msg = ErrMsg.c_str();
	m_strMessage = str_msg;
	FactoryLog();
	return bRes;
}
bool CMonitor::runCheckFlowAllStation( int i_type ){
	int i ;
	int i_limitStation = 0; 
//	g_str_station = "BBTEST";
//	runInsertData(i_type);
	bool b_Res = false;
	for (i = 0 ; i < i_Station_Count ; i ++){
		if (st_Test_Flow[i].find( m_str_OffCMD.c_str())  != std::string::npos ){
			b_Res = true; 
			i_limitStation = i;
			break;
		}
	}

	if ( b_Res ){
			CString csInsertCmd;
			csInsertCmd.Format(_T("need to check , szStation = %s "), m_str_OffCMD.c_str());
			ErrMsg =  csInsertCmd;
			TraceLog(MSG_INFO,  ErrMsg);		
			//	ErrMsg = "can't find station , runCheckFlowAllStation";
				//AfxMessageBox(ErrMsg.c_str());
			//	TraceLog(MSG_INFO,  ErrMsg);		
		for (i = 0 ; i <= i_limitStation ; i ++){
			checkStation = st_Test_Flow[i];
			if  (!runCheckFlow(i_type)){
				b_Res = false;
				return false;
			}else{
				//CString csInsertCmd;
				//csInsertCmd.Format(_T(" check OK, Station = %s " ), checkStation);
				//ErrMsg =  csInsertCmd;
				//TraceLog(MSG_INFO,  ErrMsg);
			}
		}
	}else{
			ErrMsg = "can't find station , runCheckFlowAllStation";
			AfxMessageBox(ErrMsg.c_str());
			TraceLog(MSG_INFO,  ErrMsg);
			return false;		
	}

	return true;
}
bool CMonitor::runCheckFlow( int i_type)
{	
	bool bReturn = false;
	CString str_dllF32SERVER2 = F32SERVERDB;

	HMODULE hDll ;
	hDll = ::LoadLibrary(str_dllF32SERVER2);

	if( hDll != NULL )
	{	
		typedef bool (_stdcall *lpGetYrstationInfoByIdAndStation)( 
																unsigned short WeekCount,const unsigned char* szId,          
			                                                    unsigned short IdLen,const unsigned char* Station,       
																unsigned short StationLen);
		lpGetYrstationInfoByIdAndStation iGetYrstationInfoByIdAndStation = (lpGetYrstationInfoByIdAndStation)::GetProcAddress(hDll,"GetYrstationInfoByIdAndStation");

		unsigned char sz_ID[ID_SIZE_BUFFER] ="";
		unsigned char szStation[ID_SIZE_BUFFER] ="";
		
		if( iGetYrstationInfoByIdAndStation != NULL)
		{	

			sprintf_s((char*)szStation, ID_SIZE_BUFFER, "%s", checkStation.c_str());//station name (before this station)
			//sprintf_s((char*)szStation, ID_SIZE_BUFFER, "%s", g_str_station.c_str());

	
			if ( i_type == 1 ){ //arm board id
				sprintf_s((char*)sz_ID, ID_SIZE_BUFFER,"%s", g_strPicasso.c_str() );
				ErrMsg = " check flow1 = " ;
				ErrMsg =  ErrMsg + g_strPicasso.c_str() ;
				//AfxMessageBox(ErrMsg.c_str());
				TraceLog(MSG_INFO,  ErrMsg);
			}
			else if( i_type == 2 )
			{//scalar id
				sprintf_s((char*)sz_ID, ID_SIZE_BUFFER,"%s", g_strScalarID.c_str() );
				ErrMsg = " check flow2 = ";
				ErrMsg = ErrMsg 	+ g_strScalarID.c_str() ;
				//AfxMessageBox(ErrMsg.c_str());
				TraceLog(MSG_INFO,  ErrMsg);
			}
			else
			{
				ErrMsg = "can't find id type ";
				AfxMessageBox(ErrMsg.c_str());
				TraceLog(MSG_INFO,  ErrMsg);
				return false;
			}

			std::string sta =(char*) sz_ID;
			int i_week = GetCurrentWeek();

			CString csiGetYrstationInfoByIdAndStation;
			csiGetYrstationInfoByIdAndStation.Format(_T("i_week = %d, sz_ID = %s, szStation = %s "), i_week,  sz_ID, szStation);
			ErrMsg = "csiGetYrstationInfoByIdAndStation cmd = ";
			ErrMsg = ErrMsg + csiGetYrstationInfoByIdAndStation.GetBuffer(0);
			csiGetYrstationInfoByIdAndStation.ReleaseBuffer();
			TraceLog(MSG_INFO,  ErrMsg);

			if(  iGetYrstationInfoByIdAndStation( i_week, sz_ID, ID_SIZE_BUFFER, szStation, ID_SIZE_BUFFER) ) {
				csiGetYrstationInfoByIdAndStation.Format(_T("i_week = %d, sz_ID = %s, szStation = %s "), i_week,  sz_ID, szStation);
				ErrMsg = "csiGetYrstationInfoByIdAndStation pass";	
				TraceLog(MSG_INFO,  ErrMsg);
				bReturn =  true; //exist id 
			}
			else
			{

				ErrMsg = " iGetYrstationInfoByIdAndStation false = " + sta ;
				TraceLog(MSG_INFO,  ErrMsg);

				CString csMsg;
				csMsg.Format("%s 站无测试纪录，请回去重测, %s no test record, ", checkStation.c_str(), checkStation.c_str());
				ErrMsg = csMsg;// CW2A(csMsg.GetBuffer(0));
				TraceLog(MSG_INFO,  ErrMsg);

				showMsg(csMsg);
				//AfxMessageBox(ErrMsg.c_str());
			//	TraceLog(MSG_INFO,  ErrMsg);
				bReturn =  false;//id didn't exist
				return false;
			}

	//		unsigned char Id[15] = "F1008B28887";
	//		unsigned char szStation[20] = "MMI_EM";
	//		int i_week = GetCurrentWeek();
	//		std::string sta = (char*)Id;

	//		if(  iGetYrstationInfoByIdAndStation( i_week, Id, 15, szStation, 10) ) {
	//			ErrMsg = "Load iGetYrstationInfoByIdAndStation ok = " + sta ;
	//			//ErrMsg = ErrMsg + Id;
	//			AfxMessageBox(ErrMsg.c_str());						
	//		}
	//		else
	//		{
	//			
	//			ErrMsg = "Load iGetYrstationInfoByIdAndStation fail = " + sta ;
	//			//ErrMsg = ErrMsg + Id;
	//			AfxMessageBox(ErrMsg.c_str());				
	//		}

	//		unsigned char Id1[15] = "F1008B28888";
	//		sta = (char*)Id1;
	////		 Id[15] = "F1008B28888";
	//		if(  iGetYrstationInfoByIdAndStation( i_week, Id1, 15, szStation, 10) ) {
	//			ErrMsg = "Load iGetYrstationInfoByIdAndStation ok = " + sta ;
	//			//ErrMsg = ErrMsg + Id;
	//			AfxMessageBox(ErrMsg.c_str());						
	//		}
	//		else
	//		{
	//			
	//			ErrMsg = "Load iGetYrstationInfoByIdAndStation fail = " + sta ;
	//			//ErrMsg = ErrMsg + Id;
	//			AfxMessageBox(ErrMsg.c_str());				
	//		}

	//brunGetExistHDCPKEY("F1008B28887" );
	//brunGetExistHDCPKEY("F1008B28888" );

		}
		else
		{
			ErrMsg = ("Load GetYrstationInfoByIdAndStation Fail");
			AfxMessageBox( ErrMsg.c_str() );
			TraceLog(MSG_INFO,  ErrMsg);
			return false;
		}
	}
	else
	{
			ErrMsg = ("Load str_dllF32SERVER2 Fail");
			AfxMessageBox( ErrMsg.c_str() );
			TraceLog(MSG_INFO,  ErrMsg);
			return false;
	}
	return bReturn;
}
bool CMonitor::runInsertData(int i_type)
{	
	bool bReturn = false;
	CString str_dllF32SERVER2 = F32SERVERDB;

	HMODULE hDll ;
	hDll = ::LoadLibrary(str_dllF32SERVER2);

	if( hDll != NULL )
	{	
		typedef bool (_stdcall *lpInsertYrstation)(const unsigned char* Model,     unsigned short ModelLen,
				   		        				   const unsigned char* Id,        unsigned short IdLen,
												   const unsigned char* Shift,     unsigned short ShiftLen,
												   const unsigned char* Station,   unsigned short StationLen,
												   const unsigned char* Trid,      unsigned short TridLen,
												   const unsigned char* Operator,  unsigned short OperatorLen,
												   unsigned short CycleTime,
												   const unsigned char* CheckInfo, unsigned short CheckInfoLen);
		lpInsertYrstation iInsertYrstation = (lpInsertYrstation)::GetProcAddress(hDll,"InsertYrstation");
		if( iInsertYrstation != NULL)
		{	
			unsigned char sz_ID[ID_SIZE_BUFFER] ="";
			unsigned char szStation[ID_SIZE_BUFFER] ="";
			unsigned char szLine[5]="B21";

			sprintf_s((char*)szStation, ID_SIZE_BUFFER, "%s", g_str_station.c_str());

			unsigned char szModel[ID_SIZE_BUFFER]="";
			sprintf_s((char*)szModel, ID_SIZE_BUFFER, "%s", g_str_modelName.c_str());
			unsigned char szTrid[5]="";
			unsigned char szOperator[5]="";
			unsigned char szCheckInfo[5]="";

			//char szTemp[ID_SIZE_BUFFER];
			//sprintf_s(szTemp,ID_SIZE_BUFFER, "F1008B28888");
			//sprintf_s(szTemp,10,"%s",m_pIPhone->m_szSN.c_str());

		//	unsigned char Id[ID_SIZE_BUFFER] = {0};
			unsigned char Station[ID_SIZE_BUFFER]  = {0};
			if ( i_type == 1 ){ //arm board id
				if ( g_strPicasso.empty()){
					ErrMsg = "  picasso is empty = " ;
					ErrMsg =  ErrMsg + g_strPicasso.c_str() ;
					TraceLog(MSG_INFO,  ErrMsg);
					AfxMessageBox(ErrMsg.c_str());
					return false;
				}
				sprintf_s((char*)sz_ID, ID_SIZE_BUFFER,"%s", g_strPicasso.c_str() );

				ErrMsg = "  iInsertYrstation 1 = " ;
				ErrMsg =  ErrMsg + g_strPicasso.c_str() ;
			//	AfxMessageBox(ErrMsg.c_str());
				TraceLog(MSG_INFO,  ErrMsg);
			}
			else if( i_type == 2 )
			{//scalar id
				if ( g_strScalarID.empty()){
					ErrMsg = "  scalar board is empty = " ;
					ErrMsg =  ErrMsg + g_strScalarID.c_str() ;
					TraceLog(MSG_INFO,  ErrMsg);
					AfxMessageBox(ErrMsg.c_str());
					return false;
				}
				sprintf_s((char*)sz_ID, ID_SIZE_BUFFER,"%s", g_strScalarID.c_str() );
				ErrMsg = " iInsertYrstation 2 = ";
				ErrMsg = ErrMsg 	+ g_strScalarID.c_str() ;
			//	AfxMessageBox(ErrMsg.c_str());
				TraceLog(MSG_INFO,  ErrMsg);
			}
			else
			{
				ErrMsg = "can't find id type ";
				AfxMessageBox(ErrMsg.c_str());
				TraceLog(MSG_INFO,  ErrMsg);
				return false;
			}

			//memcpy(sz_ID,szTemp,ID_SIZE_BUFFER);
			bReturn = iInsertYrstation(szModel,   (unsigned short)strlen((char*)szModel),
									   sz_ID,	(unsigned short)strlen((char*)sz_ID),
									   szLine,    (unsigned short)strlen((char*)szLine), 
									   szStation, (unsigned short)strlen((char*)szStation),
									   szTrid,     5,
									   szOperator,   5,
									   0,
									   szCheckInfo,  5);
				CString csInsertCmd;
				csInsertCmd.Format(_T("szModel = %s, sz_ID = %s, szOperator = %s , szStation = %s "), szModel,  sz_ID, szOperator, szStation);
				ErrMsg = "iInsertYrstation cmd = " + csInsertCmd;
				TraceLog(MSG_INFO,  ErrMsg);

			if(bReturn) 
			{
			//	CString csInsertCmd;
			//	csInsertCmd.Format(_T("szModel = %s, sz_ID = %s, szOperator = %s , szStation = %s "), szModel,  sz_ID, szOperator, szStation);
				ErrMsg = "iInsertYrstation pass";
				TraceLog(MSG_INFO,  ErrMsg);
			}
			else
			{
				ErrMsg = "iInsertYrstation fail ";
				AfxMessageBox(ErrMsg.c_str());				
				TraceLog(MSG_INFO,  ErrMsg);
			}
			goto Exit_FreeLibrary;
		}
		else
		{	
			ErrMsg = "Load InsertYrstation fail ";
			AfxMessageBox(ErrMsg.c_str());				
			TraceLog(MSG_INFO,  ErrMsg);
			goto Exit_FreeLibrary;
		}
	}
	else
	{	
		ErrMsg = "Load str_dllF32SERVER2 fail ";
		AfxMessageBox(ErrMsg.c_str());				
		TraceLog(MSG_INFO,  ErrMsg);
		goto Exit;
	}
Exit_FreeLibrary:
	FreeLibrary(hDll);
Exit:
	return bReturn;
}



int CMonitor::GetWeek	(	struct tm* date	)
{
	if (NULL == date)
	{
		return 0; // or -1 or throw exception
	}
	if (::mktime(date) < 0) // Make sure _USE_32BIT_TIME_T is NOT defined.
	{
		return 0; // or -1 or throw exception
	}
	// The basic calculation:
	// {Day of Year (1 to 366) + 10 - Day of Week (Mon = 1 to Sun = 7)} / 7
	int monToSun = (date->tm_wday == 0) ? 7 : date->tm_wday; // Adjust zero indexed week day
	int week = ((date->tm_yday + 11 - monToSun) / 7); // Add 11 because yday is 0 to 365.

	// Now deal with special cases:
	// A) If calculated week is zero, then it is part of the last week of the previous year.
	if (week == 0)
	{
		// We need to find out if there are 53 weeks in previous year.
		// Unfortunately to do so we have to call mktime again to get the information we require.
		// Here we can use a slight cheat - reuse this function!
		// (This won't end up in a loop, because there's no way week will be zero again with these values).
		tm lastDay = { 0 };
		lastDay.tm_mday = 31;
		lastDay.tm_mon = 11;
		lastDay.tm_year = date->tm_year - 1;
		// We set time to sometime during the day (midday seems to make sense)
		// so that we don't get problems with daylight saving time.
		lastDay.tm_hour = 12;
		week = GetWeek(&lastDay);
	}
	// B) If calculated week is 53, then we need to determine if there really are 53 weeks in current year
	//    or if this is actually week one of the next year.
	else if (week == 53)
	{
		// We need to find out if there really are 53 weeks in this year,
		// There must be 53 weeks in the year if:
		// a) it ends on Thurs (year also starts on Thurs, or Wed on leap year).
		// b) it ends on Friday and starts on Thurs (a leap year).
		// In order not to call mktime again, we can work this out from what we already know!
		int lastDay = date->tm_wday + 31 - date->tm_mday;
		if (lastDay == 5) // Last day of the year is Friday
		{
			// How many days in the year?
			int daysInYear = date->tm_yday + 32 - date->tm_mday; // add 32 because yday is 0 to 365
			if (daysInYear < 366)
			{
				// If 365 days in year, then the year started on Friday
				// so there are only 52 weeks, and this is week one of next year.
				week = 1;
			}
		}
		else if (lastDay != 4) // Last day is NOT Thursday
		{
			// This must be the first week of next year
			week = 1;
		}
		// Otherwise we really have 53 weeks!
	}
	return week;
}

int CMonitor::GetCurrentWeek()
{
	tm today;
	time_t now;
	time(&now);
	errno_t error = ::localtime_s(&today, &now);
	//int a = GetWeek(&today);
	return GetWeek(&today);
}


/*no use
*
*
*
*


*/

bool CMonitor::runCheckPCBAID( char *szvalue )
{
	bool b_res = true;
	std::string szId ;
	//runInsertData(); lionttest
   // szId = "F1008B28888";
	sprintf_s( szvalue , 12, "%s", "F1008B28887" );
	szId = szvalue;
	if  ( ! (IfRepeated( szvalue ) ) ){
		ErrMsg = "Single ID verify OK , ID = " +  szId;
		AfxMessageBox(ErrMsg.c_str());
		TraceLog(MSG_INFO,  ErrMsg);
	}
	else{
		ErrMsg = "Fail duplicate ID , ID = " +  szId;
		AfxMessageBox(ErrMsg.c_str());
		//AfxMessageBox("ID duplicate");
		b_res = false;
	}
	
	sprintf_s( szvalue , 12, "%s", "F1008B28888" );
	szId = szvalue;
	if  ( ! (IfRepeated( szvalue ) ) ){
		ErrMsg = "Single ID verify OK , ID = " +  szId;
		TraceLog(MSG_INFO,  ErrMsg);
		AfxMessageBox(ErrMsg.c_str());
	}
	else{
		ErrMsg = "Fail duplicate ID , ID = " +  szId;
		AfxMessageBox(ErrMsg.c_str());
		//AfxMessageBox("ID duplicate");
		b_res = false;
	}
	

	/* test result */
	if(b_res)
	{
		str_msg = CW2A(L"ReadScalarID PASS");
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "ReadScalarID PASS!");
		SetPICSData(str_Pics, "PASS");
	}
	else
	{
	//	str_msg = CW2A(L"ReadScalarID ");
		m_strResult = "FAIL";
		m_strMessage = ErrMsg ;
		TraceLog(MSG_INFO, ErrMsg );
		SetPICSData(str_Pics, "FAIL");
	}
	
	FactoryLog();
	return b_res;
}



//bool CMonitor::IfRepeated(unsigned short IdType)
bool CMonitor::IfRepeated( char *szvalue )
{
	int IdType = 1; // id
	//AfxMessageBox (g_str_station.c_str());
	//AfxMessageBox (g_str_modelName.c_str());
	unsigned char Sn[ID_SIZE_BUFFER] = "";
	//AfxMessageBox("sprintf_s 1");
	sprintf_s((char*)Sn, ID_SIZE_BUFFER, "%s", szvalue);
	//AfxMessageBox("sprintf_s 2");
	std::string stid = szvalue;
	AfxMessageBox(stid.c_str());
	CString str_dllF32SERVER2 = F32SERVERDB;
	HINSTANCE hDll;
	hDll = ::LoadLibrary(str_dllF32SERVER2);
	if(hDll != NULL)
	{
		typedef bool (_stdcall *lpIfRepeatedId)(const unsigned short IdType, const unsigned char* InSn,      unsigned short SnLen);
		lpIfRepeatedId iIfRepeatedId = (lpIfRepeatedId)::GetProcAddress(hDll,"IfRepeatedId");
		if (iIfRepeatedId != NULL)
		{
			if(iIfRepeatedId(IdType,Sn,ID_SIZE))
			{	FreeLibrary(hDll);
				AfxMessageBox("get it duplicae");
				return true;
			}
			else
			{	FreeLibrary(hDll);
				AfxMessageBox("not get it, pass");
				return false;
			}
		}
		else
		{
			AfxMessageBox("can't get IfRepeatedId");
			FreeLibrary(hDll);
			return true;
		}
	}
	else
	{	
		AfxMessageBox("LoadLibrary(str_dllF32SERVER2 FAIL");
		return true;
	}
}
