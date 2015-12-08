
#include "stdafx.h"
#include "UnifyUI_FacTestTool.h"
#include "ResultDlg.h"

IMPLEMENT_DYNAMIC(CResultDlg, CDialog)

CResultDlg::CResultDlg(CWnd* pParent)
	: CDialog(CResultDlg::IDD, pParent)
{
	m_str_errorCode = _T("");
	m_str_message = _T("");
	m_color_text = RGB(0, 0, 0);
	m_i_textSize = 16;

	m_brush.CreateSolidBrush(RGB(172, 213, 121));
}

CResultDlg::~CResultDlg()
{
	m_brush.DeleteObject();
}

void CResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RESULT_STATIC_CAPTION, m_lable_errorCode);
	DDX_Control(pDX, IDC_RESULT_STATIC_MSG, m_label_message);
}

BEGIN_MESSAGE_MAP(CResultDlg, CDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

HBRUSH CResultDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(nCtlColor == CTLCOLOR_DLG)
	{
         return m_brush;
	}
	
	return hbr;
}

BOOL CResultDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_i_textSize = 16;
	COLORREF cl_backgroud = COLOR_WHITE;
	COLORREF cl_font = COLOR_BLACK;
	if ((m_str_errorCode == _T("PASS")) || (m_str_errorCode == _T("送检OOB")))
	{
		m_brush.DeleteObject();
		m_brush.CreateSolidBrush(BKGCOLOUR_PASS);

		cl_backgroud = BKGCOLOUR_PASS;
		cl_font = COLOR_GREEN;
		
		m_str_message = _T("");		
	}
	else if (m_str_errorCode == _T(""))
	{
		m_brush.DeleteObject();
		m_brush.CreateSolidBrush(BKGCOLOUR_RETEST);

		cl_backgroud = BKGCOLOUR_RETEST;
		cl_font = COLOR_BLUE;
		m_i_textSize = 55;
	}
	else
	{
		m_brush.DeleteObject();
		m_brush.CreateSolidBrush(BKGCOLOUR_FAIL);

		cl_backgroud = BKGCOLOUR_FAIL;
		cl_font = COLOR_RED;	
	}

	m_lable_errorCode.SetBkColor(cl_backgroud);
	m_lable_errorCode.SetTextColor(cl_font);
	m_lable_errorCode.SetFontBold(TRUE);
	m_lable_errorCode.SetFontSize(100);	

	if (m_str_errorCode.GetLength()>15)
	{
		m_lable_errorCode.SetFontSize(30);
	}
	else if (m_str_errorCode.GetLength()>6)
	{
		m_lable_errorCode.SetFontSize(55);
	}

	m_lable_errorCode.SetWindowText(m_str_errorCode);

	m_label_message.SetBkColor(cl_backgroud);
	m_label_message.SetTextColor(cl_font);
	m_label_message.SetFontBold(TRUE);
	m_label_message.SetFontSize(m_i_textSize);
	m_label_message.SetWindowText(m_str_message);

	return TRUE;
}

void CResultDlg::SetText(CString str_errorCode, CString str_message)
{
	m_str_errorCode = str_errorCode;
	m_str_message = str_message;
}

void CResultDlg::SetTextColor(COLORREF color)
{
	m_color_text = color;
}

void CResultDlg::SetTextSize(int i_size)
{
	m_i_textSize = i_size;
}
