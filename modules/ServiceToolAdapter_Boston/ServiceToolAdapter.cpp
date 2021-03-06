// E330_Service_Adapter.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ServiceToolAdapter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(disable: 4996)

/******************************************************************************
 * export  extern C API ,
 *   1. New interface
 *   2. Del interface
******************************************************************************/
ISRV_API void GetQisdaModuleInfo(char* szName, char* szTitle, char* szType, char* szInfo, bool* isDongleLock)
{
	strncpy(szName,  QISDA_MODULE_NAME,  strlen(QISDA_MODULE_NAME));
	strncpy(szTitle, QISDA_MODULE_TITLE, strlen(QISDA_MODULE_TITLE));
	strncpy(szType,  QISDA_MODULE_TYPE,  strlen(QISDA_MODULE_TYPE));
	strncpy(szInfo,  QISDA_MODULE_INFO,  strlen(QISDA_MODULE_INFO));
	*isDongleLock  = QISDA_MODULE_CHECK_DONGLE;
}

ISRV_API HANDLE QisdaDllNew()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CServiceToolController* p_ISRV = new CServiceToolController;

	return (HANDLE)p_ISRV;
}

ISRV_API void QisdaDllDelete(HANDLE hResource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CServiceToolController* p_ISRV = (CServiceToolController*)hResource;
	if (p_ISRV)
	{
		delete (CServiceToolController*)p_ISRV;
		p_ISRV = NULL;
	}
}

ISRV_API int SetCallBackMsg(HANDLE hResource, int(*CallBack)(const char*, const char*))
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CServiceToolController* pSTController = (CServiceToolController*)hResource;
	pSTController->SetCallSendMsg(CallBack);
	return NO_ERROR;
}

ISRV_API int SetCallBackPort(HANDLE hResource, int(*CallBack)(const char*))
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CServiceToolController* pSTController = (CServiceToolController*)hResource;
	pSTController->SetCallSetPort(CallBack);
	return NO_ERROR;
}

ISRV_API int SetCallBackResult(HANDLE hResource, int(*CallBack)(const int, const char*, const char*, const char*, const char*, const char*, const char*))
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CServiceToolController* pSTController = (CServiceToolController*)hResource;
	pSTController->SetCallSendResult(CallBack);
	return NO_ERROR;
}

ISRV_API int SetParameter(HANDLE hResource, char* szKey, char* szValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CServiceToolController* pSTController = (CServiceToolController*)hResource;
	return pSTController->SetParameterValue(szKey, szValue);
}

ISRV_API int GetParameter(HANDLE hResource, char* szKey, char* szValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CServiceToolController* pSTController;

	if (NULL == hResource)
		pSTController = new CServiceToolController();
	else
		pSTController = (CServiceToolController*)hResource;

	pSTController->GetParameterValue(szKey, szValue);

	if (NULL == hResource && pSTController)
	{
		delete (CServiceToolController*)pSTController;
		pSTController = NULL;
	}

	return NO_ERROR;
}

ISRV_API int GetPortCount(HANDLE hResource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//CServiceToolController* pSTController = (CServiceToolController*)hResource;
	//return pSTController->GetPortCount();
	return 1;
}

ISRV_API int GetPortByIndex(HANDLE hResource, int nIndex, char* szPort)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//CServiceToolController* pSTController = (CServiceToolController*)hResource;
	//return pSTController->GetPortByIndex(nIndex, szPort);
	return 1;
}

ISRV_API int GetInfoCount(HANDLE hResource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CServiceToolController* pSTController = (CServiceToolController*)hResource;
	return pSTController->GetInfoCount();
}

ISRV_API int GetInfoByIndex(HANDLE hResource, int nIndex, char* szName, char* szInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CServiceToolController* pSTController = (CServiceToolController*)hResource;
	return pSTController->GetInfoByIndex(nIndex, szName, szInfo);
}

ISRV_API int Begin(HANDLE hResource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CServiceToolController* pSTController = (CServiceToolController*)hResource;
	return pSTController->Enhance_OnAPPLaunch();
}

ISRV_API int PreRun(HANDLE hResource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CServiceToolController* pSTController = (CServiceToolController*)hResource;
	return pSTController->Enhance_OnRunInit(0);
}

ISRV_API int Run(HANDLE hResource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CServiceToolController* pSTController = (CServiceToolController*)hResource;
	return pSTController->Enhance_RunTest(0);
}

