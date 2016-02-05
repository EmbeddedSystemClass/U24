/***************************************************************************************
* Copyright (C) : QCS/WGC18                                      
* Module name	: Download Page  
* File name     : DownloadDlg.h             
* Summary       : Download station.                                                  
* Version	Author         Date          Abstract                      
* 1.0       Alex.Chen      2011/07/06    First version  
****************************************************************************************/

#pragma once

/*======================================== Const ======================================*/
#define     DEVICE_NUMBER               8
#define     DOWNLOAD_IMAG_PATH          _T("C:\\Image")
#define     PARAMETER_ONE               "COM"
#define VMS_DBACCESS				_T("VMS_DBACCESS.dll")

//#define     WM_USER_SETRESLUTNOTIFY     WM_USER + 210
/*======================================== Const ======================================*/

#include "afxcmn.h"
#include "afxwin.h"
#include <dbt.h>
#include <initguid.h>
#include <SetupAPI.h>
#include "..\..\Modules\UnifyInterface\FacTestToolInterface.h"
#include "..\..\lib\WinDDK\inc\wxp\usbioctl.h"
#include "Label.h"
#include "CommDef.h"
#include "PageInterface.h"
#include "UnifyUI_FacTestToolDlg.h"
#include "ResultDlg.h"
#include <Cfgmgr32.h>

#include "FAOperator.h"
#include "FAOperator_new.h"

#pragma warning(disable:4996)

typedef struct
{
	char   chHandleName  [MAX_PATH];
	char   chInstanceName[MAX_PATH];
	char   chFriendlyName[MAX_PATH];
	char   chDeviceDescr [MAX_PATH];
	char   chParentName  [MAX_PATH];
	char   chSerialNumber[MAX_PATH];
	int    nRealPort;
	DWORD  dwDeviceNumber;

} USBDEVICE_STRUCT;


/* Headset DL Class */
class CHandsetDL
{
public:
	CHandsetDL(st_UIIDparameter* p_st_uiParameter, int i_slot, CString str_picasso, IFacTestToolInterface* p_DLInterface, std::map<CString, CString> map_StationIndexMap, st_UIControl* p_st_uiControl);
	virtual ~CHandsetDL();
	bool HandsetRun(void);
	CWinThread* GetThreadHandle() {return m_p_h_thread;};
	
public:
	//static CCriticalSection  m_obj_CritSection;          
private:
	static UINT DLRun(LPVOID pParam);            // PreDL and ReDL
	
	void   FireUIMessage(const char* sz_message);
	void   FireUIResult (const char* sz_result);
	void   FireUIPicasso(const char* sz_picasso);
	bool   ReadFA(char* sz_picasso=NULL);
	bool   WriteFA(char* sz_errCode);
	bool   RestoreFA();
	bool   InitialFA();
	bool   InitialFA_new();
	bool InSertCIMPicassoControl();
	bool CheckPicassoDuplicate(char *sz_errCode);
	bool InsertPicassoControl(char *sz_errCode);
	void LogMsg(CString logContent);
	bool CheckRouting(char *sz_picasso,char *sz_errCode);

	void   FireRelayControl(bool bOn);
private:
	st_UIIDparameter*      m_p_st_uiParameter;
	int                    m_i_slot;
	CString                m_str_picasso;
	CString                m_str_mobileid;
	IFacTestToolInterface* m_p_handsetDLInterface;
	CWinThread*            m_p_h_thread;

	std::map<CString, CString> m_map_StationIndexMap;
	std::map<CString, CString> m_map_StationVsDll;

	CFAOperator				 m_obj_FA;
	CFAOperator_new          m_obj_newFA;
	bool                     m_b_Rework;
	CQueryCIM                m_obj_QueryCIM;
	st_UIControl*            m_p_st_uiControl;
	CWriteLog				 m_obj_log;
	bool                     m_b_runRes;
};

/* Main Dialog Class */
class CDownLoadDlg : public CPageInterface, public QSYNC_LIB::INotify_Char
{
	DECLARE_DYNAMIC(CDownLoadDlg)

public:
	CDownLoadDlg(CWnd* pParent = NULL);
	virtual ~CDownLoadDlg();

	enum { IDD = IDD_DOWNLOAD_STATION };

public:

	virtual void Event(const char* str_event, long nParam);

