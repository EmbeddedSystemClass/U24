#ifndef _SIM_ENCODER_API_H_
#define _SIM_ENCODER_API_H_

/*--------------------------------------------------------------------------*/
/* TYPE DEFINITION                                                          */
/*--------------------------------------------------------------------------*/
/// Enum to define return value from SIM encorder DLL interfaces.
typedef enum
{
	SIM_RET_SUCCESS,								///< ' 0 = Success.
	SIM_RET_NONE_SUPP_FUNC,							///< ' 1 = The specific function is not supported for specifiec SIM type.
	SIM_RET_NONE_ENOUGH_MEM,						///< ' 2 = No enough memory.
	SIM_RET_INVALID_PARAM,							///< ' 3 = The specific parameter is not valid.
	SIM_RET_INVALID_OBJECT,							///< ' 4 = The specific object handle is not valid.
	SIM_RET_NO_SIMMODE,								///< ' 5 - The sim mode is not set yet
	SIM_RET_NO_IMEI,								///< ' 6 = IMEI is not set yet.
	SIM_RET_NO_FSN,									///< ' 7 = FSN is not set yet.
	SIM_RET_NO_SIM_FILE,							///< ' 8 = SIM file is not set yet.
	SIM_RET_NO_UNLOCK_CODE,							///< ' 9 = Unlock code is not set yet.
	SIM_RET_NO_RSAKEY,								///< ' 10 - RSA Key is not set yet.
	SIM_RET_INVALID_USIM_CODE,						///< ' 11 - USim code is the same with a certain one code
	SIM_RET_FILE_PARSE_FAIL,						///< ' 12 = Parse SIM file fails.
	SIM_RET_BUF_NOT_ENOUGH,							///< ' 13 = The specific output buffer is too small.	
	SIM_RET_RESULT_FAIL,							///< ' 14 = The test result fails.
	SIM_RET_LOCNAME_TOOLONG,						///< ' 15 - The loc file name is too long
	SIM_RET_NONE_ENOUGH_LOCK_CAPACITY,              ///< ' 16 - The maximum lock capacity is 25
	SIM_RET_LEN_TOO_LONG,                           ///< ' 17 - Value is too long for TLV
	SIM_RET_PENALTY_BEYOND_CAPACITY,                ///< ' 18 - Penalty is beyond encoder capacity
	SIM_RET_SECURITY_ERROR,                         ///< ' 19 - Security algorithms error
	SIM_RET_ENCODER_BEYOND_CAPACITY                 ///< ' 20 - Encoder cannot encode this SIM file.
} T_SIM_RET_VALUE;

typedef struct
{
	char *szNetworkCode;							///< ' Unlock code for Network Lock.
	char *szServiceProviderCode;					///< ' Unlock code for Service Provider Lock.
	char *szCorporateCode;							///< ' Unlock code for Corporate Lock.
	char *szNetworkSubsetCode;						///< ' Unlock code for Network Subset Lock.
	char *szSimCode;								///< ' Unlock code for SIM Lock.
	char *szUsimCode;								///< ' Unblock code for User SIM Lock.
	char *szReserved;								///< ' Reserved code for furture usage.
} T_SIM_UNLOCK_CODE;

typedef struct
{
	unsigned char *pPrivateExp;
	unsigned char *pPublicExp;
	unsigned char *pModulus;
	int iPrivateExpSize;
	int iPublicExpSize;
	int iModulusSize;
} T_RSA;

typedef int (__stdcall *pISimEnc_AllocSimEncObj)(int nSimTypeId, void *&hSimEncHandle);
typedef int (__stdcall *pISimEnc_FreeSimEncObj)(const void *hSimEncHandle);
typedef int (__stdcall *pISimEnc_EnableTextLog)(const void *hSimEncHandle, bool bEnableTextLog, char *szTextLogFilePath);
typedef int (__stdcall *pISimEnc_SetImei)(const void *hSimEncHandle, char* szImei, int nImeiLen);
typedef int (__stdcall *pISimEnc_SetFlashSerialNumber)(const void *hSimEncHandle, char *szFsn, int nFsnLen);
typedef int (__stdcall *pISimEnc_SetSimLockFile)(const void *hSimEncHandle, char *szSimFilePath);
typedef int (__stdcall *pISimEnc_SetUnlockCodes)(const void *hSimEncHandle, T_SIM_UNLOCK_CODE *ptSimUnlockCode);
typedef int (__stdcall *pISimEnc_GenSimLockData)(const void *hSimEncHandle, unsigned char *pcSimLockData, int *pnSimLockDataLen);
typedef int (__stdcall *pISimEnc_CheckSimLockData)(const void *hSimEncHandle, unsigned char *pcSimLockData, int nSimLockDataLen);
typedef int (__stdcall *pISimEnc_SetRSAKey)(const void *hSimEncHandle, T_RSA *ptRSAKey);


class SIMEncoder_Function
{
public:
	SIMEncoder_Function();
	virtual ~SIMEncoder_Function();

private:
	static HMODULE m_hEncoderModule;
	static void *m_hSimEncHandle;

public:
	bool Initial_SIMEncoder (char* szErrMsg);
	bool ISimEnc_AllocSimEncObj(int nSimTypeId, char* szErrMsg);
	bool ISimEnc_FreeSimEncObj();
	bool ISimEnc_EnableTextLog(bool bEnableTextLog, char *szTextLogFilePath, char* szErrMsg);
	bool ISimEnc_SetImei(char *szImei, int nImeiLen, char* szErrMsg);
	bool ISimEnc_SetFlashSerialNumber(char *szFsn, int nFsnLen, char* szErrMsg);
	bool ISimEnc_SetSimLockFile(char *szSimFilePath, char* szErrMsg);
	bool ISimEnc_SetUnlockCodes(T_SIM_UNLOCK_CODE *ptSimUnlockCode, char* szErrMsg);
	bool ISimEnc_GenSimLockData(char *pcSimLockData, int *pnSimLockDataLen, char* szErrMsg);
	bool ISimEnc_CheckSimLockData(char *pcSimLockData, int nSimLockDataLen, char* szErrMsg);
	bool ISimEnc_SetRSAKey(T_RSA *ptRSAKey, char* szErrMsg);
};

#endif // End of #ifndef _SIM_ENCODER_API_H_