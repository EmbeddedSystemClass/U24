/*********************************************************************************
 * COPYRIGHT Qisda WGC18 BB Team 2011
 *
 * Description: .h file of IFuncBT_Dev object.
 * FileName   : IFuncBT_Dev.h
 * Version    : $Revision:   1.0  $
 * Author     : Gilpin.Xi
 * Time       : 2011/03/30
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/03/30      Gilpin.Xi             Create.
 ********************************************************************************/
 
#ifndef _BT_DEV_H
#define _BT_DEV_H
 
#include "..\..\CommonUtility\BaseObject\IFunc_Dev.h"
 
class IFuncBT_Dev : public virtual IFunc_Dev
{
 
public:
	IFuncBT_Dev(){}
	IFuncBT_Dev(ITesterDevice* pDevice):IFunc_Dev(pDevice){}
	virtual ~IFuncBT_Dev()
	{
 
	}
 
	virtual bool BTTestConfigureModulation(int iBurstCount, int iPacketType) = 0;
	virtual bool BTTestConfigurePwr(int iBurstCount) = 0;
	virtual bool BTTestDetach() = 0;
	virtual bool BTTestEvalModulation(String fTestModArray) = 0;
	virtual bool BTTestEvalPower(String& fTestPowerArray) = 0;
	virtual bool BTTestInput(int iInput) = 0;
	virtual bool BTTestModeConn() = 0;
	virtual bool BTTestOutput(int iOutput) = 0;
	virtual bool BTTestSetCabLoss(int inPort, float fDataIn, int outPort, float fDataOut) = 0;
	virtual bool BTTestSetModulationChannel(int iChannel) = 0;
	virtual bool BTTestSetPwrChannel(int iChannel) = 0;
	virtual bool CheckDeviceBT() = 0;
	virtual bool InitBTTest(double dMaxlevel) = 0;
	virtual bool BTTestSetFreq(float fFreq){return 0;}

};

typedef IFuncBT_Dev* PIFuncBT_Dev;

#endif //_WLAN_DEV_H
