// RFCertificationRx.cpp : implementation file
//
#include "stdafx.h"
#include "UnifyUI_ServiceTool.h"
#include "RFCertificationRx.h"
#include "VersionInfo.h"

#include <comdef.h> // _com_error in HwDetectSetup
#include <Dbt.h> // DBT_DEVTYP_DEVICEINTERFACE in OnDeviceChange
#include <psapi.h> // GetModuleFileNameEx
#pragma comment(lib, "psapi")

CRFCertificationRx* g_ui;

// RFCertificationRx dialog

IMPLEMENT_DYNAMIC(CRFCertificationRx, CDialog)

CRFCertificationRx::CRFCertificationRx(CWnd* pParent /*=NULL*/)
	: CDialog(CRFCertificationRx::IDD, pParent)
	, bRxTestStop(false)
{

}

CRFCertificationRx::~CRFCertificationRx()
{
}

void CRFCertificationRx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_PORT, m_combComport);
	DDX_Text(pDX, IDC_EDIT_RX_CHANNEL, m_sChannel);
	DDX_Text(pDX, IDC_EDIT_RX_RateMask, m_sRateMask);
	
	DDX_Control(pDX, IDC_COMBO_CHAIN, m_combChain);
	DDX_Control(pDX, IDC_COMBO_RX_WlanMode, m_combWlanMode);
	
}


BEGIN_MESSAGE_MAP(CRFCertificationRx, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CRFCertificationRx::OnBnClickedButtonRun)
	ON_CBN_DROPDOWN(IDC_COMBO_PORT, &CRFCertificationRx::OnCbnDropdownComboPort)
	ON_BN_CLICKED(IDC_BUTTON_RUN2, &CRFCertificationRx::OnBnClickedButtonRun2)
END_MESSAGE_MAP()


// RFCertificationRx message handlers
BOOL CRFCertificationRx::OnInitDialog()
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
	m_dllCtrl.SetCallBackRxResult(RxResultCall);

	g_ui = this;

	::GetModuleFileName(NULL, m_pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	m_pthToolDir.ReleaseBuffer();
	m_pthToolDir = m_pthToolDir.Left(m_pthToolDir.ReverseFind('\\'));

	UIInit();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CRFCertificationRx::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	m_dllCtrl.UnLoad();

	return CDialog::DestroyWindow();
}

void CRFCertificationRx::SetInit(dlltoolinfo_t& data)
{
	m_data = data;
}

int CRFCertificationRx::MsgCall(const char* szMsg, const char* szTag)
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

void CRFCertificationRx::PrintMsg(CString message, CString tag, bool isLog)
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

int CRFCertificationRx::UpdatePortCall(const char* szPort)
{
	CString port;
	if (szPort != NULL) {
		port.Format(_T("%s"), CA2T(szPort));
	}
	g_ui->SetPortUpdate(port);

	return 0;
}

void CRFCertificationRx::SetPortUpdate(CString port)
{
	CComboBox* ccb = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
	ccb->ResetContent();
	ccb->AddString(port);
	ccb->SetCurSel(0);
	Invalidate(FALSE);
}

int CRFCertificationRx::ResultCall(const int nIndex, const char* szUnit, const char* szLower, const char* szUpper, const char* szMeasured, const char* szResult, const char* szErrCode, const char* szMsg)
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

int CRFCertificationRx::RxResultCall(const char* szMeasured)
{
	CString measured;

	if (szMeasured != NULL) {
		measured.Format(_T("%s"), CA2T(szMeasured));
	}

	g_ui->SetRxResult(CT2A(measured));

	return 0;
}

void CRFCertificationRx::SetResult(int nIndex, char* szUnit, char* szLower, char* szUpper, char* szMeasured, char* szResult, char* szErrCode, char* szMsg)
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

void CRFCertificationRx::SetRxResult(char* szMeasured)
{
	CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT_PACKET);
	edit->SetWindowText(CA2T(szMeasured));
}

void CRFCertificationRx::UIInit()
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 16;
	lf.lfWeight = 100;
	HFONT font = ::CreateFontIndirect(&lf);
	GetDlgItem(IDC_BUTTON_RUN)->SendMessage(WM_SETFONT, (WPARAM)font, TRUE);

	CString title;
	title = m_data.title;
	SetWindowText(title);

	
	m_combChain.AddString(_T("0"));
	m_combChain.AddString(_T("1"));
	for (int i = 0; i < m_combChain.GetCount(); i++)
	{
	   m_combChain.SetItemData(i, i);
	}
	m_combChain.SetCurSel(0);

	m_combWlanMode.AddString(_T("0"));
	m_combWlanMode.AddString(_T("1"));
	m_combWlanMode.AddString(_T("4"));
	m_combWlanMode.AddString(_T("5"));
	m_combWlanMode.AddString(_T("8"));
	m_combWlanMode.SetItemData(0, 0);
	m_combWlanMode.SetItemData(1, 1);
	m_combWlanMode.SetItemData(2, 4);
	m_combWlanMode.SetItemData(3, 5);
	m_combWlanMode.SetItemData(4, 8);
	m_combWlanMode.SetCurSel(0);
}



