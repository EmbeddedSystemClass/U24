// TouchGraphDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UnifyDLL_FTWireless.h"
#include "TouchGraphDlg.h"

// CTouchGraphDlg dialog

IMPLEMENT_DYNAMIC(CTouchGraphDlg, CDialog)

CTouchGraphDlg::CTouchGraphDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTouchGraphDlg::IDD, pParent)
{
	hOld = GetModuleHandle(NULL);
	hDll = GetModuleHandle(MMIBB_DLLNAME);
	AfxSetResourceHandle(hDll);
}

CTouchGraphDlg::~CTouchGraphDlg()
{

}

void CTouchGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTouchGraphDlg, CDialog)
END_MESSAGE_MAP()


// CTouchGraphDlg message handlers
BOOL CTouchGraphDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	AfxSetResourceHandle(hOld);

	//set Touch Graph Dialog size
	CRect DlgRect(0, 0, m_iDlgWidth, m_iDlgHight);
	CWnd::SetWindowPos(NULL, 0, 0, DlgRect.Width(), DlgRect.Height(), SWP_NOZORDER | SWP_NOMOVE);

	SetWindowText(_T("Multi-touch test"));
	Create_Test_TouchPoint(1);//show pair:all;Line:all

	return TRUE;
}

CLabel* CTouchGraphDlg::NewLabel(CString nText, CRect rect, int nStyle)
{
	CLabel* p_Label = new CLabel();
	p_Label->Create(nText, WS_CHILD | WS_VISIBLE | nStyle, rect, this);
	return p_Label;
}

bool CTouchGraphDlg::Initial_Parameter(int PanelHPixel, int PanelVPixel, int BlockLenght, int BlockWidth, int DisplayRate, int LineDistance)
{
	Distance = LineDistance;
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

	//Pair1,Line 1
	StartPoint_X[0][0] = 0;
	StartPoint_Y[0][0] = 0;

	//Pair1,Line 2
	StartPoint_X[0][1] = 0;
	StartPoint_Y[0][1] = 0 + (LineDistance + BlockWidth) / Rate;

	//Pair2,Line 1
	StartPoint_X[1][0] = (PanelHPixel - LineDistance - 2 * BlockLenght) / Rate;
	StartPoint_Y[1][0] = 0;

	//Pair2,Line 2
	StartPoint_X[1][1] = (PanelHPixel - BlockLenght) / Rate;
	StartPoint_Y[1][1] = 0;

	//Pair3,Line 1
	StartPoint_X[2][0] = 0;
	StartPoint_Y[2][0] = (PanelVPixel - LineDistance - 2 * BlockWidth) / Rate;

	//Pair3,Line 2
	StartPoint_X[2][1] = 0;
	StartPoint_Y[2][1] = (PanelVPixel - BlockWidth) / Rate;

	//Pair4,Line 1
	StartPoint_X[3][0] = 0;
	StartPoint_Y[3][0] = 0;

	//Pair4,Line 2
	StartPoint_X[3][1] = 0 + (LineDistance + 2 * BlockLenght) / Rate;
	StartPoint_Y[3][1] = 0;

	//加上Dialog Frame(6,33)
	m_iDlgWidth = H_Number * Lenght + 6;
	m_iDlgHight = V_Number * Width + 33;

	for (int i = 0; i < TouchLine_Pair_MAX_Number; i++)
		for (int j = 0; j < EveryPair_Line_MAX_Number; j++)
		{
			if ((i + 1) % 2 != 0) //Pair 1、3;Line 1-2
			{
				for (int m = 0; m < H_Number; m++)
				{
					TouchLine[i][j].p_TouchBlock[m] = NULL;
				}

			}
			else//Pair 2、4;Line 1-2
			{
				for (int n = 0; n < V_Number; n++)
				{
					TouchLine[i][j].p_TouchBlock[n] = NULL;
				}
			}
		}

	return true;
}

