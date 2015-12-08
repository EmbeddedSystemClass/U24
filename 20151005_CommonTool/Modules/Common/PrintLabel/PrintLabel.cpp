
#include "stdafx.h"
#include "PrintLabel.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWinApp theApp;
using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)) {
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	} else {
	}

	return nRetCode;
}


CPrintLabel::CPrintLabel(void)
{
	m_p_labelSoftware = NULL;
	m_p_disp = NULL;
	m_b_attached = FALSE;
	m_b_load = FALSE;

}

CPrintLabel::~CPrintLabel(void)
{
	DisConnect();
}


/*
	获取连接结果
*/
int CPrintLabel::GetConnect(IApplication * p_app)
{
	if(p_app == NULL) {
		return -60;
	}

	if(m_p_labelSoftware != NULL) {
		return -70;
	}

	m_p_labelSoftware = p_app;

	/* Connect */
	int i_connectResult = -1;
	i_connectResult = Connect();
	if(i_connectResult == -3) {
		return -30;
	}
	if(i_connectResult == -4) {
		return -40;
	}
	if(i_connectResult == -5) {
		return -50;
	}

	return OK;
}


/*
	打开连接 --- 打开Codesoft
*/
int CPrintLabel::Connect(void)
{
	HRESULT hr = NULL;
	CLSID clsid;
	IUnknown *pInterface = NULL;

	::CoUninitialize();
	Sleep(200);

	/* 初始化COM环境 */
	if(::CoInitialize(NULL) != S_OK) {
		::CoUninitialize();
		Sleep(200);
		if(::CoInitialize(NULL) != S_OK) {
			return -3;
		}
	}

	/* 根据给定的程序标识符从注册表找出对应的类标识符 */
	hr = ::CLSIDFromProgID(L"Lppx2.Application", &clsid);
	if(FAILED(hr)) {
		return -4;
	}

	/* 查看是否有已经打开对象 */
	hr = ::GetActiveObject(clsid, NULL, &pInterface);
	if(!FAILED(hr)) {

		/* 已存在该对象 */
		hr = pInterface->QueryInterface(IID_IDispatch, (void FAR* FAR*)&m_p_disp); 
		if(!FAILED(hr)) {
			m_b_attached = TRUE;
			m_b_load = TRUE;
			m_p_labelSoftware->AttachDispatch(m_p_disp, TRUE);
		}
	}

	if(!m_b_load) {
		hr = ::CLSIDFromProgID(L"Lppx2.Application", &clsid);
		if(!FAILED(hr)) {
			m_b_load = TRUE;
			try {
				/* 创建新接口对象 */
      			m_p_labelSoftware->CreateDispatch("Lppx2.Application", &m_p_error);
			} catch (COleDispatchException m_pError) {
				m_pError.Delete();
				return -5;
			}
		}
	}
	//m_p_labelSoftware->SetVisible(TRUE);
	return m_b_load;
}


/*
	断开连接
*/
void CPrintLabel::DisConnect(void)
{
	if(m_p_labelSoftware) {
		if(m_b_attached) {
			m_p_labelSoftware->DetachDispatch();
			m_p_disp->Release();
			m_p_disp = NULL;
		} else {
			m_p_labelSoftware->Quit();
			m_p_labelSoftware->ReleaseDispatch();
		}
	}
	::CoUninitialize();
}


/**
 * Function   : PrintLabelCodesoft
 * Description: 打印IMEI，PPID标签
 * Param[in]  :
 *         (1). char *sz_IMEI        // IMEI
 *         (2). char *sz_PPID        // PPID
 * Param[out] : 
 *         (1). char *sz_outInfo     // output info
 * Return     : 
 *                   
 * Note       : 
 *
 */