void CRFCertificationRx::UIControl(bool isEnable)
{
	GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(isEnable);
}

void CRFCertificationRx::UITotalTime(double secTime)
{
	CString csTime;
	csTime.Format(_T("%.2f sec"), secTime);
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText(csTime);
}

void CRFCertificationRx::Timer(bool isEnable)
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

UINT CRFCertificationRx::WorkerThreadTimerProc(LPVOID param)
{
	CRFCertificationRx* pThis = reinterpret_cast<CRFCertificationRx*>(param);
	ASSERT(pThis != NULL);

	pThis->WorkerThreadTimerRun();

	return TRUE;
}

void CRFCertificationRx::WorkerThreadTimerRun()
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
void CRFCertificationRx::OnBnClickedButtonRun()
{
	PrintMsg(_T(""), _T("CLEAR"), false);
	UpdateData();
	if (m_combComport.GetCurSel() == CB_ERR)
	{
		PrintMsg(_T("Please select comport.\n"), _T("WARN"));
		return;
	}

	if (m_combChain.GetCurSel() == CB_ERR)
	{
		PrintMsg(_T("Please select Chain.\n"), _T("WARN"));
		return;
	}

	if (m_combWlanMode.GetCurSel() == CB_ERR)
	{
		PrintMsg(_T("Please select WlanMode.\n"), _T("WARN"));
		return;
	}	
	m_thrdMainProgress = ::AfxBeginThread(WorkerThreadFuncProc, this, THREAD_PRIORITY_ABOVE_NORMAL);
}

UINT CRFCertificationRx::WorkerThreadFuncProc(LPVOID param)
{
	CRFCertificationRx* pThis = reinterpret_cast<CRFCertificationRx*>(param);
	ASSERT(pThis != NULL);

	pThis->WorkerThreadFuncRun();

	return TRUE;
}

void CRFCertificationRx::WorkerThreadFuncRun()
{
	Timer(true);
	UIControl(false);
	bRxTestStop = false;

	char *sz_ComValue = new char[10];
	sprintf(sz_ComValue, "%d", m_combComport.GetItemData(m_combComport.GetCurSel()));
	m_dllCtrl.SetParameter("COM", sz_ComValue);

	CString csRxCHANNEL;
	GetDlgItem(IDC_EDIT_RX_CHANNEL)->GetWindowText(csRxCHANNEL);
	m_dllCtrl.SetParameter("CHANNEL", CT2A(csRxCHANNEL.GetBuffer()));

	CString csRateMask;
	GetDlgItem(IDC_EDIT_RX_RateMask)->GetWindowText(csRateMask);
	m_dllCtrl.SetParameter("RATEMASK", CT2A(csRateMask.GetBuffer()));


	char *sz_WlanMode = new char[10];
	sprintf(sz_WlanMode, "%d", m_combWlanMode.GetItemData(m_combWlanMode.GetCurSel()));
	m_dllCtrl.SetParameter("WLANMODE", sz_WlanMode);

	char *sz_Chain = new char[10];
	sprintf(sz_Chain, "%d", m_combChain.GetItemData(m_combChain.GetCurSel()));
	m_dllCtrl.SetParameter("CHAIN", sz_Chain);



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

void CRFCertificationRx::OnCbnDropdownComboPort()
{
	m_combComport.ResetContent();
	SearchPortUsbAdb(3);
	m_combComport.AddString(m_com.desc);
	m_combComport.SetItemData(0, _wtoi(m_com.port));
	m_com.port.ReleaseBuffer();
}

bool CRFCertificationRx::SearchPortUsbAdb(const UINT secTimeout)
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

bool CRFCertificationRx::FindUsbDevice()
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

bool CRFCertificationRx::FindDeviceId(CString sDeviceId)
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

bool CRFCertificationRx::SearchPortUsb(const UINT secTimeout)
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

CString CRFCertificationRx::GetDeviceName(HDEVINFO& hDevInfo, SP_DEVINFO_DATA& spDevInfoData)
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

bool CRFCertificationRx::GetComPort()
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

bool CRFCertificationRx::GetAdbDevice()
{
	return
		//(m_com.desc.Find(_T("diag")) != -1);
		(m_com.desc.Find(_T("Disgnostics")) != -1);
}

void CRFCertificationRx::OnBnClickedButtonRun2()
{
	bRxTestStop = true;
	// TODO: Add your control notification handler code here
}
