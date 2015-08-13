#include "stdafx.h"
#include "QDLCtrl.h"

#include <string>
#include <ctime> // clock_t
#include <setupapi.h>
#pragma comment(lib, "setupapi")

#include "QPacket.h"
#include "Diagcmd.h"
//#include "NV_ItemHolder.h"
#include "QUALDLL_API.h"

#pragma warning(disable: 4996)

CString m_sPort;
CString m_sDesc;

static inline void endian_swap(WORD& x)
{
    x = (x>>8) | 
        (x<<8);
}

static inline void endian_swap(DWORD& x)
{
    x = (x>>24) | 
        ((x<<8) & 0x00FF0000) |
        ((x>>8) & 0x0000FF00) |
        (x<<24);
}

static bool CheckInstance(TCHAR* szKey)
{
	HANDLE mutex = CreateMutex(NULL, TRUE, szKey);
	if (mutex) {
		if (GetLastError() == ERROR_ALREADY_EXISTS) {
			CloseHandle(mutex);
			return true;
		}
	}
	CloseHandle(mutex);
	return false;
}

/**
 * get com port string COMX
 */
static bool GetComPort()
{
	int nStart = m_sDesc.Find(_T("(COM"));
	int nEnd   = m_sDesc.ReverseFind(_T(')'));
	if (nStart == -1 || nEnd == -1 || (nEnd - nStart) < 5) {
		m_sPort.Empty();
		return false;
	}
	m_sPort = m_sDesc.Mid(nStart + 1, nEnd - nStart - 1);

	return !m_sPort.IsEmpty();
}

static bool GetAdbDevice()
{
	return
		(m_sDesc.Find(_T("Android Sooner Single ADB Interface")) != -1) ||
		(m_sDesc.Find(_T("Android ADB Interface")) != -1);
}

static bool FindDeviceId(CString sDeviceId)
{
	bool isFound =
		sDeviceId.Find(_T("USB\\VID_05C6&PID_900E\\")) != -1 || // 7227A DLoad
		sDeviceId.Find(_T("USB\\VID_05C6&PID_9008")) != -1 || // 7227A Emergency
		sDeviceId.Find(_T("USB\\VID_0502&PID_337E\\")) != -1 || // 7227A DLoad Acer
		sDeviceId.Find(_T("USB\\VID_0502&PID_9008")) != -1 || // 7227A Emergency Acer
		sDeviceId.Find(_T("USB\\VID_18D1&PID_D00D")) != -1 || // fastboot
		sDeviceId.Find(_T("USB\\VID_0502&PID_337F\\")) != -1; // fastboot

	return isFound;
}

// get Friendly Name or Device Description
static CString GetDeviceName(HDEVINFO& hDevInfo, SP_DEVINFO_DATA& spDevInfoData)
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

static bool FindUsbDevice()
{
	HDEVINFO hDevInfo = SetupDiGetClassDevs(NULL, _T("USB"), NULL, (DIGCF_ALLCLASSES | DIGCF_PRESENT));
	if (INVALID_HANDLE_VALUE == hDevInfo) {
		return false;
	}

	bool isFound = false;
	SP_DEVINFO_DATA spDevInfoData;
	spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for (int i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &spDevInfoData); i++) {
		DWORD nSize = 0;
		TCHAR DeviceInstanceId[MAX_PATH];
		if (!SetupDiGetDeviceInstanceId(hDevInfo, &spDevInfoData, DeviceInstanceId, sizeof(DeviceInstanceId), &nSize)) {
			isFound = false;
			break;
		}
		if (FindDeviceId(DeviceInstanceId)) {
			m_sDesc = GetDeviceName(hDevInfo, spDevInfoData);
			if (GetComPort() || GetAdbDevice()) {
				TRACE(_T("Find device: %s\n"), m_sDesc);
				isFound = true;
				break;
			}
		}
	}
	SetupDiDestroyDeviceInfoList(hDevInfo);

	return isFound;
}

/**
 * Search dload/diagnostic com port or ADB device
 */
bool SearchPortUsb(const UINT secTimeout)
{
	UINT timer = 0;
	bool hasPort = false;
	m_sPort.Empty();
	m_sDesc.Empty();
	for (;;) {
		hasPort = FindUsbDevice();
		if (hasPort || timer >= secTimeout) {
			break;
		}
		Sleep(1000);
		timer++;
	}

	return hasPort;
}

CQDLCtrl::CQDLCtrl()
{
	//m_fp//SendMsg = NULL;
	m_isSecureBoot = false;
}

CQDLCtrl::~CQDLCtrl()
{
}

void CQDLCtrl::SetSendMsg(LPVOID pfunc)
{
	//m_fpSendMsg = (MsgCallBack)pfunc;
}

void CQDLCtrl::SendMsg(const char* szInfo, int nStatus)
{/*
	if (m_fpSendMsg != NULL)
	{
		m_fpSendMsg(szInfo, nStatus);
	}*/
}

int CQDLCtrl::ComSet(char* szPort)
{
	m_ComPort = szPort;
	return NO_ERROR;
}

int CQDLCtrl::ComOpen(bool isEmdl)
{
	int nRetCode = ERROR_OPEN_FAILED;
	if (!m_ComPort.IsEmpty()) {
		if (m_SerCom.Open(m_ComPort, isEmdl) == TRUE) {
			nRetCode = NO_ERROR;
		}
		else {
			m_SerCom.Close();
		}
	}
	Sleep(3000); //// Wait OPEN COM port finish
	return nRetCode;
}

int CQDLCtrl::ComClose()
{
	int nRetCode = ERROR_SERIAL_NO_DEVICE;
	if (!m_ComPort.IsEmpty()) {
		nRetCode = m_SerCom.Close();
		Sleep(500);
	}
	return nRetCode;
}

int CQDLCtrl::CmdDiagReadVersion(char* OsblVersion)
{
	req_cmd_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.code = DMSS_CMD_VERREQ;

	rsp_dmss_version_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nRetCode = ERROR_FUNCTION_FAILED;
	for (int i = 0; i < QCMD_MAX_RETRY; i++) {
		nRetCode = m_SerCom.TxRx((char*)&Req, sizeof(Req), (char*)&Rsp, sizeof(Rsp), QCMD_DELAY_SHORT);
		TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
		if (nRetCode == NO_ERROR) {
			if (Rsp.code == DMSS_CMD_VERRSP) {
				sprintf(OsblVersion, "%s", Rsp.version);
				break;
			}
			else {
				nRetCode = ERROR_QCOM_DMSS_CMD_BASE + DMSS_CMD_VERREQ;
				memset(OsblVersion, 0, sizeof(OsblVersion));
			}
		}
	}
	return nRetCode;
}

int CQDLCtrl::CmdDlSetFlag(DWORD nAddress, char* flag, int nLength)
{
	req_dmss_write_32b_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.code = DMSS_CMD_WRITE_32BIT;
	DWORD wAddress = nAddress;
	endian_swap(wAddress);
	memcpy(Req.address, &wAddress, sizeof(DWORD));

	Req.length[1] = nLength;
	memcpy(Req.data, &flag, nLength);

	rsp_cmd_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nSize = nLength + 7;
	int nRetCode = ERROR_FUNCTION_FAILED;
	for (int i = 0; i < QCMD_MAX_RETRY; i++) {
		nRetCode = m_SerCom.TXRXDl((char*)&Req, nSize, (char*)&Rsp, sizeof(Rsp), QCMD_DELAY_SHORT);
		TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
		TRACE(_T("%s nRet:%d Rsp.data:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.data[0]);
		TRACE(_T("%s nRet:%d Rsp.data:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.data[1]);
		if (nRetCode == NO_ERROR) {
			if (Rsp.code == DMSS_CMD_ACK) {
				break;
			}
			else {
				nRetCode = ERROR_QCOM_DMSS_CMD_BASE + DMSS_CMD_WRITE_32BIT;
			}
		}
	}
	return nRetCode;
}

int CQDLCtrl::CmdDiagReset()
{
	req_cmd_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.code = DMSS_CMD_RESET;

	rsp_cmd_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nRetCode = ERROR_FUNCTION_FAILED;
	for (int i = 0; i < 2; i++) {
		nRetCode = m_SerCom.TXRXDiag((char*)&Req, sizeof(Req), (char*)&Rsp, sizeof(Rsp), QCMD_DELAY_LONG);
		TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
		if (nRetCode == NO_ERROR && Rsp.code == DMSS_CMD_ACK)
			break;
		else {
			nRetCode = ERROR_QCOM_DMSS_CMD_BASE + DMSS_CMD_RESET;
		}
	}
	return nRetCode;
}

int CQDLCtrl::CmdDlHexReset()
{
	req_cmd_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.code = STREAMING_CMD_RESET;

	rsp_cmd_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nRetCode = ERROR_FUNCTION_FAILED;
	for (int i = 0; i < QCMD_MAX_RETRY; i++) {
		nRetCode = m_SerCom.TXRXDl((char*)&Req, sizeof(Req), (char*)&Rsp, sizeof(Rsp), QCMD_DELAY_SHORT);
		TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
		if (nRetCode == NO_ERROR && Rsp.code == STREAMING_CMD_RESETACK)
			break;
		else {
			nRetCode = ERROR_QCOM_STRMNG_CMD_BASE + STREAMING_CMD_RESET;
		}
		if (Rsp.code == STREAMING_CMD_LOG) {
			TRACE(_T("%s target error:%s"), _T(__FUNCTION__), Rsp.data);
			CString dbg_msg;
			dbg_msg.Format(_T("#DEBUG: %s target error:%s"), _T(__FUNCTION__), Rsp.data);
			dbg_msg.Replace(_T("\n"), _T(""));
			//SendMsg(dbg_msg.GetBuffer(), QMSG_WARNING);
			dbg_msg.ReleaseBuffer();
		}
	}
	return nRetCode;
}

int CQDLCtrl::ProbeEmdl()
{
	m_SerCom.SetState(CSerialIO::EDL);
	int nRetCode = ComOpen(true);
	if (nRetCode != NO_ERROR) { // ERROR_CONNECTION_UNAVAIL 1201
		//SendMsg("ERROR[DL1007]: Open emergency COM port Failed!", QMSG_ERROR);
	}
	else {
		//SendMsg("Open emergency COM port success", QMSG_NORMAL);
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDiagProbeEmdl();
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1002]: Send Probe Failed!", QMSG_ERROR);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlNop();
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1002]: Test communication Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Test communication success", QMSG_NORMAL);
		}
	}
	return nRetCode;
}

int CQDLCtrl::Nop()
{
	return CmdDlNop();
}

int CQDLCtrl::GetVersion(char* OsblVersion)
{
	m_SerCom.SetState(CSerialIO::EDIAG);
	int nRetCode = ComOpen();
	if (nRetCode != NO_ERROR) {
		//SendMsg("ERROR[DL1007]: Open COM port Failed!", QMSG_ERROR);
	}
	else {
		//SendMsg("Open COM port success", QMSG_NORMAL);
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDiagReadVersion(OsblVersion);
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode != NO_ERROR) {
		//SendMsg("ERROR[DL1000]: Read OSBL version Failed!", QMSG_ERROR);
	}
	else {
		char szMessage[MAX_PATH];
		sprintf(szMessage, "OSBL version: %s", OsblVersion);
		memset(m_szOsblVersion, 0, sizeof(m_szOsblVersion));
		strncpy(m_szOsblVersion, OsblVersion, strlen(OsblVersion));
		//SendMsg(szMessage, QMSG_WARNING);
	}
	ComClose();

	return nRetCode;
}

int CQDLCtrl::SetFastbootSerial(char* szSerial)
{
	if (strlen(szSerial) != 8) {
		return ERROR_INVALID_DATA;
	}

	m_SerCom.SetState(CSerialIO::EDIAG);
	int nRetCode = DloadOpen();
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlSetFlag(0xf0040, szSerial, 8);
	}
	//////////////////////////////////////////////////////////////////////////
	ComClose();

	std::string msg;
	if (nRetCode != NO_ERROR) {
		msg = msg + "ERROR[DL1000]: #Set fastboot serial Failed! (" + szSerial + ")";
		//SendMsg(msg.c_str(), QMSG_ERROR);
	}
	else {
		msg = msg + "#Set fastboot serial success (" + szSerial + ")";
		//SendMsg(msg.c_str(), QMSG_NORMAL);
	}

	return nRetCode;
}

int CQDLCtrl::SetFastbootFlag()
{
	m_SerCom.SetState(CSerialIO::EDIAG);
	int nRetCode = DloadOpen();
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlSetFlag(0xf0000, "Fast Boot Flag", 14);
	}
	//////////////////////////////////////////////////////////////////////////
	ComClose();

	if (nRetCode != NO_ERROR) {
		//SendMsg("ERROR[DL1000]: #Set fastboot flag Failed", QMSG_ERROR);
	}
	else {
		//SendMsg("#Set fastboot flag success", QMSG_NORMAL);
	}

	return nRetCode;
}

int CQDLCtrl::SetFastbootFlag7k(bool IsSetFastbootFlag)
{
	UINT nFileSize = 16;
	byte* pData = new byte[nFileSize];
	int add1 = 0xFFFEF808;
	int data1 = 0xA4BC385F;
	int add2 = 0xFFFEF80C;
	int data2;
	// boot reason
	if (IsSetFastbootFlag) {
		//SendMsg("Set fastboot flag", QMSG_NORMAL);
		data2 = RD_HEX_2_FASTBOOT;
	}
	else {
		//SendMsg("Set reboot flag", QMSG_NORMAL);
		data2 = NONE_MODE;
	}
	memcpy(&pData[0], &add1, 4);
	memcpy(&pData[4], &data1, 4);
	memcpy(&pData[8], &add2, 4);
	memcpy(&pData[12], &data2, 4);
	int nRetCode = CmdSetFastbootFlag(pData, nFileSize, 2);
	delete [] pData;

	return nRetCode;
}

int CQDLCtrl::SetFastbootFlag8k(bool IsSetFastbootFlag)
{
	int nRetCode = ERROR_FUNCTION_FAILED;
	DWORD wAddress = 0;
	byte* pBuffer = new byte[1];
	memcpy(&pBuffer[0], "1", 1);
	nRetCode = CmdDlHexUploadFile(&(pBuffer[0]), 1, 0x003ffe00);
	delete[] pBuffer;
	return nRetCode;
}

