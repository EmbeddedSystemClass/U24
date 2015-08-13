#include "stdafx.h"
#include "RFErrorCodeGenerator.h"


#pragma warning (push)
#pragma warning (disable : 4996)


bool GenRFErrorCode(std::string strTech, int iBand, std::string strChannelRange, std::string strMode, std::string strItem, std::string strBasicErrCode, std::string& strErrorCode, std::string& strErrMsg)
{
    bool bIsErrorCodeValid = true ;    // To store is the new error code valid ?

    strErrorCode.clear() ;

    CString cstrBasicErrCode = strBasicErrCode.c_str() ;
    cstrBasicErrCode.MakeLower() ;

    strErrMsg.clear() ;

    if ( "device_" == cstrBasicErrCode.Left(7) )
    {
        // Rule: DVIC + Technology(1) + C/T(1) + Item Number(2) + _ + Error Code

        if (! RFErrorCode_CheckTech(strTech) ) {
            //strErrMsg += "[RF_ErrorCode] This Technology is not defined, probably wrong. \n" ;
            //strTech = strTech.substr(0, 1) ;
            strTech = "?" ;
            bIsErrorCodeValid = false ;
        }

        if(! RFErrorCode_CheckModeAndItem(strTech, strMode, strItem, true) ) {
            //strErrMsg += "[RF_ErrorCode] Test Item and Cal/Test Mode is not matched. \n" ;
            strMode = "?" ;
            bIsErrorCodeValid = false ;
        }

        // Generate Error Code
        strErrorCode = "DVIC" + strTech + strMode + strItem + "_" + strBasicErrCode ;
    }
    else if ( "qlib_" ==  cstrBasicErrCode.Left(5) )
    {
        // Generate Error Code
        strErrorCode = strBasicErrCode ;
    }
    else if ( "gsm" == cstrBasicErrCode.Left(3) || "edge" == cstrBasicErrCode.Left(4) ||
        "cdma" == cstrBasicErrCode.Left(4) || "wcdma" == cstrBasicErrCode.Left(5) || "lte" == cstrBasicErrCode.Left(3) )
    {
        // Rule: Technology(1) + Band(2) + Channel Range(1) + PowerLevel(1) + Item Number(2) + _ + Error Code

        if (! RFErrorCode_CheckTech(strTech) ) {
            //strErrMsg += "[RF_ErrorCode] This Technology is not defined, probably wrong. \n" ;
            //strTech = strTech.substr(0, 1) ;
            strTech = "?" ;
            bIsErrorCodeValid = false ;
        }

        std::string strBand ;
        if (! RFErrorCode_GetBand(strTech, iBand, strBand) ) {
            //strErrMsg += "[RF_ErrorCode] This Band is not defined, probably wrong. \n" ;
            //strBand = strBand.substr(0, 2) ;
            strBand = "??" ;
            bIsErrorCodeValid = false ;
        }

        std::string strRange = strChannelRange ;
        if (! RFErrorCode_GheckChannelRange(strRange) ) {
            //strErrMsg += "[RF_ErrorCode] This channel range is not defined, use \"M\" as default. \n" ;
            strRange = "?" ;
            bIsErrorCodeValid = false ;
        }

        if(! RFErrorCode_CheckModeAndItem(strTech, strMode, strItem, false) ) {
            //strErrMsg += "[RF_ErrorCode] Test Item and Cal/Test Mode is not matched. \n" ;
            strItem = "??" ;
            bIsErrorCodeValid = false ;
        }

        // Generate Error Code
        strErrorCode = strTech + strBand + strRange + strMode + strItem + "_" + strBasicErrCode ;
    }
    else
    {
        // Common Error Code
        strErrorCode = strBasicErrCode ;
    }

    return bIsErrorCodeValid ;
}


bool RFErrorCode_CheckTech(std::string strTech)
{
    if ( _RFECDef_Tech_GSM == strTech || _RFECDef_Tech_EDGE == strTech || _RFECDef_Tech_CDMA == strTech
        || _RFECDef_Tech_WCDMA == strTech || _RFECDef_Tech_DCHSDPA == strTech || _RFECDef_Tech_1xEVDO == strTech
        || _RFECDef_Tech_LTE == strTech )
        return true ;

    return false ;
}


