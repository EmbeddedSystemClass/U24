#include "stdafx.h"
#include "UnifyUI_FacTestTool.h"
#include "IMEIFCDlg.h"

IMPLEMENT_DYNAMIC(CIMEIFCDlg, CDialog)

CIMEIFCDlg::CIMEIFCDlg(CWnd* pParent)
	: CPageInterface(CIMEIFCDlg::IDD)
	, m_h_module (NULL)
	, m_p_facTestToolInterface (NULL)
	, m_str_imei(_T(""))
{
	m_str_errorCode = _T("PASS");
	m_str_errorMessage = _T("");
	m_st_return.i_slot = 1;
	m_p_dlgParent = (CUnifyUI_FacTestToolDlg*)pParent;// Get MainDlg pointer
	m_b_haveReadedFAFlag = false;
}

CIMEIFCDlg::~CIMEIFCDlg()
{
	if (NULL != m_h_module)
	{
		/* End */
		m_p_facTestToolInterface->End();

		/* Register */
		m_p_facTestToolInterface->Unregister(this, UI_ITEM);
		m_p_facTestToolInterface->Unregister(this, UI_MESSAGE);
		m_p_facTestToolInterface->Unregister(this, UI_RESULT);
		m_p_facTestToolInterface->Unregister(this, UI_PICSDATA);
		m_p_facTestToolInterface->Unregister(this, UI_SHOWDIALOG);
		//m_p_facTestToolInterface->Unregister(this, UI_RELAYBOARD);
		
		/* Delete interface */
		p_fn_deleteFacTestToolInterface p_delInterface = NULL;
		p_delInterface = (p_fn_deleteFacTestToolInterface)::GetProcAddress(m_h_module, (LPCSTR)DLL_DEL_INTERFACE);
		if (p_delInterface && m_p_facTestToolInterface)
		{
			p_delInterface(m_p_facTestToolInterface);
		}

		::FreeLibrary(m_h_module);
		m_h_module = NULL;
	}

	m_brush.DeleteObject();
}

void CIMEIFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_IMEI_STATIC_INFO, m_label_info);
	DDX_Control(pDX, IDC_IMEI_STATIC_TESTITEM, m_label_testItem);
	DDX_Control(pDX, IDC_IMEI_STATIC_ID, m_label_ID);
	DDX_Control(pDX, IDC_IMEI_STATIC_NAL, m_label_NAL);
	DDX_Control(pDX, IDC_STATIC_Acer22, m_label_Acer22);
	DDX_Control(pDX, IDC_LIST_IMEI_INFO, m_listctrl_info);
	DDX_Control(pDX, IDC_LIST_IMEI_TESTITEM, m_listbox_testItem);
	DDX_Control(pDX, IDC_LIST_IMEI_MESSAGE, m_listbox_message);
	DDX_Text(pDX, IDC_EDIT_IMEI_ID, m_str_imei);
//	DDV_MaxChars(pDX, m_str_imei, IMEI_LENGTH);
	DDX_Text(pDX, IDC_EDIT_IMEI_NAL, m_str_NAL);
	DDV_MaxChars(pDX, m_str_NAL, NAL_LENGTH);
	DDX_Text(pDX, IDC_EDIT_IMEI_Acer22, m_str_Acer22);
	DDV_MaxChars(pDX, m_str_Acer22, Acer22_LENGTH);

}

BEGIN_MESSAGE_MAP(CIMEIFCDlg, CDialog)
	ON_WM_CTLCOLOR()
	//ON_NOTIFY ( NM_CUSTOMDRAW, IDC_LIST_RF_MESSAGE, OnCustomdrawList )
END_MESSAGE_MAP()

HBRUSH CIMEIFCDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor ==CTLCOLOR_DLG)
	{
		return m_brush;
	}

	return hbr;
}

