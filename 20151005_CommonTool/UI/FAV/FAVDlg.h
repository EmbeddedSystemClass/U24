#pragma once
#pragma warning(disable:4996)

#include "InitialDlg.h"
#include "Label.h"
#include "MyListCtrl.h"
#include <vector>
#include <map>
#include "atlimage.h"
#include "QueryCIM.h"
#include "CommDef.h"
/*==================================DEFINE=============================================*/
#define CHECK_WIFI                             0
#define PICASSO_LENGTH                         10

#define KEYPARTBARCODE_MAXLENGTH               50
#define KEYPARTBARCODE_MINLENGTH               9

//for list control of accessory index define
#define ITEM_INDEX                             0
#define PN_INDEX                               1
#define DESCRIPTION_INDEX                      2
#define RULE_INDEX                             3
#define KEYPART_INDEX                          4
#define RESULT_INDEX                           5

/* Main UI Background color */
#define BKGCOLOUR_FACTORY		       RGB(215,240,198)
/*===================================================================================*/
// CFAVDlg dialog
class CFAVDlg : public CDialog
{
	// Construction
public:
	CFAVDlg(CWnd* pParent = NULL);	// standard constructor
	~CFAVDlg();
	// Dialog Data
	enum { IDD = IDD_FAV_DIALOG };

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
	afx_msg void OnBnClickedButtonPass();
	afx_msg void OnBnClickedButtonFail();
	afx_msg void OnBnClickedButtonUnlock();
	afx_msg void OnBnClickedButtonLock();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCbnSelendokComboErrorcode();

	/*1.Read IMEI/SOSResult for shipping os
	 *2.Read Picasso in FQC station*/
	afx_msg void OnBnClickedButtonReadIMEI();

public:
	CMyListCtrl m_listResult; // show test result

private:

	/* Thread function */
	static UINT ThreadFunc(LPVOID pParam);

	std::map<CString,CString> m_map_PICSData;          //map pics data
	std::map<CString,int>     m_map_apkTestItemIndex;  //Apk test item vs index of listCtrl
	st_UIIDparameter          m_st_idParameter;        //SO,Station info,etc. from PreDB
 
	CEdit        m_Edit_KeyPartBarcode;   //Scan key part for accessory check
	CEdit        m_Edit_SN;               //IMEI or PICASSO
	CMyListCtrl  m_LC_KeyPartList;        //List accessory for check
	CComboBox    m_comboBox_Errcode;      //error code query from PreDB
	CBrush       m_brushDiagBk;        
	CInitialDlg  dlg_Init;
	int          m_i_keyPartCount;        //accessory count 
	CString      m_strPicasso;         
	CString      m_strErrorCode;
	CString      m_strSW;                 //SW version
	CString      m_strToolVer;

	struct KeyPartInfoArray
	{
		CString strKP_ItemID;
		CString strKP_Type;
		CString strKP_Description;
		CString strKP_ID_Rule;
		CString strKP_Part_Number;
		bool    bExistInSOBOM;
		CString strKPQuantityInSO;
	};
	std::vector <KeyPartInfoArray> m_vKeyPartInfo;

public:

	CString    m_strSerialNo;	    //Id scan or read from phone, may be IMEI or PICASSO
	CQueryCIM  m_obj_QueryCIM;       //object communication with PreDB

	//for factory reset
	bool       m_bResponse;          //wait for factory reset
	bool       m_bFactoryReset;      //result of factory reset

	//UI
	bool InitialUITitle();
	void InsertErrorCodeToUI();
	void InitialUIParameter();
	void ResetForReDO();
	bool InitialResultCounter();
	void InitialFunctionList();
	void ResetKeyPartList();
	void ResetForScanAcessoryBarcode(void);

	//CIM
	bool Init_CIMInfo();
	bool GetKeypartInfo(); 
	void SetPICSData(bool bResult);
	bool InsertTestResultToCIM(bool bResult);
	void SetUIIDParameter(st_UIIDparameter st_idParameter);

	//Input ID
	bool CheckInputID();
	void CheckAndDealWithBarcode(void);  //accessory check
	
	//For shipping OS
	bool ForShipingosToParseSOS(CString &str_Result,CString &str_ErrorCode,CString &str_PhoneImei);
	void ParseApkTestItem();
	void DeleteAPKLog();
	bool PullSOSResultFromPhone();

	//Run command
	bool ExeCmd(CString Command, bool b_isAdbCmd, char* output, int i_timeout = 10000);
    bool ReadPicassoFromPhone(CString & str_picasso);
	bool KillADBServer();
	bool SynTime();
	bool KillResetProcess();

	bool KeyPartIDRuleCompare(CString strKeyPartBarcode, CString strKP_ID_Rule);

};