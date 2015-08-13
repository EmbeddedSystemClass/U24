#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\QisdaNVModule\NVItem.h"
#include "..\Common\CommonUtil\PIMStringUtility.h"


/*****************************************************************************/
//
//  export for SPU 
//
/*****************************************************************************/

QCOM_DIAG_API bool NV_IsConnect_SPU (const char* szPort) 
{
	return CCFCDiagCommand(atoi(szPort)).IsConnected();
}

QCOM_DIAG_API bool NV_ResetHandset_SPU (const char* szPort)
{
	return CNVItem(atoi(szPort)).ResetHandset();
}

QCOM_DIAG_API bool NV_ReadItem (const int port, const WORD NVItem, BYTE* pBuffer, long BufferSize, WORD *pnv_stat)
{
	if (BufferSize <=0)
		return true;

	if (! pBuffer)
		return false;
		
	DIAG_NVITEM_PACKET_F_type ReadResp = {0};

	if (! CNVItem(port).ReadNVItem (NVItem, ReadResp))
		return false;

	if (pnv_stat)
		*pnv_stat = ReadResp.nv_stat;

	int size = sizeof(ReadResp.item_data);
	if (BufferSize < size)
		size = BufferSize;

	memcpy (pBuffer, &ReadResp.item_data, size);
	return true;
}

QCOM_DIAG_API bool NV_ReadItem_SPU (const char* szPort, const char* NVItem, char* pBuffer, char* BufferSize, char *pnv_stat)
{
	int destLen = atoi(BufferSize); 
	if (destLen <=0)
		return true;

	if (! pBuffer)
		return false;

	WORD nvstatus;
	BYTE item_data [DIAG_NV_ITEM_SIZE];
	if (! NV_ReadItem (atoi(szPort), atoi(NVItem), item_data, DIAG_NV_ITEM_SIZE, &nvstatus))
		return false;

	if (pnv_stat)
		wsprintf (pnv_stat, "%d", nvstatus);

	CStringA strBuffer;
	QSYNC_LIB::ByteToChar (item_data, DIAG_NV_ITEM_SIZE, strBuffer, true);
	strBuffer.Insert (0, "HEX:");
	int len = strBuffer.GetLength();
	if (len > destLen)
		len = destLen;
	strncpy (pBuffer, strBuffer.GetString(), len);
	
	return true;
}

QCOM_DIAG_API bool NV_WriteItem (const int port, const WORD NVItem, BYTE* pBuffer, long BufferSize, WORD *pnv_stat)
{
	WORD size = (WORD)BufferSize;
	DIAG_NVITEM_PACKET_F_type ReadResp = {0};

	CNVItem nv(port);
	if (! nv.WriteNVItem (NVItem, ReadResp, pBuffer, size))
		return false;

	if (5 == ReadResp.nv_stat)
	{
		if (! nv.WriteNVItem (NVItem, ReadResp, pBuffer, size))
			return false;
	}

	if (pnv_stat)
		*pnv_stat = ReadResp.nv_stat;

	return true;
}

QCOM_DIAG_API bool NV_WriteItem_Radix_SPU (const char* szPort, const char* NVItem, char* pBuffer, char* pradix, char* BufferSize, char *pnv_stat)
{
	int size = atoi(BufferSize);
	BYTE *pWriteData = new BYTE [size];
	
	if (! pWriteData)
		return false;
	
	::ZeroMemory(pWriteData, size);

	int iRadix = atoi(pradix);
	
	if((iRadix >= 2) && (iRadix <=36 ) && (iRadix != 1)) //MSDN: 2 <= iRadax <= 36, iRadix = 0 : base = 10
		*pWriteData = (BYTE)strtoul(pBuffer, NULL, iRadix);
	else
		return false;


	/*
	//parse if pData has "HEX:ABC123", then convert it to Byte ABC123
	if (_strnicmp (pBuffer, "HEX:", 4) == 0)
	{
		QSYNC_LIB::Char2To1Byte (&pBuffer[4], size, pWriteData);
		size = (int) ceil ((size-4) / 2.);
	}
	else
	{
		memcpy (pWriteData, pBuffer, size); //dont know how to do ??
	}
	
	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
	*/
	WORD nvstatus;
	
	bool ret = NV_WriteItem (atoi(szPort), atoi(NVItem), pWriteData, size, &nvstatus);

	if (pnv_stat)
		wsprintf (pnv_stat, "%d", nvstatus);

	delete [] pWriteData;
	return ret;
}

