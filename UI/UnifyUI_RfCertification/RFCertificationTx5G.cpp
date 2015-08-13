// CRFCertification5GTx.cpp : implementation file
//

#include "stdafx.h"
#include "UnifyUI_ServiceTool.h"
#include "RFCertificationTx5G.h"
#include "VersionInfo.h"

#include <comdef.h> // _com_error in HwDetectSetup
#include <Dbt.h> // DBT_DEVTYP_DEVICEINTERFACE in OnDeviceChange
#include <psapi.h> // GetModuleFileNameEx
#pragma comment(lib, "psapi")

CRFCertification5GTx* g_ui;


// CRFCertification5GTx dialog

IMPLEMENT_DYNAMIC(CRFCertification5GTx, CDialog)

CRFCertification5GTx::CRFCertification5GTx(CWnd* pParent /*=NULL*/)
	: CDialog(CRFCertification5GTx::IDD, pParent)
	, m_sPayload(_T(""))
	, m_sSpacing(_T(""))
	, m_sPower(_T(""))
{

}

CRFCertification5GTx::~CRFCertification5GTx()
{
}

void CRFCertification5GTx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PORT, m_combComport);
	DDX_Control(pDX, IDC_COMBO_CHANNEL_BONDING, m_combChannelBonding);
	DDX_Control(pDX, IDC_COMBO_RF_CHANNEL, m_combChannel);
	DDX_Control(pDX, IDC_COMBO_MODE, m_combMode);
	DDX_Control(pDX, IDC_COMBO_RATE, m_combRate);
	DDX_Text(pDX, IDC_EDIT_PAYLOAD, m_sPayload);
	DDX_Text(pDX, IDC_EDIT_SPACING, m_sSpacing);
	DDX_Text(pDX, IDC_EDIT_TX_POWER, m_sPower);
}


BEGIN_MESSAGE_MAP(CRFCertification5GTx, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, &CRFCertification5GTx::OnCbnSelchangeComboMode)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CRFCertification5GTx::OnBnClickedButtonRun)
	ON_CBN_DROPDOWN(IDC_COMBO_PORT, &CRFCertification5GTx::OnCbnDropdownComboPort)
	ON_CBN_SELCHANGE(IDC_COMBO_CHANNEL_BOUNDING, &CRFCertification5GTx::OnCbnSelchangeComboChannelBounding)
END_MESSAGE_MAP()


// CRFCertification5GTx message handlers
BOOL CRFCertification5GTx::OnInitDialog()
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

BOOL CRFCertification5GTx::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	m_dllCtrl.UnLoad();

	return CDialog::DestroyWindow();
}

void CRFCertification5GTx::SetInit(dlltoolinfo_t& data)
{
	m_data = data;
}

int CRFCertification5GTx::MsgCall(const char* szMsg, const char* szTag)
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

void CRFCertification5GTx::PrintMsg(CString message, CString tag, bool isLog)
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

int CRFCertification5GTx::UpdatePortCall(const char* szPort)
{
	CString port;
	if (szPort != NULL) {
		port.Format(_T("%s"), CA2T(szPort));
	}
	g_ui->SetPortUpdate(port);

	return 0;
}

void CRFCertification5GTx::SetPortUpdate(CString port)
{
	CComboBox* ccb = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
	ccb->ResetContent();
	ccb->AddString(port);
	ccb->SetCurSel(0);
	Invalidate(FALSE);
}

int CRFCertification5GTx::ResultCall(const int nIndex, const char* szUnit, const char* szLower, const char* szUpper, const char* szMeasured, const char* szResult, const char* szErrCode, const char* szMsg)
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

void CRFCertification5GTx::SetResult(int nIndex, char* szUnit, char* szLower, char* szUpper, char* szMeasured, char* szResult, char* szErrCode, char* szMsg)
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

