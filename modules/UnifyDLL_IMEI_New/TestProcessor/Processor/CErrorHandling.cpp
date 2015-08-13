#include "stdafx.h"
#include "CErrorHandling.h"
#include "../../QualDLL/QualDLL_API.h"
#include "../../QualDLL/QPacket.h"
#include <comdef.h> // _com_error in HwDetectSetup

std::string IErrorHandling::m_strResult;
std::string IErrorHandling::m_strErrorCode;
std::string IErrorHandling::m_strMessage;

RF_IMPLEMENT_DYNCREATE(CErrorHandling)

CErrorHandling::CErrorHandling()
:m_DeviceMode(MODE_UNKNOWN)
{
	m_strResult = "";
	m_strErrorCode = "";
	m_strMessage = "";
}

bool CErrorHandling::SearchPortUsb(const UINT secTimeout)
{
	UINT timer = 0;
	bool hasPort = false;
	m_com.port.Empty();
	m_com.desc.Empty();
	while (!hasPort && timer <= secTimeout)
	{
		hasPort = FindUsbDevice();
		Sleep(1000);
		timer++;
	}

	if (hasPort)
	{
		if (m_DeviceMode == MODE_DLOAD)
		{
			DownloadQcomUsbLog();
		}
		else if (m_DeviceMode == MODE_FASTBOOT)
		{
			FastbootDump();
			m_strErrorCode = CommErr_Device_In_Fastboot_Mode;
			m_strMessage = "Device In Fastboot Mode";
		}
		else
		{
			m_strErrorCode = CommErr_Get_ADB_Fail;
			m_strMessage = "Fail to find ADB device of phone.";
		}
	}
	else
	{
		m_strErrorCode = CommErr_Get_ADB_Fail;
		m_strMessage = "Fail to find ADB device of phone.";
	}

	return hasPort;
}

bool CErrorHandling::FindUsbDevice()
{
	HDEVINFO hDevInfo = SetupDiGetClassDevs(NULL, _T("USB"), NULL, (DIGCF_ALLCLASSES | DIGCF_PRESENT));
	if (INVALID_HANDLE_VALUE == hDevInfo) {
		AfxMessageBox(CString(_T("SetupDiGetClassDevs(): "))
		              + _com_error(GetLastError()).ErrorMessage(), MB_ICONEXCLAMATION);
		return false;
	}

	bool isFound = false;
	SP_DEVINFO_DATA spDevInfoData;
	spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for (int i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &spDevInfoData); i++) {
		DWORD nSize = 0;
		TCHAR DeviceInstanceId[MAX_PATH];
		if (!SetupDiGetDeviceInstanceId(hDevInfo, &spDevInfoData, DeviceInstanceId, sizeof(DeviceInstanceId), &nSize)) {
			TRACE(_T("SetupDiGetDeviceInstanceId(): %s\n"), _com_error(GetLastError()).ErrorMessage());
			isFound = false;
			break;
		}
		if (FindDeviceId(DeviceInstanceId)) {
			m_com.desc = GetDeviceDesc(hDevInfo, spDevInfoData);
			if (GetComPort() || GetAdbDevice()) {
				TRACE(_T("Find device: %s\n"), m_com.desc);
				isFound = true;
				break;
			}
		}
	}
	SetupDiDestroyDeviceInfoList(hDevInfo);

	return isFound;
}

bool CErrorHandling::FindDeviceId(CString sDeviceId)
{
	bool isFound =
		sDeviceId.Find(_T("USB\\VID_05C6&PID_9008")) != -1 ||
		sDeviceId.Find(_T("USB\\VID_0918&PID_0101")) != -1 ||
		sDeviceId.Find(_T("USB\\VID_1D45&PID_0138")) != -1;
	if (isFound) {
		if (sDeviceId.Find(_T("USB\\VID_05C6&PID_9008")) != -1) {
			m_DeviceMode = MODE_DLOAD;
		}
		else if (sDeviceId.Find(_T("USB\\VID_0918&PID_0101")) != -1 || sDeviceId.Find(_T("USB\\VID_1D45&PID_0138")) != -1) {
			m_DeviceMode = MODE_FASTBOOT;
		}
		else {
			m_DeviceMode = MODE_UNKNOWN;
		}
	}

	return isFound;
}

bool CErrorHandling::GetComPort()
{
	int nStart = m_com.desc.Find(_T("(COM"));
	int nEnd   = m_com.desc.ReverseFind(_T(')'));
	if (nStart == -1 || nEnd == -1 || (nEnd - nStart) < 5) {
		m_com.port.Empty();
		return false;
	}
	m_com.port = m_com.desc.Mid(nStart + 1, nEnd - nStart - 1);
	return !m_com.port.IsEmpty();
}

bool CErrorHandling::GetAdbDevice()
{
	return (m_com.desc.Find(_T("Android ADB Interface")) != -1);
}