ISRV_API int PostRun(HANDLE hResource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CServiceToolController* pSTController = (CServiceToolController*)hResource;
	size_t ret = pSTController->Enhance_OnRunFinish(0);
	pSTController->Enhance_OnAPPEnd();
	return ret;
}

ISRV_API int ShowPopupMsg(HANDLE hResource, int(*CallBack)(const char*))
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CServiceToolController* pSTController = (CServiceToolController*)hResource;
	pSTController->SetShowPopupMsg(CallBack);
	return NO_ERROR;
}

/******************************************************************************
 *  implement class
 *  CNotifyBase_Char_Public
******************************************************************************/

bool CNotifyBase_Char_Public::Register(INotify_Char* pNotify, CStringA strEvent)
{
	return CNotifyBase_Char::Register(pNotify, strEvent.GetString());
}

bool CNotifyBase_Char_Public::Register(EventFunc_Char* pEventFunc, CStringA strEvent)
{
	return CNotifyBase_Char::Register(pEventFunc, strEvent.GetString());
}

bool CNotifyBase_Char_Public::Unregister(INotify_Char* pNotify, CStringA strEvent)
{
	return CNotifyBase_Char::Unregister(pNotify, strEvent.GetString());
}

bool CNotifyBase_Char_Public::Unregister(EventFunc_Char* pEventFunc, CStringA strEvent)
{
	return CNotifyBase_Char::Unregister(pEventFunc, strEvent.GetString());
}

void CNotifyBase_Char_Public::DefineNotify(CStringA strNewEvent)
{
	CNotifyBase_Char::DefineNotify(strNewEvent.GetString());
}

void CNotifyBase_Char_Public::Fire(CStringA strEvent, long nParam)
{
	CNotifyBase_Char::Fire(strEvent.GetString(), nParam);
}

/******************************************************************************
 *  implement class
 *  CServiceToolController
******************************************************************************/

