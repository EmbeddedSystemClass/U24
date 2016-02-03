#ifndef _C_ERROR_HANDLING_H_
#define _C_ERROR_HANDLING_H_

typedef enum
{
	MODE_UNKNOWN    = -1,
	MODE_DLOAD      =  0,
	MODE_DIAGNOSTIC =  1,
	MODE_FASTBOOT   =  2,
	MODE_OS_ADB     =  3,
	MODE_EMERGENCY  =  4,
} DeviceMode;


#include "../../CommonUtility/BaseObject/IErrorHandling.h"

#pragma once
#include <setupapi.h>
#pragma comment(lib, "setupapi")

class CErrorHandling : public IErrorHandling
{
RF_DECLARE_DYNCREATE(CErrorHandling)


// Data members
public:
protected:
private:


// Member functions
public:
	CErrorHandling();
	virtual ~CErrorHandling() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap) { return true; }

	bool SearchPortUsb(const UINT secTimeout);
	
protected:
private:
	DeviceMode m_DeviceMode;
	bool FindUsbDevice();
	bool FindDeviceId(CString DeviceId);
	CString GetDeviceDesc(HDEVINFO& hDevInfo, SP_DEVINFO_DATA& spDevInfoData);

	struct ComInfo {
		CString port;
		CString desc;
	};
	struct ComInfo m_com;
	bool GetComPort();
	bool GetAdbDevice();
	bool DownloadQcomUsbLog();
	void FastbootDump();
};


#endif // End of #ifndef _C_ERROR_HANDLING_H_
