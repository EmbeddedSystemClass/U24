#include "stdafx.h"
#include "ComDeviceUtil.h"

namespace ComDeviceUtility
{
	bool COMDeviceControl(char* szDeviceName, int iBaudRate, char* szCommand, char* szErrMsg)
	{
		HANDLE hCOMPort;
		DCB dcb;
		COMMTIMEOUTS commTimeouts;
		DWORD iBytesWritten;
		DWORD iBytesRead;

		hCOMPort = CreateFileA(szDeviceName, GENERIC_READ | GENERIC_WRITE , 0, NULL, OPEN_EXISTING , 0, NULL);

		if (hCOMPort == INVALID_HANDLE_VALUE)
		{
			sprintf_s(szErrMsg, 255, "Fail to open device");
			return false;
		}

		if (SetupComm(hCOMPort, 128, 128) != TRUE)
		{
			sprintf_s(szErrMsg, 255, "Fail to setup connection");
			CloseHandle(hCOMPort);
			return false;
		}

		if (GetCommState(hCOMPort, &dcb) != TRUE)
		{
			sprintf_s(szErrMsg, 255, "Fail to get connection state");
			CloseHandle(hCOMPort);
			return false;
		}

		dcb.BaudRate = iBaudRate;
		dcb.ByteSize = 8;
		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.fAbortOnError = TRUE;

		if (SetCommState(hCOMPort, &dcb) != TRUE)
		{
			sprintf_s(szErrMsg, 255, "Fail to set connection state");
			CloseHandle(hCOMPort);
			return false;
		}

		if (GetCommTimeouts(hCOMPort, &commTimeouts) != TRUE)
		{
			sprintf_s(szErrMsg, 255, "Fail to get connection timeouts");
			CloseHandle(hCOMPort);
			return false;
		}

		commTimeouts.ReadIntervalTimeout = 50;
		commTimeouts.ReadTotalTimeoutConstant = 50;
		commTimeouts.ReadTotalTimeoutMultiplier = 10;
		commTimeouts.WriteTotalTimeoutConstant = 50;
		commTimeouts.WriteTotalTimeoutMultiplier = 10;

		if (SetCommTimeouts(hCOMPort, &commTimeouts) != TRUE)
		{
			CloseHandle(hCOMPort);
			return false;
		}

		int iLength = strlen(szCommand);
		char* szCmd = new char[iLength + 2];
		memset(szCmd, 0, iLength + 2);
		sprintf_s(szCmd, iLength + 1, szCommand);
		szCmd[iLength] = '\r';

		char szResponse[256];
		memset(szResponse, 0, 256);

		WriteFile(hCOMPort, szCmd, iLength + 1, &iBytesWritten, NULL);
		Sleep(500);
		ReadFile(hCOMPort, (void*)szResponse, 255, &iBytesRead, NULL);

		delete [] szCmd;

		CloseHandle(hCOMPort);
		return true;
	}

