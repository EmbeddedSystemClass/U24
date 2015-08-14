#ifndef _COM_DEVICE_UTIL_H_
#define _COM_DEVICE_UTIL_H_


#include <SetupAPI.h>

#include <string>
#include <vector>
#include <map>
#include <cctype>

namespace ComDeviceUtility
{
	bool COMDeviceControl(char* szDeviceName, int iBaudRate, char* szCommand, char* szErrMsg);
	bool GetCOMPortDevByUsb(std::map<std::string, std::string>& map_strstrCOMDevice);
	bool GetCOMPortDevByWDK(std::map<std::string, std::string>& map_strstrCOMDevice);
	bool IsCOMPortAvailable(int iCOMPort, char* szErrMsg);
}


#endif // End of #ifndef _COM_DEVICE_UTIL_H_

