// TouchPointDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../../../UnifyDLL_FTWireless.h"
#include "HeadsetBottonCountTestDlg.h"


IMPLEMENT_DYNAMIC(CButtonCountTestDlg, CDialog)

CButtonCountTestDlg::CButtonCountTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CButtonCountTestDlg::IDD, pParent)
{
	hOld = GetModuleHandle(NULL);
	hDll = GetModuleHandle(MMIBB_DLLNAME);
	AfxSetResourceHandle(hDll);
}

CButtonCountTestDlg::~CButtonCountTestDlg()
{
	DestroyWindow();
	CloseHandle(hDll);
}

void CButtonCountTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CButtonCountTestDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


void CButtonCountTestDlg::OnBnClickedOk()
{
	//OnOK();
}

// CTouchPointDlg message handlers
BOOL CButtonCountTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//set Touch Graph Dialog size
	CRect DlgRect(0, 0, m_iDlgWidth, m_iDlgHight);
	CWnd::SetWindowPos(NULL, 0, 0, DlgRect.Width(), DlgRect.Height(), SWP_NOZORDER | SWP_NOMOVE);

	SetWindowText(_T("headsetButton test"));
	Show_Test_TouchScreen();

	return TRUE;
}

CLabel* CButtonCountTestDlg::NewLabel(CString nText, CRect rect, int nStyle)
{
	CLabel* p_Label = new CLabel();
	p_Label->SetFontSize(26);
	p_Label->SetFontBold(1);
	p_Label->Create(nText, WS_CHILD | WS_VISIBLE | nStyle, rect, this);

	return p_Label;
}

bool CButtonCountTestDlg::Initial_Parameter(int PanelHPixel, int PanelVPixel, int BlockLenght, int BlockWidth, int DisplayRate)
{
	Rate = DisplayRate;

	TPanelHPixel = PanelHPixel;
	TPanelVPixel = PanelVPixel;
	Lenght = BlockLenght / DisplayRate;
	Width = BlockWidth / DisplayRate;

	H_Number = PanelHPixel / BlockLenght;
	V_Number = PanelVPixel / BlockWidth;

	if ((H_Number > TouchBlock_MAX_H_Number) || (V_Number > TouchBlock_MAX_V_Number))
	{
		::AfxMessageBox(_T("Touch Block numbers is greater than defined max numbers!"));
		return false;
	}

	StartPoint_X = 0;
	StartPoint_Y = 0;

	//加上Dialog Frame(6,33)
	m_iDlgWidth = H_Number * Lenght + 6;
	m_iDlgHight = V_Number * Width + 33;

	for (int i = 0; i < V_Number; i++)
	{
		for (int j = 0; j < H_Number; j++)
		{
			TouchScreen.p_TouchBlock[i][j] = NULL;
		}
	}

	return true;
}

bool CButtonCountTestDlg::Show_Test_TouchScreen()
{
	int i, j ;
	i = 5;
	j = H_Number - 2;

	TouchScreen.TouchBlock[i][j].x_Left = StartPoint_X + j * Lenght;
	TouchScreen.TouchBlock[i][j].y_Top = StartPoint_Y + i * Width;
	TouchScreen.TouchBlock[i][j].x_Right = TouchScreen.TouchBlock[i][j].x_Left + Lenght;
	TouchScreen.TouchBlock[i][j].y_Bottom = TouchScreen.TouchBlock[i][j].y_Top + Width;

	TouchScreen.TouchBlock[i][j].Status = false;

	TouchScreen.p_TouchBlock[i][j] = NewLabel(CString(L"请按耳机键"), CRect(TouchScreen.TouchBlock[i][j].x_Left, TouchScreen.TouchBlock[i][j].y_Top, TouchScreen.TouchBlock[i][j].x_Right, TouchScreen.TouchBlock[i][j].y_Bottom), SS_CENTER);
	TouchScreen.p_TouchBlock[i][j]->SetBorder(true);
	TouchScreen.p_TouchBlock[i][j]->SetBkColor(RGB(255, 0, 0)); //显示为红色

	TouchScreen.p_TouchBlock[i][j]->ShowWindow(true);

	return true;
}

void CButtonCountTestDlg::OnDestroy()
{
	CDialog::OnCancel();
}

void CButtonCountTestDlg::CDiagDestroyWindow()
{
	CDialog::DestroyWindow();
}

void CButtonCountTestDlg::FreeResource()
{
	for (int i = 0; i < V_Number; i++)
	{
		for (int j = 0; j < H_Number; j++)
		{
			if (TouchScreen.p_TouchBlock[i][j] != NULL)
			{
				delete TouchScreen.p_TouchBlock[i][j];
				TouchScreen.p_TouchBlock[i][j] = NULL;
			}
		}
	}
}
