// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CMDMODULEEXPORT_API
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CMDMODULEEXPORT_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CMDEXPORT_EXPORTS
#define CMDMODULEEXPORT_API extern "C" __declspec(dllexport)
#else
#define CMDMODULEEXPORT_API extern "C" __declspec(dllimport)
#endif

// QLib
#include "QCTCmdModule\QualcommCmdModule.h"
// ADB
#include "ADBCmdModule\ADBCmdModule.h"
// FTD
#include "FTDCmdModule\FTDCmdModule.h"

namespace CommonFunction
{
	bool HexStringToByteArray(const char* szHexData, BYTE*& ByteArray, size_t& Len) ;
	void ByteArrayToHexString (BYTE* pByte, char* chString, long ByteCount) ;
	int HexBcdToDigit(BYTE *pucHexBcdBuf, int iHexBcdBufSize, char *sDigitBuf, int iDigitBufSize) ;
	int DigitToHexBcd(char *sDigitBuf, int iDigitBufSize, BYTE *pucHexBcdBuf, int iHexBcdBufSize) ;
	bool Common_FindKeyword (char* szTargetString, char* szKeyword) ;
	std::string FileDigest(const std::string& file) ;
}

namespace Berlin
{
    namespace CommonAPI
    {
        CMDMODULEEXPORT_API bool GetMD5_SPU(char *szFilePath, char *szMD5) ;
    }

	namespace AT_Control
	{
		CMDMODULEEXPORT_API bool ATControl_SPU(const char* szPort, char* szCommand, char* szResponse) ;	
	}

	namespace RAWCmd_Diag
	{
		CMDMODULEEXPORT_API bool RAWCmd_Diag_SPU(const char* szPort, char* szCommand, char* szResponse) ;
	}

	namespace Detect_COMPort
	{
		CMDMODULEEXPORT_API bool Detect_COMPortExist_SPU(char* szPort, char* szRetryTime, char* szRetryPeriod, char* szKeyword, char* szReturnPort) ;
    }

    namespace ADBCmd
    {
        CMDMODULEEXPORT_API bool ADBCmd_Devices_SPU(char* szRetryTime, char* szRetryPeriod, char* szResponse) ;
        CMDMODULEEXPORT_API bool ADBCmd_Common_SPU(char* szRetryTime, char* szRetryPeriod, char* szADBCmd, char* szResponse, char* szErrMsg) ;
        CMDMODULEEXPORT_API bool ADBCmd_Common_NoResponse_SPU(char* szRetryTime, char* szRetryPeriod, char* szADBCmd, char* szErrMsg) ;
        CMDMODULEEXPORT_API bool ADBCmd_KillServer_SPU(const char* szRetryTime, const char* szRetryPeriod) ;
        CMDMODULEEXPORT_API bool ADBCmd_Fastboot_Common_SPU(char* szRetryTime, char* szRetryPeriod, char* szFastbootCmd, char* szResponse, char* szErrMsg) ;
    }

    namespace FTDCmd
    {
        CMDMODULEEXPORT_API bool FTDCmd_Common_SPU (const char* szADBDevice, const char* szRetryTime, const char* szRetryPeriod, char *szFTDCmd, char* szOutput, char* szErrMsg) ;
        CMDMODULEEXPORT_API bool FTDCmd_Active_SPU (const char* szADBDevice, const char* szRetryTime, const char* szRetryPeriod, char* szErrMsg) ;
        CMDMODULEEXPORT_API bool FTDCmd_CheckIsFTD_SPU (const char* szADBDevice, const char* szRetryTime, const char* szRetryPeriod, char* szErrMsg) ;
    }

    namespace DiagCmd
    {
        CMDMODULEEXPORT_API bool DiagCmd_Common_SPU (const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod, char* szRequest, char* szResponse, char* szErrMsg) ;
        CMDMODULEEXPORT_API bool DiagCmd_GetSpecifyByte_SPU (const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod, char* szRequestWithIdx, char* szResponse, char* szErrMsg) ;
    }

	namespace PhoneInfo
	{
		CMDMODULEEXPORT_API bool FA_Read_SPU( const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod, char *szFABuffer );
		CMDMODULEEXPORT_API bool FA_Write_SPU( const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod, char *szFABuffer );
		CMDMODULEEXPORT_API bool ReadIMEI_SPU(const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod, char returnIMEI[16]) ;
	}
}