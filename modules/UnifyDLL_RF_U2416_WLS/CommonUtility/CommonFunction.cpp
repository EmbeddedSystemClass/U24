#include "stdafx.h"
#include "CommonFunction.h"
//#include "../../../Lib/Qualcomm/QDART4820/inc/QLib_Defines.h"
#include "../../../Lib/Qualcomm/QDART4823/inc/QLib_Defines.h"
#include <SetupAPI.h>


// Disable string warning
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)


bool STDStringTokenizer(
	const std::string& strSource,
	const std::string& strDelimiter,
	std::vector<std::string>& vList,
	bool bAppendDelimiter
	)
{
	bool bRes = true;
	std::string::size_type iCurrPos = 0;
	std::string::size_type iLastDelimiter = std::string::npos;
	const std::string::size_type iTotalLen = strSource.size();
	const std::string::size_type iDelimiterLen = strDelimiter.size();

	vList.clear();

	while (iCurrPos < iTotalLen)
	{
		size_t x = 0;
		for ( ; x < iDelimiterLen; ++x)
		{
			std::string::size_type stFind = strSource.find(strDelimiter[x], iCurrPos);
			if (stFind == std::string::npos)
				;
			else
			{
				iLastDelimiter = stFind;
				size_t stStrLen = stFind - iCurrPos;

				if (bAppendDelimiter)
					++stStrLen;

				vList.push_back(strSource.substr(iCurrPos, stStrLen));
				iCurrPos = stFind + 1;
				break;
			}
		}

		if (x >= iDelimiterLen)
			break;
	}

	if (iLastDelimiter + 1 < iTotalLen)
		vList.push_back(strSource.substr(iLastDelimiter + 1, iTotalLen - iLastDelimiter - 1));

	return bRes;
}

bool ReSplitVector(const std::vector<std::string>& vSource,
                   std::vector<std::string>& vResult,
                   int iCollectItemAmount)
{
    if ( 0 != (vSource.size()%iCollectItemAmount) )
        return false ;

    vResult.clear() ;

    int iResultItemAmount = vSource.size()/iCollectItemAmount ;
    for (int resultCounter=0; resultCounter<iResultItemAmount; ++resultCounter)
    {
        std::string strTmp ;
        strTmp.clear() ;

        for (int sourceCounter=0; sourceCounter<iCollectItemAmount; ++ sourceCounter)
        {    
            // Remove white space
            unsigned int iPost = 0 ;
            std::string strCurrent = vSource[resultCounter*iCollectItemAmount + sourceCounter] ;
            RemoveFrontSpace(strCurrent, strCurrent) ;

            if ( 0==sourceCounter ) {
                strTmp = strCurrent ;
            } else {
                strTmp = strTmp + "," + strCurrent ;
            }
        }

        vResult.push_back(strTmp) ;
    }
    return true ;
}

bool RemoveFrontSpace(const std::string& strOriginal, std::string& strDest)
{
    // Remove white space
    unsigned int iPos = 0 ;
    for (int i=0; i<(int)strOriginal.length(); ++i) {
        if ( '\n'==strOriginal.at(i) || ' '==strOriginal.at(i) || '\t'==strOriginal.at(i) || '\0'==strOriginal.at(i) || NULL==strOriginal.at(i) ) {
            ++iPos ;
        } else {
            break ;
        }
    }

    char szBuffer[MAX_PATH] ;
    ::ZeroMemory(szBuffer, MAX_PATH) ;
    unsigned int iSize = strOriginal.length() - iPos ;
    strOriginal._Copy_s(szBuffer, MAX_PATH, iSize, iPos) ;
    strDest = szBuffer ;

    return true ;
}

bool QueryTestItemIsEnable(const std::vector<std::string>& vTestList, int iTestItemID)
{
    if ( 0>iTestItemID )
        return false ;

    if ( (signed)vTestList.size()<=iTestItemID )
        return false ;

    if ( strstr(vTestList[iTestItemID].c_str(), "1")==NULL )
        return false ;

    return true ;
}

std::string MakeSTDstring(const char* fmt, ...)
{
	char chBuffer[512];
	va_list va;
	va_start (va,fmt);
	int len = vsprintf (chBuffer, fmt, va);
	va_end (va);
	std::string totalmsg;
	totalmsg = chBuffer;

	return totalmsg;
}

std::string MakeStr(const char* fmt, ...)
{
	char buf[4096] = {0};
	va_list va;
	va_start(va, fmt);
	vsprintf(buf, fmt, va);
	va_end(va);
	return std::string(buf);
}

bool IsDigit(std::string str)
{
	for (int i=0;i<(int)str.size();i++)
	{
		if(!std::isdigit(str[i]))
			return false;
	}
	return true;
}

