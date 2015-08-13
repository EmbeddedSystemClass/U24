#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\QisdaNVModule\NVItem.h"
#include "..\QisdaSimLockModule\SimLockStore.h"
#include "..\Common\CommonUtil\PIMStringUtility.h"
#include "..\RequestManager\Request\SimLockStoreRequest.h"



/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/
bool CQcomDiagModule::GetSimLockName(char *szLockName)
{
	using namespace PLATFORM_TOOL ;

	bool bResult, bRes = false ;

	CRequestPtr ptRequest = new CQuerySIMLockNameRequest(szLockName, bResult) ;

	if (Execute (ptRequest))
	{
		bRes = bResult ;
	}

	if (! bRes)
		PopLastLog() ;

	return bRes ;
}

bool CQcomDiagModule::GetSimLockNameNew(char *szLockName)
{
	using namespace PLATFORM_TOOL ;

	bool bResult, bRes = false ;

	CRequestPtr ptRequest = new CQuerySIMLockNameNewRequest(szLockName, bResult) ;

	if (Execute (ptRequest))
	{
		bRes = bResult ;
	}

	if (! bRes)
		PopLastLog() ;

	return bRes ;
}
/*****************************************************************************/
//
//  export for SPU 
//
/*****************************************************************************/

QCOM_DIAG_API bool NV_WriteFSN_SPU (const char* szPort, const char* szFSN_base64, const char* sznTimeOut)
{
	bool bRes = false;

	CCFCDiagCommand CDiagCommand(atoi(szPort));
	size_t nOgnDataBufLen = strlen(szFSN_base64);
	int nDecLen = Base64DecodeGetRequiredLength((int)nOgnDataBufLen);
	if (nDecLen <= 1024)
	{
		BYTE* FSNBuf = new BYTE[nDecLen];
		memset(FSNBuf, 0, nDecLen);

		bRes = Base64Decode(szFSN_base64, (int)nOgnDataBufLen, FSNBuf, &nDecLen) == TRUE;
		if (bRes)
			bRes = CDiagCommand.WriteFSN(FSNBuf, 5000);

		delete [] FSNBuf;
	}

	return bRes;
}


QCOM_DIAG_API bool NV_WriteFSN_HexString_SPU (const char* szPort, const char* szFSN, const char* sznTimeOut)
{
	bool bRes = false;

	CCFCDiagCommand CDiagCommand(atoi(szPort));

	BYTE *pFSN = NULL;
	size_t dLen = 0;

	HexStringToByteArray (szFSN, pFSN, dLen);
	bRes = CDiagCommand.WriteFSN (pFSN);

	delete [] pFSN;
	return bRes;
}


QCOM_DIAG_API bool NV_ReadFSN_SPU (const char* szPort, char* szFSN_base64, char* sznTimeOut)
{
	CCFCDiagCommand CDiagCommand(atoi(szPort));
	BYTE  szFSN16[16] = {0};
	bool bRes =CDiagCommand.ReadFSN(szFSN16, atoi(sznTimeOut));
	if (bRes)
	{
		int nDataLen = Base64EncodeGetRequiredLength(16);
		if(nDataLen > 1024)
			bRes = false;
		else
		{
			memset(szFSN_base64, 0, nDataLen);
			bRes = Base64Encode(szFSN16, 16, szFSN_base64, &nDataLen) == TRUE;
		}
	}

	return bRes;
}

QCOM_DIAG_API bool NV_ReadFSN_HexString_SPU (const char* szPort, char* szFSN, char* sznTimeOut)
{
	// this function return Hexadecimal string, 0x10AB is returned as "10AB"
	
	CCFCDiagCommand CDiagCommand(atoi(szPort));
	BYTE  szFSN16[16] = {0};
	bool bRes =CDiagCommand.ReadFSN(szFSN16, atoi(sznTimeOut));
	if (bRes)
	{
		char chTemp [36] = {0};
		ByteArrayToHexString (szFSN16, chTemp, 16);
		strcpy (szFSN, (char*)chTemp);
	}

	return bRes;
}

QCOM_DIAG_API bool NV_ReadPublicKey_SPU (const char* szPort, char* szPublicKey_base64)
{
	DIAG_NVITEM_PACKET_F_type ReadResp;

	CNVItem nvItem (atoi(szPort));
	bool bRes = nvItem.ReadNVItem (50001, ReadResp);
	if (bRes)
	{
		int nDataLen = Base64EncodeGetRequiredLength(128);
		if (nDataLen > 1024)
		{
			bRes = false;
		}
		else
		{
			memset (szPublicKey_base64, 0, nDataLen);
			bRes = Base64Encode (ReadResp.item_data, 128, szPublicKey_base64, &nDataLen) == TRUE;
		}
	}

	return bRes;

}

QCOM_DIAG_API bool NV_ReadPublicKey_HexString_SPU (const char* szPort, char* szPublicKey)
{
	// this function return Hexadecimal string, 0x10AB is returned as "10AB"
	DIAG_NVITEM_PACKET_F_type ReadResp;

	CNVItem nvItem (atoi(szPort));
	bool bRes = nvItem.ReadNVItem (50001, ReadResp);
	if (bRes)
	{

		char chTemp [260] = {0}; //Public key is 128 byte, we need 2x buffer for HexString
		ByteArrayToHexString (ReadResp.item_data, chTemp, 128);
		strcpy (szPublicKey, chTemp);
	}

	return bRes;
}


