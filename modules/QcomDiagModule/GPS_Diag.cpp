#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\RequestManager\Request\GPSRequest.h"



/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/

// Use DIAG
bool CQcomDiagModule::GPS_GetIQValue (WORD *pIValue, WORD *pQValue)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	bool bSuccess = false;

	CRequestPtr ptRequest = new CLeopardReadIQValueRequest (bSuccess, *pIValue, *pQValue);
	bRes = Execute(ptRequest) && bSuccess;

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::GPS_GetCNOvalue (WORD *pCNO)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CLeopardGetCNOvalueRequest (*pCNO, bRes);
	bRes = Execute (ptRequest) && bRes;

	if (!bRes)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::GPS_SVTrack(BYTE cmd, BYTE SVnumber)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CGPS_SV_TRACKRequest  (SVnumber, cmd, bRes);
	bRes = Execute (ptRequest) && bRes;

	if (!bRes)
		PopLastLog();

	return bRes;
}

// Use Log parsing (CNO parse log packet 0x1371)
// call sequence:
// 1. GPS_EnableLog (true)
// 2. GPS_SA_RF_VerificationModeSwitch (true,...)
// 3. GPSSVTrack (2, SVnum, Threshold)
// 4. GPSSVTrack (4, SVnum, Threshold)
// 5. GPS_SA_RF_VerificationModeSwitch (false)
// 6. GPS_EnableLog (true)

// Use Log parsing (IQ parse log packet 0x1372)
// call sequence:
// 1. GPS_EnableLog (true)
// 2. GPS_SA_RF_VerificationModeSwitch (true,...)
// 3. GPS_SA_RF_VerificationModeSwitch (true,...)
// 4. GPS_EnableLog (false)


bool CQcomDiagModule::GPS_EnableLog (bool bEnable, WORD wPacketNo)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CEnableGPSLog (bEnable, wPacketNo, bRes);
	bRes = Execute (ptRequest) && bRes;

	if (!bRes)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::GPS_EnterSA_RF_VerifMode (bool bEnable, WORD wPacketNo, const int UpBound, const int LoBound, const int packetCount, 
												int* pIValue, int* pQValue)
{	
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	

	BYTE byMode = bEnable ? 0 : 1;
	int status; // status: 0=IO error, 1=success, 2=value error.

	packet_0x1372_structure GPS_IQ = {0};
	GPS_IQ.Upbound = UpBound;
	GPS_IQ.Lobound = LoBound;
	GPS_IQ.packetCount = packetCount;

	
	CRequestPtr ptRequest = new CGPSEnterStandaloneRFRequest (
																byMode, 
																wPacketNo, 
																bRes, 
																status, 
																(void*)&GPS_IQ, 
																sizeof(packet_0x1372_structure));

	bRes = Execute (ptRequest) && bRes;

	if (bRes)
		bRes = 1 == status;

	if (!bRes)
		PopLastLog();

	if (pIValue)
		*pIValue = GPS_IQ.Ivalue;
	if (pQValue)
		*pQValue = GPS_IQ.Qvalue;


	return bRes;
}

bool CQcomDiagModule::GPS_SVTrack_byLog (BYTE cmd, BYTE SVnumber, WORD Threshold, WORD* pCNOvalue, bool bWaitLog)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	bool bSuccess;

	CRequestPtr ptRequest = new CGPSSVTrack_LogRequest (SVnumber, cmd, bSuccess, Threshold, *pCNOvalue, bWaitLog);
	bRes = Execute (ptRequest);

	if (bRes)
		bRes = bSuccess;
	if (!bRes)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::GPS_TrackingSwitch(bool bOn)
{
	using namespace PLATFORM_TOOL;

	bool bSuccess;

	CRequestPtr ptRequest = new CGPSTrackingSwitchRequest(bSuccess, bOn);
	bSuccess = (Execute(ptRequest) && bSuccess);

	if (! bSuccess)
		PopLastLog();

	return bSuccess;
}

bool CQcomDiagModule::GPS_PollingTTFFCN0(int& iTTFF, int& iCN0)
{
	using namespace PLATFORM_TOOL;

	bool bSuccess;

	CRequestPtr ptRequest = new CGPSPollingTTFFCN0Request(bSuccess, iTTFF, iCN0);
	bSuccess = (Execute(ptRequest) && bSuccess);

	if (! bSuccess)
		PopLastLog();

	return bSuccess;
}

bool CQcomDiagModule::GPS_EnterGPS(bool bEnter)
{
	using namespace PLATFORM_TOOL;

	bool bSuccess;

	CRequestPtr ptRequest = new CGPS_SA_RF_VERIF_MODE_SWITCHRequest (bEnter, bSuccess);
	bSuccess = Execute(ptRequest) && bSuccess;

	if (! bSuccess)
		PopLastLog();

	return bSuccess;
}

bool CQcomDiagModule::GPS_ClearData(int nCommand)
{
	using namespace PLATFORM_TOOL;

	bool bSuccess;

	CRequestPtr ptRequest = new CGPS_RF_Dev_Multichannel_TrackingRequest (bSuccess, nCommand);
	bSuccess = Execute(ptRequest) && bSuccess;

	if (! bSuccess)
		PopLastLog();

	return bSuccess;
}


