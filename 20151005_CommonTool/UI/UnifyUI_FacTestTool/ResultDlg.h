#pragma once

#include "Label.h"

#define BKGCOLOUR_RETEST			RGB(255,255,153)
#define BKGCOLOUR_PASS				RGB(215, 240, 198)
#define BKGCOLOUR_FAIL				RGB(215, 200, 238)
#define COLOR_WHITE					RGB(255, 255, 255)
#define COLOR_BLACK					RGB(0, 0, 0)
#define COLOR_BLUE					RGB(0, 0, 255)
#define COLOR_RED					RGB(255, 0, 0)
#define COLOR_GREEN					RGB(0, 255, 0)

class CResultDlg : public CDialog
{
	DECLARE_DYNAMIC(CResultDlg)

public:
	CResultDlg(CWnd* pParent = NULL);
	virtual ~CResultDlg();
	enum { IDD = IDD_DIALOG_RESULT };

public:
	void SetText(CString str_errorCode, CString str_message = _T(""));
	void SetTextColor(COLORREF color);
	void SetTextSize(int i_size);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	CBrush      m_brush;
	CLabel      m_lable_errorCode;
	CLabel      m_label_message;

	CString     m_str_errorCode;
	CString     m_str_message;
	COLORREF    m_color_text;
	int         m_i_textSize;
};
