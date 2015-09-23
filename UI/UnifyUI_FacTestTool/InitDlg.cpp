// InitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UnifyUI_FacTestTool.h"
#include "InitDlg.h"


// CInitDlg dialog

IMPLEMENT_DYNAMIC(CInitDlg, CDialog)

CInitDlg::CInitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInitDlg::IDD, pParent)
{
	/* Initial parameter */
	m_st_initData.i_parameterNum = 12;
	m_st_initData.b_tsFlag = false;
	m_st_initData.str_station = _T("");
	m_st_initData.str_firstProcess = _T("");
	m_st_initData.str_modelName = _T("");
	m_st_initData.str_line = _T("");
	m_st_initData.str_pcba = _T("");
	m_st_initData.str_so = DEFAULT_SO;
	m_st_initData.str_partNumber = _T("");
	m_st_initData.str_toolVer = _T("");
	m_st_initData.str_employee = DEFAULT_EMPLOYEE;
	m_st_initData.str_factoryVer = _T("");
	m_st_initData.str_userLoadVer = _T("");
	m_str_station = _T("");
}

CInitDlg::~CInitDlg()
{
}

void CInitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INIT_STATIC_Model, m_static_model);
	DDX_Control(pDX, IDC_INIT_STATIC_PartNumber, m_static_partNumber);
	DDX_Control(pDX, IDC_INIT_STATIC_LoadVer, m_static_loadVer);
	DDX_Control(pDX, IDC_INIT_STATIC_Station, m_static_station);
	DDX_Control(pDX, IDC_INIT_EDIT_Model, m_edit_model);
	DDX_Control(pDX, IDC_INIT_EDIT_PartNumber, m_edit_partNumber);
	DDX_Control(pDX, IDC_INIT_EDIT_LoadVer, m_edit_loadVer);
	DDX_Control(pDX, IDC_INIT_COMBO_Station, m_combobox_station);
	DDX_Control(pDX, IDC_INIT_BUTTON_OK, m_botton_ok);
	DDX_Control(pDX, IDC_INIT_BUTTON_CANCEL, m_botton_cancel);
	DDX_Control(pDX, IDC_INIT_EDIT_EMPLOYEEID, m_edit_employeeid);
	DDX_Control(pDX, IDC_INIT_STATIC_EMPLOYEE, m_statci_employeeid);
}


BEGIN_MESSAGE_MAP(CInitDlg, CDialog)
	ON_BN_CLICKED(IDC_INIT_BUTTON_OK, &CInitDlg::OnBnClickedInitButtonOk)
	ON_BN_CLICKED(IDC_INIT_BUTTON_CANCEL, &CInitDlg::OnBnClickedInitButtonCancel)
	ON_CBN_SELCHANGE(IDC_INIT_COMBO_Station, &CInitDlg::OnCbnSelchangeInitComboStation)
	ON_EN_KILLFOCUS(IDC_INIT_EDIT_Model, &CInitDlg::OnEnKillfocusInitEditModel)
END_MESSAGE_MAP()


// CInitDlg message handlers
bool CInitDlg::GetInitData(st_UIIDparameter& st_dataCopy)
{
	st_dataCopy.i_parameterNum = m_st_initData.i_parameterNum;
	st_dataCopy.b_tsFlag = m_st_initData.b_tsFlag;
	st_dataCopy.str_station.Format(_T("%s"), m_st_initData.str_station);
	st_dataCopy.str_modelName.Format(_T("%s"), m_st_initData.str_modelName);
	st_dataCopy.str_so.Format(_T("%s"), m_st_initData.str_so);
	st_dataCopy.str_partNumber.Format(_T("%s"), m_st_initData.str_partNumber);
	st_dataCopy.str_employee.Format(_T("%s"), m_st_initData.str_employee);
	st_dataCopy.str_userLoadVer.Format(_T("%s"), m_st_initData.str_userLoadVer);
	st_dataCopy.str_factoryVer.Format(_T("%s"), m_st_initData.str_factoryVer);
	st_dataCopy.str_toolVer.Format(_T("%s"), m_st_initData.str_toolVer);
	st_dataCopy.str_imagePath.Format(_T("%s"), m_st_initData.str_imagePath);
	st_dataCopy.str_soType.Format(_T("%s"), m_st_initData.str_soType);
	st_dataCopy.str_segment.Format(_T("%s"), m_st_initData.str_segment);

	return true;
}