BOOL CIMEIFCDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN){
		switch (pMsg->wParam){
			case VK_RETURN:
				if ((m_st_uiControll.b_NAL||m_st_uiControll.b_ACER22)&&((m_st_uiParameter.str_station == STATION_IMEI) || (m_st_uiParameter.str_station == STATION_NAL_SCAN)))
				{
					CString str_temp;
					GetDlgItem(IDC_EDIT_IMEI_ID)->GetWindowText(m_str_imei);
					GetDlgItem(IDC_EDIT_IMEI_NAL)->GetWindowText(str_temp);
					GetDlgItem(IDC_EDIT_IMEI_Acer22)->GetWindowText(m_str_Acer22);
					if (m_str_imei.GetLength() != IMEI_LENGTH)
					{
						GetDlgItem(IDC_EDIT_IMEI_ID)->SetWindowText(_T(""));
						GetDlgItem(IDC_EDIT_IMEI_ID)->SetFocus();
						pMsg->wParam = NULL;
					}
					else if (str_temp.GetLength() != NAL_LENGTH&&m_st_uiControll.b_NAL)
					{
						GetDlgItem(IDC_EDIT_IMEI_NAL)->SetWindowText(_T(""));
						GetDlgItem(IDC_EDIT_IMEI_NAL)->SetFocus();
						pMsg->wParam = NULL;
					}
					else if(m_str_Acer22.GetLength()!=Acer22_LENGTH&&m_st_uiControll.b_ACER22)
					{
						GetDlgItem(IDC_EDIT_IMEI_Acer22)->SetWindowText(_T(""));
						GetDlgItem(IDC_EDIT_IMEI_Acer22)->SetFocus();
						pMsg->wParam = NULL;
					}
					else //if input reversed
					{
						if (m_str_imei.Left(6) == m_st_uiControll.str_NAL6)
						{
							GetDlgItem(IDC_EDIT_IMEI_ID)->SetWindowText(str_temp);
							GetDlgItem(IDC_EDIT_IMEI_NAL)->SetWindowText(m_str_imei);
						}

						m_p_dlgParent->PostMessage(WM_KEYDOWN, VK_RETURN, 0);
						pMsg->wParam = NULL;
					}				
					
				}
				else if (m_st_uiParameter.str_station==STATION_FCHECK&&m_st_uiControll.b_ACER22)
				{
					CString str_temp;
					GetDlgItem(IDC_EDIT_IMEI_ID)->GetWindowText(m_str_imei);
					//GetDlgItem(IDC_EDIT_IMEI_NAL)->GetWindowText(str_temp);
					GetDlgItem(IDC_EDIT_IMEI_Acer22)->GetWindowText(m_str_Acer22);
					if (m_str_imei.GetLength() != IMEI_LENGTH)
					{
						GetDlgItem(IDC_EDIT_IMEI_ID)->SetWindowText(_T(""));
						GetDlgItem(IDC_EDIT_IMEI_ID)->SetFocus();
						pMsg->wParam = NULL;
					}
					else if(m_str_Acer22.GetLength()!=Acer22_LENGTH)
					{
						GetDlgItem(IDC_EDIT_IMEI_Acer22)->SetWindowText(_T(""));
						GetDlgItem(IDC_EDIT_IMEI_Acer22)->SetFocus();
						pMsg->wParam = NULL;
					}
					else //if input reversed
					{
						m_p_dlgParent->PostMessage(WM_KEYDOWN, VK_RETURN, 0);
						pMsg->wParam = NULL;
					}
				}
				else
				{
					m_p_dlgParent->PostMessage(WM_KEYDOWN, VK_RETURN, 0);
					pMsg->wParam = NULL;
				}
				
				break;

			case VK_ESCAPE:
				break;

			default:
				break;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

BOOL CIMEIFCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_p_dlgParent->GetUIIDParameter(m_st_uiParameter);//get parameter from UI
	m_p_dlgParent->GetTestKeyMap(m_map_keyvalue);//get IMEI parameter from UI
	m_p_dlgParent->GetStationListMap(m_map_StationIndexMap);//get station list from UI
	m_p_dlgParent->GetUIControlParameter(m_st_uiControll);
	GetStationVsDllMap(m_map_StationVsDll);              //get dll name list

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

bool CIMEIFCDlg::Init(void)
{
	
	/* Init UI */
	LogMsg(_T("begin init"));
	if (!InitUI())
	{
		ShowResultDlg(CommErr_UI_Init_Diag_Fail, m_str_errorMessage + _T("\n初始化对话框失败"));
		return false;
	}

	/* Load DLL */
	LogMsg(_T("begin load dll"));
	if (!LoadDLL())
	{
		 ShowResultDlg(CommErr_UI_DLL_Load_Fail, m_str_errorMessage + _T("\n加载 DLL 失败"));
		 return false;
	}

	
	/* set value to DLL */
	LogMsg(_T("begin set parameter to dll"));
	if (!SetParameterToDLL())// Set parameter to DLL and show it in information list control, must after LoadDLL()
	{
		ShowResultDlg(CommErr_UI_Interface_Setparameter_Fail, m_str_errorMessage);
		return false;
	}
	 
	/* DLL Begin */
	LogMsg(_T("begin dll begin"));
	if (!DLLBegin())
	{
		ShowResultDlg(CommErr_UI_Interface_Begin_Fail, m_str_errorMessage + _T("\nDLL Begin 失败"));
		return false;
	}

	/*get value form dll*/
	LogMsg(_T("begin get parameter from dll"));
	if (!GetParameterFromDLL())
	{
		ShowResultDlg(CommErr_UI_Interface_Getparameter_Fail, m_str_errorMessage);
		return false;
	}


	if (!InitTestItemList())//must after SetParameterToDLL() which is called in function LoadDll() 
	{
		return false;
	}

	return true;
}

bool CIMEIFCDlg::InitUI(void)
{
	/* Define Dialog BK Color*/
	COLORREF colour_dialogBKG;
	COLORREF colour_widgetText = RGB(0,0,0);

	if (m_st_uiControll.b_NAL)
	{
		GetDlgItem(IDC_EDIT_IMEI_NAL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_IMEI_STATIC_NAL)->ShowWindow(SW_SHOW);

		if ((m_st_uiParameter.str_station == STATION_IMEI) || (m_st_uiParameter.str_station == STATION_NAL_SCAN))
		{
			GetDlgItem(IDC_EDIT_IMEI_NAL)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_EDIT_IMEI_NAL)->EnableWindow(FALSE);
		}
			
	}

	if(m_st_uiControll.b_ACER22)
	{   
	    GetDlgItem(IDC_STATIC_Acer22)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_EDIT_IMEI_Acer22)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_IMEI_Acer22)->EnableWindow(TRUE);
	}
	

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

	m_brush.CreateSolidBrush(colour_dialogBKG);

	m_label_info.SetTextColor(colour_widgetText);
	m_label_info.SetBkColor(colour_dialogBKG);

	m_label_testItem.SetTextColor(colour_widgetText);
	m_label_testItem.SetBkColor(colour_dialogBKG);

	m_label_ID.SetTextColor(colour_widgetText);
	m_label_ID.SetBkColor(colour_dialogBKG);

	m_label_NAL.SetTextColor(colour_widgetText);
	m_label_NAL.SetBkColor(colour_dialogBKG);

	m_label_Acer22.SetTextColor(colour_widgetText);
	m_label_Acer22.SetBkColor(colour_dialogBKG);

	/* Initial information list control */
	m_listctrl_info.InsertColumn(0, _T("ID"), 0, 120);
	m_listctrl_info.InsertColumn(1, _T("Value"), 0, 300);
	m_listctrl_info.DeleteAllItems();
	m_listctrl_info.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES | LVS_EDITLABELS | LVS_EX_INFOTIP);
	m_listctrl_info.SetBkColor(RGB(247,247,255));
	m_listctrl_info.SetTextBkColor(RGB(247,247,255));
	m_listctrl_info.SetTextColor(RGB(136,0,0));

	return true;
}

