
#include "stdafx.h"
#include "LSensorLuxCheck.h"
#include <SetupAPI.h>

#pragma comment(lib, "setupapi")
RF_IMPLEMENT_DYNCREATE(CBostonLSensorLuxCheck)

bool CBostonLSensorLuxCheck::InitData(std::map<std::string, std::string>& paramMap)
{
	std::string pstrRange;
	std::string str;

	if (paramMap.find(std::string("LuxSpec")) != paramMap.end())
	{
		pstrRange = paramMap[std::string("LuxSpec")];
		m_obj_LuxSpec.SetRange(pstrRange);
	}
	else
	{
		TraceLog(MSG_INFO, _T("Get Lux Spec Fail."));
		return false;
	}

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

	// Set Comport
	if (paramMap.find(std::string("Comport")) != paramMap.end())
	{
		m_str_comport = paramMap[std::string("Comport")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Get Comport fail."));
		return false;
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

bool CBostonLSensorLuxCheck::PreRun()
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

bool CBostonLSensorLuxCheck::Run()
{


	if (m_pIPhone == NULL)
	{
		TraceLog(MSG_INFO, _T("m_pIPhone = NULL"));
		return false;
	}
	if (DetectDevice()){
		return LuxCheckProc();
	}else{
		m_strErrorCode = FunErr_LSensor_LuxCheck_Fail;
		m_strMessage = "DetectDevice Fail!";
		FactoryLog();
		return false;
	}
}

bool CBostonLSensorLuxCheck::PostRun()
{
	return true;
}

bool CBostonLSensorLuxCheck::LuxCheckProc(void)
{
	std::string m_str_Message;
	char szTmp[256];
	int iLux = 0;
	int iTotal = 0;
	int iAverage = 0;
	int iCount = 0;
	bool bRes = false;
	bool wRes = false;

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

	CString strPort;
	//strPort.Format( _T("\\\\.\\%s"), m_str_comport.c_str() );
	strPort.Format( _T("\\\\.\\COM%s"), m_strPROLIFICcomPort.c_str() );

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

	// Set write data //
	char c_sendBuffer[10]="";
	DWORD dw_writeLen;

	DWORD numberOfBytesRead = 0;
	CString CS_buffer;
	const int bufferSize = 100;
	char temp_buf[bufferSize];
	int iRetry = 0;
	int iFindPos = 0;
	bool bLocked = false;
	OVERLAPPED overlappedWrite, overlappedRead;
	DWORD dwRes;

	memset(&overlappedWrite, 0, sizeof(overlappedWrite));
	memset(&overlappedRead, 0, sizeof(overlappedRead));

	overlappedWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (overlappedWrite.hEvent == NULL)
	{
		TraceLog(MSG_INFO, _T("overlappedWrite :: CreateEvent fail"));
		CloseHandle(m_h_Com);
		m_h_Com = NULL;
		FactoryLog();
		return false;
	}

	overlappedRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (overlappedRead.hEvent == NULL)
	{
		TraceLog(MSG_INFO, _T("overlappedRead :: CreateEvent fail"));
		CloseHandle(m_h_Com);
		CloseHandle(overlappedWrite.hEvent);
		m_h_Com = NULL;
		FactoryLog();
		return false;
	}

	while (!bLocked && iRetry < m_i_retry)
	{
		strcpy(c_sendBuffer, "B");
		dw_writeLen = strlen(c_sendBuffer)*sizeof(char);

		if(!WriteFile(m_h_Com, c_sendBuffer, dw_writeLen, &dw_writeLen, &overlappedWrite))
		{
			if (GetLastError() != ERROR_IO_PENDING)
			{
				// WriteFile failed, but isn't delayed. Report error and abort.
				TraceLog(MSG_INFO, _T("B :: GetLastError() != ERROR_IO_PENDING"));
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
							 TraceLog(MSG_INFO, _T("B :GetOverlappedResult return false"));
							 wRes = false;
						 }
						 else
						 {
						  // Write operation completed successfully.
							 TraceLog(MSG_INFO, _T("WaitForSingleObject Write B PASS"));
							 wRes = true;
						 }
						 break;
					default:
						 // An error has occurred in WaitForSingleObject.
						 // This usually indicates a problem with the
						// OVERLAPPED structure's event handle.
						TraceLog(MSG_INFO, _T("B :: WriteFile fail"));
						wRes = false;
						break;
				}
			}
		}
		else
		{
			wRes = true;
			TraceLog(MSG_INFO, _T("WriteFile B PASS"));
		}; /*end of if(!WriteFile)*/

		if (wRes == false){
			CloseHandle(m_h_Com);
			CloseHandle(overlappedRead.hEvent);
			CloseHandle(overlappedWrite.hEvent);
			m_h_Com = NULL;
			FactoryLog();
			return false;
		}

		PurgeComm(m_h_Com, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
		TraceLog(MSG_INFO, _T("B : Beging to adjust."));

		for (int i = 0; i < 3; i++)
		{
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

									TraceLog(MSG_INFO, _T("1. Read GetLastError() PASS"));
									TraceLog(MSG_INFO, temp_buf);

									CS_buffer = temp_buf;
									if (-1 != (iFindPos = CS_buffer.Find("Locked")))
									{
										bLocked = true;
										break;
									}
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
		}//end of for'
		iRetry ++;
	}/*end of while*/

	if ( bLocked == true ){
		TraceLog(MSG_INFO, _T("adjust PASS"));
	}
	else
	{
		PurgeComm(m_h_Com, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
		CloseHandle(m_h_Com);
		CloseHandle(overlappedWrite.hEvent);
		CloseHandle(overlappedRead.hEvent);
		m_h_Com = NULL;

		m_strResult = "FAIL";
		m_strErrorCode = FunErr_LSensor_LuxCheck_Fail;
		m_strMessage = "adjust FAILED.";
		m_strUpper = "";
		m_strLower = "";
		m_strMeasured = "";
		TraceLog(MSG_INFO, _T("adjust FAILED"));

		FactoryLog();
		return false;
	}

	iRetry = 0;
	while (/*iCount < m_i_repeat &&*/ iRetry < m_i_retry)
	{
		memset(temp_buf, 0, bufferSize);

		if(!ReadFile(m_h_Com, temp_buf, bufferSize, &numberOfBytesRead, &overlappedRead))
		{
			//Sleep(m_i_sleep_for_Data);
			if (GetLastError() != ERROR_IO_PENDING)
			{
				// ReadFile failed, but isn't delayed. Report error and abort.
				TraceLog(MSG_INFO, _T("2.Read Locked :: GetLastError() != ERROR_IO_PENDING"));
				m_strMessage = "2.Read Locked";
				CloseHandle(m_h_Com);
				CloseHandle(overlappedWrite.hEvent);
				CloseHandle(overlappedRead.hEvent);
				m_h_Com = NULL;
				FactoryLog();
				return false;
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
						  TraceLog(MSG_INFO, _T("2.Read Locked :: GetLastError() != ERROR_IO_PENDING"));
					  }
					  else
					  {
							TraceLog(MSG_INFO, _T("2.Read GetLastError() PASS"));
							m_str_Message = temp_buf;
							TraceLog(MSG_INFO, m_str_Message);
					  }
					  break;

				  case WAIT_TIMEOUT:
					  TraceLog(MSG_INFO, _T("2. Read WAIT_TIMEOUT FAIL"));
					   break;
					  // Operation isn't complete yet. fWaitingOnRead flag isn't
					  // changed since I'll loop back around, and I don't want
					  // to issue another read until the first one finishes.
					  // This is a good time to do some background work.

				  default:
					   TraceLog(MSG_INFO, _T("2. Read WaitForSingleObject FAIL()"));
					   break;
					  // Error in the WaitForSingleObject; abort.
					  // This indicates a problem with the OVERLAPPED structure's
					  // event handle.
			   }/*end of switch*/
			}
		}/*end of readfile*/
		else
		{
			iRetry ++;
			TraceLog(MSG_INFO, _T("2. ReadFile ReadFile() PASS"));
		}

		int nBufferLength = 0 ;
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
				m_strErrorCode = _T("-");
				TraceLog(MSG_INFO, _T("Check Lux success!"));
				bRes = true;
				m_strResult = "PASS";
				m_strMessage = "Check Lux success!";
			}
			break;
		}
		else{
			TraceLog(MSG_INFO, _T("2.Read GetLastError() PASS, but no data"));
			break;
		}

	}//end of while loop

	FactoryLog();

	strcpy(c_sendBuffer, "S");
	dw_writeLen = strlen(c_sendBuffer)*sizeof(char);
	if(!WriteFile(m_h_Com, c_sendBuffer, dw_writeLen, &dw_writeLen, &overlappedWrite))
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			// WriteFile failed, but isn't delayed. Report error and abort.
			TraceLog(MSG_INFO, _T("S :: GetLastError() != ERROR_IO_PENDING"));
			m_strMessage = "S :: GetLastError()";
			CloseHandle(m_h_Com);
			CloseHandle(overlappedWrite.hEvent);
			m_h_Com = NULL;

			FactoryLog();
			return false;
		}
		else
		{
			// Write is pending.
			//dwRes = WaitForSingleObject(overlappedWrite.hEvent, INFINITE);
			dwRes = WaitForSingleObject(overlappedWrite.hEvent, m_i_sleep_for_Data);
			switch(dwRes)
			{
				case WAIT_OBJECT_0:
					 if (!GetOverlappedResult(m_h_Com, &overlappedWrite, &dw_writeLen, FALSE))
					 {
						 TraceLog(MSG_INFO, _T("S :: GetOverlappedResult false"));
					 }
					 else
					 {
					  // Write operation completed successfully.
						 TraceLog(MSG_INFO, _T("S :: WaitForSingleObject Write S PASS"));
					 }
					 break;
				default:
					 // An error has occurred in WaitForSingleObject.
					 // This usually indicates a problem with the
					// OVERLAPPED structure's event handle.
					TraceLog(MSG_INFO, _T("S :: WriteFile fail"));
					break;
			}
		}
	}

	PurgeComm(m_h_Com, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	TraceLog(MSG_INFO, _T("S : Stop adjusting."));

	CloseHandle(m_h_Com);
	CloseHandle(overlappedWrite.hEvent);
	CloseHandle(overlappedRead.hEvent);
	m_h_Com = NULL;

	if (SendJigRequest("ON") != true)
	{
		m_strResult = "FAIL";
		m_strErrorCode = CommErr_Jig_Move_Fail;
		m_strMessage = "Jig Request(ON) Fail!";
		m_strUpper = "";
		m_strLower = "";
		m_strMeasured = "";
		TraceLog(MSG_INFO, _T("Jig Request(ON) Fail!"));
		FactoryLog();
		return false;
	}
	TraceLog(MSG_INFO, _T("Jig Request(ON) Pass!"));

	return bRes;
}



