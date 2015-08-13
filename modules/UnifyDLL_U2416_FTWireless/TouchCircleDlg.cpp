// TouchCircleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UnifyDLL_FTWireless.h"
#include "TouchCircleDlg.h"


// CTouchCircleDlg dialog

IMPLEMENT_DYNAMIC(CTouchCircleDlg, CDialog)

CTouchCircleDlg::CTouchCircleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTouchCircleDlg::IDD, pParent)
{
	hOld = GetModuleHandle(NULL);
	hDll = GetModuleHandle(MMIBB_DLLNAME);
	AfxSetResourceHandle(hDll);
}

CTouchCircleDlg::~CTouchCircleDlg()
{
}

void CTouchCircleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTouchCircleDlg, CDialog)
END_MESSAGE_MAP()


// CTouchCircleDlg message handlers
BOOL CTouchCircleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	AfxSetResourceHandle(hOld);

	//set Touch Graph Dialog size
	CRect DlgRect(0, 0, m_iDlgWidth, m_iDlgHight);
	CWnd::SetWindowPos(NULL, 0, 0, DlgRect.Width(), DlgRect.Height(), SWP_NOZORDER | SWP_NOMOVE);

	/* Set window text */
	SetWindowText(_T("Circle touch test"));

	Show_Test_TouchPoint();

	return TRUE;
}

CLabel* CTouchCircleDlg::NewLabel(CString nText, CRect rect, int nStyle)
{
	CLabel* p_Label = new CLabel();
	p_Label->Create(nText, WS_CHILD | WS_VISIBLE | nStyle, rect, this);
	return p_Label;
}

bool CTouchCircleDlg::Initial_Parameter(int PanelHPixel, int PanelVPixel, int BlockLenght, int BlockWidth, int DisplayRate, int Count, int CircleDistance)
{
	CircleCount = Count;
	Distance = CircleDistance;
	Rate = DisplayRate;

	TPanelHPixel = PanelHPixel;
	TPanelVPixel = PanelVPixel;
	Lenght = BlockLenght / DisplayRate;
	Width = BlockWidth / DisplayRate;

	if (CircleCount > TouchCircle_MAX_Number)
	{
		::AfxMessageBox(_T("Touch Circle Count from XML file is greater than defined max number!"));
		return false;
	}

	for (int i = 0; i < CircleCount; i++)
	{
		StartPoint_X[i] = 0 + Distance * i / Rate;
		StartPoint_Y[i] = 0 + Distance * i / Rate;

		H_Number[i] = (PanelHPixel - Distance * i * 2) / BlockLenght;
		V_Number[i] = (PanelVPixel - Distance * i * 2) / BlockWidth;

		if ((H_Number[i] > TouchBlock_MAX_H_Number) || (V_Number[i] > TouchBlock_MAX_V_Number))
		{
			::AfxMessageBox(_T("Touch Block numbers is greater than defined max numbers!"));
			return false;
		}
	}

	//加上Dialog Frame(6,33)
	m_iDlgWidth = H_Number[0] * Lenght + 6;
	m_iDlgHight = V_Number[0] * Width + 33;

	//初始化指针
	for (int count = 0; count < CircleCount; count++)
	{
		for (int i = 0; i < V_Number[count]; i++)
		{
			for (int j = 0; j < H_Number[count]; j++)
			{
				if (i == 0) //最上面一行
				{
					TouchCircle[count].p_TouchBlock[i][j] = NULL;
				}
				if (j == 0 && i != 0 && i != V_Number[count] - 1) //最左边一行
				{
					TouchCircle[count].p_TouchBlock[i][j] = NULL;
				}
				if (j == H_Number[count] - 1 && i != 0 && i != V_Number[count] - 1) //最右边一行
				{
					TouchCircle[count].p_TouchBlock[i][j] = NULL;
				}
				if (i == V_Number[count] - 1) //最下面一行
				{
					TouchCircle[count].p_TouchBlock[i][j] = NULL;
				}
			}
		}
	}

	return true;
}

