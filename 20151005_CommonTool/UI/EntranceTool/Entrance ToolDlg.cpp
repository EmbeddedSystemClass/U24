// Entrance ToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "afxwin.h"
#include "Entrance Tool.h"
#include "Entrance ToolDlg.h"
#include <tchar.h> 


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CEntranceToolDlg 对话框

bool  b_wait_over = false;
bool  b_wait_check = false;
CString str_gl_errorcode = _T("");
CString str_gl_errorMsg = _T("");


CEntranceToolDlg::CEntranceToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEntranceToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_str_SONumber = _T(""); 
	m_str_ImployeeID = _T("");
	m_str_Model = _T("");
	m_str_Process = _T("");

	m_str_PartNumber = _T("");
	m_str_TSFlag = _T("0");

	m_str_ToolVersion = _T("");
	m_str_ImageVersion = _T("");
	m_str_ProjectName = _T("");
	m_str_EntranceToolVersion = _T("V2.021");
	m_str_state = _T("");

	b_check_SO = false;
	b_check_ImployeeID = false;
	b_syc = true;
	m_b_state = false;
	//msgDlg = new MsgDlg();

	m_nAccessoryCount = 0;
	
}

void CEntranceToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_COMBO_Station, m_COMBO_Station);
	DDX_Control(pDX, IDC_COMBO_PROJECT, m_cb_Project);
	DDX_Control(pDX, IDC_COMBO_PROCESS, m_cb_Process);
	DDX_Control(pDX, IDC_COMBO_ACCESSORY, m_cb_Accessory);
	DDX_Control(pDX, IDC_LIST_ACCESSORY, m_cl_Accessory);
	DDX_Control(pDX, IDC_COMBO1, m_combox_State);
}

BEGIN_MESSAGE_MAP(CEntranceToolDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CEntranceToolDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CEntranceToolDlg::OnBnClickedCancel)
	ON_WM_CTLCOLOR()
	ON_EN_KILLFOCUS(IDC_EDIT_SO, &CEntranceToolDlg::OnEnKillfocusEditSo)
	ON_EN_KILLFOCUS(IDC_EDIT_EmployeeID, &CEntranceToolDlg::OnEnKillfocusEditEmployeeid)
	ON_EN_SETFOCUS(IDC_EDIT_SO, &CEntranceToolDlg::OnEnSetfocusEditSo)
	ON_EN_SETFOCUS(IDC_EDIT_EmployeeID, &CEntranceToolDlg::OnEnSetfocusEditEmployeeid)
	ON_EN_CHANGE(IDC_EDIT_EmployeeID, &CEntranceToolDlg::OnEnChangeEditEmployeeid)
	ON_EN_CHANGE(IDC_EDIT_SO, &CEntranceToolDlg::OnEnChangeEditSo)
	ON_CBN_SELCHANGE(IDC_COMBO_PROJECT, &CEntranceToolDlg::OnCbnSelchangeComboProject)
	ON_CBN_SELCHANGE(IDC_COMBO_PROCESS, &CEntranceToolDlg::OnCbnSelchangeComboProcess)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CEntranceToolDlg::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO_ACCESSORY, &CEntranceToolDlg::OnCbnSelchangeComboAccessory)
END_MESSAGE_MAP()


// CEntranceToolDlg 消息处理程序

