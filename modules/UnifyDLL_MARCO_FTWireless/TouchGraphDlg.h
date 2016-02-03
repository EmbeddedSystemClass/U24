#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"
#include "Label.h"
#include <map>
#include "TouchDefine.h"

// CTouchGraphDlg dialog

#define TouchLine_Pair_MAX_Number 4
#define EveryPair_Line_MAX_Number 2

typedef struct tagTouchPairLineForm
{
	TouchBlockForm TouchBlock[TouchBlock_MAX_H_Number];
	CLabel *p_TouchBlock[TouchBlock_MAX_H_Number];

}TouchPairLineForm;

class CTouchGraphDlg : public CDialog
{
	DECLARE_DYNAMIC(CTouchGraphDlg)

public:
	CTouchGraphDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTouchGraphDlg();

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
	int Distance;

	int TPanelHPixel;
	int TPanelVPixel;
	int StartPoint_X[TouchLine_Pair_MAX_Number][EveryPair_Line_MAX_Number];
	int StartPoint_Y[TouchLine_Pair_MAX_Number][EveryPair_Line_MAX_Number];
	int Lenght;
	int Width;
	int H_Number;
	int V_Number;
	int Rate;//Display±ÈÂÊ

	int m_iDlgWidth;
	int m_iDlgHight;

	TouchPointForm PointCoord[TouchPoint_MAX_Number];
	TouchPairLineForm TouchLine[TouchLine_Pair_MAX_Number][EveryPair_Line_MAX_Number];
public:
	CLabel *NewLabel(CString nText,CRect rect,int nStyle);

	bool Initial_Parameter(int PanelHPixel,int PanelVPixel,int BlockLenght,int BlockWidth,int DisplayRate,int LineDistance);
	//bool Initial_Parameter(int PanelHPixel,int PanelVPixel,int BlockLenght,int BlockWidth,int DisplayRate);
	bool Create_Test_TouchPoint(int ShowPairIndex);
	bool Show_Test_TouchLine(int ShowPairIndex);
	bool Check_TouchPoint_Position(char *p_chPointCoord,int PairIndex);
	//bool Check_Touch_Result(int PairIndex);LIon
	bool Check_Touch_Result(int PairIndex);
	void OnDestroy();
	void FreeResource();
};
