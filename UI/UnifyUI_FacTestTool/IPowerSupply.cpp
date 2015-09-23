#include "stdafx.h"
#include "IPowerSupply.h"

bool IPowerSupply::InitPowerSupply( int iGPIBInterfaceID, int iGPIBCardAddress )
{
	m_iPowerSupplyDD = ibdev(iGPIBInterfaceID, iGPIBCardAddress, 0, T1s, 1, 0);
	if (ibsta & ERR)
		return false;

	return true;
}


bool IPowerSupply::SetCurrentLimit(double dCurrent)
{
	char szVoltage[12];
	sprintf_s(szVoltage, 11, "CURR %2.3f", dCurrent);
	std::string strCommand(szVoltage);

	return GPIB_PS_Write(strCommand);
}

bool IPowerSupply::SetVoltage(double dVoltage)
{
	char szVoltage[12];
	sprintf_s(szVoltage, 11, "VOLT %2.3f", dVoltage);
	std::string strCommand(szVoltage);

	return GPIB_PS_Write(strCommand);
}

bool IPowerSupply::SetPSOnOff(bool bIsOn)
{
	if (bIsOn)
		return GPIB_PS_Write(std::string("OUTP ON"));
	else
		return GPIB_PS_Write(std::string("OUTP OFF"));
}

bool IPowerSupply::SetCurrRangeMax()
{
	return GPIB_PS_Write(std::string("SENSe:CURRent:RANGe MAX"));
}

bool IPowerSupply::SetCurrRangeMin()
{
	return GPIB_PS_Write(std::string("SENSe:CURRent:RANGe MIN"));
}

bool IPowerSupply::GetCurrent(double &dCurrent)
{
	bool bRes = false;
	std::string strResponse;

	if (GPIB_PS_Querry(std::string("MEAS:CURR?"), strResponse))
	{
		dCurrent = atof(strResponse.c_str());
		bRes = true;
	}
	else
		dCurrent = 0.0;

	return bRes;
}

bool IPowerSupply::GPIB_PS_Querry(std::string strCommand, std::string& strResponse)
{
	char szCmd[4096];
	memset(szCmd, 0, 4096);
	char szRead[524288];
	memset(szRead, 0, 524288);

	if (sprintf_s(szCmd, 4095, "%s", strCommand.c_str()) == -1)
		return false;

	if (m_iPowerSupplyDD == -1)
		return false;

	ibwrt(m_iPowerSupplyDD, (void*)szCmd, strlen(szCmd));
	if (ibsta & ERR)
		return false;

	int iRetry = 100;
	do
	{
		ibrd(m_iPowerSupplyDD, (void*)szRead, 524287);
		if (ibsta & END)
			break;

		--iRetry;
		Sleep(100);
	} while (iRetry > 0);

	if (ibsta & ERR)
		return false;

	szRead[ibcntl] = '\0';
	strResponse = szRead;
	return true;
}

bool IPowerSupply::GPIB_PS_Write(std::string strCommand)
{
	char szCmd[4096];
	memset(szCmd, 0, 4096);

	if (sprintf_s(szCmd, 4095, "%s", strCommand.c_str()) == -1)
		return false;

	if (m_iPowerSupplyDD == -1)
		return false;

	ibwrt(m_iPowerSupplyDD, (void*)szCmd, strlen(szCmd));
	if (ibsta & ERR)
		return false;

	return true;
}

IPowerSupply::IPowerSupply()
{
	m_iGPIBCardAddress = -1;
	m_iGPIBInterfaceID = -1;
	m_iPowerSupplyDD = -1;
}

bool IPowerSupply::IsInit()
{
	if (m_iPowerSupplyDD != -1)
	{
		return true;
	}
	return false;
}
