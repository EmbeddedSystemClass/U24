
#include "stdafx.h"
#include "UnifyUI_FacTestTool.h"
#include "UnifyUI_FacTestToolDlg.h"
#include "DownLoadDlg.h"

IMPLEMENT_DYNAMIC(CDownLoadDlg, CDialog)

CDownLoadDlg::CDownLoadDlg(CWnd* pParent)
	: CPageInterface(CDownLoadDlg::IDD)
{
	/* Init member */
	m_h_module = NULL;
	m_p_DLInterface = NULL;
	m_p_dlgParent = NULL;
	m_str_swFilePath = _T("");
	m_map_running.clear();
	m_map_picasso.clear();
	m_map_COMPort.clear();
	m_map_runResult.clear();

	m_b_sync = false;

	for (int i=0;i<10;i++)
	{
		m_b_returnRes[i] = true;
	}
	
	/* Get MainDlg pointer */
	m_p_dlgParent = (CUnifyUI_FacTestToolDlg*)pParent;
}

CDownLoadDlg::~CDownLoadDlg()
{
	/* Delete brush */
	m_brush.DeleteObject();


	/* Free DLL */
	if (m_h_module)
	{
		/* End */
		m_p_DLInterface->End();

		/* Unregister */
		m_p_DLInterface->Unregister(this, UI_PROGRESS);
		m_p_DLInterface->Unregister(this, UI_MESSAGE);
		m_p_DLInterface->Unregister(this, UI_RESULT);
		m_p_DLInterface->Unregister(this, UI_PICSDATA);
		m_p_DLInterface->Unregister(this, UI_SHOWDIALOG);
		m_p_DLInterface->Unregister(this, UI_RELAYBOARD);
		m_p_DLInterface->Unregister(this, UI_PICASSO);
		
		
		p_fn_deleteFacTestToolInterface p_delInterface = NULL;
		p_delInterface = (p_fn_deleteFacTestToolInterface)::GetProcAddress(m_h_module, (LPCSTR)DLL_DEL_INTERFACE);
		if(p_delInterface && m_p_DLInterface)
		{
			p_delInterface(m_p_DLInterface);
		}
	
		::FreeLibrary(m_h_module);
		m_h_module = NULL;
	}
}

void CDownLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	for(int i = 0; i < DEVICE_NUMBER; i++)
	{
		DDX_Control(pDX, IDC_DL_STATIC_COM1 + i, m_label_deviceCOM[i]);
		DDX_Control(pDX, IDC_DL_STATIC_PERCENT1 + i, m_label_progress[i]);
		DDX_Control(pDX, IDC_DL_PROGRESS_SUB1 + i, m_processCtrl_subProgress[i]);
		DDX_Control(pDX, IDC_DL_PROGRESS_MAIN1 + i, m_processCtrl_mainProgress[i]);
		DDX_Control(pDX, IDC_DL_EDIT_PICASSO1 + i, m_edit_picasso[i]);
	}
	DDX_Control(pDX, IDC_DL_LIST_SWPATH, m_listctrl_swPath);
	DDX_Control(pDX, IDC_DL_LIST_MESSAGE, m_listbox_message);
	DDX_Control(pDX, IDC_DL_STATIC_SWPATH, m_label_swPath);
}

BEGIN_MESSAGE_MAP(CDownLoadDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_DEVICECHANGE()
	//ON_MESSAGE(WM_USER_SETRESLUTNOTIFY, SetResult)
END_MESSAGE_MAP()


HBRUSH CDownLoadDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_DLG) 
	{
         return m_brush;
	}
	
	return hbr;
}

BOOL CDownLoadDlg::DestroyWindow()
{
	return __super::DestroyWindow();

}

/*****************************************************************************
* Function name: PreTranslateMessage      
* Summary      : Jump focus to the picasso CEdit(IDC_DL_EDIT_PICASSO 1 - 8).
* Parameters   : 
* Return       : 
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
BOOL CDownLoadDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN){
		switch (pMsg->wParam){
			case VK_RETURN:
				{
					UINT i_ctrlID = this->GetFocus()->GetDlgCtrlID();

					if ((IDC_DL_EDIT_PICASSO1 <= i_ctrlID) && (IDC_DL_EDIT_PICASSO8 >= i_ctrlID))
					{

						int i_tempID = i_ctrlID;

						for (int i = 1; i <= 8; i++)
						{
							if (IDC_DL_EDIT_PICASSO8 == i_tempID)
							{
								i_tempID = IDC_DL_EDIT_PICASSO1 - 1;
							}

							i_tempID++;

							if ((this->GetDlgItem(i_tempID)->IsWindowEnabled()))
							{
								this->GetDlgItem(i_tempID)->SetFocus();

								break;
							} 
							else
							{							
								continue;
							}														
						}

						/* Get current CEdit handle */
						CEdit* p_currentCEdit = (CEdit *)this->GetFocus();

						/* Select all characters */
						p_currentCEdit->SetSel(0, -1);	

					}
					pMsg->wParam = NULL;
					break;
				}

			case VK_ESCAPE:
				pMsg->wParam = NULL;
				break;

			case 'D':
				if ((GetKeyState(VK_CONTROL)&0x80)&&(GetKeyState(VK_SHIFT)&0x80))
				{
					if (m_p_DLInterface)
					{
						m_p_DLInterface->SetParameterValue("DEBUG","");
					}
					pMsg->wParam = NULL;
				}			
				break;
			default:
				break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg); 

}


/*****************************************************************************
* Function name: OnInitDialog      
* Summary      : Init Dialog.
* Parameters   : void.
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
BOOL CDownLoadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	/* Get MainDlg Parameter */
	if (m_p_dlgParent == NULL) 
	{
		SetMessage(_T("Get main UI dialog handle Fail."));
		ShowResultDlg(CommErr_UI_Init_Diag_Fail, _T("初始化失败！"));
		m_p_dlgParent->OnCancel();
		return FALSE;	
	}
	else
	{
		if (!m_p_dlgParent->GetUIIDParameter(m_st_uiParameter))
		{
			SetMessage(_T("GetUIIDParameter Fail."));
			ShowResultDlg(CommErr_UI_Init_Diag_Fail, _T("初始化失败！"));
			m_p_dlgParent->OnCancel();
			return FALSE;
		}
		if (!m_p_dlgParent->GetUIControlParameter(m_st_uiControl))
		{
			SetMessage(_T("GetUIControlParameter Fail."));
			ShowResultDlg(CommErr_UI_Init_Diag_Fail, _T("初始化失败！"));
			m_p_dlgParent->OnCancel();
			return FALSE;
		}
		if (!m_p_dlgParent->GetStationListMap(m_map_StationIndexMap))
		{
			SetMessage(_T("GetStationListMap Fail."));
			ShowResultDlg(CommErr_UI_Init_Diag_Fail, _T("初始化失败！"));
			m_p_dlgParent->OnCancel();
			return FALSE;
		}
		if (!m_p_dlgParent->GetTestKeyMap(m_map_keyvalue))
		{
			SetMessage(_T("GetTestKeyMap Fail."));
			ShowResultDlg(CommErr_UI_Init_Diag_Fail, _T("初始化失败！"));
			m_p_dlgParent->OnCancel();
			return FALSE;
		}
	}

	/* Init */
	if (!Init())
	{
		m_p_dlgParent->OnCancel();
		return FALSE;
	}

	/* Set focus */
	CString str_foucs;
	m_p_dlgParent->PostMessage(WM_USER_SETFOCUS, (WPARAM)_T(""), (LPARAM)&str_foucs);

	for(int i = 0; i < DEVICE_NUMBER; i++)
	{
		m_edit_picasso[i].SetLimitText(10);
		m_edit_picasso[i].SetWindowText(_T(""));
	}

	SetMessage(_T("UI init Succeed."));

	return TRUE;
}


/*****************************************************************************
* Function name: Init      
* Summary      : Init.
* Parameters   : void.
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDownLoadDlg::Init(void)
{
    if(m_st_uiParameter.str_station == STATION_OS_DL && m_st_uiControl.b_AutoRun)
	{   
		m_p_dlgParent->Begin();
		DeviceDetectSetup();//RegisterDeviceNotification
	}
	/* Load DLL */
	if (!LoadDLL())
	{
		SetMessage(_T("Load DLL Fail."));
		ShowResultDlg(CommErr_UI_DLL_Load_Fail, _T("初始化失败！\nDLL加载失败！"));
		return false;
	}

	m_p_DLInterface->DefineNotify(UI_PICASSO);
	

	/* Register */
	m_p_DLInterface->Register(this, UI_PROGRESS);
	m_p_DLInterface->Register(this, UI_MESSAGE);
	m_p_DLInterface->Register(this, UI_RESULT);
	m_p_DLInterface->Register(this, UI_PICSDATA);
	m_p_DLInterface->Register(this, UI_SHOWDIALOG);
	m_p_DLInterface->Register(this, UI_RELAYBOARD);
	m_p_DLInterface->Register(this, UI_PICASSO);
	
	if (!SetParameterToDLL())
	{
		return false;
	}

	/* DLL Begin */
	if (!DLLBegin())
	{
		SetMessage(_T("DLL Begin() Fail."));
		ShowResultDlg(CommErr_UI_Interface_Begin_Fail, _T("初始化失败！\n运行Begin失败！"));
		return false;
	}

 	if (!GetParameterFromDLL())
 	{
 		return false;
 	}

	/* Init UI */
	InitUIShow();

	/* UI Show COM Port */
	if (!UIShowCOMPort())
	{
		SetMessage(_T("UI Show COM Port Fail."));
		ShowResultDlg(CommErr_UI_Init_Diag_Fail, _T("初始化失败！\n配置文档COM Port未设置！"));
		return false;
	}

	/* UI Show image file name */
	if (!UIShowImageFile())
	{
		SetMessage(_T("UI Show Image file name Fail"));
		ShowResultDlg(CommErr_UI_Init_Diag_Fail, _T("初始化失败！\n配置文档Image未设置！"));
		return false;
	}

	return true;
}


/*****************************************************************************
* Function name: LoadDLL      
* Summary      : Load DL DLL.(DL.dll)
* Parameters   : 
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDownLoadDlg::LoadDLL(void)
{
	/* Load dll */
	CString str_dllName = DOWNLOAD_DLLNAME;
	m_h_module = ::LoadLibrary(str_dllName);
	if (m_h_module == NULL)
	{
		LogMsg(_T("Load ") + str_dllName + _T(" Fail! Use") + m_st_uiParameter.str_station + _T(".dll, and retry"));
		str_dllName = m_st_uiParameter.str_station + _T(".dll");
		m_h_module = LoadLibrary(str_dllName);

		if (m_h_module == NULL)
		{
			LogMsg(_T("Load ") + str_dllName + _T(" Fail! Use") + m_st_uiParameter.str_station + _T(".dll, and retry"));
			return false;
		}
	}

	/* Get function */
	p_fn_newFacTestToolInterface p_newInterface = NULL;
	p_newInterface = (p_fn_newFacTestToolInterface)::GetProcAddress(m_h_module, DLL_NEW_INTERFACE);
	if (p_newInterface == NULL)
	{
		return false;
	}

	if (!p_newInterface(&m_p_DLInterface))
	{
		return false;
	}

	if (m_p_DLInterface == NULL)
	{
		return false;
	}

	return true;
}

bool CDownLoadDlg::DLLBegin(void)
{
	/* Begin */
	if(!m_p_DLInterface->Begin())
	{
		return false;
	}

	return true;
}


/*****************************************************************************
* Function name: InitUIShow      
* Summary      : Init UI.
* Parameters   : void.
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
void CDownLoadDlg::InitUIShow(void)
{
	/* Define Dialog BK Color */
	COLORREF colour_dialogBKG;
	COLORREF colour_widgetText = RGB(0,0,0);

#ifdef RD_MODE
	colour_dialogBKG = BKGCOLOUR_RD;
