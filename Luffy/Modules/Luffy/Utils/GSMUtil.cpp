#include "stdafx.h"
#include "GSMUtil.h"

using namespace StringUtility;

namespace GSMUtility
{
	// --------------------- GSM functions ---------------------

	bool GSM_GetPhoneModeByBand(int iBand, short& nMode) // Using QLib
	{
		nMode = 0;

		if (iBand == 850)
			nMode = PHONE_MODE_GSM_850;
		else if (iBand == 900)
			nMode = PHONE_MODE_GSM_900;
		else if (iBand == 1800)
			nMode = PHONE_MODE_GSM_1800;
		else if (iBand == 1900)
			nMode = PHONE_MODE_GSM_1900;
		else
			return false;

		return true;
	}

	bool GSM_TrafficChannelToULFrequency(int iBand, int iChannel, unsigned int& uiULFreq_Hz)
	{
		bool bRes = true;
		uiULFreq_Hz = 0;

		if (iBand == 850)
		{
			if (iChannel >= 128 && iChannel <= 251)
				uiULFreq_Hz = 824200000 + 200000 * (iChannel - 128);
			else
				bRes = false;
		}
		else if (iBand == 900)
		{
			if (iChannel >= 975 && iChannel <= 1023)
				uiULFreq_Hz = 876200000 + 200000 * (iChannel - 955);
			else if (iChannel >= 0 && iChannel <= 124)
				uiULFreq_Hz = 890000000 + 200000 * iChannel;
			else
				bRes = false;
		}
		else if (iBand == 1800)
		{
			if (iChannel >= 512 && iChannel <= 885)
				uiULFreq_Hz = 1710200000 + 200000 * (iChannel - 512);
			else
				bRes = false;
		}
		else if (iBand == 1900)
		{
			if (iChannel >= 512 && iChannel <= 810)
				uiULFreq_Hz = 1850200000 + 200000 * (iChannel - 512);
			else
				bRes = false;
		}
		else
			bRes = false;

		return bRes;
	}

	bool GSM_TrafficChannelToDLFrequency(int iBand, int iChannel, unsigned int& uiDLFreq_Hz)
	{
		bool ret;
		uiDLFreq_Hz = 0;

		if (ret = GSM_TrafficChannelToULFrequency(iBand, iChannel, uiDLFreq_Hz))
		{
			switch (iBand)
			{
			case 850:
			case 900:
				uiDLFreq_Hz += 45000000;
				break;
			case 1800:
				uiDLFreq_Hz += 95000000;
				break;
			case 1900:
				uiDLFreq_Hz += 80000000;
				break;
			default:
				ret = false;
			}
		}

		return ret;
	}

	bool GSM_GetRangeForCableLoss(int iBand, int iChannel, std::string& strRange)
	{
		int iLow, iMid, iHigh;
		strRange.clear();

		if (iBand == 850)
		{
			iLow = 128;
			iMid = 178;
			iHigh = 251;
		}
		else if (iBand == 900)
		{
			iLow = 975;
			iMid = 021;
			iHigh = 124;
		}
		else if (iBand == 1800)
		{
			iLow = 512;
			iMid = 662;
			iHigh = 885;
		}
		else if (iBand == 1900)
		{
			iLow = 512;
			iMid = 632;
			iHigh = 810;
		}
		else
			return false;

		int iDelta_Low = abs(iChannel - iLow);
		int iDelta_Mid = abs(iChannel - iMid);
		int iDelta_High = abs(iChannel - iHigh);

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

		if (strRange.empty())
			strRange = "mid";

		return true;
	}