BOOL CEntranceToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	
	SetIcon(m_hIcon, TRUE);	
	SetIcon(m_hIcon, FALSE);

	TCHAR szApp[MAX_PATH] = {0};

	GetModuleFileName(NULL,szApp,MAX_PATH);
	
	TCHAR VersionInfo[1000];
	if (GetFileVersion(szApp, VersionInfo, 1000))
	{
		//CString strtemp = VersionInfo;//1.0.0.1
		//m_str_EntranceToolVersion.Format("V%s%s%s",strtemp.Left(3),strtemp.Mid(4,1),strtemp.Mid(6,1));//V1.001

		m_Clog.WriteLog(m_str_EntranceToolVersion);
	}

	GetDlgItem(IDOK)->SetWindowText(_T("确定(OK)"));
	GetDlgItem(IDCANCEL)->SetWindowText(_T("取消(Cancel)"));
	GetDlgItem(IDC_EDIT_SO)->SetWindowText(_T("请输入7位数字工单"));
	((CEdit*)GetDlgItem(IDC_EDIT_SO))->SetLimitText(7);
	GetDlgItem(IDC_EDIT_EmployeeID)->SetWindowText(_T("请输入7或者8位员工ID"));
	((CEdit*)GetDlgItem(IDC_EDIT_EmployeeID))->SetLimitText(8);
	GetDlgItem(IDC_STATIC_SO2)->SetWindowText(_T("工单(SO):"));
	GetDlgItem(IDC_STATIC_ImployeeID)->SetWindowText(_T("工号(Employee ID):"));

	//SetDialogBkColor(RGB(0,192,0),RGB(255,255,255));
	int nInsertIndex = 0;
	GetDlgItem(IDC_STATIC_PROJECT)->SetWindowText(_T("项目(Project_Model):"));
	m_cb_Project.InsertString(nInsertIndex++, _T("请选择项目Model"));


	//sz_ProcessList.Format(_T("%s"),szProcessList);
	//result=strtok(szProcessList,rule);


	//result = strtok(szProcessList, rule);
	/*
	while (result!=NULL) 
	{
	//len=strlen(result);
	//memcpy(find,result,len);
	//m_cb_Project.InsertString(i,find);
	result = strtok(NULL, rule); 
	}
	*/

	TCHAR szProcessList[2048]={0};
	TCHAR szErrorCode[512]={0};
	TCHAR szErrorMsg[512]={0};
	TCHAR sz_count[512]={0};
	//TCHAR rule[]=",";
	//TCHAR *result;
	//
	CString str_find=_T("");
	CString str_Record=_T("");
	//int i=1;
	//int len=0;
	bool b_res = false;
	if(m_Funccheck.FconnectDB())
	{
		b_res = m_Funccheck.ProjectName_Return(szProcessList,sz_count,szErrorCode,szErrorMsg);
	}

	str_find.Format(_T("%s"),szProcessList);

	while(str_find.GetLength() > 0)
	{
		int nIndex = str_find.Find(_T(','));
		CString strItem = str_find.Left(nIndex);
		str_find = str_find.Mid(nIndex+1);
		if(strItem.MakeUpper()==_T("ENTRANCE_ENTRANCE")||strItem.MakeUpper()==_T("QA_QA")||strItem.MakeUpper()==_T("VI_VI")||strItem.MakeUpper()==_T("KEYPARTSLINK_KEYPARTSLINK")||
			strItem.MakeUpper()==_T("RUNIN_RUNIN")||strItem.MakeUpper()==_T("KEYPARTSLINK_KEYPARTSLINK")||strItem.MakeUpper()==_T("SHIPPINGQUALITYCTRL_SHIPPINGQUALITYCTRL")||strItem.MakeUpper()==_T("UNIFY_UI_UNIFY_UI"))
		{

		}
		else
		{
			m_cb_Project.InsertString(nInsertIndex++, strItem);
		}

	}

	str_Record.Format("查询得到的Project_Name为%s",szProcessList);
	m_Clog.WriteLog(str_Record,szErrorCode,szErrorMsg);

	//  result = strtok(szProcessList, rule);
	//  GetDlgItem(IDC_STATIC_PROJECT)->SetWindowText(_T("项目(Project_Model):"));
	//  m_cb_Project.InsertString(0, _T("请选择项目Model"));

	//while (result!=NULL) 
	//{   
	//	TCHAR find[512]={0};
	//	len=strlen(result);
	//	memcpy(find,result,len);
	//	str_find.Format(_T("%s"),find);
	//	if(str_find.MakeUpper()==_T("ENTRANCE_ENTRANCE")||str_find.MakeUpper()==_T("QA_QA")||str_find.MakeUpper()==_T("VI_VI")||str_find.MakeUpper()==_T("KEYPARTSLINK_KEYPARTSLINK")||
	//		str_find.MakeUpper()==_T("RUNIN_RUNIN")||str_find.MakeUpper()==_T("KEYPARTSLINK_KEYPARTSLINK")||str_find.MakeUpper()==_T("SHIPPINGQUALITYCTRL_SHIPPINGQUALITYCTRL")||str_find.MakeUpper()==_T("UNIFY_UI_UNIFY_UI"))
	//	{
	//        
	//	}
	//	else
	//	{
	//        m_cb_Project.InsertString(i,find);
	//	 i++;
	//	}
	//	result = strtok(NULL, rule); 
	//}


	//m_cb_Project.InsertString(1, _T("GORAB1A_Orange"));
	//m_cb_Project.InsertString(2, _T("GWOLB2A_Wolf"));
	//m_cb_Project.InsertString(3, _T("Luna_LG7_GLUNB4A"));
	//m_cb_Project.InsertString(4, _T("GORACS1_Orange"));
	//m_cb_Project.InsertString(5, _T("GORACS2_Orange"));
	//m_cb_Project.InsertString(6, _T("GLUACS1_Orange"));
	//m_cb_Project.InsertString(7, _T("GLUACS2_Orange"));
	//m_cb_Project.InsertString(8, _T("GCHIB1A_Chicago"));

	m_cb_Project.SetCurSel(0);

	m_combox_State.SetWindowText(_T("请选择项目状态"));
	m_combox_State.InsertString(0, _T("请选择项目状态"));
	m_combox_State.InsertString(1, _T("CR"));
	m_combox_State.InsertString(2, _T("Normal"));

	m_combox_State.SetCurSel(0);

	m_cl_Accessory.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES | LVS_EDITLABELS | LVS_EX_INFOTIP);
	m_cl_Accessory.InsertColumn(0, _T("Accessory Type"), LVCFMT_LEFT, 80);
	m_cl_Accessory.InsertColumn(1, _T("Accessory ID"), LVCFMT_CENTER, 200);

	m_cl_Accessory.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cl_Accessory.EnsureVisible(1, TRUE);


	GetDlgItem(IDC_STATIC_PROCESS)->SetWindowText(_T("站别(Station):"));

	GetDlgItem(IDC_State)->SetWindowText(_T("状态(State):"));

	CString str_windowTitle = _T("");

	str_windowTitle = _T("EntranceTool_") + m_str_EntranceToolVersion;
	SetWindowText(str_windowTitle);