#else
	if (m_st_uiParameter.b_tsFlag)
	{
		colour_dialogBKG = BKGCOLOUR_TS;
	}
	else
	{
		colour_dialogBKG = BKGCOLOUR_FACTORY;
	}
#endif

	m_brush.CreateSolidBrush(colour_dialogBKG);

	/* Image path static frame*/
	m_label_swPath.SetTextColor(colour_widgetText);
	m_label_swPath.SetBkColor(colour_dialogBKG);

	/* Image path */
	m_listctrl_swPath.InsertColumn(0, _T("ImageNum"), 0, 100);
	m_listctrl_swPath.InsertColumn(1, _T("FileName"), 0, 300);
	m_listctrl_swPath.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES | LVS_EDITLABELS | LVS_EX_INFOTIP);

	m_listctrl_swPath.DeleteAllItems();

	m_listctrl_swPath.SetBkColor(RGB(247,247,255));
    m_listctrl_swPath.SetTextBkColor(RGB(247,247,255));
	m_listctrl_swPath.SetTextColor(RGB(136,0,0));

	/* Progress Label */
	for(int i = 0; i < DEVICE_NUMBER; i++)
	{
		m_label_progress[i].SetBkColor(RGB(255,255,255));
		m_label_progress[i].SetFontBold(TRUE);
		m_label_progress[i].SetFontSize(14);
		m_label_progress[i].SetWindowText(_T("0%"));

		m_processCtrl_subProgress[i].SetRange(0,100);
		m_processCtrl_subProgress[i].SetPos(0);
		m_processCtrl_subProgress[i].SendMessage(PBM_SETBKCOLOR,0,RGB(255,255,255));
		m_processCtrl_subProgress[i].SendMessage(PBM_SETBARCOLOR,0,RGB(0,0,255)); 

		m_processCtrl_mainProgress[i].SetRange(0,100);
		m_processCtrl_mainProgress[i].SetPos(0);
		m_processCtrl_mainProgress[i].SendMessage(PBM_SETBKCOLOR,0,RGB(255,255,255));
		m_processCtrl_mainProgress[i].SendMessage(PBM_SETBARCOLOR,0,RGB(0,0,255)); 

		if ((m_st_uiParameter.str_station == STATION_TRIGGER)||(m_st_uiParameter.str_station == STATION_DL))
		{
//#ifndef RD_MODE
				m_edit_picasso[i].ShowWindow(TRUE);
				m_edit_picasso[i].SetWindowText(_T(""));
				m_edit_picasso[0].SetFocus();
// #else
 //				m_edit_picasso[i].ShowWindow(FALSE);
// #endif
		}
		else if ((m_st_uiParameter.str_station == STATION_OS_DL)||(m_st_uiParameter.str_station == STATION_REDL)) //2012.08.20 
		{
			m_edit_picasso[i].EnableWindow(FALSE);
		}
	}

	/* ListBox:UI Message */
	m_listbox_message.SetHorizontalExtent(10000);

	return;
}


/*****************************************************************************
* Function name: UIShowCOMPort      
* Summary      : Show COM port to UI. COM port from DLL.
* Parameters   : void.
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDownLoadDlg::UIShowCOMPort(void)
{
	/* Get COM Port */
	char sz_comPort[MAX_BUFFER_SIZE] = {0};
	if (m_p_DLInterface->GetParameterValue(PARAMETER_ONE, sz_comPort, sizeof(sz_comPort)) != true)
	{
		return false;
	}

	/* Convert char* to CString */
	CString str_temp;
	str_temp = ConvertString(sz_comPort);

	/* Spit COM Port */
	CStringArray stra_comPortList;
	if (SpiltString(str_temp, _T(","), stra_comPortList) != true)
	{
		return false;
	}
	if (stra_comPortList.GetSize() != DEVICE_NUMBER)
	{
		return false;
	}

	/* Show COM Port label */
	int i_count = 0;
	for(int i = 0; i < DEVICE_NUMBER; i++)
	{
		m_map_COMPort[i] = stra_comPortList.GetAt(i);

		/* COM Port default value is 0 */
		if (m_map_COMPort[i] != _T("0"))
		{
			m_label_deviceCOM[i].SetBkColor(RGB(202,255,202));
			m_label_deviceCOM[i].SetFontBold(TRUE);
			m_label_deviceCOM[i].SetFontSize(30);
			m_label_deviceCOM[i].SetWindowText(stra_comPortList.GetAt(i));

			i_count ++;
		}
		else
		{
			m_label_deviceCOM[i].SetBkColor(RGB(221, 202, 162));
			m_label_progress[i].SetWindowText(_T(""));
			m_edit_picasso[i].SetWindowText(_T(""));
			m_edit_picasso[i].EnableWindow(FALSE);
		}
	}

	/* COMPort all is 0 */
	if (i_count == 0)
	{
		return false;
	}

	return true;
}


/*****************************************************************************
* Function name: UIShowImageFile      
* Summary      : Show image file name to UI. Image file name from DLL.
* Parameters   : void.
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDownLoadDlg::UIShowImageFile(void)
{
	/* Image file path */
	m_listctrl_swPath.InsertItem(LVIF_TEXT | LVIF_STATE, 0, _T("SW PATH"), LVIS_SELECTED, LVIS_SELECTED, 0, 0);
	m_listctrl_swPath.SetItemText(0, 1, m_str_swFilePath);

	/* Get image count */
	int i_count = 0;
	i_count = m_p_DLInterface->GetItemCount();
	if (i_count == 0)
	{
		return false;
	}

	/* Get image name by count */
	char sz_fileName[MAX_BUFFER_SIZE] = { 0 };
	for (int i = 0; i < i_count; i++)
	{
		memset(sz_fileName, 0, sizeof(sz_fileName));
		if (m_p_DLInterface->GetItemByIndex(i, sz_fileName, sizeof(sz_fileName)) != true)
		{
			return false;
		}
			
		/* Convert char* to CString */
		CString str_temp;
		str_temp = ConvertString(sz_fileName);

		/* Insert to listctrl */
		CString str_index;
		str_index.Format(_T("%02d"), i+1);
		m_listctrl_swPath.InsertItem(LVIF_TEXT | LVIF_STATE, i+1, str_index, LVIS_SELECTED, LVIS_SELECTED, 0, 0);
		m_listctrl_swPath.SetItemText(i+1, 1, str_temp);
	}
	
	return true;
}


/*****************************************************************************
* Function name: SetMessage      
* Summary      : Insert message to listbox.
* Parameters   : CString str_info : message
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
void CDownLoadDlg::SetMessage(CString str_info)
{
	SYSTEMTIME st_time;
	GetLocalTime(&st_time);

	CString str_time;
	str_time.Format(_T("[%04d/%02d/%02d %02d:%02d:%02d]: "), st_time.wYear, st_time.wMonth, \
		st_time.wDay, st_time.wHour, st_time.wMinute, st_time.wSecond);

	/* Insert to Listbox */
	CSingleLock singleLock(&m_obj_messageCritSection, TRUE);
	int i_count = m_listbox_message.GetCount();
	m_listbox_message.InsertString(i_count, str_time + str_info);
	m_listbox_message.SetCurSel(i_count);

	return;
}


/*****************************************************************************
* Function name: SetProgress      
* Summary      : Update UI progress.
* Parameters   : i_slot[1,8]
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
void CDownLoadDlg::SetProgress(int i_slot, int i_mainProgress, int i_subProgress)
{
	/* Input Check */
	if ((i_slot < 0) || (i_slot >= DEVICE_NUMBER) || (i_mainProgress < 0) || (i_mainProgress > 100) || (i_subProgress < 0) || (i_subProgress > 100) )
	{
		return;
	}

	/* Update Sub progress */
	m_processCtrl_subProgress[i_slot].SetPos(i_subProgress);

	/* Update Main progress */
	if (i_subProgress == 100)
	{
		m_processCtrl_mainProgress[i_slot].SetPos(i_mainProgress);
		CString str_value;
		str_value.Format(_T("%d%%"), i_mainProgress);
		m_label_progress[i_slot].SetBkColor(RGB(255,255,255));
		m_label_progress[i_slot].SetWindowText(str_value);
	}

	return;
}


/*****************************************************************************
* Function name: SetResult      
* Summary      : Set UI Result.
* Parameters   : 
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDownLoadDlg::SetResult(int i_slot, CString str_result)
{
	CSingleLock singleLock(&m_obj_resultCritSection, TRUE);

	//st_pageReturn* p_st_result = (st_pageReturn*)(lParam);
	//int i_slot = p_st_result->i_slot;
	//CString str_result = p_st_result->str_errorcode;
	
	m_b_sync = false;

	CString strTemp;

	if ((i_slot < 0) || (i_slot >= DEVICE_NUMBER) || (str_result == _T("")))
	{
		strTemp.Format(_T("[%d] got a error code is null"),i_slot);
		LogMsg(strTemp);
		return 0;
	}
	
	/* Check Result */
	if (str_result.CompareNoCase(_T("PASS")) == 0)
	{
		/* PASS --- End */
		//m_label_progress[i_slot].SetWindowText(str_result);
		//m_label_progress[i_slot].SetBkColor(RGB(0,255,0));

		/* Save result */
		m_map_runResult[i_slot] = str_result;
	}
	else
	{	
		if (str_result.CompareNoCase(_T("FAIL")) != 0)
		{
			/* ErrorCode --- Save and Show UI */
			/*if (m_p_dlgParent->m_b_NewErrCode)
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
			m_label_progress[i_slot].SetBkColor(RGB(255,0,0));*/
			
			
			/* Save result */
			m_map_runResult[i_slot] = str_result;

			return 0;
		}
		else
		{
			/* FAIL --- End */

			if(!(m_st_uiParameter.str_station == STATION_OS_DL && m_st_uiControl.b_AutoRun))
			{
				if (m_p_dlgParent->m_b_NewErrCode)
				{
					strTemp.Format(_T("[%d] Non-Auto Run fail, set error code to UI"),i_slot);
					LogMsg(strTemp);

					if ((m_map_runResult[i_slot].Left(2) != _T("DB")) && (m_map_runResult[i_slot].Left(2) != _T("UI")))
					{
						char sz_errorCode[200]={0};
						char sz_genErrcode[200]={0};
						char sz_genErrMsg[MAX_BUFFER_SIZE]={0};
						char sz_errmsg[MAX_BUFFER_SIZE]={0};
						CStringToChar(m_map_runResult[i_slot],sz_errorCode);
						bool bCim = false;
						if (!m_p_dlgParent->m_obj_QueryErrCode.QueryErrCode(sz_errorCode, sz_genErrcode, sz_genErrMsg, sz_errmsg,bCim))
						{
							strTemp.Format(_T("Disp1:Error Not Found:%s"),m_map_runResult[i_slot]);
							LogMsg(strTemp);
						}
						strTemp = ConvertString(sz_genErrcode);
					}
					else
					{
						strTemp = m_map_runResult[i_slot];
					}
					m_label_progress[i_slot].SetWindowText(strTemp);
				}
				else
				{
					m_label_progress[i_slot].SetWindowText(m_map_runResult[i_slot]);
				}
				m_label_progress[i_slot].SetBkColor(RGB(255,0,0));
			}			
		}
	}

	/* Check handset if is running */
	if (m_map_running[i_slot])
	{
		::WaitForSingleObject(m_map_running[i_slot]->GetThreadHandle(), INFINITE);
		//::WaitForSingleObject(m_map_running[i_slot]->GetThreadHandle()->m_hThread, 10000);
		delete m_map_running[i_slot];
		m_map_running[i_slot] = NULL;
	}
	m_map_running.erase(i_slot);

	/* Check if all stop */
	if((m_st_uiParameter.str_station == STATION_OS_DL && m_st_uiControl.b_AutoRun)||m_map_running.size() == 0)
	{
		/* Reset UI */
		if ((m_st_uiParameter.str_station == STATION_TRIGGER)||(m_st_uiParameter.str_station == STATION_DL))
		{
			int i_first = -1;
			for(int i = 0; i < DEVICE_NUMBER; i++)
			{
				if (m_map_COMPort[i] != _T("0"))
				{
					m_edit_picasso[i].SetWindowText(_T(""));
					m_edit_picasso[i].EnableWindow(TRUE);
					if (i_first == -1)
					{
						i_first=i;
					}
				}
			}
			m_edit_picasso[i_first].SetFocus();
		}
		
        if(m_st_uiParameter.str_station == STATION_OS_DL && m_st_uiControl.b_AutoRun)
		{   
			st_pageReturn st_return;
			st_return.i_slot = i_slot;
			st_return.str_errorcode = m_map_runResult[i_slot];
			m_map_runResult.erase(i_slot);

			NotifyMainUI(st_return);

			m_b_returnRes[i_slot] = true;
			strTemp.Format(_T("m_b_returnRes[%d] is true"),i_slot);
			LogMsg(strTemp);
		}
		else
		{
			/* Notify MainUI */
			for(int i = 0; i < DEVICE_NUMBER; i++)
			{
				if (m_map_COMPort[i] != _T("0"))
				{
					st_pageReturn st_return;
					st_return.i_slot = i;
					st_return.str_errorcode = m_map_runResult[i];
					NotifyMainUI(st_return);
				}
			}
		}

		/* Clear map */
		if (!m_st_uiControl.b_AutoRun)
		{
			m_map_runResult.clear();
			m_map_picasso.clear();
		}
	}

	return 0;
}


