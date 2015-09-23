
#include "stdafx.h"
#include "Label.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CLabel::CLabel()
{
	m_dw_color = GetSysColor(COLOR_WINDOWTEXT);
	m_h_brush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(m_st_lf), &m_st_lf);
	m_obj_font.CreateFontIndirect(&m_st_lf);
}


CLabel::~CLabel()
{
	m_obj_font.DeleteObject();
	::DeleteObject(m_h_brush);
}


BEGIN_MESSAGE_MAP(CLabel, CStatic)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()


HBRUSH CLabel::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	if (CTLCOLOR_STATIC == nCtlColor)
	{
		pDC->SelectObject(&m_obj_font);
		pDC->SetTextColor(m_dw_color);
		pDC->SetBkMode(TRANSPARENT);
	}

	return m_h_brush;
}


CLabel& CLabel::SetText(const CString& str_text)
{
	SetWindowText(str_text);

	return *this;
}


CLabel& CLabel::SetTextColor(COLORREF dw_textColor)
{
	m_dw_color = dw_textColor;
	RedrawWindow();

	return *this;
}


CLabel& CLabel::SetBkColor(COLORREF dw_bkColor)
{
	if (m_h_brush)
	{
		::DeleteObject(m_h_brush);
	}
	m_h_brush = ::CreateSolidBrush(dw_bkColor);
	RedrawWindow();

	return *this;
}


CLabel& CLabel::SetFont(LOGFONT st_lf)
{
	m_obj_font.DeleteObject();
	m_obj_font.CreateFontIndirect(&st_lf);
	RedrawWindow();

	return *this;
}


CLabel& CLabel::SetFontBold(BOOL b_bold)
{	
	m_st_lf.lfWeight = b_bold ? FW_BOLD : FW_NORMAL;
	ReconstructFont();
	RedrawWindow();

	return *this;
}


CLabel& CLabel::SetFontName(const CString& str_fontName)
{	
	_tcscpy_s(m_st_lf.lfFaceName, str_fontName);
	ReconstructFont();
	RedrawWindow();

	return *this;
}


CLabel& CLabel::SetFontSize(int i_size)
{
	i_size *= -1;
	m_st_lf.lfHeight = i_size;
	ReconstructFont();
	RedrawWindow();

	return *this;
}


CLabel& CLabel::SetFontUnderline(BOOL b_set)
{	
	m_st_lf.lfUnderline = b_set;
	ReconstructFont();
	RedrawWindow();

	return *this;
}


CLabel& CLabel::SetFontItalic(BOOL b_set)
{
	m_st_lf.lfItalic = b_set;
	ReconstructFont();
	RedrawWindow();

	return *this;	
}


CLabel& CLabel::SetSunken(BOOL b_set)
{
	if (!b_set) {
		ModifyStyleEx(WS_EX_STATICEDGE, 0, SWP_DRAWFRAME);
	}
	else
	{
		ModifyStyleEx(0, WS_EX_STATICEDGE, SWP_DRAWFRAME);
	}
		
	return *this;	
}


CLabel& CLabel::SetBorder(BOOL b_set)
{
	if (!b_set) {
		ModifyStyle(WS_BORDER,0,SWP_DRAWFRAME);
	}
	else {
		ModifyStyle(0,WS_BORDER,SWP_DRAWFRAME);
	}
		
	return *this;	
}


void CLabel::ReconstructFont()
{
	if(m_obj_font.DeleteObject())
	{
		m_obj_font.CreateFontIndirect(&m_st_lf);  
	}

	return;
}
