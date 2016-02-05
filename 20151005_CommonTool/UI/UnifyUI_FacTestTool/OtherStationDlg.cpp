


#include "stdafx.h"
#include "..\..\Modules\Common\CommonUtil\CommonDrawer.h"
#include "UnifyUI_FacTestTool.h"
#include "OtherStationDlg.h"
#include <Gdiplus.h>

#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;

IMPLEMENT_DYNAMIC(COtherStationDlg, CDialog)

COtherStationDlg::COtherStationDlg(CWnd* pParent)
	: CPageInterface(COtherStationDlg::IDD)
	, m_h_module (NULL)
	, m_p_facTestToolInterface (NULL)
	, m_str_picasso(_T(""))
	, m_p_Image(NULL)
{
	m_str_errorCode = _T("PASS");
	m_str_errorMessage = _T("");
	m_st_return.i_slot = 0;
	m_p_dlgParent = (CUnifyUI_FacTestToolDlg*)pParent;
	m_i_multiColumn = 0;
	m_AppendImages.clear();
	m_b_haveReadedFAFlag = false;
}

COtherStationDlg::~COtherStationDlg()
{
	if (NULL != m_h_module)
	{
		/* End */
		m_p_facTestToolInterface->End();

		/* Register */
		m_p_facTestToolInterface->Unregister(this, UI_ITEM);
		m_p_facTestToolInterface->Unregister(this, UI_MESSAGE);
		m_p_facTestToolInterface->Unregister(this, UI_PATTERN);
		m_p_facTestToolInterface->Unregister(this, UI_RESULT);
		m_p_facTestToolInterface->Unregister(this, UI_PICSDATA);
		m_p_facTestToolInterface->Unregister(this, UI_JIGCONTROL);
		m_p_facTestToolInterface->Unregister(this, UI_SHOWDIALOG);
		m_p_facTestToolInterface->Unregister(this, UI_SHOWIMGDIALOG);
		m_p_facTestToolInterface->Unregister(this, UI_RELAYBOARD);

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

	/* delete Image pointer */
	if(m_p_Image)
	{
		delete m_p_Image;
		m_p_Image = NULL;
	}

	GdiplusShutdown(gdiplusToken);

	m_brush.DeleteObject();
}

void COtherStationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_RF_STATIC_TESTITEM, m_label_testItem);
	DDX_Control(pDX, IDC_RF_STATIC_ID, m_label_ID);
	DDX_Control(pDX, IDC_STATIC_PRESCAN, m_label_Prescan);
	DDX_Control(pDX, IDC_LIST_RF_MESSAGE, m_listctrl_message);
	DDX_Text(pDX, IDC_EDIT_RF_ID, m_str_picasso);
	DDV_MaxChars(pDX, m_str_picasso, PICASSO_LENGTH);
	DDX_Control(pDX, IDC_LIST_RF_TESTITEM, m_listbox_testItem);
}

BEGIN_MESSAGE_MAP(COtherStationDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	//ON_NOTIFY ( NM_CUSTOMDRAW, IDC_LIST_RF_MESSAGE, OnCustomdrawList )
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &COtherStationDlg::OnBnClickedButtonNext)
END_MESSAGE_MAP()

