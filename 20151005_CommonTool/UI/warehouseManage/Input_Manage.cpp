// Input_Manage.cpp : implementation file
//

#include "stdafx.h"
#include "warehouseManage.h"
#include "Input_Manage.h"


// Input_Manage dialog

IMPLEMENT_DYNAMIC(Input_Manage, CDialog)

Input_Manage::Input_Manage(CWnd* pParent /*=NULL*/)
	: CDialog(Input_Manage::IDD, pParent)
{
	m_check_CTNID = true;
	nIMEInum = 0;
	n_pass = 0;
	n_fail = 0;
	n_total = 0;
	m_b_lock = false;
	m_nIDLength = 0;
	m_sz_IMEIArray.RemoveAll();
}

Input_Manage::~Input_Manage()
{
}


BOOL Input_Manage::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_EditSO.SetLimitText(7);
	
	m_hwndBrush.CreateSolidBrush(RGB(215,240,198));//生成一绿色刷子

	int i_colsWidth[4] = {50, 100, 110,380};
	m_listctrl_Input.InsertColumn(0, _T("Number"), LVCFMT_LEFT, i_colsWidth[0]);
	m_listctrl_Input.InsertColumn(1, _T("Carton ID"), LVCFMT_LEFT, i_colsWidth[1]);
	m_listctrl_Input.InsertColumn(2, _T("IMEI"), LVCFMT_LEFT, i_colsWidth[2]);
	m_listctrl_Input.InsertColumn(3, _T("Result"), LVCFMT_LEFT, i_colsWidth[3]);

	m_listctrl_Input.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	((CButton *)GetDlgItem(IDC_CartonID))->SetCheck(TRUE);//
	//m_listctrl_Input.SetColumnWidth(0,50);
	//m_listctrl_Input.SetColumnWidth(1,100);
	//m_listctrl_Input.SetColumnWidth(2,110);

	return TRUE; 
}


void Input_Manage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listctrl_Input);
	DDX_Control(pDX, IDC_EDIT1, m_EditSO);
	DDX_Control(pDX, IDC_EDIT2, m_EditID);
	DDX_Control(pDX, IDC_Qty_show, m_Qty_show);
	DDX_Control(pDX, IDC_PN_show, m_PN_show);
	DDX_Control(pDX, IDC_SW_Show, m_SW_show);
	DDX_Control(pDX, IDC_Fail_show, m_Fail_show);
	DDX_Control(pDX, IDC_Pass_Show, m_Pass_show);
	DDX_Control(pDX, IDC_Total_show, m_Total_show);
	DDX_Control(pDX, IDC_CHECK1, m_Lock_check);
}


BEGIN_MESSAGE_MAP(Input_Manage, CDialog)
	ON_BN_CLICKED(IDOK, &Input_Manage::OnBnClickedOk)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &Input_Manage::OnNMCustomdrawList1)
	ON_EN_CHANGE(IDC_EDIT2, &Input_Manage::OnEnChangeEdit2)
	ON_EN_KILLFOCUS(IDC_EDIT1, &Input_Manage::OnEnKillfocusEdit1)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CartonID, &Input_Manage::OnBnClickedCartonid)
	ON_BN_CLICKED(IDC_IMEI, &Input_Manage::OnBnClickedImei)
	ON_BN_CLICKED(IDC_CHECK1, &Input_Manage::OnBnClickedCheck1)
	ON_NOTIFY(NM_THEMECHANGED, IDC_CHECK1, &Input_Manage::OnNMThemeChangedCheck1)
END_MESSAGE_MAP()


// Input_Manage message handlers



bool Input_Manage::FcheckIMEI(CString str_IMEI, int nIMEI_current_number)
{
	int nSize = m_sz_IMEIArray.GetSize();
	for(int i = 0;i< nSize;i++)
	{
		CString strTemp = m_sz_IMEIArray.GetAt(i);
		if(str_IMEI.MakeUpper() == strTemp.MakeUpper())
		{
			return false;
		}
	}

//	m_sz_IMEIArray.Add(str_IMEI);
	return true;
}

