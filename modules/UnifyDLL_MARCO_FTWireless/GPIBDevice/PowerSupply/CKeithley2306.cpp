/*********************************************************************************
 * COPYRIGHT Qisda WGC18 2011
 *
 * Description: .cpp file of Keithley2306 PowerSupply object.
 * FileName   : CKeithley2306.cpp
 * Version    : $Revision:   1.1  $
 * Author     : Gilpin.Xi
 * Time       : 2011/03/23
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/03/23      Gilpin.Xi             Create.
 ********************************************************************************/

#include "stdafx.h"
#include "CKeithley2306.h"
 
RF_IMPLEMENT_DYNCREATE(CKeithley2306)
 
CKeithley2306::CKeithley2306(void)
{
 
}
 
CKeithley2306::~CKeithley2306(void)
{
}

bool CKeithley2306::GetCurrent(double &dCurrent)
{
	bool bRes = false;
	std::string strResponse;

	char szCmdText[256]={0};
	sprintf_s(szCmdText, 255, "SENSe:FUNC \"CURR\"");
	std::string strCommand(szCmdText);

	if(GPIB_PS_Write(strCommand))
	{
		bRes = true;
	}
	else
	{
		return bRes;
	}

	if (GPIB_PS_Querry(std::string("READ?"), strResponse))// Query Voltage and Current
	{
		dCurrent = atof(strResponse.c_str());
		bRes = true;
	}
	else
	{
		dCurrent = 0.0;
		bRes = false;
	}

	return bRes;
}

bool CKeithley2306::SetPSOnOff(bool bIsOn)
{
	if (bIsOn)
		return GPIB_PS_Write(std::string("OUTPut ON"));
	else
		return GPIB_PS_Write(std::string("OUTPut OFF"));

}