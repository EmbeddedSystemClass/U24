// RFCertificationTxCw.cpp : implementation file
//
#include "stdafx.h"
#include "UnifyUI_ServiceTool.h"
#include "RFCertification5GTxCw.h"
#include "VersionInfo.h"

#include <comdef.h> // _com_error in HwDetectSetup
#include <Dbt.h> // DBT_DEVTYP_DEVICEINTERFACE in OnDeviceChange
#include <psapi.h> // GetModuleFileNameEx
#pragma comment(lib, "psapi")

CRFCertification5GTxCw* g_ui;

// CRFCertification5GTxCw dialog

IMPLEMENT_DYNAMIC(CRFCertification5GTxCw, CDialog)

CRFCertification5GTxCw::CRFCertification5GTxCw(CWnd* pParent /*=NULL*/)
	: CDialog(CRFCertification5GTxCw::IDD, pParent)
	, m_sRfGain(_T(""))
{

}

CRFCertification5GTxCw::~CRFCertification5GTxCw()
{
}

void CRFCertification5GTxCw::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_RF_CHANNEL, m_combChannel);
	DDX_Control(pDX, IDC_COMBO_PORT, m_combComport);
	DDX_Text(pDX, IDC_EDIT_RF_GAIN, m_sRfGain);
	DDX_Control(pDX, IDC_COMBO_MODE, m_combMode);
	DDX_Control(pDX, IDC_COMBO_CHANNEL_BONDING, m_combChannelBonding);
}


BEGIN_MESSAGE_MAP(CRFCertification5GTxCw, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CRFCertification5GTxCw::OnBnClickedButtonRun)
	ON_CBN_DROPDOWN(IDC_COMBO_PORT, &CRFCertification5GTxCw::OnCbnDropdownComboPort)
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, &CRFCertification5GTxCw::OnCbnSelchangeComboMode)
	ON_CBN_SELCHANGE(IDC_COMBO_CHANNEL_BOUNDING, &CRFCertification5GTxCw::OnCbnSelchangeComboChannelBounding)
END_MESSAGE_MAP()


// RFCertificationRx message handlers
BOOL CRFCertification5GTxCw::OnInitDialog()
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

BOOL CRFCertification5GTxCw::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	m_dllCtrl.UnLoad();

	return CDialog::DestroyWindow();
}

void CRFCertification5GTxCw::SetInit(dlltoolinfo_t& data)
{
	m_data = data;
}

int CRFCertification5GTxCw::MsgCall(const char* szMsg, const char* szTag)
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

void CRFCertification5GTxCw::PrintMsg(CString message, CString tag, bool isLog)
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

int CRFCertification5GTxCw::UpdatePortCall(const char* szPort)
{
	CString port;
	if (szPort != NULL) {
		port.Format(_T("%s"), CA2T(szPort));
	}
	g_ui->SetPortUpdate(port);

	return 0;
}

void CRFCertification5GTxCw::SetPortUpdate(CString port)
{
	CComboBox* ccb = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
	ccb->ResetContent();
	ccb->AddString(port);
	ccb->SetCurSel(0);
	Invalidate(FALSE);
}

int CRFCertification5GTxCw::ResultCall(const int nIndex, const char* szUnit, const char* szLower, const char* szUpper, const char* szMeasured, const char* szResult, const char* szErrCode, const char* szMsg)
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

int CRFCertification5GTxCw::RxResultCall(const char* szMeasured)
{
	CString measured;

	if (szMeasured != NULL) {
		measured.Format(_T("%s"), CA2T(szMeasured));
	}

	g_ui->SetRxResult(CT2A(measured));

	return 0;
}

void CRFCertification5GTxCw::SetResult(int nIndex, char* szUnit, char* szLower, char* szUpper, char* szMeasured, char* szResult, char* szErrCode, char* szMsg)
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

void CRFCertification5GTxCw::SetRxResult(char* szMeasured)
{
	CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT_PACKET);
	edit->SetWindowText(CA2T(szMeasured));
}

void CRFCertification5GTxCw::UIInit()
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 16;
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
}

void CRFCertification5GTxCw::UIWarning(CString message)
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

void CRFCertification5GTxCw::UIControl(bool isEnable)
{
	GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(isEnable);
}

void CRFCertification5GTxCw::UITotalTime(double secTime)
{
	CString csTime;
	csTime.Format(_T("%.2f sec"), secTime);
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText(csTime);
}

void CRFCertification5GTxCw::Timer(bool isEnable)
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

UINT CRFCertification5GTxCw::WorkerThreadTimerProc(LPVOID param)
{
	CRFCertification5GTxCw* pThis = reinterpret_cast<CRFCertification5GTxCw*>(param);
	ASSERT(pThis != NULL);

	pThis->WorkerThreadTimerRun();

	return TRUE;
}