CServiceToolController::CServiceToolController()
: m_strCurrentStation(_T("")), nTestItemIndex(9999), bProcessResult(true)
{
	this->DefineNotifyEvent();

	this->m_Parametermap[ParameterKeyWord::MODELNAME] = UNIFYMODELNAME;
	this->m_Parametermap[ParameterKeyWord::CONFIGDIR] = _T("C:\\PE_Config\\") + this->m_Parametermap[ParameterKeyWord::MODELNAME] + _T("\\Setting\\");
	this->m_Parametermap[ParameterKeyWord::LOGDIR] = _T("D:\\Log\\ServiceTool\\");

	if(QISDA_MODULE_NAME == "BBTEST_NFC")
	{
		this->m_Parametermap[ParameterKeyWord::STATIONNAME] = this->m_strCurrentStation = STATION_BBTEST;
		GetModuleFileName(NULL, this->m_Parametermap[ParameterKeyWord::WORKINGDIR].GetBufferSetLength(MAX_PATH+1), MAX_PATH);
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR].ReleaseBuffer();
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR] = this->m_Parametermap[ParameterKeyWord::WORKINGDIR].Left(this->m_Parametermap[ParameterKeyWord::WORKINGDIR].ReverseFind('\\'));
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR].Append(_T("\\Module\\Boston\\L3L4\\BBTEST_NFC\\"));
	}
	else if(QISDA_MODULE_NAME == "BBTEST_nonNFC")
	{
		this->m_Parametermap[ParameterKeyWord::STATIONNAME] = this->m_strCurrentStation = STATION_BBTEST;
		GetModuleFileName(NULL, this->m_Parametermap[ParameterKeyWord::WORKINGDIR].GetBufferSetLength(MAX_PATH+1), MAX_PATH);
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR].ReleaseBuffer();
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR] = this->m_Parametermap[ParameterKeyWord::WORKINGDIR].Left(this->m_Parametermap[ParameterKeyWord::WORKINGDIR].ReverseFind('\\'));
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR].Append(_T("\\Module\\Boston\\L3L4\\BBTEST_nonNFC\\"));
	}
	else if(QISDA_MODULE_NAME == "2G3GTEST")
	{
		this->m_Parametermap[ParameterKeyWord::STATIONNAME] = this->m_strCurrentStation = STATION_2G3GTEST;
		GetModuleFileName(NULL, this->m_Parametermap[ParameterKeyWord::WORKINGDIR].GetBufferSetLength(MAX_PATH+1), MAX_PATH);
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR].ReleaseBuffer();
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR] = this->m_Parametermap[ParameterKeyWord::WORKINGDIR].Left(this->m_Parametermap[ParameterKeyWord::WORKINGDIR].ReverseFind('\\'));
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR].Append(_T("\\Module\\Boston\\L3L4\\2G3GTEST\\"));
	}
	else if(QISDA_MODULE_NAME == "LTE")
	{
		this->m_Parametermap[ParameterKeyWord::STATIONNAME] = this->m_strCurrentStation = _T("LTE");
		GetModuleFileName(NULL, this->m_Parametermap[ParameterKeyWord::WORKINGDIR].GetBufferSetLength(MAX_PATH+1), MAX_PATH);
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR].ReleaseBuffer();
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR] = this->m_Parametermap[ParameterKeyWord::WORKINGDIR].Left(this->m_Parametermap[ParameterKeyWord::WORKINGDIR].ReverseFind('\\'));
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR].Append(_T("\\Module\\Boston\\L3L4\\LTE\\"));
	}
	else if(QISDA_MODULE_NAME == "BTWLAN")
	{
		this->m_Parametermap[ParameterKeyWord::STATIONNAME] = this->m_strCurrentStation = STATION_BTWLAN;
		GetModuleFileName(NULL, this->m_Parametermap[ParameterKeyWord::WORKINGDIR].GetBufferSetLength(MAX_PATH+1), MAX_PATH);
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR].ReleaseBuffer();
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR] = this->m_Parametermap[ParameterKeyWord::WORKINGDIR].Left(this->m_Parametermap[ParameterKeyWord::WORKINGDIR].ReverseFind('\\'));
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR].Append(_T("\\Module\\Boston\\L3L4\\BTWLAN\\"));
	}
	else if(QISDA_MODULE_NAME == "CSDEM")
	{
		this->m_Parametermap[ParameterKeyWord::STATIONNAME] = this->m_strCurrentStation = STATION_CSDEM;
		GetModuleFileName(NULL, this->m_Parametermap[ParameterKeyWord::WORKINGDIR].GetBufferSetLength(MAX_PATH+1), MAX_PATH);
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR].ReleaseBuffer();
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR] = this->m_Parametermap[ParameterKeyWord::WORKINGDIR].Left(this->m_Parametermap[ParameterKeyWord::WORKINGDIR].ReverseFind('\\'));
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR].Append(_T("\\Module\\4GEE Action CAM\\L1L2\\CSDEM\\"));
	}
	else if(QISDA_MODULE_NAME == "CSDWRITE")
	{
		this->m_Parametermap[ParameterKeyWord::STATIONNAME] = this->m_strCurrentStation = STATION_CSDWRITE;
		GetModuleFileName(NULL, this->m_Parametermap[ParameterKeyWord::WORKINGDIR].GetBufferSetLength(MAX_PATH+1), MAX_PATH);
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR].ReleaseBuffer();
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR] = this->m_Parametermap[ParameterKeyWord::WORKINGDIR].Left(this->m_Parametermap[ParameterKeyWord::WORKINGDIR].ReverseFind('\\'));
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR].Append(_T("\\Module\\BROOK\\L1L2\\CSDWRITE"));
	}
	else if(QISDA_MODULE_NAME == "CSDWRITE_SN")
	{
		this->m_Parametermap[ParameterKeyWord::STATIONNAME] = this->m_strCurrentStation = STATION_CSDWRITE_SN;
		GetModuleFileName(NULL, this->m_Parametermap[ParameterKeyWord::WORKINGDIR].GetBufferSetLength(MAX_PATH+1), MAX_PATH);
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR].ReleaseBuffer();
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR] = this->m_Parametermap[ParameterKeyWord::WORKINGDIR].Left(this->m_Parametermap[ParameterKeyWord::WORKINGDIR].ReverseFind('\\'));
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR].Append(_T("\\Module\\MARCO\\L1L2\\CSDWRITE"));
	}
	else if(QISDA_MODULE_NAME == "CSDREAD_SN")
	{
		this->m_Parametermap[ParameterKeyWord::STATIONNAME] = this->m_strCurrentStation = STATION_CSDREAD_SN;
		GetModuleFileName(NULL, this->m_Parametermap[ParameterKeyWord::WORKINGDIR].GetBufferSetLength(MAX_PATH+1), MAX_PATH);
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR].ReleaseBuffer();
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR] = this->m_Parametermap[ParameterKeyWord::WORKINGDIR].Left(this->m_Parametermap[ParameterKeyWord::WORKINGDIR].ReverseFind('\\'));
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR].Append(_T("\\Module\\MARCO\\L1L2\\CSDREAD"));
	}
	/*
	else if(QISDA_MODULE_NAME == "OS_DL")
	{
		this->m_Parametermap[ParameterKeyWord::STATIONNAME] = this->m_strCurrentStation = STATION_OS_DL;
		GetModuleFileName(NULL, this->m_Parametermap[ParameterKeyWord::WORKINGDIR].GetBufferSetLength(MAX_PATH+1), MAX_PATH);
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR].ReleaseBuffer();
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR] = this->m_Parametermap[ParameterKeyWord::WORKINGDIR].Left(this->m_Parametermap[ParameterKeyWord::WORKINGDIR].ReverseFind('\\'));
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR].Append(_T("\\Module\\Boston\\L3L4\\OS_DL\\"));
		this->m_Parametermap[_T("DLMODE")] = _T("ReDL");
	}*/
}