bool CIMEIFCDlg::LoadDLL(void)
{
	CString str_dllName;
	CString str_temp;

	/*define dll name and function name*/
	if (m_map_StationVsDll.find(m_st_uiParameter.str_station) != m_map_StationVsDll.end())
	{
		str_dllName = m_map_StationVsDll[m_st_uiParameter.str_station];
	}
	else
	{
		str_temp.Format(_T("DLL isn't defined for this station:%s in UnifyUI, use station name.dll(%s.dll) instead"), m_st_uiParameter.str_station, m_st_uiParameter.str_station);
		str_dllName = m_st_uiParameter.str_station + _T(".dll");
		LogMsg(str_temp);
		//return false;
	}

	/* load dll */
	m_h_module = LoadLibrary(str_dllName);
	if (m_h_module == NULL)
	{
		m_str_errorMessage.Format(_T("Load %s fail!"),str_dllName);
		LogMsg(m_str_errorMessage);
		return false;
	}

	/* get function address */
	p_fn_newFacTestToolInterface p_newInterface = NULL;
	p_newInterface = (p_fn_newFacTestToolInterface)::GetProcAddress(m_h_module, (LPCSTR)DLL_NEW_INTERFACE);
	if (p_newInterface == NULL)
	{
		m_str_errorMessage = _T("Initial p_fn_newFacTestToolInterface fail!");
		LogMsg(m_str_errorMessage);
		return false;
	}

	/* new interface */
	p_newInterface(&m_p_facTestToolInterface);
	if (m_p_facTestToolInterface == NULL)
	{
		m_str_errorMessage = _T("New IFacTestToolInterface fail!");
		LogMsg(m_str_errorMessage);
		return false;
	}

	/* Register */
	m_p_facTestToolInterface->Register(this, UI_ITEM);
	m_p_facTestToolInterface->Register(this, UI_MESSAGE);
	m_p_facTestToolInterface->Register(this, UI_RESULT);
	m_p_facTestToolInterface->Register(this, UI_PICSDATA);
	m_p_facTestToolInterface->Register(this, UI_SHOWDIALOG);
	//m_p_facTestToolInterface->Register(this, UI_RELAYBOARD);

	return true;
}

bool CIMEIFCDlg::DLLBegin(void)
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

/*****************************************************************************
* Function name: InitTestItemList     
* Summary      : Get test item from dll and show them in current page
* Parameters   : void
*                
* Return       : void
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Nancy.Yang    2011/07/06          First version             
*****************************************************************************/
bool CIMEIFCDlg::InitTestItemList(void)
{
	char sz_testItem[100] = {0};
	CString str_testItem;
	int i_itemCount = m_p_facTestToolInterface->GetItemCount();

	for (int i=0;i<i_itemCount;i++)
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
			LogMsg(_T("Get test item from DLL fail!"));
			return false;
		}
	}
	return true;
}

/*****************************************************************************
* Function name: InitInformationList     
* Summary      : Get information from the main dialog and show them in current page
* Parameters   : void
*                
* Return       : void
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Nancy.Yang    2011/07/13          First version             
*****************************************************************************/
bool CIMEIFCDlg::AddInformationList(CString str_key, CString str_value)
{
	int i_count = m_listctrl_info.GetItemCount();
	m_listctrl_info.InsertItem(i_count, str_key);
	m_listctrl_info.SetItemText(i_count, 1, str_value);

	return true;
}

bool CIMEIFCDlg::SetParameterToDLL(void)
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
//20121029 del by Nancy, get tool version from dll
	//if (!m_p_facTestToolInterface->SetParameterValue(TOOL_VERSION,T2A( m_st_uiParameter.str_toolVer)))
	//{
	//	m_str_errorMessage.Format(_T("Tool Version(%s) is invalide!"), m_st_uiParameter.str_toolVer);
	//	return false;
	//}

	strTemp.Format(_T("PART_NUMBER---%s"),m_st_uiParameter.str_partNumber);
	LogMsg(strTemp);
	if (!m_p_facTestToolInterface->SetParameterValue(PART_NUMBER,T2A( m_st_uiParameter.str_partNumber)))
	{
		m_str_errorMessage.Format(_T("Part Number(%s) is invalide!"), m_st_uiParameter.str_partNumber);
		return false;
	}

	strTemp.Format(_T("MODEL_NAME---%s"),m_st_uiParameter.str_modelName);
	LogMsg(strTemp);
	if (!m_p_facTestToolInterface->SetParameterValue(MODEL_NAME,T2A(m_st_uiParameter.str_modelName)))
	{
		m_str_errorMessage.Format(_T("Model Name(%s) is invalide!"), m_st_uiParameter.str_modelName);
		return false;
	}

	strTemp.Format(_T("SO---%s"),m_st_uiParameter.str_so);
	LogMsg(strTemp);
	if (!m_p_facTestToolInterface->SetParameterValue(SO,T2A(m_st_uiParameter.str_so)))
	{
		m_str_errorMessage.Format(_T("SO(%s) is invalide!"), m_st_uiParameter.str_so);
		return false;
	}

	strTemp.Format(_T("STATION_NAME---%s"),m_st_uiParameter.str_station);
	LogMsg(strTemp);
	if (!m_p_facTestToolInterface->SetParameterValue(STATION_NAME,T2A(m_st_uiParameter.str_station)))
	{
		m_str_errorMessage.Format(_T("Station(%s) is invalide!"), m_st_uiParameter.str_station);
		return false;
	}

	strTemp.Format(_T("SW_VERSION---%s"),m_st_uiParameter.str_userLoadVer);
	LogMsg(strTemp);
	if (!m_p_facTestToolInterface->SetParameterValue(SW_VERSION, T2A(m_st_uiParameter.str_userLoadVer)))
	{
		m_str_errorMessage.Format(_T("UserLoad version(%s) is invalide!"), m_st_uiParameter.str_userLoadVer);
		return false;
	}

	strTemp.Format(_T("FACTORY_VERSION---%s"),m_st_uiParameter.str_factoryVer);
	LogMsg(strTemp);
	if (!m_p_facTestToolInterface->SetParameterValue(FACTORY_VERSION, T2A(m_st_uiParameter.str_factoryVer)))
	{
		m_str_errorMessage.Format(_T("Factory Version(%s) is invalide!"), m_st_uiParameter.str_factoryVer);
		return false;
	}

	if (!m_p_facTestToolInterface->SetParameterValue(LOGPATH, T2A(LOG_PATH)))//set at end
	{
		m_str_errorMessage.Format(_T("Log path(%s) is invalide!"), LOG_PATH);
		return false;
	}

	/* Set HW Version */
	/*if (!m_p_facTestToolInterface->SetParameterValue(HW_VERSION, T2A(m_st_uiParameter.str_pcba)))
	{
		m_str_errorMessage.Format(_T("HW version(%s) is invalide!"), m_st_uiParameter.str_pcba);
		return false;
	}*/
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
		AddInformationList(Itr->first,Itr->second);
		LogMsg(Itr->first + _T("--") + Itr->second);
	}

	return true;
	
}

