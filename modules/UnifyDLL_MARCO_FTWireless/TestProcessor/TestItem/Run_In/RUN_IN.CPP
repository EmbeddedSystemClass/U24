/*********************************************************************************
 * COPYRIGHT Qisda WR24 2012
 * Author     : Lion.Wu
 ********************************************************************************/

#include "stdafx.h"
#include "RUN_IN.h"
#include <string>
#include <direct.h>
#include "io.h"

template <class T> 
std::string ConvertToString(T);

RF_IMPLEMENT_DYNCREATE(CRUN_IN)

template <class T> 
std::string ConvertToString(T value) {
  std::stringstream ss;
  ss << value;
  return ss.str();
}

bool CRUN_IN::PreRun()
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

bool CRUN_IN::Run()
{
	bool passFail = false;

	passFail = getTargetLog();
	if ( !passFail) {
		str_msg = CW2A(L"getTargetLog FAIL");
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "getTargetLog FAIL!");
		SetPICSData(str_Pics, "FAIL");
		FactoryLog();
		return false;
	}

	passFail = readLog();
	if ( !passFail) {
		str_msg = CW2A(L"FAIL! run in log not exist");
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "FAIL! run in log not exist");
		SetPICSData(str_Pics, "FAIL");
		FactoryLog();
		saveLogWhenFail();
		return false;
	}

	passFail = copyDeleteLog();
	if ( !passFail) {
		str_msg = CW2A(L"copyDeleteLog FAIL");
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "copyDeleteLog FAIL!");
		SetPICSData(str_Pics, "FAIL");
		FactoryLog();
		return false;
	}
	if (m_str_TestItem == runIn_ThermalLog)  
	{
		m_str_TestItem = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
		m_strErrorCode = FunErr_GPU_2D_Test_Fail;
		//str_Pics = CW2A(L"Fine_Wifi_AP");
		saveLogWhenFail();
		return true;
	//	passFail = go_runIn_2D( m_i_Index );
	}
	if (m_str_TestItem == runIn_2D)  
	{
		m_str_TestItem = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
		m_strErrorCode = FunErr_GPU_2D_Test_Fail;
		//str_Pics = CW2A(L"Fine_Wifi_AP");
		passFail = go_runIn_2D( m_i_Index );
	}
	else if (m_str_TestItem == runIn_3D)
	{
		m_str_TestItem = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
		m_strErrorCode = FunErr_GPU_3D_Test_Fail;
		////str_Pics = CW2A(L"Fine_Wifi_AP");
		passFail = go_runIn_3D( m_i_Index );
	}
	else if (m_str_TestItem == runIn_CPU)
	{
		m_str_TestItem = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
		m_strErrorCode = FunErr_Check_CPU_Version_Fail;
		////str_Pics = CW2A(L"Fine_Wifi_AP");
		passFail = go_runIn_CPU( m_i_Index );
	}
	else if (m_str_TestItem == runIn_Audio)
	{
		m_str_TestItem = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
		m_strErrorCode = FunErr_Speaker1_Test_Fail;
		////str_Pics = CW2A(L"Fine_Wifi_AP");
		passFail = go_runIn_Audio( m_i_Index );
	}
	else if (m_str_TestItem == runIn_Video)
	{
		m_str_TestItem = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
		m_strErrorCode = FunErr_Camera_Test_Fail;
		////str_Pics = CW2A(L"Fine_Wifi_AP");
		passFail = go_runIn_Video( m_i_Index );
	}
	else if (m_str_TestItem == runIn_Storage)
	{
		m_str_TestItem = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
		m_strErrorCode = FunErr_Check_eMMC_Fail;
		////str_Pics = CW2A(L"Fine_Wifi_AP");
		passFail = go_runIn_Storage( m_i_Index );
	}
	else if (m_str_TestItem == runIn_Memory)
	{
		m_str_TestItem = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
		m_strErrorCode = FunErr_DDR_ReadWrite_Data_Fail;
		////str_Pics = CW2A(L"Fine_Wifi_AP");
		passFail = go_runIn_Memory( m_i_Index );
	}
	else if (m_str_TestItem == runIn_Sleep)
	{
		m_str_TestItem = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
		m_strErrorCode = FunErr_System_Suspend_Resume_Fail;
		////str_Pics = CW2A(L"Fine_Wifi_AP");
		passFail = go_runIn_Sleep( m_i_Index );
	}
	else if (m_str_TestItem == runIn_Reboot)
	{
		m_str_TestItem = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
		m_strErrorCode = FunErr_System_Reboot_Test_Fail;
		////str_Pics = CW2A(L"Fine_Wifi_AP");
		passFail = go_runIn_Reboot( m_i_Index );
	}
	else if (m_str_TestItem == runIn_Camera)
	{
		m_str_TestItem = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
		m_strErrorCode = FunErr_Switch_Camera_Fail;
		////str_Pics = CW2A(L"Fine_Wifi_AP");
		passFail = go_runIn_Camera( m_i_Index );
	}
	else if (m_str_TestItem == runIn_AudioToRece)
	{
		m_str_TestItem = CStr::IntToStr(BatteryDetectSwitch_BaseItemCode);
		m_strErrorCode = FunErr_Receiver_Test_Fail;
		////str_Pics = CW2A(L"Fine_Wifi_AP");
		passFail = go_runIn_CPU_Loading( m_i_Index );
	}
	//if ( !passFail ) saveLogWhenFail();
	//saveLogWhenFail();
	return passFail;
}