QCOM_DIAG_API bool NV_WriteItem_64Base_SPU (const char* szPort, const char* szNVItem, char* szWriteData_Base64, char* sznDataLen,char* szNV_stat)
{
	bool bRes = false;
	int nDecLen = Base64DecodeGetRequiredLength((int)strlen(szWriteData_Base64));
	BYTE* DataBuf = new BYTE[nDecLen];
	memset(DataBuf, 0, nDecLen);
	WORD nv_stat = 0;
	bRes = Base64Decode(szWriteData_Base64, (int)strlen(szWriteData_Base64), DataBuf, &nDecLen) && NV_WriteItem(atoi(szPort), atoi(szNVItem), DataBuf, nDecLen, &nv_stat);

	if (bRes)
	{
		CStringA strNVState;
		strNVState.Format("%d", nv_stat);
		memcpy(szNV_stat, strNVState, strNVState.GetLength()+1);
	}

	delete [] DataBuf;
	return bRes;
}


QCOM_DIAG_API bool NV_ReadNV453_SPU (const char* szPort, char* value)
{
	WORD nvstatus;
	BYTE buffer [DIAG_NV_ITEM_SIZE];

	if (! NV_ReadItem (atoi(szPort), 453, buffer, DIAG_NV_ITEM_SIZE, &nvstatus))
		return false;

	sprintf (value, "%d", buffer[0]);
	return true;
}

QCOM_DIAG_API bool NV_WriteNV453_SPU (const char* szPort, char* value)
{
	WORD nvstatus;

	BYTE writeval = (BYTE)atoi(value);

	return NV_WriteItem (atoi(szPort), 453, &writeval, 1, &nvstatus);
}

QCOM_DIAG_API bool NV_BackupPartition (const char* szPort)
{
	return CNVItem(atoi(szPort)).BackupNVPartition(60000);
}

QCOM_DIAG_API bool NV_RestorePartition (const char* szPort)
{
	return CNVItem(atoi(szPort)).RestoreNVPartition(60000);
}

QCOM_DIAG_API bool NV_ResetProcessInfo(char* szPort,char* szProcessIndex)
{
	CNVItem nvItem(atoi(szPort));
	return nvItem.ResetProcessInfo (szProcessIndex);
}

QCOM_DIAG_API bool NV_ReadProcessInfo(char* szPort,char* szProcessIndex,char* szYear,char* szMonth,char* szDay,char* szLine,char* szStation)
{
	CNVItem nvItem(atoi(szPort));
	return nvItem.ReadProcessInfo(szProcessIndex,szYear,szMonth,szDay,szLine,szStation);
}

QCOM_DIAG_API bool NV_PowerOff_SPU (const char* szPort)
{
	CNVItem nvItem(atoi(szPort));
	return nvItem.PowerOff ();
}
QCOM_DIAG_API bool NV_WriteItem22CodeSN_Wolf_SPU (const char* szPort, const char* szBuffer, char* szError)
{
	//if(strlen(szBuffer) != 22)
	//{
	//	sprintf(szError, "22CodeSN Length Fail");
	//	return false;
	//}

	WORD nvstatus;
	BYTE writeval [128] = {0};

	memcpy(writeval, szBuffer, 23);

	return NV_WriteItem (atoi(szPort), 50031, writeval, 22, &nvstatus);
}

QCOM_DIAG_API bool NV_ReadNV543_SPU (const char* szPort, char* value)
{
	WORD nvstatus;
	BYTE buffer [DIAG_NV_ITEM_SIZE];

	if (! NV_ReadItem (atoi(szPort), 543, buffer, DIAG_NV_ITEM_SIZE, &nvstatus))
		return false;

	sprintf (value, "%d", buffer[0]);
	return true;
}

QCOM_DIAG_API bool NV_WriteNV543_SPU (const char* szPort, char* value)
{
	WORD nvstatus;

	BYTE writeval = (BYTE)atoi(value);

	return NV_WriteItem (atoi(szPort), 543, &writeval, 1, &nvstatus);

}