/*****************************************************************************
* Function name: ShowResultDlg      
* Summary      : Result dialog.
* Parameters   : 
* Return       : bool
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDownLoadDlg::ShowResultDlg(CString str_errorCode, CString str_message)
{
	m_dlg_result.SetText(str_errorCode, str_message);
	m_dlg_result.SetTextSize(30);
	m_dlg_result.DoModal();

	LogMsg(str_errorCode + str_message);

	return true;
}

/*****************************************************************************
* Function name: NotifyMainUI      
* Summary      : Notify message to main UI.
* Parameters   : 
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
void CDownLoadDlg::NotifyMainUI(st_pageReturn st_return)
{
	if (m_p_dlgParent != NULL)
	{
		m_p_dlgParent->SendMessage(WM_USER_SETRESULT, (WPARAM)_T(""), (LPARAM)&st_return);
	}

	return;
}


/*****************************************************************************
* Function name: Event      
* Summary      : Event deal with.
* Parameters   : 
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
void CDownLoadDlg::Event(const char* str_event, long nParam)
{
	/* UI_PROGRESS */
	if (strcmp(str_event, UI_PROGRESS) == 0)
	{
		CSingleLock singleLock(&m_obj_UIProcess, TRUE);
		st_UIProgress* p_st_progress = (st_UIProgress*)nParam;
		SetProgress(p_st_progress->i_slot, p_st_progress->i_mainProgress, p_st_progress->i_subProgress);
	}
	/* UI_MESSAGE */
	else if (strcmp(str_event, UI_MESSAGE) == 0)
	{
		CSingleLock singleLock(&m_obj_UIMessage, TRUE);
		st_UIMessage* p_st_message = (st_UIMessage*)nParam;
		CString str_message;
		CString str_temp;
		str_temp = ConvertString(p_st_message->sz_message);
		str_message.Format(_T("[%d] %s"), p_st_message->i_slot, str_temp);
		LogMsg(str_message);
		SetMessage(str_message);
	}
	/* UI_RESULT */
	else if (strcmp(str_event, UI_RESULT) == 0)
	{		
		CSingleLock singleLock(&m_obj_UIResult, TRUE);

		st_UIResult* p_st_message = (st_UIResult*)nParam;
		CString str_result;
		str_result = ConvertString(p_st_message->sz_errCode);

		while (m_b_sync != false)
		{
			Sleep(20);
			continue;
		}
		m_b_sync = true;

		m_st_return.i_slot = p_st_message->i_slot;
		m_st_return.str_errorcode = str_result;
	//	m_b_returnRes[m_st_return.i_slot] = true;

		CString str_message;
		str_message.Format(_T("[%d] Received ErrorCode:%s"), p_st_message->i_slot, str_result);
		LogMsg(str_message);
		SetMessage(str_message);
		//::PostMessage(this->m_hWnd, WM_USER_SETRESLUTNOTIFY, (WPARAM)_T(""), (LPARAM)&m_st_return);
		SetResult(p_st_message->i_slot,str_result);
	}
	/* PICS Data */
	else if (strcmp(str_event, UI_PICSDATA) == 0)
	{
		CSingleLock singleLock(&m_obj_UIPicsdata, TRUE);
		st_PICSData* p_st_picsData = (st_PICSData*)nParam;
		m_p_dlgParent->SetPICSData(p_st_picsData->i_slot, p_st_picsData->sz_key, p_st_picsData->sz_value);
	}
	/*Relay board control*/
	else if (strcmp(str_event, UI_RELAYBOARD) == 0)
	{
		CSingleLock singleLock(&m_obj_UIRelayBoard, TRUE);
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
	
		m_label_progress[i_slot].SetWindowText(_T("10%"));
		m_edit_picasso[i_slot].SetWindowText(str_picasso);

		m_p_dlgParent->m_map_picasso[i_slot] = str_picasso;

		CString str_message;
		str_message.Format(_T("[%d] Get Picasso:%s"), p_st_message->i_slot, str_picasso);
		LogMsg(str_message);
		SetMessage(str_message);
	}
	/*Power supply
	else if (strcmp(str_event, UI_POWERSUPPLY) == 0)
	{
		CSingleLock singleLock(&m_obj_UIResult, TRUE);
		st_PowerSupply* p_st_powerSupply = (st_PowerSupply*)nParam;
		m_p_dlgParent->SetPowerSupply(p_st_powerSupply->i_slot,p_st_powerSupply->i_Seq,p_st_powerSupply->dw_Voltage,p_st_powerSupply->dw_current);
	}*/
	
	return;
}


/*****************************************************************************
* Function name: Run      
* Summary      : Main UI call.
* Parameters   : 
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDownLoadDlg::Run(void)
{
	/* Run */
	for(int i = 0; i < DEVICE_NUMBER; i++)
	{
		if (m_map_COMPort[i] != _T("0"))
		{
			CString str_picasso;
			m_edit_picasso[i].GetWindowText(str_picasso);

#ifndef RD_MODE		

			st_UIResult st_result;
			if (str_picasso == _T(""))
			{
				st_result.i_slot = i;
				strcpy_s(st_result.sz_errCode, "FAIL");
				m_p_DLInterface->Fire(UI_RESULT, (long)&st_result);
				continue;
			}
			
			if (!m_st_uiParameter.b_tsFlag)
			{
				/* Check Picasso rule and Check Routing Result */
				if (m_map_picasso.find(i) != m_map_picasso.end())
				{
					/* Get error code from invalid picasso list */
					LogMsg(m_map_picasso[i]);
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
					st_result.i_slot = i;
					CStringToChar(str_errorCode, st_result.sz_errCode); 
					m_p_DLInterface->Fire(UI_RESULT, (long)&st_result);

					st_result.i_slot = i;
					strcpy_s(st_result.sz_errCode, "FAIL");
					m_p_DLInterface->Fire(UI_RESULT, (long)&st_result);
					continue;
				}
			}
			
#endif

			/* Run */
			m_map_running[i] = new CHandsetDL(&m_st_uiParameter, (i), str_picasso, m_p_DLInterface,m_map_StationIndexMap,&m_st_uiControl);
			if (m_map_running[i] == NULL)
			{
				/* Show to UI */
				st_UIResult st_result;
				st_result.i_slot = i;
				CStringToChar(CommErr_UI_Run_Thread_Fail, st_result.sz_errCode); 
				m_p_DLInterface->Fire(UI_RESULT, (long)&st_result);

				st_result.i_slot = i;
				strcpy_s(st_result.sz_errCode, "FAIL");
				m_p_DLInterface->Fire(UI_RESULT, (long)&st_result);
				return false;
			}

			if (!m_map_running[i]->HandsetRun())
			{
				/* Show to UI */
				st_UIResult st_result;
				st_result.i_slot = i;
				CStringToChar(CommErr_UI_Run_Thread_Fail, st_result.sz_errCode); 
				m_p_DLInterface->Fire(UI_RESULT, (long)&st_result);

				st_result.i_slot = i;
				strcpy_s(st_result.sz_errCode, "FAIL");
				m_p_DLInterface->Fire(UI_RESULT, (long)&st_result);
				return false;
			}
			Sleep(500);
		}
	}

	return true;
}


/*****************************************************************************
* Function name: Stop      
* Summary      : Main UI call.
* Parameters   : 
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDownLoadDlg::Stop(void)
{
	if (m_p_DLInterface)
	{
		for (int i = 0; i < DEVICE_NUMBER; i++)
		{
			if (m_p_DLInterface->Stop(i) != true)
			{
				return false;
			}
		}
	}

	return true;
}


/*****************************************************************************
* Function name: GetPicasso      
* Summary      : Main UI call.---- Check routing by CIM
* Parameters   : 
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDownLoadDlg::GetPicasso(std::map<int, CString>& map_picasso)
{	
	/* Reset UI */
	m_listbox_message.ResetContent();

	map_picasso.clear();

	for(int i = 0; i < DEVICE_NUMBER; i++)
	{
		if (m_map_COMPort[i] != _T("0"))
		{
			m_label_progress[i].SetBkColor(RGB(255,255,255));
			m_label_progress[i].SetWindowText(_T("0%"));

			m_processCtrl_subProgress[i].SetPos(0);
			m_processCtrl_mainProgress[i].SetPos(0);

			m_edit_picasso[i].EnableWindow(FALSE);
			m_map_running[i] = NULL;		
		}
	}

	if ((m_st_uiParameter.str_station == STATION_TRIGGER)||(m_st_uiParameter.str_station == STATION_DL))
	{
		for(int i = 0; i < DEVICE_NUMBER; i++)
		{
			if (m_map_COMPort[i] != _T("0"))
			{
				CString str_picasso;
				m_edit_picasso[i].GetWindowText(str_picasso);
				map_picasso[i] = str_picasso;
			
			}
		}
	}
	/* OSDL */
	else 
	{

		for (int i = 0; i < DEVICE_NUMBER; i++)
		{
			if (m_map_COMPort[i] != _T("0"))
			{
				m_edit_picasso[i].SetWindowText(_T(""));
			}
		}

#ifndef FACTORY_MODE
		if (!m_st_uiControl.b_ReadFA)
		{
			LogMsg(_T("RD mode, b_ReadFA is false, skip to get picasso"));
			return true;
		}
#endif

		char sz_readFAData[512]={0};
		char sz_picasso[11] = {0};
		CString str_tmp = _T("");
		for(int i = 0; i < DEVICE_NUMBER; i++)
		{
			if (m_map_COMPort[i] != _T("0"))
			{
				memset(sz_readFAData,0,sizeof(sz_readFAData));
				if (m_st_uiControl.i_FAArea == OLDFA)
				{
					CFAOperator obj_FA;
					if (!m_p_DLInterface->GetFAData(i, sz_readFAData, sizeof(sz_readFAData)))
					{
						st_UIResult st_result;
						st_result.i_slot = i;
						CStringToChar(_T("GetFAData Fail"), st_result.sz_msg);
						CStringToChar(CommErr_UI_Interface_GetFA_Fail, st_result.sz_errCode);
						m_p_DLInterface->Fire(UI_RESULT, (long)&st_result);
					}
					else
					{
						obj_FA.SetFA(sz_readFAData);
						obj_FA.GetPicassoNumber(sz_picasso);
						map_picasso[i] = ConvertString(sz_picasso);
						m_edit_picasso[i].SetWindowText(map_picasso[i]);

						st_UIMessage st_message;
						st_message.i_slot = i;

						sprintf(st_message.sz_message, "GetFAData Success, picasso = %s",sz_picasso);
						m_p_DLInterface->Fire(UI_MESSAGE, (long)&st_message);

					}
				}
				else if (m_st_uiControl.i_FAArea == NEWFA)
				{
					CFAOperator_new obj_newFA;
					if (!m_p_DLInterface->GetFASector(i, 0, sz_readFAData, sizeof(sz_readFAData)))
					{
						st_UIResult st_result;
						st_result.i_slot = i;
						CStringToChar(_T("GetFASector Fail"), st_result.sz_msg);
						CStringToChar(CommErr_UI_Interface_GetFA_Fail, st_result.sz_errCode);
						m_p_DLInterface->Fire(UI_RESULT, (long)&st_result);
					}
					else
					{
						obj_newFA.SetSectorData(sz_readFAData);
						map_picasso[i] = obj_newFA.GetPicassoNumber();
						m_edit_picasso[i].SetWindowText(map_picasso[i]);

						CStringToChar(map_picasso[i],sz_picasso);
						st_UIMessage st_message;
						st_message.i_slot = i;
						sprintf(st_message.sz_message, "GetFAData Success, picasso = %s",sz_picasso);		
						m_p_DLInterface->Fire(UI_MESSAGE, (long)&st_message);
					}	
				}
			}
		}

	}

	return true;
}