bool COMDeviceControl(char* szDeviceName, int iBaudRate, char* szCommand, char* szErrMsg)
{
	HANDLE hCOMPort;
	DCB dcb;
	COMMTIMEOUTS commTimeouts;
	DWORD iBytesWritten;
	DWORD iBytesRead;

	hCOMPort = CreateFile(szDeviceName, GENERIC_READ | GENERIC_WRITE , 0, NULL, OPEN_EXISTING , 0, NULL);

	if (hCOMPort == INVALID_HANDLE_VALUE)
	{
		sprintf_s(szErrMsg, 255, "Fail to open device");
		return false;
	}

	if (SetupComm(hCOMPort, 128, 128) != TRUE)
	{
		sprintf_s(szErrMsg, 255, "Fail to setup connection");
		CloseHandle(hCOMPort);
		return false;
	}

	if (GetCommState(hCOMPort, &dcb) != TRUE)
	{
		sprintf_s(szErrMsg, 255, "Fail to get connection state");
		CloseHandle(hCOMPort);
		return false;
	}

	dcb.BaudRate = iBaudRate;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.fAbortOnError = TRUE;

	if (SetCommState(hCOMPort, &dcb) != TRUE)
	{
		sprintf_s(szErrMsg, 255, "Fail to set connection state");
		CloseHandle(hCOMPort);
		return false;
	}

	if (GetCommTimeouts(hCOMPort, &commTimeouts) != TRUE)
	{
		sprintf_s(szErrMsg, 255, "Fail to get connection timeouts");
		CloseHandle(hCOMPort);
		return false;
	}

	commTimeouts.ReadIntervalTimeout = 50;
	commTimeouts.ReadTotalTimeoutConstant = 50;
	commTimeouts.ReadTotalTimeoutMultiplier = 10;
	commTimeouts.WriteTotalTimeoutConstant = 50;
	commTimeouts.WriteTotalTimeoutMultiplier = 10;

	if (SetCommTimeouts(hCOMPort, &commTimeouts) != TRUE)
	{
		CloseHandle(hCOMPort);
		return false;
	}

	int iLength = strlen(szCommand);
	char* szCmd = new char[iLength + 2];
	memset(szCmd, 0, iLength + 2);
	sprintf_s(szCmd, iLength + 1, szCommand);
	szCmd[iLength] = '\r';

	char szResponse[256];
	memset(szResponse, 0, 256);

	WriteFile(hCOMPort, szCmd, iLength + 1, &iBytesWritten, NULL);
	Sleep(500);
	ReadFile(hCOMPort, (void*)szResponse, 255, &iBytesRead, NULL);

	delete [] szCmd;

	CloseHandle(hCOMPort);
	return true;
}


//////////////////////////////////////////////////////////////////////////
//    GSM Common Functions
//////////////////////////////////////////////////////////////////////////

double GSM_ULChannelToFrequency(int iBand, int iChannel)
{
    double dFreq = 0.0;

    if (iBand == 850)
    {
        if (iChannel >= 128 && iChannel <= 251)
            dFreq = 824.2 + 0.2 * (iChannel - 128);
    }
    else if (iBand == 900)
    {
        if (iChannel >= 975 && iChannel <= 1023)
            dFreq = 876.2 + 0.2 * (iChannel - 955) ;
        else if (iChannel >= 0 && iChannel <= 124)
            dFreq = 890.0 + 0.2 * iChannel ;
    }
    else if (iBand == 1800)
    {
        if (iChannel >= 512 && iChannel <= 885)
            dFreq = 1710.2 + 0.2 * (iChannel - 512);
    }
    else if (iBand == 1900)
    {
        if (iChannel >= 512 && iChannel <= 810)
            dFreq = 1850.2 + 0.2 * (iChannel - 512);
    }

    return dFreq;
}

double GSM_DLChannelToFrequency(int iBand, int iChannel)
{
    double dFreq = 0.0;

    if (iBand == 850)
        dFreq = GSM_ULChannelToFrequency(iBand, iChannel) + 45 ;
    else if (iBand == 900)
        dFreq = GSM_ULChannelToFrequency(iBand, iChannel) + 45 ;
    else if (iBand == 1800)
        dFreq = GSM_ULChannelToFrequency(iBand, iChannel) + 95 ;
    else if (iBand == 1900)
        dFreq = GSM_ULChannelToFrequency(iBand, iChannel) + 80 ;

    return dFreq;
}

bool GSM_GetBandNameForCableLoss(int iBand, std::string& strBandName)
{
    strBandName.clear() ;

    if (iBand==850)
        strBandName = "GSM850" ;
    else if (iBand==900)
        strBandName = "GSM900" ;
    else if (iBand==1800)
        strBandName = "GSM1800" ;
    else if (iBand==1900)
        strBandName = "GSM1900" ;
    else
        return false ;

    return true ;
}

bool GSM_GetRangeForTxRxCableLoss(int iBand, int iChannel, std::string& strRange)
{
    int iLow, iMid, iHigh;
    strRange.clear();

    if (iBand==850)
    {
        iLow = 128 ;
        iMid = 178 ;
        iHigh= 251;
    }
    else if (iBand==900)
    {
        iLow = 975 ;
        iMid = 021;
        iHigh= 124;
    }
    else if (iBand==1800)
    {
        iLow = 512 ;
        iMid = 662 ;
        iHigh= 885 ;
    }
    else if (iBand=1900)
    {
        iLow = 512 ;
        iMid = 632 ;
        iHigh= 810 ;
    }
    else
        return false;

    int iDelta_Low = abs(iChannel - iLow);
    int iDelta_Mid = abs(iChannel - iMid);
    int iDelta_High= abs(iChannel - iHigh);

    if (iDelta_Low < iDelta_Mid)
    {
        if (iDelta_Low < iDelta_High)
            strRange = "low";
        else
            strRange = "high";
    }
    else
    {
        if (iDelta_Mid < iDelta_High)
            strRange = "mid";
        else
            strRange = "high";
    }

    return true;
}


bool EDGE_IsMultiSlot(const std::string& strSlotEnableList, int& iMultiAmount, bool& bIsMultiSlot)
{
    int iSlotCounter = 0 ;

    std::vector<std::string> vTmp ;
    STDStringTokenizer(strSlotEnableList, ",", vTmp) ;

    for (int i=0; i<(signed)vTmp.size(); ++i)
    {
        if ( strstr(vTmp[i].c_str(), "ON") != NULL ) {
            ++iSlotCounter ;
        }
    }

    if ( 1<iSlotCounter ) {
        bIsMultiSlot = true ;
        iMultiAmount = iSlotCounter ;
    } else if ( 1==iSlotCounter ) {
        bIsMultiSlot = false ;
        iMultiAmount = iSlotCounter ;
    } else {
        bIsMultiSlot = false ;
        iMultiAmount = -1 ;
        return false ;
    }

    return true ;
}


