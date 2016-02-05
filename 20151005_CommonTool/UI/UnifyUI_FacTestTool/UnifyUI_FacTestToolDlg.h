
#pragma once

#include "afxcmn.h"
#include "atlimage.h"
#include "InitDlg.h"
#include "ResultDlg.h"
#include "label.h"
#include "afxwin.h"
#include "CommDef.h"
#include "QueryCIM.h"
#include "Communicate.h"
#include "IPowerSupply.h"
#include "WriteLog.h"
#include "Popup.h"
#include "PopUpImg.h"
#include "QueryErrorCode.h"
#include "RelayBoard.h"
//#include "Windows.h"

//#define   ARRAY_SIZE             128

class CUnifyUI_FacTestToolDlg : public CDialog
{
public:
	CUnifyUI_FacTestToolDlg(CWnd* pParent = NULL);
    virtual ~CUnifyUI_FacTestToolDlg();
	virtual void OnCancel();

	enum { IDD = IDD_UNIFYUI_FACTESTTOOL_DIALOG };

	/* Get/Set UI ID functions */
	bool SetUIIDParameter(st_UIIDparameter st_idParameter);
	bool GetUIIDParameter(st_UIIDparameter &st_idParameter);

	/* Get/Set IMEI/FC parameter functions, only for IMEI/FC station use */
// 	bool SetIMEIFCParameter(st_IMEIFCParameter st_stationIMEIFCParameter);
// 	bool GetIMEIFCPrameter(st_IMEIFCParameter &st_stationIMEIFCParameter);

	
	//get ui control parameter
	bool GetUIControlParameter(st_UIControl &st_uiControl);

	/* set CIM PICS data */
	bool SetPICSData(int i_slot, char* sz_key, char* sz_value);

	/* set relayboard */
	bool SetPowerSupply(int i_slot, int i_seq, double dw_vol, double dw_current);

	/* set powersupply */
	bool SetRelayBoard(int i_slot, int i_seq, bool b_onoff);

	/* Query station routing */
	bool QueryStationRoutingInfo(char *p_sz_picasso);

	/*query stationname line index*/
	bool GetStationListMap(std::map<CString, CString> &map_StationVsIndex);

	/*get test item key value*/
	bool GetTestKeyMap(std::map<CString, CString> &map_TestItem);

	/*query line index line index with T A P sequence*/
	//bool GetStationLineIndexMap( std::map<CString, CString> &map_StationVsLineIndex );

	/*insert piscasso control to cim*/
	bool InsertPicassoControlToCIM(char *sz_picasso, char *sz_mobileid);

	/*check piscasso duplicate in cim*/
	bool CheckPicassoDuplicateFromCIM(char *sz_Picasso, char *sz_timeinfoHex);

	/*Query IMEIresult From CIM*/
	bool QueryIMEIresultFromCIM(char *sz_imei, char *modelName, char *sz_Picasso);

	bool QueryIMEIStatusFromCIM(char *sz_imei, char *sz_status);

	/*check picasso state*/
	bool CheckPicassoStat( char * sz_picasso ,char *sz_TimeInfo, char *sz_status);

	CString getErrCode();
	CString getErrMsg();

	bool SendCommand(const BYTE* b_command, DWORD dw_writeLen);

	bool CheckNal(char* sz_imei, char* sz_nal);

	bool QueryNAL(char* sz_imei, char* sz_nal);

	bool QueryPicassoFromCIM( char* sz_imei, char *sz_model,char* sz_picasso);

