#include "StdAfx.h"
#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <Setupapi.h>
#include <Cfgmgr32.h>
#include <WinIoCtl.h>
#include "../../Lib/WinDDK/inc/wxp/usb100.h"
#include "../MobileCommand/NewADB/usb/api/adb_api_legacy.h"
#include "../MobileCommand/NewADB/usb/api/adb_api.h"
#include "DownloadA9.h"


GUID GUIDUsbstor				= {0x4D36E967, 0xE325, 0x11CE, {0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18}};
GUID GUIDUsb					= {0x36FC9E60, 0xC465, 0x11CF, {0x80, 0x56, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};
GUID GUIDUsbInterface			= {0xA5DCBF10, 0x6530, 0x11D2, {0x90, 0x1f, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED}};
GUID GUIDDisk					= {0x53f56307, 0xb6bf, 0x11D0, {0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b}};
GUID GUIDQualcommDiagUsbClass	= {0x86e0d1e0, 0x8089, 0x11d0, {0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73}};
GUID GUIDQualcommModemUsbClass	= {0x2c7089aa, 0x2e0e, 0x11d1, {0xb1, 0x14, 0x00, 0xc0, 0x4f, 0xc2, 0xaa, 0xe4}};
GUID GUIDAndroidUsbClassID		= {0xf72fe0d4, 0xcbcb, 0x407d, {0x88, 0x14, 0x9e, 0xd6, 0x73, 0xd0, 0xdd, 0x6b}};


CMutex CDownloadA9::m_Mutex_Command;


CDownloadA9::CDownloadA9(int iSlot, int iCOMPort)
	: m_iSlot(iSlot),
	m_iCOMPort(iCOMPort)
{
	DefineNotify(EVENT_DL_PROGRESS);

	char szModuleFilePath[256];
	memset(szModuleFilePath, 0, 256);
	GetModuleFileNameA(NULL, szModuleFilePath, 256);
	PathRemoveFileSpecA(szModuleFilePath);
	m_strExeCmdPath = szModuleFilePath;
}

void CDownloadA9::SetDownloadPath(const char* szDLPath)
{
	m_strDLPath = szDLPath;
}

void CDownloadA9::SetDownloadFile(const char* szFileName, const char* szFileType)
{
	std::pair<std::string, std::string> pair_DLFile;
	pair_DLFile.first = szFileType;
	pair_DLFile.second = szFileName;
	m_vec_DLFile.push_back(pair_DLFile);
}

bool CDownloadA9::GetADBSerial()
{
	CSingleLock singleLock(&m_Mutex_Command);

	std::vector<USBDEVICE_STRUCT> QualDevs;
	std::vector<USBDEVICE_STRUCT> ADBDevs;

	// 1. Find all Qualcomm devices and ADB devices
	if (! (FindQualcomDevice(QualDevs) && FindADBdevice(ADBDevs)))
		return false;

	// 2. Pair Qualcomm devices with ADB devices
	std::map<int, std::string> map_DevicePair; // map<COMPort, ADBSerial>

	for (std::vector<USBDEVICE_STRUCT>::iterator iterQual = QualDevs.begin(); iterQual != QualDevs.end(); ++iterQual)
	{
		for (std::vector<USBDEVICE_STRUCT>::iterator iterADB = ADBDevs.begin(); iterADB != ADBDevs.end(); ++iterADB)
		{
			if (strcmp((*iterQual).chParentName, (*iterADB).chParentName) == 0)
			{
				std::string strSerial = "";
				if (GetSerialNumberFromADB((*iterADB).chHandleName, &strSerial ))
				{
					int iCOMPort = GetComportFromUSB(*iterQual);
					map_DevicePair[iCOMPort] = strSerial;
					break;
				}
			}
		}
	}

	// 3. Determine ADB serial
	if (map_DevicePair.find(m_iCOMPort) == map_DevicePair.end())
	{
		AddMessage("Fail to find phone ADB serial");
		return false;
	}
	else
	{
		m_strADBSerial = map_DevicePair[m_iCOMPort];

		char szMsg[256];
		memset(szMsg, 0, 256);
		sprintf(szMsg, "COM %03d - ADB Serial = %s", m_iSlot, m_strADBSerial.c_str());
		AddMessage(szMsg);
	}

	Sleep(1000);
	
	singleLock.Unlock();

	Sleep(2000);
	return true;
}

bool CDownloadA9::GetADBSerial_New()
{
	CSingleLock singleLock(&m_Mutex_Command);

	std::vector<USBDEVICE_STRUCT> QualDevs;
	std::vector<USBDEVICE_STRUCT> ADBDevs;

	// 1. Find all Qualcomm devices and ADB devices
	if (! (FindQualcomDevice(QualDevs) && FindADBdevice(ADBDevs)))
	{
		return false;
	}

	// 2.  find USB root by COM port of this instance
	for (std::vector<USBDEVICE_STRUCT>::iterator iterQual = QualDevs.begin(); iterQual != QualDevs.end(); ++iterQual)
	{
		int iCOMPort = GetComportFromUSB(*iterQual);
		if ( m_iCOMPort == iCOMPort)
		{
			m_USB_COM_Interface_Name = (*iterQual).chHandleName;
			m_USB_Root_Name = (*iterQual).chParentName;
			break;
		}
		else
		{
			AddMessage(" Can't find COM port in USB device list");
			return false;
		}

	}

	// 3. Find ADB interface that under the same USB root 
	for (std::vector<USBDEVICE_STRUCT>::iterator iterADB = ADBDevs.begin(); iterADB != ADBDevs.end(); ++iterADB)
	{
		if (strcmp(m_USB_Root_Name.c_str() , (*iterADB).chParentName )==0 )
		{
			m_USB_ADB_Interface_Name = (*iterADB).chHandleName;
			break;
		}
		else
		{
			AddMessage(" Can't find ADB Device of the same USB root");
			return false;
		}
	}

	// 4. Create ADB interface, and get serial number
	m_pADB = new ADBDevice(m_USB_ADB_Interface_Name.c_str(),10);
	if ( m_pADB)
	{
		m_pADB->GetSerialNumber(&m_strADBSerial);
	}
	else
	{
		AddMessage(" Can't create ADB interface");
		return false;
	}

	//final result
	char szMsg[256];
	sprintf(szMsg, "COM %03d - ADB Serial = %s", m_iCOMPort, m_strADBSerial.c_str());
	AddMessage(szMsg);

	singleLock.Unlock();

	Sleep(2000);
	return true;
}

bool CDownloadA9::SetToFastbootMode()
{
	bool bSuccess = false;
	char szTmp[1024];

	CSingleLock singleLock(&m_Mutex_Command, TRUE);

	// 1. Switch phone to fastboot mode
	AddMessage("[Cmd] adb reboot bootloader");

	//adb
	if (! SendAdbCmd("reboot bootloader", NULL, 0, 10000))
	{
		AddMessage("[Cmd] adb reboot bootloader FAIL");
		return false;
	}

	//acb
	//if (! SendAcbCmd("reboot bootloader", NULL, 0, 10000))
	//{
	//	AddMessage("[Cmd] adb reboot bootloader FAIL");
	//	return false;
	//}

	Sleep(7000);

	// 2. Check if phone is in fastboot mode

	for (int i = 0; i < 20; ++i)
	{
		memset(szTmp, 0, 1024);
		AddMessage("[Cmd] fastboot devices");

		if (! SendFastbootCmd("devices", szTmp, 1024))
		{
			AddMessage("[Cmd] fastboot devices FAIL");
			return false;
		}

		if (strstr(szTmp, m_strADBSerial.c_str()) != NULL)
		{
			bSuccess = true;
			break;
		}

		Sleep(500);
	}

	Sleep(5000);

	if (bSuccess)
		AddMessage("Phone is now in fastboot mode");
	else
		AddMessage("Fail to switch phone to fastboot mode");

	singleLock.Unlock();

	Sleep(2000);
	return bSuccess;
}
bool CDownloadA9::SetToFastbootModeACB()
{
   bool bSuccess = false;
	char szTmp[1024];

	CSingleLock singleLock(&m_Mutex_Command, TRUE);

	// 1. Switch phone to fastboot mode
	AddMessage("[Cmd] acb reboot bootloader");
	
	//acb
	if (! SendAcbCmd("reboot bootloader", NULL, 0, 10000))
	{
		AddMessage("[Cmd] acb reboot bootloader FAIL");
		return false;
	}

	Sleep(7000);

	// 2. Check if phone is in fastboot mode

	for (int i = 0; i < 20; ++i)
	{
		memset(szTmp, 0, 1024);
		AddMessage("[Cmd] fastboot devices");

		if (! SendFastbootCmd("devices", szTmp, 1024))
		{
			AddMessage("[Cmd] fastboot devices FAIL");
			return false;
		}

		if (strstr(szTmp, m_strADBSerial.c_str()) != NULL)
		{
			bSuccess = true;
			break;
		}

		Sleep(500);
	}

	Sleep(5000);

	if (bSuccess)
		AddMessage("Phone is now in fastboot mode");
	else
		AddMessage("Fail to switch phone to fastboot mode");

	singleLock.Unlock();

	Sleep(2000);
	return bSuccess;

}

bool CDownloadA9::FastbootDL()
{
	bool bRet = true;
	
	char szCmd[512];
	char szReturn[1024];

	UpdateProgress("Start fastboot download", ADBProtocol, 0);


	for (unsigned int i = 0; i < m_vec_DLFile.size(); ++i)
	{
		std::pair<std::string, std::string> pair_DLFile = m_vec_DLFile[i];
		memset(szCmd, 0, 512);
		sprintf(szCmd, "flash %s %s\\%s", pair_DLFile.first.c_str(), m_strDLPath.c_str(), pair_DLFile.second.c_str());

		char szMsg[512];
		memset(szMsg, 0, 512);
		sprintf(szMsg, "[Cmd] fastboot %s", szCmd);
		AddMessage(szMsg);

		memset(szReturn, 0, 1024);
		if (SendFastbootCmd(szCmd, szReturn, 1024, 330000))
		{
			if (strlen(szReturn) != 0)
			{
				AddMessage(szReturn);
				if ((! strstr(szReturn, "OKAY")) && (! strstr(szReturn, "successfully")))
				{
					memset(szMsg, 0, 512);
					sprintf(szMsg, "#%02d file download SUCCESS", i + 1);
					AddMessage(szMsg);
					Sleep(500);
				}
			}
			else
			{
				AddMessage("[Cmd] fastboot flash success but no response");
				bRet = false;
				break;
			}
		}
		else
		{
			AddMessage("[Cmd] fastboot flash FAIL");
			bRet = false;
			break;
		}

		// Update progress bar
		sprintf(szCmd, "Download %s finish", pair_DLFile.first.c_str());
		UpdateProgress(szCmd, ADBProtocol, ((i + 1) * 100.0) / m_vec_DLFile.size());
	}

	UpdateProgress("Finish fastboot download", ADBProtocol, 100);
	
	return bRet;
}

bool CDownloadA9::FastbootDL_New()
{
	bool bRet = true;

	std::vector<USBDEVICE_STRUCT> ADBDevs;
	FindADBdevice(ADBDevs);

	m_pFastboot =NULL;
	for (std::vector<USBDEVICE_STRUCT>::iterator iterQual = ADBDevs.begin(); iterQual != ADBDevs.end(); ++iterQual)
	{
		ADBDevice *p_adb_tmp= NULL;
		p_adb_tmp = new ADBDevice ((*iterQual).chHandleName , 10);

		std::string strFastbootSerial="";
		p_adb_tmp->GetSerialNumber(&strFastbootSerial);

		if (strFastbootSerial == m_strADBSerial )
		{
			m_USB_Fastboot_Interface_Name = (*iterQual).chHandleName;
			m_pFastboot = p_adb_tmp;
		}
		else
		{
			delete p_adb_tmp;
		}
	}
	
	for (unsigned int i = 0; i < m_vec_DLFile.size(); ++i)
	{
		char szDLFilePath[512] = {0};
		std::pair<std::string, std::string> pair_DLFile = m_vec_DLFile[i];
		
		memset(szDLFilePath, 0, 512);
		sprintf(szDLFilePath, "%s\\%s", m_strDLPath.c_str(), pair_DLFile.second.c_str());
		if(!ADBFlashAll(pair_DLFile.first.c_str(), szDLFilePath))
		{
			bRet = false;
			break;
		}

		// Update progress bar
		char szCmd[100] = {0};
		sprintf(szCmd, "Download %s finish", pair_DLFile.first.c_str());
		UpdateProgress(szCmd, ADBProtocol, ((i + 1) * 100.0) / m_vec_DLFile.size());
	}

	return bRet;
}

bool CDownloadA9::FastbootReset()
{
	CSingleLock singleLock(&m_Mutex_Command, TRUE);

	AddMessage("[Cmd] fastboot reboot");

	SendFastbootCmd("reboot", NULL, 0, 10000);
	Sleep(1000);
	SendFastbootCmd("reboot", NULL, 0, 10000);
	Sleep(1000);
	SendFastbootCmd("reboot", NULL, 0, 10000);
	Sleep(15000);


	return true;
}

bool CDownloadA9::FastbootReset_New()
{
	CSingleLock singleLock(&m_Mutex_Command, TRUE);

	if(NULL != m_pFastboot)
	{
		m_pFastboot->FastbootReboot(0);
	}

	return true;
}

bool CDownloadA9::FindQualcomDevice(std::vector<USBDEVICE_STRUCT> &inUSBdevice)
{
	GUID *pGUID = &GUIDQualcommDiagUsbClass;

	USBDEVICE_STRUCT devStruct = {0};
	BYTE chTemp [MAX_PATH];
	DWORD dwRequired;

	BYTE bTmpStringData[MAX_PATH];
	HDEVINFO hDeviceInfo;
	SP_DEVICE_INTERFACE_DATA deviceInfoData;
	SP_DEVICE_INTERFACE_DETAIL_DATA *pDeviceDetailData = (SP_DEVICE_INTERFACE_DETAIL_DATA *)bTmpStringData;
	
	SP_DEVINFO_DATA devInfo = {0};
	devInfo.cbSize = sizeof(SP_DEVINFO_DATA);

	hDeviceInfo = SetupDiGetClassDevs(pGUID, NULL, NULL, (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));
	if (INVALID_HANDLE_VALUE == hDeviceInfo)
		return false;

	bool bFound = false;

	// enum devices
	int i = 0;
	while (1)
	{
		// Get device interface data
		deviceInfoData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		if (! SetupDiEnumDeviceInterfaces(hDeviceInfo, 0, pGUID, i, &deviceInfoData))
			break;

		memset(&devStruct, 0, sizeof(USBDEVICE_STRUCT));

		// Get device path
		pDeviceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		if (SetupDiGetDeviceInterfaceDetail(hDeviceInfo,&deviceInfoData,pDeviceDetailData,256 ,NULL, &devInfo) &&
			(pDeviceDetailData->cbSize > 0) &&
			(0 == _strnicmp("\\\\?\\usb", pDeviceDetailData->DevicePath, 7))) // We only care about usb device
		{
			strcpy(devStruct.chHandleName, pDeviceDetailData->DevicePath);

			// Get the device instance of parent. This points to USB Composite Device
			DEVINST devInstParent;
			CM_Get_Parent(&devInstParent, devInfo.DevInst, 0);

			// Get the device ID of the USB root.
			CM_Get_Device_ID(devInstParent, (char*)chTemp, MAX_PATH, 0);
			strcpy(devStruct.chParentName, (char*)chTemp);

			// Get device instance name
			if (SetupDiGetDeviceInstanceId(hDeviceInfo, &devInfo, (PSTR)chTemp, MAX_PATH, &dwRequired))
				strcpy(devStruct.chInstanceName, (char*)chTemp);

			// Get device friendly name
			DWORD dataType;
			if (SetupDiGetDeviceRegistryProperty(hDeviceInfo, &devInfo, SPDRP_FRIENDLYNAME, &dataType, chTemp, MAX_PATH, &dwRequired))
				strcpy(devStruct.chFriendlyName, (char*)chTemp);

			if (SetupDiGetDeviceRegistryProperty(hDeviceInfo, &devInfo, SPDRP_DEVICEDESC, &dataType, chTemp, MAX_PATH, &dwRequired))
				strcpy(devStruct.chDeviceDescr, (char*)chTemp);

			inUSBdevice.push_back(devStruct);
			bFound = true;
		}
		++i;
	}

	// Destroy handle
	SetupDiDestroyDeviceInfoList(hDeviceInfo);
	return bFound;
}

bool CDownloadA9::FindADBdevice(std::vector<USBDEVICE_STRUCT> &inUSBdevice)
{
	GUID *pGUID = &GUIDAndroidUsbClassID;

	USBDEVICE_STRUCT devStruct = {0};
	BYTE chTemp [MAX_PATH];
	DWORD dwRequired;

	BYTE bTmpStringData[MAX_PATH];
	HDEVINFO hDeviceInfo;
	SP_DEVICE_INTERFACE_DATA deviceInfoData;
	SP_DEVICE_INTERFACE_DETAIL_DATA *pDeviceDetailData = (SP_DEVICE_INTERFACE_DETAIL_DATA *)bTmpStringData;

	SP_DEVINFO_DATA devInfo = {0};
	devInfo.cbSize = sizeof(SP_DEVINFO_DATA);

	hDeviceInfo = SetupDiGetClassDevs(pGUID, NULL, NULL, (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));
	if (INVALID_HANDLE_VALUE == hDeviceInfo)
		return false;

	bool bFound = false;

	// enum devices
	int i = 0;
	while (1)
	{
		// Get device interface data
		deviceInfoData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		if (! SetupDiEnumDeviceInterfaces(hDeviceInfo, 0, pGUID, i, &deviceInfoData))
			break;

		memset(&devStruct, 0, sizeof(USBDEVICE_STRUCT));

		// Get device path
		pDeviceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		if (SetupDiGetDeviceInterfaceDetail(hDeviceInfo,&deviceInfoData,pDeviceDetailData,256 ,NULL, &devInfo) &&
			(pDeviceDetailData->cbSize > 0) 
			&&(0 == _strnicmp("\\\\?\\usb", pDeviceDetailData->DevicePath, 7))) // We only care about usb device
		{
			strcpy(devStruct.chHandleName, pDeviceDetailData->DevicePath);

			// Get the device instance of parent. This points to USB Composite Device
			DEVINST devInstParent;
			CM_Get_Parent(&devInstParent, devInfo.DevInst, 0);

			// Get the device ID of the USB root.
			CM_Get_Device_ID(devInstParent, (char*)chTemp, MAX_PATH, 0);
			strcpy(devStruct.chParentName, (char*)chTemp);

			// Get device instance name
			if (SetupDiGetDeviceInstanceId(hDeviceInfo, &devInfo, (PSTR)chTemp, MAX_PATH, &dwRequired))
				strcpy(devStruct.chInstanceName, (char*)chTemp);

			// Get device friendly name
			DWORD dataType;
			if (SetupDiGetDeviceRegistryProperty(hDeviceInfo, &devInfo, SPDRP_FRIENDLYNAME, &dataType, chTemp, MAX_PATH, &dwRequired))
				strcpy(devStruct.chFriendlyName, (char*)chTemp);

			if (SetupDiGetDeviceRegistryProperty(hDeviceInfo, &devInfo, SPDRP_DEVICEDESC, &dataType, chTemp, MAX_PATH, &dwRequired))
				strcpy(devStruct.chDeviceDescr, (char*)chTemp);

			inUSBdevice.push_back(devStruct);
			bFound = true;
		}
		++i;
	}

	// Destroy handle
	SetupDiDestroyDeviceInfoList(hDeviceInfo);
	return bFound;
}

bool CDownloadA9::GetSerialNumberFromADB(const char* chUsbName, std::string* strBuffer)
{
	bool ret=false;

	ADBDevice *pAdb = new ADBDevice(chUsbName ,10);
	ret = pAdb->GetSerialNumber( strBuffer);
	delete pAdb;

	return ret;

	/*
	ADBAPIHANDLE hInterface; 
	ADBAPIHANDLE hADB;
	bool ret = false;

	memset(chBuffer, 0, dwLen);

	hADB = AdbEnumInterfaces(GUIDAndroidUsbClassID, true, true, true);
	if (! hADB)
		return false;

	BYTE byTemp[2048];
	AdbInterfaceInfo *adbInterfaceInfo = (AdbInterfaceInfo*) byTemp;
	unsigned long size = 2048;

	while (AdbNextInterface(hADB, adbInterfaceInfo, &size))
	{
		char chTemp[2048];
		unsigned long str_len = wcslen(adbInterfaceInfo->device_name) + 1;
		WideCharToMultiByte(
			CP_ACP,
			0,
			adbInterfaceInfo->device_name,
			str_len,
			chTemp,
			2048,
			NULL,
			NULL
			);

		if (strcmp(chTemp, chUsbName) == 0)
		{
			hInterface = AdbCreateInterfaceByName(adbInterfaceInfo->device_name);
			if (hInterface != NULL)
			{
				size = 2048;
				if (AdbGetSerialNumber(hInterface, chTemp, &size, true))
					strcpy(chBuffer, chTemp);

				AdbCloseHandle(hInterface);
				ret = true;
				break;
			}
		}

		size = 2048;
	}

	AdbCloseHandle(hADB);
	return ret
	*/
}


bool CDownloadA9::ADBFlashAll(const char* sz_parmeter, const char* sz_fileName)
{
	if ((sz_parmeter == NULL) || (sz_fileName == NULL)) 
	{
		AddMessage("CDownloadA9::ADBFlashAll input parameter NULL");
		return false;
	}

	AddMessage("Fastboot download partition parameter : %s", sz_parmeter);

	if ( !m_pFastboot->IsActive()) 
	{
		AddMessage("m_pFastboot IsActive() check fail");
		return false;
	}

	//create File
	HANDLE h_file  = NULL;
	DWORD dw_fileSize = 0;
	h_file = CreateFile(sz_fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (h_file == INVALID_HANDLE_VALUE) 
	{
		AddMessage("CreateFile Failure.");
		return false;
	}

	//Get file size
	dw_fileSize = GetFileSize(h_file, NULL);
	if (dw_fileSize == INVALID_FILE_SIZE)
	{
		CloseHandle(h_file);
		AddMessage("GetFileSize is more than 2G.");
		return false;
	}

	//check zero file 
	if (dw_fileSize <= 0)
	{
		CloseHandle(h_file);
		AddMessage("File size <= 0.");
		return false;
	}

	// download protocol 1--- "download:%08x"  example:"download:00162c00" 
	char sz_Msg[100] = {0};
	char sz_command[64] = {0};
	sprintf(sz_command, "download:%08x", dw_fileSize);
	sprintf(sz_Msg, "[FBDL_1], START, write download : size = %08x", dw_fileSize);
	
	AddMessage(sz_Msg);
	if ( !m_pFastboot->Write(sz_command, strlen(sz_command), 60000)) 
	{
		AddMessage("[FBDL_1], write download : size fail ");
		return false;
	}
	else
		AddMessage("[FBDL_1], write download : size pass ");
	
	AddMessage("[FBDL_1], END");
	AddMessage(" ");

	// download protocol 2--- if ReadResponse = DATA , go head
	AddMessage("[FBDL_2], START, read response");
	if ( RESPONSE::DATA != m_pFastboot->ReadResponse(60000) ) 
	{
		AddMessage("[FBDL_2], read response != DATA ");
		return false;
	}
	else
		AddMessage("[FBDL_2], read response = DATA ");
	
	AddMessage("[FBDL_2], END");
	AddMessage(" ");

	//download protocol 3--- write data
	DWORD dw_bytesToRead = dw_fileSize;
	DWORD dw_bytesReaded = 0;
	DWORD dw_blockSize   = 4096;
	DWORD dw_ReadBytes = 0;
	char sz_byteSize[4096] = {0};

	int i_progress = 0;
	int less_message = 1;
	unsigned long long l_count = 0;

	AddMessage("[FBDL_3], START, write data");
	while(dw_bytesToRead > 0) 
	{
		if(dw_blockSize > dw_bytesToRead) 
		{
			dw_blockSize = dw_bytesToRead;
		}

		if(!ReadFile(h_file, sz_byteSize, dw_blockSize, &dw_ReadBytes, NULL) || dw_ReadBytes != dw_blockSize)
		{
			AddMessage("[FBDL_3] read data from file fail");
			CloseHandle(h_file);
			return false;
		}

		dw_bytesReaded += dw_blockSize;
		dw_bytesToRead -= dw_blockSize;
		i_progress = (int)((l_count * 100) / dw_fileSize);
		
		int xfer = (dw_blockSize > 4096) ? 4096 : dw_blockSize;
		if (!m_pFastboot->Write(sz_byteSize, xfer, 60000))
		{
			AddMessage("[FBDL_3] Write file bytes to device fail");
			CloseHandle(h_file);
			return false;
		}

		l_count += xfer;

		//prorgess shown control
		if (  (i_progress)/10 != less_message )
		{
			AddMessage("[FBDL_3] Write %s : %d ", sz_parmeter, i_progress );
			less_message = i_progress/10 ;
		}
		
	}
	AddMessage("[FBDL_3], END");
	AddMessage(" ");

	//download protocol 4--- read response
	AddMessage("[FBDL_4], START, read response");
	while (1)
	{
		RESPONSE ret = m_pFastboot->ReadResponse(300000);
		
		if(RESPONSE::OKAY == ret) 
		{
			AddMessage("[FBDL_4], After DL, read protocol response = OKAY");
			break;
		}
		else if(RESPONSE::DATA == ret)
		{
			AddMessage("[FBDL_4], After DL, read protocol response = DATA");
		}
		else if(RESPONSE::INFO == ret) 
		{
			AddMessage("[FBDL_4], After DL, read protocol response = INFO");
		}
		else if(RESPONSE::FAIL == ret)
		{
			AddMessage("[FBDL_4], After DL, read protocol response = FAIL");
			CloseHandle(h_file);
			return false;
		}
		else if(RESPONSE::TIMEOUT == ret)
		{
			AddMessage("[FBDL_4], After DL, read protocol response = TIMEOUT");
			CloseHandle(h_file);
			return false;
		}
	}
	AddMessage("[FBDL_4], END");
	AddMessage(" ");

	//download protocol 5--- Write target partition , flash:partition
	AddMessage("[FBDL_5], START, Write target partition");
	sprintf(sz_command, "flash:%s", sz_parmeter);
	if (m_pFastboot->Write(sz_command, strlen(sz_command), 300000)) 
	{
		while(1){
			AddMessage("[FBDL_5], read response after writing target partition");

			char strOutput[512] = {0};
			RESPONSE ret = m_pFastboot->ReadResponse_Msg(300000, strOutput);
			AddMessage(strOutput);

			if (ret == RESPONSE::OKAY) 
			{
				AddMessage("[FBDL_5] Read flash command response = OKAY" );
				break;
			}
			else if(ret == RESPONSE::INFO)
			{
				int pos = 0, iLength;
				bool bGetPartition = false;
				//char szPartition[20] = {0};
				char szBuff[200] = {0};
				CString strToken, strOutputAll;

				strOutputAll = strOutput;
				strOutputAll.Replace("\t", " ");
				iLength = strOutputAll.GetLength();
				
				for (int i = 0; i < iLength; i++)
				{
					strToken = strOutputAll.Tokenize(" ", pos);
					
					if(bGetPartition)
						break;
					
					if((pos < iLength) && (strToken.Find("INFO") == 0))
						bGetPartition = true;
					else
						bGetPartition = false;
				}

				sprintf(szBuff, "[FBDL_5] Read flash command response = INFO, partition ready = %s", strToken.GetBuffer());
				AddMessage(szBuff);
			}
			else if (ret == RESPONSE::FAIL) 
			{
				AddMessage("[FBDL_5] Read flash command response = FAIL" );
				CloseHandle(h_file);
				return false;
			}
			else if (ret == RESPONSE::TIMEOUT) 
			{
				AddMessage("[FBDL_5] Read flash command response = TIMEOUT" );
				CloseHandle(h_file);
				return false;
			}
		}
		AddMessage("[FBDL_5], read response end");
	}
	AddMessage("[FBDL_5], End");
	
	AddMessage("Partition DL PASS, wait for next partition or end");
	AddMessage(" ");

	CloseHandle(h_file);
	return true;
}

int CDownloadA9::GetComportFromUSB(USBDEVICE_STRUCT& dev)
{
	// Need to modify this function
	char tofind[]   = "(COM";
	char *pdest;
	int  port;

	pdest = strstr(dev.chFriendlyName, tofind);
	pdest += 4;
	port = atoi(pdest);
	return port;
}

void CDownloadA9::UpdateProgress(const char* chMessage, const int nStep, const int nProgress)
{
	DL_PROGRESS Msg = { (char*)chMessage, (DownloadStep)nStep, nProgress };
	Fire(EVENT_DL_PROGRESS, (long)&Msg);
}

void CDownloadA9::AddMessage(const char* chMessage, ...)
{
	va_list args;
	long totallen;
	char* buffer;
	int pos;

	va_start(args, chMessage);

	totallen = _vscprintf(chMessage, args) + 1; // 1 for '\0'
	buffer = new char [totallen];
	vsprintf(buffer, chMessage, args);

	// Replace tab with 4 spaces.
	CString cstrTemp(buffer);
	pos = cstrTemp.Find('\t');
	while (pos != -1 ) //tab
	{
		cstrTemp.Replace("\t", "    ");
		totallen += 3;
		pos = cstrTemp.Find('\t');
	}

	// Search for carriage return and send message per line
	pos = 0;
	CString resToken = cstrTemp.Tokenize("\n", pos);
	while (resToken != "")
	{
		UpdateProgress(resToken);
		resToken = cstrTemp.Tokenize("\n", pos);
	}

	delete [] buffer;
	va_end(args);
}

bool CDownloadA9::SendAdbCmd(const char *chCmd, char *chReturn, long dwReturnLen, int timeout)
{
	return MyExecAdbCmd("adb", chCmd, chReturn, dwReturnLen, timeout);
}

bool CDownloadA9::SendAcbCmd(const char *chCmd, char *chReturn, long dwReturnLen, int timeout)
{
	return MyExecAdbCmd("acb", chCmd, chReturn, dwReturnLen, timeout);
}

bool CDownloadA9::SendFastbootCmd(const char* chCmd, char *chReturn, long dwReturnLen, int timeout)
{
	return MyExecAdbCmd("fastboot", chCmd, chReturn, dwReturnLen, timeout);
}

bool CDownloadA9::MyExecAdbCmd(const char* chExeName, const char* chCmd, char *chReturn, long dwReturnLen, int timeout)
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
		si.cb = sizeof(STARTUPINFO);
		si.hStdError = hWrite;
		si.hStdOutput = hWrite;
		si.hStdInput = hRead;
		si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;

		char szCmdLine[1024];
		memset(szCmdLine, 0, 1024);

		if (strstr(chCmd, "devices") != NULL)
		{
			// Enumerate devices, do not need serial number
			_stprintf(szCmdLine, _T("\"%s\\%s\" %s"), m_strExeCmdPath.c_str(), chExeName, chCmd);
		}
		else
		{
			if (! m_strADBSerial.empty())
				_stprintf(szCmdLine, _T("\"%s\\%s\" -s %s %s"), m_strExeCmdPath.c_str(), chExeName, m_strADBSerial.c_str(), chCmd);
			else
				_stprintf(szCmdLine, _T("\"%s\\%s\" %s"), m_strExeCmdPath.c_str(), chExeName, chCmd);
		}

		if (! CreateProcess(
			NULL,
			szCmdLine,
			NULL,
			NULL,
			TRUE,
			CREATE_DEFAULT_ERROR_MODE,
			NULL,
			m_strExeCmdPath.c_str(),
			&si,
			&processInfo
			))
		{
			throw _T("CreateProcess");
		}

		WaitForSingleObject(processInfo.hProcess, timeout);

		// if needs return value
		if (chReturn != NULL)
		{
			DWORD dwAvail, numread;

			BOOL bRet = PeekNamedPipe(hRead, NULL, 0, NULL, &dwAvail, 0);
			if (bRet && dwAvail > 0)
			{
				char *pBuffer = new char [dwAvail+1];
				memset(pBuffer, 0, dwAvail+1);

				::ReadFile(hRead, pBuffer, dwAvail, &numread, 0);

				memset(chReturn, 0, dwReturnLen);

				if (dwReturnLen >= dwAvail)
					memcpy(chReturn, pBuffer, dwAvail);
				else
					memcpy(chReturn, pBuffer, dwReturnLen);

				delete [] pBuffer;
			}
		}
	}
	catch(const TCHAR* szError)
	{
		LPVOID lpMsgBuf;

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0,
			NULL
			);

		TCHAR szMessage[1024];
		_stprintf(szMessage, "%s: %s", szError, lpMsgBuf);

		LocalFree(lpMsgBuf);
		ret = false;
	}

	CloseHandle(hRead);
	CloseHandle(hWrite);
	CloseHandle(processInfo.hThread);
	processInfo.hThread = NULL;
	CloseHandle(processInfo.hProcess);
	processInfo.hProcess = NULL;

	return ret;
}

