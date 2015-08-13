/*********************************************************************************
 * COPYRIGHT Qisda WGC18 2011
 *
 * Description: fastboot mode read/write FA.
 * FileName   : FastbootRWFA.h
 * Version    : $Revision:   1.1  $
 * Author     : Alex.Chen
 * Time       : 2011/06/13
 * History    :
 *  NO   Date            Updated by        Content of change
 *  1    2011/06/13      Alex.Chen         Create.
 ********************************************************************************/

#pragma once

extern "C" {
#include "..\..\lib\WinDDK\inc\wxp\usbdi.h"
}
//#include "..\MobileCommand\ADB\usb\api\adb_api.h"
#include "..\MobileCommand\NewADB\usb\api\adb_api.h"

#pragma comment(lib, "AdbWinApi.lib")

enum RESPONSE
{
	INFO,
	OKAY,
	FAIL,
	FAILFAEMPTY,
	DATA,
	UNKNOW,
	TIMEOUT
};

class ADBDevice
{
public:
	ADBDevice(GUID class_id, unsigned short vendor_id, unsigned short product_id, unsigned char interface_id, const char* szComport, int nRetryCount = 20);
	ADBDevice(GUID class_id, unsigned short vendor_id, unsigned short product_id, unsigned char interface_id, int nRetryCount = 20);
	virtual ~ADBDevice();

public:
	bool IsActive(void);
	bool Write(char* write_buf, unsigned long bytes_to_write, unsigned long time_out = 0xFFFFFFF);
	bool Read(char* read_buf, unsigned long nbuffersize, unsigned long& nRaedSize, unsigned long time_out = 0xFFFFFFF);
	RESPONSE ReadResponse(unsigned long time_out = 0xFFFFFFF);

private:
	GUID          m_class_id;
	ADBAPIHANDLE  m_adb_interface;
	ADBAPIHANDLE  m_adb_read_endpoint;
	ADBAPIHANDLE  m_adb_write_endpoint;

};

extern GUID adb_class_idFA;
