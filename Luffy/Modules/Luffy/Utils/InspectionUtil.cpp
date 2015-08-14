#include "stdafx.h"
#include "InspectionUtil.h"

namespace InspectionUtility
{
	// ------------------- InRange functions --------------------
	bool InRange(int iLowerBound, int iUpperBound, int iMeasured)
	{
		if (iMeasured <= iUpperBound && iMeasured >= iLowerBound)
			return true;

		return false;
	}

	bool InRange(double dLowerBound, double dUpperBound, double dMeasured)
	{
		if (dMeasured <= dUpperBound && dMeasured >= dLowerBound)
			return true;

		return false;
	}

	bool QueryTestItemIsEnable(const std::vector<std::string>& vTestList, int iTestItemID)
	{
		if (0 > iTestItemID)
			return false;

		if ((signed)vTestList.size() <= iTestItemID)
			return false;

		if (strstr(vTestList[iTestItemID].c_str(), "1") == NULL)
			return false;

		return true;
	}

	bool CMW_CheckReliability(int iReliabilityIndicator, std::string& strErrMsg)
	{
		strErrMsg.clear();

		switch (iReliabilityIndicator)
		{
		case 0:
			return true;
		case 1:
			strErrMsg = "Measurement timeout";
			return false;
		case 3:
			strErrMsg = "Over driven";
			return true;
		case 4:
			strErrMsg = "Under driven";
			return true;
		case 6:
			strErrMsg = "Trigger timeout";
			return false;
		case 7:
			strErrMsg = "Acquisition error";
			return false;
		case 8:
			strErrMsg = "Sync error";
			return false;
		case 15:
			strErrMsg = "Reference frequency error";
			return false;
		case 16:
			strErrMsg = "RF not available";
			return false;
		case 17:
			strErrMsg = "RF level not settled";
			return false;
		case 18:
			strErrMsg = "RF frequency not settled";
			return false;
		case 19:
			strErrMsg = "Call not established";
			return false;
		case 20:
			strErrMsg = "Call type not usable";
			return false;
		case 21:
			strErrMsg = "Call lost";
			return false;
		case 101:
			strErrMsg = "Firmware error";
			return false;
		case 102:
			strErrMsg = "Unidentified error";
			return false;
		case 103:
			strErrMsg = "Parameter error";
			return false;
		default:
			strErrMsg = "Unknown error";
			return false;
		}
	}


} // namespace Inspection
