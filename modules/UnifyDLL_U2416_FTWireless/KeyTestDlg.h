#pragma once

#include "resource.h"
#include "Label.h"
#include <map>
#include "TouchDefine.h"

// CTouchPointDlg dialog

typedef struct tagTouchScreenForm
{
	TouchBlockForm TouchBlock[TouchBlock_MAX_H_Number][TouchBlock_MAX_V_Number];
	CLabel *p_TouchBlock[TouchBlock_MAX_H_Number][TouchBlock_MAX_V_Number];

}TouchScreenForm;

class CKeyTestDlg : public CDialog
{
	DECLARE_DYNAMIC(CKeyTestDlg)

public:
	CKeyTestDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CKeyTestDlg();

	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_TOUCHDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	HINSTANCE hOld; // 旧的资源句柄
	HINSTANCE hDll; // DLL资源句柄

	//TouchBlock
	int TPanelHPixel;
	int TPanelVPixel;
	int StartPoint_X;
	int StartPoint_Y;
	int Lenght;
	int Width;
	int H_Number;
	int V_Number;
	int Rate;//Display比率

	int m_iDlgWidth;//对话框的宽度
	int m_iDlgHight;//对话框的高度

	TouchPointForm PointCoord[TouchPoint_MAX_Number];
	TouchScreenForm TouchScreen;

	//接收Point
	int Point_Number;

public:
	CLabel *NewLabel(CString nText,CRect rect,int nStyle);
	afx_msg void OnBnClickedOk();

	bool Initial_Parameter(int PanelHPixel,int PanelVPixel,int BlockLenght,int BlockWidth,int DisplayRate);
	bool Show_Test_TouchScreen();
	bool Check_Key_Position(char *p_chkeyID);
	bool Check_Key_Result(std::string& ErrorCode);


	void OnDestroy();
	void FreeResource();
};