CServiceToolController::~CServiceToolController()
{
}

size_t CServiceToolController::SetParameterValue(char* sz_keyword, char* sz_value)
{
	std::string key = sz_keyword;

	if (key == "XMLCMDItem")
	{
		this->m_Parametermap[_T("XMLCMDItem")] = CA2CT(sz_value);
		SetTestItemXMLValueForCSDWrite();
	}
	//CString strVlaue = CA2CT(sz_value);
	if (key == "image_path") {
		//this->m_Parametermap[_T("IMAGEPATH")] = strVlaue.Right(strVlaue.GetLength() - strVlaue.ReverseFind('\\') - 1);
		this->m_Parametermap[_T("IMAGEPATH")] = CA2CT(sz_value);
	}
	return NO_ERROR;
}

size_t CServiceToolController::SetTestItemXMLValueForCSDWrite()
{
	size_t ret = NOERROR;
	CString cstrTestItemPath;
	CMyMSXML m_TestItemXML;

	if (this->m_Parametermap[ParameterKeyWord::STATIONNAME] == STATION_2G3GTEST )
	{
		cstrTestItemPath = this->m_Parametermap[ParameterKeyWord::WORKINGDIR] + 
												_T("Qisda\\") + 
												this->m_Parametermap[ParameterKeyWord::MODELNAME] + 
												_T("_") + 
												this->m_Parametermap[ParameterKeyWord::STATIONNAME] + 
												_T("_TestItem_CMW.xml");
	}
	else
	{
		cstrTestItemPath = this->m_Parametermap[ParameterKeyWord::WORKINGDIR] + 
												_T("\\Qisda\\") + 
												this->m_Parametermap[ParameterKeyWord::MODELNAME] + 
												_T("_") + 
												this->m_Parametermap[ParameterKeyWord::STATIONNAME] + 
												_T("_TestItem.xml");
	}

	if (::_taccess(cstrTestItemPath, 0) == 0)
	{
		if (m_TestItemXML.Load(cstrTestItemPath) != ERROR_SUCCESS)
		{
			//ret = LOAD_TESTITEM_FAIL;
			ret = 0x00020000;
		}
	}
	else
	{
		//ret = LOAD_TESTITEM_FAIL;
		ret = 0x00020000;
	}

	if (NOERROR == ret)
	{
		//Write test item parameter

		XMLNode TagNode = m_TestItemXML.SearchNode(_T("//Configuration//ProcessObjectSet//WriteCSDTagObjects//ProcessObject//XMLCMDItem"));
		if (TagNode != NULL)
		{
			m_TestItemXML.SetNodeText(TagNode, this->m_Parametermap[_T("XMLCMDItem")]);
		}
		m_TestItemXML.Save();
	}

	return ret;
}

