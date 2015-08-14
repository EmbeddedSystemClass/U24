#ifndef _WCDMA_UTIL_H_
#define _WCDMA_UTIL_H_


#include <SetupAPI.h>

#include <string>
#include <vector>
#include <map>
#include <cctype>

#include "Qualcomm/inc/QLib_Defines.h"

namespace WCDMAUtility
{
	// WCDMA functions
	bool WCDMA_GetPhoneRFModeByBand(int iBand, short& nRFMode) ;
	bool WCDMA_GetPhoneNVModeByBand(int iBand, short& nNVMode) ;
	bool WCDMA_GetBSTxChannelByMSTxChannel(int iBand, int iMSTxChannel, int& iBSTxChannel);
	bool WCDMA_GetMSTxChannelByBSTxChannel(int iBand, int iBSTxChannel, int& iMSTxChannel);
	bool WCDMA_ULChannelToFrequency(int iBand, int iMSTxChannel, unsigned int& uiFreq_Hz);
	bool WCDMA_DLChannelToFrequency(int iBand, int iBSTxChannel, unsigned int& uiFreq_Hz);
	bool WCDMA_GetBandNameForCableLoss(int iBand, std::string& strBandName);
	bool WCDMA_GetRangeForMSTxCableLoss(int iBand, int iMSTxChannel, std::string& strRange);
	bool WCDMA_GetRangeForBSTxCableLoss(int iBand, int iBSTxChannel, std::string& strRange);
	bool WCDMA_CalculateLossByChannel(int iBand, int iChannel, double dLoss_Low, double dLoss_Mid, double dLoss_High, double& dLoss);
	bool WCDMA_GetBandForCMW(int iBand, std::string& strBand);
}

#endif // End of #ifndef _CDMA_UTIL_H_

