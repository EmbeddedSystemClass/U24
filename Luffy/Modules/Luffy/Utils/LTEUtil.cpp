#include "stdafx.h"
#include "LTEUtil.h"

namespace LTEUtility
{
	//////////////////////////////////////////////////////////////////////////
	// LTE Functions
	//////////////////////////////////////////////////////////////////////////

	bool LTE_GetPhoneRFModeByBand(int iBand, short& nRFMode) // Using QLib
	{
		nRFMode = 0;

		switch (iBand)
		{
		case 1:
		case 2100:
			nRFMode = PHONE_MODE_LTE_B1 ;
			return true ;
		case 2:
		case 1900:
			nRFMode = PHONE_MODE_LTE_B2 ;
			return true ;
		case 3:
		case 1800:
			nRFMode = PHONE_MODE_LTE_B3 ;
			return true ;
		case 4:
		case 1700:
			nRFMode = PHONE_MODE_LTE_B4 ;
			return true ;
		case 7:
		case 2600:
			nRFMode = PHONE_MODE_LTE_B7 ;
			return true ;
		case 8:
		case 900:
			nRFMode = PHONE_MODE_LTE_B8 ;
			return true ;
		case 12:
			nRFMode = PHONE_MODE_LTE_B12 ;
			return true ;
		case 13:
			nRFMode = PHONE_MODE_LTE_B13 ;
			return true ;
		case 17:
			nRFMode = PHONE_MODE_LTE_B17 ;
			return true ;
		case 20:
			nRFMode = PHONE_MODE_LTE_B20 ;
			return true ;
		default :
			return false ;
		}

		return false ;
	}


	bool LTE_GetPhoneNVModeByBand(int iBand, short& nNVMode) // Using QLib
	{
		nNVMode = 0;

		switch (iBand)
		{
		case 1:
		case 2100:
			nNVMode = NV_MODE_LTE_B1 ;
			return true ;
		case 2:
		case 1900:
			nNVMode = NV_MODE_LTE_B2 ;
			return true ;
		case 3:
		case 1800:
			nNVMode = NV_MODE_LTE_B3 ;
			return true ;
		case 4:
		case 1700:
			nNVMode = NV_MODE_LTE_B4 ;
			return true ;
		case 7:
		case 2600:
			nNVMode = NV_MODE_LTE_B7 ;
			return true ;
		case 8:
		case 900:
			nNVMode = NV_MODE_LTE_B8 ;
			return true ;
		case 12:
			nNVMode = NV_MODE_LTE_B12 ;
			return true ;
		case 13:
			nNVMode = NV_MODE_LTE_B13 ;
			return true ;
		case 17:
			nNVMode = NV_MODE_LTE_B17 ;
			return true ;
		case 20:
			nNVMode = NV_MODE_LTE_B20 ;
			return true ;
		default :
			return false ;
		}

		return false ;
	}

	bool LTE_GetBSTxChannelByMSTxChannel(int iBand, unsigned int iMSTxChannel, unsigned int& iBSTxChannel)
	{
		iBSTxChannel = 0 ;

		// Define Operating Band
		if ( iBand>=1 && iBand<=26 )
		{
			iBSTxChannel = iMSTxChannel - 18000 ;
			return true ;
		}
		else if ( iBand>=33 && iBand<=43 )
		{
			iBSTxChannel = iMSTxChannel ;
			return true ;
		}

		// Define Frequency Band
		switch (iBand)
		{
		case 2100 : // Band1
		case 1900 : // Band2
		case 1800 : // Band3
		case 1700 : // Band4
		case 2600 : // Band7
		case 900  :	// Band8
			iBSTxChannel = iMSTxChannel - 18000 ;
			return true ;

		default :
			return false ;
		}

		return false ;
	}

	bool LTE_GetMSTxChannelByBSTxChannel(int iBand, unsigned int iBSTxChannel, unsigned int& iMSTxChannel)
	{
		iMSTxChannel = 0;

		// Define Operating Band
		if ( iBand>=1 && iBand<=26 )
		{
			iMSTxChannel = iBSTxChannel + 18000 ;
			return true ;
		}
		else if ( iBand>=33 && iBand<=43 )
		{
			iBSTxChannel = iMSTxChannel ;
			return true ;
		}

		// Define Frequency Band
		switch (iBand)
		{
		case 2100 : // Band1
		case 1900 : // Band2
		case 1800 : // Band3
		case 1700 : // Band4
		case 2600 : // Band7
		case 900 :	// Band8
			iMSTxChannel = iBSTxChannel + 18000 ;
			return true ;

		default :
			return false ;
		}

		return false ;
	}