/********************************************/
//ShowPairIndex
//0:Pair:all;Line:all
//1:Pair:1;Line:1-2
//2:Pair:2;Line:1-2
//3:Pair:3;Line:1-2
//4:Pair:4;Line:1-2
/********************************************/
bool CTouchGraphDlg::Create_Test_TouchPoint(int ShowPairIndex)
{
	//int i = 0;
	//int j = 0;

	//动态创建Touch Test Block(Pair 1-4;Line 1-2)
	for (int i = 0; i < TouchLine_Pair_MAX_Number; i++)
		for (int j = 0; j < EveryPair_Line_MAX_Number; j++)
		{
			if ((i + 1) % 2 != 0) //显示Pair 1、3;Line 1-2
			{
				for (int m = 0; m < H_Number; m++)
				{
					//TouchLine[i][j].p_TouchBlock[m] = new CLabel();

					TouchLine[i][j].TouchBlock[m].x_Left = StartPoint_X[i][j] + m * Lenght;
					TouchLine[i][j].TouchBlock[m].y_Top = StartPoint_Y[i][j];
					TouchLine[i][j].TouchBlock[m].x_Right = TouchLine[i][j].TouchBlock[m].x_Left + Lenght;
					TouchLine[i][j].TouchBlock[m].y_Bottom = TouchLine[i][j].TouchBlock[m].y_Top + Width;

					TouchLine[i][j].TouchBlock[m].Status = false;

					TouchLine[i][j].p_TouchBlock[m] = NewLabel(_T(""), CRect(TouchLine[i][j].TouchBlock[m].x_Left, TouchLine[i][j].TouchBlock[m].y_Top, TouchLine[i][j].TouchBlock[m].x_Right, TouchLine[i][j].TouchBlock[m].y_Bottom), SS_CENTER);
					TouchLine[i][j].p_TouchBlock[m]->SetBorder(true);
					TouchLine[i][j].p_TouchBlock[m]->SetBkColor(RGB(255, 0, 0)); //显示为红色

					if ((i + 1) == ShowPairIndex || ShowPairIndex == 0)
						TouchLine[i][j].p_TouchBlock[m]->ShowWindow(true);
					else
						TouchLine[i][j].p_TouchBlock[m]->ShowWindow(false);
				}
			}
			else//显示Pair 2、4;Line 1-2
			{
				for (int n = 0; n < V_Number; n++)
				{
					//TouchLine[i][j].p_TouchBlock[n] = new CLabel();

					TouchLine[i][j].TouchBlock[n].x_Left = StartPoint_X[i][j];
					TouchLine[i][j].TouchBlock[n].y_Top = StartPoint_Y[i][j] + n * Width;
					TouchLine[i][j].TouchBlock[n].x_Right = TouchLine[i][j].TouchBlock[n].x_Left + Lenght;
					TouchLine[i][j].TouchBlock[n].y_Bottom = TouchLine[i][j].TouchBlock[n].y_Top + Width;

					TouchLine[i][j].TouchBlock[n].Status = false;

					TouchLine[i][j].p_TouchBlock[n] = NewLabel(_T(""), CRect(TouchLine[i][j].TouchBlock[n].x_Left, TouchLine[i][j].TouchBlock[n].y_Top, TouchLine[i][j].TouchBlock[n].x_Right, TouchLine[i][j].TouchBlock[n].y_Bottom), SS_CENTER);
					TouchLine[i][j].p_TouchBlock[n]->SetBorder(true);
					TouchLine[i][j].p_TouchBlock[n]->SetBkColor(RGB(255, 0, 0)); //显示为红色

					if ((i + 1) == ShowPairIndex || ShowPairIndex == 0)
						TouchLine[i][j].p_TouchBlock[n]->ShowWindow(true);
					else
						TouchLine[i][j].p_TouchBlock[n]->ShowWindow(false);
				}
			}
		}

	return true;
}

/********************************************/
//ShowPairIndex
//0:Pair:all;Line:all
//1:Pair:1;Line:1-2
//2:Pair:2;Line:1-2
//3:Pair:3;Line:1-2
//4:Pair:4;Line:1-2
/********************************************/
bool CTouchGraphDlg::Show_Test_TouchLine(int ShowPairIndex)
{
	//显示Touch Line
	for (int i = 0; i < TouchLine_Pair_MAX_Number; i++)
		for (int j = 0; j < EveryPair_Line_MAX_Number; j++)
		{
			if ((i + 1) % 2 != 0) //显示Pair 1、3;Line 1-2
			{
				for (int m = 0; m < H_Number; m++)
				{
					if ((i + 1) == ShowPairIndex || ShowPairIndex == 0)
						TouchLine[i][j].p_TouchBlock[m]->ShowWindow(true);
				}

			}
			else//显示Pair 2、4;Line 1-2
			{
				for (int n = 0; n < V_Number; n++)
				{
					if ((i + 1) == ShowPairIndex || ShowPairIndex == 0)
						TouchLine[i][j].p_TouchBlock[n]->ShowWindow(true);
				}
			}
		}

	return true;

}

