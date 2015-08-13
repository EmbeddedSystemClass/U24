/*********************************************************************************
 * COPYRIGHT Qisda WGC18 BB Team 2011
 *
 * Description: .h file of IFunc_Dev object.
 * FileName   : IFunc_Dev.h
 * Version    : $Revision:   1.0  $
 * Author     : Gilpin.Xi
 * Time       : 2011/03/23
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/03/23      Gilpin.Xi             Create.
 ********************************************************************************/

#ifndef __I_FUNC_DEV__
#define __I_FUNC_DEV__
 
#pragma once
 
#include "IGPIBDevice.h"
#include "ITesterDevice.h"

class IFunc_Dev
{
// Modified the access specifier of data member: m_pTesterDevice
// from protected to public
// so that class: CSet3GTestSetPortNLoss can use this data member freely
// as a reference to class: CTesterDevice
 
public:
	ITesterDevice* m_pTesterDevice;
 
protected:
	static int m_iCMUMode;
	static int m_iGPIBCardAddress;//GPIB Card Address for Tester Device
	static int m_h_Base;

public:
	IFunc_Dev() {}
	IFunc_Dev(ITesterDevice* pDevice)
		:m_pTesterDevice(pDevice)
	{
	}
 
	virtual ~IFunc_Dev() {
 
	}

	void SetCMUMode(int iCMUMode)
	{
		m_iCMUMode = iCMUMode;
	}
	int GetCMUMode()
	{
		return m_iCMUMode;
	}
 
	static bool DoGPIBSelect(int iGBIPCardAddress)
	{
		if (iGBIPCardAddress >= 0)
		{
			m_iGPIBCardAddress = iGBIPCardAddress;
			return true;
		}
		else
			return false;
	}
  
//Common Function For Devices
	virtual bool ExitDevice(void) = 0;
	virtual bool ClearDevice(void) = 0;
	virtual bool ResetDevice(void) = 0;
	virtual bool InitDeviceRFNSigOnly(int iDevicePad) = 0;
	virtual bool CheckDeviceRFNSig(void) = 0;
	virtual bool SetInput(int iInput) = 0;
	virtual bool SetOutput(int iOutput) = 0;
	virtual bool SetCableLoss(int iRfIn, float fRfInCableLoss, int iRfOut, float fRfOutCableLoss) = 0;
	virtual bool SetInputCableLoss(int iRfIn, float fRfInCableLoss) = 0;
	virtual bool SetOutputCableLoss(int iRfOut, float fRfInCableLoss) = 0;
	virtual bool SetRFGenBegin(){return 0;}
 
};
 
typedef IFunc_Dev* PIFunc_Dev;
 
#endif//__I_FUNC_DEV__
 