//////////////////////////////////////////////////////////////////////////
//    WCDMA Common Functions
//////////////////////////////////////////////////////////////////////////

double WCDMAUplinkChannelToFrequency(int iBand, int iChannel)
{
	double dFreq = 0.0;

	if (iBand == 1 || iBand == 2100)
	{
		if (iChannel >= 9612 && iChannel <= 9888)
			dFreq = 1922.4 + (iChannel - 9612) * 0.2;
	}
	else if (iBand == 2 || iBand == 1900)
	{
		if (iChannel >= 9262 && iChannel <= 9538)
			dFreq = 1852.4 + (iChannel - 9262) * 0.2;
	}
	else if (iBand == 4 || iBand == 1700)
	{
		if (iChannel >= 1312 && iChannel <= 1513)
			dFreq = 1712.4 + (iChannel - 1312) * 0.2;
	}
	else if (iBand == 5 || iBand == 850)
	{
		if (iChannel >= 4132 && iChannel <= 4233)
			dFreq = 826.4 + (iChannel - 4132) * 0.2;
	}
	else if (iBand == 8 || iBand == 900)
	{
		if (iChannel >= 2712 && iChannel <= 2863)
			dFreq = 882.4 + (iChannel - 2712) * 0.2;
	}

	return dFreq;
}

bool  WCDMACheckValidBand(int iBand)
{
	return (iBand == WCDMA_850_BAND_ID) || (iBand == WCDMA_900_BAND_ID) || (iBand == WCDMA_AWS_BAND_ID)|| (iBand == WCDMA_PCS_BAND_ID) || (iBand == WCDMA_IMT_BAND_ID) ||
			(iBand == 5) || (iBand == 8) || (iBand == 4) || (iBand == 2) ||(iBand == 1);
}

bool  WCDMACheckValidChannel(int iBand,int iChannel)
{
	ASSERT(WCDMACheckValidBand(iBand));
	if(iBand == WCDMA_850_BAND_ID || iBand == 5)
	{
		if ((iChannel <= 4233 && iChannel >= 4132) || (iChannel <= 4458 && iChannel >= 4357)|| (iChannel==1007) || (iChannel==1012) || (iChannel==1032) || (iChannel==1037)||
			(iChannel==1062) || (iChannel==1087) || (iChannel==782) || (iChannel==787) || (iChannel==807) || (iChannel==812)||
			(iChannel==837) || (iChannel==862))
			{return true;}
		else
			return false;
	}
	else if (iBand == WCDMA_900_BAND_ID || iBand==8)
	{
		if ((iChannel <= 2863 && iChannel >= 2712) || (iChannel <= 3088 && iChannel >= 2937))
			{return true;}
		else
			return false;
	}
	else if (iBand == WCDMA_AWS_BAND_ID || iBand==4)
	{
		if ((iChannel <= 1513 && iChannel >= 1312) || (iChannel <= 1738 && iChannel >= 1537))
			{return true;}
		else
			return false;
	}
	else if(iBand == WCDMA_PCS_BAND_ID || iBand == 2)
	{
		if ((iChannel <= 9538 && iChannel >= 9262) || (iChannel <= 9938 && iChannel >= 9662) || (iChannel==412) || (iChannel==437) || (iChannel==462) || (iChannel==487)||
			(iChannel==512) || (iChannel==537) || (iChannel==562) || (iChannel==587)||
			(iChannel==612) || (iChannel==637) || (iChannel==662) || (iChannel==687) || (iChannel== 12) || (iChannel== 37) || (iChannel== 62) || (iChannel== 87)||
			(iChannel==112) || (iChannel==137) || (iChannel==162) || (iChannel==187)||
			(iChannel==212) || (iChannel==237) || (iChannel==262) || (iChannel==287))
			{return true;}
		else
			return false;
	}
	else if (iBand == WCDMA_IMT_BAND_ID || iBand==1)
	{
		if ((iChannel <= 9888 && iChannel >= 9612) || (iChannel <= 10838 && iChannel >= 10562))
			{return true;}
		else
			return false;
	}
	//Have Checked Band, no need default
	ASSERT(0);

	return false ;
}

double WCDMAChannelToFrequency(int iBand,int iChannel)
{
	ASSERT(WCDMACheckValidChannel(iBand,iChannel));

	if(iBand == WCDMA_850_BAND_ID || iBand == 5)
	{
		if (iChannel>=4132)
			return 0.2 * (iChannel);
		else
			return 670.1 + 0.2 * (iChannel);
	}
	else if(iBand == WCDMA_900_BAND_ID || iBand == 8)
	{
			return 0.2 * (iChannel) + 340;
	}
	else if(iBand == WCDMA_AWS_BAND_ID || iBand == 4)
	{	
		/* ignore additional channels that are not in CalChannelList */
		if (iChannel <= 1513 && iChannel >= 1312)
			return 1450 + 0.2 * (iChannel);
		else
			return 1805 + 0.2 * (iChannel);
	}
	else if(iBand == WCDMA_PCS_BAND_ID || iBand == 2)
	{
		if (iChannel>=9262)
			return 0.2 * (iChannel);
		else
			return 1850.1 + 0.2 * (iChannel);
	}	
	else if(iBand == WCDMA_IMT_BAND_ID || iBand==1)
	{
			return 0.2 * (iChannel);
	}

	return -1;
}