//	AfxMessageBox(str_windowTitle);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CEntranceToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRect rect;
        CPaintDC dc(this);
        GetClientRect(rect);
#ifdef RD_MODE
		dc.FillSolidRect(rect,RGB(0,255,0)); //RD版设置为高绿色背景
#else
		dc.FillSolidRect(rect,RGB(215,240,198)); //设置为绿色背景
#endif
        
		CDialog::OnPaint();
	}
}

bool CEntranceToolDlg::Fchecknumber(CString str_number)
{
	TCHAR c_tmpBuffer;
	TCHAR c_0 = _T('0');
	TCHAR c_9 = _T('9');
	bool b_res = false;
	for(int i=0;i<str_number.GetLength();i++)
	{
		c_tmpBuffer = str_number.GetAt(i); 
		if(c_tmpBuffer >= c_0 && c_tmpBuffer <= c_9)// '0'  48, '9' 57
		{
			//符合数字的条件} 
			b_res = true;
		}
		else
		{
			b_res = false;
		}
	}
	return b_res;
}
//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。

HBRUSH CEntranceToolDlg::CreateHBRUSH()
{
	m_brush.CreateSolidBrush(RGB(215,240,198));  // 淡绿色 - 对话框背景色
	return m_brush;
}

HBRUSH CEntranceToolDlg::DeleteHBRUSH()
{
	m_brush.DeleteObject();
	return m_brush;
}

HCURSOR CEntranceToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


UINT ThreadFunc(LPVOID lpParam)
{
	//Funccheck
	CEntranceToolDlg* pEntDlg=(CEntranceToolDlg*)lpParam;
	TCHAR sz_ErrCode[512] = {0};
	TCHAR sz_ErrMsg[512] = {0};
	CString str_ErrorCode = _T("");
	CString str_ErrorMsg = _T("");
	//CString str_process = _T("");
	////str_process = pFunccheck->GetProcessName();
	//str_process = pEntDlg->m_Funccheck.GetProcessName();
	//CString str_ProjectName = _T("");
	////str_ProjectName = pFunccheck->GetProjectName();
	//str_ProjectName = pEntDlg->m_Funccheck.GetProjectName();
	//RunTool(TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg, CString str_process, CString str_ProjectName)

	bool b_runRes = false;
	b_runRes = pEntDlg->m_Funccheck.RunTool(sz_ErrCode,sz_ErrMsg, pEntDlg->m_str_Process, pEntDlg->m_str_ModelProject, pEntDlg->m_str_AccessoryList);
	if(!b_runRes)
	{
		str_ErrorCode.Format(_T("%s"),sz_ErrCode);
		str_gl_errorcode = str_ErrorCode;
		str_ErrorMsg.Format(_T("%s"),sz_ErrMsg);
		str_gl_errorMsg = str_ErrorMsg;
		pEntDlg->msgDlg.SetErrorCode(str_ErrorCode);
		pEntDlg->msgDlg.SetError(str_ErrorMsg);
		//pEntDlg->msgDlg.DoModal();
		//AfxMessageBox("工具调用失败");

		b_wait_over = false;
		b_wait_check = true;
	}
	else
	{
		b_wait_over = true;
		b_wait_check = false;
	}

	return 0;
}

