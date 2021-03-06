/*********************************************************************************
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1   2014    Lion.Wu            Create for Detroit project MMI test
 ********************************************************************************/

#include "stdafx.h"
#include "../../../UnifyDLL_FTWireless.h"
#include "FQC_KeyButtonRetryTestWindow.h"


IMPLEMENT_DYNAMIC(CFQC_KeyButtonRetryTestWindow, CDialog)

CFQC_KeyButtonRetryTestWindow::CFQC_KeyButtonRetryTestWindow(CWnd* pParent,int nFlag /*=NULL*/)
	: CDialog(CFQC_KeyButtonRetryTestWindow::IDD, pParent)
{
	hOld=GetModuleHandle(NULL); //保存以前的资源搜索模块
	if ( nFlag == 0){//MMIBB
		hDll = GetModuleHandle(MMIBB_DLLNAME);
	}else //FQC
	{	
		hDll = GetModuleHandle(FQC_DLLNAME);
	}
	AfxSetResourceHandle(hDll);//设置新的模块

}

CFQC_KeyButtonRetryTestWindow::~CFQC_KeyButtonRetryTestWindow()
{
	DestroyWindow();
	CloseHandle(hDll);
}

void CFQC_KeyButtonRetryTestWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFQC_KeyButtonRetryTestWindow, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


void CFQC_KeyButtonRetryTestWindow::OnBnClickedOk()
{
    //OnOK();
}

// CTouchPointDlg message handlers
BOOL CFQC_KeyButtonRetryTestWindow::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect DlgRect(0,0,m_iDlgWidth,m_iDlgHight);
	CWnd::SetWindowPos(NULL,0,0,DlgRect.Width(),DlgRect.Height(),SWP_NOZORDER|SWP_NOMOVE);

	if ( nWindowsStyle ==0 ){

		Add_BounceWindowHeadetKey();
		SetWindowText(_T("HeadsetKey Test"));		/* Set window text */
	}
	else
	{
		Add_BounceWindowSideKey();
		SetWindowText(_T("SideKey Test"));		/* Set window text */
	}
	return TRUE;
}


CLabel *CFQC_KeyButtonRetryTestWindow::NewLabelHeadSetKey(CString nText,CRect rect,int nStyle)
{
	CLabel *p_Label = new CLabel();
	p_Label->SetFontSize(26);
	p_Label->SetFontBold(1);
	p_Label->Create(nText, WS_CHILD|WS_VISIBLE|nStyle, rect, this);   //创建静态文本

	return p_Label;
}

CLabel *CFQC_KeyButtonRetryTestWindow::NewLabelSideKey(CString nText,CRect rect,int nStyle)
{
	CLabel *p_Label = new CLabel();
	p_Label->SetFontSize(12);
	p_Label->Create(nText, WS_CHILD|WS_VISIBLE|nStyle, rect, this);   //创建静态文本

	return p_Label;
}