int WCDMA_GetDLChannel(int iBand, int iULChannel)
{
	int iDLChannel = 0;

	if (iBand == 1 || iBand == 2100)
		iDLChannel = iULChannel + 950;
	else if (iBand == 2 || iBand == 1900)
		iDLChannel = iULChannel + 400;
	else if (iBand == 4 || iBand == 1700)
		iDLChannel = iULChannel + 225;
	else if (iBand == 5 || iBand == 850)
		iDLChannel = iULChannel + 225;
	else if (iBand == 7 || iBand == 2600)
		iDLChannel = iULChannel + 225;
	else if (iBand == 8 || iBand == 900)
		iDLChannel = iULChannel + 225;

	return iDLChannel;
}

int WCDMA_GetULChannel(int iBand, int iDLChannel)
{
	int iULChannel = 0;

	if (iBand == 1 || iBand == 2100)
		iULChannel = iDLChannel - 950;
	else if (iBand == 2 || iBand == 1900)
		iULChannel = iDLChannel - 400;
	else if (iBand == 4 || iBand == 1700)
		iULChannel = iDLChannel - 225;
	else if (iBand == 5 || iBand == 850)
		iULChannel = iDLChannel - 225;
	else if (iBand == 7 || iBand == 2600)
		iULChannel = iDLChannel - 225;
	else if (iBand == 8 || iBand == 900)
		iULChannel = iDLChannel - 225;

	return iULChannel;
}

bool WCDMA_GetBandNameForCableLoss(int iBand, std::string& strBandName)
{
	strBandName.clear();

	if (iBand == 1 || iBand == 2100)
		strBandName = "UMTSBand1";
	else if (iBand == 2 || iBand == 1900)
		strBandName = "UMTSBand2";
	else if (iBand == 4 || iBand == 1700)
		strBandName = "UMTSBand4";
	else if (iBand == 5 || iBand == 850)
		strBandName = "UMTSBand5";
	else if (iBand == 7 || iBand == 2600)
		strBandName = "UMTSBand7";
	else if (iBand == 8 || iBand == 900)
		strBandName = "UMTSBand8";

	if (strBandName.empty())
		return false;

	return true;
}

bool WCDMA_GetRangeForTxCableLoss(int iBand, int iChannel, std::string& strRange)
{
	int iLow, iMid, iHigh;
	int iRange;
	strRange.clear();

	if (iBand == 1 || iBand == 2100)
	{
		iLow = 9612;
		iMid = 9750;
		iHigh = 9888;
	}
	else if (iBand == 2 || iBand == 1900)
	{
		iLow = 9262;
		iMid = 9400;
		iHigh = 9538;
	}
	else if (iBand == 3 || iBand == 1800)
	{
		iLow = 937;
		iMid = 1113;
		iHigh = 1288;
	}
	else if (iBand == 4 || iBand == 1700)
	{
		iLow = 1312;
		iMid = 1412;
		iHigh = 1513;
	}
	else if (iBand == 5 || iBand == 850)
	{
		iLow = 4132;
		iMid = 4182;
		iHigh = 4233;
	}
	else if (iBand == 6 || iBand == 800)
	{
		iLow = 4162;
		iMid = 4175;
		iHigh = 4188;
	}
	else if (iBand == 7 || iBand == 2600)
	{
		iLow = 2012;
		iMid = 2175;
		iHigh = 2338;
	}
	else if (iBand == 8 || iBand == 900)
	{
		iLow = 2712;
		iMid = 2787;
		iHigh = 2863;
	}
	else
		return false;

	int iDelta_Low = abs(iChannel - iLow);
	int iDelta_Mid = abs(iChannel - iMid);
	int iDelta_High = abs(iChannel - iHigh);

	if (iDelta_Low < iDelta_Mid)
	{
		if (iDelta_Low < iDelta_High)
			iRange = 1;
		else
			iRange = 3;
	}
	else
	{
		if (iDelta_Mid < iDelta_High)
			iRange = 2;
		else
			iRange = 3;
	}

	switch (iRange)
	{
	case 1:
		strRange = "low";
		break;
	case 3:
		strRange = "high";
		break;
	default:
		strRange = "mid";
	}

	return true;
}

bool WCDMA_GetRangeForRxCableLoss(int iBand, int iChannel, std::string& strRange)
{
	int iLow, iMid, iHigh;
	int iRange;
	strRange.clear();

	if (iBand == 1 || iBand == 2100)
	{
		iLow = 10562;
		iMid = 10700;
		iHigh = 10838;
	}
	else if (iBand == 2 || iBand == 1900)
	{
		iLow = 9662;
		iMid = 9800;
		iHigh = 9938;
	}
	else if (iBand == 3 || iBand == 1800)
	{
		iLow = 1162;
		iMid = 1338;
		iHigh = 1513;
	}
	else if (iBand == 4 || iBand == 1700)
	{
		iLow = 1537;
		iMid = 1637;
		iHigh = 1738;
	}
	else if (iBand == 5 || iBand == 850)
	{
		iLow = 4357;
		iMid = 4407;
		iHigh = 4458;
	}
	else if (iBand == 6 || iBand == 800)
	{
		iLow = 4387;
		iMid = 4400;
		iHigh = 4413;
	}
	else if (iBand == 7 || iBand == 2600)
	{
		iLow = 2237;
		iMid = 2400;
		iHigh = 2563;
	}
	else if (iBand == 8 || iBand == 900)
	{
		iLow = 2937;
		iMid = 3012;
		iHigh = 3088;
	}
	else
		return false;

	int iDelta_Low = abs(iChannel - iLow);
	int iDelta_Mid = abs(iChannel - iMid);
	int iDelta_High = abs(iChannel - iHigh);

	if (iDelta_Low < iDelta_Mid)
	{
		if (iDelta_Low < iDelta_High)
			iRange = 1;
		else
			iRange = 3;
	}
	else
	{
		if (iDelta_Mid < iDelta_High)
			iRange = 2;
		else
			iRange = 3;
	}

	switch (iRange)
	{
	case 1:
		strRange = "low";
		break;
	case 3:
		strRange = "high";
		break;
	default:
		strRange = "mid";
	}

	return true;
}


