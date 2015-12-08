// Pics_SSSO2_Factory.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <atlenc.h>
#include "Pics_SSSO2_Factory.h"
#include "PicsCtrl.h"
#include "SSSO2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSSSO2 SSSO2_Ctrl;
CPicsCtrl Pics_Interface_Ctrl;

using namespace std;

#pragma warning(push)
#pragma warning(disable:4996)


long IMEI_Status=0; 
char IMEI_SPC_Code[7]="\0";

char IMEI_BT_Network_Unlock_Code[17]="\0";
char IMEI_BT_Service_Provider_Unlock_Code[17]="\0";
char IMEI_BT_Coporate_Unlock_Code[17]="\0";
char IMEI_BT_Network_Subset_Unlock_Code[17]="\0";
char IMEI_BT_User_SIM_Unlock_Code[17]="\0";
char IMEI_BT_User_USIM_Unlock_Code[17]="\0";
char IMEI_BT_Serial_Code[17]="\0";
char IMEI_BT_Phone_Lock_Code[17]="\0";

ISimEnc_AllocSimEncObj			AllocSimEncObj;
ISimEnc_SetImei					SetImei;
ISimEnc_SetFlashSerialNumber	SetFlashSerialNumber;
ISimEnc_SetSimLockFile			SetSimLockFile;
ISimEnc_SetUnlockCodes			SetUnlockCodes;
ISimEnc_CheckSimLockData		CheckSimLockData;
ISimEnc_FreeSimEncObj			FreeSimEncObj;
ISimEnc_GenSimLockData			GenSimLockData;

// The one and only application object

CWinApp theApp;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
	}

	return nRetCode;
}

PICS_SSSO2_FACTORY_API bool SetNV7109Data64Encode(char* szData)
{
	BYTE sz7109Src[128]={0};

	int nDataLen = Base64EncodeGetRequiredLength(128);
	memset(szData, 0, nDataLen);
	bool bRes = Base64Encode(sz7109Src, 128, szData, &nDataLen) == TRUE;

	return bRes;
}

PICS_SSSO2_FACTORY_API bool GetBytesFromFactoryArea(const char* szFactory, const char* szIndex, const char* szBytes, char* szData)
{
	int nIndex = atoi(szIndex);
	int nBytes = atoi(szBytes);

	BYTE  szFactoryArea128[256] = {0};
	int nDataLen = Base64DecodeGetRequiredLength((int)strlen(szFactory));
	memset(szFactoryArea128, 0x00, nDataLen);

	bool bRes = Base64Decode(szFactory, (int)strlen(szFactory), szFactoryArea128, &nDataLen) == TRUE;
	if(bRes)
	{
		if(nIndex == 45)
		{
			BYTE szDataTemp[128] = {0};
			memcpy(szDataTemp, &szFactoryArea128[nIndex], nBytes);
			nDataLen = Base64EncodeGetRequiredLength(nBytes);
			Base64Encode(szDataTemp, nBytes, szData, &nDataLen);
		}
		else
		{
			memcpy(szData, &szFactoryArea128[nIndex], nBytes);
			szData[nBytes] = '\0';
		}	
	}
	return bRes;
}
PICS_SSSO2_FACTORY_API bool SetPicasso(char* szFactory, const char* szPicassoNo)
{

	BYTE  szFactoryArea128[256] = {0};
	int nDataLen = Base64DecodeGetRequiredLength((int)strlen(szFactory));
	memset(szFactoryArea128, 0x00, nDataLen);

	bool bRes = Base64Decode(szFactory, (int)strlen(szFactory), szFactoryArea128, &nDataLen) == TRUE;
	if(bRes)
	{
		BYTE szDataTemp[128] = {0};
		for(int i=0;i<10;i++)//94 to 103 is for picasso no
		{
			szFactoryArea128[94+i]=szPicassoNo[i];
		}

		memcpy(szDataTemp, &szFactoryArea128[0], 128);
		nDataLen = Base64EncodeGetRequiredLength(128);
		bRes = Base64Encode(szDataTemp, 128, szFactory, &nDataLen) == TRUE;
	}
	return bRes;
}
PICS_SSSO2_FACTORY_API bool SetTranceiver(char* szFactory, const char* szTransNO)
{

	BYTE  szFactoryArea128[256] = {0};
	int nDataLen = Base64DecodeGetRequiredLength((int)strlen(szFactory));
	memset(szFactoryArea128, 0x00, nDataLen);

	bool bRes = Base64Decode(szFactory, (int)strlen(szFactory), szFactoryArea128, &nDataLen) == TRUE;
	if(bRes)
	{
		BYTE szDataTemp[128] = {0};
		for(int i=0;i<10;i++)//104 to 113 is for tranceiver no
		{
			szFactoryArea128[104+i]=szTransNO[i];
		}

		memcpy(szDataTemp, &szFactoryArea128[0], 128);
		nDataLen = Base64EncodeGetRequiredLength(128);
		bRes = Base64Encode(szDataTemp, 128, szFactory, &nDataLen) == TRUE;
	}
	return bRes;
}
PICS_SSSO2_FACTORY_API bool SetGSN(char* szFactory, char* szGSNString)
{

	BYTE  szFactoryArea128[256] = {0};
	int nDataLen = Base64DecodeGetRequiredLength((int)strlen(szFactory));
	memset(szFactoryArea128, 0x00, nDataLen);

	bool bRes = Base64Decode(szFactory, (int)strlen(szFactory), szFactoryArea128, &nDataLen) == TRUE;
	if(bRes)
	{
		BYTE szDataTemp[128] = {0};
		for(int i=0;i<10;i++)//114 to 123 is for MSN
		{
			szFactoryArea128[114+i]=szGSNString[i];
		}

		memcpy(szDataTemp, &szFactoryArea128[0], 128);
		nDataLen = Base64EncodeGetRequiredLength(128);
		bRes = Base64Encode(szDataTemp, 128, szFactory, &nDataLen) == TRUE;
	}
	return bRes;
}

