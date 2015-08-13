#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\RequestManager\Request\WP7Request.h"
#include "..\RequestManager\Request\ODMRequest.h"

/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/
bool CQcomDiagModule::WP7_WritePVK()
{
	using namespace PLATFORM_TOOL;

	CRequestPtr ptRequest = new CPVKRequest();
	bool bRes = Execute (ptRequest);

	if (bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::WP7_WritePVK_Wolf(const char* szFileSrc, char* szError)
{
	using namespace PLATFORM_TOOL;

	int			nStatus			= -1;
	int			nIsBegin		= 0;
	int			nIsMore			= 0;
	int			nCheckSum		= 0;
	int			nDataLength		= 0;
	bool		bRes			= false;
	FILE*		pBinFile		= NULL;
	char*		pData			= new char[512];
	CRequestPtr ptRequest ;

	pBinFile = fopen(szFileSrc, "rb");
	
	if( pBinFile == NULL )
		return bRes;

	fseek(pBinFile, 0, SEEK_END);
	nDataLength = ftell(pBinFile);
	rewind(pBinFile);

	if( nDataLength < 512)
	{
		nIsBegin = 1;
		nIsMore = 0;

		BYTE nTmp1;
		fread(pData,sizeof(char), nDataLength,pBinFile);

		//checksum = *(pData) ^ *(pData+1) ^ *(pData+2) ...... ^ *(pData + DataLength - 1)
		for(int i = 1; i < nDataLength; i++)
		{
			if(i == 1)
			{
				nCheckSum = *(pData) ^ *(pData + 1);
			}
			else 
			{
				nTmp1 = *(pData + i);
				nCheckSum = nCheckSum ^ nTmp1;
			}
		}
		
		ptRequest = new CWolfWritePVKRequest(nStatus, nIsBegin, nIsMore, nCheckSum, nDataLength, pData);
	}
	else
	{
		//just fail for EVT2
		//have to be added after EVT2
		return bRes;
	}

	bRes = Execute (ptRequest);//(Execute (ptRequest) && bRes);

	if((nStatus == 0)||(nStatus == 99))
	{
		bRes = true;
		strcpy(szError,"PASS");
	}
	else if(nStatus == 1)
	{
		bRes = false;
		strcpy(szError,"checksum error");
	}
	else if (nStatus == 2)
	{
		bRes = false;
		strcpy(szError,"Write error");
	}
	else
	{
		bRes = false;
		strcpy(szError,"unknown error");
	}

	if (bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::WP7_PVKisWritten_Wolf(char *szStatus)
{
	using namespace PLATFORM_TOOL;

	bool bWrite = false;
	CRequestPtr ptRequest = new CWolfPVKisWriteRequest(bWrite);
	bool bRes = Execute (ptRequest);

	if (bRes == false)
	{
		PopLastLog();
	}

	else
	{
		bWrite ? strcpy(szStatus, "Written") : strcpy(szStatus, "Not Written");
	}

	return bRes;
}

bool CQcomDiagModule::WP7_DelPVK()
{
	using namespace PLATFORM_TOOL;

	CRequestPtr ptRequest = new CDeletePVKRequest();
	bool bRes = Execute (ptRequest);

	if (bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::WP7_ClearPVK_Wolf ()
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CWolfClearPVKRequest(bRes);
	bRes = Execute (ptRequest);

	/* Naomi 20110711
	if (bRes == false)
		PopLastLog();

	return bRes;*/
	return true; 
}

bool CQcomDiagModule::WP7_CheckPVK (int &status)
{
	using namespace PLATFORM_TOOL;

	CRequestPtr ptRequest = new CCheckPVKRequest(status);
	bool bRes = Execute (ptRequest);

	if (bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::WP7_CheckPVK_Wolf (char* szError)
{
	using namespace PLATFORM_TOOL;
	int nStatus = -1;

	CRequestPtr ptRequest = new CWolfCheckPVKRequest(nStatus);
	
	bool bRes = Execute (ptRequest);

	if(nStatus == 0)
	{
		bRes = true;
		strcpy(szError,"PASS");
	}
	else if(nStatus == 2)
	{
		bRes = false;
		strcpy(szError,"ERROR_FILE_NOT_FOUND");
	}
	else if (nStatus == 5)
	{
		bRes = false;
		strcpy(szError,"ERROR_ACCESS_DENIED");
	}
	else if (nStatus == 8)
	{
		bRes = false;
		strcpy(szError,"ERROR_NOT_ENOUGH_MEMORY");
	}
	else if (nStatus == 39)
	{	
		bRes = false;
		strcpy(szError,"ERROR_HANDLE_DISK_FULL");
	}
	else if (nStatus == 110)
	{
		bRes = false;
		strcpy(szError,"ERROR_OPEN_FAILED");
	}
	else if (nStatus == 1127)
	{
		bRes = false;
		strcpy(szError,"ERROR_DISK_OPERATION_FAILED");
	}
	else
	{
		bRes = false;
		strcpy(szError,"UNKNOWN_ERROR");
	}

	if (bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::WP7_BackUpPVK_Wolf (char* szError)
{
	using namespace PLATFORM_TOOL;
	int nStatus = -1;

	CRequestPtr ptRequest = new CWolfBackUpPVKRequest(nStatus);

	bool bRes = Execute (ptRequest);

	if(nStatus == 0)
	{
		bRes = true;
		strcpy(szError,"PASS");
	}
	else if(nStatus == 29)
	{
		bRes = false;
		strcpy(szError,"ERROR_WRITE_FAULT");
	}
	else if (nStatus == 110)
	{
		bRes = false;
		strcpy(szError,"ERROR_OPEN_FAILED");
	}
	else
	{
		bRes = false;
		strcpy(szError,"UNKNOWN_ERROR");
	}

	if (bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::WP7_RestorePVK_Wolf (char* szError)
{
	using namespace PLATFORM_TOOL;
	int nStatus = -1;

	CRequestPtr ptRequest = new CWolfRestorePVKRequest(nStatus);

	bool bRes = Execute (ptRequest);

	if(nStatus == -1)
	{
		bRes = true;
		strcpy(szError,"PASS");
	}
	else if(nStatus == 30)
	{
		bRes = false;
		strcpy(szError,"ERROR_READ_FAULT");
	}
	else if (nStatus == 110)
	{
		bRes = false;
		strcpy(szError,"ERROR_OPEN_FAILED");
	}
	else if (nStatus == 5807)
	{
		bRes = false;
		strcpy(szError,"ERROR_DATABASE_BACKUP_CORRUPT");
	}
	else
	{
		bRes = false;
		strcpy(szError,"UNKNOWN_ERROR");
	}

	if (bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::WP7_MasterClear()
{
	using namespace PLATFORM_TOOL;

	bool bRes;

	CRequestPtr ptRequest = new CLeopardMasterClearRequest(bRes);
	bRes = Execute (ptRequest);

	if (bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::WP7_Reset()
{
	using namespace PLATFORM_TOOL;

	bool bRes;

	CRequestPtr ptRequest = new CWolfResetRequest(bRes);
	bRes = Execute (ptRequest);

	// Because this command has no response
	return true;
}



bool CQcomDiagModule::WP7_ReadHWVersion (int& nVariant, int& nMem, int& nEvt, int& nEvtMinor, int& nPilotRun)
{
	using namespace PLATFORM_TOOL;

	CRequestPtr ptRequest = new CLeopardHWversionRequest (nVariant, nMem, nEvt, nEvtMinor, nPilotRun);
	
	bool bRes = Execute(ptRequest);
	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::WP7_ReadSWVersion (char* SWVersion)
{
	using namespace PLATFORM_TOOL;

	bool bRes;
	std::string stdSWver = "00000000000000000000000000000000000000000000000000000000000000000";

	CRequestPtr ptRequest = new CLeopardSWversionRequest (stdSWver);
	bRes = Execute (ptRequest);

	strcpy (SWVersion, stdSWver.c_str());

	if (bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::WP7_ReadHWVersion_Wolf (int& nVariant, int& nPCBStage, int& nMajorSN, int& nMinorSN, int& nMem, int& neMMC, int& nTouchPanel, int& nModule)
{
	using namespace PLATFORM_TOOL;

	CRequestPtr ptRequest = new CWolfHWversionRequest (nVariant, nPCBStage, nMajorSN, nMinorSN, nMem, neMMC, nTouchPanel, nModule);

	bool bRes = Execute(ptRequest);
	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::WP7_PowerOffMode (int nMode)//0 off 1 reset
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CMSPowerRequest(nMode);
	bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::WP7_CIT_SpecifyFileName(const char* szFileDest)
{
	using namespace PLATFORM_TOOL;

	int			nFileNameLength	= 0;
	int			nStatus			= 0;
	bool		bRes			= false;
	char*		pFileDest		= new char[512];
	CRequestPtr ptRequest ;

	strcpy(pFileDest, szFileDest);
	nFileNameLength = strlen(pFileDest);

	if( pFileDest == NULL )
		return bRes;


	if( nFileNameLength <= 512)
	{
		ptRequest = new CWP7CITSpecifyFileNameRequest(nStatus, nFileNameLength, pFileDest);

		bRes = Execute (ptRequest);

		if(nStatus != 0)
			bRes = false;
	}
	else //length of file name has to be smaller than 512
	{
		bRes = false;
	}


	return bRes;
}

bool CQcomDiagModule::WP7_CIT_Write(const char* szFileSrc, char* szError)
{
	using namespace PLATFORM_TOOL;

	int			nStatus			= 0;
	int			nIsBegin		= 0;
	int			nIsMore			= 0;
	int			nDataLength		= 0;
	bool		bRes			= false;
	FILE*		pCITFile		= NULL;
	char*		pData			= new char[512];
	CRequestPtr ptRequest ;


	char*		szFilePath	= new char [100];

	::GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	::PathRemoveFileSpecA(szFilePath);
	strcat(szFilePath,"\\");
	strcat(szFilePath, szFileSrc);

	pCITFile = fopen(szFilePath, "r");

	if( pCITFile == NULL )
		return bRes;

	fseek(pCITFile, 0, SEEK_END);
	nDataLength = ftell(pCITFile);
	rewind(pCITFile);

	if( nDataLength < 512)
	{
		nIsBegin = 1;
		nIsMore = 0;

		memset(pData, 0, 512);
		fread(pData,sizeof(char), nDataLength, pCITFile);
		ptRequest = new CWP7CITWriteRequest(nStatus, nIsBegin, nIsMore, nDataLength, pData);
	}
	else
	{
		//just fail for EVT2
		//have to be added after EVT2
		return bRes;
	}

	bRes = Execute (ptRequest);

	if(nStatus != 0)
		bRes = false;

	return bRes;
}

bool CQcomDiagModule::WP7_CIT_Read(const char* szFileDest, char* szError)
{
	using namespace PLATFORM_TOOL;

	int			nStatus			= 0;
	int			nIsBegin		= -1;
	int			nIsMore			= -1;
	int			nDataLength		= 0;
	bool		bRes			= false;
	FILE*		pCITFile		= NULL;
	char*		pData			= new char[512];
	CRequestPtr ptRequest ;

	pCITFile = fopen(szFileDest, "w");

	if( pCITFile == NULL )
		return bRes;

	do{
		if(nIsBegin == -1)
			nIsBegin = 1;
		else
			nIsBegin = 0;

		ptRequest = new CWP7CITReadRequest(nStatus, nIsBegin, nIsMore, nDataLength, pData);
		bRes = Execute (ptRequest);

		if(nStatus != 0)
			bRes = false;
		else
			bRes = true;

		if(bRes){
			DWORD nWritten = 0;
			nWritten = fwrite(pData, 1, nDataLength, pCITFile);

			/*if(nIsMore != 0)
				nWritten = fwrite("\n", 1, 1, pCITFile);*/
		}
	}while((bRes) && (nIsMore != 0));

	delete [] pData;
	fclose(pCITFile);
	return bRes;
}

bool CQcomDiagModule::WP7_Setup_Sleep(const char* szSleep)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	int nSleep = atoi(szSleep);

	if( (nSleep<0) || (nSleep>255))
		return bRes;

	CRequestPtr ptRequest = new CSetupSleepRequest(bRes, nSleep);
	bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}
/*****************************************************************************/
//
//  export for SPU 
//
/*****************************************************************************/

QCOM_DIAG_API bool WP7_WritePVK_SPU (const char*szPort)
{
	// First we use WP7_WriteFile to write dpu.bin file to ./
	// then send this PVK diag cmd.
	CQcomDiagModule diag(atoi(szPort));
	return diag.WP7_WritePVK();
}

QCOM_DIAG_API bool WP7_WritePVK_Wolf_SPU (const char* szPort, const char* szFileSrc, char* szError)
{
	// First we use WP7_WriteFile to write dpu.bin file to ./
	// then send this PVK diag cmd.
	CQcomDiagModule diag(atoi(szPort));
	return diag.WP7_WritePVK_Wolf(szFileSrc, szError);
}

QCOM_DIAG_API bool WP7_PVKisWritten_Wolf_SPU (const char* szPort, char *szStatus)
{
	CQcomDiagModule diag(atoi(szPort));
	return diag.WP7_PVKisWritten_Wolf(szStatus);
}

QCOM_DIAG_API bool WP7_DelPVK_SPU (const char*szPort)
{
	CQcomDiagModule diag (atoi(szPort));
	return diag.WP7_DelPVK();
}


QCOM_DIAG_API bool WP7_ClearPVK_Wolf_SPU (const char*szPort)
{
	CQcomDiagModule diag (atoi(szPort));
	return diag.WP7_ClearPVK_Wolf();
}

QCOM_DIAG_API bool WP7_CheckPVK_SPU (const char*szPort, char* checkstatus)
{
	CQcomDiagModule diag (atoi(szPort));

	int status = -1;
	bool ret = diag.WP7_CheckPVK (status);
	sprintf (checkstatus, "%d", status); //0=success, 1=fail
	return ret;
}

QCOM_DIAG_API bool WP7_CheckPVK_Wolf_SPU (const char* szPort, char* szError)
{
	CQcomDiagModule diag (atoi(szPort));

	bool ret = diag.WP7_CheckPVK_Wolf (szError);
	
	return ret;
}

QCOM_DIAG_API bool WP7_BackUpPVK_Wolf_SPU (const char* szPort, char* szError)
{
	CQcomDiagModule diag (atoi(szPort));

	bool ret = diag.WP7_BackUpPVK_Wolf (szError);

	return ret;
}

QCOM_DIAG_API bool WP7_RestorePVK_Wolf_SPU (const char* szPort, char* szError)
{
	CQcomDiagModule diag (atoi(szPort));

	bool ret = diag.WP7_RestorePVK_Wolf (szError);

	return ret;
}

QCOM_DIAG_API bool WP7_ReadHWversion_SPU (const char* szPort, char* szHWVer, char* szVarientVer, char* szMemoryVer, char* szEvtVer, char* szEvtMinorVer, char* szPilotRunVer)
{
	CQcomDiagModule diag (atoi(szPort));

	int nVariant=0, nMem=0, nEvt=0, nEvtMinor=0, nPilotRun=0;
	bool ret = diag.WP7_ReadHWVersion (nVariant, nMem, nEvt, nEvtMinor, nPilotRun);
	if (ret)
	{
		sprintf (szHWVer, "%d.%d.%d.%d.%d", nVariant, nMem, nEvt, nEvtMinor, nPilotRun);
		sprintf (szVarientVer, "%d", nVariant);		// 1=US, 2=EU, 3=AWT
		sprintf (szMemoryVer, "%d", nMem);			// 1=MCP_HYNIX, 2=MCP_TOSHIBA, 3=MCP_SAMSUNG, 255=MCP_UNKNOWN
		sprintf (szEvtVer, "%d", nEvt);				// 0=EVT, 1=DVT, 2=PVT, 3=MP, 255=Unknown
		sprintf (szEvtMinorVer, "%d", nEvtMinor);	// 0=EVB0, 1=EVT1, ...5=EVT5, 255=Unknown
		sprintf (szPilotRunVer, "%d", nPilotRun);	// 1=PilotRun1, 2=PilotRun2, 3=PilotRun3, 255=Unknown
	}

	return ret;
}

QCOM_DIAG_API bool WP7_ReadSWversion_SPU (const char* szPort, char* szVersion)
{
	return CQcomDiagModule(atoi(szPort)).WP7_ReadSWVersion (szVersion);
}

//*************************************************************************************************************************
//
//szVariant:	default = 2;		1 = No Band, 2 = EU variant, 3 = US variant, 4 = AWS variant, 5 = CDMA (800)
//szPCBStage:	default = E;		B = EVB (0), E = EVT (1), D = DVT (2), P = PVT (3), M = MP Version (4)
//szMajorSN:	default = 0;		0-9 = Major HW Stage Serial number
//szMinorSN:	default = 1;		1-9 = Minor HW Stage Serial number
//szMem:		default = 0;		0 = N/A, 1 = Elpida DDR 512MB, 2 = Hynix DDR 512MB
//szeMMC:		default = 0;		0 = N/A, 1 = SanDisk MLC 8G, 2 = SanDisk TLC 8G
//szTouchPanel:	default = 0;		0 = N/A, 1 = J-Touch 
//szModule:		default = 0;		0 = N/A, 1~9 = Vendor
//
//*************************************************************************************************************************
QCOM_DIAG_API bool WP7_ReadHWversion_Wolf_SPU (const char* szPort, char* szHWVer , char* szVariant, char* szPCBStage, char* szMajorSN, char* szMinorSN, char* szMem, char* szeMMC, char* szTouchPanel, char* szModule)
{
	CQcomDiagModule diag (atoi(szPort));

	int nVariant = 0, nPCBStage = 0, nMajorSN = 0, nMinorSN = 0, nMem = 0, neMMC = 0, nTouchPanel = 0, nModule = 0;
	bool ret = diag.WP7_ReadHWVersion_Wolf (nVariant, nPCBStage, nMajorSN, nMinorSN, nMem, neMMC, nTouchPanel, nModule);
	if (ret)
	{
		sprintf (szHWVer, "%c%c%c%c%c%c%c%c", nVariant, nPCBStage, nMajorSN, nMinorSN, nMem, neMMC, nTouchPanel, nModule);
		sprintf (szVariant, "%c", nVariant);
		sprintf (szPCBStage, "%c", nPCBStage);
		sprintf (szMajorSN, "%c", nMajorSN);
		sprintf (szMinorSN, "%c", nMinorSN);
		sprintf (szMem, "%c", nMem);
		sprintf (szeMMC, "%c", neMMC);
		sprintf (szTouchPanel, "%c", nTouchPanel);
		sprintf (szModule, "%c", nModule);
	}

	return ret;
}

QCOM_DIAG_API bool WP7_MasterReset_SPU (const char* szPort)
{
	CQcomDiagModule diag (atoi(szPort));

	bool bRet = diag.WP7_MasterClear() ;
	if (bRet)
	{
		bRet = diag.WP7_Reset() ;
	}

	return bRet ;
}

QCOM_DIAG_API bool WP7_PowerOffMode_SPU (const char* szPort, char* sznPowerOffMode)
{
	CQcomDiagModule diag (atoi(szPort));

	return diag.WP7_PowerOffMode(::atoi(sznPowerOffMode));
}

QCOM_DIAG_API bool WP7_CIT_SpecifyFileName_SPU (const char* szPort, const char* szFileDest)
{
	//before write or read CIT file, it's necessary to specify file name first

	CQcomDiagModule diag(atoi(szPort));
	return diag.WP7_CIT_SpecifyFileName(szFileDest);
}

QCOM_DIAG_API bool WP7_CIT_Write_SPU (const char* szPort, const char* szFileSrc, char* szError)
{
	//Write cit_input.txt or cit_output.txt to Mobile

	CQcomDiagModule diag(atoi(szPort));
	return diag.WP7_CIT_Write(szFileSrc, szError);
}

QCOM_DIAG_API bool WP7_CIT_Read_SPU (const char* szPort, const char* szFileDest, char* szError)
{
	//Read cit_input.txt or cit_output.txt to Mobile

	CQcomDiagModule diag(atoi(szPort));
	return diag.WP7_CIT_Read(szFileDest, szError);
}
QCOM_DIAG_API bool WP7_Setup_Sleep_SPU (const char* szPort, char* szTime)
{
	CQcomDiagModule diag(atoi(szPort));
	return diag.WP7_Setup_Sleep(szTime);
}