// -------------------- WiFi functions ---------------------

double WiFi_ChannelToFrequency(int iChannel)
{
	double dFrequency = 2412.0 + (iChannel - 1) * 5.0;

	return dFrequency;
}

// -------------------- Auto detect device functions ---------------------

bool GetCOMPortDevByWDK(std::map<std::string, std::string>& map_strstrCOMDevice)
{
	map_strstrCOMDevice.clear();

	BOOL bContinue = FALSE;

	// 1. Retrieves the GUID(s) associated with the specified class name "Ports"
	DWORD uiGuidSize = 0;
	GUID* ptGuidBuf = NULL;

	::SetupDiClassGuidsFromNameA("Ports", NULL, 0, &uiGuidSize);

	if (uiGuidSize > 0)
	{
		ptGuidBuf = new GUID[uiGuidSize];
		ASSERT(ptGuidBuf != NULL);

		if (ptGuidBuf != NULL)
		{
			bContinue = ::SetupDiClassGuidsFromNameA("Ports", ptGuidBuf, uiGuidSize, &uiGuidSize);
		}
	}

	// 2. Returns a device information set that contains all devices of a specified class
	HDEVINFO hDevInfoSet = INVALID_HANDLE_VALUE;

	if (bContinue)
	{
		hDevInfoSet = ::SetupDiGetClassDevsA(ptGuidBuf, NULL, NULL, DIGCF_PRESENT);
		bContinue = (hDevInfoSet != INVALID_HANDLE_VALUE);
	}

	// 3. Start enumeration
	DWORD uiIndex = 0;

	while (bContinue)
	{
		// Returns one context structure for a device information element of a device information set.
		SP_DEVINFO_DATA tDevInfo;
		tDevInfo.cbSize = sizeof(SP_DEVINFO_DATA);
		if ((bContinue = ::SetupDiEnumDeviceInfo(hDevInfoSet, uiIndex, &tDevInfo)))
		{
			// Opens a registry storage key for device-specific configuration information
			HKEY hKey = ::SetupDiOpenDevRegKey(hDevInfoSet, &tDevInfo, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE);

			if (hKey != NULL)
			{
				// Read the name of the port.
				char szDev[256];
				DWORD uiType = 0, uiDevSize = 256;
				memset(szDev, 0, 256);

				if ((::RegQueryValueExA(hKey, "PortName", NULL, &uiType, (LPBYTE)szDev, &uiDevSize) == ERROR_SUCCESS) && (uiType == REG_SZ))
				{
					std::string strTmp = szDev;

					memset(szDev, 0, 256);
					uiDevSize = 256;
					if (SetupDiGetDeviceRegistryProperty(hDevInfoSet, &tDevInfo, SPDRP_DEVICEDESC, &uiType, (PBYTE)szDev, uiDevSize, &uiDevSize) && (uiType == REG_SZ))
						map_strstrCOMDevice[strTmp] = std::string(szDev);
					else
						map_strstrCOMDevice[strTmp] = std::string("");
				}

				// Close the key.
				::RegCloseKey(hKey);
			}
		}

		++uiIndex;
	}

	// clean-up.
	if (ptGuidBuf != NULL)
		delete [] ptGuidBuf;

	if (hDevInfoSet != INVALID_HANDLE_VALUE)
		::SetupDiDestroyDeviceInfoList(hDevInfoSet);

	return true;
}

//////////////////////////////////////////////////////////////////////////
// LTE Functions ( Copy Form RF )
//////////////////////////////////////////////////////////////////////////
bool LTE_GetPhoneRFModeByBand(int iBand, short& nRFMode)
{
	nRFMode = 0;
	switch (iBand)
	{
	case 1:
	case 2100:
		nRFMode = PHONE_MODE_LTE_B1;
		return true;
	case 2:
	case 1900:
		nRFMode = PHONE_MODE_LTE_B2;
		return true;
	case 3:
	case 1800:
		nRFMode = PHONE_MODE_LTE_B3;
		return true;
	case 4:
	case 1700:
		nRFMode = PHONE_MODE_LTE_B4;
		return true;
	case 7:
	case 2600:
		nRFMode = PHONE_MODE_LTE_B7;
		return true;
	case 8:
	case 900:
		nRFMode = PHONE_MODE_LTE_B8;
		return true;
	case 12:
		nRFMode = PHONE_MODE_LTE_B12;
		return true;
	case 13:
		nRFMode = PHONE_MODE_LTE_B13;
		return true;
	case 17:
		nRFMode = PHONE_MODE_LTE_B17;
		return true;
	case 20:
		nRFMode = PHONE_MODE_LTE_B20;
		return true;
	case 28:
	case 281://28A
	case 282://28B
		nRFMode = PHONE_MODE_LTE_B28;
		return true;
	default :
		return false;
	}
	return false;
}

