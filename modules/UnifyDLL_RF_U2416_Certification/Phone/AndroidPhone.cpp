#include "stdafx.h"
#include "AndroidPhone.h"
#include "Shlwapi.h"
#include <io.h>



// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


//#define _USE_ADBSHELL_


RF_IMPLEMENT_DYNCREATE(CAndroidPhone)


CAndroidPhone::CAndroidPhone()
	: m_hQcomDiagModule(NULL)
	, m_hNeptuneCtrl(NULL)
	, m_hQMSLPhone(NULL)
{
}

CAndroidPhone::~CAndroidPhone()
{
	if (m_hQcomDiagModule != NULL)
	{
		FreeLibrary(m_hQcomDiagModule);
		m_hQcomDiagModule = NULL;
	}

	if (m_hNeptuneCtrl != NULL)
	{
		FreeLibrary(m_hNeptuneCtrl);
		m_hNeptuneCtrl = NULL;
	}

	if (m_hQMSLPhone != NULL)
	{
		QLIB_DisconnectServer(m_hQMSLPhone);
		m_hQMSLPhone = NULL;
	}
}

bool CAndroidPhone::InitData(std::map<std::string, std::string>& paramMap)
{
	// COM port
	if (paramMap.find(std::string("COMPort")) != paramMap.end())
	{
		m_strCOMport = paramMap[std::string("COMPort")];
		m_strDeviceName = m_strCOMport;
	}
	else
		return false;

	// Qisda command module
	if (paramMap.find(std::string("QisdaCmdModule")) != paramMap.end())
	{
		m_strQisdaCommandModuleName = paramMap[std::string("QisdaCmdModule")];
	}
	else
		return false;

	return true;
}

bool CAndroidPhone::WLANLoadFABin(char *szOutput)
{
	pDetroit_WLANLoadFABin pWLANLoadFABin = (pDetroit_WLANLoadFABin) GetProcAddress (m_hNeptuneCtrl, "Detroit_WLAN_LoadFABin");
	if (! pWLANLoadFABin){
		return false;
	}

	char szIn[FTD_BUFSIZE]={0};

	int ret = pWLANLoadFABin (FTD_PORT, FTD_TIMEOUT, szIn, szOutput);

	return FTD_OK == ret;
}

bool CAndroidPhone::WLANLoadNextBin(char *szOutput)
{
	pDetroit_WLANLoadNextBin pWLANLoadNextBin = (pDetroit_WLANLoadNextBin) GetProcAddress (m_hNeptuneCtrl, "Detroit_WLAN_LoadNextBin");
	if (! pWLANLoadNextBin){
		return false;
	}

	char szIn[FTD_BUFSIZE]={0};

	int ret = pWLANLoadNextBin (FTD_PORT, FTD_TIMEOUT, szIn, szOutput);

	return FTD_OK == ret;
}

bool CAndroidPhone::WLANSaveBinToFA()
{
	pDetroit_WLANSaveBinToFA pWLANSaveBinToFA = (pDetroit_WLANSaveBinToFA) GetProcAddress (m_hNeptuneCtrl, "Detroit_WLAN_SaveBin");
	if (! pWLANSaveBinToFA){
		return false;
	}

	char szIn[FTD_BUFSIZE]={0};
	char szOutput[FTD_BUFSIZE]={0};

	int ret = pWLANSaveBinToFA (FTD_PORT, FTD_TIMEOUT, szIn, szOutput);

	return FTD_OK == ret;
}

bool CAndroidPhone::MyExecAdbCmd(const char* chCmd, char **chReturn, bool bWithShell)
{
	bool ret = true;
	HANDLE hWrite, hRead;

	PROCESS_INFORMATION processInfo;
	memset(&processInfo, 0, sizeof(PROCESS_INFORMATION));

	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	try
	{
		if (! CreatePipe(&hRead, &hWrite, &saAttr, 0))
			throw _T("CreatePipe");

		STARTUPINFO si = {0};
		si.cb = sizeof (STARTUPINFO);
		si.hStdError = hWrite;
		si.hStdOutput = hWrite;
		si.hStdInput = hRead;
		si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;

		TCHAR szWorkingDir[MAX_PATH], szExePath[MAX_PATH];
		TCHAR szCmdLine[1024];

		// Set WorkPath and ExePath
		GetModuleFileName(NULL, szExePath, MAX_PATH);
		PathRemoveFileSpec(szExePath);
		_tcscpy(szWorkingDir, szExePath);

		SetCurrentDirectory(szWorkingDir);

		PathAppend(szExePath, "adb.exe");

		if (bWithShell)
			_stprintf(szCmdLine, _T("\"%s\" shell %s"), szExePath, chCmd);
		else
			_stprintf(szCmdLine, _T("\"%s\" %s"), szExePath, chCmd);

		if (! CreateProcess(NULL,	// do not send parameter
			szCmdLine,				// need enclosing quotation
			NULL,
			NULL,
			TRUE,					// important!
			CREATE_DEFAULT_ERROR_MODE,
			NULL,
			szWorkingDir,			// child process's full path.
			&si,
			&processInfo))
		{
			throw _T("CreateProcess");
		}

		WaitForSingleObject(processInfo.hProcess, INFINITE);

		// if needs return value
		if (chReturn != NULL)
		{
			DWORD dwAvail, numread;

			BOOL bRet = PeekNamedPipe (hRead, NULL, 0, NULL, &dwAvail, 0);
			if (bRet && dwAvail > 0)
			{
				*chReturn = new char [dwAvail+1];
				char *pBuffer = *chReturn;
				memset(pBuffer, 0, dwAvail+1);

				::ReadFile(hRead, pBuffer, dwAvail, &numread, 0);
			}
		}
	}
	catch (const TCHAR* szError)
	{
		LPVOID lpMsgBuf;

		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, GetLastError(),
			//MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), 
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);

		TCHAR szMessage[1024];
		_stprintf (szMessage, "%s: %s", szError, lpMsgBuf);

		LocalFree(lpMsgBuf);
		ret = false;
	}

	CloseHandle(hRead);
	CloseHandle(hWrite);
	CloseHandle(processInfo.hProcess);
	processInfo.hProcess = NULL;
	CloseHandle(processInfo.hThread);
	processInfo.hThread = NULL;

	return ret; 
}


// --------------------- FA functions ----------------------

bool CAndroidPhone::GetFAData(char* szValue, int iSize)
{
	pDetroit_FAC_FARead FA_Read = NULL;
	FA_Read = (pDetroit_FAC_FARead)GetProcAddress(m_hNeptuneCtrl, "Detroit_FAC_FARead") ;

	if (!FA_Read)
	{
		return false ;
	}

	char pOutput[512] = {0};
	char chIn[512] = {0};
	strcpy (chIn, "0,512");

	bool ret = FA_Read(FTD_PORT, FTD_TIMEOUT, chIn, pOutput) == FTD_OK; 
	if (ret)
	{
		memset (szValue, 0, iSize);
		strncpy(&szValue[32], &pOutput[94], 10); // Detroit picasso byte 94-103, UI put it at byte 32
	}

	return ret;
}

bool CAndroidPhone::SetFAData(char* szValue)
{
	
	pDetroit_FAC_FAWrite FA_Write = NULL;
	FA_Write = (pDetroit_FAC_FAWrite)GetProcAddress(m_hNeptuneCtrl, "Detroit_FAC_FAWrite") ;

	char pInBuffer[4096] = {0};
	char pOutBuffer[4096]={0};
	memset(pInBuffer, 0, 4096);

	sprintf_s(pInBuffer,sizeof(pInBuffer),"0,512,%s",szValue);
	if (!FA_Write) 
	{
		return false;
	}
	bool ret = FA_Write(FTD_PORT, FTD_TIMEOUT, pInBuffer, pOutBuffer) == FTD_OK ;
	return ret;
}

bool CAndroidPhone::GetFASector( int i_sectorNum, char *sz_sectorData, int i_sectorSize )
{
	ASSERT(i_sectorSize == 512);

	pDetroit_FAC_FARead FA_Read = NULL;
	FA_Read = (pDetroit_FAC_FARead)GetProcAddress(m_hNeptuneCtrl, "Detroit_FAC_FARead") ;

	if (!FA_Read)
	{
		return false ;
	}

	char chIn[512] = {0};

	char sz_outBuffer[4096]={0};//must be 4096,lower will set 4096 bytes to this cach
	
	sprintf_s(chIn,sizeof(chIn),"%d,%d",i_sectorSize*i_sectorNum,i_sectorSize);

	bool ret = FA_Read(FTD_PORT, FTD_TIMEOUT, chIn, sz_outBuffer) == FTD_OK; 

	if (ret)
	{
		memcpy(sz_sectorData,sz_outBuffer,i_sectorSize);
	}
	else
	{
		memset(sz_sectorData,0,i_sectorSize);
	}
	
	return ret;
}

bool CAndroidPhone::SetFASector( int i_sectorNum, char *sz_sectorData, int i_sectorSize )
{
	ASSERT(i_sectorSize == 512);

	pDetroit_FAC_FAWrite FA_Write = NULL;
	FA_Write = (pDetroit_FAC_FAWrite)GetProcAddress(m_hNeptuneCtrl, "Detroit_FAC_FAWrite") ;

	if (!FA_Write) 
	{
		return false;
	}

	char pInBuffer[560];
	char sz_outBuffer[4096]={0};
	char sz_tmp[256] = {0};
	memset(sz_tmp,0,sizeof(sz_tmp));
	memset(pInBuffer, 0, sizeof(pInBuffer));

	bool ret = false;
	//chIn(begin position, total size)
	if ((i_sectorNum == 0) && (strlen(sz_sectorData) < 256))
	{
		memcpy(sz_tmp,sz_sectorData,sizeof(sz_tmp));
		sprintf_s(pInBuffer,sizeof(pInBuffer),"0,%d,%s",strlen(sz_tmp),sz_tmp);
		ret = FA_Write(FTD_PORT, FTD_TIMEOUT, pInBuffer, sz_outBuffer) == FTD_OK ;
		if (ret)
		{
			memcpy(sz_tmp,sz_sectorData+256,sizeof(sz_tmp));
			sprintf_s(pInBuffer,sizeof(pInBuffer),"256,%d,%s",strlen(sz_tmp),sz_tmp);
			ret = FA_Write(FTD_PORT, FTD_TIMEOUT, pInBuffer, sz_outBuffer) == FTD_OK ;
		}
	}
	else
	{
		sprintf_s(pInBuffer,sizeof(pInBuffer),"%d,%d,%s",i_sectorSize*i_sectorNum,strlen(sz_sectorData),sz_sectorData);

		bool ret = FA_Write(FTD_PORT, FTD_TIMEOUT, pInBuffer, sz_outBuffer) == FTD_OK ;
	}

	return ret;

}

bool CAndroidPhone::GetFAData_QMSL(char* szValue, int iSize)
{
	/*
	unsigned char pBuffer1[128];
	unsigned char pBuffer2[128];
	unsigned char pBuffer3[128];
	unsigned char pBuffer4[128];

	unsigned short unStatus;

	if (m_hQMSLPhone != NULL)
	{
		if (! QLIB_DIAG_NV_READ_F(m_hQMSLPhone, 2497, pBuffer1, 128, &unStatus))
			return false;
		if (! QLIB_DIAG_NV_READ_F(m_hQMSLPhone, 2498, pBuffer2, 128, &unStatus))
			return false;
		if (! QLIB_DIAG_NV_READ_F(m_hQMSLPhone, 2499, pBuffer3, 128, &unStatus))
			return false;
		if (! QLIB_DIAG_NV_READ_F(m_hQMSLPhone, 2500, pBuffer4, 128, &unStatus))
			return false;

		memcpy(szValue, pBuffer1, 128);
		memcpy(szValue + 128, pBuffer2, 128);
		memcpy(szValue + 256, pBuffer3, 128);
		memcpy(szValue + 384, pBuffer4, 128);
		return true;
	}
	*/

	return false;
}

bool CAndroidPhone::SetFAData_QMSL(char* szValue)
{
	/*
	unsigned short unStatus;

	if (m_hQMSLPhone != NULL)
	{
		if (! QLIB_DIAG_NV_WRITE_F(m_hQMSLPhone, 2497, (unsigned char*)szValue, 128, &unStatus))
			return false;
		if (! QLIB_DIAG_NV_WRITE_F(m_hQMSLPhone, 2498, (unsigned char*)(szValue + 128), 128, &unStatus))
			return false;
		if (! QLIB_DIAG_NV_WRITE_F(m_hQMSLPhone, 2499, (unsigned char*)(szValue + 256), 128, &unStatus))
			return false;
		if (! QLIB_DIAG_NV_WRITE_F(m_hQMSLPhone, 2500, (unsigned char*)(szValue + 384), 128, &unStatus))
			return false;

		return true;
	}
	*/

	return false;
}


// -------------------- Phone functions --------------------

bool CAndroidPhone::Initial()
{
	// Load library
	/*if (NULL == m_hQcomDiagModule)
	{
		m_hQcomDiagModule = LoadLibrary(m_strQisdaCommandModuleName.c_str());
		if (NULL == m_hQcomDiagModule)
			return false;
	}*/

	if(! Initial_QMSL())
		return false;
	else
		return Initial_FTD();
}

bool CAndroidPhone::Disconnect()
{
	if (m_hQcomDiagModule)
	{
		FreeLibrary(m_hQcomDiagModule);
		m_hQcomDiagModule = NULL;
	}

	return Disconnect_FTD() ;
}

bool CAndroidPhone::IsConnected()
{
	/*pNV_IsConnect_SPU Is_Connected = (pNV_IsConnect_SPU)GetProcAddress(m_hQcomDiagModule, "NV_IsConnect_SPU");
	if (NULL == Is_Connected)
		return false;

	if (! Is_Connected(m_strCOMport.c_str()) )
		return false ;*/

	// FTD
	return IsConnected_FTD() ;
}

bool CAndroidPhone::Reboot()
{
	pNV_ResetHandset_SPU ResetHandset = (pNV_ResetHandset_SPU)GetProcAddress(m_hQcomDiagModule, "NV_ResetHandset_SPU");
	if (! ResetHandset)
		return false;

	return ResetHandset(m_strCOMport.c_str());
}

bool CAndroidPhone::ReadNVItem(WORD NVItem, BYTE* pBuffer, const int BufferSize)
{
	pNV_ReadItem NV_ReadItem = (pNV_ReadItem)GetProcAddress(m_hQcomDiagModule, "NV_ReadItem") ;
		if (! NV_ReadItem)
			return false;

	WORD nvstatus;

	return NV_ReadItem(atoi(m_strCOMport.c_str()), NVItem, pBuffer, BufferSize, &nvstatus);
}

bool CAndroidPhone::WriteNVItem(WORD NVItem, BYTE* pBuffer, const int BufferSize)
{
	pNV_WriteItem NV_WriteItem = (pNV_WriteItem)GetProcAddress(m_hQcomDiagModule, "NV_WriteItem") ;
		if (! NV_WriteItem)
			return false;

	WORD nvstatus;

	return NV_WriteItem(atoi(m_strCOMport.c_str()), NVItem, pBuffer, BufferSize, &nvstatus);
}


bool CAndroidPhone::NVBackup()
{
	pDetroit_NVBackup_SPU Detroit_NVBackup_SPU = (pDetroit_NVBackup_SPU)GetProcAddress(m_hQcomDiagModule, "Detroit_NVBackup_SPU");
	if (! Detroit_NVBackup_SPU)
		return false;

	return Detroit_NVBackup_SPU(const_cast<char*>(m_strCOMport.c_str()));
}


// ------------------------- ATControl --------------------------