bool CDownLoadDlg::GetPicassoForOS_DL_AutoRun(std::map<int, CString>& map_picasso,int i_slot)
{
	m_edit_picasso[i_slot].SetWindowText(_T(""));
#ifndef FACTORY_MODE
	if (!m_st_uiControl.b_ReadFA)
	{
		return true;
	}
#endif
	char sz_readFAData[512]={0};
	char sz_picasso[11] = {0};
	CString str_tmp = _T("");
	if (m_map_COMPort[i_slot] != _T("0"))
	{
		memset(sz_readFAData,0,sizeof(sz_readFAData));
		if (m_st_uiControl.i_FAArea == OLDFA)
		{
			CFAOperator obj_FA;
			if (!m_p_DLInterface->GetFAData(i_slot, sz_readFAData, sizeof(sz_readFAData)))
			{
				st_UIResult st_result;
				st_result.i_slot = i_slot;
				CStringToChar(_T("GetFAData Fail"), st_result.sz_msg);
				CStringToChar(CommErr_UI_Interface_GetFA_Fail, st_result.sz_errCode);
				m_p_DLInterface->Fire(UI_RESULT, (long)&st_result);

				st_result.i_slot = i_slot;
				strcpy_s(st_result.sz_errCode, "FAIL");
				m_p_DLInterface->Fire(UI_RESULT, (long)&st_result);
				return false;
			}
			else
			{
				obj_FA.SetFA(sz_readFAData);
				obj_FA.GetPicassoNumber(sz_picasso);
				map_picasso[i_slot] = ConvertString(sz_picasso);
				m_edit_picasso[i_slot].SetWindowText(map_picasso[i_slot]);

				st_UIMessage st_message;
				st_message.i_slot = i_slot;

				sprintf(st_message.sz_message, "GetFAData Success, picasso = %s",sz_picasso);
				m_p_DLInterface->Fire(UI_MESSAGE, (long)&st_message);
			}
		}
		else if (m_st_uiControl.i_FAArea == NEWFA)
		{
			CFAOperator_new obj_newFA;
			if (!m_p_DLInterface->GetFASector(i_slot, 0, sz_readFAData, sizeof(sz_readFAData)))
			{
				st_UIResult st_result;
				st_result.i_slot = i_slot;
				CStringToChar(_T("GetFASector Fail"), st_result.sz_msg);
				CStringToChar(CommErr_UI_Interface_GetFA_Fail, st_result.sz_errCode);
				m_p_DLInterface->Fire(UI_RESULT, (long)&st_result);

				st_result.i_slot = i_slot;
				strcpy_s(st_result.sz_errCode, "FAIL");
				m_p_DLInterface->Fire(UI_RESULT, (long)&st_result);
				return false;
			}
			else
			{
				obj_newFA.SetSectorData(sz_readFAData);
				map_picasso[i_slot] = obj_newFA.GetPicassoNumber();
				m_edit_picasso[i_slot].SetWindowText(map_picasso[i_slot]);

				CStringToChar(map_picasso[i_slot],sz_picasso);
				st_UIMessage st_message;
				st_message.i_slot = i_slot;
				sprintf(st_message.sz_message, "GetFAData Success, picasso = %s",sz_picasso);		
				m_p_DLInterface->Fire(UI_MESSAGE, (long)&st_message);
			}	
		}
	}

	return true;

}
/*****************************************************************************
* Function name: SetPicasso      
* Summary      : Main UI call. --- Return the not passed picasso.
* Parameters   : m_map_picasso: The fail record.
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDownLoadDlg::SetPicasso(std::map<int, CString> map_picasso)
{
	m_map_picasso.clear();
	m_map_picasso = map_picasso;

	return true;
}

bool CDownLoadDlg::ResetPage(void)
{
	for(int i = 0; i < DEVICE_NUMBER; i++)
	{
		if (m_map_COMPort[i] != _T("0"))
		{
			m_edit_picasso[i].SetFocus();
			break;
		}
	}

	return true;
}

void CDownLoadDlg::LogMsg( CString logContent )
{
	if (logContent.Find(_T("%")) == -1)
	{
	    m_obj_log.SaveLog(logContent);
	}
}

void CDownLoadDlg::SetTestResult( int i_slot, CString str_testresult )
{
	m_label_progress[i_slot].SetWindowText(str_testresult);

	if (str_testresult == _T("PASS"))
	{		
		m_label_progress[i_slot].SetBkColor(RGB(0,255,0));
	}
	else
	{
		m_label_progress[i_slot].SetBkColor(RGB(255,0,0));
	}
}

bool CDownLoadDlg::SetParameterToDLL( void )
{
	CString strTemp;
	USES_CONVERSION;


	if (!m_p_DLInterface->SetParameterValue(LOGPATH, T2A(LOG_PATH)))
	{
		SetMessage(_T("Set log path to DLL Fail."));
		ShowResultDlg(CommErr_UI_Interface_Setparameter_Fail, _T("初始化失败！\r\n设置log路径失败！"));
		return false;
	}

#ifdef RD_MODE
	if (!m_p_DLInterface->SetParameterValue(TOOLMODE, "RD"))
	{
		SetMessage(_T("Set Tool mode to DLL Fail."));
		ShowResultDlg(CommErr_UI_Interface_Setparameter_Fail, _T("初始化失败！\r\n设置Tool mode失败！"));
		return false;
	}
#else
	if (!m_p_DLInterface->SetParameterValue(TOOLMODE, "FACTORY"))
	{
		SetMessage(_T("Set Tool mode to DLL Fail."));
		ShowResultDlg(CommErr_UI_Interface_Setparameter_Fail, _T("初始化失败！\r\n设置Tool mode失败！"));
		return false;
	}
#endif
	
	
	/* Set Factory Version */
	strTemp.Format(_T("FACTORY_VERSION---%s"),m_st_uiParameter.str_factoryVer);
	LogMsg(strTemp);
	if (!m_p_DLInterface->SetParameterValue(FACTORY_VERSION, T2A(m_st_uiParameter.str_factoryVer)))
	{
		SetMessage(_T("Set Factory Version to DLL Fail."));
		ShowResultDlg(CommErr_UI_Interface_Setparameter_Fail, _T("初始化失败！\r\n设置Factory Version失败！"));
		return false;
	}

	CString str_imagePath;
	

	/* Set DL mode */
	if ((m_st_uiParameter.str_station == STATION_OS_DL)||(m_st_uiParameter.str_station == STATION_REDL)) //2012.08.20 
	{
		if (!m_p_DLInterface->SetParameterValue(DLMODE, "ReDL"))
		{
			SetMessage(_T("Set DL mode to DLL Fail."));
			ShowResultDlg(CommErr_UI_Interface_Setparameter_Fail, _T("初始化失败！\r\n设置DL mode失败！"));
			return false;
		}
		str_imagePath = m_st_uiParameter.str_userLoadVer;
	}
	else
	{
		if (!m_p_DLInterface->SetParameterValue(DLMODE, "PreDL"))
		{
			SetMessage(_T("Set DL mode to DLL Fail."));
			ShowResultDlg(CommErr_UI_Interface_Setparameter_Fail, _T("初始化失败！\r\n设置DL mode失败！"));
			return false;
		}
		str_imagePath = m_st_uiParameter.str_factoryVer;
	}
	
	/* Set SW image path */
	if (m_st_uiParameter.str_imagePath.Find(_T("\\")) == -1)
	{
		m_str_swFilePath = m_str_swFilePath + DOWNLOAD_IMAG_PATH + _T("\\") + m_st_uiParameter.str_modelName + _T("\\") + str_imagePath;
	}
	else
	{
		m_str_swFilePath = m_st_uiParameter.str_imagePath;
	}

	if (_taccess(m_str_swFilePath, 0) != 0) 
	{
		SetMessage(m_str_swFilePath +_T("Image path is not exsit."));
		ShowResultDlg(CommErr_UI_DL_Image_Not_Exist, _T("初始化失败！\r\nImage文件路径不存在！"));
		return false;
	}


	strTemp.Format(_T("IMAGEPATH---%s"),m_str_swFilePath);
	LogMsg(strTemp);
	if (!m_p_DLInterface->SetParameterValue(IMAGEPATH, T2A(m_str_swFilePath)))
	{
		SetMessage(_T("Set image path to DLL Fail."));
		ShowResultDlg(CommErr_UI_Interface_Setparameter_Fail, _T("初始化失败！\r\n设置Image文件路径失败！"));
		return false;
	}


	/* Set SW Version */
	strTemp.Format(_T("SW_VERSION---%s"),m_st_uiParameter.str_userLoadVer);
	LogMsg(strTemp);
	if (!m_p_DLInterface->SetParameterValue(SW_VERSION, T2A(m_st_uiParameter.str_userLoadVer)))
	{
		SetMessage(_T("Set SWVersion to DLL Fail."));
		ShowResultDlg(CommErr_UI_Interface_Setparameter_Fail, _T("初始化失败！\r\n设置SWVersion失败！"));
		return false;
	}

	/* Set HW Version */
	/*if (!m_p_DLInterface->SetParameterValue(HW_VERSION, T2A(m_st_uiParameter.str_pcba)))
	{
		SetMessage(_T("Set HWVersion to DLL Fail."));
		ShowResultDlg(CommErr_UI_Interface_Setparameter_Fail, _T("初始化失败！\r\n设置HWVersion失败！"));
		return false;
	}*/

	/* Set Tool Version */
	/*if (!m_p_DLInterface->SetParameterValue(TOOL_VERSION, T2A(m_st_uiParameter.str_toolVer)))
	{
		SetMessage(_T("Set Tool Version to DLL Fail."));
		ShowResultDlg(CommErr_UI_Interface_Setparameter_Fail, _T("初始化失败！\r\n设置ToolVersion失败！"));
		return false;
	}*/

	strTemp.Format(_T("MODEL_NAME---%s"),m_st_uiParameter.str_modelName);
	LogMsg(strTemp);
	if (!m_p_DLInterface->SetParameterValue(MODEL_NAME, T2A(m_st_uiParameter.str_modelName)))
	{
		SetMessage(_T("Set model name to DLL Fail."));
		ShowResultDlg(CommErr_UI_Interface_Setparameter_Fail, _T("初始化失败！\r\n设置Model Name失败！"));
		return false;
	}

	strTemp.Format(_T("RESERVE1---%s"),m_st_uiParameter.str_Reserve1);
	LogMsg(strTemp);
	if (!m_p_DLInterface->SetParameterValue(RESERVE1, T2A(m_st_uiParameter.str_Reserve1)))
	{
		SetMessage(_T("Set RESERVE1 Value to DLL Fail."));
		ShowResultDlg(CommErr_UI_Interface_Setparameter_Fail, _T("初始化失败！\r\n设置RESERVE1 Value失败！"));
		return false;
	}

	strTemp.Format(_T("RESERVE2---%s"),m_st_uiParameter.str_Reserve2);
	LogMsg(strTemp);
	if (!m_p_DLInterface->SetParameterValue(RESERVE2, T2A(m_st_uiParameter.str_Reserve2)))
	{
		SetMessage(_T("Set RESERVE2 Value to DLL Fail."));
		ShowResultDlg(CommErr_UI_Interface_Setparameter_Fail, _T("初始化失败！\r\n设置RESERVE2 Value失败！"));
		return false;
	}

	std::map<CString,CString>::iterator Itr;
	for (Itr = m_map_keyvalue.begin();Itr!= m_map_keyvalue.end();Itr++)
	{
		if (!m_p_DLInterface->SetParameterValue(T2A(Itr->first),T2A(Itr->second)))
		{
			SetMessage(_T("Set parameter to DLL Fail."));
			CString str_errormsg;
			str_errormsg.Format(_T("(%s,%s) is invalide!"),Itr->first,Itr->second);
			ShowResultDlg(CommErr_UI_Interface_Setparameter_Fail, str_errormsg);
			return false;
		}
		LogMsg(Itr->first + _T("--") + Itr->second);
	}

	return true;
}