BOOL COtherStationDlg::OnInitDialog()
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

	if (m_st_uiControl.b_PreScan)
	{
		GetDlgItem(IDC_BUTTON_NEXT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_PRESCAN)->ShowWindow(SW_SHOW);
	    GetDlgItem(IDC_STATIC_PRESCAN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PRESCAN)->EnableWindow(TRUE);		
		((CEdit*)GetDlgItem(IDC_EDIT_PRESCAN))->SetLimitText(PICASSO_LENGTH);
	}

	/*write station*/
	if (m_st_uiControl.b_WriteTagFrame)
	{
		//tag
		GetDlgItem(IDC_EDIT_TAG)->ShowWindow(SW_SHOW);
	    GetDlgItem(IDC_STATIC_TAG)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_TAG)->EnableWindow(true);		
		((CEdit*)GetDlgItem(IDC_EDIT_TAG))->SetLimitText(TAG_LENGTH);

		GetDlgItem(IDC_EDIT_RF_ID)->EnableWindow(false);
	}

	/*write sn*/
	if (m_st_uiControl.b_WriteSnFrame)
	{
		GetDlgItem(IDC_EDIT_SN)->ShowWindow(SW_SHOW);
	    GetDlgItem(IDC_STATIC_SN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_SN)->EnableWindow(true);		
		((CEdit*)GetDlgItem(IDC_EDIT_SN))->SetLimitText(SN_LENGTH);
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

bool COtherStationDlg::Init(void)
{
	m_str_invalidepicasso = _T("");

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

bool COtherStationDlg::InitUI(void)
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
	else if(!m_st_uiParameter.b_OpenbyEntranceTool)
	{
		LogMsg(_T("COtherStationDlg::InitUI b_OpenbyEntranceTool = false"));
		colour_dialogBKG = BKGCOLOUR_FACTORY_BAT;
	}
	else
	{
		LogMsg(_T("COtherStationDlg::InitUI b_OpenbyEntranceTool = true"));
		colour_dialogBKG = BKGCOLOUR_FACTORY;
	}
#endif

	m_brush.CreateSolidBrush(colour_dialogBKG);

	m_label_testItem.SetTextColor(colour_widgetText);
	m_label_testItem.SetBkColor(colour_dialogBKG);

	m_label_ID.SetTextColor(colour_widgetText);
	m_label_ID.SetBkColor(colour_dialogBKG);

	m_label_Prescan.SetTextColor(colour_widgetText);
	m_label_Prescan.SetBkColor(colour_dialogBKG);

	/* Init message list */
	//InitMessageList();

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	return true;
}

bool COtherStationDlg::LoadDLL(void)
{
	/* Confirm DLL Name */
	CString str_dllName;
	CString str_temp;

	str_dllName = DLLNAME;

	/* Load dll */
	m_h_module = LoadLibrary(str_dllName);
	if (m_h_module == NULL)
	{ 
		m_str_errorMessage = _T("Load ") + str_dllName + _T(" Fail!");
		LogMsg(m_str_errorMessage);
		return false;
	}

	/* Get function address */
	p_fn_newFacTestToolInterface p_newInterface = NULL;
	p_newInterface = (p_fn_newFacTestToolInterface)::GetProcAddress(m_h_module, (LPCSTR)DLL_NEW_INTERFACE);
	if (p_newInterface == NULL)
	{
		m_str_errorMessage = _T("Initial p_fn_newFacTestToolInterface Fail!");
		LogMsg(m_str_errorMessage);
		return false;
	}

	/* New interface */
	p_newInterface(&m_p_facTestToolInterface);
	if (m_p_facTestToolInterface == NULL)
	{
		m_str_errorMessage = _T("New IFacTestToolInterface Fail!");
		LogMsg(m_str_errorMessage);
		return false;
	}

	/* Register */
	m_p_facTestToolInterface->Register(this, UI_ITEM);
	m_p_facTestToolInterface->Register(this, UI_MESSAGE);
	m_p_facTestToolInterface->Register(this, UI_PATTERN);
	m_p_facTestToolInterface->Register(this, UI_RESULT);
	m_p_facTestToolInterface->Register(this, UI_PICSDATA);
	m_p_facTestToolInterface->Register(this, UI_JIGCONTROL);
	m_p_facTestToolInterface->Register(this, UI_SHOWDIALOG);
	m_p_facTestToolInterface->Register(this, UI_SHOWIMGDIALOG) ;
	m_p_facTestToolInterface->Register(this, UI_RELAYBOARD);
	
	return true;
}

bool COtherStationDlg::DLLBegin(void)
{
	/* Begin */
	if(!m_p_facTestToolInterface->Begin())
	{
		if (m_str_errorMessage == _T(""))
		{
			m_str_errorMessage = _T("Dll Begin Fail!");
		}
		return false;
	}

	return true;
}



bool COtherStationDlg::PostRun()
{
	return m_p_facTestToolInterface ->PostRun();
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
bool COtherStationDlg::InitTestItemList(void)
{
	if (m_p_facTestToolInterface == NULL)
	{
		m_str_errorMessage = _T("DLL do not loaded!");
		return false;
	}

	char sz_testItem[100] = {0};
	CString str_testItem;
	int i_itemCount = m_p_facTestToolInterface->GetItemCount();

	for (int i=0; i<i_itemCount; i++)
	{
		ZeroMemory(sz_testItem,sizeof(sz_testItem));

		if (m_p_facTestToolInterface->GetItemByIndex(i, sz_testItem, 100))
		{
			str_testItem = ConvertString(sz_testItem);
			m_listbox_testItem.AddString(str_testItem);
			m_map_testItem[str_testItem] = i;
		}
		else
		{
			m_str_errorMessage = _T("Get test item from DLL Fail!");
			LogMsg(m_str_errorMessage);
			return false;
		}
	}

	return true;
}

void COtherStationDlg::InitMessageList(bool b_multiColumn)
{
	if (b_multiColumn)
	{
		int i_colsWidth[12] = {72, 60, 60, 50, 50, 50, 50, 70, 38, 50, 50, 200};
		m_listctrl_message.InsertColumn(0, _T("Time"), LVCFMT_LEFT, i_colsWidth[0]);
		m_listctrl_message.InsertColumn(1, _T("ItemCode"), LVCFMT_RIGHT, i_colsWidth[1]);
		m_listctrl_message.InsertColumn(2, _T("Band"), LVCFMT_LEFT, i_colsWidth[2]);
		m_listctrl_message.InsertColumn(3, _T("Channel"), LVCFMT_LEFT, i_colsWidth[3]);
		m_listctrl_message.InsertColumn(4, _T("ErrorCode"), LVCFMT_RIGHT, i_colsWidth[4]);
		m_listctrl_message.InsertColumn(5, _T("UpperBound"), LVCFMT_RIGHT, i_colsWidth[5]);
		m_listctrl_message.InsertColumn(6, _T("LowerBound"), LVCFMT_RIGHT, i_colsWidth[6]);
		m_listctrl_message.InsertColumn(7, _T("EvaluatedValue"), LVCFMT_RIGHT, i_colsWidth[7]);
		m_listctrl_message.InsertColumn(8, _T("Unit"), LVCFMT_RIGHT, i_colsWidth[8]);
		m_listctrl_message.InsertColumn(9, _T("Result"), LVCFMT_CENTER, i_colsWidth[9]);
		m_listctrl_message.InsertColumn(10, _T("Elapse Time"), LVCFMT_RIGHT, i_colsWidth[10]);
		m_listctrl_message.InsertColumn(11, _T("Message"), LVCFMT_LEFT, i_colsWidth[11]);
	}
	else
	{
		m_listctrl_message.InsertColumn(11, _T("Message"), LVCFMT_LEFT, 500);
	}

	m_listctrl_message.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listctrl_message.SetView(LV_VIEW_DETAILS);

	return;
}

void COtherStationDlg::ClearMessageList(void)
{
	m_listctrl_message.DeleteAllItems();
}

 
 bool COtherStationDlg::GetTag(std::map<int, CString> &map_tag)
{
	/* Clear UI After Go */
	GetDlgItem(IDC_EDIT_TAG)->EnableWindow(false);

	USES_CONVERSION;

	/* Check if scan Tag */
	if (m_st_uiControl.b_ScanTag)
	{
		if (!CheckScanTAG())
		{
			return false;
		}
		CString str_scaned_tag;
		GetDlgItem(IDC_EDIT_TAG)->GetWindowText(str_scaned_tag);
		map_tag[0] = str_scaned_tag;
		m_p_facTestToolInterface->SetParameterValue(INIT_SLOT, "0");
		m_p_facTestToolInterface->SetParameterValue("TAG", T2A(map_tag[0]));
	}

	return true;
}

 bool COtherStationDlg::GetSn(std::map<int, CString> &map_sn)
{
	/* Clear UI After Go */
	GetDlgItem(IDC_EDIT_SN)->EnableWindow(false);

	USES_CONVERSION;

	/* Check if scan Tag */
	if (m_st_uiControl.b_ScanSn)
	{
		if (!CheckScanSN())
		{
			return false;
		}
		CString str_scaned_sn;
		GetDlgItem(IDC_EDIT_SN)->GetWindowText(str_scaned_sn);
		map_sn[0] = str_scaned_sn;
		m_p_facTestToolInterface->SetParameterValue(INIT_SLOT, "0");
		m_p_facTestToolInterface->SetParameterValue("SN", T2A(map_sn[0]));
	}

	return true;
}

bool COtherStationDlg::GetPicasso(std::map<int, CString> &map_picasso)
{
	/* Clear UI After Go */
	ClearMessageList();
	ResetLatestClock();
	GetDlgItem(IDC_EDIT_RF_ID)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_TAG)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_SN)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);

	USES_CONVERSION;

	/* Check if scan Picasso */
	if (m_st_uiControl.b_ScanPicasso)
	{
		if (!CheckScan())
		{
			return false;
		}
		CString str_scaned;
		GetDlgItem(IDC_EDIT_RF_ID)->GetWindowText(str_scaned);
		map_picasso[0] = str_scaned;
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
	/** 
	* @brief set InitSlot to DLL at first for all situation, 20140821
	*/
	m_p_facTestToolInterface->SetParameterValue(INIT_SLOT, "0");

	if (m_st_uiControl.b_ScanPicasso)
	{
		m_p_facTestToolInterface->SetParameterValue("PICASSO", T2A(map_picasso[0]));//Set picasso for PreRun, nancy.yang 20130802
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

		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
		GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));

		return false;
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
	}
	else
	{	
		/* Read FA */
		m_b_haveReadedFAFlag = false;
		char sz_faData[512] = {0};
		if (m_st_uiControl.i_FAArea == OLDFA)
		{
			if (!m_p_facTestToolInterface->GetFAData(0,sz_faData,sizeof(sz_faData)))
			{
				m_p_facTestToolInterface ->PostRun();

				m_st_return.str_errorMsg = _T("读取FA失败\nRead FA fail!");
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
 			if (!m_p_facTestToolInterface->GetFASector(0,0,sz_faData,sizeof(sz_faData)))
 			{
				m_p_facTestToolInterface ->PostRun();

 				m_st_return.str_errorMsg = _T("读取FA失败\nRead FA fail!");
 				m_st_return.str_errorcode = CommErr_UI_Interface_GetFA_Fail;
 				m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
 				GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));
 		
 				return false;
 			}
			m_obj_newFA.SetSectorData(sz_faData);
			m_str_picasso = m_obj_newFA.GetPicassoNumber();

			/* FA log*/ 	
			LogMsg(_T("GetFA DATA: ") + ConvertString(sz_faData));
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

		GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(m_str_picasso);

		/* Have Read FA */
		m_b_haveReadedFAFlag = true;
	}

	/* Set value to DLL */
	if (!m_st_uiControl.b_ScanPicasso)
	{
		m_p_facTestToolInterface->SetParameterValue("PICASSO", T2A(m_str_picasso));
	}

	/* Default slot = 1 */
	map_picasso[0] = m_str_picasso;

	return true;
}