bool CAndroidPhone::Send_ATCommand(char* szPort, char* szCommand, char* szResponse)
{
  	pATControl_SPU ATControl_SPU = (pATControl_SPU)GetProcAddress(m_hQcomDiagModule, "ATControl_SPU");
	if (! ATControl_SPU)
		return false;

	return ATControl_SPU(szPort, szCommand, szResponse);
}


// ---------------- Detroit FTD basic commands ----------------

bool CAndroidPhone::Initial_FTD()
{
	if (m_hNeptuneCtrl == NULL)
	{
		m_hNeptuneCtrl = LoadLibrary("NeptuneCtrl.dll");
		if (! m_hNeptuneCtrl)
			return false;

		// Call init device
		pInitDevice FTD_InitDevice = (pInitDevice)GetProcAddress(m_hNeptuneCtrl, "InitDevice");
		if (FTD_InitDevice == NULL)
			return false;

		if (FTD_InitDevice(FTD_PORT, FTD_TIMEOUT) != FTD_OK)
			return false;
	}

	return true;
}

bool CAndroidPhone::Disconnect_FTD()
{
	// Shutdown ADB Server
	bool bRes = MyExecAdbCmd("kill-server", NULL, false) ;

	// Disconnect FTD
	if ( m_hNeptuneCtrl )
	{
		// Call free device
		pFreeDevice FTD_FreeDevice = (pFreeDevice)GetProcAddress(m_hNeptuneCtrl, "FreeDevice");
		if (NULL == FTD_FreeDevice)
			return false;

		if (FTD_FreeDevice (FTD_PORT, FTD_TIMEOUT) != FTD_OK)
			return false;

		// Free device
		FreeLibrary(m_hNeptuneCtrl);
		m_hNeptuneCtrl = NULL;
	}

	return true;
}

bool CAndroidPhone::IsConnected_FTD()
{
	// Test FTD
	pDetroit_TestFtdMode Detroit_TestFtdMode = (pDetroit_TestFtdMode)GetProcAddress(m_hNeptuneCtrl, "Detroit_TestFtdMode");
	if (! Detroit_TestFtdMode)
		return false;

	char chError[FTD_BUFSIZE];
	if (Detroit_TestFtdMode (FTD_PORT, FTD_TIMEOUT, chError) != FTD_OK)
		return false;

	return true;
}


// ------------------------- Detroit BT -----------------------

bool CAndroidPhone::BTModuleOnOffSwitch(bool bEnable, int idelay)
{
#ifdef _USE_ADBSHELL_
	if (bEnable)
	{
		bool b_ret = MyExecAdbCmd("am start -n com.oem.engineeringmode.Bluetooth/.BTAddress");
		if (b_ret)
		{
			b_ret = MyExecAdbCmd("\"echo 200000 > /sys/module/hci_uart/parameters/tx_idle_delay\"");
		}
		if (b_ret)
		{
			b_ret = MyExecAdbCmd("bttest enable");
		}
		Sleep(idelay*1000);		
		return b_ret;
	}
	else
	{
		return MyExecAdbCmd("bttest disable");
	}
#else
	bool b_ret = MyExecAdbCmd("stop");
	if (! b_ret)
		return false;

	pDetroit_BTTestMode FTD_BTEnterTestMode = (pDetroit_BTTestMode)GetProcAddress(m_hNeptuneCtrl, "Detroit_BTTestMode");
	if (! FTD_BTEnterTestMode)
		return false;

	char szError[FTD_BUFSIZE] = {0};
	char szInput[FTD_BUFSIZE] = {0};

	if (bEnable)
	{
		//sprintf(szInput, "1,%d",idelay);
		sprintf(szInput, "1");
	}
	else
	{
		sprintf(szInput, "0");
	}

	int ret = FTD_BTEnterTestMode(FTD_PORT, FTD_TIMEOUT, szInput, szError);
	return ret == FTD_OK;
#endif

	return true;
}

bool CAndroidPhone::BTEnterTestMode()
{
#ifdef _USE_ADBSHELL_
	bool b_ret = MyExecAdbCmd("bttest dut_mode_enable");
	Sleep(1000);
	return b_ret;
	
#else
#endif
	return true;
}

bool CAndroidPhone::BTStartTxPower(int ichannel, int ipower, int itype)
{
#ifdef _USE_ADBSHELL_
	return MyExecAdbCmd("bttest tx_continous_mode 1 7 4");
#else
	pDetroit_BTTXPower FTD_BTTXPower = (pDetroit_BTTXPower)GetProcAddress(m_hNeptuneCtrl, "Detroit_BTTXPower");
	if (! FTD_BTTXPower)
	{
		return false;
	}
	char szError[FTD_BUFSIZE] = {0};
	char szInput[FTD_BUFSIZE] = {0};
	sprintf(szInput,"%d,%d,%d", ichannel,ipower,itype);
	int ret = FTD_BTTXPower(FTD_PORT, FTD_TIMEOUT, szInput, szError);
	return ret == FTD_OK;
#endif
}

bool CAndroidPhone::BTStartTxLtePower(int ichannel, int iDataLength, int itype)
{
#ifdef _USE_ADBSHELL_
	return MyExecAdbCmd("bttest tx_continous_mode 1 7 4");
#else
	pDetroit_BTTXPower FTD_BTTXLtePower = (pDetroit_BTTXPower)GetProcAddress(m_hNeptuneCtrl, "Detroit_BTLteTx");
	if (! FTD_BTTXLtePower)
	{
		return false;
	}
	char szError[FTD_BUFSIZE] = {0};
	char szInput[FTD_BUFSIZE] = {0};
	sprintf(szInput,"%d,%d,%d", ichannel,iDataLength,itype);
	int ret = FTD_BTTXLtePower(FTD_PORT, FTD_TIMEOUT, szInput, szError);
	return ret == FTD_OK;
#endif
}



// ----------------------Detroit QMI----------------------
bool CAndroidPhone::Detroit_QmiRfTxTest()
{
	pDetroit_QmiRfTxTest FTD_Detroit_QmiRfTxTest = (pDetroit_QmiRfTxTest)GetProcAddress(m_hNeptuneCtrl, "Detroit_QmiRfTxTest");
	if (! FTD_Detroit_QmiRfTxTest){
		return false;
	}

	char szError[FTD_BUFSIZE] = {0};
	char szInput[FTD_BUFSIZE] = {0};

	int ret = FTD_Detroit_QmiRfTxTest(FTD_PORT, FTD_TIMEOUT, szInput, szError);

	return ret == FTD_OK;
}

bool CAndroidPhone::Detroit_QmiRfTxOff()
{
	pDetroit_QmiRfTxOff FTD_Detroit_QmiRfTxOff = (pDetroit_QmiRfTxOff)GetProcAddress(m_hNeptuneCtrl, "Detroit_QmiRfTxOff");
	if (! FTD_Detroit_QmiRfTxOff){
		return false;
	}

	char szError[FTD_BUFSIZE] = {0};
	char szInput[FTD_BUFSIZE] = {0};

	int ret = FTD_Detroit_QmiRfTxOff(FTD_PORT, FTD_TIMEOUT, szInput, szError);

	return ret == FTD_OK;
}

// ----------------------- Detroit WiFi -----------------------

bool CAndroidPhone::WiFiModuleOnOffSwitch(bool bEnable)
{
#ifdef _USE_ADBSHELL_
	bool b_ret = false;
	if (bEnable)
	{
		b_ret = MyExecAdbCmd("./system/bin/fa_read_wifi");

		if (b_ret)
		{
			b_ret = MyExecAdbCmd("insmod /system/lib/modules/cfg80211.ko");
		}
		
		if (b_ret)
		{
			b_ret = MyExecAdbCmd("insmod /system/lib/modules/ath6kl_sdio.ko testmode=1");
		}
		
		return b_ret;
	}
	else
	{
		b_ret = MyExecAdbCmd("rmmod ath6kl_sdio");
		if (b_ret)
		{
			b_ret = MyExecAdbCmd("rmmod cfg80211");
		}
		return b_ret;	
	}
		
#else
	pDetroit_WLAN_Mode Detroit_WLAN_Mode = (pDetroit_WLAN_Mode)GetProcAddress(m_hNeptuneCtrl, "Detroit_WLAN_Mode");
	if (! Detroit_WLAN_Mode)
		return false;

	int ret;
	char szError[FTD_BUFSIZE];
	int port = atoi(m_strCOMport.c_str());

	if (bEnable)
		ret = Detroit_WLAN_Mode(FTD_PORT, FTD_TIMEOUT, "2", szError);
	else
		ret = Detroit_WLAN_Mode(FTD_PORT, FTD_TIMEOUT, "0", szError);

	return FTD_OK == ret;
#endif

	return true;
}

bool CAndroidPhone::WiFiStartTxPower(int iRate, int iChannel, int iPower)
{

	int cmdRate = 0;

	switch (iRate)
	{
	case 0:
		break;
	case 1:
		cmdRate = 0;
		break;
	case 2:
		cmdRate = 1;
		break;
	case 6:
		cmdRate = 4;
		break;
	case 7:
		cmdRate = 19;
		break;
	case 9:
		cmdRate = 5;
		break;
	case 11:
		cmdRate = 3;
		break;
	case 12:
		cmdRate = 6;
		break;
	case 18:
		cmdRate = 7;
		break;
	case 24:
		cmdRate = 8;
		break;
	case 36:
		cmdRate = 9;
		break;
	case 48:
		cmdRate = 10;
		break;
	case 54:
		cmdRate = 11;
		break;
	default:
		return false;
	}
#ifdef _USE_ADBSHELL_

	char szCmd[512];
	memset(szCmd, 0, 512);
	if (0 == iRate && 0 == iChannel && 0 == iPower)
		sprintf(szCmd, "athtestcmd -i wlan0 --tx off");
	else
		sprintf(szCmd, "athtestcmd -i wlan0 --tx tx99 --txfreq %d --txpwr %d --txrate %d", iChannel, iPower, cmdRate);

	return MyExecAdbCmd(szCmd);
#else
	pDetroit_WLAN_TXPower Detroit_WLAN_TXPower = (pDetroit_WLAN_TXPower)GetProcAddress(m_hNeptuneCtrl, "Detroit_WLAN_TXPower");
	if (! Detroit_WLAN_TXPower)
		return false;

	char szError[FTD_BUFSIZE], szIn[FTD_BUFSIZE];
	//int port = atoi(m_strCOMport.c_str());
	sprintf(szIn, "%d,%d,%d", iChannel, iPower, cmdRate);

	int ret = Detroit_WLAN_TXPower(FTD_PORT, FTD_TIMEOUT, szIn, szError);
	return FTD_OK == ret;
#endif

	return true;
}

bool CAndroidPhone::WifiModuleOn (bool bEnable)
{
#ifdef _NOT_USE_FTD_

	if (bEnable)
	{
		bool ret = false;
		char **pbuffer = NULL;

		//MyExecAdbCmd(pbuffer, "shell", "rmmod wlan");
		/*
		MyExecAdbCmdEx (pbuffer, "pull", "/etc/wifi/wpa_supplicant.conf");
		::Sleep(500);
		MyExecAdbCmdEx (pbuffer, "pull", "/persist/WCNSS_qcom_cfg.ini");
		::Sleep(500);
		MyExecAdbCmdEx (pbuffer, "push", "wpa_supplicant.conf /data/misc/wifi/");
		::Sleep(500);
		MyExecAdbCmdEx (pbuffer, "push", "WCNSS_qcom_cfg.ini /data/misc/wifi/");
		::Sleep(500);*/
		MyExecAdbCmdEx (pbuffer, "shell", "insmod /system/lib/modules/wlan.ko con_mode=5");
		::Sleep(500);
		MyExecAdbCmdEx (pbuffer, "shell", "ptt_socket_app");
		/*::Sleep(500);
		MyExecAdbCmdEx (pbuffer, "shell", "ftmdaemon");
		::Sleep(500);
*/
		//FTM start
		//ret = QLIB_FTM_WLAN_GEN6_START(m_hQMSLPhone, 3660);

		//return ret;
		return true;
	}
	else
	{
		//FTM stop
		return QLIB_FTM_WLAN_GEN6_STOP(m_hQMSLPhone);
	}

#else
	pDetroit_WLAN_Mode Detroit_WLAN_Mode = (pDetroit_WLAN_Mode) GetProcAddress (m_hNeptuneCtrl, "Detroit_WLAN_Mode");
	if (! Detroit_WLAN_Mode)
		return false;

	int ret = 0;
	bool isOk = false;
	char szInput[4096] = {0};
	char szError[FTD_BUFSIZE];

	//if( nBin>=0 ){
		sprintf(szInput, "2");
	//}else{
	//	sprintf(szInput, "2,NULL");
	//}
	if (bEnable)
	{
		ret = Detroit_WLAN_Mode (FTD_PORT, FTD_TIMEOUT, szInput, szError); //2 -> test mode on
		if( ret==FTD_OK ){
			QLIB_SetLibraryMode(true);
			//isOk = QLIB_FTM_WLAN_GEN6_START(m_hQMSLPhone, 3660);
			//isOk = QLIB_FTM_WLAN_GEN6_START(m_hQMSLPhone, 3680);
		}
		return isOk;
	}
	else
	{
		isOk = QLIB_FTM_WLAN_GEN6_STOP(m_hQMSLPhone);
		if( isOk ){
			ret = Detroit_WLAN_Mode (FTD_PORT, FTD_TIMEOUT, "0", szError); //0 -> off
		}
		
		return FTD_OK == ret;
	}
#endif

	return true;
}

bool CAndroidPhone::WifiModuleOnCertification(bool bEnable)
{
	char szErr[FTD_BUFSIZE] = {0};
	char szOut[FTD_BUFSIZE] = {0};
	char szIn[FTD_BUFSIZE] ={0};
	CString csCommandShell;

	csCommandShell.Format("shell start ftd");
	if( !ExecAdbCommand(csCommandShell, szOut, szErr) )
		return false;
	
	Sleep(500);

	pDetroit_WLAN_Mode Detroit_WLAN_Mode = (pDetroit_WLAN_Mode)GetProcAddress(m_hNeptuneCtrl, "Detroit_WLAN_Mode");
	if (!Detroit_WLAN_Mode)
	{
		return false;
	}

	int ret = 0;
	//bool isOk = false;
	char szInput[4096] = {0};
	char szError[FTD_BUFSIZE];


	char buf[32];
	sprintf(szInput, _T("2"));

	if (bEnable)
	{
		ret = Detroit_WLAN_Mode (FTD_PORT, FTD_TIMEOUT, szInput, szError); //2 -> test mode on
		return FTD_OK == ret;
	}
	else
	{

		ret = Detroit_WLAN_Mode(FTD_PORT, FTD_TIMEOUT, _T("0"), szError); //0 -> off
		return FTD_OK == ret;
	}

	return false;
}

