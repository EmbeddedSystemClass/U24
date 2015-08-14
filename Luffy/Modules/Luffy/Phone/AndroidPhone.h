#ifndef _C_ANDROID_PHONE_H_
#define _C_ANDROID_PHONE_H_


#include "BaseObject/IPhone.h"
#include "Qisda_API.h"

#include <Shlwapi.h>

#include "Utils/CommonDefine.h"

#include "Utils/ComDeviceUtil.h"
#include "Utils/StringUtil.h"
#include "Utils/ExecCmdUtil.h"
#include "MonitorId.h"

#define FA_SIZE         512
#include "MonitorRuncard.h"
#include "MonitorSo.h"
#include "MonitorPartNo.h"
#include "MonitorWbcModel.h"
#include  <algorithm>
#include "FacTestToolInterface.h"

class CAndroidPhone : public IPhone
{
GP_DECLARE_DYNCREATE(CAndroidPhone)


// Data members
public:
	char m_szWorkingDir[512];
	std::string m_szIMEI;
	std::string m_szSN;
	std::string m_szWifiAddr;
	std::string m_szMACAddr;

private:
	HMODULE m_hQisdaModule;

	std::string m_strCOMport;
	std::string m_strKeyword;

	std::string m_szBbTestToolDir;          //记录BBTest tool的路径
	std::string m_szBbTestToolName;         //记录BBTest tool的名字
	std::string m_szAdbToolDir;             //记录ADB.exe的路径
	std::string m_szPADProductToolDir;      //记录量产工具的路径
	std::string m_szFlashToolName;          //记录烧录Tool名
	std::string m_szFactoryImageDir;        //记录Factory Image路径
	std::string m_szFactoryFwFile;          //fw文件名
	std::string m_szFactoryMiscFile;        //MISC文件名
	std::string m_szFactoryRecoveryFile;    //RECOVERY文件名
	std::string m_szFactorySystemFile ;     //SYSTEM文件名

	std::string m_szUserImageDir;           //记录User Image的路径
	std::string m_szUserFwFile;             //fw文件名
	std::string m_szUserMiscFile;           //MISC文件名
	std::string m_szUserRecoveryFile;       //RECOVERY文件名
	std::string m_szUserSystemFile ;        //SYSTEM文件名

	std::string m_szPMIC;					//PMIC self-test
	std::string m_szNAND;					//eMMCSizeTest
	std::string m_szDDR;					//DDRSizeTest
	std::string m_szHDMI;
	std::string m_szUart;					//Uart Loopback
	std::string m_szEthernet;				//Ethernet Loopback
	std::string m_szSDWriteProtected;
	std::string m_szSDPlugin;
	std::string m_szSDWriteRead;
	std::string m_szSDeMMC;
	std::string m_szSDClockRate;
	std::string m_szRecoverKey;
	double m_dNandSize; 
	double m_dDDRSize;
	std::string m_szNandSize;
	std::string m_szDDRSize;
	std::string m_szSDeMMCSize;
	std::string m_szSDType;
	std::string m_szSDProtected;

	//纽扣电池
	std::string m_szCoincellVolCmd;			//CoincellVol CMD
	int m_iCoincellVolSize;					//  Vol(mV) = m_szCoincellVolSize * 5.86(mV)

	//USB,读取Touch Panel的VID和PID
	std::string m_szIdProductCmd;    //IdProduct CMD
	std::string m_szIdVendorCmd;         //IdVendor CMD
	std::string m_szIdProductSize;
	std::string m_szIdVendorSize;
	
	//播放SD中的一段高清MP4视频，当然也可以指定其他路径
	std::string m_szAudioCmd;

	bool m_bNeedToGetFA;
	char m_szFaData[FA_SIZE];
	char m_szRuncard[30];
	static CMutex m_Mutex_Command;

// Member functions
public:
	CAndroidPhone();
	virtual ~CAndroidPhone();

	virtual bool InitData(pugi::xml_document *pXMLTestItem, pugi::xml_document *pXMLConfig);

	// Initialize functions
	virtual bool DetectDevice();
	virtual bool Initial();
	virtual void SetWorkingDir(const char* cszWorkingDir);
	virtual void SetComPort(std::string strComPort){m_strCOMport = strComPort;}
	virtual std::string GetComPort(){return m_strCOMport;}

	// FA functions
	virtual bool SetFAData(char* szValue);
	virtual bool GetSerialNumber(char* szValue, int iSize);
	virtual bool GetTimeStamp(char* szValue, int iSize);
	bool GetFAData();
    virtual bool SetIMEI(char* szvalue, int iSize);
	virtual bool SetSN(char* szvalue, int iSize);
	virtual bool GetAndReturnId(char* szvalue, int iSize);
	virtual bool LoadPhoneInfo(void);

	bool ReadWifiAddress(std::string szWorkingExe, std::string szWorkingDir, std::string *szWIFIMAC);
	bool WriteSn(std::string szWorkingExe, std::string szWorkingDir);
	bool ReadSn(std::string szWorkingExe, std::string szWorkingDir, std::string *szSN);
	bool SetWifiEnabled(bool bWifiOn);
	bool PlayMusic(void);
	bool pullRunInLog(std::string szLocalDir = "");
	bool pullCPULoadLog(char *chLocalPath);
	bool GetRunInResult(void);
	bool WriteMacAddress(std::string szWorkingExe, std::string szWorkingDir);
	bool SetPhoneMacAddress(std::string szMacAdd);
	std::string  GetRecoverKeyStatus(std::string szWorkingExe, std::string szWorkingDir);
	std::string  GetRecoverKeyStatus();
	bool PlayMovie(std::string szWorkingCmd);
	bool PlayMovie();
	bool LoadOs(std::string szFacImgName, std::string szFacImgDir, std::string szWorkingDir);
	bool EraseOs(std::string szFacImgName, std::string szFacImgDir, std::string szWorkingDir);
	bool LoadFactoryOs();
	bool LoadUserOs();
	bool EraseFactoryOs();
	bool EraseUserOs();
	bool BbTest(char* szCmdLine, char* szWorkDir, std::string *buf);
	std::string BbTest(bool enabledPMIC, bool enabledNAND, bool enabledDDRSize, bool enabledHDMI, bool enabledUart,
		        bool enabledEthernet, bool enabledSDWriteProtected, bool enabledSDPlugin, bool enabledSDWriteRead,
				bool enabledSDeMMCSize, bool enabledSDClockRate, bool enabledRecoverKey);
	std::string CheckIdProduct();
	std::string CheckIdVendor();
	std::string CheckCoincellVol();

	bool CheckLabelPartNo(void);
	bool IfRepeatedLabel(void);
	bool IfRepeatedId(void);
	// Qisda module phone functions
	virtual bool IsConnected();
	virtual bool Disconnect();
	virtual bool Reboot();
	virtual bool PushDownFileToSDCard(std::string szPcFilePath);

	bool ReadIMEI(char* szIMEI, int iSize);
	//bool ReadId(char* szId,int iIdLen);
	//bool GetRunCardById(unsigned char* szId,unsigned short IdLen,unsigned char* szRuncard,unsigned short RuncardLen);
	//bool GetSoInfo();
private:
	bool IfRepeated(unsigned short IdType);    // 1 = ID ,2 = SN
public:
	CMonitorId Id;
	CMonitorRuncard Runcard;
	CMonitorSo So;
	CMonitorPartNo PartNo;
	CMonitorWbcModel WbcFileName;
};


#endif // End of #ifndef _C_ANDROID_PHONE_H_