void CEntranceToolDlg::OnBnClickedOk()
{
	//m_Funccheck.PrCheck2(m_sz_Errorcode,m_sz_ErrorMsg);

	// TODO: Add your control notification handler code here
	//if(m_str_state.MakeUpper() == _T("NORMAL"))
	//  {
	//       m_b_state = true;
	//  }
	//  else
	//  {
	//	  if(m_str_state.MakeUpper() == _T("CR"))
	//	  {
	//	     m_b_state = false;
	//	  }
	//	  else
	//	  {
	//			msgDlg.SetErrorCode(CString(_T("提示")));
	//			msgDlg.SetError(CString(_T("请选择状态选项Normal 或者 CR")));
	//			msgDlg.DoModal();
	//			return;
	//	  }
	//  }

	TCHAR sz_ErrCode[512];
	TCHAR sz_ErrMsg[512];
	memset(sz_ErrCode,ERRORNUMBER,0);
	memset(sz_ErrMsg,ERRORNUMBER,0);
	bool b_update=true;

	CString str_Record = _T("");
	CString str_path = _T("");
	CString str_Station = _T("");
	CString str_temp = _T("");
	CString str_ComputerName = _T("");

	TCHAR sz_PCname[15] = {0}; 
	DWORD dwSize = sizeof(sz_PCname);
	::GetComputerName(sz_PCname,&dwSize);
	str_ComputerName.Format(_T("%s"),sz_PCname);

	//GetDlgItem(IDOK)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_EmployeeID)->GetWindowText(m_str_ImployeeID);
	m_str_ImployeeID.Trim();
	GetDlgItem(IDC_EDIT_SO)->GetWindowText(m_str_SONumber);
	m_str_SONumber.Trim();

	str_Record = m_str_SONumber;
	str_Record = str_Record + _T("    ") + m_str_ImployeeID;
	str_Record = str_Record + _T("    ") + str_ComputerName;


	m_Clog.WriteLog(str_Record,sz_ErrCode,sz_ErrMsg);

	//Delete adb.exe in process manager system
	if(!m_Funccheck.KillAdbProcess(sz_ErrCode,sz_ErrMsg))
	{
		msgDlg.SetErrorCode(CString(sz_ErrCode));
		msgDlg.SetError(CString(sz_ErrCode));
		msgDlg.DoModal();
		return;
	}


	//TCHAR sz_Path[50];
	//CString str_Flag=_T("");
	//::GetPrivateProfileString(_T("Test_Flag"), _T("Check"), _T(""), sz_Path, sizeof(sz_Path), INIFILEPASH);
	//str_Flag.Format("%s",sz_Path);
	//      if(str_Flag==_T("0"))
	//{

	//Upgrade by self
#ifndef RD_MODE
	b_update=m_Funccheck.Entrance_Tool_Update(m_str_EntranceToolVersion.GetBuffer(),m_str_SONumber.GetBuffer(),sz_ErrCode,sz_ErrMsg);
	m_str_EntranceToolVersion.ReleaseBuffer();
	m_str_SONumber.ReleaseBuffer();
#endif

	//}
	if(!b_update)                
	{
		return;
	}

	if(b_update&&!m_Funccheck.PrCheck1(m_str_SONumber.GetBuffer(),m_str_ImployeeID.GetBuffer(),m_str_Process.GetBuffer(), m_str_Model.GetBuffer(), sz_ErrCode,sz_ErrMsg))
	{
		m_str_SONumber.ReleaseBuffer(); //Temp 20140512
		m_str_ImployeeID.ReleaseBuffer();
		m_str_Process.ReleaseBuffer();
		m_str_Model.ReleaseBuffer();
		CString sz_temp_Errorcode  = _T("");
		CString sz_temp_ErrorMsg  = _T("");
		sz_temp_Errorcode = sz_ErrCode;
		sz_temp_ErrorMsg = sz_ErrMsg;
		msgDlg.SetErrorCode(sz_temp_Errorcode);
		msgDlg.SetError(CString(sz_temp_ErrorMsg));
		msgDlg.DoModal();
		return;
	}
	m_str_SONumber.ReleaseBuffer();
	m_str_ImployeeID.ReleaseBuffer();
	//::MessageBox(NULL,_T("功能1测试完成"),_T("提示"),MB_OK);
	CString str_Imagepath;
	str_Imagepath = _T("");
	//PrCheck2(CString str_ProjectName, CString& str_ImagePath, TCHAR* sz_SO,TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
	bool b_test2 = m_Funccheck.PrCheck2(m_str_ModelProject,m_str_ProjectName,str_Imagepath,m_str_SONumber.GetBuffer(), m_str_EntranceToolVersion,sz_ErrCode,sz_ErrMsg);
	m_str_SONumber.ReleaseBuffer();
	if(b_test2)
	{
		//	//::MessageBox(NULL,_T("功能2测试完成"),_T("提示"),MB_OK);
	}
	else
	{
		CString sz_temp_Errorcode  = _T("");
		CString sz_temp_ErrorMsg  = _T("");
		sz_temp_Errorcode.Format(_T("%s"),sz_ErrCode);
		sz_temp_ErrorMsg.Format(_T("%s"),sz_ErrMsg);
		//msgDlg.SetErrorCode(CString(sz_temp_Errorcode));
		//   msgDlg.SetError(CString(CString(sz_temp_ErrorMsg)));
		//      msgDlg.DoModal();

		MsgDlg obl_dlg;
		obl_dlg.SetErrorCode(CString(sz_temp_Errorcode));
		obl_dlg.SetError(CString(CString(sz_temp_ErrorMsg)));
		obl_dlg.DoModal();
		return;
	}
	//QuerySO_Info_All(sz_so, sz_model, sz_partNo, sz_soType, sz_toolVer, sz_facImageVer, sz_userImageVer,sz_errorCode, sz_errorMsg)
	//Copy Tool from server
	b_test2 = m_Funccheck.PrCheck3(m_str_ProjectName, m_str_SONumber.GetBuffer(), sz_ErrCode, sz_ErrMsg);
	m_str_SONumber.ReleaseBuffer();

	if(b_test2)
	{
		AfxBeginThread(ThreadFunc,this,0,0,0,0);
	}
	else
	{
		CString str_temp_Errorcode  = _T("");
		CString str_temp_ErrorMsg  = _T("");
		str_temp_Errorcode.Format(_T("%s"),sz_ErrCode);
		str_temp_ErrorMsg.Format(_T("%s"),sz_ErrMsg);
		msgDlg.SetErrorCode(str_temp_Errorcode);
		msgDlg.SetError(str_temp_ErrorMsg);
		msgDlg.DoModal();
		//OnCancel();
		return;
	}

	while (1)
	{
		if (true==b_wait_over)
		{   
			TCHAR sz_status[15]="N";
			m_Funccheck.Update_ImployeeIdStatus(m_str_ImployeeID.GetBuffer(), sz_status,sz_ErrCode,sz_ErrMsg);
			m_str_ImployeeID.ReleaseBuffer();
			OnCancel();
			break;
		}
		else
		{
			if(b_wait_check)
			{
				msgDlg.SetErrorCode(str_gl_errorcode);
				msgDlg.SetError(str_gl_errorMsg);
				msgDlg.DoModal();
				b_wait_check=false;
				break;
			}
			Sleep(500);
		}
	}
}

