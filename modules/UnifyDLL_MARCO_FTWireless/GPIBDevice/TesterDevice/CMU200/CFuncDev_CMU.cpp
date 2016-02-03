/*********************************************************************************
 * COPYRIGHT Qisda WGC18 2011
 *
 * Description: .cpp file of CFuncDev_CMU object.
 * FileName   : CFuncDev_CMU.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Gilpin.Xi
 * Time       : 2011/03/23
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/03/23      Gilpin.Xi             Create.
 ********************************************************************************/

#include "stdafx.h"
#include "CFuncDev_CMU.h"
 
#pragma warning( disable : 4996 ) 
 
int CFuncDev_CMU::m_h_RF_NSig = -1;

CFuncDev_CMU::CFuncDev_CMU()
{

}
 
CFuncDev_CMU::CFuncDev_CMU(ITesterDevice* pDevice)
:IFunc_Dev(pDevice)
{
	m_pTesterDevice = pDevice;
}
 
CFuncDev_CMU::~CFuncDev_CMU()
{

}
 
bool CFuncDev_CMU::ClearDevice(void)
{
	return GPIB_Write(std::string("0;*CLS"));
}
 
bool CFuncDev_CMU::ExitDevice(void)
{
	if(m_h_Base != -1){
		ibclr(m_h_Base);
		ibonl(m_h_Base,0);
		m_h_Base = -1;
	}

	if(m_h_RF_NSig != -1)
	{
		ibclr(m_h_RF_NSig);
		ibonl(m_h_RF_NSig,0);
		m_h_RF_NSig = -1;
	}

	SetDeviceID(-1);

	return true;
}

bool CFuncDev_CMU::ResetDevice(void)
{
	if (GPIB_Write(std::string("0;*RST")) == false)
	{
		return false;
	}
	if (GPIB_Write(std::string("0;*CLS")) == false)
	{
		return false;
	}

	return true;
}
   
bool CFuncDev_CMU::SetInput(int iInput)
{
	char cCmd[100]="";
	ZeroMemory(cCmd, sizeof(cCmd));
 
	sprintf(cCmd, "%d;INPut RF%d;", m_iCMUMode, iInput);
	std::string strCommand(cCmd);
	if (GPIB_Write(strCommand) == false)
	{
		return false;
	}

	Sleep(50);

	return true;
}
 
bool CFuncDev_CMU::SetOutput(int iOutput)
{
	char cCmd[100]="";
	ZeroMemory(cCmd, sizeof(cCmd));
 
	sprintf(cCmd, "%d;OUTP RF%d;", m_iCMUMode, iOutput);
	std::string strCommand(cCmd);
	if (GPIB_Write(strCommand) == false)
	{
		return false;
	}

	Sleep(50);

	return true;
 
}
 
bool CFuncDev_CMU::SetCableLoss(int iRfIn, float fRfInCableLoss, int iRfOut, float fRfOutCableLoss)
{
	char cCmd[100]="";
	ZeroMemory(cCmd, sizeof(cCmd));
 	std::string strCommand;

	sprintf(cCmd, "0;CONFigure:USER:CORRection:LOSS:TABLe:ENABle OFF");
	strCommand = cCmd;
	if (GPIB_Write(strCommand) == false)
		return false;
 
 
	sprintf(cCmd, "%d;SOUR:CORR:LOSS:INPUT%d %8.3f;*OPC?", m_iCMUMode, iRfIn, fRfInCableLoss);
	strCommand = cCmd;
	if (GPIB_Write(strCommand) == false)
	{
		return false;
	}
 
	sprintf(cCmd, "%d;SOUR:CORR:LOSS:OUTPUT%d %8.3f;*OPC?", m_iCMUMode, iRfOut, fRfOutCableLoss);
	strCommand = cCmd;
	if (GPIB_Write(strCommand) == false)
	{
		return false;
	}

	return true;
}
 
bool CFuncDev_CMU::SetInputCableLoss(int iRfIn, float fRfInCableLoss)
{
 
	char cCmd[100]="";
	ZeroMemory(cCmd, sizeof(cCmd));
  	std::string strCommand;

	sprintf(cCmd, "0;CONFigure:USER:CORRection:LOSS:TABLe:ENABle OFF");
	strCommand = cCmd;
	if (GPIB_Write(strCommand) == false)
		return false;
 
	sprintf(cCmd, "%d;SOUR:CORR:LOSS:INPUT%d %8.3f;*OPC?", m_iCMUMode, iRfIn, fRfInCableLoss);
	strCommand = cCmd;
	if (GPIB_Write(strCommand) == false)
	{
		return false;
	}

	return true;
}
 
