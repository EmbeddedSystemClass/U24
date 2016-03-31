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

//	AfxMessageBox(g_strPicasso.c_str());
	//runCheckFlow( 1); // ok, good
	//GetCurrentWeek(); //ok, good
	if (m_str_TestItem == ReadScalarID)
	{
		//TraceLog = CStr::IntToStr(Monitor_BaseItemcode);
		//m_strErrorCode = FunErr_Check_PMid_Fail;
		//str_Pics = CW2A(L"Fine_Wifi_AP");
		char *sz_value = new char[ID_SIZE_BUFFER]  ;
		passFail = runReadScalarID( sz_value, ID_SIZE);
	}
	
	else if (m_str_TestItem == InsertData)
	{
		//TraceLog = CStr::IntToStr(Monitor_BaseItemcode);
		//m_strErrorCode = FunErr_INSERT_DATA_Fail;
	//	char *sz_value = new char[ID_SIZE_BUFFER]  ;
	//	int i_id_type = CStr::StrToInt(m_str_CMD);
		int i_id_type = CStr::StrToInt(m_str_CMD);
		passFail = runInsertData( i_id_type);
	}
	else if (m_str_TestItem == CheckFlow)
	{
		//TraceLog = CStr::IntToStr(Monitor_BaseItemcode);
		//m_strErrorCode = FunErr_CHECK_FLOW_Fail;
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
		//TraceLog = CStr::IntToStr(Monitor_BaseItemcode);
		//m_strErrorCode = FunErr_CHECK_FLOW_Fail;
		char *sz_value = new char[ID_SIZE_BUFFER]  ;
		int i_id_type = CStr::StrToInt(m_str_CMD);
		if ( i_id_type == 2) {
			char *sz_value = new char[ID_SIZE_BUFFER]  ;
			passFail = runReadScalarID( sz_value, ID_SIZE);
		}
		passFail = runCheckFlowAllStation( i_id_type);
	}
	else
	{
		//m_strMessage = "The Type of ID is not defined";
		//TraceLog(MSG_INFO, "The Type of ID is not defined");
		return false;
	}

	//}else	if (m_str_TestItem == CheckPCBAID)
	//{
	//	m_str_TestItem = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
	//	//m_strErrorCode = FunErr_Check_PMid_Fail;
	//	//str_Pics = CW2A(L"Fine_Wifi_AP");
	//	char *sz_value = new char[ID_SIZE_BUFFER]  ;
	//	passFail = runCheckPCBAID( sz_value);
	//}

	return passFail;

}



