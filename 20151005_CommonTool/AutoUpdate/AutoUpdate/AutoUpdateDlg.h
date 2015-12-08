
// AutoUpdateDlg.h : 头文件
//

#pragma once


// CAutoUpdateDlg 对话框
class CAutoUpdateDlg : public CDialog
{
// 构造
public:
	CAutoUpdateDlg(CWnd* pParent = NULL);	// 标准构造函数
	CProgressCtrl m_process;

// 对话框数据
	enum { IDD = IDD_AUTOUPDATE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

  
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedButton1();
};