bool RFErrorCode_GetBand(std::string strTech, int iBand, std::string &strBand)
{
    if ( _RFECDef_Tech_GSM == strTech || _RFECDef_Tech_EDGE == strTech )
    {
        switch (iBand)
        {
        case 1:
        case 850:
            strBand = "01" ;
            return true ;
        case 2:
        case 900:
            strBand = "02" ;
            return true ;
        case 3:
        case 1800:
            strBand = "03" ;
            return true ;
        case 4:
        case 1900:
            strBand = "04" ;
            return true ;
        default :
            break ;
        }
    }
    else if ( _RFECDef_Tech_CDMA == strTech || _RFECDef_Tech_1xEVDO == strTech)
    {
        switch (iBand)
        {
        case 0:
        case 800:
            strBand = "00" ;
            return true ;
        case 1:
        case 1900:
            strBand = "01" ;
            return true ;
        default:
            break ;
        }
    }
    else if ( _RFECDef_Tech_WCDMA == strTech || _RFECDef_Tech_DCHSDPA == strTech )
    {
        switch (iBand)
        {
        case 1:
        case 2100:
            strBand = "01" ;
            return true ;
        case 2:
        case 1900:
            strBand = "02" ;
            return true ;
        case 4:
        case 1700:
            strBand = "04" ;
            return true ;
        case 5:
        case 850:
            strBand = "05" ;
            return true ;
        case 8:
        case 900:
            strBand = "08" ;
            return true ;
        default:
            break ;
        }
    }
    else if ( _RFECDef_Tech_LTE == strTech )
    {
        switch (iBand)
        {
        case 1:
        case 2100:
            strBand = "01" ;
            return true ;
        case 2:
        case 1900:
            strBand = "02" ;
            return true ;
        case 3:
        case 1800:
            strBand = "03" ;
            return true ;
        case 4:
        case 1700:
            strBand = "04" ;
            return true ;
        case 5:
        case 850:
            strBand = "05" ;
            return true ;
        case 6:
        case 800:
            strBand = "06" ;
            return true ;
        case 7:
        case 2600:
            strBand = "07" ;
            return true ;
        case 8:
        case 900:
            strBand = "08" ;
            return true ;
        case 20:
            strBand = "20" ;
            return true ;
        case 28:
		case 281:
		case 282:
            strBand = "28" ;
            return true ;
        default :
            break ;
        }
    }

    // Format
    char szTmp[256] ;
    ::ZeroMemory(szTmp, 256) ;
    sprintf(szTmp, "%02d", iBand) ;
    strBand = szTmp ;

    return false ;
}

bool RFErrorCode_GheckChannelRange(std::string& strChannelRange)
{
    if ( (0 == stricmp(strChannelRange.c_str(), "LOW")) || (0 == stricmp(strChannelRange.c_str(), "L")) )
    {
        strChannelRange = "L" ;
        return true ;
    }
    else if ( (0 == stricmp(strChannelRange.c_str(), "MID")) || (0 == stricmp(strChannelRange.c_str(), "M")) )
    {
        strChannelRange = "M" ;
        return true ;
    }
    else if ( (0 == stricmp(strChannelRange.c_str(), "HIGH")) || (0 == stricmp(strChannelRange.c_str(), "H")) )
    {
        strChannelRange = "H" ;
        return true ;
    }

    return false ;
}

bool RFErrorCode_CheckModeAndItem(std::string strTech, std::string& strMode, std::string& strItem, bool bIsDeviceMode)
{
    bool bIsValid = true ;

    if ( strTech != strItem.substr(0, 1) ) {
        bIsValid = false ;
    }

    if ( "K" == strItem.substr(1, 1) )
    {
        if ( _RFECDef_Mode_Cal != strMode )
            bIsValid = false ;
    }
    else if ( "T" == strItem.substr(1, 1) )
    {
        if (! (_RFECDef_Mode_TestNoPower == strMode || _RFECDef_Mode_TestInHPower == strMode || _RFECDef_Mode_TestInLPower == strMode) )
            bIsValid = false ;
    }

    strItem = strItem.substr(2, 2) ;

    // DVIC_ only "T" and "C" mode.
    if (bIsDeviceMode)
    {
        if ( _RFECDef_Mode_TestNoPower == strMode || _RFECDef_Mode_TestInHPower == strMode || _RFECDef_Mode_TestInLPower == strMode )
        {
            strMode = "T" ;
        }
        else if ( _RFECDef_Mode_Cal == strMode )
        {
            strMode = "C" ;
        }
        else
        {
            bIsValid = false ;
        }
    }
    else
    {
        // GSM only L/H power mode, use H power mode as default.
        if ( "G" == strTech.substr(1, 1) && _RFECDef_Mode_Cal != strMode )
        {
            if ( _RFECDef_Mode_TestNoPower == strMode )
            {
                strMode = _RFECDef_Mode_TestInHPower ;
            }
        }
        else if ( "W" == strTech.substr(1, 1) && _RFECDef_Mode_Cal != strMode )
        {
            if ( _RFECDef_Item_WCDMATest_RxLev == strItem )
                strMode = _RFECDef_Mode_TestInHPower ;
            else
                strMode = _RFECDef_Mode_TestInHPower ;
        }
    }

    return bIsValid ;
}


#pragma warning (pop)