bool CFQC_KeyButtonRetryTestWindow::Initial_Parameter(int PanelHPixel,int PanelVPixel,int BlockLenght,int BlockWidth,int DisplayRate, int windowsStyle)
{
	nWindowsStyle = windowsStyle;
	Lenght = BlockLenght/DisplayRate;
	Width = BlockWidth/DisplayRate;

	H_Number = PanelHPixel/BlockLenght;
	V_Number = PanelVPixel/BlockWidth;

	if((H_Number>BounceWindow_MAX_H_Number)||(V_Number>BounceWindow_MAX_V_Number))
	{
		::AfxMessageBox(_T("Touch Block numbers is greater than defined max numbers!"));
		return false;
	}

	StartPoint_X = 0;
	StartPoint_Y = 0;

	//加上Dialog Frame(6,33)
	m_iDlgWidth = H_Number*Lenght+6;
	m_iDlgHight = V_Number*Width+33;

	for(int i=0;i<V_Number;i++)
	{
		for(int j=0;j<H_Number;j++)
		{
			newButtonWindow.sBounceWindowCLable[i][j] = NULL;
		}
	}

	return true;
}
bool CFQC_KeyButtonRetryTestWindow::Add_BounceWindowSideKey()
{
	int j;
	j = H_Number - 1;
	int i = 1;
	for (int k = 0 ; k < 3 ; k++){
		i += 2;
		//if ( i  != 7 ){
		if ( i  == 3 ){
			newButtonWindow.BounceWindowBlock[i][j].x_Left = StartPoint_X + j*Lenght;
			newButtonWindow.BounceWindowBlock[i][j].y_Top = StartPoint_Y + i*Width;
		}else{// power key
			newButtonWindow.BounceWindowBlock[i][j].x_Left = Width;
			newButtonWindow.BounceWindowBlock[i][j].y_Top = 0;
		}
		newButtonWindow.BounceWindowBlock[i][j].x_Right = newButtonWindow.BounceWindowBlock[i][j].x_Left + Lenght;
		newButtonWindow.BounceWindowBlock[i][j].y_Bottom = newButtonWindow.BounceWindowBlock[i][j].y_Top + Width;
		newButtonWindow.BounceWindowBlock[i][j].Status = false;
		newButtonWindow.sBounceWindowCLable[i][j] = NewLabelSideKey((" "), CRect(newButtonWindow.BounceWindowBlock[i][j].x_Left,newButtonWindow.BounceWindowBlock[i][j].y_Top,newButtonWindow.BounceWindowBlock[i][j].x_Right,newButtonWindow.BounceWindowBlock[i][j].y_Bottom), SS_CENTER);
		newButtonWindow.sBounceWindowCLable[i][j]->SetBorder(true);
		newButtonWindow.sBounceWindowCLable[i][j]->SetBkColor(RGB(255,0,0)); //显示为红色
		newButtonWindow.sBounceWindowCLable[i][j]->ShowWindow(true);

	}

	/*title bar */
	i+=2;
	newButtonWindow.BounceWindowBlock[i][j].x_Left=  0;
	newButtonWindow.BounceWindowBlock[i][j].y_Top = m_iDlgHight - Width * 6;
	newButtonWindow.BounceWindowBlock[i][j].x_Right = m_iDlgWidth;
	newButtonWindow.BounceWindowBlock[i][j].y_Bottom = newButtonWindow.BounceWindowBlock[i][j].y_Top + Width;
	newButtonWindow.BounceWindowBlock[i][j].Status = false;
	newButtonWindow.sBounceWindowCLable[i][j] = NewLabelSideKey(CString(L"KEY TEST 按键测试"), CRect(newButtonWindow.BounceWindowBlock[i][j].x_Left,newButtonWindow.BounceWindowBlock[i][j].y_Top,newButtonWindow.BounceWindowBlock[i][j].x_Right,newButtonWindow.BounceWindowBlock[i][j].y_Bottom), SS_CENTER);
	newButtonWindow.sBounceWindowCLable[i][j]->SetBorder(true);
	newButtonWindow.sBounceWindowCLable[i][j]->SetBkColor(RGB(255,255,0));
	newButtonWindow.sBounceWindowCLable[i][j]->SetFontBold(true);
	newButtonWindow.sBounceWindowCLable[i][j]->SetTextColor(RGB(255,0,0));
	newButtonWindow.sBounceWindowCLable[i][j]->SetFontSize(30);
	newButtonWindow.sBounceWindowCLable[i][j]->ShowWindow(true);
	return true;
}

bool CFQC_KeyButtonRetryTestWindow::Check_SideKey_Position(char *p_chkeyID)
{
	int bInvalidNeed = false;
	int  j = H_Number - 1;
	for(int i=3;i<8;i+=2)
	{
		int nkeyId = CStr::StrToInt(p_chkeyID);

		if (nkeyId == 212){//capkey 
			if (i == 3) {
				if (!newButtonWindow.BounceWindowBlock[i][j].Status){
					bInvalidNeed = true;
					newButtonWindow.BounceWindowBlock[i][j].Status = true;
					newButtonWindow.sBounceWindowCLable[i][j]->SetBkColor(RGB(0,255,0)); //显示为绿色
				}
			}
		}
		//}else if (nkeyId == 114){//volume down
		//	if (i == 5) {
		//		if (!newButtonWindow.BounceWindowBlock[i][j].Status){
		//			bInvalidNeed = true;
		//			newButtonWindow.BounceWindowBlock[i][j].Status = true;
		//			newButtonWindow.sBounceWindowCLable[i][j]->SetBkColor(RGB(0,255,0)); //显示为绿色
		//		}
		//	}
		//}
		else if (nkeyId == 116){  // power key
			if (i == 7) {
				if (!newButtonWindow.BounceWindowBlock[i][j].Status){
					bInvalidNeed = true;
					newButtonWindow.BounceWindowBlock[i][j].Status = true;
					newButtonWindow.sBounceWindowCLable[i][j]->SetBkColor(RGB(0,255,0)); //显示为绿色
				}
			}
		}
	}

	if(bInvalidNeed)
		Invalidate(false);//false

	return true;
}