bool LTE_GetPhoneNVModeByBand(int iBand, short& nNVMode)
{
	nNVMode = 0;
	switch (iBand)
	{
	case 1:
	case 2100:
		nNVMode = NV_MODE_LTE_B1;
		return true;
	case 2:
	case 1900:
		nNVMode = NV_MODE_LTE_B2;
		return true;
	case 3:
	case 1800:
		nNVMode = NV_MODE_LTE_B3;
		return true;
	case 4:
	case 1700:
		nNVMode = NV_MODE_LTE_B4;
		return true;
	case 7:
	case 2600:
		nNVMode = NV_MODE_LTE_B7;
		return true;
	case 8:
	case 900:
		nNVMode = NV_MODE_LTE_B8;
		return true;
	case 12:
		nNVMode = NV_MODE_LTE_B12;
		return true;
	case 13:
		nNVMode = NV_MODE_LTE_B13;
		return true;
	case 17:
		nNVMode = NV_MODE_LTE_B17;
		return true;
	case 20:
		nNVMode = NV_MODE_LTE_B20;
		return true;
	case 28:
	case 281://28A
		nNVMode = NV_MODE_LTE_B28;
		return true;
	case 282://28B
		nNVMode = NV_MODE_LTE_B28_B;
		return true;
	default :
		return false;
	}
	return false;
}

bool LTE_GetBSTxChannelByMSTxChannel(int iBand, unsigned int iMSTxChannel, unsigned int& iBSTxChannel)
{
	iBSTxChannel = 0;
	if (iBand >= 1 && iBand <= 28)
	{
		iBSTxChannel = iMSTxChannel - 18000;
		return true;
	}
	else if (281 <= iBand && iBand <= 282)//28A, 28B
	{
		iBSTxChannel = iMSTxChannel - 18000;
		return true;
	}
	else if (iBand >= 33 && iBand <= 43)
	{
		iBSTxChannel = iMSTxChannel;
		return true;
	}

	// Define Frequency Band
	switch (iBand)
	{
	case 2100 : // Band1
	case 1900 : // Band2
	case 1800 : // Band3
	case 1700 : // Band4
	case 2600 : // Band7
	case  900 :	// Band8
		iBSTxChannel = iMSTxChannel - 18000;
		return true;
	default :
		return false;
	}
	return false;
}

bool LTE_GetMSTxChannelByBSTxChannel(int iBand, unsigned int iBSTxChannel, unsigned int& iMSTxChannel)
{
	iMSTxChannel = 0;

	// Define Operating Band
	if (1 <= iBand && iBand <= 28)
	{
		iMSTxChannel = iBSTxChannel + 18000;
		return true;
	}
	else if (281 <= iBand && iBand <= 282)//28A, 28B
	{
		iMSTxChannel = iBSTxChannel + 18000;
		return true;
	}
	else if (33 <= iBand && iBand <= 43)
	{
		iBSTxChannel = iMSTxChannel;
		return true;
	}

	// Define Frequency Band
	switch (iBand)
	{
	case 2100 : // Band1
	case 1900 : // Band2
	case 1800 : // Band3
	case 1700 : // Band4
	case 2600 : // Band7
	case  900 :	// Band8
		iMSTxChannel = iBSTxChannel + 18000;
		return true;
	default :
		return false;
	}
	return false;
}

bool LTE_GetBandFrequency(int iBand,
                          unsigned int& iBSTxChannelLower, unsigned int& iBSTxChannelUpper,
                          unsigned int& iMSTxChannelLower, unsigned int& iMSTxChannelUpper,
                          unsigned int& iBSTxFreqLower,    unsigned int& iBSTxFreqUpper,
                          unsigned int& iMSTxFreqLower,    unsigned int& iMSTxFreqUpper)
{
	// ref.: http://niviuk.free.fr/lte_band.php
	int iDuplexSpacing_MHz = 0;
	unsigned int iTmpCHLower;
	unsigned int iTmpCHUpper;
	unsigned int iTmpFreqLower;
	unsigned int iTmpFreqUpper;
	switch (iBand)
	{
	case 1:
	case 2100:
		iDuplexSpacing_MHz = 190;
		iTmpCHLower   =    0;
		iTmpCHUpper   =  599;
		iTmpFreqLower = 2110;
		iTmpFreqUpper = 2170;
		break;
	case 2:
	case 1900:
		iDuplexSpacing_MHz = 80;
		iTmpCHLower   = 600;
		iTmpCHUpper   = 1199;
		iTmpFreqLower = 1930;
		iTmpFreqUpper = 1990;
		break;
	case 3:
	case 1800:
		iDuplexSpacing_MHz = 95;
		iTmpCHLower   = 1200;
		iTmpCHUpper   = 1949;
		iTmpFreqLower = 1805;
		iTmpFreqUpper = 1880;
		break;
	case 4:
	case 1700:
		iDuplexSpacing_MHz = 400;
		iTmpCHLower   = 1950;
		iTmpCHUpper   = 2399;
		iTmpFreqLower = 2110;
		iTmpFreqUpper = 2155;
		break;
	case 5:
	case 850:
		iDuplexSpacing_MHz = 45;
		iTmpCHLower   = 2400;
		iTmpCHUpper   = 2649;
		iTmpFreqLower = 869;
		iTmpFreqUpper = 894;
		break;
	case 7:
	case 2600:
		iDuplexSpacing_MHz = 120;
		iTmpCHLower   = 2750;
		iTmpCHUpper   = 3449;
		iTmpFreqLower = 2620;
		iTmpFreqUpper = 2690;
		break;
	case 8:
	case 900:
		iDuplexSpacing_MHz = 45;
		iTmpCHLower   = 3450;
		iTmpCHUpper   = 3799;
		iTmpFreqLower =  925;
		iTmpFreqUpper =  960;
		break;
	case 12:
		iDuplexSpacing_MHz = 30;
		iTmpCHUpper   = 5010;
		iTmpCHLower   = 5179;
		iTmpFreqUpper =  729;
		iTmpFreqLower =  746;
		break;
	case 13:
		iDuplexSpacing_MHz = -31;
		iTmpCHUpper   = 5180;
		iTmpCHLower   = 5279;
		iTmpFreqUpper =  746;
		iTmpFreqLower =  756;
		break;
	case 17:
		iDuplexSpacing_MHz = 30;
		iTmpCHUpper   = 5849;
		iTmpCHLower   = 5730;
		iTmpFreqUpper =  746;
		iTmpFreqLower =  734;
		break;
	case 20:
		iDuplexSpacing_MHz = -41;
		iTmpCHUpper   = 6449;
		iTmpCHLower   = 6150;
		iTmpFreqUpper =  821;
		iTmpFreqLower =  791;
		break;
	case 28:
	case 281://28A
		iDuplexSpacing_MHz = 55;
		iTmpCHUpper   = 9509;
		iTmpCHLower   = 9210;
		iTmpFreqUpper =  788;
		iTmpFreqLower =  758;
		break;
	case 282://28B
		iDuplexSpacing_MHz = 55;
		iTmpCHUpper   = 9659;
		iTmpCHLower   = 9360;
		iTmpFreqUpper =  803;
		iTmpFreqLower =  773;
		break;
	default :
		return false;
	}

	// Calculate MSTx part.
	iMSTxChannelLower = iTmpCHLower + 18000;
	iMSTxChannelUpper = iTmpCHUpper + 18000;
	iMSTxFreqLower = iTmpFreqLower - iDuplexSpacing_MHz;
	iMSTxFreqUpper = iTmpFreqUpper - iDuplexSpacing_MHz;

	// Copy BSTx data.
	iBSTxChannelLower = iTmpCHLower;
	iBSTxChannelUpper = iTmpCHUpper;

	// MHz to Hz.
	iBSTxFreqLower = iTmpFreqLower * 1000000;
	iBSTxFreqUpper = iTmpFreqUpper * 1000000;
	iMSTxFreqLower *= 1000000;
	iMSTxFreqUpper *= 1000000;

	return true;
}

