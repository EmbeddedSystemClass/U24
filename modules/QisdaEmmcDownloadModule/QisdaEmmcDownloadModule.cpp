// QisdaEmmcDownloadModule.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "QisdaEmmcDownloadModule.h"
#include <Setupapi.h> //SetupDiGetClassDevs
#include <Cfgmgr32.h> //CM_Get_Parent...
//#include <Ntmsapi.h> // GetVolumeName...
//#include <stdio.h>
#include <winioctl.h>

#define DELETE_BUFFER(x)                    if(x) {delete [] x; x=NULL;}
#define IOCTL_DISK_GET_LENGTH_INFO          CTL_CODE(IOCTL_DISK_BASE, 0x0017, METHOD_BUFFERED, FILE_READ_ACCESS)
#define LODWORD(x)							((DWORD)(x))
#define HIDWORD(x)							((DWORD)(((UINT64)(x) >> 32) & 0xFFFFFFFF))
#define MBR_START							0
#define MAX_MBR_COUNT						4
#define MAX_EBR_COUNT						15
#define WRITE_PROTECTION_SIZE				131072 //64M = 128K sector
#define DEFAULT_TRANSFER					1048576 //1M
#define TRANSFER_ALIGN                      1024

#define PARTITION_XML                       "partition.xml"
#define RAWPROGRAM_XML                      "rawprogram0.xml"
#define PATCH_XML                           "patch0.xml"


/* Declaration of IOCTL_DISK_GET_PARTITION_INFO_EX */
#ifndef IOCTL_DISK_GET_PARTITION_INFO_EX
#define IOCTL_DISK_GET_PARTITION_INFO_EX CTL_CODE(IOCTL_DISK_BASE, 0x0012, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef struct _PARTITION_INFORMATION_GPT {
      GUID PartitionType;
      GUID PartitionId;
      DWORD64 Attributes;
      WCHAR Name[36];
} PARTITION_INFORMATION_GPT;

typedef enum _PARTITION_STYLE {
      PARTITION_STYLE_MBR,
      PARTITION_STYLE_GPT,
      PARTITION_STYLE_RAW
} PARTITION_STYLE;

typedef struct _PARTITION_INFORMATION_MBR {
      BYTE PartitionType;
      BOOLEAN BootIndicator;
      BOOLEAN RecognizedPartition;
      DWORD HiddenSectors;
} PARTITION_INFORMATION_MBR;

typedef struct _PARTITION_INFORMATION_EX {
      PARTITION_STYLE PartitionStyle;
      LARGE_INTEGER StartingOffset;
      LARGE_INTEGER PartitionLength;
      DWORD PartitionNumber;
      BOOLEAN RewritePartition;
      union {
            PARTITION_INFORMATION_MBR Mbr;
            PARTITION_INFORMATION_GPT Gpt;
      };
} PARTITION_INFORMATION_EX;
#endif

/* Declaration of IOCTL_DISK_GET_DRIVE_GEOMETRY_EX */
#ifndef IOCTL_DISK_GET_DRIVE_GEOMETRY_EX

#define IOCTL_DISK_GET_DRIVE_GEOMETRY_EX CTL_CODE(IOCTL_DISK_BASE, 0x0028, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef struct _DISK_GEOMETRY_EX {
      DISK_GEOMETRY Geometry;
      LARGE_INTEGER DiskSize;
      BYTE Data[1];
} DISK_GEOMETRY_EX;

#endif //IOCTL_DISK_GET_DRIVE_GEOMETRY_EX







#ifdef _DEBUG
#define new DEBUG_NEW
#endif










GUID GUIDUsbstor              = {0x4D36E967, 0xE325, 0x11CE, { 0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18}};
GUID GUIDUsb                  = {0x36FC9E60, 0xC465, 0x11CF, { 0x80, 0x56, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};
GUID GUIDUsbInterface         = {0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1f, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED}};
GUID GUIDDisk                 = {0x53f56307, 0xb6bf, 0x11D0, { 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b}};
GUID GUIDQualcommDiagUsbClass = {0x86e0d1e0, 0x8089, 0x11d0, { 0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73}};
GUID GUIDQualcommModemUsbClass= {0x2c7089aa, 0x2e0e, 0x11d1, { 0xb1, 0x14, 0x00, 0xc0, 0x4f, 0xc2, 0xaa, 0xe4}};

//GUID *pGUID = (GUID*)&GUIDDisk;
//GUID *pGUID = (GUID*)&GUIDQualcommDiagUsbClass;

/*
#define ANDROID_USB_CLASS_ID \
{0xf72fe0d4, 0xcbcb, 0x407d, {0x88, 0x14, 0x9e, 0xd6, 0x73, 0xd0, 0xdd, 0x6b}};
#define QUALCOMM_DIAG_USB_CLASS_ID \
{0x86e0d1e0L, 0x8089, 0x11d0, {0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73}};
#define QUALCOMM_MODEM_USB_CLASS_ID \
{0x2c7089aa, 0x2e0e,0x11d1,{0xb1, 0x14, 0x00, 0xc0, 0x4f, 0xc2, 0xaa, 0xe4}};


// from partition.xml
DL_STRUCT gDownloadPart[] = 
{
	{"primary1",  "c", false, "FAT"      , 1433600, "false", "fat.bin"     , 0, false},
	{"primary2", "4d", true , "DBL"      , 1000   , "true" , "dbl.mbn"     , 0, false},
	{"primary3", "46", false, "OSBL"     , 3000   , "false", "osbl.mbn"    , 0, false},
	{"primary4", "5" , false, "EXT"      , 1000000, ""     , ""            , 0, false},
	{"ext1"    , "4e", false, "EFS2"     , 130048 , "false", ""            , 0, false},
	{"ext2"    , "4c", false, "FOTA"     , 64000  , "false", ""            , 0, false},
	{"ext3"    , "4a", false, "MODEM_ST1", 6144   , "false", "dummy_fs.bin", 0, false},
	{"ext4"    , "4b", false, "MODEM_ST2", 6144   , "false", "dummy_fs.bin", 0, false},
	{"ext5"    , "58", false, "MODEM_BKP", 6144   , "false", "dummy_fs.bin", 0, false},
	{"ext6"    , "48", false, "APPS"     , 850000 , "false", "boot.img"    , 0, false},

	//EOF
	{""        , ""  , false, ""         , 0      , ""     , ""            , 0}
};

*/




// The one and only application object

CWinApp theApp;
HMODULE ghInstance;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;
	ghInstance = theApp.m_hInstance;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
	}

	return nRetCode;
}









//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CDownloadEmmc::CDownloadEmmc(int comport)
	: m_COMport (comport)
	, hEvent(NULL)
	, hVolume(NULL)
	, m_FileList(NULL)
	, SECTOR_SIZE(512)
{
	DefineNotify (EVENT_DL_PROGRESS);
	//PrimaryList = NULL;
	//ExtList = NULL;

	::AfxOleInit();
	::CoInitializeEx (NULL, COINIT_MULTITHREADED);
}

CDownloadEmmc::~CDownloadEmmc()
{
	//DELETE_BUFFER(PrimaryList);
	//DELETE_BUFFER(ExtList);
	//DELETE_BUFFER(gDownloadPart);
	DELETE_BUFFER(m_FileList);

	if (hVolume)
	{
		CloseHandle (hVolume);
		hVolume = NULL;
	}

	if (hEvent)
	{
		CloseHandle (hEvent);
		hEvent = NULL;
	}

	::CoUninitialize();
}

bool CDownloadEmmc::Register(INotify_ANSI* pNotify,const std::string& strEvent)
{
	return CNotifyBase_ANSI::Register(pNotify, strEvent);
}
bool CDownloadEmmc::Register(EventFunc_ANSI* pEventFunc,const std::string& strEvent)
{
	return CNotifyBase_ANSI::Register(pEventFunc, strEvent);
}
bool CDownloadEmmc::Unregister(INotify_ANSI* pNotify,const std::string& strEvent)
{
	return CNotifyBase_ANSI::Unregister(pNotify, strEvent);
}
bool CDownloadEmmc::Unregister(EventFunc_ANSI* pEventFunc,const std::string& strEvent)
{
	return CNotifyBase_ANSI::Unregister(pEventFunc, strEvent);
}
void CDownloadEmmc::DefineNotify(const std::string& strNewEvent)
{
	CNotifyBase_ANSI::DefineNotify(strNewEvent);
}
void CDownloadEmmc::Fire(const std::string& strEvent,long nParam)
{
	CNotifyBase_ANSI::Fire(strEvent, nParam);
}

void CDownloadEmmc::SetCOMport (const int comport)
{
	m_COMport = comport;
}

void CDownloadEmmc::SetDownloadPath (const char* fullpath)
{
	strcpy (m_DLpath, fullpath);
}

void CDownloadEmmc::SetDownloadFile (const char* chFile, const char* chType)
{
	m_ToBeDLfile[chType] = PathFindFileName (chFile);
}