//  [2009/09/04 Nancy]
//////////////////////////////////////////////////////////////////////////
// this function for H12V set NV30001, 30002, 30003
//////////////////////////////////////////////////////////////////////////
PICS_SSSO2_FACTORY_API bool SetNVItem(char* szFactory, char* szNVItem, char*szIntFlag)
{
	BYTE  szFactoryArea128[256] = {0};
	int nDataLen = Base64DecodeGetRequiredLength((int)strlen(szFactory));
	memset(szFactoryArea128, 0x00, nDataLen);

	bool bRes = Base64Decode(szFactory, (int)strlen(szFactory), szFactoryArea128, &nDataLen) == TRUE;
	if(bRes)
	{
		BYTE szDataTemp[128] = {0};

		if(1 == atoi(szIntFlag))//int
		{
			szFactoryArea128[0] = atoi(szNVItem);
			szFactoryArea128[1] = '\0';
		}
		else//string
		{
			int len = (int)strlen(szNVItem);
			for(int i=0;i<len;i++)
			{
				szFactoryArea128[i]=szNVItem[i];
			}
			szFactoryArea128[len]='\0';
		}

		memcpy(szDataTemp, &szFactoryArea128[0], 128);
		nDataLen = Base64EncodeGetRequiredLength(128);
		bRes = Base64Encode(szDataTemp, 128, szFactory, &nDataLen) == TRUE;
	}
	return bRes;

}

