// OutPut_Manage.cpp : implementation file
//

#include "stdafx.h"
#include "warehouseManage.h"
#include "OutPut_Manage.h"


// OutPut_Manage dialog

IMPLEMENT_DYNAMIC(OutPut_Manage, CDialog)

OutPut_Manage::OutPut_Manage(CWnd* pParent /*=NULL*/)
	: CDialog(OutPut_Manage::IDD, pParent)
{
	m_check_CTNID = true;
	nIMEInum = 0;
	m_b_lock = false;
	m_nIDLength = 15;
}

OutPut_Manage::~OutPut_Manage()
{
}

BOOL OutPut_Manage::OnInitDialog()
{
	CDialog::OnInitDialog();

	//m_Edit_SO_OutPut.SetLimitText(6);
	m_hwndBrush.CreateSolidBrush(RGB(215,240,198));//生成一绿色刷子

	int i_colsWidth[4] = {50, 100, 110,380};
	m_listctrl_OutPut.InsertColumn(0, _T("Number"), LVCFMT_LEFT, i_colsWidth[0]);
	m_listctrl_OutPut.InsertColumn(1, _T("Carton ID"), LVCFMT_LEFT, i_colsWidth[1]);
	m_listctrl_OutPut.InsertColumn(2, _T("IMEI"), LVCFMT_LEFT, i_colsWidth[2]);
	m_listctrl_OutPut.InsertColumn(3, _T("Result"), LVCFMT_LEFT, i_colsWidth[3]);

	m_listctrl_OutPut.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	//m_listctrl_OutPut.SetColumnWidth(0,100);
	//m_listctrl_OutPut.SetColumnWidth(1,110);

	//int nRow = m_listctrl_Input.InsertItem(0, _T("11"));//插入行
	//nRow = m_listctrl_Input.InsertItem(1, _T("12"));//插入行
	//m_listctrl_Input.SetItemText(nRow, 1, _T("jacky"));//设置数据
	//m_listctrl_Input.SetItemText(nRow, 2, _T("jacky1"));//设置数据

	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	m_n_Fail = 0;
	m_n_Pass = 0;

	return TRUE; 
}


void OutPut_Manage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listctrl_OutPut);
	DDX_Control(pDX, IDC_EDIT1, m_Edit_SO_OutPut);
	DDX_Control(pDX, IDC_Fail_show, m_Fail_show);
	DDX_Control(pDX, IDC_Pass_Show, m_Pass_show);
	DDX_Control(pDX, IDC_Total_show, m_Total_show);
	DDX_Control(pDX, IDC_EDIT2, m_Edit_ReworkReason);
	DDX_Control(pDX, IDC_EDIT4, m_Edit_DOA_RMA_NO);
	DDX_Control(pDX, IDC_CHECK1, m_Button_Lock);
	DDX_Control(pDX, IDC_CHECK3, m_Button_DOA_RMA_Option);
}


BEGIN_MESSAGE_MAP(OutPut_Manage, CDialog)
	ON_BN_CLICKED(IDOK, &OutPut_Manage::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &OutPut_Manage::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &OutPut_Manage::OnBnClickedRadio2)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK1, &OutPut_Manage::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &OutPut_Manage::OnBnClickedCheck2)
	ON_EN_CHANGE(IDC_EDIT4, &OutPut_Manage::OnEnChangeEdit4)
END_MESSAGE_MAP()


// OutPut_Manage message handlers

bool OutPut_Manage::FcheckIMEI(CString str_IMEI, int nIMEI_current_number)
{
	bool b_res = true;
	int nSize = m_szIMEIArray.GetSize();
	for(int i = 0;i< nSize;i++)
	{
		CString strTemp = m_szIMEIArray.GetAt(i);
		if(str_IMEI.MakeUpper() == strTemp.MakeUpper())
		{
			return false;
		}
	}

//	m_szIMEIArray.Add(str_IMEI);
	return b_res;
}

