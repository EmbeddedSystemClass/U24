#ifndef _INSPECTION_UTIL_H_
#define _INSPECTION_UTIL_H_


#include <SetupAPI.h>

#include <string>
#include <vector>
#include <map>
#include <cctype>

namespace InspectionUtility
{
	//InRange Function
	bool InRange(int iLowerBound, int iUpperBound, int iMeasured);
	bool InRange(double dLowerBound, double dUpperBound, double dMeasured);

	bool QueryTestItemIsEnable(const std::vector<std::string>& vTestList, int iTestItemID);

	bool CMW_CheckReliability(int iReliabilityIndicator, std::string& strErrMsg);
}


#endif // End of #ifndef _INSPECTION_UTIL_H_

