
// Run_inDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "atlimage.h"
#include "..\..\Modules\Common\CommonUtil\QisdaShopFloor.h"


#define IDC_DYNAMIC_CREATE_STATIC		2000
#define IDC_DYNAMIC_CREATE_EDIT			2100
#define IDC_DYNAMIC_CREATE_CHECKBOX		2200

const COLORREF RDPermissionColor = RGB(155, 114, 186);
const COLORREF PassCaptionColor = RGB(0, 108, 0);

// CRun_inDlg dialog
class CRun_inDlg : public CDialog
{
// Construction
public:
	CRun_inDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_RUN_IN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPassButton();
	afx_msg void OnBnClickedFailButton();
	
	BOOL PreTranslateMessage(MSG* pMsg);
	bool SetUIIDParameter(st_UIIDparameter st_idParameter);

protected:
	void runCIM(bool res);
	bool CallSODialog();
	void ShowLargeMsg(CStringA Msg, CStringA Caption = _T("ERROR"), COLORREF CaptionColor = RGB(255, 0, 0));
	bool InitCIM();
	bool InitUI();
	bool UpdateCSV(CString Picasso, bool Result);
	bool ReadIni(CString &CSVPath, UINT &Duration, UINT &Retry);
	bool OpenCSVFile(CStdioFile &File, CString CSVPath, UINT Retry);
	bool ReadCSV(CStdioFile &File, std::vector<CString> &vFileContent);
	bool FindRecord(std::vector<CString> &vFileContent, CString &RunIn_StartTime, CString Picasso);
	bool CheckTimeStamp(CString RunIn_StartTime, UINT Duration);
	bool UpdateCIM(bool Result);
	bool Check_SO(char* sz_picaso, char *szErrCode, char *szErrMsg);
	bool InitialUITitle();

	/*Sunlight add 20120412*/
	void UpdateEmloyee();

private:
	CString m_cstrSO;
	CString m_cstrEmployee;
	CString m_Toolver;
	CString m_ModulePath;
	CString m_str_titlePicturePath;
	st_UIIDparameter m_st_idParameter;

	CBrush m_brush;

	char m_sz_PCname[15];
	char m_sz_line[50];
	char m_sz_process[50];
	char m_sz_segment[50];

	CStringArray m_PICSItemNameArray;

	int m_PICSItemCount;

	COLORREF m_curColor;

	CQisdaShopFloor m_CIM;

private:
	CStatic *m_PICSItemNameStatic;
	CEdit *m_PICSItemEdit;
	CButton *m_PICSItemCheck;
public:
	CButton m_Pass_Button;
	CButton m_Fail_Button;
	afx_msg void OnClose();
};
