
#include "stdafx.h"
#include "ADBDevice.h"
#include <assert.h>
#include <string.h>

GUID adb_class_id = ANDROID_USB_CLASS_ID;

ADBDevice::ADBDevice(GUID class_id, unsigned short vendor_id, unsigned short product_id, unsigned char interface_id,const char* szComport,int nRetryCount)
	:m_class_id(class_id)
	,m_adb_interface(NULL)
	,m_adb_read_endpoint(NULL)
	,m_adb_write_endpoint(NULL)
{
	do
	{
		nRetryCount--;
		m_adb_interface = MultiAdbCreateInterface(m_class_id, vendor_id, product_id, interface_id, szComport);
		if (m_adb_interface == NULL)
		{
			::Sleep(500);
		}
	}
	while(m_adb_interface == NULL && nRetryCount > 0);

	if (m_adb_interface)
	{
		/* Read/Write ADB */
		m_adb_read_endpoint =	AdbOpenDefaultBulkReadEndpoint(m_adb_interface,AdbOpenAccessTypeReadWrite,AdbOpenSharingModeReadWrite);
		m_adb_write_endpoint =	AdbOpenDefaultBulkWriteEndpoint(m_adb_interface,AdbOpenAccessTypeReadWrite,AdbOpenSharingModeReadWrite);

		/* Wait a moment for device initialization */
		if(m_adb_read_endpoint && m_adb_write_endpoint)
		{
			::Sleep(500);
		}

		DWORD size=1024;
		char *szDeviceName=new char[size];
		memset(szDeviceName, 0, size);

		if(AdbGetInterfaceName(m_adb_interface,szDeviceName,&size,true))
		{
			m_adb_device_name = szDeviceName;
		}
		delete [] szDeviceName;

	}
	assert(m_adb_interface && m_adb_read_endpoint && m_adb_write_endpoint);
}


ADBDevice::ADBDevice(GUID class_id, unsigned short vendor_id, unsigned short product_id, unsigned char interface_id, int nRetryCount)
	:m_class_id(class_id)
	,m_adb_interface(NULL)
	,m_adb_read_endpoint(NULL)
	,m_adb_write_endpoint(NULL)
{
	do
	{
		nRetryCount--;
		m_adb_interface = AdbCreateInterface(m_class_id, vendor_id, product_id, interface_id);
		if(m_adb_interface == NULL)
		{
			::Sleep(500);
		}
	}
	while(m_adb_interface == NULL && nRetryCount > 0);

	if(m_adb_interface)
	{
		/* Read/Write ADB */
		m_adb_read_endpoint =	AdbOpenDefaultBulkReadEndpoint(m_adb_interface,AdbOpenAccessTypeReadWrite,AdbOpenSharingModeReadWrite);
		m_adb_write_endpoint =	AdbOpenDefaultBulkWriteEndpoint(m_adb_interface,AdbOpenAccessTypeReadWrite,AdbOpenSharingModeReadWrite);

		/*wait a moment for device initialization */
		if(m_adb_read_endpoint && m_adb_write_endpoint)
		{
			::Sleep(500);
		}
	}
	assert(m_adb_interface && m_adb_read_endpoint && m_adb_write_endpoint);
}

ADBDevice::ADBDevice(const char* interface_name ,int nRetryCount)
{

	//from char to wchar_t
	int nIndex = MultiByteToWideChar(CP_ACP, 0, interface_name, -1, NULL, 0);
	wchar_t wchar_interface[1024];
	MultiByteToWideChar(CP_ACP, 0, interface_name, -1, wchar_interface, nIndex);
	
	//AdbCreateInterfaceByName
	do
	{
		nRetryCount--;
		m_adb_interface = AdbCreateInterfaceByName(wchar_interface);
		if(m_adb_interface == NULL)
		{
			::Sleep(500);
		}
	}
	while(m_adb_interface == NULL && nRetryCount > 0);

	if(m_adb_interface)
	{
		/* Read/Write ADB */
		m_adb_read_endpoint =	AdbOpenDefaultBulkReadEndpoint(m_adb_interface,AdbOpenAccessTypeReadWrite,AdbOpenSharingModeReadWrite);
		m_adb_write_endpoint =	AdbOpenDefaultBulkWriteEndpoint(m_adb_interface,AdbOpenAccessTypeReadWrite,AdbOpenSharingModeReadWrite);

		/*wait a moment for device initialization */
		if(m_adb_read_endpoint && m_adb_write_endpoint)
		{
			::Sleep(500);
		}
	}


	assert(m_adb_interface && m_adb_read_endpoint && m_adb_write_endpoint);
	
}