void COtherStationDlg::Event(const char* str_event, long nParam)
{
	
	if (strcmp(str_event, UI_RESULT) == 0)
	{
		/* for 2g/3g/FT/Wls result */
		if (m_i_multiColumn == 0)  
		{
			m_i_multiColumn = 2;
			InitMessageList(true);	
		}

		st_UIResult* p_st_uiResult = (st_UIResult*)nParam;	
		m_str_errorCode = ConvertString(p_st_uiResult->sz_errCode);
		m_str_errorMessage = ConvertString(p_st_uiResult->sz_msg);
		if (m_i_multiColumn == 2)
		{
			AddMessageList(p_st_uiResult);
		}
		else //single column receives multi result
		{
			AddMessage(m_str_errorMessage);
		}
		
	}
	else if (strcmp(str_event, UI_MESSAGE) == 0)
	{
		/* for MMI result */
		if (m_i_multiColumn == 0)
		{
			m_i_multiColumn = 1;
			InitMessageList(false);
		}
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
		m_listbox_testItem.SetCurSel(m_map_testItem[str_testItem]);
	}
	else if (strcmp(str_event, UI_PICSDATA) == 0)
	{
		/* pics data*/
		st_PICSData* p_st_picsData = (st_PICSData*)nParam;
		m_p_dlgParent->SetPICSData(0, p_st_picsData->sz_key, p_st_picsData->sz_value);

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
		else if(str_request == _T("CARDIN"))
		{
			const BYTE b_command[10] = {"out04on#"};//focus on
			if (!m_p_dlgParent->SendCommand(b_command, 8))
			{
				m_p_facTestToolInterface->ResopnseToJig(false);
			} 
			else 
			{
				m_p_facTestToolInterface->ResopnseToJig(true);
			}
		}
		else if(str_request == _T("CARDOUT"))
		{
			const BYTE b_command[10] = {"out04off#"};//focus off
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
	else if (strcmp(str_event, UI_SHOWIMGDIALOG) == 0)
	{
		st_ShowImgDialog* p_st_showDialog = (st_ShowImgDialog*)nParam;
		
		CString strImgPath, strImgText, strButtonYES, strButtonNO ;
		strImgPath = ConvertString(p_st_showDialog->szImgPath) ;
		strImgText = ConvertString(p_st_showDialog->szImgText) ;
		strButtonYES = ConvertString(p_st_showDialog->sz_buttonIDOK) ;
		strButtonNO = ConvertString(p_st_showDialog->sz_buttonIDCANCEL) ;

		CPopUpImg popImgDlg;
		popImgDlg.SetImgPath(strImgPath, strImgText) ;
		popImgDlg.SetButtonName(strButtonYES, strButtonNO) ;

		if (popImgDlg.DoModal() == IDOK)
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

	return;
}

 clock_t COtherStationDlg::GetDurationTimeAndUpdateLatestClock(void)
 {
 	clock_t cl_curClock = clock();
 	clock_t cl_duration = cl_curClock - m_cl_LatestClock;
 	m_cl_LatestClock = cl_curClock;
 
 	return cl_duration;
 }
 
 void COtherStationDlg::ResetLatestClock(void)
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
void COtherStationDlg::AddMessageList(const st_UIResult* p_st_Msg)
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

	/*get current time and calculate the test time*/
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	double d_durationTime = (double)GetDurationTimeAndUpdateLatestClock()/CLOCKS_PER_SEC;
	str_durationTime.Format(_T("%.4lf"), d_durationTime);
	str_curTime.Format(_T("%02d:%02d:%02d.%03d"), systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);

	/*convert char[] into CString*/
	str_itemCode = ConvertString(p_st_Msg->sz_itemCode);
	str_band = ConvertString( p_st_Msg->sz_band);
	str_channel = ConvertString(p_st_Msg->sz_channel);
	str_errcode = ConvertString(p_st_Msg->sz_errCode);
	str_upper = ConvertString(p_st_Msg->sz_upper);
	str_lower = ConvertString(p_st_Msg->sz_lower);
	str_measured = ConvertString(p_st_Msg->sz_measured);
	str_unit = ConvertString(p_st_Msg->sz_unit);
	str_result = ConvertString(p_st_Msg->sz_result);
	str_message = ConvertString(p_st_Msg->sz_msg);

	/*insert test result to message list control*/
	int i_count = m_listctrl_message.GetItemCount();
	m_listctrl_message.InsertItem(i_count, str_curTime);
	m_listctrl_message.SetItemText(i_count, 1, str_itemCode);
	m_listctrl_message.SetItemText(i_count, 2, str_band);
	m_listctrl_message.SetItemText(i_count, 3, str_channel);
	m_listctrl_message.SetItemText(i_count, 4, str_errcode);
	m_listctrl_message.SetItemText(i_count, 5, str_upper);
	m_listctrl_message.SetItemText(i_count, 6, str_lower);
	m_listctrl_message.SetItemText(i_count, 7, str_measured);
	m_listctrl_message.SetItemText(i_count, 8, str_unit);
	m_listctrl_message.SetItemText(i_count, 9, str_result);
	m_listctrl_message.SetItemText(i_count, 10, str_durationTime);
	m_listctrl_message.SetItemText(i_count, 11, str_message);
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
void COtherStationDlg::AddMessage(CString str_info)
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

bool COtherStationDlg::ShowResultDlg(const CString str_errorCode, const CString str_message)
{
	LogMsg(str_errorCode + str_message);

  	m_dlg_result.SetText(str_errorCode, str_message);
  	m_dlg_result.DoModal();

	return true;
}

bool COtherStationDlg::Run(void)
{
	bool b_res = true;
	CString str_errcode;
	CString str_msg;
	m_str_errorCode = _T("");
	m_str_errorMessage = _T("");

	/* Check Picasso */

	LogMsg(m_map_StationIndexMap[m_st_uiParameter.str_station]);
	LogMsg(m_st_uiParameter.str_segment);

	if (!m_st_uiParameter.b_tsFlag)
	{
		/* If first station ,erase and initial FA,then set Picasso */
#ifdef FACTORY_MODE
		if ((m_map_StationIndexMap[m_st_uiParameter.str_station]==_T("01") && m_st_uiParameter.str_segment==_T("TLINE")) || m_st_uiControl.b_ReadFA || m_st_uiControl.b_WriteFA)
#else
		if (m_st_uiControl.b_ReadFA || m_st_uiControl.b_WriteFA)
#endif
		{
			{
				b_res = ReadFA();				
			}
		}

		if (b_res)
		{
#ifdef FACTORY_MODE
			if (m_map_StationIndexMap[m_st_uiParameter.str_station]==_T("01") && m_st_uiParameter.str_segment==_T("TLINE"))
#else
			//if (m_st_uiControl.b_WriteFA && (m_map_StationIndexMap[m_st_uiParameter.str_station]==_T("01") && m_st_uiParameter.str_segment==_T("TLINE")))
			//if (m_st_uiControl.b_WriteFA && (m_map_StationIndexMap[m_st_uiParameter.str_station]==_T("01")))//20140210 changed by Nancy.Yang,there have no segment define in RD mode 
			if (m_st_uiControl.b_WriteFA && (true))//20151007 lion
#endif
			{
				b_res = InitialFA();				
			}
		}

#ifdef FACTORY_MODE
		if(b_res)
		{
			//If sencond station ,double check PICASSO duplicate
			if (m_map_StationIndexMap[m_st_uiParameter.str_station]==_T("02") && m_st_uiParameter.str_segment==_T("TLINE"))
			{
				b_res = CheckPicasso();
			}	
		}	
#endif
	}	

	if (b_res)
	{
		/* Run */
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

			WriteFA("N");
		}

	}
	else
	{
		str_errcode = m_str_errorCode;
		str_msg = m_str_errorMessage;
	}

	m_p_facTestToolInterface->PostRun();
	LogMsg(_T("PostRun finished"));

	m_st_return.str_errorcode = str_errcode;
	m_st_return.str_errorMsg = str_msg;
	m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);

	return b_res;
}

HBRUSH COtherStationDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor == CTLCOLOR_DLG)
	{
		return m_brush;
	}
	return hbr;
}

