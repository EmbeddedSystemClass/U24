#ifndef _C_WIRELESS_WIFI_CONTINOUS_RX_TEST_H_
#define _C_WIRELESS_WIFI_CONTINOUS_RX_TEST_H_


#include "../../../CommonUtility/BaseObject/ITestProcessor.h"
//for WIFIGUID
#include <windows.h>
#include <wlanapi.h>
#include <Windot11.h>           // for DOT11_SSID struct
#include <objbase.h>
#include <wtypes.h>

//#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>


// Need to link with Wlanapi.lib and Ole32.lib
#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "ole32.lib")

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
	int m_iCommandDelay;
	//int m_iStartDelay;
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


// Member functions
public:
	CWireless_WiFi_CSDWifiTest() {}
	~CWireless_WiFi_CSDWifiTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool MainFunction();
	bool getWifiApInfo();
};


#endif // End of #ifndef _C_WIRELESS_WIFI_CONTINOUS_RX_TEST_H_
