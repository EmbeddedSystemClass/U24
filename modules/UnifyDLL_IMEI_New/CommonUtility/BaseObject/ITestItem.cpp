#include "stdafx.h"
#include "ITestItem.h"


RF_IMPLEMENT_DYNAMIC(ITestItem)


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
