#include "stdafx.h"
#include "CN4010A.h"
#include "shlwapi.h"

#ifdef _N4010

GP_IMPLEMENT_DYNCREATE(CN4010A)

#define CMDSEQUENCE_LOG  ".\\LOG\\CmdSeq.txt"
#define LOCK_DELAY       6000000

char CN4010A::logFileName[256] = {0};
bool CN4010A::bWriteCmdLog = false;


CN4010A::CN4010A()
	: WaitCount (1000)
	, WaitInterval (500)
	, InquiryCount(100)
	, InquiryInterval(500)

{
	m_pInstrument = NULL;
	memset (m_szErrorMsg, 0, 1024);
}



bool CN4010A::DetectVISADevice (std::string& strDeviceName)
{
#ifdef _DONT_DETECT_VISA_DEVICE
	return true;
#endif

	// BTTest Visa dll
	if (VI_SUCCESS != viOpenDefaultRM (&defaultRM))
	{
		CmdLog("Executed viOpenDefaultRM Failed");
		return false;
	}
	else
		CmdLog("Executed viOpenDefaultRM Success");

	if (VI_SUCCESS != viOpen(defaultRM, szVISAname, VI_NULL, VI_NULL, &vi))
	{
		CmdLog("Executed viOpen Failed");
		return false;
	}
	else
		CmdLog("Executed viOpen Success");

	if (! GetDeviceName(strDeviceName))
	{
		CmdLog("Executed GetDeviceName Failed");
		return false;
	}
	else
		CmdLog("Executed GetDeviceName Success");

	return true;
}

void CN4010A::ReleaseVISADevice()
{
	viClose (vi);
	viClose (defaultRM);
}

bool CN4010A::Lock()
{
#ifdef _DONT_DETECT_VISA_DEVICE
	return true;
#endif

	return viLock (vi, VI_EXCLUSIVE_LOCK, LOCK_DELAY, VI_NULL, VI_NULL) == VI_SUCCESS;
}

void CN4010A::Unlock()
{
	viUnlock (vi);
}

bool CN4010A::GetDeviceName (std::string& strDeviceName)
{
#ifdef _DONT_DETECT_VISA_DEVICE
	return true;
#endif

	char buf [256] = {0};

	// Get exclusive lock
	if (VI_SUCCESS != viLock (vi, VI_EXCLUSIVE_LOCK, 2000, VI_NULL, VI_NULL))
		return false;

	// Send an *IDN? string to the device
	if (VI_SUCCESS != viPrintf (vi, "*IDN?\n"))
	{
		viUnlock(vi);
		return false;
	}

	// Read results
	if (VI_SUCCESS != viScanf (vi, "%t", &buf))
	{
		viUnlock (vi);
		return false;
	}

	// Print results
	strDeviceName = buf;

	SendHWLatchSCPIcmd();

	// release lock
	viUnlock (vi);
	return true;
}





bool CN4010A::SendHWLatchSCPIcmd()
{
	ViPUInt32 retCnt1 = 0;

	unsigned char cmdOut1[] = "diag:hw:latch:sel \"DAP Trig Qual Enable Cmd\"";
	viWrite(vi, cmdOut1, sizeof(cmdOut1), retCnt1);
	unsigned char cmdOut2[] = "diag:hw:latch:val 0";
	viWrite(vi, cmdOut2, sizeof(cmdOut2),retCnt1);
	unsigned char cmdOut3[] = "diag:hw:latch:sel \"DAP Trig Enable Cmd\"";
	viWrite(vi, cmdOut3, sizeof(cmdOut3),retCnt1);
	unsigned char cmdOut4[] = "diag:hw:latch:val 32";
	viWrite(vi, cmdOut4, sizeof(cmdOut4),retCnt1);

	return true;
}



