#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\RequestManager\Request\BTRequest.h"
#include "..\QisdaNVModule\NVItem.h"



/*****************************************************************************/
//
//  export for SPU 
//
/*****************************************************************************/

QCOM_DIAG_API bool PhoneInfo_GetIMEI_SPU(const char* szPort, char* szAddr)
{
	TSTRING strAddr;
	strAddr = CNVItem(atoi(szPort)).GetIMEI();

#ifdef _UNICODE
	wcstombs(szAddr, strAddr.c_str(), 12);
#else
	sprintf(szAddr, "%s", strAddr.c_str());
#endif

	if(szAddr)
		return true;
	else
		return false;

	return false;
}

QCOM_DIAG_API bool PhoneInfo_WriteIMEI_SPU(const char* szPort, char* szAddr)
{
	bool bRes = false;
	
	bRes = CNVItem(atoi(szPort)).WriteIMEI(szAddr);

	if(bRes)
		return true;
	else
		return false;

	return false;
}