void CEntranceToolDlg::OnBnClickedCancel()
{
	OnCancel();
}


HBRUSH CEntranceToolDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH m_brush = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
    
	switch(nCtlColor)
	{
	  case CTLCOLOR_STATIC:
		  {	 
			  DeleteHBRUSH();
			  m_brush = CreateHBRUSH();
			  pDC->SetBkMode(TRANSPARENT);
		  }
	}

	return m_brush;
}

void CEntranceToolDlg::OnEnChangeEditSo()
{
	b_check_SO = false;
	GetDlgItem(IDC_EDIT_SO)->GetWindowText(m_str_SONumber);
	m_str_SONumber.Trim();

	if(m_str_SONumber.GetLength() == 6)
	{
		b_check_SO = true;
	}
	else
	{
		b_check_SO = false;
	}

	if(b_check_SO && b_check_ImployeeID && (m_str_Process != _T("")))
	{
		GetDlgItem(IDOK)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDOK)->EnableWindow(false);
	}

}

void CEntranceToolDlg::OnEnKillfocusEditSo()
{
	// TODO: Add your control notification handler code here
	//HWND hwnd_SO = (HWND)GetDlgItem(IDC_EDIT_SO);
	if(CWnd::GetFocus()==GetDlgItem(IDCANCEL))
	{ 
		OnCancel(); 
		return;
	} 

	if(b_syc)
	{
		b_check_SO = false;
		b_syc = false;
		GetDlgItem(IDC_EDIT_SO)->GetWindowText(m_str_SONumber);
		m_str_SONumber.Trim();

		if(m_str_SONumber.GetLength() < 6)
		{
			msgDlg.SetErrorCode(CString(_T("ET0036")));
			msgDlg.SetError(CString(_T("Please input SO 6 Numbers\n请输入SO 6个数字:")));
			msgDlg.DoModal();
			((CEdit*)GetDlgItem(IDC_EDIT_SO))->SetFocus();
		}
		else
		{
			b_check_SO = true;
		}

	
		if(b_check_SO && b_check_ImployeeID && (m_str_Process != _T("")))
		{
			GetDlgItem(IDOK)->EnableWindow(true);
		}
		else
		{
			GetDlgItem(IDOK)->EnableWindow(false);
		}

		b_syc = true;
	}
}

void CEntranceToolDlg::OnEnKillfocusEditEmployeeid()
{
	// TODO: Add your control notification handler code here
	if(CWnd::GetFocus()==GetDlgItem(IDCANCEL))
	{ 
		OnCancel(); 
	} 

	if(b_syc)
	{
		b_syc = false;
		b_check_ImployeeID = false;

		GetDlgItem(IDC_EDIT_EmployeeID)->GetWindowText(m_str_ImployeeID);
		m_str_ImployeeID.Trim();

		if(m_str_ImployeeID.GetLength() < 7)
		{
			msgDlg.SetErrorCode(CString(_T("ET0037")));
			msgDlg.SetError(CString(_T("Please input ID Numbers:7 or 8\n请输入ID 7或者8个数字:")));
			msgDlg.DoModal();
			((CEdit*)GetDlgItem(IDC_EDIT_EmployeeID))->SetFocus();
		}
		else
		{
			b_check_ImployeeID = true;
		}

		if(b_check_SO && b_check_ImployeeID && (m_str_Process != _T("")) && m_cl_Accessory.GetItemCount() == m_nAccessoryCount)
		{
			GetDlgItem(IDOK)->EnableWindow(true);
			//this->UpdateWindow();
		}
		else
		{
			GetDlgItem(IDOK)->EnableWindow(false);
		}
		b_syc = true;
	}

}

void CEntranceToolDlg::OnEnSetfocusEditSo()
{
	// TODO: Add your control notification handler code here

	GetDlgItem(IDC_EDIT_SO)->GetWindowText(m_str_SONumber);
	m_str_SONumber.Trim();

	if(m_str_SONumber == _T("请输入7位数字工单"))
	{
		GetDlgItem(IDC_EDIT_SO)->SetWindowText(_T(""));  
	}
}

void CEntranceToolDlg::OnEnSetfocusEditEmployeeid()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_EmployeeID)->GetWindowText(m_str_ImployeeID);
	m_str_ImployeeID.Trim();

	if(m_str_ImployeeID == _T("请输入7或者8位员工ID"))
	{
		GetDlgItem(IDC_EDIT_EmployeeID)->SetWindowText(_T(""));
	}

}

