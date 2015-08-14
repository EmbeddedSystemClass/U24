#ifndef _I_PHONE_H_
#define _I_PHONE_H_

#include "CToolBaseObject.h"
#include "Utils/Logger/CLogger.h"
#include "utils/xml/pugixml.hpp"

interface IPhone : public CToolBaseObject
{
GP_DECLARE_DYNAMIC(IPhone)


// Data members
public:
	int m_iSlot;

	std::string m_strDeviceName;
	std::string m_strQisdaCommandModuleName;
	std::string m_strWorkingDir;

	//Log
	//p_fn_log_callback m_plog_callback;
	CLogger* m_pLog;

protected:
private:


// Member functions
public:
	IPhone();
	virtual ~IPhone() {}

	void SetLog(CLogger* pLog);
	void CmdLog(std::string strMsg);
		
	virtual bool InitData(pugi::xml_document *pXMLTestItem, pugi::xml_document *pXMLConfig) = 0;

	// Initialize functions
	virtual bool DetectDevice() = 0;
	virtual bool Initial() = 0;
	virtual void SetWorkingDir(const char* cszWorkingDir) = 0;
	
	// FA functions
	virtual bool SetFAData(char* szValue) = 0;
	virtual bool GetSerialNumber(char* szValue, int iSize) = 0;
	virtual bool GetTimeStamp(char* szValue, int iSize) = 0;
	virtual bool SetIMEI(char* szvalue, int iSize) = 0;
	virtual bool SetSN(char* szvalue, int iSize) = 0;
	virtual bool GetAndReturnId(char* szvalue, int iSize) = 0;
	virtual bool LoadPhoneInfo(void) = 0;

	// Qisda module phone functions
	virtual bool IsConnected() = 0;
	virtual bool Disconnect() = 0;
	virtual bool Reboot() = 0;
	virtual bool PushDownFileToSDCard(std::string szPcFilePath) = 0;

	// Share info functions
	virtual void SetComPort(std::string strComPort) = 0;
	virtual std::string GetComPort() = 0;
};


#endif // End of #ifndef _I_PHONE_H_
