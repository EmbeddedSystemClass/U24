// 1.h : main header file for the 1 DLL
//

#pragma once

#define  FA_EXPORTS // where to define this
#ifdef FA_EXPORTS
#define FA_OPERATOR_API extern "C" __declspec(dllexport)
#else
#define FA_API extern "C" __declspec(dllimport)
#endif

// CMy1App
// See 1.cpp for the implementation of this class
//

#include <string>
#include <afxmt.h>
using std::string;

HMODULE hFADll = NULL;
typedef bool (*ReadFA)(char*, unsigned char*, char*, char*);
typedef bool (*WriteFA)(char*, unsigned char*, char*, char*);

ReadFA pfnReadFA = NULL;
WriteFA pfnWriteFA = NULL;
bool SearchTargetStation(unsigned char* faData, int index, int& nTestResult);
bool SearchTargetStation(unsigned char* faData, int index, int& nTargetPosition, int& nTargetStationInfoLength);

//fix the problem the data 0x00 will cut the block's length.
//for example strle(faData) will be cut in the position 0x00
//so all 0x00 will store into '0'
//when restore will restore to 0x00
unsigned char MaybeStore0(unsigned char inData);
unsigned char MaybeRestore0(unsigned char inData);
bool WriteFA_SpecialForStore0(char* szPort, unsigned char* faData, char* szErrCode, char* szErrMsg);
bool ReadFA_SpecialForRetore0(char* szPort, unsigned char* faData, char* szErrCode, char* szErrMsg);

int TransferIntToHex_BCD(int nInt);

CCriticalSection CritSection_FA;

//static unsigned char szTempDRMKey[512] = {0};

//bool ReadFA (char* szConnect, 
//			 unsigned char* szFAData, 
//			 char* szErrCode,
//			 char* szErrMsg);
//
//bool WriteFA (char* szConnect, 
//			  unsigned char* szFAData, 
//			  char* szErrCode,
//			  char* szErrMsg);

FA_OPERATOR_API bool FA_ReadFactoryArea(char* szPort, 
										char* szFAData, 
										char* szErrCode,
										char* szErrMsg);

FA_OPERATOR_API bool FA_RWFA_Wolf (char* szPort);
FA_OPERATOR_API bool FA_RWMobileID_Wolf (char* szPort, char* szMobileID);

FA_OPERATOR_API bool FA_WriteFactoryArea(char* szPort, 
										 char* szFAData, 
										 char* szErrCode,
										 char* szErrMsg);

FA_OPERATOR_API bool FA_EraseFactoryArea(char* szPort, 
										 char* szErrCode,
										 char* szErrMsg);

FA_OPERATOR_API bool FA_EraseTargetStationInfo(char* szPort,
											   char* index,
											   char* szErrCode,
											   char* szErrMsg);

FA_OPERATOR_API bool FA_WriteMobileID  (char* szPort, 
										char* szSlot,
										char* szErrCode,
										char* szErrMsg);

FA_OPERATOR_API bool FA_ReadMobileID (char* szPort, 
									  char* szMobileID,
									  char* szErrCode,
									  char* szErrMsg);

FA_OPERATOR_API bool FA_ReadModelName (char* szPort, 
									   char* szModelname,
									   char* szErrCode,
									   char* szErrMsg);

FA_OPERATOR_API bool FA_WriteModelName (char* szPort, 
										char* szModelName,
										char* szErrCode,
										char* szErrMsg);

FA_OPERATOR_API bool FA_WriteFAVersion (char* szPort, 
										char* szFAVersion,
										char* szErrCode,
										char* szErrMsg);

FA_OPERATOR_API bool FA_ReadFAVersion (char* szPort, 
									   char* szFAVersion,
									   char* szErrCode,
									   char* szErrMsg);

FA_OPERATOR_API bool FA_WriteMEVersion (char* szPort, 
										char* szMEVersion,
										char* szErrCode,
										char* szErrMsg);

FA_OPERATOR_API bool FA_ReadMEVersion (char* szPort, 
									   char* szMEVersion,
									   char* szErrCode,
									   char* szErrMsg);

FA_OPERATOR_API bool FA_WriteSWVersion (char* szPort, 
										char* szSWVersion,
										char* szErrCode,
										char* szErrMsg);