void CIMEIFCDlg::AddMessage(const CString str_info)
{
	SYSTEMTIME st_time;
	GetLocalTime(&st_time);

	CString str_time;
	str_time.Format(_T("[%04d/%02d/%02d %02d:%02d:%02d]: "), st_time.wYear, st_time.wMonth, \
		st_time.wDay, st_time.wHour, st_time.wMinute, st_time.wSecond);

	/* Insert to Listbox */
	int i_count = m_listbox_message.GetCount();
	m_listbox_message.InsertString(i_count, str_time + str_info);
	m_listbox_message.SetCurSel(i_count);

	return;
}

void CIMEIFCDlg::ClearMessageList(void)
{
	m_listbox_message.ResetContent();
	GetDlgItem(IDC_EDIT_IMEI_ID)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_IMEI_NAL)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDIT_IMEI_Acer22)->EnableWindow(FALSE);
}

bool CIMEIFCDlg::GetPicasso(std::map<int, CString> &map_picasso)
{
	USES_CONVERSION;

	ClearMessageList();

	if (!CheckInput())
	{
		return false;
	}
	/* PreRun */
	if (m_p_facTestToolInterface == NULL)
	{
		m_st_return.str_errorMsg = _T("Load DLL Fail!");
		m_st_return.str_errorcode = CommErr_UI_DLL_Load_Fail;
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
		GetDlgItem(IDC_EDIT_IMEI_ID)->SetWindowText(_T(""));
		return false;
	}
	if ((!m_p_facTestToolInterface->PreRun()))
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
		GetDlgItem(IDC_EDIT_IMEI_ID)->SetWindowText(_T(""));

		
		return false;
	}

#ifndef FACTORY_MODE
	if (!m_st_uiControll.b_ReadFA)
	{
		/* Not Read FA */
		LogMsg(_T("RD mode, b_ReadFA is false, skip to get picasso"));
		return true;
	}
#endif

	CString str_picasso;
	char sz_faData[512] = {0};
	char sz_picasso[11] = {0};
	m_b_haveReadedFAFlag = false;

	m_p_facTestToolInterface->SetParameterValue("IMEI",T2A(m_str_imei));

	if (m_st_uiParameter.str_station != STATION_NAL_SCAN)
	{
		if (m_st_uiControll.i_FAArea == OLDFA)
		{
			if (!m_p_facTestToolInterface->GetFAData(1,sz_faData,sizeof(sz_faData)))
			{
				m_p_facTestToolInterface ->PostRun();

				m_st_return.str_errorMsg = _T("Dll return error! Read FA fail!");
				m_st_return.str_errorcode = CommErr_UI_Interface_GetFA_Fail;
				m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);

				return false;
			}

			m_obj_faOperator.SetFA(sz_faData);
			m_obj_faOperator.GetPicassoNumber(sz_picasso);
			str_picasso = ConvertString(sz_picasso);

			/* FA log*/
			LogMsg(_T("Get FA data: ") + ConvertToVisibleString(sz_faData,512));
		}
		else if (m_st_uiControll.i_FAArea == NEWFA)
		{
			if (!m_p_facTestToolInterface->GetFASector(1,0,sz_faData,sizeof(sz_faData)))
			{
				m_p_facTestToolInterface ->PostRun();

				m_st_return.str_errorMsg = _T("Dll return error! Read FA fail!");
				m_st_return.str_errorcode = CommErr_UI_Interface_GetFA_Fail;
				m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);


				return false;
			}

			m_obj_newFA.SetSectorData(sz_faData);
			str_picasso = m_obj_newFA.GetPicassoNumber();

			/* FA log*/ 	
			LogMsg(_T("GetFA DATA: ") + ConvertString(sz_faData));

			/* Have Read FA */
			m_b_haveReadedFAFlag = true;
		}
		else
		{
			LogMsg(_T("INI Setting Fail."));	
			return false;
		}
	}
	
	m_p_facTestToolInterface->SetParameterValue("PICASSO",T2A(str_picasso));
	map_picasso[1] = str_picasso;

#ifdef FACTORY_MODE
	if (m_st_uiParameter.str_station != STATION_NAL_SCAN)
	{
		if (!m_st_uiParameter.b_tsFlag && !CheckImeiPicasso(m_str_imei))
		{
			m_st_return.str_errorcode = m_str_errorCode;
			m_st_return.str_errorMsg = m_str_errorMessage;

			m_p_facTestToolInterface->PostRun();

			m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
			GetDlgItem(IDC_EDIT_IMEI_ID)->SetWindowText(_T(""));
			return false;
		}
	}
	else
	{
		char sz_imei[60]={0};
		char sz_model[10]={0};
		char sz_picasso[15]={0};

		CStringToChar(m_str_imei,sz_imei);
		CStringToChar(m_st_uiParameter.str_modelName,sz_model);

		if (!m_st_uiParameter.b_tsFlag && !QueryPicassoFromCIM(sz_imei,sz_model,sz_picasso))
		{
			m_st_return.str_errorcode = m_str_errorCode;
			m_st_return.str_errorMsg = m_str_errorMessage;
			m_p_facTestToolInterface->PostRun();

			m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
			return false;
		}

		str_picasso = ConvertString(sz_picasso);

		m_p_facTestToolInterface->SetParameterValue("PICASSO",T2A(str_picasso));
		map_picasso[1] = str_picasso;
	}
