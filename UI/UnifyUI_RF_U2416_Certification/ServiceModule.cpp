// ServiceModule.cpp : implementation file
//

#include "stdafx.h"
#include "UnifyUI_ServiceTool.h"
#include "ServiceModule.h"
//#include "FuncDownload.h"
//#include "FuncInfoList.h"
//#include "FuncInfoWriter.h"
//#include "FuncFactoryInfoList.h"
#include "RFCertificationTx.h"
#include "CSDWRITE.h"
#include "RFCertificationRx.h"
#include "RFCertificationTxCw.h"
//#include "RFCertification2G4Tx.h"
//#include "RFCertificationTx5G.h"
//#include "RFCertificationRx5G.h"
//#include "RFCertification5GTxCw.h"
#include "KeyPro.h"


// CServiceModule dialog

IMPLEMENT_DYNAMIC(CServiceModule, CDialog)

CServiceModule::CServiceModule(CWnd* pParent /*=NULL*/)
	: CDialog(CServiceModule::IDD, pParent)
{

}

CServiceModule::~CServiceModule()
{
}

void CServiceModule::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MODULE_BUTTON0, m_btnMODULE_BUTTON0);
	DDX_Control(pDX, IDC_MODULE_BUTTON1, m_btnMODULE_BUTTON1);
	DDX_Control(pDX, IDC_MODULE_BUTTON2, m_btnMODULE_BUTTON2);
	DDX_Control(pDX, IDC_MODULE_BUTTON3, m_btnMODULE_BUTTON3);
	DDX_Control(pDX, IDC_MODULE_BUTTON4, m_btnMODULE_BUTTON4);
	DDX_Control(pDX, IDC_MODULE_BUTTON5, m_btnMODULE_BUTTON5);
	DDX_Control(pDX, IDC_MODULE_BUTTON6, m_btnMODULE_BUTTON6);
	DDX_Control(pDX, IDC_MODULE_BUTTON7, m_btnMODULE_BUTTON7);
}


BEGIN_MESSAGE_MAP(CServiceModule, CDialog)
	ON_BN_CLICKED(IDC_MODULE_BUTTON0, &CServiceModule::OnBnClickedModuleButton0)
	ON_BN_CLICKED(IDC_MODULE_BUTTON1, &CServiceModule::OnBnClickedModuleButton1)
	ON_BN_CLICKED(IDC_MODULE_BUTTON2, &CServiceModule::OnBnClickedModuleButton2)
	ON_BN_CLICKED(IDC_MODULE_BUTTON3, &CServiceModule::OnBnClickedModuleButton3)
	ON_BN_CLICKED(IDC_MODULE_BUTTON4, &CServiceModule::OnBnClickedModuleButton4)
	ON_BN_CLICKED(IDC_MODULE_BUTTON5, &CServiceModule::OnBnClickedModuleButton5)
	ON_BN_CLICKED(IDC_MODULE_BUTTON6, &CServiceModule::OnBnClickedModuleButton6)
	ON_BN_CLICKED(IDC_MODULE_BUTTON7, &CServiceModule::OnBnClickedModuleButton7)
END_MESSAGE_MAP()


// CServiceModule message handlers

BOOL CServiceModule::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	//LOGFONT lf;
	//memset(&lf, 0, sizeof(LOGFONT));
	//lf.lfHeight = 18;
	//lf.lfWeight = 500;
	//HFONT font = ::CreateFontIndirect(&lf);
	//for (int i = 0; i < MAX_BUTTON; i++) {
	//	GetDlgItem(IDC_MODULE_BUTTON0+i)->SendMessage(WM_SETFONT, (WPARAM)font, TRUE);
	//}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CServiceModule::OnBnClickedButtonTool(int nIndex)
{
	TRACE(_T("%s index=%d\n"), _T(__FUNCTION__), nIndex);

	if (m_info[nIndex].isLockDongle && CheckKeyPro() != NO_ERROR) {
		return;
	}

	if (m_info[nIndex].type == _T("EXEC")) {
		CString exe_path;
		exe_path.Empty();
		if(0 != exe_path.IsEmpty())
		{
			exe_path = m_info[nIndex].path.Left(m_info[nIndex].path.ReverseFind(_T('\\'))) + _T("\\") + m_info[nIndex].title;
			if (!IsPathExist(exe_path))
			{
				exe_path.Empty();
			}
		}

		if(0 != exe_path.IsEmpty())
		{
			exe_path = m_info[nIndex].path.Left(m_info[nIndex].path.ReverseFind(_T('\\'))) + _T("\\") + m_info[nIndex].name + _T("\\") + m_info[nIndex].title;
		}

		if (!IsPathExist(exe_path)) {
			CString errmsg;
			errmsg.Format(_T("%s not found"), m_info[nIndex].title);
			::AfxMessageBox(errmsg);
			return;
		}
		CString exe_param = m_info[nIndex].info;
		Exec(exe_path, exe_param);
	}

	else if (m_info[nIndex].type == _T("RFCERTIFICATIONTX")) {
		CRFCertificationTx dlg(this);
		dlg.SetInit(m_info[nIndex]);
		dlg.DoModal();
	}
	else if (m_info[nIndex].type == _T("RFCERTIFICATIONRX")) {
		CRFCertificationRx dlg(this);
		dlg.SetInit(m_info[nIndex]);
		dlg.DoModal();
	}
	else if (m_info[nIndex].type == _T("RFCERTIFICATIONTXCWWAVE")) {
		CRFCertificationTxCw dlg(this);
		dlg.SetInit(m_info[nIndex]);
		dlg.DoModal();
	}
	else if (m_info[nIndex].type == _T("CSDWRITE")) {
		CCSDWRITE dlg(this);
		dlg.SetInit(m_info[nIndex]);
		dlg.DoModal();
	}
}

