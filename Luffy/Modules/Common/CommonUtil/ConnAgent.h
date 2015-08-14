// ConnAgent.h : interface of the CConnAgent class
//

#pragma once

/*--------------------------------------------------------------------------*/
/* CONSTANT DECLARATION                                                     */
/*--------------------------------------------------------------------------*/

#define CONN_PORT_USE_REGISTRY					1	// search available COM from registry (HKEY_LOCAL_MACHINE\HARDWARE\DEVICEMAP\SERIALCOMM).
#define CONN_PORT_USE_QLIB						2	// search available COM from QLib DLL.
#define CONN_PORT_USE_WDK						3	// search available COM from Windows Drivre Kits.

class CConnAgent
{
public:
	CConnAgent();
	~CConnAgent();

// Attributes
public:

// Operations
public:
	static BOOL SearchQpst();
	static BOOL GetCommDevAry(int iSolution, CUIntArray& auiCommDevAry, CStringArray* pacstrCommDevNameAry = NULL);

protected:
	static BOOL GetCommDevAryUseRegistry(CUIntArray& auiCommDevAry);
	static HKEY GetSerialPortHKey();
	static UINT CommNameToIndex(const TCHAR* sComm, const int iCommLen);
	static BOOL GetCommDevAryUseQLib(CUIntArray& auiCommDevAry);
	static BOOL GetCommDevAryUseWdk(CUIntArray& auiCommDevAry, CStringArray* pacstrCommDevNameAry);
};
