// InitialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KeyPartsLink.h"
#include "InitialDlg.h"


// CInitialDlg dialog

IMPLEMENT_DYNAMIC(CInitialDlg, CDialog)

CInitialDlg::CInitialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInitialDlg::IDD, pParent)
{
	m_strSyncTimeServerIP = _T("10.85.68.13");
	m_b_IsUsedEntranceTool = false;
}

CInitialDlg::~CInitialDlg()
{
}

void CInitialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MODELNAME, m_CBModelName);
	DDX_Control(pDX, IDC_EDIT_PARTNUMBER, m_Edit_PartNumber);
	DDX_Control(pDX, IDC_EDIT_SONUMBER, m_Edit_SONumber);
	DDX_Control(pDX, IDC_EDIT_LINENAME, m_Edit_LineName);
	DDX_Control(pDX, IDC_EDIT_EMPLOYEEID, m_Edit_EmployeeID);
}


BEGIN_MESSAGE_MAP(CInitialDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CInitialDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CInitialDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CInitialDlg message handlers

BOOL CInitialDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	if(RUN_MODE_OFFLINE == true)
	{
		m_CBModelName.EnableWindow(true);
		m_CBModelName.AddString(_T("GDMTB1A"));
		m_CBModelName.AddString(_T("GSAPB1A"));
		m_CBModelName.SelectString(0,_T("GDMTB1A"));

		m_Edit_PartNumber.EnableWindow(true);
		m_Edit_PartNumber.SetLimitText(12);
		m_Edit_PartNumber.SetWindowTextW(_T("XX.XXXXX.XXX"));

		m_Edit_LineName.EnableWindow(true);
		m_Edit_LineName.SetLimitText(6);
		m_Edit_LineName.SetWindowTextW(_T("F0XA0X"));

		m_Edit_SONumber.SetLimitText(SONUMBER_MAXLENGTH);
		m_Edit_SONumber.SetWindowTextW(_T("000000"));
		m_Edit_EmployeeID.SetLimitText(EMPLOYEEID_MAXLENGTH);
		m_Edit_EmployeeID.SetWindowTextW(_T("1234567"));

		GetWorkStationName(m_strPCName);
		m_strLineName = _T("OCA");
		m_strProcess  = _T("OCA");
		m_strSegment  = _T("OCA");
		m_CBModelName.SetFocus();

		return FALSE;  // return TRUE unless you set the focus to a control
	}

	if (m_b_IsUsedEntranceTool == false)
	{
		m_Edit_EmployeeID.SetLimitText(EMPLOYEEID_MAXLENGTH);
		m_Edit_PartNumber.SetLimitText(12);
		m_Edit_SONumber.SetLimitText(SONUMBER_MAXLENGTH);

		m_Edit_SONumber.SetFocus();

		return FALSE;  // return TRUE unless you set the focus to a control
	}
	else
	{
		m_Edit_EmployeeID.SetWindowText(m_strEmployeeID);
		m_Edit_SONumber.SetWindowText(m_strSONumber);
		OnBnClickedOk();
		return FALSE;
	}
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CInitialDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	if(RUN_MODE_OFFLINE == true)
	{
		m_CBModelName.GetWindowTextW(m_strModelName);
		m_Edit_PartNumber.GetWindowTextW(m_strPartNumber);
		m_Edit_LineName.GetWindowTextW(m_strLineName);
		m_Edit_SONumber.GetWindowTextW(m_strSONumber);
		m_Edit_EmployeeID.GetWindowTextW(m_strEmployeeID);

		if( m_strPartNumber.GetLength() != 12)
		{
			AfxMessageBox(_T("Input Part Number Length Error!"));
			return;
		}

		for(int i = 0;i < m_strPartNumber.GetLength();i++)
		{
			if(( i ==2 || i ==8 ) && m_strPartNumber.GetAt(2) == '.')
				continue;
			if(m_strPartNumber.GetAt(i) >= 'a' && m_strPartNumber.GetAt(i) <= 'z')
				continue;
			if(m_strPartNumber.GetAt(i) >= '0' && m_strPartNumber.GetAt(i) <= '9')
				continue;
			if(m_strPartNumber.GetAt(i) >= 'A' && m_strPartNumber.GetAt(i) <= 'Z')
				continue;
			AfxMessageBox(_T("Input Part Number Error!"));
			return;
		}

		if(m_strSONumber.GetLength() < SONUMBER_MINLENGTH ||m_strSONumber.GetLength()  > SONUMBER_MAXLENGTH)
		{
			AfxMessageBox(_T("Input SO Error!"));
			return;
		}
		for(int i = 0;i < m_strSONumber.GetLength();i++)
		{
			if(m_strSONumber.GetAt(i) >= '0' && m_strSONumber.GetAt(i) <= '9')
				continue;
			AfxMessageBox(_T("Input SO Error!"));
			return;
		}

		OnOK();
		return;
	}

	m_Edit_SONumber.GetWindowTextW(m_strSONumber);
	m_Edit_EmployeeID.GetWindowTextW(m_strEmployeeID);

	bool DBresult = theApp.pConnectDB();
	if(!DBresult)
	{
		AfxMessageBox(_T("ConnectDB fail!"));
		if (m_b_IsUsedEntranceTool == false) 
			return;
		else
			exit(0);
	}

	if(m_strSONumber.GetLength() < SONUMBER_MINLENGTH ||m_strSONumber.GetLength()  > SONUMBER_MAXLENGTH)
	{
		AfxMessageBox(_T("SO Number Length is not correct!"));
		if (m_b_IsUsedEntranceTool == false) 
			return;
		else
			exit(0);
	}
	if(m_strEmployeeID.GetLength() < EMPLOYEEID_MINLENGTH || m_strEmployeeID.GetLength() > EMPLOYEEID_MAXLENGTH)
	{
		AfxMessageBox(_T("Employee ID Length is not correct!"));
		if (m_b_IsUsedEntranceTool == false) 
			return;
		else
			exit(0);
	}

	//  QueryEmployeeID_RightEx
	if(!QueryEmployeeID_RightEx(m_strEmployeeID,m_strEmployeeIDLevel,m_strEmployeeIDStatus,m_strPCName_QueriedByEmployeeID))
		return;

	//QuerySO_Info
	if(!QuerySO_InfoEx(m_strSONumber, m_strModelName, m_strPartNumber, m_strSOType,m_strSOQty,m_strSOCRtype,m_strSegment_QueriedBySO))
		return;

	//Check SO Segment
	if (m_strPartNumber.Mid(0,3) != _T("9J.") && 
		m_strPartNumber.Mid(0,3) != _T("9B.") && 
		m_strPartNumber.Mid(0,3) != _T("CS.") && 
		m_strPartNumber.Mid(0,3) != _T("5G."))
	{
		AfxMessageBox(_T("PN = ") + m_strPartNumber + _T(" Error!\n 5G.* for FA; CS.* or 9J.* or or 9B.* for PACKAGE!"));
		return;
	}

	//Employee ID level control
	if (m_strEmployeeIDLevel !=_T("20")) //FULL
	{
		if (m_strPartNumber.Mid(0,3) == _T("5G.") && m_strEmployeeIDLevel !=_T("22"))
		{
			AfxMessageBox(_T("The right of Employ ID:" + m_strEmployeeID + " is not enough! Level = " + m_strEmployeeIDLevel + ".Please Call Production line Leader"));
			if (m_b_IsUsedEntranceTool == false) 
				return; 
			else 
				exit(0);
		}
		if (( m_strPartNumber.Mid(0,3) == _T("9J.") || 
			  m_strPartNumber.Mid(0,3) == _T("9B.") || 
			  m_strPartNumber.Mid(0,3) == _T("CS.") 
			) 
			&& m_strEmployeeIDLevel !=_T("23"))
		{
			AfxMessageBox(_T("The right of Employ ID:" + m_strEmployeeID + " is not enough! Level = " + m_strEmployeeIDLevel + ".Please Call Production line Leader"));
			if (m_b_IsUsedEntranceTool == false) 
				return; 
			else 
				exit(0);
		}
	}

	//QueryStation_Info
	GetWorkStationName(m_strPCName);
	//Add Lennon PC Name for Test Use
	if (m_strPCName == _T("O-30B3")||m_strPCName == _T("O-35A4"))
	{
		if (m_strPartNumber.Mid(0,3) == _T("5G."))
		{
			//m_strPCName = _T("F03F01"); // For FAPRINT
			m_strPCName = _T("F05L01"); // For FALINK
			//m_strPCName = _T("F31L02"); // For FALINK2
		}
		if (( m_strPartNumber.Mid(0,3) == _T("9J.") || 
			  m_strPartNumber.Mid(0,3) == _T("9B.") || 
			  m_strPartNumber.Mid(0,3) == _T("CS.") ))
		{
			m_strPCName = _T("P05C01");  //For PPIDLINK
			//m_strPCName = _T("F04P02"); // For CSD_PRINT
			//m_strPCName = _T("P04L03"); // For CSD_LINK
		}
		m_strSyncTimeServerIP = _T("10.85.17.140");
	}
	if(!QueryStation_InfoEx( m_strPCName, m_strLineName,m_strProcess,m_strSegment))
		return;

	/* if ( m_strSegment != m_strSegment_QueriedBySO)
	{
		AfxMessageBox(_T("The WorkStation:") + m_strPCName +  _T(" is set in Segment:") + m_strSegment 
			+ _T("\nBut the SO:") + m_strSONumber + _T(" is created for Segment:") + m_strSegment_QueriedBySO );
		return;
	} */

	if ( m_b_IsUsedEntranceTool == false && 
		m_strEmployeeIDStatus != _T("Y") &&
		m_strPCName != m_strPCName_QueriedByEmployeeID )
	{
		AfxMessageBox(_T("The used/linked WorkStation of this Employee ") + m_strEmployeeID + _T("(Status=")  + m_strEmployeeIDStatus
			+ _T(") is ") + m_strPCName_QueriedByEmployeeID + _T("\n Please call Line Leader for help!"));
		return;
	}

	// Query All Processes in Segement According Segment + Model 
	if(!QuerySegmentProcessEx( m_strModelName, m_strSegment,m_vProcessSelect))
		return;

	// QueryKeyparts_Info
	std::vector <KeyPartInfoArray> vKeyPartInfo;
	if( m_strProcess.Find( _T("FAPRINT"),0) != -1 || m_strProcess.Find( _T("CSD_PRINT"),0) != -1 )
	{
		for(unsigned int i = 0; i < m_vProcessSelect.size();i++)
		{
			CString strProcess = m_vProcessSelect[i];
			std::vector <KeyPartInfoArray> vKeyPartInfo_temp;
			if ( strProcess.Find(_T("FALINK"),0) != -1 || strProcess.Find(_T("CSD_LINK"),0) != -1)
			{
				if(!QueryKeyparts_InfoEx( m_strModelName, m_strPartNumber, strProcess, vKeyPartInfo_temp))
					return;
				for(unsigned int j = 0; j < vKeyPartInfo_temp.size();j++)
					vKeyPartInfo.push_back(vKeyPartInfo_temp[j]);
			}
		}
	}
	else
	{
		if(!QueryKeyparts_InfoEx( m_strModelName, m_strPartNumber, m_strProcess, vKeyPartInfo))
			return;
	}

	//Check The KeyPart Part Number if exist in BACH SO BOM and Quantity
	//int ActualKeyPartCountInBACH = 0;
	for(unsigned int i = 0; i< vKeyPartInfo.size();i++)
	{
		vKeyPartInfo[i].bExistInSOBOM = true;
		vKeyPartInfo[i].strKPQuantityInSO = _T("");
		CString strCount;
		if (QueryPNinBACH_BySOEx(vKeyPartInfo[i].strKP_Part_Number,m_strSONumber,strCount))
			vKeyPartInfo[i].strKPQuantityInSO = strCount;
		else
			vKeyPartInfo[i].bExistInSOBOM = false;
	}
	//if Quantity = 0 and there is another kp with same kptype, the Quantity = 0 one will not be inlcuded. Modified by Lennon 20120118
	for(unsigned int i = 0; i< vKeyPartInfo.size();i++)
	{
		if (vKeyPartInfo[i].strKPQuantityInSO == _T("0"))
		{
				for(unsigned int j = 0; j< vKeyPartInfo.size();j++)
				{
					if (vKeyPartInfo[j].bExistInSOBOM == false)
						continue;
					if(j == i)
						continue;
					if((vKeyPartInfo[j].strKP_Type == vKeyPartInfo[i].strKP_Type) && vKeyPartInfo[j].strKPQuantityInSO!=_T("0"))
					{
						vKeyPartInfo[i].bExistInSOBOM = false;
						break;
					}
				}
		}
	}

	//Check conflicts among all KP_ID_Rules. Add by Lennon 20140326
	for(unsigned int i = 0; i< vKeyPartInfo.size();i++)
	{
		if (vKeyPartInfo[i].bExistInSOBOM == true)
		{
				for(unsigned int j = 0; j< vKeyPartInfo.size();j++)
				{
					if (vKeyPartInfo[j].bExistInSOBOM == false)
						continue;
					if(j == i)
						continue;
					if( vKeyPartInfo[j].strKP_Part_Number == vKeyPartInfo[i].strKP_Part_Number )
						continue;
					if(CheckRuleConflict( vKeyPartInfo[j].strKP_ID_Rule,vKeyPartInfo[i].strKP_ID_Rule))
					{
						CString StrMsg;
						StrMsg.Format(_T("The KPID rule conflicts between \n\n %s:%s:%s \n\n %s:%s:%s \n\n Please call Line Leader for help!"),
							vKeyPartInfo[j].strKP_Name ,vKeyPartInfo[j].strKP_Part_Number , vKeyPartInfo[j].strKP_ID_Rule,
							vKeyPartInfo[i].strKP_Name ,vKeyPartInfo[i].strKP_Part_Number , vKeyPartInfo[i].strKP_ID_Rule );
						AfxMessageBox(StrMsg);
						return;
					}
				}
		}
	}

	//Sort the kp and copy to m_vKeyPartInfo
	int ActualKeyPartCountInBACH = 0;
	if ( m_strProcess.Find(_T("FAPRINT"),0) == -1 && m_strProcess.Find(_T("CSD_PRINT"),0) == -1 ) 
	{
		for(unsigned int i = 0; i< vKeyPartInfo.size();i++)
		{
			if (true == vKeyPartInfo[i].bExistInSOBOM)
			{
				m_vKeyPartInfo.insert(m_vKeyPartInfo.begin() + ActualKeyPartCountInBACH,vKeyPartInfo[i]);
				ActualKeyPartCountInBACH++;
			}
			else
			{
				m_vKeyPartInfo.push_back(vKeyPartInfo[i]);
			}
		}
	}

	if ( m_strProcess.Find(_T("FAPRINT"),0) != -1 ||  m_strProcess.Find(_T("CSD_PRINT"),0) != -1)
	{
		for(unsigned int i = 0; i< vKeyPartInfo.size();i++)
		{
			if (vKeyPartInfo[i].strKP_Name == _T("MB"))
			{
				if (true == vKeyPartInfo[i].bExistInSOBOM)
				{
					m_vKeyPartInfo.insert(m_vKeyPartInfo.begin() + ActualKeyPartCountInBACH,vKeyPartInfo[i]);
					ActualKeyPartCountInBACH++;
				}
				/*else
				{
					m_vKeyPartInfo.push_back(vKeyPartInfo[i]);
				}*/
			}
		}
		if (ActualKeyPartCountInBACH == 0)
		{
			AfxMessageBox(_T("Please call line leader! Not Found PCBA Print info in KPPCS or BACHS"));
			return;
		}
		if (ActualKeyPartCountInBACH > 1)
		{
			AfxMessageBox( _T("Please call line leader! Multi PCBA Print info in KPPCS or BACHS"));
			return;
		}
	}
	//theApp.pCloseDB();
	OnOK();
}

