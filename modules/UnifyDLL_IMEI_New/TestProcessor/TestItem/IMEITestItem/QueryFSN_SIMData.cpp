#include "stdafx.h"
#include "QueryFSN_SIMData.h"
#include <atlenc.h>

RF_IMPLEMENT_DYNCREATE(CQueryFSN_SIMData)

bool CQueryFSN_SIMData::PreRun()
{
	m_strResult = "";
	m_strItemCode = "";
	m_strErrorCode = "";
	m_strBand = "";
	m_strChannel = "";
	m_strUpper = "";
	m_strLower = "";
	m_strMeasured = "";
	m_strUnit = "";
	m_strMessage = "";
	return true;
}

bool CQueryFSN_SIMData::Run()
{
	m_hSSSO2Handle = ::LoadLibrary("Pics_SSSO2_Factory.dll");

	bool bRes = MainFunction();

	if (m_hSSSO2Handle != NULL)
	{
		FreeLibrary(m_hSSSO2Handle);
		m_hSSSO2Handle = NULL;
	}
	return bRes;
}

bool CQueryFSN_SIMData::PostRun()
{
	return true;
}

bool CQueryFSN_SIMData::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find(std::string("RetryTimes")) != paramMap.end())
	{
		m_iRetry = atoi(paramMap[std::string("RetryTimes")].c_str());
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to find 'RetryTimes' for class 'CQueryFSN_SIMData' default Retry=3"));
		m_iRetry = 3;
	}

	if (paramMap.find(std::string("SIMType")) == paramMap.end())
	{
		TraceLog(MSG_INFO, _T("Failed to find 'SIMType' for class 'CQueryFSN_SIMData'"));
		return false;
	}
	strcpy(m_szSIMType, paramMap[std::string("SIMType")].c_str());

	if (paramMap.find(std::string("LocalAssign")) == paramMap.end())
	{
		TraceLog(MSG_INFO, _T("Failed to find 'LocalAssign' for class 'CQueryFSN_SIMData. Get XML Name From Server"));
	}
	m_bNameServer = true;
	std::string strTmp;
	if(paramMap[std::string("LocalAssign")].compare("TRUE") == 0 || paramMap[std::string("LocalAssign")].compare("true") == 0)
	{
		m_bNameServer = false;
		if (paramMap.find(std::string("AssignXML")) == paramMap.end())
		{
			TraceLog(MSG_INFO, _T("Failed to find 'AssignXML' for class 'CQueryFSN_SIMData'"));
			return false;
		}
		m_strXMLName = paramMap[std::string("AssignXML")];
	}
	
	return true;
}

