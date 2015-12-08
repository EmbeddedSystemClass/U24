#ifndef __BENTLYINTERFACE_H__
#define __BENTLYINTERFACE_H__

#include "NotifyBase.h"

//consts
const int DefaultStringLength = 512;
const int DefaultLargeTextSize = 70;
const COLORREF DefaultLargeTextColor = RGB(0, 255, 0);//Green
const COLORREF DefaultErrorTextColor = RGB(255, 0, 0);//Red

//Events name for notify
const char UI_Message[]    = "UIMessage";
const char UI_ShowDialog[] = "ShowDialog";
const char UI_Log[] = "UILog";

//Struct for events
struct sShowDialog
{
	sShowDialog():nTextSize(DefaultLargeTextSize),
		      TextColor(DefaultLargeTextColor),
		      nCaptionTextSize(DefaultLargeTextSize),
		      CaptionTextColor(DefaultLargeTextColor)
	{}

	char szMessage[DefaultStringLength];
	int nTextSize;
	COLORREF TextColor;

	char szCaption[DefaultStringLength];
	int nCaptionTextSize;
	COLORREF CaptionTextColor;
};

struct sUILog
{
	int nSeverity;
	char szMessage[DefaultStringLength];
};

//enum for device type, add new device type if necessary
enum enumDeviceType {COM};

//Interface between UI and DLL
interface IBentlyInterface : public QSYNC_LIB::INotifyBase_Char
{
	virtual ~IBentlyInterface(){}
	virtual void GetProjectName(char* lpBuffer, DWORD dwBufferSize) = 0;
	virtual void GetStationName(char* lpBuffer, DWORD dwBufferSize) = 0;
	virtual void GetDLLver(char* lpBuffer, DWORD dwBufferSize) = 0;
	virtual bool GetDeviceName (enumDeviceType &enumType, char* lpBuffer, DWORD dwBufferSize) = 0;
	virtual bool IsConnected (const char* szDeviceID) = 0;
	virtual int  GetTestItemCount() = 0;
	virtual bool GetTestItemName (const int index, char* lpBuffer, DWORD dwSize) = 0;
	virtual bool PreRun (const int index) = 0;
	virtual bool Run (const int index) = 0;
	virtual bool PostRun (const int index) = 0;
	virtual bool StopTest() = 0;
	virtual void GetErrorCode (char *lpBuffer, DWORD dwBufferSize) = 0;
};

//function pointers for export
typedef bool (*pfnNewBentlyInterface)(IBentlyInterface**); //MMI_DLL_API bool NewBentlyInterface (IBentlyInterface** ppIMMI);
typedef bool (*pfnDeleteBentlyInterface)(IBentlyInterface*); //MMI_DLL_API bool DeleteBentlyInterface (IBentlyInterface** ppIMMI);

#endif //__BENTLYINTERFACE_H__