void CRFCertification5GTxCw::WorkerThreadTimerRun()
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

void CRFCertification5GTxCw::OnBnClickedButtonRun()
{
	PrintMsg(_T(""), _T("CLEAR"), false);
	UpdateData();
	if (m_combComport.GetCurSel() == CB_ERR)
	{
		PrintMsg(_T("Please select comport.\n"), _T("WARN"));
		return;
	}
	if (m_sRfGain.IsEmpty())
	{
		PrintMsg(_T("Please enter RF Gain in the input field.\n"), _T("WARN"));
		return;
	}

	m_thrdMainProgress = ::AfxBeginThread(WorkerThreadFuncProc, this, THREAD_PRIORITY_ABOVE_NORMAL);
}

UINT CRFCertification5GTxCw::WorkerThreadFuncProc(LPVOID param)
{
	CRFCertification5GTxCw* pThis = reinterpret_cast<CRFCertification5GTxCw*>(param);
	ASSERT(pThis != NULL);

	pThis->WorkerThreadFuncRun();

	return TRUE;
}

void CRFCertification5GTxCw::WorkerThreadFuncRun()
{
	Timer(true);
	UIControl(false);

	char *sz_ComValue = new char[10];
	sprintf(sz_ComValue, "%d", m_combComport.GetItemData(m_combComport.GetCurSel()));
	m_dllCtrl.SetParameter("COM", sz_ComValue);

	char *sz_ChannelValue = new char[10];
	sprintf(sz_ChannelValue, "%d", m_combChannel.GetItemData(m_combChannel.GetCurSel()));
	m_dllCtrl.SetParameter("CHANNEL", sz_ChannelValue);

	CString RfGain;
	GetDlgItem(IDC_EDIT_RF_GAIN)->GetWindowText(RfGain);
	m_dllCtrl.SetParameter("RFGAIN", CT2A(RfGain.GetBuffer()));

	char *sz_Bond = new char[10];
	sprintf(sz_Bond, "%d", m_combChannelBonding.GetItemData(m_combChannelBonding.GetCurSel()));
	m_dllCtrl.SetParameter("BOND", sz_Bond);
	
	delete sz_ChannelValue;
	delete sz_ComValue;
	delete sz_Bond;
	RfGain.ReleaseBuffer();

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

void CRFCertification5GTxCw::OnCbnDropdownComboPort()
{
	m_combComport.ResetContent();
	SearchPortUsbAdb(3);
	m_combComport.AddString(m_com.desc);
	m_combComport.SetItemData(0, _wtoi(m_com.port));
	m_com.port.ReleaseBuffer();
}

bool CRFCertification5GTxCw::SearchPortUsbAdb(const UINT secTimeout)
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

bool CRFCertification5GTxCw::FindUsbDevice()
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

bool CRFCertification5GTxCw::FindDeviceId(CString sDeviceId)
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

bool CRFCertification5GTxCw::SearchPortUsb(const UINT secTimeout)
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

CString CRFCertification5GTxCw::GetDeviceName(HDEVINFO& hDevInfo, SP_DEVINFO_DATA& spDevInfoData)
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

bool CRFCertification5GTxCw::GetComPort()
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

bool CRFCertification5GTxCw::GetAdbDevice()
{
	return
		//(m_com.desc.Find(_T("diag")) != -1);
		(m_com.desc.Find(_T("Disgnostics")) != -1);
}

void CRFCertification5GTxCw::OnCbnSelchangeComboMode()
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
		m_combChannelBonding.ResetContent();
		m_combChannelBonding.AddString(_T("20MHz"));
		m_combChannelBonding.SetItemData(0, 0);
		m_combChannelBonding.SetCurSel(0);
	}
	else if(m_combMode.GetCurSel() == 1)
	{
		m_combChannelBonding.ResetContent();
		m_combChannelBonding.AddString(_T("20MHz"));
		m_combChannelBonding.SetItemData(0, 0);
		m_combChannelBonding.AddString(_T("40MHz"));
		m_combChannelBonding.SetItemData(1, 3);
		m_combChannelBonding.SetCurSel(0);
	}
	else if(m_combMode.GetCurSel() == 2)
	{
		m_combChannelBonding.ResetContent();
		m_combChannelBonding.AddString(_T("20MHz"));
		m_combChannelBonding.SetItemData(0, 0);
		m_combChannelBonding.AddString(_T("40MHz"));
		m_combChannelBonding.SetItemData(1, 3);
		m_combChannelBonding.AddString(_T("80MHz"));
		m_combChannelBonding.SetItemData(2, 6);
		m_combChannelBonding.SetCurSel(0);
	}
}

void CRFCertification5GTxCw::OnCbnSelchangeComboChannelBounding()
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
}