bool CDownloadA9::GetRTCTime(char* szYear)
{
	CSingleLock singleLock(&m_Mutex_Command);
	
	singleLock.Lock();

	bool bRes = false;
	char szBuff[512] = {0};
	
	if( SendAdbCmd("shell date", szBuff, 511, 10000) )
	{
		char szTmp[520] = {0};
	
		AddMessage("[RTC] Get RTC Pass");

		sprintf(szTmp, "[RTC] %s", szBuff);
		AddMessage(szTmp);

		if(strstr(szBuff, szYear) != NULL)
		{
			AddMessage("[RTC] RTC Compare Pass");
			bRes = true;
		}
		else
			AddMessage("[RTC] RTC Compare Fail");
		
	}
	else
		AddMessage("[RTC] Get RTC Fail");

	singleLock.Unlock();

	return bRes;
}

bool CDownloadA9::GetVIDPID(char* PID)
{
	bool bRet = true;
	

	std::vector<USBDEVICE_STRUCT> ADBDevs;
	FindADBdevice(ADBDevs);

	m_pFastboot =NULL;
	for (std::vector<USBDEVICE_STRUCT>::iterator iterQual = ADBDevs.begin(); iterQual != ADBDevs.end(); ++iterQual)
	{
		ADBDevice *p_adb_tmp= NULL;
		p_adb_tmp = new ADBDevice ((*iterQual).chHandleName , /*10*/128);

		std::string strFastbootSerial="";
		p_adb_tmp->GetSerialNumber(&strFastbootSerial);

		if (strFastbootSerial == m_strADBSerial )
		{
			findPID = (*iterQual).chParentName;
			strcpy(PID,findPID.c_str());
		}
		else
		{
			delete p_adb_tmp;
		}
	}
	
	return bRet;
}

bool CDownloadA9::Register(INotify_ANSI* pNotify,const std::string& strEvent)
{
	return CNotifyBase_ANSI::Register(pNotify, strEvent);
}
bool CDownloadA9::Register(EventFunc_ANSI* pEventFunc,const std::string& strEvent)
{
	return CNotifyBase_ANSI::Register(pEventFunc, strEvent);
}
bool CDownloadA9::Unregister(INotify_ANSI* pNotify,const std::string& strEvent)
{
	return CNotifyBase_ANSI::Unregister(pNotify, strEvent);
}
bool CDownloadA9::Unregister(EventFunc_ANSI* pEventFunc,const std::string& strEvent)
{
	return CNotifyBase_ANSI::Unregister(pEventFunc, strEvent);
}
void CDownloadA9::DefineNotify(const std::string& strNewEvent)
{
	CNotifyBase_ANSI::DefineNotify(strNewEvent);
}
void CDownloadA9::Fire(const std::string& strEvent,long nParam)
{
	CNotifyBase_ANSI::Fire(strEvent, nParam);
}
