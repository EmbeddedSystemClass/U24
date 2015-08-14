#include "StdAfx.h"
#include <Setupapi.h>
#include <Cfgmgr32.h>
#include "EnumCOMPort.h"




GUID GUIDUsbstor				= {0x4D36E967, 0xE325, 0x11CE, {0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18}};
GUID GUIDUsb					= {0x36FC9E60, 0xC465, 0x11CF, {0x80, 0x56, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};
GUID GUIDUsbInterface			= {0xA5DCBF10, 0x6530, 0x11D2, {0x90, 0x1f, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED}};
GUID GUIDDisk					= {0x53f56307, 0xb6bf, 0x11D0, {0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b}};
GUID GUIDQualcommDiagUsbClass	= {0x86e0d1e0, 0x8089, 0x11d0, {0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73}};
GUID GUIDQualcommModemUsbClass	= {0x2c7089aa, 0x2e0e, 0x11d1, {0xb1, 0x14, 0x00, 0xc0, 0x4f, 0xc2, 0xaa, 0xe4}};
GUID GUIDAndroidUsbClassID		= {0xf72fe0d4, 0xcbcb, 0x407d, {0x88, 0x14, 0x9e, 0xd6, 0x73, 0xd0, 0xdd, 0x6b}};




bool CEnumCOMPort::FindQualcomDevice(std::vector<USBDEVICE_STRUCT> &inUSBdevice)
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
			strcpy_s(devStruct.chHandleName, pDeviceDetailData->DevicePath);

			// Get the device instance of parent. This points to USB Composite Device
			DEVINST devInstParent;
			CM_Get_Parent(&devInstParent, devInfo.DevInst, 0);

			// Get the device ID of the USB root.
			CM_Get_Device_ID(devInstParent, (char*)chTemp, MAX_PATH, 0);
			strcpy_s(devStruct.chParentName, (char*)chTemp);

			// Get device instance name
			if (SetupDiGetDeviceInstanceId(hDeviceInfo, &devInfo, (PSTR)chTemp, MAX_PATH, &dwRequired))
				strcpy_s(devStruct.chInstanceName, (char*)chTemp);

			// Get device friendly name
			DWORD dataType;
			if (SetupDiGetDeviceRegistryProperty(hDeviceInfo, &devInfo, SPDRP_FRIENDLYNAME, &dataType, chTemp, MAX_PATH, &dwRequired))
				strcpy_s(devStruct.chFriendlyName, (char*)chTemp);

			if (SetupDiGetDeviceRegistryProperty(hDeviceInfo, &devInfo, SPDRP_DEVICEDESC, &dataType, chTemp, MAX_PATH, &dwRequired))
				strcpy_s(devStruct.chDeviceDescr, (char*)chTemp);

			inUSBdevice.push_back(devStruct);
			bFound = true;
		}
		++i;
	}

	// Destroy handle
	SetupDiDestroyDeviceInfoList(hDeviceInfo);
	return bFound;
}




bool CEnumCOMPort::FindADBdevice(std::vector<USBDEVICE_STRUCT> &inUSBdevice)
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
			strcpy_s(devStruct.chHandleName, pDeviceDetailData->DevicePath);

			// Get the device instance of parent. This points to USB Composite Device
			DEVINST devInstParent;
			CM_Get_Parent(&devInstParent, devInfo.DevInst, 0);

			// Get the device ID of the USB root.
			CM_Get_Device_ID(devInstParent, (char*)chTemp, MAX_PATH, 0);
			strcpy_s(devStruct.chParentName, (char*)chTemp);

			// Get device instance name
			if (SetupDiGetDeviceInstanceId(hDeviceInfo, &devInfo, (PSTR)chTemp, MAX_PATH, &dwRequired))
				strcpy_s(devStruct.chInstanceName, (char*)chTemp);

			// Get device friendly name
			DWORD dataType;
			if (SetupDiGetDeviceRegistryProperty(hDeviceInfo, &devInfo, SPDRP_FRIENDLYNAME, &dataType, chTemp, MAX_PATH, &dwRequired))
				strcpy_s(devStruct.chFriendlyName, (char*)chTemp);

			if (SetupDiGetDeviceRegistryProperty(hDeviceInfo, &devInfo, SPDRP_DEVICEDESC, &dataType, chTemp, MAX_PATH, &dwRequired))
				strcpy_s(devStruct.chDeviceDescr, (char*)chTemp);

			inUSBdevice.push_back(devStruct);
			bFound = true;
		}
		++i;
	}

	// Destroy handle
	SetupDiDestroyDeviceInfoList(hDeviceInfo);
	return bFound;
}

int CEnumCOMPort::GetComportFromUSB(USBDEVICE_STRUCT& dev)
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

int CEnumCOMPort::GetComportFromUSBByKeyword(USBDEVICE_STRUCT& dev, char* szKeyWord)
{
	int pos1 = -1;
	int pos2 = -1;
	int port = -1;
	CString cstrFriendlyName;
	CString cstrKeyword;
	CString cstrFind;

	cstrKeyword = szKeyWord;
	cstrKeyword += "(COM";
	
	cstrFriendlyName = dev.chFriendlyName;
	cstrFriendlyName.Replace(" ","");
	pos1 = cstrFriendlyName.Find(cstrKeyword) + cstrKeyword.GetLength();
	pos2 = cstrFriendlyName.Find(")");
	cstrFind = cstrFriendlyName.Mid(pos1, (pos2-pos1));
	
	port = atoi(cstrFind.GetBuffer());

	return port;
}

bool CEnumCOMPort::CheckCOMPortKeyword(USBDEVICE_STRUCT& dev, char* szKeyword)
{
	char* szLocate;
	szLocate = strstr(dev.chFriendlyName, szKeyword);
	if(szLocate != NULL)
		return true;

	return false;

}