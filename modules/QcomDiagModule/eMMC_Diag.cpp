#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\RequestManager\Request\eMMCRequest.h"

/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/

bool CQcomDiagModule::eMMC_GetSize (DWORD *dwSize)
{
	using namespace PLATFORM_TOOL;

	bool bRes, bSuccess = false;
	
	CRequestPtr ptRequest = new CGeteMMCSizeRequest (*dwSize, bSuccess);
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


QCOM_DIAG_API bool eMMC_GetSize_SPU (const char* szPort, char* szSize)
{
	CQcomDiagModule diag (atoi(szPort));
	DWORD dwSize;
	bool ret = diag.eMMC_GetSize (&dwSize);
	sprintf (szSize, "%u", dwSize);

	return ret;
}