BOOL COtherStationDlg::OnEraseBkgnd(CDC * pDC)
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

BOOL COtherStationDlg::PreTranslateMessage(MSG* pMsg)
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

bool COtherStationDlg::ResetPage()
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
 		GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));
		GetDlgItem(IDC_EDIT_TAG)->SetWindowText(_T(""));
		GetDlgItem(IDC_EDIT_SN)->SetWindowText(_T(""));
 	}


	/*write station*/

	if (m_st_uiControl.b_WriteTagFrame)
	{
		GetDlgItem(IDC_EDIT_TAG)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TAG)->SetFocus();
	}
	else
	{
		GetDlgItem(IDC_EDIT_RF_ID)->EnableWindow(TRUE);
		((CEdit   *)GetDlgItem(IDC_EDIT_RF_ID))->SetFocus();
	}
	/*sn*/
	if (m_st_uiControl.b_WriteSnFrame)
	{
		GetDlgItem(IDC_EDIT_SN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SN)->SetFocus();
	}
	m_str_errorCode = _T("");
	m_str_errorMessage = _T("");

	return true;

}

bool COtherStationDlg::WriteFA(char* sz_result)
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

			LogMsg(_T("COtherStationDlg::Write FA Result DATA: ") + ConvertToVisibleString(sz_FA,512));

			if (!m_p_facTestToolInterface->SetFAData(0, sz_FA))
			{
				LogMsg(_T("SetFAData Fail"));
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
				LogMsg(m_str_errorMessage);
				return false;
			}

			/* Write FA record */
			if (!m_p_facTestToolInterface->SetFASector(0,m_obj_newFA.GetPointer(), sz_faData, sizeof(sz_faData)))
			{
				m_str_errorMessage = _T("Write FA Record to phone Fail.");
				LogMsg(m_str_errorMessage);
				return false;
			}

			m_obj_newFA.IncRecordNum();

			memset(sz_faData,0x00,sizeof(sz_faData));

			if (!m_obj_newFA.GetBasicData(sz_faData))
			{
				m_str_errorMessage = _T("Get FA Basic data Fail.");
				LogMsg(m_str_errorMessage);
				return false;
			}

			/* Update FA pointer*/
			if (!m_p_facTestToolInterface->SetFASector(0,0, sz_faData, sizeof(sz_faData)))
			{
				m_str_errorMessage = _T("Update FA Basic to phone Fail.");
				LogMsg(m_str_errorMessage);
				return false;
			}
		}
	}

	LogMsg(_T("Write FA finished"));	
	return true;
}

