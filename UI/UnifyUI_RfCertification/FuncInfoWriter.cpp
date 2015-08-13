// FuncInfoWriter.cpp : implementation file
//

#include "stdafx.h"
#include "UnifyUI_ServiceTool.h"
#include "FuncInfoWriter.h"
#include "VersionInfo.h"
#include "ServiceCheck.h"
#include "KeyPro.h"


CFuncInfoWriter* g_ui;


// CFuncInfoWriter dialog

IMPLEMENT_DYNAMIC(CFuncInfoWriter, CDialog)

CFuncInfoWriter::CFuncInfoWriter(CWnd* pParent /*=NULL*/)
	: CDialog(CFuncInfoWriter::IDD, pParent)
{

}

CFuncInfoWriter::~CFuncInfoWriter()
{
}

void CFuncInfoWriter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_GRID_INFOEDIT, m_uiInfoEdit);
}


BEGIN_MESSAGE_MAP(CFuncInfoWriter, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CFuncInfoWriter::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BUTTON_READ, &CFuncInfoWriter::OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_BUTTON_FOLDER, &CFuncInfoWriter::OnBnClickedButtonFolder)
	ON_BN_CLICKED(IDC_BUTTON_FOLDER_RSA, &CFuncInfoWriter::OnBnClickedButtonFolderRsa)
	ON_BN_CLICKED(IDC_BUTTON_FOLDER_SIMLOCK, &CFuncInfoWriter::OnBnClickedButtonFolderSimlock)
END_MESSAGE_MAP()


// CFuncInfoWriter message handlers