FA_OPERATOR_API bool FA_ReadSWVersion (char* szPort, 
									   char* szSWVersion,
									   char* szErrCode,
									   char* szErrMsg);

FA_OPERATOR_API bool FA_WritePCBAVersion (char* szPort, 
										  char* szPCBAVersion,
										  char* szErrCode,
										  char* szErrMsg);

FA_OPERATOR_API bool FA_ReadPCBAVersion (char* szPort, 
										 char* szPCBAVersion,
										 char* szErrCode,
										 char* szErrMsg);

FA_OPERATOR_API bool FA_WritePicassoNumber (char* szPort, 
											char* szPicasso,
											char* szErrCode,
											char* szErrMsg);

FA_OPERATOR_API bool FA_ReadPicassoNumber (char* szPort, 
										   char* szPicasso,
										   char* szErrCode,
										   char* szErrMsg);

FA_OPERATOR_API bool FA_WriteTranceiverNumber (char* szPort, 
											   char* szXCVR,
											   char* szErrCode,
											   char* szErrMsg);

FA_OPERATOR_API bool FA_ReadTranceiverNumber  (char* szPort, 
											   char* szXCVR,
											   char* szErrCode,
											   char* szErrMsg);

FA_OPERATOR_API bool FA_WriteAllVersion   (char* szPort, 
										   char* szModelName,
										   char* szSWVersion,
										   char* szPCBAVersion,
										   char* szFAVersion,
										   char* szErrCode,
										   char* szErrMsg);

FA_OPERATOR_API bool FA_ReadAllVersion (char* szPort, 
										char* szModelName,
										char* szMobileID,
										char* szSWVersion,
										char* szPCBAVersion,
										char* szPicasso,
										char* szFAVersion,
										char* szErrCode,
										char* szErrMsg);

FA_OPERATOR_API bool FA_ReadStationInfo(char* szPort, 
										char* szStationIndex,
										char* szLineType,
										char* szTestResult,
										char* szTestCount,
										char* szTestDateTime,
										char* szTestLineStation,
										char* szReserve,
										char* szErrCode,
										char* szErrMsg);

FA_OPERATOR_API bool FA_WriteStationInfo(char* szPort, 
										 char* szStationIndex,
										 char* szLineType,
										 char* szStationExternLen,
										 char* szTestResult,
										 char* szReserve,
										 char* szStationExternInfo,
										 char* szErrCode,
										 char* szErrMsg);

FA_OPERATOR_API bool FA_CheckStationInfo(char* szPort, 
										 char* szStationIndex,	
										 char* szErrCode,
										 char* szErrMsg);

FA_OPERATOR_API bool FA_CheckMobileID     (char* szPort, 
										   char* szMobileID,
										   char* szErrCode,
										   char* szErrMsg);

FA_OPERATOR_API bool FA_CheckUISWVersion  (char* szPort, 
										   char* szUISW,
										   char* szFASW,
										   char* szErrCode,
										   char* szErrMsg);

FA_OPERATOR_API bool FA_CheckUIPCBAVersion (char* szPort, 
											char* szUIPCBA,
											char* szFAPCBA,
											char* szErrCode,
											char* szErrMsg);

FA_OPERATOR_API bool FA_CheckUIMEVersion  (char* szPort, 
										   char* szUIME,
										   char* szFAME,
										   char* szErrCode,
										   char* szErrMsg);

FA_OPERATOR_API bool FA_CheckUIPicasso    (char* szPort, 
										   char* szUIPicasso,
										   char* szFAPicasso,
										   char* szErrCode,
										   char* szErrMsg);

FA_OPERATOR_API bool FA_CheckUITranceiver (char* szPort, 
										   char* szUIXCVR,
										   char* szFAXCVR,
										   char* szErrCode,
										   char* szErrMsg);

FA_OPERATOR_API bool FA_CheckUIModelName  (char* szPort, 
										   char* szUIModelName,
										   char* szFAModelName,
										   char* szErrCode,
										   char* szErrMsg);

FA_OPERATOR_API bool FA_CheckPicassoRule  (char* szPort, 
										   char* szPicasso,
										   char* szModelID,
										   char* szErrCode,
										   char* szErrMsg);