bool CInitialDlg::QueryKeyparts_InfoEx(CString strModelName, CString strPartNumber, CString strProcess,std::vector <KeyPartInfoArray>& vKeyPartInfo)
{
	bool DBresult = false;
	char sz_Model[32]={0};
	char sz_PN[32]={0};
	char sz_Process[32]={0};

	char sz_Info[8192]={0};
	char sz_Count[32]={0};
	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};

	int IndexID = 0;
	int nStart = 0;
	int nSubStart = 0;
	int nPos = 0;
	int nSubPos = 0;
	int ActualKeyPartCount = 0;
	int KeyPartCount = 0;

	CString delims = _T(";");
	CString Subdelims = _T(",");

	CString OneKeyPartInfo;
	CString strKeyPartsInfo;

	KeyPartInfoArray tempstrArray;

	WideCharToMultiByte(CP_ACP,NULL,strModelName.GetBuffer(), -1,sz_Model, strModelName.GetLength()*3, NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strPartNumber.GetBuffer(), -1,sz_PN, strPartNumber.GetLength()*3, NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strProcess.GetBuffer(), -1,sz_Process, strProcess.GetLength()*3, NULL, NULL);
	// QueryKeyparts_Info
	//QueryKeyparts_Info by sz_Model + sz_Model + sz_Process
	DBresult = theApp.pQueryKeyparts_Info(sz_Model, sz_Model,sz_Process,sz_Count, sz_Info, sz_Errorcode, sz_ErrMsg);
	if (DBresult)
	{
		CString strTemp;
		theApp.MultiByteToWideCharEnhanced(sz_Info,strTemp);
		strKeyPartsInfo = strKeyPartsInfo + strTemp;
		KeyPartCount = atoi(sz_Count);
	}
	else
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pQueryKeyparts_Info! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}
	//QueryKeyparts_Info by sz_PN + sz_Model + sz_Process
	sz_Info[0] = 0;
	sz_Count[0] = 0;
	DBresult = theApp.pQueryKeyparts_Info(sz_PN, sz_Model,sz_Process,sz_Count, sz_Info, sz_Errorcode, sz_ErrMsg);
	if (DBresult)
	{
		CString strTemp;
		theApp.MultiByteToWideCharEnhanced(sz_Info,strTemp);
		strKeyPartsInfo = strKeyPartsInfo + strTemp;
		KeyPartCount = KeyPartCount + atoi(sz_Count);
	}
	else
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pQueryKeyparts_Info! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}

	// Analyze and Deal with strKeyPartsInfo
	if (strKeyPartsInfo == _T(""))
	{
		AfxMessageBox(_T("pQueryKeyparts_Info -> No Keyparts Found! "));
		return false;
	}
	nStart = 0;
	nSubStart = 0;
	nPos = 0;
	nSubPos = 0;
	ActualKeyPartCount = 0;
	IndexID = 0;
	while (strKeyPartsInfo.Find(delims,nStart) > 0 || nStart < strKeyPartsInfo.GetLength())
	{
		if (strKeyPartsInfo.Find(delims,nStart) > 0)
			nPos = strKeyPartsInfo.Find(delims,nStart);
		else
			nPos = strKeyPartsInfo.GetLength();
		OneKeyPartInfo = strKeyPartsInfo.Mid(nStart,nPos-nStart);
		while (OneKeyPartInfo.Find(Subdelims,nSubStart) > 0 || nSubStart < OneKeyPartInfo.GetLength())
		{
			if (OneKeyPartInfo.Find(Subdelims,nSubStart) > 0)
				nSubPos = OneKeyPartInfo.Find(Subdelims,nSubStart);
			else
				nSubPos = OneKeyPartInfo.GetLength();
			CString strTemp = OneKeyPartInfo.Mid(nSubStart,nSubPos-nSubStart);
			strTemp.TrimLeft();
			strTemp.TrimRight();
			if(IndexID == 0)
				tempstrArray.strKP_Type = strTemp;
			if(IndexID == 1)
				tempstrArray.strKP_Name = strTemp;
			if(IndexID == 2)
				tempstrArray.strKP_ID_Rule = strTemp;
			if(IndexID == 3)
				tempstrArray.strKP_Part_Number = strTemp;
			if(IndexID == 4)
				tempstrArray.strChkDuplic = strTemp;
			nSubStart = nSubPos + 1;
			IndexID++;
		}
		if(IndexID != 5)
		{
			AfxMessageBox(_T("Check KeyPart Info Error! Info = ") + OneKeyPartInfo );
			theApp.pCloseDB();
			return false;
		}
		vKeyPartInfo.push_back(tempstrArray);
		nStart = nPos + 1;
		ActualKeyPartCount++;
		nSubStart = 0;
		nSubPos = 0;
		IndexID = 0;
	}
	if ( ActualKeyPartCount != KeyPartCount )
	{
		AfxMessageBox(_T("pQueryKeyparts_Info Err = ") + strKeyPartsInfo );
		theApp.pCloseDB();
		return false;
	}
	return true;
}

