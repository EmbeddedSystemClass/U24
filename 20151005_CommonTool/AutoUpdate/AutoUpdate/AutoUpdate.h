
// AutoUpdate.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "Function.h"

// CAutoUpdateApp:
// �йش����ʵ�֣������ AutoUpdate.cpp
//

class CAutoUpdateApp : public CWinAppEx,public Function
{
public:
	CAutoUpdateApp();
    Function m_fun;

// ��д
	public:
	virtual BOOL InitInstance();
    
// ʵ��

	DECLARE_MESSAGE_MAP()

};

extern CAutoUpdateApp theApp;