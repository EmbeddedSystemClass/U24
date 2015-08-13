#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\RequestManager\Request\NFCRequest.h"


/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/

bool CQcomDiagModule::NFC_IsConnected()
{
	using namespace PLATFORM_TOOL;

	bool bRes,bStatus			 = false;
	std::string stdSWver = "000000000000000000000000000000000000000000000000000000000000";

	CRequestPtr ptRequest = new CLeopardNFCIsConnectedRequest(bStatus);
	
	if(Execute(ptRequest))
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


QCOM_DIAG_API bool NFC_IsConnected_SPU(const char* szPort)
{
	CQcomDiagModule		diag (atoi(szPort));
	bool				ret = diag.NFC_IsConnected ();

	return ret;
}