#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\RequestManager\Request\ODMRequest.h"


/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/

bool CQcomDiagModule::Temp_UpdateTouchFW()
{
	using namespace PLATFORM_TOOL;

	bool bRes = true;
	CRequestPtr ptRequest = new CDiagUpdateTouchFW();
	if (! Execute(ptRequest))
		bRes = false;

	if(bRes == false)
		PopLastLog();

	return bRes;
}


/*****************************************************************************/
//
//  export for SPU 
//
/*****************************************************************************/

QCOM_DIAG_API bool Temp_UpdateTouchFW_SPU (const char* szPort)
{
	return CQcomDiagModule(atoi(szPort)).Temp_UpdateTouchFW();
}