bool CRUN_IN::PostRun()
{
	return true;
}

bool CRUN_IN::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find(std::string("XMLTestItem")) != paramMap.end())
	{
		m_str_TestItem = paramMap[std::string("XMLTestItem")];
	}
	else
	{
		TraceLog(MSG_INFO, "Failed to Get filed 'TestItem' for class 'CRUN_IN'");
		return false;
	}

	std::string str_SleepTime;
	m_i_SleepTime = 60000; // default = 1 min
	if (paramMap.find(std::string("XMLSleepTime")) != paramMap.end())
	{
		str_SleepTime = paramMap[std::string("XMLSleepTime")];
		m_i_SleepTime = CStr::StrToInt(str_SleepTime);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter OnSleepTime for CRUN_IN");
		return false;
	}

	if (paramMap.find(std::string("XMLCMDItem")) != paramMap.end())
	{
		m_str_CMD = paramMap[std::string("XMLCMDItem")];
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter XMLCMDItem for CRUN_IN");
		return false;
	}

	if (paramMap.find(std::string("XMLOffCMDItem")) != paramMap.end())
	{
		m_str_OffCMD = paramMap[std::string("XMLOffCMDItem")];
		m_i_Index = CStr::StrToInt(m_str_OffCMD);
	}
	else
	{
		TraceLog(MSG_INFO, "Fail to find parameter XMLOffCMDItem for CRUN_IN");
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
		TraceLog(MSG_INFO, "Fail to find parameter RetryTime for CRUN_IN");
		return false;
	}


	return true;
}


bool CRUN_IN::copyDeleteLog(){
	bool b_Res = false;
	CFileFind cFileFind;

	b_Res = DeleteFile(str_filePath);//Delete Old Image
	m_strMessage = "Delete LOG Fail ";
	if ( b_Res ) { // del OK
		if(cFileFind.FindFile(str_filePath)){ /*make sure log  is not exist*/
			TraceLog(MSG_ERROR, m_strMessage);
			b_Res = false;
		}else{
			b_Res = true;
		}
	}else{
		TraceLog(MSG_ERROR, m_strMessage);
	}

	return b_Res;

}
bool CRUN_IN::readLog(){
	bool b_Res = false; 
	CStdioFile readFile;
	CString strLine, restoken;
	int curPos=0;	
	int nIndex = 0 ;

	for( int i = 0 ; i <  n_array_upper ; i ++ ) runInResult[i] = -1;

	/* Check image file if exist */
	if(::_taccess(str_filePath, 0) == 0) 
	{
		b_Res = true;
	} 
	else 
	{
		b_Res = false;
	}

	if ( b_Res ) {
		readFile.Open(str_filePath , CFile::modeRead);
		readFile.SeekToBegin(); 
	//	while(readFile.ReadString(strLine))
	//	{
	//  }
		readFile.ReadString(strLine);
		restoken = strLine.Tokenize(",",curPos);
		while(restoken !="")
		{
			runInResult[nIndex] = atoi(restoken) ;
			nIndex++;
			if ( nIndex > n_array_upper ){
				b_Res = false; 
				TraceLog(MSG_INFO, "readLog FAIL!");
				break; 
			}
			restoken = strLine.Tokenize(",",curPos);
		}
		readFile.Close();

	}
	return b_Res;

}