PRINTLABEL_API BOOL PrintIMEIPPID(char *sz_IMEI, char *sz_PPID, char *sz_outInfo)
{

	/*=========================================== 读INI文件 ================================================*/
	/* Check Input param */
	if(sz_IMEI == NULL || sz_PPID == NULL) {
		_tcscpy(sz_outInfo, _T("IMEI or PPID is empty."));
		return false;
	}

	/* Get setup.ini file pathname */
	TCHAR sz_tmpBuf[100];
	memset(sz_tmpBuf, 0, sizeof(sz_tmpBuf));
	::GetPrivateProfileString("CURRENT_PROJECT", "Project", "", sz_tmpBuf, sizeof(sz_tmpBuf), ".\\INI\\setup.ini");
	CString str_iniFileName;
	str_iniFileName.Format(_T(".\\INI\\%s_setup.ini"), sz_tmpBuf);

	/* Get var name */
	memset(sz_tmpBuf, 0, sizeof(sz_tmpBuf));
	::GetPrivateProfileString("PRINT_IMEIPPID", "Var1_IMEI", "", sz_tmpBuf, sizeof(sz_tmpBuf), str_iniFileName);
	CString str_varNameOne;	
	str_varNameOne.Format(_T("%s"), sz_tmpBuf);

	memset(sz_tmpBuf, 0, sizeof(sz_tmpBuf));
	::GetPrivateProfileString("PRINT_IMEIPPID", "Var2_PPID", "", sz_tmpBuf, sizeof(sz_tmpBuf), str_iniFileName);
	CString str_varNameTwo;	
	str_varNameTwo.Format(_T("%s"), sz_tmpBuf);

	memset(sz_tmpBuf, 0, sizeof(sz_tmpBuf));
	::GetPrivateProfileString("PRINT_IMEIPPID", "Var3_P-P-I-D", "", sz_tmpBuf, sizeof(sz_tmpBuf), str_iniFileName);
	CString str_varNameThree;	
	str_varNameThree.Format(_T("%s"), sz_tmpBuf);

	/* Label file name */
	memset(sz_tmpBuf, 0, sizeof(sz_tmpBuf));
	::GetPrivateProfileString("PRINT_IMEIPPID", "LABELFILE", "", sz_tmpBuf, sizeof(sz_tmpBuf), str_iniFileName);
	CString str_labelFileName;	
	TCHAR szFolder[200] = {0};
	::GetCurrentDirectory(200, szFolder);
	str_labelFileName.Format(_T("%s\\Label_File\\%s"),szFolder, sz_tmpBuf);

	/* Check var if existent */
	if(str_varNameOne == _T(""), str_varNameTwo == _T(""), str_varNameThree == _T("")) {
		_tcscpy(sz_outInfo, _T("The variable name is not exist in the .ini file."));
		return false;
	}

	/* Check label file if existent */
	if(_taccess(str_labelFileName, 0) != 0) {
		_stprintf(sz_outInfo, _T("%s is not exist."), str_labelFileName);
		return false;
	}

	/* Convert to CString from char* */
	CString str_IMEI;
	CString str_PPID;
	str_IMEI.Format(_T("%s"), sz_IMEI);
	str_PPID.Format(_T("%s"), sz_PPID);


	/*=========================================== 打印label ================================================*/
	CPrintLabel  obj_printLabel;
	IApplication obj_labeleSoftware;

	/* Connect */
	int i_getRes = NG;
	i_getRes = obj_printLabel.GetConnect(&obj_labeleSoftware);
	if(i_getRes != OK) {
		//_tcscpy(sz_outInfo, _T("Connect CodeSoft failure."));
		if(i_getRes == -30) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -30."));
		}
		if(i_getRes == -40) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -40."));
		}
		if(i_getRes == -50) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -50."));
		}
		if(i_getRes == -60) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -60."));
		}
		if(i_getRes == -70) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -70."));
		}
		return false;
	}

	/* Open .lab file */
	Documents Docs = obj_labeleSoftware.GetDocuments();
	if(Docs == NULL) {
		_tcscpy(sz_outInfo, _T("Open lab template file failure."));
		return false;
	}
	if(Docs.Open(str_labelFileName, FALSE) == NULL) {
		_tcscpy(sz_outInfo, _T("Open lab template file failure."));
		return false;
	}
	Sleep(100);

	/* Get variable from the .lab file */
	IDocument MyDoc =  obj_labeleSoftware.GetActiveDocument();
	Variables Vars = MyDoc.GetVariables();
	Variable MyVar;
	COleVariant nItem;

	/* Get variable number from the .lab file */
	short i_varCount = 0;
	i_varCount = Vars.GetCount();

	/* Replace the variable name with the value(IMIE,PPID) */
	for(short i = 0; i < i_varCount; i++)
	{
		nItem = short(i + 1);
		MyVar = Vars.Item(nItem);
		if(MyVar.GetName() == str_varNameOne) {
			MyVar.SetValue(str_IMEI);
		}
		if(MyVar.GetName() == str_varNameTwo) {
			MyVar.SetValue(str_PPID);
		}
		if(MyVar.GetName() == str_varNameThree) {
			CString str_tmp;
			try{
				str_tmp.Format("%s", str_PPID);
				str_tmp.Insert(2, "-");
				str_tmp.Insert(9, "-");
				str_tmp.Insert(15, "-");
				str_tmp.Insert(19, "-");
				str_tmp.Insert(24, "-");

			}catch(CException *e) {
				e->Delete();
				_tcscpy(sz_outInfo, _T("Replace the value failure."));
				return false;
			}
			MyVar.SetValue(str_tmp);
		}
	}

	/* Print */
	MyDoc.PrintDocument(1);
	// MyDoc.PrintLabel(1, -1, -1, -1, -1, NULL); 

	return true;
}


