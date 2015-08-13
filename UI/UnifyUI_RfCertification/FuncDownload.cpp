// FuncDownload.cpp : implementation file
//

#include "stdafx.h"
#include "UnifyUI_ServiceTool.h"
#include "FuncDownload.h"
#include "VersionInfo.h"
#include "ServiceCheck.h"


CFuncDownload* g_ui;

// CFuncDownload dialog

IMPLEMENT_DYNAMIC(CFuncDownload, CDialog)

CFuncDownload::CFuncDownload(CWnd* pParent /*=NULL*/)
	: CDialog(CFuncDownload::IDD, pParent)
{

}

CFuncDownload::~CFuncDownload()
{
}

void CFuncDownload::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFuncDownload, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CFuncDownload::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BUTTON_FOLDER, &CFuncDownload::OnBnClickedButtonFolder)
END_MESSAGE_MAP()


// CFuncDownload message handlers

BOOL CFuncDownload::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_eventTimer = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_thrdTimer = NULL;

	if (!m_dllCtrl.Load(m_data.path)) {
		CString msg;
		msg.Format(_T("Cannot load dll: %s"), m_data.path);
		::AfxMessageBox(msg);
		SendMessage(WM_CLOSE);
	}
	m_dllCtrl.SetCallBackMsg(MsgCall);
	m_dllCtrl.SetCallBackPort(UpdatePortCall);

	g_ui = this;

	::GetModuleFileName(NULL, m_pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	m_pthToolDir.ReleaseBuffer();
	m_pthToolDir = m_pthToolDir.Left(m_pthToolDir.ReverseFind('\\'));

	UIInit();

	if ((m_data.info.Find(_T("KEYLOCK")) != -1)) {
		CString FullMask = _T("C:\\*.qsrvk");
		CString path_key;
		if (IsFindFilePath(FullMask, path_key)) {
			path_key = _T("C:\\") + path_key;
			if(service_key_check(CT2A(path_key.GetBuffer())) == NO_ERROR) {
				// check ok, do nothing
			}
			else {
				::AfxMessageBox(_T("invalid key\nCannot open function"));
				SendMessage(WM_CLOSE);
			}
			path_key.ReleaseBuffer();
		}
		else {
			::AfxMessageBox(_T("no key\nCannot open function"));
			SendMessage(WM_CLOSE);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CFuncDownload::DestroyWindow()
{
	m_dllCtrl.UnLoad();
	return CDialog::DestroyWindow();
}

void CFuncDownload::OnBnClickedButtonRun()
{
	PrintMsg(_T(""), _T("CLEAR"), false);
	m_thrdMainProgress = ::AfxBeginThread(WorkerThreadFuncProc, this, THREAD_PRIORITY_ABOVE_NORMAL);
}

void CFuncDownload::OnBnClickedButtonFolder()
{
	// TODO: Add your control notification handler code here
	CString pthFolder;
	TCHAR szPath[MAX_PATH] = {0};
	GetDlgItem(IDC_EDIT_PATH)->GetWindowText(szPath, MAX_PATH);

	BROWSEINFO bi = {0};
	bi.hwndOwner = m_hWnd;
	bi.pszDisplayName = szPath; // can not be NULL
	bi.lpszTitle = _T("Select Folder");
	//bi.pidlRoot = GetItemIdList(szPath);
	//bi.lpfn = BrowseCallbackProc;					// callback function
	LPITEMIDLIST pIdList = SHBrowseForFolder(&bi);	// display dialog
	if (pIdList != NULL) {
		if (SHGetPathFromIDList(pIdList, szPath)) {
			pthFolder = szPath; // save path
		}
		else {
			return;
		}
	}
	else {
		return;
	}
	GetDlgItem(IDC_EDIT_PATH)->SetWindowText(pthFolder);
	UpdateData(FALSE);
	LPMALLOC lpMalloc;
	if (FAILED(SHGetMalloc(&lpMalloc))) {
		return;
	}
	lpMalloc->Free(pIdList);
	lpMalloc->Release();
	pthFolder += _T("\\");

	GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(true);
}

void CFuncDownload::SetInit(dlltoolinfo_t& data)
{
	m_data = data;
}

int CFuncDownload::MsgCall(const char* szMsg, const char* szTag)
{
	CString msg, tag;
	if (szMsg != NULL) {
		msg.Format(_T("%s"), CA2T(szMsg, GetACP()));
	}
	if (szTag != NULL) {
		tag.Format(_T("%s"), CA2T(szTag, GetACP()));
	}
	g_ui->PrintMsg(msg, tag);

	return 0;
}

void CFuncDownload::PrintMsg(CString message, CString tag, bool isLog)
{
	CRichEditCtrl* rchCtrl = reinterpret_cast<CRichEditCtrl*>(GetDlgItem(IDC_RICHEDIT_MESSAGE));
	CHARFORMAT cf;
	memset(&cf, 0, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR | CFM_BOLD;

	if (tag == _T("PASS")) {
		cf.crTextColor = RGB(0, 0xcc, 0);
		rchCtrl->SetBackgroundColor(FALSE, RGB(0x90, 0xee, 0x90));
	}
	else if (tag == _T("FAIL")) {
		cf.crTextColor = RGB(0x8b, 0, 0);
		cf.dwEffects = CFE_BOLD;
		rchCtrl->SetBackgroundColor(FALSE, RGB(0xf0, 0x80, 0x80));
	}
	else if (tag == _T("WARN")) {
		cf.crTextColor = RGB(0, 0, 0xcc);
	}
	else if (tag == _T("CLEAR")) {
		cf.crTextColor = RGB(0, 0, 0);
		rchCtrl->SetWindowText(NULL);
		rchCtrl->SetBackgroundColor(FALSE, RGB(0xff, 0xff, 0xff));
	}
	else {
		cf.crTextColor = RGB(0, 0, 0);
	}
	rchCtrl->SetSelectionCharFormat(cf);

	int nStart, nEnd;
	CString csProgress;
	rchCtrl->GetWindowText(csProgress);
	nStart = nEnd = csProgress.GetLength();

	int ProgressPos = csProgress.Find(_T("Progress"), ((nEnd - 30) > 0 ? (nEnd - 30) : 0));
	int LineCount = csProgress.Remove(0xa);
	if (ProgressPos > 0) {
		nStart = ProgressPos - LineCount + 1;	//// just try
		nEnd = -1;
	}
	rchCtrl->SetSel(nStart, nEnd);
	rchCtrl->ReplaceSel(message);
	rchCtrl->PostMessage(WM_VSCROLL, SB_BOTTOM, 0);

	if (!isLog) {
		return;
	}
	if (message.Find(_T("Progress:")) != -1) {
		message.Replace(_T("Progress:"), _T(""));
		message.Replace(_T("% completed"),  _T(""));
		message.Trim();
		message += (message.Find(_T("100")) != -1) ? _T("\n") : _T(" ");
	}
	CString pthLog;
	pthLog.Format(_T("%s\\%s.log"), m_pthToolDir, CA2T(TOOL_NAME));
	CStdioFile fLog;
	UINT nOpenFlags =
		CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyWrite;
	if (fLog.Open(pthLog, nOpenFlags)) {
		fLog.SeekToEnd();
		fLog.WriteString(message);
		fLog.Close();
	}
	Invalidate(FALSE);
}

int CFuncDownload::UpdatePortCall(const char* szPort)
{
	CString port;
	if (szPort != NULL) {
		port.Format(_T("%s"), CA2T(szPort));
	}
	g_ui->SetPortUpdate(port);

	return 0;
}

void CFuncDownload::SetPortUpdate(CString port)
{
	CComboBox* ccb = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
	ccb->ResetContent();
	ccb->AddString(port);
	ccb->SetCurSel(0);
	Invalidate(FALSE);
}

void CFuncDownload::UIInit()
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 64;
	lf.lfWeight = 400;
	HFONT font = ::CreateFontIndirect(&lf);
	GetDlgItem(IDC_BUTTON_RUN)->SendMessage(WM_SETFONT, (WPARAM)font, TRUE);

	CString title;
	title = m_data.title;
	SetWindowText(title);

	const int MAX_MSG = 2048;
	char buffer[MAX_MSG];
	memset(buffer, 0, MAX_MSG);
	if (m_dllCtrl.GetParameter("msg_process", buffer) == NO_ERROR) {
		GetDlgItem(IDC_EDIT_PROCESS)->SetWindowText(CA2T(buffer));
	}
	memset(buffer, 0, MAX_MSG);
	if (m_dllCtrl.GetParameter("msg_warning", buffer) == NO_ERROR) {
		CString msg = CA2T(buffer);
		UIWarning(msg);
	}

	int nPort = m_dllCtrl.GetPortCount();
	CComboBox* ccb = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
	CString port;
	for (int nIndex = 0; nIndex < nPort; nIndex++) {
		memset(buffer, 0, MAX_MSG);
		m_dllCtrl.GetPortByIndex(nIndex, buffer);
		port = CA2T(buffer);
		ccb->AddString(port);
	}
}

void CFuncDownload::UIWarning(CString message)
{
	CHARFORMAT cf;
	memset(&cf, 0, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR | CFM_BOLD | CFM_SIZE;
	cf.crTextColor = RGB(0xf0, 0, 0);
	cf.dwEffects = CFE_BOLD;
	cf.yHeight = 360;
	CRichEditCtrl* eWarn = (CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT_WARNING);
	eWarn->SetSelectionCharFormat(cf);
	if (message.IsEmpty()) {
		message =
			_T("PLEASE DO NOT REMOVE USB CABLE FROM DEVICE OR PC before download finishes");
	}
	int nStart, nEnd;
	nStart = nEnd = 0;
	eWarn->SetSel(nStart, nEnd);
	eWarn->ReplaceSel(message);
}

void CFuncDownload::UIControl(bool isEnable)
{
	GetDlgItem(IDC_COMBO_PORT)->EnableWindow(isEnable);
	GetDlgItem(IDC_BUTTON_FOLDER)->EnableWindow(isEnable);
	GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(isEnable);
}

void CFuncDownload::UITotalTime(double secTime)
{
	CString csTime;
	csTime.Format(_T("%.2f sec"), secTime);
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText(csTime);
}

void CFuncDownload::Timer(bool isEnable)
{
	if (isEnable) {
		if (m_thrdTimer == NULL) {
			ResetEvent(m_eventTimer);
			m_thrdTimer = ::AfxBeginThread(WorkerThreadTimerProc, this);
		}
	}
	else {
		SetEvent(m_eventTimer);
		::WaitForSingleObject(m_thrdTimer, INFINITE);
		m_thrdTimer = NULL;
	}
}

UINT CFuncDownload::WorkerThreadTimerProc(LPVOID param)
{
	CFuncDownload* pThis = reinterpret_cast<CFuncDownload*>(param);
	ASSERT(pThis != NULL);

	pThis->WorkerThreadTimerRun();

	return TRUE;
}

void CFuncDownload::WorkerThreadTimerRun()
{
	clock_t start, now;
	start = clock();
	do {
		now = clock();
		double elapse_time = static_cast<double>(now - start) / CLOCKS_PER_SEC;
		UITotalTime(elapse_time);
		Sleep(20);
	} while (WaitForSingleObject(m_eventTimer, 0) == WAIT_TIMEOUT);
}

UINT CFuncDownload::WorkerThreadFuncProc(LPVOID param)
{
	CFuncDownload* pThis = reinterpret_cast<CFuncDownload*>(param);
	ASSERT(pThis != NULL);

	pThis->WorkerThreadFuncRun();

	return TRUE;
}

void CFuncDownload::WorkerThreadFuncRun()
{
	UIControl(false);
	Timer(true);
	CString path;
	GetDlgItem(IDC_EDIT_PATH)->GetWindowText(path);
	m_dllCtrl.SetParameter("image_path", CT2A(path.GetBuffer()));
	path.ReleaseBuffer();
	CString port;
	GetDlgItem(IDC_COMBO_PORT)->GetWindowText(port);
	m_dllCtrl.SetParameter("port", CT2A(port.GetBuffer()));
	port.ReleaseBuffer();

	int nRetCode = m_dllCtrl.PreRun();
	if (nRetCode == NO_ERROR) {
		nRetCode = m_dllCtrl.Run();
	}
	if (nRetCode == NO_ERROR) {
		nRetCode = m_dllCtrl.PostRun();
	}

	Timer(false);
	CString csTime;
	GetDlgItem(IDC_STATIC_TIME)->GetWindowText(csTime);
	csTime = _T("Elapsed Time: ") + csTime + _T("\n\n");
	PrintMsg(csTime, _T(""));
	UIControl(true);
}