ADBDevice::~ADBDevice()
{
	//if (IsActive())
	//{
		AdbCloseHandle(m_adb_interface);
		AdbCloseHandle(m_adb_read_endpoint);
		AdbCloseHandle(m_adb_write_endpoint);
	//}
}


bool ADBDevice::IsActive()
{
	return m_adb_interface && m_adb_read_endpoint && m_adb_write_endpoint;
}


bool ADBDevice::Write(char* write_buf,unsigned long bytes_to_write,unsigned long time_out)
{
	bool bRes = false;
	if (IsActive())
	{
		unsigned long bytes_written = 0;
		if (bytes_to_write > 0)
		{
			bRes = AdbWriteEndpointSync(m_adb_write_endpoint, write_buf, bytes_to_write, &bytes_written, time_out) && (bytes_written == bytes_to_write);
		}
	}
	return bRes;
}


bool ADBDevice::Read(char* read_buf,unsigned long nbuffersize,unsigned long& nRaedSize,unsigned long time_out)
{
	bool bRes = false;
	if (IsActive())
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
	if(Read(szBuffer,1024,nRead,time_out))
	{
		if (nRead >= 4)
		{
			if(strncmp(szBuffer,"INFO",4) == 0)
			{
				rsp = INFO;
			}
			else if(strncmp(szBuffer,"OKAY",4) == 0)
			{
				rsp = OKAY;
			}
			else if(strncmp(szBuffer,"FAIL",4) == 0)
			{
				rsp = FAIL;
			}
			else if(strncmp(szBuffer,"DATA",4) == 0)
			{
				rsp = DATA;
			}
		}
		else
		{
			rsp = UNKNOW;
		}
	} else {
		rsp = TIMEOUT;
	}

	// read FA --- case: FA is empty
	if (rsp == FAIL) 
	{
		if(strncmp(szBuffer, "FAILPartition empty", 19) == 0)
		{
			return FAILFAEMPTY;
		}
	}

	return rsp;
}

RESPONSE ADBDevice::ReadResponse_Msg(unsigned long time_out, TCHAR* strOutput)
{
	Sleep(100);
	RESPONSE rsp = TIMEOUT;
	unsigned long nRead  = 0;
	char szBuffer[1024] = {0};
	
	if(Read(szBuffer,1024,nRead,time_out))
	{
		// Start...Add by Lennon 20130608 to filter non-printable char
		for (int i = 0; i < 1024; ++ i)
		{
			//	if(!isprint(szBuffer[i])) //0x20~0x7E 
			if(szBuffer[i]<0x20 || szBuffer[i]>0x7E)
			{
				szBuffer[i]=0;
				break;
			}
		}
		// End

		int pos = 0;
		CString strTmpAll;
		CString strTmp;
		strTmpAll = szBuffer;
		strTmp = strTmpAll.Tokenize("!", pos);
		strTmp = "[Read_Response], " + strTmp;
		sprintf(strOutput, "%s", strTmp);
		if ( nRead >= 4)
		{
			if(strncmp(szBuffer,"INFO",4) == 0)
			{
				rsp = INFO;
			}
			else if(strncmp(szBuffer,"OKAY",4) == 0)
			{
				rsp = OKAY;
			}
			else if(strncmp(szBuffer,"FAIL",4) == 0)
			{
				rsp = FAIL;
			}
			else if(strncmp(szBuffer,"DATA",4) == 0)
			{
				rsp = DATA;
			}
		}
		else
		{
			rsp = UNKNOW;
		}
	}
	else
	{
		rsp = TIMEOUT;
	}

	// read FA --- case: FA is empty
	if (rsp == FAIL) 
	{
		if(strncmp(szBuffer, "FAILPartition empty", 19) == 0)
		{
			return FAILFAEMPTY;
		}
	}

	return rsp;
}

