// InitDlg_S3.cpp : implementation file
//

#include "stdafx.h"
#include "UnifyUI_FacTestTool.h"
#include "InitDlg_S3.h"


// CInitDlg_S3 dialog

IMPLEMENT_DYNAMIC(CInitDlg_S3, CDialog)

CInitDlg_S3::CInitDlg_S3(CWnd* pParent /*=NULL*/)
	: CDialog(CInitDlg_S3::IDD, pParent)
{
	m_st_initData.str_so = _T("");
	m_st_initData.str_line = _T("");
	m_st_initData.str_daynight = _T("");
	str_type = _T("");
}

CInitDlg_S3::~CInitDlg_S3()
{
}

void CInitDlg_S3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INIT_EDIT_LINE, m_edit_line);
	DDX_Control(pDX, IDC_INIT_EDIT_SO, m_edit_so_s3);
	DDX_Control(pDX, IDC_INIT_COMBO_DAYNIGHT, m_combobox_daynight);
	DDX_Control(pDX, IDC_INIT_BUTTON_OK, m_botton_ok);
	DDX_Control(pDX, IDC_INIT_BUTTON_CANCEL, m_botton_cancel);

}


BEGIN_MESSAGE_MAP(CInitDlg_S3, CDialog)
	ON_BN_CLICKED(IDC_INIT_BUTTON_OK, &CInitDlg_S3::OnBnClickedInitButtonOk)
	ON_BN_CLICKED(IDC_INIT_BUTTON_CANCEL, &CInitDlg_S3::OnBnClickedInitButtonCancel)
	ON_CBN_SELCHANGE(IDC_INIT_COMBO_DAYNIGHT, &CInitDlg_S3::OnCbnSelchangeInitComboType)
END_MESSAGE_MAP()

BOOL CInitDlg_S3::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	InitialTypeComBoBox();
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
	//	m_edit_line.SetWindowText(str_line);
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

/***************** Initialization functions *******************/

//bool CInitDlg_S3::InitialProcedure()
//{
//	return InitialWidgetProperty();
//}

//bool CInitDlg_S3::InitialWidgetProperty()
//{
//	return true;
//}

bool CInitDlg_S3::InitialTypeComBoBox()
{
	TCHAR sz_temp[2048] = {0};
	TCHAR sz_iniFolderName[512] = {0};
	CString cs_day(_T("DAY 白"));
	CString cs_night(_T("NIGHT 晚"));
	//cs_male = 
	m_combobox_daynight.ResetContent();
	m_combobox_daynight.InsertString(0, cs_day);
	m_combobox_daynight.InsertString(1, cs_night);
	m_combobox_daynight.SelectString(0,str_type);
		
	m_edit_so_s3.SetLimitText(7);// SetLimitText
	//m_combobox_type.ResetContent();
	//TCHAR sz_SectionBuffer[1024]={0};
	//int len = GetPrivateProfileSectionNames(sz_SectionBuffer,sizeof(sz_SectionBuffer),sz_iniFolderName);
	//TCHAR *p_begin = sz_SectionBuffer;
	//TCHAR *p_end = sz_SectionBuffer + len;

	return true;
}

void CInitDlg_S3::OnCbnSelchangeInitComboType()
{
	// TODO: Add your control notification handler code here
	m_combobox_daynight.GetLBText(m_combobox_daynight.GetCurSel(), m_st_initData.str_daynight);

}

/*****************Button dispose function****************/
void CInitDlg_S3::OnBnClickedInitButtonOk()
{
	// TODO: Add your control notification handler code here
	CString str_message = _T("");

	/* Get Model Name */	
	m_edit_line.GetWindowText(m_st_initData.str_line);
	/*get so*/
	m_edit_so_s3.GetWindowText(m_st_initData.str_so);

	if (m_st_initData.str_line == _T(""))
	{
		str_message = _T("信息不能为空，请重新输入\n Please input power line!");
		AfxMessageBox(str_message, MB_OK);
		return;
	}

	if (m_st_initData.str_daynight == _T(""))
	{
		str_message = _T("信息不能为空，请重新选择!\nGender info is empty,Please select it once again!");
		AfxMessageBox(str_message, MB_OK);
		return;
	}

	if (m_st_initData.str_so == _T(""))
	{
		str_message = _T("信息不能为空，请重新输入\n  Please input so !");
		AfxMessageBox(str_message, MB_OK);
		return;
	}

	CDialog::OnOK();
}

void CInitDlg_S3::OnBnClickedInitButtonCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

BOOL CInitDlg_S3::PreTranslateMessage(MSG* pMsg)
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


bool CInitDlg_S3::GetInitData(st_UIIDparameter& st_dataCopy)
{
	//st_dataCopy.i_parameterNum = m_st_initData.i_parameterNum;
	//st_dataCopy.b_tsFlag = m_st_initData.b_tsFlag;
		//	m_st_idParameter.str_line = m_st_idParameter.str_line; 
	st_dataCopy.str_line.Format(_T("%s"), m_st_initData.str_line);
	st_dataCopy.str_so.Format(_T("%s"), m_st_initData.str_so);
	st_dataCopy.str_daynight.Format(_T("%s"), m_st_initData.str_daynight);

	return true;
}
//void CInitDlg_S3::RegistryManager(void)
//{
//	::RegCreateKey(HKEY_CURRENT_USER, _T("Software\\UnifyUI\\EnterParameter"), &m_hRegKey);
//}

//CString CInitDlg_S3::GetRegStringValue(LPCTSTR lpszKeyName, LPCTSTR lpszDefRegValue)
//{
//	CString regData;
//	DWORD regType, regDataLen = 1000;
//	LONG res = ::RegQueryValueEx(m_hRegKey, lpszKeyName, NULL, &regType, (BYTE*)regData.GetBuffer(regDataLen), &regDataLen);
//
//	regData.ReleaseBuffer();
//	if (res != ERROR_SUCCESS) {
//		regData = lpszDefRegValue;
//		::RegSetValueEx(m_hRegKey, lpszKeyName, NULL, REG_SZ, (BYTE*)(LPCTSTR)regData, (regData.GetLength()+1)*sizeof(TCHAR));
//	}
//
//	return regData;
//}
//
//

//void CInitDlg_S3::SetRegStringValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue)
//{
//	if (lpszValue == NULL)
//		::RegSetValueEx(m_hRegKey, lpszKeyName, NULL, REG_SZ, (BYTE*)lpszValue, 0);
//	else
//		::RegSetValueEx(m_hRegKey, lpszKeyName, NULL, REG_SZ, (BYTE*)lpszValue, ((DWORD)_tcslen(lpszValue)+1)*sizeof(TCHAR));
//}
void CInitDlg_S3::OnEnKillfocusInitEditModel()
{
	// TODO: Add your control notification handler code here
	CString str_line = _T("");
	m_edit_line.GetWindowText(str_line);
	if (str_line.IsEmpty())
	{
		AfxMessageBox(_T("Please input model name first!\n请输入ModelName"), MB_OK);
		m_edit_line.GetFocus();
		m_combobox_daynight.ResetContent();
	}
	else
	{
		InitialTypeComBoBox();
	}
	
}

// CInitDlg_S3 message handlers