void COtherStationDlg::SetDisplayImage(const CString& str_DisplayImage)
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


bool COtherStationDlg::CheckPicasso()
{
	bool b_ret = false;
	char sz_Picasso[15] = {0};
	char sz_Mobileid_str[30] = {0};

	LogMsg(_T("Begin to Check picasso..."));
	
	b_ret = ReadFA();
	if (!b_ret)
	{
		return false;
	}

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

	//log mobile info
	CString str_tmp = _T("");
	str_tmp.Format(_T("Second station:%s;Mobile picasso:%s;MobileID:%s"),m_st_uiParameter.str_station,ConvertString(sz_Picasso),ConvertString(sz_Mobileid_str));
	LogMsg(str_tmp);

	/* Check Picasso duplicate */
	if (!m_p_dlgParent->CheckPicassoDuplicateFromCIM(sz_Picasso,sz_Mobileid_str))
	{
		m_str_errorMessage = m_p_dlgParent->getErrMsg();
		m_str_errorCode = m_p_dlgParent->getErrCode();
		LogMsg(_T("Check Picasso Duplicate Fail!"));
		return false;
	}

	LogMsg(_T("Check Picasso Duplicate Pass!"));
	return true;
}

bool COtherStationDlg::SetPicasso( std::map<int, CString> map_picasso )
{
	if (!map_picasso.empty())
	{
		m_str_invalidepicasso = map_picasso[0];
		LogMsg(_T("Invalidepicasso from UI is ") + m_str_invalidepicasso);
	}
		
	return true;
}

