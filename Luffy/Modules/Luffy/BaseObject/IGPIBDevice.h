#ifndef _I_GPIB_DEVICE_H_
#define _I_GPIB_DEVICE_H_

#include "CToolBaseObject.h"
#include "Utils/Logger/CLogger.h"

interface IGPIBDevice : public CToolBaseObject
{
GP_DECLARE_DYNAMIC(IGPIBDevice)


// Data members
public:
	int m_iSlot;

	static int g_iGPIBCardAddress;
	static int g_iGPIBInterfaceID;
	static std::vector<std::string> g_vDeviceName;
	static std::vector<int> g_vNI488Address;
	static std::vector<int> g_vPrimaryAddress;
	static std::vector<int> g_vSecondaryAddress;

	//Log
	CLogger *m_pLog;


protected:
	std::string m_strDeviceName;
	int m_iNI488Address;
	int m_iPAD;
	int m_iSAD;
	int m_iDD;

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

	bool InitData(std::string strDeviceName, int iNI488Address, int iPAD, int iSAD);

	void SetLog(CLogger* pLog);
	void CmdLog(std::string strMsg);

	bool IsAvaliable();

	bool GetDeviceName(std::string& strDeviceName);
	int GetNI488Address();
	int GetPrimaryAddress();
	int GetSecondaryAddress();

	bool GPIB_OPComplete(std::string strCommand);
	bool GPIB_Query(std::string strCommand, std::string& strResponse);
	bool GPIB_Query_BYTE(std::string strCommand, BYTE* pResponse, int& iSize);
	bool GPIB_Write(std::string strCommand);

protected:
	bool GPIB_Read(std::string& strResponse);

private:
};


#endif // End of #ifndef _I_GPIB_DEVICE_H_