void Input_Manage::OnBnClickedOk()
{
	bool bInsertPass= false;
	CString strError = _T("PASS");
	CString str_SO = _T("");
	CString str_InputID = _T("");
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
	char sz_EntranceToolVer[50]={0};
	char sz_Qty[50]={0};

    char sz_result[50];

	char sz_idType[50] = {0};
	char sz_idLength[50]={0};

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
    memset(sz_result,0,sizeof(sz_result));
	char sz_count[5];
	memset(sz_count,sizeof(sz_count),0);


	m_EditSO.GetWindowText(str_SO);
	m_EditID.GetWindowText(str_InputID);

	if(str_InputID.IsEmpty())
	{
		msgDlg.SetErrorCode(CString(_T("WM0004")));
		msgDlg.SetError(CString(_T("请输入Carton ID!\nPlease Input Carton ID")));
		msgDlg.DoModal();
		return;
	}

	b_res = DB_Input.FconnectDB();
	if(!b_res)
	{
		msgDlg.SetErrorCode(CString(_T("WM0003")));
		msgDlg.SetError(CString(_T("连接数据库失败!\nConnect DB Fail!")));
		msgDlg.DoModal();
		DB_Input.FcloseDB();
		return;
	}

	CString str_IMEIList = _T("");
	CString str_IMEIList_S = _T("");
	CString str_IMEInum = 0;
	if(m_check_CTNID)
	{
		/*Remove Carton ID length check from 20140402 Magan*/
		//if(str_InputID.Trim().GetLength() != 13&&str_InputID.Trim().GetLength()!=10)
		//{
		//	msgDlg.SetErrorCode(CString(_T("WM0100")));
		//	msgDlg.SetError(CString(_T("Carton ID 位数不为13或者10!")));
		//	msgDlg.DoModal();
		//	DB_Input.FcloseDB();
		//	return;
		//}
		b_res = DB_Input.pQueryIMEI_ByCarton(str_InputID.GetBuffer(), sz_count, sz_IMEIList, sz_errcode, sz_errMsg);
		str_InputID.ReleaseBuffer();
		if(!b_res)
		{
			msgDlg.SetErrorCode((CString)sz_errcode);
			msgDlg.SetError((CString)sz_errMsg);
			msgDlg.DoModal();
			DB_Input.FcloseDB();
			return;
		}
		str_CartonID = str_InputID;

		str_IMEIList.Format(_T("%s"),sz_IMEIList);
		str_IMEInum.Format(_T("%s"),sz_count);
		str_IMEIList_S.Format(_T("%s"),sz_IMEIList);
		if(str_IMEIList.GetLength() == 0)
		{
			msgDlg.SetErrorCode(CString(_T("WM0103")));
			msgDlg.SetError(CString(_T("该CartonID") + str_CartonID + _T("查询出来的ID号为空!")));
			msgDlg.DoModal();
			DB_Input.FcloseDB();
			return;
		}
		while(str_IMEIList_S.GetLength() > 0)
		{
			if(str_IMEIList_S == _T(","))
				break;
			int nIndex = str_IMEIList_S.Find(_T(','));
			CString strItem = str_IMEIList_S.Left(nIndex);
			str_IMEIList_S = str_IMEIList_S.Mid(nIndex+1);
			char sz_cartonID[50];
			memset(sz_cartonID,0,sizeof(sz_cartonID));
			memset(sz_status,0,sizeof(sz_status));
			b_res = DB_Input.pQueryIMEIStatus(strItem.GetBuffer(),sz_status,sz_cartonID,sz_errcode,sz_errMsg);
			strItem.ReleaseBuffer();
			if(!b_res)
			{
				msgDlg.SetErrorCode((CString)sz_errcode);
				msgDlg.SetError((CString)sz_errMsg);
				msgDlg.DoModal();
				DB_Input.FcloseDB();
				return;
			}
			if(strcmp(sz_status,"P")!=0)
			{
				msgDlg.SetErrorCode((CString)sz_errcode);
				msgDlg.SetError((CString)(_T("ID号")+strItem+_T("的当前状态不为P,无法进行入库")));
				msgDlg.DoModal();
				DB_Input.FcloseDB();
				return;
			}
		}
	}
	else
	{
		//if(str_InputID.Trim().GetLength() != 15)
		if(str_InputID.Trim().GetLength() != m_nIDLength)
		{
			msgDlg.SetErrorCode(CString(_T("WM0100")));
			msgDlg.SetError(CString(_T("号码长度错误!")));
			msgDlg.DoModal();
			DB_Input.FcloseDB();
			return;
		}
		str_IMEIList = str_InputID + _T(",");
		char sz_cartonID[50];
		memset(sz_cartonID,0,sizeof(sz_cartonID));
		//QueryIMEIStatus(char * sz_InputID,char * sz_status,char * sz_cartonID, char * sz_ErrCode,char * sz_ErrMsg)
		memset(sz_status,0,sizeof(sz_status));
		b_res = DB_Input.pQueryIMEIStatus(str_InputID.GetBuffer(),sz_status,sz_cartonID,sz_errcode,sz_errMsg);
		str_InputID.ReleaseBuffer();
		if(!b_res)
		{
			msgDlg.SetErrorCode((CString)sz_errcode);
			msgDlg.SetError((CString)sz_errMsg);
			msgDlg.DoModal();
			DB_Input.FcloseDB();
			return;
		}
		if(strcmp(sz_status,"P")!=0)
		{
			msgDlg.SetErrorCode((CString)sz_errcode);
			msgDlg.SetError((CString)(_T("ID号")+str_InputID+_T("的当前状态不为P,无法进行入库")));
			msgDlg.DoModal();
			DB_Input.FcloseDB();
			return;
		}
		str_CartonID.Format(_T("%s"),sz_cartonID);
	}


	while(str_IMEIList.GetLength() > 0)
	{
		strError = _T("PASS");
		int nIndex = str_IMEIList.Find(_T(','));
		CString strItem = str_IMEIList.Left(nIndex);
		str_IMEIList = str_IMEIList.Mid(nIndex+1);
		b_res = FcheckIMEI(strItem, 0);
		if(!b_res)
		{
			msgDlg.SetErrorCode(CString(_T("WM0101")));
			msgDlg.SetError(CString(_T("该ID号") + strItem + _T("重复!")));
			msgDlg.DoModal();
			DB_Input.FcloseDB();
			return;
		}

		//b_res = pQueryOOBstatus(sz_IMEI, sz_status, sz_errcode, sz_errMsg);
		memset(sz_status,0,sizeof(sz_status));
		b_res = DB_Input.pQueryOOBstatus(strItem.GetBuffer(), sz_status, sz_errcode, sz_errMsg);
		strItem.ReleaseBuffer();
		str_Status.Format(_T("%s"),sz_status);
		if(!b_res)
		{
			msgDlg.SetErrorCode((CString)sz_errcode);
			msgDlg.SetError((CString)sz_errMsg);
			msgDlg.DoModal();
			DB_Input.FcloseDB();
			return;
		}

		//GetIMEI_Info(char * sz_InputID, char * sz_model,char * sz_Picasso, char * sz_partno,char * sz_SO, char * sz_sw,char * sz_ErrCode, char * sz_ErrMsg)

		b_res = DB_Input.pGetIMEI_Info(strItem.GetBuffer(),sz_module,sz_Picasso,sz_partno,sz_SO,sz_sw,sz_errcode,sz_errMsg);
		strItem.ReleaseBuffer();
		if(!b_res)
		{
			msgDlg.SetErrorCode((CString)sz_errcode);
			msgDlg.SetError((CString)sz_errMsg);
			msgDlg.DoModal();
			DB_Input.FcloseDB();
			return;
		}

		//(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char * sz_toolVer, char * sz_facImageVer, char * sz_userImageVer,char* sz_errorCode, char* sz_errorMsg);
		b_res = DB_Input.pQuerySO_Info_All_V2(str_SO.GetBuffer(),sz_model_SO,sz_partNo_SO,sz_soType_SO,sz_toolVer_SO,sz_facImageVer_SO,sz_userImageVer_SO,sz_EntranceToolVer,sz_Qty,sz_errcode,sz_errMsg);
		str_SO.ReleaseBuffer();
		if(!b_res)
		{
			msgDlg.SetErrorCode((CString)sz_errcode);
			msgDlg.SetError((CString)sz_errMsg);
			msgDlg.DoModal();
			DB_Input.FcloseDB();
			return;
		}

		bInsertPass = true;
		if(str_Status == _T("0") || str_Status.IsEmpty() || str_Status == _T("1"))
		{

			b_res = DB_Input.pQueryOOBResult(sz_Picasso,sz_result,sz_errcode,sz_errMsg);
			if(!b_res && (strcmp(sz_errcode, _T("DB1000")) == 0))
			{
				msgDlg.SetErrorCode((CString)sz_errcode);
				msgDlg.SetError((CString)sz_errMsg);
				msgDlg.DoModal();
				DB_Input.FcloseDB();
				return;
			}
			else if(!b_res && str_Status == _T("1")) //Need but not record
			{
					bInsertPass = false;
					strError = _T("OOB记录不存在");
			}
			else
			{
				if(b_res && (strcmp(sz_result,"N")==0)) //fail record
				{
					bInsertPass = false;
					strError = _T("OOB存在Fail的记录");
				}

			}

			if(bInsertPass)
			{
				if(strcmp(sz_module,_T("")) == 0)
				{
					msgDlg.SetErrorCode(CString(_T("WM0008")));
					msgDlg.SetError(CString(_T("该ID查询的Module为空!\nSuch Module queried by ID is NULL")));
					msgDlg.DoModal();
					DB_Input.FcloseDB();
					return;
				}
				//(*QuerySegmentProcess)(char * sz_modelname, char * sz_segment,char * sz_process, char * sz_ErrCode,char * sz_ErrMsg);
				b_res = DB_Input.pQuerySegmentProcess(sz_module, "",sz_ProcessList,sz_errcode,sz_errMsg);
				if(!b_res)
				{
					msgDlg.SetErrorCode((CString)sz_errcode);
					msgDlg.SetError((CString)sz_errMsg);
					msgDlg.DoModal();
					DB_Input.FcloseDB();
					return;
				}
				str_ProcessList.Format(_T("%s"),sz_ProcessList);
				//CString str_test = str_ProcessList.Right(1);
				if(str_ProcessList.Right(1) == _T(","))
				{
					str_ProcessList = str_ProcessList.Left(str_ProcessList.GetLength() - 1);
				}
				if(str_ProcessList.Right(4) == _T(",OOB"))
				{
					str_ProcessList = str_ProcessList.Left(str_ProcessList.GetLength() - 4);
				}
				str_Process = str_ProcessList.Mid(str_ProcessList.ReverseFind(_T(',')) + 1);

				b_res = DB_Input.pGetProcessResult(sz_Picasso,sz_module,str_Process.GetBuffer());
				str_Process.ReleaseBuffer();
				if(!b_res)
				{
					bInsertPass = false;
					strError = str_Process + _T("test fail");
				}
			}
		}
		else
		{
			msgDlg.SetErrorCode(CString(_T("WM0007")));
			msgDlg.SetError(CString(_T("数据库中不存在") + str_Status + _T("状态!\nSuch status is not exist ")));
			msgDlg.DoModal();
			DB_Input.FcloseDB();
			return;
		}

		if(bInsertPass)
		{
			CString strImage_SO = _T("");
			CString strImage_IMEI = _T("");
			strImage_SO.Format(_T("%s"),sz_userImageVer_SO);
			strImage_IMEI.Format(_T("%s"),sz_sw);

			CString strPart = sz_partNo_SO;
			if(strPart.Left(2) != _T("CS")) //cs not check so
			{
				if(strcmp(str_SO.GetBuffer(),sz_SO) != 0)
				{
					bInsertPass = false;
					strError.Format(_T("SO inconsistent. SO Input: %s -- SO by ID: %s"), str_SO.GetBuffer(), sz_SO);
				}
			}
		}

		if(bInsertPass)
		{
			if(strcmp(sz_partNo_SO,sz_partno) != 0)
			{
				bInsertPass = false;
				strError.Format(_T("PartNo inconsistent. SO PartNo: %s -- PartNo by ID: %s"), sz_partNo_SO, sz_partno); 
			}
		}

		if(bInsertPass)
		{
			//Start Add by Lennon 2013.03.28 Insert WareHouse Record TableName = IMEICheckInfo
			char sz_Status[32]={0};
			char sz_InsertDate[64] = {0};
			char sz_InsertTime[64] = {0};
			char sz_ErrCode[512]={0};
			char sz_ErrMsg[512]={0};
			b_res = DB_Input.pQuery_WareHouseStatus_V2(sz_Picasso,strItem.GetBuffer(),sz_Status,sz_InsertDate, sz_InsertTime, sz_ErrCode,sz_ErrMsg);
			strItem.ReleaseBuffer( );
			if(!b_res)
			{
				msgDlg.SetErrorCode((CString)sz_ErrCode);
				msgDlg.SetError((CString)sz_ErrMsg);
				msgDlg.DoModal();
				DB_Input.FcloseDB();
				return;
			}
			CString str_WareHouseStatus = sz_Status;
			if(_T("L") == str_WareHouseStatus )   // report error already exist
			{
//				msgDlg.SetErrorCode((CString)sz_ErrCode);
				CString strErrMsg;
				strErrMsg.Format( _T("该手机已经入库! 入库时间为：%s -- %s "), sz_InsertDate, sz_InsertTime);
				msgDlg.SetErrorCode(strErrMsg);
				msgDlg.SetError((CString)_T(""));
				msgDlg.DoModal();
				DB_Input.FcloseDB();
				return;
		}
			else// (_T("L") != str_WareHouseStatus )
			{
				b_res = DB_Input.pInsert_WareHouseIn(sz_Picasso,strItem.GetBuffer(),sz_SO, sz_partno,sz_sw,sz_Qty ,str_CartonID.GetBuffer(),sz_ErrCode,sz_ErrMsg);
				strItem.ReleaseBuffer( );
				str_CartonID.ReleaseBuffer( );
				if(!b_res)
				{
					msgDlg.SetErrorCode((CString)sz_ErrCode);
					msgDlg.SetError((CString)sz_ErrMsg);
					msgDlg.DoModal();
					DB_Input.FcloseDB();
					return;
				}
			}
			//End   Add by Lennon 2013.03.38
		}

		this->	m_sz_IMEIArray.Add(strItem);
		CString str_number = _T("");
		str_number.Format("%d",(nIMEInum + 1));
		int nRow = m_listctrl_Input.InsertItem(nIMEInum,str_number);//插入行
		m_listctrl_Input.SetItemText(nRow, 1, str_CartonID);//设置数据
		m_listctrl_Input.SetItemText(nRow, 2, strItem);//设置数据
		m_listctrl_Input.SetItemText(nRow, 3, strError);//设置数据

		if(!bInsertPass)
		{
			m_listctrl_Input.SetItemColor(nRow,RGB(255,0,0));
			m_listctrl_Input.EnsureVisible(nIMEInum,true);
			nIMEInum++;
			n_fail++;
		}
		else
		{
			m_listctrl_Input.SetItemColor(nRow,RGB(0,255,0));
			m_listctrl_Input.EnsureVisible(nIMEInum,true);
			nIMEInum++;
			n_pass++;
		}
	}
	m_EditID.SetWindowTextA(_T(""));
	DB_Input.FcloseDB();
	CString str_total = _T("");
	CString str_Fail = _T("");
	CString str_Pass = _T("");
	str_total.Format("%d",m_sz_IMEIArray.GetSize());
	str_Fail.Format("%d",n_fail);
	str_Pass.Format("%d",n_pass);
	m_Pass_show.SetWindowTextA(str_Pass);
	m_Fail_show.SetWindowTextA(str_Fail);
	m_Total_show.SetWindowTextA(str_total);
}

