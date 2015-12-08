// OutPut_CSD.cpp : implementation file
//

#include "stdafx.h"
#include "warehouseManage.h"
#include "OutPut_CSD.h"


// OutPut_CSD dialog

IMPLEMENT_DYNAMIC(OutPut_CSD, CDialog)

OutPut_CSD::OutPut_CSD(CWnd* pParent /*=NULL*/)
	: CDialog(OutPut_CSD::IDD, pParent)
{   

	PicassoNum = 0;
	n_fail = 0;
	n_pass = 0;
	n_total = 0;
	m_szIMEIArray.RemoveAll();
	m_szPicassoArray.RemoveAll();
	m_b_lock = false;

	m_nIDLength = 15;

}

OutPut_CSD::~OutPut_CSD()
{
}

BOOL OutPut_CSD::OnInitDialog()
{
	CDialog::OnInitDialog();
    
	m_Edit_SO.SetLimitText(7);
	m_hwndBrush.CreateSolidBrush(RGB(215,240,198));//生成一绿色刷子

	int i_colsWidth[4] = {50, 100, 110,480};
	m_listctrl_OutCSD.InsertColumn(0, _T("Number"), LVCFMT_LEFT, i_colsWidth[0]);
	m_listctrl_OutCSD.InsertColumn(1, _T("IMEI"), LVCFMT_LEFT, i_colsWidth[1]);
	m_listctrl_OutCSD.InsertColumn(2, _T("PICASSO"), LVCFMT_LEFT, i_colsWidth[2]);
	m_listctrl_OutCSD.InsertColumn(3, _T("Result"), LVCFMT_LEFT, i_colsWidth[3]);

	m_listctrl_OutCSD.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	((CButton *)GetDlgItem(IDC_CHECK3))->SetCheck(false);

	return TRUE; 
}
void OutPut_CSD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listctrl_OutCSD);
	DDX_Control(pDX, IDC_EDIT1, m_Edit_SO);
	DDX_Control(pDX, IDC_CHECK3,m_Edit_CHECK);
}


BEGIN_MESSAGE_MAP(OutPut_CSD, CDialog)
	ON_EN_KILLFOCUS(IDC_EDIT1, &OutPut_CSD::OnEnKillfocusEdit1)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &OutPut_CSD::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK3, &OutPut_CSD::OnBnClickedCheck3)
END_MESSAGE_MAP()


// OutPut_CSD message handlers
void OutPut_CSD::OnEnKillfocusEdit1()
{   

}

HBRUSH OutPut_CSD::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
	case IDC_Model_show:
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

bool OutPut_CSD::FcheckIMEI(CString str_IMEI, int nIMEI_current_number)
{
	int nSize = m_szIMEIArray.GetSize();

	for(int i = 0;i< nSize;i++)
	{
		CString strTemp = m_szIMEIArray.GetAt(i);
		if(str_IMEI.MakeUpper() == strTemp.MakeUpper())//m_sz_IMEIList[i].MakeUpper())
		{
			return false;
		}
	}

	return true;
}

