
#pragma once

#include "afxwin.h"
#include "Label.h"
#include <io.h>
#include "atlimage.h"

#define  INI_FILE_NAME      _T("Run_In_Start.ini")

class CRun_inDlg : public CDialog
{
public:
	CRun_inDlg(CWnd* pParent = NULL);
	virtual ~CRun_inDlg();

	enum { IDD = IDD_RUN_IN_DIALOG };
	bool SetUIIDParameter(st_UIIDparameter st_idParameter);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedPassButton();
	BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

private:
	CBrush   m_brush;
	CLabel   m_static_picasso;
	CLabel   m_static_battery;
	CLabel   m_static_charger;
	CLabel   m_static_usbcable;

	CString  m_str_picasso;
	CString  m_str_battery;
	CString  m_str_charger;
	CString  m_str_usbcable;

	CString  m_str_savePath;
	CString  m_str_titlePicturePath;
	int      m_i_retryCount;
	int      m_i_picassoFlag;
	int      m_i_batteryFlag;
	int      m_i_chargerFlag;
	int      m_i_USBCableFlag;

	st_UIIDparameter m_st_idParameter;

private:
	void InitUIShow(void);
	bool ReadIniFile(void);
	bool WriteStartTimeToFile(CString csTime);
	bool InitialUITitle();

};