bool CDownloadEmmc::IsConnected()
{
	std::vector<USBDEVICE_STRUCT>  QualDevs;
	std::vector<EMMCDEVICE_STRUCT> UsbMassDevs;


	bool bRet = FindQualcomDevice (QualDevs) &&
				FindUsbMassStorageDevice(UsbMassDevs);
	if (! bRet)
		return false;

	m_eMmcDevice.clear();

#ifdef _SDCARD_SIMULATE_
	m_eMmcDevice[m_COMport] = UsbMassDevs[0];

#else

	//match parent
	for (std::vector<USBDEVICE_STRUCT>::iterator iterQual = QualDevs.begin(); 
		iterQual != QualDevs.end(); 
		iterQual++)
	{
		for (std::vector<EMMCDEVICE_STRUCT>::iterator iterMass = UsbMassDevs.begin();
			iterMass != UsbMassDevs.end();
			iterMass++)
		{
			if (strcmp ((*iterQual).chParentName, (*iterMass).usbDev.chParentName) == 0)
			{
				int COMport = GetComportFromUSB(*iterQual);
				m_eMmcDevice[COMport] = *iterMass;
				break;
			}

		} //for iterMass

	} //for iterUSB
#endif

	std::map<int, EMMCDEVICE_STRUCT>::iterator it;
	it = m_eMmcDevice.find (m_COMport);
	bRet = it != m_eMmcDevice.end();
	if (bRet)
	{
		// if found device, print msg to UI
		AddMessage ("Found device:");
		AddMessage ("Friendly Name: %s"       , (*it).second.usbDev.chFriendlyName);
		AddMessage ("Device Path: %s"         , (*it).second.usbDev.chHandleName);
		AddMessage ("Device Inst: %s"         , (*it).second.usbDev.chInstanceName);
		AddMessage ("Device Number: %d"       , (*it).second.usbDev.dwDeviceNumber);
		AddMessage ("Device Letter: %s"       , (*it).second.driveDev.chDriveLetter);
		AddMessage ("Device Size: %I64u bytes", (*it).second.driveDev.u64DiskSize);
		AddMessage ("Device Size: %dG"        , (*it).second.driveDev.sizeG);
		AddMessage ("Block Size: %u bytes"    , (*it).second.driveDev.dwSectorSize);
		AddMessage ("eMMC Name: %s"           , (*it).second.driveDev.chEMMCname);
		AddMessage ("SerialNumber: %s"        , (*it).second.driveDev.chSerialNumber);

	}


	return bRet;
}


int CDownloadEmmc::GetComportFromUSB (USBDEVICE_STRUCT& dev)
{
	// need to modify this function! 

	char tofind[]   = "(COM";
	char *pdest;
	int  port;

	pdest = strstr (dev.chFriendlyName, tofind);
	pdest += 4;
	port = atoi (pdest);
	return port;
}

bool CDownloadEmmc::FindQualcomDevice (std::vector<USBDEVICE_STRUCT> &inUSBdevice)
{
#ifdef _SDCARD_SIMULATE_
	return true;
#endif


	GUID *pGUID = &GUIDQualcommDiagUsbClass;

	USBDEVICE_STRUCT devStruct = {0};
	BYTE chTemp [MAX_PATH];
	DWORD dwRequired;

	BYTE bTmpStringData[MAX_PATH];
	HDEVINFO hDeviceInfo;
	SP_DEVICE_INTERFACE_DATA deviceInfoData;
	SP_DEVICE_INTERFACE_DETAIL_DATA *pDeviceDetailData = (SP_DEVICE_INTERFACE_DETAIL_DATA *)bTmpStringData;
	
	SP_DEVINFO_DATA devInfo = {0};
	devInfo.cbSize = sizeof (SP_DEVINFO_DATA);

	hDeviceInfo = SetupDiGetClassDevs (pGUID, NULL, NULL, (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));
	if (INVALID_HANDLE_VALUE == hDeviceInfo)
		return false;

	bool bFound = false;

	// enum devices
	int i=0;
	while (1)
	{
		//get device interface data
		deviceInfoData.cbSize = sizeof (SP_DEVICE_INTERFACE_DATA);
		if (! SetupDiEnumDeviceInterfaces ( hDeviceInfo, 0, pGUID, i, &deviceInfoData))
			break;

		memset (&devStruct, 0, sizeof (USBDEVICE_STRUCT));

		//get device path
		pDeviceDetailData->cbSize = sizeof (SP_DEVICE_INTERFACE_DETAIL_DATA);
		if (SetupDiGetDeviceInterfaceDetail (hDeviceInfo,&deviceInfoData,pDeviceDetailData,256 ,NULL, &devInfo) &&
			(pDeviceDetailData->cbSize > 0) 
			&& (0 == _strnicmp("\\\\?\\usb", pDeviceDetailData->DevicePath, 7))) // we only care about usb device
		{
			strcpy (devStruct.chHandleName, pDeviceDetailData->DevicePath);

			// get VID PID by SetupDiGetDeviceRegistryProperty ()

		
			// Get the device instance of parent. This points to USB Composite Device
			DEVINST devInstParent;
			CM_Get_Parent (&devInstParent, devInfo.DevInst, 0);

			// Get the device ID of the USB root.
			CM_Get_Device_ID (devInstParent, (char*)chTemp, MAX_PATH, 0);
			strcpy (devStruct.chParentName, (char*)chTemp);

			// get device instance name
			if (SetupDiGetDeviceInstanceId (hDeviceInfo, &devInfo, (PSTR)chTemp, MAX_PATH, &dwRequired))
				strcpy (devStruct.chInstanceName, (char*)chTemp);

			// get device friendly name
			DWORD dataType;
			if (SetupDiGetDeviceRegistryProperty (hDeviceInfo, &devInfo, SPDRP_FRIENDLYNAME, &dataType, chTemp, MAX_PATH, &dwRequired))
				strcpy (devStruct.chFriendlyName, (char*)chTemp);

			if (SetupDiGetDeviceRegistryProperty (hDeviceInfo, &devInfo, SPDRP_DEVICEDESC, &dataType, chTemp, MAX_PATH, &dwRequired))
				strcpy (devStruct.chDeviceDescr, (char*)chTemp);


			inUSBdevice.push_back (devStruct);
			bFound = true;

		}//get device path
		++i;

	}//while (1)

	//destroy handle
	SetupDiDestroyDeviceInfoList (hDeviceInfo);
	return bFound;
}

bool CDownloadEmmc::FindUsbMassStorageDevice(std::vector<EMMCDEVICE_STRUCT> &inEMMCdevice)
{
	std::vector<USBDEVICE_STRUCT> UsbDevs;
	std::vector<DRIVEDEVICE_STRUCT> DriveDevs;

	// enum all connected disk
	bool bRet = DetectDiskDevice(UsbDevs) &&
				DetectDrives(DriveDevs);

	if (! bRet)
		return false;


	// match UsbDevs and DriveDevs
	for(std::vector<USBDEVICE_STRUCT>::iterator iterUSB = UsbDevs.begin(); 
		iterUSB != UsbDevs.end(); 
		iterUSB++)
	{
		for(std::vector<DRIVEDEVICE_STRUCT>::iterator iterDRIVE = DriveDevs.begin();
			iterDRIVE != DriveDevs.end();
			iterDRIVE++)
		{
			if ((*iterUSB).dwDeviceNumber == (*iterDRIVE).dwDeviceNumber)
			{
				EMMCDEVICE_STRUCT emmcdev = {0};
				memcpy (&emmcdev.usbDev, iterUSB._Myptr , sizeof (USBDEVICE_STRUCT));
				memcpy (&emmcdev.driveDev, &(*iterDRIVE), sizeof (DRIVEDEVICE_STRUCT));
				inEMMCdevice.push_back (emmcdev);
				break;
			}
		}
	}


	return bRet;
}

