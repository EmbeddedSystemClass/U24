/*********************************************************************************
 * COPYRIGHT Qisda WGC18 2011
 *
 * Description: .cpp file of CFuncGPS_CMU object.
 * FileName   : CFuncGPS_CMU.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Gilpin.Xi
 * Time       : 2011/03/31
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/03/31      Gilpin.Xi             Create.
 ********************************************************************************/

#include "stdafx.h"
#include "CFuncGPS_CMU.h"
#include "CCMU200.h"

CFuncGPS_CMU::CFuncGPS_CMU()
{
 
}
 
CFuncGPS_CMU::CFuncGPS_CMU(ITesterDevice* pDevice)
	:IFuncGPS_Dev(pDevice)
{
	m_pTesterDevice = pDevice;
}
 
CFuncGPS_CMU::~CFuncGPS_CMU()
{

}

bool CFuncGPS_CMU::GPSTestSetPowerLevel(double iLevel)
{
	char cCmd[100]="";
	String szReadData;

	sprintf(cCmd, "%d;SOURce:RFGenerator:TX:LEVel %4.1f dBm; *OPC?", m_iCMUMode, iLevel);
	std::string strCommand(cCmd);	
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
	return true;
}

bool CFuncGPS_CMU::GPSTestSetChannel(double iChannel)
{
	char cCmd[100]="";
	String szReadData;

	sprintf(cCmd, "%d;SOURce:RFGenerator:TX:FREQuency %6.2f MHz; *OPC?", m_iCMUMode, iChannel);
	std::string strCommand(cCmd);

	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;

	return true;

}

bool CFuncGPS_CMU::GPSTestSetPwrOn(void)
{
	char cCmd[100]="";
	String szReadData;
 	std::string strCommand;

	sprintf(cCmd, "%d;SOURce:RFGenerator:MODulation OFF;*OPC?", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;

	sprintf(cCmd, "%d;INITiate:RFGenerator:TX; *OPC?", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;

	return true;

}

bool CFuncGPS_CMU::GPSTestSetPwrOff(void)
{
	char cCmd[100]="";
	String szReadData;

	sprintf(cCmd, "%d;ABORt:RFGenerator:TX; *OPC?", m_iCMUMode);
	std::string strCommand(cCmd);

	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;

	return true;

}
