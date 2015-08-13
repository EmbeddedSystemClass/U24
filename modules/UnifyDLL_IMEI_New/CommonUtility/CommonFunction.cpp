#include "stdafx.h"
#include "CommonFunction.h"
#include <SetupAPI.h>

#pragma warning (disable : 4996)

// std::string tokenizer
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

double GSMUplinkChannelToFrequency(int iBand, int iChannel)
{
	double dFreq = 0.0;

	if (iBand == 850)
	{
		if (iChannel >= 128 && iChannel <= 251)
			dFreq = 824.2 + 0.2 * (iChannel - 128);
	}
	else if (iBand == 900)
	{
		if (iChannel >= 0 && iChannel <= 124)
			dFreq = 890.0 + 0.2 * iChannel;
		else if (iChannel >= 955 && iChannel <= 1023)
			dFreq = 876.2 + 0.2 * (iChannel - 955);
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

double GSMDwonlinkChannelToFrequency(int iBand, int iChannel)
{
	double dFreq = 0.0;

	if (iBand == 850)
	{
	}
	else if (iBand == 900)
	{
	}
	else if (iBand == 1800)
	{
	}
	else if (iBand == 1900)
	{
	}

	return dFreq;
}

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

/*********************************************
			CDMA 2000
*********************************************/

double CDMA2000UplinkChannelToFrequency(int iBand, int iChannel,const int iUL_OR_DL) //iUL_OR_DL = 0 is UpLinkFrequency.iUL_OR_DL = 1 is DownLinkFrequency
{
	double dFreq = 0.0;
	if(iBand = 800)
	{
		if(iChannel <800)
		{
			dFreq = 825.0 + 0.03 * (iChannel) + iUL_OR_DL*45;
//			return true;
		}
		else
		{
			dFreq = 825.0 + 0.03 * (iChannel - 1023) + iUL_OR_DL*45;
//			return true;
				
		}
	}
	else if(iBand = 1900)
	{	
		dFreq = 1850.0 + 0.05 * (iChannel) + iUL_OR_DL*80;
//		return true;
	}

	return dFreq; 
}

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
					{
						map_strstrCOMDevice[strTmp] = std::string(szDev);
					}
					else
					{
						map_strstrCOMDevice[strTmp] = std::string("");
					}
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


std::string MakeStr(const char* fmt, ...)
{
	char buf[4096] = {0};
	va_list va;
	va_start(va, fmt);
	vsprintf(buf, fmt, va);
	va_end(va);
	return std::string(buf);
}

bool CheckNoCaseString(char *szFirst, char *szSecond)
{
	CString strFirst = szFirst;
	CString strSecond = szSecond;

	strFirst.MakeLower();
	strSecond.MakeLower();

	if(strFirst != strSecond)	return false;

	return true;
}