#endif
	

	return true;
}
/*****************************************************************************
* Function name: Check_IMEI_Validity     
* Summary      : check IMEI match the rule
* Parameters   : imei
*                
* Return       : void
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Nancy.Yang    2011/07/08          First version             
*****************************************************************************/
bool CIMEIFCDlg::Check_IMEI_Validity(const CString str_imei)
{
	int s = 0;
	int i = 0;
	int t = 0;

	for(i=0; i<14; i++)
	{
		if(i%2 == 0)
		{
			s += _ttoi(str_imei.Mid(i,1));
		}
		else
		{
			t = _ttoi(str_imei.Mid(i,1)) * 2;
			s = s + t%10 + t/10;
		}
	}

	if( (s % 10) != 0)
	{
		t = (s/10 + 1) * 10 - s;
	}
	else
	{
		t = 0;
	}

	if(t == _ttoi(str_imei.Mid(14,1)))
	{
		return true;
	}
	else 
	{
		return false;
	}
}
bool CIMEIFCDlg::Check_MEID_Validity(CString str_MEID)
{
	str_MEID.MakeLower();
	int s = 0;
	int i,t;
	for(i=0;i<14;i++)
	{
		if(i%2==0)
		{
			if(str_MEID.GetAt(i)>='0'&& str_MEID.GetAt(i)<='9')	
			{
				char temp1[10];
				CStringToChar(str_MEID.Mid(i,1), temp1);
				s = s + atoi(temp1);
				//s = s + atoi(str_MEID.Mid(i,1));
			}
			if(str_MEID.GetAt(i)>='a'&& str_MEID.GetAt(i)<='f')
				s = s + str_MEID.GetAt(i)-'a'+10;
		}
		else
		{
			if(str_MEID.GetAt(i)>='0'&& str_MEID.GetAt(i)<='9')
			{
				char temp1[10];
				CStringToChar(str_MEID.Mid(i,1), temp1);
				t = atoi(temp1)*2;
				//t=atoi(str_MEID.Mid(i,1))*2;
			}
			if(str_MEID.GetAt(i)>='a'&& str_MEID.GetAt(i)<='f')
			{
				t=(str_MEID.GetAt(i)-'a'+10)*2;

			}
			s=s+t%16+t/16;
		}
	}
	if(s%16!=0)
		t=(s/16+1)*16-s;
	else
		t=0;

	if(str_MEID.GetAt(14)>='0'&& str_MEID.GetAt(14)<='9')
	{
		char temp1[10];
		CStringToChar(str_MEID.Mid(14,1), temp1);
		if(t==atoi(temp1))
			return true;
	}
	else if(str_MEID.GetAt(14)>='a'&& str_MEID.GetAt(14)<='f')
	{
		if(t==str_MEID.GetAt(14)-'a'+10)
			return true;
	}
	return false;
	
}

bool CIMEIFCDlg::Check_IMEI_FirstPart(const CString str_imei, const CString str_imeiFirstPart)
{
	if (str_imeiFirstPart != str_imei.Left(str_imeiFirstPart.GetLength()))
	{
		return false;
	}

	return true;
}

/*****************************************************************************
* Function name: Check_IMEI_Difference     
* Summary      : Check IMEI if is equal to the last one.
* Parameters   : imei
*                
* Return       : void
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Nancy.Yang      2011/07/08          First version             
*****************************************************************************/
bool CIMEIFCDlg::Check_IMEI_Difference(const CString str_imei, const CString str_imeiLastWrite, const int i_imeiDiff)
{
	/* Check Input */

	TCHAR sz_temp[20] = {0}; 
	CString str_temp = str_imeiLastWrite.Right(7);
	str_temp = str_temp.Left(6);

	_stprintf_s(sz_temp, _T("%.6d"), _ttoi(str_temp) + i_imeiDiff);

	str_temp = str_imeiLastWrite.Left(8) + sz_temp;

	if (str_temp != str_imei.Left(14))
	{
		return false;
	}

	return true;
}

/*****************************************************************************
* Function name: Event     
* Summary      : Event deal with from DLL.
* Parameters   : str_event
*                
* Return       : void
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Nancy.Yang      2011/07/08          First version             
*****************************************************************************/
void CIMEIFCDlg::Event(const char* str_event, long nParam)
{
	if (strcmp(str_event, UI_RESULT) == 0)
	{
		/* UI Result */
		st_UIResult* p_st_uiResult = (st_UIResult*)nParam;
		m_str_errorCode = ConvertString(p_st_uiResult->sz_errCode);
		m_str_errorMessage = ConvertString(p_st_uiResult->sz_msg);
		AddMessage(m_str_errorMessage);
	}
	else if (strcmp(str_event, UI_MESSAGE) == 0)
	{
		/* UI Message */
		st_UIMessage* p_st_uiMessage = (st_UIMessage*)nParam;
		CString str_message;
		str_message = ConvertString(p_st_uiMessage->sz_message);
		AddMessage(str_message);
	}
	else if (strcmp(str_event, UI_ITEM) == 0)
	{
		/* UI Item: Focus on current test item */
		st_UIItem* p_st_uiItem = (st_UIItem*)nParam;
		CString str_testItem;
		str_testItem = ConvertString(p_st_uiItem->sz_item);
		m_listbox_testItem.SetCurSel(m_map_testItem[str_testItem]);
	}
	else if (strcmp(str_event, UI_PICSDATA) == 0)
	{
		/* PICS Data*/
		st_PICSData* p_st_picsData = (st_PICSData*)nParam;
		m_p_dlgParent->SetPICSData(1, p_st_picsData->sz_key, p_st_picsData->sz_value);
	}
	else if (strcmp(str_event, UI_SHOWDIALOG) == 0)
	{
		/* PICS Data*/
		st_ShowDialog* p_st_showDialog = (st_ShowDialog*)nParam;
		CString str_message;
		str_message = ConvertString(p_st_showDialog->sz_message);

		//if (str_message == _T("PASSWORD_DLG"))
		//{
		//	CPassWord PassWordDlg;
		//	if(PassWordDlg.DoModal()==IDCANCEL)
		//	{
		//		m_p_facTestToolInterface->ResopnseToShowDlg("0");
		//	}
		//	else
		//	{
		//		m_p_facTestToolInterface->ResopnseToShowDlg("1");
		//	}
		//}
	}

	return;
}

