// FuncFactoryInfoList.cpp : implementation file
//

#include "stdafx.h"
#include "UnifyUI_ServiceTool.h"
#include "FuncFactoryInfoList.h"
#include "VersionInfo.h"
#include "KeyPro.h"


CFuncFactoryInfoList* g_ui;

// CFuncFactoryInfoList dialog

IMPLEMENT_DYNAMIC(CFuncFactoryInfoList, CDialog)

CFuncFactoryInfoList::CFuncFactoryInfoList(CWnd* pParent /*=NULL*/)
	: CDialog(CFuncFactoryInfoList::IDD, pParent)
{

}

CFuncFactoryInfoList::~CFuncFactoryInfoList()
{
}

void CFuncFactoryInfoList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFuncFactoryInfoList, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CFuncFactoryInfoList::OnBnClickedButtonRun)
END_MESSAGE_MAP()


// CFuncFactoryInfoList message handlers
BOOL CFuncFactoryInfoList::OnInitDialog()
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
	m_dllCtrl.SetCallBackResult(ResultCall);

	g_ui = this;

	::GetModuleFileName(NULL, m_pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	m_pthToolDir.ReleaseBuffer();
	m_pthToolDir = m_pthToolDir.Left(m_pthToolDir.ReverseFind('\\'));

	UIInit();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CFuncFactoryInfoList::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	m_dllCtrl.UnLoad();

	return CDialog::DestroyWindow();
}

void CFuncFactoryInfoList::OnBnClickedButtonRun()
{
	// TODO: Add your control notification handler code here
	PrintMsg(_T(""), _T("CLEAR"), false);
	m_thrdMainProgress = ::AfxBeginThread(WorkerThreadFuncProc, this, THREAD_PRIORITY_ABOVE_NORMAL);
}

void CFuncFactoryInfoList::SetInit(dlltoolinfo_t& data)
{
	m_data = data;
}

int CFuncFactoryInfoList::MsgCall(const char* szMsg, const char* szTag)
{
	CString msg, tag;
	if (szMsg != NULL) {
		msg.Format(_T("%s"), CA2T(szMsg));
	}
	if (szTag != NULL) {
		tag.Format(_T("%s"), CA2T(szTag));
	}
	g_ui->PrintMsg(msg, tag);

	return 0;
}

void CFuncFactoryInfoList::PrintMsg(CString message, CString tag, bool isLog)
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

int CFuncFactoryInfoList::UpdatePortCall(const char* szPort)
{
	CString port;
	if (szPort != NULL) {
		port.Format(_T("%s"), CA2T(szPort));
	}
	g_ui->SetPortUpdate(port);

	return 0;
}

void CFuncFactoryInfoList::SetPortUpdate(CString port)
{
	CComboBox* ccb = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
	ccb->ResetContent();
	ccb->AddString(port);
	ccb->SetCurSel(0);
	Invalidate(FALSE);
}

int CFuncFactoryInfoList::ResultCall(const int nIndex, const char* szUnit, const char* szLower, const char* szUpper, const char* szMeasured, const char* szResult, const char* szErrCode, const char* szMsg)
{
	CString unit, lower, upper, measured, result, errcode, msg;

	if (szUnit != NULL) {
		unit.Format(_T("%s"), CA2T(szUnit));
	}
	if (szLower != NULL) {
		lower.Format(_T("%s"), CA2T(szLower));
	}
	if (szUpper != NULL) {
		upper.Format(_T("%s"), CA2T(szUpper));
	}
	if (szMeasured != NULL) {
		measured.Format(_T("%s"), CA2T(szMeasured));
	}
	if (szResult != NULL) {
		result.Format(_T("%s"), CA2T(szResult));
	}
	if (szMsg != NULL) {
		errcode.Format(_T("%s"), CA2T(szErrCode));
	}
	if (szMsg != NULL) {
		msg.Format(_T("%s"), CA2T(szMsg));
	}

	g_ui->SetResult(nIndex, CT2A(unit), CT2A(lower), CT2A(upper), CT2A(measured), CT2A(result), CT2A(errcode), CT2A(msg));

	return 0;
}

void CFuncFactoryInfoList::SetResult(int nIndex, char* szUnit, char* szLower, char* szUpper, char* szMeasured, char* szResult, char* szErrCode, char* szMsg)
{
	CListCtrl* list = (CListCtrl*)GetDlgItem(IDC_LIST_INFO);

	list->SetItemText(nIndex, 1, CA2T(szUnit));
	list->SetItemText(nIndex, 2, CA2T(szLower));
	list->SetItemText(nIndex, 3, CA2T(szUpper));
	list->SetItemText(nIndex, 4, CA2T(szMeasured));
	list->SetItemText(nIndex, 5, CA2T(szResult));
	list->SetItemText(nIndex, 6, CA2T(szErrCode));
	list->SetItemText(nIndex, 7, CA2T(szMsg));
}

