#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"
#include "Label.h"
#include <map>
#include "TouchDefine.h"

// CTouchGraphDlg dialog

#define PARALLEL_MAX_NUMBER 2

typedef struct tagTouchParallelForm
{
	TouchBlockForm TouchBlock[TouchBlock_MAX_H_Number];
	CLabel *p_TouchBlock[TouchBlock_MAX_H_Number];

}TouchParallelForm;

class CTouchParallelDlg : public CDialog
{
	DECLARE_DYNAMIC(CTouchParallelDlg)

public:
	CTouchParallelDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTouchParallelDlg();

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
	int StartPoint_X[PARALLEL_MAX_NUMBER];
	int StartPoint_Y[PARALLEL_MAX_NUMBER];
	int Lenght;
	int Width;
	int H_Number;
	int V_Number;
	int Rate;//Display±ÈÂÊ

	int m_iDlgWidth;
	int m_iDlgHight;

	TouchPointForm PointCoord[TouchPoint_MAX_Number];
	TouchParallelForm TouchLine[PARALLEL_MAX_NUMBER];

public:
	CLabel *NewLabel(CString nText,CRect rect,int nStyle);

	bool Initial_Parameter(int PanelHPixel,int PanelVPixel,int BlockLenght,int BlockWidth,int DisplayRate,int LineDistance);
	bool Show_Test_Parallel();
	bool Check_TouchPoint_Position(char *p_chPointCoord);
	bool Check_Touch_Result();
	void OnDestroy();
	void FreeResource();
};
