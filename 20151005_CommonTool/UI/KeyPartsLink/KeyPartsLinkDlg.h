/**********************************************************
*   Copyright (C) 2011 Qisda Corporation
*   All Rights Reserved.
*   Class name : CPdaRec                                                     
*   Summary    : Record management for FA station and QA team
*   Version     Author            Date                Abstract  
*   1.0         Nancy.Yang        2011/05/12          First version
************************************************************
*/

// KeyPartsLinkDlg.h : header file
//

#pragma once

#include "InitialDlg.h"
#include "OCA_Para_Setting.h"
#include "afxwin.h"
#include "Label.h"
#include "MyListCtrl.h"
#include "afxcmn.h"
#include <vector>
#import ".\SSS\SSS_O2.dll"

#define TOOL_VER                               "V2.022"
#define PICASSO_LENGTH                         10
#define PPID_LENGTH                            23
#define IMEI_LENGTH                            15
#define SERVICETAG_LENGTH                      7
#define KEYPARTBARCODE_MAXLENGTH               50
#define KEYPARTBARCODE_MINLENGTH               8
#define KEYPARTBARCODE_MINLENGTH_WITHSN        8
#define ITEM_INDEX                             0
#define PN_INDEX                               1
#define KPNAME_INDEX                           2
#define RULE_INDEX                             3
#define CHKDUPLIC_INDEX                        4
#define KEYPART_INDEX                          5
#define RESULT_INDEX                           6
#define GRAY_NA                                114
#define GRAY_MUTEX                             192




// CKeyPartsLinkDlg dialog
class CKeyPartsLinkDlg : public CDialog
{
// Construction
public:
	CKeyPartsLinkDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_KeyPartsLink_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLock();
	afx_msg void OnBnClickedButtonEditOCAPara();
	afx_msg void OnBnClickedButtonInquiry();
	afx_msg void OnBnClickedButtonUnlink();
	afx_msg void OnBnClickedCheckUnLink();

	//CLabel m_LTitle;
	CLabel m_LShowPass;
	CComboBox m_CBProcessSelect;
	CComboBox m_CBIndexSelect;
	CEdit m_Edit_SN;
	CEdit m_Edit_KeyPartBarcode;
	CMyListCtrl m_LC_KeyPartList;
	CMyListCtrl m_LC_Result;
	CMyListCtrl m_UN_Link;
	CMyListCtrl m_Link_Text;
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CInitialDlg m_InitialDlg;
	COCA_Para_Setting m_OCA_Para_Setting;
	CString m_strProcessSelect;
	CString m_strKeyPartBarcode;
	CString m_strSerialNo;
	CString m_strPicassoNumber;
	CString m_strSOCount;

	CString m_strLogFileName;

	CString m_str_PCBA_barcode;
	char m_sz_Process[32];
	bool b_DoRemove;
	//std::vector <CString> vKeyPart;
	//std::vector <CString> vAccessoriesScanned;
	//std::vector <CString> m_vProcessSelect;

	//int m_Result_Count;
	int m_KeyPartList_ColumnsCount;
	
	int m_BatteryCount_InBACH;
	int m_BatteryCount_Scanned;
	int m_BatteryCount_Scanned_PassRunIn;

	SSS_O2::_clsO2Check *m_pITS_O2;

	//bool TransferBarcodeToPartNumber(void);
	bool KeyPartIDRuleCompare(CString strKeyPartBarcode, CString strKP_ID_Rule);
	bool SetValueAndInsertPASSData(void);
	bool CheckRoutingCurrentStation(void);
	bool CheckRoutingNextStation(void);
	bool CheckRoutingEx(CString strPicassoNumber,CString strModelName,CString strProcess);
	bool CheckSNBarCodeAndGetPicasso(void);

	void ResetForScanAcessoryBarcode(void);
	void ResetForReDO(void);
	void CheckAndDealWithKeyPartBarcode(void);
	void ExecuteOperationsIfCompleted (void);
	afx_msg void OnCbnSelendokComboProcess();

	//****For Wolf key part link	Dave.Wu 20111007
	bool CheckBatteryAndCharger();
	//****Dave.Wu 20111007
	bool DoPrintTravelLabel(void);
	bool GenerateAndPrintPCBALabel(void);
	bool CheckKeyparts_DuplicateEx(CString strPicassoID, CString strKeyPartID/*, CString strKeyPartName,CString strModelName*/);
	bool UpdateEmployeeID_StatusEx(CString strEmployId, CString strStatus,CString strPCName);
	bool InsertKeypartsInfoEx(CString strPicasso, CString strProcess,CString strKeyPartId,CString strKeyPartName,CString strEmployId,CString strKeyPartPN);
	bool QuerySOCountEx(CString strId, CString strSONumber,CString strProcess,CString &strSOCount);
	bool QueryholdInfoEx(CString strPicassoNumber,CString strModelName,CString strProcess);
	bool QueryCRHoldInfoEx(CString strPicassoNumber);
	bool CheckSOEx(CString strSo,CString strSoCRtype,CString strPicasso,CString strModel,CString strProcess,CString strSegment);
	bool CheckProcessTestDateEx(CString strPicassoNumber,CString strModelName,CString strSegment,CString strProcess);
	bool QueryIMEI_ByPicassoEx(CString strPicassoid ,CString &strIMEI);
	bool QueryIMEIStatusEx(CString strIMEI,CString &strStatus,CString &strCartonID);
	bool GetPicassoEx(CString strId, CString strModelName, CString &strPicasso,CString &strPartNO);
	bool QueryKeypartsByIDEx(CString strId, CString strProcess, std::vector <CInitialDlg::KeyPartInfoArray> &vKeyPartInfo);
	bool CheckKPsStatusInPTKPWIP();
	bool CheckPTKPWIP_BeforeScan();
	bool CheckPTKPWIP_BeforeInsertPTKPWIP(CString strPicasso, CString strProcess,CString strKeyPartId,CString strKeyPartName,CString strEmployId,CString strKeyPartPN);
	bool Query_PCBA_SerialNOEx(CString str_Picasso , CString str_AcerMB , CString str_YWW,CString str_Employee,CString &str_Serial);
	bool Query_PCBA_NOEx(CString str_Picasso , CString strProcess, CString &str_PCBAno );
	bool QueryNALEx(CString str_IMEI , CString &str_NAL);
	bool QueryIMEIbyNALEx(CString str_NAL, CString &str_IMEI);
	bool CheckNAL_ValidityEx ( CString str_PartNO, CString str_NAL);
	bool Initial_SSSO2Dll( );
	bool SSSO2_UpdateNALEx(CString str_IMEI ,CString str_NAL);
	bool CheckBadge_ValidityEx(CString str_Badge , CString str_PartNO);
	bool InitialUITitle();
	bool SyncTime(CString strServerIP);
	int  GetWeekID(unsigned int year,unsigned int month,unsigned int day,CString &str_Year,CString &str_WeekId);
	void ConvertModelName( CString strModelName,CString &strModelName_Normal);
};