// SPU_UI_Extend.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "SPU_UI_Extend.h"
#include "../../Common/CommonUtil\UnicodeUtility.h"
#include "DebugDlg.h"
#include "InputDlg.h"
#include "MessageDlg.h"
#include "SPU_UI_Export.h"

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
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


// CSPU_UI_ExtendApp

BEGIN_MESSAGE_MAP(CSPU_UI_ExtendApp, CWinApp)
END_MESSAGE_MAP()


// CSPU_UI_ExtendApp construction

CSPU_UI_ExtendApp::CSPU_UI_ExtendApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CSPU_UI_ExtendApp object

CSPU_UI_ExtendApp theApp;


// CSPU_UI_ExtendApp initialization

BOOL CSPU_UI_ExtendApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

INT CBTMessageBox(HWND,LPSTR,LPSTR,UINT);
LRESULT CALLBACK CBTProc(INT, WPARAM, LPARAM);

HHOOK hhk;
CString strButton1;
CString strButton2;

INT CBTMessageBox(HWND hwnd, LPTSTR lpText, LPTSTR lpCaption,LPSTR lpBut1, LPSTR lpBut2,UINT uType)
{	
	CString strCaption = lpCaption;
	if(hwnd == NULL)
		hwnd = AfxGetApp()->GetMainWnd()->GetSafeHwnd();
	if(::IsWindow(hwnd) && hwnd && strCaption.IsEmpty())
	{
		TCHAR szBuffer[1024] = {0};
		::GetWindowText(hwnd,szBuffer,1024);
		strCaption = szBuffer;
	}
	strButton1=  lpBut1;
	strButton2=  lpBut2;
	hhk = SetWindowsHookEx(WH_CBT, &CBTProc, 0,GetCurrentThreadId());
	return MessageBox(hwnd, lpText, strCaption, uType);
}

LRESULT CALLBACK CBTProc(INT nCode, WPARAM wParam, LPARAM lParam)
{
	HWND hChildWnd;    // msgbox is "child"	
	// notification that a window is about to be activated
	// window handle is wParam
	if (nCode == HCBT_ACTIVATE)
	{
		// set window handles
		hChildWnd = (HWND)wParam;
		//to get the text of the Yes button
		UINT result;
		if (GetDlgItem(hChildWnd,IDYES)!=NULL)
			result= SetDlgItemText(hChildWnd,IDYES,strButton1);
		if (GetDlgItem(hChildWnd,IDNO)!=NULL)
			result= SetDlgItemText(hChildWnd,IDNO,strButton2);
		if (GetDlgItem(hChildWnd,IDRETRY)!=NULL)
			result= SetDlgItemText(hChildWnd,IDRETRY,strButton1);
		if (GetDlgItem(hChildWnd,IDOK)!=NULL)
			result= SetDlgItemText(hChildWnd,IDOK,strButton1);
		if (GetDlgItem(hChildWnd,IDCANCEL)!=NULL)
			result= SetDlgItemText(hChildWnd,IDCANCEL,strButton2);
		//Continue changing other button captions

		// exit CBT hook
		UnhookWindowsHookEx(hhk);
	}
	// otherwise, continue with any possible chained hooks
	else CallNextHookEx(hhk, nCode, wParam, lParam);
	return 0;
}

SPU_EXTEND_UI_API bool SPU_Print_MFC(const TSTRING& strName,const TSTRING& strOutput,CWnd* pParent)
{
	bool bRes = true;	
	
	if(_T("PASSFAILMessage") == strName)
	{
		CMessageDlg messageDlg(strOutput.c_str(),_T(""),pParent);
		messageDlg.DoModal();
	}
	else
	{
		bRes = false;
	}

	return bRes;
}

SPU_EXTEND_UI_API bool SPU_Print(const TSTRING& strName,const TSTRING& strOutput,HWND hParent)
{
	bool bRes = true;	

	CWnd temWnd;
	temWnd.FromHandle(hParent);
	bRes = SPU_Print_MFC(strName,strOutput,&temWnd);
	temWnd.Detach();

	return bRes;
}