PICS_SSSO2_FACTORY_API bool SetKLFByte(char* szFactory,char *szComputeName)
{
	CString Computer_Name;
	Computer_Name.Format("%s",szComputeName);
	BYTE  szFactoryArea128[256] = {0};
	int nDataLen = Base64DecodeGetRequiredLength((int)strlen(szFactory));
	memset(szFactoryArea128, 0x00, nDataLen);

	bool bRes = Base64Decode(szFactory, (int)strlen(szFactory), szFactoryArea128, &nDataLen) == TRUE;
	if(bRes)
	{
		BYTE szDataTemp[128] = {0};

		unsigned char KLF_Byte=szFactoryArea128[3];
		KLF_Byte|=0x80;
		szFactoryArea128[3]=KLF_Byte;

		//sunlight add for test count   
		szFactoryArea128[59]+=0x01;
		//sunlight add for process info
		CTime Time=CTime::GetCurrentTime();
		szFactoryArea128[29]=Time.GetDay();
		szFactoryArea128[30]=Time.GetMonth();
		szFactoryArea128[31]=Time.GetYear()-2000;
		szFactoryArea128[32]=((unsigned char)(atoi(Computer_Name.Mid(4,2)))<<4)+(unsigned char)(atoi(Computer_Name.Mid(1,2)));

		memcpy(szDataTemp, &szFactoryArea128[0], 128);
		nDataLen = Base64EncodeGetRequiredLength(128);
		bRes = Base64Encode(szDataTemp, 128, szFactory, &nDataLen) == TRUE;
	}
	return bRes;
}
PICS_SSSO2_FACTORY_API bool CompareTranceiver(char* szFactory, const char* szTransNO)
{

	BYTE  szFactoryArea128[256] = {0};
	int nDataLen = Base64DecodeGetRequiredLength((int)strlen(szFactory));
	memset(szFactoryArea128, 0x00, nDataLen);
	char Picasso_Read[11]={0};
	char XCVR_Read[11]={0};

	bool bRes = Base64Decode(szFactory, (int)strlen(szFactory), szFactoryArea128, &nDataLen) == TRUE;
	if(bRes)
	{
		for(int i=0;i<10;i++)
		{
			Picasso_Read[i]=szFactoryArea128[94+i];
			XCVR_Read[i]=szFactoryArea128[104+i];
		}
		if(strncmp((char*)XCVR_Read,szTransNO,10)!=0)
		{
			return false;
		}
	}
	return bRes;
}
PICS_SSSO2_FACTORY_API bool ComparePicasso(char* szFactory, const char* szPicassoNo)
{

	BYTE  szFactoryArea128[256] = {0};
	int nDataLen = Base64DecodeGetRequiredLength((int)strlen(szFactory));
	memset(szFactoryArea128, 0x00, nDataLen);
	char Picasso_Read[11]={0};
	char XCVR_Read[11]={0};

	bool bRes = Base64Decode(szFactory, (int)strlen(szFactory), szFactoryArea128, &nDataLen) == TRUE;
	if(bRes)
	{
		for(int i=0;i<10;i++)
		{
			Picasso_Read[i]=szFactoryArea128[94+i];
			XCVR_Read[i]=szFactoryArea128[104+i];
		}
		if(strncmp((char*)Picasso_Read,szPicassoNo,10)!=0)
		{
			return false;
		}
	}
	return bRes;
}
PICS_SSSO2_FACTORY_API bool ComCtryObjInitial(const char* szCurDirect)
{
	if((!SSSO2_Ctrl.Initial_SSSO2Dll())||(!Pics_Interface_Ctrl.Initial_PICSInterfaceDLL()))
		return FALSE;
	else 
		return TRUE;
}

//////////PICS///////////
PICS_SSSO2_FACTORY_API bool Create_IMEI_PICS(char *szInIMEI,char * szRes)
{
	short temp = Pics_Interface_Ctrl.CreateIMEIRecord(szInIMEI);
	_itoa_s(temp,szRes,SPU_BUFFER_SIZE,10);
	return temp==0?TRUE:FALSE;
}

