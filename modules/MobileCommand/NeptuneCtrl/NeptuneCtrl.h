// NeptuneCtrl.h : main header file for the NEPTUNECTRL DLL
//

#if !defined(AFX_NEPTUNECTRL_H__55621C79_343E_4F50_B554_A3ED4394BC0A__INCLUDED_)
#define AFX_NEPTUNECTRL_H__55621C79_343E_4F50_B554_A3ED4394BC0A__INCLUDED_

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
// CNeptuneCtrlApp
// See NeptuneCtrl.cpp for the implementation of this class
//

class CNeptuneCtrlApp : public CWinApp
{
public:
	CNeptuneCtrlApp();
	~CNeptuneCtrlApp();

	CmdCtrl* m_pCmdCtrl[QTH_PORT_LAST];

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNeptuneCtrlApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CNeptuneCtrlApp)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEPTUNECTRL_H__55621C79_343E_4F50_B554_A3ED4394BC0A__INCLUDED_)
