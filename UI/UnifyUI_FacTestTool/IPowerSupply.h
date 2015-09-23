#ifndef _I_POWER_SUPPLY_H_
#define _I_POWER_SUPPLY_H_


//#include "IGPIBDevice.h"
#include "../../lib/NI488/inc/ni488.h"
#include <string>

interface IPowerSupply //: public IGPIBDevice
{

// Data members
public:
	int m_iGPIBCardAddress;
	int m_iGPIBInterfaceID;
	int m_iPowerSupplyDD;
protected:
	//int m_iPort;//for the same PowerSupply different port output
private:


// Member functions
public:
	IPowerSupply();
	virtual ~IPowerSupply() {}

	//bool InitDevice(int i_Address, int iPort = 1, int iNum=1);
	//virtual inline bool CheckInitialized(int iNum=1) { return (GetDeviceID_PS(iNum) != -1); }
	bool IsInit();
	bool InitPowerSupply( int iGPIBInterfaceID, int iGPIBCardAddress );
	bool GPIB_PS_Write(std::string strCommand);
	bool GPIB_PS_Querry(std::string strCommand, std::string& strResponse);

	virtual bool SetCurrentLimit(double dCurrent);
	virtual bool SetVoltage(double dVoltage);
	virtual bool SetPSOnOff(bool bIsOn);
	virtual bool SetCurrRangeMax();
	virtual bool SetCurrRangeMin();
	virtual bool GetCurrent(double &dCurrent);

protected:
	//virtual bool SetupDevice(int iCalDevicePad, int iPort, int &deviceID);

private:
};


#endif // End of #ifndef _I_POWER_SUPPLY_H_