PICS_SSSO2_FACTORY_API bool QuerySpc_PICS(char *szInIMEI,char *szRes,char *szOutSpc)
{
	short temp = Pics_Interface_Ctrl.QuerySPCFromPICS(szInIMEI,szOutSpc);
	_itoa_s(temp,szRes,SPU_BUFFER_SIZE,10);
	return temp==0?TRUE:FALSE;
}
PICS_SSSO2_FACTORY_API bool GetFSNCode_PICS(char *szInIMEI,char * szFSN,char *szRes)
{
	short temp = Pics_Interface_Ctrl.GetFSNCode(szInIMEI,szFSN);
	_itoa_s(temp,szRes,SPU_BUFFER_SIZE,10);
	return temp==0?TRUE:FALSE;
}
PICS_SSSO2_FACTORY_API bool GetSPCCode_PICS(char *szInIMEI,char * szSPC,char *szRes)
{
	short temp = Pics_Interface_Ctrl.GetSPCCodeFromPICS(szInIMEI,szSPC);
	_itoa_s(temp,szRes,SPU_BUFFER_SIZE,10);
	return temp==0?TRUE:FALSE;
}
PICS_SSSO2_FACTORY_API bool QueryFSNCode_PICS(char *szInIMEI,char * szFSN,char *szRes)
{
	short temp = Pics_Interface_Ctrl.QueryFSNCode(szInIMEI,szFSN);
	_itoa_s(temp,szRes,SPU_BUFFER_SIZE,10);
	return temp==0?TRUE:FALSE;
}
PICS_SSSO2_FACTORY_API bool GetSIMData_PICS(char* szSimType,char *szInIMEI,char * szFSN,char *szSimDataFile ,char *szSimDataFromPICS,char *szSimDataSize,char *szRes)
{						
	short nSimType = atoi(szSimType);
	short nSimDataSize = atoi(szSimDataSize); 
	char szSimDataFromPICS_Temp[4096] = {0};
	short temp = Pics_Interface_Ctrl.GetSIMData(nSimType,szInIMEI,szFSN,szSimDataFile,szSimDataFromPICS_Temp,&nSimDataSize);
	ASSERT(SPU_BUFFER_SIZE > nSimDataSize*2);
	if(SPU_BUFFER_SIZE > nSimDataSize*2)
	{
		memcpy(szSimDataFromPICS,szSimDataFromPICS_Temp,nSimDataSize*2);
		_itoa_s(nSimDataSize,szSimDataSize,SPU_BUFFER_SIZE,10);
		_itoa_s(temp,szRes,SPU_BUFFER_SIZE,10);
	}
	return temp==0?TRUE:FALSE;
}
PICS_SSSO2_FACTORY_API bool QueryUnlockUnblockCode_PICS(char *IMEI_Array,
													char *Network_Unlock_Code_Array,
													char *Service_Provider_Unlock_Code_Array,
													char *Coporate_Unlock_Code_Array,
													char *Network_Subset_Unlock_Code_Array,
													char *User_SIM_Unlock_Code_Array,
													char *User_USIM_Unlock_Code_Array,
													char *Serial_Code_Array,
													char *SPC_Array,
													char *Phone_Lock_Code_Array)
{
	short temp = Pics_Interface_Ctrl.QueryUnlockUnblockCode_BT(IMEI_Array,
		Network_Unlock_Code_Array,
		Service_Provider_Unlock_Code_Array,
		Coporate_Unlock_Code_Array,
		Network_Subset_Unlock_Code_Array,
		User_SIM_Unlock_Code_Array,
		User_USIM_Unlock_Code_Array,
		Serial_Code_Array,
		SPC_Array,
		Phone_Lock_Code_Array);
	return temp==0?TRUE:FALSE;
}

//////////SSS02///////////
PICS_SSSO2_FACTORY_API bool Check_MAL_PN_SSSO2(char *szInIMEI,char *szPort,char *szInMal,char *szErrCode, char *szErrMsg)
{
	return SSSO2_Ctrl.SSSO2_CheckNAL_PN(szInIMEI,szPort,szInMal,szErrCode,szErrMsg);
}
PICS_SSSO2_FACTORY_API bool GetProcessResult_SSS02(char *Picasso, char* Function, char* Model, char* szErrCode, char* szErrMsg)
{
	return SSSO2_Ctrl.SSSO2_GetProcessResult(Picasso, Function, Model, szErrCode, szErrMsg);
}
PICS_SSSO2_FACTORY_API bool CheckPicassoSMT_SSS02(char *Picasso, char* szProcess)
{
	return SSSO2_Ctrl.SSS02_CheckPicassoInSMT(Picasso, szProcess);
}

PICS_SSSO2_FACTORY_API bool CheckPiacssoLuna_SSS02(char *Picasso, char *szErrCode, char *szErrMsg)
{
	return SSSO2_Ctrl.SSS02_CheckPicassoForLuna(Picasso, szErrCode, szErrMsg);
}

PICS_SSSO2_FACTORY_API bool CheckPicassoPICS_SSS02(char* szModel, char* Picasso, char* szIMEI, char* szMobileID, char* szProcess, char* szError)
{
	return SSSO2_Ctrl.SSS02_CheckPiacssoInPICS(szModel, Picasso, szIMEI, szMobileID, szProcess, szError);
}

PICS_SSSO2_FACTORY_API bool QueryIMEI_SSSO2_V2(char *szStationtState,
											   char *pIMEI,
											   char *szMSN,
											   char *szBTAddr,
											   char *szMACAddr,
											   char *szSWver,
											   char *szHWver,
											   char *szPVK,
											   char *szErrCode, 
											   char *szErrMsg,
											   char *szStatusOutput)
{
	return SSSO2_Ctrl.SSSO2_QueryIMEI_V2(szStationtState,
		pIMEI,
		szMSN,
		szBTAddr,
		szMACAddr,
		szSWver,
		szHWver,
		szPVK,
		szErrCode,
		szErrMsg,
		szStatusOutput);
}