/*****************************************************************************/
//
//  export for SPU 
//
/*****************************************************************************/

// call sequence for AP
// by DIAG cmd
// 1. GPS_SVTrack_SPU (com, 2, ...)
// 2. GPS_GetCNOvalue_SPU or GPS_GetIQValue_SPU
// 3. GPS_SVTrack_SPU (com, 4, ...)


// by Log parsing
// CNO
// 2. GPS_EnableLog_SPU
// 3. GPS_EnterSA_RF_VerifMode_SPU (true, ...)
// 5. GPS_EnterSA_RF_VerifMode (false,...)
// 6. GPS_EnableLog (false
// 






QCOM_DIAG_API bool GPS_GetIQValue_SPU (const char* szPort, char* Ivalue, char* Qvalue)
{
	WORD wIVal, wQVal;

	CQcomDiagModule diag(atoi(szPort));
	bool ret = diag.GPS_GetIQValue (&wIVal, &wQVal);

	if (Ivalue)
		::sprintf (Ivalue, "%d", wIVal);
	if (Qvalue)
		::sprintf (Qvalue, "%d", wQVal);
	return ret;
}

QCOM_DIAG_API bool GPS_GetCNOvalue_SPU (const char* szPort, char* szCNOvalue)
{
	WORD wCNO;

	CQcomDiagModule diag (atoi(szPort));
	bool ret = diag.GPS_GetCNOvalue (&wCNO);

	if (szCNOvalue)
		::sprintf (szCNOvalue, "%d", wCNO);
	return ret;
}

QCOM_DIAG_API bool GPS_SVTrack_SPU (const char*szPort, char* szCmd, char* szSVnumber)
{
	CQcomDiagModule diag (atoi(szPort));
	return diag.GPS_SVTrack(atoi (szCmd), atoi(szSVnumber));
}

QCOM_DIAG_API bool GPS_EnableLog_SPU (const char* szPort, const char* szbEnable, const char* packetNo)
{
	CQcomDiagModule diag (atoi(szPort));
	return diag.GPS_EnableLog (TransferCharToBool (szbEnable), atoi(packetNo));
}

QCOM_DIAG_API bool GPS_EnterSA_RF_VerifMode_SPU (const char* szPort, const char* szbEnable, const char* packetNo, const char* szUpbound, const char* szLoBound, const char* szPacketCount, 
												 char* szIValue, char* szQValue)
{
	int IValue = 0;
	int QValue = 0;

	int up=0, lo=0, count=0;
	if (szUpbound)
		up = atoi(szUpbound);
	if (szLoBound)
		lo = atoi(szLoBound);
	if (szPacketCount)
		count = atoi (szPacketCount);


	CQcomDiagModule diag (atoi(szPort));
	bool ret = diag.GPS_EnterSA_RF_VerifMode (TransferCharToBool(szbEnable), atoi(packetNo), up, lo, count, &IValue, &QValue);


	if (szIValue)
		sprintf (szIValue, "%d", IValue);
	if (szQValue)
		sprintf (szQValue, "%d", QValue);

	return ret;
}

QCOM_DIAG_API bool GPS_SVTrack_byLog_SPU (const char*szPort, char* szCmd, char* szSVnumber, const char* szThreshold, char* szCNOvalue)
{
	WORD threshold = atoi (szThreshold);
	BYTE SVnumber = atoi (szSVnumber);
	BYTE cmd = atoi (szCmd);
	WORD CNOvalue;

	CQcomDiagModule diag (atoi(szPort));
	bool ret = diag.GPS_SVTrack_byLog (cmd, SVnumber, threshold, &CNOvalue);
	sprintf (szCNOvalue, "%d", CNOvalue);

	return ret;
}

QCOM_DIAG_API bool GPS_TrackingSwitch_SPU(const char* szPort, const char* szOnOffSwitch)
{
	CQcomDiagModule diag(atoi(szPort));

	bool bSwitch;
	int iSwitch = atoi(szOnOffSwitch);

	if (iSwitch == 0)
		bSwitch = false;
	else if (iSwitch == 1)
		bSwitch = true;
	else
		return false;

	return diag.GPS_TrackingSwitch(bSwitch);
}

QCOM_DIAG_API bool GPS_PollingTTFFCN0_SPU(const char* szPort, char* TTFF, char* CN0)
{
	CQcomDiagModule diag(atoi(szPort));

	int iTTFF, iCN0;

	bool bRes = diag.GPS_PollingTTFFCN0(iTTFF, iCN0);

	sprintf(TTFF, "%d", iTTFF);
	sprintf(CN0, "%d", iCN0);

	return bRes;
}

QCOM_DIAG_API bool GPS_EnterGPS_SPU (const char* szPort, const char* szEnter)
{
	CQcomDiagModule diag(atoi(szPort));
	return diag.GPS_EnterGPS (TransferCharToBool(szEnter));
}

QCOM_DIAG_API bool GPS_ClearData_SPU (const char* szPort, const char* szClearCommand)
{
	CQcomDiagModule diag(atoi(szPort));

	// szClearCommand
	// 0 - clear start
	// 2 - stop and clear
	
	int nCommand = atoi(szClearCommand);
	return diag.GPS_ClearData(nCommand);
}