RESPONSE ADBDevice::ReadResponse7K(CString& Command, unsigned long time_out)
{
	Sleep(100);
	RESPONSE rsp = TIMEOUT;
	unsigned long nRead  = 0;
	char szBuffer[1024] = {0};
	if(Read(szBuffer,1024,nRead,time_out) && nRead >= 4)
	{
		szBuffer[nRead] = 0;
		if(strncmp(szBuffer,"INFO",4) == 0)
		{
			rsp = INFO;
		}
		else if(strncmp(szBuffer,"OKAY",4) == 0)
		{
			rsp = OKAY;
		}
		else if(strncmp(szBuffer,"FAIL",4) == 0)
		{
			rsp = FAIL;
		}
		else if(strncmp(szBuffer,"DATA",4) == 0)
		{
			rsp = DATA;
		}
		else
		{
			rsp = UNKNOW;
		}
		Command = szBuffer;
	} else {
		rsp = TIMEOUT;
	}

	// read FA --- case: FA is empty
	if (rsp == FAIL) 
	{
		if(strncmp(szBuffer, "FAILPartition empty", 19) == 0)
		{
			return FAILFAEMPTY;
		}
	}

	return rsp;
}
bool ADBDevice::GetSerialNumber(std::string *strSerialNumber)
{
	if (NULL ==m_adb_interface )
	{
		return false;
	}

	if ( !m_adb_serial_number.empty() )
	{
		*strSerialNumber= m_adb_serial_number;	
		return true;
	}

	//serial number not exist , get serial number
	char chTemp[1024];
	unsigned long size = 1024;
	if (AdbGetSerialNumber(m_adb_interface, chTemp, &size, true))
	{
		m_adb_serial_number = chTemp;
		*strSerialNumber= m_adb_serial_number;
		return true;
	}

	return true;
}

bool ADBDevice::AdbReboot( int mode )
{
	if (NULL == m_adb_interface)
	{
		return false;
	}

	//fix me
	byte sz_open_cmd[24] = {0x4F,0x50,0x45,0x4E,0x03,0x00,0x00,0x00,
							0x00,0x00,0x00,0x00,0x12,0x00,0x00,0x00,
							0xF0,0x06,0x00,0x00,0xB0,0xAF,0xBA,0xB1};

	if ( !Write( (char*)sz_open_cmd, 24 , 1000)) 
	{
		return false;
	}
	Sleep(100);

	// different boot mode
	char sz_command[64] = {0};
	if (0 ==mode) sprintf(sz_command, "reboot");
	if (1 ==mode) sprintf(sz_command, "reboot:bootloader");

	if ( !Write(sz_command, strlen(sz_command)+1, 1000)) 
	{
		return false;
	}

	if ( ReadResponse(10000) != RESPONSE::OKAY) 
	{
		return false;	
	}
	
	return true;
}

bool ADBDevice::FastbootReboot( int mode )
{
	// different boot mode
	char sz_command[64] = {0};
	if (0 ==mode) sprintf(sz_command, "reboot");
	if (1 ==mode) sprintf(sz_command, "reboot:bootloader");

	if ( !Write(sz_command, strlen(sz_command)+1, 1000)) 
	{
		return false;
	}

	if ( ReadResponse(10000) != RESPONSE::OKAY) 
	{
		return false;
	}

	return true;

}