BOOL CInitDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	RegistryManager();

	/* Initial procedure */
	if (! InitialProcedure())
	{
		CString str_message = _T("");
		str_message = _T("创建初始化对话框失败!\nCreate initial dialog fail!");
		AfxMessageBox(str_message, MB_OK);
		EndDialog(0);
	}

	//m_edit_loadVer.SetLimitText(6);
	m_edit_partNumber.SetLimitText(15);

	CString str_model = GetRegStringValue(UI_PARAM_MODEL,_T(""));
	if (str_model != _T(""))
	{
		m_edit_model.SetWindowText(str_model);
		m_str_station = GetRegStringValue(UI_PARAM_STATION,_T(""));
		m_combobox_station.InsertString(-1,m_str_station);
		m_combobox_station.SelectString(0,m_str_station);
 		m_st_initData.str_station = m_str_station;
	}
	
	m_edit_partNumber.SetWindowText(GetRegStringValue(UI_PARAM_PARTNO,_T("")));
	m_edit_loadVer.SetWindowText(GetRegStringValue(UI_PARAM_LOADVERSION,_T("")));

	return true;
}

/***************** Initialization functions *******************/

bool CInitDlg::InitialProcedure()
{
	return InitialWidgetProperty();
}

bool CInitDlg::InitialWidgetProperty()
{
	return true;
}

bool CInitDlg::InitialStationComBoBox(CString str_modelName)
{
	TCHAR sz_temp[2048] = {0};
	TCHAR sz_iniFolderName[512] = {0};
	CFileFind file_findCtrl;

	wsprintf(sz_iniFolderName, _T("%s\\%s\\Setting\\UIControl.ini"), CONFIG_PATH, str_modelName);
	if (!file_findCtrl.FindFile(sz_iniFolderName))
	{
		CString str_message;
		str_message.Format(_T("未找到UI ini配置文件!\nCan not find %s!"),sz_iniFolderName);
		AfxMessageBox(str_message, MB_OK);
		return false;
	}

	m_combobox_station.ResetContent();
	TCHAR sz_SectionBuffer[1024]={0};
	int len = GetPrivateProfileSectionNames(sz_SectionBuffer,sizeof(sz_SectionBuffer),sz_iniFolderName);
	TCHAR *p_begin = sz_SectionBuffer;
	TCHAR *p_end = sz_SectionBuffer + len;

	bool bfind = false;
	while(p_end > p_begin)
	{
		CString str_secName = p_begin;
		p_begin += str_secName.GetLength();
		p_begin++;

		if ((str_secName != _T(""))&&(str_secName != _T("COMMON")))
		{
			m_combobox_station.InsertString(-1, str_secName);
		}	

		//find if the station exist in new model
		if (str_secName == m_str_station)
		{
			bfind = true;
		}
	}	

	if(bfind)
	{
		m_combobox_station.SelectString(0,m_str_station);
	}

	return true;
}

void CInitDlg::OnCbnSelchangeInitComboStation()
{
	// TODO: Add your control notification handler code here
	m_combobox_station.GetLBText(m_combobox_station.GetCurSel(), m_st_initData.str_station);

}

