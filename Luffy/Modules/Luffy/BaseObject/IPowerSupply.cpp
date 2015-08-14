#include "stdafx.h"
#include "IPowerSupply.h"
#include "ITestItem.h"

GP_IMPLEMENT_DYNAMIC(IPowerSupply)


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

	if (GPIB_Query(std::string("MEAS:CURR?"), strResponse))
	{
		dCurrent = atof(strResponse.c_str());
		bRes = true;
	}
	else
		dCurrent = 0.0;

	return bRes;
}

bool IPowerSupply::QueryVoltageSetting(double& dVoltage)
{
    bool bRes = false;
    std::string strResponse;

    if (GPIB_Query(std::string("VOLTage?"), strResponse))
    {
        dVoltage = atof(strResponse.c_str());
        bRes = true;
    }
    else
        dVoltage = 0.0 ;

    return bRes;
}

bool IPowerSupply::QueryCurrentSetting(double& dCurrent)
{
    bool bRes = false;
    std::string strResponse;

    if (GPIB_Query(std::string("CURRent?"), strResponse))
    {
        dCurrent = atof(strResponse.c_str());
        bRes = true;
    }
    else
        dCurrent = 0.0 ;

    return bRes;
}

bool IPowerSupply::LoadPowerSupplySetting(pugi::xml_document *pXMLTestItem)
{
	pugi::xml_node myParm_node;

	// CGSMTxCal Para
	myParm_node = pXMLTestItem->child("Configuration").child("PowerSupply").child("Settings");

	//Voltage_Step
	m_dStepVoltage = myParm_node.child("Voltage_Step").text().as_double(ERROR_DOUBLE);
	//CHECK_PARM("Voltage_Step", m_dStepVoltage , ERROR_DOUBLE);

	//Voltage_Target
	m_dFinalVoltage = myParm_node.child("Voltage_Target").text().as_double(ERROR_DOUBLE);
	//CHECK_PARM("Voltage_Target", m_dFinalVoltage , ERROR_DOUBLE);

	//Current
	m_dCurrentLimit = myParm_node.child("Current").text().as_double(ERROR_DOUBLE);
	//CHECK_PARM("Current", m_dCurrentLimit , ERROR_DOUBLE);

	//WaitAfterOnOff
	m_iDelay = myParm_node.child("WaitAfterOnOff").text().as_int(ERROR_INT);
	//CHECK_PARM("WaitAfterOnOff", m_iDelay , ERROR_INT);

	return true;
}

bool IPowerSupply::SetPowerSupplyOn()
{

	if (! IsAvaliable())
	{
		return false;
	}

	if (! SetVoltage(m_dStepVoltage))
	{
		return false;
	}

	if (! SetCurrentLimit(m_dCurrentLimit))
	{
		return false;
	}

	if (! SetCurrRangeMax())
	{
		return false;
	}

	if (! SetPSOnOff(true))
	{
		return false;
	}

	Sleep(500) ;
	if (! SetVoltage(m_dFinalVoltage))
	{
		return false;
	}

	return true;
}

bool IPowerSupply::SetPowerSupplyOff()
{
	if (! IsAvaliable())
	{
		return false;
	}

	if (! SetVoltage(m_dStepVoltage))
	{
		return false;
	}

	if (! SetCurrentLimit(0))
	{
		return false;
	}

	if (! SetCurrRangeMax())
	{
		return false;
	}

	if (! SetPSOnOff(false))
	{
		return false;
	}

	return true;
}