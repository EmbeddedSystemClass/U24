#ifndef _CDMA_UTIL_H_
#define _CDMA_UTIL_H_


#include <SetupAPI.h>

#include <string>
#include <vector>
#include <map>
#include <cctype>

#include "Qualcomm/inc/QLib_Defines.h"

namespace CDMAUtility
{
	// CDMA functions
	bool CDMA_GetPhoneRFModeByBand(int iBand, short& nRFMode);
	bool CDMA_GetPhoneNVModeByBand(int iBand, short& nNVMode);
	bool CDMA_ChannelToFrequency(int iBand, int iChannel, int iULorDL, unsigned int& uiFreq_Hz);
	bool CDMA_GetBandNameForCableLoss(int iBand, std::string& strBandName, bool bIsEVDO = false);
	bool CDMA_GetRangeForMSTxCableLoss(int iBand, int iMSTxChannel, std::string& strRange);
	bool CDMA_GetRangeForBSTxCableLoss(int iBand, int iBSTxChannel, std::string& strRange);
	bool CDMA_CalculateLossByChannel(int iBand, int iChannel, double dLoss_Low, double dLoss_Mid, double dLoss_High, double& dLoss);
	bool CDMA_GetBandForCMW(int iBand, std::string& strBand);
}

#endif // End of #ifndef _CDMA_UTIL_H_

