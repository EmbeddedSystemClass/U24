// IMEICartion.cpp : implementation file
//

#include "stdafx.h"
#include "FAV.h"
#include "IMEICartion.h"



// CIMEICartion dialog

IMPLEMENT_DYNAMIC(CIMEICartion, CDialog)

CIMEICartion::CIMEICartion(CWnd* pParent /*=NULL*/)
	: CDialog(CIMEICartion::IDD, pParent)
{
	bEraseCarton = true;
}

CIMEICartion::~CIMEICartion()
{
}

void CIMEICartion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(CIMEICartion, CDialog)
	ON_BN_CLICKED(IDOK, &CIMEICartion::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_IMEI, &CIMEICartion::OnEnChangeEditImei)
	ON_EN_KILLFOCUS(IDC_EDIT_IMEI, &CIMEICartion::OnEnKillfocusEditImei)
	ON_EN_UPDATE(IDC_EDIT_IMEI, &CIMEICartion::OnEnUpdateEditImei)
	ON_BN_CLICKED(IDC_BUTTON1, &CIMEICartion::OnBnClickedButton1)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CIMEICartion message handlers

void CIMEICartion::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
//	CString str_IMEI;
//	CString str_Carton;
//	GetDlgItem(IDC_EDIT_IMEI)->GetWindowText(str_IMEI);
//	GetDlgItem(IDC_EDIT_CARTON)->GetWindowText(str_Carton);
//
//	if(str_IMEI.GetLength() == 0 || str_Carton.GetLength() == 0)
//	{
//		AfxMessageBox(_T("请输入IMEI号码和Carton号码， 然后再按\"Check\" 按钮"),MB_OK, MB_ICONSTOP);
//		return;
//	}
//
//	char sz_IMEIList[2048] = {0};
//	char sz_count[50] ={0};
//	char sz_errcode[100] ={0};
//	char sz_errMsg[2048] ={0};
//	char sz_carton[50] = {0};
//	::WideCharToMultiByte(CP_ACP, 0, str_Carton, -1, sz_carton, str_Carton.GetLength() * 2, NULL, NULL);
//
//
//	bool bRes = ConnectDB();
//	if(bRes)
//	{
//		bRes = QueryIMEI_ByCarton(sz_carton, sz_count, sz_IMEIList, sz_errcode, sz_errMsg);
//		CloseDB();
//		if(!bRes)
//		{
//			AfxMessageBox(_T("请检查网络， 查询数据库失败"),MB_OK, MB_ICONSTOP);
//			return;
//		}
//
//		bool bCheckOK = false;
//		CString str_IMEIList;// = sz_IMEIList;
//
//		int  len = 0;
//		len = strlen(sz_IMEIList);
//		int  unicodeLen = ::MultiByteToWideChar( CP_ACP,0,sz_IMEIList,-1,NULL,0 );  
//		wchar_t *  pUnicode;  
//		pUnicode = new  wchar_t[unicodeLen+1];  
//
//		::MultiByteToWideChar( CP_ACP,0,sz_IMEIList,-1,pUnicode,unicodeLen );  
//		str_IMEIList = pUnicode;
//
//		delete pUnicode;
//	//	str_IMEIList.ReleaseBuffer();
//
////		str_IMEIList.Format(_T("%s"), sz_IMEIList);
//
//		int index = str_IMEIList.Find(str_IMEI.GetBuffer());
//		str_IMEI.ReleaseBuffer();
//		if(index != -1)
//			bCheckOK = true;
//
////		str_IMEIList.ReleaseBuffer();
//		//while(str_IMEIList.GetLength() > 0)
//		//{
//		//	if(str_IMEIList == _T(","))
//		//		break;
//		//	int nIndex = str_IMEIList.Find(_T(','));
//		//	CString strItem = str_IMEIList.Left(nIndex);
//		//	str_IMEIList = str_IMEIList.Mid(nIndex+1);
//		//	if(strItem == str_IMEI)
//		//	{	
//		//		bCheckOK = true;
//		//		break;
//		//	}
//		//}
//
//		if(!bCheckOK)
//		//	AfxMessageBox(_T("IMEI和Carton一致"), MB_OK, MB_ICONINFORMATION );
//		//else
//			AfxMessageBox(_T("不一致"),MB_OK, MB_ICONSTOP); 
//	}
//	else
//	{
//		AfxMessageBox(_T("请检查网络， 查询数据库失败"),MB_OK, MB_ICONSTOP);
//		return;
//	}
//
//	GetDlgItem(IDC_EDIT_IMEI)->SetWindowText(_T(""));
//	GetDlgItem(IDC_EDIT_CARTON)->SetWindowText(_T(""));
//	GetDlgItem(IDC_EDIT_IMEI)->SetFocus();
}

void CIMEICartion::OnEnChangeEditImei()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

BOOL CIMEICartion::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	GetDlgItem(IDC_EDIT_IMEI)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_CARTON)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_CARTON)->SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CIMEICartion::OnEnKillfocusEditImei()
{
	// TODO: Add your control notification handler code here
//	GetDlgItem(IDC_EDIT_CARTON)->SetFocus();

}

