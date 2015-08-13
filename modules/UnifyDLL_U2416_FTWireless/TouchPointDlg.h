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

class CTouchPointDlg : public CDialog
{
	DECLARE_DYNAMIC(CTouchPointDlg)

public:
	CTouchPointDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTouchPointDlg();

	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_TOUCHDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	HINSTANCE hOld;
	HINSTANCE hDll;

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

	int m_iDlgWidth;
	int m_iDlgHight;

	TouchPointForm PointCoord[TouchPoint_MAX_Number];
	TouchScreenForm TouchScreen;

	//接收Point
	int Point_Number;

public:
	CLabel *NewLabel(CString nText,CRect rect,int nStyle);

	bool Initial_Parameter(int PanelHPixel,int PanelVPixel,int BlockLenght,int BlockWidth,int DisplayRate);
	bool Show_Test_TouchScreen();
	bool Show_TouchPoint_Position();
	bool Check_Directly_Four_TouchPoint(char *p_chPointCoord);
	void OnDestroy();
	void FreeResource();
};