bool CTouchCircleDlg::Show_Test_TouchPoint()
{
	//动态创建Touch Test Block(由外圈到内圈)
	for (int count = 0; count < CircleCount; count++)
	{
		for (int i = 0; i < V_Number[count]; i++)
		{
			for (int j = 0; j < H_Number[count]; j++)
			{
				if (i == 0) //显示最上面一行
				{
					//TouchCircle[count].p_TouchBlock[i][j] = new CLabel();
					if (j == 0)
						TouchCircle[count].TouchBlock[i][j].x_Left = StartPoint_X[count];
					else
						TouchCircle[count].TouchBlock[i][j].x_Left = TouchCircle[count].TouchBlock[i][j - 1].x_Right;
					TouchCircle[count].TouchBlock[i][j].y_Top = StartPoint_Y[count];
					TouchCircle[count].TouchBlock[i][j].x_Right = TouchCircle[count].TouchBlock[i][j].x_Left + Lenght;
					TouchCircle[count].TouchBlock[i][j].y_Bottom = TouchCircle[count].TouchBlock[i][j].y_Top + Width;

					TouchCircle[count].TouchBlock[i][j].Status = false;

					TouchCircle[count].p_TouchBlock[i][j] = NewLabel(_T(""), CRect(TouchCircle[count].TouchBlock[i][j].x_Left, TouchCircle[count].TouchBlock[i][j].y_Top, TouchCircle[count].TouchBlock[i][j].x_Right, TouchCircle[count].TouchBlock[i][j].y_Bottom), SS_CENTER);
					TouchCircle[count].p_TouchBlock[i][j]->SetBorder(true);
					TouchCircle[count].p_TouchBlock[i][j]->SetBkColor(RGB(255, 0, 0)); //显示为红色
				}
				if (j == 0 && i != 0 && i != V_Number[count] - 1) //显示最左边一行
				{
					//TouchCircle[count].p_TouchBlock[i][j] = new CLabel();

					TouchCircle[count].TouchBlock[i][j].x_Left = StartPoint_X[count];
					TouchCircle[count].TouchBlock[i][j].y_Top = TouchCircle[count].TouchBlock[i - 1][j].y_Bottom;
					TouchCircle[count].TouchBlock[i][j].x_Right = TouchCircle[count].TouchBlock[i][j].x_Left + Width;
					TouchCircle[count].TouchBlock[i][j].y_Bottom = TouchCircle[count].TouchBlock[i][j].y_Top + Lenght;

					TouchCircle[count].TouchBlock[i][j].Status = false;

					TouchCircle[count].p_TouchBlock[i][j] = NewLabel(_T(""), CRect(TouchCircle[count].TouchBlock[i][j].x_Left, TouchCircle[count].TouchBlock[i][j].y_Top, TouchCircle[count].TouchBlock[i][j].x_Right, TouchCircle[count].TouchBlock[i][j].y_Bottom), SS_CENTER);
					TouchCircle[count].p_TouchBlock[i][j]->SetBorder(true);
					TouchCircle[count].p_TouchBlock[i][j]->SetBkColor(RGB(255, 0, 0)); //显示为红色
				}
				if (j == H_Number[count] - 1 && i != 0 && i != V_Number[count] - 1) //显示最右边一行
				{
					//TouchCircle[count].p_TouchBlock[i][j] = new CLabel();

					TouchCircle[count].TouchBlock[i][j].x_Left = TouchCircle[count].TouchBlock[i - 1][j].x_Left;
					TouchCircle[count].TouchBlock[i][j].y_Top = TouchCircle[count].TouchBlock[i - 1][j].y_Bottom;
					TouchCircle[count].TouchBlock[i][j].x_Right = TouchCircle[count].TouchBlock[i][j].x_Left + Width;
					TouchCircle[count].TouchBlock[i][j].y_Bottom = TouchCircle[count].TouchBlock[i][j].y_Top + Lenght;

					TouchCircle[count].TouchBlock[i][j].Status = false;

					TouchCircle[count].p_TouchBlock[i][j] = NewLabel(_T(""), CRect(TouchCircle[count].TouchBlock[i][j].x_Left, TouchCircle[count].TouchBlock[i][j].y_Top, TouchCircle[count].TouchBlock[i][j].x_Right, TouchCircle[count].TouchBlock[i][j].y_Bottom), SS_CENTER);
					TouchCircle[count].p_TouchBlock[i][j]->SetBorder(true);
					TouchCircle[count].p_TouchBlock[i][j]->SetBkColor(RGB(255, 0, 0)); //显示为红色
				}
				if (i == V_Number[count] - 1) //显示最下面一行
				{
					//TouchCircle[count].p_TouchBlock[i][j] = new CLabel();

					if (j == 0)
					{
						TouchCircle[count].TouchBlock[i][j].x_Left = StartPoint_X[count];
						TouchCircle[count].TouchBlock[i][j].y_Top = TouchCircle[count].TouchBlock[i - 1][j].y_Bottom;
					}
					else
					{
						TouchCircle[count].TouchBlock[i][j].x_Left = TouchCircle[count].TouchBlock[i][j - 1].x_Right;
						TouchCircle[count].TouchBlock[i][j].y_Top = TouchCircle[count].TouchBlock[i][j - 1].y_Top;
					}
					TouchCircle[count].TouchBlock[i][j].x_Right = TouchCircle[count].TouchBlock[i][j].x_Left + Lenght;
					TouchCircle[count].TouchBlock[i][j].y_Bottom = TouchCircle[count].TouchBlock[i][j].y_Top + Width;

					TouchCircle[count].TouchBlock[i][j].Status = false;

					TouchCircle[count].p_TouchBlock[i][j] = NewLabel(_T(""), CRect(TouchCircle[count].TouchBlock[i][j].x_Left, TouchCircle[count].TouchBlock[i][j].y_Top, TouchCircle[count].TouchBlock[i][j].x_Right, TouchCircle[count].TouchBlock[i][j].y_Bottom), SS_CENTER);
					TouchCircle[count].p_TouchBlock[i][j]->SetBorder(true);
					TouchCircle[count].p_TouchBlock[i][j]->SetBkColor(RGB(255, 0, 0)); //显示为红色
				}
			}
		}
	}

	return true;
}

