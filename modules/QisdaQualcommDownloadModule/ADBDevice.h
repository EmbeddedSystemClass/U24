/***************************************************************************************
* Copyright (C) : QCS/WGC18                                      
* Module name	: Common   
* File name     : ADBDevice.h             
* Summary       : ADB download                                                             
* Version     Author            Date                 Abstract                      
* 1.0         Alex.Chen         2011/07/18           First version  
****************************************************************************************/

#pragma once

extern "C" 
{
	#include "..\..\lib\WinDDK\inc\wxp\usbdi.h"
}
//#include "..\MobileCommand\ADB\usb\api\adb_api.h"
#include "..\MobileCommand\NewADB\usb\api\adb_api.h"
#include <string>

#pragma comment(lib, "AdbWinApi.lib")

using namespace std;

#define USB_BUF_SIZE_ADBDevice 512
/*================= Fastboot download protocol =================
  -USB Entry point command
  -[response] format
    -¡§INFO¡¨
    -¡§OKAY¡¨
    -¡§FAIL¡¨
    -¡§DATA¡¨
  -ADB download protocol
    -send: ¡§download:%08x¡¨ ex:"download:00162c00"
    -read: [response]      ex:"OKAY"
    -send: RAWData         
    -read: [response]      ex:"OKAY"
    -send: "flash:%s"      ex:"flash:userdata"
    -read: [response]      ex:"OKAY"
  -All ADB command
    -send: ¡§version-bootloader¡¨
    -send: ¡§version-baseband¡¨
    -send: ¡§serialno¡¨
    -send: "signature"
    -send: ¡§download:%08x¡¨
    -send: "flash:%s"
    -send: "reboot"
================= Fastboot download protocol =================*/

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
	ADBDevice(const char* interface_name ,int nRetryCount);
	virtual ~ADBDevice();

public:
	bool IsActive(void);
	bool Write(char* write_buf, unsigned long bytes_to_write, unsigned long time_out = 0xFFFFFFF);
	bool Read(char* read_buf, unsigned long nbuffersize, unsigned long& nRaedSize, unsigned long time_out = 0xFFFFFFF);
	RESPONSE ReadResponse(unsigned long time_out = 0xFFFFFFF);
	RESPONSE ReadResponse_Msg(unsigned long time_out = 0xFFFFFFF, TCHAR* strOutput = NULL);
	RESPONSE ReadResponse7K(CString& Command, unsigned long time_out = 0xFFFFFFF);
	bool GetSerialNumber(std::string *strSerialNumber);
	bool AdbReboot (int mode);
	bool FastbootReboot(int mode);
	std::string GetDeviceName (){ return m_adb_device_name; };
	char m_usb_buf_ADBDevice[USB_BUF_SIZE_ADBDevice];
	int m_usb_buf_len_ADBDevice;

private:
	GUID          m_class_id;
	ADBAPIHANDLE  m_adb_interface;
	ADBAPIHANDLE  m_adb_read_endpoint;
	ADBAPIHANDLE  m_adb_write_endpoint;
	std::string   m_adb_serial_number;
	std::string   m_adb_device_name;
};

extern GUID adb_class_id;