bool CQueryFSN_SIMData::MainFunction()
{
	TraceLog(MSG_INFO, "Query FSN SIMData START");
	
	strcpy(m_szIMEI, g_strIMEI.c_str());

	// 1. Query FSN Code from SSS02 //
	if(!QueryFSNFromPICS()) return false;

	if(!DealFSN()) return false;
	if(!QuerySIMData())	return false;

	m_strMessage = "Query FSN SIMData success...";
	m_strResult = "PASS";
	TraceLog(MSG_INFO, m_strMessage);
	FactoryLog();
	return true;
}
bool CQueryFSN_SIMData::QuerySIMData()
{
	char szSIMFileName[128];
	if(m_bNameServer)
	{
		strcpy(szSIMFileName, m_mapQueryData["SIMFileName"].c_str());
		m_strMessage = "SIM File Name takes from server";
		FactoryLog();
	}
	else
	{
		strcpy(szSIMFileName, m_strXMLName.c_str());
		m_strMessage = "SIM File Name takes from local";
		FactoryLog();
	}

	m_strMessage = MakeStr("SIM File: %s", szSIMFileName);
	FactoryLog();

	// 1. Get SIM Data From Server //
	char szSimDatalength[10] = {0};
	strcpy(szSimDatalength, "1024");
	char szSimDataFromPICS[FTD_BUF_SIZE] = {0};
	char szStatus[FTD_BUF_SIZE] = {0};

	PFGetSIMData_PICS fpGetSIM = (PFGetSIMData_PICS)::GetProcAddress(m_hSSSO2Handle, "GetSIMData_PICS");
	if(fpGetSIM == NULL)
	{
		m_strMessage = "Load GetSIMData_PICS fuction fail.";
		m_strErrorCode = CommErr_Query_Sim_Data_From_PICS_Fail;
		FactoryLog();
		return false;
	}
	if( !fpGetSIM(m_szSIMType, m_szIMEI, m_szFSN, szSIMFileName, szSimDataFromPICS, szSimDatalength, szStatus))
	{
		m_strResult = "FAIL";
		m_strMessage = MakeStr("Get SIM data from PICS fail...Status: %s", szStatus);
		m_strErrorCode = CommErr_Query_Sim_Data_From_PICS_Fail;
		FactoryLog();
		return false;
	}

	m_strMessage = MakeStr("SIM data length get from sever: %s", szSimDatalength);
	FactoryLog();
	m_strMessage = MakeStr("SIM Data: %s", szSimDataFromPICS);
	FactoryLog();
	m_strMessage = "Get SIM data from PICS success...";
	FactoryLog();


	// 2. Fixed SIM Length //
	char szSplitSimData[FTD_BUF_SIZE] = {0};
	CString CStrSplitSimData(szSimDataFromPICS);
	int nLength_SIMData = (2 * atoi(szSimDatalength));
	CString csSIMData = CStrSplitSimData.Left(nLength_SIMData);
	strcpy(szSplitSimData, (LPCTSTR)csSIMData);
	m_strMessage = MakeStr("Split Sim Data: %s", szSplitSimData);
	FactoryLog();


	// 3. Take Public Key //
	char szSplitPublicKey[FTD_BUF_SIZE] = {0};
	CString CStrPublicKey(szSimDataFromPICS);
	CString csPublicKey = CStrPublicKey.Left(256);
	strcpy(szSplitPublicKey, (LPCTSTR)csPublicKey);
	m_strMessage = MakeStr("Split Public Key: %s", szSplitPublicKey);
	FactoryLog();

	m_mapQueryData["SIMData"] = szSplitSimData;
	m_mapQueryData["SIMPublicKey"] = szSplitPublicKey;

	return true;
}

bool CQueryFSN_SIMData::QueryFSNFromPICS()
{
	m_strMessage = MakeStr("Query FSN form PICS by IMEI---> %s", m_szIMEI);
	FactoryLog();	
	PFQueryFSNCode_PICS fpQuery = (PFQueryFSNCode_PICS)::GetProcAddress(m_hSSSO2Handle, "QueryFSNCode_PICS");
	if(fpQuery == NULL)
	{
		m_strMessage = "Load QueryFSNCode_PICS fuction fail.";
		m_strErrorCode = CommErr_Query_FSN_From_PICS_Fail;
		FactoryLog();
		return false;
	}

	char szStatus[FTD_BUF_SIZE] = {0};

	if( !fpQuery(m_szIMEI, m_szFSN, szStatus))
	{
		if(strcmp(szStatus, "-10") == 0)
		{
			// If Status = -10, Try another one.
			m_strMessage = "Use GetFSNFromPICS fuction.";
			FactoryLog();
			if(!GetFSNFromPICS()) return false;
		}
		else
		{
			m_strMessage = "Query FSN from PICS fail.";
			m_strErrorCode = CommErr_Query_FSN_From_PICS_Fail;
			FactoryLog();
			return false;
		}
	}
	m_strMessage = MakeStr("FSN-------> %s", m_szFSN);
	FactoryLog();
	return true;
}
bool CQueryFSN_SIMData::GetFSNFromPICS()
{
	char szStatus[FTD_BUF_SIZE] = {0};

	m_strMessage = MakeStr("Get FSN form PICS by IMEI---> %s", m_szIMEI);
	FactoryLog();
	PFGetFSNCode_PICS fpGetFSN = (PFGetFSNCode_PICS)::GetProcAddress(m_hSSSO2Handle, "GetFSNCode_PICS");
	if(fpGetFSN == NULL)
	{
		m_strResult = "FAIL";
		m_strMessage = "Load QueryFSNCode_PICS fuction fail.";
		m_strErrorCode = CommErr_Create_FSN_From_PICS_Fail;
		FactoryLog();
		return false;
	}
	if( !fpGetFSN(m_szIMEI, m_szFSN, szStatus))
	{
		m_strResult = "FAIL";
		m_strMessage = MakeStr("Get FSN from PICS fail, Status: %s", szStatus);
		m_strErrorCode = CommErr_Create_FSN_From_PICS_Fail;
		FactoryLog();
		return false;
	}
	return true;
}
bool CQueryFSN_SIMData::DealFSN()
{
	char szFSN_Base64[FTD_BUF_SIZE] = {0};
	char szFSN_D16[FTD_BUF_SIZE] = {0};
	char szErrCode[FTD_BUF_SIZE] = {0};
	char szErrMsg[FTD_BUF_SIZE] = {0};
	
	if( !DealPICS_FSN(m_szFSN, szFSN_Base64, szFSN_D16, "16", szErrCode, szErrMsg))
	{
		m_strResult = "FAIL";
		m_strErrorCode = szErrCode;
		m_strMessage = szErrMsg;
		FactoryLog();
		return false;
	}

	m_mapQueryData["FSN"] = m_szFSN;
	m_mapQueryData["FSN_D16"] = szFSN_D16;

	m_strMessage = szErrMsg;
	FactoryLog();
	m_strMessage = "Deal FSN from PICS success...";
	FactoryLog();
	return true;
}