void CRFCertification5GTx::UIInit()
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 32;
	lf.lfWeight = 200;
	HFONT font = ::CreateFontIndirect(&lf);
	GetDlgItem(IDC_BUTTON_RUN)->SendMessage(WM_SETFONT, (WPARAM)font, TRUE);

	CString title;
	title = m_data.title;
	SetWindowText(title);

	m_combMode.AddString(_T("11a/g"));
	m_combMode.AddString(_T("11n"));
	m_combMode.AddString(_T("11ac"));
	m_combMode.SetCurSel(0);

	m_combChannelBonding.AddString(_T("20MHz"));
	m_combChannelBonding.SetItemData(0, 0);
	m_combChannelBonding.SetCurSel(0);

	m_combChannel.AddString(_T("NOT SET"));
	m_combChannel.SetItemData(0, 0);

	char sz_text[50] = {0};
	CString strTxt;
	for (int i = 0; i < 8; i++ )
	{
		sprintf(sz_text, "RF_CHAN_%d(%d)", i*4+36, 5180+i*20);
		strTxt = sz_text;
		m_combChannel.AddString(strTxt);
		m_combChannel.SetItemData(i+1, i*4+36);
	}
	for (int i = 0; i < 12; i++ )
	{
		sprintf(sz_text, "RF_CHAN_%d(%d)", i*4+100, 5500+i*20);
		strTxt = sz_text;
		m_combChannel.AddString(strTxt);
		m_combChannel.SetItemData(i+1+8, i*4+100);
	}
	for (int i = 0; i < 5; i++ )
	{
		sprintf(sz_text, "RF_CHAN_%d(%d)", i*4+149, 5745+i*20);
		strTxt = sz_text;
		m_combChannel.AddString(strTxt);
		m_combChannel.SetItemData(i+1+8+12, i*4+149);
	}
	m_combChannel.SetCurSel(0);

	m_combRate.AddString(_T("11g 6 Mbps - 7")); // rate 7
	m_combRate.AddString(_T("11g 9 Mbps - 8"));
	m_combRate.AddString(_T("11g 12 Mbps - 9"));
	m_combRate.AddString(_T("11g 18 Mbps - 10"));
	m_combRate.AddString(_T("11g 24 Mbps - 11"));
	m_combRate.AddString(_T("11g 36 Mbps - 12"));
	m_combRate.AddString(_T("11g 48 Mbps - 13"));
	m_combRate.AddString(_T("11g 54 Mbps - 14"));
	m_combRate.SetCurSel(0);
	for (int i = 0; i <= 7; i++)
	{
		m_combRate.SetItemData(i, i+7);
	}
}

void CRFCertification5GTx::UIWarning(CString message)
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

void CRFCertification5GTx::UIControl(bool isEnable)
{
	GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(isEnable);
}

void CRFCertification5GTx::UITotalTime(double secTime)
{
	CString csTime;
	csTime.Format(_T("%.2f sec"), secTime);
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText(csTime);
}

void CRFCertification5GTx::Timer(bool isEnable)
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

UINT CRFCertification5GTx::WorkerThreadTimerProc(LPVOID param)
{
	CRFCertification5GTx* pThis = reinterpret_cast<CRFCertification5GTx*>(param);
	ASSERT(pThis != NULL);

	pThis->WorkerThreadTimerRun();

	return TRUE;
}

void CRFCertification5GTx::WorkerThreadTimerRun()
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

UINT CRFCertification5GTx::WorkerThreadFuncProc(LPVOID param)
{
	CRFCertification5GTx* pThis = reinterpret_cast<CRFCertification5GTx*>(param);
	ASSERT(pThis != NULL);

	pThis->WorkerThreadFuncRun();

	return TRUE;
}

void CRFCertification5GTx::WorkerThreadFuncRun()
{
	Timer(true);
	UIControl(false);

	CString payload;
	GetDlgItem(IDC_EDIT_PAYLOAD)->GetWindowText(payload);
	m_dllCtrl.SetParameter("PAYLOAD", CT2A(payload.GetBuffer()));

	CString spacing;
	GetDlgItem(IDC_EDIT_SPACING)->GetWindowText(spacing);
	m_dllCtrl.SetParameter("SPACING", CT2A(spacing.GetBuffer()));

	CString power;
	GetDlgItem(IDC_EDIT_TX_POWER)->GetWindowText(power);
	m_dllCtrl.SetParameter("POWER", CT2A(power.GetBuffer()));

	char *sz_ComValue = new char[10];
	sprintf(sz_ComValue, "%d", m_combComport.GetItemData(m_combComport.GetCurSel()));
	m_dllCtrl.SetParameter("COM", sz_ComValue);

	char *sz_ChannelValue = new char[10];
	sprintf(sz_ChannelValue, "%d", m_combChannel.GetItemData(m_combChannel.GetCurSel()));
	m_dllCtrl.SetParameter("CHANNEL", sz_ChannelValue);

	char *sz_Rate = new char[10];
	sprintf(sz_Rate, "%d", m_combRate.GetItemData(m_combRate.GetCurSel()));
	m_dllCtrl.SetParameter("RATE", sz_Rate);

	char *sz_Bond = new char[10];
	sprintf(sz_Bond, "%d", m_combChannelBonding.GetItemData(m_combChannelBonding.GetCurSel()));
	m_dllCtrl.SetParameter("BOND", sz_Bond);

	CString mode;
	GetDlgItem(IDC_COMBO_MODE)->GetWindowText(mode);
	if (mode.Find(_T("11a/g")) != -1)
	{
		m_dllCtrl.SetParameter("PREAMBLE", "0");
	}
	else if (mode.Find(_T("11n")) != -1)
	{
		m_dllCtrl.SetParameter("PREAMBLE", "2");
	}
	else if (mode.Find(_T("11ac")) != -1)
	{
		m_dllCtrl.SetParameter("PREAMBLE", "2");
	}

	payload.ReleaseBuffer();
	spacing.ReleaseBuffer();
	power.ReleaseBuffer();
	delete sz_ChannelValue;
	delete sz_Rate;
	delete sz_Bond;

	int nRetCode = m_dllCtrl.Begin();

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

	UIControl(true);
	Timer(false);
	CString csTime;
	GetDlgItem(IDC_STATIC_TIME)->GetWindowText(csTime);
	csTime = _T("Elapsed Time: ") + csTime + _T("\n\n");
	PrintMsg(csTime, _T(""));
}

