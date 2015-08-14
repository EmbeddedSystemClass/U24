#ifndef _BTWIFI_UTIL_H_
#define _BTWIFI_UTIL_H_


#include <SetupAPI.h>

#include <string>
#include <vector>
#include <map>
#include <cctype>

namespace BTWiFiUtility
{
	//Convert Functions
	double WiFi_ChannelToFrequency(int iChannel);
	double WiFi_TxPowerConvertion(int iPwrCntlMode, const char* szPower);
	std::string WiFi_TxPowerConvertion(int iPwrCntlMode, std::string szPower);
}

#endif // End of #ifndef _BTWIFI_UTIL_H_