PICS_SSSO2_FACTORY_API bool QueryFG_SN_SSSO2_Wolf(char *szStationtState,
											   char *pIMEI,
											   char *szMSN,
											   char *szBTAddr,
											   char *szMACAddr,
											   char *szSWver,
											   char *szHWver,
											   char *szPVK,
											   char *szErrCode, 
											   char *szErrMsg,
											   char *szStatusOutput)
{
	return SSSO2_Ctrl.SSSO2_QueryFG_SN_Wolf(szStationtState,
		pIMEI,
		szMSN,
		szBTAddr,
		szMACAddr,
		szSWver,
		szHWver,
		szPVK,
		szErrCode,
		szErrMsg,
		szStatusOutput);
}

PICS_SSSO2_FACTORY_API bool QueryIMEI_SSSO2(char *szStationtState,
											   char *pIMEI,
											   char *szMSN,
											   char *szBTAddr,
											   char *szMACAddr,
											   char *szSWver,
											   char *szHWver,
											   char *szErrCode, 
											   char *szErrMsg,
											   char *szStatusOutput)
{
	return SSSO2_Ctrl.SSSO2_QueryIMEI(szStationtState,
		pIMEI,
		szMSN,
		szBTAddr,
		szMACAddr,
		szSWver,
		szHWver,
		szErrCode,
		szErrMsg,
		szStatusOutput);
}

PICS_SSSO2_FACTORY_API bool UpdateIMEIStatus_SSSO2(char *pIMEI,char *szSWver,char *szHWver,char *szMal,char *szErrCode, char *szErrMsg)
{
	return SSSO2_Ctrl.SSSO2_UpdateIMEIStatus(pIMEI,szSWver,szHWver,szMal,szErrCode,szErrMsg);
}

//sunlight add for leopard, 2011/01/06
PICS_SSSO2_FACTORY_API bool UpdateIMEIStatus_SSSO2_F(char *pIMEI,char *szErrCode, char *szErrMsg) 
{
	return SSSO2_Ctrl.SSSO2_UpdateIMEIStatus_F(pIMEI,szErrCode,szErrMsg);
}
////
// For Rework, AJ, 2011/04/21
PICS_SSSO2_FACTORY_API bool UpdateIMEIStatus_SSSO2_R(char *pIMEI,char *szErrCode, char *szErrMsg) 
{
	return SSSO2_Ctrl.SSSO2_UpdateIMEIStatus_R(pIMEI,szErrCode,szErrMsg);
}

PICS_SSSO2_FACTORY_API bool UpdateGSNPN_SSSO2(char *pPICASSO,char *pGSN,char *pPN,char *szErrCode, char *szErrMsg)
{
	return SSSO2_Ctrl.SSSO2_UpdateGSNPN(pPICASSO,pGSN,pPN,szErrCode,szErrMsg);
}

PICS_SSSO2_FACTORY_API bool QuerySN5_SSS02(char* szIMEI, char* szServiceTag, char *szErrCode, char *szErrMsg)
{
	return SSSO2_Ctrl.SSSO2_QuerySN5(szIMEI, szServiceTag, szErrCode,szErrMsg);
}

PICS_SSSO2_FACTORY_API bool QueryMobileIDByPicasso_SSS02(char* szModel, char* szPicasso, char* szMobileID, char *szErrCode, char *szErrMsg)
{
	return SSSO2_Ctrl.SSS02_ReadMobileID(szModel, szPicasso, szMobileID, szErrCode, szErrMsg);
}
PICS_SSSO2_FACTORY_API bool UpdateMobileIDByPicasso_SSS02(char* szModel, char* szPicasso, char* szMobileID, char *szErrCode, char *szErrMsg)
{
	return SSSO2_Ctrl.SSS02_WriteMobileID(szModel, szPicasso, szMobileID, szErrCode, szErrMsg);
}

/* Luna: Alex Add Start 2010/11/23 */
PICS_SSSO2_FACTORY_API bool UpdateIMEIST_SSSO2(char* szServiceTag, char* szIMEI, char* szErrCode, char* szErrMsg)
{
	return SSSO2_Ctrl.SSSO2_UpdateIMEIST(szServiceTag, szIMEI, szErrCode, szErrMsg);
}
/* Luna: Alex Add End 2010/11/23 */

/* Luna: Alex Add Start 2010/12/07 */
PICS_SSSO2_FACTORY_API bool QueryST_SSSO2(char *szStationtState,
											   char *pIMEI,
											   char *szMSN,
											   char *szBTAddr,
											   char *szMACAddr,
											   char *szSWver,
											   char *szHWver,
											   char *szErrCode, 
											   char *szErrMsg,
											   char *szStatusOutput)
{
	return SSSO2_Ctrl.SSSO2_QueryST(szStationtState,
		pIMEI,
		szMSN,
		szBTAddr,
		szMACAddr,
		szSWver,
		szHWver,
		szErrCode,
		szErrMsg,
		szStatusOutput);
}
/* Luna: Alex Add End 2010/12/07 */

