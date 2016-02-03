/*********************************************************************************
 * COPYRIGHT Qisda WGC18 2011
 *
 * Description: .h file of Keithley2304 PowerSupply object.
 * FileName   : CKeithley2304.h
 * Version    : $Revision:   1.0  $
 * Author     : Gilpin.Xi
 * Time       : 2011/03/23
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/03/23      Gilpin.Xi             Create.
 ********************************************************************************/

#ifndef	_KEITHLEY_2304_H
#define	_KEITHLEY_2304_H

#include "../../CommonUtility/BaseObject/IPowerSupply.h"
 
class CKeithley2304 : public IPowerSupply
{
	RF_DECLARE_DYNCREATE(CKeithley2304)
// Data members
public:
protected:
private:


// Member functions
public:
	CKeithley2304(void);
	virtual ~CKeithley2304(void);
	virtual bool SetCurrentLimit(double dCurrent);

protected:
private: 
};

#endif//_KEITHLEY_2304_H
