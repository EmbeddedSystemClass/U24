#include "stdafx.h"
#include "..\..\Modules\Common\CommonUtil\CommonDrawer.h"
#include "UnifyUI_FacTestTool.h"
#include "OthersMultiDlg.h"
#include <Gdiplus.h>

#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;

GUID GUIDQualcommDiagUsbClass	= {0x86e0d1e0, 0x8089, 0x11d0, {0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73}};

IMPLEMENT_DYNAMIC(COthersMultiDlg, CDialog)

COthersMultiDlg::COthersMultiDlg(CWnd* pParent)
	: CPageInterface(COthersMultiDlg::IDD)
	, m_h_module (NULL)
	, m_p_facTestToolInterface (NULL)
	, m_str_picasso(_T(""))
	, m_p_Image(NULL)
{
	m_str_errorCode = _T("PASS");
	m_str_errorMessage = _T("");
	m_st_return.i_slot = 1;
	m_p_dlgParent = (CUnifyUI_FacTestToolDlg*)pParent;
	m_i_multiColumn = 0;
	m_AppendImages.clear();
	m_b_haveReadedFAFlag = false;
	//m_PreRun_Flag = false;
	m_b_isSlotAvailable[0] = false;
	m_b_isSlotAvailable[1] = false;
	m_b_isSlotAvailable[2] = false;
	m_b_isSlotAvailable[3] = false;

	for (int i=0;i<10;i++)
	{
		m_b_returnRes[i] = true;
	}

	for (int i=0; i<DEVICE_NUMBER_OTHER; i++)
	{
		m_h_dllHandle[i] = NULL;
		m_p_ToolInterface[i] = NULL;
	}
}

COthersMultiDlg::~COthersMultiDlg()
{
	if (NULL != m_h_module)
	{
		/* End */
		//m_p_facTestToolInterface->End();

		/* Register */
		m_p_facTestToolInterface->Unregister(this, UI_ITEM);
		m_p_facTestToolInterface->Unregister(this, UI_MESSAGE);
		m_p_facTestToolInterface->Unregister(this, UI_PATTERN);
		m_p_facTestToolInterface->Unregister(this, UI_RESULT);
		m_p_facTestToolInterface->Unregister(this, UI_PICSDATA);
		m_p_facTestToolInterface->Unregister(this, UI_JIGCONTROL);
		m_p_facTestToolInterface->Unregister(this, UI_SHOWDIALOG);
		m_p_facTestToolInterface->Unregister(this, UI_RELAYBOARD);
		m_p_facTestToolInterface->Unregister(this, UI_PICASSO);

		/* Delete interface */
		p_fn_deleteFacTestToolInterface p_delInterface = NULL;
		p_delInterface = (p_fn_deleteFacTestToolInterface)::GetProcAddress(m_h_module, (LPCSTR)DLL_DEL_INTERFACE);
		if(p_delInterface && m_p_facTestToolInterface)
		{
			p_delInterface(m_p_facTestToolInterface);
		}

		::FreeLibrary(m_h_module);
		m_h_module = NULL;
	}

	for (int i=0; i<DEVICE_NUMBER_OTHER; i++)
	{
		if (NULL != m_h_dllHandle[i])
		{
			/* End */
			if (m_b_isSlotAvailable[i]) m_p_ToolInterface[i]->End(i);

			/* Register */
			m_p_ToolInterface[i]->Unregister(this, UI_ITEM);
			m_p_ToolInterface[i]->Unregister(this, UI_MESSAGE);
			m_p_ToolInterface[i]->Unregister(this, UI_PATTERN);
			m_p_ToolInterface[i]->Unregister(this, UI_RESULT);
			m_p_ToolInterface[i]->Unregister(this, UI_PICSDATA);
			m_p_ToolInterface[i]->Unregister(this, UI_JIGCONTROL);
			m_p_ToolInterface[i]->Unregister(this, UI_SHOWDIALOG);
			m_p_ToolInterface[i]->Unregister(this, UI_RELAYBOARD);
			m_p_ToolInterface[i]->Unregister(this, UI_PICASSO);

			/* Delete interface */
			p_fn_deleteFacTestToolInterface p_delInterface = NULL;
			p_delInterface = (p_fn_deleteFacTestToolInterface)::GetProcAddress(m_h_dllHandle[i], (LPCSTR)DLL_DEL_INTERFACE);
			if(m_h_dllHandle[i] && m_p_ToolInterface[i])
			{
				p_delInterface(m_p_ToolInterface[i]);
			}

			::FreeLibrary(m_h_dllHandle[i]);
			m_h_dllHandle[i] = NULL;
		}
	}

	/* delete Image pointer */
	if(m_p_Image)
	{
		delete m_p_Image;
		m_p_Image = NULL;
	}

	GdiplusShutdown(gdiplusToken);

	m_brush.DeleteObject();
}

void COthersMultiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    
	for(int i = 0; i < DEVICE_NUMBER_OTHER; i++)
	{
		DDX_Control(pDX, IDC_STATIC1 + i, m_label_progress[i]);
		DDX_Control(pDX, IDC_EDIT_RF_ID + i, m_edit_picasso[i]);
	}

	DDX_Control(pDX, IDC_RF_STATIC_TESTITEM, m_label_testItem);
	DDX_Control(pDX, IDC_RF_STATIC_ID, m_label_ID);
	DDX_Control(pDX, IDC_STATIC_PRESCAN, m_label_Prescan);
	DDX_Control(pDX, IDC_LIST_RF_MESSAGE, m_listctrl_message);
	DDX_Text(pDX, IDC_EDIT_RF_ID, m_str_picasso);
	DDV_MaxChars(pDX, m_str_picasso, PICASSO_LENGTH);
	DDX_Control(pDX, IDC_LIST_RF_TESTITEM, m_listbox_testItem);
	DDX_Control(pDX, IDC_LIST_TESTITEM2, m_listbox_testItem2);
	DDX_Control(pDX, IDC_LIST_TESTITEM3, m_listbox_testItem3);
	DDX_Control(pDX, IDC_LIST_TESTITEM4, m_listbox_testItem4);
}

BEGIN_MESSAGE_MAP(COthersMultiDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_DEVICECHANGE()
	ON_WM_ERASEBKGND()
	//ON_NOTIFY ( NM_CUSTOMDRAW, IDC_LIST_RF_MESSAGE, OnCustomdrawList )
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &COthersMultiDlg::OnBnClickedButtonNext)
END_MESSAGE_MAP()

BOOL COthersMultiDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	/* Get MainDlg parameter */
	if (m_p_dlgParent == NULL)
	{
		ShowResultDlg(CommErr_UI_Init_Diag_Fail, _T("m_p_dlgParent is NULL"));
		return FALSE;
	}
	if (!m_p_dlgParent->GetUIIDParameter(m_st_uiParameter))
	{
		ShowResultDlg(CommErr_UI_Init_Diag_Fail, _T("UI parameters is NULL"));
		m_p_dlgParent->OnCancel();
		return FALSE;
	}
	if (!m_p_dlgParent->GetStationListMap(m_map_StationIndexMap))
	{
		ShowResultDlg(CommErr_UI_Init_Diag_Fail, _T("Station List is NULL"));
		m_p_dlgParent->OnCancel();
		return FALSE;
	}
	if (!m_p_dlgParent->GetUIControlParameter(m_st_uiControl))
	{
		ShowResultDlg(CommErr_UI_Init_Diag_Fail, _T("UI control parameters is NULL"));
		m_p_dlgParent->OnCancel();
		return FALSE;
	}
	if (!m_p_dlgParent->GetTestKeyMap(m_map_keyvalue))
	{
		ShowResultDlg(CommErr_UI_Init_Diag_Fail, _T("Test key items is NULL"));
		m_p_dlgParent->OnCancel();
		return FALSE;
	}
	/* Get Map */
	GetStationVsDllMap(m_map_StationVsDll);

	if (m_st_uiControl.b_PreScan)
	{
		GetDlgItem(IDC_BUTTON_NEXT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_PRESCAN)->ShowWindow(SW_SHOW);
	    GetDlgItem(IDC_STATIC_PRESCAN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PRESCAN)->EnableWindow(TRUE);		
		((CEdit*)GetDlgItem(IDC_EDIT_PRESCAN))->SetLimitText(PICASSO_LENGTH);
	}


	LogMsg(_T("begin init"));
	/* Init */
	if (!Init())
	{
		m_p_dlgParent->OnCancel();
		return FALSE;
	}

	/* Set focus */
	CString str_foucs;
	m_p_dlgParent->PostMessage(WM_USER_SETFOCUS, (WPARAM)_T(""), (LPARAM)&str_foucs);

	return TRUE; 
}

bool COthersMultiDlg::Init(void)
{
	//m_str_invalidepicasso = _T("");
	if(m_st_uiControl.b_AutoRun)
	{   
		m_p_dlgParent->Begin();
		DeviceDetectSetup();//RegisterDeviceNotification
	}

	/* Init UI */
	if (!InitUI())
	{
		ShowResultDlg(CommErr_UI_Init_Diag_Fail, m_str_errorMessage + _T("\n初始化对话框失败"));
		return false;
	}

	LogMsg(_T("begin load dll"));
	/* Load DLL */
	if (!LoadDLL())
	{
		ShowResultDlg(CommErr_UI_DLL_Load_Fail, m_str_errorMessage + _T("\n加载DLL 失败"));
		return false;
	}

	LogMsg(_T("begin set parameter to dll"));
	/* Set value to DLL */
	if (!SetParameterToDLL())
	{
		ShowResultDlg(CommErr_UI_Interface_Setparameter_Fail, m_str_errorMessage);
		return false;
	}

	LogMsg(_T("begin dll begin"));
	/* DLL Begin */
	if (!DLLBegin())
	{
		ShowResultDlg(CommErr_UI_Interface_Begin_Fail, m_str_errorMessage + _T("\nDLL Begin 失败"));
		return false;
	}

	LogMsg(_T("begin get parameter"));
	/* Set value to DLL */
	if (!GetParameterFromDLL())
	{
		ShowResultDlg(CommErr_UI_Interface_Getparameter_Fail, m_str_errorMessage);
		return false;
	}

	LogMsg(_T("end dll begin"));
	/* Init test item list */
	if (!InitTestItemList())
	{
		ShowResultDlg(CommErr_UI_Init_TestItem_Fail, m_str_errorMessage + _T("\n获取Test Item 失败"));
		return false;
	}

	return true;
}

bool COthersMultiDlg::InitUI(void)
{
	/* Define Color*/
	COLORREF colour_widgetText = RGB(0,0,0);

#ifdef RD_MODE
	colour_dialogBKG = BKGCOLOUR_RD;
#else
	if (m_st_uiParameter.b_tsFlag == true)
	{
		colour_dialogBKG = BKGCOLOUR_TS;
	}
	else
	{
		colour_dialogBKG = BKGCOLOUR_FACTORY;
	}
#endif
    
    for(int i = 0;i<DEVICE_NUMBER_OTHER;i++)
	{
		m_label_progress[i].SetBkColor(RGB(255,255,255));
		m_label_progress[i].SetFontBold(TRUE);
		m_label_progress[i].SetFontSize(14);
	}

	m_brush.CreateSolidBrush(colour_dialogBKG);

	m_label_testItem.SetTextColor(colour_widgetText);
	m_label_testItem.SetBkColor(colour_dialogBKG);

	m_label_ID.SetTextColor(colour_widgetText);
	m_label_ID.SetBkColor(colour_dialogBKG);

	m_label_Prescan.SetTextColor(colour_widgetText);
	m_label_Prescan.SetBkColor(colour_dialogBKG);

	/* Init message list */
	InitMessageList(true);

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	return true;
}

bool COthersMultiDlg::LoadDLL(void)
{
	/* Confirm DLL Name */
	CString str_dllName;
	if (m_map_StationVsDll.find(m_st_uiParameter.str_station) != m_map_StationVsDll.end())
	{
		str_dllName = m_map_StationVsDll[m_st_uiParameter.str_station];
	}
	else
	{
		/*m_str_errorMessage.Format(_T("Station % not exist!"), m_st_uiParameter.str_station);
		return false;*/
		LogMsg(_T("New station found ,use default FT.dll"));
		str_dllName = FT_DLLNAME;
	}

	/* Load dll */
	m_h_module = LoadLibrary(str_dllName);
	if (m_h_module == NULL)
	{ 
		LogMsg(_T("Load ") + str_dllName + _T(" Fail! Use") + m_st_uiParameter.str_station + _T(".dll, and retry"));
		str_dllName = m_st_uiParameter.str_station + _T(".dll");
		m_h_module = LoadLibrary(str_dllName);

		if (m_h_module == NULL)
		{
			m_str_errorMessage = _T("Load ") + str_dllName + _T(" Fail! Use") + m_st_uiParameter.str_station + _T(".dll, and retry");
			return false;
		}
	}

	/* Get function address */
	p_fn_newFacTestToolInterface p_newInterface = NULL;
	p_newInterface = (p_fn_newFacTestToolInterface)::GetProcAddress(m_h_module, (LPCSTR)DLL_NEW_INTERFACE);
	if (p_newInterface == NULL)
	{
		m_str_errorMessage = _T("Initial p_fn_newFacTestToolInterface Fail!");
		return false;
	}

	/* New interface */
	p_newInterface(&m_p_facTestToolInterface);
	if (m_p_facTestToolInterface == NULL)
	{
		m_str_errorMessage = _T("New IFacTestToolInterface Fail!");
		return false;
	}

	m_p_facTestToolInterface->DefineNotify(UI_PICASSO);

	/* Register */
	m_p_facTestToolInterface->Register(this, UI_ITEM);
	m_p_facTestToolInterface->Register(this, UI_MESSAGE);
	m_p_facTestToolInterface->Register(this, UI_PATTERN);
	m_p_facTestToolInterface->Register(this, UI_RESULT);
	m_p_facTestToolInterface->Register(this, UI_PICSDATA);
	m_p_facTestToolInterface->Register(this, UI_JIGCONTROL);
	m_p_facTestToolInterface->Register(this, UI_SHOWDIALOG);
	m_p_facTestToolInterface->Register(this, UI_RELAYBOARD);
	m_p_facTestToolInterface->Register(this, UI_PICASSO);


	for (int i=0; i<DEVICE_NUMBER_OTHER; i++)
	{
		/* Load dll2 */
		m_h_dllHandle[i] = LoadLibrary(str_dllName);
		if (m_h_dllHandle[i] == NULL)
		{ 
			LogMsg(_T("Load ") + str_dllName + _T(" Fail! Use") + m_st_uiParameter.str_station + _T(".dll, and retry"));
			str_dllName = m_st_uiParameter.str_station + _T(".dll");
			m_h_dllHandle[i] = LoadLibrary(str_dllName);

			if (m_h_dllHandle[i] == NULL)
			{
				m_str_errorMessage = _T("Load ") + str_dllName + _T(" Fail! Use") + m_st_uiParameter.str_station + _T(".dll, and retry");
				return false;
			}
		}

		/* Get function address */
		p_fn_newFacTestToolInterface p_newInterface = NULL;
		p_newInterface = (p_fn_newFacTestToolInterface)::GetProcAddress(m_h_dllHandle[i], (LPCSTR)DLL_NEW_INTERFACE);
		if (p_newInterface == NULL)
		{
			m_str_errorMessage = _T("Initial p_fn_newFacTestToolInterface Fail!");
			return false;
		}

		/* New interface */
		p_newInterface(&m_p_ToolInterface[i]);
		if (m_p_ToolInterface[i] == NULL)
		{
			m_str_errorMessage = _T("New IFacTestToolInterface Fail!");
			return false;
		}

		m_p_ToolInterface[i]->DefineNotify(UI_PICASSO);

		/* Register */
		m_p_ToolInterface[i]->Register(this, UI_ITEM);
		m_p_ToolInterface[i]->Register(this, UI_MESSAGE);
		m_p_ToolInterface[i]->Register(this, UI_PATTERN);
		m_p_ToolInterface[i]->Register(this, UI_RESULT);
		m_p_ToolInterface[i]->Register(this, UI_PICSDATA);
		m_p_ToolInterface[i]->Register(this, UI_JIGCONTROL);
		m_p_ToolInterface[i]->Register(this, UI_SHOWDIALOG);
		m_p_ToolInterface[i]->Register(this, UI_RELAYBOARD);
		m_p_ToolInterface[i]->Register(this, UI_PICASSO);
	}
	return true;
}

bool COthersMultiDlg::DLLBegin(void)
{
	/* Begin */
	/*if(!m_p_facTestToolInterface->Begin())
	{
		if (m_str_errorMessage == _T(""))
		{
			m_str_errorMessage = _T("Dll Begin Fail!");
		}
		return false;
	}*/
	
	if(m_st_uiControl.str_Device1==_T("1")) m_b_isSlotAvailable[0] = true;
	if(m_st_uiControl.str_Device2==_T("1")) m_b_isSlotAvailable[1] = true;
	if(m_st_uiControl.str_Device3==_T("1")) m_b_isSlotAvailable[2] = true;
	if(m_st_uiControl.str_Device4==_T("1")) m_b_isSlotAvailable[3] = true;

	for (int i=0; i<DEVICE_NUMBER_OTHER&&m_b_isSlotAvailable[i]; i++)
	{
		if(!m_p_ToolInterface[i]->Begin(i))
		{
			if (m_str_errorMessage == _T(""))
			{
				m_str_errorMessage = _T("Dll Begin Fail!");
			}
			return false;
		}
	}
	
	return true;
}

bool COthersMultiDlg::PostRun()
{
	return true;
}
/*****************************************************************************
* Function name: InitTestItemList     
* Summary      : get test item from dll and show them in UI
* Parameters   : void
*                
* Return       : void
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Nancy.Yang    2011/07/06          First version             
*****************************************************************************/
bool COthersMultiDlg::InitTestItemList(void)
{
	if (m_p_facTestToolInterface == NULL)
	{
		m_str_errorMessage = _T("DLL do not loaded!");
		return false;
	}

	char sz_testItem[100] = {0};
	CString str_testItem;
	int i_itemCount = m_p_ToolInterface[0]->GetItemCount();

	for (int i=0; i<i_itemCount; i++)
	{
		ZeroMemory(sz_testItem,sizeof(sz_testItem));

		if (m_p_ToolInterface[0]->GetItemByIndex(i, sz_testItem, 100))
		{
			str_testItem = ConvertString(sz_testItem);
			m_listbox_testItem.AddString(str_testItem);
			m_listbox_testItem2.AddString(str_testItem);
			m_listbox_testItem3.AddString(str_testItem);
			m_listbox_testItem4.AddString(str_testItem);
			m_map_testItem[str_testItem] = i;
		}
		else
		{
			m_str_errorMessage = _T("Get test item from DLL Fail!");
			return false;
		}
	}

	return true;
}

void COthersMultiDlg::InitMessageList(bool b_multiColumn)
{
	if (b_multiColumn)
	{
		int i_colsWidth[14] = {50,50,72, 60, 60, 50, 50, 50, 50, 70, 38, 50, 50, 200};
		m_listctrl_message.InsertColumn(0, _T("Decive"), LVCFMT_LEFT, i_colsWidth[0]);
		m_listctrl_message.InsertColumn(1, _T("Picasso"), LVCFMT_LEFT, i_colsWidth[1]);
		m_listctrl_message.InsertColumn(2, _T("Time"), LVCFMT_LEFT, i_colsWidth[2]);
		m_listctrl_message.InsertColumn(3, _T("ItemCode"), LVCFMT_RIGHT, i_colsWidth[3]);
		m_listctrl_message.InsertColumn(4, _T("Band"), LVCFMT_LEFT, i_colsWidth[4]);
		m_listctrl_message.InsertColumn(5, _T("Channel"), LVCFMT_LEFT, i_colsWidth[5]);
		m_listctrl_message.InsertColumn(6, _T("ErrorCode"), LVCFMT_RIGHT, i_colsWidth[6]);
		m_listctrl_message.InsertColumn(7, _T("UpperBound"), LVCFMT_RIGHT, i_colsWidth[7]);
		m_listctrl_message.InsertColumn(8, _T("LowerBound"), LVCFMT_RIGHT, i_colsWidth[8]);
		m_listctrl_message.InsertColumn(9, _T("EvaluatedValue"), LVCFMT_RIGHT, i_colsWidth[9]);
		m_listctrl_message.InsertColumn(10, _T("Unit"), LVCFMT_RIGHT, i_colsWidth[10]);
		m_listctrl_message.InsertColumn(11, _T("Result"), LVCFMT_CENTER, i_colsWidth[11]);
		m_listctrl_message.InsertColumn(12, _T("Elapse Time"), LVCFMT_RIGHT, i_colsWidth[12]);
		m_listctrl_message.InsertColumn(13, _T("Message"), LVCFMT_LEFT, i_colsWidth[13]);
		
	}
	else
	{   m_listctrl_message.InsertColumn(0, _T("Decive"), LVCFMT_LEFT, 50);
		m_listctrl_message.InsertColumn(12, _T("Message"), LVCFMT_LEFT, 500);
	}

	m_listctrl_message.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listctrl_message.SetView(LV_VIEW_DETAILS);

	return;
}

void COthersMultiDlg::ClearMessageList(void)
{
	m_listctrl_message.DeleteAllItems();
}