bool CTouchCircleDlg::Check_TouchPoint_Position(char* p_chPointCoord)
{
	StrVtr vtrstr;
	String strPointCoord = CString(p_chPointCoord);
	vtrstr.clear();
	CStr::ParseString(strPointCoord.c_str(), _T(","), vtrstr);
	if (vtrstr.size() % 2 != 0)
	{
		::AfxMessageBox(_T("Get Point Coord from target is not correct number!"));
		return false;
	}

	if (vtrstr.size() / 2 > TouchPoint_MAX_Number)
	{
		::AfxMessageBox(_T("Get Point Coord from target is large than defined max number!"));
		return false;
	}

	int bInvalidNeed = false;

	//判断Point的位置(由外圈到内圈)，动态更新显示
	for (int PointCount = 0; PointCount < (int)vtrstr.size() / 2; PointCount++)
	{
		//截取X,Y的坐标值
		//std::string strTempX(vtrstr[0+2*PointCount],2,4);//除X:
		//std::string strTempY(vtrstr[1+2*PointCount],2,4);//除Y:

		//乘上TouchPanel的比率
		//LA:1024X1024/1280X800
		//Orange:800X480/800X480
		//PointCoord[PointCount].x = CStr::StrToInt(strTempX)*TPanelHPixel/1024;
		//PointCoord[PointCount].y = CStr::StrToInt(strTempY)*TPanelVPixel/1024;

		PointCoord[PointCount].x = CStr::StrToInt(vtrstr[0 + 2 * PointCount]);
		PointCoord[PointCount].y = CStr::StrToInt(vtrstr[1 + 2 * PointCount]);
		TRACE("Check Point => X: %d Y: %d", PointCoord[PointCount].x, PointCoord[PointCount].y);

		for (int count = 0; count < CircleCount; count++)
		{
			for (int i = 0; i < V_Number[count]; i++)
			{
				for (int j = 0; j < H_Number[count]; j++)
				{
					if (i == 0 || i == (V_Number[count] - 1) || j == 0 || j == (H_Number[count] - 1)) //上、下、左、右
					{
						if ((PointCoord[PointCount].x > TouchCircle[count].TouchBlock[i][j].x_Left * Rate) && (PointCoord[PointCount].x < TouchCircle[count].TouchBlock[i][j].x_Right * Rate)
						    && (PointCoord[PointCount].y > TouchCircle[count].TouchBlock[i][j].y_Top * Rate) && (PointCoord[PointCount].y < TouchCircle[count].TouchBlock[i][j].y_Bottom * Rate))
						{
							if (!TouchCircle[count].TouchBlock[i][j].Status)
							{
								bInvalidNeed = true;
								TouchCircle[count].TouchBlock[i][j].Status = true;
								TouchCircle[count].p_TouchBlock[i][j]->SetBkColor(RGB(0, 255, 0)); //显示为绿色
							}
						}
					}
				}
			}
		}
	}

	if (bInvalidNeed)
		Invalidate(false);//false->刷

	return true;
}