bool CAndroidPhone::WifiPowerOnTx (int iRate, int iChannel, int iPower)
{
//#ifdef _NOT_USE_FTD_

	if (0 == iRate && 0 == iChannel && 0 == iPower) // Turn off Tx Power
	{
		bool ret = true;

		//Stop TX
		return ret && QLIB_FTM_WLAN_GEN6_TX_PKT_START_STOP(m_hQMSLPhone, 0);
	}

	else
	{
		unsigned int cmdRateMap = 0;

		switch(iRate)
		{
		case 11:
			cmdRateMap = 3;
			break;

		case 54:
			cmdRateMap = 14;
			break;

		case 72:
			cmdRateMap = 38;
			break;

		default:
			return false;
		}

		unsigned int cmdPreamble = 0;

		switch(cmdRateMap)
		{
		/*	0: HAL_PHY_RATE_11B_LONG_1_MBPS
			1: HAL_PHY_RATE_11B_LONG_2_MBPS
			2: HAL_PHY_RATE_11B_LONG_5_5_MBPS
			3: HAL_PHY_RATE_11B_LONG_11_MBPS	*/
		case 0: case 1: case 2: case 3:
			cmdPreamble = 4;
			break;

		/*	4: HAL_PHY_RATE_11B_SHORT_2_MBPS
			5: HAL_PHY_RATE_11B_SHORT_5_5_MBPS
			6: HAL_PHY_RATE_11B_SHORT_11_MBPS	*/
		case 4: case 5: case 6:
			cmdPreamble = 3;
			break;

		/*	7:  HAL_PHY_RATE_11A_6_MBPS
			8:  HAL_PHY_RATE_11A_9_MBPS
			9:  HAL_PHY_RATE_11A_12_MBPS
			10: HAL_PHY_RATE_11A_18_MBPS
			11: HAL_PHY_RATE_11A_24_MBPS
			12: HAL_PHY_RATE_11A_36_MBPS
			13: HAL_PHY_RATE_11A_48_MBPS
			14: HAL_PHY_RATE_11A_54_MBPS	*/
		case 7: case 8: case 9: case 10: case 11: case 12: case 13: case 14:
			cmdPreamble = 0;
			break;

		/*	38: HAL_PHY_RATE_MCS_1NSS_MM_SG_72_2_MBPS	*/
		case 38:
			cmdPreamble = 2;
			break;

		default:
			return false;
		}

		for(int i = 0;i < 1;i++)
		{
			Sleep(500);
			/*
			if(iChannel >= 36)
			{
				if(!QLIB_FTM_WLAN_GEN6_SET_CHANNEL(m_hQMSLPhone, 40) ||
					!QLIB_FTM_WLAN_GEN6_ENABLE_CHAINS(m_hQMSLPhone, 6) ||
					!QLIB_FTM_WLAN_GEN6_SET_TX_FRAME (m_hQMSLPhone, 0, 1000, 200, true, iRate, cmdPreamble) ||
					!QLIB_FTM_WLAN_GEN6_CLOSE_TPC_LOOP(m_hQMSLPhone, 0) ||
					!QLIB_FTM_WLAN_GEN6_SET_TX_WAVEFORM_GAIN_V2(m_hQMSLPhone, iPower) ||
					!QLIB_FTM_WLAN_GEN6_TX_PKT_START_STOP(m_hQMSLPhone, 1))
					continue;
			}

			if(!QLIB_FTM_WLAN_GEN6_SET_CHANNEL(m_hQMSLPhone, iChannel) ||
				!QLIB_FTM_WLAN_GEN6_ENABLE_CHAINS(m_hQMSLPhone, 6) ||
				!QLIB_FTM_WLAN_GEN6_SET_TX_FRAME (m_hQMSLPhone, 0, 1000, 200, true, iRate, cmdPreamble) ||
				!QLIB_FTM_WLAN_GEN6_CLOSE_TPC_LOOP(m_hQMSLPhone, 0) ||
				!QLIB_FTM_WLAN_GEN6_SET_TX_WAVEFORM_GAIN_V2(m_hQMSLPhone, iPower) ||
				!QLIB_FTM_WLAN_GEN6_TX_PKT_START_STOP(m_hQMSLPhone, 1))
				continue;
			*/
			bool isOk = true;
			//Set FTM start
			//isOk = QLIB_FTM_WLAN_GEN6_START(m_hQMSLPhone, 3660);
			//if (!isOk) { 
			//	continue;
			//}
			//Set TX
			isOk = QLIB_FTM_WLAN_GEN6_ENABLE_CHAINS(m_hQMSLPhone, 6);
			if (!isOk) { 
				continue;
			}
			//Set close loop
			isOk = QLIB_FTM_WLAN_GEN6_CLOSE_TPC_LOOP(m_hQMSLPhone, 1);
			if (!isOk) {
				continue;
			}
			//Set channel
			isOk = QLIB_FTM_WLAN_GEN6_SET_CHANNEL(m_hQMSLPhone, iChannel);
			if (!isOk) { 
				continue;
			}
			//Set rate
			isOk = QLIB_FTM_WLAN_GEN6_SET_TX_FRAME(m_hQMSLPhone, 0, 2400, 10, true, cmdRateMap, cmdPreamble);
			if (!isOk) { 
				continue;
			}
/*			//Set Packet Gain
			isOk = QLIB_FTM_WLAN_GEN6_SET_TX_WAVEFORM_GAIN_V2(m_hQMSLPhone, iPower);
			if (!isOk) { 
				continue;
			}*/
			//Set closed loop power
			isOk = QLIB_FTM_WLAN_GEN6_SET_CLOSED_LOOP_POWER(m_hQMSLPhone, iPower);
			if (!isOk) { 
				continue;
			}
			/* marked for CDMA
			//Set power index source
			isOk = QLIB_FTM_WLAN_GEN6_SET_PWR_INDEX_SOURCE(m_hQMSLPhone, 2);
			if (!isOk) { 
				continue;
			}*/
			
			//TX start
			isOk = QLIB_FTM_WLAN_GEN6_TX_PKT_START_STOP(m_hQMSLPhone, 1);
			if (!isOk) { 
				continue;
			}

			return true;
		}

		return false;
	}
/*
#else // use FTD

	int cmdRate=0;

	switch (iRate)
	{
	case 0: break;
	case 1:  cmdRate = 0; break;
	case 2:  cmdRate = 1; break;
		//case 5.5: cmdRate = 2; break;
	case 6:  cmdRate = 4; break;
	case 7:  cmdRate = 19; break; //HT20 MCS7
	case 9:  cmdRate = 5; break;
	case 11: cmdRate = 3; break;
	case 12: cmdRate = 6; break;
	case 18: cmdRate = 7; break;
	case 24: cmdRate = 8; break;
	case 36: cmdRate = 9; break;
	case 48: cmdRate = 10; break;
	case 54: cmdRate = 11; break;
	default:
		return false;
	}

	pC7_WLAN_TXPower C7_WLAN_TXPower = (pC7_WLAN_TXPower) GetProcAddress (m_hNeptuneCtrl, "C7_WLAN_TXPower");
	if (! C7_WLAN_TXPower)
		return false;

	char szError[FTD_BUFSIZE], szIn[FTD_BUFSIZE];

	if (0==iRate && 0==iChannel && 0==iPower) // turn off
		strcpy (szIn, "0");
	else
		sprintf (szIn, "%d,%d,%d", iChannel, iPower, cmdRate);

	int ret = C7_WLAN_TXPower (FTD_PORT, 5000, szIn, szError);
	return FTD_OK == ret;

#endif
*/
	return false;
}

// Data storage
enum NVMEM_OPTION
{
   DataNone = 0,
   DataFlash,
   DataEeprom,
   DataOtp,
   DataDontLoad,
   DataFile,
   DataDram
};

double asyncPMCB(unsigned int iGain, unsigned int iFreq, double dPowerLevel,
   unsigned int iNumAvg, unsigned int iTriggerType, int iTriggerLevel)
{
	
   double power[100], pwr;
   double measuredAvgPower = 0;
   unsigned int it = 0;
   while (it < iNumAvg)
   {
      printf("Please enter a power to simulate the measured power\n");
      scanf("%lf",&pwr);
      // get measured power from your instrument for your calibration program
      power[it ++] = pwr;
      measuredAvgPower += power[it-1];
   }
   if (iNumAvg>0)
      measuredAvgPower = measuredAvgPower/iNumAvg; // average the measured iNumAvg powers;
   else
      measuredAvgPower = -999.00;
   return measuredAvgPower;
}

bool CAndroidPhone::WifiPowerOnTxCertification (int iChannel, int iPower, int iRateBitIndex, int iWlandMode, int iChain)
{
	char sLibraryVersion[50];
	bool isOk = true;
	QLIB_GetLibraryVersion(sLibraryVersion );
	printf("QLibrary Demo\n\n");
	printf("QLIB DLL Version: %s\n\n", sLibraryVersion );

	TCHAR szNowPath[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szNowPath);
	CString path = szNowPath;
	CString csBinPath;
	//csBinPath.Format("%s\\u2416\\bdwlan30.bin", path);
	csBinPath.Format("%s\\u2416\\bdwlan30.bin", "C:\\");
	//C:\u2416

	// ROME Chip ID
	#define ROME_CHIPID   0x3e
	// Loopback IP for local system or any remote system IP with DUT
	char UDT_IP[20] = "127.0.0.1";
	// Chip specific DLL
	#define ROME_DLLID    "qc6174"


	//CString csBIN_FILE = BIN_FILE;
	if (_taccess(csBinPath, 0) != 0) 
	{
		CString csMsg = csBinPath + _T("not exist");
		AfxMessageBox(csMsg);
		return false;
	}

	QLIB_FTM_WLAN_Atheros_UNLoadDUT(m_hQMSLPhone); //unload first;
	
	unsigned char bRet;
	bRet = 	QLIB_FTM_WLAN_Atheros_LoadDUT(m_hQMSLPhone,(unsigned char  *)ROME_DLLID,(unsigned char *)csBinPath.GetBuffer(), DataFile, ROME_CHIPID);
	csBinPath.ReleaseBuffer();
	if ( !bRet ){
		return false;
	}

	if ( !bRet ){
		return false;
	}
		for(int i = 0;i < 1;i++) 
		{
			Sleep(500);

			bool isOk = true;

			isOk = QLIB_FTM_WLAN_TLV_Create(m_hQMSLPhone, _OP_TX);
			if (!isOk) {
					continue;
			}

			char buf[32];
			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("txMode"), _itoa(3,buf,10)); // int txMode = 3; // Tx99 a = b = g = ac = n
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("channel"), _itoa( iChannel,buf,10));//iChannel
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("tpcm"), _itoa(0,buf,10));
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("txPower0"), _itoa(iPower, buf ,10)); //iPower
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("antenna"), _itoa(0,buf,10));
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("bandwidth"), _itoa(0,buf,10));
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("rateBitIndex0"), _itoa(iRateBitIndex,buf,10));
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("wlanMode"), _itoa(iWlandMode,buf,10));//b = 4, a/g = 0,  n = 1, ac = 8
			if (!isOk) {
					continue;
			}

			iChain += 1;
			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("txChain0"), _itoa(iChain,buf,10)); //chain0, chain 1
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("pktLen0"), _itoa( 1500, buf, 10));//packet size , iPayloadSize
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("shortGuard"), _itoa(0,buf,10));
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("numPackets"), _itoa(0,buf,10)); //o for count tx 
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("txPattern"), _itoa(0,buf,10));
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("scramblerOff"),_itoa(0,buf,10));
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("agg"),_itoa(1,buf,10));//aggregate 
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("aifsn"),_itoa(1,buf,10));
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("broadcast"), _itoa(0,buf,10));//broadcast, 
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("flags"), _itoa(24,buf,10));
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_Complete(m_hQMSLPhone);
		   
			//Sleep(5000);
		   // Start to run CLPC calibration iterations by power measurement call back
			//int numMeasAvg = 1;
		 //  asyncPMMessageCB pPMfunc = &asyncPMCB;
		 //  QLIB_FTM_WLAN_Atheros_Tx_CAL(m_hQMSLPhone, pPMfunc, (unsigned int)numMeasAvg);
		//   QLIB_FTM_WLAN_Atheros_Tx_CAL(m_hQMSLPhone, NULL, (unsigned int)numMeasAvg);

			if (!isOk) {
				Sleep(10000);
				continue;
			}
		}

	return isOk;
}

bool CAndroidPhone::WifiPowerOn5GTxCertification(int iRate, int iChannel, int iPower, int iPreamble, int iPayloadSize, int iSpacing, int iBond,int iChain)
{
	if (0 == iRate && 0 == iChannel && 0 == iPower) // Turn off Tx Power
	{
		bool ret = true;

		//Stop TX
		return ret && QLIB_FTM_WLAN_GEN6_TX_PKT_START_STOP(m_hQMSLPhone, 0);
	}
	else
	{
		for(int i = 0;i < 1;i++)
		{
			Sleep(500);

			bool isOk = true;

			//Set TX
			isOk = QLIB_FTM_WLAN_GEN6_ENABLE_CHAINS(m_hQMSLPhone, 6);
			if (!isOk) { 
				continue;
			}
			//Set power control mode
			isOk = QLIB_FTM_WLAN_GEN6_CLOSE_TPC_LOOP_V2(m_hQMSLPhone, 1);
			if (!isOk) {
				continue;
			}
			isOk = QLIB_FTM_WLAN_GEN6_TX_PKT_START_STOP(m_hQMSLPhone, 0);
			if (!isOk) {
				continue;
			}
			isOk = QLIB_FTM_WLAN_GEN6_SET_PWR_INDEX_SOURCE(m_hQMSLPhone, 3);
			if (!isOk) {
				continue;
			}
			//Set channel
			if (iChannel != 0)
			{
				isOk = QLIB_FTM_WLAN_GEN6_SET_CHANNEL_V2(m_hQMSLPhone, iChannel, iBond);
				if (!isOk) { 
					continue;
				}
			}
			//Set rate
			isOk = QLIB_FTM_WLAN_GEN6_SET_TX_FRAME_V2(m_hQMSLPhone, 0, iPayloadSize, 2, 165, iSpacing, true, iRate, iPreamble);
			if (!isOk) { 
				continue;
			}
			//Set closed loop power
			isOk = QLIB_FTM_WLAN_GEN6_SET_CLOSED_LOOP_POWER(m_hQMSLPhone, iPower);
			if (!isOk) { 
				continue;
			}
			/* marked for CDMA
			//Set power index source
			isOk = QLIB_FTM_WLAN_GEN6_SET_PWR_INDEX_SOURCE(m_hQMSLPhone, 2);
			if (!isOk) { 
			continue;
			}*/

			//TX start
			isOk = QLIB_FTM_WLAN_GEN6_TX_PKT_START_STOP(m_hQMSLPhone, 1);
			if (!isOk) { 
				continue;
			}

			return true;
		}

		return false;
	}

	return false;
}

bool CAndroidPhone::WifiPowerStopTx( int m_iChannel)
{
	//Stop TX
	bool isOk = false;
	char buf[32];

	isOk = QLIB_FTM_WLAN_TLV_Create(m_hQMSLPhone, _OP_TX);
	if (!isOk) {
		return isOk;
	}

	isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("txMode"), _itoa(0,buf,10)); //0 = off,  int txMode = 3; // Tx99 a = b = g = ac = n
	if (!isOk) {
			return isOk;
	}

	isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("channel"), _itoa( m_iChannel,buf,10));//iChannel
	if (!isOk) {
		return isOk;
	}

	isOk = QLIB_FTM_WLAN_TLV_Complete(m_hQMSLPhone);

	return isOk;

//	return QLIB_FTM_WLAN_GEN6_TX_PKT_START_STOP(m_hQMSLPhone, 0);
}

bool CAndroidPhone::WifiPowerStopTxCw()
{
	//Stop CW
	return QLIB_FTM_WLAN_GEN6_STOP_WAVEFORM(m_hQMSLPhone);
}

