#include "stdafx.h"
//#include "QualDLL.h"
#include "QualDLL_API.h"
#include "QDLCtrl.h"
//#include "Logger.h"

#pragma warning(disable: 4996)


/*--------------------------------------------------------------------------*/
/* Global Variables				                                            */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTION DEFINITIONS                                               */
/*--------------------------------------------------------------------------*/
static bool IsPathExist(const CString path)
{
	return (::GetFileAttributes(path) != INVALID_FILE_ATTRIBUTES);
}

/*--------------------------------------------------------------------------*/
/* EXPORTED FUNCTION DEFINITIONS                                            */
/*--------------------------------------------------------------------------*/
// If 'MFC regular DLL' is dynamically linked to 'MFC source library DLL',
// please insert 'AFX_MANAGE_STATE' macro as the first line of any exported function.
// example: FACTORYDLLAPI int func()
//			{
//				AFX_MANAGE_STATE(AfxGetStaticModuleState());
//				return 0;
//			}
int CheckSignedHeaderHex(const char* const path)
{
	if (!IsPathExist(path)) {
		TRACE("%s is not exist\n", path);
		return ERROR_PATH_NOT_FOUND;
	}

	const char* const pch = strrchr(path, '\\') + 1;
	bool isHex =
		strncmp(pch, "NPRG8650.hex", strlen(pch)) == 0 ||
		strncmp(pch, "ENPRG8650.hex", strlen(pch)) == 0;
	if (!isHex) {
		TRACE("filename is not recognized: %s\n", pch);
		return ERROR_FILE_INVALID;
	}

	char SignatureSize[9] = {0};
	char CertChainSize[9] = {0};

	FILE* fp;
	fp = fopen(path, "rb");
	if (fp == NULL) {
		return ERROR_FILE_CORRUPT;
	}
	else {
		fseek(fp, 95, SEEK_SET);
		fread(SignatureSize, 1, 8, fp);
		fseek(fp, 95+8+21, SEEK_SET);
		fread(CertChainSize, 1, 8, fp);
		fclose(fp);
	}

	TRACE("check hex: %s\n", path);
	TRACE("hex signature:    %s\n", SignatureSize);
	TRACE("hex certificates: %s\n", CertChainSize);

	int nRetCode = ERROR_FUNCTION_FAILED;
	if((memcmp(SignatureSize, "00000000", 8) == 0) || (memcmp(CertChainSize, "00000000", 8) == 0)) {
		TRACE("%s is not signed\n", pch);
		nRetCode = ERROR_INVALID_DATA;
	}
	else {
		TRACE("%s is signed\n", pch);
		nRetCode = NO_ERROR;
	}

	return nRetCode;
}

typedef struct QcImageHeader {
	UINT32 image_id;
	UINT32 header_vsn_num;
	UINT32 image_src;
	UINT32 image_dest_ptr;
	UINT32 image_size;
	UINT32 code_size;
	UINT32 signature_ptr;
	UINT32 signature_size;
	UINT32 cert_chain_ptr;
	UINT32 cert_chain_size;
} q_img_hdr_t;

typedef struct QcImageHeaderDbl{
	UINT32 codeword;            /* Codeword/magic number defining flash type
								 information. */
	UINT32 magic;               /* Magic number */
	UINT32 RESERVED_0;          /* RESERVED */
	UINT32 RESERVED_1;          /* RESERVED */
	UINT32 RESERVED_2;          /* RESERVED */
	UINT32 dbl_src;             /* Location of DBL in flash. This is given in
								 byte offset from beginning of flash.  */
	UINT8* dbl_dest_ptr;        /* Pointer to location to store DBL in RAM.
								 Also, entry point at which execution begins. */
	UINT32 dbl_image_size;      /* Size of DBL image in bytes */
	UINT32 dbl_code_size;       /* Size of code region in DBL image in bytes */
	UINT8* dbl_sig_ptr;         /* Pointer to images attestation signature */
	UINT32 dbl_sig_size;        /* Size of the attestation signature in bytes */
	UINT8* dbl_cert_chain_ptr;  /* Pointer to the chain of attestation
								 certificates associated with the image. */
	UINT32 dbl_cert_chain_size; /* Size of the attestation chain in bytes */
	UINT32 fsbl_image_size;     /* Size of FSBL image in bytes */
	UINT32 fsbl_src;            /* Location of FSBL in flash. This is given in
								 byte offset from beginning of flash.  */
	UINT8* fsbl_dest_ptr;       /* Pointer to location to store FSBL in RAM. */
	UINT32 RESERVED_3;          /* RESERVED */
	UINT32 RESERVED_4;          /* RESERVED */
	UINT32 RESERVED_5;          /* RESERVED */
	UINT32 RESERVED_6;          /* RESERVED */
} q_img_hdr_dbl;