/*****************Button dispose function****************/
void CInitDlg::OnBnClickedInitButtonOk()
{
	// TODO: Add your control notification handler code here
	CString str_message = _T("");

	/* Get Model Name */	
	m_edit_model.GetWindowText(m_st_initData.str_modelName);

	/* Get Part Number */
	m_edit_partNumber.GetWindowText(m_st_initData.str_partNumber);

	/* Get loader version */
	m_edit_loadVer.GetWindowText(m_st_initData.str_userLoadVer);
	m_edit_loadVer.GetWindowText(m_st_initData.str_factoryVer);

	/*get employee id*/
	m_edit_employeeid.GetWindowText(m_st_initData.str_employee);

	if (m_st_initData.str_employee == _T(""))
	{
		m_st_initData.str_employee = DEFAULT_EMPLOYEE;
	}

	if (m_st_initData.str_modelName == _T(""))
	{
		str_message = _T("请输入项目名称!\nPlease input mode name!");
		AfxMessageBox(str_message, MB_OK);
		return;
	}

	if (m_st_initData.str_station == _T(""))
	{
		str_message = _T("站别信息不能为空，请重新选择!\nStation info is empty,Please select it once again!");
		AfxMessageBox(str_message, MB_OK);
		return;
	}
	else
	{
#ifndef RD_MODE
		if (m_st_initData.str_partNumber == _T(""))
		{
			str_message = _T("请输入9J料号配置文件信息!\nPlease input part number ini name!");
			AfxMessageBox(str_message, MB_OK);
			return;
		}

		if (m_st_initData.str_userLoadVer == _T(""))
		{
			str_message = _T("请输入下载软体版本!\nPlease input download software version!");
			AfxMessageBox(str_message, MB_OK);
			return;
		}

		if (m_st_initData.str_employee == _T(""))
		{
			str_message = _T("请输入员工ID!\nPlease input employee ID!");
			AfxMessageBox(str_message, MB_OK);
			return;
		}
#else
		if ((m_st_initData.str_station == STATION_IMEI)||
			(m_st_initData.str_station == STATION_FCHECK))
		{
			if (m_st_initData.str_partNumber == _T(""))
			{
				str_message = _T("请输入9J料号配置文件信息!\nPlease input part number ini name!");
				AfxMessageBox(str_message, MB_OK);
				return;
			}

		}
		else if ((m_st_initData.str_station == STATION_TRIGGER)||(m_st_initData.str_station == STATION_DL))
		{
			if (m_st_initData.str_factoryVer == _T(""))
			{
				str_message = _T("请输入下载工厂软体版本!\nPlease input download software version!");
				AfxMessageBox(str_message, MB_OK);
				return;
			}

		}
		else if ((m_st_initData.str_station == STATION_OS_DL)||(m_st_initData.str_station == STATION_REDL)) //2012.08.20 
		{
			if (m_st_initData.str_userLoadVer == _T(""))
			{
				str_message = _T("请输入下载用户软体版本!\nPlease input download software version!");
				AfxMessageBox(str_message, MB_OK);
				return;
			}

		}

#endif

	}

	SetRegStringValue(UI_PARAM_MODEL,m_st_initData.str_modelName);
	SetRegStringValue(UI_PARAM_PARTNO,m_st_initData.str_partNumber);
	SetRegStringValue(UI_PARAM_LOADVERSION,m_st_initData.str_userLoadVer);
// 	CString str_select = _T("");
// 	str_select.Format(_T("%d"),m_combobox_station.GetCurSel());
	SetRegStringValue(UI_PARAM_STATION,m_st_initData.str_station);

	CDialog::OnOK();
}

void CInitDlg::OnBnClickedInitButtonCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

BOOL CInitDlg::PreTranslateMessage(MSG* pMsg)
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

void CInitDlg::RegistryManager(void)
{
	::RegCreateKey(HKEY_CURRENT_USER, _T("Software\\UnifyUI\\EnterParameter"), &m_hRegKey);
}

CString CInitDlg::GetRegStringValue(LPCTSTR lpszKeyName, LPCTSTR lpszDefRegValue)
{
	CString regData;
	DWORD regType, regDataLen = 1000;
	LONG res = ::RegQueryValueEx(m_hRegKey, lpszKeyName, NULL, &regType, (BYTE*)regData.GetBuffer(regDataLen), &regDataLen);

	regData.ReleaseBuffer();
	if (res != ERROR_SUCCESS) {
		regData = lpszDefRegValue;
		::RegSetValueEx(m_hRegKey, lpszKeyName, NULL, REG_SZ, (BYTE*)(LPCTSTR)regData, (regData.GetLength()+1)*sizeof(TCHAR));
	}

	return regData;
}



void CInitDlg::SetRegStringValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue)
{
	if (lpszValue == NULL)
		::RegSetValueEx(m_hRegKey, lpszKeyName, NULL, REG_SZ, (BYTE*)lpszValue, 0);
	else
		::RegSetValueEx(m_hRegKey, lpszKeyName, NULL, REG_SZ, (BYTE*)lpszValue, ((DWORD)_tcslen(lpszValue)+1)*sizeof(TCHAR));
}
void CInitDlg::OnEnKillfocusInitEditModel()
{
	// TODO: Add your control notification handler code here
	CString str_model = _T("");
	m_edit_model.GetWindowText(str_model);
	if (str_model.IsEmpty())
	{
		AfxMessageBox(_T("Please input model name first!\n请输入ModelName"), MB_OK);
		m_edit_model.GetFocus();
		m_combobox_station.ResetContent();
	}
	else
	{
		InitialStationComBoBox(str_model);
	}
	
}