//bool CAndroidPhone::WifiPowerOnTxCwCertification(int iChannel, int iRfGain)
//virtual bool WifiPowerOnTxCwCertification (int iChannel, int iPower, int iRateBitIndex, int iWlandMode, int iChain) = 0 ;
bool CAndroidPhone::WifiPowerOnTxCwCertification (int iChannel, int iPower, int iRateBitIndex, int iWlandMode, int iChain)
{
	
	bool isOk = true;
	char sLibraryVersion[50];

	QLIB_GetLibraryVersion(sLibraryVersion );
	printf("QLibrary Demo\n\n");
	printf("QLIB DLL Version: %s\n\n", sLibraryVersion );

	TCHAR szNowPath[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szNowPath);
	CString path = szNowPath;
	CString csBinPath;
	//csBinPath.Format("%s\\u2416\\bdwlan30.bin", path);
	csBinPath.Format("%s\\u2416\\bdwlan30.bin", "C:\\");
	//C:\u2416

	// ROME Chip ID
	#define ROME_CHIPID   0x3e
	// Loopback IP for local system or any remote system IP with DUT
	char UDT_IP[20] = "127.0.0.1";
	// Chip specific DLL
	#define ROME_DLLID    "qc6174"


	//CString csBIN_FILE = BIN_FILE;
	if (_taccess(csBinPath, 0) != 0) 
	{
		CString csMsg = csBinPath + _T("not exist");
		AfxMessageBox(csMsg);
		return false;
	}

	QLIB_FTM_WLAN_Atheros_UNLoadDUT(m_hQMSLPhone); //unload first;

	unsigned char bRet = QLIB_FTM_WLAN_Atheros_LoadDUT(m_hQMSLPhone,(unsigned char  *)ROME_DLLID,(unsigned char *)csBinPath.GetBuffer(), DataFile, ROME_CHIPID);
	csBinPath.ReleaseBuffer();
	if ( !bRet ){
		return false;
	}


	if ( !bRet ){
		return false;
	}
		for(int i = 0;i < 1;i++)
		{
			Sleep(500);

			bool isOk = true;

			isOk = QLIB_FTM_WLAN_TLV_Create(m_hQMSLPhone, _OP_TX);
			if (!isOk) {
					continue;
			}

			char buf[32];
			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("txMode"), _itoa(1,buf,10)); // int txMode = 3; // Tx99 a = b = g = ac = n
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("channel"), _itoa( iChannel,buf,10));//iChannel
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("tpcm"), _itoa(0,buf,10));
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("txPower0"), _itoa(iPower, buf ,10)); //iPower
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("antenna"), _itoa(0,buf,10));
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("bandwidth"), _itoa(0,buf,10));
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("rateBitIndex0"), _itoa(iRateBitIndex,buf,10));
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("wlanMode"), _itoa(iWlandMode,buf,10));//b = 4, a/g = 0,  n = 1, ac = 8
			if (!isOk) {
					continue;
			}

			iChain += 1;
			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("txChain0"), _itoa(iChain,buf,10)); //chain0, chain 1
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("pktLen0"), _itoa( 1500, buf, 10));//packet size , iPayloadSize
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("shortGuard"), _itoa(0,buf,10));
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("numPackets"), _itoa(0,buf,10)); //o for count tx 
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("txPattern"), _itoa(0,buf,10));
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("scramblerOff"),_itoa(0,buf,10));
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("agg"),_itoa(1,buf,10));//aggregate 
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("aifsn"),_itoa(1,buf,10));
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("broadcast"), _itoa(0,buf,10));//broadcast, 
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("flags"), _itoa(24,buf,10));
			if (!isOk) {
					continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_Complete(m_hQMSLPhone);

		   // Start to run CLPC calibration iterations by power measurement call back
			//int numMeasAvg = 1;
		 //  asyncPMMessageCB pPMfunc = &asyncPMCB;
		 //  QLIB_FTM_WLAN_Atheros_Tx_CAL(m_hQMSLPhone, pPMfunc, (unsigned int)numMeasAvg);

		   
			if (!isOk) {
				Sleep(10000);
				continue;
			}
		}
		return isOk;
}

bool CAndroidPhone::WifiPowerOn5GTxCwCertification(int iChannel, int iRfGain, int iBond)
{
	for(int i = 0;i < 1;i++)
	{
		Sleep(500);

		bool isOk = true;

		//Set TX
		isOk = QLIB_FTM_WLAN_GEN6_ENABLE_CHAINS(m_hQMSLPhone, 6);
		if (!isOk) { 
			continue;
		}
		isOk = QLIB_FTM_WLAN_GEN6_TX_PKT_START_STOP(m_hQMSLPhone, 0);
		if (!isOk) {
			continue;
		}
		//Set channel
		if (iChannel != 0)
		{
			isOk = QLIB_FTM_WLAN_GEN6_SET_CHANNEL_V2(m_hQMSLPhone, iChannel, iBond);
			if (!isOk) { 
				continue;
			}
		}
		iRfGain *=65536;
		isOk = QLIB_FTM_WLAN_GEN6_SET_TX_WAVEFORM_GAIN_V2(m_hQMSLPhone, iRfGain);
		if (!isOk) {
			continue;
		}
		isOk = QLIB_FTM_WLAN_GEN6_GEN_CW(m_hQMSLPhone, 0, 255);
		if (!isOk) {
			continue;
		}

		return true;
	}

	return false;
}

bool CAndroidPhone::WifiPowerOnRxGetPacket (char* rxFrameCounter)
{
   //Get Rx status report
	bool isOk = true;

	isOk = QLIB_FTM_WLAN_TLV_Create(m_hQMSLPhone, _OP_RX_STATUS);
	if (!isOk) {
		return false;
	}


	isOk = QLIB_FTM_WLAN_TLV_Complete(m_hQMSLPhone);

   
	if (!isOk) {
	//	Sleep(10000);
	//	continue; 
		return false;
	}

	//int Ntest;
   printf ("==============================\n");
   QLIB_FTM_WLAN_TLV_GetRspParam(m_hQMSLPhone,"totalPkt",rxFrameCounter);
  // QLIB_FTM_WLAN_TLV_GetRspParam(m_hQMSLPhone,"totalPkt",&Ntest);
   printf("Rx Report : totalPkt    = %4s\n",rxFrameCounter);

   //QLIB_FTM_WLAN_TLV_GetRspParam(m_hQMSLPhone,"goodPackets",rxFrameCounter);
   //printf("Rx Report : goodPackets = %4s\n",rxFrameCounter);
   //QLIB_FTM_WLAN_TLV_GetRspParam(m_hQMSLPhone,"crcErrPkt",rxFrameCounter);
   //printf("Rx Report : crcErrPkt   = %4s\n",rxFrameCounter);
   //QLIB_FTM_WLAN_TLV_GetRspParam(m_hQMSLPhone,"secErrPkt",rxFrameCounter);
   //printf("Rx Report : secErrPkt   = %4s\n",rxFrameCounter);
   //QLIB_FTM_WLAN_TLV_GetRspParam(m_hQMSLPhone,"rssi",rxFrameCounter);
   //printf("Rx Report : rssi        = %4s\n",rxFrameCounter);




	return true;
}


//virtual bool WifiPowerOnRxCertification (int iChannel, int iWlandMode,int rateMask, int iChain);
bool CAndroidPhone::WifiPowerOnRxCertification(int iChannel, int iWlandMode,int iRateMask, int iChain)
{
	char sLibraryVersion[50];
	bool isOk = true;
	QLIB_GetLibraryVersion(sLibraryVersion );
	printf("QLibrary Demo\n\n");
	printf("QLIB DLL Version: %s\n\n", sLibraryVersion );

	TCHAR szNowPath[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szNowPath);
	CString path = szNowPath;
	CString csBinPath;
	csBinPath.Format("%s\\u2416\\bdwlan30.bin", "c:\\");

	// ROME Chip ID
	#define ROME_CHIPID   0x3e
	// Loopback IP for local system or any remote system IP with DUT
	char UDT_IP[20] = "127.0.0.1";
	// Chip specific DLL
	#define ROME_DLLID    "qc6174"


	//CString csBIN_FILE = BIN_FILE;
	if (_taccess(csBinPath, 0) != 0)  
	{
		CString csMsg = csBinPath + _T("not exist");
		AfxMessageBox(csMsg);
		return false;
	}

	QLIB_FTM_WLAN_Atheros_UNLoadDUT(m_hQMSLPhone); //unload first;

	unsigned char bRet = QLIB_FTM_WLAN_Atheros_LoadDUT(m_hQMSLPhone,(unsigned char  *)ROME_DLLID,(unsigned char *)csBinPath.GetBuffer(), DataFile, ROME_CHIPID);
	csBinPath.ReleaseBuffer();
	if ( !bRet ){
		return false;
	}


	if ( !bRet ){
		return false;
	}
		for(int i = 0;i < 1;i++)
		{
			Sleep(500);

			bool isOk = true;

			isOk = QLIB_FTM_WLAN_TLV_Create(m_hQMSLPhone, _OP_RX);
			if (!isOk) {
				continue;
			}

			char buf[32], key[10];
			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("channel"), _itoa(iChannel,buf,10)); // int txMode = 3; // Tx99 a = b = g = ac = n
			if (!isOk) {
				continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("wlanMode"), _itoa(iWlandMode,buf,10));//b = 4, a/g = 0,  n = 1, ac = 8
			if (!isOk) {
				continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("rxMode"), _itoa( 0, buf, 10));//packet size , iPayloadSize
			if (!isOk) {
				continue;
			}

			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("rateMask"), _itoa( iRateMask, buf, 10));//packet size , iPayloadSize
			if (!isOk) {
				continue;
			}

			iChain += 1;
			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("rxChain"), _itoa(iChain,buf,10)); //chain0, chain 1
			if (!isOk) {
				continue;
			}


			//isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("enANI"), _itoa(0,buf,10));//broadcast, 
			//if (!isOk) {
			//		continue;
			//}

			//isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("antenna"), _itoa(0,buf,10));
			//if (!isOk) {
			//		continue;
			//}

			isOk = QLIB_FTM_WLAN_TLV_Complete(m_hQMSLPhone);

		   
			if (!isOk) {
				Sleep(10000);
				continue;
			}
		}
		
		//AfxMessageBox("reciver ");

	return isOk;


		//void StopRxWithReport()
		//{
		   char strData[128];

		   //80-WL400-11 Refer Section "Steps to do WLAN RF receive tests"
		   //             Refer Table "QRCT steps to perform WLAN receive test"
		   // Execute _OP_RX_STATUS will stop current RX
		   isOk = QLIB_FTM_WLAN_TLV_Create(m_hQMSLPhone, _OP_RX_STATUS);
		   isOk = QLIB_FTM_WLAN_TLV_Complete(m_hQMSLPhone);

		   //Get Rx status report
		   printf ("==============================\n");
		   isOk = QLIB_FTM_WLAN_TLV_GetRspParam(m_hQMSLPhone,"totalPkt",strData);
		   printf("Rx Report : totalPkt    = %4s\n",strData);
		   isOk = QLIB_FTM_WLAN_TLV_GetRspParam(m_hQMSLPhone,"goodPackets",strData);
		   printf("Rx Report : goodPackets = %4s\n",strData);
		   QLIB_FTM_WLAN_TLV_GetRspParam(m_hQMSLPhone,"crcErrPkt",strData);
		   printf("Rx Report : crcErrPkt   = %4s\n",strData);
		   QLIB_FTM_WLAN_TLV_GetRspParam(m_hQMSLPhone,"secErrPkt",strData);
		   printf("Rx Report : secErrPkt   = %4s\n",strData);
		   QLIB_FTM_WLAN_TLV_GetRspParam(m_hQMSLPhone,"rssi",strData);
		   printf("Rx Report : rssi        = %4s\n",strData);

	return false;
}
//bool CAndroidPhone::WifiPowerOnRxGetPacket(unsigned long* rxFrameCounter,int iChain)


//bool CAndroidPhone::WifiPowerOnRxGetPacket(int iChannel,int iPreamble, unsigned long* rxFrameCounter,int iChain)
//{
//	for(int i = 0;i < 1;i++)
//	{
//		Sleep(500);
//
//		bool isOk = true;
//
//		unsigned long totalMacRxPackets = 0;
//		unsigned long totalMacFcsErrPackets = 0;
//		//isOk = QLIB_FTM_WLAN_GEN6_GET_RX_PACKET_COUNTS(m_hQMSLPhone, rxFrameCounter, &totalMacRxPackets, &totalMacFcsErrPackets);
//		//if (!isOk) { 
//		//	continue;
//		//}
//         // printf("\n(=Read ESN");
//
//			 unsigned long _iESN = 0;
//
//			 // Read the ESN, size, status
//			 //ESN = 0XDEADBEEF
//			 if (QLIB_DIAG_READ_ESN_F(  m_hQMSLPhone, &_iESN ) )
//			 {
//				printf("\nQLIB_DIAG_READ_ESN_F: PASS, ESN = 0x%X", _iESN );
//			 }
//			 else
//				printf("\nQLIB_DIAG_READ_ESN_F: FAIL");
//
//            // Get extended version info
//            unsigned long _iMSM_HW_Version = 0;
//            unsigned long _iMobModel = 0;
//            char _sMobSwRev[512];
//            char _sModelStr[512];
//
//			//Hw/Sw Version = 0  4097   
//            isOk = QLIB_DIAG_EXT_BUILD_ID_F( m_hQMSLPhone, &_iMSM_HW_Version, &_iMobModel, _sMobSwRev, _sModelStr );
//
//            // Print the results
//            if (isOk)
//            {
//               printf("\n");
//               printf("      Ext MSM HW Version: 0x%X\n", _iMSM_HW_Version  );
//               printf("       Ext Mobile Model#: 0x%X\n", _iMobModel        );
//               printf("Ext Mobile Mobile SW Rev: %s\n", _sMobSwRev      );
//               printf("            Ext Model ID: %s\n", _sModelStr      );
//
//               // Report the phone status
//              // ReportPhoneState();
//            }
//            else
//               printf("\nQLIB_DIAG_EXT_BUILD_ID_F = FAIL");
//
//			// ROME Chip ID
//			#define ROME_CHIPID   0x3e
//			// Loopback IP for local system or any remote system IP with DUT
//			char UDT_IP[20] = "127.0.0.1";
//			// Chip specific DLL
//			#define ROME_DLLID    "qc6174"
//			// Board data file where depends on chip variation
//			char BIN_FILE[200] =  _T("C:\\u2416\\bdwlan30.bin");
//
//			CString csBIN_FILE = BIN_FILE;
//			if (_taccess(csBIN_FILE, 0) != 0) 
//			{
//				AfxMessageBox(_T("C:\\u2416\\bdwlan30.bin not exist"));
//				return false;
//			}
//
//			unsigned char bRet = QLIB_FTM_WLAN_Atheros_LoadDUT(m_hQMSLPhone,(unsigned char  *)ROME_DLLID,(unsigned char *)BIN_FILE, DataFile, ROME_CHIPID);
//
//			if ( !bRet ){
//				return false;
//			}
//
//
//			isOk = QLIB_FTM_WLAN_TLV_Create(m_hQMSLPhone, _OP_RX);
//			if (!isOk) {
//					continue;
//			}
//
//			char buf[32], key[10];
//			//isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("txMode"), _itoa(3,buf,10)); // int txMode = 3; // Tx99 a = b = g = ac = n
//			//if (!isOk) {
//			//		continue;
//			//}
//
//			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("channel"), _itoa( iChannel,buf,10));//iChannel
//			if (!isOk) {
//					continue;
//			}
//
//			//isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("wlanMode"), _itoa(iPreamble,buf,10));//b = 4, a/g = 0,  n = 1, ac = 8
//			//if (!isOk) {
//			//		continue;
//			//}
//			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("wlanMode"), _itoa(0, buf,10));//b = 4, a/g = 0,  n = 1, ac = 8
//			if (!isOk) {
//					continue;
//			}
//			//isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("wlanMode"), _itoa(4, buf,10));//b = 4, a/g = 0,  n = 1, ac = 8
//			//if (!isOk) {
//			//		continue;
//			//}
//			//isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("wlanMode"), _itoa(1, buf,10));//b = 4, a/g = 0,  n = 1, ac = 8
//			//if (!isOk) {
//			//		continue;
//			//}
//			//isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("wlanMode"), _itoa(8, buf,10));//b = 4, a/g = 0,  n = 1, ac = 8
//			//if (!isOk) {
//			//		continue;
//			//}
//
//			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("rxMode"), _itoa(0,buf,10));//rxMode = 0
//			if (!isOk) {
//					continue;
//			}
//
//			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("rateMask"), _itoa(59,buf,10));//rxMode = 0
//			if (!isOk) {
//					continue;
//			}
//
//			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("rxChain"), _itoa(iChain, buf,10)); //chain0, chain 1
//			if (!isOk) {
//					continue;
//			}
//
//			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("enANI"), _itoa(0, buf,10)); //chain0, chain 1
//			if (!isOk) {
//					continue;
//			}
//
//			isOk = QLIB_FTM_WLAN_TLV_AddParam(m_hQMSLPhone, _T("antenna"), _itoa(0, buf,10)); //chain0, chain 1
//			if (!isOk) {
//					continue;
//			}
//
//			isOk = QLIB_FTM_WLAN_TLV_Complete(m_hQMSLPhone);
//			if (!isOk) {
//				continue;
//			}
//
//
//		//void StopRxWithReport()
//		//{
//		   char strData[128];
//
//		   //80-WL400-11 Refer Section "Steps to do WLAN RF receive tests"
//		   //             Refer Table "QRCT steps to perform WLAN receive test"
//		   // Execute _OP_RX_STATUS will stop current RX
//		   QLIB_FTM_WLAN_TLV_Create(m_hQMSLPhone, _OP_RX_STATUS);
//		   QLIB_FTM_WLAN_TLV_Complete(m_hQMSLPhone);
//
//		   //Get Rx status report
//		   printf ("==============================\n");
//		   QLIB_FTM_WLAN_TLV_GetRspParam(m_hQMSLPhone,"totalPkt",strData);
//		   printf("Rx Report : totalPkt    = %4s\n",strData);
//		   QLIB_FTM_WLAN_TLV_GetRspParam(m_hQMSLPhone,"goodPackets",strData);
//		   printf("Rx Report : goodPackets = %4s\n",strData);
//		   QLIB_FTM_WLAN_TLV_GetRspParam(m_hQMSLPhone,"crcErrPkt",strData);
//		   printf("Rx Report : crcErrPkt   = %4s\n",strData);
//		   QLIB_FTM_WLAN_TLV_GetRspParam(m_hQMSLPhone,"secErrPkt",strData);
//		   printf("Rx Report : secErrPkt   = %4s\n",strData);
//		   QLIB_FTM_WLAN_TLV_GetRspParam(m_hQMSLPhone,"rssi",strData);
//		   printf("Rx Report : rssi        = %4s\n",strData);
//
//		return true;
//	}
//
//	return false;
//}

