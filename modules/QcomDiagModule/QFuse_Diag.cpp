#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\RequestManager\Request\ODMRequest.h"

/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/




bool CQcomDiagModule::Wolf_ReadQFuse (BYTE* pBuffer, long bufsize)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false, bSuccess;
	BYTE err;

	CRequestPtr ptRequest = new CWolfReadQFuse (bSuccess, err, pBuffer, bufsize);

	bRes = Execute(ptRequest);
	if (!bRes || !bSuccess)
	{
		PopLastLog();
		bRes = false;
	}

	//delete ptRequest;
	return bRes;

}

bool CQcomDiagModule::Wolf_WriteQFuse()
{
	using namespace PLATFORM_TOOL;

	bool bRes = false, bSuccess;
	DWORD readBuf[6];

	// Read
	if (! Wolf_ReadQFuse ((BYTE*)readBuf, 24))
		return false;

	// Write
	readBuf[4] |= 0x02000000; //set bit25 to 1
	CRequestPtr ptRequest = new CWolfWriteQFuse (bSuccess, (BYTE*)readBuf, 24);

	bRes = Execute(ptRequest);
	if (!bRes || !bSuccess)
	{
		PopLastLog();
		bRes = false;
	}

	//delete ptRequest;
	return bRes;
}


bool CQcomDiagModule::Wolf_VerifyQFuse()
{
	DWORD readBuf[6];

	if (Wolf_ReadQFuse ((BYTE*)readBuf, 24))
	{
		if (readBuf[4] & 0x02000000) //???
			return true;
	}

	return false;
}


/*****************************************************************************/
//
//  export for SPU 
//
/*****************************************************************************/

QCOM_DIAG_API bool Wolf_WriteQFuse_SPU (const char* szPort)
{
	CQcomDiagModule diag(atoi(szPort));
	return diag.Wolf_WriteQFuse();
}

QCOM_DIAG_API bool Wolf_VerifyQFuse_SPU (const char* szPort)
{
	CQcomDiagModule diag(atoi(szPort));
	return diag.Wolf_VerifyQFuse ();
}
