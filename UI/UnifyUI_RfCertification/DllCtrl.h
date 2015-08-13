#pragma once
#include "NotifyBase.h"
#include "DLInterface.h"


typedef HANDLE (*fQisdaDllNew)();
typedef void (*fQisdaDllDelete)(HANDLE hResourse);

typedef int (*fSetCallBackMsg)(HANDLE hResource, int(*CallBack)(const char*, const char*)); // NO_ERROR
typedef int (*fSetCallBackPort)(HANDLE hResource, int(*CallBack)(const char*)); // NO_ERROR
typedef int (*fSetCallBackResult)(HANDLE hResource, int(*CallBack)(const int, const char*, const char*, const char*, const char*, const char*, const char*, const char*)); // NO_ERROR
typedef int (*fSetCallBackRxResult)(HANDLE hResource, int(*CallBack)(const char*)); // NO_ERROR

typedef int (*fSetParameter)(HANDLE hResource, char* szKey, char* szValue); // NO_ERROR
typedef int (*fGetParameter)(HANDLE hResource, char* szKey, char* szValue); // NO_ERROR

typedef int (*fGetPortCount)(HANDLE hResource);
typedef int (*fGetPortByIndex)(HANDLE hResource, int nIndex, char* szPort);

typedef int (*fGetInfoCount)(HANDLE hResource);
typedef int (*fGetInfoByIndex)(HANDLE hResource, int nIndex, char* szName, char* szInfo);

typedef int (*fBegin)(HANDLE hResource);
typedef int (*fPreRun)(HANDLE hResource);
typedef int (*fRun)(HANDLE hResource);
typedef int (*fPostRun)(HANDLE hResource);


class CDllCtrl
{
public:
	CDllCtrl(void);
	~CDllCtrl(void);

private:
	IDLInterface* m_iDL;
	HMODULE m_hDll;
	HANDLE m_hRes;
	fQisdaDllNew m_dllQisdaDllNew;
	fQisdaDllDelete m_dllQisdaDllDelete;
	fSetCallBackMsg m_dllSetCallBackMsg; // NO_ERROR
	fSetCallBackPort m_dllSetCallBackPort; // NO_ERROR
	fSetCallBackResult m_dllSetCallBackResult; // NO_ERROR
	fSetCallBackRxResult m_dllSetCallBackRxResult; // NO_ERROR
	fSetParameter m_dllSetParameter; // NO_ERROR
	fGetParameter m_dllGetParameter; // NO_ERROR
	fGetPortCount m_dllGetPortCount;
	fGetPortByIndex m_dllGetPortByIndex;
	fGetInfoCount m_dllGetInfoCount;
	fGetInfoByIndex m_dllGetInfoByIndex;
	fBegin m_dllBegin;
	fPreRun m_dllPreRun;
	fRun m_dllRun;
	fPostRun m_dllPostRun;
public:
	TCHAR m_szNowPath[MAX_PATH];
	bool Load(CString& dll_path);
	void UnLoad();

	int SetCallBackMsg(int(*CallBack)(const char*, const char*)); // NO_ERROR
	int SetCallBackPort(int(*CallBack)(const char*)); // NO_ERROR
	int SetCallBackResult(int(*CallBack)(const int, const char*, const char*, const char*, const char*, const char*, const char*, const char*)); // NO_ERROR
	int SetCallBackRxResult(int(*CallBack)(const char*)); // NO_ERROR

	int SetParameter(char* szKey, char* szValue); // NO_ERROR
	int GetParameter(char* szKey, char* szValue); // NO_ERROR

	int GetPortCount();
	int GetPortByIndex(int nIndex, char* szPort);

	int GetInfoCount();
	int GetInfoByIndex(int nIndex, char* szName, char* szInfo);

	int Begin();
	int PreRun();
	int Run();
	int PostRun();
};
