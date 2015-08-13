#include "stdafx.h"
#include "IPowerSupply.h"


RF_IMPLEMENT_DYNAMIC(IPowerSupply)


bool IPowerSupply::SetCurrentLimit(double dCurrent)
{
	char szVoltage[12];
	sprintf_s(szVoltage, 11, "CURR %2.3f", dCurrent);
	std::string strCommand(szVoltage);

	return GPIB_Write(strCommand);
}

bool IPowerSupply::SetVoltage(double dVoltage)
{
	char szVoltage[12];
	sprintf_s(szVoltage, 11, "VOLT %2.3f", dVoltage);
	std::string strCommand(szVoltage);

	return GPIB_Write(strCommand);
}

bool IPowerSupply::SetPSOnOff(bool bIsOn)
{
	if (bIsOn)
		return GPIB_Write(std::string("OUTP ON"));
	else
		return GPIB_Write(std::string("OUTP OFF"));
}

bool IPowerSupply::SetCurrRangeMax()
{
	return GPIB_Write(std::string("SENSe:CURRent:RANGe MAX"));
}

bool IPowerSupply::SetCurrRangeMin()
{
	return GPIB_Write(std::string("SENSe:CURRent:RANGe MIN"));
}

bool IPowerSupply::GetCurrent(double &dCurrent)
{
	bool bRes = false;
	std::string strResponse;

	if (GPIB_Querry(std::string("MEAS:CURR?"), strResponse))
	{
		dCurrent = atof(strResponse.c_str());
		bRes = true;
	}
	else
		dCurrent = 0.0;

	return bRes;
}