bool CAndroidPhone::WifiPowerOnRxSetChannel(int iChannel)
{
	for(int i = 0;i < 1;i++)
	{
		Sleep(500);

		bool isOk = true;

		//Set RX
		isOk = QLIB_FTM_WLAN_GEN6_ENABLE_CHAINS(m_hQMSLPhone, 4);
		if (!isOk) { 
			continue;
		}
		//Set channel
		if (iChannel != 0)
		{
			isOk = QLIB_FTM_WLAN_GEN6_SET_CHANNEL(m_hQMSLPhone, iChannel);
			if (!isOk) { 
				continue;
			}
		}

		return true;
	}

	return false;
}

bool CAndroidPhone::WifiPowerOn5GRxGetPacket(unsigned long* rxFrameCounter)
{
	for(int i = 0;i < 1;i++)
	{
		Sleep(500);

		bool isOk = true;

		unsigned long totalMacRxPackets = 0;
		unsigned long totalMacFcsErrPackets = 0;
		isOk = QLIB_FTM_WLAN_GEN6_GET_RX_PACKET_COUNTS(m_hQMSLPhone, rxFrameCounter, &totalMacRxPackets, &totalMacFcsErrPackets);
		if (!isOk) { 
			continue;
		}
		return true;
	}

	return false;
}

bool CAndroidPhone::WifiPowerOn5GRxSetChannel(int iChannel, int iBond)
{
	for(int i = 0;i < 1;i++)
	{
		Sleep(500);

		bool isOk = true;

		//Set RX
		isOk = QLIB_FTM_WLAN_GEN6_ENABLE_CHAINS(m_hQMSLPhone, 4);
		if (!isOk) { 
			continue;
		}
		//Set channel
		if (iChannel != 0)
		{
			isOk = QLIB_FTM_WLAN_GEN6_SET_CHANNEL_V2(m_hQMSLPhone, iChannel, iBond);
			if (!isOk) { 
				continue;
			}
		}

		return true;
	}

	return false;
}

bool CAndroidPhone::WifiPowerOnRxCertificationReset()
{
	for(int i = 0;i < 1;i++)
	{
		Sleep(500);

		bool isOk = true;

		isOk = QLIB_FTM_WLAN_GEN6_RESET_RX_PACKET_STATISTICS(m_hQMSLPhone);
		if (!isOk) { 
			continue;
		}
		return true;
	}

	return false;
}

bool CAndroidPhone::WifiPowerOnTx (int iRate, int iChannel, int iPower,int iRegulatorFixed)
{
//#ifdef _NOT_USE_FTD_

	if (0 == iRate && 0 == iChannel && 0 == iPower) // Turn off Tx Power
	{
		bool ret = true;

		//Stop TX
		return ret && QLIB_FTM_WLAN_GEN6_TX_PKT_START_STOP(m_hQMSLPhone, 0);
	}

	else
	{
		unsigned int cmdRateMap = 0;

		switch(iRate)
		{
		case 11:
			cmdRateMap = 3;
			break;

		case 54:
			cmdRateMap = 14;
			break;

		case 72:
			cmdRateMap = 38;
			break;

		default:
			return false;
		}

		unsigned int cmdPreamble = 0;

		switch(cmdRateMap)
		{
		/*	0: HAL_PHY_RATE_11B_LONG_1_MBPS
			1: HAL_PHY_RATE_11B_LONG_2_MBPS
			2: HAL_PHY_RATE_11B_LONG_5_5_MBPS
			3: HAL_PHY_RATE_11B_LONG_11_MBPS	*/
		case 0: case 1: case 2: case 3:
			cmdPreamble = 4;
			break;

		/*	4: HAL_PHY_RATE_11B_SHORT_2_MBPS
			5: HAL_PHY_RATE_11B_SHORT_5_5_MBPS
			6: HAL_PHY_RATE_11B_SHORT_11_MBPS	*/
		case 4: case 5: case 6:
			cmdPreamble = 3;
			break;

		/*	7:  HAL_PHY_RATE_11A_6_MBPS
			8:  HAL_PHY_RATE_11A_9_MBPS
			9:  HAL_PHY_RATE_11A_12_MBPS
			10: HAL_PHY_RATE_11A_18_MBPS
			11: HAL_PHY_RATE_11A_24_MBPS
			12: HAL_PHY_RATE_11A_36_MBPS
			13: HAL_PHY_RATE_11A_48_MBPS
			14: HAL_PHY_RATE_11A_54_MBPS	*/
		case 7: case 8: case 9: case 10: case 11: case 12: case 13: case 14:
			cmdPreamble = 0;
			break;

		/*	38: HAL_PHY_RATE_MCS_1NSS_MM_SG_72_2_MBPS	*/
		case 38:
			cmdPreamble = 2;
			break;

		default:
			return false;
		}

		for(int i = 0;i < 1;i++)
		{
			Sleep(500);
			/*
			if(iChannel >= 36)
			{
				if(!QLIB_FTM_WLAN_GEN6_SET_CHANNEL(m_hQMSLPhone, 40) ||
					!QLIB_FTM_WLAN_GEN6_ENABLE_CHAINS(m_hQMSLPhone, 6) ||
					!QLIB_FTM_WLAN_GEN6_SET_TX_FRAME (m_hQMSLPhone, 0, 1000, 200, true, iRate, cmdPreamble) ||
					!QLIB_FTM_WLAN_GEN6_CLOSE_TPC_LOOP(m_hQMSLPhone, 0) ||
					!QLIB_FTM_WLAN_GEN6_SET_TX_WAVEFORM_GAIN_V2(m_hQMSLPhone, iPower) ||
					!QLIB_FTM_WLAN_GEN6_TX_PKT_START_STOP(m_hQMSLPhone, 1))
					continue;
			}

			if(!QLIB_FTM_WLAN_GEN6_SET_CHANNEL(m_hQMSLPhone, iChannel) ||
				!QLIB_FTM_WLAN_GEN6_ENABLE_CHAINS(m_hQMSLPhone, 6) ||
				!QLIB_FTM_WLAN_GEN6_SET_TX_FRAME (m_hQMSLPhone, 0, 1000, 200, true, iRate, cmdPreamble) ||
				!QLIB_FTM_WLAN_GEN6_CLOSE_TPC_LOOP(m_hQMSLPhone, 0) ||
				!QLIB_FTM_WLAN_GEN6_SET_TX_WAVEFORM_GAIN_V2(m_hQMSLPhone, iPower) ||
				!QLIB_FTM_WLAN_GEN6_TX_PKT_START_STOP(m_hQMSLPhone, 1))
				continue;
			*/
			bool isOk = true;
			//Set FTM start
			//isOk = QLIB_FTM_WLAN_GEN6_START(m_hQMSLPhone, 3660);
			//if (!isOk) { 
			//	continue;
			//}
			//Set TX
			isOk = QLIB_FTM_WLAN_GEN6_ENABLE_CHAINS(m_hQMSLPhone, 6);
			if (!isOk) { 
				continue;
			}
			//Set close loop
			isOk = QLIB_FTM_WLAN_GEN6_CLOSE_TPC_LOOP(m_hQMSLPhone, 1);
			if (!isOk) {
				continue;
			}
			//Set channel
			isOk = QLIB_FTM_WLAN_GEN6_SET_CHANNEL(m_hQMSLPhone, iChannel);
			if (!isOk) { 
				continue;
			}
			//Set rate
			isOk = QLIB_FTM_WLAN_GEN6_SET_TX_FRAME(m_hQMSLPhone, 0, 2400, 10, true, cmdRateMap, cmdPreamble);
			if (!isOk) { 
				continue;
			}
/*			//Set Packet Gain
			isOk = QLIB_FTM_WLAN_GEN6_SET_TX_WAVEFORM_GAIN_V2(m_hQMSLPhone, iPower);
			if (!isOk) { 
				continue;
			}*/
			//Set closed loop power
			isOk = QLIB_FTM_WLAN_GEN6_SET_CLOSED_LOOP_POWER(m_hQMSLPhone, iPower);
			if (!isOk) { 
				continue;
			}
			/* marked for CDMA
			//Set power index source
			isOk = QLIB_FTM_WLAN_GEN6_SET_PWR_INDEX_SOURCE(m_hQMSLPhone, 2);
			if (!isOk) { 
				continue;
			}*/
			
			//TX start
			isOk = QLIB_FTM_WLAN_GEN6_TX_PKT_START_STOP(m_hQMSLPhone, 1);
			if (!isOk) { 
				continue;
			}

			/*3=regulatory=1=enable , 2=fixed=0=disable ,Liontest, xml*/
			isOk = QLIB_FTM_WLAN_GEN6_SET_PWR_INDEX_SOURCE(m_hQMSLPhone, iRegulatorFixed);
			if (!isOk) {
				continue;
			}

			/*lionadded*/
			//isOk = QLIB_FTM_WLAN_GEN6_SET_TX_FRAME_V2(m_hQMSLPhone, 0, 2400, 2, 165, 10, true, cmdRateMap, cmdPreamble);
			//if (!isOk) { 
			//	continue;
			//}
			return true;
		}

		return false;
	}
/*
#else // use FTD

	int cmdRate=0;

	switch (iRate)
	{
	case 0: break;
	case 1:  cmdRate = 0; break;
	case 2:  cmdRate = 1; break;
		//case 5.5: cmdRate = 2; break;
	case 6:  cmdRate = 4; break;
	case 7:  cmdRate = 19; break; //HT20 MCS7
	case 9:  cmdRate = 5; break;
	case 11: cmdRate = 3; break;
	case 12: cmdRate = 6; break;
	case 18: cmdRate = 7; break;
	case 24: cmdRate = 8; break;
	case 36: cmdRate = 9; break;
	case 48: cmdRate = 10; break;
	case 54: cmdRate = 11; break;
	default:
		return false;
	}

	pC7_WLAN_TXPower C7_WLAN_TXPower = (pC7_WLAN_TXPower) GetProcAddress (m_hNeptuneCtrl, "C7_WLAN_TXPower");
	if (! C7_WLAN_TXPower)
		return false;

	char szError[FTD_BUFSIZE], szIn[FTD_BUFSIZE];

	if (0==iRate && 0==iChannel && 0==iPower) // turn off
		strcpy (szIn, "0");
	else
		sprintf (szIn, "%d,%d,%d", iChannel, iPower, cmdRate);

	int ret = C7_WLAN_TXPower (FTD_PORT, 5000, szIn, szError);
	return FTD_OK == ret;

#endif
*/
	return false;
}

bool CAndroidPhone::WifiPowerOnCw (int iChannel, int iGain, long iTone, unsigned long iAmpl)
{
	bool isOk = true;

	if (0 == iGain && 0 == iTone && 0 == iAmpl) // Turn off Cw Power
	{
		isOk = QLIB_FTM_WLAN_GEN6_STOP_RF_WAVEFORM(m_hQMSLPhone);
		if (isOk)
			return isOk && QLIB_FTM_WLAN_GEN6_STOP_WAVEFORM(m_hQMSLPhone);
		else
			return false;
	}

	else	// Start Cw Power
	{
		isOk = QLIB_FTM_WLAN_GEN6_ENABLE_CHAINS(m_hQMSLPhone, 6);
		if (isOk)
			isOk = QLIB_FTM_WLAN_GEN6_SET_CHANNEL(m_hQMSLPhone, iChannel);
		if (isOk)
			isOk = QLIB_FTM_WLAN_GEN6_TX_PKT_START_STOP(m_hQMSLPhone, 0);
		if (isOk)
			isOk = QLIB_FTM_WLAN_GEN6_SET_TX_WAVEFORM_GAIN_V2(m_hQMSLPhone, iGain);
		if (isOk)
			isOk = QLIB_FTM_WLAN_GEN6_GEN_RF_CW(m_hQMSLPhone, iTone, iAmpl);
		if (isOk)
			isOk = QLIB_FTM_WLAN_GEN6_START_RF_WAVEFORM(m_hQMSLPhone);

		if (!isOk)
			return false;
	}

	return true;
}

// ----------------------- Detroit GPS -----------------------

bool CAndroidPhone::GPS_EnterGPS(bool bEnter)
{
	unsigned char status = 0;
	
	CGPS_Gen8_Rx_RfMode_Struct_type mode;
	mode.u_RfMode = 0;

	return QLIB_DIAG_GPS_SA_RF_VERIF_MODE_SWITCH_WithWait(this->m_hQMSLPhone,
														  bEnter ? GPS_SA_RF_VERIF_MODE_SWITCH_ENTER_MODE : GPS_SA_RF_VERIF_MODE_SWITCH_EXIT_MODE,
														  &status) && 
		   QLIB_GPS_GEN8_PRx_Mode_Config(this->m_hQMSLPhone, (void *)&mode);
}