bool CDownLoadDlg::GetParameterFromDLL( void )
{
	/* Get Tool Version */
	char sz_toolversion[30]={0};
	char sz_VIDPID[100]={0};
	
	if (!m_p_DLInterface->GetParameterValue(TOOL_VERSION, sz_toolversion,30))
	{
		SetMessage(_T("Get Tool Version From DLL Fail."));
		ShowResultDlg(CommErr_UI_Interface_Getparameter_Fail, _T("初始化失败！\n获取ToolVersion失败！"));
		return false;
	}
	m_st_uiParameter.str_toolVer = ConvertString(sz_toolversion);

	m_p_dlgParent->SetUIIDParameter(m_st_uiParameter);


	if(m_st_uiParameter.str_station == STATION_OS_DL && m_st_uiControl.b_AutoRun)
	{
		if (!m_p_DLInterface->GetParameterValue("SUPPORTED_VIDPID", sz_VIDPID,100))
		{
			if (m_st_uiParameter.str_station == STATION_OS_DL)
			{
				SetMessage(_T("Get VID PID From DLL Fail."));
				ShowResultDlg(CommErr_UI_Interface_Getparameter_Fail, _T("初始化失败！\n获取 VID PID 失败！"));
				return false;
			}
		}
		m_str_VIDPID = ConvertString(sz_VIDPID);
	}
	

	return true;
}


BOOL CDownLoadDlg::OnDeviceChange(UINT nEventType,DWORD_PTR dwData)
{
	CString cs ;
	if (DBT_DEVICEARRIVAL == nEventType || DBT_DEVICEREMOVECOMPLETE == nEventType) 
	{
		PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)dwData;
		switch (pHdr->dbch_devicetype) {
		case DBT_DEVTYP_DEVICEINTERFACE:
			PDEV_BROADCAST_DEVICEINTERFACE pDevInf;
			 pDevInf = (PDEV_BROADCAST_DEVICEINTERFACE)pHdr;
			
			cs.Format(_T("%s"), pDevInf->dbcc_name);
			GetDevice((PDEV_BROADCAST_DEVICEINTERFACE)pHdr,nEventType);
			break;
		default:
			break;
		}
	}
	return TRUE;
}
void CDownLoadDlg::ParserDBCCName(TCHAR dbcc_name[1],CString &str_vendor,CString &str_product,CString &str_para)
{
	// \\?\usb#vid_22b8&pid_2d01#6&.......	
	TCHAR *szToken;
	TCHAR *szNextToken;
	::_tcstok_s(dbcc_name,_T("_&"),&szNextToken);
	szToken = ::_tcstok_s(szNextToken,_T("_&"),&szNextToken);
	str_vendor = szToken;

	::_tcstok_s(szNextToken,_T("_&"),&szNextToken);	
	//szToken = ::_tcstok_s(szNextToken,_T("_&#"),&szNextToken);
	szToken = ::_tcstok_s(szNextToken,_T("_#"),&szNextToken);
	str_product = szToken;	

	szToken = ::_tcstok_s(szNextToken,_T("#"),&szNextToken);	
	str_para = szToken;
}
GUID GUIDAndroidUsbClassID1		= {0xf72fe0d4, 0xcbcb, 0x407d, {0x88, 0x14, 0x9e, 0xd6, 0x73, 0xd0, 0xdd, 0x6b}};

int CDownLoadDlg::FindADBdevice()
{
	GUID *pGUID = &GUIDAndroidUsbClassID1;

//	USBDEVICE_STRUCT devStruct = {0};
	DWORD dwRequired;

	BYTE bTmpStringData[MAX_PATH];
	HDEVINFO hDeviceInfo;
	SP_DEVICE_INTERFACE_DATA deviceInfoData;
	SP_DEVICE_INTERFACE_DETAIL_DATA *pDeviceDetailData = (SP_DEVICE_INTERFACE_DETAIL_DATA *)bTmpStringData;

	SP_DEVINFO_DATA devInfo = {0};
	devInfo.cbSize = sizeof(SP_DEVINFO_DATA);

	hDeviceInfo = SetupDiGetClassDevs(pGUID, NULL, NULL, (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));
	if (INVALID_HANDLE_VALUE == hDeviceInfo)
		return -1;

	bool bFound = false;

	// enum devices
	int i = 0;
	while (1)
	{
		// Get device interface data
		deviceInfoData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		if (! SetupDiEnumDeviceInterfaces(hDeviceInfo, 0, pGUID, i, &deviceInfoData))
			break;

		memset(&devStruct, 0, sizeof(USBDEVICE_STRUCT));

		// Get device path
		pDeviceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		if (SetupDiGetDeviceInterfaceDetail(hDeviceInfo,&deviceInfoData,pDeviceDetailData,256 ,NULL, &devInfo) &&
			(pDeviceDetailData->cbSize > 0) 
			&&(0 == _wcsnicmp(_T("\\\\?\\usb"), pDeviceDetailData->DevicePath, 7))) // We only care about usb device
		{
	
			  char buffer[512];
			 wcstombs ( buffer,  pDeviceDetailData->DevicePath, sizeof(buffer) );
			 TRACE( "The value of x is %s \n", buffer );
	

			// Get the device instance of parent. This points to USB Composite Device
			DEVINST devInstParent;
			CM_Get_Parent(&devInstParent, devInfo.DevInst, 0);

			DWORD dataType;
			unsigned char out_data[1024] = {0};
			if (SetupDiGetDeviceRegistryProperty(hDeviceInfo, &devInfo, SPDRP_ADDRESS, &dataType, (unsigned char*)out_data, MAX_PATH, &dwRequired))
			{
				sz_id = new char;
				_snprintf(sz_id, sizeof(out_data), "%d", out_data[0]);
				return atoi( sz_id);
			}
		
		}
		++i;
		if ( i > 100) break; // time out
	}

	// Destroy handle
	SetupDiDestroyDeviceInfoList(hDeviceInfo);
	return -1;
}

bool CDownLoadDlg::IsValidVIDPID(CString str_vendor,CString str_product)
{   	
	CString strtemp;
	strtemp.Format(_T("%s&%s"),str_vendor,str_product);

	m_str_VIDPID.MakeUpper();
	strtemp.MakeUpper();

	if (m_str_VIDPID.Find(strtemp)!=-1)
	{
		return true;
	}

	return false;
}

void CDownLoadDlg::GetDevice(PDEV_BROADCAST_DEVICEINTERFACE lpbv,UINT nEventType)
{
	CSingleLock singleLock(&m_obj_UIConnect,TRUE);
	if(lpbv)
	{	
		CString strmsg;
		CString str_vendor;	
		CString str_product;		
		CString str_para;	

		int nPortNumber = FindADBdevice();
		//return;
		if ( nPortNumber == -1){
			strmsg.Format(_T("DeviceChanged,ERRor, errPortNumber:%d"), nPortNumber);
			LogMsg(strmsg);
			return ;
		}
		strmsg.Format(_T("DeviceChanged, Type:0x%X, dbcc_name:%s"),nEventType,lpbv->dbcc_name);
		LogMsg(strmsg);
		ParserDBCCName(lpbv->dbcc_name,str_vendor,str_product,str_para);
		//str_para like '5&8639f5&0&7' and then str_Comport = '7'
		//if (str_para.Find('&') == -1)
		//{
		//	return;
		//}
		CString str_Comport;
		str_Comport.Format(_T("%d"), nPortNumber);
			//str_para.Mid(str_para.ReverseFind('&')+1);

		if(IsValidVIDPID(str_vendor,str_product))
		{
			if(nEventType == DBT_DEVICEREMOVECOMPLETE)//device remove
			{   
				//LogMsg(_T("device remove"));

				CString str_UIComPort = _T("");	

				for(int i=0;i<8;i++)					
				{			
					m_label_deviceCOM[i].GetWindowText(str_UIComPort);	
					if(str_Comport==str_UIComPort)			
					{   
						strmsg.Format(_T("[slot:%d ---- port:%s]: DisConnect"),i,str_Comport);
						LogMsg(strmsg);	
						m_label_progress[i].SetWindowText(_T("DISConnect"));	
						m_label_progress[i].SetBkColor(RGB(255,255,255));	
						return;			
					}		
				}
			}
			else if(nEventType == DBT_DEVICEARRIVAL)//device insert
			{   
				//LogMsg(_T("device arrival"));

				CString str_UIComPort = _T("");	
				for(int i=0;i<8;i++)	
				{				
					m_label_deviceCOM[i].GetWindowText(str_UIComPort);		
					if(str_Comport==str_UIComPort)	
					{  	
						CString str_tmp;
						strmsg.Format(_T("[slot:%d ---- port:%s]: Connect"),i,str_Comport);
						LogMsg(strmsg);	

						if (!m_b_returnRes[i])
						{
							LogMsg(_T("device arrival, but the slot is not end"));
							return;
						}
						else
						{
							str_tmp.Format(_T("[%d] ====================== Run BEGIN(V%s) ==========================="),i, m_p_dlgParent->m_str_appver);
							LogMsg(str_tmp);
							m_edit_picasso[i].SetWindowText(_T(""));
							m_label_progress[i].SetBkColor(RGB(255,255,255));
							m_label_progress[i].SetWindowText(_T("5%"));
							m_processCtrl_subProgress[i].SetPos(0);	
							m_processCtrl_mainProgress[i].SetPos(0);	
							m_p_dlgParent->m_map_picasso[i] = _T("");

							m_b_returnRes[i] = false;

							str_tmp.Format(_T("m_b_returnRes[%d] is false"),i);
							LogMsg(str_tmp);

							m_map_running[i] = new CHandsetDL(&m_st_uiParameter, (i), _T(""), m_p_DLInterface,m_map_StationIndexMap,&m_st_uiControl);
							if (m_map_running[i] == NULL)
							{
								/* Show to UI */
								st_UIResult st_result;
								st_result.i_slot = i;
								CStringToChar(CommErr_UI_Run_Thread_Fail, st_result.sz_errCode); 
								m_p_DLInterface->Fire(UI_RESULT, (long)&st_result);

								strcpy_s(st_result.sz_errCode, "FAIL");
								m_p_DLInterface->Fire(UI_RESULT, (long)&st_result);
								return;
							}

							if (!m_map_running[i]->HandsetRun())
							{
								/* Show to UI */
								st_UIResult st_result;
								st_result.i_slot = i;
								CStringToChar(CommErr_UI_Run_Thread_Fail, st_result.sz_errCode); 
								m_p_DLInterface->Fire(UI_RESULT, (long)&st_result);

								strcpy_s(st_result.sz_errCode, "FAIL");
								m_p_DLInterface->Fire(UI_RESULT, (long)&st_result);
								return;
							}
							return;
						}
					}
				}
			}
		}//IsValidVIDPID
		else
		{
            LogMsg(_T("not invalid vidpid, ignore it"));
		}
	}
}