//bool CMonitor::ExecAdbOut(CString Command, char* output, char* ErrorCode)
//{
//	bool isOk = false;
//	DWORD nPipeSize = 1024 * 1024; //1M pipeline
//
//	CString pthToolDir;
//	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
//	pthToolDir.ReleaseBuffer();
//	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));
//	CString path_adb = pthToolDir + _T("\\adb.exe");
//	if (_taccess(path_adb, 0) == -1)
//	{
//		strcpy(ErrorCode, "ERROR: No adb.exe exist!");
//		return false;
//	}
//
//	HANDLE hRead, hWrite;
//	SECURITY_ATTRIBUTES sa;
//	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
//	sa.lpSecurityDescriptor = NULL;
//	sa.bInheritHandle = TRUE;
//	if (!CreatePipe(&hRead, &hWrite, &sa, nPipeSize))
//	{
//		strcpy(ErrorCode, "ERROR: CreatePipe fail!");
//		return false;
//	}
//
//	//HANDLE hProcess = NULL;
//	PROCESS_INFORMATION processInfo;
//	STARTUPINFO startupInfo;
//	::ZeroMemory(&startupInfo, sizeof(startupInfo));
//	startupInfo.cb = sizeof(startupInfo);
//	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
//	startupInfo.wShowWindow	= SW_HIDE;
//	startupInfo.hStdError = hWrite;
//	startupInfo.hStdOutput = hWrite;
//
//	Command = _T("\"") + path_adb + _T("\" ") + Command;
//	TRACE(_T("Cmd: %s\n"), Command);
//	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo))
//	{
//		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, 10 * 1000); // timeout in 10 seconds
//
//		CloseHandle(hWrite);
//		hWrite = NULL;
//		//if timeout then exit the process
//		if (TimeOutSignal == WAIT_TIMEOUT)
//		{
//			isOk = false;
//			TerminateProcess(processInfo.hProcess, 0);
//			strcpy(ErrorCode, "ERROR: Adb timeout");
//		}
//		else
//		{
//
//			isOk = true;
//			DWORD bytesRead;
//			std::string	std_out = "";
//			std::string std_find_string = "error";
//			char* message = new char[nPipeSize];
//			memset(message, 0, sizeof(message));
//			::ReadFile(hRead, message, nPipeSize, &bytesRead, NULL);
//			message[bytesRead] = '\0';
//
//			std_out = message;
//			if ( std_out.find( std_find_string )  != string::npos) 
//			{
//				isOk = false;
//				strcpy(ErrorCode, "ERROR: adb com fail!");
//			}
//			else
//			{
//				strcpy(output, message);
//				strcpy(ErrorCode, "Adb command ok");
//			}
//			delete [] message;
//
//		}
//	}
//	else
//	{
//		isOk = false;
//		strcpy(ErrorCode, "ERROR: Execute adb.exe fail!");
//	}
//	Command.ReleaseBuffer();
//	CloseHandle(hRead);
//	if (hWrite)CloseHandle(hWrite);
//	//CloseHandle(hProcess);
//
//	CloseHandle(processInfo.hProcess);
//	CloseHandle(processInfo.hThread);
//	//hProcess = NULL;
//
//	return isOk;
//}

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
			//sprintf_s((char*)szStation, ID_SIZE_BUFFER, "%s", csStation);

	
			if ( i_type == 1 ){ //arm board id
				sprintf_s((char*)sz_ID, ID_SIZE_BUFFER,"%s", g_strPicasso.c_str() );
				ErrMsg = " check flow1 = " ;
				ErrMsg =  ErrMsg + g_strPicasso.c_str() ;
				//AfxMessageBox(ErrMsg.c_str());
				//TraceLog(MSG_INFO,  ErrMsg);
			}
			else if( i_type == 2 )
			{//scalar id
				sprintf_s((char*)sz_ID, ID_SIZE_BUFFER,"%s", g_strScalarID.c_str() );
				ErrMsg = " check flow2 = ";
				ErrMsg = ErrMsg 	+ g_strScalarID.c_str() ;
				//AfxMessageBox(ErrMsg.c_str());
				//TraceLog(MSG_INFO,  ErrMsg);
			}
			else
			{
				ErrMsg = "can't find id type ";
				AfxMessageBox(ErrMsg.c_str());
				//TraceLog(MSG_INFO,  ErrMsg);
				return false;
			}

			std::string sta =(char*) sz_ID;
			int i_week = GetCurrentWeek();

			CString csiGetYrstationInfoByIdAndStation;
			csiGetYrstationInfoByIdAndStation.Format(_T("i_week = %d, sz_ID = %s, szStation = %s "), i_week,  sz_ID, szStation);
			ErrMsg = "csiGetYrstationInfoByIdAndStation cmd = ";
			ErrMsg = ErrMsg + csiGetYrstationInfoByIdAndStation.GetBuffer(0);
			csiGetYrstationInfoByIdAndStation.ReleaseBuffer();
			//TraceLog(MSG_INFO,  ErrMsg);

			if(  iGetYrstationInfoByIdAndStation( i_week, sz_ID, ID_SIZE_BUFFER, szStation, ID_SIZE_BUFFER) ) {
				csiGetYrstationInfoByIdAndStation.Format(_T("i_week = %d, sz_ID = %s, szStation = %s "), i_week,  sz_ID, szStation);
				ErrMsg = "csiGetYrstationInfoByIdAndStation pass";	
				//TraceLog(MSG_INFO,  ErrMsg);
				bReturn =  true; //exist id 
			}
			else
			{

				ErrMsg = " iGetYrstationInfoByIdAndStation false = " + sta ;
				//TraceLog(MSG_INFO,  ErrMsg);

				CString csMsg;
				csMsg.Format("%s 站无测试纪录，请回去重测, %s no test record, ", checkStation.c_str(), checkStation.c_str());
				ErrMsg = csMsg;// CW2A(csMsg.GetBuffer(0));
				//TraceLog(MSG_INFO,  ErrMsg);

				//showMsg(csMsg);
				//AfxMessageBox(ErrMsg.c_str());
			//	//TraceLog(MSG_INFO,  ErrMsg);
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
			//TraceLog(MSG_INFO,  ErrMsg);
			return false;
		}
	}
	else
	{
			ErrMsg = ("Load str_dllF32SERVER2 Fail");
			AfxMessageBox( ErrMsg.c_str() );
			//TraceLog(MSG_INFO,  ErrMsg);
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

			sprintf_s((char*)szStation, ID_SIZE_BUFFER, "%s", csStation);

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

				if ( g_strScalarID.empty()){
					ErrMsg = "  scalar board is empty = " ;
					ErrMsg =  ErrMsg + g_strScalarID.c_str() ;
					//TraceLog(MSG_INFO,  ErrMsg);
					AfxMessageBox(ErrMsg.c_str());
					return false;
				}
				sprintf_s((char*)sz_ID, ID_SIZE_BUFFER,"%s", g_strScalarID.c_str() );
				ErrMsg = " iInsertYrstation 2 = ";
				ErrMsg = ErrMsg 	+ g_strScalarID.c_str() ;
			//	AfxMessageBox(ErrMsg.c_str());
				//TraceLog(MSG_INFO,  ErrMsg);

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
				//TraceLog(MSG_INFO,  ErrMsg);

			if(bReturn) 
			{
			//	CString csInsertCmd;
			//	csInsertCmd.Format(_T("szModel = %s, sz_ID = %s, szOperator = %s , szStation = %s "), szModel,  sz_ID, szOperator, szStation);
				ErrMsg = "iInsertYrstation pass";
				//TraceLog(MSG_INFO,  ErrMsg);
			}
			else
			{
				ErrMsg = "iInsertYrstation fail ";
				AfxMessageBox(ErrMsg.c_str());				
				//TraceLog(MSG_INFO,  ErrMsg);
			}
			goto Exit_FreeLibrary;
		}
		else
		{	
			ErrMsg = "Load InsertYrstation fail ";
			AfxMessageBox(ErrMsg.c_str());				
			//TraceLog(MSG_INFO,  ErrMsg);
			goto Exit_FreeLibrary;
		}
	}
	else
	{	
		ErrMsg = "Load str_dllF32SERVER2 fail ";
		AfxMessageBox(ErrMsg.c_str());				
		//TraceLog(MSG_INFO,  ErrMsg);
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