void OutPut_Manage::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();

	CString str_SO = _T("");
	CString str_CartonID = _T("");
	CString str_Status = _T("");
	CString str_ProcessList = _T("");
	CString str_Process = _T("");
	bool b_res = false;
	char sz_errcode[256];
	char sz_errMsg[256];
	char sz_IMEI[50];
	char sz_status[50];
	char sz_module[50];
	char sz_Picasso[50];
	char sz_partno[50];
	char sz_SO[50];
	char sz_sw[50];
	char sz_ProcessList[256];
	char sz_IMEIList[1024];

	char sz_model_SO[50];
	char sz_partNo_SO[50];
	char sz_soType_SO[50];
	char sz_toolVer_SO[50];
	char sz_facImageVer_SO[50];
	char sz_userImageVer_SO[50];

	memset(sz_model_SO,0,sizeof(sz_model_SO));
	memset(sz_partNo_SO,0,sizeof(sz_partNo_SO));
	memset(sz_soType_SO,0,sizeof(sz_soType_SO));
	memset(sz_toolVer_SO,0,sizeof(sz_toolVer_SO));
	memset(sz_facImageVer_SO,0,sizeof(sz_facImageVer_SO));
	memset(sz_userImageVer_SO,0,sizeof(sz_userImageVer_SO));

	memset(sz_IMEIList,0,sizeof(sz_IMEIList));

	memset(sz_errcode,0,sizeof(sz_errcode));
	memset(sz_errMsg,0,sizeof(sz_errMsg));
	memset(sz_IMEI,0,sizeof(sz_IMEI));
	memset(sz_status,0,sizeof(sz_status));
	memset(sz_module,0,sizeof(sz_module));
	memset(sz_Picasso,0,sizeof(sz_Picasso));
	memset(sz_partno,0,sizeof(sz_partno));
	memset(sz_SO,0,sizeof(sz_SO));
	memset(sz_sw,0,sizeof(sz_sw));
	memset(sz_ProcessList,0,sizeof(sz_ProcessList));


	//int nIMEInum = 0;
	char sz_count[5];
	memset(sz_count,sizeof(sz_count),0);



	b_res = DB_Output.FconnectDB();
	if(!b_res)
	{
		msgDlg.SetErrorCode(CString(_T("WM0003")));
		msgDlg.SetError(CString(_T("连接数据库失败!\nConnect DB Fail!")));
		msgDlg.DoModal();
		return;
	}

	m_Edit_SO_OutPut.GetWindowText(str_CartonID);
	

	if(!b_res)
	{
		CString str_errcode = _T("");
		CString str_errMsg = _T("");
		str_errcode.Format(_T("%s"),sz_errcode);
		str_errMsg.Format(_T("%s"),sz_errMsg);
		msgDlg.SetErrorCode(str_errcode);
		msgDlg.SetError(str_errMsg);
		msgDlg.DoModal();
		return;
	}
	CString str_IMEIList = _T("");
	if(m_check_CTNID)
	{
		/*Remove Carton ID length check from 20140402 Magan*/
		//if(str_CartonID.Trim().GetLength() != 13&&str_CartonID.Trim().GetLength() != 10)
		//{
		//	msgDlg.SetErrorCode(CString(_T("WM0100")));
		//	msgDlg.SetError(CString(_T("Carton ID 位数不为13或者10!")));
		//	msgDlg.DoModal();
		//	DB_Output.FcloseDB();
		//	return;
		//}
        b_res = DB_Output.pQueryIMEI_ByCarton(str_CartonID.GetBuffer(), sz_count, sz_IMEIList, sz_errcode, sz_errMsg);
		str_CartonID.ReleaseBuffer();
		if(!b_res)
		{
			msgDlg.SetErrorCode((CString)sz_errcode);
			msgDlg.SetError((CString)sz_errMsg);
			msgDlg.DoModal();
			DB_Output.FcloseDB();
			return;
		}
		str_IMEIList.Format(_T("%s"),sz_IMEIList);
		if(str_IMEIList == _T(""))
		{
			msgDlg.SetErrorCode((CString)_T("WM0999"));
			msgDlg.SetError((CString)_T("该Carton 对应的ID 为空!"));
			msgDlg.DoModal();
			DB_Output.FcloseDB();
			return;
		}
	}
	else
	{
		//if(str_CartonID.Trim().GetLength() != 15)
		//{
		//	msgDlg.SetErrorCode(CString(_T("WM0100")));
		//	msgDlg.SetError(CString(_T("IMEI 位数不为15!")));
		//	msgDlg.DoModal();
		//	DB_Output.FcloseDB();
		//	return;
		//}
		str_IMEIList = str_CartonID + _T(",");
		char sz_cartonID[50];
		memset(sz_cartonID,0,sizeof(sz_cartonID));
		//QueryIMEIStatus(char * sz_InputID,char * sz_status,char * sz_cartonID, char * sz_ErrCode,char * sz_ErrMsg)
		b_res = DB_Output.pQueryIMEIStatus(str_CartonID.GetBuffer(),sz_status,sz_cartonID,sz_errcode,sz_errMsg);
		str_CartonID.ReleaseBuffer();
		if(!b_res)
		{
			msgDlg.SetErrorCode((CString)sz_errcode);
			msgDlg.SetError((CString)sz_errMsg);
			msgDlg.DoModal();
			DB_Output.FcloseDB();
			return;
		}
		str_CartonID.Format(_T("%s"),sz_cartonID);
	}

	while(str_IMEIList.GetLength() > 0)
	{

		int nIndex = str_IMEIList.Find(_T(','));
		CString strItem = str_IMEIList.Left(nIndex);
		str_IMEIList = str_IMEIList.Mid(nIndex+1);

		b_res = FcheckIMEI(strItem, 0);
		if(!b_res)
		{
			msgDlg.SetErrorCode(CString(_T("WM0101")));
			msgDlg.SetError(CString(_T("该ID号") + strItem + _T("重复!")));
			msgDlg.DoModal();
			DB_Output.FcloseDB();
			return;
		}

		b_res = DB_Output.pQueryOOBstatus(strItem.GetBuffer(), sz_status, sz_errcode, sz_errMsg);
		strItem.ReleaseBuffer();
		str_Status.Format(_T("%s"),sz_status);
		if(!b_res)
		{
			msgDlg.SetErrorCode((CString)sz_errcode);
			msgDlg.SetError((CString)sz_errMsg);
			msgDlg.DoModal();
			DB_Output.pCloseDB();
			return;
		}

		if(str_Status == _T("1")) //Need OOB record
		{
			b_res = DB_Output.pUpdateOOBStatusToFalse(strItem.GetBuffer(),sz_errcode,sz_errMsg);
			strItem.ReleaseBuffer();
			if(!b_res)
			{
				CString str_number = _T("");
			    str_number.Format("%d",(nIMEInum + 1));
				int nRow = m_listctrl_OutPut.InsertItem(nIMEInum,str_number);//插入行
				m_listctrl_OutPut.SetItemText(nRow, 1, str_CartonID);//设置数据
				m_listctrl_OutPut.SetItemText(nRow, 1, strItem);//设置数据
				m_listctrl_OutPut.SetItemText(nRow, 2, _T("更新OOB状态失败!\nUpdate OOB status fail"));//设置数据
				m_listctrl_OutPut.SetItemColor(nRow,RGB(255,0,0));
				nIMEInum++;
				m_n_Fail++;
				this->	m_szIMEIArray.Add(strItem);
				continue;
			}
		}
		{
			//Start Add by Lennon 2013.03.28 Insert WareHouse Record TableName = IMEICheckInfo
			char sz_Status[32]={0};
			//char sz_ReworkReason[512]={0};
			//char sz_DOARMANo[512]={0};

			b_res = DB_Output.pGetIMEI_Info(strItem.GetBuffer(),sz_module,sz_Picasso,sz_partno,sz_SO,sz_sw,sz_errcode,sz_errMsg);
			strItem.ReleaseBuffer();
			if(!b_res)
			{
				msgDlg.SetErrorCode((CString)sz_errcode);
				msgDlg.SetError((CString)sz_errMsg);
				msgDlg.DoModal();
				DB_Output.pCloseDB();
				return;
			}

			b_res = DB_Output.pQuery_WareHouseStatus(sz_Picasso,strItem.GetBuffer(),sz_Status, sz_errcode,sz_errMsg);
			strItem.ReleaseBuffer( );
			if(!b_res)
			{
				msgDlg.SetErrorCode((CString)sz_errcode);
				msgDlg.SetError((CString)sz_errMsg);
				msgDlg.DoModal();
				DB_Output.pCloseDB();
				return;
			}
			CString str_WareHouseStatus = sz_Status;
			if (_T("L") == str_WareHouseStatus )
			{
				CString strReworkReason=_T("");
				CString strDOARMANo=_T("");
				m_Edit_ReworkReason.GetWindowText(strReworkReason);
				m_Edit_DOA_RMA_NO.GetWindowText(strDOARMANo);

				b_res = DB_Output.pUpdate_WareHouseOut(sz_Picasso,strItem.GetBuffer(),strReworkReason.GetBuffer(),strDOARMANo.GetBuffer(), sz_errcode,sz_errMsg);
				strItem.ReleaseBuffer( );
				strReworkReason.ReleaseBuffer( );
				strDOARMANo.ReleaseBuffer( );
				if(!b_res)
				{
					msgDlg.SetErrorCode((CString)sz_errcode);
					msgDlg.SetError((CString)sz_errMsg);
					msgDlg.DoModal();
					DB_Output.pCloseDB();
					return;
				}
			}

			if(m_b_lock)
			{
				b_res = DB_Output.pInsertWareHouseOutHoldInfo(sz_Picasso,strItem.GetBuffer(),sz_module,sz_partno,sz_errcode,sz_errMsg);
				strItem.ReleaseBuffer( );
				if(!b_res)
				{
					msgDlg.SetErrorCode((CString)sz_errcode);
					msgDlg.SetError((CString)sz_errMsg);
					msgDlg.DoModal();
					DB_Output.pCloseDB();
					return;
				}
			}
			//End   Add by Lennon 2013.03.38
		}
		//b_res = DB_Output.pInsertPTOOBRework(strItem.GetBuffer(),sz_errcode,sz_errMsg);
		//strItem.ReleaseBuffer();
		this->	m_szIMEIArray.Add(strItem);

		if(!b_res)
		{
			CString str_number = _T("");
			str_number.Format("%d",(nIMEInum + 1));
			int nRow = m_listctrl_OutPut.InsertItem(nIMEInum,str_number);//插入行
			m_listctrl_OutPut.SetItemText(nRow, 1, str_CartonID);//设置数据
			m_listctrl_OutPut.SetItemText(nRow, 2, strItem);//设置数据
			m_listctrl_OutPut.SetItemText(nRow, 3, _T("插入Rework记录失败!\nInsert Rework record fail"));//设置数据
			m_listctrl_OutPut.SetItemColor(nRow,RGB(255,0,0));
			nIMEInum++;
			m_n_Fail++;
			continue;
		}

		CString str_number = _T("");
		str_number.Format("%d",(nIMEInum + 1));
		int nRow = m_listctrl_OutPut.InsertItem(nIMEInum,str_number);//插入行
		m_listctrl_OutPut.SetItemText(nRow, 1, str_CartonID);//设置数据
		m_listctrl_OutPut.SetItemText(nRow, 2, strItem);//设置数据
		m_listctrl_OutPut.SetItemText(nRow, 3, "PASS");//设置数据
		m_listctrl_OutPut.SetTextBkColor(RGB(0,255,0));
		m_n_Pass++;
		nIMEInum++;
  }



	b_res = DB_Output.FcloseDB();
	if(!b_res)
	{
		msgDlg.SetErrorCode(CString(_T("WM0003")));
		msgDlg.SetError(CString(_T("关闭数据库失败!\nClose DB Fail!")));
		msgDlg.DoModal();
		return;
	}
	m_Edit_SO_OutPut.SetWindowTextA(_T(""));
	CString str_total = _T("");
	CString str_Fail = _T("");
	CString str_Pass = _T("");

	str_total.Format("%d",m_szIMEIArray.GetSize());
	str_Fail.Format("%d",m_n_Fail);
	str_Pass.Format("%d",m_n_Pass);
	m_Pass_show.SetWindowTextA(str_Pass);
	m_Fail_show.SetWindowTextA(str_Fail);
	m_Total_show.SetWindowTextA(str_total);
}

