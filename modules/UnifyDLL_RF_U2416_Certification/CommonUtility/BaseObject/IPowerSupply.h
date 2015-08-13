#ifndef _I_POWER_SUPPLY_H_
#define _I_POWER_SUPPLY_H_


#include "IGPIBDevice.h"


interface IPowerSupply : public IGPIBDevice
{
RF_DECLARE_DYNAMIC(IPowerSupply)


// Data members
public:
protected:
private:


// Member functions
public:
	IPowerSupply() {}
	virtual ~IPowerSupply() {}

	virtual bool SetCurrentLimit(double dCurrent);
	virtual bool SetVoltage(double dVoltage);
	virtual bool SetPSOnOff(bool bIsOn);
	virtual bool SetCurrRangeMax();
	virtual bool SetCurrRangeMin();
	virtual bool GetCurrent(double &dCurrent);

protected:
private:
};


#endif // End of #ifndef _I_POWER_SUPPLY_H_