/* Wolf: Sunlight Add Start 2011/08/23 */
PICS_SSSO2_FACTORY_API bool QueryPVKforAcer_SSSO2(char * sz_IMEI,char *sz_PVK,char *sz_ErrCode, char *sz_ErrMsg)
{
	return SSSO2_Ctrl.SSSO2_QueryPVKforAcer(sz_IMEI,sz_PVK,sz_ErrCode,sz_ErrMsg);
}
/* Wolf: Sunlight Add End 2011/08/23  */

/////////////////SimEncorder

PICS_SSSO2_FACTORY_API bool CheckSimData_SimEncorder(char* szSimType,
												 char *szIMEIFromPhone,
												 char *szFSN,
												 char*szIMEIFromScanner,
												 char *szSimDateFileName,
												 char *szSimDataForPhone_Base64,
												 char *szLogString)
{

	szLogString[0] = '\0';
	char temp_log[1024];
	CString LogStr;
	HMODULE hDll2 = ::LoadLibrary(_T("SimEncorder.dll"));
	ASSERT(hDll2);
	if(hDll2 == NULL) return false;
	AllocSimEncObj       = (ISimEnc_AllocSimEncObj )::GetProcAddress(hDll2, "_ISimEnc_AllocSimEncObj@8");
	SetImei              = (ISimEnc_SetImei )::GetProcAddress(hDll2, "_ISimEnc_SetImei@12");
	SetFlashSerialNumber = (ISimEnc_SetFlashSerialNumber )::GetProcAddress(hDll2, "_ISimEnc_SetFlashSerialNumber@12");
	SetSimLockFile       = (ISimEnc_SetSimLockFile )::GetProcAddress(hDll2, "_ISimEnc_SetSimLockFile@8");
	SetUnlockCodes       = (ISimEnc_SetUnlockCodes )::GetProcAddress(hDll2, "_ISimEnc_SetUnlockCodes@8");
	CheckSimLockData     = (ISimEnc_CheckSimLockData )::GetProcAddress(hDll2, "_ISimEnc_CheckSimLockData@12");
	FreeSimEncObj        = (ISimEnc_FreeSimEncObj )::GetProcAddress(hDll2, "_ISimEnc_FreeSimEncObj@4");

	int nSimType = atoi(szSimType);
	void * hSimEncHandle = NULL;
	int result=NULL;
	result=AllocSimEncObj(nSimType,hSimEncHandle);
	if(result!=0)
	{
		sprintf(temp_log,"AllocSimEncObj fail, return %d",result);
		LogStr+=temp_log;
		LogStr+="\n";
		::memcpy(szLogString,LogStr.GetBuffer(),LogStr.GetLength());
		szLogString[LogStr.GetLength()] ='\0';
		return false;
	}

	result=SetImei(hSimEncHandle,szIMEIFromPhone, (int)strlen(szIMEIFromPhone));
	if(result!=0)
	{
		sprintf(temp_log,"SetImei fail, return %d",result);
		LogStr+=temp_log;
		LogStr+="\n";
		::memcpy(szLogString,LogStr.GetBuffer(),LogStr.GetLength());
		szLogString[LogStr.GetLength()] ='\0';
		return false;
	}

	result=SetFlashSerialNumber(hSimEncHandle,szFSN, (int)strlen(szFSN));
	if(result!=0)
	{
		sprintf(temp_log,"SetFlashSerialNumber fail, return %d",result);
		LogStr+=temp_log;
		LogStr+="\n";
		::memcpy(szLogString,LogStr.GetBuffer(),LogStr.GetLength());
		szLogString[LogStr.GetLength()] ='\0';
		return false;
	}

	sprintf(temp_log,"\\\\10.85.68.13\\simfile\\%s",szSimDateFileName);
	result=SetSimLockFile(hSimEncHandle,temp_log);
	if(result!=0)
	{
		sprintf(temp_log,"SetSimLockFile fail, return %d",result);
		LogStr+=temp_log;
		LogStr+="\n";
		::memcpy(szLogString,LogStr.GetBuffer(),LogStr.GetLength());
		szLogString[LogStr.GetLength()] ='\0';
		return false;
	}
	T_SIM_UNLOCK_CODE tSimUnlockCode;
	sprintf(temp_log,"Query unlock unblock code from PICS by IMEI(%s)......",szIMEIFromScanner);
	LogStr+=temp_log;
	LogStr+="\n";
	IMEI_Status=Pics_Interface_Ctrl.QueryUnlockUnblockCode_BT(szIMEIFromScanner,
		IMEI_BT_Network_Unlock_Code,
		IMEI_BT_Service_Provider_Unlock_Code,
		IMEI_BT_Coporate_Unlock_Code,
		IMEI_BT_Network_Subset_Unlock_Code,
		IMEI_BT_User_USIM_Unlock_Code,
		IMEI_BT_User_SIM_Unlock_Code,
		IMEI_BT_Serial_Code,
		IMEI_SPC_Code,
		IMEI_BT_Phone_Lock_Code);
	if(IMEI_Status)
	{
		sprintf(temp_log,"Query unlock unlock code from PICS fail......");
		LogStr+=temp_log;
		LogStr+="\n";
		::memcpy(szLogString,LogStr.GetBuffer(),LogStr.GetLength());
		szLogString[LogStr.GetLength()] ='\0';
		return false;
	}
	else
	{
		sprintf(temp_log,"Query unlock unblock code from PICS success...NW->%s,SS->%s",IMEI_BT_Network_Unlock_Code,IMEI_BT_User_USIM_Unlock_Code);
		LogStr+=temp_log;
		LogStr+="\n";
	}

	tSimUnlockCode.szNetworkCode			= IMEI_BT_Network_Unlock_Code;
	tSimUnlockCode.szServiceProviderCode	= IMEI_BT_Service_Provider_Unlock_Code;
	tSimUnlockCode.szCorporateCode			= IMEI_BT_Coporate_Unlock_Code;
	tSimUnlockCode.szNetworkSubsetCode		= IMEI_BT_Network_Subset_Unlock_Code;
	tSimUnlockCode.szSimCode				= IMEI_BT_User_SIM_Unlock_Code;
	tSimUnlockCode.szUsimCode				= IMEI_BT_User_USIM_Unlock_Code;
	tSimUnlockCode.szReserved				= NULL;

	result=SetUnlockCodes(hSimEncHandle,&tSimUnlockCode);
	if(result!=0)
	{
		sprintf(temp_log,"SetUnlockCodes fail, return %d",result);
		LogStr+=temp_log;
		LogStr+="\n";
		::memcpy(szLogString,LogStr.GetBuffer(),LogStr.GetLength());
		szLogString[LogStr.GetLength()] ='\0';
		return false;
	}
	int nDecLen = Base64DecodeGetRequiredLength( (int)strlen(szSimDataForPhone_Base64));
	BYTE* SimDataForPhone = new BYTE[nDecLen];
	memset(SimDataForPhone, 0, nDecLen);
	Base64Decode(szSimDataForPhone_Base64, (int)strlen(szSimDataForPhone_Base64), SimDataForPhone, &nDecLen);
	result=CheckSimLockData(hSimEncHandle,(char*)SimDataForPhone,nDecLen);
	delete [] SimDataForPhone;

	if(result!=0)
	{
		sprintf(temp_log,"CheckSimLockData fail, return %d, length %d",result, nDecLen);
		LogStr+=temp_log;
		LogStr+="\n";
		::memcpy(szLogString,LogStr.GetBuffer(),LogStr.GetLength());
		szLogString[LogStr.GetLength()] ='\0';
		return false;
	}

	result=FreeSimEncObj(hSimEncHandle);
	if(result!=0)
	{
		sprintf(temp_log,"FreeSimEncObj fail, return %d",result);
		LogStr+=temp_log;
		LogStr+="\n";
		::memcpy(szLogString,LogStr.GetBuffer(),LogStr.GetLength());
		szLogString[LogStr.GetLength()] ='\0';
		return false;
	}
	::memcpy(szLogString,LogStr.GetBuffer(),LogStr.GetLength());
	szLogString[LogStr.GetLength()] ='\0';
	return true;
}
PICS_SSSO2_FACTORY_API bool EraseKLFByte(char* szFactory)
{
//	CString Computer_Name;
//	Computer_Name.Format("%s",szComputeName);
	BYTE  szFactoryArea128[256] = {0};
	int nDataLen = Base64DecodeGetRequiredLength((int)strlen(szFactory));
	memset(szFactoryArea128, 0x00, nDataLen);

	bool bRes = Base64Decode(szFactory, (int)strlen(szFactory), szFactoryArea128, &nDataLen) == TRUE;
	if(bRes)
	{
		BYTE szDataTemp[128] = {0};

		unsigned char KLF_Byte=szFactoryArea128[3];
//		KLF_Byte|=0x80;
		KLF_Byte &= 0x7F;
		szFactoryArea128[3]=KLF_Byte;
		memcpy(szDataTemp, &szFactoryArea128[0], 128);
		nDataLen = Base64EncodeGetRequiredLength(128);
		bRes = Base64Encode(szDataTemp, 128, szFactory, &nDataLen) == TRUE;
	}
	return bRes;
}

