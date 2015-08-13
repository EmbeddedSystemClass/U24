#include "stdafx.h"
#include "QueryMSNBt.h"

#define Dummy_IMEI                  "000000011234560"//"123456789012345","449281960000000"

RF_IMPLEMENT_DYNCREATE(CQueryMSNBt)

bool CQueryMSNBt::PreRun()
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

bool CQueryMSNBt::Run()
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

bool CQueryMSNBt::PostRun()
{
	return true;
}

bool CQueryMSNBt::InitData(std::map<std::string, std::string>& paramMap)
{

	if (paramMap.find(std::string("RetryTimes")) != paramMap.end())
	{
		m_iRetry = atoi(paramMap[std::string("RetryTimes")].c_str());
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to find 'RetryTimes' for class 'CQueryMSNBt' default Retry=3"));
		m_iRetry = 3;
	}

	if (paramMap.find(std::string("Station")) == paramMap.end())
	{
		TraceLog(MSG_INFO, _T("Failed to find 'Station' for class 'CQueryMSNBt."));
		return false;
	}
	m_strStation = paramMap[std::string("Station")];
	if((m_strStation.compare("IMEI") != 0) && (m_strStation.compare("FCHECK") != 0))
	{
		TraceLog(MSG_INFO, "Failed to find 'IMEI' or 'FCHECK' in Station for class 'CQueryMSNBt.");
		return false;
	}
	
	return true;
}

bool CQueryMSNBt::MainFunction()
{
	TraceLog(MSG_INFO, "Query BT WLAN START");
	
	char szIMEI[32] = {0};
	strcpy(szIMEI, g_strIMEI.c_str());
	m_strMessage = MakeStr("Query BT GSN & status from server by IMEI/Picasso---> %s", szIMEI);
	FactoryLog();

	// 1. Query BT MAC by IMEI from SSS02 //
	PFQueryIMEI_SSSO2_V2 fpQuery = (PFQueryIMEI_SSSO2_V2)::GetProcAddress(m_hSSSO2Handle, "QueryIMEI_SSSO2_V2");
	if(fpQuery == NULL)
	{
		m_strMessage = "Load QueryIMEI_SSSO2_V2 fuction fail.";
		m_strErrorCode = CommErr_Query_IMEI_From_SSSO2_Fail;
		FactoryLog();
		return false;
	}

	char szMSN[FTD_BUF_SIZE] = {0};
	char szBTAddr[FTD_BUF_SIZE] = {0};
	char szMACAddr[FTD_BUF_SIZE] = {0};
	char szSWver[FTD_BUF_SIZE] = {0};
	char szHWver[FTD_BUF_SIZE] = {0};
	char szPVK[FTD_BUF_SIZE] = {0};
	char szErrCode[FTD_BUF_SIZE] = {0};
	char szErrMsg[FTD_BUF_SIZE] = {0};
	char szStatusOutput[FTD_BUF_SIZE] = {0};

	//Try B Status
	if( !fpQuery("B", szIMEI, szMSN, szBTAddr, szMACAddr, szSWver, szHWver, szPVK, szErrCode, szErrMsg, szStatusOutput))
	{
		TraceLog(MSG_INFO, "Try Query 'B' State fail");

		//Try P Status
		if( !fpQuery("P", szIMEI, szMSN, szBTAddr, szMACAddr, szSWver, szHWver, szPVK, szErrCode, szErrMsg, szStatusOutput))
		{
			TraceLog(MSG_INFO, "Try Query 'P' State fail");
			m_strMessage = "Query BT and GSN from server fail......";
			FactoryLog();
			m_strMessage = MakeStr("Error code: %s, Error message: %s, StatusOutput: %s", szErrCode, szErrMsg, szStatusOutput);
			FactoryLog();
			m_strErrorCode = CommErr_Query_IMEI_From_SSSO2_Fail;
			m_strMessage = "Execute Check_MAL_PN_SSSO2 fuction fail.";
			FactoryLog();
			return false;
		}
	}
	
	m_strMessage = MakeStr("Status_SSSO2-------> %s", szStatusOutput);
	FactoryLog();
	m_strMessage = MakeStr("MSN-------> %s", szMSN);
	FactoryLog();
	m_strMessage = MakeStr("WLANAddr-------> %s", szMACAddr);
	FactoryLog();
	m_strMessage = MakeStr("BTAddr-------> %s", szBTAddr);
	FactoryLog();


	if(m_strStation.compare("IMEI") == 0)
	{
		// 2. Check IMEI Status //
		char szMobileIMEI[32] = {0};
		strcpy(szMobileIMEI, m_mapQueryData["IMEI_IN_MOBILE"].c_str());
		char szPICSFileFlag[32] = {0};
		if( !CheckIMEIStatus(szStatusOutput, szIMEI, szMobileIMEI, szPICSFileFlag, szErrCode, szErrMsg))
		{
			m_strErrorCode = szErrCode;
			m_strMessage = "Execute IMOP_CheckIMEIStatus fuction fail.";
			FactoryLog();
			m_strMessage = MakeStr("%s", szErrMsg);
			FactoryLog();
			return false;
		}
		m_strMessage = MakeStr("%s", szErrMsg);
		FactoryLog();

		// 3. Check BT Length //
		if(! CheckBTLength(szBTAddr, "12", szErrCode, szErrMsg))
		{
			m_strErrorCode = szErrCode;
			m_strMessage = "Execute IMOP_CheckBTLength fuction fail.";
			FactoryLog();
			m_strMessage = MakeStr("%s", szErrMsg);
			FactoryLog();
			return false;
		}
		m_strMessage = MakeStr("%s", szErrMsg);
		FactoryLog();


		// 4. Check MAC Length //
		if(! CheckMACLength(szMACAddr, "12", szErrCode, szErrMsg))
		{
			m_strErrorCode = szErrCode;
			m_strMessage = "Execute IMOP_CheckMACLength fuction fail.";
			FactoryLog();
			m_strMessage = MakeStr("%s", szErrMsg);
			FactoryLog();
			return false;
		}		
		m_strMessage = MakeStr("%s", szErrMsg);
		FactoryLog();

		m_mapQueryData["PVK"] = szPVK;
		m_mapQueryData["SAVE_PICS_FILE_FLAG"] = szPICSFileFlag;
		m_mapQueryData["IMEI_BT_GSN_FLAG"] = "1";
		TraceLog(MSG_INFO, "QUERY IMEI STATUS END");
	}

	m_mapQueryData["MSN"] = szMSN;
	m_mapQueryData["BT_ADDR"] = szBTAddr;
	m_mapQueryData["MAC_ADDR"] = szMACAddr;

	m_strMessage = "Query BT WLAN success";
	m_strResult = "PASS";
	TraceLog(MSG_INFO, m_strMessage);
	FactoryLog();
	return true;
}

