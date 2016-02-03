#pragma once

#include "../../../resource.h"
#include "../../../Label.h"
#include "../UnifyInterface/FacTestToolInterface.h"

#define BounceWindow_MAX_H_Number 96
#define BounceWindow_MAX_V_Number 160


//UI display block
struct sWindowsSize
{
	int x_Left;
	int y_Top;
	int x_Right;
	int y_Bottom;

	bool Status;//true:Touched;false:not touched
};


struct sBounceWindow
{
	sWindowsSize BounceWindowBlock[BounceWindow_MAX_H_Number][BounceWindow_MAX_V_Number];
	CLabel *sBounceWindowCLable[BounceWindow_MAX_H_Number][BounceWindow_MAX_V_Number];
};

class CFQC_KeyButtonRetryTestWindow : public CDialog
{
	DECLARE_DYNAMIC(CFQC_KeyButtonRetryTestWindow)

public:
	CFQC_KeyButtonRetryTestWindow(CWnd* pParent = NULL, int nFlag = 0);   // standard constructor
	//CFQC_KeyButtonRetryTestWindow(int nStyle);   // standard constructor
	
	virtual ~CFQC_KeyButtonRetryTestWindow();

	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_TOUCHDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	HINSTANCE hOld; // 旧的资源句柄
	HINSTANCE hDll; // DLL资源句柄

	//BounceWindowBlock

	int StartPoint_X;
	int StartPoint_Y;
	int Lenght;
	int Width;
	int H_Number;
	int V_Number;
	int Rate;//Display比率

	int m_iDlgWidth;//对话框的宽度
	int m_iDlgHight;//对话框的高度
 
	int nWindowsStyle;
	sBounceWindow newButtonWindow;

	//接收Point
	int Point_Number;

public:
	CLabel *NewLabelHeadSetKey(CString nText,CRect rect,int nStyle);
	CLabel *NewLabelSideKey(CString nText,CRect rect,int nStyle);
	
	afx_msg void OnBnClickedOk();

	bool Initial_Parameter(int PanelHPixel,int PanelVPixel,int BlockLenght,int BlockWidth,int DisplayRate, int windowsStyle);
	bool Add_BounceWindowHeadetKey();

	bool Show_Button_BounceWindowSideKey();
	bool Add_BounceWindowSideKey();
	bool Check_SideKey_Position(char *p_chkeyID);
	bool Check_SideKey_Result(std::string& ErrorCode);

	void OnDestroy();
	void CDiagDestroyWindow();
	void FreeResource();
};
