/*********************************************************************************
 * COPYRIGHT Qisda WGC18 BB Team 2011
 *
 * Description: .h file of CFuncWLAN_CMU object.
 * FileName   : CFuncWLAN_CMU.h
 * Version    : $Revision:   1.0  $
 * Author     : Gilpin.Xi
 * Time       : 2011/03/23
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/03/23      Gilpin.Xi             Create.
 ********************************************************************************/
 
#ifndef CFUNWLAN_CMU_H_H
#define CFUNWLAN_CMU_H_H
 
#include "..\IFuncWLAN_Dev.h"
#include "..\..\..\CommonUtility\BaseObject\IGPIBDevice.h"
#include "CFuncDev_CMU.h"

class CFuncWLAN_CMU : public IFuncWLAN_Dev, public CFuncDev_CMU
{
private:

public:
	CFuncWLAN_CMU();
	CFuncWLAN_CMU(ITesterDevice* pDevice);
	virtual ~CFuncWLAN_CMU();
	bool InitWLANDev(int iAVFactor);
	
	bool WLANEvalPower(vector<float> & vtrf);
	bool WLANSetPortAndCableloss(int iInput,double iInputCableloss,int iOutput,double iOutputCableloss);
	bool WLANSetAnyserFreq(int iChannel);
	bool WLANFetchPower(String & fTestPowerArray);
};

#endif //CFUNWLAN_CMU_H_H