bool COtherStationDlg::SetTag( std::map<int, CString> map_tag )
{
	if (!map_tag.empty())
	{
		m_str_invalidetag = map_tag[0];
		LogMsg(_T("InvalideTag from UI is ") + m_str_invalidetag);
	}
		
	return true;
}

bool COtherStationDlg::SetSn( std::map<int, CString> map_sn )
{
	if (!map_sn.empty())
	{
		m_str_invalidesn = map_sn[0];
		LogMsg(_T("Invalide Sn from UI is ") + m_str_invalidesn);
	}
		
	return true;
}


bool COtherStationDlg::CheckPicassoDuplicate()
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

bool COtherStationDlg::ComparePicassoTag()
{
	CString str_picassoUI = _T("");
	GetDlgItem(IDC_EDIT_RF_ID)->GetWindowText(str_picassoUI);
			
	if (m_str_picasso.CompareNoCase(str_picassoUI) != 0)
	{
		m_p_facTestToolInterface ->PostRun();
		m_st_return.str_errorMsg.Format(_T("扫入的picasso(%s)与手机中(%s)的不匹配!\nPicasso match error!"),str_picassoUI,m_str_picasso);
		m_st_return.str_errorcode = CommErr_UI_Picasso_Match_Phone_Fail;
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
		GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));
		return false;
	}

	return true;
}

bool COtherStationDlg::CheckScanTAG()
{
	CString str_TagUI = _T("");
	GetDlgItem(IDC_EDIT_TAG)->GetWindowText(str_TagUI);
	
	m_st_return.str_errorcode = CommErr_UI_TAG_Not_Input;
	//if this station need scan picasso
	if (str_TagUI == _T(""))
	{
		m_st_return.str_errorMsg.Format(_T("本站(%s)需要扫入Tag!\nNo Tag scanned in!"),m_st_uiParameter.str_station);
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
		GetDlgItem(IDC_EDIT_TAG)->SetWindowText(_T(""));
		return false;
	}
	else
	{
		if (str_TagUI.GetLength() == TAG_LENGTH)
		{
			if((str_TagUI.SpanIncluding(_T("01231456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"))).GetLength() == TAG_LENGTH)
			{
				char sz_tag[15]={0};
				CStringToChar(str_TagUI,sz_tag);
			//	m_p_dlgParent->SetPICSData(0,"id",sz_tag);
			}
			else
			{
				m_st_return.str_errorMsg.Format(_T("TAG 含无效字符!\nTag  include invalid chars!"),str_TagUI);
				m_st_return.str_errorcode = CommErr_UI_Picasso_Invalide;
				m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
				GetDlgItem(IDC_EDIT_TAG)->SetWindowText(_T(""));
				return false;
			}
		}	
		else
		{
			m_st_return.str_errorMsg.Format(_T("TAG 长度不正确!\nTAG Length invalid!"),str_TagUI);
			m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
			GetDlgItem(IDC_EDIT_TAG)->SetWindowText(_T(""));
			return false;
		}
	}
	
	m_st_return.str_errorcode = "";
	return true;
}


bool COtherStationDlg::CheckScanSN()
{
	CString str_SnUI = _T("");
	GetDlgItem(IDC_EDIT_SN)->GetWindowText(str_SnUI);
	
	m_st_return.str_errorcode = CommErr_UI_SN_Not_Input;
	//if this station need scan picasso
	if (str_SnUI == _T(""))
	{
		m_st_return.str_errorMsg.Format(_T("本站(%s)需要扫入Serial Number!\nNo SN scanned in!"),m_st_uiParameter.str_station);
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
		GetDlgItem(IDC_EDIT_SN)->SetWindowText(_T(""));
		return false;
	}
	else
	{
		if (str_SnUI.GetLength() == SN_LENGTH)
		{
			if((str_SnUI.SpanIncluding(_T("01231456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"))).GetLength() == SN_LENGTH)
			{
				char sz_sn[41]={0};
				CStringToChar(str_SnUI,sz_sn);
			//	m_p_dlgParent->SetPICSData(0,"id",sz_tag);
			}
			else
			{
				m_st_return.str_errorMsg.Format(_T("SN 含无效字符!\nSN  include invalid chars!"),str_SnUI);
				m_st_return.str_errorcode = CommErr_UI_SN_Not_Input;
				m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
				GetDlgItem(IDC_EDIT_SN)->SetWindowText(_T(""));
				return false;
			}
		}	
		else
		{
			m_st_return.str_errorMsg.Format(_T("SN 长度不正确!\nSN Length invalid!"),str_SnUI);
			m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
			GetDlgItem(IDC_EDIT_SN)->SetWindowText(_T(""));
			return false;
		}
	}
	
	m_st_return.str_errorcode = "";
	return true;
}

bool COtherStationDlg::CheckScan()
{
	CString str_picassoUI = _T("");
	GetDlgItem(IDC_EDIT_RF_ID)->GetWindowText(str_picassoUI);
	
	//if this station need scan picasso
	if (str_picassoUI == _T(""))
	{
		m_st_return.str_errorMsg.Format(_T("本站(%s)需要扫入picasso!\nNo Picasso scanned in!"),m_st_uiParameter.str_station);
		m_st_return.str_errorcode = CommErr_UI_Picasso_Not_Input;
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
		GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));
		return false;
	}
	else
	{
		if (str_picassoUI.GetLength() == PICASSO_LENGTH)
		{
			if((str_picassoUI.SpanIncluding(_T("01231456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"))).GetLength() == PICASSO_LENGTH)
			{
				char sz_picasso[15]={0};
				CStringToChar(str_picassoUI,sz_picasso);
				m_p_dlgParent->SetPICSData(0,"id",sz_picasso);
			}
			else
			{
				m_st_return.str_errorMsg.Format(_T("picasso 含无效字符!\nPicasso include invalid chars!"),str_picassoUI);
				m_st_return.str_errorcode = CommErr_UI_Picasso_Invalide;
				m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
				GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));
				return false;
			}
		}	
		else
		{
			m_st_return.str_errorMsg.Format(_T("picasso 长度不正确!\nPicasso Length invalid!"),str_picassoUI);
			m_st_return.str_errorcode = CommErr_UI_Picasso_Invalide;
			m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
			GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(_T(""));
			return false;
		}
	}

	return true;
}

