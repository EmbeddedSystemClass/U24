#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\RequestManager\Request\SDRequest.h"


/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/

bool CQcomDiagModule::SD_GetSize (DWORD *dwSize)
{
	using namespace PLATFORM_TOOL;

	bool bRes, bSuccess = false;
	std::string stdSWver = "000000000000000000000000000000000000000000000000000000000000";

	CRequestPtr ptRequest = new CLeopardGetSDcardSizeRequest (*dwSize, bSuccess);
	bRes = Execute (ptRequest);


	if (bRes == false)
	{
		bSuccess = false;
		PopLastLog();
	}

	return bSuccess;
}

bool CQcomDiagModule::SD_GetStatus (DWORD *dwStatus)
{
	using namespace PLATFORM_TOOL;

	bool bRes, bSuccess = false;
	std::string stdSWver = "000000000000000000000000000000000000000000000000000000000000";

	CRequestPtr ptRequest = new CLeopardGetSDcardStatusRequest (*dwStatus, bSuccess);
	bRes = Execute (ptRequest);

	if (bRes == false)
	{
		bSuccess = false;
		PopLastLog();
	}

	return bSuccess;
}

bool CQcomDiagModule::SD_GetSizenStatus_Sys (DWORD *dwSize, DWORD *dwStatus)
{
	using namespace PLATFORM_TOOL;

	bool bRes, bSuccess = false;
	std::string stdSWver = "000000000000000000000000000000000000000000000000000000000000";

	CRequestPtr ptRequest = new CLeopardGetSDcardSizenStatusRequest_Sys (*dwSize,*dwStatus, bSuccess);
	bRes = Execute (ptRequest);


	if (bRes == false)
	{
		bSuccess = false;
		PopLastLog();
	}

	return bSuccess;
}

/*****************************************************************************/
//
//  export for SPU 
//
/*****************************************************************************/


QCOM_DIAG_API bool SD_GetSize_SPU (const char* szPort, char* szSize)
{
	CQcomDiagModule diag (atoi(szPort));
	DWORD dwSize;
	bool ret = diag.SD_GetSize (&dwSize);
	sprintf (szSize, "%u", dwSize);

	return ret;
}



QCOM_DIAG_API bool SD_GetStatus_SPU (const char* szPort, char* szStatus)
{
	CQcomDiagModule diag (atoi(szPort));
	DWORD dwStatus;
	bool ret = diag.SD_GetStatus (&dwStatus);
	sprintf (szStatus, "%u", dwStatus);

	return ret;
}


QCOM_DIAG_API bool SD_GetSizenStatus_Sys_SPU(const char* szPort, char* szSize, char* szStatus)
{
	CQcomDiagModule diag (atoi(szPort));
	DWORD dwSize,dwStatus;
	bool ret = diag.SD_GetSizenStatus_Sys (&dwSize,&dwStatus);
	sprintf (szSize, "%u", dwSize);
	sprintf (szStatus, "%u", dwStatus);
	return ret;
}
