#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\RequestManager\Request\WIFIRequest.h"
#include "..\QisdaNVModule\NVItem.h"


/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/

bool CQcomDiagModule::WiFi_ModuleSwitch(bool bOn)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	CRequestPtr ptRequest = new CWolf_WiFiModuleSwitchRequest(bOn, bRes);
	bRes = (Execute(ptRequest) && bRes);

	if (! bRes)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::WiFi_ModuleCheck (bool& bOn)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	CRequestPtr ptRequest = new CWolf_WiFiModuleCheckRequest(bOn, bRes);
	bRes = (Execute(ptRequest) && bRes);

	if (! bRes)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::WiFi_ChangeIfToNative()
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CLeopardChangeWifiFWtoNativeRequest (bRes);
	bRes = Execute (ptRequest) && bRes;

	if (! bRes)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::WiFi_TurnOnTXPower (BYTE nChannel)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CLeopardTurnOnTXPowerRequest (nChannel, bRes);
	bRes = Execute (ptRequest) && bRes;

	if (! bRes)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::WiFi_TurnOnTXPower2 (BYTE Rate, BYTE nChannel, BYTE Power)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CLeopardTurnOnTXPower2Request (Rate, nChannel, Power, bRes);
	bRes = Execute (ptRequest) && bRes;

	if (! bRes)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::WiFi_TurnOnTXPower2ChN (BYTE Rate, BYTE nChannel, BYTE Power)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CLeopardTurnOnTXPower2ChNRequest (Rate, nChannel, Power, bRes);
	bRes = Execute (ptRequest) && bRes;

	if (! bRes)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::WiFi_TurnOnRXPower2 (BYTE Rate, BYTE nChannel, int& Data1, int& Data2, int& Data3)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CLeopardTurnOnRXPower2Request (Rate, nChannel, Data1, Data2, Data3);
	bRes = Execute (ptRequest);

	if (! bRes)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::WiFi_TurnOnRXPower2ChN (BYTE Rate, BYTE nChannel, int& Data1, int& Data2, int& Data3)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CLeopardTurnOnRXPower2ChNRequest (Rate, nChannel, Data1, Data2, Data3);
	bRes = Execute (ptRequest);

	if (! bRes)
		PopLastLog();

	return bRes;
}



bool CQcomDiagModule::Wifi_StartTx (int rate, int channel, int power)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CLeopardWifiTestRequest (rate, channel, power, bRes);
	bRes = Execute (ptRequest) && bRes;

	if (! bRes)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::Wifi_ReadMacAddress(char* szAddress)
{
	using namespace PLATFORM_TOOL;
	bool bResult, bRes = false;

	CRequestPtr ptRequest = new CWolfReadMACRequest (szAddress, bResult);

	if (Execute (ptRequest))
	{
		bRes = bResult;
	}

	if (! bRes)
		PopLastLog();

	return bRes;
}


/*****************************************************************************/
//
//  export for SPU 
//
/*****************************************************************************/

QCOM_DIAG_API bool WiFi_ModuleSwitchOn_SPU(const char* szPort)
{
	CQcomDiagModule diag(atoi(szPort));
	return diag.WiFi_ModuleSwitch(true);
}

QCOM_DIAG_API bool WiFi_ModuleSwitchOff_SPU(const char* szPort)
{
	CQcomDiagModule diag(atoi(szPort));
	return diag.WiFi_ModuleSwitch(false);
}

QCOM_DIAG_API bool WiFi_ModuleCheck_SPU(const char* szPort, char* szOn)
{
	bool ret, bOn;

	CQcomDiagModule diag(atoi(szPort));
	ret = diag.WiFi_ModuleCheck (bOn);
	if (ret)
		strcpy (szOn, bOn ? "ON" : "OFF");
	else
		strcpy (szOn, "OFF");

	return ret;
}

QCOM_DIAG_API bool WiFi_ChangeIfToNative_SPU (const char* szPort)
{
	CQcomDiagModule diag (atoi(szPort));
	return diag.WiFi_ChangeIfToNative ();
}