void CRFCertification5GTx::OnCbnSelchangeComboMode()
{
	// reset channel
	m_combChannel.ResetContent();
	m_combChannel.AddString(_T("NOT SET"));
	m_combChannel.SetItemData(0, 0);

	char sz_text[50] = {0};
	CString strTxt;
	for (int i = 0; i < 8; i++ )
	{
		sprintf(sz_text, "RF_CHAN_%d(%d)", i*4+36, 5180+i*20);
		strTxt = sz_text;
		m_combChannel.AddString(strTxt);
		m_combChannel.SetItemData(i+1, i*4+36);
	}
	for (int i = 0; i < 12; i++ )
	{
		sprintf(sz_text, "RF_CHAN_%d(%d)", i*4+100, 5500+i*20);
		strTxt = sz_text;
		m_combChannel.AddString(strTxt);
		m_combChannel.SetItemData(i+1+8, i*4+100);
	}
	for (int i = 0; i < 5; i++ )
	{
		sprintf(sz_text, "RF_CHAN_%d(%d)", i*4+149, 5745+i*20);
		strTxt = sz_text;
		m_combChannel.AddString(strTxt);
		m_combChannel.SetItemData(i+1+8+12, i*4+149);
	}
	m_combChannel.SetCurSel(0);

	if (m_combMode.GetCurSel() == 0)
	{
		// reset text
		GetDlgItem(IDC_STATIC_PS1)->SetWindowText(_T("(Range: 1 to 4091)"));

		m_combChannelBonding.ResetContent();
		m_combChannelBonding.AddString(_T("20MHz"));
		m_combChannelBonding.SetItemData(0, 0);
		m_combChannelBonding.SetCurSel(0);
	}
	else if(m_combMode.GetCurSel() == 1)
	{
		// reset text
		GetDlgItem(IDC_STATIC_PS1)->SetWindowText(_T("(Range: 1 to 4091)"));

		m_combChannelBonding.ResetContent();
		m_combChannelBonding.AddString(_T("20MHz"));
		m_combChannelBonding.SetItemData(0, 0);
		m_combChannelBonding.AddString(_T("40MHz"));
		m_combChannelBonding.SetItemData(1, 3);
		m_combChannelBonding.SetCurSel(0);
	}
	else if(m_combMode.GetCurSel() == 2)
	{
		// reset text
		GetDlgItem(IDC_STATIC_PS1)->SetWindowText(_T("(Range: 1 to 756)"));

		m_combChannelBonding.ResetContent();
		m_combChannelBonding.AddString(_T("20MHz"));
		m_combChannelBonding.SetItemData(0, 0);
		m_combChannelBonding.AddString(_T("40MHz"));
		m_combChannelBonding.SetItemData(1, 3);
		m_combChannelBonding.AddString(_T("80MHz"));
		m_combChannelBonding.SetItemData(2, 6);
		m_combChannelBonding.SetCurSel(0);
	}

	if (m_combMode.GetCurSel() == 0 && m_combChannelBonding.GetCurSel() == 0) // 11 a/g -> 20MHz
	{
		m_combRate.ResetContent();
		m_combRate.AddString(_T("11g 6 Mbps - 7")); // rate 7
		m_combRate.AddString(_T("11g 9 Mbps - 8"));
		m_combRate.AddString(_T("11g 12 Mbps - 9"));
		m_combRate.AddString(_T("11g 18 Mbps - 10"));
		m_combRate.AddString(_T("11g 24 Mbps - 11"));
		m_combRate.AddString(_T("11g 36 Mbps - 12"));
		m_combRate.AddString(_T("11g 48 Mbps - 13"));
		m_combRate.AddString(_T("11g 54 Mbps - 14"));
		m_combRate.SetCurSel(0);
		for (int i = 0; i <= 7; i++)
		{
			m_combRate.SetItemData(i, i+7);
		}
	}
	else if(m_combMode.GetCurSel() == 1 && m_combChannelBonding.GetCurSel() == 0) // 11 n -> 20MHz
	{
		m_combRate.ResetContent();
		m_combRate.AddString(_T("MCS0 6.5 Mbps"));
		m_combRate.AddString(_T("MCS1 13 Mbps"));
		m_combRate.AddString(_T("MCS2 19.5 Mbps"));
		m_combRate.AddString(_T("MCS3 26 Mbps"));
		m_combRate.AddString(_T("MCS4 39 Mbps"));
		m_combRate.AddString(_T("MCS5 52 Mbps"));
		m_combRate.AddString(_T("MCS6 58.5 Mbps"));
		m_combRate.AddString(_T("MCS7 65 Mbps"));
		m_combRate.AddString(_T("MCS0 MM SG 7.2 Mbps"));
		m_combRate.AddString(_T("MCS1 MM SG 14.4 Mbps"));
		m_combRate.AddString(_T("MCS2 MM SG 21.7 Mbps"));
		m_combRate.AddString(_T("MCS3 MM SG 28.9 Mbps"));
		m_combRate.AddString(_T("MCS4 MM SG 43.3 Mbps"));
		m_combRate.AddString(_T("MCS5 MM SG 57.8 Mbps"));
		m_combRate.AddString(_T("MCS6 MM SG 65 Mbps"));
		m_combRate.AddString(_T("MCS7 MM SG 72.2 Mbps"));
		m_combRate.SetCurSel(0);
		for (int i = 0; i <= 15; i++)
		{
			m_combRate.SetItemData(i, i+23);
		}
	}
	else if(m_combMode.GetCurSel() == 1 && m_combChannelBonding.GetCurSel() == 1) // 11 n -> 40MHz
	{
		m_combRate.ResetContent();
		m_combRate.AddString(_T("MCS0 13.5 Mbps (40MHz) - 39")); // rate 39
		m_combRate.AddString(_T("MCS1 27 Mbps (40MHz) - 40")); // rate 40
		m_combRate.AddString(_T("MCS2 40.5 Mbps (40MHz) - 41"));
		m_combRate.AddString(_T("MCS3 54 Mbps (40MHz) - 42"));
		m_combRate.AddString(_T("MCS4 81 Mbps (40MHz) - 43"));
		m_combRate.AddString(_T("MCS5 108 Mbps (40MHz) - 44"));
		m_combRate.AddString(_T("MCS6 121.5 Mbps (40MHz) - 45"));
		m_combRate.AddString(_T("MCS7 135 Mbps (40MHz) - 46"));
		m_combRate.AddString(_T("MCS0 MM SG 15 Mbps (40MHz) - 47"));
		m_combRate.AddString(_T("MCS1 MM SG 30 Mbps (40MHz) - 48"));
		m_combRate.AddString(_T("MCS2 MM SG 45 Mbps (40MHz) - 49"));
		m_combRate.AddString(_T("MCS3 MM SG 60 Mbps (40MHz) - 50"));
		m_combRate.AddString(_T("MCS4 MM SG 90 Mbps (40MHz) - 51"));
		m_combRate.AddString(_T("MCS5 MM SG 120 Mbps (40MHz) - 52"));
		m_combRate.AddString(_T("MCS6 MM SG 135 Mbps (40MHz) - 53"));
		m_combRate.AddString(_T("MCS7 MM SG 150 Mbps (40MHz) - 54"));
		m_combRate.SetCurSel(0);
		for (int i = 0; i <= 15; i++)
		{
			m_combRate.SetItemData(i, i+39);
		}
	}
	else if(m_combMode.GetCurSel() == 2 && m_combChannelBonding.GetCurSel() == 0) // 11 ac -> 20MHz
	{
		m_combRate.ResetContent();
		m_combRate.AddString(_T("11ac MCS0 NGI 6.5 Mbps (20MHz)"));  // rate 63
		m_combRate.AddString(_T("11ac MCS1 NGI 13 Mbps (20MHz)"));  // rate 64
		m_combRate.AddString(_T("11ac MCS2 NGI 19.5 Mbps (20MHz)"));  // rate 65
		m_combRate.AddString(_T("11ac MCS3 NGI 26 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS4 NGI 39 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS5 NGI 52 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS6 NGI 58.5 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS7 NGI 65 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS8 NGI 78 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS0 SGI 7.2 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS1 SGI 14.4 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS2 SGI 21.6 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS3 SGI 28.8 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS4 SGI 43.3 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS5 SGI 57.7 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS6 SGI 65 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS7 SGI 72.2 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS8 SGI 86.6 Mbps (20MHz)"));
		m_combRate.SetCurSel(0);
		for (int i = 0; i <= 17; i++)
		{
			m_combRate.SetItemData(i, i+63);
		}
	}
	else if(m_combMode.GetCurSel() == 2 && m_combChannelBonding.GetCurSel() == 1) // 11 ac -> 40MHz
	{
		m_combRate.ResetContent();
		m_combRate.AddString(_T("11ac MCS0 NGI 13.5 Mbps (40MHz)"));  // rate 81
		m_combRate.AddString(_T("11ac MCS1 NGI 27 Mbps (40MHz)"));  // rate 82
		m_combRate.AddString(_T("11ac MCS2 NGI 40 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS3 NGI 54 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS4 NGI 81 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS5 NGI 108 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS6 NGI 121.5 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS7 NGI 135 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS8 NGI 162 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS9 NGI 180 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS0 SGI 15 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS1 SGI 30 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS2 SGI 45 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS3 SGI 60 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS4 SGI 90 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS5 SGI 120 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS6 SGI 135 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS7 SGI 150 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS8 SGI 180 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS9 SGI 200 Mbps (40MHz)"));
		m_combRate.SetCurSel(0);
		for (int i = 0; i <= 19; i++)
		{
			m_combRate.SetItemData(i, i+81);
		}

	}
	else if(m_combMode.GetCurSel() == 2 && m_combChannelBonding.GetCurSel() == 2) // 11 ac -> 80MHz
	{
		m_combRate.ResetContent();
		m_combRate.AddString(_T("11ac MCS0 NGI 29.3 Mbps (80MHz)"));  // rate 101
		m_combRate.AddString(_T("11ac MCS1 NGI 58.5 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS2 NGI 87.8 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS3 NGI 117 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS4 NGI 175.5 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS5 NGI 234 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS6 NGI 263.3 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS7 NGI 292.5 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS8 NGI 351 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS9 NGI 390 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS0 SGI 32.5 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS1 SGI 65 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS2 SGI 97.5 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS3 SGI 130 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS4 SGI 195 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS5 SGI 260 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS6 SGI 292.5 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS7 SGI 325 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS8 SGI 390 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS9 SGI 433.3 Mbps (80MHz)"));
		m_combRate.SetCurSel(0);
		for (int i = 0; i <= 19; i++)
		{
			m_combRate.SetItemData(i, i+101);
		}
	}
}

