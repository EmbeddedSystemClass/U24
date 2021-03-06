#include "stdafx.h"
#include "CDMAUtil.h"

namespace CDMAUtility
{
	//////////////////////////////////////////////////////////////////////////
	// CDMA Functions
	//////////////////////////////////////////////////////////////////////////

	bool CDMA_GetPhoneRFModeByBand(int iBand, short& nRFMode) // Using QLib
	{
		nRFMode = 0;

		if (iBand == 800 || iBand == 0)
			nRFMode = PHONE_MODE_CDMA_800;
		else if (iBand == 1900 || iBand == 1)
			nRFMode = PHONE_MODE_CDMA_1900;
		else if (iBand == 1700 || iBand == 15)
			nRFMode = PHONE_MODE_CDMA_BC15;
		else
			return false;

		return true;
	}

	bool CDMA_GetPhoneNVModeByBand(int iBand, short& nNVMode) // Using QLib
	{
		nNVMode = 0;

		if (iBand == 800 || iBand == 0)
			nNVMode = NV_MODE_BC0 ;
		else if (iBand == 1900 || iBand == 1)
			nNVMode = NV_MODE_BC1 ;
		else if (iBand == 1700 || iBand == 15)
			nNVMode = NV_MODE_BC15 ;
		else
			return false;

		return true;
	}

	bool CDMA_GetPhoneFTMModeByBand(int iBand, short& nNVMode)
	{
		nNVMode = 0;

		if (iBand == 800 || iBand == 0)
			nNVMode = NV_MODE_BC0 ;
		else if (iBand == 1900 || iBand == 1)
			nNVMode = NV_MODE_BC1 ;
		else if (iBand == 1700 || iBand == 15)
			nNVMode = NV_MODE_BC15 ;
		else
			return false;

		return true;
	}

	bool CDMA_ChannelToFrequency(int iBand, int iChannel, int iULorDL/*UL:0, DL:1*/, unsigned int& uiFreq_Hz)
	{
		bool bRes = true;
		uiFreq_Hz = 0;

		if (iBand == 0 || iBand == 800)
		{
			if (iChannel >= 1 && iChannel <= 799) // 1 <= N <= 799, 0.030 * N + 825.000 MHz
				uiFreq_Hz = 825000000 + iChannel * 30000;
			else if (iChannel >= 991 && iChannel <= 1023) // 991 <= N <= 1023, 0.030 * (N − 1023) + 825.000 MHz
				uiFreq_Hz = 825000000 + (iChannel - 1023) * 30000;
			else if (iChannel >= 1024 && iChannel <= 1323) // 1024 <= N <= 1323, 0.030 * (N - 1024) + 815.040 MHz
				uiFreq_Hz = 815040000 + (iChannel - 1024) * 30000;
			else
				bRes = false;

			if (iULorDL)
				uiFreq_Hz = uiFreq_Hz + 45000000;
		}
		else if (iBand == 1 || iBand == 1900)
		{
			if (iChannel >= 0 && iChannel <= 1199) // 0<= N <= 1199, 0.050 * N + 1850.000 MHz
				uiFreq_Hz = 1850000000 + iChannel * 50000;
			else
				bRes = false;

			if (iULorDL)
				uiFreq_Hz = uiFreq_Hz + 80000000;
		}
		else if (iBand == 15 || iBand == 1700) // AWS
		{
			if (iChannel >= 0 && iChannel <= 899) // 0<= N <= 899, 0.050 * N + 1710.000 MHz
				uiFreq_Hz = 1710000000 + iChannel * 50000;
			else
				bRes = false;

			if (iULorDL)
				uiFreq_Hz = uiFreq_Hz + 400000000;
		}
		else
			bRes = false;

		return bRes;
	}

	bool CDMA_GetBandNameForCableLoss(int iBand, std::string& strBandName, bool bIsEVDO)
	{
		strBandName.clear() ;

		if (! bIsEVDO)
			strBandName = "CDMABand" ;
		else
			strBandName = "EVDOBand" ;

		if (iBand == 0 || iBand == 800)
			strBandName += "0";
		else if (iBand == 1 || iBand == 1900)
			strBandName += "1";
		else if (iBand == 15 || iBand == 1700)
			strBandName += "15";
		else
			return false;

		return true;
	}

	bool CDMA_GetRangeForMSTxCableLoss(int iBand, int iMSTxChannel, std::string& strRange)
	{
		int iLow, iMid, iHigh;
		strRange.clear();

		if (iBand == 0 || iBand == 800)
		{
			iLow = 991;
			iMid = 350;
			iHigh = 799;
		}
		else if (iBand == 1 || iBand == 1900)
		{
			iLow = 0;
			iMid = 600;
			iHigh = 1199;
		}
		else if (iBand == 15 || iBand == 1700)
		{
			iLow = 0;
			iMid = 450;
			iHigh = 899;
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

	bool CDMA_GetRangeForBSTxCableLoss(int iBand, int iBSTxChannel, std::string& strRange)
	{
		int iLow, iMid, iHigh;
		strRange.clear();

		if (iBand == 0 || iBand == 800)
		{
			iLow = 991;
			iMid = 350;
			iHigh = 799;
		}
		else if (iBand == 1 || iBand == 1900)
		{
			iLow = 0;
			iMid = 600;
			iHigh = 1199;
		}
		else if (iBand == 15 || iBand == 1700)
		{
			iLow = 0;
			iMid = 450;
			iHigh = 899;
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

	bool CDMA_CalculateLossByChannel(int iBand, int iChannel, double dLoss_Low, double dLoss_Mid, double dLoss_High, double& dLoss)
	{
		dLoss = 0.0;

		int iLow, iMid, iHigh;

		if ( 0 == iBand || 800 == iBand )
		{
			iLow = 0;		//iLow = 991;
			iMid = 420;		//iMid = 350 or 387?;
			iHigh = 832;	//iHigh = 799;

			if (991 <= iChannel && iChannel <= 1023)
				iChannel -= 991;
			else if (1 <= iChannel && iChannel <= 799)
				iChannel += 33;
			else
				return false;
		}
		else if ( 1 == iBand || 1900 == iBand )
		{
			iLow = 0;
			iMid = 600;
			iHigh = 1199 ;

			if (! (iLow <= iChannel && iChannel <= iHigh))
				return false;
		}
		else if ( 15 == iBand || 1700 == iBand )
		{
			iLow = 0;
			iMid = 450;
			iHigh = 899;

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

	bool CDMA_GetBandForCMW(int iBand, std::string& strBand)
	{
		strBand.clear();

		if (iBand == 0 || iBand == 800)
			strBand = "USC";
		else if (iBand == 1 || iBand == 1900)
			strBand = "NAPC";
		else if (iBand == 15 || iBand == 1700)
			strBand = "AWS";
		else
			return false;

		return true;
	}
}