	bool GetCOMPortDevByUsb(std::map<std::string, std::string>& map_strstrCOMDevice)
	{
		map_strstrCOMDevice.clear();

		BOOL bContinue = FALSE;

		// 1. Retrieves the GUID(s) associated with the specified class name "Ports"
		DWORD uiGuidSize = 0;
		GUID* ptGuidBuf = NULL;

		::SetupDiClassGuidsFromNameA("USB", NULL, 0, &uiGuidSize);   //Ports
		/*SetupDiClassGuidsFromNameA 通过设备的类的名字得到设备类型的GUID
		USB是类的名字，若是COM口，则类名为Ports
		*/
		if (uiGuidSize > 0)
		{
			ptGuidBuf = new GUID[uiGuidSize];
			ASSERT(ptGuidBuf != NULL);

			if (ptGuidBuf != NULL)
			{
				bContinue = ::SetupDiClassGuidsFromNameA("USB", ptGuidBuf, uiGuidSize, &uiGuidSize);
			}
		}

		// 2. Returns a device information set that contains all devices of a specified class
		HDEVINFO hDevInfoSet = INVALID_HANDLE_VALUE;

		if (bContinue)
		{
			hDevInfoSet = ::SetupDiGetClassDevsA(ptGuidBuf, NULL, NULL, DIGCF_PRESENT);   
			/*若SetupDiGetClassDevsA运行成功，返回设备信息结构的句柄，该结构包含与指定参数匹配的所有已安装设备。如果失败，则返回INVALID_HANDLE_VALUE。
			第一个参数是 GUID* ClassGuid  ptGuidBuf指在创建设备列表的时候提供一个指向GUID的指针
			第四个参数是 DWORD Flags  当指定为DIGCF_PRESENT时只返回当前存在的设备   当指定为DIGCF_ALLCLASSES返回所有已安装的设备。如果这个标志设置了，ClassGuid参数将被忽略
			*/
			bContinue = (hDevInfoSet != INVALID_HANDLE_VALUE);
		}

		// 3. Start enumeration
		DWORD uiIndex = 0;

		//std::map<int,std::string> map_MyUsb;

		while (bContinue)
		{
			// Returns one context structure for a device information element of a device information set.
			SP_DEVINFO_DATA tDevInfo;
			tDevInfo.cbSize = sizeof(SP_DEVINFO_DATA);
			if ((bContinue = ::SetupDiEnumDeviceInfo(hDevInfoSet, uiIndex, &tDevInfo)))
			{
				// Opens a registry storage key for device-specific configuration information,只能获得注册表下的 设备接口类名->设备实例名->符号链接名->设备描述符(Device Parameters,又叫友好名)下的键的值
				HKEY hKey = ::SetupDiOpenDevRegKey(hDevInfoSet, &tDevInfo, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE);

				if (hKey != NULL)
				{
					// Read the name of the port.
					char szDev[256];
					DWORD uiType = 0, uiDevSize = 256;
					memset(szDev, 0, 256);
					
					//RegQueryValueExA用来查询DeviceInterfaceGUIDs的键值及数据类型，返回ERROR_SUCCESS表示成功
					if ((::RegQueryValueExA(hKey, "DeviceInterfaceGUIDs", NULL, &uiType, (LPBYTE)szDev, &uiDevSize) == ERROR_SUCCESS) && (uiType == REG_MULTI_SZ))  //Portname
					{
						std::string strTmp = szDev;

						memset(szDev, 0, 256);
						uiDevSize = 256;

						//map_MyUsb.clear();
						if (SetupDiGetDeviceRegistryPropertyA(hDevInfoSet, &tDevInfo, SPDRP_DEVICEDESC, &uiType, (PBYTE)szDev, uiDevSize, &uiDevSize) && (uiType == REG_SZ))
							map_strstrCOMDevice[strTmp] = std::string(szDev);
							//map_MyUsb.insert(make_pair(uiIndex,std::string(szDev)));
						else
							map_strstrCOMDevice[strTmp] = std::string("");
							//map_MyUsb.insert(make_pair(uiIndex,std::string("")));
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

	bool IsCOMPortAvailable(int iCOMPort, char* szErrMsg)
	{
		/* Check input */
		if (0 == iCOMPort)
		{
			sprintf_s(szErrMsg, MAX_PATH, "The handset is not connected.");
			return false;
		}

		/* Check COM Port */
		int i_comPort = iCOMPort;
		wchar_t sz_comPort[MAX_PATH] = {0};
		swprintf_s(sz_comPort, MAX_PATH, L"\\\\.\\COM%d", i_comPort);
		wchar_t sz_comPortTwo[MAX_PATH] = {0};
		swprintf_s(sz_comPortTwo, MAX_PATH, L"COM%d", i_comPort);          // For COM port is < 10


		/* Wait 5s */
		int i_count = 0;
		for (int i = 0; i <= 10; i++)
		{
			HANDLE h_handle = ::CreateFile(sz_comPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
			if (INVALID_HANDLE_VALUE == h_handle)
			{
				h_handle = ::CreateFile(sz_comPortTwo, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
				if (INVALID_HANDLE_VALUE == h_handle)
				{
					i_count++;
					if (i_count == 10)
					{
						sprintf_s(szErrMsg, 255, "Open COM port Fail.");
						return false;
					}
					else
					{
						Sleep(500);
						continue;
					}
				} 
				else
				{
					::CloseHandle(h_handle);
					break;
				}
			}
			else
			{
				::CloseHandle(h_handle);
				break;
			}
		}

		return true;
	}
	bool GetCOMPortDevByWDK(std::map<std::string, std::string>& map_strstrCOMDevice)
	{
		map_strstrCOMDevice.clear();

		BOOL bContinue = FALSE;

		// 1. Retrieves the GUID(s) associated with the specified class name "Ports"
		DWORD uiGuidSize = 0;
		GUID* ptGuidBuf = NULL;

		::SetupDiClassGuidsFromNameA("AndroidUsbDeviceClass", NULL, 0, &uiGuidSize);
		/*SetupDiClassGuidsFromNameA 通过设备的类的名字得到设备类型的GUID
		USB是类的名字，若是COM口，则类名为Ports
		*/
		if (uiGuidSize > 0)
		{
			ptGuidBuf = new GUID[uiGuidSize];
			ASSERT(ptGuidBuf != NULL);

			if (ptGuidBuf != NULL)
			{
				bContinue = ::SetupDiClassGuidsFromNameA("AndroidUsbDeviceClass", ptGuidBuf, uiGuidSize, &uiGuidSize);
			}
		}

		// 2. Returns a device information set that contains all devices of a specified class
		HDEVINFO hDevInfoSet = INVALID_HANDLE_VALUE;

		if (bContinue)
		{
			hDevInfoSet = ::SetupDiGetClassDevsA(ptGuidBuf, NULL, NULL, DIGCF_PRESENT);   
			/*若SetupDiGetClassDevsA运行成功，返回设备信息结构的句柄，该结构包含与指定参数匹配的所有已安装设备。如果失败，则返回INVALID_HANDLE_VALUE。
			第一个参数是 GUID* ClassGuid  ptGuidBuf指在创建设备列表的时候提供一个指向GUID的指针
			第四个参数是 DWORD Flags  当指定为DIGCF_PRESENT时只返回当前存在的设备   当指定为DIGCF_ALLCLASSES返回所有已安装的设备。如果这个标志设置了，ClassGuid参数将被忽略
			*/
			bContinue = (hDevInfoSet != INVALID_HANDLE_VALUE);
		}

		// 3. Start enumeration
		DWORD uiIndex = 0;

		//std::map<int,std::string> map_MyUsb;

		while (bContinue)
		{
			// Returns one context structure for a device information element of a device information set.
			SP_DEVINFO_DATA tDevInfo;
			tDevInfo.cbSize = sizeof(SP_DEVINFO_DATA);
			if ((bContinue = ::SetupDiEnumDeviceInfo(hDevInfoSet, uiIndex, &tDevInfo)))
			{
				// Opens a registry storage key for device-specific configuration information,只能获得注册表下的 设备接口类名->设备实例名->符号链接名->设备描述符(Device Parameters,又叫友好名)下的键的值
				HKEY hKey = ::SetupDiOpenDevRegKey(hDevInfoSet, &tDevInfo, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE);

				if (hKey != NULL)
				{
					// Read the name of the port.
					char szDev[256];
					DWORD uiType = 0, uiDevSize = 256;
					memset(szDev, 0, 256);
					
					//RegQueryValueExA用来查询DeviceInterfaceGUIDs的键值及数据类型，返回ERROR_SUCCESS表示成功
					if ((::RegQueryValueExA(hKey, "DeviceInterfaceGUIDs", NULL, &uiType, (LPBYTE)szDev, &uiDevSize) == ERROR_SUCCESS) && (uiType == REG_MULTI_SZ))
					{
						std::string strTmp = szDev;

						memset(szDev, 0, 256);
						uiDevSize = 256;

						//map_MyUsb.clear();
						if (SetupDiGetDeviceRegistryPropertyA(hDevInfoSet, &tDevInfo, SPDRP_DEVICEDESC, &uiType, (PBYTE)szDev, uiDevSize, &uiDevSize) && (uiType == REG_SZ))
							map_strstrCOMDevice[strTmp] = std::string(szDev);
							//map_MyUsb.insert(make_pair(uiIndex,std::string(szDev)));
						else
							map_strstrCOMDevice[strTmp] = std::string("");
							//map_MyUsb.insert(make_pair(uiIndex,std::string("")));
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

} // namespace ComDeviceUtility