/*Sunlight ad 20111208*/
PICS_SSSO2_FACTORY_API bool UpdateIMEIStatus_F(char *pIMEI,char *szSWver,char *szHWver,char *szMal,char *szErrCode, char *szErrMsg)
{
	return SSSO2_Ctrl.SSSO2_UpdateIMEIStatus_F_New(pIMEI,szSWver,szHWver,szMal,szErrCode,szErrMsg);
}

PICS_SSSO2_FACTORY_API bool UpdateIMEIStatus_I(char *pIMEI,char *szSWver,char *szErrCode, char *szErrMsg) 
{
	return SSSO2_Ctrl.SSSO2_UpdateIMEIStatus_I_New(pIMEI,szSWver,szErrCode,szErrMsg);
}
/*Sunlight add end*/
/*20140626 Magan Mei Add Start*/
PICS_SSSO2_FACTORY_API bool UpdateIMEIStatus_B(char *pIMEI,char *szErrCode, char *szErrMsg)
{
	return SSSO2_Ctrl.SSSO2_UpdateIMEIStatus_B(pIMEI,szErrCode,szErrMsg);
}

PICS_SSSO2_FACTORY_API bool UpdateIMEIStatus_W(char *pIMEI,char *szErrCode, char *szErrMsg)
{
	return SSSO2_Ctrl.SSSO2_UpdateIMEIStatus_W(pIMEI,szErrCode,szErrMsg);
}
/*20140626 Magan Mei Add End*/