void CFuncFactoryInfoList::UIInit()
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

	int nPort = m_dllCtrl.GetPortCount();
	CComboBox* ccb = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
	CString port;
	for (int nIndex = 0; nIndex < nPort; nIndex++) {
		memset(buffer, 0, MAX_MSG);
		m_dllCtrl.GetPortByIndex(nIndex, buffer);
		port = CA2T(buffer);
		ccb->AddString(port);
	}

	CListCtrl* list = (CListCtrl*)GetDlgItem(IDC_LIST_INFO);
	list->DeleteAllItems();
	DWORD dwStyle = list->GetExtendedStyle();
	//dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	//dwStyle |= LVS_EX_FLATSB;
	list->SetExtendedStyle(dwStyle);
	list->InsertColumn(0, _T("Item"), LVCFMT_LEFT, 150);
	list->InsertColumn(1, _T("Unit"), LVCFMT_CENTER, 70);
	list->InsertColumn(2, _T("Lower"), LVCFMT_CENTER, 70);
	list->InsertColumn(3, _T("Upper"), LVCFMT_CENTER, 70);
	list->InsertColumn(4, _T("Measured"), LVCFMT_CENTER, 70);
	list->InsertColumn(5, _T("Result"), LVCFMT_CENTER, 60);
	list->InsertColumn(6, _T("ErrorCode"), LVCFMT_LEFT, 180);
	list->InsertColumn(7, _T("Msg"), LVCFMT_LEFT, 180);
}

void CFuncFactoryInfoList::UIWarning(CString message)
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

void CFuncFactoryInfoList::UIControl(bool isEnable)
{
	GetDlgItem(IDC_COMBO_PORT)->EnableWindow(isEnable);
	GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(isEnable);
}

void CFuncFactoryInfoList::UITotalTime(double secTime)
{
	CString csTime;
	csTime.Format(_T("%.2f sec"), secTime);
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText(csTime);
}

void CFuncFactoryInfoList::Timer(bool isEnable)
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

UINT CFuncFactoryInfoList::WorkerThreadTimerProc(LPVOID param)
{
	CFuncFactoryInfoList* pThis = reinterpret_cast<CFuncFactoryInfoList*>(param);
	ASSERT(pThis != NULL);

	pThis->WorkerThreadTimerRun();

	return TRUE;
}

void CFuncFactoryInfoList::WorkerThreadTimerRun()
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

UINT CFuncFactoryInfoList::WorkerThreadFuncProc(LPVOID param)
{
	CFuncFactoryInfoList* pThis = reinterpret_cast<CFuncFactoryInfoList*>(param);
	ASSERT(pThis != NULL);

	pThis->WorkerThreadFuncRun();

	return TRUE;
}

void CFuncFactoryInfoList::WorkerThreadFuncRun()
{
	if (m_data.isLockDongle && CheckKeyPro() != NO_ERROR) {
		return;
	}

	UIControl(false);
	Timer(true);
	
	CListCtrl* list = (CListCtrl*)GetDlgItem(IDC_LIST_INFO);
	list->DeleteAllItems();

	CString port;
	GetDlgItem(IDC_COMBO_PORT)->GetWindowText(port);
	m_dllCtrl.SetParameter("port", CT2A(port.GetBuffer()));
	port.ReleaseBuffer();

	int nRetCode = m_dllCtrl.Begin();
	int nTestItem = m_dllCtrl.GetInfoCount();

	if (nTestItem !=0) {
		const int MAX_INFO_LENGTH = 1024;
		char* szName = new char[MAX_PATH];
		char* szInfo = new char[MAX_INFO_LENGTH];
		for (int i = 0; i < nTestItem; i++) {
			memset(szName, 0, MAX_PATH);
			memset(szInfo, 0, MAX_INFO_LENGTH);
			m_dllCtrl.GetInfoByIndex(i, szName, szInfo);
			list->InsertItem(i, CA2T(szName));
			//list->SetItemText(i, 1, CA2T(szInfo));
		}
		delete szName;
		delete szInfo;
	}

	if (nRetCode == NO_ERROR)
	{
		nRetCode = m_dllCtrl.PreRun();
	}

	if (nRetCode == NO_ERROR)
	{
		nRetCode = m_dllCtrl.Run();
	}

	//if (nRetCode == NO_ERROR)
	{
		nRetCode = m_dllCtrl.PostRun();
	}

	Timer(false);
	CString csTime;
	GetDlgItem(IDC_STATIC_TIME)->GetWindowText(csTime);
	csTime = _T("Elapsed Time: ") + csTime + _T("\n\n");
	PrintMsg(csTime, _T(""));
	UIControl(true);
}