	bool LTE_GetBandFrequency(int iBand,
		unsigned int& iBSTxChannelLower, unsigned int& iBSTxChannelUpper,
		unsigned int& iMSTxChannelLower, unsigned int& iMSTxChannelUpper,
		unsigned int& iBSTxFreqLower,    unsigned int& iBSTxFreqUpper,
		unsigned int& iMSTxFreqLower,    unsigned int& iMSTxFreqUpper)
	{
		// ref.: http://niviuk.free.fr/lte_band.php

		int iDuplexSpacing_MHz = 0 ;
		unsigned int iTmpCHLower ;
		unsigned int iTmpCHUpper ;
		unsigned int iTmpFreqLower ;
		unsigned int iTmpFreqUpper ;

		switch (iBand)
		{
		case 1 :
		case 2100 :
			iDuplexSpacing_MHz = 190 ;
			iTmpCHLower = 0 ;
			iTmpCHUpper = 599 ;
			iTmpFreqLower = 2110 ;
			iTmpFreqUpper = 2170 ;
			break ;
		case 2 :
		case 1900 :
			iDuplexSpacing_MHz = 80 ;
			iTmpCHLower = 600 ;
			iTmpCHUpper = 1199 ;
			iTmpFreqLower = 1930 ;
			iTmpFreqUpper = 1990 ;
			break ;
		case 3 :
		case 1800 :
			iDuplexSpacing_MHz = 95 ;
			iTmpCHLower = 1200 ;
			iTmpCHUpper = 1949 ;
			iTmpFreqLower = 1805 ;
			iTmpFreqUpper = 1880 ;
			break ;
		case 4 :
		case 1700 :
			iDuplexSpacing_MHz = 400 ;
			iTmpCHLower = 1950 ;
			iTmpCHUpper = 2399 ;
			iTmpFreqLower = 2110 ;
			iTmpFreqUpper = 2155 ;
			break ;
		case 7 :
		case 2600 :
			iDuplexSpacing_MHz = 120 ;
			iTmpCHLower = 2750 ;
			iTmpCHUpper = 3449 ;
			iTmpFreqLower = 2620 ;
			iTmpFreqUpper = 2690 ;
			break ;
		case 8 :
		case 900 :
			iDuplexSpacing_MHz = 45 ;
			iTmpCHLower = 3450 ;
			iTmpCHUpper = 3799 ;
			iTmpFreqLower = 925 ;
			iTmpFreqUpper = 960 ;
			break ;
		case 12 :
			iDuplexSpacing_MHz = 30 ;
			iTmpCHUpper = 5010 ;
			iTmpCHLower = 5179 ;
			iTmpFreqUpper = 729 ;
			iTmpFreqLower = 746 ;
			break ;

		case 13 :
			iDuplexSpacing_MHz = -31 ;
			iTmpCHUpper = 5180 ;
			iTmpCHLower = 5279 ;
			iTmpFreqUpper = 746 ;
			iTmpFreqLower = 756 ;
			break ;

		case 17 :
			iDuplexSpacing_MHz = 30 ;
			iTmpCHUpper = 5730 ;
			iTmpCHLower = 5849 ;
			iTmpFreqUpper = 734 ;
			iTmpFreqLower = 746 ;
			break ;

		case 20 :
			iDuplexSpacing_MHz = -41 ;
			iTmpCHLower = 6150 ;
			iTmpCHUpper = 6449 ;
			iTmpFreqLower = 791 ;
			iTmpFreqUpper = 821 ;
			break ;

		default :
			return false ;
		}


		// Calculate MSTx part.
		iMSTxChannelLower = iTmpCHLower + 18000 ;
		iMSTxChannelUpper = iTmpCHUpper + 18000 ;
		iMSTxFreqLower = iTmpFreqLower - iDuplexSpacing_MHz ;
		iMSTxFreqUpper = iTmpFreqUpper - iDuplexSpacing_MHz ;

		// Copy BSTx data.
		iBSTxChannelLower = iTmpCHLower ;
		iBSTxChannelUpper = iTmpCHUpper ;

		// MHz to Hz.
		iBSTxFreqLower = iTmpFreqLower * 1000000 ;
		iBSTxFreqUpper = iTmpFreqUpper * 1000000 ;
		iMSTxFreqLower *= 1000000 ;
		iMSTxFreqUpper *= 1000000 ;

		return true ;
	}