	bool GSM_CalculateLossByChannel(int iBand, int iChannel, double dLoss_Low, double dLoss_Mid, double dLoss_High, double& dLoss)
	{
		dLoss = 0.0;

		int iLow, iMid, iHigh;

		if (iBand == 850)
		{
			iLow = 128;
			iMid = 190;
			iHigh = 251;

			if (! (iLow <= iChannel && iChannel <= iHigh))
				return false;
		}
		else if (iBand == 900)
		{
			iLow = 0;
			iMid = 86;
			iHigh = 173;

			if (975 <= iChannel && iChannel <= 1023)
				iChannel -= 975;
			else if (0 <= iChannel && iChannel <= 124)
				iChannel += 49;
			else
				return false;
		}
		else if (iBand == 1800)
		{
			iLow = 512;
			iMid = 698;
			iHigh = 885;

			if (! (iLow <= iChannel && iChannel <= iHigh))
				return false;
		}
		else if (iBand == 1900)
		{
			iLow = 512;
			iMid = 661;
			iHigh = 810;

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

	bool GSM_GetMaxPowerByPCL (int iBand, int iPCL, int& powerDbm)
	{
		if (850 == iBand || 900 == iBand)
		{
			if (iPCL >= 2 && iPCL <= 19)
				powerDbm = 39 - 2*(iPCL-2);
			else
				return false;
		}
		else if (1800 == iBand)
		{
			if (iPCL >= 29 && iPCL <= 31)
				powerDbm = 36 - 2*(iPCL-29);
			else if (iPCL >= 0 && iPCL <= 15)
				powerDbm = 30 - 2*iPCL;
			else
				return false;
		}
		else if (1900 == iBand)
		{
			if (30 == iPCL)
				powerDbm = 33;
			else if (31 == iPCL)
				powerDbm = 32;
			else if (iPCL >= 0 && iPCL <= 15)
				powerDbm = 30 - 2*iPCL;
			else
				return false;
		}
		else
		{
			return false;
		}

		return true;
	}

	int  GSM_GetLargestPowerPCL (const int iBand, std::vector<int> vPCL)
	{
		int maxPCL=-1;

		if (850 == iBand || 900 == iBand)
		{
			// 2 to 19 (39dBm to 5 dBm)
			maxPCL = 20;
			for (unsigned int i=0; i<vPCL.size(); ++i)
			{
				if (vPCL[i] < maxPCL)
					maxPCL = vPCL[i];
			}
		}
		else if (1800 == iBand)
		{
			//29,30,31,0,1, ..., 15
			maxPCL = 16;
			for (unsigned int i=0; i<vPCL.size(); ++i)
			{
				if (vPCL[i] >= 29 && vPCL[i] <= 31)
				{
					if (maxPCL >= 29)
					{
						if (vPCL[i] < maxPCL)
							maxPCL = vPCL[i];
					}
					else
					{
						maxPCL = vPCL[i];
					}
				}
				else
				{
					if (maxPCL < 29)
					{
						if (vPCL[i] < maxPCL)
							maxPCL = vPCL[i];
					}
				}
			}
		}
		else if (1900 == iBand)
		{
			//30,31,0,1, ..., 15

			maxPCL = 16;
			for (unsigned int i=0; i<vPCL.size(); ++i)
			{
				if (vPCL[i] >= 30 && vPCL[i] <= 31)
				{
					if (maxPCL >= 30)
					{
						if (vPCL[i] < maxPCL)
							maxPCL = vPCL[i];
					}
					else
					{
						maxPCL = vPCL[i];
					}
				}
				else
				{
					if (maxPCL < 30)
					{
						if (vPCL[i] < maxPCL)
							maxPCL = vPCL[i];
					}
				}
			}

		}

		return maxPCL;
	}

	int GSM_GetCMUMode (int iBand, bool bSig)
	{
		if (bSig)
		{
			switch (iBand)
			{
			case 850 : return _GSM850MS_SIG;
			case 900 : return _GSM900MS_SIG;
			case 1800: return _GSM1800MS_SIG;
			case 1900: return _GSM1900MS_SIG;
			}
		}
		else
		{
			switch (iBand)
			{
			case 850 : return _GSM850MS_NSIG;
			case 900 : return _GSM900MS_NSIG;
			case 1800: return _GSM1800MS_NSIG;
			case 1900: return _GSM1900MS_NSIG;
			}
		}

		return -1;
	}

	bool EDGE_GetMaxPowerByGamma (int iBand, int iGamma, int& powerDbm)
	{
		if (iGamma < 0 || iGamma > 31)
			return false;

		if (850==iBand || 900==iBand)
		{
			powerDbm = 39 - (2 * iGamma);
		}
		else if (1800==iBand || 1900==iBand)
		{
			powerDbm = 36 - (2 * iGamma);
		}
		return true;
	}

	int  EDGE_GetGammaByPCL (const int iBand, const int iPCL)
	{
		int gamma = 8;
		int power;
		GSM_GetMaxPowerByPCL (iBand, iPCL, power);

		if (850==iBand || 900==iBand)
		{
			gamma = (39 - power) / 2;
		}
		else if (1800==iBand || 1900==iBand)
		{
			gamma = (36 - power) / 2;
		}

		return gamma;
	}

	int EDGE_GetActiveSlot (const std::string& vSrc, std::vector<int> &vActive, int *pSlot1, int* pSlot2)
	{
		// init val
		int retVal=0;
		vActive.clear();
		if (pSlot2)
			*pSlot2 = -1;
		if (pSlot1)
			*pSlot1 = -1;


		std::vector<std::string> vTemp;
		STDStringTokenizer (vSrc, ",", vTemp);

		int total = vTemp.size();
		for (int i=0; i < total; ++i)
		{
			if (vTemp[i] == "ON")
			{
				++retVal;
				vActive.push_back (i);
			}
		}

		if (retVal >= 2)
		{
			if (pSlot2)
				*pSlot2 = vActive[1];
		}

		if (retVal >= 1)
		{
			if (pSlot1)
				*pSlot1 = vActive[0];
		}

		return retVal;
	}
}
