// QisdaAndroidCmdCtrl.h : main header file for the QisdaAndroidCmdCtrl DLL
//

#if !defined(AFX_QisdaAndroidCmdCtrl_H__55621C79_343E_4F50_B554_A3ED4394BC0A__INCLUDED_)
#define AFX_QisdaAndroidCmdCtrl_H__55621C79_343E_4F50_B554_A3ED4394BC0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "CmdCtrl.h"
#include "CmdType.h"

/////////////////////////////////////////////////////////////////////////////
// CQisdaAndroidCmdCtrlApp
// See QisdaAndroidCmdCtrl.cpp for the implementation of this class
//

class CQisdaAndroidCmdCtrlApp : public CWinApp
{
public:
	CQisdaAndroidCmdCtrlApp();
	~CQisdaAndroidCmdCtrlApp();
	DISALLOW_COPY_AND_ASSIGN(CQisdaAndroidCmdCtrlApp);

	CmdCtrl* m_pCmdCtrl[QTH_PORT_MAX];

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQisdaAndroidCmdCtrlApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CQisdaAndroidCmdCtrlApp)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QisdaAndroidCmdCtrl_H__55621C79_343E_4F50_B554_A3ED4394BC0A__INCLUDED_)
