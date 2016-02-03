/*********************************************************************************
 * COPYRIGHT Qisda WGC18 2011
 *
 * Description: .h file of Keithley2306 PowerSupply object.
 * FileName   : CKeithley2306.h
 * Version    : $Revision:   1.0  $
 * Author     : Gilpin.Xi
 * Time       : 2011/03/23
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/03/23      Gilpin.Xi             Create.
 ********************************************************************************/

#ifndef	_KEITHLEY_2306_H
#define	_KEITHLEY_2306_H

#include "../../CommonUtility/BaseObject/IPowerSupply.h"
 
class CKeithley2306 : public IPowerSupply
{
	RF_DECLARE_DYNCREATE(CKeithley2306)
// Data members
public:
protected:
private:


// Member functions
public:
	CKeithley2306(void);
	virtual ~CKeithley2306(void);
	virtual bool GetCurrent(double &dCurrent);
	virtual bool SetPSOnOff(bool bIsOn);

protected:
private: 
};

#endif//_KEITHLEY_2304_H
