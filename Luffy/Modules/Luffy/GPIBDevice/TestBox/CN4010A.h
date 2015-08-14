#ifndef _C_CN4010A_H_
#define _C_CN4010A_H_


#include "BaseObject/IVisaDevice.h"
#include "AgilentVISA/inc/visa.h"
#include "AgilentVISA/lib/AgN4010BT4API.h"
#import "AgilentVISA/lib/WLanTest/Agilent_N4010_WLanTest.tlb" //__uuidof(Instrument)

using namespace Agilent_N4010_WLanTest;



class CN4010A : public IVISADevice
{
GP_DECLARE_DYNCREATE(CN4010A)


// Data members
public:
	char m_szErrorMsg[1024];
	Instrument_InterfacePtr m_pInstrument;

protected:
	ViSession defaultRM, vi;
	char      szVISAname[256];
	std::string m_strWorkDir;

private:
	static bool bWriteCmdLog;
	std::string m_strOption;

	int  WaitCount;
	int  WaitInterval;
	int  InquiryCount;
	int  InquiryInterval;


// Member functions
public:
	CN4010A();
	virtual ~CN4010A() {}
	//virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool InitData(pugi::xml_document *pXMLTestItem, std::string strNickName);
	
	virtual bool GetDeviceOption (std::string& strOption);
	virtual bool Reset();
	virtual bool WriteCmd (const char* fmt, ...);
	virtual bool ReadCmd (char* szResult, const char* fmt, ...);
	virtual double ReadCmd (const char* fmt, ...);


	// Setup
	bool SetupTestSet (const char* freqSrc, const char* pwClass, const char* psrm, const char* pin, const char* mode);
	void ClearUpTestSet ();
	bool InquiryEUT   (const char* phoneBTAddr = "");
	bool SetCableLoss (const double dLoss);

	// Event Handler callback
	//static ViStatus _VI_FUNCH my_handler (ViSession vi, ViEventType eventType, ViEvent context, ViAddr usrHandle);


	//bool RunTest (const char* testTag);
	bool WaitTest (const char* testTag, const int index=1);
	int  IsPass (const char* fmt, ...);
	

	bool DetectVISADevice (std::string& strDeviceName);
	bool GetDeviceName(std::string& strDeviceName);
	bool Lock();
	void Unlock();
	void ReleaseVISADevice();
	bool SendHWLatchSCPIcmd();
	bool StartDeviceWifi ();
	bool StopDeviceWifi ();

	void SetVISAname(std::string strName) { strcpy_s(szVISAname, 256, strName.c_str()); }
	void SetWriteCmdLog(bool bEnable) { bWriteCmdLog = bEnable; }
	void SetWaitCount(int iCount) { WaitCount = iCount; }
	void SetWaitInterval(int iInterval) { WaitInterval = iInterval; }
	void SetInquiryCount(int iCount) { InquiryCount = iCount; }
	void SetInquiryInterval(int iInterval) { InquiryInterval = iInterval; }

	bool GetWriteCmdLog() { return bWriteCmdLog; }

protected:
private:
	static char logFileName[256];
	void InitDebug (const char* chFileName);
	static void Dbg (const char* dir, const char* chPrint);



	bool InitRegisterSetting();
	bool HasSystemError();

};


#endif // End of #ifndef _C_N4010A_H_
