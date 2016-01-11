
// AutoUpdate.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "AutoUpdate.h"
#include "AutoUpdateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef UNICODE
#define argv __wargv
#else
#define argv __argv
#endif //UNICODE

// CAutoUpdateApp

BEGIN_MESSAGE_MAP(CAutoUpdateApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CAutoUpdateApp ����

CAutoUpdateApp::CAutoUpdateApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CAutoUpdateApp ����

CAutoUpdateApp theApp;


// CAutoUpdateApp ��ʼ��

BOOL CAutoUpdateApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();
 
    CString strEntranceToolVer;
	CString strEtranceServerPath;

    if (__argc == 3 )
	{
	//	int i_index = 1;
	/*	st_tempIDParameter.i_parameterNum = __argc-1;
		st_tempIDParameter.str_so.Format(_T("%s"), argv[i_index++]);
		st_tempIDParameter.str_employee.Format(_T("%s"), argv[i_index++]);		
		st_tempIDParameter.str_userLoadVer.Format(_T("%s"), argv[i_index++]);
		st_tempIDParameter.str_toolVer.Format(_T("%s"), argv[i_index++]);
		st_tempIDParameter.str_imagePath.Format(_T("%s"), argv[i_index++]);*/
		//param1: tool version
        strEntranceToolVer = CString(argv[1]);
		strEtranceServerPath = CString (argv[2]);
		//AfxMessageBox(strEntranceToolVer, MB_OK);
		//AfxMessageBox(strEtranceServerPath, MB_OK);
		//param2: server path
	}else
	{
		CString str_message = _T("");
		str_message = _T("The parameters Transferred from Entrance tool are not correct!");
		AfxMessageBox(str_message, MB_OK);
		return FALSE;
	}

	   //\\10.85.58.13\VerifySW\07.Tools\Factory_Tool\Entrance\TLFT_Common_v2.018_Entrance
    //\\10.82.212.78\YTT_upload\TLFT_Common_v2.018_Entrance
    bool bRes = m_fun.DoAutoUpdate((TCHAR*)LPCTSTR(strEntranceToolVer),(TCHAR*)LPCTSTR(strEtranceServerPath));

    if (bRes ==false)
	{
		return FALSE;
	}

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CAutoUpdateDlg dlg;
	//m_pMainWnd = &dlg;

	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
		m_fun.ExecuteEntranceTool();
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