size_t CServiceToolController::GetParameterValue(char* sz_keyword, char* sz_value)
{
	std::string key = sz_keyword;
	if(QISDA_MODULE_NAME == "BBTEST_NFC" || QISDA_MODULE_NAME == "BBTEST_nonNFC")
	{
		if (key == "msg_process") {
			char* msg = "BBTEST \r\n"
						"\r\n"
						"1. Please make sure the image in the PCBA is Factory image. \r\n"
						"    If necessary, using \"Download\" function in L1L2 to download Factory image first please. \r\n"
						"2. Please make sure the equipment setting is correct. \r\n"
						"3. Put the PCBA on the RIGHT site (BB) of the fixture and close it. \r\n"
						"4. Click \"RUN\" to start BBTEST process and wait for process finish. \r\n"
						"5. Please open the fixture and close it before executing the same function or others. \r\n"
						"6. Please turn off the power supply manually if test FAIL. \r\n";
			strncpy(sz_value, msg, strlen(msg));
		}
		else if (key == "msg_warning") {
			char* msg = "";
			strncpy(sz_value, msg, strlen(msg));
		}
	}
	else if(QISDA_MODULE_NAME == "2G3GTEST")
	{
		if (key == "msg_process") {
			char* msg = "2G3GTEST \r\n"
						"\r\n"
						"1. Please make sure the image in the PCBA is Factory image. \r\n"
						"    If necessary, using \"Download\" function in L1L2 to download Factory image first please. \r\n"
						"2. Please make sure the equipment setting is correct. \r\n"
						"2. Put the PCBA on the LEFT site (RF) of the fixture and close it. \r\n"
						"4. Click \"RUN\" to start 2G3GTEST process and wait for process finish. \r\n"
						"5. Please open the fixture and close it before executing the same function or others. \r\n"
						"6. Please turn off the power supply manually if test FAIL. \r\n";
			strncpy(sz_value, msg, strlen(msg));
		}
		else if (key == "msg_warning") {
			char* msg = "";
			strncpy(sz_value, msg, strlen(msg));
		}
	}
	else if(QISDA_MODULE_NAME == "LTE")
	{
		if (key == "msg_process") {
			char* msg = "LTE \r\n"
						"\r\n"
						"1. Please make sure the image in the PCBA is Factory image. \r\n"
						"    If necessary, using \"Download\" function in L1L2 to download Factory image first please. \r\n"
						"2. Please make sure the equipment setting is correct. \r\n"
						"2. Put the PCBA on the LEFT site (RF) of the fixture and close it. \r\n"
						"4. Click \"RUN\" to start LTE process and wait for process finish. \r\n"
						"5. Please open the fixture and close it before executing the same function or others. \r\n"
						"6. Please turn off the power supply manually if test FAIL. \r\n";
			strncpy(sz_value, msg, strlen(msg));
		}
		else if (key == "msg_warning") {
			char* msg = "";
			strncpy(sz_value, msg, strlen(msg));
		}
	}
	else if(QISDA_MODULE_NAME == "BTWLAN")
	{
		if (key == "msg_process") {
			char* msg = "BTWLAN \r\n"
						"\r\n"
						"1. Please make sure the image in the PCBA is Factory image. \r\n"
						"    If necessary, using \"Download\" function in L1L2 to download Factory image first please. \r\n"
						"2. Please make sure the equipment setting is correct. \r\n"
						"2. Put the PCBA on the LEFT site (RF) of the fixture and close it. \r\n"
						"4. Click \"RUN\" to start BTWLAN process and wait for process finish. \r\n"
						"5. Please open the fixture and close it before executing the same function or others. \r\n"
						"6. Please turn off the power supply manually if test FAIL. \r\n";
			strncpy(sz_value, msg, strlen(msg));
		}
		else if (key == "msg_warning") {
			char* msg = "";
			strncpy(sz_value, msg, strlen(msg));
		}
	}
	else if(QISDA_MODULE_NAME == "CSDEM")
	{
		if (key == "msg_process") {
			char* msg = "Service CSDEM \r\n"
						"Need to ˇ§Open Authorizationˇ¨ before process this function.\r\n"
						"1. Power on the device. \r\n"
						"2. Connect the device to PC through USB cable. \r\n"
						"3. Click \"RUN\" to start process. \r\n"
						"4. Follow the pop-up message to test. \r\n"
						"Must process the ˇ§ Close Authorizationˇ¨ function after the repair process. \r\n";
			strncpy(sz_value, msg, strlen(msg));
		}
	}
	else if(QISDA_MODULE_NAME == "CSDREAD_SN")
	{
		if (key == "SN") {
			this->m_pITI->GetParameterValue(_T("SN"), sz_value, 0);
		}

		else if (key == "msg_warning") {
			char* msg = "";
			strncpy(sz_value, msg, strlen(msg));
		}
	}

	return NO_ERROR;
}

void CServiceToolController::SetCallSendMsg(LPVOID pfunc)
{
	m_fpSendMsg = (MsgCallBack)pfunc;
}