bool CAndroidPhone::GPS_GetCN0value(double& dCN0, int& nFreq, unsigned short iTestMode, unsigned short iCaptureSize_kSamples, unsigned char iNumberOfIntegration)
{
//#ifdef _NOT_USE_FTD_
	return QLIB_DIAG_GEN8_GET_EMBEDDED_CALCULATED_CTON(this->m_hQMSLPhone,
													   iTestMode,
													   iCaptureSize_kSamples,
													   iNumberOfIntegration,
													   &dCN0,
													   &nFreq);
/*#else
	pC7_GPSValueCN0 FTD_GPSGetCN0 = (pC7_GPSValueCN0) GetProcAddress (m_hNeptuneCtrl, "C7_GPSValueCN0");
	if (! FTD_GPSGetCN0)
		return false;

	char szOut[FTD_BUFSIZE] = {0};
	char szIn[FTD_BUFSIZE] ={0};
	int ret = FTD_GPSGetCN0 (FTD_PORT, FTD_TIMEOUT, szIn, szOut);
	iCN0 = atoi (szOut);
	return FTD_OK == ret;
#endif*/
}


bool CAndroidPhone::GPS_GetCN0value(int& iCN0)
{
	pDetroit_GPSValueCN0 FTD_GPSGetCN0 = (pDetroit_GPSValueCN0) GetProcAddress (m_hNeptuneCtrl, "Detroit_GPSValueCN0");
	if (! FTD_GPSGetCN0)
		return false;

	char szOut[FTD_BUFSIZE] = {0};
	char szIn[FTD_BUFSIZE] ={0};
	int ret = FTD_GPSGetCN0 (FTD_PORT, FTD_TIMEOUT, szIn, szOut);
	iCN0 = atoi (szOut);
	return FTD_OK == ret;
}


// ----------------- FMR phone functions ------------------
bool CAndroidPhone::FMR_Rssi(char* sz_inData, char* sz_outData)
{
	int nRetCode;

	/* Get DLL function */
	pDetroit_FMRrssi FTD_Detroit_FMRrssi = (pDetroit_FMRrssi)::GetProcAddress(m_hNeptuneCtrl, "Detroit_FMRrssi");
	if (!FTD_Detroit_FMRrssi)
	{
		return false;
	}

	if(FTD_Detroit_FMRrssi)
	{
		nRetCode = FTD_Detroit_FMRrssi( FTD_PORT, FTD_TIMEOUT, sz_inData, sz_outData );

		if(nRetCode == 1) {//HS_OK
			return true;
		}else {
			return false;
		}
	}
		
	return false;
}

// ----------------------- Detroit WCDMA 2100_B1 Rssi -----------------------

bool CAndroidPhone::WCDMA_Rssi (unsigned short iChannel, unsigned short* iRX_AGC, unsigned short* iRX_Power_dBm, unsigned short* iFTM_Error_Code)
{
	bool isOk = true;

	isOk = QLIB_FTM_SET_MODE(m_hQMSLPhone, PHONE_MODE_WCDMA_IMT);

	if (isOk)
		isOk = QLIB_FTM_SET_CHAN(m_hQMSLPhone, iChannel);

	if (isOk)
		isOk = QLIB_FTM_GET_RX_LEVEL_DBM(m_hQMSLPhone, iRX_AGC, iRX_Power_dBm, iFTM_Error_Code);

	if (!isOk)
		return false;

	return true;
}

// ----------------- FMR phone functions ------------------
bool CAndroidPhone::FSG_Enable(char* sz_inData, char* sz_outData)
{
	int nRetCode;

	/* Get DLL function */
	pSapporo_FSG_Enable FTD_FSG_Enable = (pSapporo_FSG_Enable)::GetProcAddress(m_hNeptuneCtrl, "Detroit_FMRrssi");
	if (!FTD_FSG_Enable)
	{
		return false;
	}

	if(FTD_FSG_Enable)
	{
		nRetCode = FTD_FSG_Enable( FTD_PORT, FTD_TIMEOUT, sz_inData, sz_outData );

		if(nRetCode == 1) {//HS_OK
			return true;
		}else {
			return false;
		}
	}
		
	return false;
}

//////////////////////////////////////////////////////////////////////////
// ----------------- QMSL phone functions ------------------
//////////////////////////////////////////////////////////////////////////

bool CAndroidPhone::Initial_QMSL() 
{
	if (m_hQMSLPhone)
	{
		if (QLIB_IsPhoneConnected(m_hQMSLPhone))
		{
			QLIB_FlushRxBuffer(m_hQMSLPhone);
			return true;
		}
	}
	else
	{
		QLIB_DisconnectAllServers();
		unsigned short iTargetType = QLIB_TARGET_TYPE_APQ;

		//QLIB_SetTargetType(APQ)
		//QLIB_SetLibraryMode(QPST)

		QLIB_SetLibraryMode(false);
		//QLIB_SetLibraryMode(true);
		  //Set TargetType

		QLIB_SetTargetType((unsigned char)iTargetType);

		//AfxMessageBox(m_strCOMport.c_str());
		
		//m_hQMSLPhone = QLIB_ConnectServer( (unsigned)atoi(m_strCOMport.c_str()));
		m_hQMSLPhone = QLIB_ConnectServerWithWait( (unsigned)atoi(m_strCOMport.c_str()) ,5000);

		if (m_hQMSLPhone != NULL)
		{
			if (QLIB_IsPhoneConnected(m_hQMSLPhone))
			{
				return true;
			}
		}
	}

	return false;
}

bool CAndroidPhone::IsConnected_QMSL()
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_IsPhoneConnected(m_hQMSLPhone))
			return true;
	}

	return Initial_QMSL();
}

bool CAndroidPhone::Disconnect_QMSL()
{
	if (m_hQMSLPhone != NULL)
	{
		QLIB_DisconnectServer(m_hQMSLPhone);
		m_hQMSLPhone = NULL;
	}

	return true;
}

bool CAndroidPhone::QMSL_IsFTM_Mode(unsigned char* pMode)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_IsFTM_Mode(m_hQMSLPhone, pMode))
			return true;
	}

	return false;
}

bool CAndroidPhone::ReadNV_QMSL(unsigned short unIndex, unsigned char* pData, int iSize)
{
	if (m_hQMSLPhone != NULL)
	{
		unsigned short unStatus = 0;
		if (QLIB_DIAG_NV_READ_F(m_hQMSLPhone, unIndex, pData, iSize, &unStatus))
			return true;
	}

	return false;
}

bool CAndroidPhone::QMSL_FTM_SET_MODE_ID(short nMode)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_MODE_ID(m_hQMSLPhone, nMode))
			return true;
	}

	return false;
}

bool CAndroidPhone::QMSL_FTM_SET_PATH_DELAY(short *piDelay)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_PATH_DELAY(m_hQMSLPhone, piDelay))
			return true;
	}

	return false;
}

bool CAndroidPhone::QMSL_FTM_GSM_POLAR_TX_SWEEP_CAL(unsigned long iNumDAC, unsigned long* piDACList, unsigned long* piDACDuration)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_GSM_POLAR_TX_SWEEP_CAL(m_hQMSLPhone, iNumDAC, piDACList, piDACDuration))
			return true;
	}

	return false;
}


bool CAndroidPhone::QMSL_FTM_SET_MODE(short nMode)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_MODE(m_hQMSLPhone, nMode))
			return true;
	}

	return false;
}

bool CAndroidPhone::QMSL_FTM_SET_CHAN(unsigned short unChannel)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_CHAN(m_hQMSLPhone, unChannel))
			return true;
	}

	return false;
}

bool CAndroidPhone::QMSL_FTM_SET_GSM_LINEAR_PA_RANGE(unsigned short unSlot, unsigned short unPARange)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_GSM_LINEAR_PA_RANGE(m_hQMSLPhone, unSlot, unPARange))
			return true;
	}

	return false;
}

bool CAndroidPhone::QMSL_DIAG_CONTROL_F(int iMode)
{
	if(m_hQMSLPhone != NULL)
	{
		if (QLIB_DIAG_CONTROL_F(m_hQMSLPhone,iMode))
			return true;			
	}

	return false;
}

bool CAndroidPhone::QMSL_FTM_GSM_SET_LINEAR_RGI(unsigned short unSlot, unsigned short unRGI, unsigned char ucModType)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_GSM_SET_LINEAR_RGI(m_hQMSLPhone, unSlot, unRGI, ucModType))
			return true;
	}

	return false;
}

bool CAndroidPhone::QMSL_FTM_SET_TRANSMIT_BURST(
	unsigned char ucSlot,
	short nDataSource,
	unsigned char ucTSCIndex,
	unsigned long ulNumBursts,
	unsigned char ucIsInfiniteDuration
	)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_TRANSMIT_BURST(m_hQMSLPhone, ucSlot, nDataSource, ucTSCIndex, ulNumBursts, ucIsInfiniteDuration))
			return true;
	}

	return false;
}

bool CAndroidPhone::QMSL_FTM_SET_TX_FRAME_MATRIX(
	unsigned char iSlotNum, 
	unsigned char iTurnSlotOn, 
	unsigned short iPwrLevel, 
	unsigned char iModScheme
	)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_TX_FRAME_MATRIX(m_hQMSLPhone, iSlotNum, iTurnSlotOn, iPwrLevel, iModScheme))
			return true;
	}

	return false;
}

bool CAndroidPhone::QMSL_FTM_SET_TX_ON()
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_TX_ON(m_hQMSLPhone))
			return true;
	}

	return false;
}

bool CAndroidPhone::QMSL_FTM_SET_TX_OFF()
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_TX_OFF(m_hQMSLPhone))
			return true;
	}

	return false;
}


bool CAndroidPhone::QMSL_FTM_SET_PA_RANGE(unsigned short unRange)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_PA_RANGE(m_hQMSLPhone, unRange))
			return true;
	}

	return false;
}

bool CAndroidPhone::QMSL_FTM_SET_PDM(unsigned short unPDM)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_PDM(m_hQMSLPhone, 2, unPDM))
			return true;
	}

	return false;
}

bool CAndroidPhone::QMSL_FTM_SET_PDM_V2(unsigned short iPDMtype, unsigned short unPDM)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_PDM(m_hQMSLPhone, iPDMtype, unPDM))
			return true;
	}

	return false;
}

bool CAndroidPhone::QMSL_FTM_SET_PA_DAC_INPUT(unsigned short iDACinput)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_SET_PA_DAC_INPUT(m_hQMSLPhone, iDACinput))
			return true;
	}

	return false;
}

bool CAndroidPhone::QMSL_FTM_TX_CS_SWEEP(unsigned char iListSize, short* aiI_DC_Correction, short* aiQ_DC_Correction, 
									 unsigned short* aiSegmentLength, unsigned short* piStatus)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_TX_CS_SWEEP(m_hQMSLPhone, iListSize, aiI_DC_Correction, aiQ_DC_Correction, aiSegmentLength, piStatus))
			return true;
	}

	return false;
}


bool CAndroidPhone::QMSL_ConfigureTimeOut(unsigned long unTimoutID, unsigned long unTimeout)
{
    if ( NULL!=m_hQMSLPhone )
    {
        if ( QLIB_ConfigureTimeOut(m_hQMSLPhone, unTimoutID, unTimeout) )
            return true ;
    }
    return false ;
}

bool CAndroidPhone::QMSL_FTM_SET_COMMAND_CODE(short nCmdCode)
{
    if ( 59!=nCmdCode && 75!=nCmdCode )
        return false ;

    if ( NULL!=m_hQMSLPhone )
    {
        if ( QLIB_FTM_SET_COMMAND_CODE(m_hQMSLPhone, nCmdCode) )
            return true ;
    }
    return false ;
}

bool CAndroidPhone::QMSL_FTM_SET_SECONDARY_CHAIN(unsigned short unMode)
{
    if ( NULL!=m_hQMSLPhone )
    {
        if ( QLIB_FTM_SET_SECONDARY_CHAIN(m_hQMSLPhone, unMode) )
            return true ;
    }
    return false ;
}

bool CAndroidPhone::QMSL_FTM_SET_LNA_RANGE(unsigned char iLNArange)
{
    if ( NULL!=m_hQMSLPhone )
    {
        if ( QLIB_FTM_SET_LNA_RANGE(m_hQMSLPhone, iLNArange) )
            return true ;
    }
    return false ;
}

bool CAndroidPhone::QMSL_FTM_DO_ENH_XO_DC_CAL(void* pFTM_ENH_XO_Cal_Request, void* pFTM_XO_Cal_Response)
{
    if ( NULL!=m_hQMSLPhone )
    {
        if ( QLIB_FTM_DO_ENH_XO_DC_CAL(m_hQMSLPhone, pFTM_ENH_XO_Cal_Request, pFTM_XO_Cal_Response))
            return true ;
    }
    return false ;
}

bool CAndroidPhone::QMSL_FTM_DO_ENH_XO_FT_COLLECT_SAMPLE(void* pFTM_ENH_XO_Cal_Request, void* pFTM_XO_Cal_Response)
{
    if ( NULL!=m_hQMSLPhone )
    {
        if ( QLIB_FTM_DO_ENH_XO_FT_COLLECT_SAMPLE(m_hQMSLPhone, pFTM_ENH_XO_Cal_Request, pFTM_XO_Cal_Response) )
            return true ;
    }
    return false ;
}

bool CAndroidPhone::QMSL_FTM_CDMA_CW_WAVEFORM(unsigned char bSelectCW)
{
    if ( NULL!=m_hQMSLPhone )
    {
        if ( QLIB_FTM_CDMA_CW_WAVEFORM(m_hQMSLPhone, bSelectCW) )
            return true ;
    }
    return false ;
}

bool CAndroidPhone::QMSL_FTM_DO_ENH_XO_FT_CURVE_CAL(void* pFTM_ENH_XO_Cal_Request, void* pFTM_XO_Cal_Response)
{
    if ( NULL!=m_hQMSLPhone )
    {
        if ( QLIB_FTM_DO_ENH_XO_FT_CURVE_CAL(m_hQMSLPhone, pFTM_ENH_XO_Cal_Request, pFTM_XO_Cal_Response) )
            return true ;
    }
    return false ;
}

bool CAndroidPhone::QMSL_FTM_DA_CAL(unsigned short iListSize, 
					   unsigned short *aiSegLength, 
					   unsigned char *aiRgi,	
					   unsigned char *aiGsmMode, 
					   unsigned char *aiPaRange)
{
	if (m_hQMSLPhone != NULL)
	{
		if (QLIB_FTM_DA_CAL(m_hQMSLPhone, iListSize, aiSegLength, aiRgi, aiGsmMode, aiPaRange) )
			return true;
	}

	return false;

}


bool CAndroidPhone::QMSL_RFCAL_GSM_Tx_DA_Cal_Results(unsigned short iNvModeId,
										void *aGSM_DA_Cal,
										void *aGSM_DA_Cal_NV,
										unsigned char bCachedByNVManager)
{
	if (m_hQMSLPhone != NULL)
	{
		if( QLIB_RFCAL_GSM_Tx_DA_Cal_Results(m_hQMSLPhone, iNvModeId, aGSM_DA_Cal, aGSM_DA_Cal_NV, bCachedByNVManager) )
			return true;
	}

	return false;

}

bool CAndroidPhone::QMSL_FTM_TX_CFG2_AMAM_SWEEP(unsigned long iPreDcDuration,
								   unsigned long iEdgeDuration,
								   unsigned long iCalRgi,
								   unsigned long *piDigScale)
{
	if (m_hQMSLPhone != NULL)
	{
		if( QLIB_FTM_TX_CFG2_AMAM_SWEEP(m_hQMSLPhone, iPreDcDuration, iEdgeDuration, iCalRgi, piDigScale) ){
		//if( QLIB_FTM_TX_CFG2_AMAM_SWEEP_V2(m_hResourceContext, iPreDcDuration, iEdgeDuration, iCalRgi, 0, piDigScale)){
			return true;
		}
	}

	return false;
}

