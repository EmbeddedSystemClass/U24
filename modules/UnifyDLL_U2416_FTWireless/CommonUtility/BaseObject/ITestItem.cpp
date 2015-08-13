#include "stdafx.h"
#include "ITestItem.h"


RF_IMPLEMENT_DYNAMIC(ITestItem)


double ITestItem::GetCableLoss(const std::string strRfType, const std::string strRfBand, std::string strRfRange)
{
	std::map<std::string, std::string> mapCableLoss;
	double dLoss = 0.0;

	if (strRfType == "TX")
	{
		if (g_mapTxCableLossData.find(strRfBand) != g_mapTxCableLossData.end())
		{
			mapCableLoss = g_mapTxCableLossData[strRfBand];
			if (mapCableLoss.find(strRfRange) != mapCableLoss.end())
			{
				std::string strCableLoss = mapCableLoss[strRfRange];
				dLoss = atof(strCableLoss.c_str());
			}
		}
	}
	else if (strRfType == "RX")
	{
		if (g_mapRxCableLossData.find(strRfBand) != g_mapRxCableLossData.end())
		{
			mapCableLoss = g_mapRxCableLossData[strRfBand];
			if (mapCableLoss.find(strRfRange) != mapCableLoss.end())
			{
				std::string strCableLoss = mapCableLoss[strRfRange];
				dLoss = atof(strCableLoss.c_str());
			}
		}
	}

	return dLoss;
}

bool ITestItem::InRange(int iLowerBound, int iUpperBound, int iMeasured)
{
	if (iMeasured <= iUpperBound && iMeasured >= iLowerBound)
		return true;

	return false;
}

bool ITestItem::InRange(double dLowerBound, double dUpperBound, double dMeasured)
{
	if (dMeasured <= dUpperBound && dMeasured >= dLowerBound)
		return true;

	return false;
}

bool ITestItem::Begin()
{
	return true;
}

bool ITestItem::IsConnected()
{
	return true;
}

bool ITestItem::IsConnectedMulti()
{
	return true;
}

bool ITestItem::IsMobileInFTDMode()
{
	return true;
}

bool ITestItem::IsSocketReady()
{
	return true;
}

bool ITestItem::IsSocketReadyMulti()
{
	return true;
}

bool ITestItem::PreRun(int nIndex)
{
	return true;
}

bool ITestItem::Run(int nIndex)
{
	return true;
}

bool ITestItem::PostRun(int nIndex)
{
	return true;
}

bool ITestItem::Disconnect()
{
	return true;
}

bool ITestItem::End()
{
	return true;
}