int CheckSignedHeaderMbn(const char* const path)
{
	if (!IsPathExist(path)) {
		TRACE("%s is not exist\n", path);
		return ERROR_PATH_NOT_FOUND;
	}

	const char* const pch = strrchr(path, '\\') + 1;
	int offset = 0;
	if (strncmp(pch, "dbl.mbn", strlen(pch)) == 0) {
		offset = 8192;
	}
	else if (strncmp(pch, "osbl.mbn", strlen(pch)) == 0) {
		offset = 0;
	}
	else if (strncmp(pch, "amss.mbn", strlen(pch)) == 0) {
		offset = 4096;
	}
	else if (strncmp(pch, "dsp1.mbn", strlen(pch)) == 0) {
		offset = 4096;
	}
	else {
		TRACE("filename is not recognized: %s\n", pch);
		return ERROR_FILE_INVALID;
	}

	int nRetCode = ERROR_FUNCTION_FAILED;
	FILE* fp = fopen(path, "rb");
	if (fp == NULL) {
		nRetCode = ERROR_FILE_CORRUPT;
	}
	else {
		q_img_hdr_t header = {0};
		q_img_hdr_dbl header_dbl = {0};
		fseek(fp, offset, SEEK_SET);
		if (strncmp(pch, "dbl.mbn", strlen(pch)) == 0) {
			fread(&header_dbl, 1, sizeof(header_dbl), fp);
			if(header_dbl.dbl_sig_size == 0 || header_dbl.dbl_cert_chain_size == 0) {
				TRACE("%s is not signed\n", pch);
				nRetCode = ERROR_INVALID_DATA;
			}
			else {
				TRACE("%s is signed\n", pch);
				nRetCode = NO_ERROR;
			}
		}
		else {
			fread(&header, 1, sizeof(header), fp);
			if(header.signature_size == 0 || header.cert_chain_size == 0) {
				TRACE("%s is not signed\n", pch);
				nRetCode = ERROR_INVALID_DATA;
			}
			else {
				TRACE("%s is signed\n", pch);
				nRetCode = NO_ERROR;
			}
		}
		fclose(fp);
	}

	return nRetCode;
}

int CheckSignedInfoHex(const char* const path)
{
	if (!IsPathExist(path)) {
		TRACE("%s is not exist\n", path);
		return ERROR_PATH_NOT_FOUND;
	}
	const char* const pch = strrchr(path, '\\') + 1;
	bool isHex =
		strncmp(pch, "NPRG8650.hex", strlen(pch)) == 0 ||
		strncmp(pch, "ENPRG8650.hex", strlen(pch)) == 0;
	if (!isHex) {
		TRACE("filename is not recognized: %s\n", pch);
		return ERROR_FILE_INVALID;
	}

	char* SBL_ATTRIBUTE[5] = {"MODEL_ID","OEM_ID","DEBUG","HW_ID","SW_ID"};
	char* HEX_ATTRIBUTE[5] = {"4D4F44454C5F4944","4F454D5F4944","4445425547","48575F4944","53575F4944"};
	char* HEX_INFO[5] = {"30303032","30303033","30303030303030303030303030303046","30303234303045313030303330303032","30303030303030303030303030303033"};

	TRACE("check hex: %s ", pch);
	FILE* fp = fopen(path, "rb");
	if (fp == NULL) {
		return ERROR_FILE_CORRUPT;
	}
	else {
		UINT8 buffer[512] = {0};
		fseek(fp, -5442, SEEK_END);
		fread(buffer, 1, sizeof(buffer), fp);
		fclose(fp);

		bool isFound = false;
		UINT8 filtered_buffer[512] = {0};
		UINT8 hex_sign_info[32] = {0};
		UINT8 check_buffer[32] = {0};
		for(int i = 0, j = 0; i < sizeof(buffer); i ++) {
			if(buffer[i] == '\r') {
				i += 10;
				j -= 2;
			}
			else {
				filtered_buffer[j] = buffer[i];
				j++;
			}
		}
		const int nMaxAttrs = 5;
		for (int i = 0; i < nMaxAttrs; i++) {
			isFound = false;
			for (int j = 0; j < sizeof(filtered_buffer); j++) {
				memcpy(check_buffer, filtered_buffer + j, 32);
				if (memcmp(check_buffer, HEX_ATTRIBUTE[i], strlen(HEX_ATTRIBUTE[i])) == 0) {
					isFound = true;
					memcpy(hex_sign_info, filtered_buffer + j - (strlen(HEX_INFO[i]) + 2), strlen(HEX_INFO[i]));
					if (memcmp(hex_sign_info, HEX_INFO[i], strlen(HEX_INFO[i])) == 0) {
						break;
					}
					else {
						TRACE("%s: %s INFO check fail\n", SBL_ATTRIBUTE[i], HEX_INFO[i]);
						return ERROR_INVALID_DATA;
					}
				}
			}
			if (!isFound) {
				TRACE("cannot find %s\n", SBL_ATTRIBUTE[i]);
				return ERROR_NOT_FOUND;
			}
		}
	}

	TRACE("INFO check ok\n");
	return NO_ERROR;
}