bool CN4010A::StartDeviceWifi ()
{
	// WLanTest dll
	HRESULT hres;

	//for Agilent_N4010_WLanTest.tlb
	CoInitialize(NULL);

	Instrument_InterfacePtr pInst(__uuidof(Instrument));
	m_pInstrument = pInst;

#ifdef _DONT_DETECT_VISA_DEVICE
	return true;
#endif

	try
	{

		//BSTR  MyBstr = SysAllocString(strVISAname);
		CComBSTR MyBstr = szVISAname;
		hres = m_pInstrument->put_InstrumentVISAResourceString (MyBstr);
		if (FAILED(hres))
			return false;

		hres = m_pInstrument->ActivateInstrument();
		if (hres != S_OK)
		{
			return false;
		}

		if (S_OK != m_pInstrument->SetWlanMode())
			return false;

		if (S_OK != m_pInstrument->Preset())
			return false;

		if (S_OK != m_pInstrument->Preset())
			return false;

		m_pInstrument->PutFrequency (2417.0e6);

		m_pInstrument->PutFrequency (2412.0e6);

	}
	catch (_com_error /*&er*/)
	{
		//std::string strlog = std::string((char*)er.Description());
		//SetItemError (strlog.c_str(), FunErr_WLAN_Test_Init_Device_Fail);
		//FactoryLog();
		return false;
	}


	return true;
}

bool CN4010A::StopDeviceWifi ()
{
	if (m_pInstrument)
	{
		m_pInstrument->SetBluetoothMode();
		m_pInstrument->Dispose();
		CoUninitialize();
		m_pInstrument = NULL;
	}
	return true;
}





