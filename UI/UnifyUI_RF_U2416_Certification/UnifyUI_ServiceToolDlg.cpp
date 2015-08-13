// UnifyUI_ServiceToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UnifyUI_ServiceTool.h"
#include "UnifyUI_ServiceToolDlg.h"
#include "ServiceModule.h"
//#include "FuncDownload.h"
#include "Utility.h"
#include <algorithm>

#include "atlimage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUnifyUI_ServiceToolDlg dialog




CUnifyUI_ServiceToolDlg::CUnifyUI_ServiceToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUnifyUI_ServiceToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUnifyUI_ServiceToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MODULE, m_uiTabModule);
}

BEGIN_MESSAGE_MAP(CUnifyUI_ServiceToolDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MODULE, &CUnifyUI_ServiceToolDlg::OnTcnSelchangeTabModule)
	ON_CBN_SELCHANGE(IDC_COMBO_MODULE, &CUnifyUI_ServiceToolDlg::OnCbnSelchangeComboModule)
//	ON_WM_CLOSE()
ON_WM_CLOSE()
END_MESSAGE_MAP()


// CUnifyUI_ServiceToolDlg message handlers

BOOL CUnifyUI_ServiceToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	TCHAR* szKey = _T("{772C516C-48C9-4995-A192-4E0132381F4F}");
	if (!CheckOneInstance(this->m_hWnd, m_mutexOneInstance, szKey)) {
		SendMessage(WM_CLOSE);
		return FALSE;
	}
	::GetModuleFileName(NULL, m_pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	m_pthToolDir.ReleaseBuffer();
	m_pthToolDir = m_pthToolDir.Left(m_pthToolDir.ReverseFind(_T('\\')));
	m_nProjectSelected = -1;
	m_nTabSelected = -1;

	UITabSetup();
	UITitleSetup();

	// Show Banner
	CString pth_banner = m_pthToolDir + _T("\\Banner.png");
	CImage imgObj ;
	imgObj.Load(pth_banner) ;
	((CStatic*)GetDlgItem(IDC_PIC_BANNER))->SetBitmap (imgObj);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUnifyUI_ServiceToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUnifyUI_ServiceToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CUnifyUI_ServiceToolDlg::UITabSetup()
{
	UIModuleSetup();
	for (size_t nIndex = 0; nIndex < m_projects.size(); nIndex++) {
		CComboBox* uiProject = (CComboBox*)GetDlgItem(IDC_COMBO_MODULE);
		uiProject->AddString(m_projects.at(nIndex).name);
	}
}

void CUnifyUI_ServiceToolDlg::UITitleSetup()
{
	// Use .exe name as title
	CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('.'));

	m_strTitle = PathFindFileName (pthToolDir);
	SetWindowText (m_strTitle);
}

size_t CUnifyUI_ServiceToolDlg::GetToolInfo(CString scan_path,	std::vector<toolproj_t>& projects)
{
	if (!IsPathExist(scan_path)) {
		return 0;
	}
	scan_path += _T("\\*.*");

	CFileFind Finder;
	BOOL isFound = Finder.FindFile(scan_path);
	while (isFound) {
		isFound = Finder.FindNextFile();
		if (Finder.IsDots()) {
			continue;
		}
		if (!Finder.IsDirectory()) {
			continue;
		}
		toolproj_t project;
		project.name = Finder.GetFileTitle();
		project.path = Finder.GetFilePath();
		GetTabs(project.path, project.tabs);
		projects.push_back(project);
		TRACE(_T("project dir add: %s\n"), Finder.GetFileTitle());
	}
	TRACE(_T("dirs size: %d\n"), projects.size());
	Finder.Close();

	return projects.size();
}

