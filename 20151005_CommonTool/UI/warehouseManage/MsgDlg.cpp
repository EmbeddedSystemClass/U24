// MsgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MsgDlg.h"


// MsgDlg dialog

IMPLEMENT_DYNAMIC(MsgDlg, CDialog)

MsgDlg::MsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(MsgDlg::IDD, pParent)
{

}

MsgDlg::~MsgDlg()
{
}

void MsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_Ercode, m_errcode);
	DDX_Control(pDX, IDC_STATIC_ERMsg, m_errmsg);
}


BEGIN_MESSAGE_MAP(MsgDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &MsgDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL MsgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_errcode.SetTextColor(RGB(255, 0, 0));
	m_errcode.SetFontBold(TRUE);
	m_errcode.SetFontSize(55);
	m_errcode.SetWindowText(m_cstrMsg_code);

    m_errmsg.SetTextColor(RGB(0, 0, 255));
	m_errmsg.SetFontBold(FALSE);
	m_errmsg.SetFontSize(20);
	m_errmsg.SetWindowText(m_cstrMsg);


	return true;
}

HBRUSH MsgDlg::CreateHBRUSH()
{
	m_brush.CreateSolidBrush(RGB(215,240,198));  // 淡绿色 - 对话框背景色
	return m_brush;
}

HBRUSH MsgDlg::DeleteHBRUSH()
{
	m_brush.DeleteObject();
	return m_brush;
}

void MsgDlg::SetTextColor(COLORREF color)
{
	m_TextColor = color;
}

void MsgDlg::SetTextSize(int iSize)
{
	m_iTextSize = iSize;
}

void MsgDlg::SetError(CString& cstrMsg)
{
	//m_cstrMsg = cstrMsg;

	CString str_display = _T("");
	str_display = cstrMsg;
	if(cstrMsg.IsEmpty())
	{
		return ;
	}
	else
	{
		str_display.Trim();
	}
	int nLength = 0;
	nLength = str_display.GetLength();
	int n_spillt_black = 0;
	int n_spillt = 0;
	CString str_Text_modify = _T("");
	if(nLength < 40)
	{
		m_cstrMsg = str_display;
		return;
	}

	int n_line = 0;
	int n_lengthModify = 0;
	int n_lengthTemp = 0;
	int n_length_Total = 0;
	int n_length_delete = 0;      //换行前的字符个数
	CString str_temp = _T("");

	while(str_display.GetLength() > 0)
	{
		n_spillt_black = str_display.Find(_T(" "));
		n_spillt = str_display.Find(_T("\\"));
		if(n_spillt_black == -1 && n_spillt == -1)
		{
			int n_test = str_display.GetLength();
			int n_test1 = str_Text_modify.GetLength();
			if((str_Text_modify.GetLength() + str_display.GetLength() - n_length_delete - n_line) > 25)
			{
				str_Text_modify = str_Text_modify + _T("\n");
			}
			str_Text_modify = str_Text_modify + str_display;
			str_display = _T("");
			break;
		}
		if(n_spillt_black <= 40 || n_spillt <= 40)
		{
			if(n_spillt_black >= n_spillt)
			{
				if(n_spillt == -1)
				{
					str_temp = str_display.Mid(0,n_spillt_black + 1);
					str_display = str_display.Mid(n_spillt_black + 1);
				}
				else
				{
					str_temp = str_display.Mid(0,n_spillt + 1);
					str_display = str_display.Mid(n_spillt + 1);
				}
			}
			else
			{
				if(n_spillt_black == -1)
				{
					str_temp = str_display.Mid(0,n_spillt + 1);
					str_display = str_display.Mid(n_spillt + 1);
				}
				else
				{
					str_temp = str_display.Mid(0,n_spillt_black + 1);
					str_display = str_display.Mid(n_spillt_black + 1);
				}

			}
		}
		else//都大于25个字符
		{
			if(n_spillt_black >= n_spillt)
			{
				str_temp = str_display.Mid(0,n_spillt + 1);
				str_display = str_display.Mid(n_spillt + 1);
			}
			else
			{
				str_temp = str_display.Mid(0,n_spillt_black + 1);
				str_display = str_display.Mid(n_spillt_black + 1);
			}

			//n_length_delete = n_length_delete + str_temp.GetLength();
			n_length_delete = str_Text_modify.GetLength() - n_line;
			n_line++;
			str_Text_modify = str_Text_modify + _T("\n");

			str_Text_modify = str_Text_modify + str_temp + _T("\n");
			n_length_delete = n_length_delete + str_temp.GetLength();
			continue;
		}

		if(str_Text_modify.IsEmpty())
		{
			n_lengthModify = 0;
		}
		else
		{
			if(n_line == 0)
			{
				n_lengthModify = str_Text_modify.GetLength();
			}
			else
			{
				n_lengthModify = str_Text_modify.GetLength() - n_length_delete - n_line;
			}

		}

		if(str_temp.IsEmpty())
		{
			n_lengthTemp = 0;
		}
		else
		{
			n_lengthTemp = str_temp.GetLength();
		}

		n_length_Total = n_lengthModify + n_lengthTemp;

		if(n_length_Total > 40)
		{
			n_line++;
			n_length_delete = n_length_delete + n_lengthModify;
			str_Text_modify = str_Text_modify + _T("\n");
			str_Text_modify = str_Text_modify + str_temp;
		}
		else
		{
			str_Text_modify = str_Text_modify + str_temp;
		}
	}
	m_cstrMsg = str_Text_modify;
}