PRINTLABEL_API BOOL PrintPPID(char *sz_PPID, char *sz_outInfo)
{
	/*=========================================== 读INI文件 ================================================*/
	/* Check Input param */
	if(sz_PPID == NULL) {
		_tcscpy(sz_outInfo, _T("PPID is empty."));
		return false;
	}

	/* Get setup.ini file pathname */
	TCHAR sz_tmpBuf[100];
	memset(sz_tmpBuf, 0, sizeof(sz_tmpBuf));
	::GetPrivateProfileString("CURRENT_PROJECT", "Project", "", sz_tmpBuf, sizeof(sz_tmpBuf), ".\\INI\\setup.ini");
	CString str_iniFileName;
	str_iniFileName.Format(_T(".\\INI\\%s_setup.ini"), sz_tmpBuf);

	/* Get var name */
	memset(sz_tmpBuf, 0, sizeof(sz_tmpBuf));
	::GetPrivateProfileString("PRINT_PPID", "Var1_PPID", "", sz_tmpBuf, sizeof(sz_tmpBuf), str_iniFileName);
	CString str_varNameOne;	
	str_varNameOne.Format(_T("%s"), sz_tmpBuf);

	memset(sz_tmpBuf, 0, sizeof(sz_tmpBuf));
	::GetPrivateProfileString("PRINT_PPID", "Var2_P-P-I-D", "", sz_tmpBuf, sizeof(sz_tmpBuf), str_iniFileName);
	CString str_varNameTwo;	
	str_varNameTwo.Format(_T("%s"), sz_tmpBuf);

	/* Label file name */
	memset(sz_tmpBuf, 0, sizeof(sz_tmpBuf));
	::GetPrivateProfileString("PRINT_PPID", "LABELFILE", "", sz_tmpBuf, sizeof(sz_tmpBuf), str_iniFileName);
	CString str_labelFileName;	
	TCHAR szFolder[200] = {0};
	::GetCurrentDirectory(200, szFolder);
	str_labelFileName.Format(_T("%s\\Label_File\\%s"),szFolder, sz_tmpBuf);

	/* Check var if existent */
	if(str_varNameOne == _T(""), str_varNameTwo == _T("")) {
		_tcscpy(sz_outInfo, _T("The variable name is not exist in the .ini file."));
		return false;
	}

	/* Check label file if existent */
	if(_taccess(str_labelFileName, 0) != 0) {
		_stprintf(sz_outInfo, _T("%s is not exist."), str_labelFileName);
		return false;
	}

	/* Convert to CString from char* */
	CString str_PPID;
	str_PPID.Format(_T("%s"), sz_PPID);


	/*=========================================== 打印label ================================================*/
	CPrintLabel  obj_printLabel;
	IApplication obj_labeleSoftware;

	/* Connect */
	int i_getRes = NG;
	i_getRes = obj_printLabel.GetConnect(&obj_labeleSoftware);
	if(i_getRes != OK) {
		//_tcscpy(sz_outInfo, _T("Connect CodeSoft failure."));
		if(i_getRes == -30) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -30."));
		}
		if(i_getRes == -40) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -40."));
		}
		if(i_getRes == -50) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -50."));
		}
		if(i_getRes == -60) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -60."));
		}
		if(i_getRes == -70) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -70."));
		}
		return false;
	}

	/* Open .lab file */
	Documents Docs = obj_labeleSoftware.GetDocuments();
	if(Docs == NULL) {
		_tcscpy(sz_outInfo, _T("Open lab template file failure."));
		return false;
	}
	if(Docs.Open(str_labelFileName, FALSE) == NULL) {
		_tcscpy(sz_outInfo, _T("Open lab template file failure."));
		return false;
	}
	Sleep(100);

	/* Get variable from the .lab file */
	IDocument MyDoc =  obj_labeleSoftware.GetActiveDocument();
	Variables Vars = MyDoc.GetVariables();
	Variable MyVar;
	COleVariant nItem;

	/* Get variable number from the .lab file */
	short i_varCount = 0;
	i_varCount = Vars.GetCount();

	/* Replace the variable name with the value(IMIE,PPID) */
	for(short i = 0; i < i_varCount; i++)
	{
		nItem = short(i + 1);
		MyVar = Vars.Item(nItem);
		if(MyVar.GetName() == str_varNameOne) {
			MyVar.SetValue(str_PPID);
		}
		if(MyVar.GetName() == str_varNameTwo) {
			CString str_tmp;
			try{
				str_tmp.Format("%s", str_PPID);
				str_tmp.Insert(2, "-");
				str_tmp.Insert(9, "-");
				str_tmp.Insert(15, "-");
				str_tmp.Insert(19, "-");
				str_tmp.Insert(24, "-");

			}catch(CException *e) {
				e->Delete();
				_tcscpy(sz_outInfo, _T("Replace the value failure."));
				return false;
			}
			MyVar.SetValue(str_tmp);
		}
	}

	/* Print */
	MyDoc.PrintDocument(1);
	// MyDoc.PrintLabel(1, -1, -1, -1, -1, NULL); 

	return true;
}