void CRFCertification5GTx::OnCbnSelchangeComboChannelBounding()
{
	if(m_combChannelBonding.GetCurSel() == 0) // 20MHz
	{
		m_combChannel.ResetContent();
		m_combChannel.AddString(_T("NOT SET"));
		m_combChannel.SetItemData(0, 0);

		char sz_text[50] = {0};
		CString strTxt;
		for (int i = 0; i < 8; i++ )
		{
			sprintf(sz_text, "RF_CHAN_%d(%d)", i*4+36, 5180+i*20);
			strTxt = sz_text;
			m_combChannel.AddString(strTxt);
			m_combChannel.SetItemData(i+1, i*4+36);
		}
		for (int i = 0; i < 12; i++ )
		{
			sprintf(sz_text, "RF_CHAN_%d(%d)", i*4+100, 5500+i*20);
			strTxt = sz_text;
			m_combChannel.AddString(strTxt);
			m_combChannel.SetItemData(i+1+8, i*4+100);
		}
		for (int i = 0; i < 5; i++ )
		{
			sprintf(sz_text, "RF_CHAN_%d(%d)", i*4+149, 5745+i*20);
			strTxt = sz_text;
			m_combChannel.AddString(strTxt);
			m_combChannel.SetItemData(i+1+8+12, i*4+149);
		}
		m_combChannel.SetCurSel(0);
	}
	else if(m_combChannelBonding.GetCurSel() == 1)  // 40MHz
	{
		m_combChannel.ResetContent();
		m_combChannel.AddString(_T("NOT SET"));
		m_combChannel.SetItemData(0, 0);

		char sz_text[50] = {0};
		CString strTxt;
		for (int i = 0; i < 7; i++ )
		{
			sprintf(sz_text, "RF_CHAN_%d(%d)", i*4+38, 5190+i*20);
			strTxt = sz_text;
			m_combChannel.AddString(strTxt);
			m_combChannel.SetItemData(i+1, i*4+38+2);
		}
		for (int i = 0; i < 11; i++ )
		{
			sprintf(sz_text, "RF_CHAN_%d(%d)", i*4+102, 5510+i*20);
			strTxt = sz_text;
			m_combChannel.AddString(strTxt);
			m_combChannel.SetItemData(i+1+7, i*4+102+2);
		}
		for (int i = 0; i < 4; i++ )
		{
			sprintf(sz_text, "RF_CHAN_%d(%d)", i*4+151, 5755+i*20);
			strTxt = sz_text;
			m_combChannel.AddString(strTxt);
			m_combChannel.SetItemData(i+1+7+11, i*4+151+2);
		}

		m_combChannel.SetCurSel(0);
	}
	else if(m_combChannelBonding.GetCurSel() == 2)  // 80MHz
	{
		m_combChannel.ResetContent();
		m_combChannel.AddString(_T("NOT SET"));
		m_combChannel.SetItemData(0, 0);

		char sz_text[50] = {0};
		CString strTxt;
		for (int i = 0; i < 5; i++ )
		{
			sprintf(sz_text, "RF_CHAN_%d(%d)", i*4+42, 5210+i*20);
			strTxt = sz_text;
			m_combChannel.AddString(strTxt);
			m_combChannel.SetItemData(i+1, i*4+42+2);
		}
		for (int i = 0; i < 9; i++ )
		{
			sprintf(sz_text, "RF_CHAN_%d(%d)", i*4+106, 5530+i*20);
			strTxt = sz_text;
			m_combChannel.AddString(strTxt);
			m_combChannel.SetItemData(i+1+5, i*4+106+2);
		}
		for (int i = 0; i < 2; i++ )
		{
			sprintf(sz_text, "RF_CHAN_%d(%d)", i*4+155, 5775+i*20);
			strTxt = sz_text;
			m_combChannel.AddString(strTxt);
			m_combChannel.SetItemData(i+1+5+9, i*4+155+2);
		}

		m_combChannel.SetCurSel(0);
	}

	if (m_combMode.GetCurSel() == 0 && m_combChannelBonding.GetCurSel() == 0) // 11 a/g -> 20MHz
	{
		m_combRate.ResetContent();
		m_combRate.AddString(_T("11g 6 Mbps - 7")); // rate 7
		m_combRate.AddString(_T("11g 9 Mbps - 8"));
		m_combRate.AddString(_T("11g 12 Mbps - 9"));
		m_combRate.AddString(_T("11g 18 Mbps - 10"));
		m_combRate.AddString(_T("11g 24 Mbps - 11"));
		m_combRate.AddString(_T("11g 36 Mbps - 12"));
		m_combRate.AddString(_T("11g 48 Mbps - 13"));
		m_combRate.AddString(_T("11g 54 Mbps - 14"));
		m_combRate.SetCurSel(0);
		for (int i = 0; i <= 7; i++)
		{
			m_combRate.SetItemData(i, i+7);
		}
	}
	else if(m_combMode.GetCurSel() == 1 && m_combChannelBonding.GetCurSel() == 0) // 11 n -> 20MHz
	{
		m_combRate.ResetContent();
		m_combRate.AddString(_T("MCS0 6.5 Mbps"));
		m_combRate.AddString(_T("MCS1 13 Mbps"));
		m_combRate.AddString(_T("MCS2 19.5 Mbps"));
		m_combRate.AddString(_T("MCS3 26 Mbps"));
		m_combRate.AddString(_T("MCS4 39 Mbps"));
		m_combRate.AddString(_T("MCS5 52 Mbps"));
		m_combRate.AddString(_T("MCS6 58.5 Mbps"));
		m_combRate.AddString(_T("MCS7 65 Mbps"));
		m_combRate.AddString(_T("MCS0 MM SG 7.2 Mbps"));
		m_combRate.AddString(_T("MCS1 MM SG 14.4 Mbps"));
		m_combRate.AddString(_T("MCS2 MM SG 21.7 Mbps"));
		m_combRate.AddString(_T("MCS3 MM SG 28.9 Mbps"));
		m_combRate.AddString(_T("MCS4 MM SG 43.3 Mbps"));
		m_combRate.AddString(_T("MCS5 MM SG 57.8 Mbps"));
		m_combRate.AddString(_T("MCS6 MM SG 65 Mbps"));
		m_combRate.AddString(_T("MCS7 MM SG 72.2 Mbps"));
		m_combRate.SetCurSel(0);
		for (int i = 0; i <= 15; i++)
		{
			m_combRate.SetItemData(i, i+23);
		}
	}
	else if(m_combMode.GetCurSel() == 1 && m_combChannelBonding.GetCurSel() == 1) // 11 n -> 40MHz
	{
		m_combRate.ResetContent();
		m_combRate.AddString(_T("MCS0 13.5 Mbps (40MHz) - 39")); // rate 39
		m_combRate.AddString(_T("MCS1 27 Mbps (40MHz) - 40")); // rate 40
		m_combRate.AddString(_T("MCS2 40.5 Mbps (40MHz) - 41"));
		m_combRate.AddString(_T("MCS3 54 Mbps (40MHz) - 42"));
		m_combRate.AddString(_T("MCS4 81 Mbps (40MHz) - 43"));
		m_combRate.AddString(_T("MCS5 108 Mbps (40MHz) - 44"));
		m_combRate.AddString(_T("MCS6 121.5 Mbps (40MHz) - 45"));
		m_combRate.AddString(_T("MCS7 135 Mbps (40MHz) - 46"));
		m_combRate.AddString(_T("MCS0 MM SG 15 Mbps (40MHz) - 47"));
		m_combRate.AddString(_T("MCS1 MM SG 30 Mbps (40MHz) - 48"));
		m_combRate.AddString(_T("MCS2 MM SG 45 Mbps (40MHz) - 49"));
		m_combRate.AddString(_T("MCS3 MM SG 60 Mbps (40MHz) - 50"));
		m_combRate.AddString(_T("MCS4 MM SG 90 Mbps (40MHz) - 51"));
		m_combRate.AddString(_T("MCS5 MM SG 120 Mbps (40MHz) - 52"));
		m_combRate.AddString(_T("MCS6 MM SG 135 Mbps (40MHz) - 53"));
		m_combRate.AddString(_T("MCS7 MM SG 150 Mbps (40MHz) - 54"));
		m_combRate.SetCurSel(0);
		for (int i = 0; i <= 15; i++)
		{
			m_combRate.SetItemData(i, i+39);
		}
	}
	else if(m_combMode.GetCurSel() == 2 && m_combChannelBonding.GetCurSel() == 0) // 11 ac -> 20MHz
	{
		m_combRate.ResetContent();
		m_combRate.AddString(_T("11ac MCS0 NGI 6.5 Mbps (20MHz)"));  // rate 63
		m_combRate.AddString(_T("11ac MCS1 NGI 13 Mbps (20MHz)"));  // rate 64
		m_combRate.AddString(_T("11ac MCS2 NGI 19.5 Mbps (20MHz)"));  // rate 65
		m_combRate.AddString(_T("11ac MCS3 NGI 26 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS4 NGI 39 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS5 NGI 52 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS6 NGI 58.5 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS7 NGI 65 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS8 NGI 78 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS0 SGI 7.2 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS1 SGI 14.4 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS2 SGI 21.6 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS3 SGI 28.8 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS4 SGI 43.3 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS5 SGI 57.7 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS6 SGI 65 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS7 SGI 72.2 Mbps (20MHz)"));
		m_combRate.AddString(_T("11ac MCS8 SGI 86.6 Mbps (20MHz)"));
		m_combRate.SetCurSel(0);
		for (int i = 0; i <= 17; i++)
		{
			m_combRate.SetItemData(i, i+63);
		}
	}
	else if(m_combMode.GetCurSel() == 2 && m_combChannelBonding.GetCurSel() == 1) // 11 ac -> 40MHz
	{
		m_combRate.ResetContent();
		m_combRate.AddString(_T("11ac MCS0 NGI 13.5 Mbps (40MHz)"));  // rate 81
		m_combRate.AddString(_T("11ac MCS1 NGI 27 Mbps (40MHz)"));  // rate 82
		m_combRate.AddString(_T("11ac MCS2 NGI 40 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS3 NGI 54 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS4 NGI 81 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS5 NGI 108 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS6 NGI 121.5 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS7 NGI 135 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS8 NGI 162 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS9 NGI 180 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS0 SGI 15 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS1 SGI 30 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS2 SGI 45 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS3 SGI 60 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS4 SGI 90 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS5 SGI 120 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS6 SGI 135 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS7 SGI 150 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS8 SGI 180 Mbps (40MHz)"));
		m_combRate.AddString(_T("11ac MCS9 SGI 200 Mbps (40MHz)"));
		m_combRate.SetCurSel(0);
		for (int i = 0; i <= 19; i++)
		{
			m_combRate.SetItemData(i, i+81);
		}

	}
	else if(m_combMode.GetCurSel() == 2 && m_combChannelBonding.GetCurSel() == 2) // 11 ac -> 80MHz
	{
		m_combRate.ResetContent();
		m_combRate.AddString(_T("11ac MCS0 NGI 29.3 Mbps (80MHz)"));  // rate 101
		m_combRate.AddString(_T("11ac MCS1 NGI 58.5 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS2 NGI 87.8 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS3 NGI 117 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS4 NGI 175.5 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS5 NGI 234 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS6 NGI 263.3 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS7 NGI 292.5 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS8 NGI 351 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS9 NGI 390 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS0 SGI 32.5 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS1 SGI 65 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS2 SGI 97.5 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS3 SGI 130 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS4 SGI 195 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS5 SGI 260 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS6 SGI 292.5 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS7 SGI 325 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS8 SGI 390 Mbps (80MHz)"));
		m_combRate.AddString(_T("11ac MCS9 SGI 433.3 Mbps (80MHz)"));
		m_combRate.SetCurSel(0);
		for (int i = 0; i <= 19; i++)
		{
			m_combRate.SetItemData(i, i+101);
		}
	}
}