bool CAndroidPhone::QMSL_RFCAL_GSM_Tx_Linear_Process_PreDist_Cal_Results(unsigned short iNvModeId, 
															void *aGSM_PreDist_Cal,
															void *aGSM_PreDist_Cal_NV,
															unsigned char bCachedByNVManager)
{
	if (m_hQMSLPhone != NULL)
	{
		if( QLIB_RFCAL_GSM_Tx_Linear_Process_PreDist_Cal_Results(m_hQMSLPhone, iNvModeId, aGSM_PreDist_Cal, aGSM_PreDist_Cal_NV, bCachedByNVManager) ){
			return true;
		}
	}

	return false;

}

bool CAndroidPhone::QMSL_RFCAL_NV_Manager_WriteToPhone(void *pWriteToPhoneStatus)
{
	if (m_hQMSLPhone != NULL)
	{
		if( QLIB_RFCAL_NV_Manager_WriteToPhone(m_hQMSLPhone, pWriteToPhoneStatus) ){
			return true;
		}
	}

	return false;

}

bool CAndroidPhone::QMSL_FTM_GSM_DO_TX_Kvco(unsigned short iLowChan,unsigned short iHiChan,
							   unsigned short *piLowChanKvco,unsigned short *piHiChanKvco)
{
	if (m_hQMSLPhone != NULL)
	{
		if( QLIB_FTM_GSM_DO_TX_KV_CAL(m_hQMSLPhone, iLowChan, iHiChan, piLowChanKvco, piHiChanKvco) ){
			return true;
		}
	}

	return false;
}

bool CAndroidPhone::QMSL_FTM_SET_RX_BURST(unsigned char iSlotNum, short iRxFunction)
{
	if (m_hQMSLPhone != NULL)
	{
		if( QLIB_FTM_SET_RX_BURST(m_hQMSLPhone,  iSlotNum, iRxFunction) ){
			return true;
		}
	}

	return false;
}

bool CAndroidPhone::QMSL_FTM_RX_GAIN_RANGE_CAL(unsigned char iRxRangeBitMask, unsigned char iNumAverages, unsigned char bToggleRx, 
										   unsigned long aiRSSI_Result[8])
{

	if (m_hQMSLPhone != NULL)
	{
		if( QLIB_FTM_RX_GAIN_RANGE_CAL(m_hQMSLPhone, iRxRangeBitMask, iNumAverages, bToggleRx, aiRSSI_Result) ){
			return true;
		}
	}

	return false;

} 

bool CAndroidPhone::QMSL_RFCAL_GSM_Rx_Gain_Range_Results(unsigned short iNvModeId, void* aGSM_Rx_Gain_Range_Result, void* aGSM_Rx_Gain_Range_NV, 
													 unsigned char bCachedByNVManager)
{
	if (m_hQMSLPhone != NULL)
	{
		if( QLIB_RFCAL_GSM_Rx_Gain_Range_Results(m_hQMSLPhone, iNvModeId, 
																aGSM_Rx_Gain_Range_Result, 
																aGSM_Rx_Gain_Range_NV, 
																bCachedByNVManager)){
			return true;
		}
	}

	return false;

}

bool CAndroidPhone::QMSL_RFCAL_GSM_CAL_ARFCN(unsigned short iNvModeId, 
								unsigned short* aGSM_Cal_ARFCN_List, 
								unsigned char iNumOfChannel, 
								void * pGSM_Cal_ARFCN_NV, 
								unsigned char bCachedByNVManager)

{
	if (m_hQMSLPhone != NULL)
	{
		if( QLIB_RFCAL_GSM_CAL_ARFCN(m_hQMSLPhone, iNvModeId, 
												aGSM_Cal_ARFCN_List, 
												iNumOfChannel, 
												pGSM_Cal_ARFCN_NV, 
												bCachedByNVManager)){
			return true;
		}
	}

	return false;

}

bool CAndroidPhone::QMSL_FTM_LOAD_RF_NV(unsigned short* pusFTM_Error_Code)
{
	if (m_hQMSLPhone != NULL)
	{
		if( QLIB_FTM_LOAD_RF_NV(m_hQMSLPhone, pusFTM_Error_Code) ){
			return true;
		}
	}

	return false;

}

bool CAndroidPhone::QMSL_DIAG_NV_WRITE_F(unsigned short iItemID, unsigned char* szValue, int iLength, unsigned short* iStatus)
{
	if (m_hQMSLPhone != NULL)
	{
		if (! QLIB_DIAG_NV_WRITE_F(m_hQMSLPhone, iItemID, szValue, iLength, iStatus))
			return false;

		return true;
	}

	return false;
}

bool CAndroidPhone::QMSL_FTM_SET_TX_ENH_CAL_ACTION(int iState)
{
	if (m_hQMSLPhone!=NULL)
	{
		if (QLIB_FTM_SET_TX_ENH_CAL_ACTION(m_hQMSLPhone,iState))
			return true;
	}
	return false;
}

bool CAndroidPhone::QMSL_FTM_LOAD_TX_LINEARIZER_CAL_DATA(void* pFTM_Tx_Lin_CalData_Req, void* pFTM_Tx_Lin_CalData_Res)
{
    if (m_hQMSLPhone!=NULL)
    {
        if (QLIB_FTM_LOAD_TX_LINEARIZER_CAL_DATA(m_hQMSLPhone, pFTM_Tx_Lin_CalData_Req, pFTM_Tx_Lin_CalData_Res))
            return true;
    }
    return false;
}

bool CAndroidPhone::QMSL_FTM_LOAD_TX_FREQCOMP_CAL_DATA(void* pFTM_Tx_Freqcomp_CalData_Req, void* pFTM_Tx_Freqcomp_CalData_Res)
{
    if (m_hQMSLPhone!=NULL)
    {
        if (QLIB_FTM_LOAD_TX_FREQCOMP_CAL_DATA(m_hQMSLPhone, pFTM_Tx_Freqcomp_CalData_Req, pFTM_Tx_Freqcomp_CalData_Res))
            return true;
    }
    return false;
}

bool CAndroidPhone::QMSL_FTM_SET_SMPS_PA_BIAS_OVERRIDE(bool bOnOff)
{
	if (m_hQMSLPhone!=NULL)
	{
		if (QLIB_FTM_SET_SMPS_PA_BIAS_OVERRIDE(m_hQMSLPhone, bOnOff))
			return true;
	}
	return false;
}


bool CAndroidPhone::QMSL_FTM_SET_SMPS_PA_BIAS_VAL(int iPA_Bias_Value)
{
	if (m_hQMSLPhone!=NULL)
	{
		if (QLIB_FTM_SET_SMPS_PA_BIAS_VAL(m_hQMSLPhone, iPA_Bias_Value))
			return true;
	}
	return false;
}


bool CAndroidPhone::QMSL_FTM_SET_CALIBRATION_STATE(int iState)
{
	if (m_hQMSLPhone!=NULL)
	{
		if (QLIB_FTM_SET_CALIBRATION_STATE(m_hQMSLPhone,iState))
			return true;
	}
	return false;
}

bool CAndroidPhone::QMSL_FTM_SET_TX_SWEEP_STEP_SIZE(unsigned char ucStepSize)
{
	if (m_hQMSLPhone!=NULL)
	{
		if (QLIB_FTM_SET_TX_SWEEP_STEP_SIZE(m_hQMSLPhone,ucStepSize))
			return true;
	}
	return false;
}

bool CAndroidPhone::QMSL_FTM_CONFIGURE_TX_SWEEP_CAL(unsigned short iTimeInterval, unsigned short iNumofHDETReadingsPerStep)
{
	if (m_hQMSLPhone!=NULL)
	{
		if (QLIB_FTM_CONFIGURE_TX_SWEEP_CAL(m_hQMSLPhone,iTimeInterval,iNumofHDETReadingsPerStep))
			return true;
	}
	return false;
}

bool CAndroidPhone::QMSL_FTM_TX_SWEEP_CAL(bool bOnOff)
{
	if (m_hQMSLPhone!=NULL)
	{
		if (QLIB_FTM_TX_SWEEP_CAL(m_hQMSLPhone,(unsigned char)bOnOff))
			return true;
	}
	return false;
}

bool CAndroidPhone::QMSL_FTM_GET_HDET_FROM_TX_SWEEP_CAL(unsigned short usIndex,unsigned short* usHDET)
{
	if (m_hQMSLPhone!=NULL)
	{
		if (QLIB_FTM_GET_HDET_FROM_TX_SWEEP_CAL(m_hQMSLPhone,usIndex,usHDET))
			return true;
	}
	return false;
}

bool CAndroidPhone::QMSL_FTM_TX_RX_FREQ_CAL_SWEEP(FTM_Tx_Rx_Freq_Cal_Sweep_Request_Response *pFTM_Tx_Rx_Freq_Cal_Sweep_Request_Response)
{
   if (m_hQMSLPhone!=NULL)
   {
		if(QLIB_FTM_TX_RX_FREQ_CAL_SWEEP(m_hQMSLPhone,pFTM_Tx_Rx_Freq_Cal_Sweep_Request_Response))
			return true;
   }
   return false;
}

bool CAndroidPhone::QMSL_DIAG_NV_WRITE_SetBatchMode()
{
	if (m_hQMSLPhone!=NULL)
	{
		if(QLIB_DIAG_NV_WRITE_SetBatchMode(m_hQMSLPhone,true))
			return true;
	}
	return false;
}

bool CAndroidPhone::QMSL_DIAG_NV_WRITE_SetBatchMode_False()
{
	if (m_hQMSLPhone!=NULL)
	{
		if(QLIB_DIAG_NV_WRITE_SetBatchMode(m_hQMSLPhone, false))
			return true;
	}
	return false;
}

bool CAndroidPhone::QMSL_DIAG_NV_WRITE_FlushBatchQueue()
{
	unsigned short _iLastErrorNvId;
	unsigned short _iLastTotalStatus;
	unsigned char _iLastItemStatus;
	BOOL BOk = true;
	if (m_hQMSLPhone!=NULL)
	{
		if(QLIB_DIAG_NV_WRITE_FlushBatchQueue(m_hQMSLPhone, &_iLastErrorNvId, &_iLastTotalStatus, &_iLastItemStatus))
			return true;
	}
	return false;
}

bool CAndroidPhone::QMSL_FTM_GET_ENH_THERM(unsigned long* iThermValue)
{
	if (m_hQMSLPhone!=NULL)
	{
		if(QLIB_FTM_GET_ENH_THERM(m_hQMSLPhone, iThermValue))
			return true;
	}
	return false;
}

bool CAndroidPhone::QMSL_FTM_GET_THERM(unsigned char* ucThermValue)
{
	if (m_hQMSLPhone!=NULL)
	{
		if(QLIB_FTM_GET_THERM(m_hQMSLPhone, ucThermValue))
			return true;
	}
	return false;
}

bool CAndroidPhone::QMSL_RFCAL_Internal_Thermistor_Calibration_Results(void*  pInt_Therm_Cal_Meas,  
																	   void*  pInt_Therm_Cal_NV,  
																	   unsigned char  bCachedByNVManager)
{
	if (m_hQMSLPhone!=NULL)
	{
		if( QLIB_RFCAL_Internal_Thermistor_Calibration_Results(m_hQMSLPhone, pInt_Therm_Cal_Meas, pInt_Therm_Cal_NV, bCachedByNVManager) )
			return true;
	}
	return false;
}



//////////////////////////////////////////////////////////////////////////
// GSM
//////////////////////////////////////////////////////////////////////////


bool CAndroidPhone::QMSL_ChangeFTM_Mode(int ucFTMMode)
{
    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone != NULL)
    {
        if (QLIB_DIAG_CONTROL_F(m_hQMSLPhone, ucFTMMode))
            return true;
    }

    return false;
}

bool CAndroidPhone::QMSL_ChangeFTM_BootMode(int iMode)
{
    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone != NULL)
    {
        if (QLIB_ChangeFTM_BootMode(m_hQMSLPhone, iMode, FALSE))
            return true;
    }

    return false;
}

bool CAndroidPhone::QMSL_FTM_GSM_BER_StartSession(unsigned long iTimeOut)
{
    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        if ( QLIB_FTM_GSM_BER_StartSession(m_hQMSLPhone, iTimeOut) )
            return true ;
    }

    return false ;
}

bool CAndroidPhone::QMSL_FTM_GSM_BER_AssignBCCH (unsigned short iChannel, int iBand)
{
    unsigned char szBand ;

    // QLIB define:
    // 0 = PGSM 900 = FTM_GSM_BER_PGSM_900
    // 1 = EGSM 900 = FTM_GSM_BER_EGSM_900
    // 2 = PCS 1900 = FTM_GSM_BER_PCS_1900
    // 3 = DCS 1800 = FTM_GSM_BER_DCS_1800
    // 4 = CELL 850 = FTM_GSM_BER_CELL_850
    if (iBand==850)
        szBand = 4 ;
    else if (iBand==1800)
        szBand = 3 ;
    else if (iBand==1900)
        szBand = 2 ;
    else if (iBand==900)
        szBand = 1 ;
    else
        return false ;

    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        if ( QLIB_FTM_GSM_BER_AssignBCCH(m_hQMSLPhone, iChannel, szBand) )
            return true ;
    }

    return false ;
}


bool CAndroidPhone::QMSL_FTM_GSM_BER_Handover_NoLoopbackConfig(unsigned short iChannel,
                                                           int iBand,
                                                           unsigned char iSlot,
                                                           unsigned char iPowerLevel )
{
    const unsigned char iTSC = 0 ;
    const unsigned char iMODE = 0 ;

    // QLIB define:
    // 0 = PGSM 900 = FTM_GSM_BER_PGSM_900
    // 1 = EGSM 900 = FTM_GSM_BER_EGSM_900
    // 2 = PCS 1900 = FTM_GSM_BER_PCS_1900
    // 3 = DCS 1800 = FTM_GSM_BER_DCS_1800
    // 4 = CELL 850 = FTM_GSM_BER_CELL_850
    unsigned char szBand ;
    if (iBand==850)
        szBand = 4 ;
    else if (iBand==1800)
        szBand = 3 ;
    else if (iBand==1900)
        szBand = 2 ;
    else if (iBand==900)
        szBand = 1 ;
    else
        return false ;

    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        unsigned char bRet = false ;
        bRet = QLIB_FTM_GSM_BER_Handover_NoLoopbackConfig(m_hQMSLPhone,
            iChannel,
            szBand,
            iTSC,
            iSlot,
            iMODE,
            iPowerLevel ) ;
        if (bRet)
            return true ;
    }

    return false ;
}


bool CAndroidPhone::QMSL_FTM_GSM_BER_IsTrafficValid()
{
    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        if (QLIB_FTM_GSM_BER_IsTrafficValid(m_hQMSLPhone))
            return true ;
    }
    return false ;
}


bool CAndroidPhone::QMSL_FTM_GSM_BER_ClearStatus()
{
    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        if (QLIB_FTM_GSM_BER_ClearStatus(m_hQMSLPhone))
            return true ;
    }
    return false ;
}


bool CAndroidPhone::QMSL_FTM_GSM_BER_StopSession()
{
    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        if (QLIB_FTM_GSM_BER_StopSession(m_hQMSLPhone))
            return true ;
    }
    return false ;
}

bool CAndroidPhone::QMSL_FTM_GSM_BER_ReleaseTCH()
{
    if (m_hQMSLPhone!=NULL)
    {
        if (QLIB_FTM_GSM_BER_ReleaseTCH(m_hQMSLPhone))
            return true ;
    }
    return false ;
}

bool CAndroidPhone::QMSL_FTM_EGPRS_BER_Establish_SRB_LOOPBACK()
{
    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        if (QLIB_FTM_EGPRS_BER_Establish_SRB_LOOPBACK(m_hQMSLPhone))
            return true ;
    }
    return false ;
}