PRINTLABEL_API BOOL PrintPPIDID(char *sz_PPID, char *sz_outInfo)
{
	/*=========================================== 读INI文件 ================================================*/
	/* Check Input param */
	if(sz_PPID == NULL) {
		_tcscpy(sz_outInfo, _T("PPID is empty."));
		return false;
	}

	/* Get setup.ini file pathname */
	TCHAR sz_tmpBuf[100];
	memset(sz_tmpBuf, 0, sizeof(sz_tmpBuf));
	::GetPrivateProfileString("CURRENT_PROJECT", "Project", "", sz_tmpBuf, sizeof(sz_tmpBuf), ".\\INI\\setup.ini");
	CString str_iniFileName;
	str_iniFileName.Format(_T(".\\INI\\%s_setup.ini"), sz_tmpBuf);

	/* Get var name */
	memset(sz_tmpBuf, 0, sizeof(sz_tmpBuf));
	::GetPrivateProfileString("PRINT_PPIDID", "Var1_PPID", "", sz_tmpBuf, sizeof(sz_tmpBuf), str_iniFileName);
	CString str_varNameOne;	
	str_varNameOne.Format(_T("%s"), sz_tmpBuf);

	memset(sz_tmpBuf, 0, sizeof(sz_tmpBuf));
	::GetPrivateProfileString("PRINT_PPIDID", "Var2_P-P-I-D", "", sz_tmpBuf, sizeof(sz_tmpBuf), str_iniFileName);
	CString str_varNameTwo;	
	str_varNameTwo.Format(_T("%s"), sz_tmpBuf);

	/* Label file name */
	memset(sz_tmpBuf, 0, sizeof(sz_tmpBuf));
	::GetPrivateProfileString("PRINT_PPIDID", "LABELFILE", "", sz_tmpBuf, sizeof(sz_tmpBuf), str_iniFileName);
	CString str_labelFileName;	
	TCHAR szFolder[200] = {0};
	::GetCurrentDirectory(200, szFolder);
	str_labelFileName.Format(_T("%s\\Label_File\\%s"),szFolder, sz_tmpBuf);

	/* Check var if existent */
	if(str_varNameOne == _T(""), str_varNameTwo == _T("")) {
		_tcscpy(sz_outInfo, _T("The variable name is not exist in the .ini file."));
		return false;
	}

	/* Check label file if existent */
	if(_taccess(str_labelFileName, 0) != 0) {
		_stprintf(sz_outInfo, _T("%s is not exist."), str_labelFileName);
		return false;
	}

	/* Convert to CString from char* */
	CString str_PPID;
	str_PPID.Format(_T("%s"), sz_PPID);


	/*=========================================== 打印label ================================================*/
	CPrintLabel  obj_printLabel;
	IApplication obj_labeleSoftware;

	/* Connect */
	int i_getRes = NG;
	i_getRes = obj_printLabel.GetConnect(&obj_labeleSoftware);
	if(i_getRes != OK) {
		//_tcscpy(sz_outInfo, _T("Connect CodeSoft failure."));
		if(i_getRes == -30) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -30."));
		}
		if(i_getRes == -40) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -40."));
		}
		if(i_getRes == -50) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -50."));
		}
		if(i_getRes == -60) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -60."));
		}
		if(i_getRes == -70) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -70."));
		}
		return false;
	}

	/* Open .lab file */
	Documents Docs = obj_labeleSoftware.GetDocuments();
	if(Docs == NULL) {
		_tcscpy(sz_outInfo, _T("Open lab template file failure."));
		return false;
	}
	if(Docs.Open(str_labelFileName, FALSE) == NULL) {
		_tcscpy(sz_outInfo, _T("Open lab template file failure."));
		return false;
	}
	Sleep(100);

	/* Get variable from the .lab file */
	IDocument MyDoc =  obj_labeleSoftware.GetActiveDocument();
	Variables Vars = MyDoc.GetVariables();
	Variable MyVar;
	COleVariant nItem;

	/* Get variable number from the .lab file */
	short i_varCount = 0;
	i_varCount = Vars.GetCount();

	/* Replace the variable name with the value(IMIE,PPID) */
	for(short i = 0; i < i_varCount; i++)
	{
		nItem = short(i + 1);
		MyVar = Vars.Item(nItem);
		if(MyVar.GetName() == str_varNameOne) {
			MyVar.SetValue(str_PPID);
		}
		if(MyVar.GetName() == str_varNameTwo) {
			CString str_tmp;
			try{
				str_tmp.Format("%s", str_PPID);
				str_tmp.Insert(2, "-");
				str_tmp.Insert(9, "-");
				str_tmp.Insert(15, "-");
				str_tmp.Insert(19, "-");
				str_tmp.Insert(24, "-");

			}catch(CException *e) {
				e->Delete();
				_tcscpy(sz_outInfo, _T("Replace the value failure."));
				return false;
			}
			MyVar.SetValue(str_tmp);
		}
	}

	/* Print */
	MyDoc.PrintDocument(1);
	// MyDoc.PrintLabel(1, -1, -1, -1, -1, NULL); 

	return true;
}