void CIMEICartion::OnEnUpdateEditImei()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.

	// TODO:  Add your control notification handler code here
	CString in;
	GetDlgItemText(IDC_EDIT_IMEI, in);
	if (in.GetLength()>1)
	{
		if (in.GetAt(in.GetLength()-1)=='/n' || in.GetAt(in.GetLength()-1)=='/r' )
			GetDlgItem(IDC_EDIT_CARTON)->SetFocus();
	}


}

void CIMEICartion::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_CARTON)->SetWindowText(_T(""));

	GetDlgItem(IDC_EDIT_CARTON)->SetFocus();
}

void CIMEICartion::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if(nChar == VK_RETURN)
	{
		CString str_IMEI, str_Carton;
		GetDlgItem(IDC_EDIT_IMEI)->GetWindowText(str_IMEI);
		GetDlgItem(IDC_EDIT_CARTON)->GetWindowText(str_Carton);

		if(str_IMEI == _T(""))
			GetDlgItem(IDC_EDIT_IMEI)->SetFocus();
		else if(str_Carton == _T(""))
			GetDlgItem(IDC_EDIT_CARTON)->SetFocus();
		else
			OnBnClickedButton1();
	}

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}
BOOL CIMEICartion::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			CString str_IMEI, str_Carton;
			GetDlgItem(IDC_EDIT_IMEI)->GetWindowText(str_IMEI);
			GetDlgItem(IDC_EDIT_CARTON)->GetWindowText(str_Carton);

			if(str_Carton == _T(""))
				GetDlgItem(IDC_EDIT_CARTON)->SetFocus();
			else if(str_IMEI == _T(""))
				GetDlgItem(IDC_EDIT_IMEI)->SetFocus();
			else
			{
				Check();
			}
		}
	}

	return __super::PreTranslateMessage(pMsg);
}
void CIMEICartion::Check(void)
{
	CString str_IMEI;
	CString str_Carton;
	GetDlgItem(IDC_EDIT_IMEI)->GetWindowText(str_IMEI);
	GetDlgItem(IDC_EDIT_CARTON)->GetWindowText(str_Carton);

	if(str_IMEI.GetLength() == 0 || str_Carton.GetLength() == 0)
	{
		AfxMessageBox(_T("请输入IMEI号码和Carton号码， 然后再按\"Check\" 按钮"),MB_OK, MB_ICONSTOP);
		return;
	}

	char sz_IMEIList[2048] = {0};
	char sz_count[50] ={0};
	char sz_errcode[100] ={0};
	char sz_errMsg[2048] ={0};
	char sz_carton[50] = {0};
	::WideCharToMultiByte(CP_ACP, 0, str_Carton, -1, sz_carton, str_Carton.GetLength() * 2, NULL, NULL);


	bool bRes = ConnectDB();
	if(bRes)
	{
		bRes = QueryIMEI_ByCarton(sz_carton, sz_count, sz_IMEIList, sz_errcode, sz_errMsg);
		CloseDB();
		if(!bRes)
		{
			AfxMessageBox(_T("请检查网络， 查询数据库失败"),MB_OK, MB_ICONSTOP);
			return;
		}

		bool bCheckOK = false;
		CString str_IMEIList;// = sz_IMEIList;

		int  len = 0;
		len = strlen(sz_IMEIList);
		int  unicodeLen = ::MultiByteToWideChar( CP_ACP,0,sz_IMEIList,-1,NULL,0 );  
		wchar_t *  pUnicode;  
		pUnicode = new  wchar_t[unicodeLen+1];  

		::MultiByteToWideChar( CP_ACP,0,sz_IMEIList,-1,pUnicode,unicodeLen );  
		str_IMEIList = pUnicode;

		delete pUnicode;
	//	str_IMEIList.ReleaseBuffer();

//		str_IMEIList.Format(_T("%s"), sz_IMEIList);

		int index = str_IMEIList.Find(str_IMEI.GetBuffer());
		str_IMEI.ReleaseBuffer();
		if(index != -1)
			bCheckOK = true;

//		str_IMEIList.ReleaseBuffer();
		//while(str_IMEIList.GetLength() > 0)
		//{
		//	if(str_IMEIList == _T(","))
		//		break;
		//	int nIndex = str_IMEIList.Find(_T(','));
		//	CString strItem = str_IMEIList.Left(nIndex);
		//	str_IMEIList = str_IMEIList.Mid(nIndex+1);
		//	if(strItem == str_IMEI)
		//	{	
		//		bCheckOK = true;
		//		break;
		//	}
		//}

		if(!bCheckOK)
		//	AfxMessageBox(_T("IMEI和Carton一致"), MB_OK, MB_ICONINFORMATION );
		//else
			AfxMessageBox(_T("不一致"),MB_OK, MB_ICONSTOP); 
	}
	else
	{
		AfxMessageBox(_T("请检查网络， 查询数据库失败"),MB_OK, MB_ICONSTOP);
		return;
	}

	GetDlgItem(IDC_EDIT_IMEI)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_IMEI)->SetFocus();
}