	bool LTE_MSTxChannelToFrequency(int iBand, unsigned int iMSTxChannel, unsigned int& uiFreq_Hz)
	{
		uiFreq_Hz = 0 ;

		unsigned int iNULL = 0 ;
		unsigned int iChannelUpperBound = 0 ;
		unsigned int iChannelLowerBound = 0 ;
		unsigned int iFreqUpperBound = 0 ;
		unsigned int iFreqLowerBound = 0 ;

		if (! LTE_GetBandFrequency(iBand, iNULL, iNULL, iChannelLowerBound, iChannelUpperBound, iNULL, iNULL, iFreqLowerBound, iFreqUpperBound) )
			return false ;

		// Calculate the step size for each channel
		int iFreqDalta = (iFreqUpperBound-iFreqLowerBound) / (iChannelUpperBound-iChannelLowerBound+1) ;

		// if Channel in range, calculate the Frequency
		if (iMSTxChannel >= iChannelLowerBound && iMSTxChannel <= iChannelUpperBound) {
			uiFreq_Hz = iFreqLowerBound + (iMSTxChannel - iChannelLowerBound) * iFreqDalta ;
		}
		else {
			return false ;
		}

		// Check is the frequency in range?
		if ( uiFreq_Hz >= iFreqLowerBound || uiFreq_Hz <= iFreqUpperBound ) {
			return true ;
		} else {
			uiFreq_Hz = 0 ;
		}

		return false ;
	}

	bool LTE_BSTxChannelToFrequency(int iBand, unsigned int iBSTxChannel, unsigned int& uiFreq_Hz, bool bIsCal)
	{
		uiFreq_Hz = 0;

		unsigned int iNULL = 0 ;
		unsigned int iChannelUpperBound = 0 ;
		unsigned int iChannelLowerBound = 0 ;
		unsigned int iFreqUpperBound = 0 ;
		unsigned int iFreqLowerBound = 0 ;

		// Get all current 
		if (! LTE_GetBandFrequency(iBand, iChannelLowerBound, iChannelUpperBound, iNULL, iNULL, iFreqLowerBound, iFreqUpperBound, iNULL, iNULL) )
			return false ;

		// Calculate the step size for each channel
		int iFreqDalta = (iFreqUpperBound-iFreqLowerBound) / (iChannelUpperBound-iChannelLowerBound+1) ;

		// if Channel in range, calculate the Freq
		if (iBSTxChannel >= iChannelLowerBound && iBSTxChannel <= iChannelUpperBound) {
			uiFreq_Hz = iFreqLowerBound + (iBSTxChannel - iChannelLowerBound) * iFreqDalta ;

			if (bIsCal)
				uiFreq_Hz = 500000 + uiFreq_Hz ;
		}
		else {
			return false ;
		}

		// Check is the frequency in range?
		if ( uiFreq_Hz >= iFreqLowerBound || uiFreq_Hz <= iFreqUpperBound ) {
			return true ;
		} else {
			uiFreq_Hz = 0 ;
		}

		return false ;
	}

	bool LTE_GetBandNameForCableLoss(int iBand, std::string& strBandName)
	{
		strBandName = "LTEBand" ;

		switch (iBand)
		{
		case 1:
		case 2100:
			strBandName = strBandName + "1" ;
			return true ;
		case 2:
		case 1900:
			strBandName = strBandName + "2" ;
			return true ;
		case 3:
		case 1800:
			strBandName = strBandName + "3" ;
			return true ;
		case 4:
		case 1700:
			strBandName = strBandName + "4" ;
			return true ;
		case 7:
		case 2600:
			strBandName = strBandName + "7" ;
			return true ;
		case 8:
		case 900:
			strBandName = strBandName + "8" ;
			return true ;
		case 12:
			strBandName = strBandName + "12" ;
			return true ;
		case 13:
			strBandName = strBandName + "13" ;
			return true ;
		case 17:
			strBandName = strBandName + "17" ;
			return true ;
		case 20:
			strBandName = strBandName + "20" ;
			return true ;
		default :
			strBandName.clear() ;
			return false ;
		}

		return false ;
	}

