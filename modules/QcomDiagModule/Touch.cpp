#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\RequestManager\Request\TouchRequest.h"


/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/

bool CQcomDiagModule::Touch_UpdateTouchFW()
{
	using namespace PLATFORM_TOOL;

	bool bRes = true;
	CRequestPtr ptRequest = new CDiagUpdateTouchFW();
	if (! Execute(ptRequest))
		bRes = false;

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::Touch_CalibrateTouch()
{
	using namespace PLATFORM_TOOL;

	bool bRes, bSuccess = false;
	CRequestPtr ptRequest = new CDiagCalibrateTouch(bSuccess);
	bRes = Execute(ptRequest);

	if (bRes == false)
	{
		bSuccess = false;
		PopLastLog();
	}

	return bSuccess;
}

bool CQcomDiagModule::Touch_GetTouchStream(char *szTouchStream)
{
	using namespace PLATFORM_TOOL;

	bool bRes = true;
	CRequestPtr ptRequest = new CDiagGetTouchStream(szTouchStream);
	if (! Execute(ptRequest))
		bRes = false;

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::Touch_GetTouchDelta(char *szTouchDelta)
{
	using namespace PLATFORM_TOOL;

	bool bRes = true;
	CRequestPtr ptRequest = new CDiagGetTouchDelta(szTouchDelta);
	if (! Execute(ptRequest))
		bRes = false;

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::Touch_GetTouchIDAC(char *szTouchIDAC)
{
	using namespace PLATFORM_TOOL;

	bool bRes = true;
	CRequestPtr ptRequest = new CDiagGetTouchIDAC(szTouchIDAC);
	if (! Execute(ptRequest))
		bRes = false;

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::Touch_GetTouchBaseLine (char *szTouchBaseline)
{
	using namespace PLATFORM_TOOL;

	bool bRes = true;
	CRequestPtr ptRequest = new CDiagGetTouchBaseLine(szTouchBaseline);
	if (! Execute(ptRequest))
		bRes = false;

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::Touch_EnableSensor()
{
	using namespace PLATFORM_TOOL;

	bool bRes, bSuccess = false;
	CRequestPtr ptRequest = new CDiagTouchEnableSensor(bSuccess);
	bRes = Execute(ptRequest);

	if (bRes == false)
	{
		bSuccess = false;
		PopLastLog();
	}

	return bSuccess;
}

bool CQcomDiagModule::Touch_DisableSensor()
{
	using namespace PLATFORM_TOOL;

	bool bRes, bSuccess = false;
	CRequestPtr ptRequest = new CDiagTouchDisableSensor(bSuccess);
	bRes = Execute(ptRequest);

	if (bRes == false)
	{
		bSuccess = false;
		PopLastLog();
	}

	return bSuccess;
}

bool CQcomDiagModule::Touch_EnterTestPage()
{
	using namespace PLATFORM_TOOL;

	bool bRes;
	CRequestPtr ptRequest = new CDiagTouchEnterTestPage();
	bRes = Execute(ptRequest);

	if (bRes == false)
	{
		PopLastLog();
	}

	return bRes;
}

bool CQcomDiagModule::Touch_CheckVersion(char* szTouchVersion)
{
	using namespace PLATFORM_TOOL;

	bool bRes;
	bool bSuccess = false;
	CRequestPtr ptRequest = new CDiagTouchCheckVersion(*szTouchVersion, bSuccess);
	bRes = Execute(ptRequest);

	if (bRes == false)
	{
		PopLastLog();
	}

	return bRes;
}

bool CQcomDiagModule::Touch_ClearStreamBuffer()
{
	using namespace PLATFORM_TOOL;

	bool bRes;
	bool bSuccess = false;
	CRequestPtr ptRequest = new CDiagTouchClearStreamBuffer(bSuccess);
	bRes = Execute(ptRequest);

	if (bRes == false)
	{
		PopLastLog();
	}

	return bRes && bSuccess;
}
/*****************************************************************************/
//
//  export for SPU 
//
/*****************************************************************************/

QCOM_DIAG_API bool Touch_UpdateTouchFW_SPU (const char* szPort)
{
	return CQcomDiagModule(atoi(szPort)).Touch_UpdateTouchFW();
}

QCOM_DIAG_API bool Touch_CalibrateTouch_SPU (const char* szPort)
{	
	return CQcomDiagModule(atoi(szPort)).Touch_CalibrateTouch();
}

QCOM_DIAG_API bool Touch_GetTouchStream_SPU (const char* szPort, char *szTouchStream)
{
	return CQcomDiagModule(atoi(szPort)).Touch_GetTouchStream(szTouchStream);
}

QCOM_DIAG_API bool Touch_GetTouchDelta_SPU (const char* szPort, char *szTouchDelta)
{
	return CQcomDiagModule(atoi(szPort)).Touch_GetTouchDelta(szTouchDelta);
}
QCOM_DIAG_API bool Touch_GetTouchIDAC_SPU (const char* szPort, char *szTouchIDAC)
{
	return CQcomDiagModule(atoi(szPort)).Touch_GetTouchIDAC(szTouchIDAC);
}

QCOM_DIAG_API bool Touch_GetTouchBaseLine_SPU (const char* szPort, char *szTouchBaseLine)
{
	return CQcomDiagModule(atoi(szPort)).Touch_GetTouchBaseLine(szTouchBaseLine);
}

QCOM_DIAG_API bool Touch_EnableSensor_SPU(const char* szPort)
{
	return CQcomDiagModule(atoi(szPort)).Touch_EnableSensor();
}

QCOM_DIAG_API bool Touch_DisableSensor_SPU(const char* szPort)
{
	return CQcomDiagModule(atoi(szPort)).Touch_DisableSensor();
}

QCOM_DIAG_API bool Touch_EnterTestPage_SPU(const char* szPort)
{
	return CQcomDiagModule(atoi(szPort)).Touch_EnterTestPage();
}

QCOM_DIAG_API bool Touch_CheckVersion_SPU(const char* szPort, char* szTouchVersion)
{
	CQcomDiagModule diag (atoi(szPort));
	char dwSize;

	bool ret = diag.Touch_CheckVersion (&dwSize);
	sprintf (szTouchVersion, "%u", (BYTE)dwSize);

	return ret;
}
QCOM_DIAG_API bool Touch_ClearStreamBuffer_SPU(const char* szPort)
{
	CQcomDiagModule diag (atoi(szPort));
	return diag.Touch_ClearStreamBuffer();
}