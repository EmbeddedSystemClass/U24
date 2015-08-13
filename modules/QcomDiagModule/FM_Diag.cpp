#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\RequestManager\Request\BTRequest.h"

/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/

bool CQcomDiagModule::FM_ConnectFM (bool bConnect, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	struct_32byte buffer;

	CRequestPtr ptRequest = new CLeopard_BT_FM_Request (bConnect ? 51 : 52 , buffer, bRes);
	bRes = Execute (ptRequest) && bRes;

	if (! bRes)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::FM_TuneFM (DWORD dwFrequency)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	struct_32byte buffer={0};
	*((DWORD*)&buffer.Reserved) = dwFrequency;

	CRequestPtr ptRequest = new CLeopard_BT_FM_Request (53, buffer, bRes);
	bRes = Execute (ptRequest) && bRes;

	if (! bRes)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::FM_GetRSSI (WORD *pRSSI)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	*pRSSI = 0;
	struct_32byte buffer;

	CRequestPtr ptRequest = new CLeopard_BT_FM_Request (54, buffer, bRes);
	bRes = Execute (ptRequest) && bRes;

	*pRSSI = *((WORD*)&buffer.Reserved);

	if (! bRes)
		PopLastLog();

	return bRes;
}


/*****************************************************************************/
//
//  export for SPU 
//
/*****************************************************************************/

QCOM_DIAG_API bool FM_ConnectFM_SPU(const char* szPort, char* szbConnect, char* szTimeout)
{

	CQcomDiagModule diag (atoi(szPort));
	return diag.FM_ConnectFM (TransferCharToBool (szbConnect), ::atoi(szTimeout));
	
}



QCOM_DIAG_API bool FM_TuneFM_SPU(const char* szPort, char* szFMFrequence)
{

	CQcomDiagModule diag (atoi(szPort));
	return diag.FM_TuneFM (::atoi (szFMFrequence));

}


QCOM_DIAG_API bool FM_GetRSSI_SPU(const char* szPort, char* szRSSI)
{
	bool bRes;
	WORD nRSSI = 0;


	CQcomDiagModule diag (atoi(szPort));
	bRes = diag.FM_GetRSSI (&nRSSI);


	if(bRes)
	{
		CStringA strRSSI;
		strRSSI.Format("%d", nRSSI);
		memcpy(szRSSI, strRSSI, strRSSI.GetLength()+1);
	}
	return bRes;
}
