/*********************************************************************************
 * COPYRIGHT Qisda WGC18 BB Team 2011
 *
 * Description: .h file of IFuncGPS_Dev object.
 * FileName   : IFuncGPS_Dev.h
 * Version    : $Revision:   1.0  $
 * Author     : Gilpin.Xi
 * Time       : 2011/03/31
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/03/31      Gilpin.Xi             Create.
 ********************************************************************************/
 
#ifndef __I_FUNC_GPS_DEV__
#define __I_FUNC_GPS_DEV__
 
#include "..\..\CommonUtility\BaseObject\IFunc_Dev.h"
 
class IFuncGPS_Dev : public virtual IFunc_Dev
{
 
public:
	IFuncGPS_Dev(){}
	IFuncGPS_Dev(ITesterDevice* pDevice):IFunc_Dev(pDevice){}
	virtual ~IFuncGPS_Dev(){}
 
	//GPS Test Function
	virtual bool GPSTestSetPowerLevel(double iLevel) = 0;
	virtual bool GPSTestSetChannel(double iChannel) = 0;
	virtual bool GPSTestSetPwrOn(void) = 0;
	virtual bool GPSTestSetPwrOff(void) = 0;

};

typedef IFuncGPS_Dev* PIFuncGPS_Dev;

#endif //__I_FUNC_GPS_DEV__
