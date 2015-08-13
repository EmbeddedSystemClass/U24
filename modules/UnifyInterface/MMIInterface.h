#ifndef _MMIINTERFACE_H
#define _MMIINTERFACE_H


#include "NotifyBase.h"

const char UI_Message[]    = "UIMessage";
const char UI_ShowDialog[] = "ShowDialog";
const char UI_Log[] = "UILog";

struct sUILog
{
	int nSeverity;
	char strMessage[512];
};

interface IMMIInterface : public QSYNC_LIB::INotifyBase_Char
{
	virtual ~IMMIInterface(){}
	virtual void GetProjectName(char* lpBuffer, DWORD dwBufferSize) = 0;
	virtual bool GetDeviceName (char* lpBuffer, DWORD dwBufferSize) = 0;
	virtual bool IsConnected (const char* szDeviceID) = 0;
	virtual int  GetOnDeviceTestItemCount() = 0;
	virtual bool GetOnDeviceTestItemName (const int index, char* lpBuffer, DWORD dwSize) = 0;
	virtual int  GetOnDeviceTestItemResult (const int index) = 0;
	virtual int  GetOnPCTestItemCount() = 0;
	virtual bool GetOnPCTestItemName (const int index, char* lpBuffer, DWORD dwSize) = 0;
	virtual bool StartOnPCTest() = 0;
	virtual bool PreRun (const int index) = 0;
	virtual bool Run (const int index) = 0;
	virtual bool PostRun (const int index) = 0;
	virtual bool StopOnPCTest() = 0;
	virtual void GetErrorCode (char *lpBuffer, DWORD dwSize) = 0;
};


typedef bool (*pfnNewMMIInterface)(IMMIInterface**); //MMI_DLL_API bool NewMMIInterface (IMMIInterface** ppIMMI);
typedef bool (*pfnDeleteMMIInterface)(IMMIInterface*); //MMI_DLL_API bool DeleteMMIInterface (IMMIInterface** ppIMMI);




#endif //_MMIINTERFACE_H