// TouchCapkeyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UnifyDLL_FTWireless.h"
#include "TouchCapkeyDlg.h"


// CTouchCapkeyDlg dialog

IMPLEMENT_DYNAMIC(CTouchCapkeyDlg, CDialog)

CTouchCapkeyDlg::CTouchCapkeyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTouchCapkeyDlg::IDD, pParent)
{
	hOld=GetModuleHandle(NULL);
	hDll = GetModuleHandle(MMIBB_DLLNAME);
	AfxSetResourceHandle(hDll);
}

CTouchCapkeyDlg::~CTouchCapkeyDlg()
{
}

void CTouchCapkeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTouchCapkeyDlg, CDialog)
END_MESSAGE_MAP()


// CTouchCapkeyDlg message handlers
BOOL CTouchCapkeyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	AfxSetResourceHandle(hOld);

	//set Touch Graph Dialog size
	CRect DlgRect(0,0,m_iDlgWidth,m_iDlgHight);
	CWnd::SetWindowPos(NULL,0,0,DlgRect.Width(),DlgRect.Height(),SWP_NOZORDER|SWP_NOMOVE);

	/* Set window text */
	SetWindowText(_T("Capkey touch test"));
	Show_Test_TouchPoint();

	return TRUE;
}

CLabel *CTouchCapkeyDlg::NewLabel(CString nText,CRect rect,int nStyle)
{
	CLabel *p_Label = new CLabel();
	p_Label->Create(nText, WS_CHILD|WS_VISIBLE|nStyle, rect, this);
	return p_Label;
}

bool CTouchCapkeyDlg::Initial_Parameter(int PanelHPixel,int PanelVPixel,int BlockLenght,int BlockWidth,int DisplayRate)
{
	Rate = DisplayRate;

	TPanelHPixel = PanelHPixel;
	TPanelVPixel = PanelVPixel;
	Lenght = BlockLenght/DisplayRate;
	Width = BlockWidth/DisplayRate;

	H_Number = PanelHPixel/BlockLenght;
	V_Number = PanelVPixel/BlockWidth;

	StartPoint_X = 0;
	StartPoint_Y = 0;

	//加上Dialog Frame(6,33)
	m_iDlgWidth = H_Number*Lenght+6;
	m_iDlgHight = V_Number*Width+33;

	for(int i=0;i<V_Number;i++)
	{
		for(int j=0;j<H_Number;j++)
		{
			TouchCapkey.p_TouchBlock[i][j] = NULL;

		}
	}

	return true;
}

bool CTouchCapkeyDlg::Show_Test_TouchPoint()
{
	//动态创建Touch Test Block
	for(int i=0;i<V_Number;i++)
	{
		for(int j=0;j<H_Number;j++)
		{
			//TouchCapkey.p_TouchBlock[i][j] = new CLabel();

			TouchCapkey.TouchBlock[i][j].x_Left = StartPoint_X + j*Lenght;
			TouchCapkey.TouchBlock[i][j].y_Top = StartPoint_Y + i*Width;
			TouchCapkey.TouchBlock[i][j].x_Right = TouchCapkey.TouchBlock[i][j].x_Left + Lenght;
			TouchCapkey.TouchBlock[i][j].y_Bottom = TouchCapkey.TouchBlock[i][j].y_Top + Width;

			TouchCapkey.TouchBlock[i][j].Status = false;

			TouchCapkey.p_TouchBlock[i][j] = NewLabel(_T(""), CRect(TouchCapkey.TouchBlock[i][j].x_Left,TouchCapkey.TouchBlock[i][j].y_Top,TouchCapkey.TouchBlock[i][j].x_Right,TouchCapkey.TouchBlock[i][j].y_Bottom), SS_CENTER);
			TouchCapkey.p_TouchBlock[i][j]->SetBorder(true);
			TouchCapkey.p_TouchBlock[i][j]->SetBkColor(RGB(255,0,0)); //显示为红色
			if (i == V_Number - 1) {
				if (j == 1 || j == 3 || j == 5 || j == 7)
				{
					TouchCapkey.p_TouchBlock[i][j]->ShowWindow(true);
				}
				else
				{
					TouchCapkey.p_TouchBlock[i][j]->ShowWindow(false);
				}
			}
			else
			{
				TouchCapkey.p_TouchBlock[i][j]->ShowWindow(false);
			}
		}
	}

	return true;
}

bool CTouchCapkeyDlg::Check_TouchPoint_Position(char *p_chCapkeyID)
{
	int bInvalidNeed = false;

	for(int i=0;i<V_Number;i++)
	{
		for(int j=0;j<H_Number;j++)
		{
			if(i == (V_Number-1))//下
			{
				int nCapkeyId = atoi(p_chCapkeyID);
				if (nCapkeyId == 139)
				{
					if (j == 7) {
						if (!TouchCapkey.TouchBlock[i][j].Status)
						{
							bInvalidNeed = true;
							TouchCapkey.TouchBlock[i][j].Status = true;
							TouchCapkey.p_TouchBlock[i][j]->SetBkColor(RGB(0,255,0)); //显示为绿色
						}
					}
				}
				else if (nCapkeyId == 158)
				{
					if (j == 5) {
						if (!TouchCapkey.TouchBlock[i][j].Status)
						{
							bInvalidNeed = true;
							TouchCapkey.TouchBlock[i][j].Status = true;
							TouchCapkey.p_TouchBlock[i][j]->SetBkColor(RGB(0,255,0)); //显示为绿色
						}
					}
				}
				else if (nCapkeyId == 217)
				{
					if (j == 3) {
						if (!TouchCapkey.TouchBlock[i][j].Status)
						{
							bInvalidNeed = true;
							TouchCapkey.TouchBlock[i][j].Status = true;
							TouchCapkey.p_TouchBlock[i][j]->SetBkColor(RGB(0,255,0)); //显示为绿色
						}
					}
				}
				else if (nCapkeyId == 102)
				{
					if (j == 1) {
						if (!TouchCapkey.TouchBlock[i][j].Status)
						{
							bInvalidNeed = true;
							TouchCapkey.TouchBlock[i][j].Status = true;
							TouchCapkey.p_TouchBlock[i][j]->SetBkColor(RGB(0,255,0)); //显示为绿色
						}
					}
				}
			}
		}
	}

	if(bInvalidNeed)
		Invalidate(false);

	return true;
}

bool CTouchCapkeyDlg::Check_Touch_Result()
{
	//判断测试的区域是否全部触碰过(由外圈到内圈)
	if(TouchCapkey.TouchBlock[V_Number-1][1].Status == true &&
		TouchCapkey.TouchBlock[V_Number-1][3].Status == true &&
		TouchCapkey.TouchBlock[V_Number-1][5].Status == true &&
		TouchCapkey.TouchBlock[V_Number-1][7].Status == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CTouchCapkeyDlg::OnDestroy()
{
	CDialog::OnCancel();
}

void CTouchCapkeyDlg::FreeResource()
{
	for(int i=0;i<V_Number;i++)
	{
		for(int j=0;j<H_Number;j++)
		{
			if(TouchCapkey.p_TouchBlock[i][j] != NULL)
				delete TouchCapkey.p_TouchBlock[i][j];
		}
	}
}
