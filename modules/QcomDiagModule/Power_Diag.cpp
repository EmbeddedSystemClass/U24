#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\RequestManager\Request\PowerMgrRequest.h"
#include "..\Common\CommonUtil\PIMStringUtility.h"

/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/

bool CQcomDiagModule::PW_ReadBatteryVoltage(int& nVoltage)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	CRequestPtr ptRequest = new CReadBatteryVoltage(&nVoltage, bRes);
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::PW_USBChargeStop()
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CChargingSwitchRequest(6,false,bRes);
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::PW_USBChargeStart()
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CChargingSwitchRequest(6,true,bRes);
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::PW_USBChargingStatus(bool &isCharging)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	CRequestPtr ptRequest = new CChargingStatusRequest(isCharging, bRes);
	bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::PW_GetGaugeDeviceName(std::string &DevName)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	CRequestPtr ptRequest = new CGasGaugeDeviceNameRequest(DevName);
	bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::PW_GetChargeICDeviceID (BYTE& byID)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	CRequestPtr ptRequest = new CGasGaugeICDeviceIDRequest (byID);
	bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::PW_GetCoinCellVoltage(int& iVoltage)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	CRequestPtr ptRequest = new CReadCoincellVoltageRequest(iVoltage, bRes);

	bRes = (Execute(ptRequest) && bRes);

	if (! bRes)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::PW_PowerKeyEvent ()
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	CRequestPtr ptRequest = new CPowerKeyEventRequest ();
	bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

/*****************************************************************************/
//
//  export for SPU 
//
/*****************************************************************************/
bool PW_ReadBatteryVoltage_SPU(char* szPort, char* szVolt)
{
	CQcomDiagModule diag (atoi(szPort));
	int Volt = 0;
	bool ret = diag.PW_ReadBatteryVoltage(Volt);
	sprintf (szVolt, "%d", Volt);

	return ret;
}

bool PW_USBChargeStop_SPU(char* szPort)
{
	CQcomDiagModule diag (atoi(szPort));

	bool ret = diag.PW_USBChargeStop();
	
	return ret;
}

bool PW_USBChargeStart_SPU(char* szPort)
{
	CQcomDiagModule diag (atoi(szPort));

	bool ret = diag.PW_USBChargeStart();
	
	return ret;
}

bool PW_USBChargingStatus_SPU(char *szPort, char* szStatus)
{
	CQcomDiagModule diag (atoi(szPort));
	bool isCharging;
	bool ret = diag.PW_USBChargingStatus(isCharging);
	
	isCharging ? sprintf (szStatus, "%s", "Charging") : sprintf (szStatus, "%s", "Not charging");

	return ret;
}

bool PW_GetGaugeDeviceName_SPU(char *szPort, char *szDevName)
{
	CQcomDiagModule diag (atoi(szPort));
	std::string DevName = "";
	bool ret = diag.PW_GetGaugeDeviceName(DevName);
	
	sprintf(szDevName, "%s", DevName.c_str());
	return ret;
}

QCOM_DIAG_API bool PW_GetChargeICDeviceID_SPU (const char *szPort, char* szBuffer)
{
	BYTE byID;
	bool ret = CQcomDiagModule(atoi(szPort)).PW_GetChargeICDeviceID (byID);
	if (ret)
	{
		CStringA cstrtemp;
		QSYNC_LIB::ByteToChar (&byID, 1, cstrtemp, false);
		strcpy (szBuffer, cstrtemp.GetString());
	}

	return ret;
}

QCOM_DIAG_API bool PW_GetCoinCellVoltage_SPU(const char* szPort, char* szVoltage)
{
	bool bRes = false;
	int iVoltage = 0;

	CQcomDiagModule diag(atoi(szPort));

	bRes = diag.PW_GetCoinCellVoltage(iVoltage);

	if (! bRes)
	{
		sprintf(szVoltage, "0");
		return false;
	}
	else
	{
		sprintf(szVoltage, "%d", iVoltage);
		return true;
	}
}

QCOM_DIAG_API bool PW_PowerKeyEvent_SPU (char* szPort)
{
	return CQcomDiagModule(atoi(szPort)).PW_PowerKeyEvent();
}