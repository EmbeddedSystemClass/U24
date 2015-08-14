#ifndef _GSM_UTIL_H_
#define _GSM_UTIL_H_


#include <SetupAPI.h>

#include <string>
#include <vector>
#include <map>
#include <cctype>

#include "Qualcomm/inc/QLib_Defines.h"
#include "Utils/StringUtil.h"

// CMU200 define
#define MAP_COMMAND					"SYSTem:REMote:ADDRess:SEC %d,\"%s\""

#define _RF_NSIG					1
#define _GSM900MS_NSIG				2
#define _GSM900MS_SIG				3
#define _GSM850MS_NSIG				4
#define _GSM850MS_SIG				5
#define _GSM1800MS_NSIG				6
#define _GSM1800MS_SIG				7
#define _GSM1900MS_NSIG				8
#define _GSM1900MS_SIG				9
#define _CMUBT						10
#define _CMUBT_SIG					11
#define _AUDIO_NSIG					12
#define _WCDMA19UEFDD_SIG			22
#define _WCDMA19UEFDD_NSIG			23
#define _CDMA2KCellMS_NSig			25
#define _CDMA2KCellMS_Sig			26
#define _CDMA2KPCSMS_NSig			27
#define _CDMA2KPCSMS_Sig			28


#define IDSTR_RF_NSIG				"RF_NSig"
#define IDSTR_GSM900MS_NSIG			"GSM900MS_NSig"
#define IDSTR_GSM900MS_SIG			"GSM900MS_Sig"
#define IDSTR_GSM850MS_NSIG			"GSM850MS_NSig"
#define IDSTR_GSM850MS_SIG			"GSM850MS_Sig"
#define IDSTR_GSM1800MS_NSIG		"GSM1800MS_NSig"
#define IDSTR_GSM1800MS_SIG			"GSM1800MS_Sig"
#define IDSTR_GSM1900MS_NSIG		"GSM1900MS_NSig"
#define IDSTR_GSM1900MS_SIG			"GSM1900MS_Sig"
#define IDSTR_AUDIO_NSIG			"AUDIO_NSig"
#define IDSTR_BT_SIG				"BLUETOOTH_Sig"
#define IDSTR_WCDMA19UEFDD_SIG		"WCDMA19UEFDD_Sig"
#define IDSTR_WCDMA19UEFDD_NSIG		"WCDMA19UEFDD_NSig"
#define IDSTR_CDMA2KCellMS_NSig		"CDMA2KCellMS_NSig"
#define IDSTR_CDMA2KCellMS_Sig		"CDMA2KCellMS_Sig"
#define IDSTR_CDMA2KPCSMS_NSig		"CDMA2KPCSMS_NSig"
#define IDSTR_CDMA2KPCSMS_Sig		"CDMA2KPCSMS_Sig"

namespace GSMUtility
{
	// GSM functions
	bool GSM_GetPhoneModeByBand(int iBand, short& nMode);
	bool GSM_TrafficChannelToULFrequency(int iBand, int iChannel, unsigned int& uiULFreq_Hz);
	bool GSM_TrafficChannelToDLFrequency(int iBand, int iChannel, unsigned int& uiDLFreq_Hz);
	bool GSM_GetRangeForCableLoss(int iBand, int iChannel, std::string& strRange);
	bool GSM_CalculateLossByChannel(int iBand, int iChannel, double dLoss_Low, double dLoss_Mid, double dLoss_High, double& dLoss);
	bool GSM_GetMaxPowerByPCL (int iBand, int iPCL, int& powerDbm);
	int  GSM_GetLargestPowerPCL (const int iBand, std::vector<int> vPCL);
	int  GSM_GetCMUMode (int iBand, bool bSig);

	// EDGE functions
	bool EDGE_GetMaxPowerByGamma (int iBand, int iGamma, int& powerDbm);
	int  EDGE_GetActiveSlot (const std::string& vSrc, std::vector<int> &vActive, int *pSlot1 = NULL, int* pSlot2 = NULL);
	int  EDGE_GetGammaByPCL (const int iBand, const int iPCL);
}

#endif // End of #ifndef _GSM_UTIL_H_

