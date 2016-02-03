#pragma once

#include "resource.h"
#include "Label.h"
#include <map>
#include "TouchDefine.h"

// CTouchCircle dialog

#define TouchCapkey_MAX_Number 1

typedef struct tagTouchCapkeyForm
{
	TouchBlockForm TouchBlock[TouchBlock_MAX_H_Number][TouchBlock_MAX_V_Number];
	CLabel *p_TouchBlock[TouchBlock_MAX_H_Number][TouchBlock_MAX_V_Number];

}TouchCapkeyForm;

class CTouchCapkeyDlg : public CDialog
{
	DECLARE_DYNAMIC(CTouchCapkeyDlg)

public:
	CTouchCapkeyDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTouchCapkeyDlg();

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
	int Rate;//Display±ÈÂÊ

	int m_iDlgWidth;
	int m_iDlgHight;

	TouchPointForm PointCoord;
	TouchCapkeyForm TouchCapkey;
public:
	CLabel *NewLabel(CString nText,CRect rect,int nStyle);

	bool Initial_Parameter(int PanelHPixel,int PanelVPixel,int BlockLenght,int BlockWidth,int DisplayRate);
	bool Show_Test_TouchPoint();
	bool Check_TouchPoint_Position(char *p_chPointCoord);
	bool Check_Touch_Result();
	void OnDestroy();
	void FreeResource();
};
