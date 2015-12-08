#pragma once


// CLargeMsgBox dialog

#include "Label.h"
#include "..\DialogResources\resource.h"

class CLargeMsgBox : public CDialog
{
	DECLARE_DYNAMIC(CLargeMsgBox)

public:
	CLargeMsgBox (CWnd* pParent = NULL);   // standard constructor
	CLargeMsgBox (CString &inMsg, CWnd* pParent = NULL);
	virtual ~CLargeMsgBox();

	void SetCaptionColor (COLORREF color);
	void SetMessageColor (COLORREF color);
	
	void SetCaptionSize (int nSize);
	void SetMessageSize (int nSize);

	void SetCaptionBold (BOOL bBold);
	void SetMessageBold (BOOL bBold);
	
	void SetCaption (CString& inMsg);
	void SetMessage (CString& inMsg);

// Dialog Data
protected:
	enum { IDD = IDD_LARGE_MSG_DIALOG_DIALOGS_DLL };
	CLabel   m_MessageLabel;
	CLabel   m_CaptionLabel;

	CString  m_MessageString;
	CString  m_CaptionString;

	COLORREF m_MessageColor;
	COLORREF m_CaptionColor;

	int      m_MessageSize;
	int		 m_CaptionSize;

	BOOL	 m_CaptionBold;
	BOOL	 m_MessageBold;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