void CRFCertification5GTx::OnBnClickedButtonRun()
{
	PrintMsg(_T(""), _T("CLEAR"), false);
	UpdateData();
	if (m_combComport.GetCurSel() == CB_ERR)
	{
		PrintMsg(_T("Please select comport.\n"), _T("WARN"));
		return;
	}
	if (m_sPayload.IsEmpty() || m_sSpacing.IsEmpty() || m_sPower.IsEmpty())
	{
		PrintMsg(_T("Please enter payload/spacing/power in the input field.\n"), _T("WARN"));
		return;
	}

	CString payload;
	GetDlgItem(IDC_EDIT_PAYLOAD)->GetWindowText(payload);
	int iPayload = _wtoi(payload);
	if(m_combMode.GetCurSel() == 2) // 11ac
	{
		if(iPayload > 756)
		{
			m_sPayload = "756";
			GetDlgItem(IDC_EDIT_PAYLOAD)->SetWindowText(m_sPayload);
			GetDlgItem(IDC_EDIT_PAYLOAD)->UpdateData(FALSE);
		}
	}
	else
	{
		if(iPayload > 4091)
		{
			m_sPayload = "4091";
			GetDlgItem(IDC_EDIT_PAYLOAD)->SetWindowText(m_sPayload);
			GetDlgItem(IDC_EDIT_PAYLOAD)->UpdateData(FALSE);
		}
	}
	
	m_thrdMainProgress = ::AfxBeginThread(WorkerThreadFuncProc, this, THREAD_PRIORITY_ABOVE_NORMAL);
}