bool LTE_MSTxChannelToFrequency(int iBand, unsigned int iMSTxChannel, unsigned int& uiFreq_Hz)
{
	uiFreq_Hz = 0;
	unsigned int iNULL = 0;
	unsigned int iChannelUpperBound = 0;
	unsigned int iChannelLowerBound = 0;
	unsigned int iFreqUpperBound = 0;
	unsigned int iFreqLowerBound = 0;
	if (!LTE_GetBandFrequency(iBand, iNULL, iNULL, iChannelLowerBound, iChannelUpperBound, iNULL, iNULL, iFreqLowerBound, iFreqUpperBound))
		return false;

	// Calculate the step size for each channel
	int iFreqDalta = (iFreqUpperBound - iFreqLowerBound) / (iChannelUpperBound - iChannelLowerBound + 1);

	// if Channel in range, calculate the Frequency
	if (iMSTxChannel >= iChannelLowerBound && iMSTxChannel <= iChannelUpperBound)
		uiFreq_Hz = iFreqLowerBound + (iMSTxChannel - iChannelLowerBound) * iFreqDalta;
	else
		return false;

	// Check is the frequency in range?
	if (uiFreq_Hz >= iFreqLowerBound || uiFreq_Hz <= iFreqUpperBound)
		return true;
	else
		uiFreq_Hz = 0;

	return false;
}

bool LTE_BSTxChannelToFrequency(int iBand, unsigned int iBSTxChannel, unsigned int& uiFreq_Hz, bool bIsCal)
{
	uiFreq_Hz = 0;
	unsigned int iNULL = 0;
	unsigned int iChannelUpperBound = 0;
	unsigned int iChannelLowerBound = 0;
	unsigned int iFreqUpperBound = 0;
	unsigned int iFreqLowerBound = 0;
	// Get all current
	if (!LTE_GetBandFrequency(iBand, iChannelLowerBound, iChannelUpperBound, iNULL, iNULL, iFreqLowerBound, iFreqUpperBound, iNULL, iNULL))
		return false;

	// Calculate the step size for each channel
	int iFreqDalta = (iFreqUpperBound - iFreqLowerBound) / (iChannelUpperBound - iChannelLowerBound + 1);

	// if Channel in range, calculate the Freq
	if (iBSTxChannel >= iChannelLowerBound && iBSTxChannel <= iChannelUpperBound)
	{
		uiFreq_Hz = iFreqLowerBound + (iBSTxChannel - iChannelLowerBound) * iFreqDalta;
		if (bIsCal)
			uiFreq_Hz = 500000 + uiFreq_Hz;
	}
	else
	{
		return false;
	}

	// Check is the frequency in range?
	if (uiFreq_Hz >= iFreqLowerBound || uiFreq_Hz <= iFreqUpperBound)
		return true;
	else
		uiFreq_Hz = 0;

	return false;
}

bool LTE_GetBandNameForCableLoss(int iBand, std::string& strBandName)
{
	strBandName = "LTEBand";
	switch (iBand)
	{
	case 1:
	case 2100:
		strBandName = strBandName + "1";
		return true;
	case 2:
	case 1900:
		strBandName = strBandName + "2";
		return true;
	case 3:
	case 1800:
		strBandName = strBandName + "3";
		return true;
	case 4:
	case 1700:
		strBandName = strBandName + "4";
		return true;
	case 7:
	case 2600:
		strBandName = strBandName + "7";
		return true;
	case 8:
	case 900:
		strBandName = strBandName + "8";
		return true;
	case 12:
	case 13:
	case 17:
	case 20:
		strBandName = strBandName + MakeStr("%d", iBand);
		return true;
	case 281:
		strBandName = strBandName + "28A";
		return true;
	case 282:
		strBandName = strBandName + "28B";
		return true;
	default :
		strBandName.clear();
		return false;
	}
	return false;
}