void MsgDlg::SetErrorCode(CString& cstrMsg)
{
	m_cstrMsg_code = cstrMsg;

	//CString str_display = _T("");
	//str_display = cstrMsg;
	//if(cstrMsg.IsEmpty())
	//{
	//	return ;
	//}
	//else
	//{
	//	str_display.Trim();
	//}
	//int nLength = 0;
	//nLength = str_display.GetLength();
	//int n_spillt_black = 0;
	//int n_spillt = 0;
	//CString str_Text_modify = _T("");
	//if(nLength < 25)
	//{
	//	str_Text_modify = str_display;
	//}

	//int n_line = 0;
	//int n_lengthModify = 0;
	//int n_lengthTemp = 0;
	//int n_length_Total = 0;
	//int n_length_delete = 0;      //换行前的字符个数
	//CString str_temp = _T("");

	//while(str_display.GetLength() >= 25)
	//{
	//	n_spillt_black = str_display.Find(" ");
	//	n_spillt = str_display.Find("\\");
	//	if(n_spillt_black == 0 && n_spillt == 0)
	//	{

	//		break;
	//	}
	//	if(n_spillt_black <= 25 && n_spillt <= 25)
	//	{
	//		if(n_spillt_black >= n_spillt)
	//		{
	//			str_temp = str_display.Mid(0,n_spillt_black);
	//			str_display = str_display.Mid(n_spillt_black + 1);
	//		}
	//		else
	//		{
	//			str_temp = str_display.Mid(0,n_spillt);
	//			str_display = str_display.Mid(n_spillt + 1);
	//		}
	//	}

	//	if(str_Text_modify.IsEmpty())
	//	{
	//		n_lengthModify = 0;
	//	}
	//	else
	//	{
	//		if(n_line == 0)
	//		{
	//			n_lengthModify = str_Text_modify.GetLength();
	//		}
	//		else
	//		{
	//			n_lengthModify = str_Text_modify.GetLength() - n_length_delete - (2 * n_line);
	//		}

	//	}

	//	if(str_temp.IsEmpty())
	//	{
	//		n_lengthTemp = 0;
	//	}
	//	else
	//	{
	//		n_lengthTemp = str_temp.GetLength();
	//	}

	//	n_length_Total = n_lengthModify + n_lengthTemp;

	//	if(n_length_Total > 25)
	//	{
	//		n_line++;
	//		n_length_delete = n_length_delete + n_lengthModify;
	//		str_Text_modify = str_Text_modify + _T("\\n");
	//		str_Text_modify = str_Text_modify + str_temp;
	//	}
	//	else
	//	{
	//		str_Text_modify = str_Text_modify + str_temp;
	//	}
	//}
 //   m_cstrMsg_code = str_Text_modify;
	////SetWindowText(str_Text_modify);
}


HBRUSH MsgDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH m_brush = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	if(nCtlColor == CTLCOLOR_DLG)
	{
		DeleteHBRUSH();
		m_brush = CreateHBRUSH();
	}
	return m_brush;
}

void MsgDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
