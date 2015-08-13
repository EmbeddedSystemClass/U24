#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\QisdaNVModule\NVItem.h"

/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/


/*CY 20141022 */
QCOM_DIAG_API bool NV_Check_T47_SPU(const char* szPort, char *szNV30002, char *szNV30003, char *szNV30004, char *szErrorCode)
{
	TSTRING		strAddr;

	bool bRes = CNVItem(atoi(szPort)).GetNv_T47(szNV30002, szNV30003, szNV30004);

	if( bRes )
		return true;
	else{
		strcpy(szErrorCode, CommErr_Check_NV_Backup_Fail);
		return false;
	}
	return false;
}