BOOL CFuncInfoWriter::OnInitDialog()
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

	m_isBrowseFile = false;
	m_isBrowseFileRsa = false;
	
	CString csSimlockPath;
	GetDlgItem(IDC_EDIT_PATH_SIMLOCK)->GetWindowText(csSimlockPath);
	if( csSimlockPath.IsEmpty() == true ){
		m_isBrowseFileSimlock = false;
	}
	m_isRead = false;

	UIInit();

	if (m_data.info.Find(_T("WONLY")) != -1) {
		UIWONLYInit();
	}
	else if ((m_data.info.Find(_T("RW_FILE_KEYLOCK")) != -1)) {
		CString FullMask = _T("C:\\*.qsrvk");
		CString path_key;
		if (IsFindFilePath(FullMask, path_key)) {
			path_key = _T("C:\\") + path_key;
			if(service_key_check(CT2A(path_key.GetBuffer())) == NO_ERROR) {
				UIRW_FILEInit();
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
	else if ((m_data.info.Find(_T("RW_FILE")) != -1)) {
		UIRW_FILEInit();
	}
	else if ((m_data.info.Find(_T("RW")) != -1)) {
		UIRWInit();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CFuncInfoWriter::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	m_dllCtrl.UnLoad();

	return CDialog::DestroyWindow();
}

void CFuncInfoWriter::OnBnClickedButtonRun()
{
	// TODO: Add your control notification handler code here	
	PrintMsg(_T(""), _T("CLEAR"), false);
	m_thrdMainProgress = ::AfxBeginThread(WorkerThreadFuncProc, this, THREAD_PRIORITY_ABOVE_NORMAL);
}

void CFuncInfoWriter::SetInit(dlltoolinfo_t& data)
{
	m_data = data;
}

int CFuncInfoWriter::MsgCall(const char* szMsg, const char* szTag)
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

void CFuncInfoWriter::PrintMsg(CString message, CString tag, bool isLog)
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

	UpdateWindow();

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

int CFuncInfoWriter::UpdatePortCall(const char* szPort)
{
	CString port;
	if (szPort != NULL) {
		port.Format(_T("%s"), CA2T(szPort));
	}
	g_ui->SetPortUpdate(port);

	return 0;
}

void CFuncInfoWriter::SetPortUpdate(CString port)
{
	CComboBox* ccb = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
	ccb->ResetContent();
	ccb->AddString(port);
	ccb->SetCurSel(0);
	Invalidate(FALSE);
}

void CFuncInfoWriter::UIInit()
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 64;
	lf.lfWeight = 400;
	HFONT font = ::CreateFontIndirect(&lf);
	GetDlgItem(IDC_BUTTON_RUN)->SendMessage(WM_SETFONT, (WPARAM)font, TRUE);

	LOGFONT lfRead;
	memset(&lfRead, 0, sizeof(LOGFONT));
	lfRead.lfHeight = 50;
	lfRead.lfWeight = 200;
	HFONT fontRead = ::CreateFontIndirect(&lfRead);
	GetDlgItem(IDC_BUTTON_READ)->SendMessage(WM_SETFONT, (WPARAM)fontRead, TRUE);

	CString title;
	title = m_data.title;
	SetWindowText(title);

	const int MAX_MSG = 2048;
	char buffer[MAX_MSG];
	memset(buffer, 0, MAX_MSG);
	if (m_dllCtrl.GetParameter("msg_process", buffer) == NO_ERROR) {
		GetDlgItem(IDC_EDIT_PROCESS)->SetWindowText(CA2T(buffer));
	}

	int nLockRow = 0;
	memset(buffer, 0, MAX_MSG);
	/*if (m_dllCtrl.GetParameter("info_lock_row_number", buffer) == NO_ERROR) {
		nLockRow = atoi(buffer);
	}*/

	int nPort = m_dllCtrl.GetPortCount();
	CComboBox* ccb = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
	CString port;
	for (int nIndex = 0; nIndex < nPort; nIndex++) {
		memset(buffer, 0, MAX_MSG);
		m_dllCtrl.GetPortByIndex(nIndex, buffer);
		port = CA2T(buffer);
		ccb->AddString(port);
	}

	m_uiInfoEdit.SetColumnCount(2);
	m_uiInfoEdit.SetFixedColumnCount(1);
	m_uiInfoEdit.SetColumnWidth(0, 120);
	m_uiInfoEdit.SetColumnWidth(1, 320);
	m_uiInfoEdit.SetDefCellHeight(30);

	m_uiInfoEdit.SetRowCount(m_dllCtrl.GetInfoCount() + 1);
	//m_uiInfoEdit.SetFixedRowCount(1 + nLockRow);

	m_uiInfoEdit.SetItemText(0, 0, _T("Name"));
	m_uiInfoEdit.SetItemText(0, 1, _T("Info"));

	if (m_data.info.Find(_T("WONLY")) != -1) {
		const int MAX_INFO_LENGTH = 1024;
		char* szName = new char[MAX_PATH];
		char* szInfo = new char[MAX_INFO_LENGTH];
		for (int i = 0; i < m_dllCtrl.GetInfoCount(); i++) {
			memset(szName, 0, MAX_PATH);
			memset(szInfo, 0, MAX_INFO_LENGTH);
			m_dllCtrl.GetInfoByIndex(i, szName, szInfo);
			m_uiInfoEdit.SetItemText(i+1, 0, CA2T(szName));
			m_uiInfoEdit.SetItemText(i+1, 1, CA2T(szInfo));
		}
		delete szName;
		delete szInfo;

		if (m_dllCtrl.GetInfoCount() > 0) {
			GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(false);
			m_uiInfoEdit.EnableWindow(false);
		}
	}

	GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(FALSE);
}

void CFuncInfoWriter:: UIWONLYInit()
{
	UIHide(IDC_BUTTON_READ);
	UIHide(IDC_EDIT_PATH);
	UIHide(IDC_BUTTON_FOLDER);
	UIHide(IDC_STATIC_V1);
	GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(true);
	GetDlgItem(IDC_GRID_INFOEDIT)->EnableWindow(true);
}

void CFuncInfoWriter:: UIRWInit()
{
	UIHide(IDC_EDIT_PATH);
	UIHide(IDC_BUTTON_FOLDER);
	UIHide(IDC_STATIC_V1);
}

void CFuncInfoWriter:: UIRW_FILEInit()
{

}

void CFuncInfoWriter::UIHide(const int& item)
{
	GetDlgItem(item)->ShowWindow(SW_HIDE);
}

void CFuncInfoWriter::UIWarning(CString message)
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

void CFuncInfoWriter::UIControl(bool isEnable, bool isPass)
{
	GetDlgItem(IDC_COMBO_PORT)->EnableWindow(isEnable);
	GetDlgItem(IDC_GRID_INFOEDIT)->EnableWindow(isEnable);
	if (!isEnable && !isPass) {
		GetDlgItem(IDC_BUTTON_FOLDER)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_FOLDER_RSA)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_FOLDER_SIMLOCK)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_READ)->EnableWindow(false);
	}

	if (m_data.info.Find(_T("WONLY")) != -1 && isEnable) {
		GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(TRUE);	
	}
	else if (m_data.info.Find(_T("RW_FILE")) != -1 && isEnable) {

		if (isEnable && (!m_isRead || !m_isBrowseFile))
		{
			GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(false);
		}

		if (isEnable && m_isRead && m_isBrowseFile) {
			GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(true);
			
		}
		if (isEnable && isPass)
		{
			GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(false);
			m_isRead = false;
			m_isBrowseFile = false;
		}
		
		GetDlgItem(IDC_BUTTON_READ)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_FOLDER)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_FOLDER_RSA)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_FOLDER_SIMLOCK)->EnableWindow(true);
	}
	else if (m_data.info.Find(_T("RW")) != -1 && isEnable) {
		if (isEnable && m_isRead) {
			GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(true);
		}
		if (isEnable && isPass) {
			GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(false);
			m_isRead = false;
		}
		GetDlgItem(IDC_BUTTON_READ)->EnableWindow(true);
	}
}