bool CDownloadEmmc::DetectDiskDevice (std::vector<USBDEVICE_STRUCT> &inUSBdevice)
{
	GUID *pGUID = &GUIDDisk;
	USBDEVICE_STRUCT devStruct = {0};
	BYTE chTemp [MAX_PATH];
	DWORD dwRequired;

	BYTE bTmpStringData[MAX_PATH];
	HDEVINFO hDeviceInfo;
	SP_DEVICE_INTERFACE_DATA deviceInfoData;
	SP_DEVICE_INTERFACE_DETAIL_DATA *pDeviceDetailData = (SP_DEVICE_INTERFACE_DETAIL_DATA *)bTmpStringData;
	
	SP_DEVINFO_DATA devInfo = {0};
	devInfo.cbSize = sizeof (SP_DEVINFO_DATA);

	hDeviceInfo = SetupDiGetClassDevs (pGUID, NULL, NULL, (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));
	if (INVALID_HANDLE_VALUE == hDeviceInfo)
		return false;

	bool bFound = false;

	// enum devices
	int i=0;
	while (1)
	{
		//get device interface data
		deviceInfoData.cbSize = sizeof (SP_DEVICE_INTERFACE_DATA);
		if (! SetupDiEnumDeviceInterfaces ( hDeviceInfo, 0, pGUID, i, &deviceInfoData))
			break;

		memset (&devStruct, 0, sizeof (USBDEVICE_STRUCT));

		//get device path
		pDeviceDetailData->cbSize = sizeof (SP_DEVICE_INTERFACE_DETAIL_DATA);
		if (SetupDiGetDeviceInterfaceDetail (hDeviceInfo,&deviceInfoData,pDeviceDetailData,256 ,NULL, &devInfo) &&
			(pDeviceDetailData->cbSize > 0) 
			&& (0 == _strnicmp("\\\\?\\usb", pDeviceDetailData->DevicePath, 7))) // we only care about usb device
		{
			strcpy (devStruct.chHandleName, pDeviceDetailData->DevicePath);
		
			// Get the device instance of parent. This points to USBSTOR (USB Mass Storage Device)
			DEVINST devInstParent;
			CM_Get_Parent (&devInstParent, devInfo.DevInst, 0);

			// Get the device instance of grand parent. This points to USB Composite Device
			CM_Get_Parent (&devInstParent, devInstParent, 0); //!!

			//// Get the device ID of the USB root.
			CM_Get_Device_ID (devInstParent, (char*)chTemp, MAX_PATH, 0);
			strcpy (devStruct.chParentName, (char*)chTemp);

			// get device instance name
			if (SetupDiGetDeviceInstanceId (hDeviceInfo, &devInfo, (PSTR)chTemp, MAX_PATH, &dwRequired))
			{
				strcpy (devStruct.chInstanceName, (char*)chTemp);
				//printf ("Device Inst = %s\n", chTemp);
			}

			// get device friendly name, assume that qualcomm eMMC always has "Qualcomm" friendly name
			DWORD dataType;
			if (SetupDiGetDeviceRegistryProperty (hDeviceInfo, &devInfo, SPDRP_FRIENDLYNAME, &dataType, chTemp, MAX_PATH, &dwRequired))
			{
				strcpy (devStruct.chFriendlyName, (char*)chTemp);
				if (strstr (devStruct.chFriendlyName, "Qualcomm") == NULL)
				{
					++i;
					continue;
				}
			}
			else
			{
				++i;
				continue;
			}

			if (SetupDiGetDeviceRegistryProperty (hDeviceInfo, &devInfo, SPDRP_DEVICEDESC, &dataType, chTemp, MAX_PATH, &dwRequired))
				strcpy (devStruct.chDeviceDescr, (char*)chTemp);

			
			// get Device Number.
			// since we need to open handle, check if the device in busy state?
			char* pName = (char*)chTemp;
			sprintf (pName, "Global\\%s_EventName", &devStruct.chHandleName[4]);
			CreateDeviceEvent ((char*)chTemp);

			if (WaitForSingleObject (hEvent, 5000) == WAIT_OBJECT_0)
			{
				ResetEvent (hEvent);

				HANDLE handle = CreateFile (devStruct.chHandleName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
				if (INVALID_HANDLE_VALUE != handle)
				{
					STORAGE_DEVICE_NUMBER sdn;
					if (DeviceIoControl (handle, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &sdn, sizeof(sdn), &dwRequired, NULL))
					{
						devStruct.dwDeviceNumber = sdn.DeviceNumber;
						inUSBdevice.push_back (devStruct);
						bFound = true;
					}

					CloseHandle (handle);

				}
				SetEvent (hEvent);
			}
			else
			{
				//???
				DWORD dwError;
				dwError = GetLastError();
			}

			//slock.Unlock();
			DestroyDeviceEvent();
			

		}//get device path
		++i;

	}//while (1)

	//destroy handle
	SetupDiDestroyDeviceInfoList (hDeviceInfo);
	return bFound;
}

bool CDownloadEmmc::DetectDrives (std::vector<DRIVEDEVICE_STRUCT> &inDriveDevice)
{
	DWORD dwError;
	DRIVEDEVICE_STRUCT devStruct = {0};
	const int bufsize = 256;
	char DriveList [bufsize];
	char chName [ 256];

	DWORD dwReturn = GetLogicalDriveStrings (bufsize, DriveList);
	if (0 == dwReturn)
	{
		dwError = GetLastError();
		return false;
	}


	bool bFound = false;
	// parse
	char* ptr = DriveList;
	char* ptrStart = ptr;
	for (int i=0; i<bufsize; ++i)
	{
		strcpy (chName, ptr);
		if (DRIVE_REMOVABLE == GetDriveType (chName))
		{
			// get DeviceNumber
			char chName2[256] = "\\\\.\\"; //"\\\\.\\X:"
			strncpy (&chName2[4], chName, 2);

			HANDLE hHandle = CreateFile (chName2, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
			if (hHandle != INVALID_HANDLE_VALUE)
			{
				STORAGE_DEVICE_NUMBER sdn;
				if (DeviceIoControl (hHandle, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &sdn, sizeof(sdn), &dwError, NULL))
				{

					//if (sdn.DeviceNumber == gDeviceNumber)
					//{
					strcpy (devStruct.chDriveLetter, chName);
					strcpy (devStruct.chHandleName, chName2);
					devStruct.dwDeviceNumber = sdn.DeviceNumber;

					// get the size
					DISK_GEOMETRY dg;
					DISK_GEOMETRY_EX dgex;
					if (DeviceIoControl(hHandle, IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, 0, 0, &dgex, sizeof(dgex), &dwError, 0))
						devStruct.u64DiskSize = dgex.DiskSize.QuadPart;

					// get the block size
					if (DeviceIoControl(hHandle, IOCTL_DISK_GET_DRIVE_GEOMETRY, 0, 0, &dg, sizeof(dg), &dwError, 0))
						devStruct.dwSectorSize = dg.BytesPerSector;


					// Query IOCTL_STORAGE_QUERY_PROPERTY
					// The following is Product ID description  of szProductID
					// BYTE[0-1]: "MM" means eMMC device.
					// BYTE[2-7]: Product name from CID. For SanDisk, it should be SEMXXG. XX indicates size. For example: SEM16G.
					// BYTE[8-15]: Product serial number from CID. For example: 3222852A.
					// The new product string is MMAAAAAABBBBBBBB, in which AAAAAA means product name and BBBBBBBB means product serial number.
					// For example: MMSEM16G3222852A

					BYTE outbuf[512];
					STORAGE_PROPERTY_QUERY Query;
					Query.PropertyId = StorageDeviceProperty;
					Query.QueryType = PropertyStandardQuery;
					
					if (DeviceIoControl (hHandle, IOCTL_STORAGE_QUERY_PROPERTY, &Query, sizeof(STORAGE_PROPERTY_QUERY), outbuf, 512, &dwError, NULL))
					{
						STORAGE_DEVICE_DESCRIPTOR *pDesc = (STORAGE_DEVICE_DESCRIPTOR*) outbuf;
						char* szProductID = (char*)(outbuf) + pDesc->ProductIdOffset;
						strncpy (devStruct.chEMMCname, szProductID, 16);

						char temp[16] = {0};
						strncpy (temp, &szProductID[5], 2);
						devStruct.sizeG = atoi (temp);

						strncpy (devStruct.chSerialNumber, &szProductID[8], 8);
					}


					inDriveDevice.push_back (devStruct);
					bFound = true;
					//}
					
				}//if (DeviceIoControl (hHandle, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &sdn, sizeof(sdn), &dwError, NULL))

				CloseHandle (hHandle);
			}
		}//if (DRIVE_REMOVABLE == GetDriveType (chName))

		while (*ptr++ != 0); // search for next string
		if (0 == *ptr)
			break;

	} //for (int i=0; i<bufsize; ++i)

	return bFound;
}

bool CDownloadEmmc::CreateDeviceEvent (const char* chName)
{
	SECURITY_DESCRIPTOR securityDesc;
	InitializeSecurityDescriptor (&securityDesc, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl (&securityDesc, TRUE, NULL, FALSE);

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof (SECURITY_ATTRIBUTES);
	sa.bInheritHandle = false;
	sa.lpSecurityDescriptor = &securityDesc;

	hEvent = CreateEvent (&sa, FALSE, TRUE, chName);
	return hEvent != NULL;
}

void CDownloadEmmc::DestroyDeviceEvent()
{
	if (hEvent)
	{
		CloseHandle (hEvent);
		hEvent = NULL;
	}
}

HANDLE CDownloadEmmc::OpenDeviceAndEvent ()
{
	HANDLE hHandle = INVALID_HANDLE_VALUE;
	char chHandleName [256];

		
	int index = m_COMport;
	strcpy (chHandleName, m_eMmcDevice[index].usbDev.chHandleName);

	char chEventName [256];
	sprintf (chEventName, "Global\\%s_EventName", &chHandleName[4]);
	CreateDeviceEvent (chEventName);

	if (WaitForSingleObject (hEvent, 5000) != WAIT_OBJECT_0)
		return hHandle;

	ResetEvent (hEvent);

	// Create volume handle and LOCK
	hVolume = CreateFile (m_eMmcDevice[index].driveDev.chHandleName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
	if (INVALID_HANDLE_VALUE == hVolume)
		return hHandle;

	DWORD dwSize;
	if (! DeviceIoControl (hVolume, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &dwSize, NULL))
	{
		CloseHandle (hVolume);
		hVolume = NULL;
		return hHandle;
	}


	// Create USB handle
	hHandle = CreateFile (chHandleName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
	
	return hHandle;
}

void CDownloadEmmc::CloseDeviceAndEvent(HANDLE hHandle)
{
	DestroyDeviceEvent();

	if (hVolume)
	{
		DWORD dwSize;
		int  index = m_COMport;
		AddMessage ("DISMOUNT volume %s", m_eMmcDevice[index].driveDev.chHandleName);
		DeviceIoControl (hVolume, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0, &dwSize, 0);
		DeviceIoControl (hVolume, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &dwSize, NULL);
		CloseHandle (hVolume);
		hVolume = NULL;
	}


	if (hHandle)
	{
		CloseHandle (hHandle);
		hHandle = NULL;
	}
}

bool CDownloadEmmc::EmmcDownload ()
{
	bool ret = true;
	int  index = m_COMport;

	UpdateProgress ("Start eMMC download...", StreamingProtocol, 0); 
	SECTOR_SIZE = m_eMmcDevice[index].driveDev.dwSectorSize;
	DISK_SIZE = m_eMmcDevice[index].driveDev.u64DiskSize;


	ret = ParsePartitionXML() &&
		  HasFilesToDownload();

	if (ret)
	{
		HANDLE hHandle = OpenDeviceAndEvent();

		int i=0;
		while (m_FileList[i].chFileName[0] != 0)
		{
			if (m_FileList[i].NeedToDL)
			{
				if ((_stricmp (m_FileList[i].chLabel, "MBR") == 0) ||
					(_stricmp (m_FileList[i].chLabel, "EXT") == 0))
				{
					DownloadMBR_EBR (hHandle, &m_FileList[i]); //need patch
				}
				else
				{
					DownloadFile (hHandle, 
								m_FileList[i].chFileName, 
								m_FileList[i].dwSectorOffset * SECTOR_SIZE,
								m_FileList[i].StartSector * SECTOR_SIZE);
				}
			}

			++i;
		}

		CloseDeviceAndEvent(hHandle);

	}//if (ret)

	DELETE_BUFFER (m_FileList);

	UpdateProgress ("eMMC download finish", StreamingProtocol, 100);
	return ret;
}


bool CDownloadEmmc::ParsePartitionXML()
{
	char        xmlfile[MAX_PATH];
	CMyMSXML    theXML;
	XMLNodeList nodeList;
	int         listsize;

	//*********************************************************************
	//                    rawprogram0.xml
	//*********************************************************************


	PathCombine (xmlfile, m_DLpath, RAWPROGRAM_XML);
	if (theXML.Load (xmlfile) != ERROR_SUCCESS)
	{
		AddMessage ("Load xml %s fail", xmlfile);
		return false;
	}

	
	CString rootname = theXML.GetNodeName(theXML.GetRoot());
	nodeList = theXML.GetChildNodes(rootname);
	if (! nodeList)
		return false;

	listsize = theXML.GetNodeListSize (nodeList);

	// allocate gDownloadPart
	m_FileList = new DownloadList [listsize + 1]; //1 for EOF
	memset (m_FileList, 0, (listsize+1)*sizeof(DownloadList));

	int j=0;
	for (int i = 0; i < listsize; ++i)
	{
		XMLNode node = theXML.GetNodeListItem (nodeList, i);
		if (NULL == node)
			continue;

		//<program file_sector_offset="0" filename="osbl.mbn" label="OSBL" num_partition_sectors="3000" physical_partition_number="0" size_in_KB="1500.0" start_sector="1434601"/>
		CString temp;
		temp = theXML.GetNodeAttrValue (node, _T("filename"));
		if (temp.IsEmpty())
			continue;

		strcpy (m_FileList[j].chFileName, temp.GetString());

		temp = theXML.GetNodeAttrValue (node, _T("file_sector_offset"));
		m_FileList[j].dwSectorOffset = atoi (temp.GetString());

		temp = theXML.GetNodeAttrValue (node, _T("label"));
		strcpy (m_FileList[j].chLabel, temp);
		
		temp = theXML.GetNodeAttrValue (node, _T("num_partition_sectors"));
		m_FileList[j].PartitionSectorNum = atoi (temp.GetString());

		temp = theXML.GetNodeAttrValue (node, _T("physical_partition_number"));
		m_FileList[j].PhysicalPartitionNum = atoi (temp.GetString());

		temp = theXML.GetNodeAttrValue (node, _T("size_in_KB"));
		m_FileList[j].size = (DWORD)(atof (temp.GetString()) * 1024);

		temp = theXML.GetNodeAttrValue (node, _T("start_sector"));
		m_FileList[j].StartSector = atoi (temp.GetString());


		// match with m_ToBeDLfile
		CString label = m_FileList[j].chLabel;
		if (! m_ToBeDLfile [label].IsEmpty())
			m_FileList[j].NeedToDL = true;

		// MBR / EXT always need to DL
		if ((label.CompareNoCase ("MBR") == 0) ||
			(label.CompareNoCase ("EXT") == 0))
			m_FileList[j].NeedToDL = true;

		//0x00000001 = file start+offset [734003200 bytes], name fat.bin [starting block 0]
		AddMessage ("0x%016I64x = file start+offset [%u bytes], name %s [starting block %I64u]", 
					m_FileList[j].StartSector,
					m_FileList[j].size,
					m_FileList[j].chFileName,
					m_FileList[j].dwSectorOffset);

		++j;

	}//for (int i = 0; i < listsize; ++i)

	return true;
}

bool CDownloadEmmc::HasFilesToDownload()
{
	//gDownloadPart
	int i=0;
	while (m_FileList[i].chFileName[0] != 0) //while not EOF
	{
		if (m_FileList[i].NeedToDL)
		{
			if (_stricmp (m_FileList[i].chLabel, "MBR") != 0 ||
				_stricmp (m_FileList[i].chLabel, "EXT") != 0)
			return true;
		}
	}

	return false;
}

bool CDownloadEmmc::DownloadMBR_EBR (HANDLE hDevice, DownloadList *pDLstruct)
{
	HANDLE hFile;
	BYTE   *pMBR = NULL;
	BOOL   bRet;
	DWORD  dwActual;
	UINT64 dwAddr;



	// allocate buffer
	pMBR = new BYTE [pDLstruct->size];
	if (! pMBR)
	{
		AddMessageWithLastError ("Allocate memory fail in MBR_EBR_ReadAndPatch");
		return false;
	}
	
	// open MBR / EBR file and read to pMBR
	char fullname[MAX_PATH];
	PathCombine (fullname, m_DLpath, pDLstruct->chFileName);
	hFile = CreateFile (fullname, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		AddMessageWithLastError ("Open file \"%s\" fail in MBR_EBR_ReadAndPatch", fullname);
		DELETE_BUFFER (pMBR);
		return false;
	}

	dwAddr = pDLstruct->dwSectorOffset * SECTOR_SIZE;
	DWORD dwLow  = LODWORD (dwAddr);
	DWORD dwHigh = HIDWORD (dwAddr);
	SetFilePointer (hFile, dwLow, (PLONG)&dwHigh, FILE_BEGIN);
	bRet = ReadFile (hFile, pMBR, pDLstruct->size, &dwActual, NULL) && (pDLstruct->size==dwActual);
	CloseHandle (hFile);
	if (! bRet)
	{
		AddMessageWithLastError ("Read file \"%s\" fail in MBR_EBR_ReadAndPatch", fullname);
		DELETE_BUFFER (pMBR);
		return false;
	}


	// check "patch0.xml"
	PatchMBR_EBR (pDLstruct, pMBR);


	//
	// write to device
	//

	// Move device offset
	dwAddr = pDLstruct->StartSector * SECTOR_SIZE;
	dwLow  = LODWORD (dwAddr);
	dwHigh = HIDWORD (dwAddr);
	if (SetFilePointer (hDevice, dwLow, (PLONG)&dwHigh, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		DWORD dwError = GetLastError();
		if (dwError != NO_ERROR)
		{
			AddMessage ("SetFilePointer hDevice fail, LastError=%d", dwError);
			DELETE_BUFFER (pMBR);
			return false;
		}
	}

	bRet = WriteFile (hDevice, pMBR, pDLstruct->size, &dwActual, NULL) && (pDLstruct->size == dwActual);
	if (! bRet)
		AddMessageWithLastError ("WriteFile hDevice fail in DownloadMBR_EBR");


	DELETE_BUFFER (pMBR);
	return TRUE == bRet;
}


void CDownloadEmmc::PatchMBR_EBR (const DownloadList *pDLstruct, BYTE* pBuffer)
{
	CMyMSXML    theXML;
	XMLNodeList nodeList;
	long        byteOffset;
	long        patchSectorOffset;
	DWORD       size;
	bool        found;
	char        szWhat[512];
	char        szValue[512];


	char xmlfile[MAX_PATH];
	PathCombine (xmlfile, m_DLpath, PATCH_XML);
	if (theXML.Load (xmlfile) != ERROR_SUCCESS)
	{
		AddMessage ("Load patch xml \"%s\" fail", xmlfile);
		return;
	}

	CString rootname = theXML.GetNodeName(theXML.GetRoot());
	nodeList = theXML.GetChildNodes (rootname);
	if (! nodeList)
		return;

	found =false;
	size = 0;

	int listsize = theXML.GetNodeListSize (nodeList);
	for (int i = 0; i < listsize; ++i)
	{
		XMLNode node = theXML.GetNodeListItem (nodeList, i);
		if (NULL == node)
			continue;

		size = 0;

		//<patch byte_offset="506" filename="partition0.bin" physical_partition_number="0" size_in_bytes="4" start_sector="0" value="NUM_DISK_SECTORS-1437601." what="Update MBR with the length of the EXT Partition."/>
		//<patch byte_offset="458" filename="partition0.bin" physical_partition_number="0" size_in_bytes="4" start_sector="6" value="NUM_DISK_SECTORS-1654272." what="Update final partition with actual size."/>		
		
		CString strfile = theXML.GetNodeAttrValue (node, _T("filename"));
		if (strfile.CompareNoCase (pDLstruct->chFileName) == 0)
		{
			CString temp;
			temp = theXML.GetNodeAttrValue (node, _T("byte_offset"));
			byteOffset = atoi(temp.GetString());

			temp = theXML.GetNodeAttrValue (node, _T("size_in_bytes"));
			size = atoi(temp.GetString());

			temp = theXML.GetNodeAttrValue (node, _T("start_sector"));
			patchSectorOffset = atoi(temp.GetString());

			temp = theXML.GetNodeAttrValue (node, _T("value"));
			strcpy (szValue, temp.GetString());

			temp = theXML.GetNodeAttrValue (node, _T("what"));
			strcpy (szWhat, temp.GetString());


			// hardcode to determine MBR or EBR
			if (_stricmp (pDLstruct->chLabel, "MBR") ==0)
			{
				if (strstr (szWhat, "MBR") != NULL)
				{
					found = true;
					break;
				}
			}
			else if (_stricmp (pDLstruct->chLabel, "EXT") ==0)
			{
				if (strstr (szWhat, "MBR") == NULL)
				{
					found = true;
					break;
				}
			}

		}

	}//for (int i = 0; i < listsize; ++i)


	if (!found || 0==size)
		return;

	// check!
	if (patchSectorOffset < pDLstruct->dwSectorOffset)
		return; 


	if (4 == size)
	{
		int pos = strcspn (szValue, "0123456789");
		DWORD patch_value = atoi(&szValue[pos]);
		DWORD totalsector = (DWORD)(DISK_SIZE / SECTOR_SIZE);

		BYTE  *pPatch = pBuffer + (patchSectorOffset - pDLstruct->dwSectorOffset)*SECTOR_SIZE + byteOffset;
		DWORD *dwpatchVal = (DWORD*)pPatch;

		*dwpatchVal = totalsector - patch_value;
		AddMessage ("%s = %d", szWhat, *dwpatchVal);
	}
	else
	{
		AddMessage ("Undefined patch size!");
	}

}

bool CDownloadEmmc::DownloadFile (HANDLE hDevice, const char* chFileName, const UINT64 dwOffset, const UINT64 sectorOffset)
{
	UINT64 dwAddr;
	UINT64 TotalBytes, HasTransferBytes;
	DWORD  dwLow, dwHigh;
	int    percent, prevPercent = 0;
	char   chFile [256];
	BOOL   bRet;

	AddMessage ("Writing file \"%s\" to file partition at %016x", chFileName, sectorOffset);

	// allocate memory
	BYTE *pBuffer = new BYTE [DEFAULT_TRANSFER];
	if (! pBuffer)
	{
		AddMessage ("allocate memory pBuffer fail");
		return false;
	}

	// open the file
	PathCombine (chFile, m_DLpath, chFileName);
	HANDLE hFile = CreateFile (chFile, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		AddMessageWithLastError ("Create file %s fail", chFile);
		delete [] pBuffer;
		return false;
	}

	// move file offset
	dwLow  = LODWORD (dwOffset);
	dwHigh = HIDWORD (dwOffset);
	if (INVALID_SET_FILE_POINTER == SetFilePointer (hFile, dwLow, (PLONG)&dwHigh, FILE_BEGIN))
	{
		AddMessageWithLastError ("SetFilePointer hDevice dwLow=%u dwHigh=%u fail", dwLow, dwHigh);
		CloseHandle (hFile);
		delete [] pBuffer;
		return false;
	}


	// get file size
	DWORD dwSize, dwTransfer, dwActual, dwError;
	dwSize = GetFileSize (hFile, &dwHigh);
	TotalBytes = ((UINT64)dwHigh << 16) + dwSize;
	TotalBytes -= dwOffset;
	AddMessage ("File size = %u", TotalBytes);


	if ((TotalBytes % TRANSFER_ALIGN) != 0)
	{
		TotalBytes = (TotalBytes + TRANSFER_ALIGN) / TRANSFER_ALIGN * TRANSFER_ALIGN;
		AddMessage ("Rounded up to %d", TotalBytes);
	}



	dwAddr = sectorOffset;
	dwTransfer = DEFAULT_TRANSFER;
	HasTransferBytes = 0;

	// transfer the file to device
	while (dwSize)
	{
		memset (pBuffer, 0, DEFAULT_TRANSFER);
		if (dwTransfer > dwSize)
			dwTransfer = dwSize;

		// read from file
		bRet =	ReadFile (hFile, pBuffer, dwTransfer, &dwActual, 0) &&
				(dwTransfer == dwActual);
		if (!bRet)
		{
			AddMessageWithLastError ("ReadFile %s fail", chFileName);
			break;
		}


		// the remaining bytes
		if (dwTransfer < DEFAULT_TRANSFER)
		{
			dwTransfer = (DWORD)(TotalBytes - HasTransferBytes);
			dwSize = dwTransfer; // to end loop
		}
		//printf ("WriteFile position %016llx, size %u\n", dwAddr, dwTransfer);

		// Move device offset
		dwLow  = LODWORD(dwAddr);
		dwHigh = HIDWORD(dwAddr);
		if (SetFilePointer (hDevice, dwLow, (PLONG)&dwHigh, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{
			dwError = GetLastError();
			if (dwError != NO_ERROR)
			{
				bRet = FALSE;
				AddMessage ("SetFilePointer hDevice fail, LastError=%d", dwError);
				break;
			}
		}


		// write to device
		bRet = WriteFile (hDevice, pBuffer, dwTransfer, &dwActual, 0) && (dwActual==dwTransfer);
		if (! bRet)
		{
			AddMessageWithLastError ("WriteFile hDevice");
			break;
		}

		dwAddr += dwActual;
		HasTransferBytes += dwActual;
		percent = (int)(100 * HasTransferBytes / TotalBytes);
		if (percent > 100)
			percent = 100;

		if (percent > prevPercent) // to prevent print more than once
		{
			prevPercent = percent;
			char chMsg[512];
			wsprintf (chMsg, "Downloading %s... ", chFileName);
			UpdateProgress (chMsg, StreamingProtocol, percent);
		}

		Sleep(0);
		dwSize -= dwActual;
	}

	delete [] pBuffer;
	CloseHandle (hFile);
	//printf ("File write complete\n");
	return TRUE==bRet;
}

void CDownloadEmmc::UpdateProgress (const char* chMessage, const int nStep, const int nProgress)
{
	DL_PROGRESS Msg = {(char*)chMessage, (DownloadStep)nStep, nProgress};
	Fire (EVENT_DL_PROGRESS, (long)&Msg);
}

void CDownloadEmmc::AddMessage (const char* chMessage, ...)
{
	va_list args;
	int     len;
	char    *buffer;

	va_start (args, chMessage);

	len = _vscprintf (chMessage, args) + 1; // 1 for '\0'
	buffer = new char [len];
	vsprintf (buffer, chMessage, args);
	UpdateProgress (buffer);
	delete [] buffer;

	va_end (args);
}

void CDownloadEmmc::AddMessageWithLastError (const char* chMessage, ...)
{
	va_list args;
	int     len;
	char    *buffer;
	LPVOID  lpMsgBuf;

    DWORD   dw = GetLastError(); 
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

	va_start (args, chMessage);

	len = _vscprintf (chMessage, args) + 1; // 1 for '\0'
	buffer = new char [len];
	vsprintf (buffer, chMessage, args);
	va_end (args);

	char showmsg[512];
	wsprintf (showmsg, "%s, LastError = %d: %s", buffer, dw, lpMsgBuf);
	UpdateProgress (showmsg);

	delete [] buffer;
	LocalFree (lpMsgBuf);
}



/*
//-------------------------------------------------------------------------
// Work for Waterford EVB1 calculation
//-------------------------------------------------------------------------

bool CDownloadEmmc::IsConnected()
{
	std::vector<USBDEVICE_STRUCT> UsbDevs;
	std::vector<DRIVEDEVICE_STRUCT> DriveDevs;
	int index = 0;

	UpdateProgress ("Search for eMMC devices...", Connection, 0);


	bool bRet = DetectDiskDevice(UsbDevs) && 
				DetectDrives(DriveDevs);

	if (! bRet)
	{
		UpdateProgress ("Search for eMMC devices fail", Connection, 100);
		return false;
	}

	// match UsbDevs and DriveDevs
	for(std::vector<USBDEVICE_STRUCT>::iterator iterUSB = UsbDevs.begin(); 
		iterUSB != UsbDevs.end(); 
		iterUSB++)
	{
		for(std::vector<DRIVEDEVICE_STRUCT>::iterator iterDRIVE = DriveDevs.begin();
			iterDRIVE != DriveDevs.end();
			iterDRIVE++)
		{
			if ((*iterUSB).dwDeviceNumber == (*iterDRIVE).dwDeviceNumber)
			{
				EMMCDEVICE_STRUCT emmcdev = {0};
				memcpy (&emmcdev.usbDev, iterUSB._Myptr , sizeof (USBDEVICE_STRUCT));
				memcpy (&emmcdev.driveDev, &(*iterDRIVE), sizeof (DRIVEDEVICE_STRUCT));
				m_eMmcDevice[index++] = emmcdev;

				AddMessage ("Found device:");
				AddMessage ("Friendly Name: %s", emmcdev.usbDev.chFriendlyName);
				AddMessage ("Device Path: %s", emmcdev.usbDev.chHandleName);
				AddMessage ("Device Inst: %s", emmcdev.usbDev.chInstanceName);
				AddMessage ("Device Number: %d", emmcdev.usbDev.dwDeviceNumber);
				AddMessage ("Device Letter: %s", emmcdev.driveDev.chDriveLetter);
				AddMessage ("Device Size: %I64u bytes", emmcdev.driveDev.u64DiskSize);
				AddMessage ("Block Size: %u bytes", emmcdev.driveDev.dwSectorSize);
			}
		}
	}

	UpdateProgress ("Search for devices...", Connection, 100);


	// temp solution...
	return m_eMmcDevice.size() > 0;
}

bool CDownloadEmmc::EmmcDownload ()
{
	bool ret = true;
	BYTE* pMBR = NULL;
	BYTE* pEBR = NULL;
	DWORD dwMBRsize, dwEBRsize;
	DWORD dwSize;
	UINT64 dwTableOffset;
	char chMsg[512];

	UpdateProgress ("Start eMMC download...", StreamingProtocol, 0); 


	int index = m_COMport;
	DWORD SECTOR_SIZE = m_eMmcDevice[index].driveDev.dwSectorSize;
	if (0 == SECTOR_SIZE) //???
		SECTOR_SIZE = 512;


	//calculate MBR
	dwTableOffset = CalculateMBR (index, &pMBR, &dwMBRsize);

	//calculate EBR
	ret = CalculateEBR (index, &pEBR, &dwEBRsize, dwTableOffset);

	if (ret)
	{
		DetermineFileToDownload();

		//
		//prepare to write
		//
		char chHandleName [256];
		strcpy (chHandleName, m_eMmcDevice[index].usbDev.chHandleName);

		char chEventName [256];
		sprintf (chEventName, "Global\\%s_EventName", &chHandleName[4]);
		CreateDeviceEvent (chEventName);

		if (WaitForSingleObject (hEvent, 5000) == WAIT_OBJECT_0)
		{
			ResetEvent (hEvent);


			//HANDLE hDisk = CreateFile (m_eMmcDevice[index].driveDev.chHandleName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
			//ret = DeviceIoControl (hDisk, FSCTL_LOCK_VOLUME, 0, 0, 0, 0, &dwSize, 0);


			HANDLE hHandle = CreateFile (chHandleName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
			if (hHandle != INVALID_HANDLE_VALUE)
			{
				//ret = DeviceIoControl (hHandle, FSCTL_LOCK_VOLUME, 0, 0, 0, 0, &dwSize, 0)

				// 1. write MBR
				if (ret)
				{
					UpdateProgress ("Write MBR", StreamingProtocol, 0); 
					SetFilePointer (hHandle, 0, 0, FILE_BEGIN);
					ret = WriteFile (hHandle, pMBR, dwMBRsize, &dwSize, 0)==TRUE;
					
					wsprintf (chMsg, "Write MBR %s", ret? "success" : "fail");
					UpdateProgress (chMsg, StreamingProtocol, 100);
				}

				// 2. write primary partition file
				if (ret)
				{
					int i=0;
					while (PrimaryList[i].chFileName[0] != 0)
					{
						UINT64 dwOffset = PrimaryList[i].dwSectorOffset * SECTOR_SIZE;
						
						AddMessage ("Writing file \"%s\" to file partition at %016x\n", PrimaryList[i].chFileName, dwOffset);

						char chFile[MAX_PATH];
						sprintf (chFile, "%s%s", m_DLpath, PrimaryList[i].chFileName);
						if (! DownloadFile (hHandle, chFile, dwOffset))
						{
							//printf ("Download file %s fail!!\n", chFile);
							ret = false;
							break;
						}
						
						++i;
					}
				}

				// 3. Write EBR
				if (ret)
				{
					UpdateProgress ("Write EBR", StreamingProtocol, 0); 

					UINT64 dwOffset = dwTableOffset * SECTOR_SIZE;
					DWORD dwLow = LODWORD(dwOffset);
					DWORD dwHigh= HIDWORD(dwOffset);
					if (SetFilePointer (hHandle, dwLow, (PLONG)&dwHigh, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
					{
						DWORD dwError = GetLastError();
						if (dwError != NO_ERROR)
						{
							AddMessage ("SetFilePointer fail with error: %d", dwError);
							ret = false;
						}
					}

					if (ret)
						ret = WriteFile (hHandle, pEBR, dwEBRsize, &dwSize, 0)==TRUE;

					if (! ret)
						AddMessage ("Write EBR fail!");

					UpdateProgress ("Write EBR", StreamingProtocol, 100);
				}

				// 4. write extention partition file
				if (ret)
				{

					int EBRcount = dwEBRsize / 512;
					for (int i=0; i<EBRcount; ++i)
					{
						if (ExtList[i].chFileName[0] != 0)
						{
							UINT64 dwOffset = ExtList[i].dwSectorOffset * SECTOR_SIZE;
							AddMessage ("Writing file \"%s\" to file partition at %016x\n", ExtList[i].chFileName, dwOffset);

							char chFile[MAX_PATH];
							sprintf (chFile, "%s%s", m_DLpath, ExtList[i].chFileName);
							if (! DownloadFile (hHandle, chFile, dwOffset))
							{
								//printf ("Download file %s fail!!\n", chFile);
								ret = false;
								break;
							}
							
						}
					}

				} //if (ret)


				//DeviceIoControl (hHandle, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0, &dwSize, 0);
				CloseHandle (hHandle);

			} //if (hHandle != INVALID_HANDLE_VALUE)

			SetEvent (hEvent);

		}
		else
		{
			//cannot get lock
			ret = false;
		}

		DestroyDeviceEvent();

	} //if (ret)

	if (pMBR)
		delete [] pMBR;
	if (pEBR)
		delete [] pEBR;

	if (PrimaryList)
	{
		delete [] PrimaryList;
		PrimaryList = NULL;
	}

	if (ExtList)
	{
		delete [] ExtList;
		ExtList = NULL;
	}

	UpdateProgress ("eMMC download finish", StreamingProtocol, 100);
	return ret;
}

UINT64 CDownloadEmmc::CalculateMBR (int devIndex, BYTE** pBuffer, DWORD* dwSize)
{
	*dwSize = 512;
	*pBuffer = (BYTE*) new MBR_STRUCT;
	MBR_STRUCT* pMBR = (MBR_STRUCT*)*pBuffer;
	memset (pMBR, 0, sizeof(MBR_STRUCT));
	
	PrimaryList = new DownloadList [MAX_MBR_COUNT];
	memset (PrimaryList, 0, MAX_MBR_COUNT * sizeof(DownloadList));

	//
	// Start calculating primary partition and create MBR
	//

	UINT64 dwSectOffset = MBR_START;
	//printf ("Begin MBR at %016llx\n", dwSectOffset);

	dwSectOffset += 1; // MBR itself occupies 512byte (1 sector size)

	// search primary table
	char chKey[32], *stopchar;
	int index;
	UINT64 SumOfPrimarySize = 0; // remaining size for EBR = Total - sum of primary

	for (int i=0; i<MAX_MBR_COUNT; ++i) // max 4 primary table
	{
		sprintf (chKey, "primary%d", i+1);
		index = gDownloadPartSearchIndex ("order", chKey);
		if (-1 == index)
			break;

		pMBR->partition[i].LBA_FirstSector = (DWORD) dwSectOffset;	//LBA of first absolute sector in the partition
		if (gDownloadPart[index].bootable)
			pMBR->partition[i].bootable |= 0x80;
		pMBR->partition[i].partitionType = (BYTE) strtoul (gDownloadPart[index].type, &stopchar, 16);
		//gDownloadPart[i].offset = dwAddrOffset;

		UINT64 size = gDownloadPart[index].size;
		UINT64 DiskSize = m_eMmcDevice[devIndex].driveDev.u64DiskSize;
		DWORD SECTOR_SIZE = m_eMmcDevice[devIndex].driveDev.dwSectorSize;

		if (_strnicmp (gDownloadPart[index].label, "EXT", 3) == 0)
		{
			size = (DiskSize / SECTOR_SIZE) - 1 - SumOfPrimarySize; // 1 = MBR itself
		}
		else
		{
			SumOfPrimarySize += gDownloadPart[index].size;
			dwSectOffset += gDownloadPart[index].size;

			strcpy (PrimaryList[i].chFileName, gDownloadPart[index].filename);
			PrimaryList[i].dwSectorOffset = pMBR->partition[i].LBA_FirstSector;
			PrimaryList[i].size = gDownloadPart[index].size;

			//match with m_ToBeDLfile
			CString label = gDownloadPart[index].label;
			CString temp = m_ToBeDLfile [label];
			if (temp.IsEmpty())
			{
				PrimaryList[i].chFileName[0] = 0; //do not download
			}
			else
			{
				// use user input file name
				strcpy (PrimaryList[i].chFileName, m_ToBeDLfile[label].GetBuffer(256));
				m_ToBeDLfile[label].ReleaseBuffer();
			}


		}
		pMBR->partition[i].NumberOfSector = (DWORD) size;

		//prtn 0 at 0x00000000000001be: bootable 0x00, type 0x0c, 1st sect 0x00000001, size 0x0015e000
		//wsprintf (chMsg, "prtn %d at %016I64x: bootable 0x%02x, type 0x%02x, 1st sect 0x%08x, size 0x%08x\n",
		char chMsg[512] = {0};
		wsprintf (chMsg, "prtn %d at %016x: bootable 0x%02x, type 0x%02x, 1st sect 0x%08x, size 0x%08x\n",
			i, 
			(UINT64)&pMBR->partition[i] - (UINT64)pMBR, 
			pMBR->partition[i].bootable, 
			pMBR->partition[i].partitionType,
			pMBR->partition[i].LBA_FirstSector,
			pMBR->partition[i].NumberOfSector); //size); // in sector unit
		AddMessage (chMsg);

	}

	pMBR->MbrSignature = 0xaa55;
	pMBR->DiskSignature = 0xfffffffe;// ???

	//print signature message
	//BYTE* pSign = (BYTE*)(&pMBR->MbrSignature);
	//printf ("MBR disk signature 0x%08x, signature bytes 0x%x 0x%x\n", pMBR->DiskSignature, *pSign, *(pSign + 1));


	return dwSectOffset;

}

bool CDownloadEmmc::CalculateEBR (int devIndex, BYTE** pBuffer, DWORD* dwSize, const UINT64 dwOffset)
{
	int i, index, extListIndex, EBRcount;
	UINT64 dataOffset;
	char chKey[32], *stopchar;

	// how many EBRpartition 
	i=0;
	EBRcount = 0;
	while (gDownloadPart[i].order[0] != 0)
	{
		if (_strnicmp (gDownloadPart[i].order, "ext", 3) == 0)
			EBRcount++;
		++i;
	}

	// allocate memory
	*dwSize = EBRcount * sizeof (MBR_STRUCT);
	*pBuffer = new BYTE [*dwSize];
	MBR_STRUCT *pEBR = (MBR_STRUCT*)(*pBuffer);
	memset (pEBR, 0, *dwSize);


	// calculate first offset of EBR data = (offset + EBR) aligned to 64M??
	UINT64 DataStart = dwOffset + (*dwSize);
	DataStart = (DataStart + WRITE_PROTECTION_SIZE) / WRITE_PROTECTION_SIZE * WRITE_PROTECTION_SIZE; //align
	dataOffset = DataStart;

	// generate gExtList and calculate its offset / size
	ExtList = new DownloadList [EBRcount];
	memset (ExtList, 0, EBRcount * sizeof(DownloadList));

	i=0;
	extListIndex = 0;
	for (int i=0; i<EBRcount; ++i)
	{
		sprintf (chKey, "ext%d", i+1);
		index = gDownloadPartSearchIndex ("order", chKey);
		if (-1 == index)
			continue;

		strcpy (ExtList [extListIndex].chFileName, gDownloadPart[index].filename);
		ExtList[extListIndex].dwSectorOffset = (DWORD)dataOffset;
		ExtList[extListIndex].size = gDownloadPart[index].size;

		//match with m_ToBeDLfile
		CString label = gDownloadPart[index].label;
		CString temp = m_ToBeDLfile [label];
		if (temp.IsEmpty())
		{
			ExtList[extListIndex].chFileName[0] = 0; //do not download
		}
		else
		{
			// use user input file name
			strcpy (ExtList[extListIndex].chFileName, m_ToBeDLfile[label].GetBuffer(256)); 
			m_ToBeDLfile[label].ReleaseBuffer();
		}


		// update
		dataOffset += gDownloadPart[index].size;
		dataOffset = (dataOffset + WRITE_PROTECTION_SIZE) / WRITE_PROTECTION_SIZE * WRITE_PROTECTION_SIZE; //align
		
		extListIndex++;
	}

	// fill the EBR structure
	for (i=0; i<EBRcount; ++i)
	{
		sprintf (chKey, "ext%d", i+1);
		index = gDownloadPartSearchIndex ("order", chKey);
		if (-1 == index)
			continue;

		//
		// 1st partition.
		//
		pEBR[i].partition[0].LBA_FirstSector = (DWORD)(ExtList[i].dwSectorOffset - dwOffset - i);
		pEBR[i].partition[0].NumberOfSector = gDownloadPart[index].size;
		pEBR[i].partition[0].partitionType = (BYTE) strtoul (gDownloadPart[index].type, &stopchar, 16);
		if (gDownloadPart[index].bootable)
			pEBR[i].partition[0].bootable |= 0x80;

		//
		// 2nd partition
		//
		UINT64 DiskSize = m_eMmcDevice[devIndex].driveDev.u64DiskSize;
		DWORD  SECTOR_SIZE = m_eMmcDevice[devIndex].driveDev.dwSectorSize;

		if (EBRcount-1 == i) //last EBR
		{
			// last EBR size = total remaining size
			pEBR[i].partition[0].NumberOfSector = (DWORD)((DiskSize / SECTOR_SIZE) - ExtList[i].dwSectorOffset);

			// previous 2nd partition also needs update
			pEBR[i-1].partition[1].NumberOfSector = pEBR[i].partition[0].LBA_FirstSector + pEBR[i].partition[0].NumberOfSector;
			
			// 2nd partition fill with 0
		}
		else
		{
			pEBR[i].partition[1].LBA_FirstSector = i+1;
			pEBR[i].partition[1].NumberOfSector = (DWORD)(ExtList[i+1].dwSectorOffset - dwOffset - (i+1)) + ExtList[i+1].size;
			pEBR[i].partition[1].partitionType = 5; //EXT
			if (gDownloadPart[index+1].bootable)
				pEBR[i].partition[1].bootable |= 0x80;

		}

		// signature
		pEBR[i].MbrSignature = 0xaa55;
	}

	//// print info to screen
	//UINT64 dwSectOffset = dwOffset;
	//for (i=0; i<EBRcount; ++i)
	//{
	//	printf ("Begin EBR at %016llx\n", dwSectOffset * SECTOR_SIZE);

	//	//prtn 0 at 0x000000002bdf43be: bootable 0x00, type 0x4e, 1st sect 0x0000105f, size 0x0001fc00
	//	printf ("prtn 0 at %016llx: bootable 0x%02x, type 0x%02x, 1st sect 0x%08x, size 0x%08x\n",
	//		(UINT64)&pEBR[i].partition[0] - (UINT64)&pEBR[0] + (dwOffset * SECTOR_SIZE),
	//		pEBR[i].partition[0].bootable, 
	//		pEBR[i].partition[0].partitionType,
	//		pEBR[i].partition[0].LBA_FirstSector,
	//		pEBR[i].partition[0].NumberOfSector); // in sector unit

	//	//prtn 1 at 0x000000002bdf43ce: bootable 0x00, type 0x05, 1st sect 0x00000001, size 0x00030a5e
	//	printf ("prtn 1 at %016llx: bootable 0x%02x, type 0x%02x, 1st sect 0x%08x, size 0x%08x\n",
	//		(UINT64)&pEBR[i].partition[1] - (UINT64)&pEBR[0] + (dwOffset * SECTOR_SIZE),
	//		pEBR[i].partition[1].bootable, 
	//		pEBR[i].partition[1].partitionType,
	//		pEBR[i].partition[1].LBA_FirstSector,
	//		pEBR[i].partition[1].NumberOfSector); // in sector unit

	//	BYTE* pSign = (BYTE*)(&pEBR[i].MbrSignature);
	//	printf ("EBR signature bytes 0x%x 0x%x\n", *pSign, *(pSign + 1));


	//	dwSectOffset++; // 1 EBR = 1 sector
	//}

	return true;
}

bool CDownloadEmmc::HasFilesToDownload(DWORD dwEBRsize)
{
	// Check Primary List
	for (int i=0; i<MAX_MBR_COUNT; ++i)
	{
		if (PrimaryList[i].chFileName[0] != 0)
		{
			return true;
		}
	}

	// Check ExtendedList
	int EBRcount = dwEBRsize / 512;
	for (int i=0; i<EBRcount; ++i)
	{
		if (ExtList[i].chFileName[0] != 0)
			return true;
	}

	return false;
}

void CDownloadEmmc::GetFileNameOnly (const char* fullpath, char* chBuffer, DWORD dwSize)
{
	size_t len = strlen (fullpath);
	int i = (int)(len-1);
	
	while (fullpath[--i] != '\\');
	strcpy (chBuffer, &fullpath[++i]);
}


bool CDownloadEmmc::WriteMBR (HANDLE hHandle, BYTE* pMBR, DWORD dwMBRsize)
{
	DWORD dwSize;
	char chMsg[512];

	UpdateProgress ("Write MBR", StreamingProtocol, 0);

	SetFilePointer (hHandle, 0, 0, FILE_BEGIN);
	BOOL ret = WriteFile (hHandle, pMBR, dwMBRsize, &dwSize, 0);

	wsprintf (chMsg, "Write MBR %s", ret? "success" : "fail");
	UpdateProgress (chMsg, StreamingProtocol, 100);

	return TRUE==ret;
}


bool CDownloadEmmc::WriteEBR (HANDLE hHandle, UINT64 dwOffset, BYTE* pEBR, DWORD dwEBRsize)
{
	UpdateProgress ("Write EBR", StreamingProtocol, 0);

	DWORD dwSize;
	DWORD dwLow = LODWORD(dwOffset);
	DWORD dwHigh= HIDWORD(dwOffset);

	if (SetFilePointer (hHandle, dwLow, (PLONG)&dwHigh, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		DWORD dwError = GetLastError();
		if (dwError != NO_ERROR)
		{
			AddMessage ("SetFilePointer fail with error: %d", dwError);
			AddMessage ("Write EBR fail!");
			return false;
		}
	}

	if (! WriteFile (hHandle, pEBR, dwEBRsize, &dwSize, 0))
	{
		AddMessage ("Write EBR fail!");
		return false;
	}

	UpdateProgress ("Write EBR", StreamingProtocol, 100);
	return true;
}

bool CDownloadEmmc::WritePrimaryPartitionFiles (HANDLE hHandle, int index)
{
	DWORD SECTOR_SIZE = m_eMmcDevice[index].driveDev.dwSectorSize;
	if (0 == SECTOR_SIZE) //???
		SECTOR_SIZE = 512;

	int i=0;
	while (PrimaryList[i].chFileName[0] != 0)
	{
		UINT64 dwOffset = PrimaryList[i].dwSectorOffset * SECTOR_SIZE;
		AddMessage ("Writing file \"%s\" to file partition at %016x\n", PrimaryList[i].chFileName, dwOffset);

		char chFile[MAX_PATH];
		sprintf (chFile, "%s%s", m_DLpath, PrimaryList[i].chFileName);
		if (! DownloadFile (hHandle, chFile, dwOffset))
		{
			AddMessage ("Download file %s fail!!\n", chFile);
			return false;
		}
		++i;
	}

	return true;
}


bool CDownloadEmmc::WriteExtendedPartitionFiles (HANDLE hHandle, int index, DWORD dwEBRsize)
{
	DWORD SECTOR_SIZE = m_eMmcDevice[index].driveDev.dwSectorSize;
	if (0 == SECTOR_SIZE) //???
		SECTOR_SIZE = 512;

	int EBRcount = dwEBRsize / 512;
	for (int i=0; i<EBRcount; ++i)
	{
		if (ExtList[i].chFileName[0] != 0)
		{
			UINT64 dwOffset = ExtList[i].dwSectorOffset * SECTOR_SIZE;
			AddMessage ("Writing file \"%s\" to file partition at %016x\n", ExtList[i].chFileName, dwOffset);

			char chFile[MAX_PATH];
			sprintf (chFile, "%s%s", m_DLpath, ExtList[i].chFileName);
			if (! DownloadFile (hHandle, chFile, dwOffset))
			{
				AddMessage ("Download file %s fail!!\n", chFile);
				return false;
			}
			
		}
	}

	return true;
}

int CDownloadEmmc::gDownloadPartSearchIndex (const char* key, const char* value)
{
	int retval = -1;
	if (_stricmp (key, "order") == 0)
	{
		int i=0;
		while (gDownloadPart[i].order[0] != 0)
		{
			if (_stricmp (gDownloadPart[i].order, value) == 0)
				return i;
			++i;
		}
	}

	return retval;
}


bool CDownloadEmmc::DownloadFile (HANDLE hDevice, const char* chFile, const UINT64 dwOffset)
{
	UINT64 dwAddr = dwOffset;
	UINT64 TotalBytes, HasTransferBytes;
	int    percent, prevPercent = 0;
	char   chFileName [256];
	char   chTemp [256];


	// allocate memory
	BYTE *pBuffer = new BYTE [DEFAULT_TRANSFER];
	if (! pBuffer)
	{
		AddMessage ("allocate memory pBuffer fail");
		return false;
	}

	GetFileNameOnly (chFile, chFileName, 256);

	// open the file
	HANDLE hFile = CreateFile (chFile, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		AddMessageWithLastError ("Create file %s fail", chFile);
		return false;
	}

	// get file size
	DWORD dwSize, dwHigh, dwTransfer, dwActual, dwError;
	dwSize = GetFileSize (hFile, &dwHigh);
	TotalBytes = ((UINT64)dwHigh << 16) + dwSize;
	AddMessage ("File size = %u", TotalBytes);

	dwTransfer = DEFAULT_TRANSFER;
	HasTransferBytes = 0;

	// Move device's offset
	DWORD dwLow = LODWORD(dwAddr);
	dwHigh = HIDWORD(dwAddr);
	if (INVALID_SET_FILE_POINTER == SetFilePointer (hDevice, dwLow, (PLONG)&dwHigh, FILE_BEGIN))
	{
		AddMessageWithLastError ("SetFilePointer hDevice dwLow=%u dwHigh=%u fail", dwLow, dwHigh);
		CloseHandle (hFile);
		delete [] pBuffer;
		return false;
	}

	// transfer the file to device
	BOOL bRet;
	while (dwSize)
	{
		memset (pBuffer, 0, DEFAULT_TRANSFER);

		if (dwSize < dwTransfer)
		{
			dwTransfer = dwSize;
		}

		strcpy (chTemp, "ReadFile hFile");
		bRet = ReadFile (hFile, pBuffer, dwTransfer, &dwActual, 0);

		if (bRet && (dwActual == dwTransfer))
		{
			// align to 256K
			if (dwTransfer < DEFAULT_TRANSFER)
			{
				dwTransfer = (dwTransfer + 262144) / 262144 * 262144;
				dwSize = dwTransfer; // to end loop
			}

			//printf ("WriteFile position %016llx, size %u\n", dwAddr, dwTransfer);

			dwLow = LODWORD(dwAddr);
			dwHigh= HIDWORD(dwAddr);
			
			if (SetFilePointer (hDevice, dwLow, (PLONG)&dwHigh, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
			{
				dwError = GetLastError();
				if (dwError != NO_ERROR)
				{
					strcpy (chTemp, "SetFilePointer hDevice");
					bRet = false;
				}
			}

			if (bRet)
			{
				strcpy (chTemp, "WriteFile hDevice");
				bRet = WriteFile (hDevice, pBuffer, dwTransfer, &dwActual, 0);
				if (bRet && (dwTransfer == dwActual))
				{
					dwAddr += dwActual;

					HasTransferBytes += dwActual;
					percent = (int)(100 * HasTransferBytes / TotalBytes);
					if (percent > 100)
						percent = 100;
					
					if (percent > prevPercent) // to prevent print more than once
					{
						prevPercent = percent;
						char chMsg[512];
						wsprintf (chMsg, "Downloading %s... ", chFileName);
						UpdateProgress (chMsg, StreamingProtocol, percent);
						Sleep(0);
					}
				}
			}
		}

		if (! bRet)
		{
			AddMessageWithLastError (chTemp);
			break;
		}

		dwSize -= dwActual;

	}

	delete [] pBuffer;
	CloseHandle (hFile);
	//printf ("File write complete\n");
	return TRUE==bRet;
}
*/