bool COthersMultiDlg::GetPicasso(std::map<int, CString> &map_picasso)
{
	/* Clear UI After Go */
	ClearMessageList();
	ResetLatestClock();

	if(m_st_uiControl.str_Device1==_T("0")
		&&m_st_uiControl.str_Device2==_T("0")
		&&m_st_uiControl.str_Device3==_T("0")
		&&m_st_uiControl.str_Device4==_T("0"))
	{
		AfxMessageBox(_T("请在PECONFIG中至少开启一个Device,再次开启Tool"));
//		return false;
	}

	GetDlgItem(IDC_EDIT_RF_ID)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);

	for(int i=1;i<5;i++)
	{
		m_label_progress[i-1].SetBkColor(RGB(255,255,255));
	}
	GetDlgItem(IDC_STATIC1)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC2)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC3)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC4)->SetWindowText(_T(""));

	USES_CONVERSION;

	Result_Flag  = 0;
	Result_Flag2 = 0;
	Result_Flag3 = 0;
	Result_Flag4 = 0;

	m_str_picasso = _T("");
	m_str_picasso2 = _T("");
	m_str_picasso3 = _T("");
	m_str_picasso4 = _T("");

	/* Check if scan Picasso */
	if (m_st_uiControl.b_ScanPicasso)
	{
		if (!CheckScan())
		{
			return false;
		}
		GetDlgItem(IDC_EDIT_RF_ID)->GetWindowText(map_picasso[1]);
		GetDlgItem(IDC_EDIT_RF_ID2)->GetWindowText(map_picasso[2]);
		GetDlgItem(IDC_EDIT_RF_ID3)->GetWindowText(map_picasso[3]);
		GetDlgItem(IDC_EDIT_RF_ID4)->GetWindowText(map_picasso[4]);
	}
	else
	{
		GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));
		GetDlgItem(IDC_EDIT_RF_ID2)->SetWindowText(_T(""));
		GetDlgItem(IDC_EDIT_RF_ID3)->SetWindowText(_T(""));
		GetDlgItem(IDC_EDIT_RF_ID4)->SetWindowText(_T(""));
	}

	/* PreRun (Only Call once) */
	if (m_p_facTestToolInterface == NULL)
	{
		m_st_return.str_errorMsg = _T("Load DLL Fail!");
		m_st_return.str_errorcode = CommErr_UI_Interface_Prerun_Fail;
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
		GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));
		return false;
	}
	if (!m_p_facTestToolInterface->PreRun())
	{   

		if (m_str_errorMessage != _T(""))
		{
			m_st_return.str_errorMsg = m_str_errorMessage;
		}
		else
		{
			m_st_return.str_errorMsg = _T("PreRun Fail!");
		}

		if (m_str_errorCode.GetLength() > 4)
		{
			m_st_return.str_errorcode = m_str_errorCode;
		}
		else
		{
			m_st_return.str_errorcode = CommErr_UI_Interface_Prerun_Fail;
		}

		m_p_facTestToolInterface ->PostRun();

		m_st_return.i_slot = 1;
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
		GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));

		m_st_return.i_slot = 2;
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
		GetDlgItem(IDC_EDIT_RF_ID2)->SetWindowText(_T(""));

		m_st_return.i_slot = 3;
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
		GetDlgItem(IDC_EDIT_RF_ID3)->SetWindowText(_T(""));

		m_st_return.i_slot = 4;
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
		GetDlgItem(IDC_EDIT_RF_ID4)->SetWindowText(_T(""));

		return false;
	}

	if (m_st_uiControl.str_Device1 == _T("0"))   /*Device 1*/
	{
		GetDlgItem(IDC_STATIC1)->SetWindowText(_T("UI NO USE"));

		m_st_return.str_errorMsg = _T("[1]-----NO USE");
		m_st_return.str_errorcode = _T("UI NO USE");
		m_st_return.i_slot = 1;
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
		GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));
		Result_Flag = 1;
	}

	if (m_st_uiControl.str_Device2 == _T("0"))   //Device 2
	{
		GetDlgItem(IDC_STATIC2)->SetWindowText(_T("UI NO USE"));
		m_st_return.str_errorMsg = _T("[2]-----NO USE");
		m_st_return.str_errorcode = _T("UI NO USE");
		m_st_return.i_slot = 2;
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
		GetDlgItem(IDC_EDIT_RF_ID2)->SetWindowText(_T(""));
		Result_Flag2 = 1;
	}

	if (m_st_uiControl.str_Device3 ==_T("0"))  //Device 3
	{
		GetDlgItem(IDC_STATIC3)->SetWindowText(_T("UI NO USE"));
		m_st_return.str_errorMsg = _T("[3]-----NO USE");
		m_st_return.str_errorcode = _T("UI NO USE");
		m_st_return.i_slot = 3;
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
		GetDlgItem(IDC_EDIT_RF_ID3)->SetWindowText(_T(""));
		Result_Flag3 = 1;
	}

	if (m_st_uiControl.str_Device4 ==_T("0"))  //Device 4
	{
		GetDlgItem(IDC_STATIC4)->SetWindowText(_T("UI NO USE"));
		m_st_return.str_errorMsg = _T("[4]-----NO USE");
		m_st_return.str_errorcode = _T("UI NO USE");
		m_st_return.i_slot = 4;
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
		GetDlgItem(IDC_EDIT_RF_ID4)->SetWindowText(_T(""));
		Result_Flag4 = 1;
	}

#ifndef FACTORY_MODE
	if (!m_st_uiControl.b_ReadFA)
	{
		/* Not Read FA */
		LogMsg(_T("RD mode, b_ReadFA is false, skip to get picasso"));
		return true;
	}
#endif

	if (m_st_uiParameter.str_station == m_st_uiParameter.str_firstProcess)
	{
		/* Not Read FA */
		GetDlgItem(IDC_EDIT_RF_ID)->GetWindowText(m_str_picasso);
		GetDlgItem(IDC_EDIT_RF_ID2)->GetWindowText(m_str_picasso2);
		GetDlgItem(IDC_EDIT_RF_ID3)->GetWindowText(m_str_picasso3);
		GetDlgItem(IDC_EDIT_RF_ID4)->GetWindowText(m_str_picasso4);
	}
	else
	{	
		/* Read FA */
		m_b_haveReadedFAFlag = false;
		char sz_faData[512] = {0};
		char sz_faData2[512] = {0};
		char sz_faData3[512] = {0};
		char sz_faData4[512] = {0};
		if (m_st_uiControl.i_FAArea == OLDFA)
		{
			if (!m_p_facTestToolInterface->GetFAData(1,sz_faData,sizeof(sz_faData)))
			{
				m_p_facTestToolInterface ->PostRun();

				m_st_return.str_errorMsg = _T("Read FA fail!");
				m_st_return.str_errorcode = CommErr_UI_Interface_GetFA_Fail;
				m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
				GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));

				return false;
			}
			char sz_picasso[15] = {0};
			m_obj_faOperator.SetFA(sz_faData);
			m_obj_faOperator.GetPicassoNumber(sz_picasso);
			m_str_picasso = ConvertString(sz_picasso);

			/* FA log*/ 	
			LogMsg(_T("GetFA DATA: ") + ConvertToVisibleString(sz_faData, 512));	
		}
		else if (m_st_uiControl.i_FAArea == NEWFA)
		{   
			if (m_st_uiControl.str_Device1!=_T("0"))   /*Device 1*/
			{
				if (!m_p_facTestToolInterface->GetFASector(1,0,sz_faData,sizeof(sz_faData)))
				{

					m_st_return.str_errorMsg = _T("[1]--Read FA fail!");
					m_st_return.str_errorcode = CommErr_UI_Interface_GetFA_Fail;
					m_st_return.i_slot = 1;
					m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
					GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));
					//GetDlgItem(IDC_EDIT_RF_ID)->EnableWindow(FALSE);
					//return false;
					Result_Flag = 1;
				}
				else
				{
					m_obj_newFA.SetSectorData(sz_faData);
					m_str_picasso = m_obj_newFA.GetPicassoNumber();
				
					if(!m_st_uiControl.b_ScanPicasso)
					{
						GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(m_str_picasso);
					}
					//GetDlgItem(IDC_EDIT_RF_ID)->EnableWindow(FALSE);
					/* FA log*/ 	
					LogMsg(_T("GetFA DATA: ") + ConvertString(sz_faData));
#ifdef RD_MODE
					if (m_str_picasso.GetLength() != 10)
					{
						m_st_return.str_errorMsg = _T("[1]---Invalid picasso!");
						m_st_return.str_errorcode = CommErr_UI_Picasso_Invalide;
						m_st_return.i_slot = 1;
						m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
						GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));
						Result_Flag = 1;
					}
#endif
				}
			}

			if (m_st_uiControl.str_Device2!=_T("0"))           //Device 2
			{
				if (!m_p_facTestToolInterface->GetFASector(2,0,sz_faData2,sizeof(sz_faData2)))
				{
					//m_p_facTestToolInterface ->PostRun();
					m_st_return.str_errorMsg = _T("[2]--Read FA fail!");
					m_st_return.str_errorcode = CommErr_UI_Interface_GetFA_Fail;
					m_st_return.i_slot = 2;
					m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
					GetDlgItem(IDC_EDIT_RF_ID2)->SetWindowText(_T(""));
					//GetDlgItem(IDC_EDIT_RF_ID2)->EnableWindow(FALSE);
					Result_Flag2 = 1;
					//return false;
				}
				else
				{
					m_obj_newFA.SetSectorData(sz_faData2);
					m_str_picasso2 = m_obj_newFA.GetPicassoNumber();
					if(!m_st_uiControl.b_ScanPicasso)
					{
						GetDlgItem(IDC_EDIT_RF_ID2)->SetWindowText(m_str_picasso2);
					}
					//GetDlgItem(IDC_EDIT_RF_ID2)->EnableWindow(FALSE);
					/* FA log*/ 	
					LogMsg(_T("GetFA DATA2: ") + ConvertString(sz_faData2));
#ifdef RD_MODE
					if (m_str_picasso2.GetLength() != 10)
					{
						m_st_return.str_errorMsg = _T("[2]--Invalid picasso!");
						m_st_return.str_errorcode = CommErr_UI_Picasso_Invalide;
						m_st_return.i_slot = 2;
						m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
						GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));
						Result_Flag2 = 1;
					}
#endif
				}
			}

			if (m_st_uiControl.str_Device3!=_T("0"))  //Device 3
			{
				if (!m_p_facTestToolInterface->GetFASector(3,0,sz_faData3,sizeof(sz_faData3)))
				{
					//m_p_facTestToolInterface ->PostRun();

					m_st_return.str_errorMsg = _T("[3]--Read FA fail!");
					m_st_return.str_errorcode = CommErr_UI_Interface_GetFA_Fail;
					m_st_return.i_slot = 3;
					m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
					GetDlgItem(IDC_EDIT_RF_ID3)->SetWindowText(_T(""));
					//GetDlgItem(IDC_EDIT_RF_ID3)->EnableWindow(FALSE);
					Result_Flag3 = 1;
					//return false;
				}
				else
				{
					m_obj_newFA.SetSectorData(sz_faData3);
					m_str_picasso3 = m_obj_newFA.GetPicassoNumber();

					if(!m_st_uiControl.b_ScanPicasso)
					{
						GetDlgItem(IDC_EDIT_RF_ID3)->SetWindowText(m_str_picasso3);
					}
					//GetDlgItem(IDC_EDIT_RF_ID3)->EnableWindow(FALSE);
					/* FA log*/ 	
					LogMsg(_T("GetFA DATA3: ") + ConvertString(sz_faData3));
#ifdef RD_MODE
					if (m_str_picasso3.GetLength() != 10)
					{
						m_st_return.str_errorMsg = _T("[3]--Invalid picasso!");
						m_st_return.str_errorcode = CommErr_UI_Picasso_Invalide;
						m_st_return.i_slot = 3;
						m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
						GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));
						Result_Flag3 = 1;
					}
#endif
				}

			}
			
			if (m_st_uiControl.str_Device4!=_T("0"))  //Device 4
			{
				if (!m_p_facTestToolInterface->GetFASector(4,0,sz_faData4,sizeof(sz_faData4)))
				{
					//m_p_facTestToolInterface ->PostRun();

					m_st_return.str_errorMsg = _T("[4]--Read FA fail!");
					m_st_return.str_errorcode = CommErr_UI_Interface_GetFA_Fail;
					m_st_return.i_slot = 4;
					m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
					GetDlgItem(IDC_EDIT_RF_ID4)->SetWindowText(_T(""));
					//GetDlgItem(IDC_EDIT_RF_ID4)->EnableWindow(FALSE);
					Result_Flag4 = 1;
					//return false;
				}
				else
				{
					m_obj_newFA.SetSectorData(sz_faData4);
					m_str_picasso4 = m_obj_newFA.GetPicassoNumber();

					if(!m_st_uiControl.b_ScanPicasso)
					{
						GetDlgItem(IDC_EDIT_RF_ID4)->SetWindowText(m_str_picasso4);
					}
					//GetDlgItem(IDC_EDIT_RF_ID4)->EnableWindow(FALSE);
					/* FA log*/ 	
					LogMsg(_T("GetFA DATA4: ") + ConvertString(sz_faData4));
#ifdef RD_MODE
					if (m_str_picasso4.GetLength() != 10)
					{
						m_st_return.str_errorMsg = _T("[4]--Invalid picasso!");
						m_st_return.str_errorcode = CommErr_UI_Picasso_Invalide;
						m_st_return.i_slot = 4;
						m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
						GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));
						Result_Flag4 = 1;
					}
#endif

				}
			}

			if(Result_Flag&&Result_Flag2&&Result_Flag3&&Result_Flag4)
			{   
				m_p_facTestToolInterface ->PostRun();
				return false;
			}

		}
		else
		{
			LogMsg(_T("INI Setting Fail."));	
			return false;
		}

		if (m_st_uiControl.b_ScanPicasso)
		{
			/* Compare Picasso */
			if (!ComparePicassoTag())
			{
				return false;
			}		
		}

		/* Have Read FA */
		m_b_haveReadedFAFlag = true;
	}

	/* Set value to DLL */

	map_picasso[1] = m_str_picasso;
	map_picasso[2] = m_str_picasso2;
	map_picasso[3] = m_str_picasso3;
	map_picasso[4] = m_str_picasso4;

	LogMsg(_T("GetFA Picasso1: ") + map_picasso[1]);
	LogMsg(_T("GetFA Picasso2: ") + map_picasso[2]);
	LogMsg(_T("GetFA Picasso3: ") + map_picasso[3]);
	LogMsg(_T("GetFA Picasso4: ") + map_picasso[4]);


	CString new_str_Picasso;
	CString strTemp;

	if (m_str_picasso==_T(""))
		new_str_Picasso.Format(_T("Empty_1%s"), _T("&"));
	else
		new_str_Picasso = m_str_picasso + _T("&");


	if (m_str_picasso2==_T(""))
	{
		strTemp.Format(_T("Empty_2%s"),_T("&"));
		new_str_Picasso += strTemp;
	}
	else
		new_str_Picasso += m_str_picasso2 + _T("&");


	if (m_str_picasso3==_T(""))
	{
		strTemp.Format(_T("Empty_3%s"),_T("&"));
		new_str_Picasso += strTemp;
	}
	else
		new_str_Picasso += m_str_picasso3 + _T("&");

	if (m_str_picasso4==_T(""))
	{
		strTemp = _T("Empty_4");
		new_str_Picasso += strTemp;
	}
	else
		new_str_Picasso += m_str_picasso4;

	LogMsg(_T("Set Parameter Picasso: ") + new_str_Picasso);
	m_p_facTestToolInterface->SetParameterValue("PICASSO", T2A(new_str_Picasso));

	return true;
}

bool COthersMultiDlg::SetResult_Other(int i_slot, CString str_result)
{
	CSingleLock singleLock(&m_obj_SetResultCritSection, TRUE);

	CString strTemp;

	if ((i_slot < 0) || (i_slot >= DEVICE_NUMBER_OTHER) || (str_result == _T("")))
	{
		strTemp.Format(_T("[%d] got a error code is null"),i_slot);
		LogMsg(strTemp);
		//return 0;
	}
	
	/* Check Result */
	if (str_result.CompareNoCase(_T("IGNORE")) == 0)
	{
		/* PASS --- End */
		//m_label_progress[i_slot].SetWindowText(str_result);
		//m_label_progress[i_slot].SetBkColor(RGB(0,255,0));

		/* Save result */
		//m_map_runResult_other[i_slot] = str_result;
	}
	else
	{	
		if (str_result.CompareNoCase(_T("FAIL")) != 0)
		{
			/* ErrorCode --- Save and Show UI */	
			if (m_p_dlgParent->m_b_NewErrCode)
			{
				
				if ((str_result.Left(2) != _T("DB")) && (str_result.Left(2) != _T("UI")))
				{
					char sz_errorCode[200]={0};
					char sz_genErrcode[200]={0};
					char sz_genErrMsg[MAX_BUFFER_SIZE]={0};
					char sz_errmsg[MAX_BUFFER_SIZE]={0};
					CStringToChar(str_result,sz_errorCode);
					bool bCim = false;
					if (!m_p_dlgParent->m_obj_QueryErrCode.QueryErrCode(sz_errorCode, sz_genErrcode, sz_genErrMsg, sz_errmsg,bCim))
					{
						strTemp.Format(_T("Disp1:Error Not Found:%s"),str_result);
						LogMsg(strTemp);
					}
					strTemp = ConvertString(sz_genErrcode);
				}
				else
				{
					strTemp = str_result;
				}
				m_label_progress[i_slot].SetWindowText(strTemp);
			}
			else
			{
				m_label_progress[i_slot].SetWindowText(str_result);
			}
			//m_label_progress[i_slot].SetBkColor(RGB(255,0,0));

			/* Save result */
			//m_map_runResult_other[i_slot] = str_result;

			//return 0;
		}
		else
		{
			/* FAIL --- End */
		}
	}

	/* Check handset if is running */
	//if (m_map_running[i_slot])
	//{
	//	::WaitForSingleObject(m_map_running[i_slot]->GetThreadHandle()->m_hThread, INFINITE);
	//	//::WaitForSingleObject(m_map_running[i_slot]->GetThreadHandle()->m_hThread, 10000);
	//	delete m_map_running[i_slot];
	//	m_map_running[i_slot] = NULL;
	//}
	//m_map_running.erase(i_slot);

	st_pageReturn st_return;
	st_return.i_slot = i_slot;
	st_return.str_errorcode = m_map_runResult_other[i_slot];
	NotifyMainUI_Other(st_return);

	if ( m_st_uiControl.b_AutoRun )
		m_map_runResult_other.erase(i_slot);

	return 0;
}

void COthersMultiDlg::Event(const char* str_event, long nParam)
{
	
	if (strcmp(str_event, UI_RESULT) == 0)
	{
		CSingleLock singleLock(&m_obj_UIResult, TRUE);
		/* for 2g/3g/FT/Wls result */
		/*if (m_i_multiColumn == 0)  
		{
			m_i_multiColumn = 2;
			InitMessageList(true);	
		}*/

		st_UIResult* p_st_uiResult = (st_UIResult*)nParam;	
		m_str_errorCode = ConvertString(p_st_uiResult->sz_errCode);
		m_str_errorMessage = ConvertString(p_st_uiResult->sz_msg);

		CString csResult = _T("");
		CString csDevice = _T("");
		ParsePara( ConvertString(p_st_uiResult->sz_result), csResult, csDevice);

		int i_slot = p_st_uiResult->i_slot;
		AddMessageList(p_st_uiResult);

		CString csItemCode = ConvertString(p_st_uiResult->sz_itemCode);
		if ( csItemCode.Compare(_T("#DLL#RUNDONE")) == 0 )
		{
			if (csResult.MakeUpper()==_T("PASS"))
			{
				m_map_runResult_other[i_slot] = _T("PASS");
			}
			else
			{
				m_map_runResult_other[i_slot] = m_str_errorCode;
				LogMsg(m_str_errorCode);
			}
			if (m_map_running[i_slot])
			{
				//::AfxMessageBox(_T("Before WaitForSingleObject")); //Neil Test
				//::WaitForSingleObject(m_map_running[i_slot]->GetThreadHandle()->m_hThread, INFINITE);
				//::AfxMessageBox(_T("After WaitForSingleObject")); //Neil Test
				
				//delete m_map_running[i_slot];
				//m_map_running[i_slot] = NULL;
			}
			//m_map_running.erase(i_slot);

			SetResult_Other( i_slot, m_map_runResult_other[i_slot]);
			Sleep(2000);
			m_b_returnRes[i_slot] = true;
		}

		//AfxMessageBox(str_errorCode);
		//m_st_return.i_slot = p_st_uiResult->i_slot;
		//m_st_return.str_errorcode = m_str_errorCode;
		//m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);

	}
	else if (strcmp(str_event, UI_MESSAGE) == 0)
	{
		///* for MMI result */
		//if (m_i_multiColumn == 0)
		//{
		//	m_i_multiColumn = 1;
		//	InitMessageList(false);
		//}
		st_UIMessage* p_st_uiMessage = (st_UIMessage*)nParam;
		CString str_message;
		str_message = ConvertString(p_st_uiMessage->sz_message);
		AddMessage(str_message);
	}
	else if (strcmp(str_event, UI_PATTERN) == 0)
	{
		/* for MMI(EM、Carema) picture display */
		st_UIPattern* p_st_uiPattern = (st_UIPattern*)nParam;
		CString str_path;
		str_path = ConvertString(p_st_uiPattern->sz_pattern);

		if (p_st_uiPattern->b_append)
		{
			if(!str_path.IsEmpty())
			{
				CSingleLock waitLock(&m_WaitingObj, TRUE);
				CSingleLock singleLock(&m_LockObj, TRUE);

				m_AppendImages.push_back(str_path);

				/* Get picture display rect */
				CRect rect;
				CStatic *pWnd = (CStatic*)GetDlgItem(IDC_STATIC_PICTURE);
				if (pWnd != NULL)
				{
					pWnd->GetWindowRect(&rect);			
					ScreenToClient(&rect);					
					InvalidateRect(&rect,TRUE);
				}
			}
		} 
		else
		{
			SetDisplayImage(str_path);
		}

	}
	else if (strcmp(str_event, UI_ITEM) == 0)
	{
		/* focus on current test item */
		st_UIItem* p_st_uiItem = (st_UIItem*)nParam;
		CString str_testItem;
		str_testItem = ConvertString(p_st_uiItem->sz_item);
        
		CString str_para_testItem = _T("");
		CString str_Device = _T("");

		ParsePara(str_testItem,str_para_testItem,str_Device);
        if(str_Device==_T("1"))
		{
        m_listbox_testItem.SetCurSel(m_map_testItem[str_para_testItem]);
		}
		else if(str_Device==_T("2"))
		{
        m_listbox_testItem2.SetCurSel(m_map_testItem[str_para_testItem]);
		}
		else if(str_Device==_T("3"))
		{
			m_listbox_testItem3.SetCurSel(m_map_testItem[str_para_testItem]);
		} 
		else if(str_Device==_T("4"))
		{
			m_listbox_testItem4.SetCurSel(m_map_testItem[str_para_testItem]);
		} 
		
	}
	else if (strcmp(str_event, UI_PICSDATA) == 0)
	{
		/* pics data*/
		st_PICSData* p_st_picsData = (st_PICSData*)nParam;
		//m_p_dlgParent->SetPICSData(1, p_st_picsData->sz_key, p_st_picsData->sz_value);
		m_p_dlgParent->SetPICSData(p_st_picsData->i_slot, p_st_picsData->sz_key, p_st_picsData->sz_value);

	}
	else if (strcmp(str_event, UI_JIGCONTROL) == 0)
	{
		st_JigRequest* p_st_jigRequest = (st_JigRequest*)nParam;
		CString str_request;
		str_request = ConvertString (p_st_jigRequest->sz_switch);
      
		if (str_request == _T("ON"))
		{
			const BYTE b_command[9] = {"out01on#"};//move in (l/p sensor)
			if (!m_p_dlgParent->SendCommand(b_command, 8))
			{
				m_p_facTestToolInterface->ResopnseToJig(false);
			} 
			else 
			{
				m_p_facTestToolInterface->ResopnseToJig(true);
			}
		}
		else if(str_request == _T("OFF"))
		{
			const BYTE b_command[10] = {"out01off#"};//move away (l/p sensor) 
			if (!m_p_dlgParent->SendCommand(b_command, 9))
			{
				m_p_facTestToolInterface->ResopnseToJig(false);
			} 
			else 
			{
				m_p_facTestToolInterface->ResopnseToJig(true);
			}
		}
		else if(str_request == _T("IN"))
		{
			const BYTE b_command[9] = {"out02on#"};//close jig
			if (!m_p_dlgParent->SendCommand(b_command, 8))
			{
				m_p_facTestToolInterface->ResopnseToJig(false);
			} 
			else 
			{
				m_p_facTestToolInterface->ResopnseToJig(true);
			}
		}
		else if(str_request == _T("OUT"))
		{
			const BYTE b_command[10] = {"out02off#"};//open jig
			if (!m_p_dlgParent->SendCommand(b_command, 9))
			{
				m_p_facTestToolInterface->ResopnseToJig(false);
			} 
			else 
			{
				m_p_facTestToolInterface->ResopnseToJig(true);
			}
		}
		else if(str_request == _T("FOCUSON"))
		{
			const BYTE b_command[10] = {"out03on#"};//focus on
			if (!m_p_dlgParent->SendCommand(b_command, 8))
			{
				m_p_facTestToolInterface->ResopnseToJig(false);
			} 
			else 
			{
				m_p_facTestToolInterface->ResopnseToJig(true);
			}
		}
		else if(str_request == _T("FOCUSOFF"))
		{
			const BYTE b_command[10] = {"out03off#"};//focus off
			if (!m_p_dlgParent->SendCommand(b_command, 9))
			{
				m_p_facTestToolInterface->ResopnseToJig(false);
			} 
			else 
			{
				m_p_facTestToolInterface->ResopnseToJig(true);
			}
		}

	}
	else if (strcmp(str_event, UI_SHOWDIALOG) == 0)
	{
		st_ShowDialog* p_st_showDialog = (st_ShowDialog*)nParam;
		CString str_message;
		str_message = ConvertString(p_st_showDialog->sz_message);

		CPopup popdlg;
		popdlg.SetText(str_message);

		if (popdlg.DoModal() == IDOK)
		{
			m_p_facTestToolInterface->ResopnseToShowDlg("1");
		}
		else
		{
			m_p_facTestToolInterface->ResopnseToShowDlg("0");
		}
	}
	else if (strcmp(str_event, UI_POWERSUPPLY) == 0)
	{
		st_PowerSupply* p_st_powerSupply = (st_PowerSupply*)nParam;
		m_p_dlgParent->SetPowerSupply(p_st_powerSupply->i_slot,p_st_powerSupply->i_Seq,p_st_powerSupply->dw_Voltage,p_st_powerSupply->dw_current);
	}
	else if (strcmp(str_event, UI_RELAYBOARD) == 0)
	{
		st_RelayBoard* p_st_relayBoard = (st_RelayBoard*)nParam;
		m_p_dlgParent->SetRelayBoard(p_st_relayBoard->i_slot,p_st_relayBoard->i_Seq,p_st_relayBoard->b_OnOff);
	}
	/*auto run check routing*/
	else if (strcmp(str_event, UI_PICASSO) == 0)
	{
		CSingleLock singleLock(&m_obj_UIPicasso, TRUE);

		st_UIMessage* p_st_message = (st_UIMessage*)nParam;
		int i_slot = p_st_message->i_slot;
		CString str_picasso;
		str_picasso = ConvertString(p_st_message->sz_message);
	
		//m_label_progress[i_slot].SetWindowText(_T("10%"));
		m_edit_picasso[i_slot].SetWindowText(str_picasso);

		m_p_dlgParent->m_map_picasso[i_slot] = str_picasso;

		CString str_message;
		str_message.Format(_T("[%d] Get Picasso:%s"), p_st_message->i_slot, str_picasso);
		LogMsg(str_message);
		//SetMessage(str_message);
	}

	return;
}

 clock_t COthersMultiDlg::GetDurationTimeAndUpdateLatestClock(void)
 {
 	clock_t cl_curClock = clock();
 	clock_t cl_duration = cl_curClock - m_cl_LatestClock;
 	m_cl_LatestClock = cl_curClock;
 
 	return cl_duration;
 }
 
 void COthersMultiDlg::ResetLatestClock(void)
 {
 	m_cl_LatestClock = clock();
 }