void CServiceToolController::SendMsg(const char *szMsg, const char *szTag)
{
	if (m_fpSendMsg != NULL)
	{
		m_fpSendMsg(szMsg, szTag);
	}
}

void CServiceToolController::SetCallSetPort(LPVOID pfunc)
{
	m_fpSetPort = (PortCallBack)pfunc;
}

void CServiceToolController::SetPort(const char *szPort)
{
	if (m_fpSetPort != NULL)
	{
		m_fpSetPort(szPort);
	}
}

void CServiceToolController::SetCallSendResult(LPVOID pfunc)
{
	m_fpSendResult = (ResultCallBack)pfunc;
}

void CServiceToolController::SendResult(const int nIndex, const char* szUnit, const char* szLower, const char* szUpper, const char* szMeasured, const char* szResult, const char* szErrCode, const char* szMsg)
{
	if (m_fpSendResult != NULL)
	{
		m_fpSendResult(nIndex, szUnit, szLower, szUpper, szMeasured, szResult, szErrCode, szMsg);
	}
}

int CServiceToolController::GetInfoCount()
{
	return (int)m_vTestItemList.size();
}

int CServiceToolController::GetInfoByIndex(int nIndex, char *szName, char *szInfo)
{
	strcpy(szName, CT2A(m_vTestItemList[nIndex].GetString()));
	return NO_ERROR;
}

void CServiceToolController::SetShowPopupMsg(LPVOID pfunc)
{
	m_fpSetShowPopupMsg = (ShowPopupMsgCallBack)pfunc;
}

int CServiceToolController::SendShowPopupMsg(const char *szMsg)
{
	int ret;
	if (m_fpSetShowPopupMsg != NULL)
	{
		ret = m_fpSetShowPopupMsg(szMsg);
	}
	return ret;
}

/*
size_t CServiceToolController::RegCallback( p_fn_Callback p_cb )
{
	if (!p_cb)
	{
		return false;
	}

	m_cbMsgDispatch = p_cb;

	return NOERROR;
}
*/
void CServiceToolController::Event(const char* str_event, long nParam)
{
	CStringA cstrFullMsg;

	if (strcmp(str_event, UI_ITEM) == 0)
	{
		st_UIItem* p_st_uiItem = (st_UIItem*)nParam;
		for (size_t i = 0; i < m_vTestItemList.size(); i++)
		{
			if(strcmp(p_st_uiItem->sz_item, CT2A(m_vTestItemList[i].GetString())) == 0)
				nTestItemIndex = i;
		}
	}

	if (strcmp(str_event, UI_RESULT) == 0)
	{
		st_UIResult* p_st_uiResult = (st_UIResult*)nParam;

		if(nTestItemIndex < m_vTestItemList.size())
		{
			SendResult(nTestItemIndex, p_st_uiResult->sz_unit, p_st_uiResult->sz_lower, p_st_uiResult->sz_upper,
				p_st_uiResult->sz_measured, p_st_uiResult->sz_result, p_st_uiResult->sz_errCode, p_st_uiResult->sz_msg);

			if(strcmp(p_st_uiResult->sz_result, "FAIL") == 0)
			{
				nTestItemIndex = 9999;
			}
		}
		else
		{
			if(strcmp(p_st_uiResult->sz_result, "PASS") == 0)
			{
				SendMsg(p_st_uiResult->sz_msg, "PASS");
			}
			else if(strcmp(p_st_uiResult->sz_result, "FAIL") == 0)
			{
				SendMsg(p_st_uiResult->sz_msg, "FAIL");
			}
			else// if(strcmp(p_st_uiResult->sz_result, "") == 0)
			{
				SendMsg(p_st_uiResult->sz_msg, "INFO");
			}
		}
	}

	else if (strcmp(str_event, UI_MESSAGE) == 0)
	{
		st_UIMessage* p_st_uiMessage = (st_UIMessage*)nParam;
		SendMsg(p_st_uiMessage->sz_message, "INFO");
	}

	else if (strcmp(str_event, UI_SHOWDIALOG) == 0)
	{
		st_ShowDialog* p_st_showDialog = (st_ShowDialog*)nParam;
		int nResult = SendShowPopupMsg(p_st_showDialog->sz_message);
		char szRet[10] = {0};
		sprintf(szRet,"%d",nResult);
		CEnhanceController::Enhance_SetResopnseToShowDlg(szRet);
	}

}

