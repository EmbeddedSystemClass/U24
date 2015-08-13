#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\RequestManager\Request\LEDRequest.h"


/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/

bool CQcomDiagModule::NLED_IsConnected()
{
	using namespace PLATFORM_TOOL;

	bool bRes,bStatus			 = false;

	CRequestPtr ptRequest = new CNLEDRequest(bStatus);
	
	if (Execute(ptRequest))
	{
		bRes = bStatus;
	}

	if (bRes == false)
		PopLastLog();

	return bRes;
}

/*****************************************************************************/
//
//  export for SPU 
//
/*****************************************************************************/


QCOM_DIAG_API bool NLED_IsConnected_SPU (const char* szPort)
{
	return CQcomDiagModule(atoi(szPort)).NLED_IsConnected();
}