//void CDownLoadDlg::GetDevice(PDEV_BROADCAST_DEVICEINTERFACE lpbv,UINT nEventType)
//{
//	CSingleLock singleLock(&m_obj_UIConnect,TRUE);
//	if(lpbv)
//	{	
//		CString strmsg;
//		CString str_vendor;	
//		CString str_product;		
//		CString str_para;	
//
//		FindADBdevice();
//		strmsg.Format(_T("DeviceChanged, Type:0x%X, dbcc_name:%s"),nEventType,lpbv->dbcc_name);
//		LogMsg(strmsg);
//		ParserDBCCName(lpbv->dbcc_name,str_vendor,str_product,str_para);
//		//str_para like '5&8639f5&0&7' and then str_Comport = '7'
//		if (str_para.Find('&') == -1)
//		{
//			return;
//		}
//		CString str_Comport = str_para.Mid(str_para.ReverseFind('&')+1);
//
//		if(IsValidVIDPID(str_vendor,str_product))
//		{
//			if(nEventType == DBT_DEVICEREMOVECOMPLETE)//device remove
//			{   
//				//LogMsg(_T("device remove"));
//
//				CString str_UIComPort = _T("");	
//
//				for(int i=0;i<8;i++)					
//				{			
//					m_label_deviceCOM[i].GetWindowText(str_UIComPort);	
//					if(str_Comport==str_UIComPort)			
//					{   
//						strmsg.Format(_T("[slot:%d ---- port:%s]: DisConnect"),i,str_Comport);
//						LogMsg(strmsg);	
//						m_label_progress[i].SetWindowText(_T("DISConnect"));	
//						m_label_progress[i].SetBkColor(RGB(255,255,255));	
//						return;			
//					}		
//				}
//			}
//			else if(nEventType == DBT_DEVICEARRIVAL)//device insert
//			{   
//				//LogMsg(_T("device arrival"));
//
//				CString str_UIComPort = _T("");	
//				for(int i=0;i<8;i++)	
//				{				
//					m_label_deviceCOM[i].GetWindowText(str_UIComPort);		
//					if(str_Comport==str_UIComPort)	
//					{  	
//						CString str_tmp;
//						strmsg.Format(_T("[slot:%d ---- port:%s]: Connect"),i,str_Comport);
//						LogMsg(strmsg);	
//
//						if (!m_b_returnRes[i])
//						{
//							LogMsg(_T("device arrival, but the slot is not end"));
//							return;
//						}
//						else
//						{
//							str_tmp.Format(_T("[%d] ====================== Run BEGIN(V%s) ==========================="),i, m_p_dlgParent->m_str_appver);
//							LogMsg(str_tmp);
//							m_edit_picasso[i].SetWindowText(_T(""));
//							m_label_progress[i].SetBkColor(RGB(255,255,255));
//							m_label_progress[i].SetWindowText(_T("5%"));
//							m_processCtrl_subProgress[i].SetPos(0);	
//							m_processCtrl_mainProgress[i].SetPos(0);	
//							m_p_dlgParent->m_map_picasso[i] = _T("");
//
//							m_b_returnRes[i] = false;
//
//							str_tmp.Format(_T("m_b_returnRes[%d] is false"),i);
//							LogMsg(str_tmp);
//
//							m_map_running[i] = new CHandsetDL(&m_st_uiParameter, (i), _T(""), m_p_DLInterface,m_map_StationIndexMap,&m_st_uiControl);
//							if (m_map_running[i] == NULL)
//							{
//								/* Show to UI */
//								st_UIResult st_result;
//								st_result.i_slot = i;
//								CStringToChar(CommErr_UI_Run_Thread_Fail, st_result.sz_errCode); 
//								m_p_DLInterface->Fire(UI_RESULT, (long)&st_result);
//
//								strcpy_s(st_result.sz_errCode, "FAIL");
//								m_p_DLInterface->Fire(UI_RESULT, (long)&st_result);
//								return;
//							}
//
//							if (!m_map_running[i]->HandsetRun())
//							{
//								/* Show to UI */
//								st_UIResult st_result;
//								st_result.i_slot = i;
//								CStringToChar(CommErr_UI_Run_Thread_Fail, st_result.sz_errCode); 
//								m_p_DLInterface->Fire(UI_RESULT, (long)&st_result);
//
//								strcpy_s(st_result.sz_errCode, "FAIL");
//								m_p_DLInterface->Fire(UI_RESULT, (long)&st_result);
//								return;
//							}
//							return;
//						}
//					}
//				}
//			}
//		}//IsValidVIDPID
//		else
//		{
//            LogMsg(_T("not invalid vidpid, ignore it"));
//		}
//	}
//}

void CDownLoadDlg::DeviceDetectSetup()
{
	HDEVNOTIFY hDevNotify;
	DEV_BROADCAST_DEVICEINTERFACE filter;	
	memset(&filter,0,sizeof(DEV_BROADCAST_DEVICEINTERFACE));
	filter.dbcc_size         = sizeof(filter);	
	filter.dbcc_devicetype   = DBT_DEVTYP_DEVICEINTERFACE;	
	filter.dbcc_classguid    = GUID_CLASS_USB_DEVICE;	
	hDevNotify = RegisterDeviceNotification(this->GetSafeHwnd(), &filter, DEVICE_NOTIFY_WINDOW_HANDLE);	
	if (!hDevNotify)	
	{		
		AfxMessageBox(_T("Cannot register device notification: "), MB_ICONEXCLAMATION);	
	}
}

//=================================================================================================//
//======================================= CHandset ================================================//
//=================================================================================================//
CHandsetDL::CHandsetDL (st_UIIDparameter* p_st_uiParameter, int i_slot, 
						CString str_picasso, 
						IFacTestToolInterface* p_DLInterface,
						std::map<CString,CString> map_StationIndexMap,
						st_UIControl* p_st_uiControl
						)
	: m_p_st_uiParameter(p_st_uiParameter)
	, m_i_slot(i_slot)
    , m_str_picasso(str_picasso)
	, m_p_handsetDLInterface(p_DLInterface)
	,m_map_StationIndexMap(map_StationIndexMap)
	,m_p_st_uiControl(p_st_uiControl)
{	
	m_b_Rework = false;
	m_str_mobileid = _T("");
	m_p_h_thread = NULL;
	m_b_runRes = false;
}

CHandsetDL::~CHandsetDL()
{

#ifndef RD_MODE
	m_obj_QueryCIM.UpLoadDBAccessDLL();
#endif

	m_p_handsetDLInterface = NULL;
}

//CCriticalSection CHandsetDL::m_obj_CritSection;