/*****************************************************************************
* Function name: IMEIFCRun     
* Summary      : Run.
* Parameters   : str_event
*                
* Return       : void
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Nancy.Yang      2011/07/08          First version             
*****************************************************************************/
bool CIMEIFCDlg::Run(void)
{
	bool b_return = false;	

	CString str_errcode = _T("");
	CString str_errMsg = _T("");

	USES_CONVERSION;
	m_p_facTestToolInterface->SetParameterValue("IMEI",T2A(m_str_imei));

	//run
	b_return = m_p_facTestToolInterface->Run();

	// write FA
	if (b_return)
	{
		b_return = WriteFA("Y");

		if(b_return)
		{
			str_errcode = _T("PASS");
			str_errMsg = _T("");
		}
		else
		{
			str_errcode = CommErr_UI_Interface_SetFA_Fail;
			str_errMsg = m_str_errorMessage;
		}
	}
	else
	{
		//record run result error
		str_errcode = m_str_errorCode;
		str_errMsg = m_str_errorMessage;
		b_return = WriteFA("N");
	}

	//PostRun
	m_p_facTestToolInterface->PostRun();

	// result
	m_st_return.str_errorcode = str_errcode;
	m_st_return.str_errorMsg = str_errMsg;
	m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);//will reset data

	m_str_imei = _T("");

	return b_return;
}

bool CIMEIFCDlg::ShowResultDlg(const CString str_errorCode, const CString str_message)
{
	LogMsg(str_errorCode + str_message);

	m_dlg_result.SetText(str_errorCode, str_message);
	m_dlg_result.DoModal();

	return true;
}


bool CIMEIFCDlg::ResetPage()
{
	m_str_imei = _T("");
	m_str_NAL = _T("");
	m_str_Acer22 = _T("");
	m_str_errorCode = _T("");
	m_str_errorMessage = _T("");

	GetDlgItem(IDC_EDIT_IMEI_ID)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_IMEI_NAL)->SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_IMEI_Acer22)->SetWindowText(_T(""));

	GetDlgItem(IDC_EDIT_IMEI_ID)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_IMEI_NAL)->EnableWindow(TRUE);
    GetDlgItem(IDC_EDIT_IMEI_Acer22)->EnableWindow(TRUE);

	((CEdit   *)GetDlgItem(IDC_EDIT_IMEI_ID))->SetFocus();

	return true;
}


bool CIMEIFCDlg::WriteFA(char* sz_result)
{
	bool b_writeFAFlag = false;

#ifdef FACTORY_MODE
	if (m_st_uiControll.b_WriteFA)
	{
		b_writeFAFlag = true;
	}
#else
	if ((m_st_uiControll.b_ReadFA) && (m_st_uiControll.b_WriteFA))
	{
		b_writeFAFlag = true;
	}
#endif

	if (b_writeFAFlag)
	{
		if (m_st_uiControll.i_FAArea == OLDFA)
		{
			/* Write Station Info */
			char sz_stationindex[5]={0};
			CStringToChar(m_map_StationIndexMap[m_st_uiParameter.str_station], sz_stationindex);

			if (!m_obj_faOperator.WriteStationInfo(sz_stationindex, "T", "0", sz_result, "0", "0" ))
			{
				AddMessage(_T("WriteStationInfo Fail in write FA result!"));
				LogMsg(_T("WriteStationInfo Fail in write FA result!"));
				m_str_errorCode = m_obj_faOperator.GetErrorCode();
				m_str_errorMessage = m_obj_faOperator.GetErrorMsg();
				return false;
			}

			char sz_FA[512] = {0};
			if (!m_obj_faOperator.GetFA(sz_FA))
			{
				m_str_errorMessage = _T("Get FA fail\n获取FA内容失败");
				LogMsg(m_str_errorMessage);
				AddMessage(m_str_errorMessage);
				return false;
			}

			LogMsg(_T("IMEI write FA :")+ConvertToVisibleString(sz_FA,512));
			AddMessage(_T("IMEI write FA :")+ConvertToVisibleString(sz_FA,512));
			if (!m_p_facTestToolInterface->SetFAData(1, sz_FA))
			{
				m_str_errorMessage = _T("Write FA to mobile fail\n写入FA到手机失败");
				LogMsg(m_str_errorMessage);
				AddMessage(m_str_errorMessage);
				return false;
			}
		}
		else if (m_st_uiControll.i_FAArea == NEWFA)
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
				m_str_errorMessage = _T("Get FA Record Fail.");
				LogMsg(m_str_errorMessage);
				AddMessage(m_str_errorMessage);
				return false;
			}


			/* Write FA record */
			if (!m_p_facTestToolInterface->SetFASector(1,m_obj_newFA.GetPointer(), sz_faData, sizeof(sz_faData)))
			{
				m_str_errorMessage = _T("Write FA Record to phone Fail.");
				LogMsg(m_str_errorMessage);
				AddMessage(m_str_errorMessage);
				return false;
			}

			m_obj_newFA.IncRecordNum();
			if (m_st_uiParameter.str_station == STATION_IMEI)
			{
				m_obj_newFA.SetImeiNumber(m_str_imei);
				if (m_st_uiControll.b_NAL)
				{
					m_obj_newFA.SetNAL(m_str_NAL);
				}
			}	

			memset(sz_faData,0x00,sizeof(sz_faData));

			if (!m_obj_newFA.GetBasicData(sz_faData))
			{
				m_str_errorMessage = _T("Get FA Basic data Fail.");
				LogMsg(m_str_errorMessage);
				AddMessage(m_str_errorMessage);
				return false;
			}

			/* Update FA pointer*/
			if (!m_p_facTestToolInterface->SetFASector(1,0, sz_faData, sizeof(sz_faData)))
			{
				m_str_errorMessage = _T("Update FA Basic data to phone Fail.");
				LogMsg(m_str_errorMessage);
				AddMessage(m_str_errorMessage);
				return false;
			}
		}
	}

	return true;
}

