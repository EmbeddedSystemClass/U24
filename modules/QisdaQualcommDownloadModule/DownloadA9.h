#ifndef _DOWNLOAD_A9_H_
#define _DOWNLOAD_A9_H_


#include <string>
#include <vector>
#include "Download.h"
#include "ADBDevice.h"

#pragma warning(push)
#pragma warning(disable : 4251)


using namespace QSYNC_LIB;


class QISDA_DOWNLOAD_CLASS_API CDownloadA9 : public CRefObject, public CNotifyBase_ANSI
{
// Data member
public:
	static CMutex m_Mutex_Command;

protected:
private:
	int m_iSlot;
	int m_iCOMPort;
	std::string m_strADBSerial;
	std::string m_USB_COM_Interface_Name;
	std::string m_USB_ADB_Interface_Name;
	std::string m_USB_Fastboot_Interface_Name;
	std::string m_USB_Root_Name;
	ADBDevice* m_pADB;
	ADBDevice* m_pFastboot;
	std::string findPID;
	

	
	std::vector<std::pair<std::string, std::string>> m_vec_DLFile; // vector<fileType, fileName>
	std::string m_strExeCmdPath;
	std::string m_strDLPath;

// Member function
public:
	CDownloadA9(int iSlot, int iCOMPort);
	virtual ~CDownloadA9(void) {}

	void SetDownloadPath(const char* szDLPath);
	void SetDownloadFile(const char* szFileName, const char* szFileType);

	bool GetADBSerial();
	bool GetADBSerial_New();
	bool SetToFastbootMode();
	bool SetToFastbootModeACB();
	bool FastbootDL();
	bool FastbootDL_New();
	bool FastbootReset();
	bool FastbootReset_New();

	bool SendAdbCmd(const char* chCmd, char *chReturn = NULL, long dwReturnLen = 0, int timeout = 30000);
    bool SendAcbCmd(const char* chCmd, char *chReturn = NULL, long dwReturnLen = 0, int timeout = 30000);
	bool SendFastbootCmd(const char* chCmd, char *chReturn = NULL, long dwReturnLen = 0, int timeout = 30000);
	bool GetRTCTime(char* szYear);
	bool GetVIDPID(char* PID);
   
	// CNotifyBase_ANSI
	virtual bool Register(INotify_ANSI* pNotify,const std::string& strEvent);
	virtual bool Register(EventFunc_ANSI* pEventFunc,const std::string& strEvent);
	virtual bool Unregister(INotify_ANSI* pNotify,const std::string& strEvent);
	virtual bool Unregister(EventFunc_ANSI* pEventFunc,const std::string& strEvent);
	virtual void DefineNotify(const std::string& strNewEvent);
	virtual void Fire(const std::string& strEvent,long nParam);
    
protected:
private:
	bool FindQualcomDevice(std::vector<USBDEVICE_STRUCT> &inUSBdevice);
	bool FindADBdevice(std::vector<USBDEVICE_STRUCT> &inUSBdevice);
	bool GetSerialNumberFromADB(const char* chUsbName, std::string *strBuffer );
	int GetComportFromUSB(USBDEVICE_STRUCT& dev);
	bool MyExecAdbCmd(const char* chExeName, const char* chCmd, char *chReturn = NULL, long dwReturnLen = 0, int timeout = INFINITE);

	//Fastboot DL 
	bool CDownloadA9::ADBFlashAll(const char* sz_parmeter, const char* sz_fileName);
	

	// UI function
	void UpdateProgress(const char* chMessage, const int nStep = 0, const int nProgress = 0);
	void AddMessage(const char* chMessage, ...);
};

typedef CRefObjectPtr<CDownloadA9> CDownloadA9Ptr;


#pragma warning(pop)


#endif //End of #ifndef _DOWNLOAD_A9_H_