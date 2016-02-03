// TouchLineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UnifyDLL_FTWireless.h"
#include "TouchLineDlg.h"


// CTouchLineDlg dialog

IMPLEMENT_DYNAMIC(CTouchLineDlg, CDialog)

CTouchLineDlg::CTouchLineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTouchLineDlg::IDD, pParent)
{
	hOld = GetModuleHandle(NULL);
	hDll = GetModuleHandle(MMIBB_DLLNAME);
	AfxSetResourceHandle(hDll);
}

CTouchLineDlg::~CTouchLineDlg()
{

}

void CTouchLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTouchLineDlg, CDialog)
END_MESSAGE_MAP()


// CTouchLineDlg message handlers
BOOL CTouchLineDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	AfxSetResourceHandle(hOld);

	//set Touch Graph Dialog size
	CRect DlgRect(0, 0, m_iDlgWidth, m_iDlgHight);
	CWnd::SetWindowPos(NULL, 0, 0, DlgRect.Width(), DlgRect.Height(), SWP_NOZORDER | SWP_NOMOVE);

	SetWindowText(_T("Linearity test"));
	Show_Test_TouchPoint();

	return TRUE;
}

CLabel* CTouchLineDlg::NewLabel(CString nText, CRect rect, int nStyle)
{
	CLabel* p_Label = new CLabel();
	p_Label->Create(nText, WS_CHILD | WS_VISIBLE | nStyle, rect, this);
	return p_Label;
}

bool CTouchLineDlg::Initial_Parameter(int PanelHPixel, int PanelVPixel, int BlockLenght, int BlockWidth, int DisplayRate)
{
	LineCount = 2;//2条交叉线
	Rate = DisplayRate;

	TPanelHPixel = PanelHPixel;
	TPanelVPixel = PanelVPixel;

	//Lenght = BlockLenght*PanelHPixel/PanelVPixel/DisplayRate;
	//Width = BlockWidth/DisplayRate;
	Lenght = BlockLenght / DisplayRate;
	Width = BlockWidth * PanelVPixel / PanelHPixel / DisplayRate;


	H_Number = PanelHPixel / BlockLenght;
	V_Number = PanelVPixel / BlockWidth;

	if ((H_Number > TouchBlock_MAX_H_Number) || (V_Number > TouchBlock_MAX_V_Number))
	{
		::AfxMessageBox(_T("Touch Block numbers is greater than defined max numbers!"));
		return false;
	}

	if (H_Number > V_Number)
		H_Number = V_Number;
	else
		V_Number = H_Number;

	for (int i = 0; i < LineCount; i++)
	{
		if (i == 0)
		{
			StartPoint_X[i] = 0;
			StartPoint_Y[i] = 0;
		}
		else if (i == 1)
		{
			StartPoint_X[i] = H_Number * Lenght - Lenght;
			StartPoint_Y[i] = 0;
		}
	}

	//加上Dialog Frame(6,33)
	m_iDlgWidth = H_Number * Lenght + 6;
	m_iDlgHight = V_Number * Width + 33;

	for (int i = 0; i < V_Number; i++)
	{
		for (int j = 0; j < H_Number; j++)
		{
			if (i == j) //显示Line 1
			{
				TouchLine[0].p_TouchBlock[i][j] = NULL;
			}
			if (i == (H_Number - j - 1)) //显示Line 2
			{
				TouchLine[1].p_TouchBlock[i][j] = NULL;
			}
		}
	}
	return true;
}