bool CInitialDlg::QueryPNinBACH_BySOEx(CString strKeyPartPN,CString strSONumber,CString &strCount)
{
	char sz_KeyPartPN[32]={0};
	char sz_szSONumber[32]={0};
	char sz_qty[512]={0};
	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};

	WideCharToMultiByte(CP_ACP,NULL,strKeyPartPN.GetBuffer(), -1,sz_KeyPartPN, strKeyPartPN.GetLength()*3, NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strSONumber.GetBuffer(), -1,sz_szSONumber, strSONumber.GetLength()*3, NULL, NULL);

	if(theApp.pQueryPNinBACH_BySO(sz_KeyPartPN,sz_szSONumber,sz_qty,sz_Errorcode,sz_ErrMsg))
	{
		theApp.MultiByteToWideCharEnhanced(sz_qty,strCount);
		return true;
	}
	else
	{
		return false;
	}

}

void CInitialDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

bool CInitialDlg::CheckRuleConflict(CString strRule1, CString strRule2)
{
	if( strRule1.GetLength() !=strRule2.GetLength())
		return false;

	for(unsigned int i = 0; i < strRule1.GetLength() ; i++)
	{
		if(strRule1.Mid(i,1) == _T("*") || strRule2.Mid(i,1) == _T("*"))
			continue;
		if(strRule1.Mid(i,1) != strRule2.Mid(i,1))
			return false;
	}

	return true;
}
void CInitialDlg::GetWorkStationName(CString & strPCName)
{
	DWORD size=0;
	GetComputerName(NULL,&size);
	wchar_t *name=new wchar_t[size];
	if(GetComputerName(name,&size))
		strPCName.SetString(name,size);
	delete [] name;
	return;
}