//PRINTLABEL_API bool PrintLabelLPTPORT(char *sz_IMEI, char *sz_PPID, char *sz_outInfo)
//{
//	/* Check Input param */
//	if(sz_IMEI == NULL || sz_IMEI == NULL) {
//		_tcscpy(sz_outInfo, _T("IMEI or PPID is empty."));
//		return false;
//	}
//
//	/* Get setup.ini file pathname */
//	TCHAR sz_tmpBuf[100];
//	memset(sz_tmpBuf, 0, sizeof(sz_tmpBuf));
//	::GetPrivateProfileString("CURRENT_PROJECT", "Project", "", sz_tmpBuf, sizeof(sz_tmpBuf), ".\\INI\\setup.ini");
//	CString str_iniFileName;
//	str_iniFileName.Format(_T(".\\INI\\%s_setup.ini"), sz_tmpBuf);
//
//	/* Port */
//	memset(sz_tmpBuf, 0, sizeof(sz_tmpBuf));
//	::GetPrivateProfileString("PRINT_LABEL", "PORT", "", sz_tmpBuf, sizeof(sz_tmpBuf), str_iniFileName);
//	CString str_port;	
//	str_port.Format(_T("%s"), sz_tmpBuf);
//
//	/* Baud */
//	DWORD dw_baud = 0;
//	dw_baud = ::GetPrivateProfileInt("PRINT_LABEL", "BAUD", 0, str_iniFileName);
//
//	/* Label file name */
//	memset(sz_tmpBuf, 0, sizeof(sz_tmpBuf));
//	::GetPrivateProfileString("PRINT_LABEL", "LABELFILENAME", "", sz_tmpBuf, sizeof(sz_tmpBuf), str_iniFileName);
//	CString str_labelFileName;	
//	str_labelFileName.Format(_T(".\\Label_File\\%s"), sz_tmpBuf);
//
//	/* Check label file if existent */
//	if(_taccess(str_labelFileName, 0) != 0) {
//		_stprintf(sz_outInfo, _T("%s is not exist."), str_labelFileName);
//		return false;
//	}
//
//	CComComm obj_comComm;    // CComComm object
//		
//	/* Check port */
//	if(str_port == _T("") || dw_baud == 0) {
//		_tcscpy(sz_outInfo, _T("Port is empty in the ini file."));
//		return false;
//	}
//	int i_findPort = -1;
//	if(str_port.Find(_T("COM")) != -1) {
//		/* Open COM port */
//		obj_comComm.SetComm(str_port, dw_baud);
//		if(obj_comComm.OpenComm() != TRUE) {
//			_tcscpy(sz_outInfo, _T("Open port failure."));
//			return false;
//		}
//	}
//	else if(str_port.Find(_T("LPT")) != -1) {
//		/* Open LPT port */
//		obj_comComm.SetComm(str_port, dw_baud);
//		if(obj_comComm.OpenLPT() != TRUE) {
//			_tcscpy(sz_outInfo, _T("Open port failure."));
//			return false;
//		}
//	}
//	else {
//		_tcscpy(sz_outInfo, _T("Port is not exist."));
//		return false;
//	}
//	
//	/* Open label file */
//	CStdioFile obj_file;
//	if(!obj_file.Open(str_labelFileName, CFile::modeRead, NULL)) {
//		_stprintf(sz_outInfo, _T("Open %s failure."), str_labelFileName);
//		return false;
//	}
//
//	/* Read label file */
//	CString str_IMEI;
//	CString str_PPID;
//	str_IMEI.Format(_T("%s"), sz_IMEI);
//	str_PPID.Format(_T("%s"), sz_PPID);
//
//	int i_findPos = -1;
//	DWORD dw_writeNum = -1;
//	try {
//		CString str_string;
//		while(obj_file.ReadString(str_string)) 
//		{
//			if(str_string == _T("")) {
//				continue;
//			}
//			i_findPos = str_string.Find("[IMEI]", 0);
//			if(i_findPos != -1) {
//				str_string.Replace("[IMEI]", str_IMEI);
//			}
//
//			i_findPos = str_string.Find("[PPID]", 0);
//			if(i_findPos != -1) {
//				str_string.Replace("[PPID]", str_PPID);
//			}
//
//			i_findPos = str_string.Find("[P-P-I-D]", 0);
//			if(i_findPos != -1) {
//				
//				/* CN0WK2PN7426105K007YA00 -> CN-0WK2PN-74261-05K-007Y-A00 */
//				CString str_tmp;
//				try{
//					str_tmp.Format("%s", str_PPID);
//					str_tmp.Insert(2, "-");
//					str_tmp.Insert(9, "-");
//					str_tmp.Insert(15, "-");
//					str_tmp.Insert(19, "-");
//					str_tmp.Insert(24, "-");
//	
//				}catch(CException *e) {
//					e->Delete();
//					obj_file.Close();
//					obj_comComm.CloseComm();
//					return false;
//				}
//				str_string.Replace("[P-P-I-D]", str_tmp);
//			}
//			
//			if(str_port.Find(_T("COM")) != -1) {
//
//				/* Write COM */
//				dw_writeNum = obj_comComm.WriteComm(str_string, str_string.GetLength());
//				if(dw_writeNum != str_string.GetLength()) {
//					obj_file.Close();
//					obj_comComm.CloseComm();
//					return false;
//				}
//			}
//			else if(str_port.Find(_T("LPT")) != -1) {
//
//				/* Write LPT */
//				dw_writeNum = obj_comComm.WriteLPT(str_string, str_string.GetLength());
//			}
//		}
//	}catch (CException *e) {
//		e->Delete();
//		obj_file.Close();
//		obj_comComm.CloseComm();
//		return false;
//	}
//
//	Sleep(1000);
//
//	/* Close */
//	obj_file.Close();
//	obj_comComm.CloseComm();
//
//	return true;
//}