bool CRUN_IN::saveLogWhenFail()
{
	bool b_res = true;

	CString csCommandPull;
	char szOutPullCommand[4096] = {0};
	char szErrorCodePullCommand[4096] = {0};

	CString csDirectory;
	CString csPath;
    CString csLogName;
	CString str_Date;

	CString str_command = _T("");
	CString str_msg = _T("");
	CString str_Picasso = _T("");
	SYSTEMTIME systemTime;
	::GetLocalTime(&systemTime);

	if(! g_strPicasso.empty() )
		str_Picasso = g_strPicasso.c_str();
	else
		str_Picasso = "0000000000";


	/*pull folder to log folder*/
	str_Date.Format(_T("_%04d%02d%02d_%02d%02d%02d"),   systemTime.wYear, systemTime.wMonth, systemTime.wDay,systemTime.wHour, systemTime.wMinute, systemTime.wSecond);

	str_filePath.Format("D:\\Log\\RunIn\\%s_RunIn_%s", str_Picasso, str_Date);
	csCommandPull.Format("%s %s" , _T("pull /data/data/com.oem.runin/"), str_filePath);
	

	if(!ExecAdbOut(csCommandPull, szOutPullCommand, szErrorCodePullCommand, m_i_SleepTime))
	{
		TraceLog(MSG_ERROR,_T("pull RunIn to log folder fail "));
		return false;
		//b_res = false;
	}
	else
	{
		TraceLog(MSG_ERROR,_T("pull RunIn to log folder pass "));
	}
	Sleep(3000);


	/*pull folder to log folder*/
	str_Date.Format(_T("_%04d%02d%02d_%02d%02d%02d"),   systemTime.wYear, systemTime.wMonth, systemTime.wDay,systemTime.wHour, systemTime.wMinute, systemTime.wSecond);

	str_filePath.Format("D:\\Log\\RunIn\\%s_Var_%s", str_Picasso, str_Date);
	csCommandPull.Format("%s %s" , _T("pull /var "), str_filePath);
	

	if(!ExecAdbOut(csCommandPull, szOutPullCommand, szErrorCodePullCommand, 0))
	{
		TraceLog(MSG_ERROR,_T("pull var to log folder fail "));
		return false;
		//b_res = false;
	}
	else
	{
		TraceLog(MSG_ERROR,_T("pull var to log folder pass "));
	}
	Sleep(3000);
	return b_res;

}

bool CRUN_IN::getTargetLog()
{
	bool b_res = true;
	CString csCommandPull;
	char szOutPullCommand[4096] = {0};
	char szErrorCodePullCommand[4096] = {0};

	CString csDirectory;
	CString csPath;
    CString csLogName;
	CString str_Date;

	CString str_command = _T("");
	CString str_msg = _T("");
	CString str_Picasso = _T("");
	SYSTEMTIME systemTime;
	::GetLocalTime(&systemTime);

	if(! g_strPicasso.empty() )
		str_Picasso = g_strPicasso.c_str();
	else
		str_Picasso = "0000000000";

	csLogName.Format("%s", m_str_CMD.c_str());

	_mkdir("D:\\Log");
	_mkdir("D:\\Log\\RunIn");
	str_Date.Format(_T("_%04d%02d%02d_%02d%02d%02d"),   systemTime.wYear, systemTime.wMonth, systemTime.wDay,systemTime.wHour, systemTime.wMinute, systemTime.wSecond);

	/*pull log to log folder*/
	str_filePath.Format("D:\\Log\\RunIn\\%s%s%s", str_Picasso, str_Date, _T(".txt"));
	//csCommandPull.Format("%s %s %s%s%s%s" , _T("pull"), csLogName, csDirectory , _T("\\") ,str_Picasso, _T(".txt"));
	csCommandPull.Format("%s %s %s" , _T("pull"), csLogName, str_filePath);


	if(!ExecAdbOut(csCommandPull, szOutPullCommand, szErrorCodePullCommand, 0))
	{
		TraceLog(MSG_ERROR,_T("pull log to log folder fail"));
		return false;
		//b_res = false;
	}	
	else
	{
		TraceLog(MSG_ERROR,_T("pull log to log folder pass "));
	}

	/*copy to tool folder*/
	csDirectory = GetDirectory();
	str_filePath.Format("%s%s%s%s", csDirectory, "\\", str_Picasso, _T(".txt"));
	csCommandPull.Format("%s %s %s" , _T("pull"), csLogName, str_filePath);

	if(!ExecAdbOut(csCommandPull, szOutPullCommand, szErrorCodePullCommand, 0))
	{
		TraceLog(MSG_ERROR,_T("copy to tool folder fail"));
		b_res = false;
	}	
	else
	{
		TraceLog(MSG_ERROR,_T("copy to tool folder pass "));
	}

	return b_res;
}

