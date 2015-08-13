// TouchGraphDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UnifyDLL_FTWireless.h"
#include "TouchParallelDlg.h"

// CTouchGraphDlg dialog

IMPLEMENT_DYNAMIC(CTouchParallelDlg, CDialog)

CTouchParallelDlg::CTouchParallelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTouchParallelDlg::IDD, pParent)
{
	hOld = GetModuleHandle(NULL);
	hDll = GetModuleHandle(MMIBB_DLLNAME);
	AfxSetResourceHandle(hDll);
}

CTouchParallelDlg::~CTouchParallelDlg()
{

}

void CTouchParallelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTouchParallelDlg, CDialog)
END_MESSAGE_MAP()


// CTouchGraphDlg message handlers
BOOL CTouchParallelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	AfxSetResourceHandle(hOld);

	//set Touch Graph Dialog size
	CRect DlgRect(0, 0, m_iDlgWidth, m_iDlgHight);
	CWnd::SetWindowPos(NULL, 0, 0, DlgRect.Width(), DlgRect.Height(), SWP_NOZORDER | SWP_NOMOVE);

	SetWindowText(_T("Multi-touch test"));
	Show_Test_Parallel();

	return TRUE;
}

CLabel* CTouchParallelDlg::NewLabel(CString nText, CRect rect, int nStyle)
{
	CLabel* p_Label = new CLabel();
	p_Label->Create(nText, WS_CHILD | WS_VISIBLE | nStyle, rect, this); //创建静态文本
	return p_Label;
}

bool CTouchParallelDlg::Initial_Parameter(int PanelHPixel, int PanelVPixel, int BlockLenght, int BlockWidth, int DisplayRate, int LineDistance)
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

	//Parallel,Line 1
	StartPoint_X[0] = (PanelHPixel - LineDistance - BlockLenght) / 2 / Rate;
	StartPoint_Y[0] = 0;

	//Parallel,Line 2
	StartPoint_X[1] = (PanelHPixel + LineDistance - BlockLenght) / 2 / Rate;
	StartPoint_Y[1] = 0;

	//加上Dialog Frame(6,33)
	m_iDlgWidth = H_Number * Lenght + 6;
	m_iDlgHight = V_Number * Width + 33;

	/* Initial pointer parameter */
	for (int i = 0; i < PARALLEL_MAX_NUMBER; i++)
	{
		for (int j = 0; j < V_Number; j++)
		{
			TouchLine[i].p_TouchBlock[j] = NULL;
		}
	}

	return true;
}

bool CTouchParallelDlg::Show_Test_Parallel()
{
	/* Dynamic Create Touch Test Block */
	for (int i = 0; i < PARALLEL_MAX_NUMBER; i++)
	{
		for (int j = 0; j < V_Number; j++)
		{
			TouchLine[i].TouchBlock[j].x_Left = StartPoint_X[i];
			TouchLine[i].TouchBlock[j].y_Top = StartPoint_Y[i] + j * Width;
			TouchLine[i].TouchBlock[j].x_Right = TouchLine[i].TouchBlock[j].x_Left + Lenght;
			TouchLine[i].TouchBlock[j].y_Bottom = TouchLine[i].TouchBlock[j].y_Top + Width;

			TouchLine[i].TouchBlock[j].Status = false;

			TouchLine[i].p_TouchBlock[j] = NewLabel(_T(""), CRect(TouchLine[i].TouchBlock[j].x_Left, TouchLine[i].TouchBlock[j].y_Top, TouchLine[i].TouchBlock[j].x_Right, TouchLine[i].TouchBlock[j].y_Bottom), SS_CENTER);
			TouchLine[i].p_TouchBlock[j]->SetBorder(true);
			TouchLine[i].p_TouchBlock[j]->SetBkColor(RGB(255, 0, 0)); //显示为红色

			TouchLine[i].p_TouchBlock[j]->ShowWindow(true);
		}
	}

	return true;
}

bool CTouchParallelDlg::Check_TouchPoint_Position(char* p_chPointCoord)
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
		//std::string strTempX(vtrstr[0+2*count], 2, 4);//除X:
		//std::string strTempY(vtrstr[1+2*count], 2, 4);//除Y:

		//乘上TouchPanel的比率
		//LA:1024X1024/1280X800
		//Orange:800X480/800X480
		//PointCoord[count].x = CStr::StrToInt(strTempX)*TPanelHPixel/1024;
		//PointCoord[count].y = CStr::StrToInt(strTempY)*TPanelVPixel/1024;

		PointCoord[count].x = CStr::StrToInt(vtrstr[0 + 2 * count]);
		PointCoord[count].y = CStr::StrToInt(vtrstr[1 + 2 * count]);
		TRACE("Check Point Line%d => X: %d Y: %d", count, PointCoord[count].x, PointCoord[count].y);

		//PointCoord[count].x = CStr::StrToInt(strTempX);
		//PointCoord[count].y = CStr::StrToInt(strTempY);

		//判断Point的位置，动态更新显示
		for (int i = 0; i < PARALLEL_MAX_NUMBER; i++)
		{
			for (int j = 0; j < V_Number; j++)
			{
				if ((PointCoord[count].x > TouchLine[i].TouchBlock[j].x_Left * Rate) && (PointCoord[count].x < TouchLine[i].TouchBlock[j].x_Right * Rate)
				    && (PointCoord[count].y > TouchLine[i].TouchBlock[j].y_Top * Rate) && (PointCoord[count].y < TouchLine[i].TouchBlock[j].y_Bottom * Rate))
				{
					if (!TouchLine[i].TouchBlock[j].Status)
					{
						bInvalidNeed = true;
						TouchLine[i].TouchBlock[j].Status = true;
						TouchLine[i].p_TouchBlock[j]->SetBkColor(RGB(0, 255, 0)); //显示为绿色
					}
				}
			}
		}
	}

	if (bInvalidNeed)
		Invalidate(false);

	return true;
}

bool CTouchParallelDlg::Check_Touch_Result()
{
	//判断测试的区域是否全部触碰过
	for (int i = 0; i < PARALLEL_MAX_NUMBER; i++)
	{
		for (int j = 0; j < V_Number; j++)
		{
			if (TouchLine[i].TouchBlock[j].Status == true)
				continue;
			else
				return false;
		}
	}

	return true;
}

void CTouchParallelDlg::OnDestroy()
{
	CDialog::OnCancel();
}

void CTouchParallelDlg::FreeResource()
{
	for (int i = 0; i < PARALLEL_MAX_NUMBER; i++)
	{
		for (int j = 0; j < V_Number; j++)
		{
			if (TouchLine[i].p_TouchBlock[j] != NULL)
				delete TouchLine[i].p_TouchBlock[j];
		}
	}
}