	bool QueryHold(char *sz_id);

// Implementation
protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support	

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFocus(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTcnSelchangeTabStationpage(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonGo();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonExit();	
	DECLARE_MESSAGE_MAP()

private:
	/* Initialization functions */
	bool InitialProcedure();
	//bool InitialParameter();
	bool InitialUIControl();
	bool InitialWidgetProperty();
	bool InitialUITitle();
	bool InitialTabCtrl();
	bool InitialUIID();
	bool InitialResultCounter();
	bool InitialTimeCounter();
	bool QueryDataFromCIM();
	bool QueryDataFromSQN();

	/* Display or make grayed functions */
	void EnableDisableButton(bool b_flag);
	void EnableDisableEdit(bool b_flag);

	/* Get ID from UI Edit */
	bool GetIDFromUI();

	/* Get Picasso Number from Station page function */
	bool GetPicassoFromStationPage();
	bool GetTagFromStationPage();
	bool GetSnFromStationPage();
	bool PostRun();

	bool SetTestResultToStation(int i_slot, CString str_result);

	/* Check Station routing function */
	bool CheckStationRouting();

	/* Result counter functions */
	bool SetTestRecord(bool b_pass, bool b_CIM);
	bool InsertTestRecordToCIM(bool b_pass);
	/* Accessory insert and check */
	bool InsertAndCheckAccessory(bool &bNeedChange);
	/* Reset UI for another test run */
public:
	bool Begin();
private:
	void ResetUIForNextRun();

	/* Thread function */
	static UINT RunThread(LPVOID pParam);

	/* Result message box function */
	bool ShowResultMsgBox(CString str_errorCode, CString str_message);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	bool CheckPicassoStat( char * sz_picasso ,char *sz_TimeInfo );
	bool QueryRecordInfoFromCIM();
	bool CheckPicassoRule();
	void RecordResultToFile(CString str_picasso, CString str_errorCode, CString str_errorMsg);
	bool SetDeviceOut();
	bool SetDeviceIn();
	bool findXML();
	bool GetDLLIniFile();
	bool Get_SWVersion();

// 	bool SetPowerSupply(int i_psNum,bool b_ONOFF);
// 	bool InitialPowerSupply();
	void LogMsg(CString logContent);

//	bool CheckUserLoadValide();
	bool CheckSoCount(CString str_picasso,CString str_so, CString str_process);
	bool NeedReTest(char *sz_id,char *sz_model, char *sz_process);

//luffy,20111226
	CString getAppVersion();
	bool ExeCmd(CString Command, int i_timeout);
	bool SynTime();
public:
	void ChangeBG();
private:
	CLabel m_static_id;
	CLabel m_static_model;
	CLabel m_static_line;
	CLabel m_static_pcba;
	CLabel m_static_so;
	CLabel m_static_partNumber;
	CLabel m_static_toolVer;
	CLabel m_static_employee;
	CLabel m_static_factoryVer;
	CLabel m_static_userLoadVer;
	CLabel m_static_record;
	CLabel m_static_total;
	CLabel m_static_pass;
	CLabel m_static_fail;
	CLabel m_static_time;
	CLabel m_static_execute;
	CEdit m_edit_model;
	CEdit m_edit_line;
	CEdit m_edit_pcba;
	CEdit m_edit_so;
	CEdit m_edit_partNumber;
	CEdit m_edit_toolVer;
	CEdit m_edit_employee;
	CEdit m_edit_factoryVer;
	CEdit m_edit_userLoadVer;
	CEdit m_edit_total;
	CEdit m_edit_pass;
	CEdit m_edit_fail;
	CEdit m_edit_time;
	CButton  m_button_start;
	CButton  m_button_stop;
	CButton  m_button_exit;
	CTabCtrl m_tab_stationPageCtrl;

	CInitDlg   m_dlg_initDlg;
	CResultDlg m_dlg_resultDlg;

	CFont   m_font_button;
	CBrush  m_brushDiagBk;

	PropertyPageAry m_pageArray;
public:
	std::map<int, CString> m_map_picasso;
	std::map<int, CString> m_map_tag;
	std::map<int, CString> m_map_sn;
	CString	 m_str_appver;
private:
	std::map<CString, CString> m_map_picsData;
	std::map<int, std::map<CString, CString>> m_map_slotPICSData;
	std::map<CString, CString> m_map_StationIndexMap;
	std::map<int,CString>      m_map_invalidPicasso;
	std::map<CString, CString> m_map_TestItem;

	st_UIIDparameter   m_st_idParameter;
	//st_IMEIFCParameter m_st_stationIMEIFCParameter;
	st_pageReturn      m_st_return;
	st_pageReturn      m_st_result;
	st_UIControl       m_st_uiControl;

	CString            m_str_titlePicturePath;
	CString            m_str_computerName;
	CString            m_str_ResultCsvPath;
	unsigned int       m_i_timeCount;
	int  m_i_totalNo;
	int  m_i_passNo;
	int  m_i_failNo;	
	bool m_b_isTesting;
	bool m_b_ChangeGB;
	CString m_str_errorCode;
	CString m_str_errorMsg;

	CCriticalSection m_obj_end;
	CCriticalSection m_obj_run;
	CCriticalSection m_obj_route;
	
	CCommunicate   m_obj_Communicate;
	IPowerSupply   m_obj_ps[PS_NUMBER];
	CQueryCIM	   m_obj_QueryCIM;
	CWriteLog      m_obj_log;
	int			   m_i_today;
	bool		   m_b_syntime;
	CString        m_str_ip;
	int			   m_i_interval;

	CRelayBoard   m_obj_relayBoard;
	int			  m_i_RBNumber;
	bool		  m_b_RBMask;
	int           m_i_cameraNo;//for multi-camera
	CString		  m_str_InsertCIM;//check IMEIUSED
	 
public:
	bool		   m_b_NewErrCode;
	CQueryErrorCode  m_obj_QueryErrCode;
	

	//for cycle test
	int           m_i_testtime;
	afx_msg void OnBnClickedButtonCycletest();
	CString cs_Model_Name;
	CString cs_Station_Name;
	CString 	m_str_iniFileName;
};