/*sunlight add 20120703*/
PICS_SSSO2_FACTORY_API bool UpdateNAL_SSSO2(char *pIMEI,char *szNAL,char *szErrCode, char *szErrMsg) 
{
	return SSSO2_Ctrl.SSSO2_UpdateNAL(pIMEI,szNAL,szErrCode,szErrMsg);
}

/*Sunlight add 20120706*/
PICS_SSSO2_FACTORY_API bool UpdateCustomerSW_SSSO2(char *pIMEI,char *szModel,char * szPartNo, char * szCustsw,char *szErrCode, char *szErrMsg)
{
	return SSSO2_Ctrl.SSSO2_UpdateCustomerSW(pIMEI,szModel,szPartNo, szCustsw,szErrCode,szErrMsg);
}

PICS_SSSO2_FACTORY_API bool GetAliyunByIMEI_SSSO2(char *pIMEI,char *szUUID,char * szErrCode, char *szErrMsg)
{
	return SSSO2_Ctrl.SSSO2_GetAliyunByIMEI(pIMEI,szUUID,szErrCode,szErrMsg);
}

/*sunlight add 20120827*/
PICS_SSSO2_FACTORY_API bool CheckCustomerPN_SSSO2(char *pIMEI,char * szErrCode, char *szErrMsg)
{
	return SSSO2_Ctrl.SSSO2_CheckCustomerPN(pIMEI,szErrCode,szErrMsg);
}


/*Sunlight add 20120924*/
PICS_SSSO2_FACTORY_API bool UpdateUnlockCode_SSSO2(char *pIMEI,
										  char *szUnlockN,
										  char *szUnlockS,
										  char *szUnlockC,
										  char *szUnlockNS,
										  char *szUnlockSIM,
										  char *szUnlockUS,
										  char *szSPC,
										  char *szPhoneLock,
										  char *szErrCode, 
										  char *szErrMsg)
{
	return SSSO2_Ctrl.SSSO2_UpdateUnlockCode(pIMEI,
		                                     szUnlockN,
											 szUnlockS,
											 szUnlockC,
											 szUnlockNS,
											 szUnlockSIM,
											 szUnlockUS,
											 szSPC,
											 szPhoneLock,
											 szErrCode,
											 szErrMsg);
}
#pragma warning(pop)