/**
 * Function   : PrintLabelCodesoft
 * Description: 打印FA流程标签
 * Param[in]  :
 *         (1). char *sz_Picasso       // Picasso
 *         (2). char *sz_Model         // Model name
 *         (3). char *sz_HW            // HW Ver
 *         (4). char *sz_SW            // SW ver
 *         (5). char *sz_SO            // So
 *         (6). char *sz_PN           // Part number
 *         (7). char *sz_SN            // SN
 *         (8). char *sz_Line          // Line
 * Param[out] : 
 *         (1). char *sz_outInfo     // output info
 * Return     : 
 *                   
 * Note       : 
 *
 */
PRINTLABEL_API BOOL PrintFATravelLabel(char *sz_Picasso, char *sz_Model, char *sz_HW, char *sz_SW, char *sz_SO, char *sz_SN, char *sz_PN, char *sz_Line,char *sz_outInfo)
{
	/* Convert to CString from char* */
	CString str_Picasso;
	CString str_SW;
	CString str_HW;
	CString str_SN;
	CString str_SO;
	CString str_Line;
	CString str_PN;
	CString str_Model;

	str_Picasso.Format(_T("%s"), sz_Picasso);
	str_SW.Format(_T("%s"), sz_SW);
	str_Model.Format(_T("%s"), sz_Model);
	str_HW.Format(_T("%s"), sz_HW);
	str_SO.Format(_T("%s"), sz_SO);
	str_SN.Format(_T("%s"), sz_SN);
	str_PN.Format(_T("%s"), sz_PN);
	str_Line.Format(_T("%s"), sz_Line);

	/*=========================================== 读INI文件 ================================================*

	/* Label file name */
	CString str_labelFileName;	
	TCHAR szFolder[200] = {0};
	::GetCurrentDirectory(200, szFolder);
	//str_labelFileName.Format(_T("%s\\Qisda\\Label_File\\%s travel label.lab"),szFolder, str_Model);
	str_labelFileName.Format(_T("C:\\PE_config\\Label_File\\%s travel label.lab"), str_Model);

	/* Check label file if existent */
	if(_taccess(str_labelFileName, 0) != 0) {
		_stprintf(sz_outInfo, _T("%s is not exist."), str_labelFileName);
		return false;
	}

	/*=========================================== 打印label ================================================*/
	CPrintLabel  obj_printLabel;
	IApplication obj_labeleSoftware;

	/* Connect */
	int i_getRes = NG;
	i_getRes = obj_printLabel.GetConnect(&obj_labeleSoftware);
	if(i_getRes != OK) {
		//_tcscpy(sz_outInfo, _T("Connect CodeSoft failure."));
		if(i_getRes == -30) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -30."));
		}
		if(i_getRes == -40) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -40."));
		}
		if(i_getRes == -50) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -50."));
		}
		if(i_getRes == -60) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -60."));
		}
		if(i_getRes == -70) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -70."));
		}
		return false;
	}

	/* Open .lab file */
	Documents Docs = obj_labeleSoftware.GetDocuments();
	if(Docs == NULL) {
		_tcscpy(sz_outInfo, _T("Open lab template file failure."));
		return false;
	}
	if(Docs.Open(str_labelFileName, FALSE) == NULL) {
		_tcscpy(sz_outInfo, _T("Open lab template file failure."));
		return false;
	}
	Sleep(100);

	/* Get variable from the .lab file */
	IDocument MyDoc =  obj_labeleSoftware.GetActiveDocument();
	Variables Vars = MyDoc.GetVariables();
	Variable MyVar;
	COleVariant nItem;

	/* Get variable number from the .lab file */
	short i_varCount = 0;
	i_varCount = Vars.GetCount();

	/* Replace the variable name with the value */
	for (short i=0; i< i_varCount;i++)
	{
		nItem = short(i + 1);
		MyVar = Vars.Item(nItem);

		if(MyVar.GetName() == _T("SW")) {
			MyVar.SetValue(str_SW);
		}
		if(MyVar.GetName() == _T("HW")) {
			MyVar.SetValue(str_HW);
		}
		if(MyVar.GetName() == _T("SO")) {
			MyVar.SetValue(str_SO);
		}
		if(MyVar.GetName() == _T("SN")) {
			MyVar.SetValue(str_SN);
		}
		if(MyVar.GetName() == _T("PN")) {
			MyVar.SetValue(str_PN);
		}
		if(MyVar.GetName() == _T("Line")) {
			MyVar.SetValue(str_Line);
		}
		if(MyVar.GetName() == _T("Model")) {
			MyVar.SetValue(str_Model);
		}
		if(MyVar.GetName() == _T("Picasso")) {
			MyVar.SetValue(str_Picasso);
		}
	}

	/* Print */
	MyDoc.PrintDocument(1);
	// MyDoc.PrintLabel(1, -1, -1, -1, -1, NULL); =	return true;
	return true;
}