bool CTouchCircleDlg::Check_Touch_Result()
{
	//判断测试的区域是否全部触碰过(由外圈到内圈)
	for (int count = 0; count < CircleCount; count++)
	{
		for (int i = 0; i < V_Number[count]; i++)
		{
			for (int j = 0; j < H_Number[count]; j++)
			{
				if (i == 0 || (i == V_Number[count] - 1) || j == 0 || (j == H_Number[count] - 1)) //上、下、左、右
				{
					if (TouchCircle[count].TouchBlock[i][j].Status == true)
						continue;
					else
						return false;
				}
			}
		}
	}

	return true;
}

void CTouchCircleDlg::OnDestroy()
{
	CDialog::OnCancel();
}

void CTouchCircleDlg::FreeResource()
{
	for (int count = 0; count < CircleCount; count++)
	{
		for (int i = 0; i < V_Number[count]; i++)
		{
			for (int j = 0; j < H_Number[count]; j++)
			{
				if (i == 0) //最上面一行
				{
					if (TouchCircle[count].p_TouchBlock[i][j] != NULL)
					{
						delete TouchCircle[count].p_TouchBlock[i][j];
						TouchCircle[count].p_TouchBlock[i][j] = NULL;
					}
				}
				if (j == 0 && i != 0 && i != V_Number[count] - 1) //最左边一行
				{
					if (TouchCircle[count].p_TouchBlock[i][j] != NULL)
					{
						delete TouchCircle[count].p_TouchBlock[i][j];
						TouchCircle[count].p_TouchBlock[i][j] = NULL;
					}
				}
				if (j == H_Number[count] - 1 && i != 0 && i != V_Number[count] - 1) //最右边一行
				{
					if (TouchCircle[count].p_TouchBlock[i][j] != NULL)
					{
						delete TouchCircle[count].p_TouchBlock[i][j];
						TouchCircle[count].p_TouchBlock[i][j] = NULL;
					}
				}
				if (i == V_Number[count] - 1) //最下面一行
				{
					if (TouchCircle[count].p_TouchBlock[i][j] != NULL)
					{
						delete TouchCircle[count].p_TouchBlock[i][j];
						TouchCircle[count].p_TouchBlock[i][j] = NULL;
					}
				}
			}
		}
	}
}