bool CTouchGraphDlg::Check_TouchPoint_Position(char* p_chPointCoord, int PairIndex)
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
		String strTempX(vtrstr[0 + 2 * count], 2, 4); //去除X:
		String strTempY(vtrstr[1 + 2 * count], 2, 4); //去除Y:

		//乘上TouchPanel的比率
		//LA:1024X1024/1280X800
		//Orange:800X480/800X480
		//PointCoord[count].x = CStr::StrToInt(strTempX)*TPanelHPixel/1024;
		//PointCoord[count].y = CStr::StrToInt(strTempY)*TPanelVPixel/1024;

		PointCoord[count].x = CStr::StrToInt(strTempX);
		PointCoord[count].y = CStr::StrToInt(strTempY);

		//判断Point的位置，动态更新显示
		for (int i = 0; i < TouchLine_Pair_MAX_Number; i++)
			for (int j = 0; j < EveryPair_Line_MAX_Number; j++)
			{
				if ((i + 1) == PairIndex)
				{
					if ((i + 1) % 2 != 0) //判断Pair 1、3;Line 1-2
					{
						for (int m = 0; m < H_Number; m++)
						{
							if ((PointCoord[count].x > TouchLine[i][j].TouchBlock[m].x_Left * Rate) && (PointCoord[count].x < TouchLine[i][j].TouchBlock[m].x_Right * Rate)
							    && (PointCoord[count].y > TouchLine[i][j].TouchBlock[m].y_Top * Rate) && (PointCoord[count].y < TouchLine[i][j].TouchBlock[m].y_Bottom * Rate))
							{
								if (!TouchLine[i][j].TouchBlock[m].Status)
								{
									bInvalidNeed = true;
									TouchLine[i][j].TouchBlock[m].Status = true;
									TouchLine[i][j].p_TouchBlock[m]->SetBkColor(RGB(0, 255, 0)); //显示为绿色
								}
							}
						}

					}
					else//判断Pair 2、4;Line 1-2
					{
						for (int n = 0; n < V_Number; n++)
						{
							if ((PointCoord[count].x > TouchLine[i][j].TouchBlock[n].x_Left * Rate) && (PointCoord[count].x < TouchLine[i][j].TouchBlock[n].x_Right * Rate)
							    && (PointCoord[count].y > TouchLine[i][j].TouchBlock[n].y_Top * Rate) && (PointCoord[count].y < TouchLine[i][j].TouchBlock[n].y_Bottom * Rate))
							{
								if (!TouchLine[i][j].TouchBlock[n].Status)
								{
									bInvalidNeed = true;
									TouchLine[i][j].TouchBlock[n].Status = true;
									TouchLine[i][j].p_TouchBlock[n]->SetBkColor(RGB(0, 255, 0)); //显示为绿色
								}
							}
						}
					}
				}
			}
	}

	if (bInvalidNeed)
		Invalidate(false);//false->刷屏
	return true;
}

bool CTouchGraphDlg::Check_Touch_Result(int PairIndex)
{
	//判断测试的区域是否全部触碰过
	for (int i = 0; i < TouchLine_Pair_MAX_Number; i++)
		for (int j = 0; j < EveryPair_Line_MAX_Number; j++)
		{
			if ((i + 1) == PairIndex)
			{
				if ((i + 1) % 2 != 0) //判断Pair 1、3;Line 1-2
				{
					for (int m = 0; m < H_Number; m++)
					{
						if (TouchLine[i][j].TouchBlock[m].Status == true)
							continue;
						else
							return false;
					}

				}
				else//判断Pair 2、4;Line 1-2
				{
					for (int n = 0; n < V_Number; n++)
					{
						if (TouchLine[i][j].TouchBlock[n].Status == true)
							continue;
						else
							return false;
					}
				}
			}
		}

	return true;
}

void CTouchGraphDlg::OnDestroy()
{
	CDialog::OnCancel();
}

void CTouchGraphDlg::FreeResource()
{
	for (int i = 0; i < TouchLine_Pair_MAX_Number; i++)
	{
		for (int j = 0; j < EveryPair_Line_MAX_Number; j++)
		{
			if ((i + 1) % 2 != 0) //Pair 1、3;Line 1-2
			{
				for (int m = 0; m < H_Number; m++)
				{
					if (TouchLine[i][j].p_TouchBlock[m] != NULL)
						delete TouchLine[i][j].p_TouchBlock[m];
				}
			}
			else//Pair 2、4;Line 1-2
			{
				for (int n = 0; n < V_Number; n++)
				{
					if (TouchLine[i][j].p_TouchBlock[n] != NULL)
						delete TouchLine[i][j].p_TouchBlock[n];
				}
			}
		}
	}
}