SPU_EXTEND_UI_API bool SPU_WaitInput_MFC(const TSTRING& strName,TCHAR* szBuffer,int nBufferSize,CWnd* pParent)
{
	bool bRes = false;

	CString strCaption;
	if(pParent && ::IsWindow(pParent->GetSafeHwnd()))
	{
		TCHAR szBuffer[1024] = {0};
		pParent->GetWindowText(szBuffer,1024);
		strCaption = szBuffer;
	}

	if(_T("ComfirmYesNo") == strName)
	{
		UINT nID = ::MessageBox(pParent ? pParent->GetSafeHwnd() : NULL,szBuffer,strCaption,MB_YESNO|MB_ICONQUESTION);
		if(nID == IDYES)
			_tcscpy_s(szBuffer,nBufferSize,_T("Yes"));
		else
			_tcscpy_s(szBuffer,nBufferSize,_T("No"));
		bRes = true;
	}
	else if(_T("ComfirmRetryCancel") == strName)
	{
		UINT nID = ::MessageBox(pParent ? pParent->GetSafeHwnd() : NULL,szBuffer,strCaption,MB_RETRYCANCEL|MB_ICONQUESTION);
		if(nID == IDRETRY)
			_tcscpy_s(szBuffer,nBufferSize,_T("Retry"));
		else
			_tcscpy_s(szBuffer,nBufferSize,_T("Cancel"));
		bRes = true;
	}
	else if(_T("ComfirmRetestFail") == strName)
	{
		UINT nID = CBTMessageBox(pParent ? pParent->GetSafeHwnd() : NULL,szBuffer,(LPTSTR)(LPCTSTR)strCaption,"RETEST","FAIL",MB_YESNO|MB_ICONQUESTION);
		if(nID == IDYES)
			_tcscpy_s(szBuffer,nBufferSize,_T("Retest"));
		else
			_tcscpy_s(szBuffer,nBufferSize,_T("Fail"));
		bRes = true;
	}
	else if(_T("ComfirmOK") == strName)
	{
		UINT nID = ::MessageBox(pParent ? pParent->GetSafeHwnd() : NULL,szBuffer,strCaption,MB_OK);
		bRes = true;
	}
	else if(_T("ComfirmRetry") == strName)
	{
		CBTMessageBox(pParent ? pParent->GetSafeHwnd() : NULL,szBuffer,(LPTSTR)(LPCTSTR)strCaption,"Retry","",MB_OK);
		_tcscpy_s(szBuffer,nBufferSize,_T("Retry"));
		bRes = true;
	}
	else if(_T("DEBUGCOMMAND") == strName)
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//For debug script
		//It should display three control buttons.
		//"F5" means Run, "F10" means step by step and "CTRL_BREAK" means break
		//It should display the info from the SPU memory and registers and variables.
		//The szBuffer will include the information from SPU.
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CDebugDlg debugDlg(szBuffer,pParent);
		if(debugDlg.DoModal() == IDOK)
		{	
			if(static_cast<int>(QSYNC_LIB::GetUnicode((LPCTSTR)debugDlg.m_szInput).size()) <= nBufferSize)
			{
				_tcscpy_s(szBuffer,nBufferSize,debugDlg.m_szInput);
				bRes = true;
			}
		}
	}
	else
	{
		CInputDlg InputDialog(strName.c_str(), szBuffer, pParent);
		if(InputDialog.DoModal() == IDOK)
		{	
			if(static_cast<int>(QSYNC_LIB::GetUnicode((LPCTSTR)InputDialog.m_szInput).size()) <= nBufferSize)
			{
				_tcscpy_s(szBuffer,nBufferSize,InputDialog.m_szInput);
				bRes = true;
			}
		}
	}

	return bRes;
}

SPU_EXTEND_UI_API bool SPU_WaitInput(const TSTRING& strName,TCHAR* szBuffer,int nBufferSize,HWND hParent)
{	
	CWnd temWnd;
	temWnd.Attach(hParent);
	bool bRes = SPU_WaitInput_MFC(strName,szBuffer,nBufferSize);
	temWnd.Detach();
	return bRes;
}