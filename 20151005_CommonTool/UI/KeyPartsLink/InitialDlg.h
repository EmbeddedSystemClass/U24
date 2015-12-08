/**********************************************************
*   Copyright (C) 2011 Qisda Corporation
*   All Rights Reserved.
*   Class name : CPdaRec                                                     
*   Summary    : Record management for FA station and QA team
*   Version     Author            Date                Abstract  
*   1.0         Nancy.Yang        2011/05/12          First version
************************************************************
*/
#pragma once
#include "afxwin.h"
#include <vector>

#define EMPLOYEEID_MAXLENGTH               8
#define EMPLOYEEID_MINLENGTH               7

#define SONUMBER_MAXLENGTH                 7
#define SONUMBER_MINLENGTH                 6

// CInitialDlg dialog

class CInitialDlg : public CDialog
{
	DECLARE_DYNAMIC(CInitialDlg)

public:
	CInitialDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInitialDlg();

// Dialog Data
	enum { IDD = IDD_INITIALDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();


	DECLARE_MESSAGE_MAP()
public:
	struct KeyPartInfoArray
	{
		CString strKP_ItemID;
		CString strKP_Type;
		CString strKP_Name;
		CString strKP_ID_Rule;
		CString strKP_ID;
		CString strKP_Part_Number;
		CString strChkDuplic;
		CString strKPQuantityInSO;
		bool bExistInSOBOM;
	};
	CComboBox m_CBModelName;
	CEdit m_Edit_PartNumber;
	CEdit m_Edit_SONumber;
	CEdit m_Edit_LineName;
	CEdit m_Edit_EmployeeID;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	bool m_b_IsUsedEntranceTool;
	CString m_strModelName;
	CString m_strKeyInPartNumber;
	CString m_strPartNumber;
	CString m_strSONumber;
	CString m_strSOType;
	CString m_strSOQty;
	CString m_strSOCRtype;
	CString m_strSegment_QueriedBySO;

	CString m_strLineName;
	CString m_strProcess;
	CString m_strSegment;
	CString m_strPCName;
	CString m_strEmployeeID;
	CString m_strEmployeeIDLevel;
	CString m_strPCName_QueriedByEmployeeID;
	CString m_strEmployeeIDStatus;
	CString m_strKeyPartsInfo;
	CString m_strSyncTimeServerIP;
	std::vector <KeyPartInfoArray> m_vKeyPartInfo;
	std::vector <CString> m_vProcessSelect;
	unsigned KeyPartCount;
	//std::vector <CString> vModelName;
	//std::vector <CString> vPartNumber;
	//std::vector <CString> vSONumber;

	//afx_msg void OnCbnDropdownComboModelname();
	//afx_msg void OnCbnDropdownComboPartnumber();
	//afx_msg void OnCbnDropdownComboSonumber();
	//afx_msg void OnEnSetfocusEditLinename();
	//afx_msg void OnEnSetfocusEditEmployeeid();

	bool CheckRuleConflict(CString strRule1, CString strRule2); // if it exist Conflict, return true;

	void GetWorkStationName(CString & strPCName);
	bool QueryKeyparts_InfoEx(CString strModelName, CString strPartNumber, CString strProcess,std::vector <KeyPartInfoArray>& vKeyPartInfo);
	bool QueryPNinBACH_BySOEx(CString strKeyPartPN,CString strSONumber,CString &strCount);
	bool QuerySegmentProcessEx(CString strModelName, CString strSegment,std::vector <CString> &vProcessSelect);
	bool QueryStation_InfoEx(CString strPCName, CString &strLine,CString &strProcess,CString &strSegment);
	bool QueryEmployeeID_RightEx(CString strEmployId, CString &strEmployeeIDLevel,CString &strStatus,CString &strPCName);
	bool QuerySO_InfoEx(CString strSONumber, CString &strModel, CString &strPN, CString &strSOType,CString &strQty,CString &strSOCRtype,CString &strSegment);
};