bool CQueryFSN_SIMData::DealPICS_FSN(char *szFSN_D32_Pics, char *szFSN_Base64_Pics, char *szFSN_D16_Pics, char* szLength, char *szErrorCode, char *szErrorString)
{
	unsigned char Device_Serial_Number_Digit[33]={0};
	unsigned char Device_Serial_Number[17]={0};
	sprintf(szErrorString,"FSN Get From PICS: %s", szFSN_D32_Pics);
	int nFSNLength = atoi(szLength);

	if(strlen(szFSN_D32_Pics) != nFSNLength*2)
	{
		sprintf(szErrorString,"the length for FSN Get from PICS (%d) is not equal to definition in INI (%d)......", (strlen(szFSN_D32_Pics))/2, nFSNLength);
		strcpy(szErrorCode, "FSN_Siez_fail");
		return false;
	}

	for(int i=0;i<nFSNLength*2;i++)
	{
		Device_Serial_Number_Digit[i] = szFSN_D32_Pics[i];
	}

	Device_Serial_Number_Digit[nFSNLength*2]='\0';

	int i = 0;
	int j = 0;
	int Data = 0;
	while(Device_Serial_Number_Digit[i] != _T('\0'))
	{

		if( (Device_Serial_Number_Digit[i] > (_T('a') - 1)) && (Device_Serial_Number_Digit[i] < (_T('f') + 1)) )
			Data = Data*16 + Device_Serial_Number_Digit[i] - _T('a') + 10;
		else if ( (Device_Serial_Number_Digit[i] > (_T('A') - 1)) && (Device_Serial_Number_Digit[i] < (_T('F') + 1)) )
			Data = Data*16 + Device_Serial_Number_Digit[i] - _T('A') + 10;
		else if ( (Device_Serial_Number_Digit[i] > (_T('0') - 1)) && (Device_Serial_Number_Digit[i] < (_T('9') + 1)) )
			Data = Data*16 + Device_Serial_Number_Digit[i] - _T('0');
		i++;


		if(i % 2 == 0)
		{
			Device_Serial_Number[j] = Data;
			Data = 0;
			j++;
		}
	}

	Device_Serial_Number[nFSNLength]='\0';
	int nDataLen = Base64EncodeGetRequiredLength(nFSNLength*2);
	Base64Encode(Device_Serial_Number, nFSNLength*2, szFSN_Base64_Pics, &nDataLen);
	szFSN_Base64_Pics[nDataLen] ='\0';

	for(i=0; i<nFSNLength+1; i++)
	{
		szFSN_D16_Pics[i] = Device_Serial_Number[i];
	}
	szFSN_D16_Pics[i] = '\0';

	return true;
}