void CFuncInfoWriter::UITotalTime(double secTime)
{
	CString csTime;
	csTime.Format(_T("%.2f sec"), secTime);
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText(csTime);
}

void CFuncInfoWriter::Timer(bool isEnable)
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

UINT CFuncInfoWriter::WorkerThreadTimerProc(LPVOID param)
{
	CFuncInfoWriter* pThis = reinterpret_cast<CFuncInfoWriter*>(param);
	ASSERT(pThis != NULL);

	pThis->WorkerThreadTimerRun();

	return TRUE;
}

void CFuncInfoWriter::WorkerThreadTimerRun()
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

UINT CFuncInfoWriter::WorkerThreadFuncProc(LPVOID param)
{
	CFuncInfoWriter* pThis = reinterpret_cast<CFuncInfoWriter*>(param);
	ASSERT(pThis != NULL);

	pThis->WorkerThreadFuncRun();

	return TRUE;
}

UINT CFuncInfoWriter::ReadThreadFuncProc(LPVOID param)
{
	CFuncInfoWriter* pThis = reinterpret_cast<CFuncInfoWriter*>(param);
	ASSERT(pThis != NULL);

	pThis->ReadThreadFuncRun();

	return TRUE;
}

void CFuncInfoWriter::WorkerThreadFuncRun()
{
	if (m_data.isLockDongle && CheckKeyPro() != NO_ERROR) {
		return;
	}

	Timer(true);
	UIControl(false, false);
	int nRetCode = NO_ERROR;

	CString path;
	CString csRsaPath;
	CString csParameterValue;
	if (m_data.info.Find(_T("RW_FILE")) != -1)
	{
		GetDlgItem(IDC_EDIT_PATH)->GetWindowText(path);
		nRetCode = m_dllCtrl.SetParameter("image_path", CT2A(path.GetBuffer()));
		path.ReleaseBuffer();

		csParameterValue = m_csFsn;
		nRetCode = m_dllCtrl.SetParameter("fsn_value", CT2A(csParameterValue.GetBuffer()));
		csParameterValue.ReleaseBuffer();

		csParameterValue = m_csNetworkCode;
		nRetCode = m_dllCtrl.SetParameter("nw_code", CT2A(csParameterValue.GetBuffer()));
		csParameterValue.ReleaseBuffer();

		GetDlgItem(IDC_EDIT_PATH_RSA)->GetWindowText(csRsaPath);
		nRetCode = m_dllCtrl.SetParameter("rsa_path", CT2A(csRsaPath.GetBuffer()));
		csRsaPath.ReleaseBuffer();

		GetDlgItem(IDC_EDIT_PATH_SIMLOCK)->GetWindowText(csParameterValue);
		nRetCode = m_dllCtrl.SetParameter("simlock_path", CT2A(csParameterValue.GetBuffer()));
		csParameterValue.ReleaseBuffer();
	}

	CString port;
	GetDlgItem(IDC_COMBO_PORT)->GetWindowText(port);
	m_dllCtrl.SetParameter("port", CT2A(port.GetBuffer()));
	port.ReleaseBuffer();

	// pre run
	m_dllCtrl.PreRun();

	if (nRetCode == NO_ERROR) {
		CString sName;
		CString sInfo;
		for (int nRow = m_uiInfoEdit.GetFixedRowCount(); nRow < m_uiInfoEdit.GetRowCount(); nRow++) {
			sName.Empty();
			sInfo.Empty();
			sName = m_uiInfoEdit.GetItemText(nRow, 0);
			sInfo = m_uiInfoEdit.GetItemText(nRow, 1);

			if (!sName.IsEmpty()) {
				sInfo.Trim();
				nRetCode = m_dllCtrl.SetParameter(CT2A(sName), CT2A(sInfo));
				if (nRetCode != NO_ERROR)
					break;
			}
		}
	}
	// run
	if (nRetCode == NO_ERROR) {
		nRetCode = m_dllCtrl.Run();
	}
	
	// if run success, read info list from device
	if (nRetCode == NO_ERROR) {
		ReadList();
	}

	// post run
	m_dllCtrl.PostRun();

	if (nRetCode == NO_ERROR) {
		::DeleteFile(path);
		::DeleteFile(csRsaPath);
	}

	GetDlgItem(IDC_EDIT_PATH)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_PATH_RSA)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_PATH_SIMLOCK)->SetWindowText(_T(""));

	m_isBrowseFile = false;
	m_isBrowseFileRsa = false;

	UIControl(true, nRetCode == NO_ERROR);
	Timer(false);
	CString csTime;
	GetDlgItem(IDC_STATIC_TIME)->GetWindowText(csTime);
	csTime = _T("Elapsed Time: ") + csTime + _T("\n\n");
	PrintMsg(csTime, _T(""));
}

