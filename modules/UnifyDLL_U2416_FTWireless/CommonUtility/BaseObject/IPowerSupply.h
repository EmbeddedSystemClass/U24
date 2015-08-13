#ifndef _I_POWER_SUPPLY_H_
#define _I_POWER_SUPPLY_H_


#include "IGPIBDevice.h"


interface IPowerSupply : public IGPIBDevice
{
RF_DECLARE_DYNAMIC(IPowerSupply)


// Data members
public:
protected:
	int m_iPort;//for the same PowerSupply different port output
private:


// Member functions
public:
	IPowerSupply() {}
	virtual ~IPowerSupply() {}

	bool InitDevice(int i_Address, int iPort = 1, int iNum=1);
	virtual inline bool CheckInitialized(int iNum=1) { return (GetDeviceID_PS(iNum) != -1); }

	virtual bool SetCurrentLimit(double dCurrent);
	virtual bool SetVoltage(double dVoltage);
	virtual bool SetPSOnOff(bool bIsOn);
	virtual bool SetCurrRangeMax();
	virtual bool SetCurrRangeMin();
	virtual bool GetCurrent(double &dCurrent);

protected:
	virtual bool SetupDevice(int iCalDevicePad, int iPort, int &deviceID);

private:
};


#endif // End of #ifndef _I_POWER_SUPPLY_H_
