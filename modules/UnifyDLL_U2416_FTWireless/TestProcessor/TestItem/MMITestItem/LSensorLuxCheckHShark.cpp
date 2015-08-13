
#include "stdafx.h"
#include "LSensorLuxCheckHShark.h"
#include <SetupAPI.h>

#pragma comment(lib, "setupapi")
RF_IMPLEMENT_DYNCREATE(CSensorLuxCheckHShark)

CSensorLuxCheckHShark::CSensorLuxCheckHShark(){
	numberOfBytesRead = 0;

}

bool CSensorLuxCheckHShark::InitData(std::map<std::string, std::string>& paramMap)
{

	std::string str;


	// Shift value //
	std::string str_shift;
	if (paramMap.find(std::string("Shift")) != paramMap.end())
	{
		str_shift = paramMap[std::string("Shift")];
		m_i_shift = CStr::StrToInt(str_shift);
	}
	else
	{
		m_i_shift = 0;
		TraceLog(MSG_INFO, _T("Set Shift value by default 0."));
	}

	// Read Repeat times //
	std::string strRepeat;
	if (paramMap.find(std::string("Repeat")) != paramMap.end())
	{
		strRepeat = paramMap[std::string("Repeat")];
		m_i_repeat = CStr::StrToInt(strRepeat);
	}
	else
	{
		m_i_repeat = 1;
		TraceLog(MSG_INFO, _T("Set Repeat for 1 time by default."));
	}

	if (paramMap.find(std::string("RetryTimes")) != paramMap.end())
	{
		str = paramMap[std::string("RetryTimes")];
		m_i_retry = CStr::StrToInt(str);
	}
	else
	{
		m_i_retry = 10;
	}

	/* Init to 5s for wait Jig*/
	m_i_sleep_for_Jig = 5;

	if (paramMap.find(std::string("SleepTime")) != paramMap.end())
	{
		str = paramMap[std::string("SleepTime")];
		m_i_sleep_for_Jig = CStr::StrToInt(str);
	}
	else
	{
		TraceLog(MSG_INFO, _T("Set SleepTime for 5s by default."));
	}

	/*wait for read data*/
	m_i_sleep_for_Data = 5000;
	if (paramMap.find(std::string("ReadDataDelay")) != paramMap.end())
	{
		str = paramMap[std::string("ReadDataDelay")];
		m_i_sleep_for_Data = CStr::StrToInt(str);
	}
	else
	{
		TraceLog(MSG_INFO, _T("Set ReadDataDelay for 5s by default."));
	}


	
	if (paramMap.find(std::string("LuxVersion")) != paramMap.end())
	{
		strLuxVersion = paramMap[std::string("LuxVersion")];
	}
	else
	{
		TraceLog(MSG_INFO, "Set EnableJIG for 1 by default.");
	}

	if (paramMap.find(std::string("ChipVersion")) != paramMap.end())
	{
		strChipVersion = paramMap[std::string("ChipVersion")];
	}
	else
	{
		TraceLog(MSG_INFO, "Set EnableJIG for 1 by default.");
	}

	
	if (paramMap.find(std::string("TestItem")) != paramMap.end())
	{
		m_str_SensorLuxTestItem = paramMap[std::string("TestItem")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to get TestItem");
	}

	

	//Enable JIG or NO
	m_i_EnableJIG = 1;
	std::string strEnableJIG;
	if (paramMap.find(std::string("EnableJIG")) != paramMap.end())
	{
		strEnableJIG = paramMap[std::string("EnableJIG")];
		m_i_EnableJIG = CStr::StrToInt(strEnableJIG);
	}
	else
	{
		TraceLog(MSG_INFO, "Set EnableJIG for 1 by default.");
	}

	return true;
}

bool CSensorLuxCheckHShark::PreRun()
{
	m_strResult    = "";
	m_strItemCode  = CStr::IntToStr(LSensor_Calibration_BaseItemCode);
	m_strErrorCode = FunErr_LSensor_LuxCheck_Fail;
	m_strBand      = "-";
	m_strChannel   = "-";
	m_strUpper     = "";
	m_strLower     = "";
	m_strMeasured  = "";
	m_strUnit      = "-";
	m_strMessage   = "";

	return true;
}

bool CSensorLuxCheckHShark::Run()
{

	bool passFail = false;
	CString cs_SensorLuxTestItem = m_str_SensorLuxTestItem.c_str();

	if (DetectDevice() == false){
		m_strErrorCode = FunErr_LSensor_LuxCheck_Fail;
		m_strMessage = "DetectDevice Fail!";
		FactoryLog();
		return false;
	}

	if (cs_SensorLuxTestItem.Compare("LSensorLuxCheck") == 0)
	{
		passFail = runLuxCheckHShark();
	}
	else if (cs_SensorLuxTestItem.Compare("CloseLight") == 0)
	{
		passFail = runCloseLight();
	}

	closeHandle();

	return passFail;
}
bool  CSensorLuxCheckHShark::initialHandle()
{
	CString strPort;
	//strPort.Format( _T("\\\\.\\%s"), m_str_comport.c_str() );
	strPort.Format( _T("\\\\.\\COM%s"), str_PROLIFICcomPort.c_str() );

	//-- Init Comport --//
	if (!ConfigureCOMPort(strPort))
	{
		if (INVALID_HANDLE_VALUE != m_h_Com){
			CloseHandle(m_h_Com);
		}
			m_h_Com = NULL;
		FactoryLog();
		return false;
	}
	TraceLog(MSG_INFO, _T("ConfigureCOMPort PASS"));


	memset(&overlappedWrite, 0, sizeof(overlappedWrite));
	memset(&overlappedRead, 0, sizeof(overlappedRead));

	overlappedWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (overlappedWrite.hEvent == NULL)
	{
		TraceLog(MSG_INFO, _T("[runCloseLight]overlappedWrite :: CreateEvent fail"));
		CloseHandle(m_h_Com);
		m_h_Com = NULL;
		FactoryLog();
		return false;
	}

	overlappedRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (overlappedRead.hEvent == NULL)
	{
		TraceLog(MSG_INFO, _T("[runCloseLight]overlappedRead :: CreateEvent fail"));
		CloseHandle(m_h_Com);
		CloseHandle(overlappedWrite.hEvent);
		m_h_Com = NULL;
		FactoryLog();
		return false;
	}
	return true;
}

bool  CSensorLuxCheckHShark::closeHandle()
{
	if(m_h_Com){
		PurgeComm(m_h_Com, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
		CloseHandle(m_h_Com);
		m_h_Com = NULL;
		TraceLog(MSG_INFO, _T("Close HANDLE OK"));
	}

	if (overlappedRead.hEvent != NULL){
		CloseHandle(overlappedRead.hEvent);
		TraceLog(MSG_INFO, _T("Close OVERLAPPED Read OK"));
	}
	if (overlappedWrite.hEvent != NULL){
		CloseHandle(overlappedWrite.hEvent);
		TraceLog(MSG_INFO, _T("Close OVERLAPPED Write OK"));
	}
	return true;
}

bool CSensorLuxCheckHShark::runLuxCheckHShark()
{
	std::string m_str_Message;
	bool bSend = false;
	bool bRece = false;

	if (SendJigRequest("OFF") != true)
	{
		m_strResult = "FAIL";
		m_strErrorCode = CommErr_Jig_Move_Fail;
		m_strMessage = _T("Jig Request(OFF) Fail!");
		TraceLog(MSG_INFO, _T("Jig Request(OFF) Fail!"));
		FactoryLog();
		return false;
	}

	TraceLog(MSG_INFO, _T("Jig Request(OFF) Pass!"));
	Sleep(2000);

	initialHandle();

	bSend = sendCMD("V");

	if(bSend){
		bRece = receCMD(strLuxVersion.c_str());
	}else{
		TraceLog(MSG_INFO, _T("sendCMD Version fail"));
		return false;
	}
	
	TraceLog(MSG_INFO, _T("Adjustment Start"));
	if (bRece){
		bSend = sendCMD("B");//Start Adject
	}else{
		TraceLog(MSG_INFO, _T("receCMD V fail"));
		m_strMessage = "receCMD V fail !";
		FactoryLog();
		return false;
	}

	int iRetry = 0;

	if(bSend){
		while ( iRetry < m_i_retry ){
			bRece = receCMD("Locked");
			iRetry++;
			if (bRece){ 
				TraceLog(MSG_INFO, _T("Adjustment PASS"));
				break;
			}
		}
		if(iRetry >= m_i_retry){
			TraceLog(MSG_INFO, _T("can't find Locked"));
			m_strMessage = _T("can't find Locked");
			FactoryLog();
			return false;
		}
	}else{
		TraceLog(MSG_INFO, _T("sendCMD B fail"));
		return false;
	}

	receCMD("SaveLog");//saving log

	bSend = sendCMD("S");//Stop
	if (bSend){
		bRece = receCMD("S");
	}else{
		TraceLog(MSG_INFO, _T("receCMD S fail"));
		return false;
	}


	m_strErrorCode = "-";
	TraceLog(MSG_INFO, _T("runLuxCheckHShark PASS"));
	m_strMessage = _T("runLuxCheckHShark PASS");
	FactoryLog();

	return true;
}


bool CSensorLuxCheckHShark::runCloseLight()
{
	std::string m_str_Message;
	bool bSend = false;
	bool bRece = false;


	TraceLog(MSG_INFO, _T("Jig Request(OFF) Pass!"));
	Sleep(2000);

	initialHandle();

	bSend = sendCMD("R");//close light
	if (bSend){
		bRece = receCMD("R");//Start Adject
	}else{
		TraceLog(MSG_INFO, _T("runCloseLight - bSend R fail"));
		return false;
	}


	if ( bRece == false){
		TraceLog(MSG_INFO, _T("runCloseLight - bRece R fail"));
		return false;
	}


	TraceLog(MSG_INFO, _T("runCloseLight PASS"));
	m_strMessage = _T("runCloseLight PASS");
	FactoryLog();

	return true;
}
bool CSensorLuxCheckHShark::PostRun()
{
	return true;
}


bool CSensorLuxCheckHShark::sendCMD(char *sz_cmd)
{
	// Set write data //
	char c_sendBuffer[10]="";
	DWORD dw_writeLen;

	CString CS_buffer;
	int iRetry = 0;
	int iFindPos = 0;
	bool bLocked = false;
	bool wRes = false;

	
	DWORD dwRes;
	strcpy(c_sendBuffer, sz_cmd);
	dw_writeLen = strlen(c_sendBuffer)*sizeof(char);

	PurgeComm(m_h_Com, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	if(!WriteFile(m_h_Com, c_sendBuffer, dw_writeLen, &dw_writeLen, &overlappedWrite))
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			// WriteFile failed, but isn't delayed. Report error and abort.
			TraceLog(MSG_INFO, _T("[sendCMD] :: GetLastError() != ERROR_IO_PENDING"));
            wRes = true;
		}
		else
		{
			// Write is pending.
			dwRes = WaitForSingleObject(overlappedWrite.hEvent, m_i_sleep_for_Data);
			switch(dwRes)
			{
				case WAIT_OBJECT_0:
					 if (!GetOverlappedResult(m_h_Com, &overlappedWrite, &dw_writeLen, FALSE))
					 {
						 TraceLog(MSG_INFO, _T("[sendCMD] :GetOverlappedResult return false"));
						 wRes = false;
					 }
					 else
					 {
					  // Write operation completed successfully.
						 TraceLog(MSG_INFO, _T("[sendCMD] WaitForSingleObject  PASS"));
						 wRes = true;
					 }
					 break;
				default:
					 // An error has occurred in WaitForSingleObject.
					 // This usually indicates a problem with the
					// OVERLAPPED structure's event handle.
					TraceLog(MSG_INFO, _T("[sendCMD] :: fail"));
					wRes = false;
					break;
			}
		}
	}
	else
	{
		wRes = true;
		TraceLog(MSG_INFO, _T("[sendCMD] ::  PASS"));
	}; /*end of if(!WriteFile)*/


	if (wRes == false){
		closeHandle();
		FactoryLog();
		return false;
	}


	return wRes;
}
bool CSensorLuxCheckHShark::receCMD(const char *sz_cmd)
{
	bool dwRes = false;
	char temp_buf[bufferSize];
	iFindPos = 0;
	CString CS_buffer;

	memset(temp_buf, 0, bufferSize);
	if(!ReadFile(m_h_Com, temp_buf, bufferSize, &numberOfBytesRead, &overlappedRead))
	{
		//Sleep(m_i_sleep_for_Data);
		if (GetLastError() != ERROR_IO_PENDING)
		{
			// ReadFile failed, but isn't delayed. Report error and abort.
			TraceLog(MSG_INFO, _T("1.Read Locked :: GetLastError() != ERROR_IO_PENDING"));
		}
		else
		{
		   dwRes = WaitForSingleObject(overlappedRead.hEvent, m_i_sleep_for_Data);
		   switch(dwRes)
		   {
			  // Read completed.
			  case WAIT_OBJECT_0:
				   // Error in communications; report it. 
				  if (!GetOverlappedResult(m_h_Com, &overlappedRead, &numberOfBytesRead, FALSE))
				  {
					  TraceLog(MSG_INFO, _T("1.Read Locked :: GetLastError() != ERROR_IO_PENDING"));
				  }
				  else
				  {
				     	int nBufferLength = 0 ;
					    nBufferLength = strlen(temp_buf);;
						if ( nBufferLength != 0 ){/*make sure data not empty*/

							TraceLog(MSG_INFO, _T("1. receCMD GetLastError() PASS"));
							TraceLog(MSG_INFO, temp_buf);

							CS_buffer = temp_buf;
							//if (-1 != (iFindPos = CS_buffer.Find("Locked")))
							if(strcmp(sz_cmd,"SaveLog")==0) {//saving log
								int iLux = 0;
								int iTotal = 0;
								int iAverage = 0;
								int iCount = 0;
								bool bRes = false;
								bool wRes = false;
								int nBufferLength = 0 ;
								std::string m_str_Message;

								nBufferLength = strlen(temp_buf);;
								if ( nBufferLength != 0 ){/*make sure data not empty*/
									StrVtr vToken;
									CStr::ParseString(temp_buf, _T("x"), vToken);
									CS_buffer.Format(_T("%s"), vToken[0].c_str());
									if (-1 != (iFindPos = CS_buffer.Find("L")))
									{
										sscanf(CS_buffer.Left(iFindPos), "%d", &iLux);
										iTotal += (iLux + m_i_shift);
										iCount++;
										sprintf_s(szTmp, 255, "%0d - Lux : %d", iCount, iLux);
										sprintf_s(szTmp, 255, "Lux : %d", iLux);
										m_str_Message = szTmp;
										TraceLog(MSG_INFO, m_str_Message);

										//Write log for calibration
										CString cstrLogLux;
										cstrLogLux.Format(_T("%s"), szTmp);
										Log_Value(cstrLogLux);
										//m_strErrorCode = _T("-");
										TraceLog(MSG_INFO, _T("Check Lux success!"));
										m_strResult = "PASS";
										m_strMessage = "Check Lux success!";
									}
									break;
								}
								return true;
							}
							else{
								if (-1 != (iFindPos = CS_buffer.Find(sz_cmd)))
								{
									m_strMessage = _T("Find:") + CS_buffer;
									TraceLog(MSG_INFO, m_strMessage);
									return true;
								}
							}//if saveLog or find Char
						}
						else  TraceLog(MSG_INFO, _T("1. Read GetLastError() PASS, but no Data"));
				  }
				  break;

			  case WAIT_TIMEOUT:
				  TraceLog(MSG_INFO, _T("1. Read WAIT_TIMEOUT FAIL"));
				  // Operation isn't complete yet. fWaitingOnRead flag isn't
				  // changed since I'll loop back around, and I don't want
				  // to issue another read until the first one finishes.
				  // This is a good time to do some background work.
				  break;

			  default:
				   TraceLog(MSG_INFO, _T("1. Read WaitForSingleObject FAIL()"));
				  // Error in the WaitForSingleObject; abort.
				  // This indicates a problem with the OVERLAPPED structure's
				  // event handle.
				  break;
		   }
		}
	}/*end of readfile*/
	else
	{
		TraceLog(MSG_INFO, _T("1. ReadFile ReadFile() PASS"));
	}

	return false;
}


bool CSensorLuxCheckHShark::SetDCB(HANDLE h_Com)
{
	bool bRes = false;
	DCB dcb;
	COMMTIMEOUTS TimeOut;

	// set up the comms parameters now that we have a handle
    SetupComm(h_Com,(DWORD)2048, (DWORD)2048);

    // Set up the DCB with our settings
    // Get it first so we fill all members
    bRes = !!GetCommState(h_Com, &dcb);

	if (!bRes)
    {
        // Handle the error.
        AfxMessageBox ("Error: Failed to Get the System Communication Settings.", MB_OK|MB_ICONEXCLAMATION );
        return bRes;
    }

	dcb.BaudRate = CBR_9600; //波特率為9600
	dcb.ByteSize = 8; //每字節有8位
	dcb.Parity = NOPARITY; //無奇偶校驗位
	dcb.StopBits = ONESTOPBIT ; //一個停止位
	// assign it
    bRes = !!SetCommState(h_Com, &dcb);


	// Set up the timeouts to use, they are quite short, since we will loop anyway.
    // Do not make them zero, else we will have a CPU load problem. Too large a value,
    // and we have to wait for comms to time out when shutting down

    // fill timeout structure
    GetCommTimeouts(h_Com, &TimeOut);
    TimeOut.ReadIntervalTimeout = 100;
	TimeOut.ReadTotalTimeoutMultiplier = 5;
	TimeOut.ReadTotalTimeoutConstant = 500;
	TimeOut.WriteTotalTimeoutMultiplier = 50;	// 50ms per char sent
	TimeOut.WriteTotalTimeoutConstant = 2000;
    SetCommTimeouts(h_Com, &TimeOut);

	SetCommMask(h_Com, EV_RXCHAR);

	PurgeComm(h_Com, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

	TraceLog(MSG_INFO, _T("SetDCB PASS"));
	return bRes;
}

bool CSensorLuxCheckHShark::ConfigureCOMPort(CString strPort)
{
	bool bRes = false;

	//Create the handle for read the COM port
    m_h_Com =  CreateFile( strPort,
                    GENERIC_READ | GENERIC_WRITE,
                    (DWORD)NULL,   // exclusive access
                    NULL,          // no security
                    OPEN_EXISTING,
                    FILE_FLAG_OVERLAPPED,
                    NULL           // hTemplate
                    );

	TraceLog(MSG_INFO, _T("ConfigureCOMPort CreateFile PASS"));
	if (( m_h_Com == NULL ) || ( INVALID_HANDLE_VALUE == m_h_Com ))
    {
		TraceLog(MSG_INFO, _T("ConfigureCOMPort,  PORT COULD NOT OPEN !"));
        return bRes;
    }

	bRes = SetDCB(m_h_Com);

	return bRes;
}

bool CSensorLuxCheckHShark::SendJigRequest(CString str_command)
{
	/*don't enable JIG for RD test, SKIP the section*/
	if (m_i_EnableJIG == 0)
	{
		return true;
	}

	/* Init wait flag to false */
	m_b_waitFlag = false;

	/* Send Jig request */
	JigRequest(str_command);

	/* Wait response(Max:10s) */
	int i_waitCount = 0;
	while (m_b_waitFlag != true)
	{
		if (i_waitCount == 50)
		{
			TraceLog(MSG_INFO, _T("Jig Request Response timeout!"));
			return false;
		}
		else
		{
			i_waitCount++;
			Sleep(200);
			continue;
		}
	}

	/* Get Jig result */
	if (m_b_jigRequest == false)
	{
		TraceLog(MSG_INFO, _T("Jig ON/OFF Fail!"));
		return false;
	}

	/* Sleep for Jig */
	int dw_sleepTime = 0;
	dw_sleepTime = m_i_sleep_for_Jig * 1000;
	Sleep(dw_sleepTime);

	TraceLog(MSG_INFO, _T("Jig Request Pass!"));
	return true;
}

void CSensorLuxCheckHShark::Log_Value(CString message)
{
	CString str_Picasso = _T("");

	if(! g_strPicasso.empty() )
		str_Picasso = g_strPicasso.c_str();
	else
		str_Picasso = "0000000000";

	SYSTEMTIME tLocal;
	::GetLocalTime(&tLocal);
	CString Logtime;
	Logtime.Format(_T("%04d%02d%02d-%02d:%02d:%02d"),
		           tLocal.wYear, tLocal.wMonth, tLocal.wDay,
				   tLocal.wHour, tLocal.wMinute, tLocal.wSecond);

	CString pthLog;
	pthLog = _T("D:\\Log\\UI_RD");

	if (::GetFileAttributes(pthLog) != INVALID_FILE_ATTRIBUTES)
		CreateDirectory(pthLog, NULL);

	pthLog = _T("D:\\Log\\UI_RD\\LuxValue.CSV");

	CStdioFile fLog;
	UINT nOpenFlags =
		CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyWrite;

	message = Logtime + _T("\t") + str_Picasso + _T("\t") + message + _T("\n");

	if (fLog.Open(pthLog, nOpenFlags))
	{
		fLog.SeekToEnd();
		fLog.WriteString(message);
		fLog.Close();
	}
}

bool CSensorLuxCheckHShark::DetectDevice()
{

	std::map<std::string, std::string> map_strstrCOMDeviceName;
	str_Keyword.assign(strChipVersion);

	if (GetCOMPortDevByWDK(map_strstrCOMDeviceName))
	{
		std::map<std::string, std::string>::iterator itr;

		if (map_strstrCOMDeviceName.size() < 1)
		{
			//CmdLog("No COM device detected");
			return false;
		}
		else
		{
			bool bFound = false;
			for (itr = map_strstrCOMDeviceName.begin(); itr != map_strstrCOMDeviceName.end(); ++itr)
			{
				std::string strDev = itr->first;
				std::string strDevName = itr->second;
				std::transform(strDevName.begin(), strDevName.end(), strDevName.begin(), ::toupper);

				if (strDevName.find(str_Keyword) !=  std::string::npos)
				{
					bFound = true;
					size_t iPosition;
					if ((iPosition = strDev.find("COM")) != std::string::npos)
						str_PROLIFICcomPort = str_DeviceName = strDev.substr(3, (strDev.size() - 3));
					else
						str_PROLIFICcomPort = str_DeviceName = strDev;
				}
			}

			if (! bFound)
			{

				TraceLog(MSG_INFO, _T("Fail to find PROLIFIC Comport : "));
				TraceLog(MSG_INFO, strChipVersion);
				return false;
			}

		}
	}
	else
	{
		//CmdLog("Fail to detect COM device with WDK");
		return false;
	}


	return true;
}


bool CSensorLuxCheckHShark::GetCOMPortDevByWDK(std::map<std::string, std::string>& map_strstrCOMDevice)
{
	map_strstrCOMDevice.clear();

	BOOL bContinue = FALSE;

	// 1. Retrieves the GUID(s) associated with the specified class name "Ports"
	DWORD uiGuidSize = 0;
	GUID* ptGuidBuf = NULL;

	::SetupDiClassGuidsFromNameA("Ports", NULL, 0, &uiGuidSize);

	if (uiGuidSize > 0)
	{
		ptGuidBuf = new GUID[uiGuidSize];
		ASSERT(ptGuidBuf != NULL);

		if (ptGuidBuf != NULL)
		{
			bContinue = ::SetupDiClassGuidsFromNameA("Ports", ptGuidBuf, uiGuidSize, &uiGuidSize);
		}
	}

	// 2. Returns a device information set that contains all devices of a specified class
	HDEVINFO hDevInfoSet = INVALID_HANDLE_VALUE;

	if (bContinue)
	{
		hDevInfoSet = ::SetupDiGetClassDevsA(ptGuidBuf, NULL, NULL, DIGCF_PRESENT);
		bContinue = (hDevInfoSet != INVALID_HANDLE_VALUE);
	}

	// 3. Start enumeration
	DWORD uiIndex = 0;

	while (bContinue)
	{
		// Returns one context structure for a device information element of a device information set.
		SP_DEVINFO_DATA tDevInfo;
		tDevInfo.cbSize = sizeof(SP_DEVINFO_DATA);
		if ((bContinue = ::SetupDiEnumDeviceInfo(hDevInfoSet, uiIndex, &tDevInfo)))
		{
			// Opens a registry storage key for device-specific configuration information
			HKEY hKey = ::SetupDiOpenDevRegKey(hDevInfoSet, &tDevInfo, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE);

			if (hKey != NULL)
			{
				// Read the name of the port.
				char szDev[256];
				DWORD uiType = 0, uiDevSize = 256;
				memset(szDev, 0, 256);

				if ((::RegQueryValueExA(hKey, "PortName", NULL, &uiType, (LPBYTE)szDev, &uiDevSize) == ERROR_SUCCESS) && (uiType == REG_SZ))
				{
					std::string strTmp = szDev;

					memset(szDev, 0, 256);
					uiDevSize = 256;
					if (SetupDiGetDeviceRegistryPropertyA(hDevInfoSet, &tDevInfo, SPDRP_DEVICEDESC, &uiType, (PBYTE)szDev, uiDevSize, &uiDevSize) && (uiType == REG_SZ))
						map_strstrCOMDevice[strTmp] = std::string(szDev);
					else
						map_strstrCOMDevice[strTmp] = std::string("");
				}

				// Close the key.
				::RegCloseKey(hKey);
			}
		}

		++uiIndex;
	}

	// clean-up.
	if (ptGuidBuf != NULL)
		delete [] ptGuidBuf;

	if (hDevInfoSet != INVALID_HANDLE_VALUE)
		::SetupDiDestroyDeviceInfoList(hDevInfoSet);

	return true;
}