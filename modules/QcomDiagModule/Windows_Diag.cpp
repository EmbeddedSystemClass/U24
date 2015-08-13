#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\RequestManager\Request\ODMRequest.h"
#include "..\Common\CommonUtil\ConnAgent.h"
#include "..\Common\CommonUtil\ProcessCtrl.h"


/*****************************************************************************/
//
//  export for SPU 
//
/*****************************************************************************/
QCOM_DIAG_API bool Windows_GetComPort_SPU (char* szPortStr, char* szPortNum, char* szPortAssign)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	CUIntArray auiCommDevAry;
	CStringArray acstrCommDevNameAry;
	CConnAgent clsConnAgent;

	if(strcmp(szPortAssign, "0") != 0)
	{
		bRes = true;
		strcpy(szPortNum, szPortAssign);
		
		return bRes;
	}	
	
	szPortStr = _strupr(szPortStr);

	if(clsConnAgent.GetCommDevAry(CONN_PORT_USE_WDK, auiCommDevAry, &acstrCommDevNameAry))
	{
		for (int i = 0; i < auiCommDevAry.GetSize(); i++)
		{
			CString cstBuff;
			
			cstBuff = acstrCommDevNameAry.GetAt(i);
			cstBuff.MakeUpper();

			if(0 <= cstBuff.Find(szPortStr))
			{
				UINT uiCommDev = auiCommDevAry.GetAt(i);
				sprintf(szPortNum, "%d", uiCommDev);
				bRes = true;
				break;
			}
		}
	}

	if(!bRes)
		strcpy(szPortNum, "none");

	return bRes;
}

QCOM_DIAG_API bool Windows_TerminateProc_ByNmae_SPU (const char* szProcName, char* szErrorCode)
{
	using namespace PLATFORM_TOOL;

	int	iProc = 0;
	bool bRes = false;
	ProcessCtrl pProcCtrl;

	iProc = pProcCtrl.KILL_PROC_BY_NAME(szProcName);
	sprintf(szErrorCode, "%d", iProc);
	
	//	0   = Process was successfully terminated
	//	603 = Process was not currently running
	if((iProc == 0) || (iProc == 603))
		bRes = true;

	return bRes;
}