/*****************************************************************************
* Function name: AddMessageList     
* Summary      : Add Message(2g/3g/Wls/FT) to massage list
* Parameters   : struct st_UIResult
*                
* Return       : void
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Nancy.Yang    2011/07/06          First version             
*****************************************************************************/
void COthersMultiDlg::AddMessageList(const st_UIResult* p_st_Msg)
{
	CString str_curTime;
	CString str_itemCode;
	CString str_band;
	CString str_channel;
	CString str_errcode;
	CString str_upper;
	CString str_lower;
	CString str_measured;
	CString str_unit;
	CString str_result;
	CString str_durationTime;
	CString str_message;

	//CString str_Parse_curTime;
	CString str_Parse_itemCode = _T("");
	CString str_Parse_band = _T("");
	CString str_Parse_channel = _T("");
	CString str_Parse_errcode = _T("");
	CString str_Parse_upper = _T("");
	CString str_Parse_lower = _T("");
	CString str_Parse_measured = _T("");
	CString str_Parse_unit = _T("");
	CString str_Parse_result = _T("");
	//CString str_Parse_durationTime = _T("");
	CString str_Parse_message = _T("");

	CString str_Device = _T("");

	/*get current time and calculate the test time*/
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	double d_durationTime = (double)GetDurationTimeAndUpdateLatestClock()/CLOCKS_PER_SEC;
	str_durationTime.Format(_T("%.4lf"), d_durationTime);
	str_curTime.Format(_T("%02d:%02d:%02d.%03d"), systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);

	/*convert char[] into CString*/
	str_itemCode = ConvertString(p_st_Msg->sz_itemCode);
    ParsePara(str_itemCode,str_Parse_itemCode,str_Device);	
	str_band = ConvertString( p_st_Msg->sz_band);
	ParsePara(str_band,str_Parse_band,str_Device);
	str_channel = ConvertString(p_st_Msg->sz_channel);
	ParsePara(str_channel,str_Parse_channel,str_Device);
	str_errcode = ConvertString(p_st_Msg->sz_errCode);
	ParsePara(str_errcode,str_Parse_errcode,str_Device);
	str_upper = ConvertString(p_st_Msg->sz_upper);
	ParsePara(str_upper,str_Parse_upper,str_Device);
	str_lower = ConvertString(p_st_Msg->sz_lower);
	ParsePara(str_lower,str_Parse_lower,str_Device);
	str_measured = ConvertString(p_st_Msg->sz_measured);
	ParsePara(str_measured,str_Parse_measured,str_Device);
	str_unit = ConvertString(p_st_Msg->sz_unit);
	ParsePara(str_unit,str_Parse_unit,str_Device);
	str_result = ConvertString(p_st_Msg->sz_result);
	ParsePara(str_result,str_Parse_result,str_Device);
	str_message = ConvertString(p_st_Msg->sz_msg);
	ParsePara(str_message,str_Parse_message,str_Device);
    
	int nDevice = 0;
	nDevice = p_st_Msg->i_slot;
	str_Device.Format(_T("%d"), nDevice+1);
	/*insert test result to message list control*/
	int i_count = m_listctrl_message.GetItemCount();
	m_listctrl_message.InsertItem(i_count, str_Device);
	/*switch(nDevice)
	{
	case 1:
        m_listctrl_message.SetItemText(i_count, 1, m_str_picasso);
		break;
	case 2:
		m_listctrl_message.SetItemText(i_count, 1, m_str_picasso2);
		break;
	case 3:
		m_listctrl_message.SetItemText(i_count, 1, m_str_picasso3);
		break;
	case 4:
		m_listctrl_message.SetItemText(i_count, 1, m_str_picasso4);
		break;
	default:
		break;
	}*/
	m_listctrl_message.SetItemText(i_count, 1, m_p_dlgParent->m_map_picasso[nDevice]);
	m_listctrl_message.SetItemText(i_count, 2, str_curTime);
	m_listctrl_message.SetItemText(i_count, 3, str_Parse_itemCode);
	m_listctrl_message.SetItemText(i_count, 4, str_Parse_band);
	m_listctrl_message.SetItemText(i_count, 5, str_Parse_channel);
	m_listctrl_message.SetItemText(i_count, 6, str_Parse_errcode);
	m_listctrl_message.SetItemText(i_count, 7, str_Parse_upper);
	m_listctrl_message.SetItemText(i_count, 8, str_Parse_lower);
	m_listctrl_message.SetItemText(i_count, 9, str_Parse_measured);
	m_listctrl_message.SetItemText(i_count, 10, str_Parse_unit);
	m_listctrl_message.SetItemText(i_count, 11, str_Parse_result);
	m_listctrl_message.SetItemText(i_count, 12, str_durationTime);
	m_listctrl_message.SetItemText(i_count, 13, str_Parse_message);
	m_listctrl_message.EnsureVisible(i_count, FALSE);

	return;
}

/*****************************************************************************
* Function name: AddMessage     
* Summary      : Add Message(MMI) to massage list
* Parameters   : struct st_UIMessage
*                
* Return       : void
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Nancy.Yang    2011/07/06          First version             
*****************************************************************************/
void COthersMultiDlg::AddMessage(CString str_info)
{
	SYSTEMTIME st_time;
	GetLocalTime(&st_time);

	CString str_time;
	str_time.Format(_T("[%04d/%02d/%02d %02d:%02d:%02d]: "), st_time.wYear, st_time.wMonth, \
		st_time.wDay, st_time.wHour, st_time.wMinute, st_time.wSecond);
    
	/* Insert to List control */
	int i_count = m_listctrl_message.GetItemCount();
	m_listctrl_message.InsertItem(i_count, str_time + str_info);
	m_listctrl_message.EnsureVisible(i_count, FALSE);

	return;
}

bool COthersMultiDlg::ShowResultDlg(const CString str_errorCode, const CString str_message)
{
	LogMsg(str_errorCode + str_message);

  	m_dlg_result.SetText(str_errorCode, str_message);
  	m_dlg_result.DoModal();

	return true;
}

bool COthersMultiDlg::Run(void)
{
	if ( (m_st_uiParameter.str_station == STATION_CAMERA || m_st_uiParameter.str_station == STATION_LTE || m_st_uiParameter.str_station == STATION_2G3GTEST) ) {
		return RunHandsetMultiTest();
	}
	bool b_res = false;
	bool b_checkpicasso = true;
	CString str_errcode;
	CString str_msg;
	m_str_errorCode = _T("");
	m_str_errorMessage = _T("");
    CString str_Slot_Available1 =_T("1");
	CString str_Slot_Available2 =_T("1");
	CString str_Slot_Available3 =_T("1");
	CString str_Slot_Available4 =_T("1");
	CString str_Slot_Available_List = _T("");
    int i = 1;
	int i_count = 0;
 	/* Check Picasso *///not support
	/*if (!m_st_uiParameter.b_tsFlag)
	{
		if (m_st_uiControl.i_FAArea == OLDFA)
		{
			b_checkpicasso = CheckPicasso();
		}
		else if (m_st_uiControl.i_FAArea == NEWFA)
		{
			b_checkpicasso = CheckPicasso_New();
		}
		LogMsg(_T("Check Picasso Pass!"));
	}*/
    
	if (Result_Flag==1)
	{
		i_count++;
		LogMsg(_T("[1]-- has return fail to OnEnd"));
		str_Slot_Available1 = _T("0");
	}

	if (Result_Flag2==1)
	{
		i_count++;
		LogMsg(_T("[2]-- has return fail to OnEnd"));
		str_Slot_Available2 = _T("0");
	}

	if (Result_Flag3==1)
	{
		i_count++;
		LogMsg(_T("[3]-- has return fail to OnEnd"));
		str_Slot_Available3 = _T("0");
	}

	if (Result_Flag4==1)
	{
		i_count++;
		LogMsg(_T("[4]-- has return fail to OnEnd"));
		str_Slot_Available4 = _T("0");
	}

#ifdef FACTORY_MODE
	for (i=1;i<5;i++)
	{	
		if ((i==1 && Result_Flag!=1) || (i==2 && Result_Flag2!=1) || (i==3 && Result_Flag3!=1) || (i==4 && Result_Flag4!=1))
		{
			st_pageReturn m_st_return;
			if (!m_st_uiParameter.b_tsFlag)
			{ 
				CString strTemp;
				strTemp.Format(_T("[%d]--check picasso rule/routing result is: %s"),i,m_map_picasso[i]);
				LogMsg(strTemp);
				/* Check Picasso rule and Check Routing Result */
				if (m_map_picasso.find(i) != m_map_picasso.end())
				{
					/* Get error code from invalid picasso list */
					
					CStringArray strary_picasso;
					CString str_errorCode;
					if (SpiltString(m_map_picasso[i], _T(","), strary_picasso))
					{
						if (strary_picasso.GetSize() == 2)
						{
							str_errorCode = strary_picasso.GetAt(1);
						} 
						else
						{
							continue;
						}
					}
					else
					{
						continue;
					}

					/* Show to UI */
					m_st_return.i_slot = i;

					switch(i)
					{
					case 1:
						str_Slot_Available1 = _T("0");
						Result_Flag = 1;
						break;
					case 2:
						str_Slot_Available2 = _T("0");
						Result_Flag2 = 1;
						break;
					case 3:
						str_Slot_Available3 = _T("0");
						Result_Flag3 = 1;
						break;
					case 4:
						str_Slot_Available4 = _T("0");
						Result_Flag4 = 1;
						break;
					default:
						break;
					}
					m_st_return.str_errorcode = str_errorCode;
					m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
					i_count++;
					//continue;
				}
			}
		}
	}
#endif

	if (b_checkpicasso)
	{
		/* Run */
		USES_CONVERSION;
		str_Slot_Available_List = str_Slot_Available1 + _T(",") + str_Slot_Available2 +_T(",") + str_Slot_Available3 +_T(",") + str_Slot_Available4; 
		LogMsg(_T("Slot Available list:")+str_Slot_Available_List);
		b_res = m_p_facTestToolInterface->SetParameterValue(SLOT_AVAILABLE_LIST,T2A(str_Slot_Available_List));
        
		if (i_count==4)
		{   
            i_count = 0;
			m_p_facTestToolInterface->PostRun();
			return false;
			
		}
		b_res = m_p_facTestToolInterface->Run();

		if (b_res)
		{
			LogMsg(_T("Run finished! and result is pass"));

			b_res = WriteFA("Y");
			if(b_res)
			{
				str_errcode = _T("PASS");
				str_msg = _T("");
			}
			else
			{				
				str_errcode = CommErr_UI_Interface_SetFA_Fail;
				str_msg = m_str_errorMessage;
			}
		}
		else
		{
			LogMsg(_T("Run finished! and result is fail"));
			str_errcode = m_str_errorCode;
			str_msg = m_str_errorMessage;
			b_res = WriteFA("N");
		}
		LogMsg(_T("Write FA finished"));
		
	}
	else
	{
		b_res = false;
		if (m_str_errorCode != _T(""))
		{
			str_errcode = m_str_errorCode;
			str_msg = m_str_errorMessage;
		}
	}

	m_p_facTestToolInterface->PostRun();
	LogMsg(_T("PostRun finished"));
    
    
	for(i = 1; i < 5; i++)
	{
		if ((i==1&&Result_Flag==1)||(i==2&&Result_Flag2==1)||(i==3&&Result_Flag3==1)||(i==4&&Result_Flag4==1))
		{
			continue;
		}
		else
		{
			st_pageReturn st_return;
			st_return.i_slot = i;
			st_return.str_errorcode = m_map_runResult_other[i];
			if(st_return.str_errorcode==_T(""))
			{
				str_msg.Format(_T("[slot%d]: error code is null, please confirm it!"),i);
			}
			else
			{
				str_msg.Format(_T("[slot%d]: ERROR CODE IS:%s"),i,st_return.str_errorcode);
				LogMsg(str_msg);
				NotifyMainUI_Other(st_return);
			}
		}
	}
	m_map_runResult_other.clear();
	m_map_picasso.clear();
	m_map_COMPort.clear();

	//m_st_return.str_errorcode = str_errcode;
	//m_st_return.str_errorMsg = str_msg;
	//m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);

	return b_res;
}

bool COthersMultiDlg::RunHandsetMultiTest(void)
{
	/* Clear UI After Go */
	ClearMessageList();
	ResetLatestClock();

	if(m_st_uiControl.str_Device1==_T("0")
		&&m_st_uiControl.str_Device2==_T("0")
		&&m_st_uiControl.str_Device3==_T("0")
		&&m_st_uiControl.str_Device4==_T("0"))
	{
		AfxMessageBox(_T("请在PECONFIG中至少开启一个Device,再次开启Tool"));
		return false;
	}

	GetDlgItem(IDC_EDIT_RF_ID)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);

	for(int i=1;i<5;i++)
	{
		m_label_progress[i-1].SetBkColor(RGB(255,255,255));
	}
	GetDlgItem(IDC_STATIC1)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC2)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC3)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC4)->SetWindowText(_T(""));

	USES_CONVERSION;
    
    Result_Flag  = 0;
    Result_Flag2 = 0;
	Result_Flag3 = 0;
	Result_Flag4 = 0;

	m_str_picasso = _T("");
	m_str_picasso2 = _T("");
	m_str_picasso3 = _T("");
	m_str_picasso4 = _T("");


	if (m_st_uiControl.str_Device1==_T("1")) {
		m_b_isSlotAvailable[0] = true;
	}
	else
	{
        GetDlgItem(IDC_STATIC1)->SetWindowText(_T("NO USE"));
		m_map_runResult_other[0] = "IGNORE";
		Result_Flag = 1;
	}

	if (m_st_uiControl.str_Device2==_T("1")) {
		m_b_isSlotAvailable[1] = true;
	}
	else
	{
        GetDlgItem(IDC_STATIC2)->SetWindowText(_T("NO USE"));
		m_map_runResult_other[1] = "IGNORE";
		Result_Flag2 = 1;
	}

	if (m_st_uiControl.str_Device3==_T("1")) {
		m_b_isSlotAvailable[2] = true;
	}
	else
	{
        GetDlgItem(IDC_STATIC3)->SetWindowText(_T("NO USE"));
		m_map_runResult_other[2] = "IGNORE";
		Result_Flag3 = 1;
	}

	if (m_st_uiControl.str_Device4==_T("1")) {
		m_b_isSlotAvailable[3] = true;
	}
	else
	{
        GetDlgItem(IDC_STATIC4)->SetWindowText(_T("NO USE"));
		m_map_runResult_other[3] = "IGNORE";
		Result_Flag4 = 1;
	}

	bool bRes_RunThread = true;
	for(int i_slot = 0; i_slot < DEVICE_NUMBER_OTHER; i_slot++)
	{
		if (m_b_isSlotAvailable[i_slot])
		{
			bRes_RunThread = true;

			CString str_picasso;
			m_edit_picasso[i_slot].GetWindowText(str_picasso);

			/* Run */
			if ( i_slot == 0 ) {
				m_map_running[i_slot] = new CHandsetOtherStation(&m_st_uiParameter, (i_slot), _T(""), str_picasso, m_p_ToolInterface[0], m_map_StationIndexMap, &m_st_uiControl);
			}else if ( i_slot == 1 ){
				m_map_running[i_slot] = new CHandsetOtherStation(&m_st_uiParameter, (i_slot), _T(""), str_picasso, m_p_ToolInterface[1], m_map_StationIndexMap, &m_st_uiControl);
			}else if ( i_slot == 2 ){
				m_map_running[i_slot] = new CHandsetOtherStation(&m_st_uiParameter, (i_slot), _T(""), str_picasso, m_p_ToolInterface[2], m_map_StationIndexMap, &m_st_uiControl);
			}else if ( i_slot == 3 ){
				m_map_running[i_slot] = new CHandsetOtherStation(&m_st_uiParameter, (i_slot), _T(""), str_picasso, m_p_ToolInterface[3], m_map_StationIndexMap, &m_st_uiControl);
			}
			if (m_map_running[i_slot] == NULL)
			{
				m_map_runResult_other[i_slot] = CommErr_UI_Run_Thread_Fail;

				bRes_RunThread =  false;
			}

			if (bRes_RunThread)
			{
				if (!m_map_running[i_slot]->HandsetRun())
				{
					m_map_runResult_other[i_slot] = CommErr_UI_Run_Thread_Fail;

					bRes_RunThread =  false;
				}
			}
			Sleep(500);
		}
	}

	for(int i_slot = 0; i_slot < DEVICE_NUMBER_OTHER; i_slot++)
	{
		/* Check handset if is running */
		if (m_b_isSlotAvailable[i_slot])
		{
			::WaitForSingleObject(m_map_running[i_slot]->GetThreadHandle()->m_hThread, INFINITE);
			//::WaitForSingleObject(m_map_running[i_slot]->GetThreadHandle()->m_hThread, 10000);
			
			delete m_map_running[i_slot];
			m_map_running[i_slot] = NULL;
		}
		SetResult_Other( i_slot, m_map_runResult_other[i_slot]);
		m_map_running.erase(i_slot);	
	}
	return true;
}

HBRUSH COthersMultiDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor == CTLCOLOR_DLG)
	{
		return m_brush;
	}
	return hbr;
}