bool CRFCertification5GTx::SearchPortUsbAdb(const UINT secTimeout)
{
	UINT timer = 0;
	bool hasPort = false;
	for (;;) {
		hasPort = SearchPortUsb(1) && GetAdbDevice();
		timer++;
		if (hasPort || timer >= secTimeout) {
			break;
		}
	}

	return SearchPortUsb(1) && GetAdbDevice();
}

bool CRFCertification5GTx::FindUsbDevice()
{
	HDEVINFO hDevInfo = SetupDiGetClassDevs(NULL, _T("USB"), NULL, (DIGCF_ALLCLASSES | DIGCF_PRESENT));
	if (INVALID_HANDLE_VALUE == hDevInfo) {
		AfxMessageBox(CString(_T("SetupDiGetClassDevs(): "))
		              + _com_error(GetLastError()).ErrorMessage(), MB_ICONEXCLAMATION);
		return false;
	}

	bool isFound = false;
	SP_DEVINFO_DATA spDevInfoData;
	spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for (int i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &spDevInfoData); i++) {
		DWORD nSize = 0;
		TCHAR DeviceInstanceId[MAX_PATH];
		if (!SetupDiGetDeviceInstanceId(hDevInfo, &spDevInfoData, DeviceInstanceId, sizeof(DeviceInstanceId), &nSize)) {
			TRACE(_T("SetupDiGetDeviceInstanceId(): %s\n"), _com_error(GetLastError()).ErrorMessage());
			isFound = false;
			break;
		}
		if (FindDeviceId(DeviceInstanceId)) {
			m_com.desc = GetDeviceName(hDevInfo, spDevInfoData);
			if (GetComPort() || GetAdbDevice()) {
				TRACE(_T("Find device: %s\n"), m_com.desc);
				isFound = true;
				break;
			}
		}
	}
	SetupDiDestroyDeviceInfoList(hDevInfo);

	return isFound;
}