bool CIMEIFCDlg::CheckImeiPicasso( CString str_imei )
{
	char sz_Picasso[11]={0};
	char sz_Timeinfo[12]={0};

	if (m_st_uiParameter.str_station != STATION_NAL_SCAN)
	{
		if (m_st_uiControll.i_FAArea == OLDFA)
		{
			m_obj_faOperator.GetPicassoNumber(sz_Picasso);
		}
		else
		{
			CStringToChar(m_obj_newFA.GetPicassoNumber(),sz_Picasso);
		}
	}
			
	char sz_imei[60]={0};
	char sz_modelname[50] = {0};
	CStringToChar(str_imei,sz_imei);
	CStringToChar(m_st_uiParameter.str_modelName,sz_modelname);
	if (!m_p_dlgParent->QueryIMEIresultFromCIM(sz_imei,sz_modelname,sz_Picasso))
	{
		m_str_errorCode = m_p_dlgParent->getErrCode();
		m_str_errorMessage = m_p_dlgParent->getErrMsg();
		return false;
	}

	return true;
}

void CIMEIFCDlg::LogMsg( CString logContent )
{
	m_obj_log.SaveLog(logContent);
}

bool CIMEIFCDlg::CheckInput()
{
	//校验IMEI
	GetDlgItem(IDC_EDIT_IMEI_ID)->GetWindowText(m_str_imei);

	if (m_str_imei == _T(""))
	{
		m_st_return.str_errorcode = CommErr_UI_IMEI_Rule_Check_Fail;
//		m_st_return.str_errorMsg = _T("未输入IMEI!");
		m_st_return.str_errorMsg = _T("未输入ID!");
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
		return false;
	}

	if(m_st_uiParameter.str_idtype == _T("IMEI") || m_st_uiParameter.str_idtype == _T("MEID"))
	{
		if (!Check_IMEI_FirstPart(m_str_imei,m_map_keyvalue[_T("IMEI_FIRST_PART")]) || (m_str_imei.GetLength() != IMEI_LENGTH))
		{
			m_st_return.str_errorcode = CommErr_UI_IMEI_First_Part_Fail;
			m_st_return.str_errorMsg = _T("IMEI/MEID 前8位不匹配,或长度不够 15 位!");
			m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
			GetDlgItem(IDC_EDIT_IMEI_ID)->SetWindowText(_T(""));
			return false;
		}

		if (!(Check_IMEI_Validity(m_str_imei) || Check_MEID_Validity(m_str_imei)))
		{
			m_st_return.str_errorcode = CommErr_UI_IMEI_Rule_Check_Fail;
			m_st_return.str_errorMsg = _T("IMEI/MEID 不符合编码规则!");
			m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
			GetDlgItem(IDC_EDIT_IMEI_ID)->SetWindowText(_T(""));
			return false;
		}
	}
	else// if(m_st_uiParameter.str_idtype == _T("ACERSN") || m_st_uiParameter.str_idtype == _T("BTWIFI") || m_st_uiParameter.str_idtype == _T("LYTROSN"))
	{
		if(m_str_imei.GetLength() != m_st_uiParameter.i_idLength)
		{
			m_st_return.str_errorcode = CommErr_UI_IMEI_First_Part_Fail;
			m_st_return.str_errorMsg = _T("ID 长度和系统中设定的不匹配");
			m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
			GetDlgItem(IDC_EDIT_IMEI_ID)->SetWindowText(_T(""));
			return false;
		}
	}
	//else
	//{
	//	m_st_return.str_errorcode = CommErr_UI_ID_Invalid;
	//	m_st_return.str_errorMsg = _T("系统中设置的ID 无效, 请于IE联系");
	//	m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
	//	GetDlgItem(IDC_EDIT_IMEI_ID)->SetWindowText(_T(""));
	//	return false;
	//}



	char sz_imei[60]={0};
	CStringToChar(m_str_imei,sz_imei);

#ifdef FACTORY_MODE
	
	if (!m_st_uiParameter.b_tsFlag)
	{
		if (!m_p_dlgParent->QueryHold(sz_imei))
		{
			m_st_return.str_errorcode = m_p_dlgParent->getErrCode();
			m_st_return.str_errorMsg = m_p_dlgParent->getErrMsg();
			m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
			return false;
		}
	}	
#endif

	if (m_st_uiControll.b_NAL)
	{   


		char sz_nal[50]={0};

		if ((m_st_uiParameter.str_station == STATION_IMEI) || (m_st_uiParameter.str_station == STATION_NAL_SCAN))
		{		
			GetDlgItem(IDC_EDIT_IMEI_NAL)->GetWindowText(m_str_NAL);

			if (m_str_NAL == _T(""))
			{
				m_st_return.str_errorcode = CommErr_UI_NAL_Not_Input;
				m_st_return.str_errorMsg = _T("Net access license not input!\r未输入入网许可证!");
				m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
				return false;
			}

			if (m_str_NAL.GetLength() != NAL_LENGTH)
			{
				m_st_return.str_errorcode = CommErr_UI_NAL_Rule_Check_Fail;
				m_st_return.str_errorMsg = _T("Check NAL rule fail!\r入网许可证(NAL)长度不符!");
				m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
				return false;
			}

			/*delete by Nancy*/
			/*if (m_st_uiControll.str_NAL6 != m_str_NAL.Left(6))
			{
				m_st_return.str_errorcode = CommErr_UI_NAL_Rule_Check_Fail;
				m_st_return.str_errorMsg = _T("Check NAL rule fail!\r入网许可证(NAL)前六位不符!");
				m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
				return false;
			}*/


			char sz_imei[60]={0};
			
			CStringToChar(m_str_imei,sz_imei);
			CStringToChar(m_str_NAL,sz_nal);

#ifdef FACTORY_MODE
			if (!m_st_uiParameter.b_tsFlag && !CheckNAL(sz_imei,sz_nal))
			{
				m_st_return.str_errorcode = m_str_errorCode;
				m_st_return.str_errorMsg = m_str_errorMessage;
				m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
				return false;
			}

			if (m_st_uiParameter.str_station == STATION_NAL_SCAN)
			{
				char sz_queryNal[50]={0};
				if (!m_st_uiParameter.b_tsFlag && !QueryNAL(sz_imei,sz_queryNal))
				{
					m_st_return.str_errorcode = m_str_errorCode;
					m_st_return.str_errorMsg = m_str_errorMessage;
					m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
					return false;
				}
				else
				{
					char sz_status[10] = {0};
					CString str_NAL;
					str_NAL = ConvertString(sz_queryNal);

					if (str_NAL.GetLength()==0)
					{
						if (!m_p_dlgParent->QueryIMEIStatusFromCIM(sz_imei,sz_status))
						{
							m_st_return.str_errorcode = m_str_errorCode;
							m_st_return.str_errorMsg = m_str_errorMessage;
							m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
							return false;
						}

						if (strcmp(sz_status,"I") != 0)
						{
							m_st_return.str_errorcode = CommErr_UI_CIM_IMEI_Status_Invalide;
							m_st_return.str_errorMsg = _T("Check IMEI status fail, it should be \'I\'!\rIMEI状态错误!");
							m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
							return false;
						}

					}
		
				}		
			}

		}
		else
		{
			if (!m_st_uiParameter.b_tsFlag && !QueryNAL(sz_imei,sz_nal))
			{
				m_st_return.str_errorcode = m_str_errorCode;
				m_st_return.str_errorMsg = m_str_errorMessage;
				m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
				return false;
			}
			else
			{
				m_str_NAL = ConvertString(sz_nal);
				GetDlgItem(IDC_EDIT_IMEI_NAL)->SetWindowText(m_str_NAL);
			}		
#endif

		}
        

		m_p_facTestToolInterface->SetParameterValue("NAL",sz_nal);

	}

    if(m_st_uiControll.b_ACER22)
	{   
        
		 GetDlgItem(IDC_EDIT_IMEI_Acer22)->GetWindowText(m_str_Acer22);

		if (m_str_Acer22 == _T(""))
		{
			m_st_return.str_errorcode = CommErr_UI_ACER22_Not_Input;
			m_st_return.str_errorMsg = _T("ACER22 not input!\r未输入ACER22码!");
			m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
			return false;
		}

		if (m_str_Acer22.GetLength() != Acer22_LENGTH)
		{
			m_st_return.str_errorcode = CommErr_UI_ACER22_Rule_Check_Fail;
			m_st_return.str_errorMsg = _T("Check ACER22 rule fail!\rACER22码长度不符!");
			m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&m_st_return);
			return false;
		}

        char sz_Acer22[50]={0};
       
		CStringToChar(m_str_Acer22,sz_Acer22);
        m_p_facTestToolInterface->SetParameterValue("ACER22",sz_Acer22);

	}

	return true;
}

