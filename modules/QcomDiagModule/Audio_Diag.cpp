#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\RequestManager\Request\AudioRequest.h"


/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/

bool CQcomDiagModule::Audio_SpeakerAmpTest()
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	CRequestPtr ptRequest = new CSpeakerAmpRequest(bRes);
	if (! Execute(ptRequest))
		bRes = false;

	if(bRes == false)
		PopLastLog();

	return bRes;
}


bool CQcomDiagModule::Audio_IsHandsetPlugged()
{
    using namespace PLATFORM_TOOL;

    bool bRes = false ;

    CRequestPtr ptRequest = new CIsHandsetPlugged(bRes);

    bRes = Execute(ptRequest) && bRes;
    if (! bRes)
        PopLastLog();

    return bRes;
}


bool CQcomDiagModule::Audio_VibratorOnOff(bool bOn)
{
    using namespace PLATFORM_TOOL;

    bool bRes = false ;
    CRequestPtr ptRequest = new CVibratorOnOff(bRes, bOn);

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

QCOM_DIAG_API bool Audio_SpeakerAmpTest_SPU (const char* szPort)
{
	return CQcomDiagModule(atoi(szPort)).Audio_SpeakerAmpTest();
}


QCOM_DIAG_API bool Audio_IsHandsetPlugged_SPU(const char* szPort)
{
    bool bRes = CQcomDiagModule(atoi(szPort)).Audio_IsHandsetPlugged() ;

    return bRes;
}


QCOM_DIAG_API bool Audio_VibratroOnOff_SPU(const char* szPort, char* szOnOff)
{
    CQcomDiagModule diag(atoi(szPort));

    bool bRes = false;

    if (atoi(szOnOff) == 1)
        bRes = diag.Audio_VibratorOnOff(true);
    else if (atoi(szOnOff) == 0)
        bRes = diag.Audio_VibratorOnOff(false);
    else
        return false;

    return bRes;
}

