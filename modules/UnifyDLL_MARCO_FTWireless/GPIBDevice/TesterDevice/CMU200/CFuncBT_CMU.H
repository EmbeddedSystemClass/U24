/*********************************************************************************
 * COPYRIGHT Qisda WGC18 BB Team 2011
 *
 * Description: .h file of CFuncBT_CMU object.
 * FileName   : CFuncBT_CMU.h
 * Version    : $Revision:   1.0  $
 * Author     : Gilpin.Xi
 * Time       : 2011/03/30
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/03/30      Gilpin.Xi             Create.
 ********************************************************************************/

#ifndef CFUNBT_CMU_H_H
#define CFUNBT_CMU_H_H
 
#include "..\IFuncBT_Dev.h"
#include "..\..\..\CommonUtility\BaseObject\IGPIBDevice.h"
#include "CFuncDev_CMU.h"

class CFuncBT_CMU : public IFuncBT_Dev, public CFuncDev_CMU
{
private:

public:
	CFuncBT_CMU();
	CFuncBT_CMU(ITesterDevice* pDevice);
	virtual ~CFuncBT_CMU();

	//CFuncBT_CMU BT Test Function
	bool CheckDeviceBT();
	bool InitBTTest(double dMaxlevel);
	bool BTTestSetCabLoss(int inPort, float fDataIn, int outPort,  float fDataOut);
	bool BTTestInput(int iInput);
	bool BTTestOutput(int iOutput);
	bool BTTestModeConn();
	bool BTTestConfigurePwr(int iBurstCount);
	bool BTTestEvalPower(String& fTestPowerArray);
	bool BTTestSetPwrChannel(int iChannel);
	bool BTTestDetach();
	bool BTTestConfigureModulation(int iBurstCount, int iPacketType);
	bool BTTestSetModulationChannel(int iChannel);
	bool BTTestEvalModulation(String fTestModArray);
	bool BTTestSetFreq(float fFreq);
};

#endif //CFUNWLAN_CMU_H_H