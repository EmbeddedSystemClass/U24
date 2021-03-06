
#include "stdafx.h"
#include "../../../UnifyDLL_FTWireless.h"
#include "SideKeyTestDlg.h"


IMPLEMENT_DYNAMIC(CDublin2KeyTestDlg, CDialog)

CDublin2KeyTestDlg::CDublin2KeyTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDublin2KeyTestDlg::IDD, pParent)
{
	hOld=GetModuleHandle(NULL); //保存以前的资源搜索模块
	hDll = GetModuleHandle(MMIBB_DLLNAME);
	AfxSetResourceHandle(hDll);//设置新的模块
}

CDublin2KeyTestDlg::~CDublin2KeyTestDlg()
{

}

void CDublin2KeyTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDublin2KeyTestDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


void CDublin2KeyTestDlg::OnBnClickedOk()  
{  
    //OnOK();   
}  


// CTouchPointDlg message handlers
BOOL CDublin2KeyTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//设置Touch Graph Dialog的大小
	CRect DlgRect(0,0,m_iDlgWidth,m_iDlgHight);
	CWnd::SetWindowPos(NULL,0,0,DlgRect.Width(),DlgRect.Height(),SWP_NOZORDER|SWP_NOMOVE); 

	/* Set window text */
	SetWindowText(_T("Key test"));

	Show_Test_TouchScreen();	

	return TRUE;
}

CLabel *CDublin2KeyTestDlg::NewLabel(CString nText,CRect rect,int nStyle)
{
	CLabel *p_Label = new CLabel();
	p_Label->Create(nText, WS_CHILD|WS_VISIBLE|nStyle, rect, this);   //创建静态文本
	return p_Label;
}

bool CDublin2KeyTestDlg::Initial_Parameter(int PanelHPixel,int PanelVPixel,int BlockLenght,int BlockWidth,int DisplayRate)
{
	Rate = DisplayRate;

	TPanelHPixel = PanelHPixel;
	TPanelVPixel = PanelVPixel;
	Lenght = BlockLenght/DisplayRate;
	Width = BlockWidth/DisplayRate;

	H_Number = PanelHPixel/BlockLenght;
	V_Number = PanelVPixel/BlockWidth;

	if((H_Number>TouchBlock_MAX_H_Number)||(V_Number>TouchBlock_MAX_V_Number))
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
			TouchScreen.p_TouchBlock[i][j] = NULL;

		}
	}

	return true;
}

bool CDublin2KeyTestDlg::Show_Test_TouchScreen()
{
	int j;
	j = H_Number - 1;
	int i = 1;
	for (int k = 0 ; k < 2 ; k++){
		i += 2;
		TouchScreen.TouchBlock[i][j].x_Left = StartPoint_X + j*Lenght;
		TouchScreen.TouchBlock[i][j].y_Top = StartPoint_Y + i*Width;
		TouchScreen.TouchBlock[i][j].x_Right = TouchScreen.TouchBlock[i][j].x_Left + Lenght;
		TouchScreen.TouchBlock[i][j].y_Bottom = TouchScreen.TouchBlock[i][j].y_Top + Width;

		TouchScreen.TouchBlock[i][j].Status = false;

		TouchScreen.p_TouchBlock[i][j] = NewLabel(_T(""), CRect(TouchScreen.TouchBlock[i][j].x_Left,TouchScreen.TouchBlock[i][j].y_Top,TouchScreen.TouchBlock[i][j].x_Right,TouchScreen.TouchBlock[i][j].y_Bottom), SS_CENTER);
		TouchScreen.p_TouchBlock[i][j]->SetBorder(true);
		TouchScreen.p_TouchBlock[i][j]->SetBkColor(RGB(255,0,0)); //显示为红色
		TouchScreen.p_TouchBlock[i][j]->ShowWindow(true);
	}
	return true;
}

bool CDublin2KeyTestDlg::Check_Key_Position(char *p_chkeyID)
{
	int bInvalidNeed = false;

	for(int i=0;i<V_Number;i++)
	{
		for(int j=0;j<H_Number;j++)
		{
			int nkeyId = CStr::StrToInt(p_chkeyID);
			if( j == H_Number - 1 )
			{
				if (nkeyId == 115){//volume up
					if (i == 3) {
						if (!TouchScreen.TouchBlock[i][j].Status){
							bInvalidNeed = true;
							TouchScreen.TouchBlock[i][j].Status = true;
							TouchScreen.p_TouchBlock[i][j]->SetBkColor(RGB(0,255,0)); //显示为绿色
						}
					}
				}else if (nkeyId == 114){
					if (i == 5) {
						if (!TouchScreen.TouchBlock[i][j].Status){
							bInvalidNeed = true;
							TouchScreen.TouchBlock[i][j].Status = true;
							TouchScreen.p_TouchBlock[i][j]->SetBkColor(RGB(0,255,0)); //显示为绿色
						}
					}
				}
				//}else if (nkeyId == 116){  // power key
				//	if (i == 1) {
				//		if (!TouchScreen.TouchBlock[i][j].Status){
				//			bInvalidNeed = true;
				//			TouchScreen.TouchBlock[i][j].Status = true;
				//			TouchScreen.p_TouchBlock[i][j]->SetBkColor(RGB(0,255,0)); //显示为绿色
				//		}
				//	}
				//}

			}/*end of if( j == H_Number - 1 ) */

		}
	}

	if(bInvalidNeed)
		Invalidate(false);//false->刷

	return true;
}

bool CDublin2KeyTestDlg::Check_Key_Result(std::string& ErrorCode)
{
	//判断测试的区域是否全部触碰过(由外圈到内圈)
	//TouchScreen.TouchBlock[3][H_Number - 1].Status == true &&
	//if(	TouchScreen.TouchBlock[3][0].Status == true &&
		//TouchScreen.TouchBlock[5][0].Status == true)
	if(	TouchScreen.TouchBlock[3][H_Number - 1].Status == true &&
		TouchScreen.TouchBlock[5][H_Number - 1].Status == true)
	{
		return true;
	}
	else 
	{

		if (TouchScreen.TouchBlock[3][H_Number - 1].Status == false)
		{
			ErrorCode = FunErr_VolumeUpKey_Test_Fail;
		}
		else if (TouchScreen.TouchBlock[5][H_Number - 1].Status == false)
		{
			ErrorCode = FunErr_VolumeDownKey_Test_Fail;
		}
		return false;
	}
}

//销毁窗口
void CDublin2KeyTestDlg::OnDestroy()   
{ 
	CDialog::OnCancel();
}

//释放new内存资源
void CDublin2KeyTestDlg::FreeResource()
{
	for(int i=0;i<V_Number;i++)
	{
		for(int j=0;j<H_Number;j++)
		{
			if(TouchScreen.p_TouchBlock[i][j] != NULL)
				delete TouchScreen.p_TouchBlock[i][j];
				TouchScreen.p_TouchBlock[i][j] = NULL;
		}
	}
}