bool CRFCertification5GTx::FindDeviceId(CString sDeviceId)
{
	/*bool isFound =
		sDeviceId.Find(_T("USB\\VID_1D45&PID_4578")) != -1 ||
		sDeviceId.Find(_T("USB\\VID_1D45&PID_4583")) != -1 ||
		sDeviceId.Find(_T("USB\\VID_1D45&PID_4585")) != -1 ||
		sDeviceId.Find(_T("USB\\VID_2931&PID_0A05")) != -1 ||
		sDeviceId.Find(_T("USB\\VID_1D45&PID_4604")) != -1 ||
		sDeviceId.Find(_T("USB\\VID_1D45&PID_459C")) != -1;

	return isFound;*/
	return true;
}

bool CRFCertification5GTx::SearchPortUsb(const UINT secTimeout)
{
	UINT timer = 0;
	bool hasPort = false;
	m_com.port.Empty();
	m_com.desc.Empty();
	for (;;) {
		hasPort = FindUsbDevice();
		if (hasPort || timer >= secTimeout) {
			break;
		}
		Sleep(1000);
		timer++;
	}

	return hasPort;
}

CString CRFCertification5GTx::GetDeviceName(HDEVINFO& hDevInfo, SP_DEVINFO_DATA& spDevInfoData)
{
	DWORD DataT;
	TCHAR buffer[MAX_PATH];
	DWORD nSize = 0;
	if (SetupDiGetDeviceRegistryProperty(
		hDevInfo, &spDevInfoData, SPDRP_FRIENDLYNAME, &DataT, (PBYTE)buffer, sizeof(buffer), &nSize)) {
	}
	else if (SetupDiGetDeviceRegistryProperty(hDevInfo, &spDevInfoData,
	         SPDRP_DEVICEDESC, &DataT, (PBYTE)buffer, sizeof(buffer), &nSize)) {
	}
	return CString(buffer);
}

bool CRFCertification5GTx::GetComPort()
{
	int nStart = m_com.desc.Find(_T("(COM"));
	int nEnd   = m_com.desc.ReverseFind(_T(')'));
	if (nStart == -1 || nEnd == -1 || (nEnd - nStart) < 5) {
		m_com.port.Empty();
		return false;
	}
	m_com.port = m_com.desc.Mid(nStart + 4, nEnd - nStart - 4);
	return !m_com.port.IsEmpty();
}

bool CRFCertification5GTx::GetAdbDevice()
{
	return
		//(m_com.desc.Find(_T("diag")) != -1);
		(m_com.desc.Find(_T("Disgnostics")) != -1);
}

void CRFCertification5GTx::OnCbnDropdownComboPort()
{
	m_combComport.ResetContent();
	SearchPortUsbAdb(3);
	m_combComport.AddString(m_com.desc);
	m_combComport.SetItemData(0, _wtoi(m_com.port));
	m_com.port.ReleaseBuffer();
}