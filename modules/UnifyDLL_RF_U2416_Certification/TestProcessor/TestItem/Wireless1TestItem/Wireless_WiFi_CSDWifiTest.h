#ifndef _C_WIRELESS_WIFI_CONTINOUS_RX_TEST_H_
#define _C_WIRELESS_WIFI_CONTINOUS_RX_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"
//for WIFIGUID
#include <windows.h>
#include <wlanapi.h>
#include <Windot11.h>           // for DOT11_SSID struct
#include <objbase.h>
#include <wtypes.h>
#include <SetupAPI.h>
#pragma comment(lib, "setupapi")

#include <stdio.h>
#include <stdlib.h>
#include <io.h>

// Need to link with Wlanapi.lib and Ole32.lib
#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "ole32.lib")

#define USB_BUF_SIZE 512
#define BUFFER_SIZE 4096
#define DREAD	                       "READ"
#define DNULL                        "NULL"
#define Postcmd					"Postcmd"
#define Wifi_RxTest	        "Wifi_RxTest"
typedef enum
{
	x,
	y,
	z,
	r
}enumWlanRateX;

typedef struct{
	int xRate;
	int yRate;
	int wMode;
	int DataRate;
}sWlanRate[2];

sWlanRate ab[] = { {0,0,0,0}, {0,0,0,0}};
//ab[1].xRate;
//ab[0].xRate = 1;
//sWlanRate[0].xRate = 10;

int arrWlanRate[20][4] = {
	{1000,2000,4,0},
	{2000,4000,4,1},
	{5500,11000,4,3},
	{11000,22000,4,5},
	{6000,12000,0,7},
	{9000,18000,0,8},
	{12000,24000,0,9},
	{18000,36000,0,10},
	{24000,48000,0,11},
	{36000,72000,0,12},
	{48000,96000,0,13},
	{54000,108000,0,14},
	{7200,14400,1,15},
	{14400,28800,1,16},
	{21700,43400,1,17},
	{28900,57800,1,18},
	{43300,86600,1,19},
	{57800,115600,1,20},
	{65000,130000,1,21},
	{72200,144400,1,22}
};
class CWireless_WiFi_CSDWifiTest : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CWireless_WiFi_CSDWifiTest)

// Data members
public:


protected:
private:
	std::string m_str_TestItem;
	int m_iCommandDelay;
	int m_iChannel;
	int m_iChain;
	int m_iWlandMode;
	int m_iRateMask;
	int nRxRate;
	int m_iCheckMin;
	int m_iCheckMax;
	int m_iResultDelay;
	int m_iMeasure;
	//int nChannel;

	std::string m_strMsg;
	std::string m_strMeasured;
	std::string m_strChannel;

    std::string m_strDeviceName;
	std::string m_strKeyword;
	std::string m_strCcomPort;

// Member functions
public:
	CWireless_WiFi_CSDWifiTest() {}
	~CWireless_WiFi_CSDWifiTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	

	HANDLE		m_h_Com;
	std::string m_str_comport;
	std::string ErrMsg;

	bool MainFunction();
	bool getWifiApInfo();
	bool runPostCmd( );

	bool DetectDevice();
	bool GetCOMPortDevByWDK(std::map<std::string, std::string>& map_strstrCOMDevice);	
	bool ExecFastbootOut(CString Command, char* output, char* ErrorCode);
	bool ExecAdbOut(CString Command, char* output, char* ErrorCode);
	bool bCallAdbFastbootCMD(CString csAdbFastboot, CString Command, char* output, char* ErrorCode, CString cs_FindData);
};


#endif // End of #ifndef _C_WIRELESS_WIFI_CONTINOUS_RX_TEST_H_
