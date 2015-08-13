
#include "stdafx.h"
#include "ADBDevice.h"
#include <assert.h>

GUID adb_class_idFA = ANDROID_USB_CLASS_ID;

ADBDevice::ADBDevice(GUID class_id, unsigned short vendor_id, unsigned short product_id, unsigned char interface_id,const char* szComport,int nRetryCount)
	:m_class_id(class_id)
	,m_adb_interface(NULL)
	,m_adb_read_endpoint(NULL)
	,m_adb_write_endpoint(NULL)
{
	do
	{
		nRetryCount--;
		m_adb_interface = MultiAdbCreateInterface(m_class_id,
			vendor_id,
			product_id,
			interface_id,
			szComport);
		if(m_adb_interface == NULL)
			::Sleep(500);
	}
	while(m_adb_interface == NULL && nRetryCount > 0);

	if(m_adb_interface)
	{
		m_adb_read_endpoint =	AdbOpenDefaultBulkReadEndpoint(m_adb_interface,AdbOpenAccessTypeReadWrite,AdbOpenSharingModeReadWrite);
		m_adb_write_endpoint =	AdbOpenDefaultBulkWriteEndpoint(m_adb_interface,AdbOpenAccessTypeReadWrite,AdbOpenSharingModeReadWrite);
		//wait a moment for device initialization
		if(m_adb_read_endpoint && m_adb_write_endpoint)
			::Sleep(500);
	}
	//assert(m_adb_interface && m_adb_read_endpoint && m_adb_write_endpoint);
}

ADBDevice::ADBDevice(GUID class_id, unsigned short vendor_id, unsigned short product_id, unsigned char interface_id,int nRetryCount)
	:m_class_id(class_id)
	,m_adb_interface(NULL)
	,m_adb_read_endpoint(NULL)
	,m_adb_write_endpoint(NULL)
{
	do
	{
		nRetryCount--;
		m_adb_interface = AdbCreateInterface(m_class_id,
			vendor_id,
			product_id,
			interface_id);
		if(m_adb_interface == NULL)
			::Sleep(500);
	}
	while(m_adb_interface == NULL && nRetryCount > 0);

	if(m_adb_interface)
	{
		m_adb_read_endpoint =	AdbOpenDefaultBulkReadEndpoint(m_adb_interface,AdbOpenAccessTypeReadWrite,AdbOpenSharingModeReadWrite);
		m_adb_write_endpoint =	AdbOpenDefaultBulkWriteEndpoint(m_adb_interface,AdbOpenAccessTypeReadWrite,AdbOpenSharingModeReadWrite);
		//wait a moment for device initialization
		if(m_adb_read_endpoint && m_adb_write_endpoint)
			::Sleep(500);
	}
	assert(m_adb_interface && m_adb_read_endpoint && m_adb_write_endpoint);
}

ADBDevice::~ADBDevice()
{
	AdbCloseHandle(m_adb_interface);
	AdbCloseHandle(m_adb_read_endpoint);
	AdbCloseHandle(m_adb_write_endpoint);
}

bool ADBDevice::IsActive()
{
	return m_adb_interface && m_adb_read_endpoint && m_adb_write_endpoint;
}

bool ADBDevice::Write(char* write_buf,unsigned long bytes_to_write,unsigned long time_out)
{
	bool bRes = false;
	if(IsActive())
	{
		unsigned long bytes_written = 0;
		if(bytes_to_write > 0)
			bRes = AdbWriteEndpointSync(m_adb_write_endpoint, write_buf, bytes_to_write, &bytes_written, time_out) && (bytes_written == bytes_to_write);
	}
	return bRes;
}

bool ADBDevice::Read(char* read_buf,unsigned long nbuffersize,unsigned long& nRaedSize,unsigned long time_out)
{
	bool bRes = false;
	if(IsActive())
	{
		bRes = AdbReadEndpointSync(m_adb_read_endpoint, read_buf, nbuffersize, &nRaedSize, time_out);
	}
	return bRes;
}

RESPONSE ADBDevice::ReadResponse(unsigned long time_out)
{
	Sleep(100);
	RESPONSE rsp = TIMEOUT;
	unsigned long nRead  = 0;
	char szBuffer[1024] = {0};
	if(Read(szBuffer,1024,nRead,time_out) && nRead >= 4)
	{
		if(strncmp(szBuffer,"INFO",4) == 0) {
			rsp = INFO;
		}
		else if(strncmp(szBuffer,"OKAY",4) == 0) {
			rsp = OKAY;
		}
		else if(strncmp(szBuffer,"FAIL",4) == 0) {
			rsp = FAIL;
		}
		else if(strncmp(szBuffer,"DATA",4) == 0) {
			rsp = DATA;
		}
		else {
			rsp = UNKNOW;
		}
	} else {
		rsp = TIMEOUT;
	}

	// read FA --- case: FA is empty
	if (rsp == FAIL) {
		if(strncmp(szBuffer, "FAILPartition empty", 19) == 0) {
			return FAILFAEMPTY;
		} else {
		
		}
	}

	return rsp;
}