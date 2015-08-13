#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\QisdaNVModule\NVItem.h"
#include "..\Common\FA_Operator\FA_Operator.h"

/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/


/*****************************************************************************/
//
//  export for SPU 
//
/*****************************************************************************/

QCOM_DIAG_API bool FA_Station_Setup (char* szPort, char* szStationAmount, char* szStationList, char* szErrCode, char* szErrMsg)
{
    int iAmount = atoi(szStationAmount) ;

    if ( (iAmount*2) != strlen(szStationList) ) {
        ::sprintf(szErrMsg, "Given Station List and Amount are not much!") ;
        return false ;
    }

    char* pszStationList = new char [iAmount];
    ::ZeroMemory(pszStationList, iAmount) ;
    
    // Convert Given Station List.
    for (int i=0; i<iAmount; ++i)
    {
        int iTemp = (szStationList[i*2]-'0')*10 + (szStationList[i*2+1]-'0') ;
        pszStationList[i] = iTemp ;
    }


    // Load DLL and Function
    HINSTANCE hDLL = ::LoadLibrary("FA_Operator.dll") ;
    if (!hDLL) {
        sprintf(szErrMsg, "Load FA_Operator.dll Fail") ;
        return false ;
    }
    
    typedef bool (*pFAFunction_4c) (char*, char*, char*, char*) ;
    typedef bool (*pFAFunction_9c) (char*, char*, char*, char*, char*, char*, char*, char*, char*) ;

    pFAFunction_9c pfnFA_WriteStationInfo  = (pFAFunction_9c)::GetProcAddress( (HMODULE)hDLL, "FA_WriteStationInfo" ) ;
    if (!pfnFA_WriteStationInfo ) {
        sprintf(szErrMsg, "Load FA_Operator.dll Function Fail") ;
        return false ;
    }
    
    pFAFunction_4c pfnFA_EraseTargetStationInfo = (pFAFunction_4c)::GetProcAddress( (HMODULE)hDLL, "FA_EraseTargetStationInfo" ) ;
    if (!pfnFA_EraseTargetStationInfo) {
        sprintf(szErrMsg, "Load FA_Operator.dll Function Fail") ;
        return false ;
    }

    
    // Write Setup Infomation
    char szTmp[512] ;
    char szReserved[512] ;
    ::memset(szTmp, 0, 512) ;
    ::memset(szReserved, 0, 512) ;

    strncpy(szTmp, pszStationList, iAmount) ;
    bool ret = pfnFA_WriteStationInfo ( szPort, "240", "N", szStationAmount, "0", szReserved,
                                        szTmp, szErrCode, szErrMsg) ;
    delete [] pszStationList ;

    if (!ret)
        return false ;

    return true ;
}


QCOM_DIAG_API bool FA_ReadStationInfo_SPU ( char* szPort,
                                            char* szStationIndex,
                                            char* szLineType,
                                            char* szTestResult,
                                            char* szTestCount,
                                            char* szTestDateTime,
                                            char* szTestLineStation,
                                            char* szReserve,
                                            char* szErrCode,
                                            char* szErrMsg )
{
    HINSTANCE hDLL = ::LoadLibrary("FA_Operator.dll") ;
    if (!hDLL) {
        sprintf(szErrMsg, "Load FA_Operator.dll Fail") ;
        return false ;
    }
    typedef bool (*pFAFunction_10c)(char*, char*, char*, char*, char*, char*, char*, char*, char*, char*) ;
    pFAFunction_10c pfnFA_ReadStationInfo  = (pFAFunction_10c)::GetProcAddress( (HMODULE)hDLL, "FA_ReadStationInfo" ) ;
    if (!pfnFA_ReadStationInfo ) {
        sprintf(szErrMsg, "Load FA_Operator.dll Function Fail") ;
        return false ;
    }

    bool ret = pfnFA_ReadStationInfo(szPort, szStationIndex, szLineType, szTestResult, szTestCount,
                                     szTestDateTime, szTestLineStation, szReserve, szErrCode, szErrMsg) ;
    if (!ret)
        return false ;

    return true ;
}


