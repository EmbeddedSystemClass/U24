#ifndef _I_GPIB_DEVICE_H_
#define _I_GPIB_DEVICE_H_

#include "../CommonUtil.h"//110323 gilpin add
#include "../../../../lib/NI488/inc/ni488.h"
#include "../CommonDefine.h"
#include "CToolBaseObject.h"
#include "../../GPIBDevice/TesterDevice/CMU200/CMU_Defines.h"//110324 gilpin add

interface IGPIBDevice : public CToolBaseObject
{
RF_DECLARE_DYNAMIC(IGPIBDevice)


// Data members
public:
	static int g_iGPIBCardAddress;
	static int g_iGPIBInterfaceID;
	static std::vector<std::string> g_vDeviceName;
	static std::vector<int> g_vNI488Address;
	static std::vector<int> g_vPrimaryAddress;
	static std::vector<int> g_vSecondaryAddress;

	static std::string m_strDeviceName;
	static int m_iNI488Address;
	static int m_iPAD;
	static int m_iSAD;
	static int m_iDD;

	int m_iPowerSupplyPAD;
	int m_iPowerSupplySAD;
	int m_iPowerSupplyDD;

	int m_iDeviceID;
	int m_iDeviceID_PS;

protected:

private:


// Member functions
public:
	static bool DetectGPIBDevice(int iGPIBInterfaceID, int iGPIBCardAddress);

	static bool GetDeviceList(std::vector<std::string>& vDeviceList);
	static int GetNI488Address(int iIndex);
	static int GetPrimaryAddress(int iIndex);
	static int GetSecondaryAddress(int iIndex);

	IGPIBDevice();
	virtual ~IGPIBDevice() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap) { return true; }

	bool InitTestDeviceData(std::string strDeviceName, int iNI488Address, int iPAD, int iSAD);
	bool InitPowerSupplyData(std::string strDeviceName, int iNI488Address, int iPAD, int iSAD);

	bool TestDeviceIsAvaliable();
	bool PowerSupplyIsAvaliable();

	bool GPIB_OPComplete(std::string strCommand);
	bool GPIB_Querry(std::string strCommand, std::string& strResponse);
	bool GPIB_Write(std::string strCommand);

	bool GPIB_PS_Write(std::string strCommand);
	bool GPIB_PS_Querry(std::string strCommand, std::string& strResponse);

	//CMU200
	inline int GetDeviceID(){	return m_iDeviceID;  }
	inline void SetDeviceID(int iDeviceID){	m_iDeviceID =  iDeviceID;}
	//PowerSupply
	inline int GetDeviceID_PS(int iNum=1){	return m_iDeviceID_PS;  }
	inline void SetDeviceID_PS(int iDeviceID, int iNum=1){	m_iDeviceID_PS =  iDeviceID;}

protected:
	bool GPIB_Read(std::string& strResponse);

private:

};


#endif // End of #ifndef _I_GPIB_DEVICE_H_
