#ifndef _C_COMMON_PROCESSOR_H_
#define _C_COMMON_PROCESSOR_H_

#include "BaseObject/ITestProcessor.h"
#include "BaseObject/ITestItem.h"
#include "BaseObject/IPowerSupply.h"
#include "BaseObject/ITestBox.h"
#include "BaseObject/IPhone.h"
#include "ErrorDefine/Common_ErrorDefine.h"
#include "Utils/CommonDefine.h"
#include "Utils/StringUtil.h"
#include "Utils/xml/pugixml.hpp"
#include "Utils/ComDeviceUtil.h"
#include "Utils/Logger/CLogger.h"
#include "FacTestToolInterface.h"

class CGPProcessor : public ITestProcessor
{
// Data members
public:

	//pugi
	pugi::xml_document *m_pXMLTestItem;
	pugi::xml_document *m_pXMLConfig;
	pugi::xml_document *m_pXMLCableloss;

	// parameter, no used
	std::map<std::string, std::map<std::string, std::string>> m_mapTestItemParams;

	// All device
	std::map<std::string, std::string> m_mapTesterDeviceParams ;
	std::string m_strTestBoxClassName;

	// Power supply
	int m_iPSCount;
	std::vector<int> m_vPowerSupplyAddr;	
	std::map<std::string, std::string> m_mapPowerSupplyParams;
	IPowerSupply* m_pIPS;
	IPowerSupply* m_pIPS2;
	IPowerSupply* m_pIPS3;
	IPowerSupply* m_pIPS4;

	// Phone
	std::map<int, IPhone*> m_mapIPhone;

	//There are too many kinds of instrument, use base object to create the instance
	bool m_bUsingGPIB;
	std::map<std::string, ITestBox*> m_mapITestBox;
	int m_iGPIBInterfaceID;
	int m_iGPIBCardAddress;
	bool m_bEraseOs;
	char m_szFailMsg[256];
	st_UIMessage m_st_UIMessage;

protected:
private:
	bool m_bStopped;
	ITestBox*		m_pITestBox;
	IPhone*			m_pIPhone_Base;

// Member functions
public:
	CGPProcessor();
	virtual ~CGPProcessor() {}

	/* Set/Get FA Data */
	virtual bool SetFAData(char* sz_value);
	virtual bool GetSerialNumber(int i_slot, char* sz_value, int i_size);
	virtual bool GetTimeStamp(int i_slot, char* sz_value, int i_size);
	virtual bool CreateAndInitPhone(int i_slot);

	/* Test sequence */
	virtual bool OnAPPLaunch();
	virtual bool PreRun(int i_slot);
	virtual bool DetectAndCreatePhone();
	virtual bool RunTestingProcedureByName(const char *szItem , int i_slot);
	virtual bool RunInitialSettingByName(const char *szItem, int i_slot);
	virtual bool PostRun(int i_slot);
	virtual bool Stop();
	virtual bool OnAPPEnd();
	virtual bool AutoDetectDevice();

	virtual bool ReadId(char* sz_value, int i_size);
	virtual bool LoadPhoneInfo(void);

	// Initialize functions
	//bool SetCableLoss(const char *szType, const char *szBand, const char *szChannel, const char *szLoss);
	bool DetectGPIBDevice();
	bool SetIMEI(char* szvalue, int iSize);
	bool SetSN(char* szvalue, int iSize);
	void SetEraseFlag(bool bFlag);
	void SendMsgToUi(const char *szMsg);
	void SetFailMsg(const char *szMsg);
	// XML
	bool SetXML(const char* sz_keyword, pugi::xml_document* pXML) ;

	// Process cable loss
	double GetCableLoss(const std::string &strRfType, const std::string &strRfBand, std::string &strRfRange);
	
	// Get test box by name which is class name
	ITestBox* GetTestBoxByName(std::string strTestBoxClassName);

	// Get DUT(phone) by name
	IPhone* GetPhoneByName(string name="");

	//Set Events from Controller
	void SetEvent(const char* szEvent, long nParam);

private:
	bool CreateTestBox(std::vector<std::string> vGPIBDeviceList, int iSlot = 1);
	bool CreatePS(std::vector<std::string> vGPIBDeviceList);
	bool CreatePhone(int iSlot = 1);
	bool CreateMyPhone(int iSlot = 1);

};


#endif // End of #ifndef _C_COMMON_PROCESSOR_H_
