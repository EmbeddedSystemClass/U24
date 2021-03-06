#include "stdafx.h"
#include "WCDMAUtil.h"

namespace WCDMAUtility
{
	//////////////////////////////////////////////////////////////////////////
	// WCDMA Functions
	//////////////////////////////////////////////////////////////////////////

	bool WCDMA_GetPhoneRFModeByBand(int iBand, short& nRFMode) // Using QLib
	{
		nRFMode = 0;

		if (iBand == 2100 || iBand == 1)
			nRFMode = PHONE_MODE_WCDMA_IMT ;
		else if (iBand == 1900 || iBand == 2)
			nRFMode = PHONE_MODE_WCDMA_1900B ; // ?
		else if (iBand == 1700 || iBand == 4)
			nRFMode = PHONE_MODE_WCDMA_BC4 ;
		else if (iBand == 850 || iBand == 5)
			nRFMode = PHONE_MODE_WCDMA_800 ;
		else if (iBand == 900 || iBand == 8)
			nRFMode = PHONE_MODE_WCDMA_BC8 ;
		else
			return false ;

		return true ;
	}

	bool WCDMA_GetPhoneNVModeByBand(int iBand, short& nNVMode) // Using QLib
	{
		nNVMode = 0;

		if (iBand == 2100 || iBand == 1)
			nNVMode = NV_MODE_WCDMA_IMT ;
		else if (iBand == 1900 || iBand == 2)
			nNVMode = NV_MODE_WCDMA_1900B ; // ?
		else if (iBand == 1700 || iBand == 4)
			nNVMode = NV_MODE_WCDMA_BC4 ;
		else if (iBand == 850 || iBand == 5)
			nNVMode = NV_MODE_WCDMA_800 ;
		else if (iBand == 900 || iBand == 8)
			nNVMode = NV_MODE_WCDMA_BC8 ;
		else
			return false ;

		return true ;
	}

	bool WCDMA_GetBSTxChannelByMSTxChannel(int iBand, int iMSTxChannel, int& iBSTxChannel)
	{
		iBSTxChannel = 0;

		if (iBand == 1 || iBand == 2100)
			iBSTxChannel = iMSTxChannel + 950;
		else if (iBand == 2 || iBand == 1900)
			iBSTxChannel = iMSTxChannel + 400;
		else if (iBand == 4 || iBand == 1700)
			iBSTxChannel = iMSTxChannel + 225;
		else if (iBand == 5 || iBand == 850)
			iBSTxChannel = iMSTxChannel + 225;
		else if (iBand == 7 || iBand == 2600)
			iBSTxChannel = iMSTxChannel + 225;
		else if (iBand == 8 || iBand == 900)
			iBSTxChannel = iMSTxChannel + 225;
		else
			return false;

		return true;
	}

	bool WCDMA_GetMSTxChannelByBSTxChannel(int iBand, int iBSTxChannel, int& iMSTxChannel)
	{
		iMSTxChannel = 0;

		if (iBand == 1 || iBand == 2100)
			iMSTxChannel = iBSTxChannel - 950;
		else if (iBand == 2 || iBand == 1900)
			iMSTxChannel = iBSTxChannel - 400;
		else if (iBand == 4 || iBand == 1700)
			iMSTxChannel = iBSTxChannel - 225;
		else if (iBand == 5 || iBand == 850)
			iMSTxChannel = iBSTxChannel - 225;
		else if (iBand == 7 || iBand == 2600)
			iMSTxChannel = iBSTxChannel - 225;
		else if (iBand == 8 || iBand == 900)
			iMSTxChannel = iBSTxChannel - 225;
		else
			return false;

		return true;
	}

	bool WCDMA_ULChannelToFrequency(int iBand, int iMSTxChannel, unsigned int& uiFreq_Hz)
	{
		bool bRes = false;
		uiFreq_Hz = 0;

		if (iBand == 1 || iBand == 2100)
		{
			if (iMSTxChannel >= 9612 && iMSTxChannel <= 9888)
			{
				uiFreq_Hz = 1922400000 + (iMSTxChannel - 9612) * 200000;
				bRes = true;
			}
		}
		else if (iBand == 2 || iBand == 1900)
		{
			if (iMSTxChannel >= 9262 && iMSTxChannel <= 9538)
			{
				uiFreq_Hz = 1852400000 + (iMSTxChannel - 9262) * 200000;
				bRes = true;
			}
		}
		else if (iBand == 4 || iBand == 1700)
		{
			if (iMSTxChannel >= 1312 && iMSTxChannel <= 1513)
			{
				uiFreq_Hz = 1712400000 + (iMSTxChannel - 1312) * 200000;
				bRes = true;
			}
		}
		else if (iBand == 5 || iBand == 850)
		{
			if (iMSTxChannel >= 4132 && iMSTxChannel <= 4233)
			{
				uiFreq_Hz = 826400000 + (iMSTxChannel - 4132) * 200000;
				bRes = true;
			}
		}
		else if (iBand == 8 || iBand == 900)
		{
			if (iMSTxChannel >= 2712 && iMSTxChannel <= 2863)
			{
				uiFreq_Hz = 882400000 + (iMSTxChannel - 2712) * 200000;
				bRes = true;
			}
		}

		return bRes;
	}

