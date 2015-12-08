
// AutoUpdate.cpp : 定义应用程序的类行为。
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


// CAutoUpdateApp 构造

CAutoUpdateApp::CAutoUpdateApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CAutoUpdateApp 对象

CAutoUpdateApp theApp;


// CAutoUpdateApp 初始化

BOOL CAutoUpdateApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
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

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CAutoUpdateDlg dlg;
	//m_pMainWnd = &dlg;

	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
		m_fun.ExecuteEntranceTool();
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
