
// TestToolDlg.h : header file
//

#pragma once

#include "..\Modules\UnifyInterface\FacTestToolInterface.h"
#include "Utils\Label.h"
#include "Utils\WriteLog.h"
#include "ResultDlg.h"
#include "Errorcode.h"
#include "DB.h"


/**********************************************************
Global function and variable for callback notify
***********************************************************/
void NotifyToTestToolDlg (const char* szEvent, long nParam) ;

/* MultiSlot Class */
class CMultiSlot
{
public:
	CMultiSlot(st_UIIDparameter* p_st_uiParameter, int i_slot, IFacTestToolInterface* p_FTTInterface);
	virtual ~CMultiSlot();
	bool DeviceRun(void);
	CWinThread* GetThreadHandle() {return m_p_h_thread;};
	bool GetPicasso();
	bool GetPicassoFromDlg();
	bool GetQCSSNFromdlg();
	bool ReadId(void);

private:
	static UINT MainFunction(LPVOID pParam);
	bool VerifyPicassoAndTimestamp();
	bool IsValidePicasso(char *szPicasso);
	bool IsValideTimestamp(char *szTimestamp);
	void GetTimestampHexStr(char *szTimestampOrig, char *szTimestampNew);
	bool TransferHexToInt_BCD(const int nHex, char* sz);
	bool CreateAndInitPhone(void);

	void FireUIMessage(const char* sz_message);
	void FireUIResult (const char* sz_result);
	void LogMsg(CString logContent);

private:
	st_UIIDparameter*      m_p_st_uiParameter;
	int                    m_i_slot;
	CString                m_str_picasso;
	CString                m_str_QCSSN;
	IFacTestToolInterface* m_p_DeviceFTTInterface;
	CWinThread*            m_p_h_thread;

	CWriteLog              m_obj_log;
	bool                   m_b_runRes;
	char                   m_szFailItem[256];
};

/* Main Dialog Class */
class CTestToolDlg : public CDialog
{
// Construction
public:
	CTestToolDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CTestToolDlg();

// Dialog Data
	enum { IDD = IDD_TESTTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
public:
	void Event(const char* szEvent, long nParam);

protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnEnChangeEditSlot0Picasso();
	afx_msg void OnEnChangeEditSlot0Qcssn();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnGo();
	afx_msg void OnBtnExit();	
	DECLARE_MESSAGE_MAP()

private:
	/* Init */
	bool Init();
	void Begin();
	bool InitialUIControl();
	bool LoadDLL();
	bool SetParameterToDLL();
	bool DLLBegin();
	bool GetParameterFromDLL();
	bool UIShowCOMPort();
	void InitUI();
	void IniRecord();
	void UpdateRecord();
	bool InitTestItemList();
	bool InitRecoverItemList();

	/* Run */
	bool GetPicasso(std::map<int, CString>& map_picasso);
	void ResetResult();
	bool Run();

	/* Result */
	bool reg_FacTestToolInterface_callback();
	bool SetPICSData(int i_slot, char* sz_key, char* sz_value);
	bool SetResult(int i_slot, CString str_result);
	bool InsertPICSDataToDB(int iSlot);
	bool ShowResultDlg(CString str_errorCode, CString str_message);

	/* Others */
	void LogMsg(CString logContent);

public:
	st_UIControl             m_st_uiControl;
	CEdit                    m_edit_devicePicasso[MAX_SLOT];                  // PICASSO
	CEdit                    m_edit_deviceSN;

	CEdit                    m_edit_deviceQCSSN[MAX_SLOT];

	std::map<int, CString>   m_map_Picasso;                                   // Picasso
	std::map<int, std::map<CString, CString>> m_map_slotPICSData;

private:
	CListBox                 m_listbox_testItem;                              // list box of test item 
	CLabel                   m_label_device_title_COM;
	CLabel                   m_label_device_title_Result;
	CLabel                   m_label_device_title_Picasso;
	CLabel                   m_label_deviceCOM[MAX_SLOT];                     // COM Port
	CLabel                   m_label_deviceResult[MAX_SLOT];                  // Result
	CLabel                   m_Label_Title;
	CListBox                 m_listbox_messagepanel;
	CEdit                    m_edit_Record_Total;
	CEdit                    m_edit_Record_Pass;
	CEdit                    m_edit_Record_Fail;
	CButton                  m_bBtnGo;
	CButton                  m_bBtnExit;

	HMODULE                  m_h_module;                                      // DLL handle
	IFacTestToolInterface*   m_p_FTTInterface;                                // Interface pointer
	CResultDlg               m_dlg_result;                                    // Result dialog

	st_UIIDparameter         m_st_uiParameter;                                // UI Parameter

	std::map<int, CString>   m_map_COMPort;                                   // COMPort   (Size: 8)
	std::map<int, CString>   m_map_runResult;                                 // Run result(Size: 1-8)
	std::map<int, CMultiSlot*> m_map_running;                                 // Handset run status(Size: 1-8)
	std::map<CString,int>    m_map_testItem;                                  // the Map is for UI to focus on the current test item, reference Event() function

	int                      m_iRecordTotal;
	int                      m_iRecordPass;
	int                      m_iRecordFail;

	int						 m_iStatus;

	bool                     m_b_sync;
	CCriticalSection         m_obj_UIMessage;
	CCriticalSection         m_obj_UIPicsdata;
	CCriticalSection         m_obj_UIResult;                                  // For UI_RESULT
	CCriticalSection         m_obj_resultCritSection;                         // For run result deal with

	CWriteLog                m_obj_log;
	CDB                      m_obj_db;
};