	bool LTE_GetRangeForMSTxCableLoss(int iBand, unsigned int iMSTxChannel, std::string& strRange)
	{
		strRange.clear() ;

		unsigned int iNULL = 0 ;
		unsigned int iLow, iMid, iHigh ;

		if (! LTE_GetBandFrequency(iBand, iNULL, iNULL, iLow, iHigh, iNULL, iNULL, iNULL, iNULL) )
			return false ;

		iMid  = (int)( (iHigh + iLow + 1) / 2 ) ;

		// Determine range
		int iDelta_Low  = abs( (int)(iMSTxChannel - iLow) ) ;
		int iDelta_Mid  = abs( (int)(iMSTxChannel - iMid) ) ;
		int iDelta_High = abs( (int)(iMSTxChannel - iHigh) ) ;

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

	bool LTE_GetRangeForBSTxCableLoss(int iBand, unsigned int iBSTxChannel, std::string& strRange)
	{
		strRange.clear();

		unsigned int iNULL = 0 ;
		unsigned int iLow, iMid, iHigh;

		if (! LTE_GetBandFrequency(iBand, iLow, iHigh, iNULL, iNULL, iNULL, iNULL, iNULL, iNULL) )
			return false ;

		iMid  = (int)( (iHigh + iLow + 1) / 2 ) ;


		// Determine range
		int iDelta_Low  = abs( (int)(iBSTxChannel - iLow) ) ;
		int iDelta_Mid  = abs( (int)(iBSTxChannel - iMid) ) ;
		int iDelta_High = abs( (int)(iBSTxChannel - iHigh) ) ;

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

	bool LTE_CalculateMSTxLossByChannel(int iBand, unsigned int iChannel, double dLoss_Low, double dLoss_Mid, double dLoss_High, double& dLoss)
	{
		dLoss = 0.0 ;
		unsigned int iLow, iMid, iHigh ;
		unsigned int iNULL = 0 ;
		if (! LTE_GetBandFrequency(iBand, iNULL, iNULL, iLow, iHigh, iNULL, iNULL, iNULL, iNULL) )
			return false ;

		iMid  = (int)( (iHigh + iLow + 1) / 2 ) ;


		// Determine Loss
		if (! (iLow <= iChannel && iChannel <= iHigh))
			return false;

		if (iChannel == iLow) {
			dLoss = dLoss_Low ;
			return true ;
		}
		else if (iChannel == iMid) {
			dLoss = dLoss_Mid ;
			return true ;
		}
		else if (iChannel == iHigh) {
			dLoss = dLoss_High ;
			return true ;
		}
		else
		{
			if (iLow < iChannel && iChannel < iMid)
				dLoss = (dLoss_Mid - dLoss_Low) * (iChannel - iLow) / (iMid - iLow) + dLoss_Low ;
			else
				dLoss = (dLoss_High - dLoss_Mid) * (iChannel - iMid) / (iHigh - iMid) + dLoss_Mid ;
			return true ;
		}

		return false ;
	}

	bool LTE_CalculateBSTxLossByChannel(int iBand, unsigned int iChannel, double dLoss_Low, double dLoss_Mid, double dLoss_High, double& dLoss)
	{
		dLoss = 0.0 ;
		unsigned int iLow, iMid, iHigh ;
		unsigned int iNULL = 0 ;
		if (! LTE_GetBandFrequency(iBand, iLow, iHigh, iNULL, iNULL, iNULL, iNULL, iNULL, iNULL) )
			return false ;

		iMid  = (int)( (iHigh + iLow + 1) / 2 ) ;


		// Determine Loss
		if (! (iLow <= iChannel && iChannel <= iHigh))
			return false;

		if (iChannel == iLow) {
			dLoss = dLoss_Low ;
			return true ;
		}
		else if (iChannel == iMid) {
			dLoss = dLoss_Mid ;
			return true ;
		}
		else if (iChannel == iHigh) {
			dLoss = dLoss_High ;
			return true ;
		}
		else
		{
			if (iLow < iChannel && iChannel < iMid)
				dLoss = (dLoss_Mid - dLoss_Low) * (iChannel - iLow) / (iMid - iLow) + dLoss_Low ;
			else
				dLoss = (dLoss_High - dLoss_Mid) * (iChannel - iMid) / (iHigh - iMid) + dLoss_Mid ;
			return true ;
		}

		return false ;
	}

	bool LTE_GetBandForCMW(int iBand, std::string& strBand)
	{
		strBand.clear();

		if (iBand == 1 || iBand == 2100)
			strBand = "OB1" ;
		else if (iBand == 2 || iBand == 1900)
			strBand = "OB2" ;
		else if (iBand == 3 || iBand == 1800)
			strBand = "OB3" ;
		else if (iBand == 4 || iBand == 1700)
			strBand = "OB4" ;
		else if (iBand == 7 || iBand == 2600)
			strBand = "OB7" ;
		else if (iBand == 8 || iBand == 900)
			strBand = "OB8" ;
		else if (iBand == 12)
			strBand = "OB12" ;
		else if (iBand == 13)
			strBand = "OB13" ;
		else if (iBand == 17)
			strBand = "OB17" ;
		else if (iBand == 20)
			strBand = "OB20" ;
		else
			return false;

		return true;
	}
}
