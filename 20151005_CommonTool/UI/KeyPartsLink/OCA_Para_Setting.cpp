// OCA_Para_Setting.cpp : implementation file
//

#include "stdafx.h"
#include "KeyPartsLink.h"
#include "OCA_Para_Setting.h"


// COCA_Para_Setting dialog

IMPLEMENT_DYNAMIC(COCA_Para_Setting, CDialog)

COCA_Para_Setting::COCA_Para_Setting(CWnd* pParent /*=NULL*/)
	: CDialog(COCA_Para_Setting::IDD, pParent)
{

}

COCA_Para_Setting::~COCA_Para_Setting()
{
}

void COCA_Para_Setting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COCA_Para_Setting, CDialog)
	ON_BN_CLICKED(IDOK, &COCA_Para_Setting::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL COCA_Para_Setting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	((CEdit*)GetDlgItem(IDC_EDIT_ROLLER_PRESSURE))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_EDIT_ROLLER_PRESSURE))->SetWindowTextW(_T("50.00"));

	((CEdit*)GetDlgItem(IDC_EDIT_Z_AXIS_HEIGHT))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_EDIT_Z_AXIS_HEIGHT))->SetWindowTextW(_T("300.00"));

	((CEdit*)GetDlgItem(IDC_EDIT_MOUNDLAYER_HEIGHT))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_EDIT_MOUNDLAYER_HEIGHT))->SetWindowTextW(_T("200.00"));


	return FALSE;  // return TRUE unless you set the focus to a control

	// EXCEPTION: OCX Property Pages should return FALSE
}

// COCA_Para_Setting message handlers

void COCA_Para_Setting::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	((CEdit*)GetDlgItem(IDC_EDIT_ROLLER_PRESSURE))->GetWindowTextW(m_strRollerPressure);
	((CEdit*)GetDlgItem(IDC_EDIT_Z_AXIS_HEIGHT))->GetWindowTextW(m_strZ_AxisHeight);
	((CEdit*)GetDlgItem(IDC_EDIT_MOUNDLAYER_HEIGHT))->GetWindowTextW(m_strMoundLayerHeight);

	char sz_RollerPressure[32]={0};
	char sz_Z_AxisHeight[32]={0};
	char sz_MoundLayerHeight[32]={0};

	double d_RollerPressure=0;
	double d_Z_AxisHeight=0;
	double d_MoundLayerHeight=0;

	WideCharToMultiByte(CP_ACP,NULL,m_strRollerPressure.GetBuffer(), -1,sz_RollerPressure, m_strRollerPressure.GetLength()*3, NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,m_strZ_AxisHeight.GetBuffer(), -1,sz_Z_AxisHeight, m_strZ_AxisHeight.GetLength()*3, NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,m_strMoundLayerHeight.GetBuffer(), -1,sz_MoundLayerHeight, m_strMoundLayerHeight.GetLength()*3, NULL, NULL);

	d_RollerPressure   = atof(sz_RollerPressure);
	d_Z_AxisHeight     = atof(sz_Z_AxisHeight);
	d_MoundLayerHeight = atof(sz_MoundLayerHeight);

	if(d_RollerPressure <= 0)
	{
			AfxMessageBox(_T("Input RollerPressure Error!"));
			return;
	}
	if(d_Z_AxisHeight <= 0)
	{
			AfxMessageBox(_T("Input Z_AxisHeight Error!"));
			return;
	}
	if(d_MoundLayerHeight <= 0)
	{
			AfxMessageBox(_T("Input MoundLayerHeight Error!"));
			return;
	}
	OnOK();
}
