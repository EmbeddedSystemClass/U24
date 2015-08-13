#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\RequestManager\Request\BTRequest.h"
#include "..\QisdaNVModule\NVItem.h"


/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/

bool CQcomDiagModule::BT_ModuleSwitch(bool bOn)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	CRequestPtr ptRequest = new CWolf_BTModuleSwitchRequest(bOn, bRes);

	bRes = (Execute(ptRequest) && bRes);

	if (! bRes)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::BT_StartPower (int nDegree)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	if((nDegree == 0x0) || (nDegree == 0x1) || (nDegree == 0x2))
	{
		struct_32byte buffer;
		buffer.Reserved[0] = (BYTE) nDegree;

		CRequestPtr ptRequest = new CLeopard_BT_FM_Request (55, buffer, bRes);
		bRes = Execute (ptRequest) && bRes;
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::BT_PowerOff()
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	bool bStatus = false;
	CRequestPtr ptRequest = new CLeopardBTPowerOffRequest(bStatus);	
	if(Execute(ptRequest))
	{
		bRes = bStatus;
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::BT_ModeRequest()
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	//first parameter
	//true  : send BYTE szEnterTestCmd[] = {0x4B, 0x03, 0x0D, 0x00, 0xDD};
	//false : send BYTE szLeaveTestCmd[] = {0x4B, 0x03, 0x0D, 0x00, 0xDE};
	CRequestPtr ptRequest = new CBTTestModeRequest(true, bRes);	
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}


/*****************************************************************************/
//
//  export for SPU 
//
/*****************************************************************************/

QCOM_DIAG_API bool BT_ModuleOn_SPU(const char* szPort)
{
	CQcomDiagModule diag(atoi(szPort));
	return diag.BT_ModuleSwitch(true);
}

QCOM_DIAG_API bool BT_ModuleOff_SPU(const char* szPort)
{
	CQcomDiagModule diag(atoi(szPort));
	return diag.BT_ModuleSwitch(false);
}

QCOM_DIAG_API bool BT_StartPower_SPU (const char* szPort, char* szDegree)
{
	CQcomDiagModule diag (atoi(szPort));
	return diag.BT_StartPower (::atoi(szDegree));
}


QCOM_DIAG_API bool BT_PowerOff_SPU(const char* szPort)
{
	CQcomDiagModule diag(atoi(szPort));
	return diag.BT_PowerOff();
}

QCOM_DIAG_API bool BT_ModeRequest_SPU(const char* szPort)
{
	CQcomDiagModule diag(atoi(szPort));
	return diag.BT_ModeRequest();
}

QCOM_DIAG_API bool BT_GetAddr_SPU(const char* szPort, char* szAddr)
{
	TSTRING		strAddr;

	strAddr = CNVItem(atoi(szPort)).GetBTAddr();

#ifdef _UNICODE
	wcstombs(szAddr, strAddr.c_str(), 12);
#else
	sprintf(szAddr, "%s", strAddr.c_str());
#endif

	if( strlen(szAddr) == 12 )
		return true;
	else
		return false;

	return false;
}

QCOM_DIAG_API bool BT_WriteAddr_SPU(const char* szPort, char* szAddr)
{
	bool bRes = false;

	bRes = CNVItem(atoi(szPort)).WriteBTaddr(szAddr);

	if(bRes)
		return true;
	else
		return false;

	return false;
}

QCOM_DIAG_API bool BT_GetAddr_C7_SPU(const char* szPort, char* szAddr)
{
	TSTRING		strAddr;

	strAddr = CNVItem(atoi(szPort)).GetBTAddr_C7();

#ifdef _UNICODE
	wcstombs(szAddr, strAddr.c_str(), 12);
#else
	sprintf(szAddr, "%s", strAddr.c_str());
#endif

	if( strlen(szAddr) == 12 )
		return true;
	else
		return false;

	return false;
}
QCOM_DIAG_API bool BT_WriteAddr_C7_SPU(const char* szPort, char* szAddr)
{
	bool bRes = false;

	bRes = CNVItem(atoi(szPort)).WriteBTaddr_C7(szAddr);

	if(bRes)
		return true;
	else
		return false;

	return false;
}
