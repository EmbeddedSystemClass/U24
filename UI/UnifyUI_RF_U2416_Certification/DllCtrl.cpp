#include "StdAfx.h"
#include "DllCtrl.h"

CDllCtrl::CDllCtrl(void)
{
	m_hDll = NULL;
	m_hRes = NULL;
}

CDllCtrl::~CDllCtrl(void)
{
}

bool CDllCtrl::Load(CString& dll_path)
{
	TCHAR szNowPath[MAX_PATH] = {0};
	memset(m_szNowPath, 0, MAX_PATH);
	GetCurrentDirectory(MAX_PATH, szNowPath);
	TRACE(_T("current path: %s\n"), szNowPath);
	memcpy(m_szNowPath, szNowPath, MAX_PATH);
	CString WorkDir = dll_path.Left(dll_path.ReverseFind(_T('\\')));
	TRACE(_T("working path: %s\n"), WorkDir);
	SetCurrentDirectory(WorkDir);

	m_hDll = ::LoadLibrary(dll_path);
	if (m_hDll == NULL) {
		return false;
	}
	m_dllQisdaDllNew = NULL;
	m_dllQisdaDllDelete = NULL;
	m_dllSetCallBackMsg = NULL;
	m_dllSetCallBackPort = NULL;
	m_dllSetCallBackResult = NULL;
	m_dllSetCallBackRxResult = NULL;
	m_dllSetParameter = NULL;
	m_dllGetParameter = NULL;
	m_dllGetPortCount = NULL;
	m_dllGetPortByIndex = NULL;
	m_dllGetInfoCount = NULL;
	m_dllGetInfoByIndex = NULL;
	m_dllBegin = NULL;
	m_dllPreRun = NULL;
	m_dllRun = NULL;
	m_dllPostRun = NULL;

	//pfnD_NewDLInterface D_NewDLInterface = (pfnD_NewDLInterface)GetProcAddress(m_hDll, "D_NewDLInterface");
	//if(!D_NewDLInterface) {
	//	return false;
	//}
	//D_NewDLInterface(&m_iDL);
	//if (!m_iDL) {
	//	return false;
	//}

	m_dllQisdaDllNew = (fQisdaDllNew)GetProcAddress(m_hDll, "QisdaDllNew");
	if (m_dllQisdaDllNew == NULL) {
		return false;
	}
	m_dllQisdaDllDelete = (fQisdaDllDelete)GetProcAddress(m_hDll, "QisdaDllDelete");
	if (m_dllQisdaDllDelete == NULL) {
		return false;
	}
	m_dllSetCallBackMsg = (fSetCallBackMsg)GetProcAddress(m_hDll, "SetCallBackMsg");
	if (m_dllSetCallBackMsg == NULL) {
		return false;
	}
	m_dllSetCallBackPort = (fSetCallBackPort)GetProcAddress(m_hDll, "SetCallBackPort");
	if (m_dllSetCallBackPort == NULL) {
		return false;
	}
	m_dllSetCallBackResult = (fSetCallBackResult)GetProcAddress(m_hDll, "SetCallBackResult");
	if (m_dllSetCallBackResult == NULL) {
		//return false;
	}
	m_dllSetCallBackRxResult = (fSetCallBackRxResult)GetProcAddress(m_hDll, "SetCallBackRxResult");
	if (m_dllSetCallBackRxResult == NULL) {
		//return false;
	}
	m_dllSetParameter = (fSetParameter)GetProcAddress(m_hDll, "SetParameter");
	if (m_dllSetParameter == NULL) {
		return false;
	}
	m_dllGetParameter = (fGetParameter)GetProcAddress(m_hDll, "GetParameter");
	if (m_dllGetParameter == NULL) {
		return false;
	}
	m_dllGetPortCount = (fGetPortCount)GetProcAddress(m_hDll, "GetPortCount");
	if (m_dllGetPortCount == NULL) {
		return false;
	}
	m_dllGetPortByIndex = (fGetPortByIndex)GetProcAddress(m_hDll, "GetPortByIndex");
	if (m_dllGetPortByIndex == NULL) {
		return false;
	}
	m_dllGetInfoCount = (fGetInfoCount)GetProcAddress(m_hDll, "GetInfoCount");
	if (m_dllGetInfoCount == NULL) {
		return false;
	}
	m_dllGetInfoByIndex = (fGetInfoByIndex)GetProcAddress(m_hDll, "GetInfoByIndex");
	if (m_dllGetInfoByIndex == NULL) {
		return false;
	}
	m_dllBegin = (fBegin)GetProcAddress(m_hDll, "Begin");
	if (m_dllBegin == NULL) {
		//return false;
	}
	m_dllPreRun = (fPreRun)GetProcAddress(m_hDll, "PreRun");
	if (m_dllPreRun == NULL) {
		return false;
	}
	m_dllRun = (fRun)GetProcAddress(m_hDll, "Run");
	if (m_dllRun == NULL) {
		return false;
	}
	m_dllPostRun = (fPostRun)GetProcAddress(m_hDll, "PostRun");
	if (m_dllPostRun == NULL) {
		return false;
	}

	m_hRes = m_dllQisdaDllNew();
	if (m_hRes == NULL) {
		return false;
	}
	return true;
}

void CDllCtrl::UnLoad()
{
	if (m_hRes != NULL) {
		m_dllQisdaDllDelete(m_hRes);
		m_hRes = NULL;
	}
	::FreeLibrary(m_hDll);
	SetCurrentDirectory(m_szNowPath);
}

int CDllCtrl::SetCallBackMsg(int(*CallBack)(const char*, const char*))
{
	return m_dllSetCallBackMsg(m_hRes, CallBack);
}

int CDllCtrl::SetCallBackPort(int(*CallBack)(const char*))
{
	return m_dllSetCallBackPort(m_hRes, CallBack);
}

int CDllCtrl::SetCallBackResult(int(*CallBack)(const int, const char*, const char*, const char*, const char*, const char*, const char*, const char*))
{
	if (m_dllSetCallBackResult != NULL)
		return m_dllSetCallBackResult(m_hRes, CallBack);
	else
		return 0;
}

int CDllCtrl::SetCallBackRxResult(int(*CallBack)(const char*))
{
	if (m_dllSetCallBackRxResult != NULL)
		return m_dllSetCallBackRxResult(m_hRes, CallBack);
	else
		return 0;
}

int CDllCtrl::SetParameter(char* szKey, char* szValue)
{
	return m_dllSetParameter(m_hRes, szKey, szValue);
}

int CDllCtrl::GetParameter(char* szKey, char* szValue)
{
	return m_dllGetParameter(m_hRes, szKey, szValue);
}

int CDllCtrl::GetPortCount()
{
	return m_dllGetPortCount(m_hRes);
}

int CDllCtrl::GetPortByIndex(int nIndex, char* szPort)
{
	return m_dllGetPortByIndex(m_hRes, nIndex, szPort);
}

int CDllCtrl::GetInfoCount()
{
	return m_dllGetInfoCount(m_hRes);
}

int CDllCtrl::GetInfoByIndex(int nIndex, char* szName, char* szInfo)
{
	return m_dllGetInfoByIndex(m_hRes, nIndex, szName, szInfo);
}

int CDllCtrl::Begin()
{
	return m_dllBegin(m_hRes);
}

int CDllCtrl::PreRun()
{
	return m_dllPreRun(m_hRes);
}

int CDllCtrl::Run()
{
	return m_dllRun(m_hRes);
}

int CDllCtrl::PostRun()
{
	return m_dllPostRun(m_hRes);
}