bool CInitialDlg::QuerySegmentProcessEx(CString strModelName, CString strSegment,std::vector <CString> &vProcessSelect)
{
	char sz_Model[32]={0};
	char sz_Segment[32]={0};
	char sz_Processes[512]={0};
	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};

	WideCharToMultiByte(CP_ACP,NULL,strModelName.GetBuffer(), -1,sz_Model, strModelName.GetLength()*3, NULL, NULL);
	WideCharToMultiByte(CP_ACP,NULL,strSegment.GetBuffer(), -1,sz_Segment, strSegment.GetLength()*3, NULL, NULL);

	bool DBresult = theApp.pQuerySegmentProcess(sz_Model, sz_Segment,sz_Processes, sz_Errorcode,sz_ErrMsg);
	if (DBresult)
	{
		CString strProcesses;
		theApp.MultiByteToWideCharEnhanced(sz_Processes,strProcesses);
		while(vProcessSelect.size())
			vProcessSelect.pop_back();
		int nSubStart = 0;
		int nSubPos = 0;
		CString Subdelims = _T(",");
		while (strProcesses.Find(Subdelims,nSubStart) > 0 || nSubStart < strProcesses.GetLength())
		{
			if (strProcesses.Find(Subdelims,nSubStart) > 0)
				nSubPos = strProcesses.Find(Subdelims,nSubStart);
			else
				nSubPos = strProcesses.GetLength();
			vProcessSelect.push_back(strProcesses.Mid(nSubStart,nSubPos-nSubStart));
			nSubStart = nSubPos + 1;
		}
	}
	else
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pQuerySegmentProcess! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}
	return true;
}