bool CHandsetDL::HandsetRun(void)
{
	m_p_h_thread = ::AfxBeginThread(DLRun, (void*)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	if (m_p_h_thread == NULL)
	{
		return false;
	}
	
	return true;
}

UINT CHandsetDL::DLRun(LPVOID pParam)
{	
	CHandsetDL* p_this = (CHandsetDL*)pParam;
	char sz_errCode[100] = {0};
	char sz_picasso[20] = {0};

#ifndef RD_MODE
	if (!p_this->m_obj_QueryCIM.LoadDBAccessDLL())
	{
		p_this->FireUIMessage("LoadDBAccessDLL Fail");

		CStringToChar(CommErr_UI_DLL_Load_Fail,sz_errCode);
		p_this->FireUIResult(sz_errCode);
		p_this->FireUIResult("FAIL");
		return 0;
	}
	p_this->FireUIMessage("LoadDBAccessDLL success");
#endif

	if ((p_this->m_p_st_uiParameter->str_station == STATION_OS_DL) && (p_this->m_p_st_uiControl->b_AutoRun))
	{
#ifndef FACTORY_MODE
		if (p_this->m_p_st_uiControl->b_ReadFA)
#endif	
		{	
			/* Read FA */
			if (!(p_this->ReadFA(sz_picasso)))
			{
				p_this->FireUIMessage("ReadFA Fail.");
				CStringToChar(CommErr_UI_Interface_GetFA_Fail,sz_errCode);
				p_this->FireUIResult(sz_errCode);
				p_this->FireUIResult("FAIL");
				return 0;
			}

			p_this->FireUIPicasso(sz_picasso);
#ifdef FACTORY_MODE
			/*check picasso rule and check routing*/
			if (!p_this->CheckRouting(sz_picasso,sz_errCode))
			{
				p_this->FireUIResult(sz_errCode);
				p_this->FireUIResult("FAIL");
				return 0;
			}
#endif
			p_this->m_str_picasso = sz_picasso;
		}
	}

	/* Set Picasso */
	CString str_temp;
	str_temp.Format(_T("%d,%s"), p_this->m_i_slot, p_this->m_str_picasso);
	CStringToChar(str_temp, sz_picasso);
	p_this->LogMsg(str_temp);

	if (!p_this->m_p_handsetDLInterface->SetParameterValue(PICASSO, sz_picasso))
	{
		p_this->FireUIMessage("Set Picasso Fail.");
		CStringToChar(CommErr_UI_Interface_Setparameter_Fail,sz_errCode);
		p_this->FireUIResult(sz_errCode);
		p_this->FireUIResult("FAIL");
		return 0;
	}
	p_this->FireUIMessage("Set Picasso Success.");

	if (p_this->m_p_st_uiControl->b_RelayBoard)
	{
		p_this->FireRelayControl(true);
		p_this->FireUIMessage("Open Relay board.");
	}

	/* PreRun */
	bool b_res = false;
	p_this->FireUIMessage("PreRun start.");
	if (!(b_res = p_this->m_p_handsetDLInterface->PreRun(p_this->m_i_slot)))
	{
		p_this->FireUIMessage("PreRun Fail.");
	}
	else
	{
		p_this->FireUIMessage("PreRun Success.");
	}
	
	if (p_this->m_p_st_uiControl->b_RelayBoard)
	{
		p_this->FireRelayControl(false);
		p_this->FireUIMessage("Close Relay board.");
	}

    /* Run */
	if (b_res)
	{
		p_this->FireUIMessage("Run start.");
		b_res = p_this->m_p_handsetDLInterface->Run(p_this->m_i_slot);
		if (!b_res)
		{
			p_this->FireUIMessage("Run Fail.");
			p_this->m_b_runRes = false;
		}
		else
		{
			p_this->FireUIMessage("Run Success.");
			p_this->m_b_runRes = true;
		}
	}

	p_this->LogMsg(p_this->m_p_st_uiParameter->str_station);
	p_this->LogMsg(p_this->m_map_StationIndexMap[p_this->m_p_st_uiParameter->str_station]);
	p_this->LogMsg(p_this->m_p_st_uiParameter->str_segment);
	
#ifdef FACTORY_MODE	
	if (!p_this->m_p_st_uiParameter->b_tsFlag && p_this->m_b_runRes)
	{
		
		/* Read FA */
		if ( ((p_this->m_map_StationIndexMap[p_this->m_p_st_uiParameter->str_station]==_T("01")) && (p_this->m_p_st_uiParameter->str_segment==_T("TLINE"))) || (p_this->m_p_st_uiControl->b_ReadFA) || (p_this->m_p_st_uiControl->b_WriteFA))
		{
			if (!(b_res = p_this->ReadFA()))
			{
				p_this->FireUIMessage("ReadFA Fail.");

				if (p_this->m_b_runRes)
				{
					CStringToChar(CommErr_UI_Interface_GetFA_Fail,sz_errCode);
					p_this->FireUIResult(sz_errCode);
				}
			}
			else
			{
				p_this->FireUIMessage("Read FA Success.");
			}
		}		
		
		/* Check Picasso with CIM */	
		if (b_res)
		{
			if (p_this->m_map_StationIndexMap[p_this->m_p_st_uiParameter->str_station]==_T("01") && p_this->m_p_st_uiParameter->str_segment==_T("TLINE"))
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
					p_this->FireUIMessage("Check Picasso Duplicate Success.");
				}		
			}
		}

		/* Initial FA */
		if (b_res)
		{
			if (p_this->m_map_StationIndexMap[p_this->m_p_st_uiParameter->str_station]==_T("01") && p_this->m_p_st_uiParameter->str_segment==_T("TLINE"))
			{
				if (!(b_res = p_this->InitialFA()))
				{
					p_this->FireUIMessage("Initial FA Fail.");

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
		}
		
		/* Write CIM */
		if (b_res)
		{
			if (p_this->m_map_StationIndexMap[p_this->m_p_st_uiParameter->str_station]==_T("01") && p_this->m_p_st_uiParameter->str_segment==_T("TLINE"))
			{
				if (!(b_res = p_this->InsertPicassoControl(sz_errCode)))
				{
					p_this->FireUIMessage("Write PicassoCheck Fail.");
					if (p_this->m_b_runRes)
					{
						p_this->FireUIResult(sz_errCode);
					}
				}
				else
				{
					p_this->FireUIMessage("Write PicassoCheck Success.");
				}	
			}	
		}	

	    /* Write FA */
		if (b_res)
		{
			if (p_this->m_p_st_uiControl->b_WriteFA)
			{
				if (!(b_res = p_this->WriteFA(sz_errCode)))
				{
					p_this->FireUIMessage("WriteFA Fail.");
					if (p_this->m_b_runRes)
					{
						p_this->FireUIResult(sz_errCode);
					}
				} 
				else
				{
					p_this->FireUIMessage("Write FA Test Result Success.");
				}
			}			
		}
		
	}
	
#else
    /* Read FA */
	if(b_res)
	{
		
		//if ((p_this->m_p_st_uiControl->b_ReadFA) || (p_this->m_p_st_uiControl->b_WriteFA))
		if (p_this->m_p_st_uiControl->b_WriteFA)
		{
			if (!(b_res = p_this->ReadFA()))
			{
				p_this->FireUIMessage("ReadFA Fail.");

				if (p_this->m_b_runRes)
				{
					CStringToChar(CommErr_UI_Interface_GetFA_Fail,sz_errCode);
					p_this->FireUIResult(sz_errCode);
				}
			}
			else
			{
				p_this->FireUIMessage("Read FA Success.");
			}
		}
	}

	/* Initial FA */
	if (b_res)
	{
		//if ((p_this->m_map_StationIndexMap[p_this->m_p_st_uiParameter->str_station]==_T("01")) && (p_this->m_p_st_uiParameter->str_segment==_T("TLINE")) && p_this->m_p_st_uiControl->b_WriteFA)
		if ((p_this->m_map_StationIndexMap[p_this->m_p_st_uiParameter->str_station]==_T("01")) && p_this->m_p_st_uiControl->b_WriteFA)//20140210 changed by Nancy.Yang,there have no segment define in RD mode 
		{
			if (!(b_res = p_this->InitialFA()))
			{
				p_this->FireUIMessage("Initial FA Fail.");

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
	}

	/* Write FA */
	if (b_res && p_this->m_p_st_uiControl->b_WriteFA)
	{
		if (!(b_res = p_this->WriteFA(sz_errCode)))
		{
			p_this->FireUIMessage("WriteFA Fail.");
			if (p_this->m_b_runRes)
			{
				p_this->FireUIResult(sz_errCode);
			}
		} 
		else
		{
			p_this->FireUIMessage("Write FA Test Result Success.");
		}
	}
#endif

	/* Post Run */
	p_this->m_p_handsetDLInterface->PostRun(p_this->m_i_slot);

	p_this->FireUIMessage("PostRun Success.");

	if (p_this->m_p_st_uiControl->b_RelayBoard)
	{
		p_this->FireRelayControl(true);
	}

	if (!b_res)
	{
		p_this->FireUIResult("FAIL");
	}
	else
	{
		p_this->FireUIResult("PASS");
	}

	return 1;
}

bool CHandsetDL::ReadFA(char* sz_picasso)
{	
	//CSingleLock singleLock(&m_obj_CritSection,true);
	FireUIMessage("Start to Read FA..");

	if (m_p_st_uiControl->i_FAArea == OLDFA)
	{
		/* Read FA */
		char sz_readFAData[512] = {0};
		if (!(m_p_handsetDLInterface->GetFAData(m_i_slot, sz_readFAData, sizeof(sz_readFAData))))
		{
			FireUIMessage("Read FA from phone Fail.");
			return false;
		}

		/* Set FA data to FA Operator */
		if (!m_obj_FA.SetFA(sz_readFAData))
		{		
			FireUIMessage("FAOperator SetFA Fail.");
			return false;
		}

		if (sz_picasso != NULL)//os dl auto run
		{
			m_obj_FA.GetPicassoNumber(sz_picasso);
		}
		
		/* FA log */
		CString str_tmp;
		str_tmp.Format(_T("[%d]Read FA Data success: %s"), m_i_slot, ConvertToVisibleString(sz_readFAData,FA_SIZE));
		FireUIMessage("Read FA Data success");
		LogMsg(str_tmp);
	}
	else if (m_p_st_uiControl->i_FAArea == NEWFA)
	{
		/* Read FA */
		char sz_readFAData[512] = {0};
		if (!(m_p_handsetDLInterface->GetFASector(m_i_slot, 0, sz_readFAData, sizeof(sz_readFAData))))
		{
			FireUIMessage("Read FA from phone Fail.");
			return false;
		}

		/* Set FA data to FA Operator */
		if (!m_obj_newFA.SetSectorData(sz_readFAData))
		{		
			FireUIMessage("FAOperator SetFA Fail.");
			return false;
		}

		if (sz_picasso != NULL)//os dl auto run
		{
			CStringToChar(m_obj_newFA.GetPicassoNumber(),sz_picasso);
		}

		/* FA log */
		CString str_tmp;
		str_tmp.Format(_T("[%d] Read FA Sector success: %s"), m_i_slot, ConvertString(sz_readFAData));
		FireUIMessage("Read FA Sector success");
		LogMsg(str_tmp);
	}

	return true;
}

bool CHandsetDL::InitialFA()
{
	FireUIMessage("In TLINE-01: Start to Initial FA..");
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
		FireUIMessage("FA Operator EraseFactoryArea Fail.");
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
		FireUIMessage("FA Operator FA_Station_Setup Fail.");
		return false;
	}

	//if rework,set to mobile old picasso and mobile id
	if (m_b_Rework)
	{	
		LogMsg(_T("Rework:SetPicassoNumber PicassoNumber is ") + ConvertString(sz_picasso));
		if (!m_obj_FA.SetPicassoNumber(sz_picasso))
		{
			FireUIMessage("SetPicassoNumber in rework error.");
			return false;
		}
		LogMsg(_T("Rework:SetMobileID mobile id is ") + ConvertToVisibleString(sz_mobileid_byt,11));
		if (!m_obj_FA.SetMobileID(sz_mobileid_byt))
		{
			FireUIMessage("SetMobileID in rework error.");
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
			FireUIMessage("FA Operator Write Picasso Fail.");
			return false;
		}

		m_obj_FA.GenerateMobileID(sz_mobileid_byt,m_i_slot);

		LogMsg(_T("new work:SetMobileID mobile id is ") + ConvertToVisibleString(sz_mobileid_byt,11));

		if (!m_obj_FA.SetMobileID(sz_mobileid_byt))
		{
			FireUIMessage("SetMobileID in new work error.");
			return false;
		}

	}

	/* Write ModelName */
	if (!m_obj_FA.SetModelName(m_p_st_uiParameter->str_modelName))
	{
		FireUIMessage("FA Operator Write ModelName Fail.");
		return false;
	}

	/* Write SW Version */
	if (!m_obj_FA.SetSWVersion(m_p_st_uiParameter->str_userLoadVer))
	{
		FireUIMessage("FA Operator Write SWVersion Fail.");
		return false;
	}

	char sz_TimeInfo[30] = {0};
	m_obj_FA.GetMobileIDHexStr(sz_TimeInfo);
	m_str_mobileid = ConvertString(sz_TimeInfo);

	/* Get FA data */
	char sz_FA[512] = {0};
	if (!m_obj_FA.GetFA(sz_FA))
	{
		FireUIMessage("FA Operator GetFA Fail.");
		return false;
	}

	/* FA log*/ 
	str_tmp.Format(_T("[%d] Write FA DATA: %s"),m_i_slot,ConvertToVisibleString(sz_FA,512));
	LogMsg(str_tmp);

	FireUIMessage("Begin: Set initial FA to Mobile");
	//CSingleLock singleLock(&m_obj_CritSection,true);
	/* Write FA */
	if (!m_p_handsetDLInterface->SetFAData(m_i_slot, sz_FA))
	{
		FireUIMessage("Set FA to Mobile fail.");
		return false;
	}

	char sz_checkFA[512]={0};
	/* Read FA */
	if (!m_p_handsetDLInterface->GetFAData(m_i_slot, sz_checkFA,sizeof(sz_checkFA)))
	{
		FireUIMessage("Read FA from Mobile fail.");
		return false;
	}

	str_tmp.Format(_T("[%d] Read FA DATA: %s"),m_i_slot,ConvertToVisibleString(sz_checkFA,512));
	LogMsg(str_tmp);

	m_obj_FA.SetFA(sz_checkFA);
	char sz_checkPicasso[20]={0};
	m_obj_FA.GetPicassoNumber(sz_checkPicasso);

	if (strcmp(sz_checkPicasso,sz_picasso) != 0)
	{
		FireUIMessage("FA data write error occurred.");	
		return false;
	}

	FireUIMessage("End: Initial FA to Mobile success!");

	return true;
}

bool CHandsetDL::InitialFA_new()
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
			FireUIMessage("Get FA Basic data Fail.");
			return false;
		}

		m_str_mobileid = m_obj_newFA.GetMobileID();

	//	CSingleLock singleLock(&m_obj_CritSection,true);
		/* Write FA */
		if (!m_p_handsetDLInterface->SetFASector(m_i_slot, 0 , sz_faData ,sizeof(sz_faData)))
		{
			FireUIMessage("Write FA Basic data Fail.");
			return false;
		}
		FireUIMessage("Write FA Basic PASS.");

		/* FA log */
		CString str_tmp;
		str_tmp.Format(_T("[%d]Initial Write FA Data: %s"), m_i_slot, ConvertString(sz_faData));
		LogMsg(str_tmp);

		/* Read FA */
		char sz_checkFA[512]={0};
		if (!m_p_handsetDLInterface->GetFASector(m_i_slot, 0 , sz_checkFA ,sizeof(sz_checkFA)))
		{
			FireUIMessage("Read FA Basic data Fail.");
			return false;
		}
		
		m_obj_newFA.SetSectorData(sz_checkFA);

		CString str_checkpicasso = m_obj_newFA.GetPicassoNumber();
		if (m_str_picasso != str_checkpicasso)
		{
			FireUIMessage("Read FA Basic data Fail.");
			return false;
		}
		str_tmp.Format(_T("[%d]Initial Read FA Data: %s"), m_i_slot, ConvertString(sz_checkFA));
		LogMsg(str_tmp);

	}
	else
	{
		m_b_Rework = true;
	}

	FireUIMessage("End: Initial FA to Mobile success!");
	return true;
}