// MODEL_ID  OEM_ID  DEBUG             HW_ID             SW_ID
// 0002      0003    000000000000000F  002400E100030002  0000000000000003
int CheckSignedInfoMbn(const char* path)
{
	if (!IsPathExist(path)) {
		TRACE("%s is not exist\n", path);
		return ERROR_PATH_NOT_FOUND;
	}

	const char* const pch = strrchr(path, '\\') + 1;
	TRACE("check image: %s ", pch);

	typedef enum QImage {
		QIMG_UNKNOWN = -1,
		QIMG_DBL  = 0,
		QIMG_OSBL = 1,
		QIMG_AMSS = 2,
		QIMG_DSP1 = 3,
	} q_img_t;
	q_img_t nIndex = QIMG_UNKNOWN;
	int offset = 0;
	int origin = SEEK_SET;
	if (strncmp(pch, "dbl.mbn", strlen(pch)) == 0) {
		nIndex = QIMG_DBL;
		offset = -1922;
		origin = SEEK_END;
	}
	else if (strncmp(pch, "osbl.mbn", strlen(pch)) == 0) {
		nIndex = QIMG_OSBL;
		offset = -1922;
		origin = SEEK_END;
	}
	else if (strncmp(pch, "amss.mbn", strlen(pch)) == 0) {
		nIndex = QIMG_AMSS;
		offset = 4922;
		origin = SEEK_SET;
	}
	else if (strncmp(pch, "dsp1.mbn", strlen(pch)) == 0) {
		nIndex = QIMG_DSP1;
		offset = 4882;
		origin = SEEK_SET;
	}
	else {
		return ERROR_FILE_INVALID;
	}

	char* SBL_ATTRIBUTE[5] ={"MODEL_ID","OEM_ID","DEBUG","HW_ID","SW_ID"};
	char* SBL_INFO[5] ={"0002","0003","000000000000000F","002400E100030002","002400E100030002"};

	// dbl, osbl, amss, dsp1
	char* SBL_SW_ID_INFO[4] ={"0000000000000000","0000000000000000","0000000000000002","0000000000000004"};

	bool isFound = false;
	UINT8 buffer[512] = {0};
	UINT8 hex_sign_info[32] = {0};
	UINT8 check_buffer[32] = {0};

	FILE* fp = fopen(path, "rb");
	if (fp == NULL) {
		return ERROR_FILE_CORRUPT;
	}
	else {
		fseek(fp, offset, origin);
		fread(buffer, 1, sizeof(buffer), fp);
		fclose(fp);

		const int nMaxAttrs = 5;
		for (int i = 0; i < nMaxAttrs; i++) {
			isFound = false;
			for (int j = 0; j < sizeof(buffer); j++) {
				if(memcmp(buffer + j, SBL_ATTRIBUTE[i], strlen(SBL_ATTRIBUTE[i])) == 0) {
					isFound = true;
					memcpy(hex_sign_info, buffer + j - (strlen(SBL_INFO[i]) + 1), strlen(SBL_INFO[i]));
					if (i == 4) {
						if (memcmp(hex_sign_info, SBL_SW_ID_INFO[nIndex], strlen(SBL_SW_ID_INFO[nIndex])) == 0) {
							break;
						}
						else {
							TRACE("%s ID INFO check fail\n", SBL_SW_ID_INFO[nIndex]);
							return ERROR_INVALID_DATA;
						}
					}
					else {
						if (memcmp(hex_sign_info, SBL_INFO[i], strlen(SBL_INFO[i])) == 0) {
							break;
						}
						else {
							TRACE("%s: %s INFO check fail\n", SBL_ATTRIBUTE[i], SBL_INFO[i]);
							return ERROR_INVALID_DATA;
						}
					}
				}
			}
			if (!isFound) {
				TRACE("cannot find %s\n", SBL_ATTRIBUTE[i]);
				return ERROR_NOT_FOUND;
			}
		}
	}

	TRACE("INFO check ok\n");
	return NO_ERROR;
}