QCOM_DIAG_API bool WiFi_TurnOnTXPower_SPU (const char* szPort, char* szChannel)
{
	CQcomDiagModule diag (atoi(szPort));
	return diag.WiFi_TurnOnTXPower (::atoi(szChannel));
}

QCOM_DIAG_API bool WiFi_TurnOnTXPower2_SPU (const char* szPort, char* szRate, char* szChannel, char* szPower)
{
	CQcomDiagModule diag (atoi(szPort));
	return diag.WiFi_TurnOnTXPower2 (atoi(szRate), ::atoi(szChannel), atoi(szPower));
}

QCOM_DIAG_API bool WiFi_TurnOnTXPower2ChN_SPU (const char* szPort, char* szRate, char* szChannel, char* szPower)
{
	CQcomDiagModule diag (atoi(szPort));
	return diag.WiFi_TurnOnTXPower2ChN (atoi(szRate), ::atoi(szChannel), atoi(szPower));
}

QCOM_DIAG_API bool WiFi_TurnOnRXPower2_SPU (const char* szPort, char* szRate, char* szChannel, char* szOutData1, char* szOutData2, char* szOutData3)
{
	int data1, data2, data3;

	CQcomDiagModule diag (atoi(szPort));
	bool ret = diag.WiFi_TurnOnRXPower2 (atoi(szRate), ::atoi(szChannel), data1, data2, data3);

	sprintf (szOutData1, "%d", data1);
	sprintf (szOutData2, "%d", data2);
	sprintf (szOutData3, "%d", data3);
	return ret;
}

QCOM_DIAG_API bool WiFi_TurnOnRXPower2ChN_SPU (const char* szPort, char* szRate, char* szChannel, char* szOutData1, char* szOutData2, char* szOutData3)
{
	int data1, data2, data3;
	CQcomDiagModule diag (atoi(szPort));

	bool ret = diag.WiFi_TurnOnRXPower2ChN (atoi(szRate), ::atoi(szChannel), data1, data2, data3);
	sprintf (szOutData1, "%d", data1);
	sprintf (szOutData2, "%d", data2);
	sprintf (szOutData3, "%d", data3);
	return ret;
}


QCOM_DIAG_API bool Wifi_StartTx_SPU (const char* szPort, char* szRate, char* szChannel, char* szPower)
{
	CQcomDiagModule diag (atoi(szPort));
	return diag.Wifi_StartTx (atoi(szRate), ::atoi(szChannel), atoi(szPower));
}

QCOM_DIAG_API bool Wifi_GetAddrFromNV_SPU(const char* szPort, char* szAddr)
{
	TSTRING strAddr;
	strAddr = CNVItem(atoi(szPort)).GetWLANAddr();

#ifdef _UNICODE
	wcstombs(szAddr, strAddr.c_str(), 12);
#else
	sprintf(szAddr, "%s", strAddr.c_str());
#endif
	
	return ( strlen(szAddr) == 12 );
}

QCOM_DIAG_API bool Wifi_WriteAddrFromNV_SPU(const char* szPort, char* szAddr)
{
	return CNVItem(atoi(szPort)).WriteWLANaddr(szAddr);
}

QCOM_DIAG_API bool Wifi_GetAddrFromNV_C7_SPU(const char* szPort, char* szAddr)
{
	TSTRING strAddr;
	strAddr = CNVItem(atoi(szPort)).GetWLANAddr_C7();

#ifdef _UNICODE
	wcstombs(szAddr, strAddr.c_str(), 12);
#else
	sprintf(szAddr, "%s", strAddr.c_str());
#endif
	
	return ( strlen(szAddr) == 12 );
}

QCOM_DIAG_API bool Wifi_WriteAddrFromNV_C7_SPU(const char* szPort, char* szAddr)
{
	return CNVItem(atoi(szPort)).WriteWLANaddr_C7(szAddr);
}

QCOM_DIAG_API bool Wifi_ReadMacAddress_SPU (const char* szPort, char* szAddr)
{
	return CQcomDiagModule(atoi(szPort)).Wifi_ReadMacAddress(szAddr);
}