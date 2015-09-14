#include "stdafx.h"
#include "IPowerSupply.h"


RF_IMPLEMENT_DYNAMIC(IPowerSupply)

bool IPowerSupply::InitDevice(int i_Address, int iPort, int iNum)
{
	int iDeviceID = -1;
	int iCalDevicePad = m_iPowerSupplyPAD;	//Primary Address
	//int iCalDevicePad = i_Address;	//Primary Address

	bool bStatus = SetupDevice(iCalDevicePad, iPort, iDeviceID);

	SetDeviceID_PS(iDeviceID, iNum);
	return bStatus;
}

bool IPowerSupply::SetupDevice(int iCalDevicePad, int iPort, int &iDeviceID)
{
	bool bStatus = true;

	iDeviceID = ibdev(g_iGPIBCardAddress,iCalDevicePad,0,T10s,1,0);
	CString szTemp;
	szTemp.Format(_T("Init DeviceID %d"), iDeviceID);
	
	if (iDeviceID == -1)
		bStatus = false;
	else 
	{
		ibclr(iDeviceID);
		if (ibsta & ERR)
		{
			ibonl(iDeviceID, 0);
			bStatus = false;
		}
	}

	m_iPort = iPort;
	return bStatus;
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

//virtual bool SetCapacitance(int nFlag);//0 = low, 1 = high
bool  IPowerSupply::SetCapacitance(int nFlag)//lion
{
	if (nFlag == 0 ){
		return GPIB_PS_Write(std::string("OUTP:TYPE LOW"));
	}
	else if(nFlag == 1 ){
		return GPIB_PS_Write(std::string("OUTP:TYPE HIGH"));
	}
	
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