CString CRUN_IN::GetDirectory()
{
	CString strDesPath;
	CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));
	strDesPath = pthToolDir;

	CFileFind cfilefind;
	if (!cfilefind.FindFile(strDesPath))
	{
		if (!CreateDirectory(strDesPath, NULL))
		{
			AfxMessageBox(_T("Create folder Img Failed!"));
		}
	}
	return strDesPath;
}


bool CRUN_IN::go_runIn_2D(int nIndex )
{
	bool b_res = false;

	if ( runInResult[nIndex] == 1 ) b_res = true;

	/* test result */
	if(b_res)
	{
		str_msg = CW2A(L"runIn_2D PASS");
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "runIn_2D PASS!");
		SetPICSData(str_Pics, "PASS");
	}
	else
	{
		str_msg = CW2A(L"runIn_2D ");
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "runIn_2D FAIL!");
		SetPICSData(str_Pics, "FAIL");
	}
	
	FactoryLog();
	return b_res;
}

bool CRUN_IN::go_runIn_3D(int nIndex )
{
	bool b_res = false;

	if ( runInResult[nIndex] == 1 ) b_res = true;

	/* test result */
	if(b_res)
	{
		str_msg = CW2A(L"runIn_3D PASS");
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "runIn_3D PASS!");
		SetPICSData(str_Pics, "PASS");
	}
	else
	{
		str_msg = CW2A(L"runIn_3D FAIL");
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "runIn_3D FAIL!");
		SetPICSData(str_Pics, "FAIL");
	}
	
	FactoryLog();
	return b_res;
}

bool CRUN_IN::go_runIn_CPU(int nIndex )
{
	bool b_res = false;

	if ( runInResult[nIndex] == 1 ) b_res = true;

	/* test result */
	if(b_res)
	{
		str_msg = CW2A(L"runIn_CPU PASS");
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "runIn_CPU PASS!");
		SetPICSData(str_Pics, "PASS");
	}
	else
	{
		str_msg = CW2A(L"runIn_CPU FAIL");
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "runIn_CPU FAIL!");
		SetPICSData(str_Pics, "FAIL");
	}
	
	FactoryLog();
	return b_res;
}

bool CRUN_IN::go_runIn_Audio(int nIndex )
{
	bool b_res = false;

	if ( runInResult[nIndex] == 1 ) b_res = true;

	/* test result */
	if(b_res)
	{
		str_msg = CW2A(L"runIn_Audio PASS");
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "runIn_Audio PASS!");
		SetPICSData(str_Pics, "PASS");
	}
	else
	{
		str_msg = CW2A(L"runIn_Audio FAIL");
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "runIn_Audio FAIL!");
		SetPICSData(str_Pics, "FAIL");
	}
	
	FactoryLog();
	return b_res;
}

bool CRUN_IN::go_runIn_CPU_Loading(int nIndex )
{
	bool b_res = false;

	if ( runInResult[nIndex] == 1 ) b_res = true;

	/* test result */
	if(b_res)
	{
		str_msg = CW2A(L"runIn_CPU_Loading PASS");
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "runIn_CPU_Loading PASS!");
		SetPICSData(str_Pics, "PASS");
	}
	else
	{
		str_msg = CW2A(L"runIn_CPU_Loading FAIL");
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "runIn_AudioToRece FAIL!");
		SetPICSData(str_Pics, "FAIL");
	}
	
	FactoryLog();
	return b_res;
}