//bool CN4010A::InitData(std::map<std::string, std::string>& paramMap)
bool CN4010A::InitData(pugi::xml_document *pXMLTestItem, std::string strNickName)
{
	pugi::xml_node myParm_node = pXMLTestItem->child("Configuration").child("Param").child(strNickName.c_str());

	strcpy_s(szVISAname, 256, myParm_node.child("VISAName").text().as_string());

	bWriteCmdLog = myParm_node.child("LogCmd").text().as_bool();

	WaitCount = myParm_node.child("WaitTestRetryCount").text().as_int();

	WaitInterval = myParm_node.child("WaitTestInterval").text().as_int();

	InquiryCount = myParm_node.child("WaitInquiryRetryCount").text().as_int();

	InquiryInterval = myParm_node.child("WaitInquiryInterval").text().as_int();

	return true;

	/*if (paramMap.find ("VISAName") == paramMap.end())
		return false;
	strcpy(strVISAname, paramMap["VISAName"].c_str());

	if (paramMap.find ("LogCmd") != paramMap.end())
		bWriteCmdLog = paramMap["LogCmd"] != "0";
	if (paramMap.find ("WaitTestRetryCount") != paramMap.end())
		WaitCount = atoi(paramMap["WaitTestRetryCount"].c_str());
	if (paramMap.find ("WaitTestInterval") != paramMap.end())
		WaitInterval = atoi(paramMap["WaitTestInterval"].c_str());

	if (paramMap.find ("WaitInquiryRetryCount") != paramMap.end())
		InquiryCount = atoi(paramMap["WaitInquiryRetryCount"].c_str());
	if (paramMap.find ("WaitInquiryInterval") != paramMap.end())
		InquiryInterval = atoi(paramMap["WaitInquiryInterval"].c_str());


	InitDebug (CMDSEQUENCE_LOG);
	return true;*/

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//             Basic Read / Write

bool CN4010A::WriteCmd (const char* fmt, ...)
{ 
#ifdef _DONT_DETECT_VISA_DEVICE
	return true;
#endif

	bool ret = true;

	char chCmd[512] = {0};
	va_list va;
	va_start (va,fmt);
	int len = vsprintf_s(chCmd, 512, fmt, va);
	strcat_s(chCmd, 512, "\n");
	va_end (va);

	Dbg ("-->", chCmd);
	ViStatus status = viPrintf (vi, chCmd);
	if (status != VI_SUCCESS)
	{
		memset (m_szErrorMsg, 0, 1024);
		viStatusDesc (vi, status, m_szErrorMsg);
		strcat_s(m_szErrorMsg, 1024, "\n");
		Dbg ("<ERR>", m_szErrorMsg);
		ret = false;
	}
	else
	{
		ret = ! HasSystemError();
	}

	return ret;
}

bool CN4010A::ReadCmd (char* szResult, const char* fmt, ...)
{
#ifdef _DONT_DETECT_VISA_DEVICE
	strcpy_s(szResult, 2, "1");
	return true;
#endif

	char chCmd[1024] = {0};

	va_list va;
	va_start (va,fmt);
	int len = vsprintf_s(chCmd, 1024, fmt, va);
	strcat_s(chCmd, 1024, "\n");
	va_end (va);

	bool ret = true;
	ViStatus status;
	
	
	//int retry = 10;
	//while (retry --)
	{
		// send command
		Dbg ("-->", chCmd);
		status = viPrintf (vi, chCmd);
		if (VI_SUCCESS == status)
		{  
			status = viScanf (vi, "%t", szResult);
			Dbg ("<--", szResult);
		}

		if (VI_SUCCESS != status)
		{
			memset (m_szErrorMsg, 0, 1024);
			viStatusDesc (vi, status, m_szErrorMsg);
			strcat_s(m_szErrorMsg, 1024, "\n");
			Dbg ("<ERR>", m_szErrorMsg);
			ret = false;
		}
		else
		{
			ret = !HasSystemError();
		}

		if (ret)
		{
			//break;
		}
		else
		{
			if ((strstr (m_szErrorMsg, "VI_ERROR_TMO") != NULL) ||  // ignore timeout
				(strstr (m_szErrorMsg, "Query UNTERMINATED") != NULL) ||
				(strstr (m_szErrorMsg, "Query INTERRUPTED") != NULL))
				ret = true;
			//else
			//	break;
		}

//		Sleep (50);

	}// while (retry --)

	return ret;
	
}

double CN4010A::ReadCmd (const char* fmt, ...)
{
	char chResult [128] = {0};
	char chCmd[512] = {0};

	va_list va;
	va_start (va,fmt);
	int len = vsprintf_s(chCmd, 512, fmt, va);
	va_end (va);

	bool ret = ReadCmd (chResult, chCmd);
	return ret ? atof (chResult) : -999999999.999;
}

//1=pass, 0=error, -1=result fail
int CN4010A::IsPass (const char* fmt, ...)
{
#ifdef _DONT_DETECT_VISA_DEVICE
	return true;
#endif

	char chTemp[128] = {0};
	char chCmd[512] = {0};


	va_list va;
	va_start (va,fmt);
	vsprintf_s(chCmd, 512, fmt, va);
	va_end (va);

	if (! ReadCmd (chTemp, chCmd))
	{
		return 0;
	}

	return atoi(chTemp) == 0 ? 1 : -1;
}


bool CN4010A::HasSystemError()
{
	bool ret = false;

	int errCode = 0;
	char chError[1024] = {0};

	viQueryf (vi, "SYST:ERR?\n", "%d,%t", &errCode, chError);
	if (errCode != 0)
	{
		strcat_s(chError, 1024, "\n");
		Dbg ("<ERR>", chError);

		if ((strstr (chError, "VI_ERROR_TMO") == NULL) &&  // ignore timeout
			(strstr (chError, "Query UNTERMINATED") == NULL) &&
			(strstr (chError, "Query INTERRUPTED") == NULL))
		{
			ret = true;
 			strcpy_s(m_szErrorMsg, 1024, chError);
		}
	}

	return ret;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CN4010A::GetDeviceOption (std::string& strOption)
{
#ifdef _DONT_DETECT_VISA_DEVICE
	strOption = "101,102,107,108";
	return true;
#endif

	if (m_strOption.size() == 0) // only query for the first time
	{
		char chOption[256] = {0};
		viQueryf (vi, "*OPT?\n", "%t", chOption);
		m_strOption = chOption;
	}
	strOption = m_strOption;
	return true;
}

void CN4010A::ClearUpTestSet ()
{
	WriteCmd ("LINK:CONTrol:DISConnect:IMMediate");
	WriteCmd ("LINK:TEST:EXIT 1");

	SendHWLatchSCPIcmd();

	////Clear the instrument status register
	//viPrintf(vi,"*SRE 0 \n");
	//viPrintf(vi,"*ESE 0 \n");
	//viPrintf(vi,"STAT:OPER:ENAB 0\n");
	//viPrintf(vi,"STAT:QUES:ENAB 0\n");
	//viPrintf(vi,"STAT:QUES:CAL:ENAB 0\n");
 //	viPrintf(vi,"STAT:QUES:INT:ENAB 0\n");
 // 	viPrintf(vi,"STAT:QUES:LINK:ENAB 0\n");
 //  	viPrintf(vi,"STAT:QUES:SEQ:ENAB 0\n");
	//
	////Clear the status byte of the instrument
	//viPrintf(vi,"*CLS\n");
}

bool CN4010A::Reset()
{
	WriteCmd ("*RST");
	WriteCmd ("SYST:PRES");
	WriteCmd ("STAT:PRES");
	WriteCmd ("*CLS");


	return true;
}


bool CN4010A::InitRegisterSetting()
{
	WORD wValue;

	//
	// OPERATION Register
	//
		
	wValue = 0;
	//wValue |= 0x0001; // bit0: calibrating 
	wValue |= 0x0010; // bit4: measuring
	//wValue |= 0x0020; // bit5: Waiting for trigger
	wValue |= 0x0100; // bit8: Paused - This feature is not implemented in the current firmware revision (A.01.02)
	wValue |= 0x0200; // bit9: Running a Sequence
	wValue |= 0x0400; // bit10:Link Operation Status register summary
	wValue |= 0x0800; // bit11:Being configured prior to performing a test
	WriteCmd ("STAT:OPER:ENAB %d", wValue); //0xf31

	WriteCmd ("STAT:OPER:LINK:ENAB 1"); // BT Link Questinable Summary
	WriteCmd ("STAT:OPER:LINK:BLU:ENABle 63");




	//
	// QUESTIONABLE Register
	//

	wValue = 0;
	//wValue |= 0x0020; // bit5: Frequency Summary				Refer to STATus:QUEStionable:FREQuency Status Register.
	//wValue |= 0x0100; // bit8: Calibration Register Summary		Refer to STATus:QUEStionable:CALibration Status Register. 
	//wValue |= 0x0200; // bit9: Integrity Register Summary		Refer to STATus:QUEStionable:INTegrity Status Register.
	wValue |= 0x0400; // bit10:Link Register Summary			Refer to STATus:QUEStionable:LINK Status Register.
	wValue |= 0x0800; // bit11:Sequence Register Summary		Refer to STATus:QUEStionable:SEQuence Status Register.
	WriteCmd ("STAT:QUES:ENAB %d", wValue); //0xf00

	//WriteCmd ("STAT:QUES:FREQ 3872"); //0xf20

	//0 Self-calibration failed. This feature is not implemented in the current firmware revision (A.01.02). 
	//WriteCmd ("STAT:QUES:CAL:ENAB 1");

	//1 Out of calibration.
	//4 ADC over range
	//5 RF over range
	//7 Over temperature.
 	//WriteCmd ("STAT:QUES:INT:ENAB 178"); //0xb2

	//0 Bluetooth Link Questionable status summary
	//1 WLAN Link Questionable status summary. (Not implemented in this release.)
  	WriteCmd ("STAT:QUES:LINK:ENAB 1");


	//0 Pass/Fail status of test sequence
   	WriteCmd ("STAT:QUES:SEQ:ENAB 1");




	//
	// STANDARD EVENT Register
	//

	wValue = 0;
	wValue |= 0x01; // bit 0: Operation. Complete All commands have been completed. Refer to Using the Operation Complete Commands.
					// bit 1: Not Used. (Always set to 0.)
	//wValue |= 0x04; // bit 2: Query Error. A query error occurred, refer to error numbers -400 to -499.
	wValue |= 0x08; // bit 3: Device Error. A device error occurred, refer to error numbers -300 to -399 and 1 to 32767.
	wValue |= 0x10; // bit 4: Execution Error. An execution error occurred, refer to error numbers -200 to -299.
	wValue |= 0x20; // bit 5: Command Error. A command syntax error occurred, refer to error numbers -100 to -199.
	wValue |= 0x80; // bit 7: Power On. Power has been turned off and on since the last time the event register was read or cleared.
	WriteCmd ("*ESE %d", wValue); //0xBD


	//
	// Service Request Enable 

	//2 Device Dependent
	//3 QUEStionable Status Summary
	//4 Message Available
	//5 Event Status Bit
	//7 OPERation Status Summary
	wValue = 0;
	wValue |= 0x04; // bit 2: Device Dependent
	wValue |= 0x08; // bit 3: QUEStionable Status Summary
	wValue |= 0x10; // bit 4: Message Available
	wValue |= 0x20; // bit 5: Event Status Bit
	wValue |= 0x80; // bit 7: OPERation Status Summary

 	WriteCmd ("*SRE %d", wValue); //96

	return true;
}

bool CN4010A::SetupTestSet (const char* freqSrc, const char* pwClass, const char* psrm, const char* pin, const char* mode)
{
	//InitRegisterSetting();

	////Register Event Handler and Enable it
	//viInstallHandler (vi, VI_EVENT_SERVICE_REQ, my_handler, (ViAddr)10);
	//viEnableEvent(vi, VI_EVENT_SERVICE_REQ, VI_HNDLR, VI_NULL);
	//viInstallHandler (vi, VI_EVENT_EXCEPTION, my_handler, (ViAddr)10);
	//viEnableEvent(vi, VI_EVENT_EXCEPTION, VI_HNDLR, VI_NULL);


	// Operating mode
	if (! WriteCmd ("LINK:TYPE %s", mode))
		return false;

	if (! WriteCmd ("LINK:STE:BDAD #hBDBDBDBDBD"))
		return false;

	//Test Set automatically disconnects its link to the EUT at the end of a test sequence
	if (! WriteCmd ("LINK:CONN:AUTO 1"))
		return false;

	// Set EUT's Power Class
	if (! WriteCmd ("LINK:EUT:PCL %s", pwClass))
		return false;

	//if (! InquiryEUT())
	//	return false;
	
	//if (! WriteCmd ("LINK:TEST:CDELay 0.125"))
	//	return false;

	//if (! WriteCmd ("SENS:CORR:LOSS:STAT 0"))
	//	return false;

	WriteCmd ("LINK:TEST:EXIT 0");


	/*
	// Set 10MHz Frequency Reference Source
	if (! WriteCmd ("SENS:ROSC:SOUR %s", freqSrc))
		return false;

	// Set Test Set's Page Scan Repetition Mode
	//if (! WriteCmd ("LINK:EUT:PRSM %s", chPRSM))
	//	return false;

	// Set the Security level (Pairing)
	if (0 == pin[0])
	{
		if (! WriteCmd ("LINK:CONN:AUTH:STAT OFF"))
			return false;
	}
	else
	{
		if (! WriteCmd ("LINK:CONN:AUTH:STAT ON"))
			return false;
		if (! WriteCmd ("LINK:EUT:PIN '%s'", pin))
			return false;
	}
	*/


	return true;
}



bool CN4010A::InquiryEUT (const char* phoneBTAddr)
{
#ifdef _DONT_DETECT_VISA_DEVICE
	return true;
#endif

	bool ret = false;
	bool InquiryRet = false ;
	long count;
	
	if(!WriteCmd ("LINK:INQ:DUR 6.4"))
	return false;

	char chBDAD [512];
	ReadCmd (chBDAD, "LINK:EUT:BDAD?");
	
	if (NULL == chBDAD)
	    return false ;

	if (chBDAD[2] != '-')
	{
		if (! WriteCmd ("LINK:EUT:BDAD:CLE"))
			return false;
	}

	Sleep (500);

	if (! WriteCmd ("LINK:CONT:INQ:IMM"))
		return false;


	int retry = InquiryCount;
	for (int i=0; i<retry; ++i)
	{
		char chBuffer[512] = {0};
		count = (long) ReadCmd ("LINK:INQ:BDAD:COUN?");
		if (count < 0)
		{
			break; // has error
		}
		else if (count > 0)
		{
			//#H00AB2C11EF87,#H00BDBD437254AC
			ReadCmd (chBuffer, "LINK:INQ:BDAD:RESP?");
			if ('#' == chBuffer[0])
			{
				// check if found PhoneBTAddr
				bool bFound = false;
				char *pChar = &chBuffer[2];

				if (0 == phoneBTAddr[0]) 
				{
					bFound = true;// match the first one.
				}
				else
				{
					for (int j=0; j<count; ++j)
					{
						if (_strnicmp (pChar, phoneBTAddr, 12) == 0)
						{
							bFound = true;
							break;
						}
						pChar += 12;
					}
				}

				if (bFound)
				{
					if (! WriteCmd ("LINK:EUT:BDAD %s", chBuffer))
						return false;

					ret = true;
					break;
				}
			}
		}

		Sleep (InquiryInterval);
	}

	return ret;
}

bool CN4010A::SetCableLoss (const double dLoss)
{
	if (! WriteCmd ("SENS:CORR:LOSS:STAT ON"))
		return false;

	if (! WriteCmd ("SENS:CORR:LOSS:FIX %f", dLoss))
		return false;

	return true;
}



bool CN4010A::WaitTest (const char* testTag, const int index)
{
	bool ret = false;

	char chBuffer[512]={0};
	int done = 0;
	int retry = WaitCount;


	for (int i=0; i<retry; ++i)
	{
		if (! ReadCmd (chBuffer, "SEQ:DONE? %s,%d", testTag, index))
		{
			return false;
		}
		else
		{
			done = atoi (chBuffer);
			if (done >= 1)
			{
				ret = true;
				break;
			}
		}

		Sleep (WaitInterval);
	}


	return ret;
}







//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//              Debug

void CN4010A::InitDebug (const char* chFileName)
{
	if (! bWriteCmdLog)
		return ;

	char chModule[MAX_PATH];
	strcpy_s(chModule, MAX_PATH, this->m_strWorkDir.c_str());
	PathRemoveFileSpecA (chModule);
	PathAppendA (chModule, chFileName);
	PathRemoveFileSpecA (chModule);

	// CreateDirectory if not exist
	if (! CreateDirectoryA (chModule, NULL))
	{
		if (GetLastError() != ERROR_ALREADY_EXISTS)
			strcpy_s(chModule, MAX_PATH, "C:\\");
	}

	sprintf_s(logFileName, 256, "%s\\%s", chModule, PathFindFileNameA(chFileName));


	// Write initial msg to log
	Dbg ("**", "**************** START Log file ****************************\n");
}


void CN4010A::Dbg (const char* dir, const char* chPrint)
{
	if (! bWriteCmdLog)
		return;

	SYSTEMTIME currTime;
	::GetLocalTime(&currTime);

	CString cstrTemp, cstrdir(dir), cstrPrint(chPrint);
	cstrTemp.Format(_T("[%02d:%02d:%02d.%03d] %s %s"), currTime.wHour, currTime.wMinute, currTime.wSecond, currTime.wMilliseconds, cstrdir.GetString(), cstrPrint.GetString());


	CStdioFile File_log;
	if(File_log.Open (CString(logFileName), CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate, NULL))
	{
		File_log.SeekToEnd();
		File_log.WriteString(cstrTemp);
		File_log.Close();
	}
}

#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
ViStatus _VI_FUNCH CN4010A::my_handler (ViSession vi, ViEventType eventType, ViEvent context, ViAddr usrHandle) 
{
	ViUInt16 status16 = 0;

	// Read status byte
	if (VI_SUCCESS != viReadSTB(vi, &status16))
		return VI_SUCCESS; //???

	if (status16 & 0x04)
	{
	//	TraceLog ("SRQ message:\t STB Error queue");
	//	
		// read system error
		char buf[256] = {0};
		int err;
		viQueryf (vi, "SYST:ERR?\n", "%d,%t", &err, buf);
		//fprintf (fLog, "\n SYST:ERR return %d,%s", err,buf);
	}
	//if (status16 & 0x08)
	//	fprintf (fLog, "\n SRQ message:\t STB Questionable Status Summary");
	if (status16 & 0x10)
	{
		//fprintf (fLog, "\n SRQ message:\t STB MessageAvailable");
	}
	if (status16 & 0x20)
	{
		//fprintf (fLog, "\n SRQ message:\t STB Event Status Summary");
	}
	//if (status16 & 0x40)
	//	fprintf (fLog, "\n SRQ message:\t STB RQS");
	if (status16 & 0x80)
	{
		Dbg ("*STB*", "STB Operation Summary Status");
	}




	return VI_SUCCESS;
}
*/