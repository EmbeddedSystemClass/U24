/*********************************************************************************
 * COPYRIGHT Qisda WGC18 2011
 *
 * Description: .cpp file of CFuncBT_CMU object.
 * FileName   : CFuncBT_CMU.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Gilpin.Xi
 * Time       : 2011/03/30
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/03/30      Gilpin.Xi             Create.
 ********************************************************************************/

#include "stdafx.h"
#include "CFuncBT_CMU.h"
 
CFuncBT_CMU::CFuncBT_CMU()
{
 
}
 
CFuncBT_CMU::CFuncBT_CMU(ITesterDevice* pDevice)
	:IFuncBT_Dev(pDevice)
{
	m_pTesterDevice = pDevice;
}
 
CFuncBT_CMU::~CFuncBT_CMU()
{

}

bool CFuncBT_CMU::CheckDeviceBT(void)
{
	char cCmd[100]="";
	ZeroMemory(cCmd, sizeof(cCmd)/sizeof(char));
	String szReadData;

	sprintf(cCmd, "0;SYST:REM:ADDR:SEC? %d",_CMUBT_SIG);
	std::string strCommand(cCmd);

	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
	
	return true;
}
 
bool CFuncBT_CMU::InitBTTest(double dMaxlevel)
{
	char cCmd[100]={0};
	//	ZeroMemory(cCmd, sizeof(cCmd)/sizeof(char));
	//ZeroMemory(cCmd, sizeof(cCmd));
 
 	std::string strCommand;
	String szReadData;

	//sprintf(cCmd, "%d;LEVel:MAXimum 20;*OPC?", m_iCMUMode);
	//if ((m_nErrCode = Gpib_Query(cCmd)) != SUCCESS)
	//	return m_nErrCode;
	//sprintf(cCmd, "%d;CONFigure:MSIGnal:PAGing:TARGet \"9ABC56781234\";*OPC?", m_iCMUMode);
	////sprintf(cCmd, "%d;CONFigure:MSIGnal:PAGing:TARGet \"681400000000\";*OPC?", m_iCMUMode);
	//if ((m_nErrCode = Gpib_Query(cCmd)) != SUCCESS)
	//	return m_nErrCode;
	sprintf(cCmd, "%d;ABOR:RFAN;*OPC?", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
	sprintf(cCmd, "%d;TRIG:SOUR IMM;*OPC?", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
	sprintf(cCmd, "%d;TRIG:SLOP POS;*OPC?", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
	sprintf(cCmd, "%d;RFAN:FREQ 2450.0MHz;*OPC?", m_iCMUMode);//110503 gilpin modify 2441.0MHz->2450.0MHz for 8260 platform
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
	sprintf(cCmd, "%d;NPOW:BWID 1000kHz;*OPC?", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
	sprintf(cCmd, "%d;LEV:MODE MAN;*OPC?", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
	sprintf(cCmd, "%d;LEV:MAX %.2f;*OPC?", m_iCMUMode,dMaxlevel);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
	return true;
 
}
 
bool CFuncBT_CMU::BTTestInput(int iInput)
{
	char cCmd[100]="";
	ZeroMemory(cCmd, sizeof(cCmd)/sizeof(char));
	String szReadData;

	sprintf(cCmd, "%d;INPut RF%d;*OPC?", m_iCMUMode, iInput);
	std::string strCommand(cCmd);
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
	return true;
 
}
 
bool CFuncBT_CMU::BTTestOutput(int iOutput)
{
	char cCmd[100]="";
	ZeroMemory(cCmd, sizeof(cCmd)/sizeof(char));
 	String szReadData;

	sprintf(cCmd, "%d;OUTP RF%d;*OPC?", m_iCMUMode, iOutput);
	std::string strCommand(cCmd);
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
	return true;
 
}
 
bool CFuncBT_CMU::BTTestSetCabLoss(int inPort, float fDataIn, int outPort,  float fDataOut)
{
	char cCmd[100]="";
	ZeroMemory(cCmd, sizeof(cCmd)/sizeof(char));
 	String szReadData;

	sprintf(cCmd, "%d;SOUR:CORR:LOSS:INPUT%d %8.3f;*OPC?", m_iCMUMode, inPort, fDataIn);
	std::string strCommand(cCmd);
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;

 /*
	sprintf(cCmd, "%d;SOUR:CORR:LOSS:OUTPUT%d %8.3f;*OPC?", m_iCMUMode, outPort, fDataOut);
	if ((m_nErrCode = Gpib_Query(cCmd)) != SUCCESS)
		return m_nErrCode;*/
 
	return true;
}
 
bool CFuncBT_CMU::BTTestModeConn(void)
{
	char cCmd[100]="";
	ZeroMemory(cCmd, sizeof(cCmd));
 
	string strRead = "NONE";
  	std::string strCommand;
 	String szReadData;

	while(strRead.substr(0, 3) != "SBY" && strRead.substr(0, 4) != "CONN")
	{
		sprintf(cCmd, "%d;SIGNalling:STATe?", m_iCMUMode);
		strCommand = cCmd;
		if (GPIB_Querry(strCommand, szReadData) == false)
			return false;

#ifdef _UNICODE
		strRead = CStr::ToAnsiString(szReadData);
#else
		strRead = szReadData;
#endif
 
		Sleep(100);
	}
 
	if (strRead.substr(0, 4) != "CONN")
	{
		sprintf(cCmd, "%d;PROC:SIGN:ACT TEST; *OPC?", m_iCMUMode);
		strCommand = cCmd;
		if (GPIB_Querry(strCommand, szReadData) == false)
			return false;
	}
 
	return true;
}
 
bool CFuncBT_CMU::BTTestConfigurePwr(int iBurstCount)
{
	char cCmd[100]="";
	ZeroMemory(cCmd, sizeof(cCmd));
 
 	String szReadData;

	sprintf(cCmd, "%d;CONF:NPOW:CONT:STATistics %d; *OPC?", m_iCMUMode, iBurstCount);
	std::string strCommand(cCmd);
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
	/*sprintf(cCmd, "%d;CONFigure:POWer:TIME:MMODe SINGle; *OPC?", m_iCMUMode);
	if ((m_nErrCode = Gpib_Query(cCmd)) != SUCCESS)
		return m_nErrCode;
 
	sprintf(cCmd, "%d;CONFigure:POWer:TIME:MFRequency:UNIT CH; *OPC?", m_iCMUMode);
	if ((m_nErrCode = Gpib_Query(cCmd)) != SUCCESS)
		return m_nErrCode;*/
 
	return true;
 
}
 
bool CFuncBT_CMU::BTTestSetPwrChannel(int iChannel)
{
	char cCmd[100]="";
	ZeroMemory(cCmd, sizeof(cCmd));

 	String szReadData;
 
	sprintf(cCmd, "%d;CONFigure:POWer:TIME:MFRequency %d; *OPC?", m_iCMUMode, iChannel);
	std::string strCommand(cCmd);
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
	return true;
 
}
 
bool CFuncBT_CMU::BTTestEvalPower(String& fTestPowerArray)
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
		if (GPIB_Querry(strCommand, szReadData) == false)
			return false;

#ifdef _UNICODE
		strRead = CStr::ToAnsiString(szReadData);
#else
		strRead = szReadData;
#endif
		Sleep(100);
	}
 
	sprintf(cCmd, "%d;FETC:NPOW?", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
#ifdef _UNICODE
	strRead = CStr::ToAnsiString(szReadData);
#else
	strRead = szReadData;
#endif
 
	char seps[] = ",";
	char *token = NULL;
	int j = 0;
	token = strtok( (char *)strRead.c_str(), seps );		//Avg. Power of Current evaluation period
	token = strtok( NULL, seps );	//Min. Power of Current evaluation period
	token = strtok( NULL, seps );	//Max. Power of Current evaluation period
	token = strtok( NULL, seps );	//Avg. Power ref. to the last stat. cycle
	if(strstr(token,_T("NAN")))
		token = "-99.0";
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
bool CFuncBT_CMU::BTTestConfigureModulation(int iBurstCount, int iPacketType)
{
	char cCmd[100]="";
	ZeroMemory(cCmd, sizeof(cCmd));
 
  	std::string strCommand;
 	String szReadData;

	sprintf(cCmd, "%d;CONFigure:MODulation:DEViation:CONTrol:STATistics %d; *OPC?", m_iCMUMode, iBurstCount);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
	sprintf(cCmd, "%d;CONFigure:SSIGnal:TMODe:TMTYpe LBT; *OPC?", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
	sprintf(cCmd, "%d;CONFigure:SSIGnal:TMODe:LBTests:PTYPe DH%d; *OPC?", m_iCMUMode, iPacketType);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
	sprintf(cCmd, "%d;CONFigure:MODulation:DEViation:MMODe SINGle; *OPC?", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
	sprintf(cCmd, "%d;CONFigure:MODulation:DEViation:MFRequency:UNIT CH; *OPC?", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
	return true;
}
bool CFuncBT_CMU::BTTestSetModulationChannel(int iChannel)
{
	char cCmd[100]="";
	ZeroMemory(cCmd, sizeof(cCmd));
 
 	String szReadData;

	sprintf(cCmd, "%d;CONFigure:MODulation:DEViation:MFRequency %d; *OPC?", m_iCMUMode, iChannel);
	std::string strCommand(cCmd);
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
	return true;
}
bool CFuncBT_CMU::BTTestEvalModulation(String fTestModArray)
{
	char cCmd[100]="";
	ZeroMemory(cCmd, sizeof(cCmd));

  	std::string strCommand;
 	String szReadData;

	sprintf(cCmd, "%d;INITiate:MODulation:DEViation; *OPC?", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
	Sleep(2000);

	string strRead = "RUN";
	while(strRead.substr(0, 3) != "RDY")
	{
		sprintf(cCmd, "%d;FETCh:MODulation:DEViation:STATus?", m_iCMUMode);
		strCommand = cCmd;
		if (GPIB_Querry(strCommand, szReadData) == false)
			return false;

#ifdef _UNICODE
		strRead = CStr::ToAnsiString(szReadData);
#else
		strRead = szReadData;
#endif
		Sleep(300);
	}
 
	sprintf(cCmd, "%d;FETch:MODulation:DEViation?", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, fTestModArray) == false)
		return false;
 
//String szReadData = GetReadBufferStr();
//#ifdef _UNICODE
//	strRead = CStr::ToAnsiString(szReadData);
//#else
//	strRead = szReadData;
//#endif
//	char seps[] = ",";
//	char *token = NULL;
//	int j = 0;
//	token = strtok( strRead, seps );
//	while( token != NULL ){
//		fTestModArray[j]= (float)atof(token);
//		token = strtok( NULL, seps );
//		j++;
//	}
 
	sprintf(cCmd, "%d;ABORt:MODulation:DEViation; *OPC? ", m_iCMUMode);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
	return true;
}
 
 
bool CFuncBT_CMU::BTTestDetach()
{
	char cCmd[100]="";
	ZeroMemory(cCmd, sizeof(cCmd));

 	String szReadData;

	sprintf(cCmd, "%d;PROC:SIGN:ACT DETach; *OPC?", m_iCMUMode);
	std::string strCommand(cCmd);
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;
 
	return true;
 
}

bool CFuncBT_CMU::BTTestSetFreq(float fFreq)
{
	char cCmd[100]="";
	ZeroMemory(cCmd, sizeof(cCmd));

 	String szReadData;

	sprintf(cCmd, "%d;RFAN:FREQ %6.1fMHz; *OPC?", m_iCMUMode, fFreq);
	std::string strCommand(cCmd);
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;

	return true;

}