PRINTLABEL_API BOOL  PrintFAPCBALabel(char *sz_PCBA_barcode ,char *sz_Model,char *sz_outInfo)
{
	/* Convert to CString from char* */
	CString str_PCBA_barcode;
	CString str_Model;

	str_PCBA_barcode.Format(_T("%s"), sz_PCBA_barcode);
	str_Model.Format(_T("%s"), sz_Model);

	/* Label file name */
	CString str_labelFileName;
	str_labelFileName.Format(_T("C:\\PE_config\\Label_File\\%s_PCBA_Label.lab"), str_Model);

	/* Check label file if existent */
	if(_taccess(str_labelFileName, 0) != 0) {
		_stprintf(sz_outInfo, _T("%s is not exist."), str_labelFileName);
		return false;
	}

	/*=========================================== 打印label ================================================*/
	CPrintLabel  obj_printLabel;
	IApplication obj_labeleSoftware;

	/* Connect */
	int i_getRes = NG;
	i_getRes = obj_printLabel.GetConnect(&obj_labeleSoftware);
	if(i_getRes != OK) {
		//_tcscpy(sz_outInfo, _T("Connect CodeSoft failure."));
		if(i_getRes == -30) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -30."));
		}
		if(i_getRes == -40) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -40."));
		}
		if(i_getRes == -50) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -50."));
		}
		if(i_getRes == -60) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -60."));
		}
		if(i_getRes == -70) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -70."));
		}
		return false;
	}

	/* Open .lab file */
	Documents Docs = obj_labeleSoftware.GetDocuments();
	if(Docs == NULL) {
		_tcscpy(sz_outInfo, _T("Open lab template file failure."));
		return false;
	}
	if(Docs.Open(str_labelFileName, FALSE) == NULL) {
		_tcscpy(sz_outInfo, _T("Open lab template file failure."));
		return false;
	}
	Sleep(100);

	/* Get variable from the .lab file */
	IDocument MyDoc =  obj_labeleSoftware.GetActiveDocument();
	Variables Vars = MyDoc.GetVariables();
	Variable MyVar;
	COleVariant nItem;

	/* Get variable number from the .lab file */
	short i_varCount = 0;
	i_varCount = Vars.GetCount();

	/* Replace the variable name with the value */
	for (short i=0; i< i_varCount;i++)
	{
		nItem = short(i + 1);
		MyVar = Vars.Item(nItem);

		if(MyVar.GetName() == _T("PCBANO")) {
			MyVar.SetValue(str_PCBA_barcode);
		}
	}

	/* Print */
	MyDoc.PrintDocument(1);
	// MyDoc.PrintLabel(1, -1, -1, -1, -1, NULL); =	return true;

	return true;
}

