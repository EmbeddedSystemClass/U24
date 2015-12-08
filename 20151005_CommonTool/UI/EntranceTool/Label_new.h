#pragma once

class CLabel : public CStatic
{
public:
	CLabel();
	CLabel& SetBkColor(COLORREF crBkgnd);
	CLabel& SetTextColor(COLORREF crText);
	CLabel& SetText(const CString& strText);
	CLabel& SetFontBold(BOOL bBold);
	CLabel& SetFontName(const CString& strFont);
	CLabel& SetFontUnderline(BOOL bSet);
	CLabel& SetFontItalic(BOOL bSet);
	CLabel& SetFontSize(int nSize);
	CLabel& SetSunken(BOOL bSet);
	CLabel& SetBorder(BOOL bSet);
	//CLabel& FlashText(BOOL bActivate);
	//CLabel& FlashBackground(BOOL bActivate);
	CLabel& SetLink(BOOL bLink);
	//CLabel& SetLinkCursor(HCURSOR hCursor);
	~CLabel();



protected:
	COLORREF	m_crText;
	HBRUSH		m_hBrush;
	HBRUSH		m_hwndBrush;
	LOGFONT		m_lf;
	CFont		m_font;
	CString		m_strText;
	BOOL		m_bState;
	BOOL		m_bTimer;
	BOOL		m_bLink;
	HCURSOR		m_hCursor;
	void ReconstructFont();
};