BOOL COthersMultiDlg::OnEraseBkgnd(CDC * pDC)
{
	using namespace Gdiplus;		
	using namespace COMMONCONTROL;

	CRect rect;
	_DoubleBufferPolicy doubleBuffer;
	GetClientRect(&rect);
	Graphics graphics(doubleBuffer.BeginHDC(pDC->GetSafeHdc(),rect));

	DWORD dwResult =  colour_dialogBKG;
	SolidBrush BKG_Brush(Color(255, GetRValue(dwResult), GetGValue(dwResult), GetBValue(dwResult)));
	// Create a Rect object.
	Rect fillRect(rect.left, rect.top, rect.Width(), rect.Height());
	// Fill the rectangle.
	graphics.FillRectangle(&BKG_Brush, fillRect);

	if(m_p_Image)
	{
		CRect picture_rect;
		CStatic *pWnd = (CStatic*)GetDlgItem(IDC_STATIC_PICTURE);
		if (pWnd == NULL)
		{
			return FALSE;
		} 
		else
		{
			pWnd->GetWindowRect(&picture_rect);
			ScreenToClient(&picture_rect);
		}

		graphics.DrawImage(m_p_Image, picture_rect.left, picture_rect.top, picture_rect.Width(), picture_rect.Height());

		{
			CSingleLock waitLock(&m_WaitingObj);
			CSingleLock singleLock(&m_LockObj, TRUE);

			for(std::vector<CString>::iterator Iter = m_AppendImages.begin(); waitLock.IsLocked() == false && Iter != m_AppendImages.end(); Iter++)
			{
				Image image(Iter->GetBuffer());
				Iter->ReleaseBuffer();
				graphics.DrawImage(&image, picture_rect.left, picture_rect.top, picture_rect.Width(), picture_rect.Height());
			}
		}
	}

	return TRUE;
}

BOOL COthersMultiDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			m_p_dlgParent->PostMessage(WM_KEYDOWN, VK_RETURN, 0);
			
			break;

		case VK_ESCAPE:

			break;

		default:
			break;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}


bool COthersMultiDlg::ResetPage()
{
	m_str_picasso = _T("");

 	if (m_st_uiControl.b_PreScan)
 	{
 		GetDlgItem(IDC_EDIT_PRESCAN)->EnableWindow(TRUE);
 		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);
 
 		if (m_str_errorCode == _T("PASS"))
 		{
 			OnBnClickedButtonNext();
 		}
 	}
 	else
 	{
		if (m_st_uiControl.b_ScanPicasso)
		{
			GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));
			GetDlgItem(IDC_EDIT_RF_ID2)->SetWindowText(_T(""));
			GetDlgItem(IDC_EDIT_RF_ID3)->SetWindowText(_T(""));
			GetDlgItem(IDC_EDIT_RF_ID4)->SetWindowText(_T(""));
		} 		
 	}

	GetDlgItem(IDC_EDIT_RF_ID)->EnableWindow(TRUE);


	((CEdit   *)GetDlgItem(IDC_EDIT_RF_ID))->SetFocus();

	m_str_errorCode = _T("");
	m_str_errorMessage = _T("");

	return true;

}

bool COthersMultiDlg::WriteFA(char* sz_result)
{
	return true;

	bool b_writeFAFlag = false;

#ifdef FACTORY_MODE
	if (m_st_uiControl.b_WriteFA)
	{
		b_writeFAFlag = true;
	}
#else
	if ((m_st_uiControl.b_ReadFA) && (m_st_uiControl.b_WriteFA))
	{
		b_writeFAFlag = true;
	}
#endif

	if (b_writeFAFlag)
	{
		if (m_st_uiControl.i_FAArea == OLDFA)
		{
			TCHAR sz_errorCode[1024] = {0};
			TCHAR sz_errorMsg[1024] = {0};

			/* Write Station Info */
			char sz_stationindex[5]={0};
			CStringToChar(m_map_StationIndexMap[m_st_uiParameter.str_station],sz_stationindex);

			if (!m_obj_faOperator.WriteStationInfo(sz_stationindex, "T", "0", sz_result, "0", "0"))
			{
				m_str_errorCode = m_obj_faOperator.GetErrorCode();
				m_str_errorMessage = m_obj_faOperator.GetErrorMsg();
				return false;
			}

			char sz_FA[512] = {0};
			m_obj_faOperator.GetFA(sz_FA);

			LogMsg(_T("COthersMultiDlg::Write FA Result DATA: ") + ConvertToVisibleString(sz_FA,512));

			if (!m_p_facTestToolInterface->SetFAData(1, sz_FA))
			{
				return false;
			}
		}
		else if (m_st_uiControl.i_FAArea == NEWFA)
		{
			m_obj_newFA.SetTestResult(ConvertString(sz_result));
			m_obj_newFA.SetRecordType(FACTORYLOG);
			m_obj_newFA.SetPartNum(m_st_uiParameter.str_partNumber);
			m_obj_newFA.SetPcba(m_st_uiParameter.str_pcba);
			m_obj_newFA.SetProcess(m_st_uiParameter.str_station);
			m_obj_newFA.SetSO(m_st_uiParameter.str_so);
			m_obj_newFA.SetToolVersion(m_st_uiParameter.str_toolVer);
			m_obj_newFA.SetUserVersion(m_st_uiParameter.str_userLoadVer);
			m_obj_newFA.SetSOType(m_st_uiParameter.str_soType);
			if (strcmp(sz_result,"Y") == 0)
			{
				m_obj_newFA.SetErrorcode(_T("PASS"));
			}
			else
			{
				m_obj_newFA.SetErrorcode(m_str_errorCode);
			}

			m_obj_newFA.SetSegment(m_st_uiParameter.str_segment);
			m_obj_newFA.SetEmployeeID(m_st_uiParameter.str_employee);
			m_obj_newFA.SetLine(m_st_uiParameter.str_line);
			m_obj_newFA.SetFactoryVersion(m_st_uiParameter.str_factoryVer);

			char sz_faData[FA_SIZE]={0};

			if (!m_obj_newFA.GetRecordData(sz_faData))
			{
				m_str_errorMessage = _T("Get FA Record data Fail.");
				return false;
			}

			/* Write FA record */
			if (!m_p_facTestToolInterface->SetFASector(1,m_obj_newFA.GetPointer(), sz_faData, sizeof(sz_faData)))
			{
				m_str_errorMessage = _T("Write FA Record to phone1 Fail.");
				return false;
			}
			if (!m_p_facTestToolInterface->SetFASector(2,m_obj_newFA.GetPointer(), sz_faData, sizeof(sz_faData)))
			{
				m_str_errorMessage = _T("Write FA Record to phone2 Fail.");
				return false;
			}
			if (!m_p_facTestToolInterface->SetFASector(3,m_obj_newFA.GetPointer(), sz_faData, sizeof(sz_faData)))
			{
				m_str_errorMessage = _T("Write FA Record to phone3 Fail.");
				return false;
			}
			if (!m_p_facTestToolInterface->SetFASector(4,m_obj_newFA.GetPointer(), sz_faData, sizeof(sz_faData)))
			{
				m_str_errorMessage = _T("Write FA Record to phone4 Fail.");
				return false;
			}

			m_obj_newFA.IncRecordNum();

			memset(sz_faData,0x00,sizeof(sz_faData));

			if (!m_obj_newFA.GetBasicData(sz_faData))
			{
				m_str_errorMessage = _T("Get FA Basic data Fail.");
				return false;
			}

			/* Update FA pointer*/
			if (!m_p_facTestToolInterface->SetFASector(1,0, sz_faData, sizeof(sz_faData)))
			{
				m_str_errorMessage = _T("Update FA Basic to phone1 Fail.");
				return false;
			}
			if (!m_p_facTestToolInterface->SetFASector(2,0, sz_faData, sizeof(sz_faData)))
			{
				m_str_errorMessage = _T("Update FA Basic to phone2 Fail.");
				return false;
			}
			if (!m_p_facTestToolInterface->SetFASector(3,0, sz_faData, sizeof(sz_faData)))
			{
				m_str_errorMessage = _T("Update FA Basic to phone3 Fail.");
				return false;
			}
			if (!m_p_facTestToolInterface->SetFASector(4,0, sz_faData, sizeof(sz_faData)))
			{
				m_str_errorMessage = _T("Update FA Basic to phone4 Fail.");
				return false;
			}
		}
	}

	return true;
}

void COthersMultiDlg::SetDisplayImage(const CString& str_DisplayImage)
{
	m_AppendImages.clear();
	m_str_ImagePath = str_DisplayImage;

	if(m_p_Image)
	{
		delete m_p_Image;
		m_p_Image = NULL;
	}

	if(m_str_ImagePath.IsEmpty() == false && ::_taccess(m_str_ImagePath, 0) != -1)
	{
		m_p_Image = new Image(m_str_ImagePath.GetBuffer());
		m_str_ImagePath.ReleaseBuffer();
	}

	InvalidateRect(NULL, TRUE);
	UpdateWindow();
}


bool COthersMultiDlg::CheckPicasso()
{   
	TCHAR sz_errorCode[1024] = {0};
	TCHAR sz_errorMsg[1024] = {0};
	char sz_Picasso[15]={0};
	char sz_PicassoUI[15]={0};
	char sz_Mobileid_str[30]={0};
	char sz_Mobileid_byt[15]={0};
	m_b_Rework = false;

	LogMsg(_T("Begin to Check picasso..."));

	/* If first station ,erase and initial FA,then set picasso */
	if (m_map_StationIndexMap[m_st_uiParameter.str_station] == _T("01"))
	{
		char sz_faData[FA_SIZE] = {0};

		if (m_p_facTestToolInterface == NULL)
		{
			m_str_errorCode = CommErr_UI_Interface_GetFA_Fail;
			m_str_errorMessage = _T("DLL interface NULL!");
			return false;
		}
		
		//read fa from mobile through interface
		if (!m_p_facTestToolInterface->GetFAData(1,sz_faData,sizeof(sz_faData)))
		{
			m_str_errorCode = CommErr_UI_Interface_GetFA_Fail;
			m_str_errorMessage = _T("Get FA data fail!");
			m_p_facTestToolInterface ->PostRun();
			return false;
		}
		//set FA to member
		m_obj_faOperator.SetFA(sz_faData);

		LogMsg(_T("FA from target is ") + ConvertToVisibleString(sz_faData,512));

		//read mobile id first to check if rework
		m_obj_faOperator.GetPicassoNumber(sz_Picasso);
		m_obj_faOperator.GetMobileID(sz_Mobileid_byt);

		if (m_obj_faOperator.IsValidePicasso(sz_Picasso) && m_obj_faOperator.IsValideMobileID(sz_Mobileid_byt))
		{					
			m_b_Rework = true;
		}
		else
		{
			/* Erase FA */
			m_obj_faOperator.EraseFactoryArea();
		}

#ifdef FACTORY_MODE
		if (!CheckPicassoDuplicate())
		{
			LogMsg(_T("CheckPicassoDuplicate Fail,picasso in mobile mismatch with CIM"));
			return false;
		}
#endif

		LogMsg(_T("EraseFactoryArea for first station "));

		/* FA Station Setup */
		char sz_stationcount[5] = {0};
		char sz_satationList[100] = {0};
		for (UINT i = 0;i < m_map_StationIndexMap.size();i++)
		{
			sz_satationList[i]= (char)(i+1);
		}		
		sprintf_s(sz_stationcount, "%d", m_map_StationIndexMap.size());

		LogMsg(_T("station list is ") + ConvertString(sz_satationList));
		LogMsg(_T("station count is ") + ConvertToVisibleString(sz_stationcount,m_map_StationIndexMap.size()));
		if (!m_obj_faOperator.FA_Station_Setup(sz_stationcount, sz_satationList))
		{
			m_str_errorMessage = m_obj_faOperator.GetErrorMsg();
			m_str_errorCode = m_obj_faOperator.GetErrorCode();
			return false;
		}

		if (m_b_Rework)
		{
			CStringToChar(m_str_picasso,sz_PicassoUI);
			if (strcmp(sz_PicassoUI,sz_Picasso) !=0)
			{
				m_str_errorMessage = _T("Scanned picasso mismatch with mobile\n输入的picasso与手机中已有的不匹配");
				m_str_errorCode = CommErr_UI_Picasso_Match_Phone_Fail;
				return false;
			}

			if (!m_obj_faOperator.SetPicassoNumber(sz_Picasso))
			{
				m_str_errorMessage = _T("Picasso invalide");
				m_str_errorCode = CommErr_UI_Picasso_Invalide;
				return false;
			}
			LogMsg(_T("Rework :SetPicassoNumber success ") + ConvertString(sz_Picasso));
			if (!m_obj_faOperator.SetMobileID(sz_Mobileid_byt))
			{
				return false;
			}
			LogMsg(_T("Rework :SetMobileID success ") + ConvertToVisibleString(sz_Mobileid_byt,11));
		}
		else
		{
			CStringToChar(m_str_picasso,sz_Picasso);
			if (!m_obj_faOperator.SetPicassoNumber(sz_Picasso))
			{
				m_str_errorMessage = _T("Picasso invalide");
				m_str_errorCode = CommErr_UI_Picasso_Invalide;
				return false;
			}
			LogMsg(_T("Newwork :SetPicassoNumber success ") + ConvertString(sz_Picasso));

			m_obj_faOperator.GenerateMobileID(sz_Mobileid_byt,1);
			m_obj_faOperator.SetMobileID(sz_Mobileid_byt);
			LogMsg(_T("Newwork :SetMobileID success ") + ConvertToVisibleString(sz_Mobileid_byt,11));
		}
		/* Write ModelName */
		if (!m_obj_faOperator.SetModelName(m_st_uiParameter.str_modelName))
		{
			m_str_errorMessage.Format(_T("model name[%s] invalide"),m_st_uiParameter.str_modelName);
			m_str_errorCode = CommErr_UI_FA_Model_Invalide;
			return false;
		}

		/* Write SW Version */
		if (!m_obj_faOperator.SetSWVersion(m_st_uiParameter.str_userLoadVer))
		{
			m_str_errorMessage.Format(_T("SW version[%s] invalide"),m_st_uiParameter.str_userLoadVer);
			m_str_errorCode = CommErr_UI_FA_SWVersion_Invalide;
			return false;
		}
		

		char sz_FA[512] = {0};
		m_obj_faOperator.GetFA(sz_FA);
		LogMsg(_T("Write FA setup: ") + ConvertToVisibleString(sz_FA,512));
		if (!m_p_facTestToolInterface->SetFAData(1, sz_FA))
		{
			m_str_errorMessage = _T("Fail in SetFAData!");
			m_str_errorCode = CommErr_UI_Interface_SetFA_Fail;
			return false;
		}


#ifdef FACTORY_MODE
		if (!m_b_Rework)
		{
			if (m_p_dlgParent != NULL)
			{
				m_obj_faOperator.GetMobileIDHexStr(sz_Mobileid_str);
				LogMsg(_T("Time info To be insert into CIM is") + ConvertString(sz_Mobileid_str));
				if (!m_p_dlgParent->InsertPicassoControlToCIM(sz_Picasso,sz_Mobileid_str))
				{
					m_str_errorMessage = m_p_dlgParent->getErrMsg();
					m_str_errorCode = m_p_dlgParent->getErrCode();
					return false;
				}
			}
		}
#endif
	}
	//if sencond station ,check picasso in mobile and CIM
	else if (m_map_StationIndexMap[m_st_uiParameter.str_station] == _T("02"))
	{
		if (!m_b_haveReadedFAFlag)
		{
			char sz_faData[FA_SIZE] = {0};

			if (m_p_facTestToolInterface == NULL)
			{
				m_str_errorCode = CommErr_UI_Interface_GetFA_Fail;
				m_str_errorMessage = _T("DLL interface NULL!");
				return false;
			}

			//read fa from mobile through interface
			if (!m_p_facTestToolInterface->GetFAData(1,sz_faData,sizeof(sz_faData)))
			{
				m_str_errorCode = CommErr_UI_Interface_GetFA_Fail;
				m_str_errorMessage = _T("Get FA data fail!");
				m_p_facTestToolInterface ->PostRun();
				return false;
			}
			//set FA to member
			m_obj_faOperator.SetFA(sz_faData);
		}
		m_obj_faOperator.GetPicassoNumber(sz_Picasso);
		m_obj_faOperator.GetMobileIDHexStr(sz_Mobileid_str);

		//log mobile info
		CString str_tmp = _T("");
		str_tmp.Format(_T("Second station:%s;Mobile picasso:%s;MobileID:%s"),m_st_uiParameter.str_station,ConvertString(sz_Picasso),ConvertString(sz_Mobileid_str));
		LogMsg(str_tmp);

		if (!m_p_dlgParent->CheckPicassoDuplicateFromCIM(sz_Picasso,sz_Mobileid_str))
		{
			m_str_errorMessage = m_p_dlgParent->getErrMsg();
			m_str_errorCode = m_p_dlgParent->getErrCode();
			return false;
		}
	}

	return true;
}

bool COthersMultiDlg::SetPicasso( std::map<int, CString> map_picasso )
{
	if (!map_picasso.empty())
	{
		//m_str_invalidepicasso = map_picasso[1];
		m_map_picasso.clear();
		m_map_picasso = map_picasso;
		//LogMsg(_T("Invalidepicasso from UI is ") + m_str_invalidepicasso);
	}
	return true;
}

bool COthersMultiDlg::CheckPicassoDuplicate()
{
	char sz_mobileCIM[30] = {0};
	char sz_picassoUI[15] = {0};
	char sz_status[5] = {0};
	char sz_Mobileid_str[30]={0};
	char sz_mobileid_byt[15]={0};
	char sz_Picasso[11]={0};
	
	LogMsg(_T("Begin to check picasso duplicate"));
	 
	CStringToChar(m_str_picasso,sz_picassoUI);
	//check UI picasso,get mobileid and status for UI picasso from cim
	if (!m_p_dlgParent->CheckPicassoStat(sz_picassoUI,sz_mobileCIM,sz_status))
	{
		m_str_errorCode = CommErr_UI_CIM_SMT_Picasso_Not_Exist;
		m_str_errorMessage = _T("No record in SMT! Error!\nSMT中没有该picasso记录，请联系ITS解决");
		return false;
	}

	//log ui picasso,mobileid and status
	CString str_tmp = _T("");
	str_tmp.Format(_T("UIPicasso:%s;MobileID:%s;Status:%s"),m_str_picasso,ConvertString(sz_mobileCIM),ConvertString(sz_status));
	LogMsg(str_tmp);

	if (m_st_uiControl.i_FAArea == OLDFA)
	{
		m_obj_faOperator.GetPicassoNumber(sz_Picasso);
		m_obj_faOperator.GetMobileIDHexStr(sz_Mobileid_str);
	}
	else if (m_st_uiControl.i_FAArea == NEWFA)
	{
		CStringToChar(m_obj_newFA.GetPicassoNumber(),sz_Picasso);
		CStringToChar(m_obj_newFA.GetMobileID(),sz_Mobileid_str);
	}

	//log mobile picasso and mobileid
	str_tmp.Format(_T("UIPicasso:%s;MobileID:%s"),ConvertString(sz_Picasso),ConvertString(sz_Mobileid_str));
	LogMsg(str_tmp);

	//There is no record in table
	if(((CString)sz_mobileCIM) == _T("")) 
	{
		if(m_b_Rework) 
		{
			m_str_errorCode = CommErr_UI_CIM_Phone_Init_Not_Record;
			m_str_errorMessage = _T("No record in CIM but found picasso in target\nCIM中没有记录，但手机中发现picasso：") + ConvertString(sz_Picasso);
			return false;
		}	
	}
	//there is record in table
	else 
	{
		if (strcmp(sz_status, "L") == 0)
		{
			m_str_errorCode = CommErr_UI_CIM_Picasso_Locked;
			m_str_errorMessage = _T("The picasso Number status error! \nPicasso已被锁定！");
			return false;

		}
		else if(strcmp(sz_status,"N") == 0)
		{
			// there is picasso number in handset
			if(((CString)sz_Picasso) != _T("")) 
			{
				if (strcmp(sz_Picasso,sz_picassoUI) != 0)
				{
					m_str_errorCode = CommErr_UI_Picasso_Match_Phone_Fail;
					m_str_errorMessage = _T("Picasso in mobile mismatched with UI! \n手机中picasso与标签不符！");
					return false;
				}

				//Time info not equal
				if(((CString)sz_mobileCIM) != ((CString)sz_Mobileid_str)) 
				{
					m_str_errorCode = CommErr_UI_CIM_MobileID_Match_Fail;
					m_str_errorMessage = _T("The mobileid not equal!\n手机中的mobileid与CIM中不匹配！");
					return false;
				}

			}
			//there is not picasso number in handset
			else 
			{
				m_str_errorCode = CommErr_UI_CIM_Phone_Reocrd_Not_Init;
				m_str_errorMessage = _T("No picasso in target but found record in CIM!\n发现CIM中有该手机的记录，但手机内信息被非法清除！");
				return false;
			}
		}
		else if(strcmp(sz_status,"R") == 0 )
		{
			// there is picasso number in handset
			if(((CString)sz_Picasso) != _T("")) 
			{
				if (strcmp(sz_Picasso,sz_picassoUI) != 0)
				{
					m_str_errorCode = CommErr_UI_Picasso_Match_Phone_Fail;
					m_str_errorMessage = _T("Rework:Picasso in mobile mismatched with UI! \n手机中的picasso与CIM中不符！");
					return false;
				}

				//Time info not equal
				if(((CString)sz_mobileCIM) != ((CString)sz_Mobileid_str)) 
				{
					m_str_errorCode = CommErr_UI_CIM_MobileID_Match_Fail;
					m_str_errorMessage = _T("Rework: The timeinfo not equal!\n手机中的ID标签信息与CIM中不符！");
					return false;
				}

			}
		}
		//if get other status
		else
		{
			m_str_errorCode = CommErr_UI_CIM_Picasso_Status_Invalide;
			m_str_errorMessage = _T("The picasso Number status invalide!");
			return false;
		}
	}

	LogMsg(_T("End CheckPicassoDuplicate for other station dlg"));

	return true;

}