PRINTLABEL_API BOOL PrintIMEI(char *sz_imei, char * sz_labelFile, char *sz_outInfo)
{
	/*=========================================== 读INI文件 ================================================*/
	/* Check Input param */
	if(sz_imei == NULL) {
		_tcscpy(sz_outInfo, _T("IMEI is empty."));
		return false;
	}

	if(sz_labelFile == NULL) {
		_tcscpy(sz_outInfo, _T("Label file is empty."));
		return false;
	}

	/* Get setup.ini file pathname */
	
	CString str_labelFileName;	
	TCHAR szFolder[200] = {0};
	::GetCurrentDirectory(200, szFolder);
	str_labelFileName.Format(_T("%s\\Label_File\\%s"),szFolder, sz_labelFile);

	/* Check label file if existent */
	if(_taccess(str_labelFileName, 0) != 0) {
		_stprintf(sz_outInfo, _T("%s is not exist."), str_labelFileName);
		return false;
	}

	/* Convert to CString from char* */
	CString str_imei;
	str_imei.Format(_T("%s"), sz_imei);

	/*=========================================== 打印label ================================================*/
	CPrintLabel  obj_printLabel;
	IApplication obj_labeleSoftware;

	/* Connect */
	int i_getRes = NG;
	i_getRes = obj_printLabel.GetConnect(&obj_labeleSoftware);
	if(i_getRes != OK) {
		//_tcscpy(sz_outInfo, _T("Connect CodeSoft failure."));
		if(i_getRes == -30) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -30."));
		}
		if(i_getRes == -40) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -40."));
		}
		if(i_getRes == -50) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -50."));
		}
		if(i_getRes == -60) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -60."));
		}
		if(i_getRes == -70) {
			_tcscpy(sz_outInfo, _T("Connect CodeSoft failure --- -70."));
		}
		return false;
	}

	/* Open .lab file */
	Documents Docs = obj_labeleSoftware.GetDocuments();
	if(Docs == NULL) {
		_tcscpy(sz_outInfo, _T("Open lab template file failure."));
		return false;
	}
	if(Docs.Open(str_labelFileName, FALSE) == NULL) {
		_tcscpy(sz_outInfo, _T("Open lab template file failure."));
		return false;
	}
	Sleep(100);

	/* Get variable from the .lab file */
	IDocument MyDoc =  obj_labeleSoftware.GetActiveDocument();
	Variables Vars = MyDoc.GetVariables();
	Variable MyVar;
	COleVariant nItem;

	/* Get variable number from the .lab file */
	short i_varCount = 0;
	i_varCount = Vars.GetCount();

	if (i_varCount < 1)
	{
		_tcscpy(sz_outInfo, _T("Find no variable."));
		return false;
	}

	/* Replace the variable name with the value(IMIE,PPID) */
	for(short i = 0; i < i_varCount; i++)
	{
		nItem = short(i + 1);
		MyVar = Vars.Item(nItem);
		if(MyVar.GetName() == _T("SN1")) {
			MyVar.SetValue(str_imei);
		}
	}

	/* Print */
	MyDoc.PrintDocument(1);
	// MyDoc.PrintLabel(1, -1, -1, -1, -1, NULL); 

	return true;
}