HANDLE QDL_Initial(char* szComPort)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

//	StartLogger();
	CQDLCtrl* pQDLCtrl = new CQDLCtrl;
	pQDLCtrl->ComSet(szComPort);
	return (HANDLE)pQDLCtrl;
}

int QDL_Close(HANDLE hResource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CQDLCtrl* pQDLCtrl = (CQDLCtrl*)hResource;
	delete pQDLCtrl;
	pQDLCtrl = NULL;
//	StopLogger();
	return NO_ERROR;
}

int QDL_Probe(HANDLE hResource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CQDLCtrl* pQDLCtrl = (CQDLCtrl*)hResource;
	return pQDLCtrl->ProbeEmdl();
}

int QDL_Nop(HANDLE hResource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CQDLCtrl* pQDLCtrl = (CQDLCtrl*)hResource;
	return pQDLCtrl->Nop();
}

int QDL_Set_CallBack_Function(HANDLE hResource, int(*CallBack)(const char*, int))
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CQDLCtrl* pQDLCtrl = (CQDLCtrl*)hResource;
	pQDLCtrl->SetSendMsg(CallBack);
	return TRUE;
}

int QDL_Get_OSBL_Version(HANDLE hResource, char * pthOsblVersion)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CQDLCtrl* pQDLCtrl = (CQDLCtrl*)hResource;
	return pQDLCtrl->GetVersion(pthOsblVersion);
}

int QDL_Set_Fastboot_Flag(HANDLE hResource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CQDLCtrl* pQDLCtrl = (CQDLCtrl*)hResource;
	return pQDLCtrl->SetFastbootFlag();
}

int QDL_Set_Fastboot_Serial(HANDLE hResource, char* szSerial) // NO_ERROR
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CQDLCtrl* pQDLCtrl = (CQDLCtrl*)hResource;
	return pQDLCtrl->SetFastbootSerial(szSerial);
}

int QDL_Reset(HANDLE hResource, char* pthHex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CQDLCtrl* pQDLCtrl = (CQDLCtrl*)hResource;
	return pQDLCtrl->Reset(pthHex);
}

int QDL_Upload_MultiImage_Emergency(HANDLE hResource, char* pthHex,
		char* pthPartition, char* pthDbl, char* pthFsbl, char* pthOsbl, bool IsRePartition)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CQDLCtrl* pQDLCtrl = (CQDLCtrl*)hResource;
	return pQDLCtrl->UploadMultiImageEmergency(pthHex, pthPartition, pthDbl, pthFsbl, pthOsbl, IsRePartition);
}

int QDL_Upload_MultiImage(
	HANDLE hResource,
	char* pthHex, char* pthPartition,
	char* pthDbl, char* pthFsbl, char* pthOsbl,
	char* pthAmss,   char* pthDsp1,  char* pthEfs2,
	char* pthAppsbl, char* pthApps, char* pthFlash,
	bool IsEraseEFS2, bool IsRePartition, SYNCHRONIZATION_t* sync)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CQDLCtrl* pQDLCtrl = (CQDLCtrl*)hResource;
	return pQDLCtrl->UploadMultiImage(
				pthHex, pthPartition,
				pthDbl, pthFsbl, pthOsbl,
				pthAmss, pthDsp1, pthEfs2,
				pthAppsbl, pthApps, pthFlash,
				IsEraseEFS2, IsRePartition, sync);
}

int QDL_Upload_MultiImage_Speedup(
	HANDLE hResource,
	char* pthHex, char* pthPartition,
	char* pthDbl, char* pthFsbl, char* pthOsbl,
	char* pthAmss,   char* pthDsp1, char* pthEfs2,
	char* pthAppsbl, char* pthApps, char* pthFlash, char* pthDt,
	char* pthBkFsbl, char* pthBkOsbl, char* pthBkDt, char* pthBkAppsbl, char* pthReserved,
	bool IsEraseEFS2, bool IsRePartition, SYNCHRONIZATION_t* sync)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CQDLCtrl* pQDLCtrl = (CQDLCtrl*)hResource;
	return pQDLCtrl->UploadMultiImageSpeedup(
				pthHex, pthPartition,
				pthDbl, pthFsbl, pthOsbl,
				pthAmss, pthDsp1, pthEfs2,
				pthAppsbl, pthApps, pthFlash, pthDt,
				pthBkFsbl, pthBkOsbl, pthBkDt, pthBkAppsbl, pthReserved,
				IsEraseEFS2, IsRePartition, sync);
}