bool CAndroidPhone::QMSL_FTM_EGPRS_BER_AssignBCCH (unsigned short iChannel, int iBand)
{
    unsigned char szBand ;

    // QLIB define:
    // 0 = PGSM 900 = FTM_GSM_BER_PGSM_900
    // 1 = EGSM 900 = FTM_GSM_BER_EGSM_900
    // 2 = PCS 1900 = FTM_GSM_BER_PCS_1900
    // 3 = DCS 1800 = FTM_GSM_BER_DCS_1800
    // 4 = CELL 850 = FTM_GSM_BER_CELL_850
    if (iBand==850)
        szBand = 4 ;
    else if (iBand==1800)
        szBand = 3 ;
    else if (iBand==1900)
        szBand = 2 ;
    else if (iBand==900)
        szBand = 1 ;
    else
        return false ;

    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        if ( QLIB_FTM_EGPRS_BER_AssignBCCH(m_hQMSLPhone, iChannel, szBand) )
            return true ;
    }

    return false ;
}



bool CAndroidPhone::QMSL_FTM_EGPRS_BER_Configure_DL_TBF (unsigned short iChannel,
                                                     int iBand)
{
    unsigned char iDL_TSC = 0 ;
    unsigned char iDL_TFI = 1 ;
    unsigned char bFirstSlotActive = 1 ;
    unsigned char iFirstSlotNum = 3 ;
    unsigned char bSecondSlotActive = 1 ;
    unsigned char iSecondSlotNum  = 4 ;

    // QLIB define:
    // 0 = PGSM 900 = FTM_GSM_BER_PGSM_900
    // 1 = EGSM 900 = FTM_GSM_BER_EGSM_900
    // 2 = PCS 1900 = FTM_GSM_BER_PCS_1900
    // 3 = DCS 1800 = FTM_GSM_BER_DCS_1800
    // 4 = CELL 850 = FTM_GSM_BER_CELL_850
    unsigned char szBand ;
    if (iBand==850)
        szBand = 4 ;
    else if (iBand==1800)
        szBand = 3 ;
    else if (iBand==1900)
        szBand = 2 ;
    else if (iBand==900)
        szBand = 1 ;
    else
        return false ;

    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        unsigned char bRet = false ;
        bRet = QLIB_FTM_EGPRS_BER_Configure_DL_TBF (m_hQMSLPhone,
            iDL_TSC,
            iDL_TFI,
            iChannel,
            szBand,
            bFirstSlotActive,
            iFirstSlotNum,
            bSecondSlotActive,
            iSecondSlotNum) ;
        if (bRet)
            return true ;
    }

    return false ;
}


bool CAndroidPhone::QMSL_FTM_EGPRS_BER_Configure_UL_TBF (unsigned short iChannel,
                                                     int iBand,
                                                     unsigned char iGamma)
{
    unsigned char iUL_TSC = 0 ;
    unsigned char iUSF = 0 ;
    unsigned char bFirstSlotActive = 1 ;
    unsigned char iFirstSlotNum = 3 ;
    unsigned char bSecondSlotActive = 1 ;
    unsigned char iSecondSlotNum  = 4 ;

    // QLIB define:
    // 0 = PGSM 900 = FTM_GSM_BER_PGSM_900
    // 1 = EGSM 900 = FTM_GSM_BER_EGSM_900
    // 2 = PCS 1900 = FTM_GSM_BER_PCS_1900
    // 3 = DCS 1800 = FTM_GSM_BER_DCS_1800
    // 4 = CELL 850 = FTM_GSM_BER_CELL_850
    unsigned char szBand ;
    if (iBand==850)
        szBand = 4 ;
    else if (iBand==1800)
        szBand = 3 ;
    else if (iBand==1900)
        szBand = 2 ;
    else if (iBand==900)
        szBand = 1 ;
    else
        return false ;

    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        unsigned char bRet = false ;
        bRet = QLIB_FTM_EGPRS_BER_Configure_UL_TBF (m_hQMSLPhone,
            iUL_TSC,
            iUSF,
            iGamma,
            bFirstSlotActive,
            iFirstSlotNum,
            bSecondSlotActive,
            iSecondSlotNum ) ;

        if (bRet)
            return true ;
    }

    return false ;
}


bool CAndroidPhone::QMSL_FTM_EGPRS_BER_Release_All_TBF()
{
    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        if ( QLIB_FTM_EGPRS_BER_Release_All_TBF(m_hQMSLPhone) )
            return true ;
    }

    return false ;
}


bool CAndroidPhone::QMSL_FTM_GSM_ConfigLoopbackType(unsigned short usLookbackType)
{
    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        if ( QLIB_FTM_GSM_ConfigLoopbackType(m_hQMSLPhone, usLookbackType) )
            return true ;
    }

    return false ;
}


bool CAndroidPhone::QMSL_FTM_GSM_BER_GetRxMetrics(unsigned char* szRxLevel, unsigned char* szRxQual)
{
    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        if ( QLIB_FTM_GSM_BER_GetRxMetrics(m_hQMSLPhone, szRxLevel, szRxQual) )
            return true ;
    }

    return false ;
}


bool CAndroidPhone::QMSL_CheckPhoneConnect(int iRetryConnect)
{
    bool bIsConnect = false ;
    int  iRetry = iRetryConnect ;
    do
    {
        if ( IsConnected_QMSL() ) {
            bIsConnect = true;
            break ;
        }

        Sleep(500);
        --iRetry ;
    } while (iRetry > 0);

    if (! bIsConnect)
        return false ;

    return true ;
}

bool CAndroidPhone::QMSL_FTM_SET_TX_POWER_LEVEL(unsigned char nPowerLevel)
{
    if (m_hQMSLPhone == NULL)
        Initial_QMSL() ;

    if (m_hQMSLPhone!=NULL)
    {
        if ( QLIB_FTM_SET_TX_POWER_LEVEL(m_hQMSLPhone, nPowerLevel) )
            return true ;
    }

    return false ;
}


//////////////////////////////////////////////////////////////////////////
// WCDMA Test
//////////////////////////////////////////////////////////////////////////

bool CAndroidPhone::QMSL_FTM_UMTS_BER_ForceStopped()
{
	if (m_hQMSLPhone!=NULL)
	{
		if ( QLIB_FTM_UMTS_BER_ForceStopped(m_hQMSLPhone) )
			return true ;
	}

	return false ;
}

bool CAndroidPhone::QMSL_FTM_WCDMA_BER_RMC_DCH_TFCS_CONFIG_REQ(unsigned char ucTFCS_Select, unsigned char* ucTFCS_Config)
{
	if (m_hQMSLPhone!=NULL)
	{
		if ( QLIB_FTM_WCDMA_BER_RMC_DCH_TFCS_CONFIG_REQ(m_hQMSLPhone, ucTFCS_Select, ucTFCS_Config) )
			return true ;
	}

	return false ;
}

bool CAndroidPhone::QMSL_FTM_WCDMA_BER_StartSession(unsigned long ulTimeOut_ms)
{
	if (m_hQMSLPhone!=NULL)
	{
		if ( QLIB_FTM_WCDMA_BER_StartSession(m_hQMSLPhone, ulTimeOut_ms) )
			return true ;
	}

	return false ;
}

bool CAndroidPhone::QMSL_FTM_WCDMA_BER_AcquirePilot(unsigned short usChannel)
{
	if (m_hQMSLPhone!=NULL)
	{
		if ( QLIB_FTM_WCDMA_BER_AcquirePilot(m_hQMSLPhone, usChannel) )
			return true ;
	}

	return false ;
}

bool CAndroidPhone::QMSL_FTM_WCDMA_BER_StopSession()
{
	if (m_hQMSLPhone!=NULL)
	{
		if ( QLIB_FTM_WCDMA_BER_StopSession(m_hQMSLPhone) )
			return true ;
	}

	return false ;
}

bool CAndroidPhone::QMSL_FTM_WCDMA_BER_Handover(unsigned char iRMC_Type, unsigned char iUL_MaxTxPwr, unsigned long iUL_DPCH_ScrCode, unsigned char iUL_PCA, unsigned char iUL_TPC_StepSize,  
											unsigned short iDL_PriCPICH_Scr_Code, unsigned short iDL_ChanCode, unsigned char iBetaC, unsigned char iBetaD, unsigned short iChannel)
{
	if (m_hQMSLPhone!=NULL)
	{
		if ( QLIB_FTM_WCDMA_BER_Handover(m_hQMSLPhone, iRMC_Type, iUL_MaxTxPwr, iUL_DPCH_ScrCode, iUL_PCA, iUL_TPC_StepSize, iDL_PriCPICH_Scr_Code, iDL_ChanCode, iBetaC, iBetaD, iChannel) )
			return true ;
	}

	return false ;
}

bool CAndroidPhone::QMSL_FTM_WCDMA_BER_Handover_V2B(
												unsigned char iRMC_Type, 
												unsigned char iUL_MaxTxPwr, 
												unsigned long iUL_DPCH_ScrCode, 
												unsigned char iUL_PCA,
												unsigned char iUL_TPC_StepSize, 
												unsigned short iDL_PriCPICH_Scr_Code, 
												unsigned short iDL_ChanCode,
												unsigned char iBetaC, 
												unsigned char iBetaD,
												short iDPCCH_PowerOffset,
												unsigned char n_312,
												unsigned char n_313,
												unsigned char n_315,
												unsigned char t_312,
												unsigned char t_313,
												unsigned short iChannel
												)
{
	if (m_hQMSLPhone!=NULL)
	{
		if ( QLIB_FTM_WCDMA_BER_Handover_V2B(
										m_hQMSLPhone			, iRMC_Type		, iUL_MaxTxPwr	, iUL_DPCH_ScrCode	, iUL_PCA			, iUL_TPC_StepSize, 
										iDL_PriCPICH_Scr_Code	, iDL_ChanCode	, iBetaC		, iBetaD			, iDPCCH_PowerOffset, n_312,
										n_313					,n_315			, t_312			, t_313				, iChannel) )
			return true ;
	}

	return false ;
}


bool CAndroidPhone::QMSL_FTM_DO_DC_CAL()
{
    if (m_hQMSLPhone!=NULL)
    {
        if ( QLIB_FTM_DO_DC_CAL(m_hQMSLPhone) )
            return true ;
    }

    return false ;
}

bool CAndroidPhone::QMSL_FTM_GET_DVGA_OFFSET(short iExpectedRX_AGC, short* iDVGAoffset)
{
	if (m_hQMSLPhone!=NULL)
	{
		if ( QLIB_FTM_GET_DVGA_OFFSET(m_hQMSLPhone, iExpectedRX_AGC, iDVGAoffset) )
			return true ;
	}

	return false ;
}

bool CAndroidPhone::QMSL_FTM_GET_CDMA_IM2(unsigned char* piI_Value, unsigned char* piQ_Value,
										 unsigned char* piTransConductance_Value)
{
	if (m_hQMSLPhone!=NULL)
	{
		if ( QLIB_FTM_GET_CDMA_IM2(m_hQMSLPhone, piI_Value, piQ_Value, piTransConductance_Value) )
			return true ;
	}

	return false ;
}

bool CAndroidPhone::QMSL_PHONE_GET_MODE( unsigned long *piPhoneMode )
{
	if(m_hQMSLPhone != NULL)
	{
		if (QLIB_GetPhoneOperatingMode(m_hQMSLPhone,piPhoneMode))
		{		
			return true;	
		}
	}

	return false;
}

bool CAndroidPhone::Adb_Shell_Start()
{
	return MyExecAdbCmd("start");
}

bool CAndroidPhone::WifiBinNumberWrite(int offset, int length, int nBin)
{
	char input_parameter[4096] = {0};
	char sz_outData[4096] = {0};
	sprintf( input_parameter, "%d,%d,%d", offset, length, nBin );

	int nRetCode;

	/* Get DLL function */
	pDetroit_FAC_CFGWrite FTD_Detroit_FAC_CFGWrite = (pDetroit_FAC_CFGWrite) GetProcAddress(m_hNeptuneCtrl, "Detroit_FAC_CFGWrite");
	if (!FTD_Detroit_FAC_CFGWrite)
	{
		return false;
	}

	if(FTD_Detroit_FAC_CFGWrite)
	{
		nRetCode = FTD_Detroit_FAC_CFGWrite( FTD_PORT, FTD_TIMEOUT, input_parameter, sz_outData );

		if(nRetCode == 1) {//HS_OK
			return true;
		}else {
			return false;
		}
	}
		
	return false;
}

bool CAndroidPhone::ExecAdbCommand(CString str_command, char* pszOutput, char* p_sz_ErrorMsg)
{
	bool b_Res = false;

	/* Get DLL function */
	PFExecAdbOutFunc pExecAdbOut = (PFExecAdbOutFunc)::GetProcAddress(m_hNeptuneCtrl, _T("Detroit_ExecAdbOut"));
	if (!pExecAdbOut)
	{
		return false;
	}

	if(pExecAdbOut)
	{
		b_Res = pExecAdbOut(str_command, pszOutput, p_sz_ErrorMsg);
	}

	return b_Res;
}

bool CAndroidPhone::ResetBinFile()
{
	char szErr[FTD_BUFSIZE] = {0};
	char szOut[FTD_BUFSIZE] = {0};
	char szIn[FTD_BUFSIZE] ={0};
	CString csCommandShell;

	csCommandShell.Format("shell mount -t ext4 -o remount,rw,nosuid,nodev,barrier=1 /dev/block/platform/msm_sdcc.1/by-name/persist /persist");
	if( !ExecAdbCommand(csCommandShell, szOut, szErr) )
		return false;
	
	Sleep(500);
	csCommandShell.Format("push WCNSS_qcom_wlan_nv0.bin %s", _T("/persist/WCNSS_qcom_wlan_nv0.bin"));
	if( !ExecAdbCommand(csCommandShell, szOut, szErr) )
		return false;

	csCommandShell.Format("push WCNSS_qcom_wlan_nv1.bin %s", _T("/persist/WCNSS_qcom_wlan_nv1.bin"));
	if( !ExecAdbCommand(csCommandShell, szOut, szErr) )
		return false;

	csCommandShell.Format("push WCNSS_qcom_wlan_nv2.bin %s", _T("/persist/WCNSS_qcom_wlan_nv2.bin"));
	if( !ExecAdbCommand(csCommandShell, szOut, szErr) )
		return false;

	return true;
}

bool CAndroidPhone::Nordic2G4_Consumer(char *szInput)
{
	PFGenCmdInOut FTD_2_4GConsumer = (PFGenCmdInOut)GetProcAddress(m_hNeptuneCtrl, "FTD_2_4GConsumer");
	if(!FTD_2_4GConsumer)
	{
		return false;
	}
	char szError[FTD_BUF_SIZE] = {0};
	int ret = FTD_2_4GConsumer(FTD_PORT, FTD_TIMEOUT, szInput, szError);
	return ret == FTD_OK;
}

bool CAndroidPhone::Nordic2G4_PowerMode(char *szInput)
{
	PFGenCmdInOut FTD_2_4GPowerMode = (PFGenCmdInOut)GetProcAddress(m_hNeptuneCtrl, "FTD_2_4GPowerMode");
	if(!FTD_2_4GPowerMode)
	{
		return false;
	}
	char szError[FTD_BUF_SIZE] = {0};
	int ret = FTD_2_4GPowerMode(FTD_PORT, FTD_TIMEOUT, szInput, szError);
	return ret == FTD_OK;
}

bool CAndroidPhone::Nordic2G4_PowerOn(char *szInput)
{
	PFGenCmdInOut FTD_2_4GTxPower = (PFGenCmdInOut)GetProcAddress(m_hNeptuneCtrl, "FTD_2_4GTxPower");
	if(!FTD_2_4GTxPower)
	{
		return false;
	}
	char szError[FTD_BUF_SIZE] = {0};
	int ret = FTD_2_4GTxPower(FTD_PORT, FTD_TIMEOUT, szInput, szError);
	return ret == FTD_OK;
}