bool CQueryMSNBt::CheckIMEIStatus(char *szStatus_SSSO2, char *szIMEI_Scanner, char *szIMEI_Mobile, char *szSave_SAVE_PICS_FILE_FLAG, char *szErrorCode, char *szErrorString)
{
	CString strBTAddr;
	char szZero[50] ={0};
	for(int i = 0; i< m_i_IMEILength; i++)
		szZero[i] ='0';

	if(strcmp(szStatus_SSSO2, _T("B")) == 0)
	{
		if(strcmp(szIMEI_Mobile, szZero) && strcmp(szIMEI_Mobile, Dummy_IMEI))
		{
			sprintf(szErrorString,"In REWORK mode, the IMEI status must be 'I'//'R'//'D', but the real status is '%s'......",szStatus_SSSO2);
			strcpy(szErrorCode, "No_IMEI_In_Phone_When_rework");
			return false;

		}
	}
	else if(!strcmp(szStatus_SSSO2, _T("I")) || !strcmp(szStatus_SSSO2, _T("R")) || !strcmp(szStatus_SSSO2, _T("D"))|| !strcmp(szStatus_SSSO2, _T("F")))
	{
		if(!strcmp(szIMEI_Mobile, szZero) || !strcmp(szIMEI_Mobile, Dummy_IMEI))
		{
			sprintf(szErrorString,"The IMEI(%s) has been burned, namely the status is not 'B', while the handset has not IMEI......",szIMEI_Scanner);
			strcpy(szErrorCode, "IMEI_Status_Error");
			return false;
		}
		if(strcmp(szIMEI_Scanner, szIMEI_Mobile))
		{
			sprintf(szErrorString,"The IMEI(%s) from handset is not equal to the IMEI(%s) from scanner......",szIMEI_Mobile, szIMEI_Scanner);
			strcpy(szErrorCode, "Check_IMEI_Fail_In_Rework");
			return false;
		}
	}
	else //P/S
	{
		if(strcmp(szStatus_SSSO2, _T("W")) == 0)
		{
			sprintf(szErrorString,"IMEI Status--->'%s'",szStatus_SSSO2);
			strcpy(szErrorCode, "IMEI_Status_In_P_S_Error");
			strcpy(szSave_SAVE_PICS_FILE_FLAG, "0");
			return false;
		}
		else if(!strcmp(szStatus_SSSO2, _T("P")) || !strcmp(szStatus_SSSO2, _T("S")))
		{
			sprintf(szErrorString,"IMEI Status--->'%s'",szStatus_SSSO2);
			strcpy(szErrorCode, "IMEI_Status_In_P_S_Error");
			strcpy(szSave_SAVE_PICS_FILE_FLAG, "0");
			return false;
		}
		else
		{
			sprintf(szErrorString,"IMEI Status--->'%s' ERROR",szStatus_SSSO2);
			strcpy(szErrorCode, "IMEI_Status_Error");
			return false;
		}
	}

	sprintf(szErrorString,"Check IMEI(%s) Status (%s) success.......",szIMEI_Scanner, szStatus_SSSO2);

	return true;
}

bool CQueryMSNBt::CheckBTLength(char *szBT_Addr,char* szLength, char *szErrorCode, char *szErrorString)
{
	CString strBTAddr = szBT_Addr;

	int nBT_Length = atoi(szLength);

	if(strBTAddr.GetLength() != nBT_Length)
	{
		sprintf(szErrorString,"Length of BT from PICS:%d  It should be:%d",strBTAddr.GetLength(),nBT_Length);
		strcpy(szErrorCode, "Check_BT_Address_Length_Fail");
		return false;
	}

	sprintf(szErrorString,"Length of BT is OK......");

	return true;
}

bool CQueryMSNBt::CheckMACLength(char *szMAC_Addr,char* szLength, char *szErrorCode, char *szErrorString)
{
	CString strMACAddr = szMAC_Addr;

	int nMAC_Length = atoi(szLength);

	if(strMACAddr.GetLength() != nMAC_Length)
	{
		sprintf(szErrorString,"Length of MAC from PICS:%d  It should be:%d",strMACAddr.GetLength(),nMAC_Length);
		strcpy(szErrorCode, "Check_WLAN_Address_Length_Fail");
		return false;
	}

	sprintf(szErrorString,"Length of MAC is OK......");

	return true;
}