size_t CUnifyUI_ServiceToolDlg::GetTabs(CString scan_path,	std::vector<tooltablist_t>& tabs)
{
	if (!IsPathExist(scan_path)) {
		return 0;
	}
	scan_path += _T("\\*.*");

	CFileFind Finder;
	BOOL isFound = Finder.FindFile(scan_path);
	while (isFound) {
		isFound = Finder.FindNextFile();
		if (Finder.IsDots()) {
			continue;
		}
		if (Finder.IsDirectory()) {
			tooltablist_t tool_tab;
			tool_tab.name = Finder.GetFileTitle();
			tool_tab.path = Finder.GetFilePath();
			GetDlls(tool_tab.path, tool_tab.buttons, tool_tab.parameter);
			tabs.push_back(tool_tab);
			TRACE(_T("dir add: %s\n"), Finder.GetFileTitle());
		}
		else {
			// do nothing
		}
	}
	TRACE(_T("dirs size: %d\n"), tabs.size());
	Finder.Close();

	return tabs.size();
}

size_t CUnifyUI_ServiceToolDlg::GetDlls(CString& path, std::vector<dlltoolinfo_t>& dlls, std::vector<toolparameter_t>& parameter)
{
	CString scan_path = path;
	if (!IsPathExist(scan_path)) {
		return 0;
	}
	scan_path += _T("\\*.dll");

	CFileFind Finder;
	BOOL isFound = Finder.FindFile(scan_path);
	while (isFound) {
		isFound = Finder.FindNextFile();
		if (Finder.IsDots()) {
			continue;
		}
		if (Finder.IsDirectory()) {
			continue;
		}
		dlltoolinfo_t data;
		toolparameter_t param;
		if (CheckQisdaDll(Finder.GetFilePath(), data, param)){
			if (data.name.IsEmpty()) {
				data.name = Finder.GetFileTitle();
			}
			data.path = Finder.GetFilePath();
			dlls.push_back(data);
			TRACE(_T("dir add: %s\n"), Finder.GetFileTitle());

			if (!param.key.IsEmpty())
			{
				parameter.push_back(param);
			}
		}
	}
	TRACE(_T("dirs size: %d\n"), dlls.size());
	Finder.Close();

	return dlls.size();
}

/**
 * process
 * 1. scan tool/Module/ to get project lists
 * 2. scan tool/Module/<project>/ to get dll lists
 */
void CUnifyUI_ServiceToolDlg::UIModuleSetup()
{
	GetToolInfo(m_pthToolDir + _T("\\Module"), m_projects);
}

typedef bool (*fGetQisdaModuleInfo)(char* szName, char* szTitle, char* szType, char* szInfo, bool* isDongleLock);
typedef int (*fGetParameter)(HANDLE hResource, char* szKey, char* szValue);

