// InitDlg_S3.cpp : implementation file
//

#include "stdafx.h"
#include "UnifyUI_FacTestTool.h"
#include "InitDlg_SO.h"


// CInitDlg_SO dialog

IMPLEMENT_DYNAMIC(CInitDlg_SO, CDialog)

CInitDlg_SO::CInitDlg_SO(CWnd* pParent /*=NULL*/)
	: CDialog(CInitDlg_SO::IDD, pParent)
{
	m_st_initData.str_so = _T("");
	//m_st_initData.str_daynight = _T("");
	//str_type = _T("");
}

CInitDlg_SO::~CInitDlg_SO()
{
}

void CInitDlg_SO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INIT_EDIT_SO, m_edit_so);
	//DDX_Control(pDX, IDC_INIT_COMBO_DAYNIGHT, m_combobox_daynight);
	DDX_Control(pDX, IDC_INIT_BUTTON_OK, m_botton_ok);
	DDX_Control(pDX, IDC_INIT_BUTTON_CANCEL, m_botton_cancel);

}


BEGIN_MESSAGE_MAP(CInitDlg_SO, CDialog)
	ON_BN_CLICKED(IDC_INIT_BUTTON_OK, &CInitDlg_SO::OnBnClickedInitButtonOk)
	ON_BN_CLICKED(IDC_INIT_BUTTON_CANCEL, &CInitDlg_SO::OnBnClickedInitButtonCancel)
	//ON_CBN_SELCHANGE(IDC_INIT_COMBO_DAYNIGHT, &CInitDlg_SO::OnCbnSelchangeInitComboType)
END_MESSAGE_MAP()

BOOL CInitDlg_SO::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	((CEdit*)GetDlgItem(IDC_INIT_EDIT_SO))->SetLimitText(TAG_LENGTH);
	//InitialTypeComBoBox();
	//RegistryManager();

	/* Initial procedure */

		//CString str_message = _T("");
		//str_message = _T("创建初始化对话框失败!\nCreate initial dialog fail!");
		//AfxMessageBox(str_message, MB_OK);
		//EndDialog(0);

	//m_edit_loadVer.SetLimitText(6);
	//m_edit_partNumber.SetLimitText(15);

	//CString str_line = GetRegStringValue(UI_PARAM_MODEL,_T(""));
	//if (str_line != _T(""))
	//{
	//	m_edit_so.SetWindowText(str_line);
	//	m_str_station = GetRegStringValue(UI_PARAM_STATION,_T(""));
	//	m_combobox_daynight.InsertString(-1,m_str_station);
	//	m_combobox_daynight.SelectString(0,m_str_station);
 //		m_st_initData.str_daynight = m_str_station;
	//}
	//
	//m_edit_partNumber.SetWindowText(GetRegStringValue(UI_PARAM_PARTNO,_T("")));
	//m_edit_loadVer.SetWindowText(GetRegStringValue(UI_PARAM_LOADVERSION,_T("")));

	return true;
}





/*****************Button dispose function****************/
void CInitDlg_SO::OnBnClickedInitButtonOk()
{
	// TODO: Add your control notification handler code here
	CString str_message = _T("");

	/* Get Model Name */	
	m_edit_so.GetWindowText(m_st_initData.str_so);

	if (m_st_initData.str_so == _T(""))
	{
		str_message = _T("SO信息不能为空，请重新输入\n Please input power line!");
		AfxMessageBox(str_message, MB_OK);
		return;
	}

	CDialog::OnOK();
}

void CInitDlg_SO::OnBnClickedInitButtonCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

BOOL CInitDlg_SO::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
			return true;
			break;
		case VK_RETURN:
			OnBnClickedInitButtonOk();
			return true;
			break;
		default:
			break;
		}
	default:
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


bool CInitDlg_SO::GetInitData(st_UIIDparameter& st_dataCopy)
{

	st_dataCopy.str_so.Format(_T("%s"), m_st_initData.str_so);
	return true;
}

void CInitDlg_SO::OnEnKillfocusInitEditModel()
{
	// TODO: Add your control notification handler code here
	CString str_so = _T("");
	m_edit_so.GetWindowText(str_so);
	if (str_so.IsEmpty())
	{
		AfxMessageBox(_T("Please input so first!\n请输入SO"), MB_OK);
		m_edit_so.GetFocus();
		//m_combobox_daynight.ResetContent();
	}
	//else
	//{
	//	InitialTypeComBoBox();
	//}
	//
}

// CInitDlg_SO message handlers