bool CFuncDev_CMU::SetOutputCableLoss(int iRfOut, float fRfOutCableLoss)
{
	char cCmd[100]="";
	ZeroMemory(cCmd, sizeof(cCmd));
	std::string strCommand;
 
	sprintf(cCmd, "0;CONFigure:USER:CORRection:LOSS:TABLe:ENABle OFF");
	strCommand = cCmd;
	if (GPIB_Write(strCommand) == false)
		return false;
 
	sprintf(cCmd, "%d;SOUR:CORR:LOSS:OUTPUT%d %8.3f;*OPC?", m_iCMUMode, iRfOut, fRfOutCableLoss);
	strCommand = cCmd;
	if (GPIB_Write(strCommand) == false)
	{
		return false;
	}

	return true;
}

bool CFuncDev_CMU::InitDeviceRFNSigOnly(int iDevicePad)
{
	char cCmd[100] = "";
	ZeroMemory(cCmd,sizeof(cCmd));
	String strRes;
 	std::string strCommand;

	if (m_h_Base == -1)
	{
		m_h_Base = ibdev(m_iGPIBCardAddress, iDevicePad,SAD_BASE,TMO,EOT,EOS);
	}
	SetDeviceID(m_h_Base);  //set device ID of IFunc_Dev::m_cGPIBInterface
 
	sprintf(cCmd, "0;*CLS;*OPC?");
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, strRes) == false)
		return false;

	if(GPIB_Write(std::string("SYST:NONV:DIS")) == false)
		return false;

	if(GPIB_Write(std::string("TRAC:REM:MODE:DISP ON")) == false)
		return false;

	if (m_pTesterDevice->m_bInternalReference)
	{
		if(GPIB_Write(std::string("CONF:SYNC:FREQ:REF:MODE INT")) == false)
			return false;
	} 
	//Add remote debug mode
	if(GPIB_Write(std::string("SYSTem:REMote:RDMode ON")) == false)
		return false;

	/* RF_NSIG */
	sprintf(cCmd, MAP_COMMAND, _RF_NSIG, IDSTR_RF_NSIG);
	strCommand = cCmd;
	if (GPIB_Write(strCommand) == false)
		return false;

	if (m_h_RF_NSig == -1)
	{
		m_h_RF_NSig = ibdev(m_iGPIBCardAddress,iDevicePad,SAD_RF_NSIG,TMO,EOT,EOS);
	}
 
	SetDeviceID(m_h_RF_NSig);  //set device ID of IFunc_Dev::m_cGPIBInterface
	m_iCMUMode = _RF_NSIG;
 
	Sleep(50);

	if (GetDeviceID() != -1)
	{
		ibclr(GetDeviceID());
		if (ibsta & ERR)
		{
			/*m_nErrCode = ERR_GPIB_WRITE;*/
		}
	}

	return true;
 
}

bool CFuncDev_CMU::CheckDeviceRFNSig(void)
{
	if (m_h_RF_NSig == -1)
	{
		return false;
	}

	SetDeviceID(m_h_RF_NSig);

	return true;
/*
	string strReadAnsi;
	String strRead;
	char cCmd[100]="";
	ZeroMemory(cCmd, sizeof(cCmd));
 
	sprintf(cCmd, "0;SYST:REM:ADDR:SEC? %d",_RF_NSIG);
	if ((Gpib_Query(cCmd)) != SUCCESS){
		LogTrace(MSG_MID, _T("Gpib_Write Fail"))
			return false;
	}else{
		strRead = GetReadBufferStr();
#ifdef _UNICODE
		strReadAnsi = CStr::ToAnsiString(strRead);
#else
		strReadAnsi = strRead;
#endif
	}
	if (strReadAnsi == "RF_NSig")
	{
		return true;
	}else{
		String strMsg = _T("Secondary is ") + strRead +_T(", not RF_NSig");
		LogTrace(MSG_MID, strMsg.c_str())
			return false;
	}
*/
}

bool CFuncDev_CMU::SetRFGenBegin()
{
	char cCmd[100]="";
	ZeroMemory(cCmd, sizeof(cCmd));
 
	sprintf(cCmd, "%d;INIT:RFG;*OPC?", m_iCMUMode);
	std::string strCommand(cCmd);
	if (GPIB_Write(strCommand) == false)
	{
		return false;
	}
	return true;
}