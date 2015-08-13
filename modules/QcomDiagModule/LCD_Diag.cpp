#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\RequestManager\Request\LCDRequest.h"


/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/

bool CQcomDiagModule::Wolf_LCDOnOffSwitch(bool bOn)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	CRequestPtr ptRequest = new CWolfLCDOnOffSwitchRequest(bRes, bOn);

	bRes = Execute(ptRequest) && bRes;

	if (! bRes)
		PopLastLog();

	return bRes;
}


/*****************************************************************************/
//
//  export for SPU 
//
/*****************************************************************************/

QCOM_DIAG_API bool Wolf_LCDOnOffSwitch_SPU(const char* szPort, char* szOnOff)
{
	CQcomDiagModule diag(atoi(szPort));

	bool bRes = false;

	if (atoi(szOnOff) == 1)
		bRes = diag.Wolf_LCDOnOffSwitch(true);
	else if (atoi(szOnOff) == 0)
		bRes = diag.Wolf_LCDOnOffSwitch(false);
	else
		return false;

	return bRes;
}