bool OutPut_CSD::FcheckPicasso(CString str_Picasso, int nPicasso_current_number)
{
	int nSize = m_szPicassoArray.GetSize();
	for(int i = 0;i< nSize;i++)
	{
		CString strTemp = m_szPicassoArray.GetAt(i);
		if(str_Picasso.MakeUpper() == strTemp.MakeUpper()) // m_sz_PicassoList[i].MakeUpper())
		{
			return false;
		}
	}

	return true;
}
void OutPut_CSD::OnBnClickedOk()
{ 
  bool b_res = false;
  CString str_Input_Picasso = _T("");
  CString str_Input_IMEI = _T("");
  CString str_ProcessList = _T("");
  CString str_Process = _T("");
  CString str_error = _T("PASS");
  char sz_module[50];
  char sz_Picasso[50];
  char sz_partno[50];
  char sz_SO[50];
  char sz_sw[50];
  char sz_status[32]={0};
  char sz_cartonID[512]={0};
  char sz_ProcessList[256];
  char sz_errcode[256];
  char sz_errMsg[256];

  memset(sz_module,0,sizeof(sz_module));
  memset(sz_Picasso,0,sizeof(sz_Picasso));
  memset(sz_partno,0,sizeof(sz_partno));
  memset(sz_SO,0,sizeof(sz_SO));
  memset(sz_sw,0,sizeof(sz_sw));
  memset(sz_ProcessList,0,sizeof(sz_ProcessList));
  memset(sz_errcode,0,sizeof(sz_errcode));
  memset(sz_errMsg,0,sizeof(sz_errMsg));
  
  if(this-> GetFocus()-> GetDlgCtrlID()==IDC_EDIT1)
  {
	  GetDlgItem(IDC_EDIT2)->SetFocus();
  }
  else if(this-> GetFocus()-> GetDlgCtrlID()==IDC_EDIT2)
  {
     GetDlgItem(IDC_EDIT3)->SetFocus();
  }

  GetDlgItem(IDC_EDIT3)->GetWindowText(str_Input_Picasso);
  GetDlgItem(IDC_EDIT2)->GetWindowText(str_Input_IMEI);
  
  if(str_Input_Picasso.GetLength() < 10)
  {
     	msgDlg.SetErrorCode(CString(_T("WM0002")));
		msgDlg.SetError(CString(_T("请输入10位的PICASSO!\nPleas1e Input 10 PICASSO")));
		msgDlg.DoModal();
		return;
  }
  
  if(str_Input_IMEI.GetLength()  != m_nIDLength)
  {
     	msgDlg.SetErrorCode(CString(_T("WM0002")));
		msgDlg.SetError(CString(_T("请输入正确的IMEI!\nPleas1e Input Correct IMEI")));
		msgDlg.DoModal();
		return;
  }
  
  b_res = FcheckIMEI(str_Input_IMEI, 0);
  if(!b_res)
  {
	  msgDlg.SetErrorCode(CString(_T("WM0101")));
	  msgDlg.SetError(CString(_T("该IMEI号") + str_Input_IMEI + _T("重复!")));
	  msgDlg.DoModal();
	  return;
  }

  b_res = FcheckPicasso(str_Input_Picasso,0);
  if(!b_res)
  {
	  msgDlg.SetErrorCode(CString(_T("WM1101")));
	  msgDlg.SetError(CString(_T("该Picasso号") + str_Input_Picasso + _T("重复!")));
	  msgDlg.DoModal();
	  return;
  }

  b_res = DB_OutCSD.FconnectDB();

  //Query IMEI/Picasso 是否匹配
  b_res = DB_OutCSD.pGetIMEI_Info(str_Input_IMEI.GetBuffer(),sz_module,sz_Picasso,sz_partno,sz_SO,sz_sw,sz_errcode,sz_errMsg);
  str_Input_IMEI.ReleaseBuffer();
  if(!b_res)
  {
	  DB_OutCSD.FcloseDB();
	  msgDlg.SetErrorCode((CString)sz_errcode);
	  msgDlg.SetError((CString)sz_errMsg);
	  msgDlg.DoModal();
	  return;
  }

  // Add by Lennnon 2013.03.29 Start
  b_res = DB_OutCSD.pQueryIMEIStatus(str_Input_IMEI.GetBuffer(),sz_status,sz_cartonID,sz_errcode,sz_errMsg);
  str_Input_IMEI.ReleaseBuffer();
  if(!b_res)
  {
  	  DB_OutCSD.FcloseDB();
	  msgDlg.SetErrorCode((CString)sz_errcode);
	  msgDlg.SetError((CString)sz_errMsg);
	  msgDlg.DoModal();
	  return;
  }
  else
  {
	  CString strStatus = sz_status;
	  if(_T("P") != strStatus)
	  {
	 	  DB_OutCSD.FcloseDB();
		  msgDlg.SetErrorCode((CString)_T("NA"));
		  msgDlg.SetError((CString)(_T("IMEI号") + str_Input_IMEI + _T("的当前状态不为P,无法进行入库")));
		  msgDlg.DoModal();
		  return;
	  }
  }
  // Add by Lennnon 2013.03.29 Start

  b_res = DB_OutCSD.pQuerySegmentProcess(sz_module, "",sz_ProcessList,sz_errcode,sz_errMsg);
  if(!b_res)
  {
	  DB_OutCSD.FcloseDB();
	  msgDlg.SetErrorCode((CString)_T(""));
	  msgDlg.SetError((CString)_T(""));
	  msgDlg.DoModal();
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

  b_res = DB_OutCSD.pGetProcessResult(sz_Picasso,sz_module,str_Process.GetBuffer());
  str_Process.ReleaseBuffer();
  if(!b_res)
  {
	  b_res = false;
	  DB_OutCSD.FcloseDB();
	  str_error =str_Process + _T("站没有测试PASS的记录");
  }

  //if(b_res)
  //{	//Start Add by Lennon 2013.03.28 Insert WareHouse Record TableName = IMEICheckInfo
		//char sz_Status[32]={0};
		//char sz_ErrCode[512]={0};
		//char sz_ErrMsg[512]={0};
		//b_res = DB_OutCSD.pQuery_WareHouseStatus(str_Input_Picasso.GetBuffer(),str_Input_IMEI.GetBuffer(),sz_Status, sz_ErrCode,sz_ErrMsg);
		//str_Input_Picasso.ReleaseBuffer( );
		//str_Input_IMEI.ReleaseBuffer( );
		//if(!b_res)
		//{
		//	DB_OutCSD.FcloseDB();
		//	msgDlg.SetErrorCode((CString)sz_ErrCode);
		//	msgDlg.SetError((CString)sz_ErrMsg);
		//	msgDlg.DoModal();
		//	return;
		//}
		//CString str_WareHouseStatus = sz_Status;
		//if (_T("L") != str_WareHouseStatus )
		//{
		//	b_res = DB_OutCSD.pInsert_WareHouseIn(str_Input_Picasso.GetBuffer(),str_Input_IMEI.GetBuffer(),sz_SO, sz_partno,sz_sw,m_str_SO_Qty.GetBuffer() ,sz_cartonID,sz_ErrCode,sz_ErrMsg);
		//	str_Input_Picasso.ReleaseBuffer( );
		//	str_Input_IMEI.ReleaseBuffer( );
		//	if(!b_res)
		//	{
		//		DB_OutCSD.FcloseDB();
		//		msgDlg.SetErrorCode((CString)sz_ErrCode);
		//		msgDlg.SetError((CString)sz_ErrMsg);
		//		msgDlg.DoModal();
		//		return;
		//	}
		//}
  //}

  if(b_res)
  {
		if(str_Input_Picasso != sz_Picasso)
		{
			  b_res = false;

			  str_error = _T("Picasso不一致.");
			  str_error = str_error + _T(" Picasso输入值为: ") + str_Input_Picasso + _T(" Picasso通过IMEI查询得到值为: ");
			  str_error = str_error + sz_Picasso;
		 }
  }


	CString str_number = _T("");
	str_number.Format("%d",(PicassoNum + 1));
	int nRow = m_listctrl_OutCSD.InsertItem(PicassoNum,str_number);//插入行
	m_listctrl_OutCSD.SetItemText(nRow, 1, str_Input_IMEI);//设置数据
	m_listctrl_OutCSD.SetItemText(nRow, 2, str_Input_Picasso);//设置数据
	m_listctrl_OutCSD.SetItemText(nRow, 3, str_error);//设置数据
	if(b_res)
	{
		n_pass++;
		m_listctrl_OutCSD.SetItemColor(nRow,RGB(0,255,0));
	}
	else
	{
 		n_fail++;
		m_listctrl_OutCSD.SetItemColor(nRow,RGB(255,0,0));
	}

	m_listctrl_OutCSD.EnsureVisible(PicassoNum,true);
	PicassoNum++;
	n_total++;
	GetDlgItem(IDC_EDIT2)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT3)->SetWindowText(_T(""));

	
	CString str_total = _T("");
	CString str_Fail = _T("");
	CString str_Pass = _T("");
	str_total.Format("%d",n_total);
	str_Fail.Format("%d",n_fail);
	str_Pass.Format("%d",n_pass);
	GetDlgItem(IDC_Total_show)->SetWindowText(str_total);
	GetDlgItem(IDC_Fail_show)->SetWindowText(str_Fail);
	GetDlgItem(IDC_Pass_Show)->SetWindowText(str_Pass);
	GetDlgItem(IDC_EDIT2)->SetFocus();

	m_szPicassoArray.Add(str_Input_Picasso);
	m_szIMEIArray.Add(str_Input_IMEI);

	DB_OutCSD.FcloseDB();

	return;
}




