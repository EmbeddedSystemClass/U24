#pragma once

#define     DEVICE_NUMBER_OTHER 4

#include <map>
#include "afxcmn.h"
#include "afxwin.h"
#include <Gdiplus.h>
#include <vector>
#include "CommDef.h"
#include "FAOperator.h"
#include "FAOperator_new.h"
#include "..\..\Modules\UnifyInterface\FacTestToolInterface.h"
#include "UnifyUI_FacTestToolDlg.h"
#include "ResultDlg.h"
#include "PageInterface.h"

class COthersMultiDlg : public CPageInterface,public QSYNC_LIB::INotify_Char
{
	DECLARE_DYNAMIC(COthersMultiDlg)

public:
	COthersMultiDlg(CWnd* pParent = NULL);
	virtual ~COthersMultiDlg();
	enum { IDD = IDD_OTHERS_MULTI_STATION };

public:
	virtual void Event(const char* str_event, long nParam);
	bool Run(void);
	bool Stop(void)
	{
		return true;
	}
	bool GetPicasso(std::map<int, CString> &map_picasso);
	bool SetPicasso(std::map<int, CString> map_picasso);

	bool GetTag(std::map<int, CString> &map_Tag){ return true;};
	bool SetTag(std::map<int, CString> map_Tag){ return true;};
	bool GetSn(std::map<int, CString> &map_Sn){ return true;};
	bool SetSn(std::map<int, CString> map_Sn){ return true;};
	void SetTestResult(int i_slot, CString str_testresult);
	bool PostRun();
	bool ResetPage();
	void ChangeGB();
	//bool DisplayResult(CString str_Result);
protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC *pDC);
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//afx_msg void OnCustomdrawList ( NMHDR* pNMHDR, LRESULT* pResult );
	DECLARE_MESSAGE_MAP()

private:
	bool Init(void);
	bool InitUI(void);
	bool LoadDLL(void);
	bool DLLBegin(void);
	void InitMessageList(bool b_multiColumn);
	bool InitTestItemList(void);
	void ClearMessageList(void);
	void AddMessageList(const st_UIResult* p_st_Msg);
	void AddMessage(CString str_info);
	
	void ResetLatestClock(void);
	clock_t GetDurationTimeAndUpdateLatestClock(void);
	bool ShowResultDlg(const CString str_errorCode, const CString str_message);

	bool WriteFA(char* sz_result);
	bool CheckPicassoDuplicate();
	bool ComparePicassoTag();
	bool CheckScan();
	void LogMsg(CString logContent);
	bool SetParameterToDLL(void);
	bool GetParameterFromDLL(void);
	void NotifyMainUI_Other(st_pageReturn st_return);
private:
	CBrush							m_brush;                   // brush
	COLORREF						colour_dialogBKG;		   // dialog background color
	CLabel							m_label_testItem;		   // static of test item
	CLabel							m_label_ID;			       // static of ID
	CLabel							m_label_Prescan;           //for prescan
	CLabel                          m_label_progress[DEVICE_NUMBER_OTHER];
	CListCtrl						m_listctrl_message;        // list control of test message 
	CListBox						m_listbox_testItem;        // list box of test item 
	CListBox                        m_listbox_testItem2;
	CListBox                        m_listbox_testItem3;
	CListBox                        m_listbox_testItem4;
	HMODULE							m_h_module;                // dll handle
	IFacTestToolInterface*			m_p_facTestToolInterface;  // point to IFacTestToolInterface
	clock_t							m_cl_LatestClock;          // for test time
	CString							m_str_picasso;             // picasso number
	CString                         m_str_picasso2;
	CString                         m_str_picasso3;
	CString                         m_str_picasso4;
	CString							m_str_invalidepicasso;     //invalide picasso
	std::map<CString,int>			m_map_testItem;            // the Map is for UI to focus on the current test item, reference Event() function
	std::map<int, CString>          m_map_runResult_other;
	std::map<int, CString>          m_map_picasso; 
	CFAOperator						m_obj_faOperator;          // object of CFAOperator
	CFAOperator_new					m_obj_newFA;				// object of CFAOperator
	st_UIIDparameter				m_st_uiParameter;          // information like station, model name, etc.
	CUnifyUI_FacTestToolDlg*		m_p_dlgParent;             // point to CUnifyUI_FacTestToolDlg
	CResultDlg						m_dlg_result;              // Result dialog
	CString							m_str_errorCode;           // errorCode
	CString							m_str_errorMessage;        // error message
	st_pageReturn					m_st_return;               // set result to the main UI
	//st_IMEIFCParameter              m_st_imeiParameter;        //imei parameter
	st_UIControl                    m_st_uiControl;           //some ui control parameter

	CString                         m_str_MPicasso;
	CString							m_str_MMobileID;

	/* show pattern member variable*/
	Gdiplus::Image*					m_p_Image;
	Gdiplus::GdiplusStartupInput	gdiplusStartupInput;
	ULONG_PTR						gdiplusToken;
	CCriticalSection				m_LockObj;
	CCriticalSection				m_WaitingObj;
	std::vector<CString>			m_AppendImages;
	CString							m_str_ImagePath;

	std::map<CString, CString> m_map_StationIndexMap;

private:
	
	/* show pattern function */
	void SetDisplayImage(const CString& str_DisplayImage);
	bool CheckPicasso();
	bool CheckPicasso_New();
private:
	int            m_i_multiColumn;
	bool		   m_b_needScan;
	CWriteLog      m_obj_log;
	bool           m_b_haveReadedFAFlag;
	bool           m_b_Rework;
	std::map<CString,CString>  m_map_keyvalue;            // 
public:
	afx_msg void OnBnClickedButtonNext();
public:
	void ParsePara(CString str_Input,CString &str_Output,CString &str_Device);
	int	Result_Flag;
	int	Result_Flag2;
	int	Result_Flag3;
	int	Result_Flag4;
	//bool m_PreRun_Flag;
};