// Entrance Tool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CEntranceToolApp:
// �йش����ʵ�֣������ Entrance Tool.cpp
//

class CEntranceToolApp : public CWinApp
{
public:
	CEntranceToolApp();


// ��д
	public:
	virtual BOOL InitInstance();


 

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CEntranceToolApp theApp;