bool CFQC_KeyButtonRetryTestWindow::Check_SideKey_Result(std::string& ErrorCode)
{
	//if(	(newButtonWindow.BounceWindowBlock[3][H_Number - 1].Status == true) &&
	//	(newButtonWindow.BounceWindowBlock[5][H_Number - 1].Status == true) &&
	//	(newButtonWindow.BounceWindowBlock[7][H_Number - 1].Status == true))
	//{
	if(	(newButtonWindow.BounceWindowBlock[3][H_Number - 1].Status == true) &&
		(newButtonWindow.BounceWindowBlock[7][H_Number - 1].Status == true))
	{
		return true;
	}
	else
	{

		if (newButtonWindow.BounceWindowBlock[3][H_Number - 1].Status == false)
		{
			ErrorCode = FunErr_VolumeUpKey_Test_Fail;
		}
		//else if (newButtonWindow.BounceWindowBlock[5][H_Number - 1].Status == false)
		//{
		//	ErrorCode = FunErr_VolumeDownKey_Test_Fail;
		//}
		else if (newButtonWindow.BounceWindowBlock[7][H_Number - 1].Status == false)
		{
			ErrorCode = FunErr_PowerKey_Test_Fail;
		}

		return false;
	}
}


bool CFQC_KeyButtonRetryTestWindow::Add_BounceWindowHeadetKey()
{
	int i, j ;
	i = 5;
	j = H_Number - 2;

	newButtonWindow.BounceWindowBlock[i][j].x_Left = 0;//StartPoint_X + j*Lenght;
	newButtonWindow.BounceWindowBlock[i][j].y_Top = StartPoint_Y + i*Width;
	newButtonWindow.BounceWindowBlock[i][j].x_Right = m_iDlgWidth;//newButtonWindow.BounceWindowBlock[i][j].x_Left + Lenght;
	newButtonWindow.BounceWindowBlock[i][j].y_Bottom = newButtonWindow.BounceWindowBlock[i][j].y_Top + Width;

	newButtonWindow.BounceWindowBlock[i][j].Status = false;

	newButtonWindow.sBounceWindowCLable[i][j] = NewLabelHeadSetKey(CString(L"请按耳机键"), CRect(newButtonWindow.BounceWindowBlock[i][j].x_Left,newButtonWindow.BounceWindowBlock[i][j].y_Top,newButtonWindow.BounceWindowBlock[i][j].x_Right,newButtonWindow.BounceWindowBlock[i][j].y_Bottom), SS_CENTER);
	newButtonWindow.sBounceWindowCLable[i][j]->SetBorder(true);
	newButtonWindow.sBounceWindowCLable[i][j]->SetBkColor(RGB(255,255,0)); //显示为红色

	newButtonWindow.sBounceWindowCLable[i][j]->ShowWindow(true);

	return true;
}

//销毁窗口
void CFQC_KeyButtonRetryTestWindow::OnDestroy()
{
	CDialog::OnCancel();
}

void CFQC_KeyButtonRetryTestWindow::CDiagDestroyWindow()
{
	CDialog::DestroyWindow();
}

//释放new内存资源
void CFQC_KeyButtonRetryTestWindow::FreeResource()
{
	for(int i=0;i<V_Number;i++)
	{
		for(int j=0;j<H_Number;j++)
		{
			if(newButtonWindow.sBounceWindowCLable[i][j] != NULL){
				delete newButtonWindow.sBounceWindowCLable[i][j];
				newButtonWindow.sBounceWindowCLable[i][j] = NULL;
			}
		}
	}
}