void CFuncInfoWriter::OnBnClickedButtonRead()
{
	PrintMsg(_T(""), _T("CLEAR"), false);
	m_thrdReadProgress = ::AfxBeginThread(ReadThreadFuncProc, this, THREAD_PRIORITY_ABOVE_NORMAL);
}

void CFuncInfoWriter::ReadThreadFuncRun()
{
	if (m_data.isLockDongle && CheckKeyPro() != NO_ERROR) {
		return;
	}

	UIControl(false, false);
	Timer(true);

	const int MAX_MSG = 2048;
	char lock_rows[MAX_MSG];

	int nLockRow = 0;
	memset(lock_rows, 0, MAX_MSG);
	if (m_dllCtrl.GetParameter("info_lock_row_number", lock_rows) == NO_ERROR)
	{
		nLockRow = atoi(lock_rows);

		m_uiInfoEdit.SetRowCount(m_dllCtrl.GetInfoCount() + 1);
		m_uiInfoEdit.SetFixedRowCount(1 + nLockRow);

		m_uiInfoEdit.SetItemText(0, 0, _T("Name"));
		m_uiInfoEdit.SetItemText(0, 1, _T("Info"));

		const int MAX_INFO_LENGTH = 1024;
		char* szName = new char[MAX_PATH];
		char* szInfo = new char[MAX_INFO_LENGTH];
		for (int i = 0; i < m_dllCtrl.GetInfoCount(); i++) {
			memset(szName, 0, MAX_PATH);
			memset(szInfo, 0, MAX_INFO_LENGTH);
			m_dllCtrl.GetInfoByIndex(i, szName, szInfo);
			m_uiInfoEdit.SetItemText(i+1, 0, CA2T(szName));
			m_uiInfoEdit.SetItemText(i+1, 1, CA2T(szInfo));
		}
		delete szName;
		delete szInfo;

		m_isRead = true;
	}
	else
	{
		m_isRead = false;
	}

	GetDlgItem(IDC_BUTTON_READ)->EnableWindow(TRUE);

	Timer(false);
	CString csTime;
	GetDlgItem(IDC_STATIC_TIME)->GetWindowText(csTime);
	csTime = _T("Elapsed Time: ") + csTime + _T("\n\n");
	PrintMsg(csTime, _T(""));

	UIControl(true, false);
}

