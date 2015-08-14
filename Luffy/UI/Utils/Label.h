/***************************************************************************************
* Copyright (C) : QCS/WGC18                                      
* Module name	: Static Control 
* File name     : Label.h             
* Summary       : Source code from Internaet.Alex.Chen Modify                                              
* Version	Author         Date          Abstract                      
* 1.0       Alex.Chen      2011/07/06    First version  
****************************************************************************************/

#if !defined(AFX_LABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_)
#define AFX_LABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif

class CLabel : public CStatic
{
public:
	CLabel();
	virtual ~CLabel();

public:
	CLabel& SetText(const CString& str_text);
	CLabel& SetTextColor(COLORREF dw_textColor);
	CLabel& SetBkColor(COLORREF dw_bkColor);
	CLabel& SetFont(LOGFONT st_lf);
	CLabel& SetFontBold(BOOL b_bold);
	CLabel& SetFontSize(int i_size);
	CLabel& SetFontName(const CString& str_fontName);
	CLabel& SetFontItalic(BOOL b_set);
	CLabel& SetFontUnderline(BOOL b_set);
	CLabel& SetSunken(BOOL b_set);
	CLabel& SetBorder(BOOL b_set);

protected:
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

private:
	void ReconstructFont(void);

private:
	CFont		m_obj_font;               // CFont object
	LOGFONT		m_st_lf;                  // Text logfont
	COLORREF	m_dw_color;               // Text color
	HBRUSH		m_h_brush;                // Brush

};

#endif
