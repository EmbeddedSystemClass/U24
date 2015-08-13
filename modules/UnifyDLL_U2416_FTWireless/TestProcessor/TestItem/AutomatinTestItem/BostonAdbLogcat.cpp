
#include "stdafx.h"
#include "BostonAdbLogcat.h"
#include <direct.h>

RF_IMPLEMENT_DYNCREATE(CBostonAdbLogcat)

bool CBostonAdbLogcat::InitData(std::map<std::string, std::string>& paramMap)
{
	return true;
}

bool CBostonAdbLogcat::PreRun()
{
	m_strResult    = "";
	m_strItemCode  = CStr::IntToStr(LSensor_Calibration_BaseItemCode);
	m_strErrorCode = "";
	m_strBand      = "-";
	m_strChannel   = "-";
	m_strUpper     = "";
	m_strLower     = "";
	m_strMeasured  = "";
	m_strUnit      = "-";
	m_strMessage   = "";

	return true;
}

bool CBostonAdbLogcat::Run()
{
	return runAdbLogcat(true);
}

bool CBostonAdbLogcat::PostRun()
{
	return true;
}

bool CBostonAdbLogcat::runAdbLogcat(bool b_IsPass)
{
	char str_LogPath[1000] = _T("D:\\Log");  
	CString str_Date;
	SYSTEMTIME systemTime;
	::GetLocalTime(&systemTime);

	_mkdir(str_LogPath);
	strcat(str_LogPath, _T("\\Android"));
	_mkdir(str_LogPath);
	str_Date.Format(_T("\\%04d%02d%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay);
	strcat(str_LogPath, str_Date);
	_mkdir(str_LogPath);

	CString str_command = _T("");
	CString str_msg = _T("");
	CString str_Picasso = _T("RD");
	if(! g_strPicasso.empty() )
		str_Picasso = g_strPicasso.c_str();
	if(b_IsPass)
	{
		str_command.Format(_T("/C adb logcat -v time -d > %s\\PASS_logcat_%s_%04d%02d%02d_%02d%02d%02d.txt"), str_LogPath, str_Picasso,
																			  systemTime.wYear, systemTime.wMonth, systemTime.wDay,
																			  systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
	}
	else
	{
		str_command.Format(_T("/C adb logcat -v time -d > %s\\FAIL_logcat_%s_%04d%02d%02d_%02d%02d%02d.txt"), str_LogPath, str_Picasso,
																			  systemTime.wYear, systemTime.wMonth, systemTime.wDay,
																			  systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
	}
	//ShellExecute(0, "open", "cmd", str_command, 0, SW_HIDE);
	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = "open";
	ShExecInfo.lpFile = "cmd";	
	ShExecInfo.lpParameters = str_command;	
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, 3000); 

	str_msg.Format(_T("Execute: %s"), str_command);
	TraceLog(MSG_INFO, std::string(CT2A(str_msg)));

	return true;
}
