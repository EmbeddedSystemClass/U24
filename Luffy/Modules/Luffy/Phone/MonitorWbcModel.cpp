#include "stdafx.h"
#include "MonitorWbcModel.h"

CMonitorWbcModel::CMonitorWbcModel(void)
{	m_szWbcFileName = "";
	m_szDdcFileName = "";
	m_szMonitorPort = "";
	m_szErrMsg = "";
}

CMonitorWbcModel::~CMonitorWbcModel(void)
{
}

bool CMonitorWbcModel::SearchWbcFileName(std::string szPartNo)
{
	unsigned char sz9J[31] = "";
	unsigned char szWBCFileName[31] = "";
	unsigned char szModelName[31] = "";
	unsigned char szDDCFileName[31] = "";
	unsigned char szInfo[31] = "";
	unsigned char szSWInfo[31] = "";
	unsigned char szPort[31] = "";

	m_szErrMsg ="";
	m_szWbcFileName = "";
	m_szDdcFileName = "";
	m_szMonitorPort = "";
	m_szInfo = "";

	CString str_dllF32SERVER2 = F32SERVERDB;
	HINSTANCE hDll;
	hDll = LoadLibrary(str_dllF32SERVER2);
	if(hDll != NULL)
	{	typedef void (_stdcall *lpGetMonitorInfoByPartNo)(const unsigned char* PartNo,	   unsigned short PartNoLen,
																unsigned char* WBCFileName, unsigned short WBCFileNameLen,
																unsigned char* ModelName,   unsigned short ModelNameLen,
																unsigned char* DDCFileName, unsigned short DDCFileNameLen,
																unsigned char* Info,		   unsigned short InfoLen,
																unsigned char* SWInfo,	   unsigned short SWInfoLen,
																unsigned char* Port,        unsigned short PortLen
																//unsigned char* USB,		 unsigned short USBLen
														 ); 
		lpGetMonitorInfoByPartNo iGetMonitorInfoByPartNo = (lpGetMonitorInfoByPartNo)::GetProcAddress(hDll,"GetMonitorInfoByPartNo");
		if(iGetMonitorInfoByPartNo != NULL)
		{	sprintf_s((char*)sz9J,31,"%s",szPartNo.c_str());
			iGetMonitorInfoByPartNo(sz9J,31,
									szWBCFileName,31,
									szModelName,31,
									szDDCFileName,31,
									szInfo,31,
									szSWInfo,31,
									szPort,31
								   );
			FreeLibrary(hDll);
			m_szWbcFileName = (char*)szWBCFileName;
			m_szDdcFileName = (char*)szDDCFileName;
			m_szMonitorPort = (char*)szPort;
			m_szInfo = (char*)szInfo;
			return true;
		}
		else
		{	m_szErrMsg = "Load address of GetMonitorInfoByPartNo fail.";
			FreeLibrary(hDll);
			return false;
		}
	}
	else
	{	m_szErrMsg = "Load f32server2.dll fail.";
		return false;
	}
}

std::string CMonitorWbcModel::GetWbcFileName(void)
{	return m_szWbcFileName;
}

std::string CMonitorWbcModel::GetDdcFileName(void)
{	return m_szDdcFileName;
}

std::string CMonitorWbcModel::GetMonitorPort(void)
{	return m_szMonitorPort;
}

std::string CMonitorWbcModel::GetErrMsg(void)
{	return m_szErrMsg;
}

std::string CMonitorWbcModel::GetMonitorEep(void)
{	return m_szInfo;
}
