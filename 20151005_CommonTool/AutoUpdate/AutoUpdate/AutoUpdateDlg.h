
// AutoUpdateDlg.h : ͷ�ļ�
//

#pragma once


// CAutoUpdateDlg �Ի���
class CAutoUpdateDlg : public CDialog
{
// ����
public:
	CAutoUpdateDlg(CWnd* pParent = NULL);	// ��׼���캯��
	CProgressCtrl m_process;

// �Ի�������
	enum { IDD = IDD_AUTOUPDATE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

  
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedButton1();
};
