#pragma once

#include "resource.h"
#include "Label.h"
#include <map>
#include "TouchDefine.h"

// CTouchCircle dialog

#define TouchCircle_MAX_Number 4

typedef struct tagTouchCircleForm
{
	TouchBlockForm TouchBlock[TouchBlock_MAX_H_Number][TouchBlock_MAX_V_Number];
	CLabel *p_TouchBlock[TouchBlock_MAX_H_Number][TouchBlock_MAX_V_Number];

}TouchCircleForm;

class CTouchCircleDlg : public CDialog
{
	DECLARE_DYNAMIC(CTouchCircleDlg)

public:
	CTouchCircleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTouchCircleDlg();

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
	int CircleCount;
	int Distance;

	int TPanelHPixel;
	int TPanelVPixel;
	int StartPoint_X[TouchCircle_MAX_Number];
	int StartPoint_Y[TouchCircle_MAX_Number];
	int Lenght;
	int Width;
	int H_Number[TouchCircle_MAX_Number];
	int V_Number[TouchCircle_MAX_Number];
	int Rate;//Display rate

	int m_iDlgWidth;
	int m_iDlgHight;

	TouchPointForm PointCoord[TouchCircle_MAX_Number];
	TouchCircleForm TouchCircle[TouchCircle_MAX_Number];
public:
	CLabel *NewLabel(CString nText,CRect rect,int nStyle);

	bool Initial_Parameter(int PanelHPixel,int PanelVPixel,int BlockLenght,int BlockWidth,int DisplayRate,int Count,int CircleDistance);
	bool Show_Test_TouchPoint();
	bool Check_TouchPoint_Position(char *p_chPointCoord);
	bool Check_Touch_Result();
	void OnDestroy();
	void FreeResource();
};

