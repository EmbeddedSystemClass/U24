/***************************************************************************
* Copyright (C) : QCS/WR318                                      
* Module name   : Dialog 
* File name     : IMEIFCDlg.h             
* Summary       : UI for IMEI FinalCheck                                                  
* Version     Author             Date                Abstract                      
* 1.0         Nancy.Yang         2011/07/07          First version  
****************************************************************************/

#pragma once

#include "afxcmn.h"
#include "afxwin.h"

#include "CommDef.h"
#include "FAOperator.h"
#include "FAOperator_new.h"
#include "..\..\Modules\UnifyInterface\FacTestToolInterface.h"
#include "UnifyUI_FacTestToolDlg.h"
#include "ResultDlg.h"
#include "PageInterface.h"
#include "PassWord.h"

class CIMEIFCDlg : public CPageInterface, public QSYNC_LIB::INotify_Char
{
	DECLARE_DYNAMIC(CIMEIFCDlg)

public:
	CIMEIFCDlg(CWnd* pParent = NULL);
	virtual ~CIMEIFCDlg();
	enum { IDD = IDD_IMEI_FC_STATION };

public:
	virtual void Event(const char* str_event, long nParam);
	bool Run(void);
	bool Stop(void)
	{
		return true;
	}
	bool GetPicasso(std::map<int, CString> &map_picasso);
	bool SetPicasso(std::map<int, CString> map_picasso)
	{
		return true;
	}

	bool GetTag(std::map<int, CString> &map_Tag) { return true; };
	bool SetTag(std::map<int, CString> map_Tag) { return true; };
	bool GetSn(std::map<int, CString> &map_Sn) { return true; };
	bool SetSn(std::map<int, CString> map_Sn) { return true; };
	bool PostRun();
	void SetTestResult(int i_slot, CString str_testresult){}

	bool ResetPage();
	void ChangeGB();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//afx_msg void OnCustomdrawList ( NMHDR* pNMHDR, LRESULT* pResult );
	DECLARE_MESSAGE_MAP()

private:
	bool Init(void);
	bool InitUI(void);
	bool LoadDLL(void);
	bool DLLBegin(void);
	bool SetParameterToDLL(void);
	bool GetParameterFromDLL(void);
	bool InitTestItemList(void);
	void ClearMessageList(void);
	/*BOOL IsStringAlphaNumber(CString &str_text);*/
	void AddMessage(const CString str_info);
	bool AddInformationList(const CString str_key, const CString str_value);
	bool Check_IMEI_Validity(const CString str_imei);
	bool Check_MEID_Validity(CString str_MEID);
	bool Check_IMEI_FirstPart(const CString str_imei, const CString str_imeiFirstPart);
	bool Check_IMEI_Difference(const CString str_imei, const CString str_imeiLastWrite, const int i_imeiDiff = 1);
	bool ShowResultDlg(const CString str_errorCode, const CString str_message);
	/*CString ConvertString(const char* sz_buffer);*/
	bool WriteFA(char* sz_result);
	bool CheckImeiPicasso( CString str_imei );
	void LogMsg(CString logContent);
	bool CheckInput();
	bool CheckNAL(char* sz_imei, char* sz_NAL);
	bool QueryNAL(char* sz_imei, char* sz_NAL);
	bool QueryPicassoFromCIM(char* sz_imei, char *sz_model, char* sz_picasso);
private:
	CBrush				       m_brush;                   // brush
	CLabel					   m_label_info;			  // static of information
	CLabel					   m_label_testItem;		  // static of test item
	CLabel					   m_label_ID;			      // static of ID
	CLabel					   m_label_NAL;			      // static of ID
	CLabel                     m_label_Acer22;           // static of Acer ID
	CListCtrl				   m_listctrl_info;           // list control of information
	CListBox				   m_listbox_testItem;        // list box of test item 
	CListBox				   m_listbox_message;         // list box of test message

	HMODULE                    m_h_module;                // DLL handle
	IFacTestToolInterface*     m_p_facTestToolInterface;  // point to IFacTestToolInterface
	std::map<CString,int>      m_map_testItem;            // the Map is for UI to focus on the current test item, reference Event() function
	CString                    m_str_imei;                // IMEI
	CString					   m_str_NAL;                 //NAL
	CString                    m_str_Acer22;              //Acer22
	st_UIIDparameter           m_st_uiParameter;          // information like station, model name, etc.
	//st_IMEIFCParameter         m_st_imeifcParameter;      // information for IMEI station.
	CUnifyUI_FacTestToolDlg*   m_p_dlgParent;             // point to CUnifyUI_FacTestToolDlg
	CResultDlg                 m_dlg_result;              // Result dialog
	CString                    m_str_errorCode;           // errorCode
	CString                    m_str_errorMessage;        // error message
	st_pageReturn              m_st_return;               // set result to the main UI
	CFAOperator                m_obj_faOperator;          // object of CFAOperator

	std::map<CString, CString> m_map_StationIndexMap;



	st_UIControl               m_st_uiControll;           //some ui control parameter
	CWriteLog				   m_obj_log;					//for log
	CFAOperator_new			   m_obj_newFA;				// object of CFAOperator
	bool                       m_b_haveReadedFAFlag;

	std::map<CString,CString>  m_map_keyvalue;            // 
};
