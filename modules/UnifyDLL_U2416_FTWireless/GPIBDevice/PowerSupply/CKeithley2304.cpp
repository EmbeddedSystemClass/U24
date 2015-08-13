/*********************************************************************************
 * COPYRIGHT Qisda WGC18 2011
 *
 * Description: .cpp file of Keithley2304 PowerSupply object.
 * FileName   : CKeithley2304.cpp
 * Version    : $Revision:   1.1  $
 * Author     : Gilpin.Xi
 * Time       : 2011/03/23
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/03/23      Gilpin.Xi             Create.
 ********************************************************************************/

#include "stdafx.h"
#include "CKeithley2304.h"

RF_IMPLEMENT_DYNCREATE(CKeithley2304)
 
CKeithley2304::CKeithley2304(void)
{
 
}
 
CKeithley2304::~CKeithley2304(void)
{
}
 
 
bool CKeithley2304::SetCurrentLimit(double dCurrent)
{
	char szCmdText[256]={0};
	sprintf_s(szCmdText, 255, ":source:current:limit %2.3f", dCurrent);
	std::string strCommand(szCmdText);

	return GPIB_PS_Write(strCommand);
}
