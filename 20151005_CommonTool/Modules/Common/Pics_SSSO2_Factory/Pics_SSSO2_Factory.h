// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PICS_SSSO2_FACTORY_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// PICS_SSSO2_FACTORY_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#include <string>

#ifdef PICS_SSSO2_FACTORY_EXPORTS
#define PICS_SSSO2_FACTORY_API extern "C" __declspec(dllexport)
#else
#define PICS_SSSO2_FACTORY_API extern "C" __declspec(dllimport)
#endif

#define SPU_BUFFER_SIZE 2048

typedef struct
{
	char *szNetworkCode;							///< ' Unlock code for Network Lock.
	char *szServiceProviderCode;					///< ' Unlock code for Service Provider Lock.
	char *szCorporateCode;							///< ' Unlock code for Corporate Lock.
	char *szNetworkSubsetCode;						///< ' Unlock code for Network Subset Lock.
	char *szSimCode;								///< ' Unlock code for SIM Lock.
	char *szUsimCode;								///< ' Unblock code for User SIM Lock.
	char *szReserved;								///< ' Reserved code for future usage.
} T_SIM_UNLOCK_CODE;

typedef int (__stdcall *ISimEnc_AllocSimEncObj)(int, void*&);
typedef int (__stdcall *ISimEnc_SetImei)(const void*, char*, int);
typedef int (__stdcall *ISimEnc_SetFlashSerialNumber)(const void*, char*, int);
typedef int (__stdcall *ISimEnc_SetSimLockFile)(const void*, char*);
typedef int (__stdcall *ISimEnc_SetUnlockCodes)(const void*, T_SIM_UNLOCK_CODE *ptSimUnlockCode);
typedef int (__stdcall *ISimEnc_CheckSimLockData)(const void*,char*, int);
typedef int (__stdcall *ISimEnc_FreeSimEncObj)(const void *);
typedef int (__stdcall *ISimEnc_GenSimLockData)(const void *hSimEncHandle, unsigned char *pcSimLockData, int *pnSimLockDataLen);


PICS_SSSO2_FACTORY_API bool GetBytesFromFactoryArea(const char* szFactory, const char* szIndex, const char* szBytes, char* szData);
PICS_SSSO2_FACTORY_API bool SetGSN(char* szFactory, char* szGSNString);
PICS_SSSO2_FACTORY_API bool SetPicasso(char* szFactory, const char* szPicassoNo);
PICS_SSSO2_FACTORY_API bool SetTranceiver(char* szFactory,const char* szTransNO);
PICS_SSSO2_FACTORY_API bool SetNVItem(char* szFactory, char* szNVItem, char*szIntFlag);

PICS_SSSO2_FACTORY_API bool SetKLFByte(char* szFactory,char *szComputeName);
PICS_SSSO2_FACTORY_API bool ComparePicasso(char* szFactory, const char* szPicassoNo);
PICS_SSSO2_FACTORY_API bool CompareTranceiver(char* szFactory, const char* szTransNO);
PICS_SSSO2_FACTORY_API bool SetNV7109Data64Encode(char* szData);
PICS_SSSO2_FACTORY_API bool ComCtryObjInitial(const char* szCurDirect);

/////////////// PICS //////////////////////
PICS_SSSO2_FACTORY_API bool Create_IMEI_PICS(char *szInIMEI,char * szRes);
PICS_SSSO2_FACTORY_API bool Create_IMEI_PICS_V2(char *szInIMEI,char * szRes);
PICS_SSSO2_FACTORY_API bool QuerySpc_PICS(char *szInIMEI,char *szRes,char *szOutSpc);
PICS_SSSO2_FACTORY_API bool GetFSNCode_PICS(char *szInIMEI,char * szFSN,char *szRes);
PICS_SSSO2_FACTORY_API bool GetSPCCode_PICS(char *szInIMEI,char * szSPC,char *szRes);
PICS_SSSO2_FACTORY_API bool QueryFSNCode_PICS(char *szInIMEI,char * szFSN,char *szRes);
PICS_SSSO2_FACTORY_API bool GetSIMData_PICS(char* szSimType,char *szInIMEI,char * szFSN,char *szSimDataFile ,char *szSimDataFromPICS,char *szSimDataSize,char *szRes);
PICS_SSSO2_FACTORY_API bool QueryUnlockUnblockCode_PICS(char *IMEI_Array,
													char *Network_Unlock_Code_Array,
													char *Service_Provider_Unlock_Code_Array,
													char *Coporate_Unlock_Code_Array,
													char *Network_Subset_Unlock_Code_Array,
													char *User_SIM_Unlock_Code_Array,
													char *User_USIM_Unlock_Code_Array,
													char *Serial_Code_Array,
													char *SPC_Array,
													char *Phone_Lock_Code_Array);

/////////////// SSS02 //////////////////////
PICS_SSSO2_FACTORY_API bool Check_MAL_PN_SSSO2(char *szInIMEI,char *szPort,char *szInMal,char *szErrCode, char *szErrMsg);
PICS_SSSO2_FACTORY_API bool GetProcessResult_SSS02(char *Picasso, char* Function, char* Model, char* szErrCode, char* szErrMsg);
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
											   char *szStatusOutput);
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
										char *szStatusOutput);
