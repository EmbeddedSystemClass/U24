#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\RequestManager\Request\C7Request.h"

/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/
bool CQcomDiagModule::C7_NVBackup()
{
	using namespace PLATFORM_TOOL;

    bool bRes, bSuccess = false;
	CRequestPtr ptRequest = new CC7NVBackupRequest(bSuccess) ;
	bRes = (Execute(ptRequest) && bSuccess) ;

	if (bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::C7_NVRestore()
{
	using namespace PLATFORM_TOOL;

	bool bRes, bSuccess = false;
	CRequestPtr ptRequest = new CC7NVRestoreRequest(bSuccess) ;
	bRes = (Execute(ptRequest) && bSuccess) ;

	if (bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::C7_DRMBackup()
{
	using namespace PLATFORM_TOOL;

    bool bRes, bSuccess = false;
	CRequestPtr ptRequest = new CC7DRMBackupRequest(bSuccess) ;
	bRes = (Execute(ptRequest) && bSuccess) ;

	if (bRes == false)
		PopLastLog();

	return bRes;
}


/*****************************************************************************/
//
//  export for SPU 
//
/*****************************************************************************/

QCOM_DIAG_API bool C7_NVBackup_SPU (char*szPort)
{
	CQcomDiagModule diag(atoi(szPort));
	return diag.C7_NVBackup();
}

QCOM_DIAG_API bool C7_NVRestore_SPU (char*szPort)
{
	CQcomDiagModule diag(atoi(szPort));
	return diag.C7_NVRestore();
}

QCOM_DIAG_API bool C7_DRMBackup_SPU (char*szPort)
{
	CQcomDiagModule diag(atoi(szPort));
	return diag.C7_DRMBackup();
}
