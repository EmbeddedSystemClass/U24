/*********************************************************************************
 * COPYRIGHT Qisda WGC18 BB Team 2011
 *
 * Description: .h file of CFuncDev_CMU object.
 * FileName   : CFuncDev_CMU.h
 * Version    : $Revision:   1.0  $
 * Author     : Gilpin.Xi
 * Time       : 2011/03/23
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/03/23      Gilpin.Xi             Create.
 ********************************************************************************/

#ifndef __FUNC_DEV_CMU_H__
#define __FUNC_DEV_CMU_H__
 
#pragma warning( disable : 4250 )

#include "..\..\..\CommonUtility\BaseObject\IFunc_Dev.h"
#include "..\..\..\CommonUtility\BaseObject\IGPIBDevice.h"

class  CFuncDev_CMU : public virtual IFunc_Dev, public IGPIBDevice
{
public:
 
	CFuncDev_CMU();
	CFuncDev_CMU(ITesterDevice* pDevice);
	virtual ~CFuncDev_CMU();
 
	virtual bool ClearDevice(void);
	virtual bool ExitDevice(void);
	virtual bool ResetDevice(void);
	virtual bool InitDeviceRFNSigOnly(int iDevicePad);
	virtual bool CheckDeviceRFNSig(void);
	virtual bool SetInput(int iInput);
	virtual bool SetOutput(int iOutput);
	virtual bool SetCableLoss(int iRfIn, float fRfInCableLoss, int iRfOut, float fRfOutCableLoss);
	virtual bool SetInputCableLoss(int iRfIn, float fRfInCableLoss);
	virtual bool SetOutputCableLoss(int iRfOut, float fRfOutCableLoss);
	virtual bool SetRFGenBegin();
 
protected:
	char *m_chCMUModeStr;
	static int m_h_RF_NSig;
 
};

typedef CFuncDev_CMU* PCFuncDev_CMU;

#endif//__FUNC_DEV_CMU_H__
