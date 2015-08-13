/*********************************************************************************
 * COPYRIGHT Qisda WGC18 2011
 *
 * Description: .cpp file of CFuncWLAN_CMU object.
 * FileName   : CFuncWLAN_CMU.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Gilpin.Xi
 * Time       : 2011/03/23
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/03/23      Gilpin.Xi             Create.
 ********************************************************************************/

#include "stdafx.h"
#include "CFuncWLAN_CMU.h"
 
CFuncWLAN_CMU::CFuncWLAN_CMU()
{
 
}
 
CFuncWLAN_CMU::CFuncWLAN_CMU(ITesterDevice* pDevice)
	:IFuncWLAN_Dev(pDevice)
{
	m_pTesterDevice = pDevice;
 
}
 
CFuncWLAN_CMU::~CFuncWLAN_CMU()
{

}

bool CFuncWLAN_CMU::InitWLANDev(int iAVFactor)
{
	char cCmd[100]="";
	ZeroMemory(cCmd, sizeof(cCmd)/sizeof(char));
 	std::string strCommand;

	sprintf(cCmd, "%d;LEV:MODE MAN", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Write(strCommand) == false)
		return false;

	sprintf(cCmd, "%d;SENS:LEV:MAX 0.0", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Write(strCommand) == false)
		return false;

	sprintf(cCmd, "%d;SENS:LEV:REF 20.0", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Write(strCommand) == false)
		return false;
	
	sprintf(cCmd, "%d;CONF:SPEC:CONT ARR,%d", m_iCMUMode,iAVFactor);
	strCommand = cCmd;
	if (GPIB_Write(strCommand) == false)
		return false;

	sprintf(cCmd, "%d;SENS:SPEC:FREQ:SPAN 56.0 MHz", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Write(strCommand) == false)
		return false;

	sprintf(cCmd, "%d;SENS:SPEC:FREQ:BWID:RES 1.0 MHz", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Write(strCommand) == false)
		return false;

	sprintf(cCmd, "%d;SENS:SPEC:LEV:RANG 100dB", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Write(strCommand) == false)
		return false;

	sprintf(cCmd, "%d;SENS:SPEC:DET RMS", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Write(strCommand) == false)
		return false;
	
	sprintf(cCmd, "%d;LEV:ATT LNO", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Write(strCommand) == false)
		return false;
	
	return true;	
}

bool CFuncWLAN_CMU::WLANSetAnyserFreq(int iChannel)
{
	char cCmd[100]="";
	float dFreq;
	ASSERT(iChannel>=1 || iChannel<=14);
	if(iChannel==14)
		dFreq=2484;
	else
		dFreq=(float)(2412+(iChannel-1)*5);
	ZeroMemory(cCmd, sizeof(cCmd)/sizeof(char));

 	std::string strCommand;
	sprintf(cCmd, "%d;SENS:SPEC:FREQ:CENT %.1f MHz", m_iCMUMode,dFreq);
	strCommand = cCmd;
	if (GPIB_Write(strCommand) == false)
		return false;

	return true;
}

bool CFuncWLAN_CMU::WLANEvalPower(vector<float> & vtrf)
{
	StrVtr vtrstr;
	String strRes;
	std::string strCommand;
	char cCmd[100]="";
	ZeroMemory(cCmd, sizeof(cCmd)/sizeof(char));

	sprintf(cCmd, "%d;READ:ARR:SPEC:MAX?", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, strRes))
	{
		CStr::ParseString(strRes,_T(","),vtrstr);

		for(StrVtrItr itr=vtrstr.begin();itr!=vtrstr.end();itr++)
			vtrf.push_back(CStr::StrToFloat(*itr));
	}
	else
		return false;
	
	return true;
}

bool CFuncWLAN_CMU::WLANFetchPower(String & fTestPowerArray)
{
	char cCmd[100]="";
	String szReadData;
	ZeroMemory(cCmd, sizeof(cCmd));
 
	std::string strCommand;
	sprintf(cCmd, "%d;INIT:NPOW; *OPC?", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
	Sleep(300);

	string strRead = "RUN";
	while(strRead.substr(0, 3) != "RDY")
	{
		sprintf(cCmd, "%d;FETC:NPOW:STAT?", m_iCMUMode);
		strCommand = cCmd;
		if (GPIB_Querry(strCommand, szReadData))
		{
#ifdef _UNICODE
			strRead = CStr::ToAnsiString(szReadData);
#else
			strRead = szReadData;
#endif
			Sleep(100);
		}
		else
			return false;
	}
 
	sprintf(cCmd, "%d;FETC:NPOW?", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData))
	{
#ifdef _UNICODE
		strRead = CStr::ToAnsiString(szReadData);
#else
		strRead = szReadData;
#endif
	}
	else
		return false;
 
	char seps[] = ",";
	char *token = NULL;
	int j = 0;
	token = strtok( (char *)strRead.c_str(), seps );		//Avg. Power of Current evaluation period
	token = strtok( NULL, seps );	//Min. Power of Current evaluation period
	token = strtok( NULL, seps );	//Max. Power of Current evaluation period
	token = strtok( NULL, seps );	//Avg. Power ref. to the last stat. cycle
	if(strstr(token,_T("NAN")))
		token=_T("-99.0");
	fTestPowerArray = token;
//	while( token != NULL ){
//		fTestPowerArray[j]= (float)atof(token);
//		token = strtok( NULL, seps );
//		j++;
//	}
 
	sprintf(cCmd, "%d;ABORt:NPOWer; *OPC? ", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
	return true;
}

bool  CFuncWLAN_CMU::WLANSetPortAndCableloss(int iInput,double iInputCableloss,int iOutput,double iOutputCableloss)
{
	char cCmd[100]="";
	ZeroMemory(cCmd, sizeof(cCmd)/sizeof(char));

	String szReadData;
	std::string strCommand;
	sprintf(cCmd, "%d;INP RF%d;*OPC?", m_iCMUMode, iInput);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;

	sprintf(cCmd, "%d;SOUR:CORR:LOSS:INPUT%d %8.3f;*OPC?", m_iCMUMode, iInput, iInputCableloss);
	strCommand = cCmd;	
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
	return true;
}
