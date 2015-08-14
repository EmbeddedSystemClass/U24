#ifndef _DETECT_COMPORT_H_
#define _DETECT_COMPORT_H_


#include <string>
#include <vector>
//#include "ADBDevice.h"

typedef struct
{
	char   chHandleName  [MAX_PATH];
	char   chInstanceName[MAX_PATH];
	char   chFriendlyName[MAX_PATH];
	char   chDeviceDescr [MAX_PATH];
	char   chParentName  [MAX_PATH];
	char   chSerialNumber[MAX_PATH];
	DWORD  dwDeviceNumber;

} USBDEVICE_STRUCT;


class CEnumCOMPort
{
public:
	bool FindQualcomDevice(std::vector<USBDEVICE_STRUCT> &inUSBdevice);
	bool FindADBdevice(std::vector<USBDEVICE_STRUCT> &inUSBdevice);
	int GetComportFromUSB(USBDEVICE_STRUCT& dev);
	int GetComportFromUSBByKeyword(USBDEVICE_STRUCT& dev, char* szKeyWord);
	bool CheckCOMPortKeyword(USBDEVICE_STRUCT& dev, char* szKeyword);





};


#endif