bool CIMEIFCDlg::CheckNAL( char* sz_imei, char* sz_NAL )
{
	if (!m_p_dlgParent->CheckNal(sz_imei,sz_NAL))
	{
		m_str_errorCode = m_p_dlgParent->getErrCode();
		m_str_errorMessage = m_p_dlgParent->getErrMsg();
		return false;
	}

	return true;
}

bool CIMEIFCDlg::QueryNAL( char* sz_imei, char* sz_NAL )
{
	if (!m_p_dlgParent->QueryNAL(sz_imei,sz_NAL))
	{
		m_str_errorCode = m_p_dlgParent->getErrCode();
		m_str_errorMessage = m_p_dlgParent->getErrMsg();
		return false;
	}

	return true;
}

bool CIMEIFCDlg::QueryPicassoFromCIM( char* sz_imei, char *sz_model, char* sz_picasso )
{
	if (!m_p_dlgParent->QueryPicassoFromCIM(sz_imei,sz_model,sz_picasso))
	{
		m_str_errorCode = m_p_dlgParent->getErrCode();
		m_str_errorMessage = m_p_dlgParent->getErrMsg();
		return false;
	}

	return true;
}
bool CIMEIFCDlg::GetParameterFromDLL( void )
{
	/* Get Tool Version */
	char sz_toolversion[30]={0};
	if (!m_p_facTestToolInterface->GetParameterValue(TOOL_VERSION, sz_toolversion,30))
	{
		m_str_errorMessage = _T("从dll获取参数ToolVersion失败！");
		LogMsg(m_str_errorMessage);
		return false;
	}
	m_st_uiParameter.str_toolVer = ConvertString(sz_toolversion);

	LogMsg(_T("tool version from dll:") + m_st_uiParameter.str_toolVer);
	m_p_dlgParent->SetUIIDParameter(m_st_uiParameter);

	return true;
}

bool CIMEIFCDlg::PostRun()
{
	return m_p_facTestToolInterface ->PostRun();
}