	bool WCDMA_DLChannelToFrequency(int iBand, int iBSTxChannel, unsigned int& uiFreq_Hz)
	{
		bool bRes = false;
		uiFreq_Hz = 0;

		if (iBand == 1 || iBand == 2100)
		{
			if (iBSTxChannel >= 10562 && iBSTxChannel <= 10838)
			{
				uiFreq_Hz = 2112400000 + (iBSTxChannel - 10562) * 200000;
				bRes = true;
			}
		}
		else if (iBand == 2 || iBand == 1900)
		{
			if (iBSTxChannel >= 9662 && iBSTxChannel <= 9938)
			{
				uiFreq_Hz = 1932400000 + (iBSTxChannel - 9662) * 200000;
				bRes = true;
			}
		}
		else if (iBand == 4 || iBand == 1700)
		{
			if (iBSTxChannel >= 1537 && iBSTxChannel <= 1738)
			{
				uiFreq_Hz = 2112400000 + (iBSTxChannel - 1537) * 200000;
				bRes = true;
			}
		}
		else if (iBand == 5 || iBand == 850)
		{
			if (iBSTxChannel >= 4357 && iBSTxChannel <= 4458)
			{
				uiFreq_Hz = 871400000 + (iBSTxChannel - 4357) * 200000;
				bRes = true;
			}
		}
		else if (iBand == 8 || iBand == 900)
		{
			if (iBSTxChannel >= 2937 && iBSTxChannel <= 3088)
			{
				uiFreq_Hz = 927400000 + (iBSTxChannel - 2937) * 200000;
				bRes = true;
			}
		}

		return bRes;
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
		else if (iBand == 8 || iBand == 900)
			strBandName = "UMTSBand8";
		else
			return false;

		return true;
	}

	bool WCDMA_GetRangeForMSTxCableLoss(int iBand, int iMSTxChannel, std::string& strRange)
	{
		int iLow, iMid, iHigh;
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
		else if (iBand == 8 || iBand == 900)
		{
			iLow = 2712;
			iMid = 2787;
			iHigh = 2863;
		}
		else
			return false;

		int iDelta_Low = abs(iMSTxChannel - iLow);
		int iDelta_Mid = abs(iMSTxChannel - iMid);
		int iDelta_High = abs(iMSTxChannel - iHigh);

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

	bool WCDMA_GetRangeForBSTxCableLoss(int iBand, int iBSTxChannel, std::string& strRange)
	{
		int iLow, iMid, iHigh;
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
		else if (iBand == 8 || iBand == 900)
		{
			iLow = 2937;
			iMid = 3012;
			iHigh = 3088;
		}
		else
			return false;

		int iDelta_Low = abs(iBSTxChannel - iLow);
		int iDelta_Mid = abs(iBSTxChannel - iMid);
		int iDelta_High = abs(iBSTxChannel - iHigh);

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

	bool WCDMA_CalculateLossByChannel(int iBand, int iChannel, double dLoss_Low, double dLoss_Mid, double dLoss_High, double& dLoss)
	{
		dLoss = 0.0;

		int iLow, iMid, iHigh;

		if (iBand == 2100)
		{
			iLow = 9612;
			iMid = 9750;
			iHigh = 9888;

			if (! (iLow <= iChannel && iChannel <= iHigh))
				return false;
		}
		else if (iBand == 1900)
		{
			iLow = 9262;
			iMid = 9400;
			iHigh = 9538;

			if (! (iLow <= iChannel && iChannel <= iHigh))
				return false;
		}
		else if (iBand == 1700)
		{
			iLow = 1312;
			iMid = 1412;
			iHigh = 1513;

			if (! (iLow <= iChannel && iChannel <= iHigh))
				return false;
		}
		else if (iBand == 850)
		{
			iLow = 4132;
			iMid = 4182;
			iHigh = 4233;

			if (! (iLow <= iChannel && iChannel <= iHigh))
				return false;
		}
		else if (iBand == 900)
		{
			iLow = 2712;
			iMid = 2787;
			iHigh = 2863;

			if (! (iLow <= iChannel && iChannel <= iHigh))
				return false;
		}
		else
			return false;

		if (iChannel == iLow)
			dLoss = dLoss_Low;
		else if (iChannel == iMid)
			dLoss = dLoss_Mid;
		else if (iChannel == iHigh)
			dLoss = dLoss_High;
		else
		{
			if (iLow < iChannel && iChannel < iMid)
				dLoss = (dLoss_Mid - dLoss_Low) * (iChannel - iLow) / (iMid - iLow) + dLoss_Low;
			else
				dLoss = (dLoss_High - dLoss_Mid) * (iChannel - iMid) / (iHigh - iMid) + dLoss_Mid;
		}

		return true;
	}

	bool WCDMA_GetBandForCMW(int iBand, std::string& strBand)
	{
		strBand.clear();

		if (iBand == 1 || iBand == 2100)
			strBand = "OB1";
		else if (iBand == 2 || iBand == 1900)
			strBand = "OB2";
		else if (iBand == 4 || iBand == 1700)
			strBand = "OB4";
		else if (iBand == 5 || iBand == 850)
			strBand = "OB5";
		else if (iBand == 8 || iBand == 900)
			strBand = "OB8";
		else
			return false;

		return true;
	}
}