bool COthersMultiDlg::ComparePicassoTag()
{
	CString str_picassoUI = _T("");
	CString str_picassoUI2 = _T("");
	CString str_picassoUI3 = _T("");
	CString str_picassoUI4 = _T("");
	GetDlgItem(IDC_EDIT_RF_ID)->GetWindowText(str_picassoUI);
	GetDlgItem(IDC_EDIT_RF_ID2)->GetWindowText(str_picassoUI2);
	GetDlgItem(IDC_EDIT_RF_ID3)->GetWindowText(str_picassoUI3);
	GetDlgItem(IDC_EDIT_RF_ID4)->GetWindowText(str_picassoUI4);
	if (m_str_picasso.CompareNoCase(str_picassoUI) != 0)
	{
		m_p_facTestToolInterface ->PostRun();
		m_st_return.str_errorMsg.Format(_T("扫入的picasso(%s)与手机中(%s)的不匹配!\nPicasso match error!"),str_picassoUI,m_str_picasso);
		m_st_return.str_errorcode = CommErr_UI_Picasso_Match_Phone_Fail;
        m_st_return.i_slot = 1;
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
		GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));
		//return false;
	}

	if (m_str_picasso2.CompareNoCase(str_picassoUI2) != 0)
	{
		m_p_facTestToolInterface ->PostRun();
		m_st_return.str_errorMsg.Format(_T("扫入的picasso(%s)与手机中(%s)的不匹配!\nPicasso match error!"),str_picassoUI,m_str_picasso2);
		m_st_return.str_errorcode = CommErr_UI_Picasso_Match_Phone_Fail;
		m_st_return.i_slot = 2;
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
		GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));
		//return false;
	}

	if (m_str_picasso3.CompareNoCase(str_picassoUI3) != 0)
	{
		m_p_facTestToolInterface ->PostRun();
		m_st_return.str_errorMsg.Format(_T("扫入的picasso(%s)与手机中(%s)的不匹配!\nPicasso match error!"),str_picassoUI,m_str_picasso3);
		m_st_return.str_errorcode = CommErr_UI_Picasso_Match_Phone_Fail;
		m_st_return.i_slot = 3;
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
		GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));
		//return false;
	}

	if (m_str_picasso4.CompareNoCase(str_picassoUI4) != 0)
	{
		m_p_facTestToolInterface ->PostRun();
		m_st_return.str_errorMsg.Format(_T("扫入的picasso(%s)与手机中(%s)的不匹配!\nPicasso match error!"),str_picassoUI,m_str_picasso4);
		m_st_return.str_errorcode = CommErr_UI_Picasso_Match_Phone_Fail;
		m_st_return.i_slot = 4;
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
		GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));
		//return false;
	}

	return true;
}

bool COthersMultiDlg::CheckScan()
{   

	CString str_picassoUI = _T("");
	CString str_picassoUI2 = _T("");
	CString str_picassoUI3 = _T("");
	CString str_picassoUI4 = _T("");
	GetDlgItem(IDC_EDIT_RF_ID)->GetWindowText(str_picassoUI);
    GetDlgItem(IDC_EDIT_RF_ID2)->GetWindowText(str_picassoUI2);
	GetDlgItem(IDC_EDIT_RF_ID3)->GetWindowText(str_picassoUI3);
	GetDlgItem(IDC_EDIT_RF_ID4)->GetWindowText(str_picassoUI4);
	//if this station need scan picasso
	if (str_picassoUI == _T(""))
	{
		m_st_return.str_errorMsg.Format(_T("本站(%s)需要扫入picasso!\nNo Picasso scanned in!"),m_st_uiParameter.str_station);
		m_st_return.str_errorcode = CommErr_UI_Picasso_Not_Input;
		m_st_return.i_slot = 1;
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
		GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));
		//return false;
	}
	else
	{
		if (str_picassoUI.GetLength() == PICASSO_LENGTH)
		{
			if((str_picassoUI.SpanIncluding(_T("01231456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"))).GetLength() == PICASSO_LENGTH)
			{
				char sz_picasso[15]={0};
				CStringToChar(str_picassoUI,sz_picasso);
				m_p_dlgParent->SetPICSData(1,"id",sz_picasso);
			}
			else
			{
				m_st_return.str_errorMsg.Format(_T("picasso 含无效字符!\nPicasso include invalid chars!"),str_picassoUI);
				m_st_return.str_errorcode = CommErr_UI_Picasso_Invalide;
				m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
				GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));
				str_picassoUI = _T("");
			}
		}		
		else
		{
			m_st_return.str_errorMsg.Format(_T("Slot 1 picasso 长度不正确!\nPicasso Length invalid!"),str_picassoUI);
			m_st_return.str_errorcode = CommErr_UI_Picasso_Invalide;
			m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
			GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));
			str_picassoUI = _T("");
		}
	}

	if (str_picassoUI2 == _T(""))
	{
		m_st_return.str_errorMsg.Format(_T("本站(%s)需要扫入picasso!\nNo Picasso scanned in!"),m_st_uiParameter.str_station);
		m_st_return.str_errorcode = CommErr_UI_Picasso_Not_Input;
		m_st_return.i_slot = 2;
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
		GetDlgItem(IDC_EDIT_RF_ID2)->SetWindowText(_T(""));
		//return false;
	}
	else
	{
		if (str_picassoUI2.GetLength() == PICASSO_LENGTH)
		{
			if((str_picassoUI2.SpanIncluding(_T("01231456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"))).GetLength() == PICASSO_LENGTH)
			{
				char sz_picasso[15]={0};
				CStringToChar(str_picassoUI,sz_picasso);
				m_p_dlgParent->SetPICSData(2,"id",sz_picasso);
			}
			else
			{
				m_st_return.str_errorMsg.Format(_T("picasso 含无效字符!\nPicasso include invalid chars!"),str_picassoUI);
				m_st_return.str_errorcode = CommErr_UI_Picasso_Invalide;
				m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
				GetDlgItem(IDC_EDIT_RF_ID2)->SetWindowText(_T(""));
				str_picassoUI2 = _T("");
			}
		}		
		else
		{
			m_st_return.str_errorMsg.Format(_T("Slot 2 picasso 长度不正确!\nPicasso Length invalid!"),str_picassoUI);
			m_st_return.str_errorcode = CommErr_UI_Picasso_Invalide;
			m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
			GetDlgItem(IDC_EDIT_RF_ID2)->SetWindowText(_T(""));
			str_picassoUI2 = _T("");
		}
	}

	if (str_picassoUI3 == _T(""))
	{
		m_st_return.str_errorMsg.Format(_T("本站(%s)需要扫入picasso!\nNo Picasso scanned in!"),m_st_uiParameter.str_station);
		m_st_return.str_errorcode = CommErr_UI_Picasso_Not_Input;
		m_st_return.i_slot = 3;
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
		GetDlgItem(IDC_EDIT_RF_ID3)->SetWindowText(_T(""));
		//return false;
	}
	else
	{
		if (str_picassoUI3.GetLength() == PICASSO_LENGTH)
		{
			if((str_picassoUI3.SpanIncluding(_T("01231456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"))).GetLength() == PICASSO_LENGTH)
			{
				char sz_picasso[15]={0};
				CStringToChar(str_picassoUI3,sz_picasso);
				m_p_dlgParent->SetPICSData(3,"id",sz_picasso);
			}
			else
			{
				m_st_return.str_errorMsg.Format(_T("picasso 含无效字符!\nPicasso include invalid chars!"),str_picassoUI);
				m_st_return.str_errorcode = CommErr_UI_Picasso_Invalide;
				m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
				GetDlgItem(IDC_EDIT_RF_ID3)->SetWindowText(_T(""));
				str_picassoUI3 = _T("");
			}
		}		
		else
		{
			m_st_return.str_errorMsg.Format(_T("Slot 3 picasso 长度不正确!\nPicasso Length invalid!"),str_picassoUI);
			m_st_return.str_errorcode = CommErr_UI_Picasso_Invalide;
			m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
			GetDlgItem(IDC_EDIT_RF_ID3)->SetWindowText(_T(""));
			str_picassoUI3 = _T("");
		}
	}

	if (str_picassoUI4 == _T(""))
	{
		m_st_return.str_errorMsg.Format(_T("本站(%s)需要扫入picasso!\nNo Picasso scanned in!"),m_st_uiParameter.str_station);
		m_st_return.str_errorcode = CommErr_UI_Picasso_Not_Input;
		m_st_return.i_slot = 4;
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
		GetDlgItem(IDC_EDIT_RF_ID4)->SetWindowText(_T(""));
		//return false;
	}
	else
	{
		if (str_picassoUI4.GetLength() == PICASSO_LENGTH)
		{
			if((str_picassoUI4.SpanIncluding(_T("01231456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"))).GetLength() == PICASSO_LENGTH)
			{
				char sz_picasso[15]={0};
				CStringToChar(str_picassoUI4,sz_picasso);
				m_p_dlgParent->SetPICSData(4,"id",sz_picasso);
			}
			else
			{
				m_st_return.str_errorMsg.Format(_T("picasso 含无效字符!\nPicasso include invalid chars!"),str_picassoUI);
				m_st_return.str_errorcode = CommErr_UI_Picasso_Invalide;
				m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
				GetDlgItem(IDC_EDIT_RF_ID4)->SetWindowText(_T(""));
				str_picassoUI4 = _T("");
			}
		}		
		else
		{
			m_st_return.str_errorMsg.Format(_T("Slot 4 picasso 长度不正确!\nPicasso Length invalid!"),str_picassoUI);
			m_st_return.str_errorcode = CommErr_UI_Picasso_Invalide;
			m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
			GetDlgItem(IDC_EDIT_RF_ID4)->SetWindowText(_T(""));
			str_picassoUI4 = _T("");
		}
	}
    
	if(str_picassoUI==_T("")&&str_picassoUI2==_T("")&&str_picassoUI3==_T("")&&str_picassoUI4==_T(""))
	{
		return false;
	}

	return true;
}

void COthersMultiDlg::LogMsg( CString logContent )
{
	m_obj_log.SaveLog(logContent);
}

//  void COthersMultiDlg::OnCustomdrawList( NMHDR* pNMHDR, LRESULT* pResult )
//  {
//  	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
//  
//  	// Take the default processing unless we set this to something else below.
//  	*pResult = CDRF_DODEFAULT;
//  
//  	// First thing - check the draw stage. If it's the control's prepaint
//  	// stage, then tell Windows we want messages for every item.
//  	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
//  	{
//  		*pResult = CDRF_NOTIFYITEMDRAW;
//  	}
//  	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
//  	{
//  		COLORREF crBkgnd;
//  		int rowIndex = pLVCD->nmcd.dwItemSpec;
//  
//  		CString subItemContent = m_listctrl_message.GetItemText(rowIndex,0);
//  
//  
//  		if ( subItemContent.Find(_T("PART_FAIL")) >= 0)  
//  		{
//  			crBkgnd = RGB(255,0,0);  
//  			pLVCD->clrTextBk = crBkgnd;    
//  		}
//  		else if (subItemContent.Find(_T("ALL_PASS")) >=0 )  
//  		{
//  			crBkgnd = RGB(0,255,0);   
//  			pLVCD->clrTextBk = crBkgnd;
//  		}
//  
//  		// Tell Windows to paint the control itself.
//  		*pResult = CDRF_DODEFAULT;
//  	}
//  
//  }

void COthersMultiDlg::OnBnClickedButtonNext()
{
	// TODO: Add your control notification handler code here
	CString str_nextpicasso;
	GetDlgItem(IDC_EDIT_PRESCAN)->GetWindowText(str_nextpicasso);
	if (str_nextpicasso.GetLength() == PICASSO_LENGTH)
	{
		GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(str_nextpicasso);
		if (m_st_uiControl.b_AutoRun)
		{
			m_p_dlgParent->PostMessage(WM_KEYDOWN, VK_RETURN, 0);		
		}
		GetDlgItem(IDC_EDIT_PRESCAN)->SetWindowText(_T(""));
		(CEdit*)GetDlgItem(IDC_EDIT_PRESCAN)->SetFocus();
	}
}

bool COthersMultiDlg::CheckPicasso_New()
{
	m_b_Rework = false;
	/* If first station ,erase and initial FA,then set Picasso */
	if (m_map_StationIndexMap[m_st_uiParameter.str_station] == _T("01"))
	{
		if (!m_b_haveReadedFAFlag)
		{
			/* Read FA */
			char sz_faData[FA_SIZE] = {0};
			if (!m_p_facTestToolInterface->GetFASector(1,0,sz_faData,sizeof(sz_faData)))
			{
				m_str_errorCode = CommErr_UI_Interface_GetFA_Fail;
				m_str_errorMessage = _T("Read FA Fail!");

				m_p_facTestToolInterface ->PostRun();
				return false;
			}

			/* Set FA */
			if (!m_obj_newFA.SetSectorData(sz_faData))
			{
				m_str_errorCode = CommErr_UI_Interface_GetFA_Fail;
				m_str_errorMessage = _T("FAOperator Fail!");
				return false;
			}

			LogMsg(_T("FA from target is ") + ConvertToVisibleString(sz_faData,512));
		}

		
		if (m_obj_newFA.IsValidePicasso())
		{
			m_b_Rework = true;
		}
		else
		{
			m_obj_newFA.EraseFactoryArea();
		}
#ifdef FACTORY_MODE
		/* Check Picasso duplicate */
		if (!CheckPicassoDuplicate())
		{
			LogMsg(_T("CheckPicassoDuplicate Fail,picasso in mobile mismatch with CIM"));
			return false;
		}
#endif		
		
		if (m_b_Rework)
		{
			if (m_obj_newFA.GetPicassoNumber() != m_str_picasso)
			{
				m_str_errorMessage = _T("Scanned picasso mismatch with mobile\n输入的picasso与手机中已有的不匹配");
				m_str_errorCode = CommErr_UI_Picasso_Match_Phone_Fail;
				return false;
			}
		}
		else
		{
			m_obj_newFA.SetPicassoNumber(m_str_picasso);
			m_obj_newFA.SetMobileID(1);
			m_obj_newFA.SetModelName(m_st_uiParameter.str_modelName);
			m_obj_newFA.SetImeiNumber(UNDEFINED);
			m_obj_newFA.SetServiceID(UNDEFINED);
		}
		
		char sz_FA[512] = {0};
		if (!m_obj_newFA.GetBasicData(sz_FA))
		{
			m_str_errorMessage = _T("Fail in Get FA Basic Data!");
			m_str_errorCode = CommErr_UI_Interface_SetFA_Fail;
			return false;
		}

		
		if (!m_p_facTestToolInterface->SetFASector(1,0, sz_FA,sizeof(sz_FA)))
		{
			m_str_errorMessage = _T("Fail in Set FA Basic Data!");
			m_str_errorCode = CommErr_UI_Interface_SetFA_Fail;
			return false;
		}

#ifdef FACTORY_MODE
		if (!m_b_Rework)
		{
			if (m_p_dlgParent != NULL)
			{
				char sz_Picasso[15]={0};
				char sz_Mobileid_str[30]={0};
				CStringToChar(m_obj_newFA.GetPicassoNumber(),sz_Picasso);
				CStringToChar(m_obj_newFA.GetMobileID(),sz_Mobileid_str);
				if (!m_p_dlgParent->InsertPicassoControlToCIM(sz_Picasso,sz_Mobileid_str))
				{
					m_str_errorMessage = m_p_dlgParent->getErrMsg();
					m_str_errorCode = m_p_dlgParent->getErrCode();
					return false;
				}
			}
		}
#endif

	}

	/* If second station ,check Picasso in mobile and CIM */
	else if (m_map_StationIndexMap[m_st_uiParameter.str_station] == _T("02"))
	{
		if (!m_b_haveReadedFAFlag)
		{
			/* Read FA */
			char sz_faData[FA_SIZE] = {0};
			if (!m_p_facTestToolInterface->GetFASector(1,0,sz_faData,sizeof(sz_faData)))
			{
				m_str_errorCode = CommErr_UI_Interface_GetFA_Fail;
				m_str_errorMessage = _T("Read FA Fail!");

				m_p_facTestToolInterface ->PostRun();
				return false;
			}

			/* Set FA */
			if (!m_obj_newFA.SetSectorData(sz_faData))
			{
				m_str_errorCode = CommErr_UI_Interface_GetFA_Fail;
				m_str_errorMessage = _T("FAOperator Fail!");
				return false;
			}
		}

		/* Check Picasso duplicate */
		char sz_Picasso[15] = {0};
		char sz_Mobileid_str[30] = {0};
		CStringToChar(m_obj_newFA.GetPicassoNumber(),sz_Picasso);
		CStringToChar(m_obj_newFA.GetMobileID(),sz_Mobileid_str);

		if (!m_p_dlgParent->CheckPicassoDuplicateFromCIM(sz_Picasso, sz_Mobileid_str))
		{
			m_str_errorMessage = m_p_dlgParent->getErrMsg();
			m_str_errorCode = m_p_dlgParent->getErrCode();
			return false;
		}
	}

	return true;
}

bool COthersMultiDlg::SetParameterToDLL( void )
{
	USES_CONVERSION;

//#ifdef RD_MODE
//	if (!m_p_facTestToolInterface->SetParameterValue(TOOLMODE, "RD"))
//	{
//		m_str_errorMessage.Format(_T("Tool mode(%s) is invalide!"), _T("RD"));
//		return false;
//	}
//#else
//	if (!m_p_facTestToolInterface->SetParameterValue(TOOLMODE, "FACTORY"))
//	{
//		m_str_errorMessage.Format(_T("Tool mode(%s) is invalide!"), _T("FACTORY"));
//		return false;
//	}
//#endif
//
//	if (!m_p_facTestToolInterface->SetParameterValue(LOGPATH, T2A(LOG_PATH)))
//	{
//		m_str_errorMessage.Format(_T("Log path(%s) is invalide!"), LOG_PATH);
//		return false;
//	}
//
//	if (!m_p_facTestToolInterface->SetParameterValue(STATION_NAME, T2A(m_st_uiParameter.str_station)))
//	{
//		m_str_errorMessage.Format(_T("Station Name(%s) is invalide!"), m_st_uiParameter.str_station);
//		return false;
//	}
//	
//	if (!m_p_facTestToolInterface->SetParameterValue(MODEL_NAME, T2A(m_st_uiParameter.str_modelName)))
//	{
//		m_str_errorMessage.Format(_T("Model Name(%s) is invalide!"), m_st_uiParameter.str_modelName);
//		return false;
//	}
//
//	/*if (!m_p_facTestToolInterface->SetParameterValue(HW_VERSION, T2A(m_st_uiParameter.str_pcba)))
//	{
//		m_str_errorMessage.Format(_T("HW Name(%s) is invalide!"), m_st_uiParameter.str_pcba);
//		return false;
//	}*/
//	
//	if (!m_p_facTestToolInterface->SetParameterValue(SW_VERSION, T2A(m_st_uiParameter.str_userLoadVer)))
//	{
//		m_str_errorMessage.Format(_T("UserLoad version(%s) is invalide!"), m_st_uiParameter.str_userLoadVer);
//		return false;
//	}
//
//	//del by luffy, get tool version from dll
//	/*if (!m_p_facTestToolInterface->SetParameterValue(TOOL_VERSION, T2A(m_st_uiParameter.str_toolVer)))
//	{
//		m_str_errorMessage.Format(_T("Tool Version(%s) is invalide!"), m_st_uiParameter.str_toolVer);
//		return false;
//	}*/
//	
//	if (!m_p_facTestToolInterface->SetParameterValue(FACTORY_VERSION, T2A(m_st_uiParameter.str_factoryVer)))
//	{
//		m_str_errorMessage.Format(_T("Factory Version(%s) is invalide!"), m_st_uiParameter.str_factoryVer);
//		return false;
//	}
//	
//	if (!m_p_facTestToolInterface->SetParameterValue(DEVICE1, T2A(m_st_uiControl.str_Device1)))
//	{
//		m_str_errorMessage.Format(_T("Device1(%s) is invalide!"), m_st_uiControl.str_Device1);
//		return false;
//	}
//	if (!m_p_facTestToolInterface->SetParameterValue(DEVICE2, T2A(m_st_uiControl.str_Device2)))
//	{
//		m_str_errorMessage.Format(_T("Device2(%s) is invalide!"), m_st_uiControl.str_Device2);
//		return false;
//	}
//	if (!m_p_facTestToolInterface->SetParameterValue(DEVICE3, T2A(m_st_uiControl.str_Device3)))
//	{
//		m_str_errorMessage.Format(_T("Device3(%s) is invalide!"), m_st_uiControl.str_Device3);
//		return false;
//	}
//	if (!m_p_facTestToolInterface->SetParameterValue(DEVICE4, T2A(m_st_uiControl.str_Device4)))
//	{
//		m_str_errorMessage.Format(_T("Device4(%s) is invalide!"), m_st_uiControl.str_Device4);
//		return false;
//	}
//
//	std::map<CString,CString>::iterator Itr;
//	for (Itr = m_map_keyvalue.begin();Itr!= m_map_keyvalue.end();Itr++)
//	{
//		if (!m_p_facTestToolInterface->SetParameterValue(T2A(Itr->first),T2A(Itr->second)))
//		{
//			m_str_errorMessage.Format(_T("(%s,%s) is invalide!"),Itr->first,Itr->second);
//			return false;
//		}
//		LogMsg(Itr->first + _T("--") + Itr->second);
//	}

	for (int i=0; i<DEVICE_NUMBER_OTHER; i++)
	{
#ifdef RD_MODE
		if (!m_p_ToolInterface[i]->SetParameterValue(TOOLMODE, "RD"))
		{
			m_str_errorMessage.Format(_T("Tool mode(%s) is invalide!"), _T("RD"));
			return false;
		}
#else
		if (!m_p_ToolInterface[i]->SetParameterValue(TOOLMODE, "FACTORY"))
		{
			m_str_errorMessage.Format(_T("Tool mode(%s) is invalide!"), _T("FACTORY"));
			return false;
		}
#endif

		if (!m_p_ToolInterface[i]->SetParameterValue(LOGPATH, T2A(LOG_PATH)))
		{
			m_str_errorMessage.Format(_T("Log path(%s) is invalide!"), LOG_PATH);
			return false;
		}

		if (!m_p_ToolInterface[i]->SetParameterValue(STATION_NAME, T2A(m_st_uiParameter.str_station)))
		{
			m_str_errorMessage.Format(_T("Station Name(%s) is invalide!"), m_st_uiParameter.str_station);
			return false;
		}
		
		if (!m_p_ToolInterface[i]->SetParameterValue(MODEL_NAME, T2A(m_st_uiParameter.str_modelName)))
		{
			m_str_errorMessage.Format(_T("Model Name(%s) is invalide!"), m_st_uiParameter.str_modelName);
			return false;
		}

		/*if (!m_p_ToolInterface[i]->SetParameterValue(HW_VERSION, T2A(m_st_uiParameter.str_pcba)))
		{
			m_str_errorMessage.Format(_T("HW Name(%s) is invalide!"), m_st_uiParameter.str_pcba);
			return false;
		}*/
		
		if (!m_p_ToolInterface[i]->SetParameterValue(SW_VERSION, T2A(m_st_uiParameter.str_userLoadVer)))
		{
			m_str_errorMessage.Format(_T("UserLoad version(%s) is invalide!"), m_st_uiParameter.str_userLoadVer);
			return false;
		}

		//del by luffy, get tool version from dll
		/*if (!m_p_ToolInterface[i]->SetParameterValue(TOOL_VERSION, T2A(m_st_uiParameter.str_toolVer)))
		{
			m_str_errorMessage.Format(_T("Tool Version(%s) is invalide!"), m_st_uiParameter.str_toolVer);
			return false;
		}*/
		
		if (!m_p_ToolInterface[i]->SetParameterValue(FACTORY_VERSION, T2A(m_st_uiParameter.str_factoryVer)))
		{
			m_str_errorMessage.Format(_T("Factory Version(%s) is invalide!"), m_st_uiParameter.str_factoryVer);
			return false;
		}
		
		if (!m_p_ToolInterface[i]->SetParameterValue(MULTI_MODE, "1"))
		{
			m_str_errorMessage.Format(_T("Multi mode(%s) is invalide!"), _T("1"));
			return false;
		}

		std::map<CString,CString>::iterator Itr2;
		for (Itr2 = m_map_keyvalue.begin();Itr2!= m_map_keyvalue.end();Itr2++)
		{
			if (!m_p_ToolInterface[i]->SetParameterValue(T2A(Itr2->first),T2A(Itr2->second)))
			{
				m_str_errorMessage.Format(_T("(%s,%s) is invalide!"),Itr2->first,Itr2->second);
				return false;
			}
			LogMsg(Itr2->first + _T("--") + Itr2->second);
		}
	}
	return true;
}