bool CBostonLSensorLuxCheck::SetDCB(HANDLE h_Com)
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

bool CBostonLSensorLuxCheck::ConfigureCOMPort(CString strPort)
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

bool CBostonLSensorLuxCheck::SendJigRequest(CString str_command)
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

void CBostonLSensorLuxCheck::Log_Value(CString message)
{
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

	message = Logtime + _T("\t") + message + _T("\n");

	if (fLog.Open(pthLog, nOpenFlags))
	{
		fLog.SeekToEnd();
		fLog.WriteString(message);
		fLog.Close();
	}
}

bool CBostonLSensorLuxCheck::DetectDevice()
{

	std::map<std::string, std::string> map_strstrCOMDeviceName;
	m_strKeyword.assign("PROLIFIC");

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

				if (strDevName.find(m_strKeyword) !=  std::string::npos)
				{
					bFound = true;
					size_t iPosition;
					if ((iPosition = strDev.find("COM")) != std::string::npos)
						m_strPROLIFICcomPort = m_strDeviceName = strDev.substr(3, (strDev.size() - 3));
					else
						m_strPROLIFICcomPort = m_strDeviceName = strDev;
				}
			}

			if (! bFound)
			{
				TraceLog(MSG_INFO, _T("Fail to find PROLIFIC Comport"));
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


bool CBostonLSensorLuxCheck::GetCOMPortDevByWDK(std::map<std::string, std::string>& map_strstrCOMDevice)
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