bool CHandsetDL::WriteFA(char* sz_errCode)
{
	//CSingleLock singleLock(&m_obj_CritSection,true);

	FireUIMessage("Start to Write FA..");

	if (m_p_st_uiControl->i_FAArea == OLDFA)
	{
		/* Not write FA */
		if (!m_p_st_uiControl->b_WriteFA)
		{
			FireUIMessage("Not Write FA.");
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
			FireUIMessage(szErrmsg);
			FireUIMessage("Set FA station info Fail.");
			return false;
		}

		/* Get FA data */
		char sz_FA[512] = {0};
		if (!m_obj_FA.GetFA(sz_FA))
		{
			FireUIMessage("FAOperator GetFA Fail.");
			return false;
		}

		/* Write FA */
		if (!m_p_handsetDLInterface->SetFAData(m_i_slot, sz_FA))
		{
			CStringToChar(CommErr_UI_Interface_SetFA_Fail,sz_errCode);
			FireUIMessage("Write FA to phone Fail.");
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
			FireUIMessage("Get FA Record data Fail.");
			return false;
		}

		/* Write FA record */
		if (!m_p_handsetDLInterface->SetFASector(m_i_slot, m_obj_newFA.GetPointer(), sz_faData, sizeof(sz_faData)))
		{
			CStringToChar(CommErr_UI_Interface_SetFA_Fail,sz_errCode);
			FireUIMessage("Write FA Record data Fail.");
			return false;
		}
		FireUIMessage("Write FA Record PASS.");

		/* FA log */
		CString str_tmp;
		str_tmp.Format(_T("[%d] Writed FA Record Data: %s"), m_i_slot, ConvertString(sz_faData));
		LogMsg(str_tmp);

		/* Update FA pointer */
		m_obj_newFA.IncRecordNum();
		memset(sz_faData,0x00,sizeof(sz_faData));

		if (!m_obj_newFA.GetBasicData(sz_faData))
		{
			FireUIMessage("Get FA Basic data Fail.");
			return false;
		}

		/* Update FA pointer*/
		if (!m_p_handsetDLInterface->SetFASector(m_i_slot,0, sz_faData, sizeof(sz_faData)))
		{
			CStringToChar(CommErr_UI_Interface_SetFA_Fail,sz_errCode);
			FireUIMessage("Write FA Basic data Fail.");
			return false;
		}
		FireUIMessage("Write FA pointer PASS.");

		/* FA log */
		str_tmp.Format(_T("[%d] Writed FA Basic Data[Update pointer]: %s"), m_i_slot, ConvertString(sz_faData));
		LogMsg(str_tmp);
	}
	
	return true;
}

bool CHandsetDL::RestoreFA()
{
//	CSingleLock singleLock(&m_obj_CritSection,true);

	FireUIMessage("RestoreFA -> Clear FA.");

	if (m_p_st_uiControl->i_FAArea == OLDFA)
	{

		TCHAR sz_errorCode[1024] = {0};
		TCHAR sz_errorMsg[1024] = {0};

		/* Erase FA */
		if (!m_obj_FA.EraseFactoryArea())
		{
			FireUIMessage("FAOperator EraseFactoryArea Fail.");
			return false;
		}

		/* Get FA data */
		char sz_FA[512] = {0};
		if (!m_obj_FA.GetFA(sz_FA))
		{
			FireUIMessage("FAOperator GetFA Fail.");
			return false;
		}

		/* Write FA */
		if (!m_p_handsetDLInterface->SetFAData(m_i_slot, sz_FA))
		{
			FireUIMessage("Write FA after clear FA Fail.");
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
			FireUIMessage("FAOperator EraseFactoryArea Fail.");
			return false;
		}

		/* Get FA basic data */
		char sz_FA[512] = {0};
		if (!m_obj_newFA.GetBasicData(sz_FA))
		{
			FireUIMessage("FAOperator GetFA Fail.");
			return false;
		}

		/* Write FA */
		if (!m_p_handsetDLInterface->SetFASector(m_i_slot, 0, sz_FA, sizeof(sz_FA)))
		{
			FireUIMessage("Write FA after clear FA Fail.");
			return false;
		}
	}

	return true;
}


void CHandsetDL::FireUIMessage(const char* sz_message)
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

	if (this->m_p_handsetDLInterface != NULL)
	{
		m_p_handsetDLInterface->Fire(UI_MESSAGE, (long)&st_message);
	}

	return;
}
	
void CHandsetDL::FireUIResult(const char* sz_result)
{
	/* Check input */
	if (sz_result == NULL)
	{
		return;
	}

	/* Send result */
	st_UIResult st_result;
	st_result.i_slot = m_i_slot;
	strcpy_s(st_result.sz_errCode, sz_result);

	if (this->m_p_handsetDLInterface != NULL)
	{
		m_p_handsetDLInterface->Fire(UI_RESULT, (long)&st_result);
	}

	return;
}

void CHandsetDL::FireUIPicasso(const char* sz_picasso)
{	
	/* Send message */
	st_UIMessage st_message;
	st_message.i_slot = m_i_slot;
	strcpy_s(st_message.sz_message, sz_picasso);

	if (this->m_p_handsetDLInterface != NULL)
	{
		m_p_handsetDLInterface->Fire(UI_PICASSO, (long)&st_message);
	}
	return;
}


void CHandsetDL::FireRelayControl( bool bOn )
{
	st_RelayBoard st_relayboard;
	st_relayboard.i_slot = m_i_slot;
	st_relayboard.i_Seq = m_i_slot + 1;
	st_relayboard.b_OnOff = bOn;

	if (this->m_p_handsetDLInterface != NULL)
	{
		m_p_handsetDLInterface->Fire(UI_RELAYBOARD, (long)&st_relayboard);
	}

	return;
}

bool CHandsetDL::CheckPicassoDuplicate(char *sz_errCode)
{
	FireUIMessage("In TLINE-01: Start to CheckPicassoDuplicate");

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
		FireUIMessage(sz_message);
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
				str_tmp = _T("No record in CIM but found picasso and mobileid in target\nCIM中没有记录，但手机中发现picasso：") + ConvertString(sz_Picasso);
				CStringToChar(str_tmp,sz_message);
				CStringToChar(CommErr_UI_CIM_Phone_Init_Not_Record,sz_errCode);
				FireUIMessage(sz_message);
				return false;
			}

		}
		else if (m_p_st_uiControl->i_FAArea == NEWFA)
		{
			//there is picasso number in handset=> fail
			if(m_obj_newFA.IsValidePicasso()) 
			{
				str_tmp = _T("No record in CIM but found picasso and mobileid in target\nCIM中没有记录，但手机中发现picasso：") + ConvertString(sz_Picasso);
				CStringToChar(str_tmp,sz_message);
				CStringToChar(CommErr_UI_CIM_Phone_Init_Not_Record,sz_errCode);
				FireUIMessage(sz_message);
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
			FireUIMessage("The picasso Number status error! \nPicasso已被锁定！");
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
						str_tmp.Format(_T("Picasso in mobile mismatched with UI! \n手机中picasso(%s)与标签(%s)不符！"),ConvertString(sz_Picasso),m_str_picasso);
						CStringToChar(str_tmp,sz_message);
						FireUIMessage(sz_message);
						return false;
					}

					//Time info not equal
					if(((CString)sz_mobileCIM) != ((CString)sz_Mobileid_str)) 
					{
						CStringToChar(CommErr_UI_CIM_MobileID_Match_Fail,sz_errCode);
						FireUIMessage("The mobile ID not equal!\n手机中的mobileid与CIM中不符！");
						return false;
					}

				}
				//there is not picasso number in handset
				else 
				{
					CStringToChar(CommErr_UI_CIM_Phone_Reocrd_Not_Init,sz_errCode);
					FireUIMessage("No picasso in target but found record in CIM!\nCIM中有记录，手机内没有记录！");
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
						str_tmp.Format(_T("Picasso in mobile mismatched with UI! \n手机中picasso(%s)与标签(%s)不符！"),ConvertString(sz_Picasso),m_str_picasso);
						CStringToChar(str_tmp,sz_message);
						FireUIMessage(sz_message);
						return false;
					}

					//Time info not equal
					if(((CString)sz_mobileCIM) != ((CString)sz_Mobileid_str)) 
					{
						CStringToChar(CommErr_UI_CIM_MobileID_Match_Fail,sz_errCode);
						FireUIMessage("The mobile ID not equal!\n手机中的mobileid与CIM中不符！");
						return false;
					}

				}
				//there is not picasso number in handset
				else 
				{
				CStringToChar(CommErr_UI_CIM_Phone_Reocrd_Not_Init,sz_errCode);
				FireUIMessage("No picasso in target but found record in CIM!\nCIM中有记录，手机内没有记录！");
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
					str_tmp.Format(_T("Picasso in mobile mismatched with UI! \n手机中picasso(%s)与标签(%s)不符！"),ConvertString(sz_Picasso),m_str_picasso);
					CStringToChar(str_tmp,sz_message);
					FireUIMessage(sz_message);
					return false;
				}

				//Time info not equal
				if(((CString)sz_mobileCIM) != ((CString)sz_Mobileid_str)) 
				{
					CStringToChar(CommErr_UI_CIM_MobileID_Match_Fail,sz_errCode);
					FireUIMessage("The mobileid not equal!\n手机中的mobileid与CIM中不符！");
					return false;
				}

			}
		}
		//if get other status
		else
		{
			CStringToChar(CommErr_UI_CIM_Picasso_Status_Invalide,sz_errCode);
			FireUIMessage("The picasso status error! \n picasso状态不合法");
			return false;
		}
	}
	return true;
}

bool CHandsetDL::InsertPicassoControl( char *sz_errCode )
{
	FireUIMessage("In TLINE-01: Start to InsertPicassoControl..");
	//rework status will not insert picasso
	if (m_b_Rework)
	{
		FireUIMessage("Rework mode, skip InsertPicassoControl");
		return true;
	}

	char sz_status[2] = {'N',0};
	
	CString str_tmp = _T("");
	str_tmp.Format(_T("(%s),(%s),(%s) insert into CIM "),m_str_picasso,m_str_mobileid,sz_status);

	char sz_picasso[20] = {0};
	char sz_TimeInfo[30] = {0};
	char sz_message[1024]={0};

	CStringToChar(m_str_picasso,sz_picasso);	
	CStringToChar(m_str_mobileid,sz_TimeInfo);

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
			FireUIMessage(sz_message);
			str_tmp += _T("fail:");
			str_tmp += str_errorMsg;
			LogMsg(str_tmp);

			if (!RestoreFA())
			{
				char sz_message[] = "Insert PicassoControl twice Fail,Erase FA data Fail.";
				FireUIMessage(sz_message);
			}
			else
			{
				char sz_message[] = "Insert PicassoControl twice Fail,Erase FA data ok.";
				FireUIMessage(sz_message);
			}
			return false;
		}
	}		

	LogMsg(str_tmp + _T("success!"));

	return true;
}

void CHandsetDL::LogMsg( CString logContent )
{
	m_obj_log.SaveLog(logContent);
}

bool CHandsetDL::CheckRouting(char *sz_picasso,char *sz_errCode)
{
	char sz_errMsg[512]={0};
	CString str_picasso = ConvertString(sz_picasso);

	if (!m_p_st_uiParameter->b_tsFlag)
	{
		if (!Check_Picasso_Rule_All(str_picasso))
		{  
			CStringToChar(CommErr_UI_Picasso_Invalide, sz_errCode); 
			CStringToChar(_T("CheckPicassoRule fail"), sz_errMsg); 
			FireUIMessage(sz_errMsg);
			return false;
		}
	}
	FireUIMessage("Check picasso rule success!");

	if (!(m_p_st_uiParameter->b_tsFlag) && (m_p_st_uiParameter->str_station != STATION_BIST) && (m_p_st_uiParameter->str_station != STATION_REDL))
	{  
		/* Check CIM routing */ 
		m_obj_QueryCIM.SetidParameter(*m_p_st_uiParameter);//it must be set first

		if (!m_obj_QueryCIM.QueryStationRoutingInfoFromCIM(sz_picasso))
		{	
			CStringToChar(m_obj_QueryCIM.GetErrorCode(),sz_errCode);
			CStringToChar(m_obj_QueryCIM.GetErrorMsg(),sz_errMsg);
			FireUIMessage(sz_errMsg);
			return false;
		}
	}

	FireUIMessage("Check routing success!");

	return true;
}