void Input_Manage::OnNMCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void Input_Manage::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	//CString str_CartonID = _T("");
	//m_EditID.GetWindowText(str_CartonID);
	//m_str_ID = str_CartonID;
	//if(m_str_ID.GetLength() == 13)
	//{
	//	Sleep(1000);
	//	OnBnClickedOk();
	//}
}

void Input_Manage::OnEnKillfocusEdit1()
{
	// TODO: Add your control notification handler code here
	
}

HBRUSH Input_Manage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	//HBRUSH hwndBrush;
	//hwndBrush.CreateSolidBrush(RGB(215,240,198));//生成一绿色刷子

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired

	switch(pWnd->GetDlgCtrlID())
	{
	//针对ID为IDC_CTL1、IDC_CTL2和IDC_CTL3的控件进行同样的设置
		case IDC_PN_show:
		case IDC_SW_Show:
		case IDC_Qty_show:
		{
		   pDC->SetBkMode(TRANSPARENT);
		   pDC->SetTextColor(RGB(0,0, 255));
		   return m_hwndBrush;
		}
		default:
			{
			   pDC->SetBkMode(TRANSPARENT);
			   return m_hwndBrush;
			}
	}
	//return hbr;
}

void Input_Manage::OnBnClickedCartonid()
{
	// TODO: Add your control notification handler code here
	m_check_CTNID = true;
}