bool COthersMultiDlg::GetParameterFromDLL( void )
{
	/* Get Tool Version */
	char sz_toolversion[30]={0};
	if (!m_p_ToolInterface[0]->GetParameterValue(TOOL_VERSION, sz_toolversion,30))
	{
		m_str_errorMessage = _T("初始化失败！获取ToolVersion失败！");
		return false;
	}
	m_st_uiParameter.str_toolVer = ConvertString(sz_toolversion);

	m_p_dlgParent->SetUIIDParameter(m_st_uiParameter);

	return true;
}

void COthersMultiDlg::ParsePara(CString str_Input,CString &str_Output,CString &str_Device)
{   
    if(str_Input==_T(""))
	{
       return;
	}
    int index = 0; 

	index = str_Input.Find(_T(",#SLOT#"));

	if(index == -1)
	{
      str_Output = str_Input;
	  return;
	}
	else
	{
	  str_Output = str_Input.Left(index);
	  str_Device = str_Input.Right(1);
	}
}

void COthersMultiDlg::NotifyMainUI_Other(st_pageReturn st_return)
{
	if (m_p_dlgParent != NULL)
	{
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&st_return);
	}

	return;
}

void COthersMultiDlg::SetTestResult( int i_slot, CString str_testresult )
{
	if (m_b_isSlotAvailable[i_slot] == false)
	{
		return;
	}
	m_label_progress[i_slot].SetWindowText(str_testresult);
    
	//LogMsg(str_testresult);

	if (str_testresult == _T("PASS"))
	{		
		m_label_progress[i_slot].SetBkColor(RGB(0,255,0));
	}
	else
	{
		m_label_progress[i_slot].SetBkColor(RGB(255,0,0));
	}
}

void COthersMultiDlg::DeviceDetectSetup()
{
	HDEVNOTIFY hDevNotify;
	DEV_BROADCAST_DEVICEINTERFACE filter;	
	memset(&filter,0,sizeof(DEV_BROADCAST_DEVICEINTERFACE));
	filter.dbcc_size         = sizeof(filter);	
	filter.dbcc_devicetype   = DBT_DEVTYP_DEVICEINTERFACE;	
	filter.dbcc_classguid    = GUID_DEVINTERFACE_USB_DEVICE ;//GUID_CLASS_USB_DEVICE;	
	hDevNotify = RegisterDeviceNotification(this->GetSafeHwnd(), &filter, DEVICE_NOTIFY_WINDOW_HANDLE);	
	if (!hDevNotify)	
	{		
		AfxMessageBox(_T("Cannot register device notification: "), MB_ICONEXCLAMATION);	
	}
}

BOOL COthersMultiDlg::OnDeviceChange(UINT nEventType,DWORD_PTR dwData)
{
	if (DBT_DEVICEARRIVAL == nEventType || DBT_DEVICEREMOVECOMPLETE == nEventType) 
	{
		PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)dwData;
		switch (pHdr->dbch_devicetype) {
		case DBT_DEVTYP_DEVICEINTERFACE:
			GetDevice((PDEV_BROADCAST_DEVICEINTERFACE)pHdr,nEventType);
			break;
		default:
			break;
		}
	}
	return TRUE;
}

void COthersMultiDlg::ParserDBCCName(TCHAR dbcc_name[1],CString &str_vendor,CString &str_product,CString &str_para)
{
	// \\?\usb#vid_22b8&pid_2d01#6&.......	
	TCHAR *szToken;
	TCHAR *szNextToken;
	::_tcstok_s(dbcc_name,_T("_&"),&szNextToken);
	szToken = ::_tcstok_s(szNextToken,_T("_&"),&szNextToken);
	str_vendor = szToken;

	::_tcstok_s(szNextToken,_T("_&"),&szNextToken);	
	szToken = ::_tcstok_s(szNextToken,_T("_&#"),&szNextToken);
	str_product = szToken;	

	szToken = ::_tcstok_s(szNextToken,_T("#"),&szNextToken);	
	str_para = szToken;
}

void COthersMultiDlg::GetDevice(PDEV_BROADCAST_DEVICEINTERFACE lpbv,UINT nEventType)
{
	CSingleLock singleLock(&m_obj_UIConnect,TRUE);
	if(lpbv)
	{	
		CString strmsg;
		CString str_vendor;	
		CString str_product;
		CString str_para;

		//strmsg.Format(_T("DeviceChanged, Type:0x%X, dbcc_name:%s"),nEventType,lpbv->dbcc_name);
		//LogMsg(strmsg);
		//ParserDBCCName(lpbv->dbcc_name,str_vendor,str_product,str_para);
		//str_para like '5&8639f5&0&7' and then str_Comport = '7'
		//if (str_para.Find('&') == -1)
		//{
		//	return;
		//}
		//CString str_Comport = str_para.Mid(str_para.ReverseFind('&')+1);

		//if(IsValidVIDPID(str_vendor,str_product))
		//{
			if(nEventType == DBT_DEVICEREMOVECOMPLETE)//device remove
			{
				strmsg.Format(_T("DeviceChanged -- Remove, Type:0x%X, dbcc_name:%s"),nEventType,lpbv->dbcc_name);
				LogMsg(strmsg);
				//LogMsg(_T("device remove"));

				//CString str_UIComPort = _T("");

				//for(int i=0;i<8;i++)				
				//{
				//	m_label_deviceCOM[i].GetWindowText(str_UIComPort);
				//	if(str_Comport==str_UIComPort)				
				//	{
				//		strmsg.Format(_T("[slot:%d ---- port:%s]: DisConnect"),i,str_Comport);
				//		LogMsg(strmsg);
				//		m_label_progress[i].SetWindowText(_T("DISConnect"));
				//		m_label_progress[i].SetBkColor(RGB(255,255,255));
				//		return;
				//	}
				//}
			}
			else if(nEventType == DBT_DEVICEARRIVAL)//device insert
			{
				strmsg.Format(_T("DeviceChanged -- Arrival, Type:0x%X, dbcc_name:%s"),nEventType,lpbv->dbcc_name);
				LogMsg(strmsg);
				//LogMsg(_T("device arrival"));

				//CString str_UIComPort = _T("");
				//for(int i=0;i<8;i++)
				//{
					//m_label_deviceCOM[i].GetWindowText(str_UIComPort);	
					//if(str_Comport==str_UIComPort)
					//{
						char DeviceID[512]={0};
						char DevicePort[512]={0};
						int nRes = FindQualcommPort(lpbv->dbcc_name, DeviceID, DevicePort);

						if( nRes != 0 ) {
							LogMsg(_T("device arrival, but the QualcommPort is not found"));
							return;
						}
						
						int i_slot = atoi( DeviceID ) - 1;
						if ( i_slot < 0 || i_slot > 3 ) {
							LogMsg(_T("device arrival, but the SLot number is not between 0 to 3"));
							return;
						}

						CString str_comport(DevicePort);
						CString str_tmp;
						strmsg.Format(_T("[slot:%d ---- port:%s]: Connect"), i_slot, str_comport);
						LogMsg(strmsg);

						if ( i_slot == 0) {
							if( m_st_uiControl.str_Device1==_T("0") ) {
								LogMsg(_T("device arrival, but the slot is not used"));
								return;
							}
							m_b_isSlotAvailable[0] = true;
						}
						if ( i_slot == 1) {
							if( m_st_uiControl.str_Device2==_T("0") ) {
								LogMsg(_T("device arrival, but the slot is not used"));
								return;
							}
							m_b_isSlotAvailable[1] = true;
						}
						if ( i_slot == 2) {
							if( m_st_uiControl.str_Device3==_T("0") ) {
								LogMsg(_T("device arrival, but the slot is not used"));
								return;
							}
							m_b_isSlotAvailable[2] = true;
						}
						if ( i_slot == 3) {
							if( m_st_uiControl.str_Device4==_T("0") ) {
								LogMsg(_T("device arrival, but the slot is not used"));
								return;
							}
							m_b_isSlotAvailable[3] = true;
						}

						CString str_picasso;
						m_edit_picasso[i_slot].GetWindowText(str_picasso);

						if (!m_b_returnRes[i_slot])
						{
							LogMsg(_T("device arrival, but the slot is not end"));
							return;
						}
						else
						{
							ResetLatestClock();
							str_tmp.Format(_T("[%d] ====================== Run BEGIN(V%s) ==========================="),i_slot, m_p_dlgParent->m_str_appver);
							LogMsg(str_tmp);
							m_edit_picasso[i_slot].SetWindowText(_T(""));
							m_label_progress[i_slot].SetBkColor(RGB(255,255,255));
							m_label_progress[i_slot].SetWindowText(_T(""));
							//m_processCtrl_subProgress[i].SetPos(0);	
							//m_processCtrl_mainProgress[i].SetPos(0);	
							m_p_dlgParent->m_map_picasso[i_slot] = str_picasso;

							m_b_returnRes[i_slot] = false;

							m_map_running[i_slot] = new CHandsetOtherStation(&m_st_uiParameter, (i_slot), str_comport, str_picasso, m_p_ToolInterface[i_slot],m_map_StationIndexMap,&m_st_uiControl);
							if (m_map_running[i_slot] == NULL)
							{
								/* Show to UI */
								//st_UIResult st_result;
								//st_result.i_slot = i_slot;
								//CStringToChar(CommErr_UI_DL_Run_Thread_Fail, st_result.sz_errCode); 
								//m_p_ToolInterface[i_slot]->Fire(UI_RESULT, (long)&st_result);

								//strcpy_s(st_result.sz_errCode, "FAIL");
								//m_p_DLInterface->Fire(UI_RESULT, (long)&st_result);
								m_map_runResult_other[i_slot] = CommErr_UI_Run_Thread_Fail;
								return;
							}

							if (!m_map_running[i_slot]->HandsetRun())
							{
								/* Show to UI */
								//st_UIResult st_result;
								//st_result.i_slot = i_slot;
								//CStringToChar(CommErr_UI_DL_Run_Thread_Fail, st_result.sz_errCode); 
								//m_p_ToolInterface[i_slot]->Fire(UI_RESULT, (long)&st_result);

								//strcpy_s(st_result.sz_errCode, "FAIL");
								//m_p_DLInterface->Fire(UI_RESULT, (long)&st_result);
								m_map_runResult_other[i_slot] = CommErr_UI_Run_Thread_Fail;
								return;
							}
							//m_map_running[i_slot]->GetThreadHandle()->m_bAutoDelete = false; //Neil Test
							return;
						}
					//}
				//}
			}
		//}//IsValidVIDPID
		//else
		//{
        //    LogMsg(_T("not invalid vidpid, ignore it"));
		//}
	}
}

int COthersMultiDlg::FindQualcommPort(const TCHAR* Interface_Name, char* DeviceID, char* DevicePort)
{
    int nTimout = 30;
	int ret=0;
	for(int i=0;i< nTimout;i++)
	{
		ret=EnumerateMatchDevice(GUIDQualcommDiagUsbClass, (TCHAR*)Interface_Name, DeviceID, DevicePort);
		if(ret==0) break;
		Sleep(1000);
	}
	return ret;
}

int COthersMultiDlg::EnumerateMatchDevice(GUID TargetDevGUID, TCHAR* pw_parent_interfac_name, char *OutDeviceName, char* OutDevicePath)
{                                       //稱Device摸,USB
	DWORD RequiredSize = 0;
	GUID *guids;   //guid
	HDEVINFO NewDeviceInfoSet;
	SP_DEVINFO_DATA DeviceInfoData;
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	DWORD rs;
	DWORD typeP;
	int DeviceIndex=0;
	bool res=false;
	char strtemp[1024];
	memset(strtemp,0,sizeof(strtemp));
	

	//DEVICE闽戈癟栋
	//YTT ASSIGN ANDROID
	GUID usb_class_id = TargetDevGUID;
	guids = &usb_class_id;
	// guids = (GUID *)usb_class_id;
	//guids=(GUID *)malloc(RequiredSize*sizeof(GUID));
	//memcpy(guids,&usb_class_id,sizeof(GUID) );
	//   ((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel("=====EnumerateMatchDevice START====\r\n");
	NewDeviceInfoSet=SetupDiGetClassDevs(guids,
		NULL,
		NULL,
		DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);


	if( NewDeviceInfoSet == INVALID_HANDLE_VALUE )
		if(!res) {
			//device information is unavailable:
			//realloc(guids,0);
			*OutDeviceName=0;
			return -3;
		}
		//ytt search interface first first
		SP_INTERFACE_DEVICE_DATA InterfaceData;
		InterfaceData.cbSize = sizeof (SP_INTERFACE_DEVICE_DATA);
		// char sDevNameBuf[300];
		int EnumResult=0;
		int i;
		for(i=0;i<=8;i++  ) // only 4?
		{
			// SetupDiEnumDeviceInterfaces(NewDeviceInfoSet, &DeviceInfoData, guids, 0, &InterfaceData);

			//if(SetupDiEnumDeviceInterfaces(NewDeviceInfoSet, &DeviceInfoData, guids, DeviceIndex, &InterfaceData))
			if(SetupDiEnumDeviceInterfaces(NewDeviceInfoSet, NULL, guids, i, &InterfaceData))
			{
				//((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel("ytt SetupDiEnumDeviceInterfaces1 ok\n");

				ULONG  iReqLen = 0;
				SetupDiGetInterfaceDeviceDetail(NewDeviceInfoSet, &InterfaceData, NULL, 0, &iReqLen, NULL);
				ULONG iDevDataLen = iReqLen; //sizeof(SP_FNCLASS_DEVICE_DATA) + 512;
				PSP_INTERFACE_DEVICE_DETAIL_DATA pDevData = (PSP_INTERFACE_DEVICE_DETAIL_DATA)malloc(iDevDataLen);

				pDevData->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
				//if  (SetupDiGetInterfaceDeviceDetail(NewDeviceInfoSet,  &InterfaceData, pDevData, iDevDataLen, &iReqLen, NULL))
				if  (SetupDiGetInterfaceDeviceDetail(NewDeviceInfoSet,  &InterfaceData, pDevData, iDevDataLen,NULL, &DeviceInfoData))
				{
					//	((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel("ytt SetupDiGetInterfaceDeviceDetail2 ok\r\n");
					//				sprintf(strtemp,"\r\num %d interface_name is %s\r\n",i, pDevData->DevicePath) ;
					//((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel("ytt rnum path is %d %s\n",i, pDevData->DevicePath);
					//			((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel(strtemp);
					//                sprintf(strtemp,"expected parent interface name  is  %s\r\n",parent_interfac_name) ;
					//((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel("interface name  is  %s\n",interfac_name);
					//				((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel(strtemp);

					//interace name 莱赣璶Τmi00┕暗
					// if interface_name not find mi00 continue 

					
					char szDevicePath[2048];
					memset(szDevicePath, 0, sizeof(szDevicePath));
					int nLength = WideCharToMultiByte(CP_ACP, 0, pDevData->DevicePath, -1, NULL, 0, NULL, NULL);
					WideCharToMultiByte(CP_ACP, 0, pDevData->DevicePath, -1, szDevicePath, nLength, NULL, NULL);   
					if( NULL==strstr(szDevicePath ,"mi_00") )
					{
						//((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel("SKIP mi_00 ");
						continue;
					}





					DEVINST devInstParent;
					// Get the device instance of parent. This points to USB Composite Device
					CM_Get_Parent(&devInstParent, DeviceInfoData.DevInst, 0);
					//memset(strtemp,0,sizeof(strtemp));

					// Get the device ID of the USB root.
					TCHAR pw_str_ParentName[1024] = {0};
					CM_Get_Device_ID(devInstParent, pw_str_ParentName, sizeof(pw_str_ParentName), 0);
					//nLength = WideCharToMultiByte(CP_ACP, 0, pw_str_ParentName, -1, NULL, 0, NULL, NULL);
					//char szParentName[2048];
					//memset(szParentName ,0 ,sizeof(szParentName));
					//WideCharToMultiByte(CP_ACP, 0, pw_str_ParentName, -1, szParentName, nLength, NULL, NULL);   

					//sprintf(strtemp,"get parent interface name:  %s\r\n",ParentName) ;
					//((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel("interface name  is  %s\n",interfac_name);
					//				((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel(strtemp);

					//ㄢparent interface name 璶璓 interface name 琌mi00 ┕暗
					//compare pid , vid , and other here
					//parent_interface \\?\USB#VID_1D45&PID_0139#BOSTON#{a5dcbf10-6530-11d2-901f-00c04fb951ed}
					//get Parent        USB\VID_1D45&PID_0139\BOSTON
					// CompareParentInterface()

					//TCHAR pw_parent_interfac_name[1024] = {0};
					//nLength = MultiByteToWideChar (CP_ACP, 0, parent_interfac_name, strlen(parent_interfac_name) + 1, NULL, 0) ;  
					//MultiByteToWideChar (CP_ACP, 0, parent_interfac_name, strlen(parent_interfac_name) + 1, pw_parent_interfac_name, nLength) ;  

					if( 1== CompareParentInterface( pw_parent_interfac_name, pw_str_ParentName) )
					{
						//sprintf(strtemp,"compare ok   %s\n",parent_interfac_name) ;
						//((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel("interface name  is  %s\n",interfac_name);
						//				   ((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel(strtemp);
						//sprintf(OutDevicePath,"QPHONE%d",spdrp_address );
						EnumResult=1;
						DeviceIndex = i;
						break;
					}





				}		  
			}   else{
				//((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel("ytt enum interface %d fail\n",i);
			}

		}//for loop end			    
		//}       


		//YTT COMPARE OK ┕ 既奔
		if(EnumResult==0)
		{
			//		((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel("matching interface not found \n");
			return -10;      
		} 


		//((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel("ytt EnumerateDevices 4\r\n");    
		DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
		// NewDeviceInfoSetい戈癟
		res=SetupDiEnumDeviceInfo(NewDeviceInfoSet,DeviceIndex,&DeviceInfoData)? true:false;
		if(!res) {
			//no such device:
			SetupDiDestroyDeviceInfoList(NewDeviceInfoSet);
			//realloc(guids,0);
			*OutDeviceName=0;
			return -1;
		}
		//((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel("ytt EnumerateDevices 5\r\n");      


		/* YTT INFO既ノぃ , LOCATION INFO WIN7ΤЧ俱戈癟,
		//XP玥琌DEVICE NAMEぃ才龟 /XP㎝WIN7ぃ璓,礚猭ㄏノ
		//戈癟いPROPERTY,矪琌 FRIENDLY NAME
		//SPDRP_HARDWAREID
		if(!SetupDiGetDeviceRegistryPropertyA(NewDeviceInfoSet,&DeviceInfoData,
		//  SPDRP_FRIENDLYNAME,&typeP,(unsigned char *)OutDeviceName,MAX_DEV_LEN,&rs) )
		SPDRP_FRIENDLYNAME,&typeP,(unsigned char *)OutDeviceName,1024,&rs) )
		{
		res = SetupDiGetDeviceRegistryPropertyA(NewDeviceInfoSet,
		&DeviceInfoData,
		SPDRP_DEVICEDESC,
		&typeP,
		(unsigned char *)OutDeviceName,
		// MAX_DEV_LEN,
		1024,
		&rs);
		if(!res){
		//incorrect device name:
		SetupDiDestroyDeviceInfoList(NewDeviceInfoSet);
		realloc(guids,0);
		*OutDeviceName=0;
		return -4;
		}
		}
		//ytt location test

		//location info
		res = SetupDiGetDeviceRegistryPropertyA(NewDeviceInfoSet,
		&DeviceInfoData,
		SPDRP_LOCATION_INFORMATION,
		&typeP,
		(unsigned char *)OutDeviceName,
		// MAX_DEV_LEN,
		1024,
		&rs);
		if(!res){
		//incorrect device name:
		SetupDiDestroyDeviceInfoList(NewDeviceInfoSet);
		realloc(guids,0);
		*OutDeviceName=0;
		return -4;
		}
		((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel("ytt SPDRP_LOCATION_INFORMATION %s\n", OutDeviceName);
		*/
		//bus number

		/* ming32gw api ぃ﹚竡
		//location info
		res = SetupDiGetDeviceRegistryPropertyA(NewDeviceInfoSet,
		&DeviceInfoData,
		SPDRP_LOCATION_PATHS,
		&typeP,
		(unsigned char *)OutDeviceName,
		// MAX_DEV_LEN,
		1024,
		&rs);
		if(!res){
		//incorrect device name:
		SetupDiDestroyDeviceInfoList(NewDeviceInfoSet);
		realloc(guids,0);
		*OutDeviceName=0;
		return -4;
		}
		*/
		//int busnum = OutDeviceName[0];
		//locaton addr
		res = SetupDiGetDeviceRegistryProperty(NewDeviceInfoSet,
			&DeviceInfoData,
			SPDRP_ADDRESS,
			&typeP,
			(unsigned char *)strtemp,
			//MAX_DEV_LEN,
			1024,
			&rs)? true:false;
		if(!res){
			//incorrect device name:
			SetupDiDestroyDeviceInfoList(NewDeviceInfoSet);
			//realloc(guids,0);
			*OutDeviceName=0;
			return -4;
		}

		int spdrp_address = strtemp[0];
		int spdrp_address2 = strtemp[1];
		int spdrp_address3 = strtemp[2];

		//((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel("ytt spdrp_address %d\n", spdrp_address);
		//sprintf(strtemp,"ytt spdrp_address %d\r\n", spdrp_address) ;
		//((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel("interface name  is  %s\n",interfac_name);
		//	((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel(strtemp);
		sprintf(OutDeviceName,"%d",spdrp_address );

		//locaton addr
		res = SetupDiGetDeviceRegistryProperty(NewDeviceInfoSet,
			&DeviceInfoData,
			SPDRP_ADDRESS,
			&typeP,
			(unsigned char *)strtemp,
			//MAX_DEV_LEN,
			1024,
			&rs)? true:false;
		if(!res){
			//incorrect device name:
			SetupDiDestroyDeviceInfoList(NewDeviceInfoSet);
			//realloc(guids,0);
			*OutDeviceName=0;
			return -4;
		}

		
		//res = SetupDiGetDeviceRegistryProperty(NewDeviceInfoSet, &DeviceInfoData, SPDRP_FRIENDLYNAME,&typeP,(unsigned char *)strtemp, 1024, &rs)? true:false;
		TCHAR pw_strtemp[1024] = {0};
		res = SetupDiGetDeviceRegistryProperty(NewDeviceInfoSet, &DeviceInfoData, SPDRP_FRIENDLYNAME,&typeP,(PBYTE)pw_strtemp, sizeof(pw_strtemp), &rs)? true:false;
		if(!res){
			//incorrect device name:
			SetupDiDestroyDeviceInfoList(NewDeviceInfoSet);
			//realloc(guids,0);
			*OutDeviceName=0;
			return -5;
		}

		int nLength = WideCharToMultiByte(CP_ACP, 0, pw_strtemp, -1, NULL, 0, NULL, NULL);     
		WideCharToMultiByte(CP_ACP, 0, pw_strtemp, -1, strtemp, nLength, NULL, NULL);  

		//	strcpy(devStruct.chFriendlyName, (char*)chTemp);
		//	((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel("SPDRP_FRIENDLYNAME");
		//    ((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel(strtemp);
		//	((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel("\r\n");
		int num=0;
		char *pLoc;
		if(NULL != (pLoc  = strstr(strtemp ,"COM")))
		{
			if(pLoc-strtemp < sizeof(strtemp))
			{
				strncpy_s(strtemp,pLoc-strtemp,pLoc+3,1024);
			}

			pLoc = strstr(strtemp,")");
			if(pLoc-strtemp < sizeof(strtemp))
			{
				strtemp[pLoc-strtemp]='\0';
			}
			strncpy_s(OutDevicePath,128,strtemp,1024);
			//		((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel("comport number");
			//		 ((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel(strtemp);
			//		((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel("\r\n");

		}
		SetupDiDestroyDeviceInfoList(NewDeviceInfoSet);
		//	((CEdit*)(GetDlgItem(IDC_EDIT1)))->ReplaceSel("=====EnumerateMatchDevice END====\r\n");
		return 0;
}