QCOM_DIAG_API bool NV_ReadItem22CodeSN_Wolf_SPU (const char* szPort, char* szBuffer)
{
	WORD nvstatus;
	BYTE buffer [23];

	if (! NV_ReadItem (atoi(szPort), 50031, buffer, 23, &nvstatus))
		return false;
	
	memcpy(szBuffer, buffer, 23);

	if(strcmp(szBuffer,"") == 0)
		sprintf(szBuffer, "22CodeSN NULL");

	return true;
}

QCOM_DIAG_API bool NV_ReadQCNName_SPU (const char* szPort, char* value)
{
	WORD nvstatus;
	BYTE buffer [DIAG_NV_ITEM_SIZE];

	if (! NV_ReadItem (atoi(szPort), 50014, buffer, DIAG_NV_ITEM_SIZE, &nvstatus))
		return false;

	memcpy(value, buffer, DIAG_NV_ITEM_SIZE);

	if (strcmp(value,"") == 0)
		return false;

	return true;
}
QCOM_DIAG_API bool NV_ReadItem22CodeSN_C7_SPU (const char* szPort, char* szBuffer)
{
	WORD nvstatus;
	BYTE buffer [23];

	if (! NV_ReadItem (atoi(szPort), 30020, buffer, 23, &nvstatus))
		return false;
	
	memcpy(szBuffer, buffer, 23);

	if(strcmp(szBuffer,"") == 0)
		sprintf(szBuffer, "22CodeSN NULL");

	return true;
}

QCOM_DIAG_API bool NV_WriteItem22CodeSN_C7_SPU (const char* szPort, const char* szBuffer, char* szError)
{
	//if(strlen(szBuffer) != 22)
	//{
	//	sprintf(szError, "22CodeSN Length Fail");
	//	return false;
	//}

	WORD nvstatus;
	BYTE writeval [128] = {0};

	memcpy(writeval, szBuffer, 23);

	return NV_WriteItem (atoi(szPort), 30020, writeval, 22, &nvstatus);
}

QCOM_DIAG_API bool Write_Public_Key_C7_SPU(const char* szPort, const char* szFileSrc)
{
	using namespace PLATFORM_TOOL ;
	
	//Decode whole sim data
	bool bRes = false ;

	BYTE* SimDataBuf = NULL;
	size_t nDecLen = 0;
	bRes = HexStringToByteArray(szFileSrc, SimDataBuf, nDecLen);

	if(nDecLen < 128 || NULL == SimDataBuf)
	{
		bRes = false;
	}

	if(bRes)
	{
		//Get first 128 bytes data, i.e. public key
		BYTE PublicKey[128] = {0};
		for(int i = 0;i < 128;i++)
		{
			PublicKey[i] = SimDataBuf[i];
		}

		WORD nvstatus;
		bRes = NV_WriteItem (atoi(szPort), 30010, PublicKey, 128, &nvstatus);

		if((bRes) && (nvstatus == 0))
			bRes = true;
		else
			bRes = false;
	}

	if(NULL != SimDataBuf)
		delete [] SimDataBuf;

	return bRes;
}

QCOM_DIAG_API bool Read_Public_Key_C7_SPU(const char* szPort, char* szPubKey)
{
	using namespace PLATFORM_TOOL ;
	

	bool bRes = false ;

	WORD nvstatus = -1;
	BYTE byteData[256] = {0};

	bRes = NV_ReadItem (atoi(szPort), 30010, byteData, 128, &nvstatus);

	if((bRes) && (nvstatus == 0))
	{
		int i, j, k;
		byte b;
		for (i = 0, j = 0; i < 128; ++i) 
		{       
			b = byteData[i]; 
			k = b >> 4;       
			szPubKey[j++] = (char)(k > 9 ? k + 0x37 : k + 0x30) ;
			k = b & 15;       
			szPubKey[j++] = (char)(k > 9 ? k + 0x37 : k + 0x30) ;  
		}
		bRes = true;
	}
	else
		bRes = false;
	
	return bRes ;
}

QCOM_DIAG_API bool NV_Check_SPU(const char* szPort, char *szNV30015, char *szNV30016, char *szNV30017, char *szErrorCode)
{
	TSTRING		strAddr;

	bool bRes = CNVItem(atoi(szPort)).GetNv_CheckSum(szNV30015, szNV30016, szNV30017);

	if( bRes )
		return true;
	else{
		strcpy(szErrorCode, CommErr_Check_NV_Backup_Fail);
		return false;
	}
	return false;
}