bool LTE_GetRangeForMSTxCableLoss(int iBand, unsigned int iMSTxChannel, std::string& strRange)
{
	strRange.clear();
	unsigned int iNULL = 0;
	unsigned int iLow, iMid, iHigh;
	if (!LTE_GetBandFrequency(iBand, iNULL, iNULL, iLow, iHigh, iNULL, iNULL, iNULL, iNULL))
		return false;
	iMid  = (int)((iHigh + iLow + 1) / 2);

	int iDelta_Low  = abs((int)(iMSTxChannel - iLow));
	int iDelta_Mid  = abs((int)(iMSTxChannel - iMid));
	int iDelta_High = abs((int)(iMSTxChannel - iHigh));
	if (iDelta_Low < iDelta_Mid)
		strRange = (iDelta_Low < iDelta_High) ? "low" : "high";
	else
		strRange = (iDelta_Mid < iDelta_High) ? "mid" : "high";

	return true;
}

bool LTE_GetRangeForBSTxCableLoss(int iBand, unsigned int iBSTxChannel, std::string& strRange)
{
	strRange.clear();
	unsigned int iNULL = 0;
	unsigned int iLow, iMid, iHigh;
	if (!LTE_GetBandFrequency(iBand, iLow, iHigh, iNULL, iNULL, iNULL, iNULL, iNULL, iNULL))
		return false;
	iMid  = (int)((iHigh + iLow + 1) / 2);

	int iDelta_Low  = abs((int)(iBSTxChannel - iLow));
	int iDelta_Mid  = abs((int)(iBSTxChannel - iMid));
	int iDelta_High = abs((int)(iBSTxChannel - iHigh));
	if (iDelta_Low < iDelta_Mid)
		strRange = (iDelta_Low < iDelta_High) ? "low" : "high";
	else
		strRange = (iDelta_Mid < iDelta_High) ? "mid" : "high";

	return true;
}

bool LTE_CalculateMSTxLossByChannel(int iBand, unsigned int iChannel, double dLoss_Low, double dLoss_Mid, double dLoss_High, double& dLoss)
{
	dLoss = 0.0;
	unsigned int iLow, iMid, iHigh;
	unsigned int iNULL = 0;
	if (!LTE_GetBandFrequency(iBand, iNULL, iNULL, iLow, iHigh, iNULL, iNULL, iNULL, iNULL))
		return false;
	iMid  = (int)((iHigh + iLow + 1) / 2);

	if (!(iLow <= iChannel && iChannel <= iHigh))
		return false;

	if (iChannel == iLow)
	{
		dLoss = dLoss_Low;
		return true;
	}
	else if (iChannel == iMid)
	{
		dLoss = dLoss_Mid;
		return true;
	}
	else if (iChannel == iHigh)
	{
		dLoss = dLoss_High;
		return true;
	}
	else
	{
		if (iLow < iChannel && iChannel < iMid)
			dLoss = (dLoss_Mid - dLoss_Low) * (iChannel - iLow) / (iMid - iLow) + dLoss_Low;
		else
			dLoss = (dLoss_High - dLoss_Mid) * (iChannel - iMid) / (iHigh - iMid) + dLoss_Mid;
		return true;
	}

	return false;
}

bool LTE_CalculateBSTxLossByChannel(int iBand, unsigned int iChannel, double dLoss_Low, double dLoss_Mid, double dLoss_High, double& dLoss)
{
	dLoss = 0.0;
	unsigned int iLow, iMid, iHigh;
	unsigned int iNULL = 0;
	if (!LTE_GetBandFrequency(iBand, iLow, iHigh, iNULL, iNULL, iNULL, iNULL, iNULL, iNULL))
		return false;
	iMid  = (int)((iHigh + iLow + 1) / 2);

	if (!(iLow <= iChannel && iChannel <= iHigh))
		return false;

	if (iChannel == iLow)
	{
		dLoss = dLoss_Low;
		return true;
	}
	else if (iChannel == iMid)
	{
		dLoss = dLoss_Mid;
		return true;
	}
	else if (iChannel == iHigh)
	{
		dLoss = dLoss_High;
		return true;
	}
	else
	{
		if (iLow < iChannel && iChannel < iMid)
			dLoss = (dLoss_Mid - dLoss_Low) * (iChannel - iLow) / (iMid - iLow) + dLoss_Low;
		else
			dLoss = (dLoss_High - dLoss_Mid) * (iChannel - iMid) / (iHigh - iMid) + dLoss_Mid;
		return true;
	}
	return false;
}

bool LTE_GetBandForCMW(int iBand, std::string& strBand)
{
	strBand.clear();
	switch (iBand)
	{
	case 1:
	case 2100:
		strBand = "OB1";
		break;
	case 2:
	case 1900:
		strBand = "OB2";
		break;
	case 3:
	case 1800:
		strBand = "OB3";
		break;
	case 4:
	case 1700:
		strBand = "OB4";
		break;
	case 7:
	case 2600:
		strBand = "OB7";
		break;
	case 8:
	case 900:
		strBand = "OB8";
		break;
	case 12:
		strBand = "OB12";
		break;
	case 13:
		strBand = "OB13";
		break;
	case 17:
		strBand = "OB17";
		break;
	case 20:
		strBand = "OB20";
		break;
	case 281:
	case 282:
		strBand = "OB28";
		break;
	default:
		return false;
	}
	return true;
}