int COthersMultiDlg::CompareParentInterface(TCHAR* OriginalParentIntrerface, TCHAR* CurrentInterface)
{
	TCHAR OrgInterface[1024] = {0};
	TCHAR CurInterface[1024] = {0};

	::_tcsncpy_s( OrgInterface, ARRAYSIZE(OrgInterface), OriginalParentIntrerface, ARRAYSIZE(OrgInterface)-1 );
	::_tcsncpy_s( CurInterface, ARRAYSIZE(CurInterface), CurrentInterface, ARRAYSIZE(CurInterface)-1 );

	TCHAR* szOrgToken;
	TCHAR* szOrgNextToken;

	TCHAR* szCurToken;
	TCHAR* szCurNextToken;

	//check vid
	::_tcstok_s( OrgInterface,_T("_&"),&szOrgNextToken);//RET 1D45   NEXT:1D45
	szOrgToken = ::_tcstok_s(szOrgNextToken,_T("_&"),&szOrgNextToken);//
	//str_vendor = szToken;
	::_tcstok_s( CurInterface,_T("_&"),&szCurNextToken);
	szCurToken = ::_tcstok_s(szCurNextToken,_T("_&"),&szCurNextToken);

	if(0!=::_tcscmp(::_tcsupr(szOrgToken),::_tcsupr(szCurToken)))
	{
		return 0;
	}
	//check pid
	::_tcstok_s(szOrgNextToken,_T("_&"),&szOrgNextToken);	
	szOrgToken = ::_tcstok_s(szOrgNextToken,_T("_&#"),&szOrgNextToken);
	//	str_product = szToken;	
	::_tcstok_s(szCurNextToken,_T("_&"),&szCurNextToken);	
	szCurToken = ::_tcstok_s(szCurNextToken,_T("_&#\\"),&szCurNextToken);

	if(0!=::_tcscmp(::_tcsupr(szOrgToken),::_tcsupr(szCurToken)))
	{
		return 0;
	}
	//check other
	szOrgToken = ::_tcstok_s(szOrgNextToken,_T("#"),&szOrgNextToken);	
	szCurToken = ::_tcstok_s(szCurNextToken,_T("#"),&szCurNextToken);	

	if(0!=::_tcscmp(::_tcsupr(szOrgToken),::_tcsupr(szCurToken)))
	{
		return 0;
	}
	return 1;
}

//=================================================================================================//
//==================================== CHandsetOtherStation =======================================//
//=================================================================================================//
CHandsetOtherStation::CHandsetOtherStation (st_UIIDparameter* p_st_uiParameter, int i_slot, 
						CString str_comport,
						CString str_picasso, 
						IFacTestToolInterface* p_OtherStationInterface,
						std::map<CString,CString> map_StationIndexMap,
						st_UIControl* p_st_uiControl
						)
	: m_p_st_uiParameter(p_st_uiParameter)
	, m_i_slot(i_slot)
	, m_str_comport(str_comport)
    , m_str_picasso(str_picasso)
	, m_p_handsetOtherStationInterface(p_OtherStationInterface)
	, m_map_StationIndexMap(map_StationIndexMap)
	, m_p_st_uiControl(p_st_uiControl)
{	
	m_b_Rework = false;
	m_str_mobileid = _T("");
	m_p_h_thread = NULL;
	m_b_runRes = false;
}

CHandsetOtherStation::~CHandsetOtherStation()
{

#ifndef RD_MODE
	m_obj_QueryCIM.UpLoadDBAccessDLL();
#endif

	m_p_handsetOtherStationInterface = NULL;
}

//CCriticalSection CHandsetOtherStation::m_obj_CritSection;