int CQDLCtrl::Reset(char* pthHex)
{
	m_SerCom.SetState(CSerialIO::EDIAG);
	int nRetCode = ComOpen();
	if (nRetCode != NO_ERROR) {
		//SendMsg("ERROR[DL1007]: Open COM port Failed!", QMSG_ERROR);
	}
	else {
		//SendMsg("Open COM port success", QMSG_NORMAL);
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDiagReset();
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode != NO_ERROR) {
		//SendMsg("Reset Failed!", QMSG_WARNING);
	}
	else {
		//SendMsg("Reset success", QMSG_NORMAL);
	}

	return nRetCode;
}

int CQDLCtrl::SwitchDiag2Dload()
{
	m_SerCom.SetState(CSerialIO::EDIAG);
	int nRetCode = ComOpen();
	if (nRetCode != NO_ERROR) {
		//SendMsg("ERROR[DL1007]: Open COM port Failed!", QMSG_ERROR);
	}
	else {
		//SendMsg("Open COM port success", QMSG_NORMAL);
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDiagProbe();
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1002]: Send Probe Failed!", QMSG_ERROR);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDiagSwitchToDlad();
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1013]: Switch to DLOAD mode Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Switch to DLOAD mode success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	ComClose();
	return nRetCode;
}

int CQDLCtrl::UploadMultiImageEmergency(char* pthHex, char* pthPartition,
										char* pthDbl, char* pthFsbl, char* pthOsbl,
										bool IsRePartition)
{
	TCHAR* szKeyCsdToucan = _T("{93DC659F-0E54-4ed4-9A87-A8942D376B32}");
	bool isCsd = CheckInstance(szKeyCsdToucan);

	m_SerCom.SetState(CSerialIO::EDL);
	int nRetCode = DloadOpen();
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		if (isCsd) {
			//SendMsg("[ACTION]: EBI2_LB_N off", QMSG_WARNING);
			::AfxMessageBox(_T("[ACTION]: EBI2_LB_N off"));
		}
		nRetCode = RunHex(pthHex);
		if (nRetCode == ERROR_TIMEOUT) {
			//SendMsg("Run HEX timeout ... skip", QMSG_NORMAL);
		}
		else if (nRetCode != NO_ERROR) {
			// 2010-11-12 Mick: try to skip this fail case
			//SendMsg("ERROR[DL1000]: Run HEX Failed!", QMSG_WARNING);
			nRetCode = ERROR_TIMEOUT;
		}
		else {
			//SendMsg("Run HEX success", QMSG_NORMAL);
		}
		if (nRetCode == NO_ERROR || nRetCode == ERROR_TIMEOUT) {
			if (isCsd) {
				//SendMsg("[ACTION]: WDOG_EN off", QMSG_WARNING);
				::AfxMessageBox(_T("[ACTION]: WDOG_EN off"));
			}
			nRetCode = CmdDlHexHello();
			if (nRetCode != NO_ERROR) {
				//SendMsg("ERROR[DL1000]: Streaming hello Failed!", QMSG_ERROR);
			}
			else {
				//SendMsg("Streaming hello success", QMSG_NORMAL);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlHexSecurity(0x0);
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Send Security Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Send Security success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadPartition(pthPartition, IsRePartition);
		if (nRetCode == STREAMING_CMD_PATTTIONTR + QPTL_DIFFER) {
			//SendMsg("ERROR[DL2000]: Partition table differs, override will be accepted!", QMSG_ERROR);
		}
		else if (nRetCode == STREAMING_CMD_PATTTIONTR + QPTL_ERASE_FAIL) {
			//SendMsg("ERROR[DL2001]: Partition erase Failed!", QMSG_ERROR);
		}
		else if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL2002]: Download partition table Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Download partition table success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode =
			UploadALLImage(false, pthDbl, pthFsbl, pthOsbl,
				NULL, NULL, NULL, NULL, NULL, NULL);
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		if (CmdDlHexReset() != NO_ERROR) {
			//SendMsg("Reset Failed!", QMSG_WARNING);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	ComClose();

	return nRetCode;
}

int CQDLCtrl::UploadMultiImage(char* pthHex, char* pthPartition,
							   char* pthDbl,  char* pthFsbl, char* pthOsbl,
							   char* pthAmss, char* pthDsp1, char* pthEfs2,
							   char* pthAppsbl, char* pthApps, char* pthFlash,
							   bool isEraseEFS2, bool IsRePartition,
							   SYNCHRONIZATION_t* sync)
{
	m_SerCom.SetState(CSerialIO::EDIAG);
	int nRetCode = DloadOpen();
	//////////////////////////////////////////////////////////////////////////
	::InterlockedIncrement(&sync->count);
	WaitForMultiThread(&sync->count, sync->number_of_threads, 100*1000); // 100 sec
	EnterCriticalSection(&sync->cs);
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = RunHex(pthHex);
		if (nRetCode == ERROR_TIMEOUT) {
			//SendMsg("Run HEX timeout ... skip", QMSG_NORMAL);
		}
		else if (nRetCode != NO_ERROR) {
			// 2010-11-12 Mick: try to skip this fail case
			//SendMsg("ERROR[DL1000]: Run HEX Failed!", QMSG_WARNING);
			nRetCode = ERROR_TIMEOUT;
		}
		else {
			//SendMsg("Run HEX success", QMSG_NORMAL);
		}
		if (nRetCode == NO_ERROR || nRetCode == ERROR_TIMEOUT) {
			nRetCode = CmdDlHexHello();
			if (nRetCode != NO_ERROR) {
				//SendMsg("ERROR[DL1000]: Streaming hello Failed!", QMSG_ERROR);
			}
			else {
				//SendMsg("Streaming hello success", QMSG_NORMAL);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	LeaveCriticalSection(&sync->cs);
	::InterlockedIncrement(&sync->count);
	WaitForMultiThread(&sync->count, (sync->number_of_threads) * 2, 100*1000); // 100 sec
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlHexSecurity(0x01);
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Send Security Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Send Security success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadPartition(pthPartition, IsRePartition);
		if (nRetCode == STREAMING_CMD_PATTTIONTR + QPTL_DIFFER) {
			//SendMsg("ERROR[DL2000]: Partition table differs, override will be accepted!", QMSG_ERROR);
		}
		else if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL2002]: Download partition table Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Download partition table success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode =
			UploadALLImage(IsRePartition, pthDbl, pthFsbl, pthOsbl,
				pthAmss, pthDsp1, pthEfs2, pthAppsbl, pthApps, pthFlash);
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR && isEraseEFS2) {
		nRetCode = EraseEFS2();
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Erase EFS2 Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Erase EFS2 success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlHexReset();
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Reset Failed!", QMSG_ERROR);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	ComClose();

	return nRetCode;
}

int CQDLCtrl::UploadMultiImageSpeedup(
		char* pthHex, char* pthPartition,
		char* pthDbl, char* pthFsbl, char* pthOsbl,
		char* pthAmss,   char* pthDsp1, char* pthEfs2,
		char* pthAppsbl, char* pthApps, char* pthFlash, char* pthDt,
		char* pthBkFsbl, char* pthBkOsbl, char* pthBkDt, char* pthBkAppsbl, char* pthReserved,
		bool isEraseEFS2, bool IsRePartition, SYNCHRONIZATION_t* sync)
{
	m_SerCom.SetState(CSerialIO::EDIAG);
	int nRetCode = DloadOpen();
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadALLImageToDDR(
					pthDbl, pthFsbl, pthOsbl,
					pthAmss, pthDsp1, pthEfs2,
					pthAppsbl, pthApps, pthFlash, pthDt,
					pthBkFsbl, pthBkOsbl, pthBkDt, pthBkAppsbl, pthReserved);
	}
	//////////////////////////////////////////////////////////////////////////
	::InterlockedIncrement(&sync->count);
	WaitForMultiThread(&sync->count , sync->number_of_threads , 100000);
	EnterCriticalSection(&sync->cs);
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = RunHex(pthHex);
		if (nRetCode == ERROR_TIMEOUT) {
			//SendMsg("Run HEX timeout ... skip", QMSG_NORMAL);
		}
		else if (nRetCode != NO_ERROR) {
			// 2010-11-12 Mick: try to skip this fail case
			//SendMsg("ERROR[DL1000]: Run HEX Failed!", QMSG_WARNING);
			nRetCode = ERROR_TIMEOUT;
		}
		else {
			//SendMsg("Run HEX success", QMSG_NORMAL);
		}
		if (nRetCode == NO_ERROR || nRetCode == ERROR_TIMEOUT) {
			nRetCode = CmdDlHexHello();
			if (nRetCode != NO_ERROR) {
				//SendMsg("ERROR[DL1000]: Streaming hello Failed!", QMSG_ERROR);
			}
			else {
				//SendMsg("Streaming hello success", QMSG_NORMAL);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	LeaveCriticalSection(&sync->cs);
	::InterlockedIncrement(&sync->count);
	WaitForMultiThread(&sync->count, (sync->number_of_threads)*2, 100000);
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlHexSecurity(0x01);
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Send Security mode Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Send Security mode success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadPartition(pthPartition, IsRePartition);
		if (nRetCode == STREAMING_CMD_PATTTIONTR + QPTL_DIFFER) {
			//SendMsg("ERROR[DL2000]: Partition table differs, override will be accepted!", QMSG_ERROR);
		}
		else if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL2002]: Download partition table Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Download partition table success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadALLImageSpeedup(
					pthDbl, pthFsbl, pthOsbl,
					pthAmss, pthDsp1, pthEfs2,
					pthAppsbl, pthApps, pthFlash, pthDt,
					pthBkFsbl, pthBkOsbl, pthBkDt, pthBkAppsbl, pthReserved);
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR && isEraseEFS2) {
		nRetCode = EraseEFS2();
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Erase EFS2 Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Erase EFS2 success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlHexReset();
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Reset Failed!", QMSG_ERROR);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	ComClose();

	return nRetCode;
}

int CQDLCtrl::UploadMultiImage7k(char* pthHex, char* pthPartition,
								 char* pthQcsblHd, char* pthQcsbl, char* pthOemsblHd, char* pthOemsbl,
								 char* pthAmssHd,  char* pthAmss,  char* pthAppsblHd, char* pthAppsbl,
								 char* pthAppsHd,  char* pthApps,  bool IsRePartition,
								 bool IsSetFastbootFlag, bool IsEraseEFS2)
{
	m_SerCom.SetState(CSerialIO::EDIAG);
	int nRetCode = DloadOpen();
	//////////////////////////////////////////////////////////////////////////
	// retry 2 times
	if (nRetCode == NO_ERROR) {
		nRetCode = RunHex(pthHex);
		if (nRetCode == ERROR_TIMEOUT) {
			//SendMsg("Run HEX timeout ... skip", QMSG_NORMAL);
		}
		else if (nRetCode != NO_ERROR) {
			// 2010-11-12 Mick: try to skip this fail case
			//SendMsg("ERROR[DL1000]: Run HEX Failed!", QMSG_WARNING);
			nRetCode = ERROR_TIMEOUT;
		}
		else {
			//SendMsg("Run HEX success", QMSG_NORMAL);
		}
		if (nRetCode == NO_ERROR || nRetCode == ERROR_TIMEOUT) {
			nRetCode = CmdDlHexHello();
			if (nRetCode != NO_ERROR) {
				//SendMsg("ERROR[DL1000]: Streaming hello Failed! Retry again.", QMSG_WARNING);
			}
			else {
				//SendMsg("Streaming hello success", QMSG_NORMAL);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode != NO_ERROR) {
		ComClose();
		// find new COM port
		SearchPortUsb(1);
		ComSet(CT2A(m_sPort));
		nRetCode = DloadOpenRetry();
		if (nRetCode == NO_ERROR || nRetCode == ERROR_TIMEOUT) {
			nRetCode = CmdDlHexHello();
			if (nRetCode != NO_ERROR) {
				//SendMsg("ERROR[DL1000]: Streaming hello Failed!", QMSG_ERROR);
			}
			else {
				//SendMsg("Streaming hello success", QMSG_NORMAL);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlHexSecurity(0x01);
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Send Security mode Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Send Security mode success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		if (IsRePartition) {
			//SendMsg("Override partition.", QMSG_NORMAL);
		}
		nRetCode = UploadPartition(pthPartition, IsRePartition);
		if (nRetCode != NO_ERROR) {
			if (nRetCode == 0x0000001b) {
				//SendMsg("ERROR[DL2000]: Need to override partition!", QMSG_ERROR);
			}
			else {
				//SendMsg("ERROR[DL2000]: Download partition table Failed!", QMSG_ERROR);
			}
		}
		else {
			//SendMsg("Download partition table success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadALLImage7k(pthQcsblHd, pthQcsbl, pthOemsblHd, pthOemsbl,
				pthAmssHd, pthAmss, pthAppsblHd, pthAppsbl, pthAppsHd, pthApps);
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR && IsEraseEFS2) {
		nRetCode = EraseEFS2();
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Erase EFS2 Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Erase EFS2 success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// set fastboot flag
	if (nRetCode == NO_ERROR) {
		nRetCode = SetFastbootFlag7k(IsSetFastbootFlag);
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR: Set Fastboot Flag Failed!", QMSG_ERROR);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlHexReset();
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Reset Failed!", QMSG_ERROR);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	ComClose();

	return nRetCode;
}

int CQDLCtrl::UploadMultiImage7kEmergency(char* pthHex, char* pthPartition,
								 char* pthQcsblHd, char* pthQcsbl, char* pthOemsblHd, char* pthOemsbl,
								 char* pthAmssHd,  char* pthAmss,  char* pthAppsblHd, char* pthAppsbl,
								 char* pthAppsHd,  char* pthApps,  bool IsRePartition,
								 bool IsSetFastbootFlag, bool IsEraseEFS2)
{
	m_SerCom.SetState(CSerialIO::EDL);
	int nRetCode = DloadOpen();
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = RunHexEmergency(pthHex);
		if (nRetCode == ERROR_TIMEOUT) {
			//SendMsg("Run HEX timeout ... skip", QMSG_NORMAL);
		}
		else if (nRetCode != NO_ERROR) {
			// 2010-11-12 Mick: try to skip this fail case
			//SendMsg("ERROR[DL1000]: Run HEX Failed!", QMSG_WARNING);
			nRetCode = ERROR_TIMEOUT;
		}
		else {
			//SendMsg("Run HEX success", QMSG_NORMAL);
		}
		if (nRetCode == NO_ERROR || nRetCode == ERROR_TIMEOUT) {
			nRetCode = CmdDlHexHello();
			if (nRetCode != NO_ERROR) {
				//SendMsg("ERROR[DL1000]: Streaming hello Failed! Retry again.", QMSG_WARNING);
			}
			else {
				//SendMsg("Streaming hello success", QMSG_NORMAL);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode != NO_ERROR) {
		ComClose();
		// find new COM port
		SearchPortUsb(1);
		ComSet(CT2A(m_sPort));
		nRetCode = DloadOpenRetry();
		if (nRetCode == NO_ERROR || nRetCode == ERROR_TIMEOUT) {
			nRetCode = CmdDlHexHello();
			if (nRetCode != NO_ERROR) {
				//SendMsg("ERROR[DL1000]: Streaming hello Failed!", QMSG_ERROR);
			}
			else {
				//SendMsg("Streaming hello success", QMSG_NORMAL);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlHexSecurity(0x01);
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Send Security mode Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Send Security mode success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		if (IsRePartition) {
			//SendMsg("Override partition.", QMSG_NORMAL);
		}
		nRetCode = UploadPartition(pthPartition, IsRePartition);
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL2000]: Download partition table Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Download partition table success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadALLImage7k(pthQcsblHd, pthQcsbl, pthOemsblHd, pthOemsbl,
				pthAmssHd, pthAmss, pthAppsblHd, pthAppsbl, pthAppsHd, pthApps);
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR && IsEraseEFS2) {
		nRetCode = EraseEFS2();
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Erase EFS2 Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Erase EFS2 success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// set fastboot flag
	if (nRetCode == NO_ERROR && IsSetFastbootFlag) {
		nRetCode = SetFastbootFlag7k(IsSetFastbootFlag);
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR: Set Fastboot Flag Failed!", QMSG_ERROR);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlHexReset();
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Reset Failed!", QMSG_ERROR);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	ComClose();

	return nRetCode;
}

int CQDLCtrl::UploadMultiImage8kEmergency(char* pthHex, char* pthFastboot,
								 bool IsSetFastbootFlag, bool IsEraseEFS2)
{
	m_SerCom.SetState(CSerialIO::EDL);
	int nRetCode = DloadOpen8K();
	//////////////////////////////////////////////////////////////////////////
	// retry 2 times
	if (nRetCode == NO_ERROR) {
		nRetCode = RunHex(pthHex);
		if (nRetCode == ERROR_TIMEOUT) {
			//SendMsg("Run HEX timeout ... skip", QMSG_NORMAL);
		}
		else if (nRetCode != NO_ERROR) {
			// 2010-11-12 Mick: try to skip this fail case
			//SendMsg("ERROR[DL1000]: Run HEX Failed!", QMSG_WARNING);
			nRetCode = ERROR_TIMEOUT;
		}
		else {
			//SendMsg("Run HEX success", QMSG_NORMAL);
		}
		if (nRetCode == NO_ERROR || nRetCode == ERROR_TIMEOUT) {
			nRetCode = CmdDlHexHello();
			if (nRetCode != NO_ERROR) {
				//SendMsg("ERROR[DL1000]: Streaming hello Failed! Retry again.", QMSG_WARNING);
			}
			else {
				//SendMsg("Streaming hello success", QMSG_NORMAL);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlHexSecurity(0x01);
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Send Security mode Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Send Security mode success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		UploadImage(Q8K_OPEN_MULTI_MODE_EMMC_USER, pthFastboot, "FASTBOOT_IMAGE");
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR && IsEraseEFS2) {
		nRetCode = EraseEFS2();
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Erase EFS2 Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Erase EFS2 success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// set fastboot flag
	if (nRetCode == NO_ERROR) {
		nRetCode = SetFastbootFlag8k(IsSetFastbootFlag);
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR: Set Fastboot Flag Failed!", QMSG_ERROR);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlHexReset();
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Reset Failed!", QMSG_ERROR);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	ComClose();

	return nRetCode;
}

int CQDLCtrl::UploadMultiImage8kEmergencyAndIRAMRead(char* pthHex, char* pthFastboot,
								 bool IsSetFastbootFlag, bool IsEraseEFS2)
{
	m_SerCom.SetState(CSerialIO::EDL);
	int nRetCode = DloadOpen8K();
	//////////////////////////////////////////////////////////////////////////
	// retry 2 times
	if (nRetCode == NO_ERROR) {
		nRetCode = RunHex(pthHex);
		if (nRetCode == ERROR_TIMEOUT) {
			//SendMsg("Run HEX timeout ... skip", QMSG_NORMAL);
		}
		else if (nRetCode != NO_ERROR) {
			// 2010-11-12 Mick: try to skip this fail case
			//SendMsg("ERROR[DL1000]: Run HEX Failed!", QMSG_WARNING);
			nRetCode = ERROR_TIMEOUT;
		}
		else {
			//SendMsg("Run HEX success", QMSG_NORMAL);
		}
		if (nRetCode == NO_ERROR || nRetCode == ERROR_TIMEOUT) {
			nRetCode = CmdDlHexHello();
			if (nRetCode != NO_ERROR) {
				//SendMsg("ERROR[DL1000]: Streaming hello Failed! Retry again.", QMSG_WARNING);
			}
			else {
				//SendMsg("Streaming hello success", QMSG_NORMAL);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlHexSecurity(0x01);
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Send Security mode Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Send Security mode success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlHexOpenImage(Q8K_OPEN_MULTI_MODE_EMMC_USER);
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Send Security mode Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Send Security mode success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	BYTE dumpBuffer[2048];

	if (nRetCode == NO_ERROR) {
		memset(dumpBuffer, 0, sizeof(dumpBuffer));
		CmdDlStreamingStrRead(dumpBuffer);
	}

	CString strFilePath;
	CString strFileName((LPCSTR)dumpBuffer, 2048);
	strFileName = strFileName.Mid(14, 10);

	// Dump SBL log
	if (nRetCode == NO_ERROR) {
		if(_taccess("D:\\Log\\eMMC\\", 0) == -1)
		{
			system("mkdir D:\\Log\\eMMC\\");
		}
		strFilePath.Format(_T("D:\\Log\\eMMC\\%s_SBL.bin"), strFileName);
	}

	if (nRetCode == NO_ERROR) {
		memset(dumpBuffer, 0, sizeof(dumpBuffer));
		CmdDlStreamingIRAMRead(dumpBuffer, 0x200, 0x2F000);
	}

	if(strFilePath.IsEmpty() == 0 && nRetCode == NO_ERROR)
	{
		FILE *pFile;
		pFile = fopen (strFilePath, "wb");
		fwrite (dumpBuffer , 1 , sizeof(dumpBuffer)-1 , pFile);
		fclose (pFile);
	}

	// Dump PBL log
	if (nRetCode == NO_ERROR) {
		if(_taccess("D:\\Log\\eMMC\\", 0) == -1)
		{
			system("mkdir D:\\Log\\eMMC\\");
		}
		strFilePath.Format(_T("D:\\Log\\eMMC\\%s_PBL.bin"), strFileName);
	}

	if (nRetCode == NO_ERROR) {
		memset(dumpBuffer, 0, sizeof(dumpBuffer));
		CmdDlStreamingIRAMRead(dumpBuffer, 0x200, 0x43F18);
	}

	if(strFilePath.IsEmpty() == 0 && nRetCode == NO_ERROR)
	{
		FILE *pFile;
		pFile = fopen (strFilePath, "wb");
		fwrite (dumpBuffer , 1 , sizeof(dumpBuffer)-1 , pFile);
		fclose (pFile);
	}
	//////////////////////////////////////////////////////////////////////////

	ComClose();

	return nRetCode;
}

int CQDLCtrl::DloadOpen()
{
	int nRetCode = ERROR_FUNCTION_FAILED;
	nRetCode = ComOpen(true);
	if (nRetCode != NO_ERROR) {
		//SendMsg("ERROR[DL1007]: Open COM port Failed!", QMSG_ERROR);
	}
	else {
		//SendMsg("Open COM port success", QMSG_NORMAL);
	}
	switch (m_SerCom.GetState()) {
	case CSerialIO::EDL:
		if (nRetCode == NO_ERROR) {
			nRetCode = CmdDiagProbeEmdl();
			if (nRetCode != NO_ERROR) {
				//SendMsg("ERROR[DL1002]: Send EM Probe Failed!", QMSG_ERROR);
			}
		}
		break;
	case CSerialIO::EDIAG:
		if (nRetCode == NO_ERROR) {
			nRetCode = CmdDiagProbe();
			if (nRetCode != NO_ERROR) {
				//SendMsg("ERROR[DL1002]: Send Probe Failed!", QMSG_ERROR);
			}
		}
		if (nRetCode == NO_ERROR) {
			nRetCode = SwitchToDload();
			if (nRetCode != NO_ERROR) {
				//SendMsg("ERROR[DL1013]: Change to DLOAD mode Failed!", QMSG_ERROR);
			}
		}
		break;
	default:
		break;
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlNop();
		if (nRetCode == NO_ERROR) {
			//SendMsg("Test communication success", QMSG_NORMAL);
		}
		else {
			//SendMsg(_T("ERROR[DL1002]: Test communication Failed!"), QMSG_ERROR);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlParameter();
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Request parameter Failed!", QMSG_ERROR);
		}
	}
	return nRetCode;
}

int CQDLCtrl::DloadOpen8K()
{
	int nRetCode = ERROR_FUNCTION_FAILED;
	nRetCode = ComOpen(true);
	if (nRetCode != NO_ERROR) {
		//SendMsg("ERROR[DL1007]: Open COM port Failed!", QMSG_ERROR);
	}
	else {
		//SendMsg("Open COM port success", QMSG_NORMAL);
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlNop();
		if (nRetCode == NO_ERROR) {
			//SendMsg("Test communication success", QMSG_NORMAL);
		}
		else {
			//SendMsg(_T("ERROR[DL1002]: Test communication Failed!"), QMSG_ERROR);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlParameter();
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Request parameter Failed!", QMSG_ERROR);
		}
	}
	return nRetCode;
}

int CQDLCtrl::DloadOpenRetry()
{
	int nRetCode = ERROR_FUNCTION_FAILED;
	nRetCode = ComOpen(true);
	if (nRetCode != NO_ERROR) {
		//SendMsg("ERROR[DL1007]: Open COM port Failed!", QMSG_ERROR);
	}
	else {
		//SendMsg("Open COM port success", QMSG_NORMAL);
	}
	return nRetCode;
}

int CQDLCtrl::SwitchToDload()
{
	int nRetCode = ERROR_FUNCTION_FAILED;
	switch (m_SerCom.GetState()) {
	case CSerialIO::EDL:
		nRetCode = NO_ERROR;
		break;
	case CSerialIO::EDIAG:
		Sleep(100);		//// Wait ReadFile function
		CmdDiagSwitchToDlad();
		ComClose();
		m_SerCom.SetState(CSerialIO::EDL);
		nRetCode = ComOpen();
		Sleep(10000);	//// Wait USB Config
		break;
	default:
		break;
	}
	return nRetCode;
}

int CQDLCtrl::RunHex(const char* path)
{
	int nRetCode = ERROR_FUNCTION_FAILED;
	WORD HexAddress = 0;
	if (IsFileExist(path) == NO_ERROR) {
		// 2010-01-20 Mick: secure boot check
		// 2010-06-02 Mick: JZ request - disable secure boot check signed hex, pre D/L disable check all
		CHexFile HexLauncher;
		if (HexLauncher.Open(path, CFile::modeRead | CFile::shareDenyNone) == FALSE) {
			//SendMsg("ERROR[DL1000]: Open HEX Failed!", QMSG_ERROR);
		}
		else {
			HexAddress = HexLauncher.GetHexAddrss();
			DWORD wAddress = HexAddress<<16;
			UINT nSize = 0;
			do {
				byte buffer[HEX_BUFFER_SIZE] = {0};
				nSize = HexLauncher.Read((char*)buffer, HEX_BUFFER_SIZE);
				nRetCode = CmdDlUploadHex(buffer, nSize, wAddress);
				wAddress += nSize;
				if (nRetCode != NO_ERROR) {
					break;
				}
			} while (nSize == HEX_BUFFER_SIZE);
			HexLauncher.Close();
		}
	}
	else {
		nRetCode = ERROR_FILE_NOT_FOUND;
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlGo(HexAddress, 0);
	}

	return nRetCode;
}

int CQDLCtrl::RunHexEmergency(const char* path)
{
	int nRetCode = ERROR_FUNCTION_FAILED;
	WORD HexAddress = 0;
	if (IsFileExist(path) == NO_ERROR) {
		// 2010-01-20 Mick: secure boot check
		// 2010-06-02 Mick: JZ request - disable secure boot check signed hex, pre D/L disable check all
		CHexFile HexLauncher;
		if (HexLauncher.Open(path, CFile::modeRead | CFile::shareDenyNone) == FALSE) {
			//SendMsg("ERROR[DL1000]: Open HEX Failed!", QMSG_ERROR);
		}
		else {
			HexAddress = HexLauncher.GetHexAddrss();
			DWORD wAddress = HexAddress<<16;
			wAddress = wAddress + 0x3000;
			UINT nSize = 0;
			do {
				byte buffer[HEX_BUFFER_SIZE] = {0};
				nSize = HexLauncher.Read((char*)buffer, HEX_BUFFER_SIZE);
				nRetCode = CmdDlUploadHex(buffer, nSize, wAddress);
				wAddress += nSize;
				if (nRetCode != NO_ERROR) {
					break;
				}
			} while (nSize == HEX_BUFFER_SIZE);
			HexLauncher.Close();
		}
	}
	else {
		nRetCode = ERROR_FILE_NOT_FOUND;
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlGo(HexAddress, 0x3000);
	}

	return nRetCode;
}

int CQDLCtrl::UploadPartition(char* path, unsigned char bOverride)
{
	int nRetCode = ERROR_FUNCTION_FAILED;
	if (IsFileExist(path) == NO_ERROR) {
		CFile fPartition;
		if (fPartition.Open(path, CFile::modeRead | CFile::shareDenyNone) == FALSE) {
			//SendMsg("ERROR[DL1000]: Open Partition Failed!", QMSG_ERROR);
		}
		else {
			UINT nFileSize = (UINT)fPartition.GetLength();
			byte* pData = new byte[nFileSize];
			fPartition.Read(pData, nFileSize);
			fPartition.Close();
			nRetCode = CmdDlHexUploadPartition(pData, nFileSize, bOverride);
			delete [] pData;
		}
	}

	return nRetCode;
}

// IsRePartition to check restore
int CQDLCtrl::UploadALLImage(bool IsRePartition,
							 char* pthDbl, char* pthFsbl, char* pthOsbl,
							 char* pthAmss, char* pthDsp1, char* pthEfs2,
							 char* pthAppsbl, char* pthApps, char* pthFlash)
{
	int nRetCode = ERROR_FUNCTION_FAILED;

	// boot code
	nRetCode = UploadImage(OPEN_MODE_DBL, pthDbl, "DBL");
	if (nRetCode != NO_ERROR) {
		return nRetCode;
	}
	nRetCode = UploadImage(OPEN_MODE_FSBL, pthFsbl, "FSBL");
	if (nRetCode != NO_ERROR) {
		return nRetCode;
	}
	nRetCode = UploadImage(OPEN_MODE_OSBL, pthOsbl, "OSBL");
	if (nRetCode != NO_ERROR) {
		return nRetCode;
	}

	// test loop to upload OSBL
	//CString msg;
	//for (int i = 0; i < 1000; i++) {
	//	msg.Format("#DEBUG: test re-download count %d", i + 1);
	//	//SendMsg(msg, QMSG_NORMAL);
	//	nRetCode = UploadImage(OPEN_MODE_OSBL, pthOsbl, "OSBL");
	//	if (nRetCode != SUCCESS) {
	//		break;
	//	}
	//}

	// modem code
	nRetCode = UploadImage(OPEN_MODE_AMSS, pthAmss, "AMSS");
	if (nRetCode != NO_ERROR) {
		return nRetCode;
	}
	nRetCode = UploadImage(OPEN_MODE_DSP1, pthDsp1, "DSP1");
	if (nRetCode != NO_ERROR) {
		return nRetCode;
	}
	nRetCode = UploadImage(OPEN_MODE_CEFS, pthEfs2, "EFS2");
	if (nRetCode != NO_ERROR) {
		return nRetCode;
	}
	// boot loader
	nRetCode = UploadImage(OPEN_MODE_APPSBL, pthAppsbl, "APPSBL");
	if (nRetCode != NO_ERROR) {
		return nRetCode;
	}
	// os image
	nRetCode = UploadImage(OPEN_MODE_APPS, pthApps, "APPS");
	if (nRetCode != NO_ERROR) {
		return nRetCode;
	}
	nRetCode = UploadImage(OPEN_MODE_FLASH_BIN, pthFlash, "Flash");
	if (nRetCode != NO_ERROR) {
		return nRetCode;
	}
	/**
	* OSBL hsusb download
	* test to download by checking the last number
	*  8650B--AUSB1AM-05 support new way to download DT image and restore RESERVED data
    *  02-3180-AUSB1XM-07   not restore SIMSECURE
    *  03-3180-AUSB1XM-07   not restore SIMSECURE
    *  04-3180-AUSB1XM-07   not restore SIMSECURE
    *  05-3180-AUSB1XM-07   support restore SIMSECURE
	*/
	// *MUST* after boot code download
	size_t nVersion = atoi(&m_szOsblVersion[strlen(m_szOsblVersion) - 2]);
	size_t nHwId = atoi(&m_szOsblVersion[0]);
	bool hasRestore = IsRePartition &&
		strlen(m_szOsblVersion) > 0 && nVersion >= OSBL_RESERVED &&
		(strstr(m_szOsblVersion, PTN_AUSTIN) != NULL ||
		strstr(m_szOsblVersion, PTN_TOUCAN) != NULL);
	if (hasRestore) {
		if (nVersion >= OSBL_RESERVED) {
			if (UploadImageSpeedup(OPEN_MODE_RESERVED, "", "reserved data") != NO_ERROR) {
				//SendMsg("#Restore reserved data Failed!", QMSG_ERROR);
			}
			else {
				//SendMsg("#Restore reserved data success", QMSG_NORMAL);
			}
		}
	}
	return nRetCode;
}

int CQDLCtrl::UploadImage(unsigned char ImageType, char* pthImage, char* Part)
{
	int nRetCode = ERROR_FUNCTION_FAILED;
	if (pthImage != NULL && strlen(pthImage) > 0) {
		// 2010-06-02 Mick: JZ request - disable secure boot check signed hex, pre D/L disable check all
#ifndef PRE_DOWNLOAD
		if (m_isSecureBoot) { // 2010-01-20 Mick: secure boot check
			int CheckHeader = CheckSignedHeaderMbn(pthImage);
			int CheckInfo   = CheckSignedInfoMbn(pthImage);
			if (CheckHeader == NO_ERROR && CheckInfo == NO_ERROR) {
				// pass
			}
			else if (CheckHeader == ERROR_FILE_INVALID && CheckInfo == ERROR_FILE_INVALID) {
				// skip
			}
			else {
				// fail
				CString message;
				message.Format(_T("ERROR[DL1008]: secure boot is enabled, but image is not signed\n%s"), pthImage);
				//SendMsg(message, QMSG_ERROR);
				return ERROR_FILE_NOT_ENCRYPTED;
			}
		}
#endif //PRE_DOWNLOAD

		nRetCode = CmdDlHexOpenImage(ImageType);
		if (nRetCode == NO_ERROR) {
			nRetCode = UploadFile(pthImage);
		}
		if (nRetCode == NO_ERROR) {
			CmdDlHexCloseImage(); // skip check close
		}

		CString message;
		MessageType type = QMSG_NORMAL;
		if (nRetCode == NO_ERROR) {
			type = QMSG_NORMAL;
			message.Format("Download %s success", Part);
		}
		else {
			type = QMSG_ERROR;
			message.Format("Download %s Fail!", Part);
			if (strncmp(Part, "DBL", 3) == 0) {
				message = "ERROR[DL2003]" + message;
			}
			if (strncmp(Part, "FSBL", 4) == 0) {
				message = "ERROR[DL2004]" + message;
			}
			if (strncmp(Part, "OSBL", 4) == 0) {
				message = "ERROR[DL2005]" + message;
			}
			if (strncmp(Part, "AMSS", 4) == 0) {
				message = "ERROR[DL2006]" + message;
			}
			if (strncmp(Part, "DSP1", 4) == 0) {
				message = "ERROR[DL2007]" + message;
			}
		}
		//SendMsg(message.GetBuffer(), type);
		message.ReleaseBuffer();
	}
	else {
		nRetCode = NO_ERROR; // by pass
	}
	return nRetCode;
}

int CQDLCtrl::UploadFile(char* pthImage)
{
	int nRetCode = ERROR_FUNCTION_FAILED;
	if (IsFileExist(pthImage) == NO_ERROR) {
		CFile fImage;
		if (fImage.Open(pthImage, CFile::modeRead | CFile::shareDenyNone) == FALSE) {
			//SendMsg("Open Failed!", QMSG_ERROR);
		}
		else {
			DWORD nImageSize = (DWORD)fImage.GetLength();
			byte* pBuffer = new byte[nImageSize];
			memset(pBuffer, 0, nImageSize*sizeof(byte));
			fImage.Read(pBuffer, nImageSize);
			fImage.Close();
#ifdef _DEBUG
			clock_t start = clock();
#endif
			DWORD wAddress = 0;
			DWORD nBytesWrite = 0;
			CString msgProgress = "";
			DWORD prevProgress = 0;
			DWORD currProgress = 0;
			do {
				if (nImageSize - nBytesWrite >= PKT_BUFFER_SIZE) {
					nRetCode = CmdDlHexUploadFile(&(pBuffer[nBytesWrite]), PKT_BUFFER_SIZE, wAddress);
				}
				else {
					nRetCode = CmdDlHexUploadFile(&(pBuffer[nBytesWrite]), nImageSize % PKT_BUFFER_SIZE, wAddress);
				}
				if (nRetCode != NO_ERROR) {
					break;
				}
				nBytesWrite += PKT_BUFFER_SIZE;
				wAddress += PKT_BUFFER_SIZE;
				currProgress = nBytesWrite / (nImageSize / 100);
				if (currProgress > prevProgress) {
					msgProgress.Format(_T(" %3.d %% completed"), currProgress);
					prevProgress = currProgress;
					//SendMsg((LPCTSTR)msgProgress, QMSG_NORMAL);
				}
			} while (nBytesWrite < nImageSize);
			delete[] pBuffer;

#ifdef _DEBUG
			clock_t end = clock();
			CString dbg_msg;
			dbg_msg.Format(_T("#DEBUG: upload image - size: %7d, time: %02.03f sec"),
				nImageSize, static_cast<double>(end - start) / CLOCKS_PER_SEC);
			//SendMsg(dbg_msg, QMSG_WARNING);
#endif
		}
	}
	return nRetCode;
}

int CQDLCtrl::IsFileExist(const char * path)
{
	if (path == NULL || strlen(path) == 0)  {
		return ERROR_PATH_NOT_FOUND;
	}

	int nRetCode = ERROR_FILE_INVALID;
	DWORD dwFileSize;
	HANDLE hFile = ::CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL,
	                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		CString message;
		message.Format(_T("File not found: %s"), path);
		//SendMsg(message, QMSG_ERROR);
	}
	else {
		dwFileSize = ::GetFileSize(hFile , NULL);
		if (dwFileSize < 128) {
			//SendMsg("Invalid file!", QMSG_ERROR);
		}
		else {
			nRetCode = NO_ERROR;
		}
		::CloseHandle(hFile);
	}

	return nRetCode;
}

int CQDLCtrl::UploadALLImageToDDR(
		char* pthDbl, char* pthFsbl, char* pthOsbl,
		char* pthAmss, char* pthDsp1, char* pthEfs2,
		char* pthAppsbl, char* pthApps, char* pthFlash, char* pthDt,
		char* pthBkFsbl, char* pthBkOsbl, char* pthBkDt, char* pthBkAppsbl, char* pthReserved)
{
	int nRetCode = NO_ERROR;

	// boot code
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageToDDR(OPEN_MODE_DBL, pthDbl, "DBL");
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageToDDR(OPEN_MODE_FSBL, pthFsbl, "FSBL");
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageToDDR(OPEN_MODE_OSBL, pthOsbl, "OSBL");
	}
	// modem code
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageToDDR(OPEN_MODE_AMSS, pthAmss, "AMSS");
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageToDDR(OPEN_MODE_DSP1, pthDsp1, "DSP1");
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageToDDR(OPEN_MODE_CEFS, pthEfs2, "EFS2");
	}
	// boot loader
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageToDDR(OPEN_MODE_DT, pthDt, "DT");
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageToDDR(OPEN_MODE_APPSBL, pthAppsbl, "APPSBL");
	}
	// os image
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageToDDR(OPEN_MODE_APPS, pthApps, "APPS");
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageToDDR(OPEN_MODE_FLASH_BIN, pthFlash, "Flash");
	}
	// 2009-09-07 Mick: backup partition
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageToDDR(OPEN_MODE_BAK_FSBL, pthBkFsbl, "backup FSBL");
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageToDDR(OPEN_MODE_BAK_OSBL, pthBkOsbl, "backup OSBL");
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageToDDR(OPEN_MODE_BAK_DT, pthBkDt, "backup DT");
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageToDDR(OPEN_MODE_BAK_APPSBL, pthBkAppsbl, "backup APPSBL");
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageToDDR(OPEN_MODE_RESERVED, pthReserved, "RESERVED");
	}

	return nRetCode;
}

int CQDLCtrl::UploadImageToDDR(unsigned char ImageType, char* pthImage, char* Part)
{
	if (pthImage == NULL || strlen(pthImage) == 0) {
		return NO_ERROR; // no download
	}

#ifdef _DEBUG
	clock_t start = clock();
#endif // _DEBUG

	// 2010-06-02 Mick: JZ request - disable secure boot check signed hex, pre D/L disable check all
#ifndef PRE_DOWNLOAD
	if (m_isSecureBoot) { // 2010-01-20 Mick: secure boot check
		int CheckHeader = CheckSignedHeaderMbn(pthImage);
		int CheckInfo   = CheckSignedInfoMbn(pthImage);
		if (CheckHeader == NO_ERROR && CheckInfo == NO_ERROR) {
			// pass
		}
		else if (CheckHeader == ERROR_FILE_INVALID && CheckInfo == ERROR_FILE_INVALID) {
			// skip
		}
		else { // fail
			CString message;
			message.Format(_T("ERROR[DL1008]: secure boot is enabled, but image is not signed\n%s"), pthImage);
			//SendMsg(message, QMSG_ERROR);
			return ERROR_INVALID_DATA;
		}
	}
#endif // PRE_DOWNLOAD

	int nRetCode = CmdUploadImageToDDR(ImageType, pthImage);
	CString message;
	MessageType type = QMSG_NORMAL;
	if (nRetCode == NO_ERROR) {
		type = QMSG_NORMAL;
		message.Format("#%s to DDR success", Part);
	}
	else {
		type = QMSG_ERROR;
		message.Format("#%s to DDR Fail! (%d)", Part, nRetCode);
		if (strncmp(Part, "DBL", 3) == 0) {
			message = "#ERROR[DL2003]" + message;
		}
		if (strncmp(Part, "FSBL", 4) == 0) {
			message = "#ERROR[DL2004]" + message;
		}
		if (strncmp(Part, "OSBL", 4) == 0) {
			message = "#ERROR[DL2005]" + message;
		}
		if (strncmp(Part, "AMSS", 4) == 0) {
			message = "#ERROR[DL2006]" + message;
		}
		if (strncmp(Part, "DSP1", 4) == 0) {
			message = "#ERROR[DL2007]" + message;
		}
		if (strncmp(Part, "DT", 2) == 0) {
			message = "#ERROR[DL2008]" + message;
		}
		if (strncmp(Part, "RESERVED", 8) == 0) {
			message = "#ERROR[DL2009]" + message;
		}
	}
	//SendMsg(message.GetBuffer(), type);
	message.ReleaseBuffer();

#ifdef _DEBUG
	clock_t end = clock();
	CString dbg_msg;
	dbg_msg.Format(_T("#DEBUG: send image - type: %d, time: %02.03f sec"),
		ImageType, static_cast<double>(end - start) / CLOCKS_PER_SEC);
	//SendMsg(dbg_msg, QMSG_WARNING);
#endif // _DEBUG

	return nRetCode;
}

int CQDLCtrl::CmdUploadImageToDDR(unsigned char ImageType, char* pthImage)
{
	if (IsFileExist(pthImage) != NO_ERROR) {
		return ERROR_FILE_NOT_FOUND;
	}

	req_qth_dmss_ddr_write_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.cmd_code = QTH_DMSS_CMD_HS_WRITE;
	Req.image_type = ImageType;
	CFile fImage;
	if (fImage.Open(pthImage, CFile::modeRead | CFile::shareDenyNone) < 0)
		return ERROR_FILE_INVALID;
	DWORD nSize = (DWORD)fImage.GetLength();
	fImage.Close();
	endian_swap(nSize); // pack as big endian
	memcpy(Req.image_size, &nSize, sizeof(DWORD));
#ifdef _DEBUG
	CString msg;
	endian_swap(nSize); // re-swap endian for debug message
	msg.Format(_T("#DEBUG: send image - type: %d, size: %d bytes"), ImageType, nSize);
	//SendMsg(msg.GetBuffer(), QMSG_WARNING);
	msg.ReleaseBuffer();
#endif //_DEBUG

	rsp_cmd_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nRetCode = ERROR_FUNCTION_FAILED;
	for (int i = 0; i < 2; i++) {
		nRetCode = m_SerCom.TxRxSpeedUp((char*)&Req, sizeof(Req), (char*)&Rsp, sizeof(Rsp), pthImage, QCMD_DELAY_SHORT);
		TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
		if (nRetCode == NO_ERROR) {
			if (Rsp.code == QTH_DMSS_CMD_HS_WRITE_RSP) {
				break;
			}
			else {
				CString dbg_msg;
				dbg_msg.Format(_T("#DEBUG: %s to DDR #%d"), strrchr(pthImage, '\\') + 1);
				//SendMsg(dbg_msg.GetBuffer(), QMSG_WARNING);
				dbg_msg.ReleaseBuffer();
				ComClose();
				ComOpen(false); // this command must in USB DLOAD mode
				nRetCode = Rsp.code;
			}
		}
	}
	return nRetCode;
}

int CQDLCtrl::CmdDlImageDdrChecksum(unsigned char ImageType, char* pthImage)
{
	if (IsFileExist(pthImage) != NO_ERROR) {
		return ERROR_PATH_NOT_FOUND;
	}
	CFile cfimage;
	if (cfimage.Open(pthImage, CFile::modeRead | CFile::shareDenyNone) < 0) {
		return ERROR_FILE_INVALID;
	}

#ifdef _DEBUG
	clock_t start = clock();
#endif

	req_qth_dmss_cmd_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.cmd_code = QTH_DMSS_CMD_CHECKSUM;
	Req.image_type = ImageType;

	DWORD nSize = (DWORD)cfimage.GetLength();
	endian_swap(nSize); // pack as big endian
	memcpy(Req.image_size, &nSize, sizeof(DWORD));
	DWORD nRead = 0;
	byte c0 = 0, c1 = 0, c2 = 0, c3 = 0;
	byte buffer[PKT_BUFFER_SIZE];
	do {
		memset(buffer, 0, PKT_BUFFER_SIZE);
		nRead = cfimage.Read((void*)buffer, PKT_BUFFER_SIZE);
		for (int i = 0; i < PKT_BUFFER_SIZE; i += 4) {
			c0 = c0 ^ buffer[i];
			c1 = c1 ^ buffer[i+1];
			c2 = c2 ^ buffer[i+2];
			c3 = c3 ^ buffer[i+3];
		}
	} while (nRead == PKT_BUFFER_SIZE);
	cfimage.Close();

	rsp_qth_checksum_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nRetCode = ERROR_FUNCTION_FAILED;
	for (int i = 0; i < QCMD_MAX_RETRY; i++) {
		nRetCode = m_SerCom.TXRXDl((char*)&Req, sizeof(Req), (char*)&Rsp, sizeof(Rsp), 5000); // wait 5 sec
		TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
		if (nRetCode == NO_ERROR && Rsp.code == STREAMING_CMD_BOLCKWRI) {
				break;
			}
			else {
			ComClose();
			ComOpen();
				nRetCode = ERROR_QCOM_SPEEDUP_BASE + 0x1e;
			}
			//if (Rsp.code == STREAMING_CMD_LOG) {
			//	TRACE(_T("%s %s"), _T(__FUNCTION__), Rsp.code + 1); // TODO: show this log outside
			//}
		}

	bool isChecked =
		Rsp.checksum[0] == c0 &&
		Rsp.checksum[1] == c1 &&
		Rsp.checksum[2] == c2 &&
		Rsp.checksum[3] == c3;
	if (!isChecked) { // if fail, show checksum
		nRetCode += 100; // error code would change from 126000 to 126130
		char checksum[64];
		memset(checksum, 0, 64);
		sprintf(checksum, "#DEBUG: checksum - PC:0x%02x%02x%02x%02x, TRG:0x%02x%02x%02x%02x",
			c0, c1, c2, c3, Rsp.checksum[0], Rsp.checksum[1], Rsp.checksum[2], Rsp.checksum[3]);
		//SendMsg(checksum, QMSG_WARNING);
	}

#ifdef _DEBUG
	clock_t end = clock();
	CString dbg_msg;
	dbg_msg.Format(_T("#DEBUG: checksum - type: %d, time: %02.03f sec"),
		ImageType, static_cast<double>(end - start) / CLOCKS_PER_SEC);
	//SendMsg(dbg_msg, QMSG_WARNING);
#endif

	return nRetCode;
}

// write only (ddr to flash)
int CQDLCtrl::UploadALLImageSpeedup(
		char* pthDbl, char* pthFsbl, char* pthOsbl,
		char* pthAmss, char* pthDsp1, char* pthEfs2,
		char* pthAppsbl, char* pthApps, char* pthFlash, char* pthDt,
		char* pthBkFsbl, char* pthBkOsbl, char* pthBkDt, char* pthBkAppsbl, char* pthReserved)
{
	int nRetCode = NO_ERROR;

	// boot code
	bool isBootCodeOk =
		IsFileExist(pthDbl) == NO_ERROR &&
		IsFileExist(pthFsbl) == NO_ERROR &&
		IsFileExist(pthOsbl) == NO_ERROR &&
		CmdDlImageDdrChecksum(OPEN_MODE_DBL, pthDbl) == NO_ERROR &&
		CmdDlImageDdrChecksum(OPEN_MODE_FSBL, pthFsbl) == NO_ERROR &&
		CmdDlImageDdrChecksum(OPEN_MODE_OSBL, pthOsbl) == NO_ERROR;
	if (isBootCodeOk) {
		if (nRetCode == NO_ERROR) {
			nRetCode = UploadImageSpeedup(OPEN_MODE_DBL, pthDbl, "DBL");
		}
		if (nRetCode == NO_ERROR) {
			nRetCode = UploadImageSpeedup(OPEN_MODE_FSBL, pthFsbl, "FSBL");
		}
		if (nRetCode == NO_ERROR) {
			nRetCode = UploadImageSpeedup(OPEN_MODE_OSBL, pthOsbl, "OSBL");
		}
	}

	// modem code
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageSpeedup(OPEN_MODE_AMSS, pthAmss, "AMSS");
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageSpeedup(OPEN_MODE_DSP1, pthDsp1, "DSP1");
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageSpeedup(OPEN_MODE_CEFS, pthEfs2, "EFS2");
	}
	// boot loader
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageSpeedup(OPEN_MODE_DT, pthDt, "DT");
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageSpeedup(OPEN_MODE_APPSBL, pthAppsbl, "APPSBL");
	}
	// os image
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageSpeedup(OPEN_MODE_APPS, pthApps, "APPS");
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageSpeedup(OPEN_MODE_FLASH_BIN, pthFlash, "Flash");
	}
	// 2009-09-07 Mick: backup partition
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageSpeedup(OPEN_MODE_BAK_FSBL, pthBkFsbl, "backup FSBL");
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageSpeedup(OPEN_MODE_BAK_OSBL, pthBkOsbl, "backup OSBL");
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageSpeedup(OPEN_MODE_BAK_DT, pthBkDt, "backup DT");
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImageSpeedup(OPEN_MODE_BAK_APPSBL, pthBkAppsbl, "backup APPSBL");
	}
	if (strlen(pthReserved) > 0 && nRetCode == NO_ERROR) {
		nRetCode = UploadImageSpeedup(OPEN_MODE_RESERVED, pthReserved, "RESERVED");
	}
	return nRetCode;
}

int CQDLCtrl::UploadImageSpeedup(unsigned char ImageType, char* ImagePath, char* Part)
{
	if (strlen(ImagePath) > 0 || ImageType == OPEN_MODE_RESERVED) {

#ifdef _DEBUG
		clock_t start = clock();
#endif

		CString message;
		int nRetCode = ERROR_FUNCTION_FAILED;
		// write RESERVED without image path input (direct restore from DDR)
		bool isSkipChecksum =
			ImageType == OPEN_MODE_RESERVED ||
			ImageType == OPEN_MODE_DBL ||
			ImageType == OPEN_MODE_FSBL ||
			ImageType == OPEN_MODE_OSBL;
		if (isSkipChecksum) {
			nRetCode = NO_ERROR;
		}
		else {
			nRetCode = CmdDlImageDdrChecksum(ImageType, ImagePath);
		}

		if (nRetCode == NO_ERROR) {
			nRetCode = CmdDlHexOpenImage(ImageType);
			if (nRetCode == NO_ERROR) {
				nRetCode = CmdDlHexWriteImageToNAND(ImageType, ImagePath);
			}
			if (nRetCode == NO_ERROR) {
				nRetCode = CmdDlHexCloseImage();
			}
		}
		else {
			// 2010-10-10 Mick: if fail, try original way to download
			ComClose();
			ComOpen();
			nRetCode = UploadImage(ImageType, ImagePath, Part);
			if (nRetCode != NO_ERROR) {
				message.Format(_T("#ERROR[DL2010]: Download %s checksum Failed!"), Part);
			}
		}

		MessageType type = QMSG_NORMAL;
		if (nRetCode == NO_ERROR) {
			type = QMSG_NORMAL;
			message.Format(_T("#Download %s success"), Part);
		}
		else {
			type = QMSG_ERROR;
			if (message.IsEmpty()) {
				message.Format(_T("#Download %s Fail!"), Part);
			}
			if (strncmp(Part, "DBL", 3) == 0) {
				message = _T("#ERROR[DL2003]") + message;
			}
			else if (strncmp(Part, "FSBL", 4) == 0) {
				message = _T("#ERROR[DL2004]") + message;
			}
			else if (strncmp(Part, "OSBL", 4) == 0) {
				message = _T("#ERROR[DL2005]") + message;
			}
			else if (strncmp(Part, "AMSS", 4) == 0) {
				message = _T("#ERROR[DL2006]") + message;
			}
			else if (strncmp(Part, "DSP1", 4) == 0) {
				message = _T("#ERROR[DL2007]") + message;
			}
			else if (strncmp(Part, "DT", 2) == 0) {
				message = _T("#ERROR[DL2008]") + message;
			}
			else if (strncmp(Part, "RESERVED", 8) == 0) {
				message = _T("#ERROR[DL2009]") + message;
			}
		}

		//SendMsg(message.GetBuffer(), type);
		message.ReleaseBuffer();

#ifdef _DEBUG
		clock_t end = clock();
		CString dbg_msg;
		dbg_msg.Format(_T("#DEBUG: write image - type: %d, time: %02.03f sec"),
			ImageType, static_cast<double>(end - start) / CLOCKS_PER_SEC);
		//SendMsg(dbg_msg, QMSG_WARNING);
#endif

		return nRetCode;
	}
	return NO_ERROR;
}

/**
 * Qisda customized write command for speedup
 *
 * @param ImageType to tell target which image to write
 * @param pthImage if ImagePath is "", hard code send
 *   #1 RESERVED: 22 block
 *   #2 SIMSECURE: 6 block
 */
int CQDLCtrl::CmdDlHexWriteImageToNAND(unsigned char ImageType, char* pthImage)
{
	req_qth_dmss_cmd_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.cmd_code = QTH_DMSS_CMD_FLASHWRITE;
	Req.image_type = ImageType;
	CFile fImage;
	DWORD nSize = 0;
	if (pthImage == NULL || strlen(pthImage) == 0) {
		if (ImageType == OPEN_MODE_RESERVED) {
			nSize = RESTORE_SIZE_RESERVED;
		}
	}
	else if (fImage.Open(pthImage, CFile::modeRead | CFile::shareDenyNone) < 0) {
		return ERROR_FILE_INVALID;
	}
	if (pthImage != NULL && strlen(pthImage) > 0) {
		nSize = (DWORD)fImage.GetLength();
		fImage.Close();
	}
	endian_swap(nSize); // pack as big endian
	memcpy(Req.image_size, &nSize, sizeof(DWORD));
	endian_swap(nSize); // re-swap endian for debug message and timeout count
#ifdef _DEBUG
	CString dbg_msg;
	dbg_msg.Format(_T("#DEBUG: write image - type: %d, size: %d bytes"), ImageType, nSize);
	//SendMsg(dbg_msg.GetBuffer(), QMSG_WARNING);
	dbg_msg.ReleaseBuffer();
#endif // _DEBUG

	rsp_cmd_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nRetCode = ERROR_FUNCTION_FAILED;
	for (int i = 0; i < QCMD_MAX_RETRY; i++) { // write spped almost 450 KB/sec
		nRetCode = m_SerCom.TXRXDl((char*)&Req, sizeof(Req), (char*)&Rsp, sizeof(Rsp), (nSize / 460800) * 1000 + 1000);
		TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
		TRACE(_T("%s timeout:%d\n"), _T(__FUNCTION__), (nSize / 460800) * 1000 + 1000);
		if (nRetCode == NO_ERROR) {
			if (Rsp.code == STREAMING_CMD_BOLCKWRI) {
				break;
			}
			else {
				nRetCode = ERROR_QCOM_DMSS_CMD_BASE + DMSS_CMD_DLOAD_SWITCH;
			}
			if (Rsp.code == STREAMING_CMD_LOG) {
				TRACE(_T("%s target error:%s"), _T(__FUNCTION__), Rsp.data);
				CString dbg_msg;
				dbg_msg.Format(_T("#DEBUG: %s target error:%s"), _T(__FUNCTION__), Rsp.data);
				dbg_msg.Replace(_T("\n"), _T(""));
				//SendMsg(dbg_msg.GetBuffer(), QMSG_WARNING);
				dbg_msg.ReleaseBuffer();
			}
		}
	}

	return nRetCode;
}

int CQDLCtrl::UploadALLImage7k(char* pthQcsblHd, char* pthQcsbL,
		char* pthOemsblHd, char* pthOemsbl, char* pthAmssHd, char* pthAmss,
		char* pthAppsblHd, char* pthAppsbl, char* pthAppsHd, char* pthApps)
{
	int nRetCode = NO_ERROR;
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImage7k(OPEN_MODE_QCSBLHDCFG, pthQcsblHd, "", "QCSBLHD");
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImage7k(OPEN_MODE_QCSBL, pthQcsbL, "", "QCSBL");
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImage7k(OPEN_MODE_OEMSBL, pthOemsbl, pthOemsblHd, "OEMSBLHD and OEMSBL");
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImage7k(OPEN_MODE_AMSS, pthAmss, pthAmssHd, "AMSSHD and AMSS");
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImage7k(OPEN_MODE_APPSBL, pthAppsbl, pthAppsblHd, "APPSBOOTHD and APPSBOOT");
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadImage7k(OPEN_MODE_APPS, pthApps, pthAppsHd, "APPSHD and APPS");
	}
	return nRetCode;
}

int CQDLCtrl::UploadImage7k(unsigned char Type, char* pthImage, char* pthHeader, char* Part)
{
	int	nHdSize = 0;
	unsigned char HdData[512];
	memset(HdData, 0, sizeof(HdData));
	int nRetCode = NO_ERROR;
	if (strlen(pthHeader) > 0) {
		CFile fHeader;
		if (fHeader.Open(pthHeader, CFile::modeRead | CFile::shareDenyNone) == FALSE) {
			nRetCode = ERROR_FILE_NOT_FOUND;
		}
		else {
			nHdSize = (int)fHeader.GetLength();
			fHeader.Read(HdData, nHdSize);
			fHeader.Close();
		}
	}

	if (pthImage != NULL && strlen(pthImage) > 0) {
		if (nRetCode == NO_ERROR) {
			nRetCode = CmdDlHexOpenImage7k(Type, HdData, nHdSize);
		}
		if (nRetCode == NO_ERROR) {
			nRetCode = UploadFile(pthImage);
		}
		if (nRetCode == NO_ERROR) {
			nRetCode = CmdDlHexCloseImage();
		}

		MessageType msg_type = QMSG_NORMAL;
		CString message;
		message.Format("Download %s success", Part);
		if (nRetCode != NO_ERROR) {
			msg_type = QMSG_ERROR;
			message.Format("Error[D16]: Download %s Failed!", Part);
		}
		//SendMsg(message.GetBuffer(), msg_type);
		message.ReleaseBuffer();

		return nRetCode;
	}
	return NO_ERROR;
}

int CQDLCtrl::CmdDiagSwitchToDlad()
{
	req_cmd_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.code = DMSS_CMD_DLOAD_SWITCH;

	rsp_cmd_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nRetCode = ERROR_FUNCTION_FAILED;
	for (int i = 0; i < QCMD_MAX_RETRY; i++) {
		nRetCode = m_SerCom.TXRXDiag((char*)&Req, sizeof(Req), (char*)&Rsp, sizeof(Rsp), QCMD_DELAY_SHORT);
		TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
		if (nRetCode == NO_ERROR) {
			if (Rsp.code == DMSS_CMD_DLOAD_SWITCH) {
				break;
			}
			else {
				nRetCode = ERROR_QCOM_DMSS_CMD_BASE + DMSS_CMD_DLOAD_SWITCH;
			}
		}
	}
	return nRetCode;
}

int CQDLCtrl::CmdDiagSetSPC(char* szSPC)
{
	req_spc_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.code = DIAG_SPC_F;
	memcpy(Req.spc, szSPC, 6);

	rsp_status_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nRetCode = m_SerCom.TXRXDiag((char*)&Req, sizeof(Req), (char*)&Rsp, sizeof(Rsp));
	TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
	if (nRetCode == NO_ERROR) {
		if (Rsp.status == 0x1) {
		// no retry, so do nothing here
		}
		else {
			nRetCode = DIAG_SPC_F;
		}
	}

	return nRetCode;
}

// test if EDIAG or EDL mode
int CQDLCtrl::CmdDiagProbe()
{
	req_cmd_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.code = DIAG_STATUS_F;

	rsp_cmd_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nRetCode = ERROR_FUNCTION_FAILED;
	for (int i = 0; i < QCMD_MAX_RETRY; i++) {
		nRetCode = m_SerCom.TxRx((char*)&Req, sizeof(Req), (char*)&Rsp, sizeof(Rsp), QCMD_DELAY_SHORT);
		TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
		if (nRetCode == NO_ERROR) {
			if (Rsp.code == DIAG_BAD_CMD_F || Rsp.code == DIAG_STATUS_F) {
				m_SerCom.SetState(CSerialIO::EDIAG);
				break;
			}
			else if (Rsp.code == DMSS_CMD_VERRSP) {
				m_SerCom.SetState(CSerialIO::EDL);
				break;
			}
			else {
				nRetCode = DIAG_STATUS_F;
			}
		}
		Sleep(100);
	}
	return nRetCode;
}

int CQDLCtrl::CmdDiagProbeEmdl()
{
	req_cmd_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.code = DIAG_STATUS_F;

	rsp_cmd_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));
	int nRetTest = ERROR_FUNCTION_FAILED;
	int nRetCode = ERROR_FUNCTION_FAILED;
	for (int i = 0; i < QCMD_MAX_RETRY; i++) { // 10 * 100 = 1 sec
		nRetCode = m_SerCom.TxRxEmdl((char*)&Req, sizeof(Req), (char*)&Rsp, sizeof(Rsp), 1);
		TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
		if (nRetCode == NO_ERROR) {
			if (Rsp.code == DMSS_CMD_VERRSP) {
				m_SerCom.SetState(CSerialIO::EDL);
				break;
			}
			else {
				nRetCode = DIAG_STATUS_F;
			}
		}
		else if (nRetCode == CSerCOMLibrary::ERROR_CSERCOMLIBRARY_TX_FAIL) {
			if (i % 100 == 0) {
				nRetTest = ComClose();
				TRACE(_T("%s Close: %d\n"), _T(__FUNCTION__), nRetTest);
				nRetTest = ComOpen(true); // would sleep 200ms
				TRACE(_T("%s Open : %d\n"), _T(__FUNCTION__),nRetTest);
			}
		}
		Sleep(100);
	}
	return nRetCode;
}

int CQDLCtrl::CmdDlNop()
{
	req_cmd_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.code = DMSS_CMD_NOP;

	rsp_cmd_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nRetCode = ERROR_FUNCTION_FAILED;
	for (int i = 0; i < QCMD_MAX_RETRY; i++) {
		nRetCode = m_SerCom.TXRXDl((char*)&Req, sizeof(Req), (char*)&Rsp, sizeof(Rsp), QCMD_DELAY_SHORT);
		TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
		if (nRetCode == NO_ERROR) {
			if (Rsp.code == DMSS_CMD_ACK) {
				break;
			}
			else {
				nRetCode = ERROR_QCOM_DMSS_CMD_BASE + DMSS_CMD_NOP;
			}
		}
		Sleep(100);
	}
	return nRetCode;
}

// 2010-01-20 Mick: add check secure boot
// JZ use ResponseDmssParameter::device_size to show enable(1) or disable(0)
int CQDLCtrl::CmdDlParameter()
{
	req_cmd_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.code = DMSS_CMD_PREQ;

	rsp_dmss_param_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nRetCode = ERROR_FUNCTION_FAILED;
	for (int i = 0; i < QCMD_MAX_RETRY; i++) {
		nRetCode = m_SerCom.TXRXDl((char*)&Req, sizeof(Req), (char*)&Rsp, sizeof(Rsp), QCMD_DELAY_SHORT);
		TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
		if (nRetCode == NO_ERROR) {
			if (Rsp.code == DMSS_CMD_PARAMS) {
				// 2010-01-20 Mick: secure boot check
				m_isSecureBoot = Rsp.device_size == 1;
				if (m_isSecureBoot) {
					//SendMsg("Check Secure Boot is enabled", QMSG_WARNING);
				}
				break;
			}
			else {
				nRetCode = ERROR_QCOM_DMSS_CMD_BASE + DMSS_CMD_PARAMS;
			}
		}
	}
	return nRetCode;
}

int CQDLCtrl::CmdDlUploadHex(byte* pData, WORD nLength, DWORD wAddress)
{
	req_dmss_write_32b_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.code = DMSS_CMD_WRITE_32BIT;
	endian_swap(wAddress);
	memcpy(Req.address, &wAddress, sizeof(DWORD));
	WORD nSize = nLength;
	endian_swap(nSize);
	memcpy(Req.length, &nSize, sizeof(WORD));
	memcpy(Req.data, pData, nLength);

	rsp_cmd_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nRetCode = ERROR_FUNCTION_FAILED;
	for (int i = 0; i < QCMD_MAX_RETRY; i++) {
		nRetCode = m_SerCom.TXRXDl((char*)&Req, 7 + nLength, (char*)&Rsp, sizeof(Rsp), QCMD_DELAY_SHORT);
		if (nRetCode == NO_ERROR) {
			if (Rsp.code == DMSS_CMD_ACK) {
				break;
			}
			else {
				nRetCode = ERROR_QCOM_DMSS_CMD_BASE + DMSS_CMD_WRITE_32BIT;
			}
		}
	}
	return nRetCode;
}

int CQDLCtrl::CmdDlGo(WORD nSegment, WORD nOffset)
{
	req_dmss_go_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.code = DMSS_CMD_GO;
	endian_swap(nSegment);
	memcpy(Req.segment, &nSegment, sizeof(WORD));
	endian_swap(nOffset);
	memcpy(Req.offset, &nOffset, sizeof(WORD));

	rsp_cmd_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nRetCode = ERROR_FUNCTION_FAILED;
	for (int i = 0; i < QCMD_MAX_RETRY; i++) {
		nRetCode = m_SerCom.TXRXDl((char*)&Req, sizeof(Req), (char*)&Rsp, sizeof(Rsp), QCMD_DELAY_SHORT);
		TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
		if (nRetCode == NO_ERROR) {
			if (Rsp.code == DMSS_CMD_ACK) {
				//ComClose();
				//TRACE(_T("ComClose()\n"));
				Sleep(2000); // wait target reset and jump to run hex
				// TODO: need to set emdl mode parameter
				//ComOpen();
				//TRACE(_T("ComOpen()\n"));
				break;
			}
			else {
				nRetCode = ERROR_QCOM_DMSS_CMD_BASE + DMSS_CMD_GO;
			}
		}
	}
	return nRetCode;
}

// 2010-01-20 Mick: add check secure boot
// JZ use ResponseStreamingHello::version to show enable(1) or disable(0)
int CQDLCtrl::CmdDlHexHello()
{
	req_strmng_hello_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.code = STREAMING_CMD_HELLO;
	memcpy(Req.magic_number, "QCOM fast download protocol host", 32);
	Req.version = 0x03;
	Req.compatible_version = 0x03;
	Req.feature_bits = 0x09;

	rsp_strmng_hello_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nRetCode = ERROR_FUNCTION_FAILED;
	for (int i = 0; i < QCMD_MAX_RETRY; i++) {
		nRetCode = m_SerCom.TXRXDl((char*)&Req, sizeof(Req), (char*)&Rsp, sizeof(Rsp), QCMD_DELAY_LONG);
		TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
		if (nRetCode == NO_ERROR) {
			if (Rsp.code == STREAMING_CMD_HELLORESP && memcmp(Rsp.magic_number, "QCOM", 4) == 0) {
				// 2010-01-20 Mick: secure boot check
				m_isSecureBoot = Rsp.version == 1;
				if (m_isSecureBoot) {
					//SendMsg("Check Secure Boot is enabled", QMSG_WARNING);
				}
				break;
			}
			else {
				nRetCode = ERROR_QCOM_STRMNG_CMD_BASE + STREAMING_CMD_HELLO;
			}
			//if (Rsp.code == STREAMING_CMD_LOG) {
			//	TRACE(_T("%s %s"), _T(__FUNCTION__), Rsp.code + 1); // TODO: show this log outside
			//}
		}
		Sleep(100);
	}
	return nRetCode;
}

int CQDLCtrl::CmdDlHexSecurity(byte nMode)
{
	req_strmng_sec_mode_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.cmd_code = STREAMING_CMD_SECMODE;
	Req.mode = nMode;

	rsp_cmd_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nRetCode = ERROR_FUNCTION_FAILED;
	for (int i = 0; i < QCMD_MAX_RETRY; i++) {
		nRetCode = m_SerCom.TXRXDl((char*)&Req, sizeof(Req), (char*)&Rsp, sizeof(Rsp), QCMD_DELAY_SHORT);
		TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
		if (nRetCode == NO_ERROR) {
			if(Rsp.code == STREAMING_CMD_SECMODER) {
				break;
			}
			else {
				nRetCode = ERROR_QCOM_STRMNG_CMD_BASE + STREAMING_CMD_SECMODE;
			}
			if (Rsp.code == STREAMING_CMD_LOG) {
				TRACE(_T("%s target error:%s"), _T(__FUNCTION__), Rsp.data); // TODO: show this log outside
			}
		}
	}
	return nRetCode;
}

int CQDLCtrl::CmdDlHexUploadPartition(byte* pData, WORD nLength, unsigned char bOverride)
{
	req_strmng_ptl_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.code = STREAMING_CMD_PATTTIONT;
	Req.repartition = bOverride;
	memcpy(Req.table, pData, nLength);

	rsp_status_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nTimeout = (bOverride == 0x01) ? 40*1000 : QCMD_DELAY_LONG; // erase takes 25 sec
	int nRetry   = (bOverride == 0x01) ? 3 : QCMD_MAX_RETRY; // if override only retry 3 times
	int nRetCode = ERROR_FUNCTION_FAILED;
#ifdef _DEBUG
	time_t rawtime;
#endif
	for (int i = 0; i < nRetry; i++) {
#ifdef _DEBUG
		time(&rawtime);
		TRACE(_T("%s %s"), _T(__FUNCTION__), ctime(&rawtime));
#endif
		nRetCode = m_SerCom.TXRXDl((char*)&Req, 2 + nLength, (char*)&Rsp, sizeof(Rsp), nTimeout);
#ifdef _DEBUG
		TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
		TRACE(_T("%s nRet:%d Rsp.status:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.status);
		time(&rawtime);
		TRACE(_T("%s %s"), _T(__FUNCTION__), ctime(&rawtime));
#endif
		if (nRetCode == NO_ERROR) {
			if (Rsp.code == STREAMING_CMD_PATTTIONTR && Rsp.status == QPTL_ACCEPT) {
				break;
			}
			else if (Rsp.code == STREAMING_CMD_PATTTIONTR && Rsp.status == QPTL_DIFFER) {
				nRetCode = STREAMING_CMD_PATTTIONTR + QPTL_DIFFER; //// re-partition
				break;
			}
			else if (Rsp.code == STREAMING_CMD_PATTTIONTR && Rsp.status == QPTL_ERASE_FAIL) {
				nRetCode = STREAMING_CMD_PATTTIONTR + QPTL_ERASE_FAIL; // erase fail
				break;
			}
			else if (Rsp.code == STREAMING_CMD_LOG) {
				nRetCode = ERROR_QCOM_STRMNG_CMD_BASE + STREAMING_CMD_PATTTIONTR;
				TRACE(_T("%s target error:%s"), _T(__FUNCTION__), Rsp.data);
				CString dbg_msg;
				dbg_msg.Format(_T("#DEBUG: %s target error:%s"), _T(__FUNCTION__), Rsp.data);
				dbg_msg.Replace(_T("\n"), _T(""));
				//SendMsg(dbg_msg.GetBuffer(), QMSG_WARNING);
				dbg_msg.ReleaseBuffer();
				break;
			}
			else {
				nRetCode = ERROR_QCOM_STRMNG_CMD_BASE + STREAMING_CMD_PATTTIONTR;
			}
			//if (Rsp.code == STREAMING_CMD_LOG) {
			//	//TRACE(_T("%s target error:%s"), _T(__FUNCTION__), Rsp.data); // TODO: show this log outside
			//}
		}
	}
	return nRetCode;
}

int CQDLCtrl::CmdSetFastbootFlag(byte* pData, WORD nLength, unsigned char nPair)
{
	req_strmng_ptl_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.code = STREAMING_CMD_FASTBOOTFLAG;
	Req.repartition = nPair;
	memcpy(Req.table, pData, nLength);

	rsp_status_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nTimeout = (nPair == 0x01) ? 40*1000 : QCMD_DELAY_LONG; // erase takes 25 sec
	int nRetry   = (nPair == 0x01) ? 3 : QCMD_MAX_RETRY; // if override only retry 3 times
	int nRetCode = ERROR_FUNCTION_FAILED;
#ifdef _DEBUG
	time_t rawtime;
#endif
	for (int i = 0; i < nRetry; i++) {
#ifdef _DEBUG
		time(&rawtime);
		TRACE(_T("%s %s"), _T(__FUNCTION__), ctime(&rawtime));
#endif
		nRetCode = m_SerCom.TXRXDl((char*)&Req, 2 + nLength, (char*)&Rsp, sizeof(Rsp), nTimeout);
#ifdef _DEBUG
		TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
		TRACE(_T("%s nRet:%d Rsp.status:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.status);
		time(&rawtime);
		TRACE(_T("%s %s"), _T(__FUNCTION__), ctime(&rawtime));
#endif
		if (nRetCode == NO_ERROR) {
			//if (Rsp.code == STREAMING_CMD_PATTTIONTR && Rsp.status == QPTL_ACCEPT) {
			//	break;
			//}
			//else if (Rsp.code == STREAMING_CMD_PATTTIONTR && Rsp.status == QPTL_DIFFER) {
			//	nRetCode = STREAMING_CMD_PATTTIONTR + QPTL_DIFFER; //// re-partition
			//	break;
			//}
			//else if (Rsp.code == STREAMING_CMD_PATTTIONTR && Rsp.status == QPTL_ERASE_FAIL) {
			//	nRetCode = STREAMING_CMD_PATTTIONTR + QPTL_ERASE_FAIL; // erase fail
			//	break;
			//}
			//else if (Rsp.code == STREAMING_CMD_LOG) {
			//	nRetCode = ERROR_QCOM_STRMNG_CMD_BASE + STREAMING_CMD_PATTTIONTR;
			//	TRACE(_T("%s target error:%s"), _T(__FUNCTION__), Rsp.data);
			//	CString dbg_msg;
			//	dbg_msg.Format(_T("#DEBUG: %s target error:%s"), _T(__FUNCTION__), Rsp.data);
			//	dbg_msg.Replace(_T("\n"), _T(""));
			//	//SendMsg(dbg_msg.GetBuffer(), QMSG_WARNING);
			//	dbg_msg.ReleaseBuffer();
			//	break;
			//}
			//else {
			//	nRetCode = ERROR_QCOM_STRMNG_CMD_BASE + STREAMING_CMD_PATTTIONTR;
			//}
			////if (Rsp.code == STREAMING_CMD_LOG) {
			////	//TRACE(_T("%s target error:%s"), _T(__FUNCTION__), Rsp.data); // TODO: show this log outside
			////}
		}
	}
	return nRetCode;
}

int CQDLCtrl::CmdDlHexOpenImage(unsigned char ImageType)
{
	req_strmng_img_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.code = STREAMING_CMD_OPENMIMAGE;
	Req.type = ImageType;

	rsp_status_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nRetCode = ERROR_FUNCTION_FAILED;
	for (int i = 0; i < QCMD_MAX_RETRY; i++) {
		nRetCode = m_SerCom.TXRXDl((char*)&Req, sizeof(Req), (char*)&Rsp, sizeof(Rsp), QCMD_DELAY_OEPN);
		TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
		TRACE(_T("%s nRet:%d Rsp.status:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.status);
		if (nRetCode == NO_ERROR) {
			if (Rsp.code == STREAMING_CMD_OPENEDMIMAGE && Rsp.status == OPEN_MULTI_SUCCESS) {
				Sleep(500);
				break;
			}
			else {
				nRetCode = ERROR_QCOM_STRMNG_CMD_BASE + STREAMING_CMD_OPENMIMAGE;
			}
			if (Rsp.code == STREAMING_CMD_LOG) {
				TRACE(_T("%s target error:%s"), _T(__FUNCTION__), Rsp.data);
				CString dbg_msg;
				dbg_msg.Format(_T("#DEBUG: %s target error:%s"), _T(__FUNCTION__), Rsp.data);
				dbg_msg.Replace(_T("\n"), _T(""));
				//SendMsg(dbg_msg.GetBuffer(), QMSG_WARNING);
				dbg_msg.ReleaseBuffer();
			}
		}
	}
	return nRetCode;
}

int CQDLCtrl::CmdDlHexOpenImage7k(unsigned char ImageType, unsigned char* szHdData, int nHdSize)
{
	req_strmng_img_7k_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.code = STREAMING_CMD_OPENMIMAGE;
	Req.type = ImageType;
	memcpy(Req.header, szHdData, nHdSize);

	rsp_status_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nRetCode = ERROR_FUNCTION_FAILED;
	for (int i = 0; i < QCMD_MAX_RETRY; i++) {
		nRetCode = m_SerCom.TXRXDl((char*)&Req, nHdSize + 2, (char*)&Rsp, sizeof(Rsp), QCMD_DELAY_OEPN);
		TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
		if (nRetCode == NO_ERROR) {
			if (Rsp.code == STREAMING_CMD_OPENEDMIMAGE && Rsp.status == OPEN_MULTI_SUCCESS) {
				break;
			}
			else {
				nRetCode = ERROR_QCOM_STRMNG_CMD_BASE + STREAMING_CMD_OPENMIMAGE;
			}
			if (Rsp.code == STREAMING_CMD_LOG) {
				TRACE(_T("%s target error:%s"), _T(__FUNCTION__), Rsp.data);
				CString dbg_msg;
				dbg_msg.Format(_T("#DEBUG: %s target error:%s"), _T(__FUNCTION__), Rsp.data);
				dbg_msg.Replace(_T("\n"), _T(""));
				//SendMsg(dbg_msg.GetBuffer(), QMSG_WARNING);
				dbg_msg.ReleaseBuffer();
			}
		}
	}
	return nRetCode;
}

int CQDLCtrl::CmdDlHexUploadFile(byte* pData, DWORD nLength, DWORD wAddress)
{
	req_strmng_write_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.code = STREAMING_CMD_STRWRITE;
	memcpy(Req.address, &wAddress, sizeof(wAddress));
	memcpy(Req.data, pData, nLength);

	rsp_cmd_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nRetCode = ERROR_FUNCTION_FAILED;
	for (int i = 0; i < QCMD_MAX_RETRY; i++) {
		nRetCode = m_SerCom.TXRXDl((char*)&Req, 5 + nLength, (char*)&Rsp, sizeof(Rsp), QCMD_DELAY_SHORT);
		TRACE(_T("%s Req.wAddress:0x%02x%02x%02x%02x\n"), _T(__FUNCTION__), Req.address[3], Req.address[2], Req.address[1], Req.address[0]);
		TRACE(_T("%s Rsp.wAddress:0x%02x%02x%02x%02x\n"), _T(__FUNCTION__), Rsp.data[3], Rsp.data[2], Rsp.data[1], Rsp.data[0]);
		TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
		if (nRetCode == NO_ERROR) {
			if(Rsp.code == STREAMING_CMD_BOLCKWRI &&
				Rsp.data[3] == Req.address[3] && Rsp.data[2] == Req.address[2] &&
				Rsp.data[1] == Req.address[1] && Rsp.data[0] == Req.address[0]) {
				break;
			}
			else {
				nRetCode = ERROR_QCOM_STRMNG_CMD_BASE + STREAMING_CMD_STRWRITE;
			}
			if (Rsp.code == STREAMING_CMD_LOG) {
				TRACE(_T("%s target error:%s"), _T(__FUNCTION__), Rsp.data);
				CString dbg_msg;
				dbg_msg.Format(_T("#DEBUG: %s target error:%s"), _T(__FUNCTION__), Rsp.data);
				dbg_msg.Replace(_T("\n"), _T(""));
				//SendMsg(dbg_msg.GetBuffer(), QMSG_WARNING);
				dbg_msg.ReleaseBuffer();
			}
		}
	}
	return nRetCode;
}

int CQDLCtrl::CmdDlHexCloseImage()
{
	req_cmd_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.code = STREAMING_CMD_CLOSE;

	rsp_cmd_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nRetCode = ERROR_FUNCTION_FAILED;
	for (int i = 0; i < QCMD_MAX_RETRY; i++) {
		nRetCode = m_SerCom.TXRXDl((char*)&Req, sizeof(Req), (char*)&Rsp, sizeof(Rsp), QCMD_DELAY_SHORT);
		TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
		if (nRetCode == NO_ERROR) {
			if (Rsp.code == STREAMING_CMD_CLOSED) {
				break;
			}
			else {
				nRetCode = ERROR_QCOM_STRMNG_CMD_BASE + STREAMING_CMD_CLOSE;
			}
			if (Rsp.code == STREAMING_CMD_LOG) {
				TRACE(_T("%s target error:%s"), _T(__FUNCTION__), Rsp.data);
				CString dbg_msg;
				dbg_msg.Format(_T("#DEBUG: %s target error:%s"), _T(__FUNCTION__), Rsp.data);
				dbg_msg.Replace(_T("\n"), _T(""));
				//SendMsg(dbg_msg.GetBuffer(), QMSG_WARNING);
				dbg_msg.ReleaseBuffer();
			}
		}
	}
	return nRetCode;
}

int CQDLCtrl::CmdDlStreamingStrRead(BYTE* lpData)
{
	req_strmng_Read_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.code = STREAMING_CMD_READ;
	DWORD dwAddress = (40960 * 512);
	memcpy(Req.address, &dwAddress, sizeof(dwAddress));
	WORD wLength = 0x200;
	memcpy(Req.length, &wLength, sizeof(wLength));

	rsp_strmng_read_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nRetCode = ERROR_FUNCTION_FAILED;
	for (int i = 0; i < QCMD_MAX_RETRY; i++) {
		nRetCode = m_SerCom.TXRXDl((char*)&Req, sizeof(Req), (char*)&Rsp, sizeof(Rsp), QCMD_DELAY_LONG);
		TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
		if (nRetCode == NO_ERROR) {
			if (Rsp.code == STREAMING_CMD_READDATA) {
				break;
			}
			else {
				nRetCode = ERROR_QCOM_STRMNG_CMD_BASE + STREAMING_CMD_READ;
			}
			if (Rsp.code == STREAMING_CMD_LOG) {
			//	TRACE(_T("%s %s"), _T(__FUNCTION__), Rsp.code + 1); // TODO: show this log outside
			}
		}
		Sleep(100);
	}
	memcpy(lpData, Rsp.dummy, sizeof(Rsp.dummy));

	return nRetCode;
}

int CQDLCtrl::CmdDlStreamingIRAMRead(BYTE* lpData, WORD wnLength, DWORD dwAddress)
{
	req_strmng_iram_read_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.code = STREAMING_CMD_IRAMREAD;
	//DWORD dwAddress = dwAddress;
	memcpy(Req.address, &dwAddress, sizeof(dwAddress));
	//WORD wLength = wnLength;
	memcpy(Req.length, &wnLength, sizeof(wnLength));

	rsp_strmng_iram_read_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nRetCode = ERROR_FUNCTION_FAILED;
	for (int i = 0; i < QCMD_MAX_RETRY; i++) {
		nRetCode = m_SerCom.TXRXDl((char*)&Req, sizeof(Req), (char*)&Rsp, sizeof(Rsp), QCMD_DELAY_LONG);
		TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
		if (nRetCode == NO_ERROR) {
			if (Rsp.code == STREAMING_CMD_READDATA) {
				break;
			}
			else {
				nRetCode = (dwAddress == 0x2F000) ? ERROR_QCOM_STRMNG_CMD_BASE + STREAMING_CMD_SBLREAD : ERROR_QCOM_STRMNG_CMD_BASE + STREAMING_CMD_PBLREAD;
			}
			if (Rsp.code == STREAMING_CMD_LOG) {
			//	TRACE(_T("%s %s"), _T(__FUNCTION__), Rsp.code + 1); // TODO: show this log outside
			}
		}
		Sleep(100);
	}
	memcpy(lpData, Rsp.dummy, sizeof(Rsp.dummy));

	return nRetCode;
}

int CQDLCtrl::WaitForMultiThread(long* lCount, long lWaitAllCount, long dwMilliseconds)
{
	long delay_time = 500;
	long timer = dwMilliseconds / delay_time;

	do {
		Sleep(delay_time);
		timer--;
	}
	while (*lCount < lWaitAllCount && timer > 0);
	return TRUE;
}

int CQDLCtrl::ErasePartitionEFS2(char* pthHex, char* pthPartition)
{
	m_SerCom.SetState(CSerialIO::EDIAG);
	int nRetCode = DloadOpen();
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = RunHex(pthHex);
		if (nRetCode == ERROR_TIMEOUT) {
			//SendMsg("Run HEX timeout ... skip", QMSG_NORMAL);
		}
		else if (nRetCode != NO_ERROR) {
			// 2010-11-12 Mick: try to skip this fail case
			//SendMsg("ERROR[DL1000]: Run HEX Failed!", QMSG_WARNING);
			nRetCode = ERROR_TIMEOUT;
		}
		else {
			//SendMsg("Run HEX success", QMSG_NORMAL);
		}
		if (nRetCode == NO_ERROR || nRetCode == ERROR_TIMEOUT) {
			nRetCode = CmdDlHexHello();
			if (nRetCode != NO_ERROR) {
				//SendMsg("ERROR[DL1000]: Streaming hello Failed!", QMSG_ERROR);
			}
			else {
				//SendMsg("Streaming hello success", QMSG_NORMAL);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlHexSecurity(0x01);
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Send Security Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Send Security success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadPartition(pthPartition, FALSE);
		if (nRetCode == -9) {
			//SendMsg("ERROR[DL2000]: Partition table is different from target!", QMSG_ERROR);
		}
		else if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL2002]: Download partition table Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Download partition table success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = EraseEFS2();
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlHexReset();
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Reset Failed!", QMSG_ERROR);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	ComClose();

	if (nRetCode != NO_ERROR) {
		//SendMsg("ERROR[DL1000]: Erase EFS2 Failed!", QMSG_ERROR);
	}
	else {
		//SendMsg("Erase EFS2 success", QMSG_NORMAL);
	}
	return nRetCode;
}

int CQDLCtrl::EraseEFS2()
{
	int nRetCode = CmdDlHexOpenImage(OPEN_MODE_CEFS);
	if (nRetCode == NO_ERROR) {
		byte buffer[PKT_BUFFER_SIZE] = {0};
		memset(buffer, 0xff, PKT_BUFFER_SIZE);

		for (DWORD wAddr = 0; wAddr < PKT_BUFFER_SIZE * 128; wAddr += PKT_BUFFER_SIZE) {
			nRetCode = CmdDlHexUploadFile(buffer, PKT_BUFFER_SIZE, wAddr);
			if (nRetCode != NO_ERROR) {
				break;
			}
		}
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDlHexCloseImage();
	}
	return nRetCode;
}
/*
int CQDLCtrl::BRTRestore(char* pthBrt, char* szSPC)
{
	m_SerCom.SetState(CSerialIO::EDIAG);
	int nRetCode = ComOpen();
	if (nRetCode != NO_ERROR) {
		//SendMsg("ERROR[DL1007]: Open COM port Failed!", QMSG_ERROR);
	}
	else {
		//SendMsg("Open COM port success", QMSG_NORMAL);
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDiagProbe();
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1002]: Send Probe Failed!", QMSG_ERROR);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		if (CmdDiagSetSPC(szSPC) != NO_ERROR)
			Sleep(15000); //// QUALCOMM : IF FAILED NEED WAIT
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = UploadBRT(pthBrt);
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[M46]: Restore BRT Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Restore BRT success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	ComClose();
	return nRetCode;
}

int CQDLCtrl::BRTBackup(char* pthBrt, char* szSPC)
{
	m_SerCom.SetState(CSerialIO::EDIAG);
	int nRetCode = ComOpen();
	if (nRetCode != NO_ERROR) {
		//SendMsg("ERROR[DL1007]: Open COM port Failed!", QMSG_ERROR);
	}
	else {
		//SendMsg("Open COM port success", QMSG_NORMAL);
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDiagProbe();
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Send Probe Failed!", QMSG_ERROR);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		if (CmdDiagSetSPC(szSPC) != NO_ERROR) {
			Sleep(15000); //// QUALCOMM : IF FAILED NEED WAIT
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = DownloadBRT(pthBrt);
		if (nRetCode != NO_ERROR) {
			//SendMsg("Backup BRT Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Backup BRT success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	ComClose();
	return nRetCode;
}

int CQDLCtrl::UploadBRT(char* path)
{
	int  nRetCode = FALSE;
	int  nvCount = -1, nCount = 0, nReadOnlyCount = 0, nSPLockCount = 0, iVal = 0;
	char szMessage[MAX_PATH];

	CFile oFile;
	if (!oFile.Open(path, CFile::modeRead | CFile::shareDenyNone))
		//SendMsg("Open BRT file failed", QMSG_ERROR);
	else {
		oFile.Read(&nvCount, sizeof(int));
		sprintf(szMessage, "Total NvCounts in BRT file =  %d", nvCount);
		//SendMsg(szMessage, QMSG_NORMAL);

		CNV_ItemHolder* cNVlist = new CNV_ItemHolder[nvCount];
		CString str;
		for (int i = 0; i < nvCount; i++) {
			oFile.Read(&iVal, sizeof(int));			//NV ID
			cNVlist[i].SetNV_ID(iVal);
			oFile.Read(&iVal, sizeof(int));			//NV Len
			cNVlist[i].SetSize(iVal);
			oFile.Read(cNVlist[i].GetData(), iVal);	//NV Data
		}

		for (int i = 0; i < nvCount; i++) {
			unsigned short Status = NV_DONE_S;
			int ItemID = cNVlist[i].GetNV_ID();
			nRetCode = DiagCmdNvWrite(ItemID, cNVlist[i].GetSize(), cNVlist[i].GetData(), &Status);
			if (nRetCode == NO_ERROR) {
				nCount++;
				sprintf(szMessage, "Restore NVItem: %d, Total Counts = %d", ItemID , nCount);
				//SendMsg(szMessage, QMSG_NORMAL);
			}
			else if (Status == NV_READONLY_S) {
				nReadOnlyCount++;
				sprintf(szMessage, "Restore NVItem: %d, This NVItem is write-protected.", ItemID);
				//SendMsg(szMessage, QMSG_NORMAL);
			}
			else if (Status == NV_SPLOCK_S) {
				nSPLockCount++;
				sprintf(szMessage, "Restore NVItem: %d, This NVItem is locked by SP.", ItemID);
				//SendMsg(szMessage, QMSG_NORMAL);
			}
		}
		sprintf(szMessage, "Total NvCounts in BRT file = %d, %d has restored, %d has write-protected, %d has locked by SP", nvCount, nCount, nReadOnlyCount, nSPLockCount);
		//SendMsg(szMessage, QMSG_NORMAL);

		delete [] cNVlist;
		cNVlist = NULL;
	}
	return (nvCount == (nCount + nReadOnlyCount + nSPLockCount)) ? 1 : 0;
}

int CQDLCtrl::DownloadBRT(char* path)
{
	int nRetCode = ERROR_FUNCTION_FAILED;
	int MAX_NVITEM = 6000;
	int perc = 0;
	int npos = 0, nvcount = 0, bcount = 0, nicount = 0, nvfailcount = 0;
	char szMessage[MAX_PATH];
	CNV_ItemHolder* lpCNVlist = new CNV_ItemHolder[MAX_NVITEM + 1];

	for (int ItemID = 0; ItemID < MAX_NVITEM; ItemID++) {
		int nDataLen = 128;
		unsigned char ItemData[129];
		unsigned short Status;
		memset(ItemData, 0, sizeof(ItemData));

		nRetCode = DiagCmdNvRead(ItemID, nDataLen, ItemData, &Status);
		if (nRetCode == NO_ERROR && Status == 0) {
			CNV_ItemHolder tempCNVobj;
			tempCNVobj.SetNV_ID(ItemID);
			tempCNVobj.SetData(ItemData, 128);
			lpCNVlist[bcount] = tempCNVobj;
			bcount++;

			sprintf(szMessage, "Backup NVItem: %d ,Total Counts = %d", ItemID , bcount);
			//SendMsg(szMessage, QMSG_NORMAL);
		}
		else {
			if (Status != NV_UNREADABLE_S && Status != NV_NOTACTIVE_S &&
				Status != NV_BADPARM_S    && Status != NV_SPLOCK_S) {
				sprintf(szMessage, "Backup NVItem %d failed.", ItemID);
				//SendMsg(szMessage, QMSG_NORMAL);
				delete [] lpCNVlist;
				return nRetCode;
			}
		}
	}
	sprintf(szMessage, "Total NvCounts %d has backuped", bcount);
	//SendMsg(szMessage, QMSG_NORMAL);

	CFile oFile;
	if (!oFile.Open(path, CFile::modeCreate | CFile::modeWrite)) {
		nRetCode = ERROR_FILE_INVALID;
		delete [] lpCNVlist;
		return nRetCode;
	}
	int iVal = 0;
	oFile.Write(&bcount, sizeof(int));
	for (int i = 0; i < bcount; i++) {
		iVal = lpCNVlist[i].GetNV_ID();
		oFile.Write(&iVal, sizeof(int));//nv id
		iVal = lpCNVlist[i].GetSize();
		oFile.Write(&iVal, sizeof(int));//item size
		oFile.Write(lpCNVlist[i].GetData(), lpCNVlist[i].GetSize());//item data
	}
	oFile.Flush();
	oFile.Close();
	delete [] lpCNVlist;

	return nRetCode;
}
*/
int CQDLCtrl::NvWrite(unsigned short ItemID, int nDataLength, byte* pData,
						 char* szSPC, unsigned short* Status)
{
	m_SerCom.SetState(CSerialIO::EDIAG);
	int nRetCode = ComOpen();
	if (nRetCode != NO_ERROR) {
		//SendMsg("ERROR[DL1007]: Open COM port Failed!", QMSG_ERROR);
	}
	else {
		//SendMsg("Open COM port success", QMSG_NORMAL);
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDiagProbe();
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Send Probe Failed!", QMSG_ERROR);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		if (CmdDiagSetSPC(szSPC) != NO_ERROR) {
			Sleep(15000); //// QUALCOMM : IF FAILED NEED WAIT
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = DiagCmdNvWrite(ItemID, nDataLength, pData, Status);
		if (nRetCode != NO_ERROR) {
			//SendMsg("Write NV item Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Write NV item success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	ComClose();
	return nRetCode;
}

int CQDLCtrl::NvRead(unsigned short ItemID, int nDataLength, byte* pData,
						char* szSPC, unsigned short* Status)
{
	m_SerCom.SetState(CSerialIO::EDIAG);
	int nRetCode = ComOpen();
	if (nRetCode != NO_ERROR) {
		//SendMsg("ERROR[DL1007]: Open COM port Failed!", QMSG_ERROR);
	}
	else {
		//SendMsg("Open COM port success", QMSG_NORMAL);
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = CmdDiagProbe();
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[DL1000]: Send Probe Failed!", QMSG_ERROR);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		if (CmdDiagSetSPC(szSPC) != NO_ERROR) {
			Sleep(15000); //// QUALCOMM : IF FAILED NEED WAIT
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (nRetCode == NO_ERROR) {
		nRetCode = DiagCmdNvRead(ItemID, nDataLength, pData, Status);
		if (nRetCode != NO_ERROR) {
			//SendMsg("ERROR[M44]: Read NV Item Failed!", QMSG_ERROR);
		}
		else {
			//SendMsg("Read NV Item success", QMSG_NORMAL);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	ComClose();
	return nRetCode;
}

int CQDLCtrl::DiagCmdNvWrite(unsigned short nItemID, int nLen,
								unsigned char* pData, unsigned short* Status)
{
	if (nLen > 128)	{ //// MAX LENGTH is 128
		nLen = 128;
	}

	nv_cmd_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.code = DIAG_CMD_WriteNVitem;
	memcpy(Req.id, &nItemID, 2);
	memcpy(Req.data, pData, nLen);

	nv_cmd_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nRetCode = ERROR_FUNCTION_FAILED;
	nRetCode = m_SerCom.TXRXDiag((char*)&Req, sizeof(Req), (char*)&Rsp, sizeof(Rsp));
	TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
	memcpy(Status, Rsp.status, 2);
	if (nRetCode != NO_ERROR || Rsp.code != DIAG_CMD_WriteNVitem || Rsp.status[0] != NV_DONE_S) {
		nRetCode = DIAG_CMD_WriteNVitem;
		if (Rsp.code == DIAG_BAD_SPC_MODE_F) {
			*Status = NV_SPLOCK_S;
		}
	}

	return nRetCode;
}

int CQDLCtrl::DiagCmdNvRead(unsigned short nItemID, int nLen,
							unsigned char* pData , unsigned short *Status)
{
	if (nLen > 128)	//// MAX LENGTH is 128
		nLen = 128;

	nv_cmd_t Req;
	memset(&Req, 0, sizeof(Req));
	Req.code = DIAG_CMD_ReadNVitem;
	memcpy(Req.id, &nItemID, 2);
	memcpy(Req.data, pData, nLen);

	nv_cmd_t Rsp;
	memset(&Rsp, 0, sizeof(Rsp));

	int nRetCode = ERROR_FUNCTION_FAILED;
	nRetCode = m_SerCom.TXRXDiag((char*)&Req, sizeof(Req), (char*)&Rsp, sizeof(Rsp));
	TRACE(_T("%s nRet:%d Rsp.code:0x%02x\n"), _T(__FUNCTION__), nRetCode, Rsp.code);
	memcpy(pData, Rsp.data, nLen);
	memcpy(Status, Rsp.status, 2);
	if (nRetCode != NO_ERROR || Rsp.code != DIAG_CMD_ReadNVitem || Rsp.status[0] != NV_DONE_S) {
		nRetCode = DIAG_CMD_ReadNVitem;
		if (Rsp.code == DIAG_BAD_PARM_F) {
			*Status = NV_UNREADABLE_S;
		}
		if (nRetCode == NO_ERROR && Rsp.code == DIAG_BAD_SPC_MODE_F) {
			*Status = NV_SPLOCK_S;
		}
		//if (Rsp.code == STREAMING_CMD_LOG) {
		//	TRACE(_T("%s target error:%s"), _T(__FUNCTION__), Rsp.data); // TODO: show this log outside
		//}
	}
	return nRetCode;
}