	bool Run(void);
	bool Stop(void);
	bool GetPicasso(std::map<int, CString>& map_picasso);
	bool GetPicassoForOS_DL_AutoRun(std::map<int, CString>& map_picasso,int i_slot);
	bool SetPicasso(std::map<int, CString> map_picasso);
	bool GetTag(std::map<int, CString> &map_Tag){ return true;};
	bool SetTag(std::map<int, CString> map_Tag){ return true;};
	bool GetSn(std::map<int, CString> &map_Sn){ return true;};
	bool SetSn(std::map<int, CString> map_Sn){ return true;};
	bool ResetPage(void);
	void SetTestResult(int i_slot, CString str_testresult);
	void ChangeGB();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

private:
	bool Init(void);
	bool LoadDLL(void);
	bool GetSWVersion(void);
	bool DLLBegin(void);
	void InitUIShow(void);
	bool UIShowCOMPort(void);
	//bool UIShowImageFile(void);
	void SetMessage(CString str_info);
	void SetProgress(int i_slot, int i_mainProgress, int i_subProgress);
	//LRESULT SetResult(WPARAM wParam, LPARAM lParam);
	bool SetResult(int i_slot, CString str_result);
	bool ShowResultDlg(CString str_errorCode, CString str_message);
	void NotifyMainUI(st_pageReturn st_return);
	void LogMsg(CString logContent);
	bool SetParameterToDLL(void);
	bool GetParameterFromDLL(void);
	bool PostRun(){return true;}
	void DeviceDetectSetup();//RegisterDeviceNotification
	BOOL OnDeviceChange(UINT nEventType,DWORD_PTR dwData);
	void GetDevice(PDEV_BROADCAST_DEVICEINTERFACE lpbv,UINT nEventType);
	void ParserDBCCName(TCHAR dbcc_name[1],CString &str_vendor,CString &str_product,CString &str_para);
	bool IsValidVIDPID(CString str_vendor,CString str_product);

	char*   sz_id;
	int FindADBdevice();
	USBDEVICE_STRUCT devStruct;
private:
	CBrush                   m_brush;                                         // Brush
	CFont                    m_font;                                          // Font
 
	CLabel                   m_label_swPath;								  // SW Image Path static frame
	CLabel                   m_label_deviceCOM[DEVICE_NUMBER];                // COM Port
	CLabel                   m_label_progress[DEVICE_NUMBER];                 // Progress value: 0% - 100%
	CProgressCtrl            m_processCtrl_subProgress[DEVICE_NUMBER];        // Sub progress
	CProgressCtrl            m_processCtrl_mainProgress[DEVICE_NUMBER];       // Main progress
	CEdit                    m_edit_picasso[DEVICE_NUMBER];                   // Picasso
	CListCtrl                m_listctrl_swPath;                               // SW Image Path
	CListBox                 m_listbox_message;                               // UI Message

	HMODULE                  m_h_module;                                      // DLL handle
	IFacTestToolInterface*   m_p_DLInterface;                                 // Interface pointer
	CUnifyUI_FacTestToolDlg* m_p_dlgParent;                                   // CUnifyUI_FacTestToolDlg
	CResultDlg               m_dlg_result;                                    // Result dialog

	CCriticalSection         m_obj_messageCritSection;                        // For CListCtrl
	CCriticalSection         m_obj_resultCritSection;                         // For run result deal with
	CString                  m_str_swFilePath;                                // SW Path
	st_UIIDparameter         m_st_uiParameter;                                // UI Parameter

	std::map<int, CString>   m_map_COMPort;                                   // COMPort   (Size: 8)
	std::map<int, CString>   m_map_picasso;                                   // Picasso   (Size: 1-8)
	std::map<int, CString>   m_map_runResult;                                 // Run result(Size: 1-8)
	std::map<int, CHandsetDL*> m_map_running;                                 // Handset run status(Size: 1-8)

	st_pageReturn            m_st_return;
	bool                     m_b_sync;
	CCriticalSection         m_obj_UIResult;                                  // For UI_RESULT
	CCriticalSection         m_obj_UIPicasso;
	CCriticalSection         m_obj_UIRelayBoard;
	CCriticalSection         m_obj_UIMessage;
	CCriticalSection         m_obj_UIProcess;
	CCriticalSection         m_obj_UIPicsdata;
	CCriticalSection         m_obj_UIConnect;
 
	std::map<CString, CString> m_map_StationIndexMap;
	st_UIControl             m_st_uiControl;
	CWriteLog				 m_obj_log;
	std::map<CString,CString>  m_map_keyvalue;
	bool                     m_b_returnRes[10];  // means dll has returned result for auto run 
	CString                  m_str_VIDPID;
};
