// TouchPointDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UnifyDLL_FTWireless.h"
#include "TouchPointDlg.h"


// CTouchPointDlg dialog

IMPLEMENT_DYNAMIC(CTouchPointDlg, CDialog)

CTouchPointDlg::CTouchPointDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTouchPointDlg::IDD, pParent)
{
	hOld = GetModuleHandle(NULL);
	hDll = GetModuleHandle(MMIBB_DLLNAME);
	AfxSetResourceHandle(hDll);
}

CTouchPointDlg::~CTouchPointDlg()
{

}

void CTouchPointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTouchPointDlg, CDialog)
END_MESSAGE_MAP()


// CTouchPointDlg message handlers
BOOL CTouchPointDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	AfxSetResourceHandle(hOld);

	//set Touch Graph Dialog size
	CRect DlgRect(0, 0, m_iDlgWidth, m_iDlgHight);
	CWnd::SetWindowPos(NULL, 0, 0, DlgRect.Width(), DlgRect.Height(), SWP_NOZORDER | SWP_NOMOVE);

	/* Set window text */
	SetWindowText(_T("4-points test"));

	Show_Test_TouchScreen();

	return TRUE;
}

CLabel* CTouchPointDlg::NewLabel(CString nText, CRect rect, int nStyle)
{
	CLabel* p_Label = new CLabel();
	p_Label->Create(nText, WS_CHILD | WS_VISIBLE | nStyle, rect, this);
	return p_Label;
}

bool CTouchPointDlg::Initial_Parameter(int PanelHPixel, int PanelVPixel, int BlockLenght, int BlockWidth, int DisplayRate)
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

bool CTouchPointDlg::Show_Test_TouchScreen()
{
	//动态创建Touch Test Block
	for (int i = 0; i < V_Number; i++)
	{
		for (int j = 0; j < H_Number; j++)
		{
			//TouchScreen.p_TouchBlock[i][j] = new CLabel();

			TouchScreen.TouchBlock[i][j].x_Left = StartPoint_X + j * Lenght;
			TouchScreen.TouchBlock[i][j].y_Top = StartPoint_Y + i * Width;
			TouchScreen.TouchBlock[i][j].x_Right = TouchScreen.TouchBlock[i][j].x_Left + Lenght;
			TouchScreen.TouchBlock[i][j].y_Bottom = TouchScreen.TouchBlock[i][j].y_Top + Width;

			TouchScreen.TouchBlock[i][j].Status = false;

			TouchScreen.p_TouchBlock[i][j] = NewLabel(_T(""), CRect(TouchScreen.TouchBlock[i][j].x_Left, TouchScreen.TouchBlock[i][j].y_Top, TouchScreen.TouchBlock[i][j].x_Right, TouchScreen.TouchBlock[i][j].y_Bottom), SS_CENTER);
			TouchScreen.p_TouchBlock[i][j]->SetBorder(true);
			TouchScreen.p_TouchBlock[i][j]->SetBkColor(RGB(255, 0, 0)); //显示为红色
			TouchScreen.p_TouchBlock[i][j]->ShowWindow(false);
		}
	}

	return true;
}

bool CTouchPointDlg::Show_TouchPoint_Position()
{

	//显示directly touch 4 point position
	for (int PointCount = 0; PointCount < Point_Number; PointCount++)
	{
		for (int i = 0; i < V_Number; i++)
		{
			for (int j = 0; j < H_Number; j++)
			{
				if ((PointCoord[PointCount].x >= TouchScreen.TouchBlock[i][j].x_Left * Rate) && (PointCoord[PointCount].x < TouchScreen.TouchBlock[i][j].x_Right * Rate)
				    && (PointCoord[PointCount].y >= TouchScreen.TouchBlock[i][j].y_Top * Rate) && (PointCoord[PointCount].y < TouchScreen.TouchBlock[i][j].y_Bottom * Rate))
				{
					TouchScreen.TouchBlock[i][j].Status = true;
					TouchScreen.p_TouchBlock[i][j]->SetBkColor(RGB(0, 255, 0)); //显示为绿色
					TouchScreen.p_TouchBlock[i][j]->ShowWindow(true);
				}
			}
		}
	}

	Invalidate(false);
	return true;
}

bool CTouchPointDlg::Check_Directly_Four_TouchPoint(char* p_chPointCoord)
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

	Point_Number = (int)vtrstr.size() / 2;

	if (Point_Number != TouchPoint_MAX_Number)
	{
		::AfxMessageBox(_T("Get Point Coord from target is large than defined max number!"));
		return false;
	}

	for (count = 0; count < Point_Number; count++)
	{
		//截取X,Y的坐标值
		//std::string strTempX(vtrstr[0+2*count],2,4);//除X:
		//std::string strTempY(vtrstr[1+2*count],2,4);//除Y:

		//乘上TouchPanel的比率
		//LA:1024X1024/1280X800
		//Orange:800X480/800X480
		//PointCoord[count].x = CStr::StrToInt(strTempX)*TPanelHPixel/1024;
		//PointCoord[count].y = CStr::StrToInt(strTempY)*TPanelVPixel/1024;

		//PointCoord[count].x = CStr::StrToInt(strTempX);
		//PointCoord[count].y = CStr::StrToInt(strTempY);
		PointCoord[count].x = CStr::StrToInt(vtrstr[0 + 2 * count]);
		PointCoord[count].y = CStr::StrToInt(vtrstr[1 + 2 * count]);
	}

	//判断Point的坐标，4-point: directly touch 4 point
	for (int PointCount = 0; PointCount < Point_Number; PointCount++)
	{
		if ((PointCoord[PointCount].x != 0) && (PointCoord[PointCount].y != 0))
			continue;
		else
			return false;
	}

	Show_TouchPoint_Position();
	Sleep(2000);//show 2s

	return true;
}

void CTouchPointDlg::OnDestroy()
{
	CDialog::OnCancel();
}

void CTouchPointDlg::FreeResource()
{
	for (int i = 0; i < V_Number; i++)
	{
		for (int j = 0; j < H_Number; j++)
		{
			if (TouchScreen.p_TouchBlock[i][j] != NULL)
				delete TouchScreen.p_TouchBlock[i][j];
		}
	}
}
