// RFCertification.cpp : implementation file
//

#include "stdafx.h"
#include "UnifyUI_ServiceTool.h"
#include "RFCertificationTx.h"
#include "VersionInfo.h"

#include <comdef.h> // _com_error in HwDetectSetup
#include <Dbt.h> // DBT_DEVTYP_DEVICEINTERFACE in OnDeviceChange
#include <psapi.h> // GetModuleFileNameEx
#pragma comment(lib, "psapi")

CRFCertificationTx* g_ui;

// CRFCertificationTx dialog

IMPLEMENT_DYNAMIC(CRFCertificationTx, CDialog)

CRFCertificationTx::CRFCertificationTx(CWnd* pParent /*=NULL*/)
	: CDialog(CRFCertificationTx::IDD, pParent)
	, m_sChannel(_T(""))
	, m_sPower(_T(""))
	, m_sRateBitIndex(_T(""))
{

}

CRFCertificationTx::~CRFCertificationTx()
{
}

void CRFCertificationTx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PORT, m_combComport);

	DDX_Text(pDX, IDC_EDIT_TX_CHANNEL, m_sChannel);
	DDX_Text(pDX, IDC_EDIT_TX_POWER2, m_sPower);
	DDX_Text(pDX, IDC_EDIT_TX_RateBitIndex, m_sRateBitIndex);

	DDX_Control(pDX, IDC_EDIT_TX_WlanMode, m_combWlanMode);
	DDX_Control(pDX, IDC_EDIT_TX_CHAIN, m_combChain);
}


BEGIN_MESSAGE_MAP(CRFCertificationTx, CDialog)
	//ON_CBN_SELCHANGE(IDC_COMBO_MODE, &CRFCertificationTx::OnCbnSelchangeComboMode)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CRFCertificationTx::OnBnClickedButtonRun)
	ON_CBN_DROPDOWN(IDC_COMBO_PORT, &CRFCertificationTx::OnCbnDropdownComboPort)
END_MESSAGE_MAP()


// CRFCertificationTx message handlers
BOOL CRFCertificationTx::OnInitDialog()
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

BOOL CRFCertificationTx::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	m_dllCtrl.UnLoad();

	return CDialog::DestroyWindow();
}

void CRFCertificationTx::SetInit(dlltoolinfo_t& data)
{
	m_data = data;
}

int CRFCertificationTx::MsgCall(const char* szMsg, const char* szTag)
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

void CRFCertificationTx::PrintMsg(CString message, CString tag, bool isLog)
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

int CRFCertificationTx::UpdatePortCall(const char* szPort)
{
	CString port;
	if (szPort != NULL) {
		port.Format(_T("%s"), CA2T(szPort));
	}
	g_ui->SetPortUpdate(port);

	return 0;
}

void CRFCertificationTx::SetPortUpdate(CString port)
{
	CComboBox* ccb = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
	ccb->ResetContent();
	ccb->AddString(port);
	ccb->SetCurSel(0);
	Invalidate(FALSE);
}

int CRFCertificationTx::ResultCall(const int nIndex, const char* szUnit, const char* szLower, const char* szUpper, const char* szMeasured, const char* szResult, const char* szErrCode, const char* szMsg)
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

void CRFCertificationTx::SetResult(int nIndex, char* szUnit, char* szLower, char* szUpper, char* szMeasured, char* szResult, char* szErrCode, char* szMsg)
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

void CRFCertificationTx::UIInit()
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

	//m_combChain.AddString(_T("0")); 
	//m_combChain.AddString(_T("1"));
	//for (int i = 0; i < m_combChain.GetCount(); i++)
	//{
	//   m_combChain.SetItemData(i, i);
	//}
	//m_combChain.SetCurSel(0);

	//m_combWlanMode.AddString(_T("0"));
	//m_combWlanMode.AddString(_T("1"));
	//m_combWlanMode.AddString(_T("4"));
	//m_combWlanMode.AddString(_T("5"));
	//m_combWlanMode.AddString(_T("8"));
	//m_combWlanMode.SetItemData(0, 0);
	//m_combWlanMode.SetItemData(1, 1);
	//m_combWlanMode.SetItemData(2, 4);
	//m_combWlanMode.SetItemData(3, 5);
	//m_combWlanMode.SetItemData(4, 8);
	//m_combWlanMode.SetCurSel(0);
	//m_combChain.SetItemData(1, i);


}

void CRFCertificationTx::UIControl(bool isEnable)
{
	GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(isEnable);
}

void CRFCertificationTx::UITotalTime(double secTime)
{
	CString csTime;
	csTime.Format(_T("%.2f sec"), secTime);
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText(csTime);
}

void CRFCertificationTx::Timer(bool isEnable)
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