void CEntranceToolDlg::OnEnChangeEditEmployeeid()
{
	b_check_ImployeeID = false;
	GetDlgItem(IDC_EDIT_EmployeeID)->GetWindowText(m_str_ImployeeID);
	m_str_ImployeeID.Trim();
    
	CString str_filename;
	char sz_Path[20];
	::GetPrivateProfileString(_T("PassWord"), _T("display"), _T(""), sz_Path, sizeof(sz_Path), INIFILEPASH);
	str_filename.Format("%s",sz_Path);
    if(str_filename==_T("0"))
	{
		CEdit* a =(CEdit*)GetDlgItem(IDC_EDIT_EmployeeID);	
		a->SetPasswordChar(_T('*'));
	}


	if(m_str_ImployeeID.GetLength() == 7||m_str_ImployeeID.GetLength() == 8)
	{
		b_check_ImployeeID = true;
	}
	else
	{
		b_check_ImployeeID = false;
	}

	if(b_check_SO && b_check_ImployeeID && (m_str_Process != _T("")) && m_cl_Accessory.GetItemCount() == m_nAccessoryCount)
	{
		GetDlgItem(IDOK)->EnableWindow(true);
		GetDlgItem(IDOK)->SetFocus();
	}
	else
	{
		GetDlgItem(IDOK)->EnableWindow(false);
	}
}



void CEntranceToolDlg::OnCbnSelchangeComboProject()
{
	// TODO: Add your control notification handler code here

	int nSel = m_cb_Project.GetCurSel();

	if(nSel == 0)
		return;

	//GWOLB1A_Wolf
	//Luna_LG7_GLUNB1A
	CString strProjectModel;
	
	m_cb_Project.GetLBText(nSel, strProjectModel);
	m_str_ModelProject = strProjectModel;

	int nTotal = strProjectModel.GetLength();

	int nIndex = strProjectModel.ReverseFind(_T('_'));
	
	CString strModel = strProjectModel.Mid(0,nIndex);
	m_str_Model = strModel;
	m_str_ProjectName = strProjectModel.Mid(nIndex + 1);

	TCHAR szProcessList[1024] ={0};
	TCHAR szErrorCode[512] = {0};
	TCHAR szErrorMsg[512] = {0};

	bool bRes = false;
	if(m_Funccheck.FconnectDB())
	{
	  bRes =m_Funccheck.Query_PorcessList(strModel.GetBuffer(), szProcessList,szErrorCode, szErrorMsg);
	}
	else
	{
		CString str_Errorcode = _T("");
		CString str_ErrorMsg = _T("");
		str_Errorcode.Format(_T("ET0057"));
		str_ErrorMsg.Format(_T("Connect DB fail\n数据库连接失败"));
		msgDlg.SetErrorCode(str_Errorcode);
		msgDlg.SetError(str_ErrorMsg);
		msgDlg.DoModal();
		return;
	}
	if(bRes)
	{
		CString strTemp = szProcessList;

		int nInsertIndex = 0;

		m_cb_Process.ResetContent();

		m_cb_Process.InsertString(nInsertIndex++, _T("请选择站别"));
        m_cb_Process.InsertString(nInsertIndex++, "RE_DL");
		while(strTemp.GetLength() > 0)
		{
			int nIndex = strTemp.Find(_T(','));
			CString strItem = strTemp.Left(nIndex);
			strTemp = strTemp.Mid(nIndex+1);

			m_cb_Process.InsertString(nInsertIndex++, strItem);
		}
		m_cb_Process.SetCurSel(0);
		
	}
	else
	{
		msgDlg.SetErrorCode((CString)szErrorCode);
		msgDlg.SetError((CString)szErrorMsg);
		msgDlg.DoModal();
		return;
	}

}

void CEntranceToolDlg::OnCbnSelchangeComboProcess()
{
	// TODO: Add your control notification handler code here
	m_str_Process = _T("");
	int nCurSel = m_cb_Process.GetCurSel();

	if(nCurSel == 0)
		return;
	m_cb_Process.GetLBText(nCurSel, m_str_Process);

	TCHAR szConsumeType[1024] ={0};
	TCHAR szErrorCode[512] = {0};
	TCHAR szErrorMsg[512] = {0};

	bool bRes = false;
	m_cl_Accessory.DeleteAllItems();
	if(m_Funccheck.FconnectDB())
	{
		bRes =m_Funccheck.Query_ConsumeType(m_str_Model.GetBuffer(), m_str_Process.GetBuffer(),szConsumeType,szErrorCode, szErrorMsg);
		m_str_Model.ReleaseBuffer();
		m_str_Process.ReleaseBuffer();
	}
	else
	{
		CString str_Errorcode = _T("");
		CString str_ErrorMsg = _T("");
		str_Errorcode.Format(_T("ET0057"));
		str_ErrorMsg.Format(_T("Connect DB fail\n数据库连接失败"));
		msgDlg.SetErrorCode(str_Errorcode);
		msgDlg.SetError(str_ErrorMsg);
		msgDlg.DoModal();
		return;
	}
	if(bRes)
	{
		CString strTemp = szConsumeType;
		m_nAccessoryCount = 0;

		int nInsertIndex = 0;

		m_cb_Accessory.ResetContent();
		if(strTemp.GetLength() == 0) //no accessory
		{
			m_cb_Accessory.EnableWindow(false);
			m_cl_Accessory.EnableWindow(false);
			GetDlgItem(IDC_EDIT_ACCESSORY)->EnableWindow(false);
		}
		else
		{
			m_cb_Accessory.EnableWindow(true);
			m_cl_Accessory.EnableWindow(true);
			GetDlgItem(IDC_EDIT_ACCESSORY)->EnableWindow(true);
			m_cb_Accessory.InsertString(nInsertIndex++, _T("请扫入耗件ID"));
			while(strTemp.GetLength() > 0)
			{
				int nIndex = strTemp.Find(_T(','));
	//			CString strItem = strTemp.Left(strTemp.Find(_T(":")));
				CString strItem = strTemp.Left(nIndex);
				strTemp = strTemp.Mid(nIndex+1);

				m_cb_Accessory.InsertString(nInsertIndex++, strItem);
				m_nAccessoryCount++;
			}
			m_cb_Accessory.SetCurSel(0);
		}
		
	}
	else
	{
		msgDlg.SetErrorCode((CString)szErrorCode);
		msgDlg.SetError((CString)szErrorMsg);
		msgDlg.DoModal();
		return;
	}
	if(b_check_SO && b_check_ImployeeID && (m_str_Process != _T("")) && m_cl_Accessory.GetItemCount() == m_nAccessoryCount)
	{
		GetDlgItem(IDOK)->EnableWindow(true);
		GetDlgItem(IDOK)->SetFocus();
	}
	else
	{
		CString strTemp;
		GetDlgItem(IDC_EDIT_SO)->GetWindowText(strTemp);
		if(strTemp == _T("请输入7位数字工单"))
		{
			GetDlgItem(IDC_EDIT_SO)->SetWindowText(_T(""));
			GetDlgItem(IDC_EDIT_SO)->SetFocus();
		}
		else
			GetDlgItem(IDC_EDIT_SO)->SetFocus();
		GetDlgItem(IDOK)->EnableWindow(false);
	}

}

void CEntranceToolDlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
	m_str_state = _T("");
	int nCurSel = m_combox_State.GetCurSel();

	if(nCurSel == 0)
	{
		CString str_Errorcode = _T("");
		CString str_ErrorMsg = _T("");
		str_Errorcode.Format(_T("ERROR"));
		str_ErrorMsg.Format(_T("请选择CR或Normal"));
		msgDlg.SetErrorCode(str_Errorcode);
		msgDlg.SetError(str_ErrorMsg);
		msgDlg.DoModal();
		return;
	}

	m_combox_State.GetLBText(nCurSel, m_str_state);

	GetDlgItem(IDC_EDIT_SO)->SetFocus();
}

BOOL CEntranceToolDlg::GetFileVersion(LPCTSTR szFN, LPTSTR szVersion, DWORD nSize)
{
	DWORD dwCount = 0; 
	DWORD dwHandle = 0;
	DWORD dwValueLen = 0;
	BOOL bRet;
	TCHAR *pcValue = NULL;
	TCHAR *pc = NULL;
	TCHAR *pBuffer = NULL;
	TCHAR szQuery[100]={0};

	if (nSize == 0)
		return FALSE;

	if ((dwCount = GetFileVersionInfoSize(szFN, &dwHandle)) != 0)
	{
		pBuffer = new TCHAR[dwCount];
		if (!pBuffer)
			return FALSE;

		if (GetFileVersionInfo(szFN, dwHandle, dwCount, pBuffer) != 0)
		{
			BOOL bVer = VerQueryValue(pBuffer, "\\VarFileInfo\\Translation", //INTERNAL USE ONLY
				(void **) &pcValue, (UINT *) &dwValueLen);

			if (bVer && dwValueLen != 0)
			{   
				wsprintf(szQuery, "\\StringFileInfo\\%04X%04X\\FileVersion", //INTERNAL USE ONLY
					*(WORD *)pcValue, *(WORD *)(pcValue+2));    // Localization OK

				bRet = VerQueryValue(pBuffer, szQuery, (void **) &pcValue, 
					(UINT *) &dwValueLen);

				if (bRet)
				{
					while ((pc = _tcschr(pcValue, '(')) != NULL)
						*pc = '{';
					while ((pc = _tcschr(pcValue, ')')) != NULL)
						*pc = '}';

					_tcsncpy(szVersion, pcValue, nSize);
					szVersion[nSize - 1] = '\0';

					delete [] pBuffer;
					return TRUE;
				}
			}
		}

		delete [] pBuffer;
	}

	return FALSE;
}
BOOL CEntranceToolDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			if(pMsg->hwnd == GetDlgItem(IDC_EDIT_ACCESSORY)->m_hWnd)
			{
				CString strTemp;
				CString strName;
				GetDlgItem(IDC_EDIT_ACCESSORY)->GetWindowText(strTemp);
				int nCulSel = m_cb_Accessory.GetCurSel();
				m_cb_Accessory.GetLBText(nCulSel, strName);

				// check times < maxtimes
				int nIndex = strName.Find(_T(":"));
				CString strMaxtimes = strName.Mid(nIndex+1);
				strName = strName.Left(nIndex);
				TCHAR szConsumeInfo[1024] ={0};
				TCHAR szErrorCode[512] ={0};
				TCHAR szErrorMsg[1024] ={0};

				bool bRes = false;
				if(m_Funccheck.FconnectDB())
				{
					bRes =m_Funccheck.Query_ConsumeInfo(strTemp.GetBuffer(), strName.GetBuffer(),szConsumeInfo,szErrorCode, szErrorMsg);
					strTemp.ReleaseBuffer();
					strName.ReleaseBuffer();
				}
				else
				{
					CString str_Errorcode = _T("");
					CString str_ErrorMsg = _T("");
					str_Errorcode.Format(_T("ET0057"));
					str_ErrorMsg.Format(_T("Connect DB fail\n数据库连接失败"));
					msgDlg.SetErrorCode(str_Errorcode);
					msgDlg.SetError(str_ErrorMsg);
					msgDlg.DoModal();
					GetDlgItem(IDC_EDIT_ACCESSORY)->SetWindowText(_T(""));
					GetDlgItem(IDC_EDIT_ACCESSORY)->SetFocus();
					return true;
				}

				if(bRes)
				{
					int nMax = atoi((LPCTSTR)strMaxtimes);
					int nReal = atoi(szConsumeInfo);
					if(nReal >= nMax)
					{
						CString str_Errorcode = _T("ET1057");
						CString str_ErrorMsg = _T("Accessory ") + strName + _T("使用次数达到上限") + strMaxtimes;
						msgDlg.SetErrorCode(str_Errorcode);
						msgDlg.SetError(str_ErrorMsg);
						msgDlg.DoModal();
						GetDlgItem(IDC_EDIT_ACCESSORY)->SetWindowText(_T(""));
						GetDlgItem(IDC_EDIT_ACCESSORY)->SetFocus();
						return true;

					}
				}
				else
				{
					msgDlg.SetErrorCode((CString)szErrorCode);
					msgDlg.SetError((CString)szErrorMsg);
					msgDlg.DoModal();
					GetDlgItem(IDC_EDIT_ACCESSORY)->SetWindowText(_T(""));
					GetDlgItem(IDC_EDIT_ACCESSORY)->SetFocus();
					return true;
				}

				int nCount = m_cl_Accessory.GetItemCount();
				int i = 0;

				for(i = 0; i < nCount; i++)
				{
					CString strExistItem = m_cl_Accessory.GetItemText(i, 0);
					if(strExistItem == strName)
					{
						m_cl_Accessory.SetItemText(nCount, 1, strTemp.GetBuffer());
						strTemp.GetBuffer();
						if(nCulSel < this->m_nAccessoryCount)
						{
							m_cb_Accessory.SetCurSel(nCulSel + 1);
							GetDlgItem(IDC_EDIT_ACCESSORY)->SetFocus();
						}					
						break;
					}
				}
				if(i == nCount)
				{
					m_cl_Accessory.InsertItem(nCount, strName.GetBuffer());
					strName.ReleaseBuffer();
					m_cl_Accessory.SetItemText(nCount, 1, strTemp.GetBuffer());
					strTemp.GetBuffer();
					if((nCount+1) < this->m_nAccessoryCount) //Other Accessory need input id
					{
						if(nCulSel < this->m_nAccessoryCount)
						{
							m_cb_Accessory.SetCurSel(nCulSel + 1);
							GetDlgItem(IDC_EDIT_ACCESSORY)->SetFocus();
						}
						else
						{
							m_cb_Accessory.SetCurSel(nCulSel - 1);
							GetDlgItem(IDC_EDIT_ACCESSORY)->SetFocus();
						}
					}
				}
				nCount = m_cl_Accessory.GetItemCount();
				this->m_str_AccessoryList = _T("");
				for(i = 0; i< nCount; i++)
				{
					CString strExistItem = m_cl_Accessory.GetItemText(i, 0);
					CString strExistID = m_cl_Accessory.GetItemText(i,1);
					this->m_str_AccessoryList += strExistItem + _T(":") + strExistID + _T(",");
				}

				GetDlgItem(IDC_EDIT_ACCESSORY)->SetWindowText(_T(""));
				
				
				if(b_check_SO && b_check_ImployeeID && (m_str_Process != _T("")) && nCount == m_nAccessoryCount)
				{
					GetDlgItem(IDOK)->EnableWindow(true);
					GetDlgItem(IDOK)->SetFocus();
					return true;
				}
				else
				{
					GetDlgItem(IDOK)->EnableWindow(false);
				}
			}
			else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_SO)->m_hWnd)
			{
				CString strTemp;
				GetDlgItem(IDC_EDIT_EmployeeID)->GetWindowText(strTemp);
				if(strTemp == _T("请输入7或者8位员工ID"))
				{
					GetDlgItem(IDC_EDIT_EmployeeID)->SetWindowText(_T(""));
					GetDlgItem(IDC_EDIT_EmployeeID)->SetFocus();
				}
				else
					GetDlgItem(IDC_EDIT_EmployeeID)->SetFocus();				
			}
			else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_EmployeeID)->m_hWnd)
			{
				m_cb_Accessory.SetCurSel(1);
				GetDlgItem(IDC_EDIT_ACCESSORY)->SetWindowText(_T(""));
				GetDlgItem(IDC_EDIT_ACCESSORY)->SetFocus();
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
void CEntranceToolDlg::OnCbnSelchangeComboAccessory()
{
	// TODO: Add your control notification handler code here

	int nCulSel = m_cb_Accessory.GetCurSel();
	if(nCulSel != 0)
	{
		GetDlgItem(IDC_EDIT_ACCESSORY)->SetFocus();
		GetDlgItem(IDC_EDIT_ACCESSORY)->SetWindowText(_T(""));
	}
}
