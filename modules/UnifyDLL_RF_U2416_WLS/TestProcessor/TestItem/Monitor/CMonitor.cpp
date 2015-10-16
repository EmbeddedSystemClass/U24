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


bool CMonitor::Run()
{
	bool passFail = false;

	///*test 1*/
	//ErrMsg = "test1, id type = 1  id value =  ";
	//ErrMsg = ErrMsg 	+ g_strPicasso.c_str() ;
	//AfxMessageBox(ErrMsg.c_str());

	//runInsertData(1);
	//runCheckFlow( 1);

	///*test 2*/
	//char *sz_value = new char[ID_SIZE_BUFFER]  ;
	//passFail = runReadScalarID( sz_value, ID_SIZE);
	//ErrMsg = "test2, id type = 2  id value =  ";
	//ErrMsg = ErrMsg 	+ g_strScalarID.c_str() ;
	//AfxMessageBox(ErrMsg.c_str());

	//runInsertData(2);
	//runCheckFlow( 2);

	///*test 3*/
	//g_strPicasso = "1234567";
	//ErrMsg = "test 3 , id type = 1  id value =  ";
	//ErrMsg = ErrMsg 	+ g_strPicasso.c_str() ;
	//AfxMessageBox(ErrMsg.c_str());

	//runCheckFlow( 1);
	///*test 4*/
	//char *sz_value1 = new char[ID_SIZE_BUFFER]  ;
	//passFail = runReadScalarID( sz_value1, ID_SIZE);
	//g_strScalarID = "12345678911";
	//ErrMsg = "test4, id type = 2  id value =  ";
	//ErrMsg = ErrMsg 	+ g_strScalarID.c_str() ;
	//AfxMessageBox(ErrMsg.c_str());

	//runCheckFlow( 2);

//	AfxMessageBox(g_strPicasso.c_str());
	//runCheckFlow( 1); // ok, good
	//GetCurrentWeek(); //ok, good
	if (m_str_TestItem == ReadScalarID)
	{
		//m_strItemCode = CStr::IntToStr(Monitor_BaseItemcode);
		m_strErrorCode = FunErr_Check_PMid_Fail;
		//str_Pics = CW2A(L"Fine_Wifi_AP");
		char *sz_value = new char[ID_SIZE_BUFFER]  ;
		passFail = runReadScalarID( sz_value, ID_SIZE);
	}
	
	else if (m_str_TestItem == InsertData)
	{
		//m_strItemCode = CStr::IntToStr(Monitor_BaseItemcode);
		m_strErrorCode = FunErr_INSERT_DATA_Fail;
	//	char *sz_value = new char[ID_SIZE_BUFFER]  ;
	//	int i_id_type = CStr::StrToInt(m_str_CMD);
		int i_id_type = CStr::StrToInt(m_str_CMD);
		passFail = runInsertData( i_id_type);
	}
	else if (m_str_TestItem == CheckFlow)
	{
		//m_strItemCode = CStr::IntToStr(Monitor_BaseItemcode); 
		m_strErrorCode = FunErr_CHECK_FLOW_Fail;
		char *sz_value = new char[ID_SIZE_BUFFER]  ;
		int i_id_type = CStr::StrToInt(m_str_CMD);
		if ( i_id_type == 2) {
			char *sz_value = new char[ID_SIZE_BUFFER]  ;
			passFail = runReadScalarID( sz_value, ID_SIZE);
		}
		passFail = runCheckFlow( i_id_type);
	}
	else if (m_str_TestItem == CheckAllFlow)
	{
		//m_strItemCode = CStr::IntToStr(Monitor_BaseItemcode);
		m_strErrorCode = FunErr_CHECK_FLOW_Fail;
		char *sz_value = new char[ID_SIZE_BUFFER]  ;
		int i_id_type = CStr::StrToInt(m_str_CMD);
		if ( i_id_type == 2) {
			char *sz_value = new char[ID_SIZE_BUFFER]  ;
			passFail = runReadScalarID( sz_value, ID_SIZE);
		}
		passFail = runCheckFlow( i_id_type);
	}

	else if (m_str_TestItem == GetHDCPKEY)
	{
		//m_strItemCode = CStr::IntToStr(Monitor_BaseItemcode); 
		m_strErrorCode = FunErr_GET_HDCP_KEY_Fail;
		passFail = runGetHDCPKEY();
		//passFail = bGetNewHDCPKEY( sz_value);
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
		szScalarId = Id.GetId();
		g_strScalarID = szScalarId;
		if(szScalarId.empty() || szScalarId.length() != ID_SIZE)
		{	
			
			ErrMsg = "Fail to read ID, ID = ";
			AfxMessageBox( ErrMsg.c_str());
			TraceLog(MSG_INFO,  ErrMsg);
			b_res = false;
		}
		else
		{	
			ErrMsg = "Pass to read ID, ID = " + szScalarId;
			TraceLog(MSG_INFO,  ErrMsg);
			sprintf_s( szvalue , ID_SIZE_BUFFER, "%s", szScalarId.c_str());
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
	//	ErrMsg = "Single ID verify OK , ID = " +  szScalarId;
	//	TraceLog(MSG_INFO,  ErrMsg);
	//}
	//else{
	//	ErrMsg = "Fail duplicate ID , ID = " +  szScalarId;
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
		//SetPICSData(str_Pics, "PASS");
	}
	else
	{
	//	str_msg = CW2A(L"ReadScalarID ");
		m_strResult = "FAIL";
		m_strMessage = ErrMsg ;
		TraceLog(MSG_INFO, ErrMsg );
		//SetPICSData(str_Pics, "FAIL");
	}

	if (b_res)
	{
		FactoryLog(b_res, "", "", "", "", "" , m_strMessage, "" , "PASS");
	}
	else
	{
		FactoryLog(b_res, "", "", "", "", "" , m_strMessage, "" , "FAIL");
	}
	//FactoryLog();
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
			char* message = new char[nPipeSize];
			memset(message, 0, sizeof(message));
			::ReadFile(hRead, message, nPipeSize, &bytesRead, NULL);
			message[bytesRead] = '\0';

			strcpy(output, message);
			strcpy(ErrorCode, "Adb command ok");
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
				//stt =  stt + scalarID;
				//AfxMessageBox(stt.c_str());
				ErrMsg = "Load GetExistHDCPKEY TRUE  " ;
				ErrMsg = ErrMsg + "ID = " + scalarID;
				TraceLog(MSG_INFO,  ErrMsg);
			}
			else
			{

				ErrMsg = "Load GetExistHDCPKEY Falsel" ;
				ErrMsg = ErrMsg + "ID = " + scalarID;
			//	AfxMessageBox(ErrMsg.c_str());
				TraceLog(MSG_INFO,  ErrMsg);
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


bool CMonitor::runGetHDCPKEY()
{
	bool bRes = false;
//	char* szKEY = new char(30);
//	std::string stKEY;
	brunGetExistHDCPKEY("F1008B28887" );
	brunGetExistHDCPKEY("F1008B28888" );

	bGetNewHDCPKEY("F1008B28887" );
	bGetNewHDCPKEY("F1008B28888" );
	//stKEY = szKEY;
	//AfxMessageBox("Get F1008B28887");
	//AfxMessageBox( s_newKey.c_str());

//	stKEY = szKEY;
	//AfxMessageBox("Get F1008B28888");
//	AfxMessageBox( s_newKey.c_str());
	
	return bRes;
}
bool CMonitor::bGetNewHDCPKEY(char *scalarID)
{
	unsigned char szHDCPKEY[30] = "";

	int   i_scalarID = 30;
	int  i_HDCPKEY = 30;

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
				s_newKey = (char*)szHDCPKEY;
				//sz_NewKEY = (char*)szHDCPKEY;
				ErrMsg = _T("GetNewHDCPKEY OK");
				ErrMsg = ErrMsg +  "New Key = " + s_newKey ;
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
bool CMonitor::runCheckFlowAllStation( int i_type ){
	int i ;
	int i_limitStation = 0; 
//	g_str_station = "BBTEST";
//	runInsertData(i_type);
//	g_strPicasso = "1234567";
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
			if(bReturn) 
			{
				CString csInsertCmd;
				csInsertCmd.Format(_T("szModel = %s, sz_ID = %s, szOperator = %s , szStation = %s "), szModel,  sz_ID, szOperator, szStation);
				ErrMsg = "iInsertYrstation pass cmd = " + csInsertCmd;
		//		AfxMessageBox(ErrMsg.c_str());				
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
		//SetPICSData(str_Pics, "PASS");
	}
	else
	{
	//	str_msg = CW2A(L"ReadScalarID ");
		m_strResult = "FAIL";
		m_strMessage = ErrMsg ;
		TraceLog(MSG_INFO, ErrMsg );
		//SetPICSData(str_Pics, "FAIL");
	}

	if (b_res)
	{
		FactoryLog(b_res, "", "", "", "", "" , m_strMessage, "" , "PASS");
	}
	else
	{
		FactoryLog(b_res, "", "", "", "", "" , m_strMessage, "" , "FAIL");
	}

	//FactoryLog();
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
