#ifndef _FT_WIRELESS_INTERFACE_H_
#define _FT_WIRELESS_INTERFACE_H_


#include "NotifyBase.h"


#define MAX_BUFFER_SIZE 512


// UI defined messages
const char UI_UPDATE_INFO[] = "UIUpdateInfo";
const char UI_UPDATE_MESSAGE[] = "UIUpdateMessage";
const char UI_LOGGING[] = "UILogging";
const char UI_MSG_DLG[] = "UIMsgDlg";


// Message data structures
struct sUIInfo
{
	char szName[MAX_BUFFER_SIZE];
	char szData[MAX_BUFFER_SIZE];
};

struct sUIMessage
{
	char szItemCode[MAX_BUFFER_SIZE];
	char szErrCode[MAX_BUFFER_SIZE];
	char szBand[MAX_BUFFER_SIZE];
	char szChannel[MAX_BUFFER_SIZE];
	char szUpper[MAX_BUFFER_SIZE];
	char szLower[MAX_BUFFER_SIZE];
	char szMeasured[MAX_BUFFER_SIZE];
	char szUnit[MAX_BUFFER_SIZE];
	char szMsg[MAX_BUFFER_SIZE];
};

struct sUILogging
{
	int iSeverity; // 0 = INFO, 1 = WARNING, 2 = ERROR
	char szLog[MAX_BUFFER_SIZE];
};

struct sUIMsgDlg
{
	char szMsg[MAX_BUFFER_SIZE];
	DWORD color;
	int iTextSize;
};


// PICS class
class CPICSHandler
{
public:
	virtual bool Set_PICS_Value(const char* szKeyWord, const char* szValue) = 0;
};


// UI interface
interface IFTWirelessInterface : public QSYNC_LIB::INotifyBase_Char
{
public:
	virtual ~IFTWirelessInterface() {}

	// Get station ID
	virtual bool __cdecl GetStationID(char* szStationID, int iSize) = 0;

	// Get tool information
	virtual bool __cdecl GetToolVersion(char* szToolVersion, int iSize) = 0;

	// Get other information
	virtual int __cdecl GetInfoCount() = 0;
	virtual bool __cdecl GetInfoByIndex(int iIndex, sUIInfo* psUserInfo) = 0;

	// Get test items
	virtual int __cdecl GetTestItemCount() = 0;
	virtual bool __cdecl GetTestItemByIndex(int iIndex, char* szName, int iSize) = 0;

	// Test sequence
	virtual bool __cdecl Begin() = 0;
	virtual bool __cdecl GetDeviceName(char* szName, int iSize) = 0;
	virtual bool __cdecl IsConnected(const char* szName) = 0;
	virtual bool __cdecl SetPicasso(const char* szPicasso) = 0;
	virtual bool __cdecl SetPICSHandler(const CPICSHandler* pCPICSHandler) = 0;
	virtual bool __cdecl PreRun(int iIndex) = 0;
	virtual bool __cdecl Run(int iIndex) = 0;
	virtual bool __cdecl PostRun(int iIndex) = 0;
	virtual bool __cdecl Disconnect(const char* szName) = 0;
	virtual bool __cdecl End() = 0;
	virtual bool __cdecl Stop(int iIndex) = 0;

	// Get test result
	virtual bool __cdecl GetTestResult(int iIndex, sUIMessage* psResult) = 0;
};


typedef bool (__cdecl *pFnNewFTWirelessInterface)(IFTWirelessInterface**); //bool D_NewFTWirelessInterface(IFTWirelessInterface**);
typedef bool (__cdecl *pFnDeleteFTWirelessInterface)(IFTWirelessInterface*); //bool D_DeleteFTWirelessInterface(IFTWirelessInterface*);


#endif // End of #ifndef _FT_WIRELESS_INTERFACE_H_