PICS_SSSO2_FACTORY_API bool QueryIMEI_SSSO2(char *szStationtState,
											   char *pIMEI,
											   char *szMSN,
											   char *szBTAddr,
											   char *szMACAddr,
											   char *szSWver,
											   char *szHWver,
											   char *szErrCode, 
											   char *szErrMsg,
											   char *szStatusOutput);
PICS_SSSO2_FACTORY_API bool UpdateIMEIStatus_SSSO2(char *pIMEI,char *szSWver,char *szHWver,char *szMal,char *szErrCode, char *szErrMsg);
PICS_SSSO2_FACTORY_API bool UpdateIMEIStatus_SSSO2_F(char *pIMEI,char *szErrCode, char *szErrMsg);  //Sunlight add for leopard, 2011/01/06
PICS_SSSO2_FACTORY_API bool UpdateIMEIStatus_SSSO2_R(char *pIMEI,char *szErrCode, char *szErrMsg);
PICS_SSSO2_FACTORY_API bool UpdateGSNPN_SSSO2(char *pPICASSO,char *pGSN,char *pPN,char *szErrCode, char *szErrMsg);

PICS_SSSO2_FACTORY_API bool CheckPicassoSMT_SSS02(char *Picasso, char* szProcess);
PICS_SSSO2_FACTORY_API bool CheckPicassoPICS_SSS02(char* szModel, char* Picasso, char* szIMEI, char* szMobileID, char* szProcess, char* szError);
PICS_SSSO2_FACTORY_API bool CheckPiacssoLuna_SSS02(char *Picasso, char *szErrCode, char *szErrMsg);

PICS_SSSO2_FACTORY_API bool QuerySN5_SSS02(char* szIMEI, char* szServiceTag, char *szErrCode, char *szErrMsg);

PICS_SSSO2_FACTORY_API bool QueryMobileIDByPicasso_SSS02(char* szModel, char* szPicasso, char* szMobileID, char *szErrCode, char *szErrMsg);
PICS_SSSO2_FACTORY_API bool UpdateMobileIDByPicasso_SSS02(char* szModel, char* szPicasso, char* szMobileID, char *szErrCode, char *szErrMsg);

/* Luna: Alex Add Start 2010/11/23 */
PICS_SSSO2_FACTORY_API bool UpdateIMEIST_SSSO2(char* szServiceTag, char* szIMEI, char* szErrCode, char* szErrMsg);
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
											   char *szStatusOutput);
/* Luna: Alex Add End 2010/12/07 */

/* Wolf: Sunlight Add Start 2011/08/23 */
PICS_SSSO2_FACTORY_API bool QueryPVKforAcer_SSSO2(char * sz_IMEI,char *sz_PVK,char *sz_ErrCode, char *sz_ErrMsg);
/* Wolf: Sunlight Add end 2011/08/23 */

/////////////// SimEncorder //////////////////


PICS_SSSO2_FACTORY_API bool CheckSimData_SimEncorder(char* szSimType,
												 char *szIMEIFromPhone,
												 char *szFSN,
												 char*szIMEIFromScanner,
												 char *szSimDateFileName,
												 char *szSimDataForPhone_Base64,
												 char *szLogString);
PICS_SSSO2_FACTORY_API bool EraseKLFByte(char* szFactory);

/*sunlight add 20111208*/
PICS_SSSO2_FACTORY_API bool UpdateIMEIStatus_F(char *pIMEI,char *szSWver,char *szHWver,char *szMal,char *szErrCode, char *szErrMsg);
PICS_SSSO2_FACTORY_API bool UpdateIMEIStatus_I(char *pIMEI,char *szSWver,char *szErrCode, char *szErrMsg); 
/*Sunlight add end*/
/*20140626 Magan Mei Add Start*/
PICS_SSSO2_FACTORY_API bool UpdateIMEIStatus_B(char *pIMEI,char *szErrCode, char *szErrMsg);
PICS_SSSO2_FACTORY_API bool UpdateIMEIStatus_W(char *pIMEI,char *szErrCode, char *szErrMsg);
/*20140626 Magan Mei Add End*/

/*Sunlight add 20120703*/
PICS_SSSO2_FACTORY_API bool UpdateNAL_SSSO2(char *pIMEI,char *szNAL,char *szErrCode, char *szErrMsg);

/*Sunlight add 20120706*/
PICS_SSSO2_FACTORY_API bool UpdateCustomerSW_SSSO2(char *pIMEI,char *szModel,char * szPartNo, char * szCustsw,char *szErrCode, char *szErrMsg);
PICS_SSSO2_FACTORY_API bool GetAliyunByIMEI_SSSO2(char *pIMEI,char *szUUID,char * szErrCode, char *szErrMsg);

/*Sunlight add 20120827*/
PICS_SSSO2_FACTORY_API bool CheckCustomerPN_SSSO2(char *pIMEI,char * szErrCode, char *szErrMsg);

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
												   char *szErrMsg);
