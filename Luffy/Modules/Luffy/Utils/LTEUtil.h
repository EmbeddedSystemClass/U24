#ifndef _LTE_UTIL_H_
#define _LTE_UTIL_H_


#include <SetupAPI.h>

#include <string>
#include <vector>
#include <map>
#include <cctype>

#include "Qualcomm/inc/QLib_Defines.h"

namespace LTEUtility
{
	// LTE functions
	bool LTE_GetPhoneRFModeByBand(int iBand, short& nRFMode) ;
	bool LTE_GetPhoneNVModeByBand(int iBand, short& nNVMode) ;
	bool LTE_GetBSTxChannelByMSTxChannel(int iBand, unsigned int iMSTxChannel, unsigned int& iBSTxChannel) ;
	bool LTE_GetMSTxChannelByBSTxChannel(int iBand, unsigned int iBSTxChannel, unsigned int& iMSTxChannel) ;
	bool LTE_GetBandFrequency(int iBand,
		unsigned int& iBSTxChannelLower, unsigned int& iBSTxChannelUpper,
		unsigned int& iMSTxChannelLower, unsigned int& iMSTxChannelUpper,
		unsigned int& iBSTxFreqLower,    unsigned int& iBSTxFreqUpper,
		unsigned int& iMSTxFreqLower,    unsigned int& iMSTxFreqUpper) ;
	bool LTE_MSTxChannelToFrequency(int iBand, unsigned int iMSTxChannel, unsigned int& uiFreq_Hz) ;
	bool LTE_BSTxChannelToFrequency(int iBand, unsigned int iBSTxChannel, unsigned int& uiFreq_Hz, bool bIsCal = false) ;
	bool LTE_GetBandNameForCableLoss(int iBand, std::string& strBandName) ;
	bool LTE_GetRangeForMSTxCableLoss(int iBand, unsigned int iMSTxChannel, std::string& strRange) ;
	bool LTE_GetRangeForBSTxCableLoss(int iBand, unsigned int iBSTxChannel, std::string& strRange) ;
	bool LTE_CalculateMSTxLossByChannel(int iBand, unsigned int iChannel, double dLoss_Low, double dLoss_Mid, double dLoss_High, double& dLoss) ;
	bool LTE_CalculateBSTxLossByChannel(int iBand, unsigned int iChannel, double dLoss_Low, double dLoss_Mid, double dLoss_High, double& dLoss) ;
	bool LTE_GetBandForCMW(int iBand, std::string& strBand);
}

#endif // End of #ifndef _LTE_UTIL_H_