void OutPut_CSD::OnBnClickedCheck3()
{   
    if(((CButton *)GetDlgItem(IDC_CHECK3))->GetCheck()==1)
	{
		((CButton *)GetDlgItem(IDC_CHECK3))->SetCheck(0);
		bool b_res=false;

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
	    

		GetDlgItem(IDC_EDIT1)->GetWindowText(m_str_so);
		if(m_str_so.GetLength() < 6)
		{
			msgDlg.SetError(CString(_T("SO小于6码，请输入正确SO!\n Please input Correct SO First!")));
			msgDlg.DoModal();
			return;
		}

		b_res = DB_OutCSD.FconnectDB();
		if(!b_res)
		{
			msgDlg.SetErrorCode(CString(_T("WM0003")));
			msgDlg.SetError(CString(_T("连接数据库失败!\nConnect DB Fail!")));
			msgDlg.DoModal();
			DB_OutCSD.FcloseDB();
			return;
		}
		b_res = DB_OutCSD.pQuerySO_Info_All_V2(m_str_so.GetBuffer(),sz_model,sz_partNo,sz_soType,sz_toolVer,sz_facImageVer,sz_userImageVer,sz_EntranceToolVer,sz_Qty,sz_errorCode,sz_errorMsg);

		DB_OutCSD.FcloseDB();

		m_str_so.ReleaseBuffer();

		if(!b_res)
		{
			msgDlg.SetErrorCode((CString)sz_errorCode);
			msgDlg.SetError((CString)sz_errorMsg);
			msgDlg.DoModal();
			return;
		}

		CString str_PartNo = sz_partNo;

		if(str_PartNo.Left(2) != _T("CS"))
		{   
			GetDlgItem(IDC_EDIT1)->SetWindowText(_T(""));
			GetDlgItem(IDC_Model_show)->SetWindowText(_T(""));
			GetDlgItem(IDC_PN_show)->SetWindowText(_T(""));
			GetDlgItem(IDC_SW_Show)->SetWindowText(_T(""));
			GetDlgItem(IDC_EDIT1)->SetFocus();
			msgDlg.SetErrorCode(CString(_T("WM1102")));
			msgDlg.SetError(CString(_T("该工单查询得到的料号不是CSD的，请转去其他入库或者出库工具")));
			msgDlg.DoModal();
			return;
		}

		GetDlgItem(IDC_Model_show)->SetWindowText(sz_model);
		GetDlgItem(IDC_PN_show)->SetWindowText(sz_partNo);
		GetDlgItem(IDC_SW_Show)->SetWindowText(sz_userImageVer);    

		m_str_SO_Qty.Format("%s",sz_Qty);

		char sz_IDType[50] ={0};
		char sz_IDLength[50] ={0};
		b_res = DB_OutCSD.pQuery_IDType(sz_partNo, sz_IDType, sz_IDLength, sz_errorCode, sz_errorMsg);

		m_nIDLength = atoi(sz_IDLength);

		((CButton *)GetDlgItem(IDC_CHECK3))->SetCheck(1);
		m_b_lock = true;
		GetDlgItem(IDC_EDIT1)->EnableWindow(false);
	}
	else
	{
		m_b_lock = false;
		GetDlgItem(IDC_EDIT1)->EnableWindow(true);
		GetDlgItem(IDC_EDIT1)->SetWindowText(_T(""));
	}
}