int QDL_Upload_MultiImage_7XXX(
	HANDLE hResource,
	char* pthHex, char* pthPartition,  char* pthQcsblHd, char* pthQcsbl,
	char* pthOemsblHd, char* pthOemsbl, char* pthAmssHd, char* pthAmss,
	char* pthAppsblHd, char* pthAppsbl, char* pthAppsHd, char* pthApps,
	bool IsRePartition, bool IsSetFastbootFlag, bool IsEraseEFS2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CQDLCtrl* pQDLCtrl = (CQDLCtrl*)hResource;
	return pQDLCtrl->UploadMultiImage7k(pthHex, pthPartition,
			pthQcsblHd, pthQcsbl, pthOemsblHd, pthOemsbl,
			pthAmssHd, pthAmss, pthAppsblHd, pthAppsbl, pthAppsHd, pthApps,
			IsRePartition, IsSetFastbootFlag, IsEraseEFS2);
}

int QDL_Upload_MultiImage_7XXX_Emergency(
	HANDLE hResource,
	char* pthHex, char* pthPartition,  char* pthQcsblHd, char* pthQcsbl,
	char* pthOemsblHd, char* pthOemsbl, char* pthAmssHd, char* pthAmss,
	char* pthAppsblHd, char* pthAppsbl, char* pthAppsHd, char* pthApps,
	bool IsRePartition, bool IsSetFastbootFlag, bool IsEraseEFS2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CQDLCtrl* pQDLCtrl = (CQDLCtrl*)hResource;
	return pQDLCtrl->UploadMultiImage7kEmergency(pthHex, pthPartition,
			pthQcsblHd, pthQcsbl, pthOemsblHd, pthOemsbl,
			pthAmssHd, pthAmss, pthAppsblHd, pthAppsbl, pthAppsHd, pthApps,
			IsRePartition, IsSetFastbootFlag, IsEraseEFS2);
}

int QDL_Upload_MultiImage_8K_Emergency(
	HANDLE hResource,
	char* pthHex, char* pthFastboot,
	bool IsSetFastbootFlag, bool IsEraseEFS2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CQDLCtrl* pQDLCtrl = (CQDLCtrl*)hResource;
	//return pQDLCtrl->UploadMultiImage8kEmergency(pthHex, pthFastboot, IsSetFastbootFlag, IsEraseEFS2);
	return pQDLCtrl->UploadMultiImage8kEmergencyAndIRAMRead(pthHex, pthFastboot, IsSetFastbootFlag, IsEraseEFS2);
}

int QDL_DIAG_SWITCH_To_Dload(HANDLE hResource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CQDLCtrl* pQDLCtrl = (CQDLCtrl*)hResource;
	return pQDLCtrl->SwitchDiag2Dload();
}

int QDL_DIAG_NV_Write(HANDLE hResource, unsigned short ItemID,
		int nDataLength, unsigned char* pData, char* szSPC, unsigned short* Status)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CQDLCtrl* pQDLCtrl = (CQDLCtrl*)hResource;
	return pQDLCtrl->NvWrite(ItemID, nDataLength, pData, szSPC, Status);
}

int QDL_DIAG_NV_Read(HANDLE hResource, unsigned short ItemID,
		int nDataLength, unsigned char* pData, char* szSPC, unsigned short* Status)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CQDLCtrl* pQDLCtrl = (CQDLCtrl*)hResource;
	return pQDLCtrl->NvRead(ItemID, nDataLength, pData, szSPC, Status);
}
/*
int QDL_DIAG_BRT_Restore(HANDLE hResource, char* pthBrt, char *szSPC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CQDLCtrl* pQDLCtrl = (CQDLCtrl*)hResource;
	return pQDLCtrl->BRTRestore(pthBrt, szSPC);
}

int QDL_DIAG_BRT_Backup(HANDLE hResource, char* pthBrt, char* szSPC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CQDLCtrl* pQDLCtrl = (CQDLCtrl*)hResource;
	return pQDLCtrl->BRTBackup(pthBrt, szSPC);
}
*/
int QDL_Erase_EFS2(HANDLE hResource, char* pthHex, char* pthPartition)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CQDLCtrl* pQDLCtrl = (CQDLCtrl*)hResource;
	return pQDLCtrl->ErasePartitionEFS2(pthHex, pthPartition);
}

