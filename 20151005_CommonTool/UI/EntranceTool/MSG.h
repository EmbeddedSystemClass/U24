#pragma once


// MSG dialog

class MSG : public CDialog
{
	DECLARE_DYNAMIC(MSG)

public:
	MSG(CWnd* pParent = NULL);   // standard constructor
	virtual ~MSG();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
