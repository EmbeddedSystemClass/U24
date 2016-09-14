// RFCertification.cpp : implementation file
//

#include "stdafx.h"
#include "UnifyUI_ServiceTool.h"
#include "CSDREAD.h"
#include "VersionInfo.h"

#include <comdef.h> // _com_error in HwDetectSetup
#include <Dbt.h> // DBT_DEVTYP_DEVICEINTERFACE in OnDeviceChange
#include <psapi.h> // GetModuleFileNameEx
#pragma comment(lib, "psapi")

CCSDREAD* g_ui;

// CCSDREAD dialog

IMPLEMENT_DYNAMIC(CCSDREAD, CDialog)

CCSDREAD::CCSDREAD(CWnd* pParent /*=NULL*/)
	: CDialog(CCSDREAD::IDD, pParent)
	, m_sTag(_T(""))
{

}

CCSDREAD::~CCSDREAD()
{
}

void CCSDREAD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TAG, m_sTag);
}


BEGIN_MESSAGE_MAP(CCSDREAD, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RUN2, &CCSDREAD::OnBnClickedButtonRun)
END_MESSAGE_MAP()


// CCSDREAD message handlers
BOOL CCSDREAD::OnInitDialog()
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
	m_dllCtrl.SetCallBackResult(ResultCall);

	CFont *myFont = new CFont();
	myFont->CreateFont( 40, 0, 0, 0, FW_BLACK, false, false,
        0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH|FF_MODERN, _T("Arial") );//Courier New

	GetDlgItem(IDC_EDIT_TAG)->EnableWindow(true);		
	((CEdit*)GetDlgItem(IDC_EDIT_TAG))->SetLimitText(TAG_LENGTH);
	((CEdit*)GetDlgItem(IDC_EDIT_TAG))->SetFont(myFont);

	CString csTest("test");
	 CFont *m_pFont = new CFont();
    m_pFont->CreatePointFont(65, _T("Arial"));
	
	GetDlgItem(IDC_TEXT_SCANTAG_CSD)->SetFont(m_pFont);


	//GetDlgItem(IDC_EDIT_TAG)->SetWindowText(_T(""));
	GetDlgItem(IDC_TEXT_SCANTAG_CSD)->SetWindowText(_T("Read Tag"));
	GetDlgItem(IDC_EDIT_TAG)->SendMessage(EM_SETREADONLY ,1 ,0);
	//IDD
	g_ui = this;

	::GetModuleFileName(NULL, m_pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	m_pthToolDir.ReleaseBuffer();
	m_pthToolDir = m_pthToolDir.Left(m_pthToolDir.ReverseFind('\\'));

	UIInit();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCSDREAD::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	m_dllCtrl.UnLoad();

	return CDialog::DestroyWindow();
}

void CCSDREAD::SetInit(dlltoolinfo_t& data)
{
	m_data = data;
}

int CCSDREAD::MsgCall(const char* szMsg, const char* szTag)
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

void CCSDREAD::PrintMsg(CString message, CString tag, bool isLog)
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

int CCSDREAD::ResultCall(const int nIndex, const char* szUnit, const char* szLower, const char* szUpper, const char* szMeasured, const char* szResult, const char* szErrCode, const char* szMsg)
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
	g_ui->SetWriteResult(CT2A(measured));
	return 0;
}

void CCSDREAD::SetResult(int nIndex, char* szUnit, char* szLower, char* szUpper, char* szMeasured, char* szResult, char* szErrCode, char* szMsg)
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

void CCSDREAD::UIInit()
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 32;
	lf.lfWeight = 200;
	HFONT font = ::CreateFontIndirect(&lf);
	GetDlgItem(IDC_BUTTON_RUN2)->SendMessage(WM_SETFONT, (WPARAM)font, TRUE);

	CString title;
	title = m_data.title;
	SetWindowText(title);


}

void CCSDREAD::UITotalTime(double secTime)
{
	CString csTime;
	csTime.Format(_T("%.2f sec"), secTime);
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText(csTime);
}

void CCSDREAD::Timer(bool isEnable)
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

UINT CCSDREAD::WorkerThreadTimerProc(LPVOID param)
{
	CCSDREAD* pThis = reinterpret_cast<CCSDREAD*>(param);
	ASSERT(pThis != NULL);

	pThis->WorkerThreadTimerRun();

	return TRUE;
}

void CCSDREAD::WorkerThreadTimerRun()
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

UINT CCSDREAD::WorkerThreadFuncProc(LPVOID param)
{
	CCSDREAD* pThis = reinterpret_cast<CCSDREAD*>(param);
	ASSERT(pThis != NULL);

	pThis->WorkerThreadFuncRun();

	return TRUE;
}


void CCSDREAD::UIControl(bool isEnable)
{
	GetDlgItem(IDC_BUTTON_RUN2)->EnableWindow(isEnable);
}                


void CCSDREAD::WorkerThreadFuncRun()
{
	Timer(true);
	UIControl(false);
	CString csString;
	CString csTag = NULL;
	int nRetCode = 0;

	if (nRetCode == NO_ERROR)
	{
		csString = _T("start to read Tag,  ") + csTag  + _T("\n");
		PrintMsg(csString, _T(""));
		csTag.ReleaseBuffer();
		//GetDlgItem(IDC_EDIT_TAG)->SetWindowText(_T(""));
		nRetCode = m_dllCtrl.Begin();
	}
	
	if (nRetCode == NO_ERROR)
	{
		nRetCode = m_dllCtrl.PreRun();
	}

	if (nRetCode == NO_ERROR)
	{
		nRetCode = m_dllCtrl.Run();
	}

	if (nRetCode == NO_ERROR)
	{
		nRetCode = m_dllCtrl.PostRun();
	}

	if (nRetCode == NO_ERROR){
		csString = _T("----------------  Read Success -----------\n\n") ;
		PrintMsg(csString, _T(""));
	}

	UIControl(true);
	
	Timer(false);
	CString csTime;
	GetDlgItem(IDC_STATIC_TIME)->GetWindowText(csTime);
	csTime = _T("Elapsed Time: ") + csTime + _T("\n\n");
	PrintMsg(csTime, _T(""));
}

void CCSDREAD::OnBnClickedButtonRun()
{
	PrintMsg(_T(""), _T("CLEAR"), false);
	UpdateData();

	m_thrdMainProgress = ::AfxBeginThread(WorkerThreadFuncProc, this, THREAD_PRIORITY_ABOVE_NORMAL);
}

int CCSDREAD::WriteResultCall(const char* szMeasured)
{
	CString measured;

	if (szMeasured != NULL) {
		measured.Format(_T("%s"), CA2T(szMeasured));
	}

	g_ui->SetWriteResult(CT2A(measured));

	return 0;
}

void CCSDREAD::SetWriteResult(char* szMeasured)
{
	CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT_TAG);
	edit->SetWindowText(CA2T(szMeasured));
}