bool CTouchLineDlg::Show_Test_TouchPoint()
{
	//动态创建Touch Test Block
	for (int i = 0; i < V_Number; i++)
	{
		for (int j = 0; j < H_Number; j++)
		{
			if (i == j) //显示Line 1
			{
				//TouchLine[0].p_TouchBlock[i][j] = new CLabel();
				if (j == 0)
				{
					TouchLine[0].TouchBlock[i][j].x_Left = StartPoint_X[0];
					TouchLine[0].TouchBlock[i][j].y_Top = StartPoint_Y[0];
				}
				else
				{
					TouchLine[0].TouchBlock[i][j].x_Left = TouchLine[0].TouchBlock[i - 1][j - 1].x_Right;
					TouchLine[0].TouchBlock[i][j].y_Top = TouchLine[0].TouchBlock[i - 1][j - 1].y_Bottom;
				}
				TouchLine[0].TouchBlock[i][j].x_Right = TouchLine[0].TouchBlock[i][j].x_Left + Lenght;
				TouchLine[0].TouchBlock[i][j].y_Bottom = TouchLine[0].TouchBlock[i][j].y_Top + Width;

				TouchLine[0].TouchBlock[i][j].Status = false;

				TouchLine[0].p_TouchBlock[i][j] = NewLabel(_T(""), CRect(TouchLine[0].TouchBlock[i][j].x_Left, TouchLine[0].TouchBlock[i][j].y_Top, TouchLine[0].TouchBlock[i][j].x_Right, TouchLine[0].TouchBlock[i][j].y_Bottom), SS_CENTER);
				TouchLine[0].p_TouchBlock[i][j]->SetBorder(true);
				TouchLine[0].p_TouchBlock[i][j]->SetBkColor(RGB(255, 0, 0)); //显示为红色
			}
			if (i == (H_Number - j - 1)) //显示Line 2
			{
				//TouchLine[1].p_TouchBlock[i][j] = new CLabel();
				if (j == H_Number - 1)
				{
					TouchLine[1].TouchBlock[i][j].x_Left = StartPoint_X[1];
					TouchLine[1].TouchBlock[i][j].y_Top = StartPoint_Y[1];
				}
				else
				{
					TouchLine[1].TouchBlock[i][j].x_Left = TouchLine[1].TouchBlock[i - 1][j + 1].x_Left - Lenght;
					TouchLine[1].TouchBlock[i][j].y_Top = TouchLine[1].TouchBlock[i - 1][j + 1].y_Bottom;
				}
				TouchLine[1].TouchBlock[i][j].x_Right = TouchLine[1].TouchBlock[i][j].x_Left + Lenght;
				TouchLine[1].TouchBlock[i][j].y_Bottom = TouchLine[1].TouchBlock[i][j].y_Top + Width;

				TouchLine[1].TouchBlock[i][j].Status = false;

				TouchLine[1].p_TouchBlock[i][j] = NewLabel(_T(""), CRect(TouchLine[1].TouchBlock[i][j].x_Left, TouchLine[1].TouchBlock[i][j].y_Top, TouchLine[1].TouchBlock[i][j].x_Right, TouchLine[1].TouchBlock[i][j].y_Bottom), SS_CENTER);
				TouchLine[1].p_TouchBlock[i][j]->SetBorder(true);
				TouchLine[1].p_TouchBlock[i][j]->SetBkColor(RGB(255, 0, 0)); //显示为红色

			}
		}
	}

	return true;
}

