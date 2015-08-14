#ifndef _I_POWER_SUPPLY_H_
#define _I_POWER_SUPPLY_H_


#include "IGPIBDevice.h"
#include "Utils/xml/pugixml.hpp"

interface IPowerSupply : public IGPIBDevice
{
GP_DECLARE_DYNAMIC(IPowerSupply)


// Data members
public:
protected:
private:
	double	m_dCurrentLimit;
	double	m_dStepVoltage;
	double	m_dFinalVoltage;
	int		m_iDelay;

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
    virtual bool QueryVoltageSetting(double &dVoltage) ;
    virtual bool QueryCurrentSetting(double &dCurrent) ;
	virtual bool LoadPowerSupplySetting(pugi::xml_document *pXMLTestItem);
	virtual bool SetPowerSupplyOn();
	virtual bool SetPowerSupplyOff();

protected:
private:
};


#endif // End of #ifndef _I_POWER_SUPPLY_H_