void CFuncInfoWriter::OnBnClickedButtonFolder()
{
	// TODO: Add your control notification handler code here
	CString pthFolder;
	bool isDoSelect = false;
	CFileDialog fd(TRUE, _T(""), _T(""), OFN_FILEMUSTEXIST, _T(""));
	fd.m_ofn.lpstrTitle = _T("Browse for write file");
	if (fd.DoModal() == IDOK) {
		pthFolder = fd.GetPathName();
		GetDlgItem(IDC_EDIT_PATH)->SetWindowText(pthFolder);
		UpdateData(false);
		isDoSelect = true;
		m_isBrowseFile = true;
		if (m_isRead && m_isBrowseFile && m_isBrowseFileRsa && m_isBrowseFileSimlock) {
			GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(true);
		}
	}
	else {
		isDoSelect = false;
	}
}

void CFuncInfoWriter::OnBnClickedButtonFolderRsa()
{
	// TODO: Add your control notification handler code here
	CString pthFolder;
	bool isDoSelect = false;
	CFileDialog fd(TRUE, _T(""), _T(""), OFN_FILEMUSTEXIST, _T(""));
	fd.m_ofn.lpstrTitle = _T("Browse for RSA file");
	if (fd.DoModal() == IDOK) {
		pthFolder = fd.GetPathName();
		GetDlgItem(IDC_EDIT_PATH_RSA)->SetWindowText(pthFolder);
		UpdateData(false);
		isDoSelect = true;
		m_isBrowseFileRsa = true;
		if (m_isRead && m_isBrowseFile && m_isBrowseFileRsa && m_isBrowseFileSimlock) {
			GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(true);
		}
	}
	else {
		isDoSelect = false;
	}
}

void CFuncInfoWriter::OnBnClickedButtonFolderSimlock()
{
	// TODO: Add your control notification handler code here
	CString pthFolder;
	bool isDoSelect = false;
	CFileDialog fd(TRUE, _T(""), _T(""), OFN_FILEMUSTEXIST, _T(""));
	fd.m_ofn.lpstrTitle = _T("Browse for Simlock file");
	if (fd.DoModal() == IDOK) {
		pthFolder = fd.GetPathName();
		GetDlgItem(IDC_EDIT_PATH_SIMLOCK)->SetWindowText(pthFolder);
		UpdateData(false);
		isDoSelect = true;
		m_isBrowseFileSimlock = true;
		if (m_isRead && m_isBrowseFile && m_isBrowseFileRsa && m_isBrowseFileSimlock) {
			GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(true);
		}
	}
	else {
		isDoSelect = false;
	}
}

void CFuncInfoWriter::ReadList()
{
	//const int MAX_MSG = 2048;
	//char buffer[MAX_MSG];
	//int nLockRow = 0;
	//memset(lock_rows, 0, MAX_MSG);
	//if (m_dllCtrl.GetParameter("info_lock_row_number", lock_rows) == NO_ERROR)
	//{
		//nLockRow = atoi(lock_rows);

		m_uiInfoEdit.SetRowCount(m_dllCtrl.GetInfoCount() + 1);
		//m_uiInfoEdit.SetFixedRowCount(1 + nLockRow);

		m_uiInfoEdit.SetItemText(0, 0, _T("Name"));
		m_uiInfoEdit.SetItemText(0, 1, _T("Info"));

		const int MAX_INFO_LENGTH = 1024;
		char* szName = new char[MAX_PATH];
		char* szInfo = new char[MAX_INFO_LENGTH];
		for (int i = 0; i < m_dllCtrl.GetInfoCount(); i++) {
			memset(szName, 0, MAX_PATH);
			memset(szInfo, 0, MAX_INFO_LENGTH);
			m_dllCtrl.GetInfoByIndex(i, szName, szInfo);
			m_uiInfoEdit.SetItemText(i+1, 0, CA2T(szName));
			m_uiInfoEdit.SetItemText(i+1, 1, CA2T(szInfo));
			m_uiInfoEdit.SetItemText(i+1, 1, CA2T(szInfo));
		}
		delete szName;
		delete szInfo;
		Invalidate(FALSE);
	//}
}