bool CInitialDlg::QueryStation_InfoEx(CString strPCName, CString &strLine,CString &strProcess,CString &strSegment)
{
	char sz_Line[32]={0};
	char sz_Process[32]={0};
	char sz_Segment[32]={0};
	char sz_PCName[512]={0};

	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};

	WideCharToMultiByte(CP_ACP,NULL,m_strPCName.GetBuffer(),-1,sz_PCName,m_strPCName.GetLength(),NULL, NULL);
	bool DBresult = theApp.pQueryStation_Info(sz_PCName,sz_Line,sz_Process,sz_Segment,sz_Errorcode,sz_ErrMsg);

	if (DBresult)
	{
		theApp.MultiByteToWideCharEnhanced(sz_Line,strLine);
		theApp.MultiByteToWideCharEnhanced(sz_Process,strProcess);
		theApp.MultiByteToWideCharEnhanced(sz_Segment,strSegment);
		return true;
	}
	else
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pQueryStation_Info! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}
}

bool CInitialDlg::QueryEmployeeID_RightEx(CString strEmployId, CString &strEmployeeIDLevel,CString &strStatus,CString &strPCName)
{
	char sz_EmployId[32]={0};
	char sz_Level[32]={0};
	char sz_Status[32]={0};
	char sz_PCName[512]={0};
	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};
	WideCharToMultiByte(CP_ACP,NULL,m_strEmployeeID.GetBuffer(),-1,sz_EmployId,m_strEmployeeID.GetLength()*3,NULL, NULL);
	bool DBresult = theApp.pQueryEmployeeID_Right_V2(sz_EmployId,sz_Level,sz_Status,sz_PCName,sz_Errorcode,sz_ErrMsg);
	if (DBresult)
	{
		theApp.MultiByteToWideCharEnhanced(sz_Level,strEmployeeIDLevel);
		theApp.MultiByteToWideCharEnhanced(sz_Status,strStatus);
		theApp.MultiByteToWideCharEnhanced(sz_PCName,strPCName);
		return true;
	}
	else
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pQueryEmployeeID_Right! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}
}