void COtherStationDlg::LogMsg( CString logContent )
{
	m_obj_log.SaveLog(logContent);
}

//  void COtherStationDlg::OnCustomdrawList( NMHDR* pNMHDR, LRESULT* pResult )
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


void COtherStationDlg::OnBnClickedButtonNext()
{
	// TODO: Add your control notification handler code here
	CString str_nextpicasso;
	GetDlgItem(IDC_EDIT_PRESCAN)->GetWindowText(str_nextpicasso);
	if (str_nextpicasso.GetLength() == PICASSO_LENGTH)
	{
		GetDlgItem(IDC_EDIT_RF_ID)->SetWindowText(str_nextpicasso);
		if (m_st_uiControl.b_AutoRunPreScan)
		{
			m_p_dlgParent->PostMessage(WM_KEYDOWN, VK_RETURN, 0);		
		}
		GetDlgItem(IDC_EDIT_PRESCAN)->SetWindowText(_T(""));
		(CEdit*)GetDlgItem(IDC_EDIT_PRESCAN)->SetFocus();
	}
}

bool COtherStationDlg::SetParameterToDLL( void )
{
	CString strTemp;
	USES_CONVERSION;

#ifdef RD_MODE
	if (!m_p_facTestToolInterface->SetParameterValue(TOOLMODE, "RD"))
	{
		m_str_errorMessage.Format(_T("Tool mode(%s) is invalide!"), _T("RD"));
		return false;
	}
#else
	if (!m_p_facTestToolInterface->SetParameterValue(TOOLMODE, "FACTORY"))
	{
		m_str_errorMessage.Format(_T("Tool mode(%s) is invalide!"), _T("FACTORY"));
		return false;
	}
#endif

	if (!m_p_facTestToolInterface->SetParameterValue(LOGPATH, T2A(LOG_PATH)))
	{
		m_str_errorMessage.Format(_T("Log path(%s) is invalide!"), LOG_PATH);
		return false;
	}

	strTemp.Format(_T("STATION_NAME---%s"),m_st_uiParameter.str_station);
	LogMsg(strTemp);
	if (!m_p_facTestToolInterface->SetParameterValue(STATION_NAME, T2A(m_st_uiParameter.str_station)))
	{
		m_str_errorMessage.Format(_T("Station Name(%s) is invalide!"), m_st_uiParameter.str_station);
		return false;
	}
	
	strTemp.Format(_T("MODEL_NAME---%s"),m_st_uiParameter.str_modelName);
	LogMsg(strTemp);
	if (!m_p_facTestToolInterface->SetParameterValue(MODEL_NAME, T2A(m_st_uiParameter.str_modelName)))
	{
		m_str_errorMessage.Format(_T("Model Name(%s) is invalide!"), m_st_uiParameter.str_modelName);
		return false;
	}

	/*if (!m_p_facTestToolInterface->SetParameterValue(HW_VERSION, T2A(m_st_uiParameter.str_pcba)))
	{
		m_str_errorMessage.Format(_T("HW Name(%s) is invalide!"), m_st_uiParameter.str_pcba);
		return false;
	}*/
	
	strTemp.Format(_T("SW_VERSION---%s"),m_st_uiParameter.str_userLoadVer);
	LogMsg(strTemp);
	if (!m_p_facTestToolInterface->SetParameterValue(SW_VERSION, T2A(m_st_uiParameter.str_userLoadVer)))
	{
		m_str_errorMessage.Format(_T("UserLoad version(%s) is invalide!"), m_st_uiParameter.str_userLoadVer);
		return false;
	}

	//del by luffy, get tool version from dll
	/*if (!m_p_facTestToolInterface->SetParameterValue(TOOL_VERSION, T2A(m_st_uiParameter.str_toolVer)))
	{
		m_str_errorMessage.Format(_T("Tool Version(%s) is invalide!"), m_st_uiParameter.str_toolVer);
		return false;
	}*/
	
	strTemp.Format(_T("FACTORY_VERSION---%s"),m_st_uiParameter.str_factoryVer);
	LogMsg(strTemp);
	if (!m_p_facTestToolInterface->SetParameterValue(FACTORY_VERSION, T2A(m_st_uiParameter.str_factoryVer)))
	{
		m_str_errorMessage.Format(_T("Factory Version(%s) is invalide!"), m_st_uiParameter.str_factoryVer);
		return false;
	}
	
	strTemp.Format(_T("RESERVE1---%s"),m_st_uiParameter.str_Reserve1);
	LogMsg(strTemp);
	if (!m_p_facTestToolInterface->SetParameterValue(RESERVE1, T2A(m_st_uiParameter.str_Reserve1)))
	{
		m_str_errorMessage.Format(_T("Reserver1 value(%s) is invalide!"), m_st_uiParameter.str_Reserve1);
		return false;
	}
    
	strTemp.Format(_T("RESERVE2---%s"),m_st_uiParameter.str_Reserve2);
	LogMsg(strTemp);
	if (!m_p_facTestToolInterface->SetParameterValue(RESERVE2, T2A(m_st_uiParameter.str_Reserve2)))
	{
		m_str_errorMessage.Format(_T("Reserver2 value(%s) is invalide!"), m_st_uiParameter.str_Reserve2);
		return false;
	}

	std::map<CString,CString>::iterator Itr;
	for (Itr = m_map_keyvalue.begin();Itr!= m_map_keyvalue.end();Itr++)
	{
		if (!m_p_facTestToolInterface->SetParameterValue(T2A(Itr->first),T2A(Itr->second)))
		{
			m_str_errorMessage.Format(_T("(%s,%s) is invalide!"),Itr->first,Itr->second);
			return false;
		}
		LogMsg(Itr->first + _T("--") + Itr->second);
	}

	return true;
}