QCOM_DIAG_API bool LockSPC_ForSPU(char* szPort)
{
    CNVItem nvItem(atoi(szPort));
    return nvItem.LockSPC();
}

QCOM_DIAG_API bool IsSPCLocked_ForSPU(char* szPort, char* szStatus)
{
    CNVItem nvItem(atoi(szPort));
    long nStatus = 0;
    bool bRes = nvItem.IsSPCLocked(nStatus);
    if(bRes)
    {
        CStringA strStatus = (nStatus == 0 ?  "SPC not locked" : "SPC Locked");
        memcpy(szStatus, strStatus, strStatus.GetLength()+1);
    }
    return bRes;
}

QCOM_DIAG_API bool UnlockSPC_ForSPU(char* szPort, char* szLock, char* szUnlockOK)
{
    CNVItem nvItem(atoi(szPort));
    bool bUnlockOK = false;
    bool bRes = nvItem.UnlockSPC(szLock, bUnlockOK);
    if (bRes)
    {
        CStringA strStatus = (bUnlockOK ? "Unlock SPC succeed." : "Unlock SPC failed.");
        memcpy(szUnlockOK, strStatus, strStatus.GetLength()+1);
    }
    return bRes;
}

QCOM_DIAG_API bool ProgramSPC_ForSPU(char* szPort, char* szLock)
{
    CNVItem nvItem(atoi(szPort));
    return nvItem.ProgramSPC(szLock);
}

bool WriteSimDataV2(long nComport, BYTE *pSimDataBuf, int nSimDataBufLen)
{
	BYTE ucSimAppLen = 16;
	BYTE ucSimApp[16] = {0};		
	DWORD nCmdStatus = 0, nStatus = 0;
	CSimLockStore simStore(nComport);

	if (simStore.StoreSimData(1, ucSimAppLen, ucSimApp, nSimDataBufLen, pSimDataBuf, &nCmdStatus, &nStatus))
		return (nCmdStatus == 0 && nStatus == 0);
	else
		return false;
}

QCOM_DIAG_API bool WriteSimDataV2_SPU(char* szPort, char* szSimDataBuf)
{
	bool bRes = false;
	size_t nDecLen = 0;
	BYTE* SimDataBuf = NULL;
	
	bRes = HexStringToByteArray(szSimDataBuf, SimDataBuf, nDecLen) && 
		   WriteSimDataV2(atoi(szPort), SimDataBuf, nDecLen);

	delete [] SimDataBuf;
	return bRes;
}

bool WriteSimDataNew(long nComport, BYTE *pSimDataBuf, int nSimDataBufLen)
{
	BYTE ucSimAppLen = 16;
	BYTE ucSimApp[16] = {0};		
	DWORD nCmdStatus = 0, nStatus = 0;

	CSimLockStore writeSimLock(nComport);
	if (writeSimLock.WriteSimDataNew(1, pSimDataBuf, nSimDataBufLen, 60000))
		return true;

	return false;
}


QCOM_DIAG_API bool WriteSimDataNew_SPU(char* szPort, char* szSimDataBuf)
{
	bool bRes = false;
	size_t nDecLen = 0;
	BYTE* SimDataBuf = NULL;
	
	bRes = HexStringToByteArray(szSimDataBuf, SimDataBuf, nDecLen) && 
		   WriteSimDataNew(atoi(szPort), SimDataBuf, nDecLen);

	delete [] SimDataBuf;
	return bRes;
}

QCOM_DIAG_API bool WriteSimData_ForSPU(char* szPort, char* szSimDataBuf_Base64, char* szSimDataBufLen)
{
	bool bRes = false;
	//int nOgnDataBufLen = atoi(szSimDataBufLen);
	size_t nOgnDataBufLen = strlen(szSimDataBuf_Base64);
	int nDecLen = Base64DecodeGetRequiredLength((int)nOgnDataBufLen);
	BYTE* SimDataBuf = new BYTE[nDecLen];
	memset(SimDataBuf, 0, nDecLen);
	bRes = Base64Decode(szSimDataBuf_Base64, (int)nOgnDataBufLen, SimDataBuf, &nDecLen);
	
	if(bRes)
	{
		BYTE ucSimAppLen = 16;
		BYTE ucSimApp[16] = {0};		
		DWORD nCmdStatus = 0, nStatus = 0;
		CSimLockStore simStore(atoi(szPort));

		if (simStore.StoreSimData(1, ucSimAppLen, ucSimApp, nDecLen, SimDataBuf, &nCmdStatus, &nStatus))
			bRes = (nCmdStatus == 0 && nStatus == 0);
		else
			bRes = false;

	}
	delete [] SimDataBuf;
	return bRes;
}

QCOM_DIAG_API bool GetSimLockName_SPU(char* szPort, char* szSimLockName)
{
	CQcomDiagModule diag (atoi(szPort)) ;

	return diag.GetSimLockName(szSimLockName);
}

QCOM_DIAG_API bool GetSimLockNameNew_SPU(char* szPort, char* szSimLockName)
{
	CQcomDiagModule diag (atoi(szPort)) ;

	return diag.GetSimLockNameNew(szSimLockName);
}