size_t CServiceToolController::Enhance_OnAPPLaunch()
{
	bool bRet = false;
	bProcessResult = true;
	size_t ret = this->MakeEnhanceController();

	if(NOERROR == ret)
		ret = CEnhanceController::Enhance_OnAPPLaunch();

	if(NOERROR == ret)
		this->RegistEvent();

	if(NOERROR == ret)
		ret = this->SetParameterToDLL();

	if(NOERROR == ret)
		bRet = this->m_pITI->OnAPPLaunch();
/*
	if((QISDA_MODULE_NAME == "OS_DL") && bRet)
	{
		char szSectorData[512];
		bRet = this->m_pITI->GetFASector(0, 0, szSectorData, 512);
	}
*/
	if (bRet == true && ret == NOERROR)
	{
		SendMsg("\nEnhance_OnAPPLaunch finished.\n", "INFO");
		return NOERROR;
	}
	else
	{
		bProcessResult = false;
		SendMsg("\nEnhance_OnAPPLaunch failed.\n", "INFO");
		return (ret == NOERROR) ? E_FAIL : ret;
	}
}

size_t CServiceToolController::Enhance_OnRunInit(int i_slot)
{
	size_t ret = CEnhanceController::Enhance_OnRunInit(i_slot);
	if(NOERROR == ret)
	{
		SendMsg("\nEnhance_OnRunInit finished.\n", "INFO");
	}
	else
	{
		bProcessResult = false;
		SendMsg("\nEnhance_OnRunInit failed.\n", "INFO");
	}
	return ret;
}

size_t CServiceToolController::Enhance_RunTest(int i_slot)
{
	size_t ret = CEnhanceController::Enhance_RunTest(i_slot);
	if(NOERROR == ret)
	{
		SendMsg("\nEnhance_RunTest finished.\n", "INFO");
	}
	else
	{
		bProcessResult = false;
		SendMsg("\nEnhance_RunTest failed.\n", "INFO");
	}
	return ret;
}

size_t CServiceToolController::Enhance_OnRunFinish(int i_slot)
{
	size_t ret = CEnhanceController::Enhance_OnRunFinish(i_slot);
	if(NOERROR == ret)
	{
		SendMsg("\nEnhance_OnRunFinish finished.\n", "INFO");
	}
	else
	{
		bProcessResult = false;
		SendMsg("\nEnhance_OnRunFinish failed.\n", "INFO");
	}
	return ret;
}

size_t CServiceToolController::Enhance_OnAPPEnd()
{
	size_t ret = CEnhanceController::Enhance_OnAPPEnd();
	if(NOERROR == ret)
	{
		SendMsg("\nEnhance_OnAPPEnd finished.\n", "INFO");
	}
	else
	{
		bProcessResult = false;
		SendMsg("\nEnhance_OnAPPEnd failed.\n", "INFO");
	}

	if(bProcessResult)
		SendMsg("", "PASS");
	else
		SendMsg("", "FAIL");
	return ret;
}
/*
size_t CServiceToolController::MsgHandler( const wchar_t* target, const wchar_t* msg_key , const wchar_t* msg_value )
{
	if(0 == _tcscmp(msg_key, CommonPageControl::BroadcastEvent::CurrentDirectory))
	{
		this->m_Parametermap[ParameterKeyWord::WORKINGDIR]= msg_value;
		this->m_Parametermap[ParameterKeyWord::CONFIGDIR] = this->m_Parametermap[ParameterKeyWord::WORKINGDIR] + _T("Qisda\\");
	}

	return NOERROR;
}
*/

size_t CServiceToolController::SetParameterToDLL()
{
	if(!this->m_pITI->SetParameterValue(ParameterKeyWord::MODELNAME, this->m_Parametermap[ParameterKeyWord::MODELNAME].GetString()))
	{
		//some log
		return 1;
	}

	if(!this->m_pITI->SetParameterValue(ParameterKeyWord::STATIONNAME, this->m_Parametermap[ParameterKeyWord::STATIONNAME].GetString()))
	{
		//some log
		return 2;
	}

	if(!this->m_pITI->SetParameterValue(ParameterKeyWord::WORKINGDIR, this->m_Parametermap[ParameterKeyWord::WORKINGDIR].GetString()))
	{
		//some log
		return 3;
	}

	if(!this->m_pITI->SetParameterValue(ParameterKeyWord::CONFIGDIR,  this->m_Parametermap[ParameterKeyWord::CONFIGDIR].GetString()))
	{
		//some log
		return 4;
	}

	if(!this->m_pITI->SetParameterValue(_T("UI_LOG_PATH"),  _T("D:\\Log\\UI_RD")))
	{
		//some log
		return 5;
	}

	//if(!this->m_pITI->SetParameterValue(_T("TOOL_MODE"),  _T("RD")))
	//{
	//	//some log
	//	return 6;
	//}
/*
	if(QISDA_MODULE_NAME == "OS_DL")
	{
		if(!this->m_pITI->SetParameterValue(_T("IMAGEPATH"),  this->m_Parametermap[_T("IMAGEPATH")].GetString()))
		{
			//some log
			return 7;
		}

		if(!this->m_pITI->SetParameterValue(_T("DLMODE"),  _T("ReDL")))
		{
			//some log
			return 8;
		}
	}
*/
	return NOERROR;

}