CString CErrorHandling::GetDeviceDesc(HDEVINFO& hDevInfo, SP_DEVINFO_DATA& spDevInfoData)
{
	DWORD DataT;
	TCHAR buffer[MAX_PATH];
	DWORD nSize = 0;
	if (SetupDiGetDeviceRegistryProperty(
		hDevInfo, &spDevInfoData, SPDRP_FRIENDLYNAME, &DataT, (PBYTE)buffer, sizeof(buffer), &nSize)) {
	}
	else if (SetupDiGetDeviceRegistryProperty(hDevInfo, &spDevInfoData,
	         SPDRP_DEVICEDESC, &DataT, (PBYTE)buffer, sizeof(buffer), &nSize)) {
	}
	return CString(buffer);
}

bool CErrorHandling::DownloadQcomUsbLog()
{
	bool bRes = false;
	HANDLE hResource = QDL_Initial(CT2A(m_com.port));

	CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));

	CString str_hex = pthToolDir + _T("\\MPRG8930.hex");
	//CString str_fastboot = pthToolDir + _T("\\8930_fastbootimage.mbn");
	int nRet = QDL_Upload_MultiImage_8K_Emergency(hResource, str_hex.GetBuffer(), _T(""), 0, 0);
	switch (nRet)
	{
	case NO_ERROR:
		m_strErrorCode = CommErr_Device_In_eDL_Mode;
		m_strMessage = "Device In eDL Mode";
		break;
	case ERROR_QCOM_DMSS_CMD_BASE + DMSS_CMD_NOP:
		m_strErrorCode = DLErr_DL_Dmss_Nop_Fail;
		m_strMessage = "DMSS NOP Fail";
		break;
	case ERROR_QCOM_DMSS_CMD_BASE + DMSS_CMD_PARAMS:
		m_strErrorCode = DLErr_DL_Dmss_Parameter_Fail;
		m_strMessage = "DMSS PARAMETER Fail";
		break;
	case ERROR_QCOM_DMSS_CMD_BASE + DMSS_CMD_WRITE_32BIT:
		m_strErrorCode = DLErr_DL_Dmss_Write32BIT_Fail;
		m_strMessage = "DMSS Write32BIT Fail";
		break;
	case ERROR_QCOM_DMSS_CMD_BASE + DMSS_CMD_GO:
		m_strErrorCode = DLErr_DL_Dmss_Go_Fail;
		m_strMessage = "DMSS GO Fail";
		break;
	case ERROR_QCOM_STRMNG_CMD_BASE + STREAMING_CMD_HELLO:
		m_strErrorCode = DLErr_DL_Streaming_Hello_Fail;
		m_strMessage = "Streaming Hello Fail";
		break;
	case ERROR_QCOM_STRMNG_CMD_BASE + STREAMING_CMD_SECMODE:
		m_strErrorCode = DLErr_DL_Streaming_Security_Fail;
		m_strMessage = "Streaming Security Fail";
		break;
	case ERROR_QCOM_STRMNG_CMD_BASE + STREAMING_CMD_OPENMIMAGE:
		m_strErrorCode = DLErr_DL_Streaming_Open_Image_Fail;
		m_strMessage = "Streaming Open Image Fail";
		break;
	case ERROR_QCOM_STRMNG_CMD_BASE + STREAMING_CMD_READ:
		m_strErrorCode = DLErr_Read_PicassoNum_Fail;
		m_strMessage = "Read Picasso number Fail";
		break;
	case ERROR_QCOM_STRMNG_CMD_BASE + STREAMING_CMD_SBLREAD:
		m_strErrorCode = DLErr_Dump_SBL_Log_Fail;
		m_strMessage = "Dump SBL Log Fail";
		break;
	case ERROR_QCOM_STRMNG_CMD_BASE + STREAMING_CMD_PBLREAD:
		m_strErrorCode = DLErr_Dump_PBL_Log_Fail;
		m_strMessage = "Dump PBL Log Fail";
		break;
	default:
		m_strErrorCode = CommErr_Device_In_eDL_Mode;
		m_strMessage = "Device In eDL Mode";
		break;
	}

	bRes = (nRet == NO_ERROR) ? true : false;

	QDL_Close(hResource);
	return bRes;
}

void CErrorHandling::FastbootDump()
{
	if(_taccess("D:\\Log\\FASTBOOT\\", 0) == -1)
	{
		system("mkdir D:\\Log\\FASTBOOT\\");
	}
	
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	CString str_SysTime;
	str_SysTime.Format(_T("%04d%02d%02d_%02d%02d%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);

	CString currentDir;
	GetModuleFileName(NULL, currentDir.GetBufferSetLength(MAX_PATH+1), MAX_PATH);
	currentDir.ReleaseBuffer();
	currentDir = currentDir.Left(currentDir.ReverseFind('\\'));

	CString str_command;
	str_command.Format(_T("/C %s\\fastboot.exe dump dprintf D:\\Log\\FASTBOOT\\%s.txt"), currentDir, str_SysTime);
	ShellExecute(0, "open", "cmd", str_command, 0, SW_HIDE);
}