bool CUnifyUI_ServiceToolDlg::CheckQisdaDll(CString& path_dll, dlltoolinfo_t& data, toolparameter_t& param)
{
	if (!IsPathExist(path_dll)) {
		return false;
	}

	TCHAR szNowPath[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szNowPath);
	TRACE(_T("current path: %s\n"), szNowPath);
	CString WorkDir = path_dll.Left(path_dll.ReverseFind(_T('\\')));
	TRACE(_T("working path: %s\n"), WorkDir);
	SetCurrentDirectory(WorkDir);

	HMODULE hDll = ::LoadLibrary(path_dll);
	if(!hDll) {
		return false;
	}
	fGetQisdaModuleInfo GetQisdaModuleInfo =
		(fGetQisdaModuleInfo)GetProcAddress(hDll, "GetQisdaModuleInfo");
	if (GetQisdaModuleInfo == NULL) {
		// skip
		::FreeLibrary(hDll);
		return false;
	}

	char szName[MAX_PATH] = {0};
	char szTitle[MAX_PATH] = {0};
	char szType[MAX_PATH] = {0};
	char szInfo[MAX_PATH] = {0};
	bool* isDongleLock = new bool;
	memset(szName, 0, MAX_PATH);
	memset(szTitle, 0, MAX_PATH);
	memset(szType, 0, MAX_PATH);
	memset(szInfo, 0, MAX_PATH);
	GetQisdaModuleInfo(szName, szTitle, szType, szInfo, isDongleLock);
	data.name = szName;
	data.title = szTitle;
	data.type = szType;
	data.info = szInfo;
	data.isLockDongle = *isDongleLock;

	fGetParameter GetParameter = (fGetParameter)GetProcAddress(hDll, "GetParameter");
	if(GetParameter != NULL)
	{
		HANDLE hResource = NULL;
		char szValue[MAX_PATH];
		memset(szValue, 0, MAX_PATH);
		if(GetParameter(hResource, "ColorChange", szValue) == NO_ERROR)
		{
			if(strlen(szValue) != 0)
			{
				param.key = szName;
				param.value = szValue;
			}
		}
	}

	::FreeLibrary(hDll);
	SetCurrentDirectory(szNowPath);
	delete isDongleLock;

	return true;
}
void CUnifyUI_ServiceToolDlg::OnTcnSelchangeTabModule(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

//	char info[20] = {0};
//	bool isValid = false;
//#ifdef _DEBUG
//	isValid = true;
//#endif //_DEBUG

	m_nTabSelected = m_uiTabModule.GetCurSel();
	for (size_t nTab = 0; nTab < m_projects.at(m_nProjectSelected).tabs.size(); nTab++) {
		m_uiTabs.at(nTab)->ShowWindow(m_nTabSelected == nTab ? SW_SHOW : SW_HIDE);
		if (m_nTabSelected == nTab) {
			m_uiTabs.at(nTab)->SetButtons();
		}
	}
}

void CUnifyUI_ServiceToolDlg::OnCbnSelchangeComboModule()
{
	// TODO: Add your control notification handler code here
	for (std::vector<CServiceModule*>::iterator it = m_uiTabs.begin(); it != m_uiTabs.end(); it++) {
		delete (*it);
	}
	m_uiTabs.clear();
	m_uiTabModule.DeleteAllItems();

	CString project_name;
	GetDlgItem(IDC_COMBO_MODULE)->GetWindowText(project_name);
	for (size_t i = 0; i < m_projects.size(); i++) {
		if (project_name == m_projects.at(i).name) {
			m_nProjectSelected = static_cast<int>(i);
			break;
		}
	}
	if (m_nProjectSelected != -1) {
		// tab
		for (size_t i = 0; i < m_projects.at(m_nProjectSelected).tabs.size() ; i++) {
			CServiceModule* tab = new CServiceModule;
			m_uiTabs.push_back(tab);
			tab->Create(m_uiTabs.at(i)->IDD, &m_uiTabModule);
			m_uiTabModule.InsertItem(static_cast<int>(i), m_projects.at(m_nProjectSelected).tabs.at(i).name);
			tab->SetButtons(m_projects.at(m_nProjectSelected).tabs.at(i).buttons, m_projects.at(m_nProjectSelected).tabs.at(i).parameter);
			tab->SetWindowPos(NULL, 4, 32, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER);
			tab->ShowWindow(i == 0 ? SW_SHOW : SW_HIDE);
		}

		// Title -- add Project name and read version from /Module/<project>/<project>.ini
		CString projname = m_projects[m_nProjectSelected].name;

		CString title;
	/*	GetWindowText (title);
		title += _T(" ") + projname;*/

		title = m_strTitle +_T(" ") + projname;

		CString cstrVersion;
		CString inifile = m_pthToolDir + _T("\\Module\\") + projname + _T("\\") + projname + _T(".ini");
		DWORD nSize = GetPrivateProfileString (_T("Tool_Version"), _T("Version"), _T(""), cstrVersion.GetBuffer(256), 256, inifile);
		cstrVersion.ReleaseBuffer();
		if (nSize != 0)
			title += _T(" - ") + cstrVersion;

		SetWindowText (title); 
	}
}

void CUnifyUI_ServiceToolDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	for (unsigned int i = 0; i < m_uiTabs.size(); ++i){
		delete m_uiTabs[i];
	}
	CDialog::OnClose();
}