size_t CServiceToolController::MakeEnhanceController()
{
	this->FreeNextEC();

	//Necessary enhance
	CEnhanceController *pToolLoader = new CToolLoader(NULL);

	//extra enhance
	if(	this->m_strCurrentStation == STATION_BBTEST ||
		//this->m_strCurrentStation == STATION_2GPTEST ||
		//this->m_strCurrentStation == STATION_3GPTEST ||
		this->m_strCurrentStation == STATION_2G3GTEST ||
		this->m_strCurrentStation == STATION_4GPTEST ||
		this->m_strCurrentStation == STATION_BTWLAN ||
		this->m_strCurrentStation == STATION_CSDEM ||
		this->m_strCurrentStation == STATION_CSDWRITE ||
		this->m_strCurrentStation == STATION_CSDWRITE_SN ||
		this->m_strCurrentStation == STATION_CSDREAD_SN ||
		//this->m_strCurrentStation == STATION_CURRENT ||
		//this->m_strCurrentStation == STATION_ONLINE_WLS ||
		//this->m_strCurrentStation == STATION_ONLINE_WLS2 ||
		//this->m_strCurrentStation == "SPRD" ||
		this->m_strCurrentStation == "LTE")	
	{
		//CEnhanceController *pTestBoxHdrStd = new CTestBoxHdrStd(pPhoneHdrStd);
		//CEnhanceController *pPowerSupplyHdrStd = new CPowerSupplyHdrStd(pTestBoxHdrStd);
		//CEnhanceController *pGPIBHdrStd = new CGPIBHdrStd(pPowerSupplyHdrStd);
		//CEnhanceController *pCablelossHdrStd = new CCablelossHdrStd(pGPIBHdrStd);
		CEnhanceController *pTestItemHdrStd = new CTestItemHdrStd(pToolLoader);

		this->SetNextEC(pTestItemHdrStd);
	}/*
	else if(this->m_strCurrentStation == STATION_OS_DL ||
		this->m_strCurrentStation == STATION_REDL ||
		this->m_strCurrentStation == _T("ReDL"))
	{
		CEnhanceController *pPhoneHdrStd = new CPhoneHdrStd(pToolLoader);
		CEnhanceController *pDownLoadHdrStd = new CDownLoadHdrStd(pPhoneHdrStd);

		this->SetNextEC(pDownLoadHdrStd);
	}*/

	return NOERROR;
}

void CServiceToolController::DefineNotifyEvent()
{
	this->m_vEvent.clear();

	//this->m_vEvent.push_back(UI_PROGRESS);
	this->m_vEvent.push_back(UI_MESSAGE);
	//this->m_vEvent.push_back(UI_PATTERN);
	this->m_vEvent.push_back(UI_ITEM);
	this->m_vEvent.push_back(UI_RESULT);
	//this->m_vEvent.push_back(UI_PICSDATA);
	//this->m_vEvent.push_back(UI_JIGCONTROL);
	this->m_vEvent.push_back(UI_SHOWDIALOG);
	//this->m_vEvent.push_back(UI_RELAYBOARD);
	//this->m_vEvent.push_back(UI_POWERSUPPLY);

	for(std::vector <CStringA>::iterator iter = this->m_vEvent.begin(); iter != this->m_vEvent.end(); iter++)
	{
		NotifyBase_Char_Public.DefineNotify(iter->GetString());
	}
}

void CServiceToolController::RegistEvent()
{
	for(std::vector <CStringA>::iterator iter = this->m_vEvent.begin(); iter != this->m_vEvent.end(); iter++)
	{
		this->m_pITI->Register(this, iter->GetString());
	}
}