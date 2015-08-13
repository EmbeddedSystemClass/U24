#include "stdafx.h"
#include "QcomDiagModule.h"
//#include "..\RequestManager\Request\BTRequest.h"
#include "..\QisdaNVModule\NVItem.h"

/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/


/*Lion20120703 */
QCOM_DIAG_API bool NV_Detroit_SPU(const char* szPort, char *szNV30015, char *szNV30016, char *szNV30017, char *szErrorCode)
{
	TSTRING		strAddr;

	bool bRes = CNVItem(atoi(szPort)).GetNv_Detroit(szNV30015, szNV30016, szNV30017);

	if( bRes )
		return true;
	else{
		strcpy(szErrorCode, CommErr_Check_NV_Backup_Fail);
		return false;
	}
	return false;
}

