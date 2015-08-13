// NeptuneCtrl.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "NeptuneCtrl.h"
#pragma comment(lib, "version.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static void TraceLogInit()
{
	TCHAR szDllPath[MAX_PATH];
	memset(szDllPath, 0, sizeof(szDllPath));
	::GetModuleFileName(NULL, szDllPath, MAX_PATH);
	*_tcsrchr(szDllPath, '\\') = NULL;
	char szLogPath[MAX_PATH];
	memset(szLogPath, 0, sizeof(szLogPath));
	sprintf(szLogPath, "%s\\trace.log", szDllPath);
	TRACE(_T("log: %s\n"), szLogPath);

	TRACER_INIT(szLogPath);
	TRACER_STREAM("=======================START DEBUG=======================\r\n");

	//// 2008-02-26 Nek Wang: get os version
	OSVERSIONINFO vi;
	vi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&vi);
	TRACER_STREAM("Windows Version: " << (long)vi.dwMajorVersion << "." << (long)vi.dwMinorVersion << "\r\n");
	TRACER_STREAM("Build: " << (long)vi.dwBuildNumber << "\r\n");
	TRACER_STREAM("Platform ID: " << (long)vi.dwPlatformId << "\r\n");

	//// 2008-02-26 Nek Wang: get dll version
	DWORD dwHandle;
	DWORD nLen = GetFileVersionInfoSize(_T("NeptuneCtrl.dll"), &dwHandle);
	if (nLen != 0) {
		char* pver = new char[nLen];
		if (GetFileVersionInfo(_T("NeptuneCtrl.dll"), dwHandle, nLen, pver)) {
			void* pBuf;
			if (VerQueryValue(pver, _T("\\"), &pBuf, reinterpret_cast<PUINT>(&nLen))) {
				VS_FIXEDFILEINFO vsInfo;
				memcpy(&vsInfo, pBuf, sizeof(VS_FIXEDFILEINFO));
				TRACER_STREAM(
					"DLL Version: " <<
					(long)((vsInfo.dwFileVersionMS & 0xFFFF0000) >> 16) << "." <<
					(long)(vsInfo.dwFileVersionMS & 0x0000FFFF)         << "." <<
					(long)((vsInfo.dwFileVersionLS & 0xFFFF0000) >> 16) << "." <<
					(long)(vsInfo.dwFileVersionLS & 0x0000FFFF)         << "\r\n"
				);
			}
		}
		delete [] pver;
	}
}

static void TraceLogExit()
{
	TRACER_EXIT();
}

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CNeptuneCtrlApp

BEGIN_MESSAGE_MAP(CNeptuneCtrlApp, CWinApp)
	//{{AFX_MSG_MAP(CNeptuneCtrlApp)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNeptuneCtrlApp construction

CNeptuneCtrlApp::CNeptuneCtrlApp()
{
#ifdef _DEBUG
	TraceLogInit();
#endif // _DEBUG
	for (int i = 0; i < QTH_PORT_LAST; i++) {
		m_pCmdCtrl[i] = NULL;
	}
}

CNeptuneCtrlApp::~CNeptuneCtrlApp()
{
#ifdef _DEBUG
	TraceLogExit();
#endif // _DEBUG
	for (int i = 0; i < QTH_PORT_LAST; i++) {
		delete m_pCmdCtrl[i];
		m_pCmdCtrl[i] = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CNeptuneCtrlApp object

CNeptuneCtrlApp theApp;