bool COtherStationDlg::GetParameterFromDLL( void )
{
	/* Get Tool Version */
	char sz_toolversion[30]={0};
	if (!m_p_facTestToolInterface->GetParameterValue(TOOL_VERSION, sz_toolversion,30))
	{
		m_str_errorMessage = _T("初始化失败！获取ToolVersion失败！");
		return false;
	}
	m_st_uiParameter.str_toolVer = ConvertString(sz_toolversion);

	m_p_dlgParent->SetUIIDParameter(m_st_uiParameter);

	return true;
}

bool COtherStationDlg::InitialFA()
{
	LogMsg(_T("Initial FA... "));

	if (m_st_uiControl.i_FAArea == NEWFA)
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

	char sz_Picasso[15]={0};
	char sz_PicassoUI[15]={0};
	char sz_Mobileid_str[30]={0};
	char sz_Mobileid_byt[15]={0};
	m_b_Rework = false;

	m_obj_faOperator.GetPicassoNumber(sz_Picasso);
	m_obj_faOperator.GetMobileID(sz_Mobileid_byt);

	if (m_obj_faOperator.IsValidePicasso(sz_Picasso) && m_obj_faOperator.IsValideMobileID(sz_Mobileid_byt))
	{					
		m_b_Rework = true;
	}
	else
	{
		/* Erase FA */
		LogMsg(_T("EraseFactoryArea for first station "));
		m_obj_faOperator.EraseFactoryArea();
	}

#ifdef FACTORY_MODE
	if (!CheckPicassoDuplicate())
	{
		LogMsg(_T("CheckPicassoDuplicate Fail,picasso in mobile mismatch with CIM"));
		return false;
	}
#endif

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
	if (!m_p_facTestToolInterface->SetFAData(0, sz_FA))
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

	return true;

}

bool COtherStationDlg::InitialFA_new()
{
	m_b_Rework = false;

	if (m_obj_newFA.IsValidePicasso())
	{
		m_b_Rework = true;
	}
	else
	{
		LogMsg(_T("EraseFactoryArea for first station "));
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
		/* lion */
		GetDlgItem(IDC_EDIT_RF_ID)->GetWindowText(m_str_picasso);		

		m_obj_newFA.SetPicassoNumber(m_str_picasso);
		m_obj_newFA.SetMobileID(0);
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

	if (!m_p_facTestToolInterface->SetFASector(0,0, sz_FA,sizeof(sz_FA)))
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

	return true;
}

bool COtherStationDlg::ReadFA()
{
	LogMsg(_T("Read FA... "));

	//Read FA from mobile
	if (!m_b_haveReadedFAFlag)
	{
		char sz_faData[FA_SIZE] = {0};
		bool b_ret = false;

		if (m_st_uiControl.i_FAArea == OLDFA)
		{
			b_ret = m_p_facTestToolInterface->GetFAData(0,sz_faData,sizeof(sz_faData));
		}
		else if (m_st_uiControl.i_FAArea == NEWFA)
		{
			b_ret = m_p_facTestToolInterface->GetFASector(0,0,sz_faData,sizeof(sz_faData));
		}

		if (!b_ret)
		{
			m_str_errorCode = CommErr_UI_Interface_GetFA_Fail;
			m_str_errorMessage = _T("Get FA data fail!");
	//		m_p_facTestToolInterface ->PostRun();//20140210 delete by Nancy.Yang
			return false;
		}

		//Set FA to member
		if (m_st_uiControl.i_FAArea == OLDFA)
		{
			m_obj_faOperator.SetFA(sz_faData);
		}
		else if (m_st_uiControl.i_FAArea == NEWFA)
		{
			if (!m_obj_newFA.SetSectorData(sz_faData))
			{
				m_str_errorCode = CommErr_UI_Interface_GetFA_Fail;
				m_str_errorMessage = _T("SetSectorData Fail!");
				return false;
			}
		}

		LogMsg(_T("FA from target is ") + ConvertToVisibleString(sz_faData,512));
	}
	else
		LogMsg(_T("Has read FA before, skip it..."));

	return true;
}

void COtherStationDlg::ChangeGB()
{
	colour_dialogBKG = BKGCOLOUR_FACTORY;
	LogMsg(_T(" COtherStationDlg::ChangeGB()"));
	m_brush.CreateSolidBrush(BKGCOLOUR_FACTORY);

	m_label_testItem.SetBkColor(BKGCOLOUR_FACTORY);

	m_label_ID.SetBkColor(BKGCOLOUR_FACTORY);

	m_label_Prescan.SetBkColor(BKGCOLOUR_FACTORY);

}