void CServiceModule::OnBnClickedModuleButton0()
{
	OnBnClickedButtonTool(0);
}

void CServiceModule::OnBnClickedModuleButton1()
{
	OnBnClickedButtonTool(1);
}

void CServiceModule::OnBnClickedModuleButton2()
{
	OnBnClickedButtonTool(2);
}

void CServiceModule::OnBnClickedModuleButton3()
{
	OnBnClickedButtonTool(3);
}

void CServiceModule::OnBnClickedModuleButton4()
{
	OnBnClickedButtonTool(4);
}

void CServiceModule::OnBnClickedModuleButton5()
{
	OnBnClickedButtonTool(5);
}

void CServiceModule::OnBnClickedModuleButton6()
{
	OnBnClickedButtonTool(6);
}

void CServiceModule::OnBnClickedModuleButton7()
{
	OnBnClickedButtonTool(7);
}

BOOL MyDataSortPredicate (DllToolInfo i, DllToolInfo j)
{
	return (i.path < j.path);
}

COLORREF ColorRefFromCString(const CString& strValue)
{
	COLORREF retColorRef = 0x000000;
	unsigned int red = 0;
	unsigned int green = 0;
	unsigned int blue = 0;
	TCHAR* strColor = new TCHAR[MAX_PATH];
	memset(strColor, 0, _tcslen(strColor));
	_tcscpy(strColor, strValue.GetString());
	_stscanf_s(strColor, _T("%u,%u,%u"), &red, &green, &blue);
	retColorRef = RGB(red,green,blue);
	return retColorRef;
}

void CServiceModule::SetButtons(std::vector<dlltoolinfo_t>& dlls, std::vector<toolparameter_t>& parameter)
{
	// Sort the vector using predicate and std::sort
	std::sort(dlls.begin(), dlls.end(), MyDataSortPredicate);

	for (size_t nIndex = 0; nIndex < dlls.size(); nIndex++) {
		m_info[nIndex] = dlls.at(nIndex);
	}
	
	for (size_t nIndex = 0; nIndex < parameter.size(); nIndex++) {
		mParam[nIndex] = parameter.at(nIndex);
	}

	for (int i = 0; i < MAX_BUTTON; i++) {
		if (m_info[i].name.GetLength() > 0) {
			GetDlgItem(IDC_MODULE_BUTTON0 + i)->SetWindowText(m_info[i].name);
		}
		else {
			GetDlgItem(IDC_MODULE_BUTTON0 + i)->SetWindowText(NULL);
			GetDlgItem(IDC_MODULE_BUTTON0 + i)->EnableWindow(false);
		}

		for (int j = 0; j < (int)parameter.size(); j++) {
			if(m_info[i].name.Compare(mParam[j].key) == 0) {
				COLORREF crColorRef = ColorRefFromCString(mParam[j].value);

				switch(i)
				{
				case 0:
					m_btnMODULE_BUTTON0.SetTextColor(crColorRef);
					break;
				case 1:
					m_btnMODULE_BUTTON1.SetTextColor(crColorRef);
					break;
				case 2:
					m_btnMODULE_BUTTON2.SetTextColor(crColorRef);
					break;
				case 3:
					m_btnMODULE_BUTTON3.SetTextColor(crColorRef);
					break;
				case 4:
					m_btnMODULE_BUTTON4.SetTextColor(crColorRef);
					break;
				case 5:
					m_btnMODULE_BUTTON5.SetTextColor(crColorRef);
					break;
				case 6:
					m_btnMODULE_BUTTON6.SetTextColor(crColorRef);
					break;
				case 7:
					m_btnMODULE_BUTTON7.SetTextColor(crColorRef);
					break;
				default:
					break;
				}
			}
		}
	}
}

void CServiceModule::SetButtons()
{
	for (int i = 0; i < MAX_BUTTON; i++) {
		if (m_info[i].name.GetLength() > 0) {
			GetDlgItem(IDC_MODULE_BUTTON0 + i)->SetWindowText(m_info[i].name);
		}
		else {
			GetDlgItem(IDC_MODULE_BUTTON0 + i)->SetWindowText(NULL);
			GetDlgItem(IDC_MODULE_BUTTON0 + i)->EnableWindow(false);
		}
	}
}