bool CTouchLineDlg::Check_TouchPoint_Position(char* p_chPointCoord)
{
	int count;
	StrVtr vtrstr;
	String strPointCoord = CString(p_chPointCoord);
	vtrstr.clear();
	CStr::ParseString(strPointCoord.c_str(), _T(","), vtrstr);
	if (vtrstr.size() % 2 != 0)
	{
		::AfxMessageBox(_T("Get Point Coord from target is not correct number!"));
		return false;
	}

	if (vtrstr.size() / 2 != TouchPoint_MAX_Number)
	{
		::AfxMessageBox(_T("Get Point Coord from target is large than defined max number!"));
		return false;
	}

	int bInvalidNeed = false;

	for (count = 0; count < (int)vtrstr.size() / 2; count++)
	{
		//截取X,Y的坐标值
		//std::string strTempX(vtrstr[0+2*count],2,4);//去除X:
		//std::string strTempY(vtrstr[1+2*count],2,4);//去除Y:

		//乘上TouchPanel的比率:
		//LA: 1024X1024/1280X800
		//Orange: 800X480/800X480
		//PointCoord[count].x = CStr::StrToInt(strTempX)*TPanelHPixel/1024;
		//PointCoord[count].y = CStr::StrToInt(strTempY)*TPanelVPixel/1024;

		//PointCoord[count].x = CStr::StrToInt(strTempX);
		//PointCoord[count].y = CStr::StrToInt(strTempY);
		PointCoord[count].x = CStr::StrToInt(vtrstr[0 + 2 * count]);
		PointCoord[count].y = CStr::StrToInt(vtrstr[1 + 2 * count]);

		//判断Point的位置，动态更新显示
		for (int i = 0; i < V_Number; i++)
		{
			for (int j = 0; j < H_Number; j++)
			{
				if (i == j) //Line 1
				{
					if ((PointCoord[count].x > TouchLine[0].TouchBlock[i][j].x_Left * Rate) && (PointCoord[count].x < TouchLine[0].TouchBlock[i][j].x_Right * Rate)
					    && (PointCoord[count].y > TouchLine[0].TouchBlock[i][j].y_Top * Rate) && (PointCoord[count].y < TouchLine[0].TouchBlock[i][j].y_Bottom * Rate))
					{
						if (!TouchLine[0].TouchBlock[i][j].Status)
						{
							bInvalidNeed = true;
							TouchLine[0].TouchBlock[i][j].Status = true;
							TouchLine[0].p_TouchBlock[i][j]->SetBkColor(RGB(0, 255, 0)); //显示为绿色
						}
					}
				}
				if (i == (H_Number - j - 1)) //Line 2
				{
					if ((PointCoord[count].x > TouchLine[1].TouchBlock[i][j].x_Left * Rate) && (PointCoord[count].x < TouchLine[1].TouchBlock[i][j].x_Right * Rate)
					    && (PointCoord[count].y > TouchLine[1].TouchBlock[i][j].y_Top * Rate) && (PointCoord[count].y < TouchLine[1].TouchBlock[i][j].y_Bottom * Rate))
					{
						if (!TouchLine[1].TouchBlock[i][j].Status)
						{
							bInvalidNeed = true;
							TouchLine[1].TouchBlock[i][j].Status = true;
							TouchLine[1].p_TouchBlock[i][j]->SetBkColor(RGB(0, 255, 0)); //显示为绿色
						}
					}
				}
			}
		}
	}

	if (bInvalidNeed)
		Invalidate(false);//false->避免出现刷屏
	return true;
}

bool CTouchLineDlg::Check_Touch_Result()
{
	//判断测试的区域是否全部触碰过
	for (int i = 0; i < V_Number; i++)
	{
		for (int j = 0; j < H_Number; j++)
		{
			if (i == j) //Line 1
			{
				if (TouchLine[0].TouchBlock[i][j].Status == true)
					continue;
				else
					return false;
			}
			if (i == (H_Number - j - 1)) //Line 2
			{
				if (TouchLine[1].TouchBlock[i][j].Status == true)
					continue;
				else
					return false;
			}
		}
	}

	return true;
}

void CTouchLineDlg::OnDestroy()
{
	CDialog::OnCancel();
}

void CTouchLineDlg::FreeResource()
{
	for (int i = 0; i < V_Number; i++)
	{
		for (int j = 0; j < H_Number; j++)
		{
			if (i == j) //显示Line 1
			{
				if (TouchLine[0].p_TouchBlock[i][j] != NULL)
					delete TouchLine[0].p_TouchBlock[i][j];
			}
			if (i == (H_Number - j - 1)) //显示Line 2
			{
				if (TouchLine[1].p_TouchBlock[i][j] != NULL)
					delete TouchLine[1].p_TouchBlock[i][j];
			}
		}
	}
}