bool CRUN_IN::go_runIn_Video(int nIndex )
{
	bool b_res = false;

	if ( runInResult[nIndex] == 1 ) b_res = true;

	/* test result */
	if(b_res)
	{
		str_msg = CW2A(L"runIn_Video PASS");
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "runIn_Video PASS!");
		SetPICSData(str_Pics, "PASS");
	}
	else
	{
		str_msg = CW2A(L"runIn_Video FAIL");
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "runIn_Video FAIL!");
		SetPICSData(str_Pics, "FAIL");
	}
	
	FactoryLog();
	return b_res;
}

bool CRUN_IN::go_runIn_Camera(int nIndex )
{
	bool b_res = false;

	if ( runInResult[nIndex] == 1 ) b_res = true;

	/* test result */
	if(b_res)
	{
		str_msg = CW2A(L"runIn_Camera PASS");
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "runIn_Camera PASS!");
		SetPICSData(str_Pics, "PASS");
	}
	else
	{
		str_msg = CW2A(L"runIn_Camera FAIL");
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "runIn_Camera FAIL!");
		SetPICSData(str_Pics, "FAIL");
	}
	
	FactoryLog();
	return b_res;
}

bool CRUN_IN::go_runIn_Storage(int nIndex )
{
	bool b_res = false;

	if ( runInResult[nIndex] == 1 ) b_res = true;

	/* test result */
	if(b_res)
	{
		str_msg = CW2A(L"runIn_Storage PASS");
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "runIn_Storage PASS!");
		SetPICSData(str_Pics, "PASS");
	}
	else
	{
		str_msg = CW2A(L"runIn_Storage FAIL");
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "runIn_Storage FAIL!");
		SetPICSData(str_Pics, "FAIL");
	}
	
	FactoryLog();
	return b_res;
}

bool CRUN_IN::go_runIn_Memory(int nIndex )
{
	bool b_res = false;

	if ( runInResult[nIndex] == 1 ) b_res = true;

	/* test result */
	if(b_res)
	{
		str_msg = CW2A(L"runIn_Memory PASS");
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "runIn_Memory PASS!");
		SetPICSData(str_Pics, "PASS");
	}
	else
	{
		str_msg = CW2A(L"runIn_Memory FAIL");
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "runIn_Memory FAIL!");
		SetPICSData(str_Pics, "FAIL");
	}
	
	FactoryLog();
	return b_res;
}

bool CRUN_IN::go_runIn_Sleep(int nIndex )
{
	bool b_res = false;

	if ( runInResult[nIndex] == 1 ) b_res = true;

	/* test result */
	if(b_res)
	{
		str_msg = CW2A(L"runIn_Sleep PASS");
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "runIn_Sleep PASS!");
		SetPICSData(str_Pics, "PASS");
	}
	else
	{
		str_msg = CW2A(L"runIn_Sleep FAIL");
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "runIn_Sleep FAIL!");
		SetPICSData(str_Pics, "FAIL");
	}
	
	FactoryLog();
	return b_res;
}

bool CRUN_IN::go_runIn_Reboot(int nIndex )
{
	bool b_res = false;

	if ( runInResult[nIndex] == 1 ) b_res = true;

	/* test result */
	if(b_res)
	{
		str_msg = CW2A(L"runIn_Reboot PASS");
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "runIn_Reboot PASS!");
		SetPICSData(str_Pics, "PASS");
	}
	else
	{
		str_msg = CW2A(L"runIn_Reboot FAIL");
		m_strResult = "FAIL";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, "runIn_Reboot FAIL!");
		SetPICSData(str_Pics, "FAIL");
	}
	
	FactoryLog();
	return b_res;
}

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
bool CRUN_IN::ExecAdbOut(CString Command, char* output, char* ErrorCode, int nSleep)
{
	bool isOk = false;
	if (nSleep == 0) nSleep = 60000;  // sleep 6 second

	DWORD nPipeSize = 1024 * 1024; //1M pipeline

	WCHAR   DllPath[MAX_PATH] = {0};
	GetModuleFileNameW((HINSTANCE)&__ImageBase, DllPath, _countof(DllPath));

	CString pthToolDir(DllPath);

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
		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, nSleep); // timeout in 10 seconds

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