bool CInitialDlg::QuerySO_InfoEx(CString strSONumber, CString &strModel, CString &strPN, CString &strSOType,CString &strQty,CString &strSOCRtype,CString &strSegment)
{
	char sz_SONumber[32]={0};
	char sz_Model[32]={0};
	char sz_PN[32]={0};
	char sz_SOType[32]={0};
	char sz_toolVer[32]={0};
	char sz_facImageVer[32]={0};
	char sz_userImageVer[32]={0};
	char sz_EntranceToolVer[32]={0};
	char sz_Qty[32]={0};
	char sz_CRtype[32]={0};
	char sz_Segment[32]={0};
	char sz_Errorcode[512]={0};
	char sz_ErrMsg[512]={0};

	WideCharToMultiByte(CP_ACP,NULL,m_strSONumber.GetBuffer(),-1,sz_SONumber,m_strSONumber.GetLength()*3,NULL, NULL);
	bool DBresult = theApp.pQuerySO_Info_All_V4(sz_SONumber,sz_Model,sz_PN,sz_SOType,sz_toolVer,sz_facImageVer,sz_userImageVer,sz_EntranceToolVer,sz_Qty,sz_CRtype,sz_Segment,sz_Errorcode,sz_ErrMsg);
	if (DBresult)
	{
		theApp.MultiByteToWideCharEnhanced(sz_Model,strModel);
		theApp.MultiByteToWideCharEnhanced(sz_PN,strPN);
		theApp.MultiByteToWideCharEnhanced(sz_SOType,strSOType);
		theApp.MultiByteToWideCharEnhanced(sz_Qty,strQty);
		theApp.MultiByteToWideCharEnhanced(sz_CRtype,strSOCRtype);
		theApp.MultiByteToWideCharEnhanced(sz_Segment,strSegment);
		return true;
	}
	else
	{
		CString strErrorcode = _T("");
		CString strErrorMsg = _T("");
		theApp.MultiByteToWideCharEnhanced(sz_Errorcode,strErrorcode);
		theApp.MultiByteToWideCharEnhanced(sz_ErrMsg,strErrorMsg);
		AfxMessageBox(_T("pQuerySO_Info_All_V4! ErrorCode = ") + strErrorcode + _T(" ,ErrorMsg = ") + strErrorMsg);
		return false;
	}
}