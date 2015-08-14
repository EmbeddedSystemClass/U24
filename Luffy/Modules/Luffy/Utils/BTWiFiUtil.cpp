#include "stdafx.h"
#include "BTWiFiUtil.h"

namespace BTWiFiUtility
{
	double WiFi_ChannelToFrequency(int iChannel)
	{
		double dFrequency = 2412.0 + (iChannel - 1) * 5.0;

		return dFrequency;
	}

	double WiFi_TxPowerConvertion(int iPwrCntlMode, const char* szPower)
	{
		double dPwr;

		if (iPwrCntlMode) // Close Loop
			dPwr = atof(szPower);
		else
			dPwr = 65536 * atoi(szPower);

		return dPwr;
	}

	std::string WiFi_TxPowerConvertion(int iPwrCntlMode, std::string strPower)
	{
		if (iPwrCntlMode) // Close Loop
			return strPower;
		else
		{
			int iRFGain = 65536 * atoi(strPower.c_str());
			char chRFGain[32];
			memset(chRFGain, 0, sizeof chRFGain);
			sprintf_s(chRFGain, 32, "%d", iRFGain);

			return (std::string)chRFGain;
		}
	}

} // namespace BTWiFi