QCOM_DIAG_API bool FA_WriteStationInfo_SPU( char* szPort, 
                                            char* szStationIndex,
                                            char* szLineType,
                                            char* szTestResult,
                                            char* szErrCode,
                                            char* szErrMsg)
{
    HINSTANCE hDLL = ::LoadLibrary("FA_Operator.dll") ;
    if (!hDLL) {
        sprintf(szErrMsg, "Load FA_Operator.dll Fail") ;
        return false ;
    }
    typedef bool (*pFAFunction_9c) (char*, char*, char*, char*, char*, char*, char*, char*, char*) ;
    pFAFunction_9c pfnFA_WriteStationInfo  = (pFAFunction_9c)::GetProcAddress( (HMODULE)hDLL, "FA_WriteStationInfo" ) ;
    if (!pfnFA_WriteStationInfo ) {
        sprintf(szErrMsg, "Load FA_Operator.dll Function Fail") ;
        return false ;
    }

    char szStationExternLen [512] ;
    sprintf_s(szStationExternLen, 511, "0") ;
    char szReserve [512] ;
    sprintf_s(szReserve, 511, "0") ;
    char szStationExternInfo [512] ;
    sprintf_s(szStationExternInfo, 511, "0") ;
    bool ret = pfnFA_WriteStationInfo ( szPort, szStationIndex, szLineType, szStationExternLen, szTestResult,
                                        szReserve, szStationExternInfo, szErrCode, szErrMsg);
    if (!ret)
        return false ;

    return true ;
}


// Erase Target Station Info
QCOM_DIAG_API bool FA_EraseTargetStationInfo_SPU (char* szPort, char* szIndex, char* szErrCode, char* szErrMsg)
{
    HINSTANCE hDLL = ::LoadLibrary("FA_Operator.dll") ;
    if (!hDLL) {
        sprintf(szErrMsg, "Load FA_Operator.dll Fail") ;
        return false ;
    }
    typedef bool (*pFAFunction_4c) (char*, char*, char*, char*) ;
    pFAFunction_4c pfnFA_EraseTargetStationInfo = (pFAFunction_4c)::GetProcAddress( (HMODULE)hDLL, "FA_EraseTargetStationInfo" ) ;
    if (!pfnFA_EraseTargetStationInfo) {
        sprintf(szErrMsg, "Load FA_Operator.dll Function Fail") ;
        return false ;
    }

    bool ret = pfnFA_EraseTargetStationInfo(szPort, szIndex, szErrCode, szErrMsg) ;
    if (!ret)
        return false ;

    return true ;
}


#define FA_NV_ITEM_1 2497
#define FA_NV_ITEM_2 2498
#define FA_NV_ITEM_3 2499
#define FA_NV_ITEM_4 2500