UINT CRFCertificationTx::WorkerThreadTimerProc(LPVOID param)
{
	CRFCertificationTx* pThis = reinterpret_cast<CRFCertificationTx*>(param);
	ASSERT(pThis != NULL);

	pThis->WorkerThreadTimerRun();

	return TRUE;
}

void CRFCertificationTx::WorkerThreadTimerRun()
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

UINT CRFCertificationTx::WorkerThreadFuncProc(LPVOID param)
{
	CRFCertificationTx* pThis = reinterpret_cast<CRFCertificationTx*>(param);
	ASSERT(pThis != NULL);

	pThis->WorkerThreadFuncRun();

	return TRUE;
}

void CRFCertificationTx::WorkerThreadFuncRun()
{
	Timer(true);
	UIControl(false);
	

	char *sz_ComValue = new char[10];
	sprintf(sz_ComValue, "%d", m_combComport.GetItemData(m_combComport.GetCurSel()));
	m_dllCtrl.SetParameter("COM", sz_ComValue);

	//CString csTxCHANNEL;
	//CString csPower;
	//CString csRateBitIndex;
	//CString csWlanMode;
	//CString csChain;

	//GetDlgItem(IDC_EDIT_TX_CHANNEL)->GetWindowText(csTxCHANNEL);
	//GetDlgItem(IDC_EDIT_TX_POWER2)->GetWindowText(csPower);
	//GetDlgItem(IDC_EDIT_TX_RateBitIndex)->GetWindowText(csRateBitIndex);
	//GetDlgItem(IDC_EDIT_TX_WlanMode)->GetWindowText(csWlanMode);
	//GetDlgItem(IDC_EDIT_TX_CHAIN)->GetWindowText(csChain);

	m_dllCtrl.SetParameter("CHANNEL", CT2A(csTxCHANNEL.GetBuffer()));
	m_dllCtrl.SetParameter("POWER", CT2A(csPower.GetBuffer()));
	m_dllCtrl.SetParameter("RATEBITINDEX", CT2A(csRateBitIndex.GetBuffer()));
	m_dllCtrl.SetParameter("WLANMODE", CT2A(csWlanMode.GetBuffer()));
	m_dllCtrl.SetParameter("CHAIN", CT2A(csChain.GetBuffer()));


	csTxCHANNEL.ReleaseBuffer();
	csPower.ReleaseBuffer();
	csRateBitIndex.ReleaseBuffer();
	csWlanMode.ReleaseBuffer();
	csChain.ReleaseBuffer();

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

//void CRFCertificationTx::OnCbnSelchangeComboMode()
//{
//	if (m_combMode.GetCurSel() == 0)
//	{
//		m_combRate.ResetContent();
//		//m_combRate.AddString(_T("11b LONG 1 Mbps"));
//		//m_combRate.AddString(_T("11b LONG 2 Mbps"));
//		//m_combRate.AddString(_T("11b LONG 5.5 Mbps"));
//		//m_combRate.AddString(_T("11b LONG 11 Mbps"));
//		//m_combRate.AddString(_T("11b SHORT 2 Mbps"));
//		//m_combRate.AddString(_T("11b SHORT 5.5 Mbps"));
//		//m_combRate.AddString(_T("11b SHORT 11 Mbps"));
//
//		m_combRate.AddString(_T("11b RATE_1Mbps_L"));
//		m_combRate.AddString(_T("11b RATE_2Mbps_L"));
//		m_combRate.AddString(_T("11b RATE_2Mbps_S"));
//		m_combRate.AddString(_T("11b RATE_5_5Mbps_L"));
//		m_combRate.AddString(_T("11b RATE_5_5Mbps_S"));
//		m_combRate.AddString(_T("11b RATE_11Mbps_L"));
//		m_combRate.AddString(_T("11b RATE_11Mbps_S"));
//
//		m_combRate.SetCurSel(0);
//		for (int i = 0; i <= 6; i++)
//		{
//			m_combRate.SetItemData(i, i);
//		}
//	}
//	else if(m_combMode.GetCurSel() == 1)
//	{
//		m_combRate.ResetContent();
//		m_combRate.AddString(_T("11g 6 Mbps"));
//		m_combRate.AddString(_T("11g 9 Mbps"));
//		m_combRate.AddString(_T("11g 12 Mbps"));
//		m_combRate.AddString(_T("11g 18 Mbps"));
//		m_combRate.AddString(_T("11g 24 Mbps"));
//		m_combRate.AddString(_T("11g 36 Mbps"));
//		m_combRate.AddString(_T("11g 48 Mbps"));
//		m_combRate.AddString(_T("11g 54 Mbps"));
//		m_combRate.SetCurSel(0);
//		for (int i = 0; i <= 7; i++)
//		{
//			m_combRate.SetItemData(i, i+7);
//		}
//	}
//	else if(m_combMode.GetCurSel() == 2)
//	{
//		m_combRate.ResetContent();
//		m_combRate.AddString(_T("MCS0 6.5 Mbps"));
//		m_combRate.AddString(_T("MCS1 13 Mbps"));
//		m_combRate.AddString(_T("MCS2 19.5 Mbps"));
//		m_combRate.AddString(_T("MCS3 26 Mbps"));
//		m_combRate.AddString(_T("MCS4 39 Mbps"));
//		m_combRate.AddString(_T("MCS5 52 Mbps"));
//		m_combRate.AddString(_T("MCS6 58.5 Mbps"));
//		m_combRate.AddString(_T("MCS7 65 Mbps"));
//		m_combRate.AddString(_T("MCS0 MM SG 7.2 Mbps"));
//		m_combRate.AddString(_T("MCS1 MM SG 14.4 Mbps"));
//		m_combRate.AddString(_T("MCS2 MM SG 21.7 Mbps"));
//		m_combRate.AddString(_T("MCS3 MM SG 28.9 Mbps"));
//		m_combRate.AddString(_T("MCS4 MM SG 43.3 Mbps"));
//		m_combRate.AddString(_T("MCS5 MM SG 57.8 Mbps"));
//		m_combRate.AddString(_T("MCS6 MM SG 65 Mbps"));
//		m_combRate.AddString(_T("MCS7 MM SG 72.2 Mbps"));
//		m_combRate.SetCurSel(0);
//		for (int i = 0; i <= 15; i++)
//		{
//			m_combRate.SetItemData(i, i+23);
//		}
//	}
//}
//
void CRFCertificationTx::OnBnClickedButtonRun()
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
		PrintMsg(_T("Please select CHAIN.\n"), _T("WARN"));
		return;
	}

	GetDlgItem(IDC_EDIT_TX_CHANNEL)->GetWindowText(csTxCHANNEL);
	GetDlgItem(IDC_EDIT_TX_POWER2)->GetWindowText(csPower);
	GetDlgItem(IDC_EDIT_TX_RateBitIndex)->GetWindowText(csRateBitIndex);
	GetDlgItem(IDC_EDIT_TX_WlanMode)->GetWindowText(csWlanMode);
	GetDlgItem(IDC_EDIT_TX_CHAIN)->GetWindowText(csChain);

	if ( csTxCHANNEL == "") { 
		PrintMsg(_T("Please fill in csTxCHANNEL.\n"), _T("WARN"));
		return ;
	}
	if ( csPower == "") { 
		PrintMsg(_T("Please fill in csPower.\n"), _T("WARN"));
		return ;
	}
	if ( csRateBitIndex == "") { 
		PrintMsg(_T("Please fill in csRateBitIndex.\n"), _T("WARN"));
		return ;
	}
	if ( csWlanMode == "") { 
		PrintMsg(_T("Please fill in csWlanMode.\n"), _T("WARN"));
		return ;
	}
	if ( csChain == "") { 
		PrintMsg(_T("Please fill in csChain.\n"), _T("WARN"));
		return ;
	}
	//if (m_combWlanMode.GetCurSel() == CB_ERR)
	//{
	//	PrintMsg(_T("Please select WLANMODE.\n"), _T("WARN"));
	//	return;
	//}	
	
	//if (m_sPayload.IsEmpty() || m_sSpacing.IsEmpty() || m_sPower.IsEmpty())
	//{
	//	PrintMsg(_T("Please enter payload/spacing/power in the input field.\n"), _T("WARN"));
	//	return;
	//}


	m_thrdMainProgress = ::AfxBeginThread(WorkerThreadFuncProc, this, THREAD_PRIORITY_ABOVE_NORMAL);
}

bool CRFCertificationTx::SearchPortUsbAdb(const UINT secTimeout)
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

bool CRFCertificationTx::FindUsbDevice()
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

bool CRFCertificationTx::FindDeviceId(CString sDeviceId)
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

bool CRFCertificationTx::SearchPortUsb(const UINT secTimeout)
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

CString CRFCertificationTx::GetDeviceName(HDEVINFO& hDevInfo, SP_DEVINFO_DATA& spDevInfoData)
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

bool CRFCertificationTx::GetComPort()
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

bool CRFCertificationTx::GetAdbDevice()
{
	return
		//(m_com.desc.Find(_T("diag")) != -1);
		(m_com.desc.Find(_T("Disgnostics")) != -1);
}

void CRFCertificationTx::OnCbnDropdownComboPort()
{
	m_combComport.ResetContent();
	SearchPortUsbAdb(3);
	m_combComport.AddString(m_com.desc);
	m_combComport.SetItemData(0, _wtoi(m_com.port));
	m_com.port.ReleaseBuffer();
}
