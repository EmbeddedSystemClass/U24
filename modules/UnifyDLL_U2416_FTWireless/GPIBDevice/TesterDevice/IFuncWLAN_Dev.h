/*********************************************************************************
 * COPYRIGHT Qisda WGC18 BB Team 2011
 *
 * Description: .h file of IFuncWLAN_Dev object.
 * FileName   : IFuncWLAN_Dev.h
 * Version    : $Revision:   1.0  $
 * Author     : Gilpin.Xi
 * Time       : 2011/03/23
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/03/23      Gilpin.Xi             Create.
 ********************************************************************************/
 
#ifndef _WLAN_DEV_H
#define _WLAN_DEV_H
 
#include "..\..\CommonUtility\BaseObject\IFunc_Dev.h"
 
class IFuncWLAN_Dev : public virtual IFunc_Dev
{
 
public:
	IFuncWLAN_Dev(){}
	IFuncWLAN_Dev(ITesterDevice* pDevice):IFunc_Dev(pDevice){}
	virtual ~IFuncWLAN_Dev()
	{
 
	}
 
	virtual bool InitWLANDev(int iAVFactor)=0;
	
	virtual bool WLANEvalPower(vector<float> & vtrf)=0;
	virtual bool WLANSetPortAndCableloss(int iInput,double iInputCableloss,int iOutput,double iOutputCableloss)=0;
	virtual bool WLANSetAnyserFreq(int iChannel)=0;
	virtual bool WLANFetchPower(String & fTestPowerArray)=0;

};

typedef IFuncWLAN_Dev* PIFuncWLAN_Dev;

#endif //_WLAN_DEV_H
