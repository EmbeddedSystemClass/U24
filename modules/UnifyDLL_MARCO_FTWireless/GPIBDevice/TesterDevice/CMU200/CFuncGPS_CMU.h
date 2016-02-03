/*********************************************************************************
 * COPYRIGHT Qisda WGC18 BB Team 2011
 *
 * Description: .h file of CFuncGPS_CMU object.
 * FileName   : CFuncGPS_CMU.h
 * Version    : $Revision:   1.0  $
 * Author     : Gilpin.Xi
 * Time       : 2011/03/31
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/03/31     Gilpin.Xi             Create.
 ********************************************************************************/

#ifndef __FUNC_GPS_CMU__
#define __FUNC_GPS_CMU__

#pragma warning( disable : 4250 )

#include "..\IFuncGPS_Dev.h"
#include "..\..\..\CommonUtility\BaseObject\IGPIBDevice.h"
#include "CFuncDev_CMU.h"

class CFuncGPS_CMU : public IFuncGPS_Dev, public CFuncDev_CMU
{
private:

public:
	CFuncGPS_CMU();
	CFuncGPS_CMU(ITesterDevice* pDevice);
	virtual ~CFuncGPS_CMU();

	//CMU200 GPS Test Function
	bool GPSTestSetPowerLevel(double iLevel);
	bool GPSTestSetChannel(double iChannel);
	bool GPSTestSetPwrOn(void);
	bool GPSTestSetPwrOff(void);
};

#endif //__FUNC_GPS_CMU__