void OutPut_Manage::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
	m_check_CTNID = true;
}

void OutPut_Manage::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	m_check_CTNID = false;
}

HBRUSH OutPut_Manage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	pDC->SetBkMode(TRANSPARENT);
	return m_hwndBrush;
}

void OutPut_Manage::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here

	CString strReworkReason=_T("");
	CString strDOARMANo=_T("");
	m_Edit_ReworkReason.GetWindowText(strReworkReason);
	m_Edit_DOA_RMA_NO.GetWindowText(strDOARMANo);
	if (_T("") == strReworkReason){
		m_Button_Lock.SetCheck(false);
		msgDlg.SetErrorCode(CString(_T("WM0005")));
		msgDlg.SetError(CString(_T("ReworkReason 不能为空\n")));
		msgDlg.DoModal();

		m_Edit_DOA_RMA_NO.SetReadOnly(false);
		m_Edit_ReworkReason.SetReadOnly(false);
		m_Button_DOA_RMA_Option.EnableWindow(true);
		m_Edit_ReworkReason.SetFocus();
		return;
	}

	if (_T("") == strDOARMANo && m_Button_DOA_RMA_Option.GetCheck()){
		m_Button_Lock.SetCheck(false);
		msgDlg.SetErrorCode(CString(_T("WM0006")));
		msgDlg.SetError(CString(_T("DOA RMA No 不能为空\n")));
		msgDlg.DoModal();
		return;
	}

	if(m_Button_Lock.GetCheck()){
		m_Edit_DOA_RMA_NO.SetReadOnly(true);
		m_Edit_ReworkReason.SetReadOnly(true);
		m_Button_DOA_RMA_Option.EnableWindow(false);
		m_b_lock = true;
	}
	else{
		m_Edit_DOA_RMA_NO.SetReadOnly(false);
		m_Edit_ReworkReason.SetReadOnly(false);
		m_Button_DOA_RMA_Option.EnableWindow(true);
		m_b_lock = false;
		m_Edit_DOA_RMA_NO.SetWindowText(_T(""));
		m_Edit_ReworkReason.SetWindowText(_T(""));
		m_Button_DOA_RMA_Option.SetCheck(0);
	}
}

void OutPut_Manage::OnBnClickedCheck2()
{
	// TODO: Add your control notification handler code here
	if(m_Button_DOA_RMA_Option.GetCheck()){
		m_Edit_DOA_RMA_NO.EnableWindow(true);
		m_Edit_DOA_RMA_NO.SetFocus();
	}
	else{
		m_Edit_DOA_RMA_NO.EnableWindow(false);
		m_Edit_DOA_RMA_NO.SetWindowText(_T(""));
	}
}

void OutPut_Manage::OnEnChangeEdit4()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