QCOM_DIAG_API bool FA_Read(const char* szPort, char* szFAData, char* szErrCode, char* szErrMsg)
{
	int nComport = atoi(szPort);
	if (0 == nComport)
	{
		sprintf_s(szErrCode, MAX_PATH, "NV0001");
		sprintf_s(szErrMsg, MAX_PATH, "NVdll, No COM Port.");
		return false;
	}

	CNVItem nv(nComport);
	DIAG_NVITEM_PACKET_F_type ReadResp = {0};

	// Read FA_NV_ITEM_1
	if (! nv.ReadNVItem(FA_NV_ITEM_1, ReadResp))
	{
		sprintf_s(szErrCode, MAX_PATH, "NV0002");
		sprintf_s(szErrMsg, MAX_PATH, "NVdll, Read NV Fail.");
		return false;
	}
	else
	{
		memcpy(szFAData + 0, ReadResp.item_data, 128);
	}

	// Read FA_NV_ITEM_2
	if (! nv.ReadNVItem(FA_NV_ITEM_2, ReadResp))
	{
		sprintf_s(szErrCode, MAX_PATH, "NV0002");
		sprintf_s(szErrMsg, MAX_PATH, "NVdll, Read NV Fail.");
		return false;
	}
	else
	{
		memcpy(szFAData + 128, ReadResp.item_data, 128);
	}

	// Read FA_NV_ITEM_3
	if (! nv.ReadNVItem(FA_NV_ITEM_3, ReadResp))
	{
		sprintf_s(szErrCode, MAX_PATH, "NV0002");
		sprintf_s(szErrMsg, MAX_PATH, "NVdll, Read NV Fail.");
		return false;
	}
	else
	{
		memcpy(szFAData + 256, ReadResp.item_data, 128);
	}

	// Read FA_NV_ITEM_4
	if (! nv.ReadNVItem(FA_NV_ITEM_4, ReadResp))
	{
		sprintf_s(szErrCode, MAX_PATH, "NV0002");
		sprintf_s(szErrMsg, MAX_PATH, "NVdll, Read NV Fail.");
		return false;
	}
	else
	{
		memcpy(szFAData+384, ReadResp.item_data, 128);
	}

	return true;
}

QCOM_DIAG_API bool FA_Write(const char* szPort, char* szFAData, char* szErrCode, char* szErrMsg)
{
	int nComport = atoi(szPort);
	if (0 == nComport)
	{
		sprintf_s(szErrCode, MAX_PATH, "NV0001");
		sprintf_s(szErrMsg,  MAX_PATH, "NVdll, No COM Port.");
		return false;
	}

	CNVItem nv(nComport);
	DIAG_NVITEM_PACKET_F_type ReadResp = {0};

	// Write FA_NV_ITEM_1
	if (nv.WriteNVItem(FA_NV_ITEM_1, ReadResp, (uint8*)(szFAData + 0), 128))
	{
		if (5 == ReadResp.nv_stat)
			nv.WriteNVItem(FA_NV_ITEM_1, ReadResp, (uint8*)(szFAData + 0), 128);
	}
	else
	{
		sprintf_s(szErrCode, MAX_PATH, "NV0003");
		sprintf_s(szErrMsg,  MAX_PATH, "NVdll, Write NV Fail.");
		return false;
	}

	// Write FA_NV_ITEM_2
	if (nv.WriteNVItem(FA_NV_ITEM_2, ReadResp, (uint8*)(szFAData + 128), 128))
	{
		if (5 == ReadResp.nv_stat)
			nv.WriteNVItem(FA_NV_ITEM_2, ReadResp, (uint8*)(szFAData + 128), 128);
	}
	else
	{
		sprintf_s(szErrCode, MAX_PATH, "NV0003");
		sprintf_s(szErrMsg,  MAX_PATH, "NVdll, Write NV Fail.");
		return false;
	}

	// Write FA_NV_ITEM_3
	if (nv.WriteNVItem(FA_NV_ITEM_3, ReadResp, (uint8*)(szFAData + 256), 128))
	{
		if (5 == ReadResp.nv_stat)
			nv.WriteNVItem(FA_NV_ITEM_3, ReadResp, (uint8*)(szFAData + 256), 128);
	}
	else
	{
		sprintf_s(szErrCode, MAX_PATH, "NV0003");
		sprintf_s(szErrMsg,  MAX_PATH, "NVdll, Write NV Fail.");
		return false;
	}

	// Write FA_NV_ITEM_4
	if (nv.WriteNVItem(FA_NV_ITEM_4, ReadResp, (uint8*)(szFAData + 384), 128))
	{
		if (5 == ReadResp.nv_stat)
			nv.WriteNVItem(FA_NV_ITEM_4, ReadResp, (uint8*)(szFAData + 384), 128);
	}
	else
	{
		sprintf_s(szErrCode, MAX_PATH, "NV0003");
		sprintf_s(szErrMsg,  MAX_PATH, "NVdll, Write NV Fail.");
		return false;
	}

	return true;
}