void Input_Manage::OnBnClickedImei()
{
	// TODO: Add your control notification handler code here
	m_check_CTNID = false;
}

void Input_Manage::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	if(m_Lock_check.GetCheck() == 1)
	{
	   m_Lock_check.SetCheck(0);

	   CString str_SO = _T("");
		m_EditSO.GetWindowText(str_SO);
		bool b_res = true;

		char sz_model[50];
		char sz_partNo[50];
		char sz_soType[50];
		char sz_toolVer[50];
		char sz_facImageVer[50];
		char sz_userImageVer[50];
		char sz_EntranceToolVer[50];
		char sz_Qty[50];
		char sz_errorCode[256];
		char sz_errorMsg[256];

		memset(sz_model,0,sizeof(sz_model));
		memset(sz_partNo,0,sizeof(sz_partNo));
		memset(sz_soType,0,sizeof(sz_soType));
		memset(sz_toolVer,0,sizeof(sz_toolVer));
		memset(sz_facImageVer,0,sizeof(sz_facImageVer));
		memset(sz_userImageVer,0,sizeof(sz_userImageVer));
		memset(sz_EntranceToolVer,0,sizeof(sz_EntranceToolVer));
		memset(sz_Qty,0,sizeof(sz_Qty));
		memset(sz_errorCode,0,sizeof(sz_errorCode));
		memset(sz_errorMsg,0,sizeof(sz_errorMsg));

		if(str_SO.IsEmpty())
		{
			msgDlg.SetErrorCode(CString(_T("WM0002")));
			msgDlg.SetError(CString(_T("请输入SO!\nPlease Input SO")));
			msgDlg.DoModal();
			return;
		}

		if(str_SO.GetLength() < 6)
		{
			msgDlg.SetErrorCode(CString(_T("WM0001")));
			msgDlg.SetError(CString(_T("请输入6个数字!\nPlease Input 6 digital numbers")));
			msgDlg.DoModal();
			m_EditSO.SetWindowText(_T(""));
			return;
		}

		b_res = DB_Input.FconnectDB();
		if(!b_res)
		{
			msgDlg.SetErrorCode(CString(_T("WM0003")));
			msgDlg.SetError(CString(_T("连接数据库失败!\nConnect DB Fail!")));
			msgDlg.DoModal();
			DB_Input.FcloseDB();
			m_EditSO.SetWindowText(_T(""));
			return;
		}

		//bool QuerySO_Info_All_V2(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char * sz_toolVer, char * sz_facImageVer, char * sz_userImageVer,char * sz_EntranceToolVer,char * sz_Qty,char* sz_errorCode, char* sz_errorMsg)
		b_res = DB_Input.pQuerySO_Info_All_V2(str_SO.GetBuffer(),sz_model,sz_partNo,sz_soType,sz_toolVer,sz_facImageVer,sz_userImageVer,sz_EntranceToolVer,sz_Qty,sz_errorCode,sz_errorMsg);
		str_SO.ReleaseBuffer();
		DB_Input.FcloseDB();
		if(!b_res)
		{
			msgDlg.SetErrorCode((CString)sz_errorCode);
			msgDlg.SetError((CString)sz_errorMsg);
			msgDlg.DoModal();
			m_EditSO.SetWindowText(_T(""));
			return;
		}

		n_total = atoi(sz_Qty);

		m_PN_show.SetWindowText(sz_partNo);
		m_SW_show.SetWindowText(sz_userImageVer);
		m_Qty_show.SetWindowText(sz_Qty);

		char sz_IDType[50] ={0};
		char sz_IDLength[50] ={0};
		b_res = DB_Input.pQuery_IDType(sz_partNo, sz_IDType, sz_IDLength, sz_errorCode, sz_errorMsg);

		m_nIDLength = atoi(sz_IDLength);
	   m_EditSO.SetReadOnly();
	   m_b_lock = true;
	   m_Lock_check.SetCheck(1);

	}
	else
	{
		m_EditSO.SetReadOnly(0);
		m_EditSO.SetWindowText(_T(""));
		m_b_lock = false;
		m_PN_show.SetWindowText(_T(""));
		m_SW_show.SetWindowText(_T(""));
		m_Qty_show.SetWindowText(_T(""));

	}
}

void Input_Manage::OnNMThemeChangedCheck1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Windows XP or greater.
	// The symbol _WIN32_WINNT must be >= 0x0501.
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