bool CHandsetOtherStation::HandsetRun(void)
{
	m_p_h_thread = ::AfxBeginThread(FacTestRun, (void*)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	if (m_p_h_thread == NULL)
	{
		return false;
	}
	
	return true;
}

UINT CHandsetOtherStation::FacTestRun(LPVOID pParam)
{	
	CHandsetOtherStation* p_this = (CHandsetOtherStation*)pParam;
	char sz_errCode[100] = {0};

#ifndef RD_MODE
	if (!p_this->m_obj_QueryCIM.LoadDBAccessDLL())
	{
		CStringToChar( CommErr_UI_DLL_Load_Fail, sz_errCode);
		p_this->FireUIResult( sz_errCode );

		return 0;
	}

#endif

	char sz_comport[20] = {0};
	CStringToChar( p_this->m_str_comport, sz_comport );
	if (!p_this->m_p_handsetOtherStationInterface->SetParameterValue(COM_PORT, sz_comport))
	{
		CStringToChar( CommErr_UI_Interface_Setparameter_Fail, sz_errCode);
		p_this->FireUIResult( sz_errCode );
		p_this->m_p_handsetOtherStationInterface->PostRun(p_this->m_i_slot);
		return 0;
	}

	/* PreRun */
	bool b_res = false;
	
	if (!(b_res = p_this->m_p_handsetOtherStationInterface->PreRun(p_this->m_i_slot)))
	{
		CStringToChar( CommErr_UI_Interface_Prerun_Fail, sz_errCode);
		p_this->FireUIResult( sz_errCode );
	}

	char sz_picasso[20] = {0};
	if (b_res)
	{

	if ((p_this->m_p_st_uiParameter->str_station == STATION_CAMERA || p_this->m_p_st_uiParameter->str_station == STATION_2G3GTEST || p_this->m_p_st_uiParameter->str_station == STATION_LTE))
	{
#ifndef FACTORY_MODE
		if (p_this->m_p_st_uiControl->b_ReadFA)
#endif	
		{	
			/* Read FA */
			if (!(b_res = p_this->ReadFA(sz_picasso)))
			{
				CStringToChar( CommErr_UI_Interface_GetFA_Fail, sz_errCode);
				p_this->FireUIResult( sz_errCode );
				p_this->m_p_handsetOtherStationInterface->PostRun(p_this->m_i_slot);
				return 0;
			}

#ifdef FACTORY_MODE
			/*check picasso rule and check routing*/
			if (!p_this->CheckRouting(sz_picasso,sz_errCode))
			{
				p_this->FireUIResult(sz_errCode);
				//p_this->FireUIResult("FAIL");
				p_this->m_p_handsetOtherStationInterface->PostRun(p_this->m_i_slot);
				return 0;
			}
#endif
			p_this->m_str_picasso = sz_picasso;
		}
	}

	}

	/* Set Picasso */
	CString str_temp;
	str_temp.Format(_T("%d,%s"), p_this->m_i_slot, p_this->m_str_picasso);
	//CStringToChar(str_temp, sz_picasso);
	p_this->LogMsg(str_temp);

	p_this->FireUIPicasso(sz_picasso);

	if (!p_this->m_p_handsetOtherStationInterface->SetParameterValue(PICASSO, sz_picasso))
	{
		CStringToChar( CommErr_UI_Interface_Setparameter_Fail, sz_errCode);
		p_this->FireUIResult( sz_errCode );
		p_this->m_p_handsetOtherStationInterface->PostRun(p_this->m_i_slot);
		return 0;
	}

    /* Run */
	if (b_res)
	{
		b_res = p_this->m_p_handsetOtherStationInterface->Run(p_this->m_i_slot);
		if (!b_res)
		{
			p_this->m_b_runRes = false;
		}
		else
		{
			p_this->m_b_runRes = true;
		}
	}
	
#ifdef FACTORY_MODE	
	if (!p_this->m_p_st_uiParameter->b_tsFlag && p_this->m_b_runRes)
	{
		/* Read FA */
		if (!(b_res = p_this->ReadFA()))
		{
			//p_this->FireUIMessage("ReadFA Fail.");

			if (p_this->m_b_runRes)
			{
				CStringToChar(CommErr_UI_Interface_GetFA_Fail,sz_errCode);
				p_this->FireUIResult(sz_errCode);
			}
		}
		else
		{
			//p_this->FireUIMessage("Read FA Success.");
		}
		
		/* Check Picasso with CIM */	
		if (b_res)
		{
			if (!(b_res = p_this->CheckPicassoDuplicate(sz_errCode)))
			{
				p_this->FireUIMessage("Check Picasso Duplicate Fail.");
				if (p_this->m_b_runRes)
				{
					p_this->FireUIResult(sz_errCode);
				}
			}
			else
			{
				//p_this->FireUIMessage("Check Picasso Duplicate Success.");
			}		
		}
		

		/* Initial FA */
		if (b_res)
		{
			if (!(b_res = p_this->InitialFA()))
			{
				//p_this->FireUIMessage("Initial FA Fail.");

				if (p_this->m_b_runRes)
				{
					CStringToChar(CommErr_UI_Interface_SetFA_Fail,sz_errCode);
					p_this->FireUIResult(sz_errCode);
				}
			}
			else
			{
				p_this->FireUIMessage("Initial FA Success.");
			}		
		}
		
		/* Write FA */
		if (b_res)
		{
			if (!(b_res = p_this->WriteFA(sz_errCode)))
			{
				//p_this->FireUIMessage("WriteFA Fail.");
				if (p_this->m_b_runRes)
				{
					p_this->FireUIResult(sz_errCode);
				}
			} 
			else
			{
				//p_this->FireUIMessage("Write FA Test Result Success.");
			}
			
		}
		

		/* Write CIM */
		if (b_res)
		{
			if (!p_this->InsertPicassoControl(sz_errCode))
			{
				//p_this->FireUIMessage("Write PicassoCheck Fail.");
				if (p_this->m_b_runRes)
				{
					p_this->FireUIResult(sz_errCode);
				}
			}
			else
			{
				//p_this->FireUIMessage("Write PicassoCheck Success.");
			}		
		}	
	}
	
#else
	if ((p_this->m_p_st_uiControl->b_ReadFA) && (p_this->m_p_st_uiControl->b_WriteFA) && b_res)
	{
		/* Read FA */
		if (!(b_res = p_this->ReadFA()))
		{
			//p_this->FireUIMessage("ReadFA Fail.");

			if (p_this->m_b_runRes)
			{
				CStringToChar(CommErr_UI_Interface_GetFA_Fail,sz_errCode);
				p_this->FireUIResult(sz_errCode);
			}
		}
		else
		{
			//p_this->FireUIMessage("Read FA Success.");
		}

		/* Initial FA */
		if (b_res)
		{
			if (!(b_res = p_this->InitialFA()))
			{
				//p_this->FireUIMessage("Initial FA Fail.");

				if (p_this->m_b_runRes)
				{
					CStringToChar(CommErr_UI_Interface_SetFA_Fail,sz_errCode);
					p_this->FireUIResult(sz_errCode);
				}
			}
			else
			{
				//p_this->FireUIMessage("Initial FA Success.");
			}		
		}

		/* Write FA */
		if (b_res)
		{
			if (!(b_res = p_this->WriteFA(sz_errCode)))
			{
				//p_this->FireUIMessage("WriteFA Fail.");
				if (p_this->m_b_runRes)
				{
					p_this->FireUIResult(sz_errCode);
				}
			} 
			else
			{
				//p_this->FireUIMessage("Write FA Test Result Success.");
			}
		}
	}
#endif
	//::AfxMessageBox(_T("Before postrun")); //Neil Test
	/* Post Run */
	p_this->m_p_handsetOtherStationInterface->PostRun(p_this->m_i_slot);
	//::AfxMessageBox(_T("After postrun")); //Neil Test

	return 1;
}

bool CHandsetOtherStation::ReadFA(char* sz_picasso)
{	
	//CSingleLock singleLock(&m_obj_CritSection,true);
		
	if (m_map_StationIndexMap[m_p_st_uiParameter->str_station] != _T("01"))
	{
		if ( !m_p_st_uiControl->b_ReadFA || !m_p_st_uiControl->b_WriteFA)
		{
			if (sz_picasso == NULL)
			{
				return true;
			}
		}
	}

	////FireUIMessage("Start to Read FA..");

	if (m_p_st_uiControl->i_FAArea == OLDFA)
	{
		/* Read FA */
		char sz_readFAData[512] = {0};
		if (!(m_p_handsetOtherStationInterface->GetFAData(m_i_slot, sz_readFAData, sizeof(sz_readFAData))))
		{
			////FireUIMessage("Read FA from phone Fail.");
			return false;
		}

		/* Set FA data to FA Operator */
		if (!m_obj_FA.SetFA(sz_readFAData))
		{		
			////FireUIMessage("FAOperator SetFA Fail.");
			return false;
		}

		if (sz_picasso != NULL)
		{
			m_obj_FA.GetPicassoNumber(sz_picasso);
		}
		
		/* FA log */
		CString str_tmp;
		str_tmp.Format(_T("[%d]Read FA Data success: %s"), m_i_slot, ConvertToVisibleString(sz_readFAData,FA_SIZE));
		//FireUIMessage("Read FA Data success");
		LogMsg(str_tmp);
	}
	else if (m_p_st_uiControl->i_FAArea == NEWFA)
	{
		/* Read FA */
		char sz_readFAData[512] = {0};
		if (!(m_p_handsetOtherStationInterface->GetFASector(m_i_slot, 0, sz_readFAData, sizeof(sz_readFAData))))
		{
			//FireUIMessage("Read FA from phone Fail.");
			return false;
		}

		/* Set FA data to FA Operator */
		if (!m_obj_newFA.SetSectorData(sz_readFAData))
		{		
			//FireUIMessage("FAOperator SetFA Fail.");
			return false;
		}

		if (sz_picasso != NULL)
		{
			CStringToChar(m_obj_newFA.GetPicassoNumber(),sz_picasso);
		}

		/* FA log */
		CString str_tmp;
		str_tmp.Format(_T("[%d] Read FA Sector success: %s"), m_i_slot, ConvertString(sz_readFAData));
		//FireUIMessage("Read FA Sector success");
		LogMsg(str_tmp);
	}

	return true;
}

bool CHandsetOtherStation::InitialFA()
{
	if (m_map_StationIndexMap[m_p_st_uiParameter->str_station] != _T("01"))
	{
		return true;
	}

	if (m_p_st_uiControl->i_FAArea == NEWFA)
	{
		if (InitialFA_new())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/* If first station ,erase and initial FA,then set picasso */
	char sz_mobileid_byt[15]={0};
	char sz_picasso[15]={0};
	char sz_picassoUI[15]={0};

	CString str_tmp;
	/*read mobile id first to check if rework*/		

	m_obj_FA.GetPicassoNumber(sz_picasso);
	m_obj_FA.GetMobileID(sz_mobileid_byt);


	if (m_obj_FA.IsValidePicasso(sz_picasso) && m_obj_FA.IsValideMobileID(sz_mobileid_byt) )
	{		
		m_b_Rework = true;

		char sz_mobileid_str[30]={0};
		m_obj_FA.GetMobileIDHexStr(sz_mobileid_str);
		str_tmp.Format(_T("[%d] Now Enter Rework procedure. Mobile Picasso:%s,MobileID:%s"),m_i_slot,ConvertString(sz_picasso),ConvertString(sz_mobileid_str));
		LogMsg(str_tmp);		
	}


	/* Erase FA */
	if (!m_obj_FA.EraseFactoryArea())
	{
		//FireUIMessage("FA Operator EraseFactoryArea Fail.");
		return false;
	}

	/* Write Start up */
	char sz_stationcount[5] = {0};
	char sz_satationList[100] = {0};
	for (UINT i = 0;i < m_map_StationIndexMap.size();i++)
	{
		sz_satationList[i] = (char)(i+1);
	}
	sprintf(sz_stationcount, "%d", m_map_StationIndexMap.size());

	LogMsg(_T("FA_Station_Setup station list is ") + ConvertToVisibleString(sz_satationList,m_map_StationIndexMap.size()));
	LogMsg(_T("FA_Station_Setup station count is ") + ConvertString(sz_stationcount));

	if (!m_obj_FA.FA_Station_Setup(sz_stationcount, sz_satationList))
	{	
		//FireUIMessage("FA Operator FA_Station_Setup Fail.");
		return false;
	}

	//if rework,set to mobile old picasso and mobile id
	if (m_b_Rework)
	{	
		LogMsg(_T("Rework:SetPicassoNumber PicassoNumber is ") + ConvertString(sz_picasso));
		if (!m_obj_FA.SetPicassoNumber(sz_picasso))
		{
			//FireUIMessage("SetPicassoNumber in rework error.");
			return false;
		}
		LogMsg(_T("Rework:SetMobileID mobile id is ") + ConvertToVisibleString(sz_mobileid_byt,11));
		if (!m_obj_FA.SetMobileID(sz_mobileid_byt))
		{
			//FireUIMessage("SetMobileID in rework error.");
			return false;
		}			
	}
	//if new work, generate mobile id, get picasso from UI and set them to FA
	else
	{
		CStringToChar(m_str_picasso,sz_picasso);
		LogMsg(_T("new work:SetPicassoNumber PicassoNumber is ") + m_str_picasso);
		if (!m_obj_FA.SetPicassoNumber(sz_picasso))
		{
			//FireUIMessage("FA Operator Write Picasso Fail.");
			return false;
		}

		m_obj_FA.GenerateMobileID(sz_mobileid_byt,m_i_slot);

		LogMsg(_T("new work:SetMobileID mobile id is ") + ConvertToVisibleString(sz_mobileid_byt,11));

		if (!m_obj_FA.SetMobileID(sz_mobileid_byt))
		{
			//FireUIMessage("SetMobileID in new work error.");
			return false;
		}

	}

	/* Write ModelName */
	if (!m_obj_FA.SetModelName(m_p_st_uiParameter->str_modelName))
	{
		//FireUIMessage("FA Operator Write ModelName Fail.");
		return false;
	}

	/* Write SW Version */
	if (!m_obj_FA.SetSWVersion(m_p_st_uiParameter->str_userLoadVer))
	{
		//FireUIMessage("FA Operator Write SWVersion Fail.");
		return false;
	}

	char sz_TimeInfo[30] = {0};
	m_obj_FA.GetMobileIDHexStr(sz_TimeInfo);
	m_str_mobileid = ConvertString(sz_TimeInfo);

	/* Get FA data */
	char sz_FA[512] = {0};
	if (!m_obj_FA.GetFA(sz_FA))
	{
		//FireUIMessage("FA Operator GetFA Fail.");
		return false;
	}

	/* FA log*/ 
	str_tmp.Format(_T("[%d] Write FA DATA: %s"),m_i_slot,ConvertToVisibleString(sz_FA,512));
	LogMsg(str_tmp);

	//FireUIMessage("Begin: Set initial FA to Mobile");
	//CSingleLock singleLock(&m_obj_CritSection,true);
	/* Write FA */
	if (!m_p_handsetOtherStationInterface->SetFAData(m_i_slot, sz_FA))
	{
		//FireUIMessage("Set FA to Mobile fail.");
		return false;
	}

	char sz_checkFA[512]={0};
	/* Read FA */
	if (!m_p_handsetOtherStationInterface->GetFAData(m_i_slot, sz_checkFA,sizeof(sz_checkFA)))
	{
		//FireUIMessage("Read FA from Mobile fail.");
		return false;
	}

	str_tmp.Format(_T("[%d] Read FA DATA: %s"),m_i_slot,ConvertToVisibleString(sz_checkFA,512));
	LogMsg(str_tmp);

	m_obj_FA.SetFA(sz_checkFA);
	char sz_checkPicasso[20]={0};
	m_obj_FA.GetPicassoNumber(sz_checkPicasso);

	if (strcmp(sz_checkPicasso,sz_picasso) != 0)
	{
		//FireUIMessage("FA data write error occurred.");	
		return false;
	}

	//FireUIMessage("End: Set initial FA to Mobile");

	return true;
}

bool CHandsetOtherStation::InitialFA_new()
{
	/* Set basic data */
	if (!m_obj_newFA.IsValidePicasso())
	{
		m_obj_newFA.SetPicassoNumber(m_str_picasso);
		m_obj_newFA.SetImeiNumber(UNDEFINED);
		m_obj_newFA.SetServiceID(UNDEFINED);
		m_obj_newFA.SetModelName(m_p_st_uiParameter->str_modelName);
		m_obj_newFA.SetMobileID(m_i_slot);
		m_obj_newFA.SetNAL(UNDEFINED);

		char sz_faData[FA_SIZE] = {0};
		if (!m_obj_newFA.GetBasicData(sz_faData))
		{
			//FireUIMessage("Get FA Basic data Fail.");
			return false;
		}

		m_str_mobileid = m_obj_newFA.GetMobileID();

	//	CSingleLock singleLock(&m_obj_CritSection,true);
		/* Write FA */
		if (!m_p_handsetOtherStationInterface->SetFASector(m_i_slot, 0 , sz_faData ,sizeof(sz_faData)))
		{
			//FireUIMessage("Write FA Basic data Fail.");
			return false;
		}
		//FireUIMessage("Write FA Basic PASS.");

		/* FA log */
		CString str_tmp;
		str_tmp.Format(_T("[%d]Initial Write FA Data: %s"), m_i_slot, ConvertString(sz_faData));
		LogMsg(str_tmp);

		/* Read FA */
		char sz_checkFA[512]={0};
		if (!m_p_handsetOtherStationInterface->GetFASector(m_i_slot, 0 , sz_checkFA ,sizeof(sz_checkFA)))
		{
			//FireUIMessage("Read FA Basic data Fail.");
			return false;
		}
		
		m_obj_newFA.SetSectorData(sz_checkFA);

		CString str_checkpicasso = m_obj_newFA.GetPicassoNumber();
		if (m_str_picasso != str_checkpicasso)
		{
			//FireUIMessage("Read FA Basic data Fail.");
			return false;
		}
		str_tmp.Format(_T("[%d]Initial Read FA Data: %s"), m_i_slot, ConvertString(sz_checkFA));
		LogMsg(str_tmp);

	}
	else
	{
		m_b_Rework = true;
	}

	return true;
}

bool CHandsetOtherStation::WriteFA(char* sz_errCode)
{
	//CSingleLock singleLock(&m_obj_CritSection,true);


	if ( !m_p_st_uiControl->b_ReadFA || !m_p_st_uiControl->b_WriteFA)
	{
		return true;
	}


	//FireUIMessage("Write FA.");
	
	if (m_p_st_uiControl->i_FAArea == OLDFA)
	{
		/* Not write FA */
		if (!m_p_st_uiControl->b_WriteFA)
		{
			//FireUIMessage("Not Write FA.");
			return true;
		}

		/* Write Station Info */
		CString str_tmp;
		char sz_stationindex[5]={0};

		CStringToChar(m_map_StationIndexMap[m_p_st_uiParameter->str_station], sz_stationindex);

		if (!m_obj_FA.WriteStationInfo(sz_stationindex, "T", "0", "1", "0", "0" ))
		{
			char szErrmsg[512]={0};
			CStringToChar(m_obj_FA.GetErrorCode(),sz_errCode);
			CStringToChar(m_obj_FA.GetErrorMsg(),szErrmsg);		
			//FireUIMessage(szErrmsg);
			//FireUIMessage("Set FA station info Fail.");
			return false;
		}

		/* Get FA data */
		char sz_FA[512] = {0};
		if (!m_obj_FA.GetFA(sz_FA))
		{
			//FireUIMessage("FAOperator GetFA Fail.");
			return false;
		}

		/* Write FA */
		if (!m_p_handsetOtherStationInterface->SetFAData(m_i_slot, sz_FA))
		{
			CStringToChar(CommErr_UI_Interface_SetFA_Fail,sz_errCode);
			//FireUIMessage("Write FA to phone Fail.");
			return false;
		}

		/* FA log */ 
		str_tmp.Format(_T("[%d] Writed FA Data: %s"),m_i_slot,ConvertToVisibleString(sz_FA,512));
		LogMsg(str_tmp);
	}
	else if (m_p_st_uiControl->i_FAArea == NEWFA)
	{
		/* Set FA Record data */
		m_obj_newFA.SetRecordType(FACTORYLOG);
		m_obj_newFA.SetPartNum(m_p_st_uiParameter->str_partNumber);
		m_obj_newFA.SetSOType(m_p_st_uiParameter->str_soType);
		m_obj_newFA.SetPcba(m_p_st_uiParameter->str_pcba);
		m_obj_newFA.SetProcess(m_p_st_uiParameter->str_station);
		m_obj_newFA.SetSO(m_p_st_uiParameter->str_so);
		m_obj_newFA.SetToolVersion(m_p_st_uiParameter->str_toolVer);
		m_obj_newFA.SetUserVersion(m_p_st_uiParameter->str_userLoadVer);
		m_obj_newFA.SetSegment(m_p_st_uiParameter->str_segment);
		m_obj_newFA.SetEmployeeID(m_p_st_uiParameter->str_employee);
		m_obj_newFA.SetLine(m_p_st_uiParameter->str_line);
		m_obj_newFA.SetFactoryVersion(m_p_st_uiParameter->str_factoryVer);

		if (m_b_runRes == true)
		{
			m_obj_newFA.SetErrorcode(_T("PASS"));
			m_obj_newFA.SetTestResult(PASSRESULT);
		}
		else
		{
			m_obj_newFA.SetErrorcode(_T("FAIL"));
			m_obj_newFA.SetTestResult(FAILRESULT);
		}

		char sz_faData[FA_SIZE]={ 0 };
		if (!m_obj_newFA.GetRecordData(sz_faData))
		{
			//FireUIMessage("Get FA Record data Fail.");
			return false;
		}

		/* Write FA record */
		if (!m_p_handsetOtherStationInterface->SetFASector(m_i_slot, m_obj_newFA.GetPointer(), sz_faData, sizeof(sz_faData)))
		{
			CStringToChar(CommErr_UI_Interface_SetFA_Fail,sz_errCode);
			//FireUIMessage("Write FA Record data Fail.");
			return false;
		}
		//FireUIMessage("Write FA Record PASS.");

		/* FA log */
		CString str_tmp;
		str_tmp.Format(_T("[%d] Writed FA Record Data: %s"), m_i_slot, ConvertString(sz_faData));
		LogMsg(str_tmp);

		/* Update FA pointer */
		m_obj_newFA.IncRecordNum();
		memset(sz_faData,0x00,sizeof(sz_faData));

		if (!m_obj_newFA.GetBasicData(sz_faData))
		{
			//FireUIMessage("Get FA Basic data Fail.");
			return false;
		}

		/* Update FA pointer*/
		if (!m_p_handsetOtherStationInterface->SetFASector(m_i_slot,0, sz_faData, sizeof(sz_faData)))
		{
			CStringToChar(CommErr_UI_Interface_SetFA_Fail,sz_errCode);
			//FireUIMessage("Write FA Basic data Fail.");
			return false;
		}
		//FireUIMessage("Write FA pointer PASS.");

		/* FA log */
		str_tmp.Format(_T("[%d] Writed FA Basic Data[Update pointer]: %s"), m_i_slot, ConvertString(sz_faData));
		LogMsg(str_tmp);
	}
	
	return true;
}

bool CHandsetOtherStation::RestoreFA()
{
//	CSingleLock singleLock(&m_obj_CritSection,true);

	//FireUIMessage("Clear FA.");

	if (m_p_st_uiControl->i_FAArea == OLDFA)
	{

		TCHAR sz_errorCode[1024] = {0};
		TCHAR sz_errorMsg[1024] = {0};

		/* Erase FA */
		if (!m_obj_FA.EraseFactoryArea())
		{
			//FireUIMessage("FAOperator EraseFactoryArea Fail.");
			return false;
		}

		/* Get FA data */
		char sz_FA[512] = {0};
		if (!m_obj_FA.GetFA(sz_FA))
		{
			//FireUIMessage("FAOperator GetFA Fail.");
			return false;
		}

		/* Write FA */
		if (!m_p_handsetOtherStationInterface->SetFAData(m_i_slot, sz_FA))
		{
			//FireUIMessage("Write FA after clear FA Fail.");
			return false;
		}
	}
	else if (m_p_st_uiControl->i_FAArea == NEWFA)
	{
		TCHAR sz_errorCode[1024] = {0};
		TCHAR sz_errorMsg[1024] = {0};

		/* Erase FA */
		if (!m_obj_newFA.EraseFactoryArea())
		{
			//FireUIMessage("FAOperator EraseFactoryArea Fail.");
			return false;
		}

		/* Get FA basic data */
		char sz_FA[512] = {0};
		if (!m_obj_newFA.GetBasicData(sz_FA))
		{
			//FireUIMessage("FAOperator GetFA Fail.");
			return false;
		}

		/* Write FA */
		if (!m_p_handsetOtherStationInterface->SetFASector(m_i_slot, 0, sz_FA, sizeof(sz_FA)))
		{
			//FireUIMessage("Write FA after clear FA Fail.");
			return false;
		}
	}

	return true;
}


void CHandsetOtherStation::FireUIMessage(const char* sz_message)
{	
	/* Check input */
	if (sz_message == NULL)
	{
		return;
	}

	/* Send message */
	st_UIMessage st_message;
	st_message.i_slot = m_i_slot;
	strcpy_s(st_message.sz_message, sz_message);

	if (this->m_p_handsetOtherStationInterface != NULL)
	{
		m_p_handsetOtherStationInterface->Fire(UI_MESSAGE, (long)&st_message);
	}

	return;
}
	
void CHandsetOtherStation::FireUIResult(const char* sz_errCode, const char* sz_result)
{
	/* Check input */
	if (sz_result == NULL)
	{
		return;
	}

	/* Send result */
	st_UIResult st_result;
	st_result.i_slot = m_i_slot;
	strcpy_s(st_result.sz_itemCode, "");
	strcpy_s(st_result.sz_band, "");
	strcpy_s(st_result.sz_channel, "");
	strcpy_s(st_result.sz_errCode, sz_errCode);
	strcpy_s(st_result.sz_upper, "");
	strcpy_s(st_result.sz_lower, "");
	strcpy_s(st_result.sz_measured, "");
	strcpy_s(st_result.sz_unit, "");
	strcpy_s(st_result.sz_result, sz_result);
	strcpy_s(st_result.sz_msg, "");

	if( strcmp(sz_result, "FAIL") == 0 )
		strcpy_s(st_result.sz_itemCode, "#DLL#RUNDONE");

	if (this->m_p_handsetOtherStationInterface != NULL)
	{
		m_p_handsetOtherStationInterface->Fire(UI_RESULT, (long)&st_result);
	}

	return;
}

void CHandsetOtherStation::FireUIPicasso(const char* sz_picasso)
{	
	/* Send message */
	st_UIMessage st_message;
	st_message.i_slot = m_i_slot;
	strcpy_s(st_message.sz_message, sz_picasso);

	if (this->m_p_handsetOtherStationInterface != NULL)
	{
		m_p_handsetOtherStationInterface->Fire(UI_PICASSO, (long)&st_message);
	}
	return;
}


void CHandsetOtherStation::FireRelayControl( bool bOn )
{
	st_RelayBoard st_relayboard;
	st_relayboard.i_slot = m_i_slot;
	st_relayboard.i_Seq = m_i_slot + 1;
	st_relayboard.b_OnOff = bOn;

	if (this->m_p_handsetOtherStationInterface != NULL)
	{
		m_p_handsetOtherStationInterface->Fire(UI_RELAYBOARD, (long)&st_relayboard);
	}

	return;
}

bool CHandsetOtherStation::CheckPicassoDuplicate(char *sz_errCode)
{
	/* Not first station, No need */
	if (m_map_StationIndexMap[m_p_st_uiParameter->str_station] != _T("01"))
	{
		return true;
	}

	char sz_mobileCIM[30] = {0};
	char sz_picassoUI[15] = {0};
	char sz_status[5] = {0};
	char sz_Mobileid_str[30]={0};
	char sz_mobileid_byt[15]={0};
	char sz_Picasso[11]={0};
	CString str_tmp;
	char sz_message[1024];

	CStringToChar(m_str_picasso, sz_picassoUI);	

	if (m_p_st_uiControl->i_FAArea == OLDFA)
	{
		m_obj_FA.GetPicassoNumber(sz_Picasso);
		m_obj_FA.GetMobileID(sz_mobileid_byt);
		m_obj_FA.GetMobileIDHexStr(sz_Mobileid_str);
	}
	else if (m_p_st_uiControl->i_FAArea == NEWFA)
	{
		CStringToChar(m_obj_newFA.GetPicassoNumber(),sz_Picasso);
		CStringToChar(m_obj_newFA.GetMobileID(),sz_Mobileid_str);
	}
	
	/* Check picasso status */
	if (!m_obj_QueryCIM.CheckPicassoStatusFromCIM(sz_picassoUI,sz_mobileCIM,sz_status))
	{		
		CString str_DBerr = m_obj_QueryCIM.GetErrorMsg();
		CString str_DBerrCode = m_obj_QueryCIM.GetErrorCode();
		CStringToChar(str_DBerrCode,sz_errCode);
		CStringToChar(str_DBerr,sz_message);
		//FireUIMessage(sz_message);
		return false;
	}

	/* There is no record in table */
	if(((CString)sz_mobileCIM) == _T("")) 
	{
		if (m_p_st_uiControl->i_FAArea == OLDFA)
		{
			//there is picasso number in handset=> fail
			if(m_obj_FA.IsValidePicasso(sz_Picasso) && m_obj_FA.IsValideMobileID(sz_mobileid_byt)) 
			{
				str_tmp = _T("No record in CIM but found picasso and mobileid in target\nCIMい?Τ??も审い??picasso") + ConvertString(sz_Picasso);
				CStringToChar(str_tmp,sz_message);
				CStringToChar(CommErr_UI_CIM_Phone_Init_Not_Record,sz_errCode);
				//FireUIMessage(sz_message);
				return false;
			}

		}
		else if (m_p_st_uiControl->i_FAArea == NEWFA)
		{
			//there is picasso number in handset=> fail
			if(m_obj_newFA.IsValidePicasso()) 
			{
				str_tmp = _T("No record in CIM but found picasso and mobileid in target\nCIMい?Τ??も审い??picasso") + ConvertString(sz_Picasso);
				CStringToChar(str_tmp,sz_message);
				CStringToChar(CommErr_UI_CIM_Phone_Init_Not_Record,sz_errCode);
				//FireUIMessage(sz_message);
				return false;
			}
		}
	}

	/* there is record in table */
	else 
	{
		if (strcmp(sz_status, "L") == 0)
		{
			CStringToChar(CommErr_UI_CIM_Picasso_Locked,sz_errCode);
			//FireUIMessage("The picasso Number status error! \nPicasso砆?﹚");
			return false;

		}
		else if(strcmp(sz_status,"N") == 0)
		{
			if (m_p_st_uiControl->i_FAArea == OLDFA)
			{
				// there is picasso number in handset
				if(m_obj_FA.IsValidePicasso(sz_Picasso)) 
				{
					if (strcmp(sz_Picasso,sz_picassoUI) != 0)
					{
						CStringToChar(CommErr_UI_Picasso_Match_Phone_Fail,sz_errCode);
						str_tmp.Format(_T("Picasso in mobile mismatched with UI! \nも审いpicasso(%s)蒓??(%s)ぃ才"),ConvertString(sz_Picasso),m_str_picasso);
						CStringToChar(str_tmp,sz_message);
						//FireUIMessage(sz_message);
						return false;
					}

					//Time info not equal
					if(((CString)sz_mobileCIM) != ((CString)sz_Mobileid_str)) 
					{
						CStringToChar(CommErr_UI_CIM_MobileID_Match_Fail,sz_errCode);
						//FireUIMessage("The mobile ID not equal!\nも审いmobileid蒓CIMいぃ才");
						return false;
					}

				}
				//there is not picasso number in handset
				else 
				{
					CStringToChar(CommErr_UI_CIM_Phone_Reocrd_Not_Init,sz_errCode);
					//FireUIMessage("No picasso in target but found record in CIM!\nCIMいΤ??も审??Τ??");
					return false;
				}
			}
			else
			{
				// there is picasso number in handset
				if(m_obj_newFA.IsValidePicasso()) 
				{
					if (strcmp(sz_Picasso,sz_picassoUI) != 0)
					{
						CStringToChar(CommErr_UI_Picasso_Match_Phone_Fail,sz_errCode);
						str_tmp.Format(_T("Picasso in mobile mismatched with UI! \nも审いpicasso(%s)蒓??(%s)ぃ才"),ConvertString(sz_Picasso),m_str_picasso);
						CStringToChar(str_tmp,sz_message);
						//FireUIMessage(sz_message);
						return false;
					}

					//Time info not equal
					if(((CString)sz_mobileCIM) != ((CString)sz_Mobileid_str)) 
					{
						CStringToChar(CommErr_UI_CIM_MobileID_Match_Fail,sz_errCode);
						//FireUIMessage("The mobile ID not equal!\nも审いmobileid蒓CIMいぃ才");
						return false;
					}

				}
				//there is not picasso number in handset
				else 
				{
				CStringToChar(CommErr_UI_CIM_Phone_Reocrd_Not_Init,sz_errCode);
				//FireUIMessage("No picasso in target but found record in CIM!\nCIMいΤ??も审??Τ??");
				return false;
				}
			}
		}
		else if(strcmp(sz_status,"R") == 0 )
		{
			// there is picasso number in handset
			if(m_obj_FA.IsValidePicasso(sz_Picasso)) 
			{
				if (strcmp(sz_Picasso,sz_picassoUI) != 0)
				{
					CStringToChar(CommErr_UI_Picasso_Match_Phone_Fail,sz_errCode);
					str_tmp.Format(_T("Picasso in mobile mismatched with UI! \nも审いpicasso(%s)蒓??(%s)ぃ才"),ConvertString(sz_Picasso),m_str_picasso);
					CStringToChar(str_tmp,sz_message);
					//FireUIMessage(sz_message);
					return false;
				}

				//Time info not equal
				if(((CString)sz_mobileCIM) != ((CString)sz_Mobileid_str)) 
				{
					CStringToChar(CommErr_UI_CIM_MobileID_Match_Fail,sz_errCode);
					//FireUIMessage("The mobileid not equal!\nも审いmobileid蒓CIMいぃ才");
					return false;
				}

			}
		}
		//if get other status
		else
		{
			CStringToChar(CommErr_UI_CIM_Picasso_Status_Invalide,sz_errCode);
			//FireUIMessage("The picasso status error! \n picasso??ぃ猭");
			return false;
		}
	}

	return true;
}

bool CHandsetOtherStation::InsertPicassoControl( char *sz_errCode )
{
	//rework status will not insert picasso
	if (m_b_Rework)
	{
		return true;
	}

	//not first station,return true
	if (m_map_StationIndexMap[m_p_st_uiParameter->str_station] != _T("01"))
	{
		return true;
	}

	char sz_picasso[20] = {0};
	char sz_TimeInfo[30] = {0};
	char sz_status[2] = {'N'};
	char sz_message[1024]={0};

	CStringToChar(m_str_picasso,sz_picasso);	
	CStringToChar(m_str_mobileid,sz_TimeInfo);

	CString str_tmp = _T("");
	str_tmp.Format(_T("(%s),(%s),(%s) insert into CIM "),m_str_picasso,m_str_mobileid,sz_status);

	if (!m_obj_QueryCIM.InsertPicassoControlToCIM(sz_picasso,sz_TimeInfo,sz_status))
	{	
		Sleep(500);
		//insert for second time
		if (!m_obj_QueryCIM.InsertPicassoControlToCIM(sz_picasso,sz_TimeInfo,sz_status))
		{
			CString str_errorCode = m_obj_QueryCIM.GetErrorCode();
			CString str_errorMsg = m_obj_QueryCIM.GetErrorMsg();

			CStringToChar(str_errorCode,sz_errCode);
			CStringToChar(str_errorMsg,sz_message);
			//FireUIMessage(sz_message);
			str_tmp += _T("fail:");
			str_tmp += str_errorMsg;
			LogMsg(str_tmp);

			if (!RestoreFA())
			{
				char sz_message[] = "Insert PicassoControl twice Fail,Erase FA data Fail.";
				//FireUIMessage(sz_message);
			}
			else
			{
				char sz_message[] = "Insert PicassoControl twice Fail,Erase FA data ok.";
				//FireUIMessage(sz_message);
			}
			return false;
		}
	}		

	LogMsg(str_tmp + _T("success!"));

	return true;
}

void CHandsetOtherStation::LogMsg( CString logContent )
{
	m_obj_log.SaveLog(logContent);
}

bool CHandsetOtherStation::CheckRouting(char *sz_picasso,char *sz_errCode)
{
	char sz_errMsg[512]={0};
	CString str_picasso = ConvertString(sz_picasso);

	if (!m_p_st_uiParameter->b_tsFlag)
	{
		if (!Check_Picasso_Rule_All(str_picasso))
		{  
			CStringToChar(CommErr_UI_Picasso_Invalide, sz_errCode); 
			CStringToChar(_T("CheckPicassoRule fail"), sz_errMsg); 
			//FireUIMessage(sz_errMsg);
			return false;
		}
	}
	//FireUIMessage("Check picasso rule success!");

	if (!(m_p_st_uiParameter->b_tsFlag) && (m_p_st_uiParameter->str_station != STATION_BIST) && (m_p_st_uiParameter->str_station != STATION_REDL))
	{  
		/* Check CIM routing */ 
		m_obj_QueryCIM.SetidParameter(*m_p_st_uiParameter);//it must be set first

		if (!m_obj_QueryCIM.QueryStationRoutingInfoFromCIM(sz_picasso))
		{	
			CStringToChar(m_obj_QueryCIM.GetErrorCode(),sz_errCode);
			CStringToChar(m_obj_QueryCIM.GetErrorMsg(),sz_errMsg);
			//FireUIMessage(sz_errMsg);
			return false;
		}
	}

	//FireUIMessage("Check routing success!");

	return true;
}
